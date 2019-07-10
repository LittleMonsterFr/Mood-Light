/*------------------------------------------------------------------------------
 *
 *  ChirpConnect.h
 *
 *  This file is part of the Chirp Connect SDK for iOS.
 *  For full information on usage and licensing, see http://chirp.io/
 *
 *  Copyright © 2011-2019, Asio Ltd.
 *  All rights reserved.
 *
 *----------------------------------------------------------------------------*/

@import AudioToolbox;
@import Foundation;

#import "chirp_connect.h"
#import "ChirpConnectDefines.h"

/**
 The main Chirp Connect SDK class. Use this to send and receive data using
 sound. Only one single instance should be instantiated per application.
 */
@interface ChirpConnect: NSObject

//------------------------------------------------------------------------------
#pragma mark - Properties
//------------------------------------------------------------------------------

/**
 The volume of the Chirp SDK within your application. This is not the overall
 system volume, just the volume of the chirp output into the main audio mix.
 Valid values are between 0.0 and 1.0.
 */
@property (nonatomic) float volume;

//------------------------------------------------------------------------------

/**
 The volume of the OS hardware volume.
 Valid values are between 0.0 and 1.0.
 @warning Not currently supported on macOS
 */
@property (nonatomic, readonly) float systemVolume;

//------------------------------------------------------------------------------

/**
 The maximum payload size, in bytes, that the SDK's current config permits.
 @return The maximum number of bytes able to be sent by the SDK's
         current config.
 */
@property (nonatomic, readonly) NSUInteger maxPayloadLength;

//------------------------------------------------------------------------------

/**
 The number of channels supported by the current protocol.
 Each channel corresponds to a different frequency band.
 Standard Chirp protocols typically only support a single channel.
 For multi-channel support, please get in touch <developers@chirp.io>
 */
@property (readonly) NSUInteger channelCount;

//------------------------------------------------------------------------------

/**
 The current channel number that Chirp signals are being sent over.
 Each channel corresponds to a different frequency band. Defaults to zero.
 Standard Chirp protocols typically only support a single channel.
 For multi-channel support, please get in touch <developers@chirp.io>
 */
- (NSUInteger)transmissionChannel;

//------------------------------------------------------------------------------

/**
 Set the current transmission channel.
 Each channel corresponds to a different frequency band. Defaults to zero.
 Standard Chirp protocols typically only support a single channel.
 For multi-channel support, please get in touch <developers@chirp.io>
 @param channel The index to set. Must be less than the maximum number of
 supported channels, as indicated by `channelCount`.
 @return nil if the channel is valid, or an NSError otherwise.
 */
- (NSError * _Nullable)setTransmissionChannel:(NSUInteger)channel;

//------------------------------------------------------------------------------

/**
 Get the current state of the SDK.
 */
@property (readonly) CHIRP_CONNECT_STATE state;

//------------------------------------------------------------------------------

/**
 Get the current state of the SDK for a given channel.
 If the channel does not exist, CHIRP_CONNECT_STATE_NOT_CREATED
 is returned.
 */
- (CHIRP_CONNECT_STATE)stateForChannel:(NSUInteger)channel;

//------------------------------------------------------------------------------

/**
 The full version string of the SDK and audio engine.
 */
@property (readonly) NSString * _Nonnull version;

//------------------------------------------------------------------------------

/**
 Information on the current audio transmission settings.
 */
@property (readonly)  NSString * _Nonnull info;

//------------------------------------------------------------------------------
#pragma mark - Methods: Initialisation
//------------------------------------------------------------------------------

/**
 Initialise the SDK. Sign up for credentials at the Chirp developer hub:
 https://developers.chirp.io/
 */
- (ChirpConnect * _Nullable) initWithAppKey:(NSString * _Nonnull) key
                                  andSecret:(NSString * _Nonnull) secret;

//------------------------------------------------------------------------------

/**
 Set the config to use within the SDK. This configures audio transmission
 properties, and must be done before starting the SDK via the
 `start` method.

 Config strings can be downloaded from the Chirp developer hub at
 https://developers.chirp.io.

 Note that this method also authenticates your application with Chirp's
 auth servers. For completely offline operation, please get in touch
 at support@chirp.io.

 @return nil if the configuration is set successfully, or an error
 @param  config A valid, non-nil config NSString
 */
- (NSError * _Nullable) setConfig:(NSString * _Nonnull) config;

//------------------------------------------------------------------------------

/**
 Set your default app config by fetching it from the Chirp REST API.
 This should typically only be used in cases in which you want your
 configuration to be generated dynamically -- for example, if you are
 rapidly prototyping different transmission settings with Chirp.

 @param completion A non-nil ChirpSetConfigFromNetworkBlock completion handler.
 */
- (void) setConfigFromNetworkWithCompletion:(ChirpSetConfigFromNetworkBlock _Nonnull) completion;

//------------------------------------------------------------------------------

/**
 Start the SDK running. This must be done before sending or receiving data
 using the SDK instance.

 @warning A config must be set before calling this method
 @return nil if the engine is started, otherwise an NSError.
 */
- (NSError * _Nullable) start;

//------------------------------------------------------------------------------

/**
 Start the SDK running in Send, Receive or SendAndReceive mode. In Send mode,
 microphone permissions will not be required. Input audio will not be processed
 in Send mode, likewise output audio will not be processed in Receive mode.
 
 @warning A config must be set before calling this method
 @param mode ChirpAudioModeSend, ChirpAudioModeReceive or ChirpAudioModeSendAndReceive mode
 @return nil if the engine is started, otherwise an NSError.
 */
