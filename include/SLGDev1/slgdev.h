////////////////////////////////////////////////////////////////////////////////////////
//名 称:slgdev.dll
//功能:管理端跟dsp端的业务逻辑通信接口，为管理端提供各种数据上传下载功能，
// 同时管理dsp端用户操作以及中转模式下各种中转处理
//说明::
//在使用本库过程中，有以下两点限制
//1. 上传用户照片时，照片内存大小限制在60KB以下
//2. 用户数据上传下载过程中，请不要在栈上分配大于150kb的内存
//3. 所有命令序列号不能为0，0序号内部占用
////////////////////////////////////////////////////////////////////////////////////////
//14:15 2016-9-20
//1. IPv6 DNS
// 支持通过 ipv4 ipv6 域名 三种设备地址连接设备。
// 例如:
//  SLG_CNDev("192.168.1.150""); //ipv4
//  SLG_CNDev("2001::51d6:120a:e9f4:d0ce"); //ipv6
//  SLG_CNDev("mr-bubblehead-2.example.com"); //hostname
// 所有 cszDevAddr 参数均可使用以上方式。
//2. TLS，rootca
// 支持ssl, 当DevConnectStatus 回调函数响应 DEV_QUERY_ENCRYPT 时
// 返回0   don't encrypt
// 返回1   need encrypt and it is server-side
// 返回2   need encrypt and it is client-side
// 返回3   need encrypt and it is client-side 并进行根证书验证，请把根证书放 "C:\\rootca"(SSL_ROOTCA_PATH) 目录下
// 例子代码:
//  int SLGDEV_CALL DevConnectNotify (const char* dev_addr, DEV_CONNECT_STATUS flag, void* pvContext) {
// ....
//  switch (flag) {
// ....
//  case DEV_QUERY_ENCRYPT: return 3; //根据应用程序需要返回不同的值
// ....
// }
//  return 0;
// }
//3. 上传证书: 通过调用 SLG_ULUpdate 函数
//3. HID card number and facility code
//  card number:facility code , Example 9001:99
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Name:slgdev.dll
//Function::the logical communication interface between administrative end and DSP end, it is used to provide the data upload and download function for the administrative end, and to control the DSP end user operation, as well as all kinds of transfers in the repeater mode operation.
//Comment:s:
//Limitations
//1. The size of each user picture is limited less than 60KB when uploading it.
//2. During the user data uploading and downloading, the internal storage distributed in stack cannot over 150KB.
//3. All commands serial number cannot be 0, 0 has been internal used.
////////////////////////////////////////////////////////////////////////////////////////
//14:15 2016-9-20
//1. IPv6 DNS
//  It supports three device addresses: ipv4, ipv6 and domain name to connect devices.
//  Examples:
//  SLG_CNDev("192.168.1.150""); //ipv4
//  SLG_CNDev("2001::51d6:120a:e9f4:d0ce"); //ipv6
//  SLG_CNDev("mr-bubblehead-2.example.com"); //hostname
//  All cszDevAddr parameters can work as the way above.
//2. TLS，rootca
//  It supports ssl; when callback function DevConnectStatus responses to DEV_QUERY_ENCRYPT
//  return to 0   don't encrypt
//  return to 1   need encrypt and it is server-side
//  return to 2   need encrypt and it is client-side
//  return to 3   need encrypt and it is client-side and processing root certificate validation (need to put the root certificate in the catalogue "C:\\rootca"(SSL_ROOTCA_PATH))
//  example codes:
//  int SLGDEV_CALL DevConnectNotify (const char* dev_addr, DEV_CONNECT_STATUS flag, void* pvContext) {
// ....
//  switch (flag) {
// ....
//  case DEV_QUERY_ENCRYPT: return 3; //Based on the need of the applications to return to different values
// ....
// }
//  return 0;
// }
//3. Upload certificate: by calling SLG_ULUpdate function
//3. HID card number and facility code
//  card number:facility code , Example 9001:99
////////////////////////////////////////////////////////////////////////////////////////
#ifndef __SLGDEV_H__
#define __SLGDEV_H__
#ifdef __cplusplus
#define EXTERN_C_BEGIN extern "C" {
#define EXTERN_C_END }
#else
#define EXTERN_C_BEGIN
#define EXTERN_C_END
#endif
EXTERN_C_BEGIN
//#define WINCE
#define SDK_VERSION  "1.4"
///////////////////////////////////////////////////////////////////////////////////////
//  config
///////////////////////////////////////////////////////////////////////////////////////
// for WINCE
#ifdef WINCE
#define WIN32
#endif
///////////////////////////////////////////////////////////////////////////////////////
//根证书路径
#define SSL_ROOTCA_PATH  "C:\\rootca"
///////////////////////////////////////////////////////////////////////////////////////
// for openssl
#define USE_SSL
///////////////////////////////////////////////////////////////////////////////////////
// macro
#ifdef _WIN32
// for windows
#define STDCALL_CALL  __stdcall
#define CDECL_CALL __cdecl
#define SLGDEV_CALL  __stdcall
typedef unsigned long /**/(__stdcall* thread_cb)(void* pParam);
#ifndef _WINSOCK2API_
typedef unsigned int SOCKET;
#endif
#else
// for linux
#define SLGDEV_CALL
#define STDCALL_CALL
#define CDECL_CALL
#define GetLastError() errno
#define WINAPI
#define _snprintf snprintf
#define _vsnprintf vsnprintf
typedef int SOCKET; //linux
typedef void* (*thread_cb)(void* pParam);
typedef long LONG;
typedef unsigned char BYTE;
typedef unsigned int UINT;
typedef unsigned short USHORT;
typedef unsigned char UCHAR;
#endif
///////////////////////////////////////////////////////////////////////////////////////
// BOOL
#ifndef BOOL
typedef int BOOL;
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
///////////////////////////////////////////////////////////////////////////////////////
// net struct
#define IP_LEN 16
#define IP_LEN64 64
#define URL_LEN 127
typedef struct
{
    unsigned short family;
    unsigned char data[26];
} addr_in;
typedef struct
{
    unsigned short Port;                 //端口
    char           IP_Address[IP_LEN64]; //点分十进制IP地址
} SOCK_ADDRESS, *PSOCK_ADDRESS;
typedef SOCK_ADDRESS NET_ADDRESS;
typedef SOCK_ADDRESS PEERADR;
struct CSockBase;
typedef void (SLGDEV_CALL* ListenCallBack)(SOCKET hSOCK, const addr_in* Addr, void* pvContext, struct CSockBase* user);
typedef void (SLGDEV_CALL* DataRecvCallBack)(SOCKET hSOCK, const char* pbData, int nLen, void* pvContext, void* user);
typedef void (SLGDEV_CALL* DataRecvFromCallBack)(SOCKET hSOCK, const char* pbData, int nLen, const addr_in* addr, void* pvContext, void* user);
///////////////////////////////////////////////////////////////////////////////////////
#define F_APP_PACKAEG  0
#define F_IMG_STANDBY  1
#define F_PSD_CERTIFI  2
///////////////////////////////////////////////////////////////////////////////////////
#define DEV_REGION_ADDR       ("224.0.1.100") //默认区域地址,用户设备自动自动搜索 //The default multicasting address,automatic search  
#define DEV_ID_LEN            24              //ID长度//ID length
#define DEV_TIMEGROUP_NUMS    8               //时间组个数 //number of the timegroup
#define DEV_USER_COMBINS      6               //用户组合个数 //number of the user group
#define DL_DEV_PIC            0x01            //照片//picture
#define DL_DEV_USER_FEAT      0x02            //人脸特征//facial features
#define DL_DEV_USER_TEXT      0x04            //用户文字信息//texts
#define DEV_CHECK_TIME        0x01            //检测时间//Testing time
#define DEV_CHECK_WEEK        0x02            //检测星期//Testing week
#define DEV_CHECK_DAY         0x04            //检测日期//Testing date
#define DEV_CHECK_MONTH       0x08            //检测月份//Testing month
#define DEV_CHECK_YEAR        0x10            //检测年份//Testing year
#define DEV_WEEK_1            0x01            //星期一有效//Monday is effective 
#define DEV_WEEK_2            0x02            //星期二有效//Tuesday is effective
#define DEV_WEEK_3            0x04            //星期三有效//Wendesday is effective
#define DEV_WEEK_4            0x08            //星期四有效//Thursday is effective
#define DEV_WEEK_5            0x10            //星期五有效//Friday is effective
#define DEV_WEEK_6            0x20            //星期六有效//Saturday is effective
#define DEV_WEEK_7            0x40            //星期天有效//Sunday is effective
#define DEV_RECORD_SAVEFAIL   0x00000001      //是否保存识别失败记录//If to save the record of recognition failure
#define DEV_SUPER_PASSWORD    0x00000002      //超级密码开门有效//Super password to open door is effective
#define DEV_HDBEEP_OPEN       0x00000004      //布防/撤防//Deployment/Disarming
#define DEV_REALTIME_RECORD   0x00000010      //实时动态记录发送//Real time dynamic record displ
#define DEV_REALTIME_USERLOAD 0x00000020
//输入编号或者卡号识别时，如果无法在当前dsp中获得人脸特征,在网络连接状态下，将从管理端实时加载人脸特征
//Input serial number or card number for identification, if failure to gain the facial features from local DSP, it will load in real-time from the administrative end when its network is connecting.
#define DEV_REALTIME_USERSEND       0x00000040 //通过设备采集的用户实时发送到管理终端//Collect users for real time transmiting to administrative end
#define DEV_DOORMANGET_OPEN         0x00000080 //开启门磁检测报警//Door magnet detection alarm
#define DEV_DOORFORCE_OPEN          0x00000100 //开启胁迫开门功能//Door is forced to open
#define DEV_REMOTE_CAP_SAVE         0x00000200 //远程采集的用户是否保存到DSP//If the remote users collection from the administrative end save to the local DSP
#define DEV_GPRS_OPEN               0x00000400 //开启GPRS模块//Open GPRS module
#define DEV_UPDATE_USERSEND         0x00000800 //特征更新时，是否实时发送用户特征//If the facial features update, to send users’ features in real time or notIf the facial features update, to send users’ features in real time or not
#define DEV_REALTIME_USERLOAD_PRIOR 0x00001000 //网络连接情况下，优先网络加载 //Network loading is priority once the  network is connecting.
#define DEV_AB_LOCK                 0x00002000 //AB互锁//AB interlocking 
#define DEV_DOOR1_NOPEN             0x00004000 //门一N+1//Door one N+1
#define DEV_DOOR2_NOPEN             0x00008000 //门二N+1//Door two N+1
#define DEV_ENABLE_COUNT            0x00010000 //在线记录数统计//Online records statistics
#define DEV_ENCRYP_DISPALY          0x00020000 //是否对用户信息进行加密显示//If display user information via encryption
#define DEV_ARMED_ENABLE_AWAYS      0x00400000 //触发报警时，报警持续 //Trigger alarm, alarm lasting
#define DEV_OPENSSL_ENABLE          0x00800000 //ssl加密//ssl encryption
#define DEV_WAKE_INPUT_ENABLED      0x20000000 //开启仅辅助输入能激活验证//Activate verification only by auxiliary input
//验证模式优先级低-高:UID->CARD->11->MIX->1N
//Verification mode priority low-high: UID->CARD->11->MIX->1N
#define DEV_VERIFY_USERID           0x01       //工号//Work number
#define DEV_VERIFY_CARD             0x02       //刷卡//Swipe card
#define DEV_VERIFY_FACE_11          0x04       //11人脸加载//11 facial features loading
#define DEV_VERIFY_FACE_MIX         0x08       //混合人脸加载//Mixed facial features loading
#define DEV_VERIFY_FACE_1N          0x10       //1:N人脸加载 //1:N facial features loading
#define DEV_VERIFY_FINGER           0x20       //指纹认证
#define DEV_VERIFY_PLUSALL          0x40       //1:1组合时多重认证//1:1 Mixed facial identification
#define DEV_VERIFY_REMOTE           0x80    //远程加载方式
//用户状态信息//User status information
#define DEV_USER_CARD_INVALID       0x01       //卡挂失 //Card reporting loss
#define DEV_USER_DLine_INVALID      0x02       //失效时间有效//Time of expiry
#define DEV_USER_BLACK              0x04       //黑名单//Blacklist
#define DEV_USER_MIX1N              0x08       //混合模式下1N用户//Mixed Facial identification mode, 1:N user
#define DEV_USER_VIPCARD            0x10       //VIP用户//VIP user
#define DEV_USER_FINGER_PRINT       0x20    //指纹认证
#define DEV_USER_SYNC_FINGEER       0x40    //指纹同步
#define DEV_USER_MAX_MID            0x40    //不得超过中等阈值
#define DEV_USER_MAX_LOW            0x80    //不得超过严格阈值
//SDK自定义
#define DEV_USER_FEAT_BASE64        0x40       //特征经过base64编码//Used base64 to code the features
#define DEV_USER_IMAGE_BASE64       0x80       //注册照经过base64编码//Used base64 to code the registered pictures
///////////////////////////////////////////////////////////////////////////////////////
typedef enum
{
    DEV_CONNECT_CUT = 0x00,                  //断开连接//Disconnecting
    DEV_CONNECT_SUCCESS,   //建立连接//Connection successful
    DEV_CONNECT_FAILUE,    //连接失败// Connection failed
    DEV_CONNECT_NONE,      //未建立连接// Unconnected
    DEV_QUERY_ENCRYPT,     //与设备是否进行加密传输// If the terminal use the encryption transmission
    //0 : don't encrypt
    //1 : need encrypt and it is server-side
    //2 : need encrypt and it is client-side
    //3 : need encrypt and it is client-side and processing root certificate validation (need to put the root certificate in the catalogue "C:\\rootca"(SSL_ROOTCA_PATH))
    SRV_QUERY_ENCRYPT,     //与中转服务器是否进行加密传输// If the transit server use the encryption transmission
    DEV_VERIFYCERT_FAILUE, //证书验证失败// Certificate validation fail
} DEV_CONNECT_STATUS;

