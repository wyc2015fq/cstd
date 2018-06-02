/*************************************************************************************/
/*名    称:CPMDev.dll
/*功    能:管理端跟dsp端的业务逻辑通信接口，为管理端提供各种数据上传下载功能，同时管理dsp端用户操作以及中转模式下各种中转处理
/*说    明:
在使用本库过程中，有以下两点限制
1. 上传用户照片时，照片内存大小限制在60KB以下
2. 用户数据上传下载过程中，请不要在栈上分配大于150kb的内存
3. 所有命令序列号不能为0，0序号内部占用
/*************************************************************************************/

#ifndef __CPMDEV_H__
#define __CPMDEV_H__

#ifdef CPMDEV_EXPORTS
#define CPMDEV_API  __declspec(dllexport)
#else
#define CPMDEV_API
#endif
#define CPMDEV_CALL

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef BOOL
  typedef int BOOL;
#endif

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define DEV_REGION_ADDR ("224.0.1.100") //默认区域地址,用户设备自动自动搜索 
#define DEV_ID_LEN 24                   //ID长度
#define DEV_TIMEGROUP_NUMS 8            //时间组个数
#define DEV_USER_COMBINS   6          //用户组合个数

#define DL_DEV_PIC         0x01         //照片
#define DL_DEV_USER_FEAT   0x02         //人脸特征
#define DL_DEV_USER_TEXT   0x04         //用户文字信息

#define DEV_CHECK_TIME     0x01         //检测时间
#define DEV_CHECK_WEEK     0x02         //检测星期
#define DEV_CHECK_DAY      0x04         //检测日期
#define DEV_CHECK_MONTH    0x08         //检测月份
#define DEV_CHECK_YEAR     0x10         //检测年份

#define DEV_WEEK_1         0x01         //星期一有效
#define DEV_WEEK_2         0x02         //星期二有效
#define DEV_WEEK_3         0x04         //星期三有效
#define DEV_WEEK_4         0x08         //星期四有效
#define DEV_WEEK_5         0x10         //星期五有效
#define DEV_WEEK_6         0x20         //星期六有效
#define DEV_WEEK_7         0x40         //星期天有效

#define DEV_RECORD_SAVEFAIL   0x00000001    //是否保存识别失败记录
#define DEV_SUPER_PASSWORD    0x00000002    //超级密码开门有效
#define DEV_HDBEEP_OPEN         0x00000004    //布防/撤防
#define DEV_REALTIME_RECORD   0x00000010    //实时动态记录发送
#define DEV_REALTIME_USERLOAD 0x00000020    //输入编号或者卡号识别时，如果无法在当前dsp中获得人脸特征,在网络连接状态下，将从管理端实时加载人脸特征
#define DEV_REALTIME_USERSEND 0x00000040    //通过设备采集的用户实时发送到管理终端
#define DEV_DOORMANGET_OPEN   0x00000080    //开启门磁检测报警
#define DEV_DOORFORCE_OPEN    0x00000100    //开启胁迫开门功能
#define DEV_REMOTE_CAP_SAVE     0x00000200    //远程采集的用户是否保存到DSP
#define DEV_GPRS_OPEN           0x00000400    //开启GPRS模块
#define DEV_UPDATE_USERSEND   0x00000800    //特征更新时，是否实时发送用户特征
#define DEV_AB_LOCK             0x00002000    //AB互锁
#define DEV_DOOR1_NOPEN         0x00004000    //门一N+1
#define DEV_DOOR2_NOPEN         0x00008000    //门二N+1

  //验证模式优先级低-高:UID->CARD->11->MIX->1N
#define DEV_VERIFY_USERID       0x01    //工号
#define DEV_VERIFY_CARD         0x02    //刷卡
#define DEV_VERIFY_FACE_11      0x04    //11人脸加载
#define DEV_VERIFY_FACE_MIX     0x08    //混合人脸加载
#define DEV_VERIFY_FACE_1N      0x10    //1N人脸加载
  //用户状态信息
#define DEV_USER_CARD_INVALID 0x01   //卡挂失
#define DEV_USER_DLine_INVALID  0x02   //失效时间有效
#define DEV_USER_BLACK          0x04   //黑名单
#define DEV_USER_MIX1N          0x08   //混合模式下1N用户
  //SDK自定义
