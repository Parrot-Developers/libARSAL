//
//  ARSAL_CentralManager.h
//  ARFreeFlight
//
//  Created by Frédéric D'HAEYER on 12/03/14.
//  Copyright (c) 2014 Parrot SA. All rights reserved.
//
#import <CoreBluetooth/CoreBluetooth.h>

@interface ARSAL_CentralManager : NSObject
- (void)addDelegate:(id <CBCentralManagerDelegate>)delegate;
- (void)removeDelegate:(id <CBCentralManagerDelegate>)delegate;
@end