typedef enum
{
    DEV_OPER_UPLOAD   = 0x01, //上传// Upload
    DEV_OPER_DOWNLOAD = 0x02, //下载// Download
    DEV_OPER_DELETE   = 0x04, //删除//Delete
} DEV_OPER_FLAG;

typedef enum
{
    DEV_AUTH_OPER = 0x01,                  //设备验证操作// Device Verification
    DEV_AUTH_SET_OPER,     //设备验证用户设置操作// Device Verification of user setup operation
    DEV_REGION_OPER,       //设备区域操作// Device region operation
    DEV_FORMAT_OPER,       //设备格式化操作// Device formatting operation
    DEV_UPDATE_OPER,       //设备程序更新操作// Device program update operation
    DEV_SYS_TIME_OPER,     //设备系统时间操作// Device system time operation
    DEV_BASEINFO_OPER,     //基本信息操作// Basic information operation
    DEV_STATISINFO_OPER,   //统计信息操作// Statistical information operation
    DEV_WORKATT_OPER,      //工作属性操作// Work attribute operation
    DEV_USER_OPER,         //用户操作// User operation
    DEV_USER_RT_OPER,      //用户实时加载操作// User real-time loading operation
    DEV_USER_RANGE_OPER,   //用户下载数据条数回馈操作// User download data number feedback operation
    DEV_RECORD_OPER,       //记录操作// Record operation
    DEV_RECORD_RANGE_OPER, //记录下载数据条数回馈操作// Record download data number feedback operation
    DEV_ALARM_OPER  ,
    DEV_ALARM_RANGE_OPER  ,
    DEV_LOG_OPER,          //日志操作// Log operation
    DEV_LOG_RANGE_OPER,    //日志下载数据条数回馈操作// Log download data number feedback operation
    DEV_CHANGEIMAGE_OPER,  //变更注册照操作// Modify registered image operation
    DEV_CIMAGE_RANGE_OPER, //变更注册照下载数据条数回馈操作// Modify registered image download data number feedback operation
    DEV_TIME_GROUP_OPER,   //时间组操作// Time group operation
    DEV_RIGHT_OPER,        //权限操作// Access operation
    DEV_USERGROUP_OPER,    //用户组操作// User group operation
    DEV_DOOR_STATE_OPER,   //开门状态操作// Door-open operation
    DEV_REMOTE_OPEN_DOOR,  //远程开门操作// Remote open door operation
    DEV_VIDEO_TRANSFER,    //视频传输// Video transmission
    DEV_USER_EX_OPER,      //用户批量操作// Massive user operation
    DEV_RECORD_EX_OPER,    //记录批量操作// Massive record operation
    DEV_LOG_EX_OPER,       //日志批量操作// Massive log operation
    DEV_CIMAGE_EX_OPER,    //变更注册照批量操作// Massive modify registered images operation
    DEV_REBOOT_OPER,       //设备重启// Reset
    DEV_USER_REMOTE_CAP,   //用户远程采集// User remotely collect
    DEV_NOPEN_OPER,        //N+1开门// N+1 door-open
    DEV_IOCTRL_OPER,       //IOCTRL控制信息 // IOCTRL information control
    CLI_AUTH_OPER,         //client像中转服务器验证操作// client as transfer server validation operation
    DEV_NETPRO_OPER,       //设备网络属性设置// Network propery setup
    DEV_SCREEN_OPER,       //屏幕校验// Screen verification
    DEV_USER_REMOTE_RECAP, //采集进度回复, DEV_RCAPINFO//Collection schedule feedback, DEV_RCAPINFO
    DEV_WAPINFO_OPER,      // for GetWpaIn
} DEV_OPER_TYPE;

//操作结果//Operate result
typedef enum
{
    OPER_SUCCESS           = 0x00, //成功// Success
    OPER_ERR_BUSY          = 0x01, //设备忙//Device is unavailable
    OPER_ERR_LIMIT         = 0x02, //已达上限//Reach upper limitation
    OPER_ERR_NOFIND        = 0x03, //没有找到对应数据//No data
    OPER_ERR_SAVEFAIL      = 0x04, //数据保存失败//Data save failure
    OPER_ERR_SETFAIL       = 0x05, //设置失败//Setup failure
    OPER_ERR_FROMAT        = 0x07, //格式化失败 //Formatting failure
    OPER_ERR_PARAM         = 0x08, //参数:错误 //Parameter: error
    OPER_ERR_DISABLE       = 0x09, //要求执行的功能没有使能//Requiring function disabled
    OPER_ERR_EXECUT        = 0x0A, //失败//Failure
    OPER_ERR_SUPPORT       = 0x10, //不支持的命令//Nonsupport order
    OPER_ERR_INPUTDATA     = 0x11, //网络端传输的数据有异常//Network input data abnormal
    OPER_BATCH_DL_COMPLETE = 0x1F, //批量下载完成//Massive download complete
} DEV_OPER_RESULT;

//时间组类型//Time group type
typedef enum
{
    DEV_NORMAL   = 0x01, //正常时间// Normal time
    DEV_HOLIDY   = 0x02, //节假日// Holiday
    DEV_ANY_TIME = 0x03, //任意时间//Anytime
} DEV_TIMEGROUP_TYPE;

//用户类型 //User type
typedef enum
{
    DEV_USER_NOMAL      = 0, //普通// Normal User
    DEV_USER_ADMIN      = 1, //管理员// Administrator
    DEV_USER_SUPERADMIN = 2, //超级管理员//Super Administrator
} DEV_USER_TYPE;

//默认权限//Default access
typedef enum
{
    DEV_DEFAULT_NO_RIGHT    = -2, //无权限// No Access
    DEV_DEFAULT_SINGLE_OPEN = -1, //单一开门权限//Single open access
} DEV_DEFAULT_RIGHT;

//记录类型 // Record type
//0x01 - 0x20: 日常记录 Normal records
//0x20 - 0xFF: 报警记录 Alarm records
#if 0
typedef enum
{
    DEV_VERIFY_SUCC              = 0x01, //验证成功// Verify successful
    DEV_VERIFY_FAIL              = 0x02, //验证失败// Verify failed
    DEV_ADMIN_SUCC               = 0x03, //管理员验证成功// Administrator verify successful
    DEV_EMER_OPEN                = 0x04, //紧急开门// Urgent door-open
    DEV_RIGHT_OPEN               = 0x05, //权限开门// Access door-open
    DEV_GROUP_OPEN               = 0x06, //组合开门// Combined door-open
    DEV_BUTTON_OPEN              = 0x07, //按钮开门// Button door-open
    DEV_START_VERIFY             = 0x0c, //开始验证// Start verification
    DEV_STOP_VERIFY              = 0x0d, //停止验证// Stop verification
    DEV_ALARM_HD_MANGET_TIMEOUT  = 0x20, //门磁超时// Door-magnet time-out
    DEV_ALARM_HD_MANGET_ILLOPEN  = 0x21, //门磁非法开门// Door-magnet illegally door-open
    DEV_ALARM_HD_OFFLINE         = 0x22, //前端掉线报警// Faceplate offline alarm
    DEV_ALARM_HD_BREAK           = 0x30, //防拆报警// Tamper alarm
    DEV_ALARM_HD_SHOCK           = 0x31, //震动报警// Vibration alarm
    DEV_ALARM_HD_FPOWR           = 0x36, //前端供电异常报警// Faceplate power supply abnormal alarm
    DEV_ALARM_HD_UPS_ON          = 0x37, //UPS备用电池开启// UPS standby battery open
    DEV_ALARM_HD_UPS_OFF         = 0x38, //UPS备用电池关闭// UPS standby battery close
    DEV_ALARM_HD_ASSIST          = 0x40, //辅助输入触发// Auxiliary input trigger
    DEV_ALARM_SF_BACKLIST        = 0xF0, //黑名单验证报警// Blacklist verify alarm
    DEV_ALARM_SF_ILLCARD         = 0xF1, //无效卡(挂失)// Invalid card (report loss)
    DEV_ALARM_SF_ILLTIME         = 0xF2, //非法时间识别// Illegal time identification
    DEV_ALARM_SF_DEADLINE        = 0xF3, //失效时间// Time of expiry
    DEV_ALARM_SF_DANGER_OPEN     = 0xF4, //胁迫开门// Forced to open
    DEV_ALARM_SF_SUPER_OPEN      = 0xF5, //超级密码开门 //Super password door-open
} DEV_REC_TYPE;
#else

