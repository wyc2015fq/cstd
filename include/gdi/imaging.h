
#ifndef __GDI_IMAGING_H__
#define __GDI_IMAGING_H__

//---------------------------------------------------------------------------
// Image file format identifiers
//---------------------------------------------------------------------------

enum ImageFormat{

ImageFormatUndefined, 
ImageFormatMemoryBMP, 
ImageFormatBMP, 
ImageFormatEMF, 
ImageFormatWMF, 
ImageFormatJPEG, 
ImageFormatPNG, 
ImageFormatGIF, 
ImageFormatTIFF, 
ImageFormatEXIF, 
ImageFormatIcon, 
};

//---------------------------------------------------------------------------
// Predefined multi-frame dimension IDs
//---------------------------------------------------------------------------
enum FrameDimension {
FrameDimensionTime, 
FrameDimensionResolution, 
FrameDimensionPage, 
};

//---------------------------------------------------------------------------
// Property sets
//---------------------------------------------------------------------------

enum FormatID {
FormatIDImageInformation, 
FormatIDJpegAppHeaders, 
};
//---------------------------------------------------------------------------
// Encoder parameter sets
//---------------------------------------------------------------------------

enum EncoderParameterSets {
EncoderCompression, 
EncoderColorDepth, 
EncoderScanMethod, 
EncoderVersion, 
EncoderRenderMethod, 
EncoderQuality, 
EncoderTransformation,
EncoderLuminanceTable,
EncoderChrominanceTable,
EncoderSaveFlag,
EncoderColorSpace,
EncoderImageItems,
EncoderSaveAsCMYK,
CodecIImageBytes,
};


//--------------------------------------------------------------------------
// ImageCodecInfo structure
//--------------------------------------------------------------------------

struct ImageCodecInfo
{
	FormatID  FormatID;
    const WCHAR* CodecName;
    const WCHAR* DllName;
    const WCHAR* FormatDescription;
    const WCHAR* FilenameExtension;
    const WCHAR* MimeType;
    DWORD Flags;
    DWORD Version;
    DWORD SigCount;
    DWORD SigSize;
    const BYTE* SigPattern;
    const BYTE* SigMask;
};

//--------------------------------------------------------------------------
// Information flags about image codecs
//--------------------------------------------------------------------------

enum ImageCodecFlags
{
    ImageCodecFlagsEncoder            = 0x00000001,
    ImageCodecFlagsDecoder            = 0x00000002,
    ImageCodecFlagsSupportBitmap      = 0x00000004,
    ImageCodecFlagsSupportVector      = 0x00000008,
    ImageCodecFlagsSeekableEncode     = 0x00000010,
    ImageCodecFlagsBlockingDecode     = 0x00000020,

    ImageCodecFlagsBuiltin            = 0x00010000,
    ImageCodecFlagsSystem             = 0x00020000,
    ImageCodecFlagsUser               = 0x00040000
};

//---------------------------------------------------------------------------
// Access modes used when calling Image::LockBits
//---------------------------------------------------------------------------

enum ImageLockMode
{
    ImageLockModeRead        = 0x0001,
    ImageLockModeWrite       = 0x0002,
    ImageLockModeUserInputBuf= 0x0004
};

//---------------------------------------------------------------------------
// Information about image pixel data
//---------------------------------------------------------------------------

struct BitmapData
{
    UINT Width;
    UINT Height;
    INT Stride;
    PixelFormat PixelFormat;
    VOID* Scan0;
    UINT_PTR Reserved;
};

//---------------------------------------------------------------------------
// Image flags
//---------------------------------------------------------------------------

enum ImageFlags
{
    ImageFlagsNone                = 0,

    // Low-word: shared with SINKFLAG_x

    ImageFlagsScalable            = 0x0001,
    ImageFlagsHasAlpha            = 0x0002,
    ImageFlagsHasTranslucent      = 0x0004,
    ImageFlagsPartiallyScalable   = 0x0008,

    // Low-word: color space definition

    ImageFlagsColorSpaceRGB       = 0x0010,
    ImageFlagsColorSpaceCMYK      = 0x0020,
    ImageFlagsColorSpaceGRAY      = 0x0040,
    ImageFlagsColorSpaceYCBCR     = 0x0080,
    ImageFlagsColorSpaceYCCK      = 0x0100,
 
    // Low-word: image size info

    ImageFlagsHasRealDPI          = 0x1000,
    ImageFlagsHasRealPixelSize    = 0x2000,

    // High-word

    ImageFlagsReadOnly            = 0x00010000,
    ImageFlagsCaching             = 0x00020000
};

