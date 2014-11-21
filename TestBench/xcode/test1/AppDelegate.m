/*
    Copyright (C) 2014 Parrot SA

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the 
      distribution.
    * Neither the name of Parrot nor the names
      of its contributors may be used to endorse or promote products
      derived from this software without specific prior written
      permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
    FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
    COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
    BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
    OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
    AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
    OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
    SUCH DAMAGE.
*/
//
//  AppDelegate.m
//  test1
//
//  Created by Frédéric D'Haeyer on 6/7/12.
//  Copyright (c) 2012 Parrot SA. All rights reserved.
//

#import "AppDelegate.h"
#include <stdio.h>
#include <libARSAL/ARSAL_Mutex.h>
#include <libARSAL/ARSAL_Thread.h>
#include <libARSAL/ARSAL_Print.h>

ARSAL_Mutex_t mutex;
ARSAL_Cond_t cond;
int variable = 0;

void *routine(void *arg)
{
    ARSAL_PRINT(ARSAL_PRINT_DEBUG, "Routine started\n");
    ARSAL_PRINT(ARSAL_PRINT_WARNING, "mutex lock\n");
    ARSAL_Mutex_Lock(&mutex);
    variable = 1;
    ARSAL_PRINT(ARSAL_PRINT_ERROR, "mutex signal\n");
    ARSAL_Cond_Signal(&cond);
    ARSAL_PRINT(ARSAL_PRINT_WARNING, "mutex unlock\n");
    ARSAL_Mutex_Unlock(&mutex);

    return NULL;
}

@implementation AppDelegate

@synthesize window = _window;
@synthesize managedObjectContext = __managedObjectContext;
@synthesize managedObjectModel = __managedObjectModel;
@synthesize persistentStoreCoordinator = __persistentStoreCoordinator;

- (void)launchThread
{
    ARSAL_Thread_t thread;

    ARSAL_PRINT(ARSAL_PRINT_ERROR, "mutex init\n");
    ARSAL_Mutex_Init(&mutex);

    ARSAL_PRINT(ARSAL_PRINT_WARNING, "condition init\n");
    ARSAL_Cond_Init(&cond);

    ARSAL_PRINT(ARSAL_PRINT_WARNING, "thread create\n");
    ARSAL_Thread_Create(&thread, routine, NULL);

    ARSAL_PRINT(ARSAL_PRINT_ERROR, "Variable : %d\n", variable);

    ARSAL_Thread_Join(thread, NULL);

    ARSAL_PRINT(ARSAL_PRINT_ERROR, "mutex lock\n");
    ARSAL_Mutex_Lock(&mutex);
    ARSAL_PRINT(ARSAL_PRINT_ERROR, "mutex wait\n");
    ARSAL_Cond_Timedwait(&cond, &mutex, 1000);
    //ARSAL_Cond_Wait(&cond, &mutex);
    ARSAL_PRINT(ARSAL_PRINT_ERROR, "mutex unlock\n");
    ARSAL_Mutex_Unlock(&mutex);

    ARSAL_PRINT(ARSAL_PRINT_DEBUG, "Variable : %d\n", variable);

    ARSAL_PRINT(ARSAL_PRINT_DEBUG, "condition destroy\n");
    ARSAL_Cond_Destroy(&cond);

    ARSAL_PRINT(ARSAL_PRINT_DEBUG, "mutex destroy\n");
    ARSAL_Mutex_Destroy(&mutex);

    ARSAL_PRINT(ARSAL_PRINT_DEBUG, "thread destroy\n");
    ARSAL_Thread_Destroy(&thread);
}

- (void)dealloc
{
    [_window release];
    [__managedObjectContext release];
    [__managedObjectModel release];
    [__persistentStoreCoordinator release];
    [super dealloc];
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    [self performSelectorInBackground:@selector(launchThread) withObject:nil];
    self.window = [[[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]] autorelease];
    // Override point for customization after application launch.
    self.window.backgroundColor = [UIColor whiteColor];
    [self.window makeKeyAndVisible];
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    // Saves changes in the application's managed object context before the application terminates.
    [self saveContext];
}