//记录类型:
//0x01 - 0x20: 正常事件
//0x20 - 0x40: 硬件报警事件
//0x40 - 0x50: 辅助输入
//0x50 - 0x60: 辅助输出
//0xf0 - 0xff: 软件报警事件
//其他: 未定义
typedef enum
{
    DEV_VERIFY_SUCC               = 0x01, //验证成功
    DEV_VERIFY_FAIL               = 0x02, //验证失败
    DEV_ADMIN_SUCC                = 0x03, //管理员验证成功
    DEV_EMER_OPEN                 = 0x04, //紧急开门
    DEV_RIGHT_OPEN                = 0x05, //权限开门
    DEV_GROUP_OPEN                = 0x06, //组合开门
    DEV_BUTTON_OPEN               = 0x07, //按钮开门
    DEV_START_VERIFY              = 0x0c, //开始验证// Start verification
    DEV_STOP_VERIFY               = 0x0d, //停止验证// Stop verification
    DEV_ALARM_HD_MANGET_TIMEOUT   = 0x20, //门磁超时
    DEV_ALARM_HD_MANGET_ILLOPEN   = 0x21, //门磁非法开门
    DEV_ALARM_HD_OFFLINE          = 0x22, //前端掉线报警
    DEV_ALARM_HD_BREAK            = 0x30, //防拆报警
    DEV_ALARM_HD_SHOCK            = 0x31, //震动报警
    DEV_ALARM_HD_FPOWR            = 0x36, //前端供电异常报警
    DEV_ALARM_HD_UPS_ON           = 0x37, //UPS备用电池开启
    DEV_ALARM_HD_UPS_OFF          = 0x38, //UPS备用电池关闭
    DEV_ALARM_HD_ASSIST           = 0x40, //辅助输入触发
    DEV_ALARM_HD_WL_DOOR_MAGNETIC = 0x41, //无线
    DEV_ALARM_HD_WL_SMOKE         = 0x42,
    DEV_ALARM_HD_WL_GAS           = 0x43,
    DEV_ALARM_HD_WL_BUTTON1       = 0x44,
    DEV_ALARM_HD_WL_BUTTON2       = 0x45,
    DEV_ALARM_HD_WL_BUTTON3       = 0x46,
    DEV_ALARM_HD_WL_BUTTON4       = 0x47,
    DEV_ALARM_HD_WL_AREA1         = 0x48,
    DEV_ALARM_HD_WL_AREA2         = 0x49,
    DEV_ALARM_HD_WL_AREA3         = 0x4A,
    DEV_ALARM_HD_WL_AREA4         = 0x4B,
    DEV_ALARM_HD_WL_AREA5         = 0x4C,
    DEV_ALARM_HD_WL_AREA6         = 0x4D,
    DEV_ALARM_HD_WL_AREA7         = 0x4E,
    DEV_ALARM_LINK_OUT1           = 0x50,   //联动输出1
    DEV_ALARM_LINK_OUT2           = 0x51,   //联动输出2
    DEV_ALARM_LINK_GPRS           = 0x52,   //联动GPRS
    DEV_ALARM_LINK_2OPEN          = 0x53,   //双开
    DEV_ALARM_LINK_2CLOSE         = 0x54,   //双闭
    DEV_ALARM_LINK_2NORMAL        = 0x55,   //常规
    DEV_ALARM_AB_RULE             = 0xE0,
    DEV_ALARM_SF_ILLREMOTE        = 0xE1,   //远程加载无效
    DEV_ALARM_SF_ILLGROUP         = 0xE2,   //组合失败，报警
    DEV_ALARM_SF_ILLCARD          = 0xE3,   //无效卡
    DEV_ALARM_SF_ERRSTATE         = 0xE4,   //错误门状态
    DEV_ALARM_SF_INTERLOCK        = 0xE5,   //AB互锁
    DEV_ALARM_SF_BACKLIST         = 0xF0,   //黑名单验证报警
    DEV_ALARM_SF_LOSSCARD         = 0xF1,   //挂失卡
    DEV_ALARM_SF_ILLTIME          = 0xF2,   //非法时间识别
    DEV_ALARM_SF_DEADLINE         = 0xF3,   //失效时间
    DEV_ALARM_SF_DANGER_OPEN      = 0xF4,   //胁迫开门
    DEV_ALARM_SF_SUPER_OPEN       = 0xF5,   //超级密码开门
    DEV_ALARM_SF_CAP_SHAKE        = 0xF6,   //采集抖动
} DEV_REC_TYPE;
#endif

typedef enum
{
    DEV_DOOR_SOURCE = 0x01,                  //门相关来源 // Source of the door
    DEV_SIGNAL_SOURCE,     //信号输入输出相关来源 // Source of signal input and output
    DEV_NO_NUM_DEV_SOURCE  //无编号设备来源//Source of no serial number devi
} DEV_REC_SOURCE_TYPE;

typedef enum
{
    DEV_LOG_ADDUSER     = 0x01, //增加用户// Add user
    DEV_LOG_DELUSER     = 0x02, //删除用户// Delete user
    DEV_LOG_REREGIST    = 0x03, //重新采集// Re-collection
    DEV_LOG_CAHNGETYPE  = 0x04, //改变用户类型// Modify user type
    DEV_UDISK_ULUSER    = 0x05, //U盘上传用户// USB flash disk upload user
    DEV_UDISK_DLUSER    = 0x06, //U盘下载用户// USB flash disk download user
    DEV_UDISK_DLRECORD  = 0x07, //U盘下载记录// USB flash disk download record
    DEV_UDISK_UPDATEAPP = 0x08, //U盘更新程序//USB flash disk update program
} DEV_LOG_TYPE;

typedef enum
{
    DEV_DOOR_NOMAL = 0x00000000, //正常状态// Normal door status
    DEV_DOOR_OPEN  = 0x00000100, //开状态// Open door status
    DEV_DOOR_CLOSE = 0x00000200  //关状态//Close door status
} DEV_DOOR_STATE;

//ID 标识// identification
typedef struct
{
    char m_ID[ DEV_ID_LEN ];
} DEV_CID;

//日期 //Date
typedef struct
{
    int m_Year;
    char m_Month;
    char m_Day;
} DEV_DATE;

//时间//Time
typedef struct
{
    int m_Msec; //毫秒 //millisecond
    char m_Hour;
    char m_Minute;
    char m_Second;
} DEV_TIME;

//日期和时间 //Date and time
typedef struct
{
    DEV_DATE m_Date;
    DEV_TIME m_Time;
} DEV_DATETIME;

//设备基本信息//Basic information
typedef struct
{
    int     m_DevType;     //设备类型//Device type
    int     m_LimitUser;   //总使用人数上限//Total user number upper limitation
    int     m_Limit1NUser; //1:N用户上限//1:N user upper limitation
    DEV_CID m_SN;          //设备编号//Device serial number
    DEV_CID m_Ver;         //DSP软件版本//DSP software version
    DEV_CID m_Space;       //磁盘容量信息//Disk size informati
} DEV_BASEINFO;
//DEV相关统计信息//DEV related statis information
typedef struct
{
    int m_TotalUsers;       //当前设备的总用户数//Total users number of current device
    int m_NoFeatUser;       //没有采集人脸特征的用户数//Number of no facial features users
    int m_TotalDays;        //识别记录保存总天数//Total days for identification record retention
    int m_TotalRecords;     //总记录数//Total record
    int m_TotalAlarm;       //总报警记录数//Total alarm record
    int m_TotalDspLog;      //总操作日志数目 //Total number of operational log
    int m_TotalChangeImage; //总变更注册照//Total modified registered imag
} DEV_STATIS;

//设备默认权限//Device default access
typedef struct
{
    int     m_TimeGID[DEV_TIMEGROUP_NUMS]; //时间组ID //Time group ID
    int     m_BaseSet;                     //参见设备功能标记定义//Refer to device functional marker definition
    int     m_DoorMangetTime;              //门磁延时时间，单位秒//Door magnet delay time, seconds
    int     m_LockTime;                    //电锁持续时间，单位秒//Electric lock persistent time, seconds
    int     m_VerifyMode;                  //用户身份验证模式,例如:DEV_VERIFY_USERID|DEV_VERIFY_CARD|DEV_VERIFY_FACE_11
    int     m_nWGType;                     //韦根协议类型(0输出韦根26,1输出韦根34)//Wiegand Protocol Type (0 output Wiegand 26, 1 output Wiegand 34)
    int     m_nWGOutType;                  //韦根输出类型(0输出ID，1输出WG内容)//Wiegand Protocol type (0 output ID，1 output WG content)
    int     m_nWGOutIDType;                //输出ID类型(0输出卡号,1输出用户ID)//Output ID type (0 output card number,1 output user ID)
    int     m_nWGOutContent;               //WG输出内容//WG output content
    BOOL    m_bWGOutPut;                   //是否WG输出//If WG output
    DEV_CID m_szSuperPWD;                  //超级密码//Super password
    DEV_DEFAULT_RIGHT m_DefaultRight;
} DEV_WORKATT;

typedef struct
{
    DEV_USER_TYPE m_UserType;    //终端设备上的用户类型(普通，管理，超管 -- 0, 1, 2) //User type in terminal device (normal, administrator, super administrator -- 0 , 1 , 2)
    DEV_CID       m_UserID;      //用户ID  //User ID
    DEV_DATETIME  m_TypeTime;    //用户类型更改时间//Time for modifying user type
    DEV_DATETIME  m_RegistTime;  //人脸注册时间，即特征采集时间，采集或者重新采集要更新此时间 //Time for registering image, which is the time for collecting facial features, collection and re-collection all need to update the time
    DEV_DATETIME  m_LastUpdTime; //最后更新的时间，识别时特征发生更新或者重新采集要更新此时间 //Last-update time, features update in the recognition process or re-collect need to update the ti
} DEV_VUSER;

//用户信息//User information
typedef struct
{
    int           m_FeatLen;      //特征大小//Feature Length
    int           m_PicLen;       //照片大小//Picture Size
    int           m_RightID;      //用户权限ID //User Right ID
    DEV_CID       m_ImageSID;     //上传注册照，对应的变更ID //uploading reg. pictures that correspond to ID of change and globally unique
    DEV_CID       m_UserID;       //用户唯一ID,关键字 //the unique User ID-Keyword
    DEV_CID       m_Admin;        //人脸注册管理员//Administrator of face registration
    DEV_CID       m_AppendID;     //卡号或者其他用于1:1的附加身份确认信息//Card Num. or other appended info for 1:1 identifications
    DEV_CID       m_UserName;     //用户名//User Name
    DEV_CID       m_Department;   //部门名称//Department Name
    DEV_DATETIME  m_DeadLineTime; //失效时间//Time of expiry
    DEV_DATETIME  m_RegistTime;   //人脸注册时间//Time of Face Registration
    DEV_DATETIME  m_LastUpdTime;  //最后更新的时间//last-update Time
    DEV_DATETIME  m_TypeUpdTime;  //用户类型变更时间//Update Time for User Types
    char          m_UserFlag;     //用户状态标记//User Status Flag
    DEV_USER_TYPE m_UserType;     //终端设备上的用户类型//User Type on terminals
    char*         m_FeatData;     //特征数据//Feature Data
    char*         m_PicData;      //照片数据//Picture Data
    int           m_bRTUser;      //是否为实时用户(0 非实时用户 1 实时采集用户 2实时特征更新用户) //if it is real-time user (0 for non real-time user, 1 for real-time image-collecting user and 2 for real-time feature-updating user
} DEV_USER;

//用户下载集合//User download array
typedef struct
{
    int        m_Count;  //Array Size
    DEV_VUSER* m_pArray; //ID集合//Array Starting Addre
} DEV_DL_USER_RANGE;

#define DEV_TYPE_FACE1 0x0 //人脸前端1//Facial Terminal 1
#define DEV_TYPE_FACE2 0x1 //人脸前端2//Facial Terminal 2
#define DEV_TYPE_CARD1 0x2 //刷卡器1//Card Scanner 1
#define DEV_TYPE_CARD2 0x3 //刷卡器2//Card Scanner 2
#define DEV_TYPE_IN    0x4 //辅助输入//Auxiliary Input
#define DEV_TYPE_OUT   0x5 //辅助输出//Auxiliary Output
#define DEV_TYPE_INWL  0x6 //无线输入//Wireless Input
#define DEV_TYPE_DOOR  0xF //门点本身//Door

