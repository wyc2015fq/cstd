
#ifndef _EAGLEEYE2_RMVA2XXSC_VIEWER_H_
#define _EAGLEEYE2_RMVA2XXSC_VIEWER_H_

#define WM_RECVTRGIMAGEOK  WM_USER + 5  //message variable for notice view after recv data
#define WM_SWICHNEWCAMERA  WM_USER + 6  //message variable for notice Mainframe after switch new camera

#define IMGWIDTH  1616        //horizontal resolution of image
#define  IMGHEIGHT  1232        //vertical resolution of image
#define IMGLENGTH  IMGWIDTH*IMGHEIGHT  //resolution of image

#define NUM_BUFF  5          //buffer number for motion jpeg 
#define  MAX_BUFF  IMGWIDTH*IMGHEIGHT*2//Size of each buffer loading MJpeg

//////////////////////////////////////////////////////////////////////////
//socket ports
#define COMMANDPORT    35000        //向相机发送命令的网络线程端口
#define TRGIMGPORT     45000        //触发模式下与相机进行数据交换的网络线程端口
#define FCONIMGPORT    55000        //全分辨率连续模式下与相机进行数据交换的网络线程端口
#define HFRIMGPORT     65000        //高祯率模式下与相机进行数据交换的网络线程端口
#define HFRBINIMGPORT  60000        //高祯率高灵敏度模式下与相机进行数据交换的网络线程端口
#define IMGINFLEN    sizeof(EagleEye2ImgInfStruct)//抓拍信息长度

//struct of buffer node for loading MJPEG
typedef struct tag_BuffNode {
  int          sizeBuff;
  char        pBuff[MAX_BUFF];
  BOOL        bIsLocked;
} BUFFNODE;
//抓拍信息
typedef struct ImgInfStruct {
  char  cLocusInf[101];      //抓拍地点信息，限50个汉字
  BYTE  byTimeInf[7];      //抓拍时间，年-月-日-时-分-秒
  BYTE  byWayNum;        //车道号
  BYTE  byTrgMode;        //触发方式，0->软触发;1->单线圈;2->双线圈(测速)
  float  fSpeed;          //车速(单位:公里/小时)
} EagleEye2ImgInfStruct;

//相机传感器驱动模式,图像采集格式和白平衡光照环境的枚举变量声明
enum EagleEye2_SensorRunMode { TrgMode, FconMode, HfrMode, HfrBinMode };
enum EagleEye2_ImgAcquisition_Format { RGB_ARRAY, YUV_ARRAY, BAYER_ARRAY };
enum EagleEye2_WhiteBalance_environment { DAYLIGHT, FLUORESCENCE, WITHOUTWB };

//struct for PIO
typedef struct PIO_indicator {
  unsigned pio1 : 1;
  unsigned pio2 : 1;
  unsigned pio3 : 1;
  unsigned pio4 : 1;
  unsigned pio5 : 1;
  unsigned pio6 : 1;
  unsigned pio7 : 1;
  unsigned pio8 : 1;
  unsigned pio9 : 1;
  unsigned pio10 : 1;
  unsigned pio1outvalue : 1;
  unsigned pio2outvalue : 1;
  unsigned pio3outvalue : 1;
  unsigned pio4outvalue : 1;
  unsigned pio5outvalue : 1;
  unsigned pio6outvalue : 1;
  unsigned pio7outvalue : 1;
  unsigned pio8outvalue: 1;
  unsigned pio9outvalue : 1;
  unsigned pio10outvalue : 1;
  unsigned reserved : 12;
} PIO_indicator;

typedef struct RTCConfig {
  UINT  m_iDay;
  UINT  m_iHour;
  UINT  m_iMinute;
  UINT  m_iMonth;
  UINT  m_iSecond;
  UINT  m_iWeek;
  UINT  m_iYear;
} RTCConfig;