enum RotateFlipType
{
    RotateNoneFlipNone = 0,
    Rotate90FlipNone   = 1,
    Rotate180FlipNone  = 2,
    Rotate270FlipNone  = 3,

    RotateNoneFlipX    = 4,
    Rotate90FlipX      = 5,
    Rotate180FlipX     = 6,
    Rotate270FlipX     = 7,

    RotateNoneFlipY    = Rotate180FlipX,
    Rotate90FlipY      = Rotate270FlipX,
    Rotate180FlipY     = RotateNoneFlipX,
    Rotate270FlipY     = Rotate90FlipX,

    RotateNoneFlipXY   = Rotate180FlipNone,
    Rotate90FlipXY     = Rotate270FlipNone,
    Rotate180FlipXY    = RotateNoneFlipNone,
    Rotate270FlipXY    = Rotate90FlipNone
};

//---------------------------------------------------------------------------
// Encoder Parameter structure
//---------------------------------------------------------------------------
class EncoderParameter
{
    ULONG   NumberOfValues;     // Number of the parameter values
    ULONG   Type;               // Value type, like ValueTypeLONG  etc.
    VOID*   Value;              // A pointer to the parameter values
};

//---------------------------------------------------------------------------
// Encoder Parameters structure
//---------------------------------------------------------------------------
class EncoderParameters
{
public:
    UINT Count;                      // Number of parameters in this structure
    EncoderParameter Parameter[1];   // Parameter values
};

#if (GDIPVER >= 0x0110)
enum ItemDataPosition
{
    ItemDataPositionAfterHeader    = 0x0,
    ItemDataPositionAfterPalette   = 0x1,
    ItemDataPositionAfterBits      = 0x2,
};

//---------------------------------------------------------------------------
// External Data Item
//---------------------------------------------------------------------------
class ImageItemData
{
public:
    UINT  Size;           // size of the structure 
    UINT  Position;       // flags describing how the data is to be used.
    VOID *Desc;           // description on how the data is to be saved.
                          // it is different for every codec type.
    UINT  DescSize;       // size memory pointed by Desc
    VOID *Data;           // pointer to the data that is to be saved in the
                          // file, could be anything saved directly.
    UINT  DataSize;       // size memory pointed by Data
    UINT  Cookie;         // opaque for the apps data member used during
                          // enumeration of image data items.
};
#endif //(GDIPVER >= 0x0110)

//---------------------------------------------------------------------------
// Property Item
//---------------------------------------------------------------------------
class PropertyItem
{
public:
    PROPID  id;                 // ID of this property
    ULONG   length;             // Length of the property value, in bytes
    WORD    type;               // Type of the value, as one of TAG_TYPE_XXX
                                // defined above
    VOID*   value;              // property value
};

//---------------------------------------------------------------------------
// Image property types 
//---------------------------------------------------------------------------
#define PropertyTagTypeByte        1
#define PropertyTagTypeASCII       2
#define PropertyTagTypeShort       3
#define PropertyTagTypeLong        4
#define PropertyTagTypeRational    5
#define PropertyTagTypeUndefined   7
#define PropertyTagTypeSLONG       9
#define PropertyTagTypeSRational  10

//---------------------------------------------------------------------------
// Image property ID tags
//---------------------------------------------------------------------------

#define PropertyTagExifIFD             0x8769
#define PropertyTagGpsIFD              0x8825