///////////////////////////////////////////////////////////////////////////////////////
//关于记录和事件来源485地址解析:
//1.485地址用一个字节表示，高4位[4-7]表示设备索引号1-15, 低4位[0-3]表示支持16种不同型的设备
//例如: 0x11 表示1号门人脸识别前端1，INDEX[4-7] == 1 TYPE[0-3] == 1
//0x10 表示1号门人脸识别前端0，INDEX[4-7] == 1 TYPE[0-3] == 0
//0x12 表示1号门刷卡器0 INDEX[4-7] == 1 TYPE[0-3] == 2
//0x13 表示1号门刷卡器1 INDEX[4-7] == 1 TYPE[0-3] == 3
//0x1F 表示门点本身 INDEX[4-7] == 1 TYPE[0-3] == F
//0x14 表示辅助输入1 INDEX[4-7] == 1 TYPE[0-3] == 4
//0x25 表示辅助输出2 INDEX[4-7] == 2 TYPE[0-3] == 5
//2. 所有设备的序号从1开始。此协议可支持15张门，16中不同类型的门点输入设备，15个辅助输入和15个辅助输出
//3. 特殊意义的地址: [0-7] == 0x00 表示非法地址，[0-7] ＝＝ 0x01表示后端板本身, [0-7] ＝＝ 0x02表示GPRS模块
//7_______3________0
//|_INDEX_|__type__|
///////////////////////////////////////////////////////////////////////////////////////
//RS485 address (1 byte) parsing of the event-recording devices::
//1.41. The 4 most significant bits[4-7] of the RS485 address represent the device indices 1-15; The 4 Least Significant bits[0-3] represent the devices of 16 different types.
// Examples: 0x11 denoting recog. terminal 1 of Door No.1，INDEX[4-7] == 1 TYPE[0-3] == 1
// 0x10 denoting recog. terminal 0 of Door No.1，INDEX[4-7] == 1 TYPE[0-3] == 0
// 0x12 denoting card scanner 0 for Door. No.1 INDEX[4-7] == 1 TYPE[0-3] == 2
// 0x13 denoting card scanner 1 for Door. No.1 INDEX[4-7] == 1 TYPE[0-3] == 3
// 0x1F denoting the Door Point INDEX[4-7] == 1 TYPE[0-3] == F
// 0x14 denoting auxiliary input 1 INDEX[4-7] == 1 TYPE[0-3] == 4
// 0x25 denoting auxiliary input 2 INDEX[4-7] == 2 TYPE[0-3] == 5
//2. All device numbers start with 1. This protocol supports 15 doors, 16 different door terminals and 15 input devices and output devices.
//3. Reserved Addresses of special purposes: [0-7] == 0x00  for illegal addresses，[0-7] ＝＝ 0x01 for internal circuitry boards, [0-7] ＝＝ 0x02 for GPRS modules
//7_______3________0
//|_INDEX_|__type__|
///////////////////////////////////////////////////////////////////////////////////////
//实时用户加载//User real-time loading
typedef struct
{
    int     m_LoadFlag;  //DL_DEV_PIC|DL_DEV_USER_FEAT=需要加载注册照片和人脸特征 //DL_DEV_PIC|DL_DEV_USER_FEAT=Loading registration pictures and facial features
    int     m_ReqSource; //请求来源,详见485地址解析//Request source, please refer to parts for 485 address parsing
    BOOL    m_bApeendID; //TRUE=卡号, FALSE=工号//TRUE=card number, FALSE=work number
    DEV_CID m_ID;        //加载ID//Loading
} DEV_USER_REAL_LOAD;

//记录信息//Log Information
typedef struct
{
    DEV_REC_TYPE        m_RecType;       //记录类型//Record Type
    DEV_REC_SOURCE_TYPE m_Source;        //记录来源(已经过时,不再使用)//Source of Recording (obsolete)
    BOOL                m_bRealTime;     //是否为实时记录 //if it is real-time recording
    int                 m_Score;         //识别得分//Score of recognition
    int                 m_PicLen;        //照片大小//Picture Size
    int                 m_VerifyMode;    //验证模式,例如:DEV_VERIFY_USERID|DEV_VERIFY_CARD|DEV_VERIFY_FACE_11  //Modes of verification, For Example: DEV_VERIFY_USERID|DEV_VERIFY_CARD|DEV_VERIFY_FACE_11
    DEV_CID             m_ChangID;       //变更记录号//Change Record ID
    DEV_CID             m_SerialID;      //识别记录流水号ID//Serial ID for Recognition records
    DEV_CID             m_UserID;        //用户ID,如果为空，表示非法记录//User ID (if empty, it means illegal records)
    DEV_CID             m_AppendID;      //卡号或者其他用于1:1的附加身份确认信息//Card Num. or other or other appended info for 1:1 identifications
    DEV_CID             m_UserName;      //用户名//User Name
    DEV_CID             m_Department;    //部门名称//Department Name
    DEV_DATETIME        m_RecTime;       //记录时间//Record Time
    char                m_ConcretSource; //记录来源485地址，详见485地址解析//485 Address for record source, please refer to parts for 485 address parsing
    char*               m_PicData;       //原始JPG图像数据(未经base64编码)//Original JPG Image Data (without base 64 encodin
} DEV_RECORD;

typedef struct
{
    BOOL         m_bRTLog;    //是否为实时操作日志//If it is real-time operational log
    DEV_LOG_TYPE m_LogType;   //日志类型//Log Type
    DEV_CID      m_SerialID;  //流水号ID//Serial ID
    DEV_CID      m_Admin;     //操作员 //Operator
    DEV_CID      m_BeOptUser; //被操作员//Be operated user
    DEV_DATETIME m_RecTime;   //记录时间 //Record Ti
} DEV_LOG;

//变更注册照//Modify Registration Picture
typedef struct
{
    int          m_PicBytes;       //注册照大小//Registration Picture Size
    BOOL         m_bRTChangeImage; //是否为实时变更注册照//If it is real-time modified registration picture
    DEV_CID      m_UserID;         //用户ID//User ID
    DEV_CID      m_SerialID;       //流水号ID//Serial ID
    DEV_CID      m_AppendID;       //卡号或者其他用于1:1的附加身份确认信息//Card Num. or other or other appended info for 1:1 identifications
    DEV_CID      m_UserName;       //用户名//User Name
    DEV_CID      m_Department;     //部门名称//Department Name
    DEV_CID      m_Admin;          //人脸注册管理员,标识此用户的人脸特征是哪个管理员采集
    //Facial feature registration administrator, to mark which administrator collects the facial feature of certain user
    DEV_DATETIME m_RecTime;        //记录时间//Record Time
    char*        m_PicData;        //JPG图像数据(未经base64编码)//Original JPG Image Data (without base64 encodin
} DEV_CHANGEIMAGE;

//记录下载区间 //Downloading records interval
typedef struct
{
    int     m_Count;      //下载多少条//Amount of the downloading
    BOOL    m_bOpenRange; //是否为开区间(true=是， false=否)//it is an open interval? (true or false)
    DEV_CID m_SID;        //从那一条开始，m_SID表示记录精确的流水号 SID组成：SN + TYPE + DATE + ID = ( 6BYTE + 1BYTE + 8BYTE + 4BYTE + \0) //From which m_SID represents the exact serial number for records. SID：SN + TYPE + DATE + ID = ( 6BYTE + 1BYTE + 8BYTE + 4BYTE
} DEV_DL_RECORD_RANGE;

//记录下载区间集//Set of Intervals for downloading records
typedef struct
{
    int m_Count;
    DEV_DL_RECORD_RANGE* m_pRange;
} DEV_DL_RECORD_ARRAY;

//时间组 //Time Group
typedef struct
{
    int                m_TGID;      //时间组ID //Time group ID
    DEV_DATETIME       m_Start;     //时间组开始时间//Time group start time
    DEV_DATETIME       m_End;       //时间组结束时间//Time group stop time
    DEV_TIMEGROUP_TYPE m_TGType;    //时间组类型//Time group type
    char               m_CheckFlag; //时间检测标记例：m_CheckFlag = CHECK_TIME|CHECK_WEEK//Time check mark e.g.: m_CheckFlag = CHECK_TIME|CHECK_WEEK
    char               m_WeekFlag;  //检测星期时，标记那些星期有效。例：m_WeekFlag = WEEK_1|WEEK_5  //Check week, mark the effective weeks. For Example: m_WeekFlag = WEEK_1|WEEK
} DEV_TIMEGROUP;

//时间组数组//Time Group Array
typedef struct
{
    int m_nCount;
    DEV_TIMEGROUP* m_pTGArray;
} DEV_TIMEGROUP_ARRAY;

#define DEV_CHECK_HOLIDY      0x01     //权限检测节假日
#define DEV_ACTION_LOCK       0x02     //电锁输出
#define DEV_ACTION_OUTPUT     0x04     //辅助输出
#define DEV_ACTION_N_1        0x08     //该权限需要N+1

//权限//Access
typedef struct
{
    int  m_RightID;                     //权限ID // Access ID
    int  m_TimeGID[DEV_TIMEGROUP_NUMS]; //时间组ID, (m_TimeGID[0]==ANY_TIME)未指定时间组，开门方式不受时间限制，任意时间段验证成功执行开门方式. //Time Group ID, (m_TimeGID[0]==ANY_TIME)Unspecified time group, no time limitation for door open pattern, if verify successfully, door opens in anytime
    BOOL m_bHolidyValid;                //节假日是否有效//If holidays are effective
    BOOL m_bActionLock;                 //电锁输出//Electric lock output
    BOOL m_bActionOutPut;               //电锁辅助输出//Electric lock auxiliary ou
} DEV_RIGHT;

//权限数组//Access Array
typedef struct
{
    int m_nCount;
    DEV_RIGHT* m_pRtArray;
} DEV_RIGHT_ARRAY;

typedef struct
{
    int     m_GroupID;                       //组ID //Group ID
    int     m_NormalValid;                   //普通用户中有效用户数//Amount of effective users in normal users
    int     m_ForceValid;                    //强制用户中有效用户数//Amount of effective users in mandatory users
    DEV_CID m_NormalUsers[DEV_USER_COMBINS]; //普通用户组合，优先级低//Normal user combination, low priority
    DEV_CID m_ForceUsers[DEV_USER_COMBINS];  //强制用户组合，优先级高//Mandatory users combination, high priority
    BOOL    m_bGroupOrder;                   //组合是否有序 1有序，0无序 //If combination in order, 1=order, 0=no ord
} DEV_USERGROUP;

typedef struct
{
    int m_nCount;
    DEV_USERGROUP* m_pUGArray;
} DEV_USERGROUP_ARRAY; //用户组数组 //Array Starting Address

//程序更新结构体
//Program update Structures
typedef struct
{
    int m_Bytes; //file size
    int m_Type;  //file type definition is as below
    // 0  固件文件 firmware document
    // 1  待机界面文件 Standby interface document
    // 2  证书文件(.pk12) Certificate document (.pk12)
    DEV_CID m_CtxInfo;
    DEV_CID m_FileName; //文件名//File Name
    char*   m_Buf;      //文件数据缓存首地址//File data-caching starting address
} DEV_APPUPDATE;

//用户批量操作//Batch User Manipulations
typedef struct
{
    int m_nCount;
    DEV_USER* m_pUserArray;
} DEV_BATCH_USER;

//批量记录//Batch Record
typedef struct
{
    int m_nCount;
    DEV_RECORD* m_pRecordArray;
} DEV_BATCH_RECORD;

//批量日志//Batch Log
typedef struct
{
    int m_nCount;
    DEV_LOG* m_pLogArray;
} DEV_BATCH_LOG;

//批量注册照//Batch Registration Pictures
typedef struct
{
    int m_nCount;
    DEV_CHANGEIMAGE* m_pCImageArray;
} DEV_BATCH_CIMAGE;

