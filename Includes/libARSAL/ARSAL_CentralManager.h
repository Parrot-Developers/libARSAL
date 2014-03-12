//
//  ARSAL_CentralManager.h
//  ARFreeFlight
//
//  Created by Frédéric D'HAEYER on 12/03/14.
//  Copyright (c) 2014 Parrot SA. All rights reserved.
//
#import <CoreBluetooth/CoreBluetooth.h>

@interface ARSAL_CentralManager : NSObject

- (void)initARCBCentralManager:(dispatch_queue_t)queue options:(NSDictionary *)options;
- (id)initWithQueue:(dispatch_queue_t)queue;
- (id)initWithDelegate:(id<CBCentralManagerDelegate>)delegate queue:(dispatch_queue_t)queue options:(NSDictionary *)options;

- (void)addDelegate:(id <CBCentralManagerDelegate>)delegate;
- (void)removeDelegate:(id <CBCentralManagerDelegate>)delegate;

- (void)connectPeripheral:(CBPeripheral *)peripheral options:(NSDictionary *)options;
- (void)cancelPeripheralConnection:(CBPeripheral *)peripheral;
- (void)scanForPeripheralsWithServices:(NSArray *)serviceUUIDs options:(NSDictionary *)options;
- (void)stopScan;

- (NSArray *)retrieveConnectedPeripheralsWithServices:(NSArray *)serviceUUIDs;
- (NSArray *)retrievePeripheralsWithIdentifiers:(NSArray *)identifiers;

@end
