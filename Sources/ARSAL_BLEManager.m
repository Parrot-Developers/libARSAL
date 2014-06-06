/**
 * @file ARSAL_BLEManager.m
 * @brief BLE network manager allow to send over ble network.
 * @brief wifi network manager allow to send over wifi network.
 * @date 06/11/2013
 * @author frederic.dhaeyer@parrot.com
 */

/*****************************************
 *
 *             inport file :
 *
 *****************************************/
#include "libARSAL/ARSAL_BLEManager.h"
#include "libARSAL/ARSAL_Error.h"

#define ARSAL_BLEMANAGER_ENABLE_DEBUG (0)
#define ARSAL_BLEMANAGER_CONNECTION_TIMEOUT_SEC (5)

#pragma mark CBUUID (String Extraction extension)
@implementation CBUUID (StringExtraction)
- (NSString *)representativeString;
{
    NSData *data = [self data];

    NSUInteger bytesToConvert = [data length];
    const unsigned char *uuidBytes = [data bytes];
    NSMutableString *outputString = [NSMutableString stringWithCapacity:16];

    for (NSUInteger currentByteIndex = 0; currentByteIndex < bytesToConvert; currentByteIndex++)
    {
        switch (currentByteIndex)
        {
        case 3:
        case 5:
        case 7:
        case 9:[outputString appendFormat:@"%02x-", uuidBytes[currentByteIndex]]; break;
        default:[outputString appendFormat:@"%02x", uuidBytes[currentByteIndex]];
        }

    }

    return outputString;
}
@end


@implementation ARSALBLEManagerNotificationData

@synthesize characteristic = _characteristic;
@synthesize value = _value;

- (id)initWithValue:(CBCharacteristic*)characteristic value:(NSData*)value
{
    self = [super init];
    if (self != nil)
    {
        _characteristic = characteristic;
        _value = value;
    }
    return self;
}

@end

@implementation ARSALBLEManagerNotification

@synthesize characteristics = _characteristics;
@synthesize notificationsArray = _notificationsArray;

- (id)init
{
    self = [super init];
    if (self != nil)
    {
        ARSAL_Sem_Init(&readCharacteristicsSem, 0, 0);
        ARSAL_Mutex_Init(&readCharacteristicMutex);
        _characteristics = nil;
        _notificationsArray = [NSMutableArray array];
    }
    return self;
}

- (void)dealloc
{
    ARSAL_Sem_Destroy(&readCharacteristicsSem);
    ARSAL_Mutex_Destroy(&readCharacteristicMutex);
}

- (void)addNotification:(ARSALBLEManagerNotificationData*)notificationData
{
    ARSAL_Mutex_Lock(&readCharacteristicMutex);
    
    [_notificationsArray addObject:notificationData];
    
    ARSAL_Mutex_Unlock(&readCharacteristicMutex);
    
    ARSAL_Sem_Post(&readCharacteristicsSem);
}

- (int)getAllNotifications:(NSMutableArray*)notificationsArray maxCount:(int)maxCount
{
    ARSAL_Mutex_Lock(&readCharacteristicMutex);
    
    NSMutableArray *removeNotifications = [NSMutableArray array];
    for (int i=0; (i < maxCount) && (i < [_notificationsArray count]); i++)
    {
        NSObject *obj = _notificationsArray[i];
        [notificationsArray addObject:obj];
        [removeNotifications addObject:obj];
    }
    [_notificationsArray removeObjectsInArray:removeNotifications];
    [removeNotifications removeAllObjects];
    
    ARSAL_Mutex_Unlock(&readCharacteristicMutex);
    return (int)[notificationsArray count];
}

- (BOOL)waitNotification
{
    int ret = ARSAL_Sem_Wait(&readCharacteristicsSem);
    return (ret == 0) ? YES : NO;
}

- (BOOL)signalNotification
{
    int ret = ARSAL_Sem_Post(&readCharacteristicsSem);
    return (ret == 0) ? YES : NO;
}
@end

#pragma mark ARSAL_BLEManager implementation
@interface ARSAL_BLEManager ()

@property (nonatomic, assign) eARSAL_ERROR discoverServicesError;
@property (nonatomic, assign) eARSAL_ERROR discoverCharacteristicsError;
@property (nonatomic, assign) eARSAL_ERROR configurationCharacteristicError;

