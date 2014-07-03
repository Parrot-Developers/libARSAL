/**
 * @file  ARSAL_BLEManager.h
 * @brief private headers of BLE manager allow to use Bluetooth low energy api's
 * @date 06/11/2013
 * @author frederic.dhaeyer@parrot.com
 */

#ifndef _ARSAL_BLEMANAGER_H_
#define _ARSAL_BLEMANAGER_H_

#import <Foundation/Foundation.h>
#import <CoreBluetooth/CoreBluetooth.h>
#include "libARSAL/ARSAL_Print.h"
#include "libARSAL/ARSAL_Mutex.h"
#include "libARSAL/ARSAL_Sem.h"
#include "libARSAL/ARSAL_CentralManager.h"
#include "libARSAL/ARSAL_Error.h"
#include "ARSAL_Singleton.h"

@interface CBUUID (StringExtraction)
- (NSString *)representativeString;
- (NSString *)shortUUID;
@end

@interface ARSALBLEManagerNotificationData : NSObject

@property (nonatomic, retain) CBCharacteristic *characteristic;
@property (nonatomic, retain) NSData *value;
@end

@interface ARSALBLEManagerNotification : NSObject
{
    ARSAL_Sem_t readCharacteristicsSem;
    ARSAL_Mutex_t readCharacteristicMutex;
}
@property (nonatomic, retain) NSArray *characteristics;
@property (nonatomic, retain) NSMutableArray *notificationsArray;
@end

@protocol ARSALBLEManagerDelegate <NSObject>
@required
- (void)onBLEDisconnect;
@end

@interface ARSAL_BLEManager : NSObject <CBCentralManagerDelegate, CBPeripheralDelegate>
{
    ARSAL_Sem_t connectionSem;
    ARSAL_Sem_t disconnectionSem;
    ARSAL_Sem_t discoverServicesSem;
    ARSAL_Sem_t discoverCharacteristicsSem;

    ARSAL_Sem_t configurationSem;
}

@property (nonatomic, assign) id <ARSALBLEManagerDelegate> delegate;
@property (nonatomic, retain) CBPeripheral *activePeripheral;
@property (nonatomic, retain) NSMutableArray *characteristicsNotifications;

DECLARE_SINGLETON_FOR_CLASS(ARSAL_BLEManager);

- (eARSAL_ERROR)connectToPeripheral:(CBPeripheral *)peripheral withCentralManager:(ARSAL_CentralManager *)centralManager;
- (void)disconnectPeripheral:(CBPeripheral *)peripheral withCentralManager:(ARSAL_CentralManager *)centralManager;
- (eARSAL_ERROR)discoverNetworkServices:(NSArray *)servicesUUIDs;
- (eARSAL_ERROR)discoverNetworkCharacteristics:(NSArray *)characteristicsUUIDs forService:(CBService *)service;
- (eARSAL_ERROR)setNotificationCharacteristic:(CBCharacteristic *)characteristic;
- (BOOL)writeData:(NSData *)data toCharacteristic:(CBCharacteristic *)characteristic;
- (BOOL)isPeripheralConnected;
- (void)registerNotificationCharacteristics:(NSArray *)characteristicsArray toKey:(NSString*)readCharacteristicsKey;
- (BOOL)unregisterNotificationCharacteristics:(NSString*)readCharacteristicsKey;
- (BOOL)cancelReadNotification:(NSString *)readCharacteristicsKey;
- (BOOL)readNotificationData:(NSMutableArray *)notificationsArray maxCount:(int)maxCount toKey:(NSString *)readCharacteristicsKey;
- (void)readData:(CBCharacteristic *)characteristic;
- (void)unlock;
- (void)reset;

@end

#endif /** _ARSAL_BLEMANAGER_H_ */

