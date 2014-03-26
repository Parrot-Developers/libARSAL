//
//  ARSAL_CentralManager.m
//  ARFreeFlight
//
//  Created by Frédéric D'HAEYER on 12/03/14.
//  Copyright (c) 2014 Parrot SA. All rights reserved.
//
#import "libARSAL/ARSAL_CentralManager.h"

@interface ARSAL_CentralManager () <CBCentralManagerDelegate>
@property (nonatomic, strong) NSMutableArray *delegateArray;
@property (nonatomic, strong) CBCentralManager *centralManager;
@end

@implementation ARSAL_CentralManager
- (void)initARCBCentralManager:(dispatch_queue_t)queue options:(NSDictionary *)options
{
    _delegateArray = [NSMutableArray array];
    _centralManager = [[CBCentralManager alloc] initWithDelegate:self queue:queue options:options];
}

- (id)init
{
    self = [super init];
    if(self)
    {
        [self initARCBCentralManager:nil options:nil];
    }
    
    return self;
}

- (id)initWithQueue:(dispatch_queue_t)queue
{
    self = [super init];
    if(self)
    {
        [self initARCBCentralManager:queue options:nil];
    }
    
    return self;
}

- (id)initWithDelegate:(id<CBCentralManagerDelegate>)delegate queue:(dispatch_queue_t)queue options:(NSDictionary *)options
{
    self = [super init];
    if(self)
    {
        [self initARCBCentralManager:queue options:options];
    }
    
    return self;
}

- (void)dealloc
{
    _centralManager.delegate = nil;
    _centralManager = nil;
    _delegateArray = nil;
}

- (void)addDelegate:(id<CBCentralManagerDelegate>)delegate
{
    @synchronized (self)
    {
        if((delegate != nil) && (![_delegateArray containsObject:delegate]))
        {
            [_delegateArray addObject:delegate];
        }
    }
}

- (void)removeDelegate:(id<CBCentralManagerDelegate>)delegate
{
    @synchronized (self)
    {
        if((delegate != nil) && ([_delegateArray containsObject:delegate]))
        {
            [_delegateArray removeObject:delegate];
        }
    }
}

#pragma mark CBCentralManagerDelegate required method
- (void)centralManagerDidUpdateState:(CBCentralManager *)central
{
    NSMutableArray *delegateArray = nil;
    @synchronized (self)
    {
        delegateArray = [_delegateArray copy];
    }
    
    for(id<CBCentralManagerDelegate>delegate in delegateArray)
    {
        [delegate centralManagerDidUpdateState:central];
    }
}

#pragma mark CBCentralManagerDelegate optional method
- (void)centralManager:(CBCentralManager *)central willRestoreState:(NSDictionary *)dict
{
    NSMutableArray *delegateArray = nil;
    @synchronized (self)
    {
        delegateArray = [_delegateArray copy];
    }
    
    for(id<CBCentralManagerDelegate>delegate in delegateArray)
    {
        if([delegate respondsToSelector:@selector(centralManager:willRestoreState:)])
            [delegate centralManager:central willRestoreState:dict];
    }
}

- (void)centralManager:(CBCentralManager *)central didRetrievePeripherals:(NSArray *)peripherals
{
    NSMutableArray *delegateArray = nil;
    @synchronized (self)
    {
        delegateArray = [_delegateArray copy];
    }
    
    for(id<CBCentralManagerDelegate>delegate in delegateArray)
    {
        if([delegate respondsToSelector:@selector(centralManager:didRetrievePeripherals:)])
            [delegate centralManager:central didRetrievePeripherals:peripherals];
    }
}

- (void)centralManager:(CBCentralManager *)central didRetrieveConnectedPeripherals:(NSArray *)peripherals
{
    NSMutableArray *delegateArray = nil;
    @synchronized (self)
    {
        delegateArray = [_delegateArray copy];
    }
    
    for(id<CBCentralManagerDelegate>delegate in delegateArray)
    {
        if([delegate respondsToSelector:@selector(centralManager:didRetrieveConnectedPeripherals:)])
            [delegate centralManager:central didRetrieveConnectedPeripherals:peripherals];
    }
}

- (void)centralManager:(CBCentralManager *)central didDiscoverPeripheral:(CBPeripheral *)peripheral advertisementData:(NSDictionary *)advertisementData RSSI:(NSNumber *)RSSI
{
    NSMutableArray *delegateArray = nil;
    @synchronized (self)
    {
        delegateArray = [_delegateArray copy];
    }
    
    for(id<CBCentralManagerDelegate>delegate in delegateArray)
    {
        if([delegate respondsToSelector:@selector(centralManager:didDiscoverPeripheral:advertisementData:RSSI:)])
            [delegate centralManager:central didDiscoverPeripheral:peripheral advertisementData:advertisementData RSSI:RSSI];
    }
}

- (void)centralManager:(CBCentralManager *)central didConnectPeripheral:(CBPeripheral *)peripheral
{
    NSMutableArray *delegateArray = nil;
    @synchronized (self)
    {
        delegateArray = [_delegateArray copy];
    }
    
    for(id<CBCentralManagerDelegate>delegate in delegateArray)
    {
        if([delegate respondsToSelector:@selector(centralManager:didConnectPeripheral:)])
            [delegate centralManager:central didConnectPeripheral:peripheral];
    }
}

- (void)centralManager:(CBCentralManager *)central didFailToConnectPeripheral:(CBPeripheral *)peripheral error:(NSError *)error
{
    NSMutableArray *delegateArray = nil;
    @synchronized (self)
    {
        delegateArray = [_delegateArray copy];
    }
    
    for(id<CBCentralManagerDelegate>delegate in delegateArray)
    {
        if([delegate respondsToSelector:@selector(centralManager:didFailToConnectPeripheral:error:)])
            [delegate centralManager:central didFailToConnectPeripheral:peripheral error:error];
    }
}

- (void)centralManager:(CBCentralManager *)central didDisconnectPeripheral:(CBPeripheral *)peripheral error:(NSError *)error
{
    NSMutableArray *delegateArray = nil;
    @synchronized (self)
    {
        delegateArray = [_delegateArray copy];
    }
    
    for(id<CBCentralManagerDelegate>delegate in delegateArray)
    {
        if([delegate respondsToSelector:@selector(centralManager:didDisconnectPeripheral:error:)])
            [delegate centralManager:central didDisconnectPeripheral:peripheral error:error];
    }
}

- (void)connectPeripheral:(CBPeripheral *)peripheral options:(NSDictionary *)options
{
    [_centralManager connectPeripheral:peripheral options:options];
}

- (void)cancelPeripheralConnection:(CBPeripheral *)peripheral
{
    [_centralManager cancelPeripheralConnection:peripheral];
}

- (void)scanForPeripheralsWithServices:(NSArray *)serviceUUIDs options:(NSDictionary *)options
{
    [_centralManager scanForPeripheralsWithServices:serviceUUIDs options:options];
}

- (void)stopScan
{
    [_centralManager stopScan];
}


- (NSArray *)retrieveConnectedPeripheralsWithServices:(NSArray *)serviceUUIDs
{
    return [_centralManager retrieveConnectedPeripheralsWithServices:serviceUUIDs];
}

- (NSArray *)retrievePeripheralsWithIdentifiers:(NSArray *)identifiers
{
    return [_centralManager retrievePeripheralsWithIdentifiers:identifiers];
}

@end