//IO设备状态//IO device status
#define DEV_IO_MODE_NORMAL 0x00  //正常状态//Normal status
#define DEV_IO_MODE_OPEN   0x01  //常开状态//Open status
#define DEV_IO_MODE_CLOSE  0x02  //常关状态//Close status
//门磁状态
#define DEV_IO_STATE_CLOSE 0x00  //门磁关//Door magnet close
#define DEV_IO_STATE_OPEN  0x01  //门磁开//Door magnet open
//执行动作
#define DEV_ACT_IO_OPEN    0x02  //执行打开动作//Perform open action
#define DEV_ACT_IO_OPENEX  0x04  //执行辅助动作//Perform underaction
#define DEV_ACT_IO_CLOSE   0x10  //执行关闭动作//Perform close action
#define DEV_ACT_MODE_SET   0x20  //设置IO设备工作模式//Setup IO device work pattern
#define DEV_ACT_MODE_GET   0x40  //获得IO设备工作模式//Acquire IO device work pattern
#define DEV_ACT_STATE_GET  0x80  //获得IO设备当前状态//Acquire IO device current stat
typedef struct
{
    char m_Source; //IO设备485地址//IO device 485 address
    char m_Action; //执行动作//Perform action
    char m_IOMode; //IO设备当前模式//IO device current pattern
    char m_IOState;//IO设备当前状态状态 //IO device current status
} DEV_IOCTRL;

//NOPEN:远程请求管理端开门
//NOPEN:Request open door remotely through Administrative End
typedef struct
{
    int                              m_CtxID; //m_Users==1权限ID, m_Users>1组合ID//m_Users==1 access ID, m_Users>1 combined ID
    int                              m_Users; //验证用户数目： 0密码开门，1权限开门，>1组合开门 //Amount of verified users: 0 open with password，1 open with access，>1 combined way to open
    DEV_IOCTRL                       m_XOpen; //控制信息//Control information
    DEV_CID m_IDS[DEV_USER_COMBINS * 2];      //验证用户数组//Verify user arr
} DEV_NOPEN;

//远程采集进度信息//Remote collection schedule information
typedef struct
{
    int     m_Total;  //采集总数//collection sum
    int     m_Count;  //当前采集数//collection number at present
    DEV_CID m_UserID; //用户登记编号//User registration serial numb
} DEV_RCAPINFO;

#define DEV_WEB_LOADUSER         0x01 //加载用户//Loading users
#define DEV_WEB_WRITEUSER        0x02 //写用户//write user
#define DEV_WEB_WRITEREC         0x04 //写记录//write record
#define DEV_WEB_SYNCTIME         0x08 //同步时间//synchronization time
#define DEV_WEB_JUDGE            0x10 //识别成功后，由服务端权限验证
#define DEV_WEB_RECORD_TXT       0x20 //记录自动上传时，只上传文字信息
#define DEV_WEB_ENABLE           0x80 //
//网络属性//Network property
//不能设置ipv6地址
typedef struct
{
    char m_WebPro1;
    char m_WebPro2;
    char m_WebUrl1[URL_LEN];
    char m_WebUrl2[URL_LEN];
    char m_IPAdr[IP_LEN];
    char m_MIPAdr[IP_LEN];
    char m_NetMask[IP_LEN];
    char m_GateWay[IP_LEN];
    char m_Dns[IP_LEN];
} DEV_NETPRO;

////////////////////////////////////////////////////////////////////////////////////////
//功能: 设备连接提醒
//参数:
//cszDevAddr 设备地址
//eCNStatus 连接状态
//pvContext 应用上下文
//说明:
//返回 <0 放弃连接 0不加密，1加密服务器端, 2加密客户端
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: Connection warning
//Parameter:
//cszDevAddr Address
//eCNStatus Connection status
//pvContext parameter for context
//Comment:
//return <0 stop connecting 0 no encryption，1 Encrypt server-side, 2 Encrypt terminal
////////////////////////////////////////////////////////////////////////////////////////
typedef int (SLGDEV_CALL* DevConnectStatus)(const char* cszDevAddr, DEV_CONNECT_STATUS eCNStatus, void* pvContext);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 查询设备中转服务器归属，返回设备所属中转服务器IP地址
//参数:
//cszDevAddr 设备地址
//eCNStatus 返回的中转服务器IP
//pvContext 应用上下文
//说明:
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function:  inquire about the transfer server which the current device belongs to and return the IP address of the server
//Parameter:
//cszDevAddr Device address
//eCNStatus IP address of transfer server to be returned
//pvContext parameter for context
//Comment:
////////////////////////////////////////////////////////////////////////////////////////
typedef BOOL (SLGDEV_CALL* QuerySrvCB)(const char* cszDevAddr, SOCK_ADDRESS* srvAddr, void* pvContext);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 设备操作结果回调
//参数: cszDevAddr 设备地址
//eType 操作类型
//eFlag 操作标识
//pvContent 操作结果内容
//nSeq 操作流水号
//eResult 操作结果
//pvContext 应用层上下文
//说明: 上层应用收到此消息后即可知道前一次操作是否成功
//eType、eFlag、eResult决定pvContent内容
////////////////////////////////////////////////////////////////////////////////////////
//Function: Call-back the device operation results
//Parameter: cszDevAddr Device address
//eType operational type
//eFlag operational flag
//pvContent contents of operation results
//nSeq Serial Number of operations
//eResult operation results
//pvContext context
//Comment: The upper-layer applications will know if the prev. operation has succeeded by receiving the message.
//eType、eFlag、eResult determine the content of pvContent
////////////////////////////////////////////////////////////////////////////////////////
//当eResult=OPER_SUCCESS && eFlag=DEV_OPER_DOWNLOAD时，pvContent != NULL, eType操作类型解析如下
//DEV_AUTH_OPER pvContent=NULL 客户端身份验证反馈
//DEV_AUTH_SET_OPER pvContent=NULL 客户端连接用户名密码设置成功反馈
//DEV_REGION_OPER pvContent=NULL
//DEV_FORMAT_OPER pvContent=NULL
//DEV_UPDATE_OPER pvContent=NULL
//DEV_SYS_TIME_OPER pvContent=DEV_DATETIME* or = NULL
//DEV_BASEINFO_OPER pvContent=DEV_BASEINFO* or = NULL
//DEV_STATISINFO_OPER pvContent=DEV_STATIS* or = NULL
//DEV_WORKATT_OPER pvContent=DEV_WORKATT* or = NULL
//DEV_USER_OPER pvContent=DEV_USER* or = NULL
//DEV_USER_RT_OPER pvContent=DEV_USER_REAL_LOAD*
//DEV_USER_RANGE_OPER pvContent=DEV_DL_USER_RANGE* or = NULL
//DEV_RECORD_OPER pvContent=DEV_RECORD* or = NULL
//DEV_RECORD_RANGE_OPER pvContent=DEV_DL_RECORD_RANGE* or = NULL
//DEV_LOG_OPER pvContent=DEV_LOG* or = NULL
//DEV_LOG_RANGE_OPER pvContent=DEV_DL_RECORD_RANGE* or = NULL
//DEV_CHANGEIMAGE_OPER pvContent=DEV_CHANGEIMAGE* or = NULL
//DEV_CIMAGE_RANGE_OPER pvContent=DEV_DL_RECORD_RANGE* or = NULL
//DEV_TIME_GROUP_OPER pvContent=DEV_TIMEGROUP_ARRAY* or = NULL
//DEV_RIGHT_OPER pvContent=DEV_RIGHT_ARRAY* or = NULL
//DEV_USERGROUP_OPER pvContent=DEV_USERGROUP_ARRAY* or = NULL
//DEV_DOOR_STATE_OPER pvContent=DEV_DOOR_STATE* or = NULL
//DEV_NOPEN_OPER pvContent=DEV_NOPEN* 设备请求管理端远程开门
//DEV_IOCTRL_OPER pvContent=DEV_IOCTRL* IO控制信息反馈
//CLI_AUTH_OPER pvContent=AUTH* nSeq代表client端口
////////////////////////////////////////////////////////////////////////////////////////
typedef BOOL (SLGDEV_CALL* DevOperResultNotify)(const char* cszDevAddr, DEV_OPER_TYPE eType,
        DEV_OPER_FLAG eFlag, void* pvContent, int nSeq,
        DEV_OPER_RESULT eResult, void* pvContext);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 初始化于反初始化系统