@property (nonatomic, assign) BOOL askDisconnection;
@property (nonatomic, assign) BOOL isDiscoveringServices;
@property (nonatomic, assign) BOOL isDiscoveringCharacteristics;
@property (nonatomic, assign) BOOL isConfiguringCharacteristics;
@property (nonatomic, strong) NSMutableDictionary *registeredNotificationCharacteristics;

- (void)ARSAL_BLEManager_Init;
@end

@implementation ARSAL_BLEManager
@synthesize discoverServicesError = _discoverServicesError;
@synthesize discoverCharacteristicsError = _discoverCharacteristicsError;
@synthesize configurationCharacteristicError = _configurationCharacteristicError;
@synthesize activePeripheral = _activePeripheral;
@synthesize characteristicsNotifications = _characteristicsNotifications;
@synthesize delegate = _delegate;

SYNTHESIZE_SINGLETON_FOR_CLASS(ARSAL_BLEManager, ARSAL_BLEManager_Init);

// Initializing function to synhtesize singleton
- (void)ARSAL_BLEManager_Init
{
    _activePeripheral = nil;
    _discoverServicesError = ARSAL_OK;
    _discoverCharacteristicsError = ARSAL_OK;
    _characteristicsNotifications = [NSMutableArray array];
    _askDisconnection = NO;
    _isDiscoveringServices = NO;
    _isDiscoveringCharacteristics = NO;
    _isConfiguringCharacteristics = NO;
    _registeredNotificationCharacteristics = [NSMutableDictionary dictionary];
    
    ARSAL_Sem_Init(&connectionSem, 0, 0);
    ARSAL_Sem_Init(&disconnectionSem, 0, 0);
    ARSAL_Sem_Init(&discoverServicesSem, 0, 0);
    ARSAL_Sem_Init(&discoverCharacteristicsSem, 0, 0);
    ARSAL_Sem_Init(&configurationSem, 0, 0);
}

- (void)dealloc
{
    ARSAL_Sem_Destroy(&connectionSem);
    ARSAL_Sem_Destroy(&disconnectionSem);
    ARSAL_Sem_Destroy(&discoverServicesSem);
    ARSAL_Sem_Destroy(&discoverCharacteristicsSem);
    ARSAL_Sem_Destroy(&configurationSem);
}

- (BOOL)isPeripheralConnected
{
    BOOL ret = NO;
    @synchronized (self)
    {
        if (self.activePeripheral != nil)
        {
            ret = YES;
        }
    }
    
    return ret;
}

- (eARSAL_ERROR)discoverNetworkServices:(NSArray *)servicesUUIDs
{
    eARSAL_ERROR result = ARSAL_OK;
    
    @synchronized (self)
    {
        // If there is an active peripheral, disconnecting it
        if(_activePeripheral != nil)
        {
            _isDiscoveringServices = YES;
            
            _discoverServicesError = ARSAL_OK;
            [self.activePeripheral discoverServices:nil];
            ARSAL_Sem_Wait(&discoverServicesSem);
            result = self.discoverServicesError;
            _discoverServicesError = ARSAL_OK;
            
            _isDiscoveringServices = NO;
        }
        else
        {
            result = ARSAL_ERROR_BLE_NOT_CONNECTED;
        }
    }
    
    return result;
}

- (eARSAL_ERROR)setNotificationCharacteristic:(CBCharacteristic *)characteristic
{
    eARSAL_ERROR result = ARSAL_OK;
    @synchronized (self)
    {
        // If there is an active peripheral, disconnecting it
        if(_activePeripheral != nil)
        {
            _isConfiguringCharacteristics = YES;
            
            _configurationCharacteristicError = ARSAL_OK;
            [self.activePeripheral setNotifyValue:YES forCharacteristic:characteristic];
            ARSAL_Sem_Wait(&configurationSem);
            result = self.configurationCharacteristicError;
            _configurationCharacteristicError = ARSAL_OK;
            
            _isConfiguringCharacteristics = NO;
        }
        else
        {
            result = ARSAL_ERROR_BLE_NOT_CONNECTED;
        }
    }
    
    return result;
}

- (eARSAL_ERROR)discoverNetworkCharacteristics:(NSArray *)characteristicsUUIDs forService:(CBService *)service
{
    eARSAL_ERROR result = ARSAL_OK;
    @synchronized (self)
    {
        // If there is an active peripheral, disconnecting it
        if(self.activePeripheral != nil)
        {
            _isDiscoveringCharacteristics = YES;
            
            NSLog(@"Service : %@", [service.UUID representativeString]);
            _discoverCharacteristicsError = ARSAL_OK;
            [_activePeripheral discoverCharacteristics:nil forService:service];
            ARSAL_Sem_Wait(&discoverCharacteristicsSem);
            result = _discoverCharacteristicsError;
            _discoverCharacteristicsError = ARSAL_OK;
            
            _isDiscoveringCharacteristics = NO;
        }
        else
        {
            result = ARSAL_ERROR_BLE_NOT_CONNECTED;
        }
    }
    
    return result;
}