#define PropertyTagNewSubfileType      0x00FE
#define PropertyTagSubfileType         0x00FF
#define PropertyTagImageWidth          0x0100
#define PropertyTagImageHeight         0x0101
#define PropertyTagBitsPerSample       0x0102
#define PropertyTagCompression         0x0103
#define PropertyTagPhotometricInterp   0x0106
#define PropertyTagThreshHolding       0x0107
#define PropertyTagCellWidth           0x0108
#define PropertyTagCellHeight          0x0109
#define PropertyTagFillOrder           0x010A
#define PropertyTagDocumentName        0x010D
#define PropertyTagImageDescription    0x010E
#define PropertyTagEquipMake           0x010F
#define PropertyTagEquipModel          0x0110
#define PropertyTagStripOffsets        0x0111
#define PropertyTagOrientation         0x0112
#define PropertyTagSamplesPerPixel     0x0115
#define PropertyTagRowsPerStrip        0x0116
#define PropertyTagStripBytesCount     0x0117
#define PropertyTagMinSampleValue      0x0118
#define PropertyTagMaxSampleValue      0x0119
#define PropertyTagXResolution         0x011A   // Image resolution in width direction
#define PropertyTagYResolution         0x011B   // Image resolution in height direction
#define PropertyTagPlanarConfig        0x011C   // Image data arrangement
#define PropertyTagPageName            0x011D
#define PropertyTagXPosition           0x011E
#define PropertyTagYPosition           0x011F
#define PropertyTagFreeOffset          0x0120
#define PropertyTagFreeByteCounts      0x0121
#define PropertyTagGrayResponseUnit    0x0122
#define PropertyTagGrayResponseCurve   0x0123
#define PropertyTagT4Option            0x0124
#define PropertyTagT6Option            0x0125
#define PropertyTagResolutionUnit      0x0128   // Unit of X and Y resolution
#define PropertyTagPageNumber          0x0129
#define PropertyTagTransferFuncition   0x012D
#define PropertyTagSoftwareUsed        0x0131
#define PropertyTagDateTime            0x0132
#define PropertyTagArtist              0x013B
#define PropertyTagHostComputer        0x013C
#define PropertyTagPredictor           0x013D
#define PropertyTagWhitePoint          0x013E
#define PropertyTagPrimaryChromaticities 0x013F
#define PropertyTagColorMap            0x0140
#define PropertyTagHalftoneHints       0x0141
#define PropertyTagTileWidth           0x0142
#define PropertyTagTileLength          0x0143
#define PropertyTagTileOffset          0x0144
#define PropertyTagTileByteCounts      0x0145
#define PropertyTagInkSet              0x014C
#define PropertyTagInkNames            0x014D
#define PropertyTagNumberOfInks        0x014E
#define PropertyTagDotRange            0x0150
#define PropertyTagTargetPrinter       0x0151
#define PropertyTagExtraSamples        0x0152
#define PropertyTagSampleFormat        0x0153
#define PropertyTagSMinSampleValue     0x0154
#define PropertyTagSMaxSampleValue     0x0155
#define PropertyTagTransferRange       0x0156

#define PropertyTagJPEGProc            0x0200
#define PropertyTagJPEGInterFormat     0x0201
#define PropertyTagJPEGInterLength     0x0202
#define PropertyTagJPEGRestartInterval 0x0203
#define PropertyTagJPEGLosslessPredictors  0x0205
#define PropertyTagJPEGPointTransforms     0x0206
#define PropertyTagJPEGQTables         0x0207
#define PropertyTagJPEGDCTables        0x0208
#define PropertyTagJPEGACTables        0x0209

#define PropertyTagYCbCrCoefficients   0x0211
#define PropertyTagYCbCrSubsampling    0x0212
#define PropertyTagYCbCrPositioning    0x0213
#define PropertyTagREFBlackWhite       0x0214

#define PropertyTagICCProfile          0x8773   // This TAG is defined by ICC
                                                // for embedded ICC in TIFF
#define PropertyTagGamma               0x0301
#define PropertyTagICCProfileDescriptor 0x0302
#define PropertyTagSRGBRenderingIntent 0x0303

#define PropertyTagImageTitle          0x0320
#define PropertyTagCopyright           0x8298

// Extra TAGs (Like Adobe Image Information tags etc.)

#define PropertyTagResolutionXUnit           0x5001
#define PropertyTagResolutionYUnit           0x5002
#define PropertyTagResolutionXLengthUnit     0x5003
#define PropertyTagResolutionYLengthUnit     0x5004
#define PropertyTagPrintFlags                0x5005
#define PropertyTagPrintFlagsVersion         0x5006
#define PropertyTagPrintFlagsCrop            0x5007
#define PropertyTagPrintFlagsBleedWidth      0x5008
#define PropertyTagPrintFlagsBleedWidthScale 0x5009
#define PropertyTagHalftoneLPI               0x500A
#define PropertyTagHalftoneLPIUnit           0x500B
#define PropertyTagHalftoneDegree            0x500C
#define PropertyTagHalftoneShape             0x500D
#define PropertyTagHalftoneMisc              0x500E
#define PropertyTagHalftoneScreen            0x500F
#define PropertyTagJPEGQuality               0x5010
#define PropertyTagGridSize                  0x5011
#define PropertyTagThumbnailFormat           0x5012  // 1 = JPEG, 0 = RAW RGB
#define PropertyTagThumbnailWidth            0x5013
#define PropertyTagThumbnailHeight           0x5014
#define PropertyTagThumbnailColorDepth       0x5015
#define PropertyTagThumbnailPlanes           0x5016
#define PropertyTagThumbnailRawBytes         0x5017
#define PropertyTagThumbnailSize             0x5018
#define PropertyTagThumbnailCompressedSize   0x5019
#define PropertyTagColorTransferFunction     0x501A
#define PropertyTagThumbnailData             0x501B// RAW thumbnail bits in
                                                   // JPEG format or RGB format
                                                   // depends on
                                                   // PropertyTagThumbnailFormat