//参数: bFlag true=初始化,false=反初始化
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: System initialization and anti-initialization
//Parameter: bFlag true=initialization,false=anti-initialization
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_InitSys(BOOL bFlag);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 注册设备连接状态提醒回调
//参数: pfnCNNotify 回调函数指针
//pvContext 应用上下文
//说明:
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: Call-back the registered device connection status warning
//Parameter: pfnCNNotify Callback function pointer
//pvContext context
//Comment:
////////////////////////////////////////////////////////////////////////////////////////
void SLGDEV_CALL SLG_RegDevConnectStatusCB(DevConnectStatus pfnCNNotify, void* pvContext);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 注册设备操作结果通知
//参数: pfnOperNotify 回调函数指针
//pvContext 应用上下文
//说明:
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: notification of results of the registered device operations
//Parameter: pfnOperNotify callback function pointer
//pvContext context
//Comment:
////////////////////////////////////////////////////////////////////////////////////////
void SLGDEV_CALL SLG_RegOperResultNotifyCB(DevOperResultNotify pfnOperNotify, void* pvContext);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 设置整个模块为中转模式
//参数:
//srvMode 工作模式 TRUE 中转模式， FALSE本地模式
//pfn 中转服务查询回调函数
//pvContext 应用上下文
//说明: 不调用该函数，默认为本地模式
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: configure the operation mode for the whole module (transfer mode)
//Parameter:
//srvMode work pattern TRUE transfer mode， FALSE local mode
//pfn  the call-back inquiry function for transfer service
//pvContext context
//Comment: The default mode is the local mode if the function is not called.
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_SetMode(BOOL srvMode, QuerySrvCB pfn, void* pvContext);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 连接到中转服务器
//参数:
//bFlag true=连接中转服务器,false=断开与中转服务器的连接
//ip 中转服务器IP地址
//name client验证用户名
//psw client验证密码
//说明: 中转服务器连接成功返回TRUE，否则FALSE。身份验证成功与否通过DevOperResultNotify回调通知外部.
//只要调用该函数，不管中转服务器连接成功与否，模式自动切换为中转模式，显示调用 SLG_CnSrv(FALSE,NULL,NULL,NULL)
//才能切换到本地模式.不调用该函数为本地模式
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: connect to the transfer server
//Parameter:
//bFlag true=connect with transfer server  false=disconnect with transfer server
//ip IP address of the transfer server
//name client  verification of client name
//psw client verification of client password
//Comment:  the transfer server will return TRUE for successful connection and FALSE for unsuccessful connection. The status of the verification (passed or failed) will be notified through the call-back function DevOperResultNotify.
//If the function is called, the mode will change to transfer mode automatically and shows calling SLG_CnSrv (FALSE,NULL,NULL,NULL),without concerning the connection status of the server.
//If the function is not called, it will be the local mode.
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_CnSrv(BOOL bFlag, const char* ip, const char* name, const char* psw);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 启动设备搜索与否
//参数: bFlag (true=启动 false=停止)
//cszRgnAddr 设备所在区域地址
//说明: 以下3个函数，当前版本中暂时不使用
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: If starting search for devices
//Parameter: bFlag (true=start false=stop)
//cszRgnAddr address of the interval the device is in
//Comment: These 3 functions cannot be used in this current version
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_StartDevSerch(BOOL bFlag, const char* cszRgnAddr = DEV_REGION_ADDR);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 与设备建立连接
//参数: cszDevAddr 设备地址
//说明:
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: Connecting with the facial recog. Device
//Parameter: cszDevAddr Device address
//Comment:
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_CNDev(const char* cszDevAddr);
////////////////////////////////////////////////////////////////////////////////////////
//功能: 断开设备连接
//参数: cszDevAddr 设备地址
//说明:
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: Disconnecting
//Parameter: cszDevAddr Device address
//Comment:
////////////////////////////////////////////////////////////////////////////////////////
void SLGDEV_CALL SLG_DCNDev(const char* cszDevAddr);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 设备验证或设置验证
//参数: cszDevAddr设备地址
//cName 用户名
//cPws 密码
//nFlag 1=验证 2=设置
//说明:
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: Device verification or setup verification
//Parameter: cszDevAddrDevice address
//cName user name
//cPws password
//nFlag 1=verification 2=setup
//Comment:
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_ULDevAuth(const char* cszDevAddr, const DEV_CID& cName, const DEV_CID& cPws, int nFlag, int nSeq = -1);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 设置设备所在区域地址
//参数:
//cszDevAddr 设备地址
//cszRgnAddr 设备区域地址
//说明: 当设备建立连接后可将设备设置到某一个区域，以后及可对这个区域进行搜索设备
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: setup interval address of the device
//Parameter:
//cszDevAddr Device address
//cszRgnAddr interval address of the device
//Comment: to set the device to certain interval after the connection, and it can be searched under this interval
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_ULDevRegionAddr(const char* cszDevAddr, const char* cszRgnAddr = DEV_REGION_ADDR, int nSeq = -1);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 设置设备系统时间
//参数:
//cszDevAddr 设备地址
//cSysTime 时间信息
//说明: DevOperResultNotify回调参数对应类型：
//eType = DEV_SYS_TIME_OPER
//eFlag = DEV_OPER_UPLOAD ，
//pvContent = NULL
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: Configuration of System Time
//Parameter:
//cszDevAddr Device address
//cSysTime Time info
//Comment: DevOperResultNotify Callback parameter corresponding type:
//eType = DEV_SYS_TIME_OPER
//eFlag = DEV_OPER_UPLOAD ，
//pvContent = NULL
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_ULDevTime(const char* cszDevAddr, const DEV_DATETIME& cSysTime, int nSeq = -1);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 获取设备系统时间
//参数: cszDevAddr 设备地址
//说明: DevOperResultNotify回调参数对应类型：
//eType = DEV_SYS_TIME_OPER
//eFlag = DEV_OPER_DOWNLOAD，
//pvContent = DEV_DATETIME*
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: Get the System Time
//Parameter: cszDevAddr Device address
//Comment: DevOperResultNotify Callback parameter corresponding type:
//eType = DEV_SYS_TIME_OPER
//eFlag = DEV_OPER_DOWNLOAD，
//pvContent = DEV_DATETIME*
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_DLDevTime(const char* cszDevAddr, int nSeq = -1);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 获取设备基本信息
//参数: cszDevAddr 设备地址
//说明:
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_DLDevBaseInfo(const char* cszDevAddr, int nSeq = -1);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 获取设备统计信息
//参数: cszDevAddr 设备地址
//说明:
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: Get basic information
//Parameter: cszDevAddr Device address
//Comment:
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_DLDevStatisInfo(const char* cszDevAddr, int nSeq = -1);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 获取设备工作属性信息
//参数:
//cszDevAddr 人脸机地址
//nSeq 操作序列号
//说明: 工作属性DEV_WORKATT通过回调函数给出。
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: Get Device work attribute information
//Parameter:
//cszDevAddr Device address
//nSeq Operation Serial Num
//Comment: Work attribute DEV_WORKATT  is given by callback funtion
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_DLDevWorkAttInfo(const char* cszDevAddr, int nSeq = -1);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 设置设备工作属性信息
//参数:
//cszDevAddr 设备地址
//cWorkAtt 工作属性结构体
//nSeq 操作序列号
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: Device work attribute information setup
//Parameter:
//cszDevAddr Device address
//cWorkAtt Work attribute structure
//nSeq Operation serial number
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_ULDevWorkAttInfo(const char* cszDevAddr, const DEV_WORKATT& cWorkAtt, int nSeq = -1);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 上传指定用户
//参数:
//cszDevAddr 设备地址
//cUser 用户信息结构体
//nSeq 操作序列号
//说明:
//用户无人脸库时，DEV_USER中的 m_FeatLen = 0， m_FeatData = NULL
//用户无注册照时，DEV_USER中的 m_PicLen= 0， m_PicData = NULL
//DevOperResultNotify回调参数对应类型：eType = DEV_USER_OPER
//eFlag = DEV_OPER_UPLOAD，pvContent= NULL
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: Uploading specific user
//Parameter:
//cszDevAddr Device address
//cUser user information structure
//nSeq Operation serial number
//Comment:
//User without face library ，DEV_USER: m_FeatLen = 0， m_FeatData = NULL
//User without registration picture，DEV_USER: m_PicLen= 0， m_PicData = NULL
//DevOperResultNotify Callback parameter corresponding type: eType = DEV_USER_OPER
//eFlag = DEV_OPER_UPLOAD，pvContent= NULL
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_ULUser(const char* cszDevAddr, const DEV_USER& cUser, int nSeq = -1);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 上传实时加载用户
//参数:
//cszDevAddr 设备地址
//cUser 用户信息结构体
//cRtLoad 实时加载信息
//nSeq 操作序列号
//说明: 设备端请求客户端上传某用户时，通过此接口上传所请求用户信息，
//cRtLoad为设备端请求时传过来的信息，客户端禁止修改。此函数实现远程用户加载功能。
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: Uploading real-time user
//Parameter:
//cszDevAddr Device address
//cUser user info structure
//cRtLoad loading information in real time
//nSeq Operation Serial Num
//Comment: If the device requests the terminal to upload certain user, it uses this port to upload the requested user information.
//cRtLoad is the information sent from the device, terminal forbids to modify. This function allows to loading user remotely.
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_ULRealTimeUser(const char* cszDevAddr, const DEV_USER& cUser, const DEV_USER_REAL_LOAD& cRtLoad);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 远程实时采集用户特征
//参数:
//cszDevAddr 设备地址
//cUser 用户信息
//说明: 管理端录入用户资料时，可通过该函数将用户资料发送的人脸机，控制人脸机采集该用户的人脸。
//远程用户采集功能，使得直接在管理端就可以采集用户，在配合远程视频的情况下采集的可视化程度和直接在机器上采集一样。
//如果使能了(用户采集实时同步)。远程采集的用户也会实时的发回管理端.
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: Remotely collect user features in real-time
//Parameter:
//cszDevAddr Device address
//cUser User Info
//Comment: The user information input via administrative end can be sent to the terminal
// by this function, then to control the terminal to collect the user’s facial features.
//The remote user collection function allows the administrative end to collect users,
// the collection visualization via remote video is the same as via the terminal.
//If enabling the User Collection Real-time Synchronization, the users that are
// collected remotely will be sent to administrative end in real-time, too.
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_ULRealTimeUserCap(const char* cszDevAddr, const DEV_USER& cUser, int nSeq = -1);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 删除指定用户
//参数:
//cszDevAddr 设备地址
//cID 用户编号
//nSeq 操作序列号
//说明: 删除成功与否，通过操作回调函数给出结果
//DevOperResultNotify回调参数对应类型：
//eType = DEV_USER_OPER
//eFlag = DEV_OPER_DELETE，
//pvContent = NULL
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function:  Deleting specific user
//Parameter:
//cszDevAddr Device address
//cID User ID
//nSeq Operation Serial Num
//Comment: Deleting or not，manipulating callback function to Acquire result
//DevOperResultNotify Callback parameter corresponding type:
//eType = DEV_USER_OPER
//eFlag = DEV_OPER_DELETE，
//pvContent = NULL
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_DELUser(const char* cszDevAddr, const DEV_CID& cID, int nSeq = -1);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 下载指定用户
//参数:
//cszDevAddr 设备地址
//cID 用户ID
//nFlag DL_DEV_USER_PIC 需下载用户照片
//DL_DEV_USER_FEAT 需要下载人脸特征
//DL_DEV_USER_PIC|DL_DEV_USER_FEAT=两者
//为0默认下载文字信息
//nSeq 操作序列号
//说明: DevOperResultNotify回调参数对应类型：
//eType = DEV_USER_OPER
//eFlag = DEV_OPER_DOWNLOAD，
//pvContent = DEV_USER*
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: Downloading specific user
//Parameter:
//cszDevAddr Device address
//cID user ID
//nFlag DL_DEV_USER_PIC needs to downloading user picture
// DL_DEV_USER_FEAT  needs to downloading facial features
// DL_DEV_USER_PIC|DL_DEV_USER_FEAT=both
// =0 means to downloading text messages
//nSeq Operation Serial Num
//Comment: DevOperResultNotify Callback parameter corresponding type:
//eType = DEV_USER_OPER
//eFlag = DEV_OPER_DOWNLOAD，
//pvContent = DEV_USER*
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_DLSingleUser(const char* cszDevAddr, const DEV_CID& cID, int nFlag, int nSeq = -1);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 下载所有用户
//参数: cszDevAddr 设备地址
//说明:
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: Batch Downloading Users
//Parameter: cszDevAddr Device address
//Comment:
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_DLAllUser(const char* cszDevAddr, int nSeq = -1);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 下载某一时间段内的注册用户
//参数:
//cszDevAddr 设备地址
//cStart 起始日期
//cEnd 终止日期
//说明:
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: Downloading users registered in certain time
//Parameter:
//cszDevAddr Device address
//cStart start date
//cEnd  end date
//Comment:
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_DLSegTimeUser(const char* cszDevAddr, const DEV_DATE& cStart, const DEV_DATE& cEnd, int nSeq = -1);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 下载识别记录区间
//参数:
//cszDevAddr 设备地址
//nSeq 操作序列
//说明: DevOperResultNotify回调参数对应类型：
//eType = DEV_RECORD_OPER
//eFlag = DEV_OPER_DOWNLOAD
//pvContent = DEV_DL_RECORD_ARRAY*
//参考DEV_DL_RECORD_ARRAY结构，一天为一个区间，该函数返回识别记录的所有区间。
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: Downloading identification record interval
//Parameter:
//cszDevAddr Device address
//nSeq Operation sequence
//Comment: DevOperResultNotify Callback parameter corresponding type:
//eType = DEV_RECORD_OPER
//eFlag = DEV_OPER_DOWNLOAD
//pvContent = DEV_DL_RECORD_ARRAY*
//Refer to DEV_DL_RECORD_ARRAY structure，one day is a interval, this function returns to all intervals of identification record.
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_DLAllIdentifyRecord(const char* cszDevAddr, int nSeq = -1);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 下载指定时间段识别记录
//参数: cszDevAddr 设备地址
//cStart 起始日期
//cEnd 终止日期
//说明:
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: Downloading specific identification record interval
//Parameter: cszDevAddr Device address
//cStart start date
//cEnd end date
//Comment:
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_DLSegTimeIdentifyRecord(const char* cszDevAddr, const DEV_DATE& cStart, const DEV_DATE& cEnd, int nSeq = -1);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 获取区间识别记录/报警记录指定ID的后续区间
//参数:
//cszDevAddr 设备地址
//cRange ID 区间
//flag DL_DEV_RECORD_PIC=需要下载记录照片
//bKnowRange true=具体数据， false=区间集合
//nSeq 操作序列
//说明: 当bKnowRange 为true该函数下载具体的数据，为false时查询指定区间在设备上的具体大小。
//DevOperResultNotify回调参数对应类型：
//下载区间时
//pvContent = DEV_DL_RECORD_ARRAY*
//cRange.m_SID = 指定SN
//cRange.m_Count = -1;
//cRange.m_bOpenRange = TRUE;
//bKnowRange = FALSE，flag = 1（无效参数）
//下载具体记录时
//DevOperResultNotify回调参数对应类型：
//pvContent = DEV_RECORD*
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: Get interval identification record / follow-up interval of alarm record of appointed ID
//Parameter:
//cszDevAddr Device address
//cRange ID interval
//flag DL_DEV_RECORD_PIC=Downloading record picture
//bKnowRange true=specific data, false=interval set
//nSeq Operation sequence
//Comment:  If bKnowRange is true, this function downloads specific data, if false, it queries the specific interval size of the device.
// DevOperResultNotify Callback parameter corresponding type:
//Downloading interval
//pvContent = DEV_DL_RECORD_ARRAY*
//cRange.m_SID = appointed SN
//cRange.m_Count = -1;
//cRange.m_bOpenRange = TRUE;
//bKnowRange = FALSE，flag = 1（invalid parameter）
//Downloading specific record
//DevOperResultNotify Callback parameter corresponding type:
//pvContent = DEV_RECORD*
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_DLRangeRec(const char* cszDevAddr, const DEV_DL_RECORD_RANGE& cRange, int nFlag, BOOL bKnowRange = FALSE, int nSeq = -1);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 下载报警记录区间
//参数:
//cszDevAddr 设备地址
//nSeq 操作序列
//说明: DevOperResultNotify回调参数对应类型：
//eType = DEV_ALARM_OPER
//eFlag = DEV_OPER_DOWNLOAD，
//pvContent = DEV_DL_RECORD_ARRAY*
//参考DEV_DL_RECORD_ARRAY结构，报警记录只有一个区间
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: Downloading interval alarm record
//Parameter:
//cszDevAddr Device address
//nSeq Operation sequence
//Comment: DevOperResultNotify Callback parameter corresponding type:
//eType = DEV_ALARM_OPER
//eFlag = DEV_OPER_DOWNLOAD，
//pvContent = DEV_DL_RECORD_ARRAY*
//Refer to DEV_DL_RECORD_ARRAY structure，the alarm record has only one interval
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_DLAllAlarmRecord(const char* cszDevAddr, int nSeq = -1);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 下载指定时间段报警记录
//参数: cszDevAddr 设备地址
//cStart 起始日期
//cEnd 终止日期
//说明:
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: Downloading specific time interval alarm record
//Parameter: cszDevAddr Device address
//cStart start date
//cEnd end date
//Comment:
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_DLSegTimeAlarmRecord(const char* cszDevAddr, const DEV_DATE& cStart, const DEV_DATE& cEnd, int nSeq = -1);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 下载区间报警记录
//参数: cszDevAddr 设备地址
//cRange ID区间
//flag DL_DEV_RECORD_PIC=需要下载记录照片 0x00=不下载照片, 0x01下载照片
//bKnowRange 指定是下载具体数据还是下载ID集合(true=具体数据， false=ID集合)
//说明: 此接口用于从某一条精确的记录ID开始下载nRang条数据
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: downloading interval alarm record
//Parameter: cszDevAddr Device address
//cRange ID interval
//flag DL_DEV_RECORD_PIC= need to download record picture 0x00= not download picture, 0x01download picture
//bKnowRange to appoint to download specific data or ID set (true=specific data， false=ID set)
//Comment: this port is used to download from a specific record to download nRang data
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_DLRangeAlarm(const char* cszDevAddr, const DEV_DL_RECORD_RANGE& cRange, int nFlag, BOOL bKnowRange = FALSE, int nSeq = -1);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 下载所有日志
//参数: cszDevAddr 设备地址
//说明:
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: Download all logs
//Parameter: cszDevAddr Device address
//Comment:
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_DLLog(const char* cszDevAddr, int nSeq = -1);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 下载区间日志
//参数: cszDevAddr 设备地址
//cRange ID区间
//bKnowRange 指定是下载具体数据还是下载ID集合(true=具体数据， false=ID集合)
//说明: 此接口用于从某一条精确的记录ID开始下载nRang条数据
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: Download interval logs
//Parameter: cszDevAddr Device address
//cRange ID interval
//bKnowRange to appoint to download specific data or ID set (true=specific data， false=ID set)
//Comment: this port is used to download from a specific record ID to download nRang data
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_DLRangeLog(const char* cszDevAddr, const DEV_DL_RECORD_RANGE& cRange, BOOL bKnowRange = FALSE, int nSeq = -1);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 获取变更注册照记录区间
//参数:
//cszDevAddr 设备地址
//nSeq 操作序列
//说明:
//DevOperResultNotify回调参数对应类型：
//eType = DEV_CHANGEIMAGE_OPER
//eFlag = DEV_OPER_DOWNLOAD，
//pvContent = DEV_DL_RECORD_ARRAY*
//参考DEV_DL_RECORD_ARRAY结构，变更注册照只有一个区间
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Acquire the record interval of modified registration pictures
//Parameter:
//cszDevAddr Device address
//nSeq Operation sequence
//Comment:
//DevOperResultNotify Callback parameter corresponding type:
//eType = DEV_CHANGEIMAGE_OPER
//eFlag = DEV_OPER_DOWNLOAD，
//pvContent = DEV_DL_RECORD_ARRAY*
//Refering to DEV_DL_RECORD_ARRAY structure, the modified registration pictures have only one interval
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_DLChangeImage(const char* cszDevAddr, int nSeq = -1);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 下载指定区间的变更注册照记录或者查询区间数据的具体大小
//参数:
//cszDevAddr 设备地址
//cRange 区间, 参考区间定义
//flag DL_DEV_RECORD_PIC=需要下载记录照片
//bKnowRange true=具体数据， false=区间集合
//nSeq 操作序列号
//说明: 当bKnowRange 为true该函数下载具体的数据，为false时查询指定区间在设备上的具体大小。
//下载区间时
//DevOperResultNotify回调参数对应类型：
//pvContent = DEV_DL_RECORD_ARRAY*
//cRange.m_SID = 指定SN
//cRange.m_Count = -1;
//cRange.m_bOpenRange = TRUE;
//bKnowRange = FALSE，flag = 1（无效参数）
//下载具体记录时
//DevOperResultNotify回调参数对应类型：
//pvContent=DEV_RECORD*
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: Downloading the record of modified registration pictures in specific interval or querying interval data size
//Parameter:
//cszDevAddr Device address
//cRange  Interval, refer to the definition of interval
//flag DL_DEV_RECORD_PIC=download recording pictures
//bKnowRange true=specific data， false=set of interval
//nSeq  Operation sequence
//Comment: If bKnowRange is true, the function downloading specific data, if it is false, the function querying the real size of the specific interval in the device.
//Downloading interval
//DevOperResultNotify Callback parameter corresponding type:
//pvContent = DEV_DL_RECORD_ARRAY*
//cRange.m_SID = appointed SN
//cRange.m_Count = -1;
//cRange.m_bOpenRange = TRUE;
//bKnowRange = FALSE，flag = 1（Invalid parameter）
//Downloading specific record
//DevOperResultNotify Callback parameter corresponding type:
//pvContent=DEV_RECORD*
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_DLRangeCImage(const char* cszDevAddr, const DEV_DL_RECORD_RANGE& cRange, int nFlag, BOOL bKnowRange = FALSE, int nSeq = -1);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 设置时间组
//参数: cszDevAddr 设备地址
//cTGArray 时间组
//说明:
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: Set time group
//Parameter: cszDevAddr Device address
//cTGArray time group
//Comment:
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_ULTimeGroup(const char* cszDevAddr, const DEV_TIMEGROUP_ARRAY& cTGArray, int nSeq = -1);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 获取时间组
//参数: dev_addr 设备地址
//cTGArray 时间组
//eType 时间组类型
//说明: cTGArray.m_nCount=0时表示下载eType类型的所有时间组
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: Get time group
//Parameter: dev_addr Device address
//cTGArray time group array
//eType time group type
//Comment: cTGArray.m_nCount=0 means to download all the eType time groups
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_DLTimeGroup(const char* cszDevAddr, const DEV_TIMEGROUP_ARRAY& cTGArray, DEV_TIMEGROUP_TYPE eType, int nSeq = -1);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 删除时间组
//参数: cszDevAddr 设备地址
//cTGArray 时间组
//eType 时间组类型
//说明: eType.m_nCount=0时表示删除tg_type类型所有时间组
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: Deleting time group
//Parameter: cszDevAddr Device address
//cTGArray Time group
//eType  Time group type
//Comment: eType.m_nCount=0 means deleting all tg_type time groups
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_DELTimeGroup(const char* cszDevAddr, const DEV_TIMEGROUP_ARRAY& cTGArray, DEV_TIMEGROUP_TYPE eType, int nSeq = -1);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 设置权限
//参数: cszDevAddr 设备地址
//cRTArray 权限
//说明:
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: Set access
//Parameter: cszDevAddr Device address
//cRTArray Access
//Comment:
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_ULRight(const char* cszDevAddr, const DEV_RIGHT_ARRAY& cRTArray, int nSeq = -1);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 发送获取权限
//参数: cszDevAddr 设备地址
//cRTArray 权限
//说明: 当rt_array.m_nCount=0时表示下载所有权限
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: sending acquire access
//Parameter: cszDevAddr Device address
//cRTArray access
//Comment:  rt_array.m_nCount=0 means downloading all access
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_DLRight(const char* cszDevAddr, const DEV_RIGHT_ARRAY& cRTArray, int nSeq = -1);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 删除权限
//参数: cszDevAddr 设备地址
//cRTArray 权限
//说明: 当rt_array.m_nCount=0时表示删除所有权限
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: Deleting access
//Parameter: cszDevAddr Device address
//cRTArray  Access
//Comment: rt_array.m_nCount=0 means deleting all access
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_DELRight(const char* cszDevAddr, const DEV_RIGHT_ARRAY& cRTArray, int nSeq = -1);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 设置用户组
//参数: cszDevAddr 设备地址
//cUGrray 用户组
//说明:
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: setup user group
//Parameter: cszDevAddr Device address
//cUGrray user group
//Comment:
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_ULUserGroup(const char* cszDevAddr, const DEV_USERGROUP_ARRAY& cUGrray, int nSeq = -1);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 发送获取用户组
//参数: cszDevAddr 设备地址
//ug_array 用户组
//说明: 当rt_array.m_nCount=0时表示下载所有用户组
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: sending acquire user group
//Parameter: cszDevAddr Device address
//ug_array user group
//Comment: rt_array.m_nCount=0 means downloading all user groups
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_DLUserGroup(const char* cszDevAddr, const DEV_USERGROUP_ARRAY& cUGrray, int nSeq = -1);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 删除用户组
//参数: cszDevAddr 设备地址
//ug_array 用户组
//说明: 当rt_array.m_nCount=0时表示删除所有用户组
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function:  Deleting user group
//Parameter: cszDevAddr Device address
//ug_array user group
//Comment: rt_array.m_nCount=0 means deleting all user groups
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_DELUserGroup(const char* cszDevAddr, const DEV_USERGROUP_ARRAY& cUGrray, int nSeq = -1);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 设置开关门状态
//参数:
//cszDevAddr 设备地址
//bEOpen true=紧急开门， false=紧急关门
//说明:
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: Set door open&close status
//Parameter:
//cszDevAddr Device address
//bEOpen true=urgent open， false=urgent close
//Comment:
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_ULOpenDoorState(const char* cszDevAddr, DEV_DOOR_STATE eState, int nSeq = -1);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 获取开关门状态
//参数: cszDevAddr 设备地址
//bEOpen true=紧急开门， false=紧急关门
//说明:
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: acquire door status open/close
//Parameter: cszDevAddr Device address
//bEOpen true=urgent open， false=urgent close
//Comment:
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_DLOpenDoorState(const char* cszDevAddr, int nSeq = -1);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 远程开门
//参数: cszDevAddr 设备地址
//bEOpen true=开门， false=关门
//pDoorArray 门点数组
//size 数组大小
//说明:
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: Remote Open Door
//Parameter: cszDevAddr Device address
//bEOpen true=Open，false=Close
//pDoorArray door point array
//size array size
//Comment:
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_ULRemoteOpenDoor(const char* cszDevAddr, BOOL bOpen, int nSeq = -1);

