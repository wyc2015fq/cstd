/*
 *-----------------------------------------------------------------------------
 *       Name                      HVDef.h
 *       Purpose                   Constant definition
 *       Development Kit           Microsoft Win32 SDK, Visual C++ 6.00
 *       Author                    Dh-Image
 *       Date                      2007/11/07
 *       Version                   1.3.0.0
 *
 *       Description
 *           This header file defines constant and external struct type
 *
 *-----------------------------------------------------------------------------
 */

#if !defined(HV_HVDEF)         //
#define HV_HVDEF

/* By C++ language compiler */
#ifdef __cplusplus
extern "C" {
#endif

  typedef HANDLE HHV;


  typedef enum tagHVTYPE {
    HV1300UCTYPE = 0,
    HV2000UCTYPE = 1,
    HV1301UCTYPE = 2,
    HV2001UCTYPE = 3,
    HV3000UCTYPE = 4,
    HV1300UMTYPE = 5,
    HV1302UCTYPE = 6,
    HV2002UCTYPE = 7,
    HV3102UCTYPE = 8,
    HV1302UMTYPE = 9,
    HV1300FCTYPE = 10,
    HV2000FCTYPE = 11,
    HV3100FCTYPE = 12,
    HV1300FMTYPE = 13,
    HV1303UCTYPE = 14,
    HV2003UCTYPE = 15,
    HV3103UCTYPE = 16,
    HV1303UMTYPE = 17,
    SV1300FMTYPE = 18,
    SV1300FCTYPE = 19,
    SV1310FCTYPE = 20,
    SV1310FMTYPE = 21,
    SV1311FCTYPE = 22,
    SV1311FMTYPE = 23,
    SV400FCTYPE  = 24,
    SV400FMTYPE  = 25,
    DH1394FXTYPE = 26,
    SV1410FCTYPE = 27,
    SV1410FMTYPE = 28,
    SV1420FCTYPE = 29,
    SV1420FMTYPE = 30,
    SV2000FCTYPE = 31,
    SV2000FMTYPE = 32,
    SV1400FCTYPE = 33,
    SV1400FMTYPE = 34,
  } HVTYPE;


  typedef enum tagHV_RESOLUTION {
    RES_MODE0   = 0,
    RES_MODE1   = 1,
    RES_MODE2   = 2,
    RES_MODE3   = 3
  } HV_RESOLUTION;


  typedef enum tagHV_SNAP_MODE {
    CONTINUATION  = 0,
    TRIGGER     = 1,
    TRIGGER_EDGE  = 2,
    TRIGGER_LEVEL = 3
  } HV_SNAP_MODE;


  typedef enum tagHV_POLARITY {
    LOW  = 0,
    HIGH = 1
  } HV_POLARITY;


  typedef enum tagHV_SNAP_SPEED {
    NORMAL_SPEED  = 0,
    HIGH_SPEED      = 1
  } HV_SNAP_SPEED;


  typedef enum tagHV_CHANNEL {
    RED_CHANNEL     = 0x10,
    GREEN_CHANNEL1    = 0x11,
    GREEN_CHANNEL2    = 0x12,
    BLUE_CHANNEL    = 0x13
  } HV_CHANNEL;

  typedef enum tagHV_AGC_CONTROL {
    AGC_GAIN_RED_CHANNEL    = 0x10,
    AGC_GAIN_GREEN_CHANNEL1   = 0x11,
    AGC_GAIN_GREEN_CHANNEL2   = 0x12,
    AGC_GAIN_BLUE_CHANNEL   = 0x13,
    AGC_GAIN      = 0x2
  } HV_AGC_CONTROL;




  typedef enum tagHV_ADC_CONTROL {
    ADC_BITS                    = 0,
    ADC_BLKLEVEL_CAL        = 1,
    ADC_BLKLEVEL_CAL_REDCHANNEL   = 0x10,
    ADC_BLKLEVEL_CAL_GREENCHANNEL1  = 0x11,
    ADC_BLKLEVEL_CAL_GREENCHANNEL2  = 0x12,
    ADC_BLKLEVEL_CAL_BLUECHANNEL  = 0x13,

    ADC_BRIGHTNESS          = 0x2

  } HV_ADC_CONTROL;


  typedef enum tagHV_ADC_LEVEL {
    ADC_LEVEL0    = 0,
    ADC_LEVEL1    = 1,
    ADC_LEVEL2    = 2,
    ADC_LEVEL3    = 3
  } HV_ADC_LEVEL;


  typedef enum tagHV_AEC_CONTROL {
    AEC_EXPOSURE_TIME   = 1,

    AEC_SHUTTER_SPEED   = 2,

    AEC_SHUTTER_UNIT    = 3

  } HV_AEC_CONTROL;

  typedef enum tagSHUTTER_UNIT {
    SHUTTER_US    = 0,
    SHUTTER_MS    = 1,
    SHUTTER_S   = 2
  } SHUTTER_UNIT_VALUE;


  typedef struct tagHV_AEC_CONTROL_CONTEXT {
    HV_AEC_CONTROL ctrl;
  } HV_AEC_CONTROL_CONTEXT, PHV_AEC_CONTROL_CONTEXT;



  typedef enum tagHV_MIRROR_DIRECTION {
    VERT_DIR  = 1
  } HV_MIRROR_DIRECTION;


  typedef enum tagHV_DEVICE_INFO {
    DESC_DEVICE_TYPE       = 0,
    DESC_RESOLUTION        = 1,
    DESC_DEVICE_MARK       = 2,
    DESC_DEVICE_SERIESNUM      = 3,
    DESC_DEVICE_BLANKSIZE        = 4,
    DESC_DEVICE_CHIPID           = 5,
    DESC_DEVICE_HARDWARE_VERSION = 6,
    DESC_DEVICE_NAME             = 7,
    DESC_DEVICE_SOFTWARE_VERSION = 8,
    DESC_DEVICE_FPGA_VERSION     = 9,
    DESC_DEVICE_VENDOR_NAME      = 10,
    DESC_DEVICE_BAYER_LAYOUT     = 11,
  } HV_DEVICE_INFO;



  typedef enum tagHV_PERIDEV_CONTROL {
    PERIDEV_BICOLOR_LAMP1 = 0,
    PERIDEV_BICOLOR_LAMP2 = 1,
    PERIDEV_IR_EMITTER    = 2,
    PERIDEV_LED       = 3,
    PERIDEV_ARRIVE_CLEAR  = 4
  } HV_PERIDEV_CONTROL;




  typedef enum tagHV_COMMAND_CODE {

    CMD_RESERVED0       = 0x00,

    CMD_RESERVED1       = 0x01,


    CMD_GET_BYTE_PER_PACKET   = 0x10,

    CMD_SET_BYTE_PER_PACKET   = 0x11,

    CMD_FRAME_SHOT        = 0x12,

    CMD_FRAME_SHOT_FINISH   = 0x13,

    CMD_GET_LUT_DWENTRIES   = 0x14,

    CMD_SET_LUT_DWENTRIES   = 0x15,
    CMD_SET_STROBE_ON           = 0x16,
    CMD_SET_STROBE_OFF          = 0x17,

    CMD_SPEC_FUN_INTERFACE1     =  0x80,
    CMD_1394_READ_BLOCK     =  0x21,


  } HV_COMMAND_CODE;

  typedef struct tagHV_CMD_PACKET_SIZE {

    DWORD dwSize;

  } HV_CMD_PACKET_SIZE;


  typedef struct tagHV_CMD_LUT_DWENTRIES {

    DWORD dwOffset;

    DWORD* pBuffer;

    DWORD dwEntries;

  } HV_CMD_LUT_DWENTRIES;


  typedef enum tagHVSTATUS {
    STATUS_OK             = 0,
    STATUS_NO_DEVICE_FOUND        = -1,
    STATUS_DEVICE_HANDLE_INVALID    = -2,
    STATUS_HW_DEVICE_TYPE_ERROR     = -3,
    STATUS_HW_INIT_ERROR        = -4,
    STATUS_HW_RESET_ERROR       = -5,
    STATUS_NOT_ENOUGH_SYSTEM_MEMORY   = -6,
    STATUS_HW_IO_ERROR          = -7,
    STATUS_HW_IO_TIMEOUT        = -8,
    STATUS_HW_ACCESS_ERROR        = -9,
    ////////////////////////////////////////////
    STATUS_OPEN_DRIVER_FAILED     = -10,
    STATUS_NOT_SUPPORT_INTERFACE    = -11,
    STATUS_PARAMETER_INVALID      = -12,
    STATUS_PARAMETER_OUT_OF_BOUND   = -13,
    STATUS_IN_WORK            = -14,
    STATUS_NOT_OPEN_SNAP        = -15,
    STATUS_NOT_START_SNAP       = -16,
    STATUS_FILE_CREATE_ERROR      = -17,
    STATUS_FILE_INVALID         = -18,
    STATUS_NOT_START_SNAP_INT     = -19,
    STATUS_INTERNAL_ERROR       = -20
  } HVSTATUS;


#define HV_SUCCESS(status) ( ((status) == STATUS_OK))

#ifdef _DEBUG
#define HV_VERIFY(status) \
  if((status) != STATUS_OK){ \
    ::MessageBox(NULL, HVGetErrorString(status), \
        "Error", MB_ICONWARNING | MB_OK);   \
  }
#else
#define HV_VERIFY(status) (status)
#endif


#define HV_MESSAGE(status) \
  if((status) != STATUS_OK){ \
    ::MessageBox(NULL, HVGetErrorString(status), \
        "Error message", MB_ICONWARNING | MB_OK);   \
  }



  typedef struct tagHV_SANP_INFO {
    HHV hhv;
    int nDevice;
    int nIndex;
    void* pParam;
  } HV_SNAP_INFO;


  typedef int (CALLBACK* HV_SNAP_PROC)(HV_SNAP_INFO* pInfo);


  typedef struct tagHV_CHECK_PARAM {
    BYTE byParam1;
    BYTE byParam2;
    BYTE byParam3;
    BYTE byParam4;
  } HV_CHECK_PARAM;

  typedef enum tagHV_INTERFACE1_ID {
    STROBE_ON_OFF     = 1,
    TRIGGER_DELAY_ON_OFF  = 2,
    TRIGGER_DELAY_VALUE   = 3,
    COLOR_MODE        = 4,
    OUTPUT_IO_0       = 5,
    OUTPUT_IO_1       = 6,
    OUTPUT_IO_2       = 7,
    OUTPUT_IO_0_POL     = 8,
    OUTPUT_IO_1_POL     = 9,
    OUTPUT_IO_2_POL     = 10,
    TRIGGER_SOURCE_IO   = 11,
    TEST_IMAGE              = 12,
    TRANSFERS_DELAY_VALUE   = 13,
  } HV_INTERFACE1_ID;

  typedef struct tagHV_INTERFACE1_CONTEXT {

    HV_INTERFACE1_ID  ID;
    DWORD       dwVal;

  } HV_INTERFACE1_CONTEXT;

  typedef enum tagCOLOR_CODE {
    MONO8    = 0,
    YUV8_411 = 1,
    YUV8_422 = 2,
    YUV8_444 = 3,
    RGB8     = 4,
    MONO16   = 5,
    RGB16    = 6
  } COLOR_CODE;


  /* extern "C" { */
#ifdef __cplusplus
}
#endif

#endif