// Thumbnail related TAGs
                                                
#define PropertyTagThumbnailImageWidth       0x5020  // Thumbnail width
#define PropertyTagThumbnailImageHeight      0x5021  // Thumbnail height
#define PropertyTagThumbnailBitsPerSample    0x5022  // Number of bits per
                                                     // component
#define PropertyTagThumbnailCompression      0x5023  // Compression Scheme
#define PropertyTagThumbnailPhotometricInterp 0x5024 // Pixel composition
#define PropertyTagThumbnailImageDescription 0x5025  // Image Tile
#define PropertyTagThumbnailEquipMake        0x5026  // Manufacturer of Image
                                                     // Input equipment
#define PropertyTagThumbnailEquipModel       0x5027  // Model of Image input
                                                     // equipment
#define PropertyTagThumbnailStripOffsets     0x5028  // Image data location
#define PropertyTagThumbnailOrientation      0x5029  // Orientation of image
#define PropertyTagThumbnailSamplesPerPixel  0x502A  // Number of components
#define PropertyTagThumbnailRowsPerStrip     0x502B  // Number of rows per strip
#define PropertyTagThumbnailStripBytesCount  0x502C  // Bytes per compressed
                                                     // strip
#define PropertyTagThumbnailResolutionX      0x502D  // Resolution in width
                                                     // direction
#define PropertyTagThumbnailResolutionY      0x502E  // Resolution in height
                                                     // direction
#define PropertyTagThumbnailPlanarConfig     0x502F  // Image data arrangement
#define PropertyTagThumbnailResolutionUnit   0x5030  // Unit of X and Y
                                                     // Resolution
#define PropertyTagThumbnailTransferFunction 0x5031  // Transfer function
#define PropertyTagThumbnailSoftwareUsed     0x5032  // Software used
#define PropertyTagThumbnailDateTime         0x5033  // File change date and
                                                     // time
#define PropertyTagThumbnailArtist           0x5034  // Person who created the
                                                     // image
#define PropertyTagThumbnailWhitePoint       0x5035  // White point chromaticity
#define PropertyTagThumbnailPrimaryChromaticities 0x5036 
                                                     // Chromaticities of
                                                     // primaries
#define PropertyTagThumbnailYCbCrCoefficients 0x5037 // Color space transforma-
                                                     // tion coefficients
#define PropertyTagThumbnailYCbCrSubsampling 0x5038  // Subsampling ratio of Y
                                                     // to C
#define PropertyTagThumbnailYCbCrPositioning 0x5039  // Y and C position
#define PropertyTagThumbnailRefBlackWhite    0x503A  // Pair of black and white
                                                     // reference values
#define PropertyTagThumbnailCopyRight        0x503B  // CopyRight holder

#define PropertyTagLuminanceTable            0x5090
#define PropertyTagChrominanceTable          0x5091

#define PropertyTagFrameDelay                0x5100
#define PropertyTagLoopCount                 0x5101

#if (GDIPVER >= 0x0110)
#define PropertyTagGlobalPalette             0x5102
#define PropertyTagIndexBackground           0x5103
#define PropertyTagIndexTransparent          0x5104
#endif //(GDIPVER >= 0x0110)

#define PropertyTagPixelUnit         0x5110  // Unit specifier for pixel/unit
#define PropertyTagPixelPerUnitX     0x5111  // Pixels per unit in X
#define PropertyTagPixelPerUnitY     0x5112  // Pixels per unit in Y
#define PropertyTagPaletteHistogram  0x5113  // Palette histogram

// EXIF specific tag

#define PropertyTagExifExposureTime  0x829A
#define PropertyTagExifFNumber       0x829D

#define PropertyTagExifExposureProg  0x8822
#define PropertyTagExifSpectralSense 0x8824
#define PropertyTagExifISOSpeed      0x8827
#define PropertyTagExifOECF          0x8828

#define PropertyTagExifVer            0x9000
#define PropertyTagExifDTOrig         0x9003 // Date & time of original
#define PropertyTagExifDTDigitized    0x9004 // Date & time of digital data generation

#define PropertyTagExifCompConfig     0x9101
#define PropertyTagExifCompBPP        0x9102

