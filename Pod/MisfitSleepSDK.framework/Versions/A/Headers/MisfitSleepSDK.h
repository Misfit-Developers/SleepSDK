//
//  MFHSession.h
//  Quantum
//
//  Created by Phillip Pasqual on 9/7/15.
//  Copyright (c) 2015 Misfit. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "MFHError.h"
#import "MFHEnums.h"

typedef void (^MFHCompletion) (NSDictionary * data, MFHError * error);
typedef void (^MFHWakeupCompletion) (MFHError *error);

@protocol MisfitSleepTrackingEventDelegate <NSObject>

- (void) performActionByEvent:(MisfitTrackingEventType)event;

@end

@protocol MisfitSleepDeviceStateDelegate <NSObject>

@optional
- (void) onDeviceStateChangeWithState:(MisfitDeviceState)state
                         serialNumber:(NSString *)serialNumber;

@end


@interface MisfitSleepSDK : NSObject

@property (nonatomic, readonly) BOOL isConnected;
@property (nonatomic, readonly) BOOL isMisfitAppInstalled;

@property (nonatomic, strong) id<MisfitSleepTrackingEventDelegate> trackingEventDelegate;
@property (nonatomic, weak) id<MisfitSleepDeviceStateDelegate> stateChangeDelegate;

@property (nonatomic, readonly) BOOL isWakeupTimeSet;
@property (nonatomic, assign, readonly) NSTimeInterval wakeupTimestamp;

+ (MisfitSleepSDK *) sharedInstance;

- (void)setWakeupTimeForHour:(NSInteger)hour minute:(NSInteger)minute
          completion:(MFHWakeupCompletion)completion;

- (void) stopWakeupTracking;

- (void) connectWithAppId:(NSString *) appKey appSecret:(NSString *) appSecret completion:(MFHCompletion) completion;

- (void) refreshServiceStatus:(MFHCompletion) completion;

- (void) disconnect;

- (BOOL) handleOpenURL:(NSURL *) url;

- (BOOL) canHandleOpenUrl:(NSURL *) url;

- (void) handleDidBecomeActive;

@end