////////////////////////////////////////////////////////////////////////////////////////
//功能: IO设备控制
//参数:
//cszDevAddr 设备地址
//DEV_IOCTRL 控制信息
//nSeq 命令执行序列号
//说明: DEV_IOCTRL:m_Source指定要做操作的端口，DEV_IOCTRL:m_Action要执行的动作
//打开门1，m_Source = 0x1f m_Action = DEV_ACT_IO_OPEN
//打辅助1，m_Source = 0x15 m_Action = DEV_ACT_IO_OPEN
//0x1f和015具体怎么解析的，请参考485地址解析
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: IO device control
//Parameter:
//cszDevAddr Device address
//DEV_IOCTRL control information
//nSeq order execute serial number
//Comment: DEV_IOCTRL:m_Source to point operational port，DEV_IOCTRL:m_Action to operate actions
//Open door 1，m_Source = 0x1f m_Action = DEV_ACT_IO_OPEN
//Open auxiliary 1，m_Source = 0x15 m_Action = DEV_ACT_IO_OPEN
//Refer to 485 address resolution to resolute 0x1f and 015
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_IOCtrl(const char* cszDevAddr, const DEV_IOCTRL& ioctrl, int nSeq = -1);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 格式化设备
//参数: cszDevAddr 设备地址
//说明:
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: Format the device
//Parameter: cszDevAddr Device address
//Comment:
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_ULFormat(const char* cszDevAddr, int nSeq = -1);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 设备程序更新
//参数: cszDevAddr 设备地址
//cAppData 程序更新结构体
//说明: 返回 0成功  1网络发送失败 2 根证书验证失败
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: device program update
//Parameter: cszDevAddr Device address
//cAppData program update structure
//Comment:
////////////////////////////////////////////////////////////////////////////////////////
int SLGDEV_CALL SLG_ULUpdate(const char* cszDevAddr, const DEV_APPUPDATE& cAppData, int nSeq = -1);
////////////////////////////////////////////////////////////////////////////////////////
//功能: 启动视频
//参数: cszDevAddr 设备地址
//hwnd 视频显示窗口句柄
//说明:
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: Start Video
//Parameter: cszDevAddr Device address
//hwnd Video display window handle
//Comment:
////////////////////////////////////////////////////////////////////////////////////////
typedef struct VideoPro_ VideoPro;
typedef int (SLGDEV_CALL* VideoPro_CallBack)(const VideoPro* pro);
struct VideoPro_
{
    SOCKET m_hVideoHandle;
    unsigned char* m_pRGBTMP;
    unsigned char* m_pRGBTRANS;
    void* m_hWnd;
    int biWidth;
    int biHeight;
    addr_in m_Addr;
    VideoPro_CallBack VideoCB;
};
BOOL SLGDEV_CALL SLG_StartVideo(const char* cszDevAddr, void* hwnd, VideoPro_CallBack VideoCB, int nSeq = -1);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 停止视频
//参数: cszDevAddr 设备地址
//说明:
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: Stop video
//Parameter: cszDevAddr Device address
//Comment:
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_StopVideo(const char* cszDevAddr, int nSeq = -1);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 批量上传用户
//参数:
//cszDevAddr 设备地址
//cUsers 批量用户数据
//nFlag DL_DEV_PIC //照片
//DL_DEV_USER_FEAT //人脸特征
//DL_DEV_USER_TEXT //用户文字信息
//说明:: 批量上传用户必须包含用户文字信息，组合如下
//nFlag = DL_DEV_USER_TEXT 上传文字信息
//nFlag = DL_DEV_USER_TEXT|DL_DEV_PIC 文字+注册照
//nFlag = DL_DEV_USER_TEXT|DL_DEV_USER_FEAT 文字+人脸库
//nFlag = DL_DEV_USER_TEXT|DL_DEV_PIC|DL_DEV_USER_FEAT文字+注册照+人脸库
//DevOperResultNotify回调参数对应类型：
//eType = DEV_USER_EX_OPER
//eFlag = DEV_OPER_UPLOAD ，
//pvContent = DEV_BATCH_USER*。
//在OPER_SUCCESS 情况下，批量上传的用户个数与返回的用户个数一致。否则设备保存失败。
//批量上传用户数设定在5个以下(和网络带宽，主机处理速度有关)。
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: Batch uploading users
//Parameter:
//cszDevAddr Device address
//cUsers batch users data
//nFlag DL_DEV_PIC //picture
// DL_DEV_USER_FEAT //facial features
// DL_DEV_USER_TEXT //text
//Comment:: Text must be included by batch uploading users, of which the combinations are:
//nFlag = DL_DEV_USER_TEXT uploading text
//nFlag = DL_DEV_USER_TEXT|DL_DEV_PIC text+reg. picture
//nFlag = DL_DEV_USER_TEXT|DL_DEV_USER_FEAT  text+face library
//nFlag = DL_DEV_USER_TEXT|DL_DEV_PIC|DL_DEV_USER_FEA Text+reg. picture+face library
//DevOperResultNotify Callback parameter corresponding type:
//eType = DEV_USER_EX_OPER
//eFlag = DEV_OPER_UPLOAD ，
//pvContent = DEV_BATCH_USER*。
//In OPER_SUCCESS condition, the amount of batch uploading users is the same as the returned users. Or the device saves failure.
// The amount of batch uploading users is limited to 5 (it is related to the network bandwidth and mainframe processing speed).
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_ULUserEx(const char* cszDevAddr, const DEV_BATCH_USER& cUsers, int nFlag, int nSeq = -1);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 批量下载用户
//参数:
//cszDevAddr 设备地址
//pUserIDArray 用户ID数组首地址
//nIDCount 数组元素个数
//nFlag 同SLG_ULUserEx
//说明: DevOperResultNotify回调参数对应类型：
//eType = DEV_USER_EX_OPER
//eFlag = DEV_OPER_DOWNLOAD ，
//pvContent= DEV_BATCH_USER*。
//OPER_SUCCESS 情况下，如果批量下载的用户数与要求下载的用户数不一致，可以通过
// SLG_DLAllUser(const char* cszDevAddr, int nSeq = -1) 来确认用户是否真的存在于设备上。
//批量下载用户数设定在5个以下(和网络带宽，主机处理速度有关)。
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: Batch Downloading Users
//Parameter:
//cszDevAddr Device address
//pUserIDArray  User ID array start address
//nIDCount Amount of array elements
//nFlag  with SLG_ULUserEx
//Comment: DevOperResultNotify Callback parameter corresponding type:
//eType = DEV_USER_EX_OPER
//eFlag = DEV_OPER_DOWNLOAD ，
//pvContent= DEV_BATCH_USER*。
//In OPER_SUCCESS condition，if the amount of batch downloading users is not the same as the amount of requested downloading users,
// it can use SLG_DLAllUser(const char* cszDevAddr, int nSeq = -1) to confirm whether the user exists in the device.
// The amount of batch downloading users is limited to 5 (it is related to the network bandwidth and mainframe processing speed).
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_DLUserEx(const char* cszDevAddr, DEV_CID* pUserIDArray, int nIDCount, int nFlag, int nSeq = -1);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 批量删除用户
//参数:
//cszDevAddr 设备地址
//pUserIDArray 待删除用户的编号数组
//nIDCount 数组元素个数
//说明: DevOperResultNotify回调参数对应类型：
//eType = DEV_USER_EX_OPER
//eFlag = DEV_OPER_DELETE，
//pvContent = DEV_BATCH_USER*。
//在OPER_SUCCESS 情况下，批量删除的用户个数与返回的用户个数不一致，
//可以通过 SLG_DLAllUser(const char* cszDevAddr, int nSeq = -1)
//来确认用户是否真的存在于设备上。单次批量删除用户数设定在10个以下为好。
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: Batch Deleting Users
//Parameter:
//cszDevAddr Device address
//pUserIDArray Waiting for deleting users serial number array
//nIDCount  The amount of array elements
//Comment: DevOperResultNotify Callback parameter corresponding type:
//eType = DEV_USER_EX_OPER
//eFlag = DEV_OPER_DELETE，
//pvContent = DEV_BATCH_USER*。
//In OPER_SUCCESS condition，if the amount of batch deleting users is not the same as the amount of requested deleting users,
// it can use SLG_DLAllUser(const char* cszDevAddr, int nSeq = -1)
//to confirm whether the user exists in the device. The amount of batch deleting users is limited to 10
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_DELUserEx(const char* cszDevAddr, DEV_CID* pUserIDArray, int nIDCount, int nSeq = -1);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 批量下载记录
//参数: cszDevAddr 设备地址
//cRange ID区间
//说明: 包括验证记录、报警记录，操作日志、变更注册照
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function:  batch download records
//Parameter: cszDevAddr Device address
//cRange ID interval
//Comment: including verification records, alarm records ,operation log and reg. image changing records
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_DLRecEx(const char* cszDevAddr, const DEV_DL_RECORD_RANGE& cRange, int nSeq = -1);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 重启设备
//参数: cszDevAddr 设备地址
//说明:
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: Restart device
//Parameter: cszDevAddr Device address
//Comment:
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_RebootDev(const char* cszDevAddr, int nSeq = -1);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 获得/设置 设备网络属性信息
//参数: cszDevAddr 设备地址
//pNetPro NULL表示获得，不为空表示设置
//说明:: 不能设置ipv6地址
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: Acquire/setup device network information
//Parameter: cszDevAddr Device address
// pNetPro NULL means to acquire, NOT NULL means to setup
//Comment:
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_NetPro(const char* cszDevAddr, DEV_NETPRO* pNetPro, int nSeq = -1);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 屏幕校验
//参数: cszDevAddr 设备地址
//说明:
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function: screen verify
//Parameter: cszDevAddr Device address
//Comment:
////////////////////////////////////////////////////////////////////////////////////////
BOOL SLGDEV_CALL SLG_ResetScreen(const char* cszDevAddr, int nSeq = -1);

