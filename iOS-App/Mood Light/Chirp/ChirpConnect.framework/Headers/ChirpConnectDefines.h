/*------------------------------------------------------------------------------
 *
 *  ChirpConnectDefines.h
 *
 *  This file is part of the Chirp Connect SDK for iOS.
 *  For full information on usage and licensing, see http://chirp.io/
 *
 *  Copyright © 2011-2019, Asio Ltd.
 *  All rights reserved.
 *
 *----------------------------------------------------------------------------*/

#ifndef ChirpConnectDefines_h
#define ChirpConnectDefines_h

#define CHIRP_STATIC_STRING static NSString * _Nonnull

/*------------------------------------------------------------------------------
 * Notifications
 *-----------------------------------------------------------------------------*/
CHIRP_STATIC_STRING CONNECT_NOTIFICATION_SENDING = @"io.chirp.notification.sending";
CHIRP_STATIC_STRING CONNECT_NOTIFICATION_SENT = @"io.chirp.notification.sent";
CHIRP_STATIC_STRING CONNECT_NOTIFICATION_RECEIVING =@"io.chirp.notification.receiving";
CHIRP_STATIC_STRING CONNECT_NOTIFICATION_RECEIVED = @"io.chirp.notification.received";
CHIRP_STATIC_STRING CONNECT_NOTIFICATION_VOLUME = @"io.chirp.notification.volume";
CHIRP_STATIC_STRING CONNECT_NOTIFICATION_VOLUME_KEY_STATE = @"volume";
CHIRP_STATIC_STRING CONNECT_NOTIFICATION_STATE = @"io.chirp.notification.state";
CHIRP_STATIC_STRING CONNECT_NOTIFICATION_STATE_KEY_STATE = @"state";
CHIRP_STATIC_STRING CONNECT_NOTIFICATION_AUTHENTICATION_STATE_UPDATED = @"io.chirp.notification.authentication";

/*------------------------------------------------------------------------------
 * Errors
 *------------------------------------------------------------------------------*/
CHIRP_STATIC_STRING CONNECT_ERROR_DOMAIN = @"io.chirp.error";

//------------------------------------------------------------------------------
#pragma mark - Typedefs
//------------------------------------------------------------------------------

typedef chirp_connect_state_t CHIRP_CONNECT_STATE;

typedef void (^ChirpStartedBlock)               (void);
typedef void (^ChirpStoppedBlock)               (void);
typedef void (^ChirpSendingBlock)               (NSData * _Nonnull data,
                                                 NSUInteger channel);
typedef void (^ChirpSentBlock)                  (NSData * _Nonnull data,
                                                 NSUInteger channel);
typedef void (^ChirpReceivingBlock)             (NSUInteger channel);
typedef void (^ChirpReceivedBlock)              (NSData * _Nonnull data,
                                                NSUInteger channel);
typedef void (^ChirpStateUpdatedBlock)          (CHIRP_CONNECT_STATE oldState,
                                                CHIRP_CONNECT_STATE newState);
typedef void (^ChirpAuthenticatedBlock)         (NSError *_Nullable error);
typedef void (^ChirpSetConfigFromNetworkBlock)  (NSError *_Nullable error);
typedef void (^ChirpVolumeChangedBlock)         (float volume);
typedef void (^ChirpAudioBufferUpdatedBlock)    (AudioBuffer buffer,
                                                 NSUInteger numberOfFrames);

//------------------------------------------------------------------------------
#pragma mark - Enums
//------------------------------------------------------------------------------

typedef NS_ENUM(NSInteger, ChirpAudioMode) {
    ChirpAudioModeSend,
    ChirpAudioModeReceive,
    ChirpAudioModeSendAndReceive
};

//------------------------------------------------------------------------------
#pragma mark - Exports
//------------------------------------------------------------------------------

FOUNDATION_EXPORT double FrameworkVersionNumber;
FOUNDATION_EXPORT const unsigned char FrameworkVersionString[];

#endif /* ChirpConnectDefines_h */
