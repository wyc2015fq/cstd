/***************************************************************
SmartCombinExport   date: 06/03/2009
-------------------------------------------------------------
版本: 1.1.0.1
作者：饶友
说明：模块采用UNICODE编码，基于DSHOW实现
更新：
-------------------------------------------------------------
创合世纪 Copyright (C) 2009 - All Rights Reserved
****************************************************************/

#ifndef _VIDEO_CTRL_
#define _VIDEO_CTRL_

#ifdef  VIDEOCTRL_EXPORTS
#ifdef __cplusplus
#define VIDEOSPE_API extern "C" __declspec(dllexport)
#else
#define VIDEOSPE_API __declspec(dllexport)
#endif
#else
#ifdef __cplusplus
#define VIDEOSPE_API extern "C" __declspec(dllimport)
#else
#define VIDEOSPE_API __declspec(dllimport)
#endif
#endif
#define VIDEO_CALL __stdcall
#define AUTO_STATE 0xffff

/****************************************************数据类型和结构*****************************************************/

/*
函数功能：将视频流回调给外部
函数参数：
[
pImage:     像素地址
width:      图像宽
height:     图像高
step:       图像步长
channel:    像素通道
cardIndex:  当前摄像头的ID号
userData:   设置回调函数时用户提供的数据
]
函数返回：无
使用说明：系统在做任何处理之前，通过该回调函数将原始视频数据回调给外部应用程序
*/
typedef void (VIDEO_CALL* OutImageStreamCallBack)
(
    BYTE* pImage , int width , int height , int step , int channel ,
    int cardIndex , long userData
);

typedef struct _Camera_Pro {
  HWND   m_ShowWnd;                    /*视频显示窗口*/
  HWND   m_EventWnd;                   /*事件响应窗口*/
  WCHAR  m_DisplayName[1024];          /*采集卡的显示名字 , 指定的优先级高*/
  WCHAR  m_FriendlyName[1024];         /*采集卡的友好名字 ， 指定的优先级中*/
  BOOL   m_BeUse;                      /*设备是否被使用*/
  long   m_AreadyLive;                 /*设备的存活状体 -1不确定状态 ， 0 设备不存活 , 1设备存活*/
  long   m_CameraID;                   /*采集卡序号， 指定的优先级低*/
  long   m_PID;                        /*指定设备的PID*/
  long   m_VID;                        /*指定设备的VID*/
  long   m_CaptureW;                   /*采集宽 */
  long   m_CaptureH;                   /*采集高*/
  long   m_FramRate;                   /*帧率*/
  BOOL   m_Eddy;                       /*是否旋转*/
  BOOL   m_Repeat;                     /*是否奇偶场重合*/
  OutImageStreamCallBack m_CallBackFn; /*视频流回调函数*/
  long   m_UserData;                   /*用户数据*/
  long   m_AEWorAEBValue;                   /*传感器视频亮度*/
} Camera_Pro;

typedef enum _VideoProperty {
  /*图像属性*/
  Brightness  = 0,                    /*亮度*/
  Contrast  = 1,                /*对比度*/
  Hue         = 2,                /*色度*/
  Saturation  = 3,                /*饱和度*/
  Sharpness = 4,                /*锐度*/
  Gamma     = 5,
  ColorEnable = 6,                /*彩色\黑白*/
  WhiteBalance          = 7,      /*白平衡*/
  BacklightCompensation = 8,      /*背光补偿*/
  Gain                = 9,      /*增益*/
  /*摄像头控制*/
  Camera_Pan      = 10,
  Camera_Tilt     = 11,
  Camera_Roll     = 12,
  Camera_Zoom     = 13,
  Camera_Exposure = 14,           /*曝光*/
  Camera_Iris     = 15,           /*光圈*/
  Camera_Focus    = 16        /*聚焦*/
} VideoProperty;


