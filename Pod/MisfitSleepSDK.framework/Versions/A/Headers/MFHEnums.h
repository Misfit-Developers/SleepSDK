//
//  MFHEnums.h
//  Quantum
//
//  Created by Phillip Pasqual on 10/29/15.
//  Copyright © 2015 Misfit. All rights reserved.
//

typedef enum {
    MisfitDeviceStateDisconnected,
    MisfitDeviceStateConnecting,
    MisfitDeviceStateConnected
} MisfitDeviceState;

typedef enum {
    MisfitTrackingEventTypeFallAsleep,
    MisfitTrackingEventTypeWakeup
} MisfitTrackingEventType;