//////////////////////////////////////////////////////////////////////////
typedef struct Eagleeye2_RMVA2xxSC_Viewer {
  img_t*          m_fPic;        //FCImage类对象
  img_t*          m_fPic1;      //FCImage类对象
  BOOL            m_bFirstShow;    //视图是否是第一次显示
  volatile HWND    m_hViewWnd;      //视图的窗口句柄，用来向视图类发送消息
  volatile HWND    m_hMainFrameWnd;  //主框架的窗口句柄，用来向主框架类发送消息
  BOOL            m_bIsDataFromCamera;//当前图像是否来自于相机
  BOOL            m_bIsJpeg;      //标示当前图像是否是JPG格式
  BOOL            m_bIsHighFR;    //标志当前需要显示的图像是否来自高祯率模式，以此区别于其它两种模式，用来在OnDraw函数中进行显示控制

  //一些通用控制变量
  BOOL          m_bIsTrgMode;        //是否是触发模式
  BOOL          m_bIsFconMode;       //是否是全分辨率连续模式
  BOOL          m_bIsHFRMode;        //是否是高祯率模式
  BOOL          m_bIsHFRBinMode;     //是否是高祯率高灵敏度模式
  BOOL          m_bIsEnsureTgtIP;    //是否确认目标相机的IP地址
  BOOL          m_bRelayIsOpen;      //继电器是否断开
  BOOL          m_bLUTEnable;        //相机是否使能图像的LUT映射变换
  BOOL          m_bAutoTrgEnable;    //切换到触发模式后是否允许自动触发抓取一祯图像
  BOOL          m_bIsRGBArray;       //相机采集的图像格式是否为RGB格式
  BOOL          m_bIsYUVArray;       //相机采集的图像格式是否为YUV格式
  BOOL          m_bIsBAYERArray;     //相机采集的图像格式是否为BAYER格式
  BOOL          m_bTrgShutterIsManual;  //触发模式快门速度是否是手动模式

  BYTE  m_byIPAdd1, m_byIPAdd2, m_byIPAdd3, m_byIPAdd4;


  BOOL  m_bPort10OUTValue;
  BOOL  m_bPort1OUTValue;
  BOOL  m_bPort2OUTValue;
  BOOL  m_bPort3OUTValue;
  BOOL  m_bPort4OUTValue;
  BOOL  m_bPort5OUTValue;
  BOOL  m_bPort6OUTValue;
  BOOL  m_bPort7OUTValue;
  BOOL  m_bPort8OUTValue;
  BOOL  m_bPort9OUTValue;

  BOOL  m_bIsPort1IN;
  BOOL  m_bIsPort2IN;
  BOOL  m_bIsPort3IN;
  BOOL  m_bIsPort4IN;
  BOOL  m_bIsPort5IN;
  BOOL  m_bIsPort6IN;
  BOOL  m_bIsPort7IN;
  BOOL  m_bIsPort8IN;
  BOOL  m_bIsPort9IN;
  BOOL  m_bIsPort10IN;
  BOOL  m_bIsFlashSynEnable;
  struct PIO_indicator  m_pioIndicator;

  BOOL  m_bIsPortDirAdj;
  BOOL  m_bIsPortOutAdj;
  BOOL  m_bIsFlashSynAdj;

  UINT  m_uiWBType; // Daylight | Fluorescence | Without WhiteBalance
  UINT  m_iWDT; // WD Countor:
  BOOL  m_bIsWDTEnable; // Enable Watch Dog

  float m_fAfeVGAGain; // CCD Signal PGA Config (0.f, 36.f)


  //抓拍信息
  char  m_strLocation[256]; // 抓拍地点
  float m_fInterval; // 线圈间距:
  int   m_iWayIndex; // 车 道 号:
  BOOL  m_bIsSingalCoin; // 单线圈触发
  BOOL  m_bIsDoubleCoin; // 双线圈触发(测速)

  // 相机参数设置
  BOOL  m_bIsShutPgaManualCtrl; // 快门、增益手动控制
  UINT  m_uiTrgShutterTime; // 快门(uS):
  float m_fTrgPga; // 增益(dB):

  BOOL  m_bIsShutPgaAutoCtrl; // 快门、增益自动控制
  int   m_iShutLimit; // 快门上限(uS):
  float m_fPGALimit; // 增益上限(dB):
  int   m_iCurShut; // 当前快门(uS):
  float m_fCurPga; // 当前增益(dB):
  // 测光区域定义:
  int   m_iSRow; // 起始行:
  int   m_iSCol; // 起始列:
  int   m_iERow; // 终止行:
  int   m_iECol; // 终止列:
  BYTE  m_byYMean; // 测光区域期望平均亮度:
  BOOL  m_bEnableADLI; // 使能非触发自测光
  int   m_iADLIP; // 自测光触发周期(秒):

  BOOL  m_bIsManualCtrl;
  BOOL  m_bSaveParam;

} Eagleeye2_RMVA2xxSC_Viewer;

#endif // _EAGLEEYE2_RMVA2XXSC_VIEWER_H_
