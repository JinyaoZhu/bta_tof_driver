///  @file bta.h
///  @version 2.1.0
///
///  @brief The main header for BltTofApi. Includes all interface functions, the config struct and
///         the declaration of the config struct organisation
///
///  Copyright Bluetechnix 2016
///
///  @author Alex Falkensteiner
///
///  @cond svn
///
///  Information of last commit
///  $Rev::               $:  Revision of last commit
///  $Author::            $:  Author of last commit
///  $Date::              $:  Date of last commit
///
///  @endcond
///

#ifndef BTA_H_INCLUDED
#define BTA_H_INCLUDED


#if (defined(WIN32) || defined(WIN64)) && !defined(PLAT_WINDOWS) && !defined(PLAT_LINUX)
#define PLAT_WINDOWS
#endif
#if defined(linux) && !defined(PLAT_WINDOWS) && !defined(PLAT_LINUX)
#define PLAT_LINUX
#endif


#ifdef PLAT_WINDOWS
#define BTA_CALLCONV __stdcall
#ifdef COMPILING_DLL
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT __declspec(dllimport)
#endif
#elif defined PLAT_LINUX
//must be empty
#define DLLEXPORT
//must be empty
#define BTA_CALLCONV
#else
#error "Please define PLAT_WINDOWS or PLAT_LINUX in your makefile/project"
#endif


///     @brief  The handle to hold the instance created by BTAopen
typedef void* BTA_Handle;

#include <bta_status.h>
#include <bta_frame.h>
#include <bta_filters.h>
#include <bta_discovery.h>
#include <bta_flash_update.h>


///     @brief  Callback function to report on informative events.
///             The implementation of this function in the application must copy the relevant data and return immediately.
///             The parameter 'verbosity' in BTA_Config can be used to turn on/off certain events.
///     @param  status The status describing the reason for the infoEvent
///     @param  msg A string containing the information for the user
typedef void (BTA_CALLCONV *FN_BTA_InfoEvent)(BTA_Status status, int8_t *msg);


///     @brief  Callback function to report on informative events.
///             The implementation of this function in the application must copy the relevant data and return immediately.
///             The parameter 'verbosity' in BTA_Config can be used to turn on/off certain events.
///     @param  handle The handle as identification for the device
///     @param  status The status describing the reason for the infoEvent
///     @param  msg A string containing the information for the user
typedef void (BTA_CALLCONV *FN_BTA_InfoEventEx)(BTA_Handle handle, BTA_Status status, int8_t *msg);


///     @brief  Callback function to report on data frames from the sensor.
///             The implementation of this function in the application must copy the relevant data and return immediately.
///             The BTA_Frame may NOT be altered!
///             Do not call BTAfreeFrame on frame, because it is free'd in the lib.
///     @param  frame A pointer to the structure containing the data frame
typedef void (BTA_CALLCONV *FN_BTA_FrameArrived)(BTA_Frame *frame);


///     @brief  Callback function to report on data frames from the sensor.
///             The implementation of this function in the application must copy the relevant data and return immediately.
///             The BTA_Frame may NOT be altered!
///             Do not call BTAfreeFrame on frame, because it is free'd in the lib.
///     @param  handle The handle as identification for the device
///     @param  frame A pointer to the structure containing the data frame
typedef void (BTA_CALLCONV *FN_BTA_FrameArrivedEx)(BTA_Handle handle, BTA_Frame *frame);


///     @brief  Callback function to report status and progress during transfer and programming
///     @param  status Please refer to bta_status.h
///     @param  percentage Contains the progress in [%].
///                          0: File transfer started (can only be reported once per file transfer).
///                        100: File transfer finished (can only be reported once per file transfer).
typedef int (BTA_CALLCONV *FN_BTA_ProgressReport)(BTA_Status status, uint8_t percentage);





///     @brief  Enumerator with queuing modes
typedef enum BTA_QueueMode {
    BTA_QueueModeDoNotQueue = 0,                  ///< No queueing
    BTA_QueueModeDropOldest = 1,                  ///< Before an overflow, the oldest item in the queue is removed
    BTA_QueueModeDropCurrent = 2,                  ///< When full, the queue remains unchanged
    BTA_QueueModeAvoidDrop = 3                   ///< When full, the equeue function returns an error to the producer (do not use in BTAopen()!)
} BTA_QueueMode;



///     @brief  Enumerator with runtime configuration parameters for the library.
///             The behaviour of the library can be changed by setting various LibParams.
///             Some information about the state of the library can be retrieved by reading various LibParams.
///             Never are they affecting the devices configuration (registers).
typedef enum BTA_LibParam {
    BTA_LibParamKeepAliveMsgInterval,           ///< The interval in seconds. If no communication during this time, a keep alive is sent. (Supported only by Ethernet cameras)
    BTA_LibParamCrcControlEnabled,              ///< Set > 0 in order to activate CRC sums for the control connection. (Supported only by Ethernet cameras)

    BTA_LibParamStreamTotalFrameCount,          ///< Readonly. Contains the total amount of frames loaded from bltstream file
    BTA_LibParamStreamAutoPlaybackSpeed,        ///< Set > 0 in order to activate playback at recording rate times this factor. Set to 0 to pause playback.
    BTA_LibParamStreamPos,                      ///< Get and set the index in the bltstream file. The first frame has index 0. If set, BTA_LibParamStreamAutoPlaybackSpeed is set to 0
    BTA_LibParamStreamPosIncrement,             ///< Writeonly. Set the increment to which position to jump to in the bltstream file. If set, BTA_LibParamStreamAutoPlaybackSpeed is set to 0

    BTA_LibParamEnableTestPattern               ///< Set > 0 in order to override camera data with test data.
} BTA_LibParam;