#define PropertyTagExifShutterSpeed   0x9201
#define PropertyTagExifAperture       0x9202
#define PropertyTagExifBrightness     0x9203
#define PropertyTagExifExposureBias   0x9204
#define PropertyTagExifMaxAperture    0x9205
#define PropertyTagExifSubjectDist    0x9206
#define PropertyTagExifMeteringMode   0x9207
#define PropertyTagExifLightSource    0x9208
#define PropertyTagExifFlash          0x9209
#define PropertyTagExifFocalLength    0x920A
#define PropertyTagExifSubjectArea    0x9214  // exif 2.2 Subject Area
#define PropertyTagExifMakerNote      0x927C
#define PropertyTagExifUserComment    0x9286
#define PropertyTagExifDTSubsec       0x9290  // Date & Time subseconds
#define PropertyTagExifDTOrigSS       0x9291  // Date & Time original subseconds
#define PropertyTagExifDTDigSS        0x9292  // Date & TIme digitized subseconds

#define PropertyTagExifFPXVer         0xA000
#define PropertyTagExifColorSpace     0xA001
#define PropertyTagExifPixXDim        0xA002
#define PropertyTagExifPixYDim        0xA003
#define PropertyTagExifRelatedWav     0xA004  // related sound file
#define PropertyTagExifInterop        0xA005
#define PropertyTagExifFlashEnergy    0xA20B
#define PropertyTagExifSpatialFR      0xA20C  // Spatial Frequency Response
#define PropertyTagExifFocalXRes      0xA20E  // Focal Plane X Resolution
#define PropertyTagExifFocalYRes      0xA20F  // Focal Plane Y Resolution
#define PropertyTagExifFocalResUnit   0xA210  // Focal Plane Resolution Unit
#define PropertyTagExifSubjectLoc     0xA214
#define PropertyTagExifExposureIndex  0xA215
#define PropertyTagExifSensingMethod  0xA217
#define PropertyTagExifFileSource     0xA300
#define PropertyTagExifSceneType      0xA301
#define PropertyTagExifCfaPattern     0xA302

// New EXIF 2.2 properties

#define PropertyTagExifCustomRendered           0xA401
#define PropertyTagExifExposureMode             0xA402
#define PropertyTagExifWhiteBalance             0xA403
#define PropertyTagExifDigitalZoomRatio         0xA404
#define PropertyTagExifFocalLengthIn35mmFilm    0xA405
#define PropertyTagExifSceneCaptureType         0xA406
#define PropertyTagExifGainControl              0xA407
#define PropertyTagExifContrast                 0xA408
#define PropertyTagExifSaturation               0xA409
#define PropertyTagExifSharpness                0xA40A
#define PropertyTagExifDeviceSettingDesc        0xA40B
#define PropertyTagExifSubjectDistanceRange     0xA40C
#define PropertyTagExifUniqueImageID            0xA420


#define PropertyTagGpsVer             0x0000
#define PropertyTagGpsLatitudeRef     0x0001
#define PropertyTagGpsLatitude        0x0002
#define PropertyTagGpsLongitudeRef    0x0003
#define PropertyTagGpsLongitude       0x0004
#define PropertyTagGpsAltitudeRef     0x0005
#define PropertyTagGpsAltitude        0x0006
#define PropertyTagGpsGpsTime         0x0007
#define PropertyTagGpsGpsSatellites   0x0008
#define PropertyTagGpsGpsStatus       0x0009
#define PropertyTagGpsGpsMeasureMode  0x00A
#define PropertyTagGpsGpsDop          0x000B  // Measurement precision
#define PropertyTagGpsSpeedRef        0x000C
#define PropertyTagGpsSpeed           0x000D
#define PropertyTagGpsTrackRef        0x000E
#define PropertyTagGpsTrack           0x000F
#define PropertyTagGpsImgDirRef       0x0010
#define PropertyTagGpsImgDir          0x0011
#define PropertyTagGpsMapDatum        0x0012
#define PropertyTagGpsDestLatRef      0x0013
#define PropertyTagGpsDestLat         0x0014
#define PropertyTagGpsDestLongRef     0x0015
#define PropertyTagGpsDestLong        0x0016
#define PropertyTagGpsDestBearRef     0x0017
#define PropertyTagGpsDestBear        0x0018
#define PropertyTagGpsDestDistRef     0x0019
#define PropertyTagGpsDestDist        0x001A
#define PropertyTagGpsProcessingMethod 0x001B
#define PropertyTagGpsAreaInformation 0x001C
#define PropertyTagGpsDate            0x001D
#define PropertyTagGpsDifferential    0x001E

#endif