- (void)saveContext
{
    NSError *error = nil;
    NSManagedObjectContext *managedObjectContext = self.managedObjectContext;
    if (managedObjectContext != nil) {
        if ([managedObjectContext hasChanges] && ![managedObjectContext save:&error]) {
            // Replace this implementation with code to handle the error appropriately.
            // abort() causes the application to generate a crash log and terminate. You should not use this function in a shipping application, although it may be useful during development.
            NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
            abort();
        }
    }
}

#pragma mark - Core Data stack

// Returns the managed object context for the application.
// If the context doesn't already exist, it is created and bound to the persistent store coordinator for the application.
- (NSManagedObjectContext *)managedObjectContext
{
    if (__managedObjectContext != nil) {
        return __managedObjectContext;
    }

    NSPersistentStoreCoordinator *coordinator = [self persistentStoreCoordinator];
    if (coordinator != nil) {
        __managedObjectContext = [[NSManagedObjectContext alloc] init];
        [__managedObjectContext setPersistentStoreCoordinator:coordinator];
    }
    return __managedObjectContext;
}

// Returns the managed object model for the application.
// If the model doesn't already exist, it is created from the application's model.
- (NSManagedObjectModel *)managedObjectModel
{
    if (__managedObjectModel != nil) {
        return __managedObjectModel;
    }
    NSURL *modelURL = [[NSBundle mainBundle] URLForResource:@"test1" withExtension:@"momd"];
    __managedObjectModel = [[NSManagedObjectModel alloc] initWithContentsOfURL:modelURL];
    return __managedObjectModel;
}

// Returns the persistent store coordinator for the application.
// If the coordinator doesn't already exist, it is created and the application's store added to it.
- (NSPersistentStoreCoordinator *)persistentStoreCoordinator
{
    if (__persistentStoreCoordinator != nil) {
        return __persistentStoreCoordinator;
    }

    NSURL *storeURL = [[self applicationDocumentsDirectory] URLByAppendingPathComponent:@"test1.sqlite"];

    NSError *error = nil;
    __persistentStoreCoordinator = [[NSPersistentStoreCoordinator alloc] initWithManagedObjectModel:[self managedObjectModel]];
    if (![__persistentStoreCoordinator addPersistentStoreWithType:NSSQLiteStoreType configuration:nil URL:storeURL options:nil error:&error]) {
        /*
          Replace this implementation with code to handle the error appropriately.

          abort() causes the application to generate a crash log and terminate. You should not use this function in a shipping application, although it may be useful during development.

          Typical reasons for an error here include:
          * The persistent store is not accessible;
          * The schema for the persistent store is incompatible with current managed object model.
          Check the error message to determine what the actual problem was.


          If the persistent store is not accessible, there is typically something wrong with the file path. Often, a file URL is pointing into the application's resources directory instead of a writeable directory.

          If you encounter schema incompatibility errors during development, you can reduce their frequency by:
          * Simply deleting the existing store:
          [[NSFileManager defaultManager] removeItemAtURL:storeURL error:nil]

          * Performing automatic lightweight migration by passing the following dictionary as the options parameter:
          [NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithBool:YES], NSMigratePersistentStoresAutomaticallyOption, [NSNumber numberWithBool:YES], NSInferMappingModelAutomaticallyOption, nil];

          Lightweight migration will only work for a limited set of schema changes; consult "Core Data Model Versioning and Data Migration Programming Guide" for details.

        */
        NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
        abort();
    }

    return __persistentStoreCoordinator;
}

#pragma mark - Application's Documents directory

// Returns the URL to the application's Documents directory.
- (NSURL *)applicationDocumentsDirectory
{
    return [[[NSFileManager defaultManager] URLsForDirectory:NSDocumentDirectory inDomains:NSUserDomainMask] lastObject];
}

@end