- (eARSAL_ERROR)connectToPeripheral:(CBPeripheral *)peripheral withCentralManager:(ARSAL_CentralManager *)centralManager
{
    eARSAL_ERROR result = ARSAL_OK;
    const struct timespec connectionTimeout = {
        .tv_sec = ARSAL_BLEMANAGER_CONNECTION_TIMEOUT_SEC,
        .tv_nsec = 0,
    };
    
    @synchronized (self)
    {
        
        [centralManager addDelegate:self];

        // If there is an active peripheral, disconnecting it
        if(_activePeripheral != nil)
        {
            [self disconnectPeripheral:_activePeripheral withCentralManager:centralManager];
        }
        
        // Connection to the new peripheral
        [centralManager connectPeripheral:peripheral options:[NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithBool:YES], CBConnectPeripheralOptionNotifyOnDisconnectionKey, nil]];
        
        if(ARSAL_Sem_Timedwait(&connectionSem, &connectionTimeout) != 0)
        {
            /* disconnect timeout */
            [centralManager cancelPeripheralConnection:peripheral];
        }
        
        if (_activePeripheral != nil)
        {
            _activePeripheral.delegate = self;
        }
        else
        {
            /* Connection failed */
            result = ARSAL_ERROR_BLE_CONNECTION;
        }
        
    }
    
    return result;
}

- (void)disconnectPeripheral:(CBPeripheral *)peripheral withCentralManager:(ARSAL_CentralManager *)centralManager
{
    @synchronized (self)
    {
        if (self.activePeripheral != nil)
        {
            _askDisconnection = YES;
            
            [centralManager cancelPeripheralConnection:_activePeripheral];
            
            ARSAL_Sem_Wait(&disconnectionSem);

            [centralManager removeDelegate:self];
            
            _askDisconnection = NO;
        }
    }
}

- (BOOL)writeData:(NSData *)data toCharacteristic:(CBCharacteristic *)characteristic
{
    BOOL result = NO;
    if((_activePeripheral != nil) && (characteristic != nil) && (data != nil))
    {
        [_activePeripheral writeValue:data forCharacteristic:characteristic type:CBCharacteristicWriteWithoutResponse];
        result = YES;
    }

    return result;
}

- (void)registerNotificationCharacteristics:(NSArray *)characteristicsArray toKey:(NSString*)readCharacteristicsKey
{
#if ARSAL_BLEMANAGER_ENABLE_DEBUG
    NSLog(@"%s:%d", __FUNCTION__, __LINE__);
#endif
    
    ARSALBLEManagerNotification *notification = [[ARSALBLEManagerNotification alloc] init];
    [notification setCharacteristics:characteristicsArray];
    
    [_registeredNotificationCharacteristics setObject:notification forKey:readCharacteristicsKey];
}

- (BOOL)readNotificationData:(NSMutableArray *)notificationArray maxCount:(int)maxCount toKey:(NSString *)readCharacteristicsKey
{
    BOOL result = NO;
#if ARSAL_BLEMANAGER_ENABLE_DEBUG
    NSLog(@"%s:%d", __FUNCTION__, __LINE__);
#endif
    
    ARSALBLEManagerNotification *notification = [_registeredNotificationCharacteristics objectForKey:readCharacteristicsKey];
    if (notification != nil)
    {
        [notification waitNotification];
        
        if ([notification.notificationsArray count] > 0)
        {
            [notification getAllNotifications:notificationArray maxCount:maxCount];
            result = YES;
        }
    }
    
    return result;
}

- (void)readData:(CBCharacteristic *)characteristic
{
#if ARSAL_BLEMANAGER_ENABLE_DEBUG
    NSLog(@"%s:%d", __FUNCTION__, __LINE__);
#endif
    
    [_activePeripheral readValueForCharacteristic:characteristic];
}

