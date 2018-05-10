///  @file bta_filters.h
///  @version 2.1.0
///
///  @brief This header file contains the configuration structs for filters
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

#ifndef BTA_FILTERS_H_INCLUDED
#define BTA_FILTERS_H_INCLUDED


typedef void* BTA_FltConfig;
typedef void* BTA_FltHandle;


/// @brief  This enum defines which filter is to be instantiated
typedef enum BTA_FltType {
    BTA_FltTypePixelIntrpl,             ///< A pixel can be replaced by the average value of a set of pixels
    BTA_FltTypeMotionDetector,          ///< A sliding average is used to calculate the satic background. Motion is then filtered
    BTA_FltTypeMorphology,              ///< Basic morphology functions (dilation, erosion)
    BTA_FltTypeOrientation,             ///< Filter for rotating and flipping data
    BTA_FltTypeIntrinsicUndistort,      ///< Intrinsic lens parameters are used to undistort the data
    BTA_FltTypeAvgsequences,            ///< Averaging of multiple sequences into one frame
    BTA_FltTypeMath,                    ///< Basic mathematical operations
} BTA_FltType;



typedef struct BTA_FltPixelIntrplConfig {
    uint32_t **pxIndicesIn;
    uint16_t *pxIndicesInLens;
    uint32_t *pxIndicesOut;
    uint16_t pxCount;
} BTA_FltPixelIntrplConfig;



typedef struct BTA_FltIntrinsicUndistortConfig {
    //BTA_ChannelId channelToProcess;       //TBA
    //BTA_DataFormat dataFormatToProcess;   //TBA
    uint16_t xRes;          ///< this filter is meant for input data of this width (0: all sizes)
    uint16_t yRes;          ///< this filter is meant for input data of this height (0: all sizes)
    float cameraMatrix[9];  ///< 3x3 Matrix: camera matrix
    float distCoeffs[5];    ///< 5x1 Vector: distortion coefficients
} BTA_FltIntrinsicUndistortConfig;



typedef struct BTA_FltMotionDetectorConfig {
    uint16_t slafWindowLength;          ///< The background adaption is done by a sliding average
    uint8_t slafStride;                 ///< The sliding average can be configured to average every nth frame
    uint16_t threshold;                 ///< If the current pixel exceeds the background by this amount: activity
} BTA_FltMotionDetectorConfig;



typedef enum BTA_FltMorphologyType {
    BTA_FltMorphologyTypeDilation,
    BTA_FltMorphologyTypeErosion,
} BTA_FltMorphologyType;

typedef struct BTA_FltMorphologyConfig {
    BTA_FltMorphologyType morphologyType;
    uint8_t *mask;
    uint8_t xRes;
    uint8_t yRes;
    BTA_ChannelId channelToProcess;
} BTA_FltMorphologyConfig;



typedef enum BTA_FltOrientationType {
    BTA_FltOrientationTypeFlipHor,
    BTA_FltOrientationTypeFlipVer,
    BTA_FltOrientationTypeRotate,
} BTA_FltOrientationType;

typedef struct BTA_FltOrientationConfig {
    BTA_FltOrientationType orientationType;
    uint16_t degrees;
} BTA_FltOrientationConfig;



typedef struct BTA_FltAvgSequencesConfig {
    uint16_t averageWindowLength;       ///< The filter expects this many sequences to be averaged
    //BTA_ChannelId channel(s)ToProcess ///< The filter currently averages only Distance, X, Y, Z and ORs Flags
    //values not to average             ///< The filter currently ignores 0 pixels. if the pixel is invalid in half the sequences the whole average is set to 0
} BTA_FltAvgSequencesConfig;



typedef enum BTA_FltMathType {
    BTA_FltMathTypeMultFM1,
} BTA_FltMathType;

typedef struct BTA_FltMathConfig {
    BTA_FltMathType mathType;
    BTA_ChannelId channelToProcess;
    float *dataFM1;                      ///< Float Matrix #1
    uint16_t xResFM1;                    ///< Number of columns of FM1
    uint16_t yResFM1;                    ///< Number of rows of FM1
} BTA_FltMathConfig;



typedef struct BTA_FltBilateralConfig {
    BTA_ChannelId channelToProcess;
} BTA_FltBilateralConfig;


#endif