#define DEV_USER_FEAT_BASE64    0x40   //特征经过base64编码
#define DEV_USER_IMAGE_BASE64   0x80   //注册照经过base64编码

  typedef enum {
    DEV_CONNECT_CUT = 0x00,     //断开连接
    DEV_CONNECT_SUCCESS,  //建立连接
    DEV_CONNECT_FAILUE ,    //连接失败
    DEV_CONNECT_NONE        //未建立连接
  } DEV_CONNECT_STATUS;

  typedef enum {
    DEV_OPER_UPLOAD = 0x01,       //上传
    DEV_OPER_DOWNLOAD = 0x02, //下载
    DEV_OPER_DELETE   = 0x04  //删除
  } DEV_OPER_FLAG;

  typedef enum {
    DEV_AUTH_OPER = 0x05,     //设备验证操作
    DEV_AUTH_SET_OPER,    //设备验证用户设置操作
    DEV_REGION_OPER ,       //设备区域操作
    DEV_FORMAT_OPER,    //设备格式化操作
    DEV_UPDATE_OPER,    //设备程序更新操作
    DEV_SYS_TIME_OPER ,   //设备系统时间操作
    DEV_BASEINFO_OPER,    //基本信息操作
    DEV_STATISINFO_OPER,  //统计信息操作
    DEV_WORKATT_OPER,       //工作属性操作
    DEV_USER_OPER ,     //用户操作
    DEV_USER_RT_OPER,   //用户实时加载操作
    DEV_USER_RANGE_OPER,    //用户下载数据条数回馈操作
    DEV_RECORD_OPER  ,    //记录操作
    DEV_RECORD_RANGE_OPER,  //记录下载数据条数回馈操作
    DEV_ALARM_OPER,
    DEV_ALARM_RANGE_OPER,
    DEV_LOG_OPER,     //日志操作
    DEV_LOG_RANGE_OPER,     //日志下载数据条数回馈操作
    DEV_CHANGEIMAGE_OPER, //变更注册照操作
    DEV_CIMAGE_RANGE_OPER,  //变更注册照下载数据条数回馈操作
    DEV_TIME_GROUP_OPER,  //时间组操作
    DEV_RIGHT_OPER,     //权限操作
    DEV_USERGROUP_OPER,   //用户组操作
    DEV_DOOR_STATE_OPER,    //开门状态操作
    DEV_REMOTE_OPEN_DOOR, //远程开门操作
    DEV_VIDEO_TRANSFER,   //视频传输
    DEV_USER_EX_OPER,   //用户批量操作
    DEV_RECORD_EX_OPER,     //记录批量操作
    DEV_LOG_EX_OPER,    //日志批量操作
    DEV_CIMAGE_EX_OPER,   //变更注册照批量操作
    DEV_REBOOT_OPER,    //设备重启
    DEV_USER_REMOTE_CAP,    //用户远程采集
    DEV_NOPEN_OPER,         //N+1开门
    DEV_IOCTRL_OPER,        //IOCTRL控制信息
    CLI_AUTH_OPER,          //client像中转服务器验证操作
    DEV_NETPRO_OPER,        //设备网络属性设置
  } DEV_OPER_TYPE;

  typedef enum {
    OPER_SUCCESS        = 0x00,      //成功
    OPER_ERR_BUSY       = 0x01,      //设备忙
    OPER_ERR_LIMIT      = 0x02,      //已达上限
    OPER_ERR_NOFIND     = 0x03,      //没有找到对应数据
    OPER_ERR_SAVEFAIL   = 0x04,      //数据保存失败
    OPER_ERR_SETFAIL    = 0x05,      //设置失败
    OPER_ERR_FROMAT   = 0x07,      //格式化失败
    OPER_ERR_PARAM    = 0x08,      //参数错误
    OPER_ERR_DISABLE  = 0x09,      //要求执行的功能没有使能
    OPER_ERR_EXECUT     = 0x0A,      //失败
    OPER_ERR_SUPPORT    = 0x10,      //不支持的命令
    OPER_ERR_INPUTDATA  = 0x11,      //网络端传输的数据有异常
    OPER_BATCH_DL_COMPLETE = 0x1F  //批量下载完成
  } DEV_OPER_RESULT; //操作结果


  typedef enum {
    DEV_NORMAL   = 0x01,        //正常时间
    DEV_HOLIDY   = 0x02,    //节假日
    DEV_ANY_TIME = 0x03         //任意时间
  } DEV_TIMEGROUP_TYPE; //时间组类型

  typedef enum {
    DEV_USER_NOMAL = 0,         //普通
    DEV_USER_ADMIN = 1,     //管理员
    DEV_USER_SUPERADMIN = 2   //超级管理员
  } DEV_USER_TYPE; //用户类型

  typedef enum {
    DEV_DEFAULT_NO_RIGHT    = -2,     //无权限
    DEV_DEFAULT_SINGLE_OPEN = -1  //单一开门权限
  } DEV_DEFAULT_RIGHT; //默认权限

  //记录类型:
  //0x01 - 0x20: 日常记录
  //0x20 - 0xFF: 报警记录
  typedef enum {
    DEV_VERIFY_SUCC = 0x01,                 //验证成功
    DEV_VERIFY_FAIL = 0x02,             //验证失败
    DEV_ADMIN_SUCC = 0x03,              //管理员验证成功
    DEV_EMER_OPEN = 0x04,               //紧急开门
    DEV_RIGHT_OPEN = 0x05,              //权限开门
    DEV_GROUP_OPEN = 0x06,              //组合开门
    DEV_BUTTON_OPEN = 0x07,             //按钮开门
    DEV_ALARM_HD_MANGET_TIMEOUT = 0x20, //门磁超时
    DEV_ALARM_HD_MANGET_ILLOPEN = 0x21, //门磁非法开门
    DEV_ALARM_HD_OFFLINE = 0x22,        //前端掉线报警
    DEV_ALARM_HD_BREAK = 0x30,          //防拆报警
    DEV_ALARM_HD_SHOCK =  0x31,         //震动报警
    DEV_ALARM_HD_FPOWR = 0x36,          //前端供电异常报警
    DEV_ALARM_HD_UPS_ON = 0x37,         //UPS备用电池开启
    DEV_ALARM_HD_UPS_OFF = 0x38,        //UPS备用电池关闭
    DEV_ALARM_HD_ASSIST = 0x40,         //辅助输入触发
    DEV_ALARM_SF_BACKLIST = 0xF0,       //黑名单验证报警
    DEV_ALARM_SF_ILLCARD = 0xF1,        //无效卡(挂失)
    DEV_ALARM_SF_ILLTIME = 0xF2,        //非法时间识别
    DEV_ALARM_SF_DEADLINE = 0xF3,       //失效时间
    DEV_ALARM_SF_DANGER_OPEN = 0xF4,    //胁迫开门
    DEV_ALARM_SF_SUPER_OPEN = 0xF5      //超级密码开门
  } DEV_REC_TYPE; //记录类型

  typedef enum {
    DEV_DOOR_SOURCE = 0x01,       //门相关来源
    DEV_SIGNAL_SOURCE,      //信号输入输出相关来源
    DEV_NO_NUM_DEV_SOURCE   //无编号设备来源
  } DEV_REC_SOURCE_TYPE;

  typedef enum {
    DEV_LOG_ADDUSER = 0x01,     //增加用户
    DEV_LOG_DELUSER = 0x02,   //删除用户
    DEV_LOG_REREGIST = 0x03,  //重新采集
    DEV_LOG_CAHNGETYPE = 0x04,  //改变用户类型
    DEV_UDISK_ULUSER = 0x05,  //U盘上传用户
    DEV_UDISK_DLUSER = 0x06,  //U盘下载用户
    DEV_UDISK_DLRECORD = 0x07,  //U盘下载记录
    DEV_UDISK_UPDATEAPP = 0x08  //U盘更新程序
  } DEV_LOG_TYPE;

  typedef enum {
    DEV_DOOR_NOMAL = 0x00000000,      //正常状态
    DEV_DOOR_OPEN = 0x00000100,   //开状态
    DEV_DOOR_CLOSE = 0x00000200   //关状态
  } DEV_DOOR_STATE;

  typedef struct {
    char m_ID[ DEV_ID_LEN ];
  } DEV_CID; //ID标识结构体

  typedef struct {
    int   m_Year;
    char  m_Month;
    char  m_Day;
  } DEV_DATE; //日期

  typedef struct {
    int   m_Msec;          //毫秒
    char  m_Hour;
    char  m_Minute;
    char  m_Second;
  } DEV_TIME; //时间

  typedef struct {
    DEV_DATE m_Date;
    DEV_TIME m_Time;
  } DEV_DATETIME; //日期和时间

  typedef struct {
    int  m_DevType;        //设备类型
    int  m_LimitUser;      //总使用人数上限
    int  m_Limit1NUser;    //1N用户上限
    DEV_CID m_SN;          //设备编号
    DEV_CID m_Ver;         //DSP软件版本
    DEV_CID m_Space;       //磁盘容量信息
  } DEV_BASEINFO; //设备基本信息

  typedef struct {
    int  m_TotalUsers;           //当前设备的总用户数
    int  m_NoFeatUser;           //没有采集人脸特征的用户数
    int  m_TotalDays;            //识别记录保存总天数
    int  m_TotalRecords;         //总记录数
    int  m_TotalAlarm;           //总报警记录数
    int  m_TotalDspLog;          //总操作日志数目
    int  m_TotalChangeImage;     //总变更注册照
  } DEV_STATIS; //DEV相关统计信息

  typedef struct {
    int      m_TimeGID[DEV_TIMEGROUP_NUMS];  //时间组ID
    int    m_BaseSet;                //参见设备功能标记定义
    int    m_DoorMangetTime;             //门磁延时时间，单位秒
    int    m_LockTime;               //电锁持续时间，单位秒
    int    m_VerifyMode;               //用户身份验证模式,例如:DEV_VERIFY_USERID|DEV_VERIFY_CARD|DEV_VERIFY_FACE_11
    int    m_nWGType;                //韦根协议类型(0输出韦根26,1输出韦根34)
    int      m_nWGOutType;               //韦根输出类型(0输出ID，1输出WG内容)
    int    m_nWGOutIDType;             //输出ID类型(0输出卡号,1输出用户ID)
    int    m_nWGOutContent;            //WG输出内容
    BOOL   m_bWGOutPut;              //是否WG输出
    DEV_CID       m_szSuperPWD;          //超级密码
    DEV_DEFAULT_RIGHT m_DefaultRight;    //设备默认权限
  } DEV_WORKATT;

  typedef struct {
    DEV_USER_TYPE m_UserType;         //终端设备上的用户类型(普通，管理，超管 -- 0 , 1 , 2)
    DEV_CID     m_UserID;           //用户ID
    DEV_DATETIME  m_TypeTime;          //用户类型更改时间
    DEV_DATETIME  m_RegistTime;       //人脸注册时间，即特征采集时间，采集或者重新采集要更新此时间
    DEV_DATETIME  m_LastUpdTime;      //最后更新的时间，识别时特征发生更新或者重新采集要更新此时间
  } DEV_VUSER;

  typedef struct {
    int      m_FeatLen;       //特征大小
    int      m_PicLen;        //照片大小
    int    m_RightID;           //用户权限ID
    DEV_CID  m_ImageSID;      //上传注册照，对应的变更ID，全局唯一
    DEV_CID  m_UserID;            //用户唯一ID,关键字
    DEV_CID  m_Admin;             //人脸注册管理员
    DEV_CID  m_AppendID;          //卡号或者其他用于1:1的附加身份确认信息
    DEV_CID  m_UserName;          //用户名
    DEV_CID  m_Department;        //部门名称
    DEV_DATETIME m_DeadLineTime;  //失效时间
    DEV_DATETIME m_RegistTime;    //人脸注册时间
    DEV_DATETIME m_LastUpdTime;   //最后更新的时间
    DEV_DATETIME m_TypeUpdTime;   //用户类型变更时间
    char m_UserFlag;              //用户状态标记
    DEV_USER_TYPE m_UserType;     //终端设备上的用户类型
    char*   m_FeatData;         //特征数据
    char*   m_PicData;          //照片数据
    int     m_bRTUser;            //是否为实时用户(0 非实时用户 1 实时采集用户 2实时特征更新用户)
  } DEV_USER;                       //用户信息

  typedef struct {
    int m_Count;         //下载多少条
    DEV_VUSER* m_pArray; //ID集合
  } DEV_DL_USER_RANGE;     //用户下载集合

  //关于记录和事件来源485地址解析:
  //1.485地址用一个字节表示，高4位[4-7]表示设备索引号1-15 , 低4位[0-3]表示支持16种不同型的设备
  // #define DEV_TYPE_FACE1    0x0      //人脸前端1
  // #define DEV_TYPE_FACE2    0x1      //人脸前端2
  // #define DEV_TYPE_CARD1    0x2      //刷卡器1
  // #define DEV_TYPE_CARD2    0x3      //刷卡器2
  // #define DEV_TYPE_IN       0x4      //辅助输入
  // #define DEV_TYPE_OUT      0x5      //辅助输出
  // #define DEV_TYPE_INWL     0x6      //无线输入
  // #define DEV_TYPE_DOOR     0xF      //门点本身
  // 例如: 0x11 表示1号门人脸识别前端1，INDEX[4-7] == 1  TYPE[0-3] == 1
  //       0x10 表示1号门人脸识别前端0，INDEX[4-7] == 1  TYPE[0-3] == 0
  //       0x12 表示1号门刷卡器0        INDEX[4-7] == 1  TYPE[0-3] == 2
  //       0x13 表示1号门刷卡器1        INDEX[4-7] == 1  TYPE[0-3] == 3
  //       0x1F 表示门点本身            INDEX[4-7] == 1  TYPE[0-3] == F
  //       0x14 表示辅助输入1           INDEX[4-7] == 1  TYPE[0-3] == 4
  //       0x25 表示辅助输出2           INDEX[4-7] == 2  TYPE[0-3] == 5
  //2. 所有设备的序号从1开始。此协议可支持15张门，16中不同类型的门点输入设备，15个辅助输入和15个辅助输出
  //3. 特殊意义的地址: [0-7] == 0x00 表示非法地址，[0-7] ＝＝ 0x01表示后端板本身, [0-7] ＝＝ 0x02表示GPRS模块
  //7_______3________0
  //|_INDEX_|__type__|

  typedef struct {
    int   m_LoadFlag;       //DL_DEV_PIC|DL_DEV_USER_FEAT=需要加载注册照片和人脸特征
    int   m_ReqSource;      //请求来源,详见485地址解析
    BOOL  m_bApeendID;      //TRUE=卡号, FALSE=工号
    DEV_CID m_ID;         //加载ID
  } DEV_USER_REAL_LOAD;         //实时用户加载

  typedef struct {
    int  m_RecType;         //记录类型
    int  m_Source;    //记录来源(已经过时,不在使用)
    BOOL m_bRealTime;                //是否为实时记录
    int  m_Score;                    //识别得分
    int  m_PicLen;                   //照片大小
    int  m_VerifyMode;               //验证模式,例如:DEV_VERIFY_USERID|DEV_VERIFY_CARD|DEV_VERIFY_FACE_11
    DEV_CID    m_ChangID;          //变更记录号
    DEV_CID    m_SerialID;         //识别记录流水号ID
    DEV_CID    m_UserID;           //用户ID,如果为空，表示非法记录
    DEV_CID    m_AppendID;         //卡号或者其他用于1:1的附加身份确认信息
    DEV_CID    m_UserName;         //用户名
    DEV_CID    m_Department;       //部门名称
    DEV_DATETIME m_RecTime;          //记录时间
    char  m_ConcretSource;           //记录来源485地址，详见485地址解析
    char* m_PicData;                 //原始JPG图像数据(未经base64编码)
  } DEV_RECORD; //记录信息

  typedef struct {
    BOOL  m_bRTLog;       //是否为实时操作日志
    DEV_LOG_TYPE m_LogType;     //日志类型
    DEV_CID m_SerialID;         //流水号ID
    DEV_CID m_Admin;            //操作员
    DEV_CID m_BeOptUser;        //被操作员
    DEV_DATETIME m_RecTime;     //记录时间
  } DEV_LOG;

  typedef struct { //变更注册照
    int    m_PicBytes;        //注册照大小
    BOOL   m_bRTChangeImage;  //是否为实时变更注册照
    DEV_CID  m_UserID;          //用户ID
    DEV_CID  m_SerialID;        //流水号ID
    DEV_CID  m_AppendID;        //卡号或者其他用于1:1的附加身份确认信息
    DEV_CID  m_UserName;        //用户名
    DEV_CID  m_Department;      //部门名称
    DEV_CID  m_Admin;           //人脸注册管理员,标识此用户的人脸特征是哪个管理员采集
    DEV_DATETIME m_RecTime;     //记录时间
    char*    m_PicData;         //JPG图像数据(未经base64编码)
  } DEV_CHANGEIMAGE;

  typedef struct {
    int  m_Count;      //下载多少条
    BOOL m_bOpenRange; //是否为开区间(true=是， false=否)
    DEV_CID m_SID;     //从那一条开始，m_SID表示记录精确的流水号 SID组成：SN + TYPE + DATE + ID = ( 6BYTE + 1BYTE + 8BYTE + 4BYTE + \0)
  } DEV_DL_RECORD_RANGE; //记录下载区间

  typedef struct {
    int m_Count;
    DEV_DL_RECORD_RANGE* m_pRange;
  } DEV_DL_RECORD_ARRAY; //记录下载区间集合

  typedef struct {
    int      m_TGID;                //时间组ID
    DEV_DATETIME m_Start;           //时间组开始时间
    DEV_DATETIME m_End;             //时间组结束时间
    DEV_TIMEGROUP_TYPE m_TGType;    //时间组类型
    char     m_CheckFlag;           //时间检测标记例：m_CheckFlag = CHECK_TIME|CHECK_WEEK
    char     m_WeekFlag;            //检测星期时，标记那些星期有效。例：m_WeekFlag = WEEK_1|WEEK_5
  } DEV_TIMEGROUP; //时间组

  typedef struct {
    int        m_nCount;
    DEV_TIMEGROUP* m_pTGArray;
  } DEV_TIMEGROUP_ARRAY; //时间组数组

  typedef struct {
    int m_RightID;                     //权限ID
    int m_TimeGID[DEV_TIMEGROUP_NUMS]; //时间组ID , (m_TimeGID[0]==ANY_TIME)未指定时间组，开门方式不受时间限制，任意时间段验证成功执行开门方式.
    BOOL m_bHolidyValid;             //节假日是否有效
    BOOL m_bActionLock;                //电锁输出
    BOOL m_bActionOutPut;            //电锁辅助输出
  } DEV_RIGHT; //权限

  typedef struct {
    int     m_nCount;
    DEV_RIGHT*  m_pRtArray;
  } DEV_RIGHT_ARRAY; //权限数组

  typedef struct {
    int  m_GroupID;                 //组ID
    int  m_NormalValid;               //普通用户中有效用户数
    int  m_ForceValid;                //强制用户中有效用户数
    DEV_CID  m_NormalUsers[DEV_USER_COMBINS]; //普通用户组合，优先级低
    DEV_CID  m_ForceUsers[DEV_USER_COMBINS];  //强制用户组合，优先级高
    BOOL m_bGroupOrder; //组合是否有序 1有序，0无序
  } DEV_USERGROUP;

  typedef struct {
    int        m_nCount;
    DEV_USERGROUP* m_pUGArray;
  } DEV_USERGROUP_ARRAY; //用户组数组

  //程序更新结构体
  typedef struct {
    int     m_Bytes;         //文件大小
    DEV_CID m_FileName;      //文件名
    char    m_TotalFiles;    //需要更新的文件总数
    char    m_FileIndex;     //当前更新到第几个
    char*   m_Buf;       //文件数据缓存首地址
  } DEV_APPUPDATE;

  //用户批量操作
  typedef struct {
    int       m_nCount;
    DEV_USER* m_pUserArray;
  } DEV_BATCH_USER;

  //批量记录
  typedef struct {
    int     m_nCount;
    DEV_RECORD* m_pRecordArray;
  } DEV_BATCH_RECORD;

  //批量日志
  typedef struct {
    int       m_nCount;
    DEV_LOG*  m_pLogArray;
  } DEV_BATCH_LOG;

  //批量注册照
  typedef struct {
    int         m_nCount;
    DEV_CHANGEIMAGE*  m_pCImageArray;
  } DEV_BATCH_CIMAGE;

  //IO设备状态