- (NSError * _Nullable) startInMode:(ChirpAudioMode)mode;

//------------------------------------------------------------------------------

/**
 Stop the SDK running.

 @return nil if the SDK has started successfully, otherwise an NSError.
 */
- (NSError * _Nullable) stop;

//------------------------------------------------------------------------------

/**
 Stop the SDK running.

 @param completion A block called when the SDK has fully finished processing and
 stopped all audio IO.
 */
- (void) stopWithCompletion:(ChirpStoppedBlock _Nonnull) completion;

//------------------------------------------------------------------------------

/**
 Set whether or not to mix playing audio with Chirp data (such as the Music app).
 Note that by default the SDK will route all audio to the device's speaker
 on start.
 Defaults to NO.
 @warning This must be set before calling `start`.
 */
@property (nonatomic, assign) BOOL shouldMixAudio;

/**
 Override current audio settings to force audio to external peripherals
 as opposed to the device's speaker.
 Defaults to NO.
 */
@property (nonatomic, assign) BOOL routeAudioToExternalPeripherals;

/**
 Deactivate the audio session when stopping the SDK. This should be set
 to NO if the shared AVAudioSession is in use elsewhere in your app.
 Defaults to YES
 */
@property (nonatomic, assign) BOOL shouldDeactivateAudioSession;

//------------------------------------------------------------------------------
#pragma mark - Methods: Send/Receive
//------------------------------------------------------------------------------

/**
 Send data. The data parameter must be a non-nil NSData instance that is valid
 for the currently configured SDK. The data instance's validity for sending
 can be checked using the isValidPayload: instance method.

 @param  data The data to be sent
 @return CHIRP_CONNECT_OK will start sending the data.
 */
- (NSError * _Nullable) send:(NSData *_Nonnull) data;


//------------------------------------------------------------------------------
#pragma mark - Blocks
//------------------------------------------------------------------------------

/**
 A block called when sending starts.
 It is passed the NSData being sent and the NSUInteger channel the data is
 sent on.
 */
@property (nonatomic, copy) ChirpSendingBlock _Nullable sendingBlock;

//------------------------------------------------------------------------------

/**
 A block called when sending ends.
 It is passed the NSData that was sent and the NSUInteger channel the data was
 sent on.

 */
@property (nonatomic, copy) ChirpSentBlock _Nullable sentBlock;

//------------------------------------------------------------------------------

/**
 A block called when receiving starts.
 It is passed the NSUInteger channel the data is being received on.
 */
@property (nonatomic, copy) ChirpReceivingBlock _Nullable receivingBlock;

//------------------------------------------------------------------------------

/**
 A block called when receiving ends.
 It is passed the received NSData and the NSUInteger channel the data was
 received on, or nil if the decode operation has failed.
 */
@property (nonatomic, copy) ChirpReceivedBlock _Nullable receivedBlock;

//------------------------------------------------------------------------------

/**
 A block called when sending/receiving state changes.
 It is passed the old and new states.
 */
@property (nonatomic, copy) ChirpStateUpdatedBlock _Nullable stateUpdatedBlock;

//------------------------------------------------------------------------------

/**
 A block called when *system* volume changes, and when the SDK is started via
 the start: method. This will be a value between 0.0f and 1.0f, or -1.0f in an
 error condition.
 */
@property (nonatomic, copy) ChirpVolumeChangedBlock _Nullable systemVolumeChangedBlock;

//------------------------------------------------------------------------------

/**
 A block called when audio samples are ready.
 */
@property (nonatomic, copy) ChirpAudioBufferUpdatedBlock _Nullable audioBufferUpdatedBlock;

//------------------------------------------------------------------------------

/**
 A block called when the SDK's authentication state is updated after
 initialisation, passed the nullable config dictionary and error.
 */
@property (nonatomic, copy) ChirpAuthenticatedBlock _Nullable authenticatedBlock;

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#pragma mark - Methods: Payload Handling
//------------------------------------------------------------------------------

/**
 The duration in seconds for a specified payload length in bytes.
 */
- (NSTimeInterval) durationForPayloadLength:(NSUInteger)length;

//------------------------------------------------------------------------------

/**
 Check whether a non-nil NSData instance is valid and able to be sent using the
 Connect SDK as it is currently configured via its config file.

 @param  data A non-nil NSData instance
 @return YES is the data instance is valid for sending
 */
- (BOOL) isValidPayload:(NSData *_Nonnull)data;

//------------------------------------------------------------------------------

/**
 Generate random data which is guaranteed to be valid for the Connect SDK's
 current config.

 @param length the length of the payload to generate in bytes.
 @return NSData A non-nil, valid NSData instance with random bytes.
 */
- (NSData * _Nonnull)randomPayloadWithLength:(NSUInteger)length;

//------------------------------------------------------------------------------

/**
 Generate random data with random length which is guaranteed to be valid for the Connect SDK's
 current config.
 Similar to running the randomPayloadWithLength:0

 @return NSData A non-nil, valid NSData instance with random bytes.
 */
- (NSData * _Nonnull)randomPayloadWithRandomLength;

//------------------------------------------------------------------------------

/**
 Set the random seed used by the SDK. This is useful if you want to control
 the sequence of random payloads generated by `randomData`.

 @param  seed A positive integer.
 */
- (void) setRandomSeed:(NSUInteger) seed;

@end

