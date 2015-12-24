//
//  MFHError.h
//  Quantum
//
//  Created by Phillip Pasqual on 9/12/15.
//  Copyright (c) 2015 Misfit. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface MFHError : NSError

typedef NS_ENUM(NSInteger, MFHErrorType)
{
    MFHNetworkError = 1,
    MFHMisfitError
};

// Internal
//@property (assign, nonatomic) NSInteger httpErrorCode;
//@property (assign, nonatomic) BOOL isBusinessError;
//@property (assign, nonatomic) NSInteger businessErrorCode;

// Debug only. Don't use them to make decision
//@property (strong, nonatomic) NSString * debugMessage;
//@property (strong, nonatomic) NSError * internalError;

// To display
@property (strong, nonatomic) NSString * localizedTitle;
@property (strong, nonatomic) NSString * localizedMessage;
@property (nonatomic) NSInteger errorCode;
@property (nonatomic) MFHErrorType errorType;

@end
