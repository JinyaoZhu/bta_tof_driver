///  @file bta_frame.h
///  @version 2.1.0
///
///  @brief This header file contains enums and structs dor the representation of a BTA_Frame
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

#ifndef BTA_FRAME_H_INCLUDED
#define BTA_FRAME_H_INCLUDED

#include <bta_status.h>
#include <stdint.h>


///     @brief Enumerator with valid frame modes to be passed with BTAsetFrameMode
typedef enum BTA_FrameMode {
    BTA_FrameModeCurrentConfig = 0,    ///< The sensors settings are not changed and data is passed through (according to device's current calculation/image mode settings)
    BTA_FrameModeDistAmp,
    BTA_FrameModeDistAmpFlags,
    BTA_FrameModeXYZ,
    BTA_FrameModeXYZAmp,
    BTA_FrameModeDistAmpColor,
    BTA_FrameModeXYZAmpFlags,
    BTA_FrameModeRawPhases,
    BTA_FrameModeIntensities,
    BTA_FrameModeDistColor
} BTA_FrameMode;


///     @brief Enumerator with channel IDs. They allow the identification of the various channels in a BTA_Frame
typedef enum BTA_ChannelId {
    BTA_ChannelIdUnknown             = 0x0,
    BTA_ChannelIdDistance            = 0x1,
    BTA_ChannelIdAmplitude           = 0x2,
    BTA_ChannelIdX                   = 0x4,
    BTA_ChannelIdY                   = 0x8,
    BTA_ChannelIdZ                   = 0x10,
    BTA_ChannelIdConfidence          = 0x20,         ///< in percent
    BTA_ChannelIdFlags               = 0x40,         ///< additional pixel-wise sensor specific information
    BTA_ChannelIdPhase0              = 0x80,
    BTA_ChannelIdPhase90             = 0x100,
    BTA_ChannelIdPhase180            = 0x200,
    BTA_ChannelIdPhase270            = 0x400,
    BTA_ChannelIdTest                = 0x800,
    BTA_ChannelIdColor               = 0x1000,
    BTA_ChannelIdAmbient             = 0x2000,
    BTA_ChannelIdPhase               = 0x4000,
    BTA_ChannelIdGrayScale           = 0x8000
} BTA_ChannelId;


///     @brief Enumerator with data formats which allows the parsing of the data in BTA_Channel.
///            The lowbyte stands for width (number of bytes).
///            The highbyte stands for unsigned/signed/floating-point (continuous numbering).
typedef enum BTA_DataFormat {
    BTA_DataFormatUnknown         = 0x0,
    BTA_DataFormatUInt8           = 0x11,
    BTA_DataFormatUInt16          = 0x12,
    //BTA_DataFormatUInt24          = 0x13,
    BTA_DataFormatUInt32          = 0x14,
    BTA_DataFormatSInt8           = 0x21,
    BTA_DataFormatSInt16          = 0x22,
    //BTA_DataFormatSInt24          = 0x23,
    BTA_DataFormatSInt32          = 0x24,
    //BTA_DataFormatFloat8          = 0x31,
    //BTA_DataFormatFloat16         = 0x32,
    //BTA_DataFormatFloat24         = 0x33,
    BTA_DataFormatFloat32         = 0x34,
    BTA_DataFormatRgb565          = 0x42,
    BTA_DataFormatRgb24           = 0x43,
	BTA_DataFormatJpeg            = 0x50,
} BTA_DataFormat;


///     @brief Enumerator with units which allows the interpretation of the data in a channel
typedef enum BTA_Unit {
    BTA_UnitUnitLess = 0,
    BTA_UnitMeter,
    BTA_UnitCentimeter,
    BTA_UnitMillimeter,
    BTA_UnitPercent
} BTA_Unit;


///     @brief BTA_Channel holds a two-dimensional array of data  (A part of BTA_Frame)
typedef struct BTA_Channel {
    BTA_ChannelId id;                 ///< Type of data in this channel
    uint16_t xRes;                    ///< Number of columns
    uint16_t yRes;                    ///< Number of rows
    BTA_DataFormat dataFormat;        ///< The bytestream in data needs to be casted to this format
    BTA_Unit unit;                    ///< Informative, for easier interpretation of the data
    uint32_t integrationTime;         ///< Integration time at which the frame was captured in [us]
    uint32_t modulationFrequency;     ///< Modulation frequency at which the frame was captured in [Hz]
    uint8_t *data;                    ///< Pixels starting with upper left pixel. For nofBytesPerPixel bigger than 1 the first byte is the lowbyte
    uint32_t dataLen;                 ///< Length of the channel data in bytes (= xRes*yRes*bytesPerPixel)
} BTA_Channel;


///     @brief BTA_Frame holds all the data gathered from one frame (one or more channels)
typedef struct BTA_Frame {
    uint8_t firmwareVersionMajor;       ///< Firmware version major
    uint8_t firmwareVersionMinor;       ///< Firmware version minor
    uint8_t firmwareVersionNonFunc;     ///< Firmware version non functional
    float mainTemp;                     ///< Main-board/processor temperature sensor in degree Celcius
    float ledTemp;                      ///< Led-board temperature sensor in degree Celcius
    float genericTemp;                  ///< Additional Generic temperature sensor in degree Celcius
    uint32_t frameCounter;              ///< Consecutive numbering of frames
    uint32_t timeStamp;                 ///< Time-stamp at which the frame was captured (in microseconds)
    BTA_Channel **channels;             ///< Data containing ucNofChannels Channel structs
    uint8_t channelsLen;                ///< The number of channel pointer stored in channels
    uint8_t sequenceCounter;            ///< If multiple sequences were captured, they can be seperated by the sequence counter
} BTA_Frame;

#endif
