///  @file bta_status.h
///  @version 2.1.0
///
///  @brief This header file contains the status ID enum used as return value for most functions
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

#ifndef BTA_STATUS_H_INCLUDED
#define BTA_STATUS_H_INCLUDED


///   @brief Error code for error handling
typedef enum BTA_Status {
    BTA_StatusOk = 0,                       ///< Everything went ok

    // Errors
    BTA_StatusInvalidParameter = -32768,    ///< At least one parameter passed is invalid or out of valid range
                                            ///< The combination of parameters is contradictory or incomplete
                                            ///< The provided frame does not contain the channel(s) expected
    BTA_StatusIllegalOperation,             ///< The data requested by the user cannot be read / written because it does not exist or is not accessible in the current configuration / state
                                            ///< The modulation frequency to be set or currently configured is not supported
                                            ///< BTAclose was already called
    BTA_StatusTimeOut,                      ///< Within the waiting period a necessary condition was not met, so the operation had to be aborted
                                            ///< After trying repeatedly the operation did not succeed
    BTA_StatusDeviceUnreachable,            ///< The connection to the device could not be established
                                            ///< An error occurred during communication
                                            ///< The device with the specified attributes could not be found
    BTA_StatusNotConnected,                 ///< The operation cannot be executed because the connection is down
    BTA_StatusInvalidVersion,               ///< 
    BTA_StatusRuntimeError,                 ///< A system resource (mutex, semaphore, thread, file) could not be created / initialized / read / written
                                            ///< The ToF device did not react as expected
    BTA_StatusOutOfMemory,                  ///< A malloc, realloc or calloc failed to reserve the needed memory
                                            ///< The buffer provided by the caller is not large enough
                                            ///< The end of the file was reached
    BTA_StatusNotSupported,                 ///< The function is not supported by this device/firmware
                                            ///<
    BTA_StatusCrcError,                     ///< The cyclic redundancy check revealed that the data in question must be corrupt
    BTA_StatusUnknown,

    // These stati are used only in infoEvent callbacks, it is merely a placeholder rather than a state
    BTA_StatusInformation = 1,              ///< The infoEvent message contains the actual information
    BTA_StatusWarning,                      ///< The infoEvent message describes the cause of the warning
} BTA_Status;


/// @brief  Deprecated BTA_EventId now only represents a BTA_Status
typedef BTA_Status BTA_EventId;

#endif