#pragma mark CBCentralManagerDelegate
- (void)centralManagerDidUpdateState:(CBCentralManager *)central
{
#if ARSAL_BLEMANAGER_ENABLE_DEBUG
    NSLog(@"%s:%d", __FUNCTION__, __LINE__);
#endif
    
    switch(central.state)
    {
    case CBCentralManagerStatePoweredOn:
        NSLog(@"CBCentralManagerStatePoweredOn");
        break;

    case CBCentralManagerStateResetting:
        NSLog(@"CBCentralManagerStateResetting");
        break;

    case CBCentralManagerStateUnsupported:
        NSLog(@"CBCentralManagerStateUnsupported");
        break;

    case CBCentralManagerStateUnauthorized:
        NSLog(@"CBCentralManagerStateUnauthorized");
        break;

    case CBCentralManagerStatePoweredOff:
        NSLog(@"CBCentralManagerStatePoweredOff");
        break;

    default:
    case CBCentralManagerStateUnknown:
        NSLog(@"CBCentralManagerStateUnknown");
        break;
    }
}

- (void)centralManager:(CBCentralManager *)central didConnectPeripheral:(CBPeripheral *)peripheral
{
#if ARSAL_BLEMANAGER_ENABLE_DEBUG
    NSLog(@"%s:%d -> %@", __FUNCTION__, __LINE__, peripheral);
#endif
    
    self.activePeripheral = peripheral;
    
    ARSAL_Sem_Post(&connectionSem);
}

- (void) centralManager:(CBCentralManager *)central didFailToConnectPeripheral:(CBPeripheral *)peripheral error:(NSError *) error
{
#if ARSAL_BLEMANAGER_ENABLE_DEBUG
    NSLog(@"%s:%d : %@", __FUNCTION__, __LINE__, peripheral);
#endif

    ARSAL_Sem_Post(&connectionSem);
}

- (void)centralManager:(CBCentralManager *)central didDisconnectPeripheral:(CBPeripheral *)peripheral error:(NSError *)error
{
#if ARSAL_BLEMANAGER_ENABLE_DEBUG
    NSLog(@"%s:%d : %@", __FUNCTION__, __LINE__, peripheral);
#endif
    
    if((_activePeripheral != nil) && (_activePeripheral == peripheral))
    {
        _activePeripheral.delegate = nil;
        _activePeripheral = nil;
        
        /* Post disconnectionSem only if the disconnect is asked */
        if(_askDisconnection)
        {
            ARSAL_Sem_Post(&disconnectionSem);
        }
        
        /* if activePeripheral is discovering services */
        if(_isDiscoveringServices)
        {
            _discoverServicesError = ARSAL_ERROR_BLE_NOT_CONNECTED;
            ARSAL_Sem_Post(&discoverServicesSem);
        }
        
        /* if activePeripheral is discovering Characteristics */
        if(_isDiscoveringCharacteristics)
        {
            _discoverCharacteristicsError = ARSAL_ERROR_BLE_NOT_CONNECTED;
            ARSAL_Sem_Post(&discoverCharacteristicsSem);
        }
        
        /* if activePeripheral is configuring Characteristics */
        if(_isConfiguringCharacteristics)
        {
            _configurationCharacteristicError = ARSAL_ERROR_BLE_NOT_CONNECTED;
            ARSAL_Sem_Post(&configurationSem);
        }
        
        /* Notify delegate */
        if(!_askDisconnection)
        {
            if ((_delegate != nil) && ([_delegate respondsToSelector:@selector(onBLEDisconnect)]))
            {
                [_delegate onBLEDisconnect];
            }
        }
    }
}

#pragma mark CBPeripheralDelegate
- (void)peripheral:(CBPeripheral *)peripheral didDiscoverServices:(NSError *)error
{
#if ARSAL_BLEMANAGER_ENABLE_DEBUG
    NSLog(@"%s:%d - %@", __FUNCTION__, __LINE__, peripheral);
#endif
    
    if (error != nil)
    {
        _discoverServicesError = ARSAL_ERROR_BLE_SERVICES_DISCOVERING;
    }
    
    ARSAL_Sem_Post(&discoverServicesSem);
}

- (void)peripheral:(CBPeripheral *)peripheral didDiscoverCharacteristicsForService:(CBService *)service error:(NSError *)error
{
#if ARSAL_BLEMANAGER_ENABLE_DEBUG
    NSLog(@"%s:%d - %@", __FUNCTION__, __LINE__, peripheral);
#endif
    
    if (error != nil)
    {
        _discoverCharacteristicsError = ARSAL_ERROR_BLE_CHARACTERISTICS_DISCOVERING;
    }
    
    ARSAL_Sem_Post(&discoverCharacteristicsSem);
}

