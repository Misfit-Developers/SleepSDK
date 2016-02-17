# MisfitSleepSDK

[![CI Status](http://img.shields.io/travis/Phill Pasqual/MisfitSleepSDK.svg?style=flat)](https://travis-ci.org/Phill Pasqual/MisfitSleepSDK)
[![Version](https://img.shields.io/cocoapods/v/MisfitSleepSDK.svg?style=flat)](http://cocoapods.org/pods/MisfitSleepSDK)
[![License](https://img.shields.io/cocoapods/l/MisfitSleepSDK.svg?style=flat)](http://cocoapods.org/pods/MisfitSleepSDK)
[![Platform](https://img.shields.io/cocoapods/p/MisfitSleepSDK.svg?style=flat)](http://cocoapods.org/pods/MisfitSleepSDK)

## Installation

MisfitSleepSDK is available through [CocoaPods](http://cocoapods.org). To install
it, simply add the following line to your Podfile:

```ruby
pod "MisfitSleepSDK"
```

## Author

Phill Pasqual, phill@misfit.com

## License

MisfitSleepSDK is available under the MIT license. See the LICENSE file for more info.

## Usage

* [Getting Started](#getting)
* [Set Up a Misfit Cloup App](#setup)
* [Configure Your Application's PList File](#configure)
* [Configure Linker Flags](#linker)
* [Adding a Reference to the Sleep SDK](#adding)
* [Enable Bluetooth LE Background Mode](#enable)
* [Setting Up the Application Delegate](#delegate)
* [Authorization](#auth)
* [Setting Up Wakeup Tracking](#wakue)
* [Receiving Sleep Events](#sleep)
* [Receiving Device State Change Events](#device)
* [Refreshing the Service Status](#refresh)
* [Errors](#errors)
* [Reference](#reference)


## <a name="getting"></a>  Getting Started
### <a name="setup"></a> Set Up a Misfit Cloup App
1. go to the [Misfit Developer Portal](https://build.misfit.com)
2. if you do not have a developer account, [create one](/signup)
3. navigate to the "Misfit Cloud API" section
4. click "Register an App"
5. fill out the form and click register
![image of regristration](/assets/images/docs/registration.png?w=700)
6. your app key and app secret will be used to authorize your iOS app with Misfit
![image of app key and secret](/assets/images/docs/appkeysecret.png?w=350)
7. make sure the Misfit iOS App is installed on your device

### <a name="configure"></a> Configure Your Application's PList File
Add the following to your application's plist file:
```
<key>CFBundleURLTypes</key>
<array>
<dict>
<key>CFBundleURLName</key>
<string></string>
<key>CFBundleURLSchemes</key>
<array>
<string>mfc-appkey</string>
</array>
</dict>
</array>
```
make sure to replace appkey with the app key that you used when setting up the cloud app (see above).

When using iOS 9, the following should also be added to the plist file:
```
<key>LSApplicationQueriesSchemes</key>
<array>
<string>shine</string>
</array>
```

### <a name="linker"></a> Configure Linker Flags
Finally, open your project properties and navigate to build settings -> linking and find the "other linker flags" section. Add "-ObjC" to both debug and release.

![image of linker flags](/assets/images/docs/linkerflags.png?w=350)

### <a name="adding"></a> Adding a Reference to the Sleep SDK
1. In Xcode, open up your project properties
2. Navigate to "Build Phases"
3. In the "Link Binaries With Libraries" section, add *CoreBluetooth.framework* and *MisfitSleepSDK.framework*

![image of frameworks](/assets/images/docs/frameworks.png?w=550)

### <a name="enable"></a> Enable Bluetooth LE Background Mode
1. In Xcode, navigate to the "Capabilities" tab in your project properties
2. In the "Background Modes" secion, check the box next to "Uses Bluetooth LE accessories"

![image of ble accessories](/assets/images/docs/ble.png?w=700)

## <a name="delegate"></a> Setting Up the Application Delegate
First, import the SDK:
```
#import <MisfitSleepSDK/MisfitSleepSDK.h>
```

The following code should be added to your application delegate to handle redirection to your app after the authorization process:
```objective-c
- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url
sourceApplication:(NSString *)sourceApplication
annotation:(id)annotation
{
BOOL canHandle = NO;
if ([[MisfitSleepSDK sharedInstance] canHandleOpenUrl:url])
{
canHandle = [[MisfitSleepSDK sharedInstance] handleOpenURL:url];
}
return canHandle;
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
[[MisfitSleepSDK sharedInstance] handleDidBecomeActive];
}
```
## <a name="auth"></a> Authorization
The following code can be used to authorize or disconnect your app with a standard UISwitch:
```objective-c
- (IBAction)onConnectSwitchChanged:(UISwitch *)sender
{
BOOL shouldConnect = sender.on;
if(shouldConnect)
{
[[MisfitSleepSDK sharedInstance]connectWithAppId:@"your-misfit-app-id"
appSecret:@"your-misfit-app-secret"
completion:^(NSDictionary * data, MFHError * error)
{
if (error) {
NSLog(@"Error: %@ Description: %@", error.localizedMessage, error.description);
return; 
}
NSString * token = [data objectForKey:@"access_token"];
}];
}
else{
[[MisfitSleepSDK sharedInstance] disconnect];
}   
}
```

## <a name="wakeup"></a> Setting Up Wakeup Tracking
The following code can be used to set an alarm with a given time:
```objective-c
- (IBAction)onAlarmSwitchChanged:(UISwitch *)sender
{
if ( [[MisfitSleepSDK sharedInstance] isConnected]  &&  sender.on)
{
void (^MFHCompletion)(MFHError * error) = ^(MFHError *error) {
if (error)
{
NSLog(@"Error: %@ Description: %@", error.localizedMessage, error.description);
return;
}
};

//get hour and minute of alarm time
NSDate *date = [_alarmTimePicker date];
NSCalendar *calendar = [NSCalendar currentCalendar];
NSDateComponents *components = [calendar components:(NSHourCalendarUnit | NSMinuteCalendarUnit) fromDate:date];
NSInteger hour = [components hour];
NSInteger minute = [components minute];

//set the wakeupTime
[[MisfitSleepSDK sharedInstance] setWakeupTimeForHour:hour minute:minute completion:MFHCompletion];
}else{
//stop tracking
[[MisfitSleepSDK sharedInstance] stopWakeupTracking];
}
}
```

## <a name="sleep"></a> Receiving Sleep Events
Setup a class in your project to receive events from the MisfitSleepTrackingEventDelegate. The example below uses a ViewController.
```objective-c
@interface MainViewController ()<MisfitSleepTrackingEventDelegate>
...
- (void)viewDidLoad {
[MisfitSleepSDK sharedInstance].trackingEventDelegate = self;
}
...
- (void) performActionByEvent:(MisfitTrackingEventType) event
{
if(event == MisfitTrackingEventTypeFallAsleep)
{
//fall asleep event triggered
}
else if(event == MisfitTrackingEventTypeWakeup)
{
//wakeup event triggered
}
}
```
## <a name="device"></a> Receiving Device State Change Events
Setup a class in your project to receive events from the MisfitSleepDeviceStateDelegate. In this example, we used a ViewController.
```objective-c
@interface MainViewController ()<MFHDeviceStateDelegate>
...
- (void)viewDidLoad {
[MisfitSleepSDK sharedInstance].stateChangeDelegate = self;
}
...
- (void)onDeviceStateChangeWithState:(MisfitDeviceState)state
serialNumber:(NSString *)serialNumber
{
if (state == MisfitDeviceStateUnavailable) {
//device is not connected
}
else if (state == MisfitDeviceStateConnecting){
//device is connecting
}
else if (state == MisfitDeviceStateAvailable){
//device is connected
} 
}
```
## <a name="refresh"></a> Refreshing the Service Status
The Misfit Sleep SDK uses the device that is currently synced with the Misfit App as the sleep tracking device. However, if a user would like to use a different device, they must change to a new device in the Misfit App devices page. After switching to a new device, your app needs to reflect this change. This can be done by calling the refreshServiceStatus: function provided by the Misfit Sleep SDK. Following is come example code showing how to refresh the service status by pressing a standard UIButton:
```objective-c
- (IBAction)onRefreshButtonPressed:(UIButton *)sender
{
[[MisfitSleepSDK sharedInstance] refreshServiceStatus:^(NSDictionary
*data, MFHError *error) {
if (error) {
NSLog(@"error: %@", error);
}
//the device that is currently synced in the Misfit App is
//returned in the data dictionary
NSLog(@“Device serial number: %@",data[@"serialNumber"]);
}];
}
```
## <a name="errors"></a> Errors
#### MFHError
```objective-c
typedef NS_ENUM(NSInteger, MFHErrorType)
{
MFHMisfitError,
MFHNetworkError
};
@property (strong, nonatomic) NSString * localizedTitle;
@property (strong, nonatomic) NSString * localizedMessage;
@property (nonatomic) NSInteger errorCode;
@property (nonatomic) MFHErrorType errorType;
```
#### MFHMisfitError
Error Code | Reason
---------- | ------
1 | invalid wakeup time
2 | current device is not supported
3 | no device is connected in the Misfit app 
4 | no Misfit device was found
5 | Bluetooth is turned off
6 | problem connecting to device
7 | invalid appKey or appSecret
#### MFHNetwork Error
standard http response codes apply


## <a name="reference"></a> Reference
#### MisfitSleepSDK.h
```objective-c
@interface MisfitSleepSDK : NSObject

//is the app connected with Misfit?
@property (nonatomic, readonly) BOOL isConnected;

@property (nonatomic, strong) id<MisfitSleepTrackingEventDelegate> trackingEventDelegate;
@property (nonatomic, weak) id<MisfitSleepDeviceStateDelegate> stateChangeDelegate;

@property (nonatomic, readonly) BOOL isWakeupTimeSet;
@property (nonatomic, assign, readonly) NSTimeInterval wakeupTimestamp;

+ (MisfitSleepSDK *) sharedInstance;

//setup a new sleep tracking session with the given wakeup hour (out of 24) and minute
- (void)setWakeupTimeForHour:(NSInteger)hour minute:(NSInteger)minute
completion:(MFHWakeupCompletion)completion;
- (void) stopWakeupTracking;

//authorize account with Misfit
- (void) connectWithAppId:(NSString *) appKey appSecret:(NSString *) appSecret completion:(MFHCompletion) completion;

//used to check if the app is still connected with Misfit and to 
//update the device being used to the device that is currently synced 
//with the Misfit App. If it has changed, the new device will 
//automatically be connected
- (void) refreshServiceStatus:(MFHCompletion) completion;

//disconnect from Misfit
- (void) disconnect;

//used in the application delegate to handle redirection from the Misfit App
- (BOOL) handleOpenURL:(NSURL *) url;
- (BOOL) canHandleOpenUrl:(NSURL *) url;
- (void) handleDidBecomeActive;

@end
```



