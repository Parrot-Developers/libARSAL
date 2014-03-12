//
//  ARSAL_CentralManager.m.m
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
    _delegateArray = [[NSMutableArray alloc] init];
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
    if((delegate != nil) && (![_delegateArray containsObject:delegate]))
    {
        [_delegateArray addObject:delegate];
    }
}

- (void)removeDelegate:(id<CBCentralManagerDelegate>)delegate
{
    if((delegate != nil) && ([_delegateArray containsObject:delegate]))
    {
        [_delegateArray removeObject:delegate];
    }
}

#pragma mark CBCentralManagerDelegate required method
- (void)centralManagerDidUpdateState:(CBCentralManager *)central
{
    for(id<CBCentralManagerDelegate>delegate in _delegateArray)
    {
        [delegate centralManagerDidUpdateState:central];
    }
}

#pragma mark CBCentralManagerDelegate optional method
- (void)centralManager:(CBCentralManager *)central willRestoreState:(NSDictionary *)dict
{
    for(id<CBCentralManagerDelegate>delegate in _delegateArray)
    {
        if([delegate respondsToSelector:@selector(centralManager:willRestoreState:)])
            [delegate centralManager:central willRestoreState:dict];
    }
}

- (void)centralManager:(CBCentralManager *)central didRetrievePeripherals:(NSArray *)peripherals
{
    for(id<CBCentralManagerDelegate>delegate in _delegateArray)
    {
        if([delegate respondsToSelector:@selector(centralManager:didRetrievePeripherals:)])
            [delegate centralManager:central didRetrievePeripherals:peripherals];
    }
}

- (void)centralManager:(CBCentralManager *)central didRetrieveConnectedPeripherals:(NSArray *)peripherals
{
    for(id<CBCentralManagerDelegate>delegate in _delegateArray)
    {
        if([delegate respondsToSelector:@selector(centralManager:didRetrieveConnectedPeripherals:)])
            [delegate centralManager:central didRetrieveConnectedPeripherals:peripherals];
    }
}

- (void)centralManager:(CBCentralManager *)central didDiscoverPeripheral:(CBPeripheral *)peripheral advertisementData:(NSDictionary *)advertisementData RSSI:(NSNumber *)RSSI
{
    for(id<CBCentralManagerDelegate>delegate in _delegateArray)
    {
        if([delegate respondsToSelector:@selector(centralManager:didDiscoverPeripheral:advertisementData:RSSI:)])
            [delegate centralManager:central didDiscoverPeripheral:peripheral advertisementData:advertisementData RSSI:RSSI];
    }
}

- (void)centralManager:(CBCentralManager *)central didConnectPeripheral:(CBPeripheral *)peripheral
{
    for(id<CBCentralManagerDelegate>delegate in _delegateArray)
    {
        if([delegate respondsToSelector:@selector(centralManager:didConnectPeripheral:)])
            [delegate centralManager:central didConnectPeripheral:peripheral];
    }
}

- (void)centralManager:(CBCentralManager *)central didFailToConnectPeripheral:(CBPeripheral *)peripheral error:(NSError *)error
{
    for(id<CBCentralManagerDelegate>delegate in _delegateArray)
    {
        if([delegate respondsToSelector:@selector(centralManager:didFailToConnectPeripheral:error:)])
            [delegate centralManager:central didFailToConnectPeripheral:peripheral error:error];
    }
}

- (void)centralManager:(CBCentralManager *)central didDisconnectPeripheral:(CBPeripheral *)peripheral error:(NSError *)error
{
    for(id<CBCentralManagerDelegate>delegate in _delegateArray)
    {
        if([delegate respondsToSelector:@selector(centralManager:didDisconnectPeripheral:error:)])
            [delegate centralManager:central didDisconnectPeripheral:peripheral error:error];
    }
}

@end