///     @brief  The BTA_Config shall be 8-byte aligned
#define BTA_CONFIG_STRUCT_STRIDE 8
#ifdef PLAT_WINDOWS
#define BTA_PRAGMA_ALIGN __declspec(align(BTA_CONFIG_STRUCT_STRIDE))
#elif defined PLAT_LINUX
#define BTA_PRAGMA_ALIGN __attribute__((aligned(BTA_CONFIG_STRUCT_STRIDE)))
#endif
///     @brief  Configuration structure to be passed with BTAopen
typedef struct BTA_Config {
    BTA_PRAGMA_ALIGN uint8_t *udpDataIpAddr;                ///< The IP address for the UDP data interface (The address the device is configured to stream to)
    BTA_PRAGMA_ALIGN uint8_t udpDataIpAddrLen;              ///< The length of udpDataIpAddr buffer in [byte]
    BTA_PRAGMA_ALIGN uint16_t udpDataPort;                  ///< The port for the UDP data interface (The port the device is configured to stream to)
    BTA_PRAGMA_ALIGN uint8_t *udpControlOutIpAddr;          ///< The IP address for the UDP control interface (outbound connection) (The address to send the command to, where the device awaits commands at)
    BTA_PRAGMA_ALIGN uint8_t udpControlOutIpAddrLen;        ///< The length of udpControlOutIpAddr buffer in [byte]
    BTA_PRAGMA_ALIGN uint16_t udpControlOutPort;            ///< The port for the UDP control interface (outbound connection) (The address to send the command to, where the device awaits commands at)
    BTA_PRAGMA_ALIGN uint8_t *udpControlInIpAddr;           ///< The IP address for the UDP control interface (inbound connection) (The address the device should answer to, usually the local IP address)
    BTA_PRAGMA_ALIGN uint8_t udpControlInIpAddrLen;         ///< The length of udpControlInIpAddr buffer in [byte]
    BTA_PRAGMA_ALIGN uint16_t udpControlInPort;             ///< The port for the UDP control interface (inbound connection) (The port the device should answer to)
    BTA_PRAGMA_ALIGN uint8_t *tcpDeviceIpAddr;              ///< The IP address for the TCP data and control interface (The device's IP address)
    BTA_PRAGMA_ALIGN uint8_t tcpDeviceIpAddrLen;            ///< The length of tcpDeviceIpAddr buffer in [byte]
    BTA_PRAGMA_ALIGN uint16_t tcpDataPort;                  ///< The port for the TCP data interface (The port the device sends data to)
    BTA_PRAGMA_ALIGN uint16_t tcpControlPort;               ///< The port for the TCP control interface (The port the device awaits commands at)

    BTA_PRAGMA_ALIGN uint8_t *uartPortName;                 ///< The port name of the UART to use (ASCII coded)
    BTA_PRAGMA_ALIGN uint32_t uartBaudRate;                 ///< The UART baud rate
    BTA_PRAGMA_ALIGN uint8_t uartDataBits;                  ///< The number of UART data bits used
    BTA_PRAGMA_ALIGN uint8_t uartStopBits;                  ///< 0: None, 1: One, 2: Two, 3: 1.5 stop bits
    BTA_PRAGMA_ALIGN uint8_t uartParity;                    ///< 0: None, 1: Odd, 2: Even, 3: Mark, 4: Space Parity
    BTA_PRAGMA_ALIGN uint8_t uartTransmitterAddress;        ///< The source address for UART communications
    BTA_PRAGMA_ALIGN uint8_t uartReceiverAddress;           ///< The target address for UART communications

    BTA_PRAGMA_ALIGN BTA_DeviceType deviceType;             ///< The device type, when not left 0 implies the type of connection to use (Ethernet, USB (P100), UART, Bltstream, ...)
    BTA_PRAGMA_ALIGN uint8_t *pon;                          ///< Product Order Number of device to be opened (0 == not specified) (ASCII coded)
    BTA_PRAGMA_ALIGN uint32_t serialNumber;                 ///< Serial number of device to be opened (0 == not specified)

    BTA_PRAGMA_ALIGN uint8_t *calibFileName;                ///< Name of the lens calibration file to be loaded into SDK (ASCII coded). It is supported by P100 based cameras in order to generate XYZ channels
    BTA_PRAGMA_ALIGN uint8_t *zFactorsFileName;             ///< A filter of the type BTA_FltTypeMath (BTA_FltType) with math type BTA_FltMathTypeMultFM1 (BTA_FltMathType) is instantiated. The file behind zFactorsFileName is loaded as Matrix. And the filter is set to be applied for Z channels.
    BTA_PRAGMA_ALIGN uint8_t *wigglingFileName;             ///< No longer supported, please use BTAwigglingUpdate()

    BTA_PRAGMA_ALIGN BTA_FrameMode frameMode;               ///< Frame mode to be set in SDK/device

    BTA_PRAGMA_ALIGN FN_BTA_InfoEvent infoEvent;            ///< Callback function pointer to the function to be called upon an informative event (optional but handy for debugging/tracking) (deprecated, use infoEventEx)
    BTA_PRAGMA_ALIGN FN_BTA_InfoEventEx infoEventEx;        ///< Callback function pointer to the function to be called upon an informative event (optional but handy for debugging/tracking)
    BTA_PRAGMA_ALIGN uint8_t verbosity;                     ///< A value to tell the library when and when not to generate InfoEvents (0: Only critical events, 10: Maximum amount of events)
    BTA_PRAGMA_ALIGN FN_BTA_FrameArrived frameArrived;      ///< Callback function pointer to the function to be called when a frame is ready (optional) (deprecated, use frameArrivedEx)
    BTA_PRAGMA_ALIGN FN_BTA_FrameArrivedEx frameArrivedEx;  ///< Callback function pointer to the function to be called when a frame is ready (optional)

    BTA_PRAGMA_ALIGN uint16_t frameQueueLength;             ///< The library queues this amount of frames internally
    BTA_PRAGMA_ALIGN BTA_QueueMode frameQueueMode;          ///< The frame queue configuration parameter
    BTA_PRAGMA_ALIGN uint16_t averageWindowLength;          ///< A filter type BTA_FltTypeAvgsequences is instantiated. averageWindowLength sets the amount of frames to average. 0: do nothing, 1: turn averaging off (set 1 sequence), >1: averaging

    BTA_PRAGMA_ALIGN uint8_t *bltstreamFilename;            ///< Only for BtaStreamLib: Specify the file (containing the stream) to read from (ASCII coded)
} BTA_Config;