////////////////////////////////////////////////////////////////////////////////////////
//功能: 设置参数
//参数: type 参数类型 如 SLG_DEBUG (value=[0 1])
//value 参数的值
//说明:
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//Function:  setup Parameter
//Parameter: type Parameter type, as SLG_DEBUG (value=[0 1])
// value  the value of Parameter
//Comment:
////////////////////////////////////////////////////////////////////////////////////////
#define SLG_DEBUG -1 //打印调试信息
long SLGDEV_CALL SLG_SetLong(long type, long value);

////////////////////////////////////////////////////////////////////////////////////////
// SetWapInfo 用于获取和设置802.1x认证相关的三个参数：开启/关闭802.1x认证、用户名、密码
// 用于获取和设置802.1x认证相关的三个参数：开启/关闭802.1x认证、用户名、密码。对应的子命
//令为S_GET和S_SET。设置成功时返回 SUC_EXECUT.
//二、该功能在高级设置中时只能获取、不能设置。设备在高级设置中时，设置相关参数会返回ERR_BUSY
////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
    int  isopen;      //WPA开关（整数） sizeof（int) 0表示关闭；1表示开启
    char username[32];// 用户名（字符串） 32字节ASCII码字符串 总长度32字节，有效数据最多31个字节。因'\n'需占用一个字节。
    char password[32];// 密码（字符串） 32字节ASCII码字符串 总长度32字节，有效数据最多31个字节。因'\n'需占用一个字节。
} WAPINFO;
BOOL SLGDEV_CALL SLG_SetWapInfo(const char* cszDevAddr, const WAPINFO* pWapInfo, int nSeq);
BOOL SLGDEV_CALL SLG_GetWapInfo(const char* cszDevAddr, int nSeq);

////////////////////////////////////////////////////////////////////////////////////////
// end
EXTERN_C_END
#endif