typedef enum _VIDEO_STATE {
  ERR_STATE = 0,
  RUN_STATE,
  PAUSE_STATE,
  STOP_STATE
} VIDEO_STATE;

typedef void* HANDLE;                   /*视频对象句柄类型*/

typedef struct _MYRECT {
  RECT     m_Rect;                    /*矩形框*/
  COLORREF m_Color;                   /*颜色*/
} MYRECT;

typedef struct _CIRCLE {
  POINT  m_Center;                    /*中心点坐标*/
  long   m_radius;                    /*半径*/
  long   m_flag;                      /*保留无效*/
  COLORREF m_Color;                   /*颜色*/
} CIRCLE;


//一下结构体暂时保留未用
typedef struct _STRING {
  POINT        m_Start;               /*开始坐标*/
  long         m_FontSize;            /*字体大小*/
  COLORREF     m_Color;               /*颜色*/
  WCHAR        m_Str[64];             /*字符串UNICODE*/
} CHZZ_STRING;

typedef struct _CROSS {
  POINT        m_Center;              /*中心点*/
  long         m_radius;              /*半径*/
  COLORREF     m_Color;               /*颜色*/
} CROSS;

typedef enum CAMTYPE { NORMAL , PRIVATE } CAMTYPE;

/**********************************************************功能函数****************************************************/

/*
函数功能：获得系统视频设备数目，创建设备列表
函数参数：无
函数返回：系统摄像头数目
*/
VIDEOSPE_API int  VIDEO_CALL  CHC_EnumCameraDeviceInSystem(CAMTYPE type);

/*
函数功能：刷新系统设备列表
函数参数：无
函数返回：无
使用说明：根据系统当前实际视频设备情况刷新设备列表
*/
VIDEOSPE_API void  VIDEO_CALL CHC_UpdateCameraDeviceList(CAMTYPE type);

/*
函数功能：释放所有枚举的设备
函数参数：无
函数返回：无
使用说明：释放设备列表中视频对象，清空设备列表, 当视频设备正在使用时，该设备不会被清除，仅标记为无效
*/
VIDEOSPE_API void  VIDEO_CALL CHC_ClearDeviceByEnum(void);

/*
函数功能：得到设备属性
函数参数：
[
index: 列表下标，范围[ 0 ,  视频设备数目)
pro  : 返回设备的相关信息，结构对象由客户输入。
]
函数返回：成功 TRUE ， 失败 FLASE
*/
VIDEOSPE_API BOOL VIDEO_CALL  CHC_GetCameraDevicePro(int index , Camera_Pro* pro);

/*
函数功能：通过摄像机的名字从设备列表中删除设备。
函数参数：
[
deviceName: 设备的名字，可以是友好名字或者设备全名
]
函数返回：成功 TRUE ， 失败 FLASE
使用说明：如果视频正在使用，仅标记其无效，当视频设备停止使用时才将其从设备列表中删除。
*/
VIDEOSPE_API BOOL VIDEO_CALL  CHC_ReleaseDeviceByName(BSTR deviceName);

/*
函数功能：创建视频控制对象
函数参数：无
函数返回：视频源对象句柄 ， 失败返回NULL
使用说明：返回的视频源对象可以作为识别模块的图像源对象
*/
VIDEOSPE_API HANDLE VIDEO_CALL CHC_CreateVideoCtrlObjEx(CAMTYPE type);

/*
函数功能：创建视频控制对象
函数参数：无
函数返回：视频源对象句柄 ， 失败返回NULL
使用说明：返回的视频源对象可以作为识别模块的图像源对象
*/
VIDEOSPE_API HANDLE VIDEO_CALL CHC_CreateVideoCtrlObj(void);

/*
函数功能：释放有视频源对象
函数参数：
[
pVideoCtrl: CHC_CreateVideoCtrlObj返回的视频源对象
]
函数返回：无
使用说明：释放之前确保视频源对象已经不再需要，所有依赖的它的关系已经解除。
*/
VIDEOSPE_API void VIDEO_CALL  CHC_FreeVideoCtrlObj(HANDLE pVideoCtrl);