///     @brief  Configuration structure to be passed with BTAstartDiscovery
typedef struct BTA_GrabbingConfig {
    uint8_t *filename;                                      ///< The filename of the *.bltstream file. Where the grabbing process stores the stream. (ASCII coded)
} BTA_GrabbingConfig;



///     @brief  This struct is used for the representation of the BTA_Config struct.
///             Programming languages that don't use header files are able to query the elements of BTA_Config generically.
typedef struct BTA_ConfigStructOrg {
    char *variableName;                                     ///< The name of the field in the BTA_Config
    uint8_t pointer;                                        ///< 1 --> field is a pointer, 0 --> field is not a pointer
} BTA_ConfigStructOrg;



///     @brief  This struct contains information about the BTA_Config struct
DLLEXPORT extern BTA_ConfigStructOrg btaConfigStructOrg[];
///     @brief  The size of the struct containing information about the BTA_Config struct
DLLEXPORT extern const uint32_t btaConfigStructOrgLen;



//----------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C"
{
#endif


///     @brief  For querying API version
///     @param  verMaj If not null it points to the major firmware version of the device on return
///     @param  verMin If not null it points to the minor firmware version of the device on return
///     @param  verNonFun If not null it points to the non functional firmware version of the device on return
///     @param  buildDateTime A char array allocated by the caller containing the date/time string (ASCII) of build on return (can be left null)
///     @param  buildDateTimeLen Size of the preallocated buffer behind buildDateTime in [byte]
///     @param  supportedDeviceTypes Array allocated by the caller containing the codes of all devices supported by a specifiy BTA implementation on return (can be left null)
///     @param  supportedDeviceTypesLen Pointer to size of supportedDeviceTypes (the number of supported devices); on return it contains number of supported device types
///     @return Please refer to bta_status.h
DLLEXPORT BTA_Status BTA_CALLCONV BTAgetVersion(uint32_t *verMaj, uint32_t *verMin, uint32_t *verNonFun, uint8_t *buildDateTime, uint32_t buildDateTimeLen, uint16_t *supportedDeviceTypes, uint32_t *supportedDeviceTypesLen);



///     @brief  Fills the discovery config structure with standard values
///     @param  config Pointer to the structure to be initialized to standard values
///     @return Please refer to bta_status.h
DLLEXPORT BTA_Status BTA_CALLCONV BTAinitDiscoveryConfig(BTA_DiscoveryConfig *config);


///     @brief  Starts the discovery of devices.
///             If possible, broadcast messages are transmitted otherwise all possible connections are tested
///     @param  discoveryConfig Parameters on how to perform the discovery.
///                             The connection interface used defines which parameters have to be set in BTA_DiscoveryConfig.
///     @param  deviceFound     The callback to be invoked when a device has been discovered
///     @param  infoEvent       The callback to be invoked when an error occurs
///     @param  handle          On return it contains the discovery handle which has to be used to stop the background process.
///     @return                 Please refer to bta_status.h
DLLEXPORT BTA_Status BTA_CALLCONV BTAstartDiscovery(BTA_DiscoveryConfig *discoveryConfig, FN_BTA_DeviceFound deviceFound, FN_BTA_InfoEventEx infoEvent, BTA_Handle *handle);


///     @brief  Stops the discovery of devices
///     @param  handle  Pass the handle from startDiscovery in order to identify the ongoing discovery process
///     @return         Please refer to bta_status.h
DLLEXPORT BTA_Status BTA_CALLCONV BTAstopDiscovery(BTA_Handle *handle);



///     @brief  With this function a literal description of the config struct can be retrieved.
///             In programming languages which do not support header files it can be used to allocate and fill the BTA_Config struct.
///     @param  fieldCount The number of elements (config struct fields) in the result on return
///     @param  bytesPerField The number of bytes per element (config struct field) in the result on return
DLLEXPORT BTA_ConfigStructOrg *BTA_CALLCONV BTAgetConfigStructOrg(uint32_t *fieldCount, uint8_t *bytesPerField);


///     @brief  Fills the config structure with standard values
///     @param  config Pointer to the structure to be initialized to standard values
///     @return Please refer to bta_status.h
DLLEXPORT BTA_Status BTA_CALLCONV BTAinitConfig(BTA_Config *config);


///     @brief  Establishes a connection to the device and returns a handle
///     @param  config Pointer to the previously initialized config structure
///     @param  handle Pointer containing the handle to the device on return
///     @return Please refer to bta_status.h
DLLEXPORT BTA_Status BTA_CALLCONV BTAopen(BTA_Config *config, BTA_Handle *handle);


///     @brief Disconnects from the sensor and closes the handle
///     @param handle Pointer to the handle of the device to be closed; points to null on return
///     @return Please refer to bta_status.h
DLLEXPORT BTA_Status BTA_CALLCONV BTAclose(BTA_Handle *handle);


///     @brief  For querying information about the device.
///             If successful, BTAfreeDeviceInfo must be called afterwards.
///     @param  handle Handle of the device to be used
///     @param  deviceInfo Pointer to pointer to structure with information about the device on return
DLLEXPORT BTA_Status BTA_CALLCONV BTAgetDeviceInfo(BTA_Handle handle, BTA_DeviceInfo **deviceInfo);


///     @brief For freeing the device information structure
///     @param deviceInfo Pointer to structure to be freed
DLLEXPORT BTA_Status BTA_CALLCONV BTAfreeDeviceInfo(BTA_DeviceInfo *deviceInfo);


///     @brief  Queries whether the handle is open and background threads are running correctly
///     @param  handle Handle of the device to be used
///     @return 1 if the service is running, 0 otherwise
DLLEXPORT uint8_t BTA_CALLCONV BTAisRunning(BTA_Handle handle);


///     @brief  Queries whether the library has a valid connection to the sensor
///     @param  handle Handle of the device to be used
///     @return 1 if connected to the sensor, 0 otherwise
DLLEXPORT uint8_t BTA_CALLCONV BTAisConnected(BTA_Handle handle);


///     @brief  Allows to set a BTA_FrameMode which defines the data delivered by the sensor.
///             The device and/or the SDK is configured depending on the frame mode, so that the desired channels are included in each BTA_Frame.
///     @param  handle Handle of the device to be used
///     @param  frameMode The desired frame-mode
///     @return Please refer to bta_status.h
DLLEXPORT BTA_Status BTA_CALLCONV BTAsetFrameMode(BTA_Handle handle, BTA_FrameMode frameMode);




///     @brief  Helper function to clone/duplicate/deep-copy a BTA_Frame structure.
///             If successful, BTAfreeFrame must be called on frameDst afterwards.
///     @param  frameSrc The pointer to the frame to be copied
///     @param  frameDst The pointer to the new duplicated frame
///     @return Please refer to bta_status.h
DLLEXPORT BTA_Status BTA_CALLCONV BTAcloneFrame(BTA_Frame *frameSrc, BTA_Frame **frameDst);


///     @brief  Actively requests a frame.
///             The device-specific library implementation defines if
///              - it triggers the capturing of a new frame, or
///              - it returns a queued frame / it waits for the next frame.
///             If successful, BTAfreeFrame must be called afterwards.
///     @param  handle Handle of the device to be used
///     @param  frame Pointer to frame (null if failed) on return (needs to be freed with BTAfreeFrame)
///     @param  timeout Timeout to wait if no frame is yet available in [ms].
///                     If timeout == 0 the function waits endlessly for a frame from the device.
///     @return Please refer to bta_status.h
DLLEXPORT BTA_Status BTA_CALLCONV BTAgetFrame(BTA_Handle handle, BTA_Frame **frame, uint32_t timeout);


///     @brief  Helper function to free a BTA_Frame structure
///     @param  frame The pointer to the frame to be freed; points to null on return
///     @return Please refer to bta_status.h
DLLEXPORT BTA_Status BTA_CALLCONV BTAfreeFrame(BTA_Frame **frame);


///     @brief  Helper function to calculate the length of a frame in serialized form; needed for BTAserializeFrame
///     @param  frame The pointer to the frame to be serialized
///     @param  frameSerializedLen Pointer to the length of the (to be) serialized frame on return.
///     @return Please refer to bta_status.h
DLLEXPORT BTA_Status BTA_CALLCONV BTAgetSerializedLength(BTA_Frame *frame, uint32_t *frameSerializedLen);


///     @brief  Helper function to convert a BTA_Frame structure into a serialized stream; useful for recording frames to files
///     @param  frame The pointer to the frame to be serialized
///     @param  frameSerialized Buffer to contain the frame data on return. Must be allocated by the caller
///     @param  frameSerializedLen Pointer to length of the buffer frameSerialized allocated by the caller.
///                                Pointer to the actual length of the serialized frame on return.
///     @return Please refer to bta_status.h
DLLEXPORT BTA_Status BTA_CALLCONV BTAserializeFrame(BTA_Frame *frame, uint8_t *frameSerialized, uint32_t *frameSerializedLen);


///     @brief  Helper function to convert a serialized stream into a BTA_Frame structure; useful for replaying recorded frames from files
///     @param  frame Double pointer to the deserialized frame on return.
///                   Don't forget to call BTAfreeFrame.
///     @param  frameSerialized Buffer that contains the frame data
///     @param  frameSerializedLen Pointer to length of the buffer frameSerialized; number of bytes parsed on return
///     @return Please refer to bta_status.h
DLLEXPORT BTA_Status BTA_CALLCONV BTAdeserializeFrame(BTA_Frame **frame, uint8_t *frameSerialized, uint32_t *frameSerializedLen);




///     @brief  Convenience function for extracting distances from a provided frame.
///             It simply returns the pointer and copies some information. The same data can be accessed directly going through the BTA_Frame structure.
///             If there is no channel with distance data present in the frame, an error is returned.
///     @param  frame The frame from which to extract the data 
///     @param  distBuffer Pointer to the distances on return (null on error)
///     @param  dataFormat Pointer to the BTA_DataFormat, thus how to parse 'distBuffer'
///     @param  unit Pointer to BTA_Unit, thus how to interpret 'distBuffer'
///     @param  xRes Pointer to the number of columns of 'distBuffer'
///     @param  yRes Pointer to the number of rows of 'distBuffer'
///     @return Please refer to bta_status.h
DLLEXPORT BTA_Status BTA_CALLCONV BTAgetDistances(BTA_Frame *frame, void **distBuffer, BTA_DataFormat *dataFormat, BTA_Unit *unit, uint16_t *xRes, uint16_t *yRes);


///     @brief  Convenience function for extracting amplitudes from a provided frame.
///             It simply returns the pointer and copies some information. The same data can be accessed directly going through the BTA_Frame structure.
///             If there is no channel with amplitude data present in the frame, an error is returned.
///     @param  frame The frame from which to extract the data 
///     @param  ampBuffer Pointer to the amplitudes on return (null on error)
///     @param  dataFormat Pointer to the BTA_DataFormat, thus how to parse 'ampBuffer'
///     @param  unit Pointer to BTA_Unit, thus how to interpret 'ampBuffer'
///     @param  xRes Pointer to the number of columns of 'ampBuffer'
///     @param  yRes Pointer to the number of rows of 'ampBuffer'
///     @return Please refer to bta_status.h
DLLEXPORT BTA_Status BTA_CALLCONV BTAgetAmplitudes(BTA_Frame *frame, void **ampBuffer, BTA_DataFormat *dataFormat, BTA_Unit *unit, uint16_t *xRes, uint16_t *yRes);


///     @brief  Convenience function for extracting flags from a provided frame.
///             It simply returns the pointer and copies some information. The same data can be accessed directly going through the BTA_Frame structure.
///             If there is no channel with flag data present in the frame, an error is returned.
///     @param  frame The frame from which to extract the data 
///     @param  flagBuffer Pointer to the flags on return (null on error)
///     @param  dataFormat Pointer to the BTA_DataFormat, thus how to parse 'flagBuffer'
///     @param  unit Pointer to BTA_Unit, thus how to interpret 'flagBuffer'
///     @param  xRes Pointer to the number of columns of 'flagBuffer'
///     @param  yRes Pointer to the number of rows of 'flagBuffer'
///     @return Please refer to bta_status.h
DLLEXPORT BTA_Status BTA_CALLCONV BTAgetFlags(BTA_Frame *frame, void **flagBuffer, BTA_DataFormat *dataFormat, BTA_Unit *unit, uint16_t *xRes, uint16_t *yRes);


///     @brief  Convenience function for extracting the 3D-coordinates from a provided frame.
///             It simply returns the pointer and copies some information. The same data can be accessed directly going through the BTA_Frame structure.
///             If there are not 3 channels with coordinate data present in the sensor data, an error is returned.
///     @param  frame The frame from which to extract the data 
///     @param  xBuffer A pointer to the cartesian x coordinates on return (null on error)
///     @param  yBuffer A pointer to the cartesian y coordinates on return (null on error)
///     @param  zBuffer A pointer to the cartesian z coordinates on return (null on error)
///     @param  dataFormat Pointer to the BTA_DataFormat, thus how to parse 'xBuffer', 'yBuffer' and 'zBuffer'
///     @param  unit Pointer to BTA_Unit, thus how to interpret 'xBuffer', 'yBuffer' and 'zBuffer'
///     @param  xRes Pointer to the number of columns of 'xBuffer', 'yBuffer' and 'zBuffer'
///     @param  yRes Pointer to the number of rows of 'xBuffer', 'yBuffer' and 'zBuffer'
///     @return Please refer to bta_status.h
DLLEXPORT BTA_Status BTA_CALLCONV BTAgetXYZcoordinates(BTA_Frame *frame, void **xBuffer, void **yBuffer, void **zBuffer, BTA_DataFormat *dataFormat, BTA_Unit *unit, uint16_t *xRes, uint16_t *yRes);


///     @brief  Convenience function for extracting colors from a provided frame.
///             It simply returns the pointer and copies some information. The same data can be accessed directly going through the BTA_Frame structure.
///             If there is no channel with color data present in the frame, an error is returned.
///     @param  frame The frame from which to extract the data 
///     @param  colorBuffer Pointer to the colors on return (null on error)
///     @param  dataFormat Pointer to the BTA_DataFormat, thus how to parse 'colorBuffer'
///     @param  unit Pointer to BTA_Unit, thus how to interpret 'colorBuffer'
///     @param  xRes Pointer to the number of columns of 'colorBuffer'
///     @param  yRes Pointer to the number of rows of 'colorBuffer'
///     @return Please refer to bta_status.h
DLLEXPORT BTA_Status BTA_CALLCONV BTAgetColors(BTA_Frame *frame, void **colorBuffer, BTA_DataFormat *dataFormat, BTA_Unit *unit, uint16_t *xRes, uint16_t *yRes);




///     @brief  Facilitates setting the integration time for the default capture sequence(s)
///     @param  handle Handle of the device to be used
///     @param  integrationTime The desired integration time in [us]
///     @return Please refer to bta_status.h
DLLEXPORT BTA_Status BTA_CALLCONV BTAsetIntegrationTime(BTA_Handle handle, uint32_t integrationTime);


///     @brief  Facilitates the retrieval of the current integration time of the default capture sequence(s)
///     @param  handle Handle of the device to be used
///     @param  integrationTime Pointer containing the integration time in [us] on return
///     @return Please refer to bta_status.h
DLLEXPORT BTA_Status BTA_CALLCONV BTAgetIntegrationTime(BTA_Handle handle, uint32_t *integrationTime);


///     @brief Facilitates setting the frame rate for the default capture sequence
///     @param handle Handle of the device to be used
///     @param frameRate The desired frame rate in [Hz]
///     @return Please refer to bta_status.h
DLLEXPORT BTA_Status BTA_CALLCONV BTAsetFrameRate(BTA_Handle handle, float frameRate);


///     @brief  Facilitates the retrieval of the current theoretical frame rate of the default capture sequence(s)
///     @param  handle Handle of the device to be used
///     @param  frameRate Pointer containing the frame rate in [Hz] on return
///     @return Please refer to bta_status.h
DLLEXPORT BTA_Status BTA_CALLCONV BTAgetFrameRate(BTA_Handle handle, float *frameRate);


///     @brief  Facilitates setting the modulation frequency for the default capture sequence(s)
///     @param  handle Handle of the device to be used
///     @param  modulationFrequency The desired modulation frequency in [Hz]
///     @return Please refer to bta_status.h
DLLEXPORT BTA_Status BTA_CALLCONV BTAsetModulationFrequency(BTA_Handle handle, uint32_t modulationFrequency);


///     @brief  Facilitates the retrieval of the current theoretical frame rate of the default capture sequence(s)
///     @param  handle Handle of the device to be used
///     @param  modulationFrequency Pointer containing the modulation frequency in [Hz] on return
///     @return Please refer to bta_status.h
DLLEXPORT BTA_Status BTA_CALLCONV BTAgetModulationFrequency(BTA_Handle handle, uint32_t *modulationFrequency);


///     @brief  Function for setting the distance offset being applied to all pixels equally.
///             It is, for all current devices, valid for the currently set modulation frequency.
///             It can only be set for predefined modulation frequencies (see device’s SUM).
///     @param  handle Handle of the device to be used
///     @param  globalOffset offset in [mm]      
///     @return Please refer to bta_status.h
DLLEXPORT BTA_Status BTA_CALLCONV BTAsetGlobalOffset(BTA_Handle handle, float globalOffset);


///     @brief  Function for getting the distance offset being applied to all pixels equally.
///             It is, for all current devices, valid for the currently set modulation frequency.
///             When changing the modulation frequency, the global offset changes.
///     @param  handle Handle of the device to be used
///     @param  globalOffset Pointer to hold offset in mm      
///     @return Please refer to bta_status.h
DLLEXPORT BTA_Status BTA_CALLCONV BTAgetGlobalOffset(BTA_Handle handle, float *globalOffset);




///     @brief  Reads registers from the device/SDK
///     @param  handle Handle of the device to be used
///     @param  address The address of the first register to read from
///     @param  data Pointer to buffer allocated by the caller. Contains register data on return.
///                  The data in the buffer on return consists of one or more register values, each 4 bytes wide.
///     @param  registerCount Pointer to the number of registers to be read.
///                           On return, if not null, it contains the number of registers actually read.
///                           If null is passed, one register is read.
///     @return Please refer to bta_status.h
DLLEXPORT BTA_Status BTA_CALLCONV BTAreadRegister(BTA_Handle handle, uint32_t address, uint32_t *data, uint32_t *registerCount);


///     @brief Writes registers to the device/SDK
///     @param handle Handle of the device to be used
///     @param address The address of the first register to write to
///     @param data Pointer to buffer containing register data to be written.
///                 The data in the buffer consists of one or more register values, each 4 bytes wide.
///     @param registerCount Pointer which contains the number of registers to be written.
///                          On return, if not null, it contains the number of registers actually written.
///                          If null is passed, one register is written.
///     @return Please refer to bta_status.h
DLLEXPORT BTA_Status BTA_CALLCONV BTAwriteRegister(BTA_Handle handle, uint32_t address, uint32_t *data, uint32_t *registerCount);




///     @brief  Function for setting a parameter for the library. Library parameters do not directly affect the camera's configuration.
///     @param  handle      Handle of the device to be used
///     @param  libParam    Identifier for the parameter (consult the support wiki for a description of its function)
///     @param  value       The value to be set for the library parameter
///     @return             Please refer to bta_status.h
DLLEXPORT BTA_Status BTA_CALLCONV BTAsetLibParam(BTA_Handle handle, BTA_LibParam libParam, float value);


///     @brief  Function for getting a parameter from the library.
///     @param  handle      Handle of the device to be used
///     @param  libParam    Identifier for the parameter (consult the support wiki for a description of its function)
///     @param  value       On return it points to the value of the library parameter
///     @return             Please refer to bta_status.h
DLLEXPORT BTA_Status BTA_CALLCONV BTAgetLibParam(BTA_Handle handle, BTA_LibParam libParam, float *value);




///     @brief  Initiates a reset of the device
///     @param  handle Handle of the device to be used
///     @return Please refer to bta_status.h
DLLEXPORT BTA_Status BTA_CALLCONV BTAsendReset(BTA_Handle handle);




///     @brief  Allows sending large data to the device.
///             Mainly this is used for sending calibration data and firmware updates.
///             This function handles the complete transfer of the file and blocks during transmission.
///             If possible, this function performs its task in a way that allows the OS to
///             regain control periodically, i.e. it uses blocking functions in a fine granularity.
///             The callback function is called (if not null) in the following cases:
///             - An error occurs -> report error status ignoring percentage.
///             - An transmission starts -> report BTA_StatusOk with percentage 0%.
///             - An transmission ends -> report BTA_StatusOk with percentage 100%.
///             (--> so the callback will always be used at least once and at least twice in case of success).
///             During transmission, progress is reported repeatedly when possible (report BTA_StatusOk with percentage).
///     @param  handle Handle of the device to be used
///     @param  flashUpdateConfig Contains the data and all the necessary information for handling it
///     @param  progressReport Callback function for reporting the status and progress during transfer and programming
///     @return Please refer to bta_status.h
DLLEXPORT BTA_Status BTA_CALLCONV BTAflashUpdate(BTA_Handle handle, BTA_FlashUpdateConfig *flashUpdateConfig, FN_BTA_ProgressReport progressReport);


///     @brief  Convenience function for doing a firmware update. Uses BTAflashUpdate() internally
///     @param  handle Handle of the device to be used
///     @param  filename Name of the binary file
///     @param  progressReport Callback function for reporting the status and progress during transfer and programming. Can be null
///     @return Please refer to bta_status.h
DLLEXPORT BTA_Status BTA_CALLCONV BTAfirmwareUpdate(BTA_Handle handle, const uint8_t *filename, FN_BTA_ProgressReport progressReport);


///     @brief  Convenience function for doing a firmware update. Uses BTAflashUpdate() internally
///     @param  handle Handle of the device to be used
///     @param  filename Name of the binary file
///     @param  progressReport Callback function for reporting the status and progress during transfer and programming. Can be null
///     @return Please refer to bta_status.h
DLLEXPORT BTA_Status BTA_CALLCONV BTAwigglingUpdate(BTA_Handle handle, const uint8_t *filename, FN_BTA_ProgressReport progressReport);


///     @brief  Convenience function for doing a firmware update. Uses BTAflashUpdate() internally
///     @param  handle Handle of the device to be used
///     @param  filename Name of the binary file
///     @param  progressReport Callback function for reporting the status and progress during transfer and programming. Can be null
///     @return Please refer to bta_status.h
DLLEXPORT BTA_Status BTA_CALLCONV BTAfpnUpdate(BTA_Handle handle, const uint8_t *filename, FN_BTA_ProgressReport progressReport);


///     @brief  Convenience function for doing a firmware update. Uses BTAflashUpdate() internally
///     @param  handle Handle of the device to be used
///     @param  filename Name of the binary file
///     @param  progressReport Callback function for reporting the status and progress during transfer and programming. Can be null
///     @return Please refer to bta_status.h
DLLEXPORT BTA_Status BTA_CALLCONV BTAfppnUpdate(BTA_Handle handle, const uint8_t *filename, FN_BTA_ProgressReport progressReport);


///     @brief  Allows reading large data from the device.
///             This function handles the complete transfer of the file and blocks during transmission.
///             If possible, this function performs its task in a way that allows the OS to
///             regain control periodically, i.e. it uses blocking functions in a fine granularity.
///             The callback function is called (if not null) in the following cases:
///             - An error occurs -> report error status ignoring percentage.
///             - An transmission starts -> report BTA_StatusOk with percentage 0%.
///             - An transmission ends -> report BTA_StatusOk with percentage 100%.
///             (--> so the callback will always be used at least once and at least twice in case of success).
///             During transmission, progress is reported repeatedly when possible (report BTA_StatusOk with percentage).
///     @param  handle Handle of the device to be used
///     @param  flashUpdateConfig Must contain the flash target and flash id to read from and optionally the flash address.
///                               Must also have have the pointer data with preallocated space denoted in dataLen.
///                               On return it contains the data read and its actual length.
///     @param  progressReport Callback function for reporting the status and progress during transfer
///     @return Please refer to bta_status.h
DLLEXPORT BTA_Status BTA_CALLCONV BTAflashRead(BTA_Handle handle, BTA_FlashUpdateConfig *flashUpdateConfig, FN_BTA_ProgressReport progressReport);




///     @brief  Writes the current configuration (i.e. register settings) to non-volatile memory
///     @param  handle Handle of the device to be used
///     @return Please refer to bta_status.h
DLLEXPORT BTA_Status BTA_CALLCONV BTAwriteCurrentConfigToNvm(BTA_Handle handle);


///     @brief  Erases the register settings previously stored with BTAwriteCurrentConfigToNvm (May require rebbot for loading default values)
///     @param  handle Handle of the device to be used
///     @return Please refer to bta_status.h
DLLEXPORT BTA_Status BTA_CALLCONV BTArestoreDefaultConfig(BTA_Handle handle);




///     @brief  (The development of filter functionality is still in progress!) Instantiates the specified filter wih specified parameters and adds it at the end of the filter processing queue
///     @param  handle Handle of the device to be used
///     @param  fltConfig Pointer to the structure containing the parameters for the filter
///     @param  fltType Enum value denoting which filter implementation to add
///     @param  fltHandle The handle to hold the filter instance on return
///     @return Please refer to bta_status.h
DLLEXPORT BTA_Status BTA_CALLCONV BTAaddFilter(BTA_Handle handle, BTA_FltConfig *fltConfig, BTA_FltType fltType, BTA_FltHandle *fltHandle);


///     @brief  (The development of filter functionality is still in progress!) Removes the last filter added
///     @param  handle Handle of the device to be used
///     @param  fltHandle The handle of the filter to be removed
///     @return Please refer to bta_status.h
DLLEXPORT BTA_Status BTA_CALLCONV BTAremoveFilter(BTA_Handle handle, BTA_FltHandle fltHandle);




///     @brief  Fills the grab config structure with standard values
///     @param  config Pointer to the structure to be initialized to standard values
///     @return Please refer to bta_status.h
DLLEXPORT BTA_Status BTA_CALLCONV BTAinitGrabbingConfig(BTA_GrabbingConfig *config);


///     @brief  Issues the library to grab (or stop grabbing) all the frames and store them to the file provided in the config.
///             When connected to USB (P100) the capture thread must be running or the BTAgetFrame must be called frequently
///             in order for the library to grab frames.
///     @param  handle Handle of the device to be used.
///     @param  config Pointer to the config structure specifying parameters for the grabbing process.
///                    Pass null in order to stop grabbing.
///     @return Please refer to bta_status.h
DLLEXPORT BTA_Status BTA_CALLCONV BTAstartGrabbing(BTA_Handle handle, BTA_GrabbingConfig *config);




///     @brief  A convenience function to convert a BTA_Status into a string
///     @param status The BTA_Status to be converted into a string
///     @param statusString A buffer allocated by the caller to contain the result on return
///     @param statusStringLen The length of the preallocated buffer in statusString
///     @return Please refer to bta_status.h
DLLEXPORT BTA_Status BTA_CALLCONV BTAstatusToString(BTA_Status status, char* statusString, uint16_t statusStringLen);

///     @brief  Deprecated. Use BTAstatusToString
DLLEXPORT BTA_Status BTA_CALLCONV BTAeventIdToString(BTA_EventId eventId, char *eventIdString, uint16_t eventIdStringLen);



///     @brief  DEPRECATED  Please use LibParam BTA_LibParamKeepAliveMsgInterval instead
DLLEXPORT BTA_Status BTA_CALLCONV BTAsetKeepAliveMsgInterval(BTA_Handle handle, float interval);
///     @brief  DEPRECATED  Please use LibParam BTA_LibParamCrcControlEnabled instead
DLLEXPORT BTA_Status BTA_CALLCONV BTAsetControlCrcEnabled(BTA_Handle handle, uint8_t enabled);


#ifdef __cplusplus
}
#endif

#endif
