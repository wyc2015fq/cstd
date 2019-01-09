#ifndef _CM_FACERECOG_H_
#define _CM_FACERECOG_H_
#ifdef __cplusplus
extern "C" {
  ;
#endif
  typedef void* HFACELIB;
  typedef void* HCAM;
  // 用户id数据
  typedef struct USERINFO {
    char id[24]; // 用户标示符，必须唯一
  } USERINFO;
#define RGBSTDH  (320) // 注册照片宽度
#define RGBSTDW  (240) // 注册照片高度
  // 人脸识别返回数据结构
  typedef struct RECOGINFO {
    USERINFO ui;
    int ispass;   // 是否通过 1表示通过 0表示未通过 -1表示无人，只有通过时，参数ui才有意义
    int score;
    int workmode; // 工作模式
    int register_pos; // 采集进度
    int max_register_num; // 采集张数
    unsigned char* facereg; // 320*240的彩色注册照片(长度320*240*3字节)
  } RECOGINFO;
  // 打开摄像头
  // 成功返回摄像头句柄
  HCAM CM_StartCam(int id);
  // 关闭摄像头
  // 成功返回1，失败返回0
  int CM_CloseCam(HCAM p);
  // 创建人脸库句柄
  // 成功返回人脸库句柄
  HFACELIB CM_CreateFaceLib();
  // 释放人脸库句柄
  // 成功返回1，失败返回0
  int CM_DeleteFaceLib(HFACELIB hLib);
  // 获取人脸库中所有人脸id，可获取多个
  // 无论ui是否为NULL，都返回人脸库中人脸包个数
  int CM_GetFaceInfo(HFACELIB hLib, USERINFO* ui);
  // 在人脸库中删除一个人脸数据包
  // 成功返回1，失败返回0
  int CM_DeleteFacePack(HFACELIB hLib, USERINFO* ui);
  // 向人脸库中添加一个人脸数据包
  // 一个人脸数据包只包含一个人的人脸数据
  // 如果是同一个人，后载入的会覆盖先载入的
  // 成功返回1，失败返回0
  int CM_AddFacePack(HFACELIB hLib, const void* buf, int buflen);
  // 获取最大人脸包大小
  int CM_GetMaxFacePackLen(HFACELIB hLib);
  // 获取人脸包数据
  // buf缓冲区，buflen缓冲区大小
  // 返回人脸包大小
  int CM_GetFacePack(HFACELIB hLib, USERINFO* ui, void* buf, int maxbuflen);
  // 回调函数
  typedef int (*RecogCallback_t)(void* userdata, RECOGINFO* fi);
  enum { // type
    CMFR_VERSIONDATE,      // 获取动态库版本日期
    CMFR_HFACELIB,          // 设置人脸库 CM_SetLong(h, CMFR_HFACELIB, (unsigned long)(HFACELIB)p);
    CMFR_WORKMODE,          // 工作模式 0表示停止 1表示注册 2表示识别。CM_SetLong(h, CMFR_WORKMODE, 1);
    CMFR_RECOGCALLBACK,     // 人脸识别回调函数，每识别一次 CM_SetLong(h, CMFR_REGISTERCALLBACK, (unsigned long)(RecogCallback_t)fun);
    CMFR_REGISTERUSERINFO,  // 人脸注册模式，输入用户id CM_SetLong(h, CMFR_REGISTERUSERINFO, (unsigned long)(USERINFO*)userid);
    CMFR_LEVEL,             // 严格程度 可取3个级别 0: 容易 1: 中等 2: 严格。小于0视为0, 大于2视为2。CM_SetLong(h, CMFR_LEVEL, 1);
    CMFR_USERDATA,          // 用户自定义数据  CM_SetLong(h, CMFR_USERDATA, (unsigned long)p);
    CMFR_SHOWVIDEOHWND,     // 显示可见光视频的窗口或控件句柄 CM_SetLong(h, CMFR_SHOWVIDEOHWND, (unsigned long)(HWND)p);
    CMFR_SHOWRECT,          // 显示视频的窗口或控件句柄 CM_SetLong(h, CMFR_SHOWVIDEOHWND, (unsigned long)(HWND)p);
    CMFR_REGISTERTIMEDELIMS,     // 采集一次的时间间隔，单位毫秒，间隔越长可获得更多的姿态，但采集越慢 CM_SetLong(h, CMFR_REGISTERTIMEDELIMS, (unsigned long)p);
    // 调试模式下使用
    CMFR_DEBUG, //
    CMFR_SHOWVIDEOHWND2,   //
  };
  // 设置工作参数
  unsigned long CM_SetLong(HCAM p, unsigned long type, unsigned long l);
  //unsigned long CM_GetLong(HCAM p, unsigned long type);
#ifdef __cplusplus
}
#endif
#endif // _CM_FACERECOG_H_