/*
函数功能：设置视频相关控制属性
函数参数：
[
pVideoCtrl：视频源对象
pro： 属性结构体
]
函数返回：无
使用说明：必须在启动视频之前设置才能生效。摄像头枚举规则：
*/
VIDEOSPE_API void VIDEO_CALL  CHC_SetCtrlPro(HANDLE pVideoCtrl , Camera_Pro* pro);

/*
函数功能：得到视频相关控制属性
函数参数：pro 属性结构体
函数参数：
[
pVideoCtrl：视频源对象
pro： 属性结构体
]
函数返回：无
*/
VIDEOSPE_API void VIDEO_CALL  CHC_GetCtrlPro(HANDLE pVideoCtrl ,  Camera_Pro* pro);

/*
函数功能：启动视频流
函数参数：
[
pVideoCtrl：视频源对象
]
函数返回：成功TRUE，失败FALSE
使用说明：启动视频之前，必须设置好Camera_Pro相关参数
*/
VIDEOSPE_API BOOL VIDEO_CALL CHC_StartVideoStream(HANDLE pVideoCtrl);

/*
函数功能：停止视频流
函数参数：
[
pVideoCtrl：视频源对象
]
函数返回：成功TRUE，失败FALSE
*/
VIDEOSPE_API BOOL VIDEO_CALL  CHC_StopVideoStream(HANDLE pVideoCtrl);

/*
函数功能：暂停视频流
函数参数：
[
pVideoCtrl：视频源对象
]
函数返回：成功TRUE，失败FALSE
*/
VIDEOSPE_API BOOL VIDEO_CALL  CHC_PauseVideoStream(HANDLE pVideoCtrl);

/*
函数功能：的到视频当前的工作状态
函数参数：
[
pVideoCtrl：视频源对象
]
函数返回：成功TRUE，失败FALSE
*/
VIDEOSPE_API VIDEO_STATE VIDEO_CALL CHC_VideoState(HANDLE pVideoCtrl);

/*
函数功能：设置视频显示的窗口
函数参数：
[
pVideoCtrl：视频源对象
videoWnd：  视频显示窗口
msgWnd：    事件通知窗口
]
函数返回：成功TRUE ， 失败FALSE
使用说明：当设置显示窗口和事件通知窗口为NULL，表示不显示视频。注意,当视频显示的窗口要销毁时，
务必在窗口销毁前将显示窗口设置为空，否则视频在一个已经销毁的窗口上显示，将导致视频异常终止。
*/
VIDEOSPE_API BOOL VIDEO_CALL  CHC_SetVideoWnd(HANDLE pVideoCtrl , HWND videoWnd , HWND msgWnd);

/*
函数功能：显示属性对话框
函数参数：
[
pVideoCtrl：视频源对象
]
函数返回：成功 TURE ， 失败 FALSE
*/
VIDEOSPE_API BOOL VIDEO_CALL  CHC_DisplayProPage(HANDLE pVideoCtrl);

/*
函数功能：得到指定属性的范围和默认值
函数参数：
[
pVideoCtrl：    视频源对象
Property：      属性类型
pMin：          最小值
pMax：          最大值
pSteppingDelta：步进值
pDefault：      默认值
]
函数返回：成功 TRUE ，失败 FALSE
*/
VIDEOSPE_API BOOL VIDEO_CALL  CHC_GetVideoProRange(HANDLE pVideoCtrl , VideoProperty Property , long* pMin , long* pMax , long* pSteppingDelta , long* pDefault);

/*
函数功能：设置指定属性的值
函数参数：
[
pVideoCtrl：视频源对象
Property:   属性类型
lValue:     设置值
]
函数返回：成功 TRUE ，失败 FALSE
使用说明：如果设置失败，属性值维持原值
*/
VIDEOSPE_API BOOL VIDEO_CALL  CHC_SetVideoPro(HANDLE pVideoCtrl , VideoProperty Property , long lValue);