#define DEV_IO_MODE_NORMAL   0x00   //正常状态
#define DEV_IO_MODE_OPEN     0x01   //常开状态
#define DEV_IO_MODE_CLOSE    0x02   //常关状态
  //门磁状态
#define DEV_IO_STATE_CLOSE   0x00   //门磁关
#define DEV_IO_STATE_OPEN    0x01   //门磁开
  //执行动作
#define DEV_ACT_IO_OPEN      0x02   //执行打开动作
#define DEV_ACT_IO_OPENEX    0x04   //执行辅助动作
#define DEV_ACT_IO_CLOSE     0x10   //执行关闭动作
#define DEV_ACT_MODE_SET     0x20   //设置IO设备工作模式
#define DEV_ACT_MODE_GET     0x40   //获得IO设备工作模式
#define DEV_ACT_STATE_GET    0x80   //获得IO设备当前状态

  typedef struct {
    //IO设备485地址
    char m_Source;
    //执行动作
    char m_Action;
    //IO设备当前模式
    char m_IOMode;
    //IO设备当前状态状态
    char m_IOState;
  } DEV_IOCTRL;

  //NOPEN:远程请求管理端开门
  typedef struct {
    //m_Users==1权限ID, m_Users>1组合ID
    int        m_CtxID;
    //验证用户数目： 0密码开门，1权限开门，>1组合开门
    int        m_Users;
    //控制信息
    DEV_IOCTRL m_XOpen;
    //验证用户数组
    DEV_CID    m_IDS[DEV_USER_COMBINS * 2];
  } DEV_NOPEN;

  typedef struct {
    unsigned short Port;           //端口
    char           IP_Address[16]; //点分十进制IP地址
  } PEERADR;