- (void)peripheral:(CBPeripheral *)peripheral didWriteValueForCharacteristic:(CBCharacteristic *)characteristic error:(NSError *)error
{
#if ARSAL_BLEMANAGER_ENABLE_DEBUG
    NSLog(@"%s:%d - %@ : %@", __FUNCTION__, __LINE__, peripheral, [error localizedDescription]);
#endif
}

- (void)peripheral:(CBPeripheral *)peripheral didUpdateValueForCharacteristic:(CBCharacteristic *)characteristic error:(NSError *)error
{
#if ARSAL_BLEMANAGER_ENABLE_DEBUG
    NSLog(@"%s:%d - %@ : %d, %@", __FUNCTION__, __LINE__, [characteristic.UUID representativeString], [characteristic.value length], [error localizedDescription]);
#endif
    
    ARSALBLEManagerNotification *foundNotification = nil;
    for (NSString* key in [_registeredNotificationCharacteristics allKeys])
    {
        ARSALBLEManagerNotification *notification = [_registeredNotificationCharacteristics objectForKey:key];
        
        for (CBCharacteristic *characteristicItem in [notification characteristics])
        {
            if ([characteristicItem.UUID.UUIDString isEqualToString:characteristic.UUID.UUIDString])
            {
                foundNotification = notification;
                break;
            }
        }
        
        if (foundNotification != nil)
        {
            break;
        }
    }
    
    if (foundNotification != nil)
    {
        ARSALBLEManagerNotificationData *notificationData = [[ARSALBLEManagerNotificationData alloc] initWithValue:characteristic value:[characteristic value]];
        [foundNotification addNotification:notificationData];
    }
}

- (void)peripheral:(CBPeripheral *)peripheral didUpdateNotificationStateForCharacteristic:(CBCharacteristic *)characteristic error:(NSError *)error
{
#if ARSAL_BLEMANAGER_ENABLE_DEBUG
    NSLog(@"%s:%d - %@ : %@", __FUNCTION__, __LINE__, [characteristic.UUID representativeString], [error localizedDescription]);
#endif
    
    if (error != nil)
    {
        _configurationCharacteristicError = ARSAL_ERROR_BLE_CHARACTERISTIC_CONFIGURING;
    }
    
    ARSAL_Sem_Post(&configurationSem);
}

- (void)peripheralDidUpdateName:(CBPeripheral *)peripheral
{
#if ARSAL_BLEMANAGER_ENABLE_DEBUG
    NSLog(@"%s:%d -> %@", __FUNCTION__, __LINE__, peripheral);
#endif
}

- (void)unlock
{
    @synchronized (self)
    {
        /* post all Semaphore to unlock the all the functions */
    #if ARSAL_BLEMANAGER_ENABLE_DEBUG
        NSLog(@"%s:%d", __FUNCTION__, __LINE__);
    #endif
        
        ARSAL_Sem_Post(&connectionSem);
        ARSAL_Sem_Post(&discoverServicesSem);
        ARSAL_Sem_Post(&discoverCharacteristicsSem);
        ARSAL_Sem_Post(&configurationSem);
        
        for (ARSALBLEManagerNotification *notification in [_registeredNotificationCharacteristics allValues])
        {
            [notification signalNotification];
        }
        
        /* disconnectionSem is not post because:
         * if the connection is fail, disconnect is not call.
         * if the connection is successful, the BLE callback is always called.
         * the disconnect function is called after the join of the network threads.
         */
    }
}

- (void)reset
{
    @synchronized (self)
    {
        /* reset all Semaphores */
#if ARSAL_BLEMANAGER_ENABLE_DEBUG
        NSLog(@"%s:%d", __FUNCTION__, __LINE__);
    #endif
        
        while (ARSAL_Sem_Trywait(&connectionSem) == 0)
        {
            /* Do nothing*/
        }
        
        while (ARSAL_Sem_Trywait(&discoverServicesSem) == 0)
        {
            /* Do nothing*/
        }
        
        while (ARSAL_Sem_Trywait(&discoverCharacteristicsSem) == 0)
        {
            /* Do nothing*/
        }
        
        while (ARSAL_Sem_Trywait(&disconnectionSem) == 0)
        {
            /* Do nothing*/
        }
        
        while (ARSAL_Sem_Trywait(&configurationSem) == 0)
        {
            /* Do nothing*/
        }
        
        [_registeredNotificationCharacteristics removeAllObjects];
    }
}

@end