/*
函数功能：得到指定属性的值
函数参数：
[
pVideoCtrl：视频源对象
Property：  属性类型
lValue ：   当前值
]
函数返回：成功 TRUE ，失败 FALSE
使用说明：返回失败，表示视频不支持该属性
*/
VIDEOSPE_API BOOL VIDEO_CALL  CHC_GetVideoPro(HANDLE pVideoCtrl , VideoProperty Property , long* lValue);

/*
函数功能：视频中画矩形框
函数参数：
[
pVideoCtrl：视频源对象
pRect：     pRect 矩形框数组起始地址
total：     矩形框数组大小
]
函数返回：成功 TRUE  , 失败 FALSE
使用说明：设置矩形框和在视频视频上面画框是异步的，要清除视频上显示的矩形框需要将数组设置为
空，将数组大小设置为0。
*/
VIDEOSPE_API BOOL VIDEO_CALL  CHC_DrawRect(HANDLE pVideoCtrl , MYRECT* pRect , int total);

/*
函数功能：视频中画矩形圆
函数参数：
[
pVideoCtrl：视频源对象
pCircle     pCircle 圆形数组起始地址
total：     矩形框数组大小
]
函数返回：成功 TRUE  , 失败 FALSE
使用说明：设置圆和在视频视频上面画圆是异步的，要清除视频上显示的圆需要将数组设置为
空，将数组大小设置为0。
*/
VIDEOSPE_API BOOL VIDEO_CALL  CHC_DrawCircle(HANDLE pVideoCtrl , CIRCLE* pCircle , int total);

/*
函数功能：视频中叠加字符
函数参数：
[
pVideoCtrl：视频源对象
pString：   STRING数组的启示地址，表示叠加行字符
total：     数组大小
]
函数返回：成功 TRUE  , 失败 FALSE
使用说明：
*/
VIDEOSPE_API BOOL VIDEO_CALL  CHC_DrawString(HANDLE pVideoCtrl , CHZZ_STRING* pString , int total);

/*
函数功能：从视频流中提取一张图片.
函数参数：
[
pVideoCtrl：视频源对象
pImage:     图像CIPImage指针
tp：        图像像素排列顺序，TRUE表示像素矩阵原点在左上角，FALSE表示坐下角。
]
函数返回：成功 TRUE  , 失败 FALSE
使用说明：当pImage结构体中任意一个属性为0时，该函数返回采集图像大小等属性填充传入的结构体，
所以可以利用这个性质，首先得到图像的大小，分配内存，然后再调用该函数得到图像。
*/
VIDEOSPE_API BOOL VIDEO_CALL  CHC_CaptureImage(HANDLE pVideoCtrl , CIPImage* pImage , BOOL tp);

VIDEOSPE_API BOOL VIDEO_CALL  CHC_SetAGBValue(long lValue);

/******视频镜像****/
VIDEOSPE_API void VIDEO_CALL  CHC_SetVideoVChange(BOOL bValue, BOOL isOwnVideo /*= FALSE*/);//

VIDEOSPE_API BOOL VIDEO_CALL  CHC_SetSenseAuto(long lValue , BOOL isOwnVideo /*= FALSE*/);//

/******打开关闭红外灯****/
VIDEOSPE_API BOOL VIDEO_CALL  CHC_OpenLight(BOOL openorclose, BOOL isOwnVideo /*= FALSE*/);//,

/******在摄像头内写数据***/
VIDEOSPE_API BOOL VIDEO_CALL  CHC_VideoWrite(int addressWrite, BYTE byteV[100]); //,BOOL isOwnVideo = FALSE

//设置视频的传感器AGC\AEC自动,参数为AEW/AEB值

#endif