#define DEV_WEB_LOADUSER    0x0001   //加载用户
#define DEV_WEB_WRITEUSER   0x0002   //写用户
#define DEV_WEB_WRITEREC    0x0004   //写记录
#define DEV_WEB_SYNCTIME    0x0008   //同步时间

  //网络属性
  typedef struct {
    char m_WebPro1;
    char m_WebPro2;
    char m_WebUrl1[127];
    char m_WebUrl2[127];

    char m_IPAdr[16];
    char m_MIPAdr[16];
    char m_NetMask[16];
    char m_GateWay[16];
    char m_Dns[16];
  } DEV_NETPRO;

  /***********************************************************************************
  功    能   设备连接提醒
  参    数
  cszDevAddr 设备地址
  eCNStatus  连接状态
  pvContext  应用上下文
  说    明
  ***********************************************************************************/
  typedef BOOL (CPMDEV_CALL* DevConnectStatus)(const char* cszDevAddr , DEV_CONNECT_STATUS eCNStatus , void* pvContext);

  /***********************************************************************************
  功    能 查询设备中转服务器归属，返回设备所属中转服务器IP地址
  参    数
  cszDevAddr 设备地址
  eCNStatus  返回的中转服务器IP
  pvContext  应用上下文
  说    明
  ***********************************************************************************/
  typedef BOOL (CPMDEV_CALL* QuerySrvCB)(const char* cszDevAddr, PEERADR& srvAddr, void* pvContext);

  /***********************************************************************************/
  /*功    能 设备操作结果回调
  /*参    数 cszDevAddr  设备地址
  eType       操作类型
  eFlag       操作标识
  pvContent   操作结果内容
  nSeq        操作流水号
  eResult     操作结果
  pvContext   应用层上下文
  /*说    明 上层应用收到此消息后即可知道前一次操作是否成功
  eType、eFlag、eResult决定pvContent内容
  当eResult=OPER_SUCCESS && eFlag=DEV_OPER_DOWNLOAD时，pvContent != NULL, eType操作类型解析如下
  //DEV_AUTH_OPER                 pvContent=NULL    客户端身份验证反馈
  //DEV_AUTH_SET_OPER             pvContent=NULL    客户端连接用户名密码设置成功反馈
  //DEV_REGION_OPER               pvContent=NULL
  //DEV_FORMAT_OPER               pvContent=NULL
  //DEV_UPDATE_OPER             pvContent=NULL
  //DEV_SYS_TIME_OPER             pvContent=DEV_DATETIME* or = NULL
  //DEV_BASEINFO_OPER             pvContent=DEV_BASEINFO* or = NULL
  //DEV_STATISINFO_OPER           pvContent=DEV_STATIS* or = NULL
  //DEV_WORKATT_OPER              pvContent=DEV_WORKATT* or = NULL
  //DEV_USER_OPER                 pvContent=DEV_USER* or = NULL
  //DEV_USER_RT_OPER              pvContent=DEV_USER_REAL_LOAD*
  //DEV_USER_RANGE_OPER           pvContent=DEV_DL_USER_RANGE* or = NULL
  //DEV_RECORD_OPER               pvContent=DEV_RECORD* or = NULL
  //DEV_RECORD_RANGE_OPER         pvContent=DEV_DL_RECORD_RANGE* or = NULL
  //DEV_LOG_OPER                  pvContent=DEV_LOG* or = NULL
  //DEV_LOG_RANGE_OPER            pvContent=DEV_DL_RECORD_RANGE* or = NULL
  //DEV_CHANGEIMAGE_OPER        pvContent=DEV_CHANGEIMAGE* or = NULL
  //DEV_CIMAGE_RANGE_OPER         pvContent=DEV_DL_RECORD_RANGE* or = NULL
  //DEV_TIME_GROUP_OPER       pvContent=DEV_TIMEGROUP_ARRAY* or = NULL
  //DEV_RIGHT_OPER          pvContent=DEV_RIGHT_ARRAY* or = NULL
  //DEV_USERGROUP_OPER        pvContent=DEV_USERGROUP_ARRAY* or = NULL
  //DEV_DOOR_STATE_OPER         pvContent=DEV_DOOR_STATE* or = NULL
  //DEV_NOPEN_OPER              pvContent=DEV_NOPEN*    设备请求管理端远程开门
  //DEV_IOCTRL_OPER               pvContent=DEV_IOCTRL*   IO控制信息反馈
  //CLI_AUTH_OPER                 pvContent=AUTH*   nSeq代表client端口
  /***********************************************************************************/
  typedef BOOL (CPMDEV_CALL* DevOperResultNotify)(const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult, void* pvContext);

  /***********************************************************************************/
  /*功    能 初始化于反初始化系统
  /*参    数 bFlag true=初始化,false=反初始化
  /***********************************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_InitSys(BOOL bFlag);

  /***********************************************************************************/
  /*功    能 注册设备连接状态提醒回调
  /*参    数 pfnCNNotify 回调函数指针
  pvContext 应用上下文
  /*说    明
  /***********************************************************************************/
  CPMDEV_API void CPMDEV_CALL CPM_RegDevConnectStatusCB(DevConnectStatus pfnCNNotify, void* pvContext);

  /***********************************************************************************/
  /*功    能 注册设备操作结果通知
  /*参    数 pfnOperNotify   回调函数指针
  pvContext  应用上下文
  /*说    明
  /***********************************************************************************/
  CPMDEV_API void CPMDEV_CALL CPM_RegOperResultNotifyCB(DevOperResultNotify pfnOperNotify, void* pvContext);

  /***********************************************************************************/
  /*功    能 设置整个模块为中转模式
  /*参    数
  srvMode    工作模式 TRUE 中转模式， FALSE本地模式
  pfn        中转服务查询回调函数
  pvContext  应用上下文
  /*说    明 不调用该函数，默认为本地模式
  /***********************************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_SetMode(BOOL srvMode, QuerySrvCB pfn,  void* pvContext);

  /***********************************************************************************/
  /*功    能 连接到中转服务器
  /*参    数
  /*bFlag    true=连接中转服务器,false=断开与中转服务器的连接
  /*ip       中转服务器IP地址
  /*name     client验证用户名
  /*psw      client验证密码
  /*说    明 中转服务器连接成功返回TRUE，否则FALSE。身份验证成功与否通过DevOperResultNotify回调通知外部.
  只要调用该函数，不管中转服务器连接成功与否，模式自动切换为中转模式，显示调用CPM_CnSrv(FALSE,NULL,NULL,NULL)
  才能切换到本地模式.不调用该函数为本地模式
  /***********************************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_CnSrv(BOOL bFlag, const char* ip, const char* name, const char* psw);

  /***********************************************************************************/
  /*功    能 启动设备搜索与否
  /*参    数 bFlag(true=启动  false=停止)
  cszRgnAddr 设备所在区域地址
  /*说    明 一下3个函数，当前版本中暂时不使用
  /***********************************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_StartDevSerch(BOOL bFlag, const char* cszRgnAddr = DEV_REGION_ADDR);

  /***********************************************************************************/
  /*功    能 与设备建立连接
  /*参    数 cszDevAddr 设备地址
  /*说    明
  /***********************************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_CNDev(const char* cszDevAddr);

  /***********************************************************************************/
  /*功    能 断开设备连接
  /*参    数 cszDevAddr 设备地址
  /*说    明
  /***********************************************************************************/
  CPMDEV_API void CPMDEV_CALL CPM_DCNDev(const char* cszDevAddr);

  /***********************************************************************************/
  /*功    能 设备验证或设置验证
  /*参    数 cszDevAddr设备地址
  cName    用户名
  cPws     密码
  nFlag    1=验证 2=设置
  /*说    明
  /***********************************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_ULDevAuth(const char* cszDevAddr, const DEV_CID& cName, const DEV_CID& cPws, int nFlag, int nSeq = -1);

  /***********************************************************************************/
  /*功    能 设置设备所在区域地址
  /*参    数
  cszDevAddr 设备地址
  cszRgnAddr 设备区域地址
  /*说    明 当设备建立连接后可将设备设置到某一个区域，以后及可对这个区域进行搜索设备
  /***********************************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_ULDevRegionAddr(const char* cszDevAddr, const char* cszRgnAddr = DEV_REGION_ADDR, int nSeq = -1);

  /**********************************************************************
  功    能 设置设备系统时间
  参    数
  cszDevAddr 设备地址
  cSysTime   时间信息
  说    明   DevOperResultNotify回调参数对应类型：
  eType = DEV_SYS_TIME_OPER
  eFlag = DEV_OPER_UPLOAD ，
  pvContent = NULL
  **********************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_ULDevTime(const char* cszDevAddr, const DEV_DATETIME& cSysTime, int nSeq = -1);

  /**********************************************************************
  功    能 获取设备系统时间
  参    数 cszDevAddr 设备地址
  说    明 DevOperResultNotify回调参数对应类型：
  eType = DEV_SYS_TIME_OPER
  eFlag = DEV_OPER_DOWNLOAD，
  pvContent  = DEV_DATETIME*
  **********************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_DLDevTime(const char* cszDevAddr, int nSeq = -1);


  /***********************************************************************************/
  /*功    能 获取设备基本信息
  /*参    数 cszDevAddr 设备地址
  /*说    明
  /***********************************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_DLDevBaseInfo(const char* cszDevAddr, int nSeq = -1);

  /***********************************************************************************/
  /*功    能 获取设备统计信息
  /*参    数 cszDevAddr 设备地址
  /*说    明
  /***********************************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_DLDevStatisInfo(const char* cszDevAddr, int nSeq = -1);

  /**********************************************************************
  功    能  获取人脸机工作属性
  参    数
  cszDevAddr 人脸机地址
  nSeq       操作序列号
  说    明   工作属性DEV_WORKATT通过回调函数给出。
  **********************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_DLDevWorkAttInfo(const char* cszDevAddr, int nSeq = -1);

  /**********************************************************************
  功    能  设置设备工作属性信息
  参    数
  cszDevAddr 设备地址
  cWorkAtt   工作属性结构体
  nSeq       操作序列号
  **********************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_ULDevWorkAttInfo(const char* cszDevAddr, const DEV_WORKATT& cWorkAtt, int nSeq = -1);

  /*********************************************************************
  功    能  上传指定用户
  参    数
  cszDevAddr 设备地址
  cUser      用户信息结构体
  nSeq       操作序列号
  说    明
  用户无人脸库时，DEV_USER中的 m_FeatLen = 0， m_FeatData = NULL
  用户无注册照时，DEV_USER中的 m_PicLen= 0， m_PicData = NULL
  DevOperResultNotify回调参数对应类型：eType = DEV_USER_OPER
  eFlag = DEV_OPER_UPLOAD，pvContent= NULL
  *********************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_ULUser(const char* cszDevAddr, const DEV_USER& cUser, int nSeq = -1);

  /**********************************************************************
  功    能    上传实时加载用户
  参    数
  cszDevAddr  设备地址
  cUser       用户信息结构体
  cRtLoad     实时加载信息
  nSeq        操作序列号
  说    明    设备端请求客户端上传某用户时，通过此接口上传所请求用户信息，
  cRtLoad为设备端请求时传过来的信息，客户端禁止修改。此函数实现远程用户加载功能。
  **********************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_ULRealTimeUser(const char* cszDevAddr, const DEV_USER& cUser, const DEV_USER_REAL_LOAD& cRtLoad);

  /**********************************************************************
  功    能   远程实时采集用户特征
  参    数
  cszDevAddr 设备地址
  cUser      用户信息
  说    明 管理端录入用户资料时，可通过该函数将用户资料发送的人脸机，控制人脸机采集该用户的人脸。
  远程用户采集功能，使得直接在管理端就可以采集用户，在配合远程视频的情况下采集的可视化程度和直接在机器上采集一样。
  如果使能了(用户采集实时同步)。远程采集的用户也会实时的发回管理端.
  **********************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_ULRealTimeUserCap(const char* cszDevAddr, const DEV_USER& cUser, int nSeq = -1);

  /**********************************************************************
  功    能   删除指定用户
  参    数
  cszDevAddr 设备地址
  cID        用户编号
  nSeq       操作序列号
  说    明  删除成功与否，通过操作回调函数给出结果
  DevOperResultNotify回调参数对应类型：
  eType = DEV_USER_OPER
  eFlag = DEV_OPER_DELETE，
  pvContent =  NULL
  **********************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_DELUser(const char* cszDevAddr, const DEV_CID& cID, int nSeq = -1);

  /**********************************************************************
  功    能  下载指定用户
  参    数
  cszDevAddr 设备地址
  cID        用户ID
  nFlag      DL_DEV_USER_PIC 需下载用户照片
             DL_DEV_USER_FEAT 需要下载人脸特征
             DL_DEV_USER_PIC|DL_DEV_USER_FEAT=两者
             为0默认下载文字信息
  nSeq       操作序列号
  说    明  DevOperResultNotify回调参数对应类型：
  eType = DEV_USER_OPER
  eFlag = DEV_OPER_DOWNLOAD，
  pvContent = DEV_USER*
  **********************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_DLSingleUser(const char* cszDevAddr, const DEV_CID& cID, int nFlag, int nSeq = -1);

  /***********************************************************************************/
  /*功    能 下载所有用户
  /*参    数 cszDevAddr 设备地址
  /*说    明
  /***********************************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_DLAllUser(const char* cszDevAddr, int nSeq = -1);

  /***********************************************************************************
  /*功    能 下载某一时间段内的注册用户
  参    数
  cszDevAddr  设备地址
  cStart      起始日期
  cEnd        终止日期
  说    明
  ***********************************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_DLSegTimeUser(const char* cszDevAddr, const DEV_DATE& cStart, const DEV_DATE& cEnd , int nSeq = -1);

  /**********************************************************************
  功    能  下载识别记录区间
  参    数
  cszDevAddr 设备地址
  nSeq       操作序列
  说    明   DevOperResultNotify回调参数对应类型：
  eType = DEV_RECORD_OPER
  eFlag = DEV_OPER_DOWNLOAD
  pvContent = DEV_DL_RECORD_ARRAY*
  参考DEV_DL_RECORD_ARRAY结构，一天为一个区间，该函数返回识别记录的所有区间。
  **********************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_DLAllIdentifyRecord(const char* cszDevAddr, int nSeq = -1);

  /***********************************************************************************/
  /*功    能 下载指定时间段识别记录
  /*参    数 cszDevAddr 设备地址
  cStart    起始日期
  cEnd      终止日期
  /*说    明
  /***********************************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_DLSegTimeIdentifyRecord(const char* cszDevAddr, const DEV_DATE& cStart, const DEV_DATE& cEnd, int nSeq = -1);

  /**********************************************************************
  功    能 获取区间识别记录/报警记录指定ID的后续区间
  参    数
  cszDevAddr    设备地址
  cRange ID     区间
  flag          DL_DEV_RECORD_PIC=需要下载记录照片
  bKnowRange    true=具体数据， false=区间集合
  nSeq          操作序列
  说    明 当bKnowRange 为true该函数下载具体的数据，为false时查询指定区间在设备上的具体大小。 DevOperResultNotify回调参数对应类型：
  下载区间时
  pvContent = DEV_DL_RECORD_ARRAY*
  cRange.m_SID = 指定SN
  cRange.m_Count = -1;
  cRange.m_bOpenRange = TRUE;
  bKnowRange = FALSE，flag = 1（无效参数）
  下载具体记录时
  DevOperResultNotify回调参数对应类型：
  pvContent = DEV_RECORD*
  **********************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_DLRangeRec(const char* cszDevAddr, const DEV_DL_RECORD_RANGE& cRange , int nFlag, BOOL bKnowRange = FALSE, int nSeq = -1);

  /**********************************************************************
  功    能  下载报警记录区间
  参    数
  cszDevAddr 设备地址
  nSeq       操作序列
  说    明  DevOperResultNotify回调参数对应类型：
  eType = DEV_ALARM_OPER
  eFlag = DEV_OPER_DOWNLOAD，
  pvContent = DEV_DL_RECORD_ARRAY*
  参考DEV_DL_RECORD_ARRAY结构，报警记录只有一个区间
  **********************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_DLAllAlarmRecord(const char* cszDevAddr, int nSeq = -1);

  /***********************************************************************************/
  /*功    能 下载指定时间段报警记录
  /*参    数 cszDevAddr 设备地址
  cStart    起始日期
  cEnd      终止日期
  /*说    明
  /***********************************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_DLSegTimeAlarmRecord(const char* cszDevAddr, const DEV_DATE& cStart, const DEV_DATE& cEnd, int nSeq = -1);

  /***********************************************************************************/
  /*功    能 下载区间报警记录
  /*参    数 cszDevAddr 设备地址
  cRange ID区间
  flag   DL_DEV_RECORD_PIC=需要下载记录照片 0x00=不下载照片, 0x01下载照片
  bKnowRange 指定是下载具体数据还是下载ID集合(true=具体数据， false=ID集合)
  /*说    明 此接口用于从某一条精确的记录ID开始下载nRang条数据
  /***********************************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_DLRangeAlarm(const char* cszDevAddr, const DEV_DL_RECORD_RANGE& cRange , int nFlag, BOOL bKnowRange = FALSE, int nSeq = -1);

  /***********************************************************************************/
  /*功    能 下载所有日志
  /*参    数 cszDevAddr 设备地址
  /*说    明
  /***********************************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_DLLog(const char* cszDevAddr, int nSeq = -1);

  /***********************************************************************************/
  /*功    能 下载区间日志
  /*参    数 cszDevAddr 设备地址
  cRange ID区间
  bKnowRange 指定是下载具体数据还是下载ID集合(true=具体数据， false=ID集合)
  /*说    明 此接口用于从某一条精确的记录ID开始下载nRang条数据
  /***********************************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_DLRangeLog(const char* cszDevAddr, const DEV_DL_RECORD_RANGE& cRange , BOOL bKnowRange = FALSE, int nSeq = -1);

  /**********************************************************************
  功    能  获取变更注册照记录区间
  参    数
  cszDevAddr 设备地址
  nSeq       操作序列
  说    明
  DevOperResultNotify回调参数对应类型：
  eType = DEV_CHANGEIMAGE_OPER
  eFlag = DEV_OPER_DOWNLOAD，
  pvContent = DEV_DL_RECORD_ARRAY*
  参考DEV_DL_RECORD_ARRAY结构，变更注册照只有一个区间
  **********************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_DLChangeImage(const char* cszDevAddr, int nSeq = -1);

  /**********************************************************************
  功    能  下载指定区间的变更注册照记录或者查询区间数据的具体大小
  参    数
  cszDevAddr   设备地址
  cRange       区间, 参考区间定义
  flag         DL_DEV_RECORD_PIC=需要下载记录照片
  bKnowRange   true=具体数据， false=区间集合
  nSeq         操作序列号
  说    明  当bKnowRange 为true该函数下载具体的数据，为false时查询指定区间在设备上的具体大小。
  下载区间时
  DevOperResultNotify回调参数对应类型：
  pvContent = DEV_DL_RECORD_ARRAY*
  cRange.m_SID   = 指定SN
  cRange.m_Count = -1;
  cRange.m_bOpenRange = TRUE;
  bKnowRange = FALSE，flag = 1（无效参数）
  下载具体记录时
  DevOperResultNotify回调参数对应类型：
  pvContent=DEV_RECORD*
  **********************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_DLRangeCImage(const char* cszDevAddr, const DEV_DL_RECORD_RANGE& cRange , int nFlag, BOOL bKnowRange = FALSE, int nSeq = -1);

  /***********************************************************************************/
  /*功    能 设置时间组
  /*参    数 cszDevAddr 设备地址
  cTGArray   时间组
  /*说    明
  /***********************************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_ULTimeGroup(const char* cszDevAddr, const DEV_TIMEGROUP_ARRAY& cTGArray, int nSeq = -1);

  /***********************************************************************************/
  /*功    能 获取时间组
  /*参    数 dev_addr 设备地址
  cTGArray 时间组
  eType 时间组类型
  /*说    明 cTGArray.m_nCount=0时表示下载eType类型的所有时间组
  /***********************************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_DLTimeGroup(const char* cszDevAddr, const DEV_TIMEGROUP_ARRAY& cTGArray, DEV_TIMEGROUP_TYPE eType, int nSeq = -1);

  /***********************************************************************************/
  /*功    能 删除时间组
  /*参    数 cszDevAddr 设备地址
  cTGArray 时间组
  eType 时间组类型
  /*说    明 eType.m_nCount=0时表示删除tg_type类型所有时间组
  /***********************************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_DELTimeGroup(const char* cszDevAddr, const DEV_TIMEGROUP_ARRAY& cTGArray, DEV_TIMEGROUP_TYPE eType, int nSeq = -1);

  /***********************************************************************************/
  /*功    能 设置权限
  /*参    数 cszDevAddr 设备地址
  cRTArray 权限
  /*说    明
  /***********************************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_ULRight(const char* cszDevAddr, const DEV_RIGHT_ARRAY& cRTArray, int nSeq = -1);

  /***********************************************************************************/
  /*功    能 发送获取权限
  /*参    数 cszDevAddr 设备地址
  cRTArray 权限
  /*说    明 当rt_array.m_nCount=0时表示下载所有权限
  /***********************************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_DLRight(const char* cszDevAddr, const DEV_RIGHT_ARRAY& cRTArray, int nSeq = -1);

  /***********************************************************************************/
  /*功    能 删除权限
  /*参    数 cszDevAddr 设备地址
  cRTArray 权限
  /*说    明 当rt_array.m_nCount=0时表示删除所有权限
  /***********************************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_DELRight(const char* cszDevAddr, const DEV_RIGHT_ARRAY& cRTArray, int nSeq = -1);

  /***********************************************************************************/
  /*功    能 设置用户组
  /*参    数 cszDevAddr 设备地址
  cUGrray 用户组
  /*说    明
  /***********************************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_ULUserGroup(const char* cszDevAddr, const DEV_USERGROUP_ARRAY& cUGrray, int nSeq = -1);

  /***********************************************************************************/
  /*功    能 发送获取用户组
  /*参    数 cszDevAddr 设备地址
  ug_array 用户组
  /*说    明 当rt_array.m_nCount=0时表示下载所有用户组
  /***********************************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_DLUserGroup(const char* cszDevAddr, const DEV_USERGROUP_ARRAY& cUGrray, int nSeq = -1);

  /***********************************************************************************/
  /*功    能 删除用户组
  /*参    数 cszDevAddr 设备地址
  ug_array 用户组
  /*说    明 当rt_array.m_nCount=0时表示删除所有用户组
  /***********************************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_DELUserGroup(const char* cszDevAddr, const DEV_USERGROUP_ARRAY& cUGrray, int nSeq = -1);

  /***********************************************************************************/
  /*功    能 设置开关门状态
  /*参    数
  cszDevAddr 设备地址
  bEOpen     true=紧急开门， false=紧急关门
  /*说    明
  /***********************************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_ULOpenDoorState(const char* cszDevAddr, DEV_DOOR_STATE eState, int nSeq = -1);

  /***********************************************************************************/
  /*功    能 获取开关门状态
  /*参    数 cszDevAddr 设备地址
  bEOpen true=紧急开门， false=紧急关门
  /*说    明
  /***********************************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_DLOpenDoorState(const char* cszDevAddr, int nSeq = -1);


  /***********************************************************************************/
  /*功    能 远程开门
  /*参    数 cszDevAddr 设备地址
  bEOpen true=开门， false=关门
  pDoorArray 门点数组
  size       数组大小
  /*说    明
  /***********************************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_ULRemoteOpenDoor(const char* cszDevAddr, BOOL bOpen, int nSeq = -1);

  /***********************************************************************************/
  /*功    能 IO设备控制
  /*参    数
  cszDevAddr 设备地址
  DEV_IOCTRL 控制信息
  nSeq       命令执行序列号
  /*说    明 DEV_IOCTRL:m_Source指定要做操作的端口，DEV_IOCTRL:m_Action要执行的动作
  打开门1，m_Source = 0x1f m_Action = DEV_ACT_IO_OPEN
  打辅助1，m_Source = 0x15 m_Action = DEV_ACT_IO_OPEN
  0x1f和015具体怎么解析的，请参考485地址解析
  /***********************************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_IOCtrl(const char* cszDevAddr, const DEV_IOCTRL& ioctrl, int nSeq = -1);

  /***********************************************************************************/
  /*功    能 格式化设备
  /*参    数 cszDevAddr 设备地址
  /*说    明
  /***********************************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_ULFormat(const char* cszDevAddr, int nSeq = -1);


  /***********************************************************************************/
  /*功    能 设备程序更新
  /*参    数 cszDevAddr 设备地址
  cAppData 程序更新结构体
  /*说    明
  /***********************************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_ULUpdate(const char* cszDevAddr, const DEV_APPUPDATE& cAppData, int nSeq = -1);

  /***********************************************************************************/
  /*功    能 启动视频
  /*参    数 cszDevAddr 设备地址
  hwnd   视频显示窗口句柄
  /*说    明
  /***********************************************************************************/
  typedef int (*frame_callback_t)(const char* cszIP, const unsigned char* cpbImage, int w, int h, int step, int nImageType, void* user);
  CPMDEV_API BOOL CPMDEV_CALL CPM_StartVideo(const char* cszDevAddr, void* hwnd, frame_callback_t frame_rgb, int nSeq = -1);

  /***********************************************************************************/
  /*功    能 停止视频
  /*参    数 cszDevAddr 设备地址
  /*说    明
  /***********************************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_StopVideo(const char* cszDevAddr, int nSeq = -1);


  /**********************************************************************
  功    能 批量上传用户
  参    数
  cszDevAddr 设备地址
  cUsers     批量用户数据
  nFlag      DL_DEV_PIC          //照片
             DL_DEV_USER_FEAT    //人脸特征
             DL_DEV_USER_TEXT    //用户文字信息
  说    明: 批量上传用户必须包含用户文字信息，组合如下
  nFlag = DL_DEV_USER_TEXT 上传文字信息
  nFlag = DL_DEV_USER_TEXT|DL_DEV_PIC  文字+注册照
  nFlag = DL_DEV_USER_TEXT|DL_DEV_USER_FEAT 文字+人脸库
  nFlag = DL_DEV_USER_TEXT|DL_DEV_PIC|DL_DEV_USER_FEAT文字+注册照+人脸库
  DevOperResultNotify回调参数对应类型：
  eType = DEV_USER_EX_OPER
  eFlag = DEV_OPER_UPLOAD ，
  pvContent =  DEV_BATCH_USER*。
  在OPER_SUCCESS 情况下，批量上传的用户个数与返回的用户个数一致。否则设备保存失败。批量上传用户数设定在5个以下(和网络带宽，主机处理速度有关)。
  **********************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_ULUserEx(const char* cszDevAddr, const DEV_BATCH_USER& cUsers, int nFlag, int nSeq = -1);

  /**********************************************************************
  功    能 批量下载用户
  参    数
  cszDevAddr    设备地址
  pUserIDArray  用户ID数组首地址
  nIDCount      数组元素个数
  nFlag         同CPM_ULUserEx
  说    明 DevOperResultNotify回调参数对应类型：
  eType = DEV_USER_EX_OPER
  eFlag = DEV_OPER_DOWNLOAD ，
  pvContent= DEV_BATCH_USER*。
  OPER_SUCCESS 情况下，如果批量下载的用户数与要求下载的用户数不一致，可以通过
  CPM_DLAllUser(const char* cszDevAddr, int nSeq = -1) 来确认用户是否真的存在于设备上。 批量下载用户数设定在5个以下(和网络带宽，主机处理速度有关)。
  **********************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_DLUserEx(const char* cszDevAddr, DEV_CID* pUserIDArray, int nIDCount, int nFlag, int nSeq = -1);

  /**********************************************************************
  功    能 批量删除用户
  参    数
  cszDevAddr     设备地址
  pUserIDArray   待删除用户的编号数组
  nIDCount       数组元素个数
  说    明 DevOperResultNotify回调参数对应类型：
  eType = DEV_USER_EX_OPER
  eFlag = DEV_OPER_DELETE，
  pvContent = DEV_BATCH_USER*。
  在OPER_SUCCESS 情况下，批量删除的用户个数与返回的用户个数不一致，可以通过CPM_DLAllUser(const char* cszDevAddr, int nSeq = -1)
  来确认用户是否真的存在于设备上。单次批量删除用户数设定在10个以下为好。
  **********************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_DELUserEx(const char* cszDevAddr, DEV_CID* pUserIDArray, int nIDCount, int nSeq = -1);


  /***********************************************************************************/
  /*功    能 批量下载记录
  /*参    数 cszDevAddr 设备地址
  cRange     ID区间
  /*说    明 包括验证记录、报警记录，操作日志、变更注册照
  /***********************************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_DLRecEx(const char* cszDevAddr, const DEV_DL_RECORD_RANGE& cRange, int nSeq = -1);


  /***********************************************************************************/
  /*功    能 重启设备
  /*参    数 cszDevAddr 设备地址
  /*说    明
  /***********************************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_RebootDev(const char* cszDevAddr, int nSeq = -1);

  /***********************************************************************************/
  /*功    能 获得/设置 设备网络信息
  /*参    数 cszDevAddr 设备地址
             pNetPro    NULL表示获得，不为空表示设置
  /*说    明
  /***********************************************************************************/
  CPMDEV_API BOOL CPMDEV_CALL CPM_NetPro(const char* cszDevAddr, DEV_NETPRO* pNetPro , int nSeq = -1);

#ifdef __cplusplus
}
#endif

#endif

