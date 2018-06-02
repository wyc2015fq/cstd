
#include "net.h"
#include "../thread.h"
#include "yuv2bmp.inl"
#include <assert.h>


//////////////////////////////////////////////////////////////////////


#define ID_LEN 24

//软件支持的功能模块
#define SUPPORT_NET 0x00000001 //支持socket联网
#define SUPPORT_2FRONT 0x00000002 //支持2前端
#define SUPPORT_GSOAP 0x00000004 //支持gsoap形式的web服务
#define SUPPORT_ENROLL 0x00000008 //支持专用采集模式
#define SUPPORT_MAINPIC 0x00000010 //支持待机界面修改
#define SUPPORT_FINGER 0x00000020 //支持指纹识别

//说明: M表示主命，区间0x01 - 0x80 ，S表示子命令，区间0x81-0xff
#define M_USER 0x01 //用户
#define M_VERIFY 0x02 //验证记录
#define M_ALARM 0x03 //报警记录
#define M_OPTLOG 0x04 //日志记录
#define M_CIMAGE 0x05 //变更注册照记录
#define M_RANGE 0x06 //区间集合数据下载
#define M_AUTH 0x07 //客户端身份验证
#define M_TIME 0x08 //系统时间
#define M_HEART 0x09 //心跳包
#define M_DEVINFO 0x0a //设备基本信息
#define M_STATIS 0x0b //统计信息
#define M_DEVSET 0x0c //设备基本设置
#define M_FORMAT 0x0d //设备格式化
#define M_DOOR_STATE 0x0e //门状态设置 ,共3种状态：0正常状态 ， 1常开状态， 2常闭状态
#define M_REMOTE_OPEN 0x0f //远程开关门,
#define M_TGROUP 0x10 //时间组
#define M_DSPRIGHT 0x11 //权限
#define M_UGROUP 0x12 //用户组
#define M_MCAST 0x13 //多播地址
#define M_APPUPDATE 0x14 //程序更新
#define M_VIDEO_OUT 0x23 //UDP网络视频传输 NET_ADDRESS
#define M_USER_EX 0x24 //批量用户上传现在
#define M_REBOOT 0x25 //设备重启
#define M_RANGE_EX 0x26 //记录批量下载
#define M_SET_IP 0x27 //远程修改IP地址
#define M_NOPEN 0x28 //N+1开门请求
#define M_IOCTRL 0x29 //IO控制
#define M_NETPRO 0x30 //网络设置

//子命令
#define S_SET 0x81 //上传\设置
#define S_DEL 0x82 //删除
#define S_GET 0x83 //下载\获得
#define S_REALTIME_SEND 0x84 //采集实时发送
#define S_REALTIME_LOAD 0x85 //实时加载
#define S_REALTIME_CAP 0x86 //实时用户采集
#define S_REALTIME_CAP_INFO 0x88 //远程采集实是进度信息
#define S_UPDATE_SEND 0x87 //更新实时发送
#define S_CLEAR_INOUT 0x88 //清空统计计数

//客户端与中转服务器主命令
#define C_CNDEV 0xB0 //client通过中转服务器设备连接
#define C_VERIFY 0xB1 //中转服务器验证client身份
#define C_HEART 0xB2 //中转服务器与client的心跳包
//客户端与中转服务器子命令
#define S_CN 0xc0 //建立连接
#define S_DN 0xc1 //断开连接

//命令执行错误状态
#define SUC_EXECUT 0x00 //成功
#define ERR_EXECUT 0x0A //失败
#define ERR_BUSY 0x01 //设备忙
#define ERR_LIMIT 0x02 //已达上限
#define ERR_NOFIND 0x03 //没有找到对应数据
#define ERR_SAVE_FAIL 0x04 //数据保存失败
#define ERR_SET_FAIL 0x05 //设置失败
#define ERR_VERIFY_FAIL 0x06 //验证失败
#define ERR_FROMAT  0x07 //格式化失败
#define ERR_PARAM   0x08 //参数错误
#define ERR_DISABLE 0x09 //要求执行的功能没有使能
#define ERR_SUPPORT 0x10 //不支持的命令
#define ERR_INPUTDATA 0x11 //网络端传输的数据有异常

//设置和获得错误状态
#define SET_ERR( state , err ) (state) = ( ((state) & 0xffffff00)+(err) )
#define GET_ERR( state ) ( (state) & 0xff )

#define SET_VMODE( cflag , vmode ) (cflag) = ( ((cflag)&0xff00ffff) + (((vmode)&0xff) << 16) )
#define GET_VMODE( cflag ) ( ((cflag)&0x00ff0000) >> 16 )

//GET/SET 0－3位数据
#define GET_TYPE_485( x ) ( (x)&0x0F )
#define SET_TYPE_485( x , v ) (x) = ( ((x)&0xF0) + (v) )
//GET/SET 获得4－7位数据
#define GET_INDEX_485( x ) ( ( (x)&0xF0 ) >> 4 )
#define SET_INDEX_485( x , v ) ( (x) = ( ((x)&0x0F) + ((v)<<4)) )

#define CMD_STATE_REPLY 0x40000000 //标记该命令为状态回复命令
#define CMD_DOWN_ID 0x80000000 //下载数据ID集合,优先检测该标记
#define CMD_ID_ALL 0x00000100 //下载所有数据的区间集合
#define CMD_ID_TIME_RANGE 0x00000200 //下载制定时间段区间集合
#define CMD_ID_RANGE 0x00000400 //确定指定区间的区间集合

//数据下载
#define DL_IMAGE 0x00000100 //需要下载照片
#define DL_FEAT 0x00000200 //需要下载特征
#define DL_APPENDID 0x00000400 //实时加载指示CID是编号还是卡号
#define LD_ENROLL 0x00000800 //需要远程采集
#define USER_TEXT 0x00000400 //用户文字信息
#define BATCH_END 0x00000800 //批量结束

//用户验证
#define AUTH_VERIFY 0x00000100 //用户身份验证

//门状态: 开，关，不指定为正常状态
#define DOOR_OPEN 0x00000100 //开
#define DOOR_CLOSE 0x00000200 //关

//时间组下载
#define DL_NORMAL_TG 0x00000100 //下载正常时间组
#define DL_HOLIDAY_TG 0x00000200 //下载节假日

//命令结构体，所有上传下载的网络数据,通过此数据结构来辨别数据类型
typedef struct {
  unsigned int m_MCmd; //主命令
  unsigned int m_SCmd; //子命令
  unsigned int m_CFlag; //特定命令配置参数，最后一个字节为命令执行状态字
  unsigned int m_Param1; //用户自定义参数1
  unsigned int m_Param2; //用户自定义参数2
} NCMD;

//日期
typedef struct {
  int m_Year;
  char m_Month;
  char m_Day;
  char m_Reserve[2]; //结构体4字节对齐
} NETDATE;

//时间
typedef struct {
  int m_Msec; //毫秒
  char m_Hour;
  char m_Minute;
  char m_Second;
  char m_Reserve[1];
} NETTIME;

//日期和时间
typedef struct {
  NETDATE m_Date;
  NETTIME m_Time;
} DATETIME;

//ID标识结构体
typedef struct {
  char m_ID[ ID_LEN ];
} CID;

//获得指定时间段的ID集合
typedef struct {
  NETDATE m_Start; //记录起始时间
  NETDATE m_End; //记录结束时间
} TIMESE;

#define DL_OPENINTERVAL 0x00000001 //下载指定记录流水号之后的N条记录，包括指定记录本身
typedef struct {
  //下载多少条,<=0 表示下载m_SID之后产生的所有记录,此场景下m_SID必须有效
  int m_Count;
  //DL_OPENINTERVAL打开开区间下载,否则闭区间下载
  int m_Flag;
  //从那一条开始，m_SID表示记录精确的流水号,
  //流水号字符串组成:
  //SN(MAC后3字节) + TYPE(记录类别) + DATETIME(日期) + ID(自定义) = (6BYTE + 1BYTE + 14BYTE + 2BYTE + \0)
  CID m_SID;
} RECORDRANGE;

//可变数组结构体。数组元素具体什么内容，需要根据NCMD设定的命令来解释
typedef struct {
  int m_Count; //数组元素的个数
  char m_Block[4]; //数组地址, 4是为了结构体的字节对齐
} CARRAY;

#define WEB_LOADUSER 0x0001 //加载用户
#define WEB_WRITEUSER 0x0002 //写用户
#define WEB_WRITEREC 0x0004 //写记录
#define WEB_SYNCTIME 0x0008 //同步时间

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
} NETPRO;

#define CHECK_TIME 0x01 //检测时间
#define CHECK_WEEK 0x02 //检测星期
#define CHECK_DAY 0x04 //检测日期
#define CHECK_MONTH 0x08 //检测月份
#define CHECK_YEAR 0x10 //检测年份

#define WEEK_1 0x01 //星期一有效
#define WEEK_2 0x02 //星期二有效
#define WEEK_3 0x04 //星期三有效
#define WEEK_4 0x08 //星期四有效
#define WEEK_5 0x10 //星期五有效
#define WEEK_6 0x20 //星期六有效
#define WEEK_7 0x40 //星期天有效

//任意时间有效 , 正常时间组，节假日时间组
enum { ANY_TIME = -1 , TG_NORMAL = 1, TG_HOLIDY = 2 };

//时间组
typedef struct {
  int m_TGID; //时间组ID
  DATETIME m_Start; //时间组开始时间
  DATETIME m_End; //时间组结束时间
  char m_TGType; //时间组类型, TG_NORMAL,TG_HOLIDY
  char m_CheckFlag; //时间检测标记例：m_CheckFlag = CHECK_TIME|CHECK_WEEK
  char m_WeekFlag; //检测星期时，标记那些星期有效。例：m_WeekFlag = WEEK_1|WEEK_5
  char m_Reserve; //保留字节
} DSPTIMEG;

//没有指定用户权限,用户默认权限ID为DEFAULT_NO_RIGHT或者DEFAULT_SINGLE_OPEN,DSP上采集的用户，
//超级管理员可以统一配置为DSP_DEFAULT_SINGLE_OPEN
//默认无权限，默认单一开门权限，组合用户最大组用户数，权限支持最大时间组
//DEFAULT_NO_RIGHT: DSP,管理端新增用户,默认值之一无权限
//DEFAULT_SINGLE_OPEN: DSP,管理端新增用户，默认值之二单一开门权限，此权限没有时间组概念
enum { DEFAULT_NO_RIGHT = -2, DEFAULT_SINGLE_OPEN = -1, MAX_GROUPUSER = 60, TGROUP_NUM = 8 };


#define CHECK_HOLIDY 0x01 //权限检测节假日
#define ACTION_LOCK 0x02 //电锁输出
#define ACTION_OUTPUT 0x04 //辅助输出
#define ACTION_N_1 0x08 //该权限需要N+1
//权限
typedef struct {
  //权限ID
  int m_RightID;

  //时间组ID , (m_TimeGID[0]==ANY_TIME)未指定时间组，开门方式不受时间限制，任意时间段验证成功执行开门方式.
  int m_TimeGID[TGROUP_NUM];

  //BIT(0):1 检测节假日, 不检测节假日
  //BIT(1):1 电锁, 1输出 0不输出
  //BIT(2):1 辅助, 1输出 0不输出
  //BIT(3):1 N＋1, 开启 0关闭
  int m_Mode;
} DSPRIGHT;


//表示用户不参与任何组
#define INVALID_GROUPID -1

//组合有序
//用户组合
//有序组合开门: 同一组用户必须有序验证，必须先验证完强制用户，在验证普通用户
//无序组合开门: 不管以何种顺序验证，只要最后验证总结果打到组合验证的要求，验证通过.
#define G_ORDER 0x01


typedef struct {
  //组ID
  int m_GroupID;
  //BIT(0):1有序，0无序
  char m_GroupFlag;
  //组合成功后，输出参数ACTION_LOCK|ACTION_OUTPUT|ACTION_N_1
  char m_Param;
  //普通用户中有效用户数
  char m_NormalValid;
  //强制用户中有效用户数
  char m_ForceValid;
  //普通用户组合，优先级低
  CID m_NormalUsers[MAX_GROUPUSER];
  //强制用户组合，优先级高
  CID m_ForceUsers[MAX_GROUPUSER];
} USERGROUP;

typedef struct {
  int m_GroupID;
  int m_StrSize;
  char m_GroupFlag;
  char m_MinNum;
  char m_BLock[2];
} GROUPEX;

typedef struct {
  int m_UserType; //终端设备上的用户类型(普通，管理，超管 -- 0 , 1 , 2)
  CID m_UserID; //用户ID
  DATETIME m_TypeUpdTime; //用户类型更改时间
  DATETIME m_RegistTime; //人脸注册时间，即特征采集时间，采集或者重新采集要更新此时间
  DATETIME m_LastUpdTime; //最后更新的时间，识别时特征发生更新或者重新采集要更新此时间
} VUSER;

//用户信息
#define USER_CARD_INVALID 0x01 //卡挂失
#define USER_DLine_VALID 0x02 //失效时间有效
#define USER_BLACK 0x04 //黑名单
#define USER_MIX1N 0x08 //1N人脸认证
#define USER_VIPCARD 0x10 //VIP用户
#define USER_FINGER_PRINT 0x20 //指纹认证
#define USER_SYNC_FINGEER 0x40 //指纹同步

typedef struct {
  int m_FeatBytes; //人脸特征字节数，==0 表示没有人脸特征
  int m_PicBytes; //注册图像的字节数，==0 表示没有图片
  int m_RightID; //用户权限ID,三种可能值(DEFAULT_NO_RIGHT,DEFAULT_SINGLE_OPEN,用户指定)
  CID m_ImageSID; //上传注册照，对应的变更ID，全局唯一
  CID m_UserID; //用户标识
  CID m_Admin; //人脸注册管理员,标识此用户的人脸特征是哪个管理员采集
  CID m_AppendID; //卡号或者其他用于1:1的附加身份确认信息
  CID m_UserName; //用户名
  CID m_Department; //部门名称
  DATETIME m_DeadLine; //失效时间
  DATETIME m_RegistTime; //人脸注册时间，即特征采集时间，采集或者重新采集要更新此时间
  DATETIME m_LastUpdTime; //最后更新的时间，识别时特征发生更新或者重新采集要更新此时间
  DATETIME m_TypeUpdTime; //用户类型变更时间
  char m_UserFlag; //用户状态标记
  char m_UserType; //终端设备上的用户类型(普通，管理，超管 -- 0 , 1 , 2)
  char m_Block[2]; //注册图片,必须JPG,人脸特征是与m_UserID唯一绑定的.m_Block的前m_FeatBytes个字节表示人脸特征, 后m_PicBytes个字节表示图片.
} USER;

//记录类型:
//0x01 - 0x20: 正常事件
//0x20 - 0x40: 硬件报警事件
//0x40 - 0x50: 辅助输入
//0x50 - 0x60: 辅助输出
//0xf0 - 0xff: 软件报警事件
//其他: 未定义
enum {
  VERIFY_SUCC = 0x01, //验证成功
  VERIFY_FAIL = 0x02, //验证失败
  ADMIN_SUCC = 0x03, //管理员验证成功
  EMER_OPEN = 0x04, //紧急开门
  RIGHT_OPEN = 0x05, //权限开门
  GROUP_OPEN = 0x06, //组合开门
  BUTTON_OPEN = 0x07, //按钮开门

  ALARM_HD_MANGET_TIMEOUT = 0x20, //门磁超时
  ALARM_HD_MANGET_ILLOPEN = 0x21, //门磁非法开门
  ALARM_HD_OFFLINE = 0x22, //前端掉线报警
  ALARM_HD_BREAK = 0x30, //防拆报警
  ALARM_HD_SHOCK = 0x31, //震动报警
  ALARM_HD_FPOWR = 0x36, //前端供电异常报警
  ALARM_HD_UPS_ON = 0x37, //UPS备用电池开启
  ALARM_HD_UPS_OFF = 0x38, //UPS备用电池关闭

  ALARM_HD_ASSIST = 0x40, //辅助输入触发
  ALARM_HD_WL_DOOR_MAGNETIC = 0x41, //无线
  ALARM_HD_WL_SMOKE = 0x42,
  ALARM_HD_WL_GAS = 0x43,
  ALARM_HD_WL_BUTTON1 = 0x44,
  ALARM_HD_WL_BUTTON2 = 0x45,
  ALARM_HD_WL_BUTTON3 = 0x46,
  ALARM_HD_WL_BUTTON4 = 0x47,
  ALARM_HD_WL_AREA1 = 0x48,
  ALARM_HD_WL_AREA2 = 0x49,
  ALARM_HD_WL_AREA3 = 0x4A,
  ALARM_HD_WL_AREA4 = 0x4B,
  ALARM_HD_WL_AREA5 = 0x4C,
  ALARM_HD_WL_AREA6 = 0x4D,
  ALARM_HD_WL_AREA7 = 0x4E,

  ALARM_AB_RULE = 0xE0,

  ALARM_SF_BACKLIST = 0xF0, //黑名单验证报警
  ALARM_SF_ILLCARD = 0xF1, //无效卡(挂失)
  ALARM_SF_ILLTIME = 0xF2, //非法时间识别
  ALARM_SF_DEADLINE = 0xF3, //失效时间
  ALARM_SF_DANGER_OPEN = 0xF4, //胁迫开门
  ALARM_SF_SUPER_OPEN = 0xF5, //超级密码开门
  LINK_EVENT = 0xF6, //联动事件
};

//记录信息,识别记录，报警记录
typedef struct {
  int m_RecType; //记录类型
  int m_Score; //识别得分 , 联动事件为联动事件的LINKID
  int m_PicBytes; //记录图像的字节数，==0 表示没有记录图片
  CID m_ChangID; //注册照记录号
  CID m_UserID; //用户ID,如果为空，表示非法记录 , 联动事件为事件描述Text
  CID m_SerialID; //识别记录流水号ID
  CID m_AppendID; //卡号或者其他用于1:1的附加身份确认信息
  CID m_UserName; //用户名
  CID m_Department; //部门名称
  DATETIME m_RecTime; //记录时间
  char m_VerifyMode; //验证模式,例:VERIFY_CARD|VERIFY_FACE_1N,刷卡+人脸
  char m_Source; //事件源设备的485地址
  char m_Block[2]; //记录图像,必须JPG , 2是为了结构体的字节对齐
} RECORD;

//操作日志类型
enum {
  DSPLOG_ADDUSER = 0x01, //增加用户
  DSPLOG_DELUSER = 0x02, //删除用户
  DSPLOG_REREGIST = 0x03, //重新采集
  DSPLOG_CAHNGETYPE = 0x04, //改变用户类型
  DSPLOG_UDISK_ULUSER = 0x05, //U盘上传用户
  DSPLOG_UDISK_DLUSER = 0x06, //U盘下载用户
  DSPLOG_UDISK_DLRECORD = 0x07 , //U盘下载记录
  DSPLOG_UDISK_UPDATEAPP = 0x08, //U盘更新程序
};

//设备操作日志
typedef struct {
  int m_LogType; //日志类型
  CID m_SerialID; //流水号ID
  CID m_Admin; //操作员
  CID m_BeOptUser; //被操作员
  DATETIME m_RecTime; //记录时间
} DSPLOG;

//变更注册照
typedef struct {
  int m_PicBytes; //注册照大小
  CID m_UserID; //用户ID
  CID m_SerialID; //流水号ID
  CID m_AppendID; //卡号或者其他用于1:1的附加身份确认信息
  CID m_UserName; //用户名
  CID m_Department; //部门名称
  CID m_Admin; //人脸注册管理员,标识此用户的人脸特征是哪个管理员采集
  DATETIME m_RecTime; //记录时间
  char m_Block[4]; //注册照内存
} CHANGEIMAGE;

//DSP相关统计信息
typedef struct {
  int m_TotalUsers; //当前设备的总用户数
  int m_NoFeatUser; //没有采集人脸特征的用户数
  int m_TotalDays; //识别记录保存总天数
  int m_TotalRecords; //总记录数
  int m_TotalAlarm; //总报警记录数
  int m_TotalDspLog; //总操作日志数目
  int m_TotalChangeImage; //总变更注册照
} DEVSTATIS;

//验证信息
typedef struct {
  CID m_UserName; //用户名
  CID m_Password; //密码
} AUTH;

//设备基本信息
typedef struct {
  int m_DevType; //设备类型
  int m_LimitUser; //总人数上限
  int m_Limit1NUser; //1N用户上限
  CID m_SN; //设备编号
  CID m_Ver; //DSP软件版本
  CID m_Space; //磁盘容量信息
} DEVINFO;

//远程采集进度信息
typedef struct {
  int m_Total; //采集总数
  int m_Count; //当前采集数
  CID m_UserID; //用户登记编号
} RCAPINFO;

//2.0版支持的功能
#define RECORD_SAVEFAIL 0x00000001 //是否保存识别失败记录
#define SUPER_PASSWORD 0x00000002 //超级密码开门有效
#define HDBEEP_OPEN 0x00000004 //撤防/布防(硬件报警蜂鸣器开启)
#define REALTIME_RECORD 0x00000010 //实时动态记录显示
#define REALTIME_USERLOAD 0x00000020 //输入编号或者卡号识别时，如果无法在当前dsp中获得人脸特征,在网络连接状态下，将从管理端实时加载人脸特征
#define REALTIME_USERSEND 0x00000040 //采集用户同步
#define DOORMANGET_OPEN 0x00000080 //开启门磁检测报警
#define DOORFORCE_OPEN 0x00000100 //开启胁迫开门功能
#define REMOTE_CAP_SAVE 0x00000200 //远程控制采集的用户是否保存到DSP
#define GPRS_OPEN 0x00000400 //开启GPRS模块
#define UPDATE_USERSEND 0x00000800 //特征更新时，是否实时发送用户特征
#define REALTIME_USERLOAD_PRIOR 0x00001000 //网络连接情况下，优先网络加载
#define AB_LOCK 0x00002000 //AB互锁
#define DOOR1_NOPEN 0x00004000 //门一N+1
#define DOOR2_NOPEN 0x00008000 //门二N+1
#define ENABLE_COUNT 0x00010000 //在线记录数统计
#define ENCRYP_DISPALY 0x00020000 //在线记录数统计

//验证模式优先级低-高:UID<CARD<11<MIX<1N
#define VERIFY_USERID 0x01 //编号
#define VERIFY_CARD 0x02 //刷卡
#define VERIFY_FACE_11 0x04 //11人脸加载
#define VERIFY_FACE_MIX 0x08 //混合人脸加载
#define VERIFY_FACE_1N 0x10 //1N人脸加载
#define VERIFY_FINGER 0x20 //指纹认证

#define OUTPUT_WG_YES 0x01 //输出伟根协议
#define OUTPUT_WG_34 0x02 //指定输出伟根34，不指定输出伟根26
#define OUTPUT_WG_CTX 0x04 //伟根输出指定内容
#define OUTPUT_WG_UID 0x08 //不标记OUTPUT_WG_CTX，输出用户ID，默认输出卡号

#define WG_OLD 0
#define WG_26 1
#define WG_34 2
#define WG_35 3
#define WG_36 4
#define WG_37 5
#define WG_44 6
#define WG_45 7

//设备工作属性设置
typedef struct {
  //设备时间组,(m_TimeGID[0]==ANY_TIME)未指定时间组,设备任意时间有效.设备时间组优先级高于用户权限时间组
  int m_DevCtx[ TGROUP_NUM ];
  //设备默认权限: DEFAULT_NO_RIGHT , DEFAULT_SINGLE_OPEN
  int m_DefaultRight;
  //参见2.0版本定义的标记
  int m_BaseSet;
  //超级密码, 将产生一条报警记录，并且会抓拍彩色全景图.
  CID m_SuperPassword;
  //门磁延时时间，单位秒
  char m_DoorMangetTime;
  //电锁持续时间，单位秒
  char m_LockTime;
  //用户身份验证模式,例如:VERIFY_USERID|VERIFY_CARD|VERIFY_FACE_11
  char m_VerifyMode;
  //BIT(0) : 0不输出扩展韦根信号,1输出.
  //BIT(1) : 0输出韦根26,1输出韦根34.
  //BIT(2) : 0按照BIT(4)输出 , 1输出m_OutputCtx中指定内容
  //BIT(4) : 0直接输出卡号,1输出编号
  //BIT(4-7): WG输出协议,0-wg26 1-wg34 2-wg35 3-wg36 4-wg37 5-wg44 6-wg45
  char m_OutputWG;
  //韦根协议输出内容
  int m_OutputCtx;
} DEVSET;

//程序更新结构体
typedef struct {
  int m_Bytes; //文件大小
  CID m_FileName; //文件名
  char m_TotalFiles; //需要更新的文件总数
  char m_FileIndex; //当前更新到第几个
  char m_Block[2]; //文件数据
} APPUPDATE;

//IO设备状态
#define IO_MODE_NORMAL 0x00 //正常状态
#define IO_MODE_OPEN 0x01 //常开状态
#define IO_MODE_CLOSE 0x02 //常关状态
//门磁状态
#define IO_STATE_CLOSE 0x00 //门磁关
#define IO_STATE_OPEN 0x01 //门磁开
//执行动作
#define ACT_IO_OPEN 0x02 //执行打开动作
#define ACT_IO_OPENEX 0x04 //执行辅助动作
#define ACT_IO_CLOSE 0x10 //执行关闭动作
#define ACT_MODE_SET 0x20 //设置IO设备工作模式
#define ACT_MODE_GET 0x40 //获得IO设备工作模式
#define ACT_STATE_GET 0x80 //获得IO设备当前状态

typedef struct {
  //IO设备485地址
  char m_Source;
  //执行动作
  char m_Action;
  //IO设备当前模式
  char m_IOMode;
  //IO设备当前状态状态
  char m_IOState;
} IOCTRL;

//NOPEN:远程请求管理端开门
typedef struct {
  //m_Users==1权限ID, m_Users>1组合ID
  int m_CtxID;
  //验证用户数目： 0密码开门，1权限开门，>1组合开门
  int m_Users;
  //控制信息
  IOCTRL m_XOpen;
  //验证用户数组
  CID m_IDS[MAX_GROUPUSER * 2];
} NOPEN;

//关于485地址解析:
//1.485地址用一个字节表示，[4-7]表示设备索引号1-15 , [0-3]表示支持16种不同型的设备
// 已经定义的设备类型 0--人脸前端 , 1--读卡器 ,
// 例如: 0x11 表示1号门人脸识别前端1， INDEX[4-7] == 1 TYPE[0-3] == 1
// 0x10 表示1号门人脸识别前端0，INDEX[4-7] == 1 TYPE[0-3] == 0
// 0x12 表示1号门刷卡器0 INDEX[4-7] == 1 TYPE[0-3] == 2
// 0x13 表示1号门刷卡器1 INDEX[4-7] == 1 TYPE[0-3] == 3
// 0x1F 表示门点本身 INDEX[4-7] == 1 TYPE[0-3] == F
// 0x14 表示辅助输入1 INDEX[4-7] == 1 TYPE[0-3] == 4
// 0x25 表示辅助输出2 INDEX[4-7] == 2 TYPE[0-3] == 5
//2. 所有设备的序号从1开始。此协议可支持15张门，16中不同类型的门点输入设备，15个辅助输入和15个辅助输出
//3. [0-7] == 0x00 表示非法地址，[0-7] ＝＝ 0x01表示后端板本身
//7_______3______0
//|_INDEX_|__type__|

#define DEV_TYPE_FACE1  0x0 //人脸前端1
#define DEV_TYPE_FACE2  0x1 //人脸前端2
#define DEV_TYPE_CARD1  0x2 //刷卡器1
#define DEV_TYPE_CARD2  0x3 //刷卡器2
#define DEV_TYPE_IN     0x4 //辅助输入
#define DEV_TYPE_OUT    0x5 //辅助输出
#define DEV_TYPE_BUTTON 0x6 //出门按钮
#define DEV_TYPE_DOOR   0xF //门点本身

#define IMAGE_UYVY  0x01
#define IMAGE_YUYV  0x02
#define IMAGE_RGB24 0x03

typedef struct {
  INT32 m_Size; //图像数据总大小BYTES
  INT32 m_W; //图像宽
  INT32 m_H; //图像高
  INT32 m_Step; //行数据步长
  char m_Type; //图像类型
  char m_Block[3]; //图像数据
} DEVIMAGE;


#define ALIGN4( size )     ( ((size)+3) & ~3 )
#define USER_STEP( obj )   ALIGN4( sizeof(USER)+ (obj).m_FeatBytes+(obj).m_PicBytes )
#define RECORD_STEP( obj ) ALIGN4( sizeof(RECORD)+(obj).m_PicBytes-2 )
#define CGIMG_STEP( obj )  ALIGN4( sizeof(CHANGEIMAGE)+(obj).m_PicBytes-4 )
#define DSPLOG_STEP( obj ) ALIGN4( sizeof(DSPLOG) )



#ifndef BOOL
typedef int BOOL;
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define DEV_REGION_ADDR ("224.0.1.100") //默认区域地址,用户设备自动自动搜索 
#define DEV_ID_LEN 24 //ID长度
#define DEV_TIMEGROUP_NUMS 8 //时间组个数
#define DEV_USER_COMBINS 6 //用户组合个数

#define DL_DEV_PIC 0x01 //照片
#define DL_DEV_USER_FEAT 0x02 //人脸特征
#define DL_DEV_USER_TEXT 0x04 //用户文字信息

#define DEV_CHECK_TIME 0x01 //检测时间
#define DEV_CHECK_WEEK 0x02 //检测星期
#define DEV_CHECK_DAY 0x04 //检测日期
#define DEV_CHECK_MONTH 0x08 //检测月份
#define DEV_CHECK_YEAR 0x10 //检测年份

#define DEV_WEEK_1 0x01 //星期一有效
#define DEV_WEEK_2 0x02 //星期二有效
#define DEV_WEEK_3 0x04 //星期三有效
#define DEV_WEEK_4 0x08 //星期四有效
#define DEV_WEEK_5 0x10 //星期五有效
#define DEV_WEEK_6 0x20 //星期六有效
#define DEV_WEEK_7 0x40 //星期天有效

#define DEV_RECORD_SAVEFAIL 0x00000001 //是否保存识别失败记录
#define DEV_SUPER_PASSWORD 0x00000002 //超级密码开门有效
#define DEV_HDBEEP_OPEN 0x00000004 //布防/撤防
#define DEV_REALTIME_RECORD 0x00000010 //实时动态记录发送
#define DEV_REALTIME_USERLOAD 0x00000020 //输入编号或者卡号识别时，如果无法在当前dsp中获得人脸特征,在网络连接状态下，将从管理端实时加载人脸特征
#define DEV_REALTIME_USERSEND 0x00000040 //通过设备采集的用户实时发送到管理终端
#define DEV_DOORMANGET_OPEN 0x00000080 //开启门磁检测报警
#define DEV_DOORFORCE_OPEN 0x00000100 //开启胁迫开门功能
#define DEV_REMOTE_CAP_SAVE 0x00000200 //远程采集的用户是否保存到DSP
#define DEV_GPRS_OPEN 0x00000400 //开启GPRS模块
#define DEV_UPDATE_USERSEND 0x00000800 //特征更新时，是否实时发送用户特征
#define DEV_AB_LOCK 0x00002000 //AB互锁
#define DEV_DOOR1_NOPEN 0x00004000 //门一N+1
#define DEV_DOOR2_NOPEN 0x00008000 //门二N+1

//验证模式优先级低-高:UID->CARD->11->MIX->1N
#define DEV_VERIFY_USERID 0x01 //工号
#define DEV_VERIFY_CARD 0x02 //刷卡
#define DEV_VERIFY_FACE_11 0x04 //11人脸加载
#define DEV_VERIFY_FACE_MIX 0x08 //混合人脸加载
#define DEV_VERIFY_FACE_1N 0x10 //1N人脸加载
//用户状态信息
#define DEV_USER_CARD_INVALID 0x01 //卡挂失
#define DEV_USER_DLine_INVALID 0x02 //失效时间有效
#define DEV_USER_BLACK 0x04 //黑名单
#define DEV_USER_MIX1N 0x08 //混合模式下1N用户
//SDK自定义
#define DEV_USER_FEAT_BASE64 0x40 //特征经过base64编码
#define DEV_USER_IMAGE_BASE64 0x80 //注册照经过base64编码

typedef enum {
  DEV_CONNECT_CUT = 0x00, //断开连接
  DEV_CONNECT_SUCCESS, //建立连接
  DEV_CONNECT_FAILUE , //连接失败
  DEV_CONNECT_NONE //未建立连接
} DEV_CONNECT_STATUS;

typedef enum {
  DEV_OPER_UPLOAD = 0x01, //上传
  DEV_OPER_DOWNLOAD = 0x02, //下载
  DEV_OPER_DELETE = 0x04 //删除
} DEV_OPER_FLAG;

typedef enum {
  DEV_AUTH_OPER = 0x05, //设备验证操作
  DEV_AUTH_SET_OPER, //设备验证用户设置操作
  DEV_REGION_OPER , //设备区域操作
  DEV_FORMAT_OPER, //设备格式化操作
  DEV_UPDATE_OPER, //设备程序更新操作
  DEV_SYS_TIME_OPER , //设备系统时间操作
  DEV_BASEINFO_OPER, //基本信息操作
  DEV_STATISINFO_OPER, //统计信息操作
  DEV_WORKATT_OPER, //工作属性操作
  DEV_USER_OPER , //用户操作
  DEV_USER_RT_OPER, //用户实时加载操作
  DEV_USER_RANGE_OPER, //用户下载数据条数回馈操作
  DEV_RECORD_OPER , //记录操作
  DEV_RECORD_RANGE_OPER, //记录下载数据条数回馈操作
  DEV_ALARM_OPER,
  DEV_ALARM_RANGE_OPER,
  DEV_LOG_OPER, //日志操作
  DEV_LOG_RANGE_OPER, //日志下载数据条数回馈操作
  DEV_CHANGEIMAGE_OPER, //变更注册照操作
  DEV_CIMAGE_RANGE_OPER, //变更注册照下载数据条数回馈操作
  DEV_TIME_GROUP_OPER, //时间组操作
  DEV_RIGHT_OPER, //权限操作
  DEV_USERGROUP_OPER, //用户组操作
  DEV_DOOR_STATE_OPER, //开门状态操作
  DEV_REMOTE_OPEN_DOOR, //远程开门操作
  DEV_VIDEO_TRANSFER, //视频传输
  DEV_USER_EX_OPER, //用户批量操作
  DEV_RECORD_EX_OPER, //记录批量操作
  DEV_LOG_EX_OPER, //日志批量操作
  DEV_CIMAGE_EX_OPER, //变更注册照批量操作
  DEV_REBOOT_OPER, //设备重启
  DEV_USER_REMOTE_CAP, //用户远程采集
  DEV_NOPEN_OPER, //N+1开门
  DEV_IOCTRL_OPER, //IOCTRL控制信息
  CLI_AUTH_OPER, //client像中转服务器验证操作
  DEV_NETPRO_OPER, //设备网络属性设置
} DEV_OPER_TYPE;

typedef enum {
  OPER_SUCCESS = 0x00, //成功
  OPER_ERR_BUSY = 0x01, //设备忙
  OPER_ERR_LIMIT = 0x02, //已达上限
  OPER_ERR_NOFIND = 0x03, //没有找到对应数据
  OPER_ERR_SAVEFAIL = 0x04, //数据保存失败
  OPER_ERR_SETFAIL = 0x05, //设置失败
  OPER_ERR_FROMAT = 0x07, //格式化失败
  OPER_ERR_PARAM = 0x08, //参数错误
  OPER_ERR_DISABLE = 0x09, //要求执行的功能没有使能
  OPER_ERR_EXECUT = 0x0A, //失败
  OPER_ERR_SUPPORT = 0x10, //不支持的命令
  OPER_ERR_INPUTDATA = 0x11, //网络端传输的数据有异常
  OPER_BATCH_DL_COMPLETE = 0x1F //批量下载完成
} DEV_OPER_RESULT; //操作结果


typedef enum {
  DEV_NORMAL = 0x01, //正常时间
  DEV_HOLIDY = 0x02, //节假日
  DEV_ANY_TIME = 0x03 //任意时间
} DEV_TIMEGROUP_TYPE; //时间组类型

typedef enum {
  DEV_USER_NOMAL = 0, //普通
  DEV_USER_ADMIN = 1, //管理员
  DEV_USER_SUPERADMIN = 2 //超级管理员
} DEV_USER_TYPE; //用户类型

typedef enum {
  DEV_DEFAULT_NO_RIGHT = -2, //无权限
  DEV_DEFAULT_SINGLE_OPEN = -1 //单一开门权限
} DEV_DEFAULT_RIGHT; //默认权限

//记录类型:
//0x01 - 0x20: 日常记录
//0x20 - 0xFF: 报警记录
typedef enum {
  DEV_VERIFY_SUCC = 0x01, //验证成功
  DEV_VERIFY_FAIL = 0x02, //验证失败
  DEV_ADMIN_SUCC = 0x03, //管理员验证成功
  DEV_EMER_OPEN = 0x04, //紧急开门
  DEV_RIGHT_OPEN = 0x05, //权限开门
  DEV_GROUP_OPEN = 0x06, //组合开门
  DEV_BUTTON_OPEN = 0x07, //按钮开门
  DEV_ALARM_HD_MANGET_TIMEOUT = 0x20, //门磁超时
  DEV_ALARM_HD_MANGET_ILLOPEN = 0x21, //门磁非法开门
  DEV_ALARM_HD_OFFLINE = 0x22, //前端掉线报警
  DEV_ALARM_HD_BREAK = 0x30, //防拆报警
  DEV_ALARM_HD_SHOCK = 0x31, //震动报警
  DEV_ALARM_HD_FPOWR = 0x36, //前端供电异常报警
  DEV_ALARM_HD_UPS_ON = 0x37, //UPS备用电池开启
  DEV_ALARM_HD_UPS_OFF = 0x38, //UPS备用电池关闭
  DEV_ALARM_HD_ASSIST = 0x40, //辅助输入触发
  DEV_ALARM_SF_BACKLIST = 0xF0, //黑名单验证报警
  DEV_ALARM_SF_ILLCARD = 0xF1, //无效卡(挂失)
  DEV_ALARM_SF_ILLTIME = 0xF2, //非法时间识别
  DEV_ALARM_SF_DEADLINE = 0xF3, //失效时间
  DEV_ALARM_SF_DANGER_OPEN = 0xF4, //胁迫开门
  DEV_ALARM_SF_SUPER_OPEN = 0xF5 //超级密码开门
} DEV_REC_TYPE; //记录类型

typedef enum {
  DEV_DOOR_SOURCE = 0x01, //门相关来源
  DEV_SIGNAL_SOURCE, //信号输入输出相关来源
  DEV_NO_NUM_DEV_SOURCE //无编号设备来源
} DEV_REC_SOURCE_TYPE;

typedef enum {
  DEV_LOG_ADDUSER = 0x01, //增加用户
  DEV_LOG_DELUSER = 0x02, //删除用户
  DEV_LOG_REREGIST = 0x03, //重新采集
  DEV_LOG_CAHNGETYPE = 0x04, //改变用户类型
  DEV_UDISK_ULUSER = 0x05, //U盘上传用户
  DEV_UDISK_DLUSER = 0x06, //U盘下载用户
  DEV_UDISK_DLRECORD = 0x07, //U盘下载记录
  DEV_UDISK_UPDATEAPP = 0x08 //U盘更新程序
} DEV_LOG_TYPE;

typedef enum {
  DEV_DOOR_NOMAL = 0x00000000, //正常状态
  DEV_DOOR_OPEN = 0x00000100, //开状态
  DEV_DOOR_CLOSE = 0x00000200 //关状态
} DEV_DOOR_STATE;

typedef struct {
  char m_ID[ DEV_ID_LEN ];
} DEV_CID; //ID标识结构体

typedef struct {
  int m_Year;
  char m_Month;
  char m_Day;
} DEV_DATE; //日期

typedef struct {
  int m_Msec; //毫秒
  char m_Hour;
  char m_Minute;
  char m_Second;
} DEV_TIME; //时间

typedef struct {
  DEV_DATE m_Date;
  DEV_TIME m_Time;
} DEV_DATETIME; //设备日期和时间

typedef struct {
  int m_DevType; //设备类型
  int m_LimitUser; //总使用人数上限
  int m_Limit1NUser; //1N用户上限
  DEV_CID m_SN; //设备编号
  DEV_CID m_Ver; //DSP软件版本
  DEV_CID m_Space; //磁盘容量信息
} DEV_BASEINFO; //设备基本信息

typedef struct {
  int m_TotalUsers; //当前设备的总用户数
  int m_NoFeatUser; //没有采集人脸特征的用户数
  int m_TotalDays; //识别记录保存总天数
  int m_TotalRecords; //总记录数
  int m_TotalAlarm; //总报警记录数
  int m_TotalDspLog; //总操作日志数目
  int m_TotalChangeImage; //总变更注册照
} DEV_STATIS; //设备统计信息

typedef struct {
  int m_TimeGID[DEV_TIMEGROUP_NUMS]; //时间组ID
  int m_BaseSet; //参见设备功能标记定义
  int m_DoorMangetTime; //门磁延时时间，单位秒
  int m_LockTime; //电锁持续时间，单位秒
  int m_VerifyMode; //用户身份验证模式,例如:DEV_VERIFY_USERID|DEV_VERIFY_CARD|DEV_VERIFY_FACE_11
  int m_nWGType; //韦根协议类型(0输出韦根26,1输出韦根34)
  int m_nWGOutType; //韦根输出类型(0输出ID，1输出WG内容)
  int m_nWGOutIDType; //输出ID类型(0输出卡号,1输出用户ID)
  int m_nWGOutContent; //WG输出内容
  BOOL m_bWGOutPut; //是否WG输出
  DEV_CID m_szSuperPWD; //超级密码
  DEV_DEFAULT_RIGHT m_DefaultRight; //设备默认权限
} DEV_WORKATT; // 设备工作信息

typedef struct {
  DEV_USER_TYPE m_UserType; //终端设备上的用户类型(普通，管理，超管 -- 0 , 1 , 2)
  DEV_CID m_UserID; //用户ID
  DEV_DATETIME m_TypeTime; //用户类型更改时间
  DEV_DATETIME m_RegistTime; //人脸注册时间，即特征采集时间，采集或者重新采集要更新此时间
  DEV_DATETIME m_LastUpdTime; //最后更新的时间，识别时特征发生更新或者重新采集要更新此时间
} DEV_VUSER;

typedef struct {
  int m_FeatLen; //特征大小
  int m_PicLen; //照片大小
  int m_RightID; //用户权限ID
  DEV_CID m_ImageSID; //上传注册照，对应的变更ID，全局唯一
  DEV_CID m_UserID; //用户唯一ID,关键字
  DEV_CID m_Admin; //人脸注册管理员
  DEV_CID m_AppendID; //卡号或者其他用于1:1的附加身份确认信息
  DEV_CID m_UserName; //用户名
  DEV_CID m_Department; //部门名称
  DEV_DATETIME m_DeadLineTime; //失效时间
  DEV_DATETIME m_RegistTime;  //人脸注册时间
  DEV_DATETIME m_LastUpdTime; //最后更新的时间
  DEV_DATETIME m_TypeUpdTime; //用户类型变更时间
  char m_UserFlag; //用户状态标记
  DEV_USER_TYPE m_UserType; //终端设备上的用户类型
  char* m_FeatData; //特征数据
  char* m_PicData; //照片数据
  int m_bRTUser; //是否为实时用户(0 非实时用户 1 实时采集用户 2 实时特征更新用户)
} DEV_USER; //用户信息

typedef struct {
  int m_Count; //下载多少条
  DEV_VUSER* m_pArray; //ID集合
} DEV_DL_USER_RANGE; //用户下载集合

//关于记录和事件来源485地址解析:
//1.485地址用一个字节表示，高4位[4-7]表示设备索引号1-15 , 低4位[0-3]表示支持16种不同型的设备
// #define DEV_TYPE_FACE1 0x0 //人脸前端1
// #define DEV_TYPE_FACE2 0x1 //人脸前端2
// #define DEV_TYPE_CARD1 0x2 //刷卡器1
// #define DEV_TYPE_CARD2 0x3 //刷卡器2
// #define DEV_TYPE_IN 0x4 //辅助输入
// #define DEV_TYPE_OUT 0x5 //辅助输出
// #define DEV_TYPE_INWL 0x6 //无线输入
// #define DEV_TYPE_DOOR 0xF //门点本身
// 例如: 0x11 表示1号门人脸识别前端1，INDEX[4-7] == 1 TYPE[0-3] == 1
// 0x10 表示1号门人脸识别前端0，INDEX[4-7] == 1 TYPE[0-3] == 0
// 0x12 表示1号门刷卡器0 INDEX[4-7] == 1 TYPE[0-3] == 2
// 0x13 表示1号门刷卡器1 INDEX[4-7] == 1 TYPE[0-3] == 3
// 0x1F 表示门点本身 INDEX[4-7] == 1 TYPE[0-3] == F
// 0x14 表示辅助输入1 INDEX[4-7] == 1 TYPE[0-3] == 4
// 0x25 表示辅助输出2 INDEX[4-7] == 2 TYPE[0-3] == 5
//2. 所有设备的序号从1开始。此协议可支持15张门，16中不同类型的门点输入设备，15个辅助输入和15个辅助输出
//3. 特殊意义的地址: [0-7] == 0x00 表示非法地址，[0-7] ＝＝ 0x01表示后端板本身, [0-7] ＝＝ 0x02表示GPRS模块
//7_______3________0
//|_INDEX_|__type__|

typedef struct {
  int m_LoadFlag; //DL_DEV_PIC|DL_DEV_USER_FEAT=需要加载注册照片和人脸特征
  int m_ReqSource; //请求来源,详见485地址解析
  BOOL m_bApeendID; //TRUE=卡号, FALSE=工号
  DEV_CID m_ID; //加载ID
} DEV_USER_REAL_LOAD; //实时用户加载

typedef struct {
  int m_RecType; //记录类型
  int m_Source; //记录来源(已经过时,不在使用)
  BOOL m_bRealTime; //是否为实时记录
  int m_Score; //识别得分
  int m_PicLen; //照片大小
  int m_VerifyMode; //验证模式,例如:DEV_VERIFY_USERID|DEV_VERIFY_CARD|DEV_VERIFY_FACE_11
  DEV_CID m_ChangID; //变更记录号
  DEV_CID m_SerialID; //识别记录流水号ID
  DEV_CID m_UserID; //用户ID,如果为空，表示非法记录
  DEV_CID m_AppendID; //卡号或者其他用于1:1的附加身份确认信息
  DEV_CID m_UserName; //用户名
  DEV_CID m_Department; //部门名称
  DEV_DATETIME m_RecTime; //记录时间
  char m_ConcretSource; //记录来源485地址，详见485地址解析
  char* m_PicData; //原始JPG图像数据(未经base64编码)
} DEV_RECORD; //记录信息

typedef struct {
  BOOL m_bRTLog; //是否为实时操作日志
  DEV_LOG_TYPE m_LogType; //日志类型
  DEV_CID m_SerialID; //流水号ID
  DEV_CID m_Admin; //操作员
  DEV_CID m_BeOptUser; //被操作员
  DEV_DATETIME m_RecTime; //记录时间
} DEV_LOG;

typedef struct { //变更注册照
  int m_PicBytes; //注册照大小
  BOOL m_bRTChangeImage; //是否为实时变更注册照
  DEV_CID m_UserID; //用户ID
  DEV_CID m_SerialID; //流水号ID
  DEV_CID m_AppendID; //卡号或者其他用于1:1的附加身份确认信息
  DEV_CID m_UserName; //用户名
  DEV_CID m_Department; //部门名称
  DEV_CID m_Admin; //人脸注册管理员,标识此用户的人脸特征是哪个管理员采集
  DEV_DATETIME m_RecTime; //记录时间
  char* m_PicData; //JPG图像数据(未经base64编码)
} DEV_CHANGEIMAGE;

typedef struct {
  int m_Count; //下载多少条
  BOOL m_bOpenRange; //是否为开区间(true=是， false=否)
  DEV_CID m_SID; //从那一条开始，m_SID表示记录精确的流水号 SID组成：SN + TYPE + DATE + ID = ( 6BYTE + 1BYTE + 8BYTE + 4BYTE + \0)
} DEV_DL_RECORD_RANGE; //记录下载区间

typedef struct {
  int m_Count;
  DEV_DL_RECORD_RANGE* m_pRange;
} DEV_DL_RECORD_ARRAY; //记录下载区间集合

typedef struct {
  int m_TGID; //时间组ID
  DEV_DATETIME m_Start; //时间组开始时间
  DEV_DATETIME m_End; //时间组结束时间
  DEV_TIMEGROUP_TYPE m_TGType; //时间组类型
  char m_CheckFlag; //时间检测标记例：m_CheckFlag = CHECK_TIME|CHECK_WEEK
  char m_WeekFlag; //检测星期时，标记那些星期有效。例：m_WeekFlag = WEEK_1|WEEK_5
} DEV_TIMEGROUP; //时间组

typedef struct {
  int m_nCount;
  DEV_TIMEGROUP* m_pTGArray;
} DEV_TIMEGROUP_ARRAY; //时间组数组

typedef struct {
  int m_RightID; //权限ID
  int m_TimeGID[DEV_TIMEGROUP_NUMS]; //时间组ID , (m_TimeGID[0]==ANY_TIME)未指定时间组，开门方式不受时间限制，任意时间段验证成功执行开门方式.
  BOOL m_bHolidyValid; //节假日是否有效
  BOOL m_bActionLock; //电锁输出
  BOOL m_bActionOutPut; //电锁辅助输出
} DEV_RIGHT; //权限

typedef struct {
  int m_nCount;
  DEV_RIGHT* m_pRtArray;
} DEV_RIGHT_ARRAY; //权限数组

typedef struct {
  int m_GroupID; //组ID
  int m_NormalValid; //普通用户中有效用户数
  int m_ForceValid; //强制用户中有效用户数
  DEV_CID m_NormalUsers[DEV_USER_COMBINS]; //普通用户组合，优先级低
  DEV_CID m_ForceUsers[DEV_USER_COMBINS]; //强制用户组合，优先级高
  BOOL m_bGroupOrder; //组合是否有序 1有序，0无序
} DEV_USERGROUP;

typedef struct {
  int m_nCount;
  DEV_USERGROUP* m_pUGArray;
} DEV_USERGROUP_ARRAY; //用户组数组

//程序更新结构体
typedef struct {
  int m_Bytes; //文件大小
  DEV_CID m_FileName; //文件名
  char m_TotalFiles; //需要更新的文件总数
  char m_FileIndex; //当前更新到第几个
  char* m_Buf; //文件数据缓存首地址
} DEV_APPUPDATE;

//用户批量操作
typedef struct {
  int m_nCount;
  DEV_USER* m_pUserArray;
} DEV_BATCH_USER;

//批量记录
typedef struct {
  int m_nCount;
  DEV_RECORD* m_pRecordArray;
} DEV_BATCH_RECORD;

//批量日志
typedef struct {
  int m_nCount;
  DEV_LOG* m_pLogArray;
} DEV_BATCH_LOG;

//批量注册照
typedef struct {
  int m_nCount;
  DEV_CHANGEIMAGE* m_pCImageArray;
} DEV_BATCH_CIMAGE;

//IO设备状态
#define DEV_IO_MODE_NORMAL 0x00 //正常状态
#define DEV_IO_MODE_OPEN 0x01 //常开状态
#define DEV_IO_MODE_CLOSE 0x02 //常关状态
//门磁状态
#define DEV_IO_STATE_CLOSE 0x00 //门磁关
#define DEV_IO_STATE_OPEN 0x01 //门磁开
//执行动作
#define DEV_ACT_IO_OPEN 0x02 //执行打开动作
#define DEV_ACT_IO_OPENEX 0x04 //执行辅助动作
#define DEV_ACT_IO_CLOSE 0x10 //执行关闭动作
#define DEV_ACT_MODE_SET 0x20 //设置IO设备工作模式
#define DEV_ACT_MODE_GET 0x40 //获得IO设备工作模式
#define DEV_ACT_STATE_GET 0x80 //获得IO设备当前状态

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
  int m_CtxID;
  //验证用户数目： 0密码开门，1权限开门，>1组合开门
  int m_Users;
  //控制信息
  DEV_IOCTRL m_XOpen;
  //验证用户数组
  DEV_CID m_IDS[DEV_USER_COMBINS * 2];
} DEV_NOPEN;

typedef struct {
  unsigned short Port; //端口
  char IP_Address[16]; //点分十进制IP地址
} PEERADR;

#define DEV_WEB_LOADUSER 0x0001 //加载用户
#define DEV_WEB_WRITEUSER 0x0002 //写用户
#define DEV_WEB_WRITEREC 0x0004 //写记录
#define DEV_WEB_SYNCTIME 0x0008 //同步时间

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

//其他标记位
#define F_DL_ID CMD_DOWN_ID //下载ID集合
#define F_DL_ID_ALL CMD_ID_ALL //下载所有数据的ID集合
#define F_ID_TIME_RANGE CMD_ID_TIME_RANGE //下载制定时间段ID集合
#define F_ID_RANGE F_ID_TIME_RANGE<<1
#define F_AUTH_VERIFY AUTH_VERIFY //用户身份验证
#define F_DL_PIC DL_IMAGE //需要下载照片
#define F_DL_FEAT DL_FEAT //需要下载人脸特征
#define F_DL_APPENDID DL_APPENDID //远程加在时，m_ID表示卡号
#define F_DL_NORMAL_TG DL_NORMAL_TG //下载正常时间组
#define F_DL_HOLIDAY_TG DL_HOLIDAY_TG //下载节假日
#define F_DOOR_CLOSE DOOR_CLOSE //紧急关门，否则为紧急开门
#define F_DOOR_OPEN DOOR_OPEN //长开或者长关状态取消
#define F_G_ORDER G_ORDER //组合开门有序
#define F_DEFAULT_NO_GROUP DEFAULT_NO_GROUP
#define TIMEGROUP_NUMS TGROUP_NUM //时间组个数
#define USER_COMBINS MAX_GROUPUSER //用户组合个数

#define USER_JPEG_SIZE (64*1024)
#define REC_JPEG_SIZE (18*1024)
#define FEAT_SIZE (90*1024)
#define OFFSET_SIZE 20
//#define TRANS_SERVER


#ifndef TRANS_SERVER
#define CMD_S_CONNECT_CUT DEV_CONNECT_CUT //断开连接
#define CMD_S_CONNECT_SUCCESS DEV_CONNECT_SUCCESS //连接成功
#define CMD_S_CONNECT_FAILUE DEV_CONNECT_FAILUE //连接失败
#define CMD_S_CONNECT_NONE DEV_CONNECT_NONE //未连接
#define CMD_S_CONNECT_REQ DEV_CONNECT_NONE+1 //请求连接 
#define CMD_S_CONNECT_REQ_CUT CMD_S_CONNECT_REQ+1 //请求断开连接
#define E_CONNECT_STATUS DEV_CONNECT_STATUS
#else
#define CMD_S_CONNECT_CUT NET_CONNECT_CUT //断开连接
#define CMD_S_CONNECT_SUCCESS NET_CONNECT_SUCCESS //连接成功
#define CMD_S_CONNECT_FAILUE NET_CONNECT_FAILUE //连接失败
#define CMD_S_CONNECT_NONE NET_CONNECT_NONE //未连接
#define CMD_S_CONNECT_REQ NET_CONNECT_NONE+1 //请求连接 
#define CMD_S_CONNECT_REQ_CUT CMD_S_CONNECT_REQ+1 //请求断开连接
#define E_CONNECT_STATUS NET_CONNECT_STATUS
#endif

#ifndef TRANS_SERVER

//客户端身份验证
typedef struct {
  DEV_CID m_UserName; //用户名
  DEV_CID m_Password; //密码
} DEV_AUTH;

typedef enum {
  DL_SINGLE = 0x01,
  DL_ALL,
  DL_SEGTIME,
  DL_RANGE
} DATA_DL_TYPE;

//获得指定时间段的ID集合
typedef struct {
  DEV_DATE m_Start; //记录起始时间
  DEV_DATE m_End; //记录结束时间
} DEV_TIMESEG;

#endif



typedef char int8;
typedef INT32 int32;
//typedef unsigned int uint32;
typedef unsigned short uint16;
#define MAX_SEND_COUNTS 5


#define M_AUTH 0x07 //客户端身份验证
#define M_HEART 0x09 //心跳包
#define M_VIDEO_OUT 0x23 //UDP网络视频传输 NET_ADDRESS

//动作子命令
#define S_SET 0x81 //启动视频
#define S_GET 0x83 //关闭视频

#define F_REQ_FLAG 0x40000000 //请勿占用此位
#define F_REPLY_FLAG 0x20000000 //请勿占用此位

//用户验证
#define AUTH_VERIFY 0x00000100 //用户身份验证

#define PACK_SIZE (60*1024) //一个包为60大小

#define ALIGN(bytes, alignments) (((bytes)+((alignments)-1))&~((alignments)-1))


typedef struct pmdev_t pmdev_t;

typedef int (*frame_fun_t)(pmdev_t* vp, int h, int w, const unsigned char* img, int step, int cn);
typedef int (*event_fun_t)(pmdev_t* vp, const char* sDevIP, const char* sClientIP, int eType, int eFlag, void* pvContent, int nSerialNo, int nResult, void* pvClientData);

typedef struct pmdev_t {
  SOCKET m_sHost;//TCP套接字,和设备建立连接，对设备端的实时控制
  SOCKET m_sVVIDEOSck;//UDP套接字，接收设备发送过来的实时视频数据

  int m_nIndex;//命令包序号
  int m_Author;//登陆设备权限验证通过标志
  int m_VideoClose;//关闭视频标志
  int m_exitThread;
  thread_t m_hTCPThread[1];//接收实时控制响应数据的线程
  thread_t m_hUDPThread[1];//接收实时视频数据的线程
  frame_fun_t m_framefun;
  event_fun_t m_eventfun;
  void* userdata;
  //int m_nTCPPort;//TCP端口 13333
  int m_nUDPPort;//UDP端口,随机选择的可用端口
  char m_strDevIP[32];//设备IP
  //char m_strClientIP[32];//本机IP
  int cn;
  int flipud;
  int run;
} pmdev_t;

#define TCPPORT 13333
#define ERR_VERIFY_FAIL 0x06 //验证失败
#define SUC_EXECUT 0x00 //成功
#define GET_ERR( state ) ( (state) & 0xff )

//包头固定程度为20
#define OFFSET_SIZE 20

#ifdef WIN32
#define SOCK_LENTH int
#define T_VOID void
#define T_HANDLE HANDLE
#define SLEEP(Milliseconds) {Sleep(Milliseconds);}
#define LASTERROR WSAGetLastError()
#define MILLISEC 1
#define MEMRESERVER_BLOCKS 10 //内存块大小
#else
#define SOCKET int
#define SOCK_LENTH socklen_t
#define T_HANDLE pthread_t
#define T_VOID void*
#define INVALID_SOCKET 0
#define SOCKET_ERROR -1
#define closesocket close
#define ioctlsocket ioctl
#define SLEEP(Milliseconds) {struct timeval tval;tval.tv_sec= 0;tval.tv_usec=(Milliseconds); select( 0 , NULL , NULL , NULL , &tval ); }
#define LASTERROR errno
#define WSAECONNRESET ECONNRESET
#define WSAECONNABORTED ECONNABORTED
#define WSAEWOULDBLOCK EWOULDBLOCK
#define WSAEINTR EINTR
#define MILLISEC 1000
#define SD_BOTH SHUT_RDWR
#define MEMRESERVER_BLOCKS 2 //内存块大小
#define INFINITE 0xFFFFFFFF

#endif

#define TRACE printf
#ifndef ASSERT
#define ASSERT assert
#endif // ASSERT


#define OFF_SET(T, member) ((int)( &(((T*)0)->member)))

#include "Base64.inl"

void CopyDevUser2DspUser(USER* dspUser , const DEV_USER* devUser)
{
  ZeroMemory(dspUser , sizeof(USER));
  memcpy(dspUser, devUser, OFF_SET(USER, m_UserType));
  dspUser->m_UserType = devUser->m_UserType;

  if (devUser->m_FeatLen) {
    if ((devUser->m_UserFlag & DEV_USER_FEAT_BASE64) && devUser->m_FeatData[3]) {
      dspUser->m_FeatBytes = deBase64(devUser->m_FeatData , devUser->m_FeatLen , dspUser->m_Block);
    }
    else {
      memcpy(dspUser->m_Block, devUser->m_FeatData, devUser->m_FeatLen);
    }
  }

  if (devUser->m_PicLen) {
    if (devUser->m_UserFlag & DEV_USER_IMAGE_BASE64) {
      dspUser->m_PicBytes = deBase64(devUser->m_PicData , devUser->m_PicLen , dspUser->m_Block + dspUser->m_FeatBytes);
    }
    else {
      memcpy(dspUser->m_Block + dspUser->m_FeatBytes , devUser->m_PicData, devUser->m_PicLen);
    }
  }
}

static int dev_find_user(const DEV_BATCH_USER* users, const char* userid)
{
  int i;

  for (i = 0; i < users->m_nCount; ++i) {
    if (0 == strncmp(users->m_pUserArray[i].m_UserID.m_ID, userid, DEV_ID_LEN - 1)) {
      return i;
    }
  }

  return -1;
}
static int dev_find_record(const DEV_BATCH_RECORD* records, const char* serialid)
{
  int i;

  for (i = 0; i < records->m_nCount; ++i) {
    if (0 == strncmp(records->m_pRecordArray[i].m_SerialID.m_ID, serialid, DEV_ID_LEN - 1)) {
      return i;
    }
  }

  return -1;
}
static int dev_insert_user(DEV_BATCH_USER* users, const DEV_USER* newuser, int issort, int newsize)
{
  const char* userid = newuser->m_UserID.m_ID;
  int k = dev_find_user(users, userid);

  if (k < 0) {
    int i = users->m_nCount;

    if (issort) {
      for (i = 0; i < users->m_nCount; ++i) {
        int t = strncmp(users->m_pUserArray[i].m_UserID.m_ID, userid, DEV_ID_LEN - 1);

        if (t >= 0) {
          break;
        }
      }
    }

    users->m_nCount++;

    if (newsize) {
      REALLOC(DEV_USER, users->m_pUserArray, newsize);
    }

    MEMINST(users->m_pUserArray, users->m_nCount - 1, i, newuser, 1);
    users->m_pUserArray[i].m_FeatData = (char*)memdup(newuser->m_FeatData, newuser->m_FeatLen);
    users->m_pUserArray[i].m_PicData = (char*)memdup(newuser->m_PicData, newuser->m_PicLen);
  }

  return 0;
}


static int dev_insert_users(DEV_BATCH_USER* users, const DEV_USER* newuser, int n, int issort)
{
  int i;
  int newcount = users->m_nCount + n;

  for (i = 0; i < n; ++i) {
    dev_insert_user(users, newuser + i, issort, i ? 0 : newcount);
  }

  return 0;
}

static int dev_insert_record(DEV_BATCH_RECORD* records, const DEV_RECORD* newrecord, int issort, int newsize)
{
  const char* serialid = newrecord->m_SerialID.m_ID;
  int k = dev_find_record(records, serialid);

  if (k < 0) {
    int i = records->m_nCount;

    if (issort) {
      for (i = 0; i < records->m_nCount; ++i) {
        int t = strncmp(records->m_pRecordArray[i].m_SerialID.m_ID, serialid, DEV_ID_LEN - 1);

        if (t >= 0) {
          break;
        }
      }
    }

    records->m_nCount++;

    if (newsize) {
      REALLOC(DEV_RECORD, records->m_pRecordArray, newsize);
    }

    MEMINST(records->m_pRecordArray, records->m_nCount - 1, i, newrecord, 1);
    records->m_pRecordArray[i].m_PicData = (char*)memdup(newrecord->m_PicData, newrecord->m_PicLen);
  }

  return 0;
}


static int dev_insert_records(DEV_BATCH_RECORD* records, const DEV_RECORD* newrecord, int n, int issort)
{
  int i;
  int newcount = records->m_nCount + n;

  for (i = 0; i < n; ++i) {
    dev_insert_record(records, newrecord + i, issort, i ? 0 : newcount);
  }

  return 0;
}

//校验和计算
UINT16 sock_checksum(UINT16* buffer, UINT32 size)
{
  UINT32 cksum = 0;

  while (size > 1) {
    int z = *buffer;
    cksum += *buffer++;
    size -= sizeof(UINT16);
    //printf("cksum = %d z = %d\n", cksum, z);
  }

  if (size) {
    cksum += *(UINT8*)buffer;
  }

  cksum = (cksum >> 16) + (cksum & 0xffff);
  cksum += (cksum >> 16);
  return (UINT16)(~cksum);
}

int write_time_out(SOCKET sock, int time)
{
  fd_set fwds;
  struct timeval wtval;
  FD_ZERO(&fwds);
  FD_SET(sock, &fwds);
  wtval.tv_sec = 0;
  wtval.tv_usec = (time * 1000);
  return select(sock + 1, NULL, &fwds, NULL, &wtval) <= 0;
}


static int mylogprintf(const char* fmt, ...)
{
  FILE* pf;
  pf = fopen("log.txt", "a");

  if (pf) {
    va_list ARGLIST;
    va_start(ARGLIST, fmt);
    vfprintf(pf, fmt, ARGLIST);
    fflush(pf);
    va_end(ARGLIST);
    fclose(pf);
  }

  return 0;
}
int safe_send(SOCKET sock, const char* data, int len)
{
  int nTotalSends = len;
  int bOK = 0;
  int send_counts = 0;
#if 1
  {
    int i;
    mylogprintf("send %d:", len);

    for (i = 0; i < len; ++i) {
      //mylogprintf("%02x ", ((unsigned char*)data)[i]);
      mylogprintf("%d ", ((char*)data)[i]);
    }

    mylogprintf("\n");
  }
#endif

  while (send_counts++ < MAX_SEND_COUNTS) {
    int nHasSend, bTimeOut = write_time_out(sock , 300);

    if (bTimeOut) {
      continue;
    }

    nHasSend = send(sock, data, nTotalSends, 0);

    if (nHasSend <= 0) {
      int32 nError = LASTERROR;
      TRACE("3---nError = %d , len = %d nHasSend = %d\r\n" , nError , len , nHasSend);

      if (WSAEINTR == nError || WSAEWOULDBLOCK == nError) {
        continue;
      }
      else {
        break;
      }
    }

    data += nHasSend;
    nTotalSends -= nHasSend;

    if (nTotalSends <= 0) {
      bOK = 1;
      break;
    }
  }

  return bOK;
}

#ifndef MIN
#define MIN(a, b) ((a)<(b) ? (a) : (b))
#endif // MIN

#ifndef MAX
#define MAX(a, b) ((a)>(b) ? (a) : (b))
#endif // MIN

unsigned long _inet_addr(const char* ip)
{
  if (NULL == ip) {
    return 0;
  }
  else {
    unsigned int a = 0;
    unsigned int b = 0;
    unsigned int c = 0;
    unsigned int d = 0;
    unsigned int ipInt = 0;
    BYTE* pInt = (BYTE*)(&ipInt);
    sscanf(ip, "%d.%d.%d.%d", &a, &b, &c, &d);
    pInt[0] = (BYTE)a;
    pInt[1] = (BYTE)b;
    pInt[2] = (BYTE)c;
    pInt[3] = (BYTE)d;
    return ipInt;
  }
}


int send_cmd_packet(pmdev_t* pv, SOCKET sock, UINT32 m_MCmd, UINT32 m_SCmd, UINT32 m_CFlag, const void* data0, int len)
{
#define OFFSET_SIZE 20
  char pBuffer[PACK_SIZE + OFFSET_SIZE] = { 0 };//最大60K大小一个包
  int nTotalSend = 0;
  int bOK = 1;
  int i = 0;
  int nSeq = ++pv->m_nIndex;
  const char* data = (const char*)data0;
  NCMD* pCmd = (NCMD*)(pBuffer + OFFSET_SIZE);

  if (!data0) {
    len = 0;
  }

  pCmd->m_MCmd = m_MCmd;
  pCmd->m_SCmd = m_SCmd;
  pCmd->m_CFlag = m_CFlag;
  pCmd->m_Param1 = -1;
  pCmd->m_Param2 = _inet_addr(pv->m_strDevIP);
  len += sizeof(NCMD);

  for (i = 0; i < len;) {
    int nSend, maxpacklen = 0;
    //组装网络命令包 加包头
    SOCK_DATA_HEADER* hdr = (SOCK_DATA_HEADER*)(pBuffer + i);
    hdr->msg_identify = nSeq; //标识
    hdr->msg_offset = i * PACK_SIZE; //偏移
    hdr->msg_checksum = 0;  //检验和
    hdr->msg_total = len;  //数据总长度
    hdr->msg_len = MIN(PACK_SIZE, (len - i)); //数据长度
    hdr->msg_len = ALIGN(hdr->msg_len, 4);
    hdr->msg_total = ALIGN(hdr->msg_total, 4);
    nSend = ALIGN(hdr->msg_len, 4) + OFFSET_SIZE;

    if (0 == i) {
      memcpy(pCmd + 1, data, hdr->msg_len - sizeof(NCMD));
    }
    else {
      memcpy(hdr + 1, data + i - sizeof(NCMD), hdr->msg_len);
    }

    hdr->msg_checksum = sock_checksum((uint16*)hdr, sizeof(SOCK_DATA_HEADER));

    //printf("send %d\n", nSend);
    if (!safe_send(sock, (const char*)hdr, nSend)) {
      bOK = 0;
      nTotalSend = SOCKET_ERROR;
      break;
    }

    nTotalSend += hdr->msg_len;
    i += hdr->msg_len;
  }

  if (SOCKET_ERROR == nTotalSend) {
    printf("send Heart CMD failed!\n");
    closesocket(pv->m_sHost); //关闭套接字
    pv->m_sHost = 0;
    pv->m_eventfun(pv, pv->m_strDevIP, NULL, DEV_CONNECT_CUT, 0, 0, 0, 0, pv->userdata);
  }

  return nTotalSend;
}

//解析设备控制命令命令头
int ResolveNCMD(const char* pBuffer, const int nLen , int* nRlt)
{
  NCMD* pCMD = (NCMD*)(pBuffer + OFFSET_SIZE);
  *nRlt = GET_ERR(pCMD->m_CFlag);
  return pCMD->m_MCmd;
}

typedef struct databuf_t {
  UINT32 msg_identify; //标识
  UINT32 msg_len; //数据长度
  UINT32 msg_total; //数据总长度
  char* buf;
} databuf_t;

int myrecv(SOCKET s, void* buf, int buflen, int istcp, int time, int isudp, struct sockaddr *from, int * fromlen)
{
  int ret = 0, i = 0;
  char* cbuf = (char*)buf;

  for (i = 0; i < buflen;) {
    if (0 != time) {
      FD_SET fd = { 1, s };
      struct timeval timeout;
      timeout.tv_sec = time / 1000, timeout.tv_usec = 1000 * (time % 1000);

      if (select(0, &fd, NULL, NULL, &timeout) == 0) {
        return -2;
      }
    }

    if (isudp) {
      ret = recvfrom(s, cbuf + i, buflen - i, 0, (struct sockaddr*)from, fromlen);
    } else {
      ret = recv(s, cbuf + i, buflen - i, 0);
    }

    if (ret <= 0) {
      return ret;
    }

    i += ret;
  }

  return i;
}

databuf_t* recv_pack_data(SOCKET s, databuf_t* databuf, int bufnum, int istcp, int* err, int isudp, struct sockaddr *from, int * fromlen)
{
  databuf_t* it = NULL;
  int i, len = 0, checksum;
  //获取包头
  SOCK_DATA_HEADER Head[1] = {0};
  //收包头
  len = myrecv(s, Head, sizeof(SOCK_DATA_HEADER), istcp, 0, isudp, from, fromlen);

#if 1
  {
    int i;

    for (i = 0; i < len; ++i) {
      //mylogprintf("%02x ", ((unsigned char*)data)[i]);
      printf("%d ", ((char*)(Head))[i]);
    }

    printf("\n");
  }
#endif

  //len = recvfrom(pv->m_sVVIDEOSck, pTemp, pack_size, 0, (struct sockaddr*)&addrClient, &len);
  if (len < 0) {
    *err = len;
    return NULL;
  }

#if 0
  {
    int i;
    unsigned char aa[20] = {117, 9, 0, 0, 0, 0, 0, 0, 20, 0, 0, 0, 20, 0, 0, 0, 98, -10, 0, 0};
    checksum = sock_checksum((UINT16*)(aa), 20);
    mylogprintf("send %d:", len);

    for (i = 0; i < len; ++i) {
      //mylogprintf("%02x ", ((unsigned char*)data)[i]);
      mylogprintf("%d ", ((char*)Head)[i]);
    }

    mylogprintf("\n");
  }
#endif
  //printf("len = %d\n", len);
  //校验
  checksum = sock_checksum((UINT16*)(Head), sizeof(SOCK_DATA_HEADER));

  if (0 != checksum) {
    *err = -3;
    return NULL; //校验出错，丢弃
  }

  //查找本次标识的数据接收缓冲
  for (i = 0; i < bufnum; ++i) {
    if (databuf[i].msg_identify == Head->msg_identify) {
      it = databuf + i;
      printf("it\n");
      break;
    }
  }

  if (NULL == it) { //没找到，则为新视频数据包。
    if (Head->msg_offset != 0) { //检验是否丢包头，否则为无命令头的非法视频数据。
      return NULL;
    }

    it = databuf;

    for (i = 1; i < bufnum; ++i) {
      if (it->msg_identify > databuf[i].msg_identify) {
        it = databuf + i;
      }
    }

    //为接收本次视频数据开辟一个快缓存，并插入到MAP映射表中
    it->buf = (char*)realloc(it->buf, Head->msg_total);
    ZeroMemory(it->buf, Head->msg_total);
    it->msg_identify = Head->msg_identify;
    it->msg_len = 0;
    it->msg_total = Head->msg_total;
  }

  assert((long)it->buf % 4 == 0);//验证缓存是否4字节对齐

  //收数据
  len = myrecv(s, ((char*)it->buf) + Head->msg_offset, Head->msg_len, istcp, 100, isudp, from, fromlen);

  printf("len = %d %d\n", len, Head->msg_len);
  *err = len;

  if (len > 0) {
    it->msg_len += Head->msg_len;
  }
  else {
    *err = len;
  }

  if (it->msg_len == Head->msg_total) {
    //printf("msg_total = %d\n", it->msg_total);

    if (it->msg_len > 2 * PACK_SIZE) {
      int adsf = 0;
    }
  }

  return it->msg_len == Head->msg_total ? it : NULL;
}

//接收视频数据线程
static DWORD WINAPI RecvVideoDataThread(pmdev_t* pv)
{
  int cn = pv->cn;
  int nLen = 0;
  int nCmd = 0;
  int nRlt = 0;
  unsigned char* pDataBuf = NULL;
  unsigned char* pVideoBuf = NULL;
  unsigned char* pRGBTMP;
  unsigned char* pRGBTRANS;

  SOCKADDR_IN addrClient = {0};
  int len = sizeof(SOCKADDR);
  char* pData = NULL;
  DEVIMAGE* pDevImage = NULL;

  enum { maxbufnum = 32, pack_size = sizeof(SOCK_DATA_HEADER) + PACK_SIZE};
  int h = 0, w = 0;
  uchar pTemp[pack_size];
  int bufsz = 0;
  int msg_identify = -1, msg_total = 0, msg_off=0;
  
  while (!pv->m_VideoClose) {
    //接收视频数据
    nLen = recvfrom(pv->m_sVVIDEOSck, (char*)pTemp, pack_size, 0, (struct sockaddr*)&addrClient, &len);

    if (nLen < 0) {
      printf("recvfrom 错误 %d\n", nLen);
      msg_off = 0;
      msg_identify = -1;
      continue;
      //pv->m_eventfun(pv, pv->m_strDevIP, NULL, DEV_CONNECT_CUT, 0, 0, 0, 0, pv->userdata);
    }

    //累积当前包的已接收的分包总长度
    //判断组包是否完成，完成则播放
    if (nLen>0) {
      SOCK_DATA_HEADER* pHead = NULL;
      pHead = (SOCK_DATA_HEADER*)(pTemp);
      if ( 0 != sock_checksum( (UINT16*)pHead, sizeof(SOCK_DATA_HEADER) ) ) {
        printf("校验出错\n");
				continue;//校验出错,丢弃
      }
      printf("msg_identify=%d msg_offset=%d msg_len=%d bufsz=%d\n", pHead->msg_identify, pHead->msg_offset, pHead->msg_len, bufsz);

      //解析网络的数据包
      if (msg_identify!=(int)pHead->msg_identify) {
        if (pHead->msg_offset == 0) {//第一个分包
          NCMD* pCMD = (NCMD*)(pTemp+OFFSET_SIZE);
          nRlt       = GET_ERR(pCMD->m_CFlag);
          if ( pCMD->m_MCmd != M_VIDEO_OUT ) {
            continue; //丢弃，非法包
          }
          if (bufsz < (int)(pHead->msg_total+sizeof(SOCK_DATA_HEADER))) {
            bufsz = pHead->msg_total+sizeof(SOCK_DATA_HEADER);
            pDataBuf = (unsigned char*)realloc(pDataBuf, bufsz);
          }
          msg_identify = pHead->msg_identify;
        } else {
          printf("不是第一个分包\n");
          continue;
        }
      }
      memcpy(pDataBuf + pHead->msg_offset, pTemp+sizeof(SOCK_DATA_HEADER), pHead->msg_len);
      msg_total += pHead->msg_len;
      if (msg_total!=(int)pHead->msg_total) {
        continue;
      }
      msg_total = 0;

      pDevImage = (DEVIMAGE*)(pDataBuf + sizeof(NCMD));

      if (pDevImage->m_W != w || pDevImage->m_H != h) {
        pVideoBuf = (unsigned char*)realloc(pVideoBuf, pDevImage->m_H * pDevImage->m_W * cn * 2);
        pRGBTMP = pVideoBuf;
        pRGBTRANS = pVideoBuf + pDevImage->m_H * pDevImage->m_W * cn;
      }

#if 1

      if (IMAGE_UYVY == pDevImage->m_Type) {
        w = pDevImage->m_H;
        h = pDevImage->m_W;
        YUV422_C_RGB(pDevImage->m_H, pDevImage->m_W, (unsigned char*)pDevImage->m_Block, pDevImage->m_W * 2, pRGBTMP, pDevImage->m_W * cn, cn);
        RGB_EDDY(pRGBTMP, pDevImage->m_H, pDevImage->m_W, pRGBTRANS, cn, pv->flipud);
      }
      else if (IMAGE_YUYV == pDevImage->m_Type) {
        w = pDevImage->m_W;
        h = pDevImage->m_H;
        YUYV_C_RGB((unsigned char*)pDevImage->m_Block, pRGBTRANS, pDevImage->m_H, pDevImage->m_W, cn);
      }

#endif

      //显示视频
      if (pv->m_framefun) {
        pv->m_framefun(pv, h, w, pRGBTRANS, w * cn, cn);
      }
      else {
        printf("%d %d %d\n", pv->m_nIndex, h, w);
      }
      msg_off = 0;
    }

    sleep(1);
  }
  
  if (pTemp) {
    free(pDataBuf);
    pDataBuf = NULL;
  }
  if (pVideoBuf) {
    free(pVideoBuf);
    pVideoBuf = 0;
    pRGBTMP = 0;
    pRGBTRANS = 0;
  }

  pv->m_VideoClose = 0;
  return 0;
}

#ifndef WIN32
int Sleep(int ms)
{
  return sleep(ms);
}
#endif
//功 能 设备验证或设置验证
//参 数 cszDevAddr设备地址
//cName 用户名
//cPws 密码
//nFlag 1=验证 2=设置
//说 明
BOOL CPM_ULDevAuth(pmdev_t* pv, const char* sLoginName, const char* sPassword, int nFlag)
{
  DEV_AUTH pAuth[1] = {0};
  //组命令数据
  strncpy(pAuth->m_UserName.m_ID, sLoginName, DEV_ID_LEN);
  strncpy(pAuth->m_Password.m_ID, sPassword, DEV_ID_LEN);
  return send_cmd_packet(pv, pv->m_sHost, M_AUTH, S_SET, ((nFlag == 2) ? 0 : F_AUTH_VERIFY), pAuth, sizeof(DEV_AUTH));
}
//功 能 设置设备所在区域地址
//参 数
//cszDevAddr 设备地址
//cszRgnAddr 设备区域地址
//说 明 当设备建立连接后可将设备设置到某一个区域，以后及可对这个区域进行搜索设备
BOOL CPM_ULDevRegionAddr(pmdev_t* pv, const char* cszGroupIP)
{
  CID cid[1] = {0};
  int slen = strlen(cszGroupIP);

  if (slen < ID_LEN) {
    memcpy(cid->m_ID, cszGroupIP, slen);
  }

  return send_cmd_packet(pv, pv->m_sHost, M_MCAST, S_SET, 0, cid, sizeof(CID));
}
BOOL devcmd_GetDevRgn(pmdev_t* pv)
{
  return send_cmd_packet(pv, pv->m_sHost, M_MCAST, S_GET, 0, 0, 0);
}
//功 能 设置设备系统时间
//参 数
//cszDevAddr 设备地址
//cSysTime 时间信息
//说 明 DevOperResultNotify回调参数对应类型：
//eType = DEV_SYS_TIME_OPER
//eFlag = DEV_OPER_UPLOAD ，
//pvContent = NULL
BOOL CPM_ULDevTime(pmdev_t* pv, const DEV_DATETIME* cTime)
{
  return send_cmd_packet(pv, pv->m_sHost, M_TIME, S_SET, 0, cTime, sizeof(DATETIME));
}
//功 能 获取设备系统时间
//参 数 cszDevAddr 设备地址
//说 明 DevOperResultNotify回调参数对应类型：
//eType = DEV_SYS_TIME_OPER
//eFlag = DEV_OPER_DOWNLOAD，
//pvContent = DEV_DATETIME*
BOOL CPM_DLDevTime(pmdev_t* pv)
{
  return send_cmd_packet(pv, pv->m_sHost, M_TIME, S_GET, 0, 0, 0);
}
//功 能 获取设备基本信息
//参 数 cszDevAddr 设备地址
//说 明
BOOL CPM_DLDevBaseInfo(pmdev_t* pv)
{
  return send_cmd_packet(pv, pv->m_sHost, M_DEVINFO, S_GET, 0, 0, 0);
}
//功 能 获取设备统计信息
//参 数 cszDevAddr 设备地址
//说 明
BOOL CPM_DLDevStatisInfo(pmdev_t* pv)
{
  return send_cmd_packet(pv, pv->m_sHost, M_STATIS, S_GET, 0, 0, 0);
}
//功 能 获取人脸机工作属性
//参 数
//cszDevAddr 人脸机地址
//nSeq 操作序列号
//说 明 工作属性DEV_WORKATT通过回调函数给出。
BOOL CPM_DLDevWorkAttInfo(pmdev_t* pv)
{
  return send_cmd_packet(pv, pv->m_sHost, M_DEVSET, S_GET, 0, 0, 0);
}
//功 能 设置设备工作属性信息
//参 数
//cszDevAddr 设备地址
//cWorkAtt 工作属性结构体
//nSeq 操作序列号
BOOL CPM_ULDevWorkAttInfo(pmdev_t* pv, const DEV_WORKATT* cAtt)
{
  DEVSET pSet[1] = {0};
  memcpy(pSet->m_DevCtx, cAtt->m_TimeGID, sizeof(cAtt->m_TimeGID));
  pSet->m_DefaultRight = cAtt->m_DefaultRight;
  pSet->m_BaseSet = cAtt->m_BaseSet;
  memcpy(pSet->m_SuperPassword.m_ID, cAtt->m_szSuperPWD.m_ID, sizeof(CID));
  pSet->m_DoorMangetTime = cAtt->m_DoorMangetTime;
  pSet->m_LockTime = cAtt->m_LockTime;
  pSet->m_VerifyMode = cAtt->m_VerifyMode;
  pSet->m_OutputWG = cAtt->m_bWGOutPut ? OUTPUT_WG_YES : 0x00;
  pSet->m_OutputWG |= cAtt->m_nWGType ? OUTPUT_WG_34 : 0x00;
  pSet->m_OutputWG |= cAtt->m_nWGOutType ? OUTPUT_WG_CTX : 0x00;
  pSet->m_OutputWG |= cAtt->m_nWGOutIDType ? OUTPUT_WG_UID : 0x00;
  pSet->m_OutputCtx = cAtt->m_nWGOutContent;
  return send_cmd_packet(pv, pv->m_sHost, M_DEVSET, S_SET, 0, pSet, sizeof(DEVSET));
}
//功 能 上传指定用户
//参 数
//cszDevAddr 设备地址
//cUser 用户信息结构体
//nSeq 操作序列号
//说 明
//用户无人脸库时，DEV_USER中的 m_FeatLen = 0， m_FeatData = NULL
//用户无注册照时，DEV_USER中的 m_PicLen= 0， m_PicData = NULL
//DevOperResultNotify回调参数对应类型：eType = DEV_USER_OPER
//eFlag = DEV_OPER_UPLOAD，pvContent= NULL
BOOL CPM_ULUser(pmdev_t* pv, const DEV_USER* cUser)
{
  BOOL bAddFlag = TRUE;
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(USER) + FEAT_SIZE + USER_JPEG_SIZE, 4) >> 2] = {0};
  USER* pUser = (USER*)nData;
  ASSERT((cUser->m_FeatLen <= FEAT_SIZE) && (cUser->m_PicLen <= USER_JPEG_SIZE));

  if (!((cUser->m_FeatLen <= FEAT_SIZE) && (cUser->m_PicLen <= USER_JPEG_SIZE))) {
    return FALSE;
  }

  CopyDevUser2DspUser(pUser , cUser);
  return send_cmd_packet(pv, pv->m_sHost, M_USER, bAddFlag ? S_SET : S_REALTIME_CAP, 0, pUser, USER_STEP(*pUser));
}
//功 能 上传实时加载用户
//参 数
//cszDevAddr 设备地址
//cUser 用户信息结构体
//cRtLoad 实时加载信息
//nSeq 操作序列号
//说 明 设备端请求客户端上传某用户时，通过此接口上传所请求用户信息，
//cRtLoad为设备端请求时传过来的信息，客户端禁止修改。此函数实现远程用户加载功能。
BOOL CPM_ULRealTimeUser(pmdev_t* pv, const DEV_USER* cUser, const DEV_USER_REAL_LOAD* cLoad)
{
  int m_CFlag = 0;
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(USER) + FEAT_SIZE + USER_JPEG_SIZE, 4) >> 2] = {0};
  USER* pUser = (USER*)nData;
  ASSERT((cUser->m_FeatLen <= FEAT_SIZE) && (cUser->m_PicLen <= USER_JPEG_SIZE));

  if (!((cUser->m_FeatLen <= FEAT_SIZE) && (cUser->m_PicLen <= USER_JPEG_SIZE))) {
    return FALSE;
  }

  if (cLoad->m_bApeendID) {
    m_CFlag |= F_DL_APPENDID;
  }

  if (cLoad->m_LoadFlag & DL_DEV_PIC) {
    m_CFlag |= F_DL_PIC;
  }

  if (cLoad->m_LoadFlag & DL_DEV_USER_FEAT) {
    m_CFlag |= F_DL_FEAT;
  }

  //pCMD->m_Param1 = cLoad.m_ReqSource;
  CopyDevUser2DspUser(pUser , cUser);
  return send_cmd_packet(pv, pv->m_sHost, M_USER, S_REALTIME_LOAD, m_CFlag, pUser, USER_STEP(*pUser));
}
//功 能 远程实时采集用户特征
//参 数
//cszDevAddr 设备地址
//cUser 用户信息
//说 明 管理端录入用户资料时，可通过该函数将用户资料发送的人脸机，控制人脸机采集该用户的人脸。
//远程用户采集功能，使得直接在管理端就可以采集用户，在配合远程视频的情况下采集的可视化程度和直接在机器上采集一样。
//如果使能了(用户采集实时同步)。远程采集的用户也会实时的发回管理端.
BOOL CPM_ULRealTimeUserCap(pmdev_t* pv, const DEV_USER* cUser)
{
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(USER), 4) >> 2] = {0};
  USER* pUser = (USER*)nData;
  ASSERT((cUser->m_FeatLen <= FEAT_SIZE) && (cUser->m_PicLen <= USER_JPEG_SIZE));

  if (!((cUser->m_FeatLen <= FEAT_SIZE) && (cUser->m_PicLen <= USER_JPEG_SIZE))) {
    return FALSE;
  }

  CopyDevUser2DspUser(pUser , cUser);
  return send_cmd_packet(pv, pv->m_sHost, M_USER, S_REALTIME_CAP, 0, pUser, USER_STEP(*pUser));
}
//功 能 删除指定用户
//参 数
//cszDevAddr 设备地址
//cID 用户编号
//nSeq 操作序列号
//说 明 删除成功与否，通过操作回调函数给出结果
//DevOperResultNotify回调参数对应类型：
//eType = DEV_USER_OPER
//eFlag = DEV_OPER_DELETE，
//pvContent = NULL
BOOL CPM_DELUser(pmdev_t* pv, const DEV_CID* cID)
{
  return send_cmd_packet(pv, pv->m_sHost, M_USER, S_DEL, 0, cID, sizeof(CID));
}
//功 能 下载指定用户
//参 数
//cszDevAddr 设备地址
//cID 用户ID
//nFlag DL_DEV_USER_PIC 需下载用户照片
// DL_DEV_USER_FEAT 需要下载人脸特征
// DL_DEV_USER_PIC|DL_DEV_USER_FEAT=两者
// 为0默认下载文字信息
//nSeq 操作序列号
//说 明 DevOperResultNotify回调参数对应类型：
//eType = DEV_USER_OPER
//eFlag = DEV_OPER_DOWNLOAD，
//pvContent = DEV_USER*
BOOL CPM_DLSingleUser(pmdev_t* pv, const DEV_CID* cID, int nFlag)
{
  int m_CFlag = 0;

  if (nFlag & DL_DEV_PIC) {
    m_CFlag |= F_DL_PIC;
  }

  if (nFlag & DL_DEV_USER_FEAT) {
    m_CFlag |= F_DL_FEAT;
  }

  return send_cmd_packet(pv, pv->m_sHost, M_USER, S_GET, 0, cID, sizeof(CID));
}
//功 能 下载所有用户
//参 数 cszDevAddr 设备地址
//说 明
BOOL CPM_DLAllUser(pmdev_t* pv)
{
  return send_cmd_packet(pv, pv->m_sHost, M_USER, S_GET, F_DL_ID | F_DL_ID_ALL, 0, 0);
}
//功 能 下载某一时间段内的注册用户
//参 数
//cszDevAddr 设备地址
//cStart 起始日期
//cEnd 终止日期
//说 明
BOOL CPM_DLSegTimeUser(pmdev_t* pv, const DEV_DATE* cStart, const DEV_DATE* cEnd)
{
  DEV_TIMESEG seg[1] = {0};
  seg->m_Start = *cStart;
  seg->m_End = *cEnd;
  return send_cmd_packet(pv, pv->m_sHost, M_USER, S_GET, F_DL_ID | F_DL_ID_ALL, seg, sizeof(TIMESE));
}
//功 能 下载识别记录区间
//参 数
//cszDevAddr 设备地址
//nSeq 操作序列
//说 明 DevOperResultNotify回调参数对应类型：
//eType = DEV_RECORD_OPER
//eFlag = DEV_OPER_DOWNLOAD
//pvContent = DEV_DL_RECORD_ARRAY*
//参考DEV_DL_RECORD_ARRAY结构，一天为一个区间，该函数返回识别记录的所有区间。
BOOL CPM_DLAllIdentifyRecord(pmdev_t* pv)
{
  return send_cmd_packet(pv, pv->m_sHost, M_VERIFY, S_GET, F_DL_ID | F_DL_ID_ALL, 0, 0);
}
//功 能 下载指定时间段识别记录
//参 数 cszDevAddr 设备地址
//cStart 起始日期
//cEnd 终止日期
//说 明
BOOL CPM_DLSegTimeIdentifyRecord(pmdev_t* pv, const DEV_DATE* cStart, const DEV_DATE* cEnd)
{
  DEV_TIMESEG seg[1] = {0};
  seg->m_Start = *cStart;
  seg->m_End = *cEnd;
  return send_cmd_packet(pv, pv->m_sHost, M_USER, S_GET, F_DL_ID | F_ID_TIME_RANGE, seg, sizeof(TIMESE));
}
//功 能 获取区间识别记录/报警记录指定ID的后续区间
//参 数
//cszDevAddr 设备地址
//cRange ID 区间
//flag DL_DEV_RECORD_PIC=需要下载记录照片
//bKnowRange true=具体数据， false=区间集合
//nSeq 操作序列
//说 明 当bKnowRange 为true该函数下载具体的数据，为false时查询指定区间在设备上的具体大小。 DevOperResultNotify回调参数对应类型：
//下载区间时
//pvContent = DEV_DL_RECORD_ARRAY*
//cRange.m_SID = 指定SN
//cRange.m_Count = -1;
//cRange.m_bOpenRange = TRUE;
//bKnowRange = FALSE，flag = 1（无效参数）
//下载具体记录时
//DevOperResultNotify回调参数对应类型：
//pvContent = DEV_RECORD*
BOOL CPM_DLRangeRec(pmdev_t* pv, const DEV_DL_RECORD_RANGE* cRange, int nFlag, BOOL bKnowRange DEFAULT(FALSE))
{
  int m_CFlag = 0;

  if (bKnowRange) {
    nFlag |= 0x80000000;
  }

  m_CFlag = (nFlag & 0x80000000) ? 0 : F_DL_ID;

  if (nFlag & DL_DEV_PIC) {
    m_CFlag |= F_DL_PIC;
  }

  return send_cmd_packet(pv, pv->m_sHost, M_RANGE, S_GET, m_CFlag, cRange, sizeof(RECORDRANGE));
}
//功 能 下载报警记录区间
//参 数
//cszDevAddr 设备地址
//nSeq 操作序列
//说 明 DevOperResultNotify回调参数对应类型：
//eType = DEV_ALARM_OPER
//eFlag = DEV_OPER_DOWNLOAD，
//pvContent = DEV_DL_RECORD_ARRAY*
//参考DEV_DL_RECORD_ARRAY结构，报警记录只有一个区间
BOOL CPM_DLAllAlarmRecord(pmdev_t* pv)
{
  return send_cmd_packet(pv, pv->m_sHost, M_ALARM, S_GET, F_DL_ID | F_DL_ID_ALL, 0, 0);
}
//功 能 下载指定时间段报警记录
//参 数 cszDevAddr 设备地址
//cStart 起始日期
//cEnd 终止日期
//说 明
BOOL CPM_DLSegTimeAlarmRecord(pmdev_t* pv, const DEV_DATE* cStart, const DEV_DATE* cEnd)
{
  DEV_TIMESEG seg[1] = {0};
  seg->m_Start = *cStart;
  seg->m_End = *cEnd;
  return send_cmd_packet(pv, pv->m_sHost, M_ALARM, S_GET, F_DL_ID | F_ID_TIME_RANGE, seg, sizeof(TIMESE));
}
//功 能 下载区间报警记录
//参 数 cszDevAddr 设备地址
//cRange ID区间
//flag DL_DEV_RECORD_PIC=需要下载记录照片 0x00=不下载照片, 0x01下载照片
//bKnowRange 指定是下载具体数据还是下载ID集合(true=具体数据， false=ID集合)
//说 明 此接口用于从某一条精确的记录ID开始下载nRang条数据
BOOL CPM_DLRangeAlarm(pmdev_t* pv, const DEV_DL_RECORD_RANGE* cRange, int nFlag, BOOL bKnowRange DEFAULT(FALSE))
{
  int m_CFlag = 0;

  if (bKnowRange) {
    nFlag |= 0x80000000;
  }

  m_CFlag = (nFlag & 0x80000000) ? 0 : F_DL_ID;

  if (nFlag & DL_DEV_PIC) {
    m_CFlag |= F_DL_PIC;
  }

  return send_cmd_packet(pv, pv->m_sHost, M_RANGE, S_GET, m_CFlag, cRange, sizeof(RECORDRANGE));
}
//功 能 下载所有日志
//参 数 cszDevAddr 设备地址
//说 明
BOOL CPM_DLLog(pmdev_t* pv)
{
  return send_cmd_packet(pv, pv->m_sHost, M_OPTLOG, S_GET, F_DL_ID | F_DL_ID_ALL, 0, 0);
}
//功 能 下载区间日志
//参 数 cszDevAddr 设备地址
//cRange ID区间
//bKnowRange 指定是下载具体数据还是下载ID集合(true=具体数据， false=ID集合)
//说 明 此接口用于从某一条精确的记录ID开始下载nRang条数据
BOOL CPM_DLRangeLog(pmdev_t* pv, const DEV_DL_RECORD_RANGE* cRange, int nFlag, BOOL bKnowRange DEFAULT(FALSE))
{
  int m_CFlag = 0;

  if (bKnowRange) {
    nFlag |= 0x80000000;
  }

  m_CFlag = (nFlag & 0x80000000) ? 0 : F_DL_ID;

  if (nFlag & DL_DEV_PIC) {
    m_CFlag |= F_DL_PIC;
  }

  return send_cmd_packet(pv, pv->m_sHost, M_RANGE, S_GET, m_CFlag, cRange, sizeof(RECORDRANGE));
}
//功 能 获取变更注册照记录区间
//参 数
//cszDevAddr 设备地址
//nSeq 操作序列
//说 明
//DevOperResultNotify回调参数对应类型：
//eType = DEV_CHANGEIMAGE_OPER
//eFlag = DEV_OPER_DOWNLOAD，
//pvContent = DEV_DL_RECORD_ARRAY*
//参考DEV_DL_RECORD_ARRAY结构，变更注册照只有一个区间
BOOL CPM_DLChangeImage(pmdev_t* pv)
{
  return send_cmd_packet(pv, pv->m_sHost, M_CIMAGE, S_GET, F_DL_ID | F_DL_ID_ALL, 0, 0);
}
//功 能 下载指定区间的变更注册照记录或者查询区间数据的具体大小
//参 数
//cszDevAddr 设备地址
//cRange 区间, 参考区间定义
//flag DL_DEV_RECORD_PIC=需要下载记录照片
//bKnowRange true=具体数据， false=区间集合
//nSeq 操作序列号
//说 明 当bKnowRange 为true该函数下载具体的数据，为false时查询指定区间在设备上的具体大小。
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
BOOL CPM_DLRangeCImage(pmdev_t* pv, const DEV_DL_RECORD_RANGE* cRange, int nFlag, BOOL bKnowRange DEFAULT(FALSE))
{
  int m_CFlag = 0;

  if (bKnowRange) {
    nFlag |= 0x80000000;
  }

  m_CFlag = (nFlag & 0x80000000) ? 0 : F_DL_ID;

  if (nFlag & DL_DEV_PIC) {
    m_CFlag |= F_DL_PIC;
  }

  return send_cmd_packet(pv, pv->m_sHost, M_RANGE, S_GET, m_CFlag, cRange, sizeof(RECORDRANGE));
}
BOOL CPM_TimeGroup_impl(pmdev_t* pv, const DEV_TIMEGROUP_ARRAY* cArray, int m_SCmd, int nType)
{
#define TG_MAX_SIZE 256
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(CARRAY) + sizeof(DSPTIMEG)*TG_MAX_SIZE, 4) >> 2] = {0};
  CARRAY* pArray = (CARRAY*)nData;
  DSPTIMEG* pTG = (DSPTIMEG*)(pArray->m_Block);
  int i, nLen = 0, m_CFlag = 0;
  ASSERT(cArray->m_nCount <= TG_MAX_SIZE);

  switch (nType) {
  case DEV_NORMAL:
    m_CFlag = F_DL_NORMAL_TG;
    break;

  case DEV_HOLIDY:
    m_CFlag = F_DL_HOLIDAY_TG;
    break;

  case DEV_ANY_TIME:
    m_CFlag = F_DL_NORMAL_TG | F_DL_HOLIDAY_TG;
    break;

  default:
    break;
  }

  pArray->m_Count = cArray->m_nCount;

  for (i = 0; i < pArray->m_Count; ++i) {
    pTG[i].m_TGID = (cArray->m_pTGArray)[i].m_TGID;
    memcpy(&(pTG[i].m_Start), &((cArray->m_pTGArray)[i].m_Start), sizeof(DATETIME));
    memcpy(&(pTG[i].m_End), &((cArray->m_pTGArray)[i].m_End), sizeof(DATETIME));
    pTG[i].m_TGType = (cArray->m_pTGArray)[i].m_TGType;
    pTG[i].m_CheckFlag = (cArray->m_pTGArray)[i].m_CheckFlag;
    pTG[i].m_WeekFlag = (cArray->m_pTGArray)[i].m_WeekFlag;
  }

  nLen = sizeof(CARRAY) + sizeof(DSPTIMEG) * (pArray->m_Count);
  return send_cmd_packet(pv, pv->m_sHost, M_TGROUP, m_SCmd, m_CFlag, pArray, nLen);
}
//功 能 设置时间组
//参 数 cszDevAddr 设备地址
//cTGArray 时间组
//说 明
BOOL CPM_ULTimeGroup(pmdev_t* pv, const DEV_TIMEGROUP_ARRAY* cTGArray)
{
  return CPM_TimeGroup_impl(pv, cTGArray, S_SET, DEV_ANY_TIME);
}
//功 能 获取时间组
//参 数 dev_addr 设备地址
//cTGArray 时间组
//eType 时间组类型
//说 明 cTGArray.m_nCount=0时表示下载eType类型的所有时间组
BOOL CPM_DLTimeGroup(pmdev_t* pv, const DEV_TIMEGROUP_ARRAY* cTGArray, DEV_TIMEGROUP_TYPE eType)
{
  return CPM_TimeGroup_impl(pv, cTGArray, S_GET, eType);
}
#define RT_MAX_SIZE 256
//功 能 删除时间组
//参 数 cszDevAddr 设备地址
//cTGArray 时间组
//eType 时间组类型
//说 明 eType.m_nCount=0时表示删除tg_type类型所有时间组
BOOL CPM_DELTimeGroup(pmdev_t* pv, const DEV_TIMEGROUP_ARRAY* cTGArray, DEV_TIMEGROUP_TYPE eType)
{
  return CPM_TimeGroup_impl(pv, cTGArray, S_DEL, eType);
}
BOOL CPM_Right_impl(pmdev_t* pv, const DEV_RIGHT_ARRAY* cArray, int m_SCmd)
{
  int i, nLen = 0;
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(CARRAY) + sizeof(DSPRIGHT)*RT_MAX_SIZE, 4) >> 2] = {0};
  CARRAY* pArray = (CARRAY*)(nData);
  DSPRIGHT* pRG = (DSPRIGHT*)(pArray->m_Block);
  ASSERT(cArray->m_nCount <= RT_MAX_SIZE);
  pArray->m_Count = cArray->m_nCount;

  for (i = 0; i < pArray->m_Count; ++i) {
    pRG[i].m_RightID = (cArray->m_pRtArray)[i].m_RightID;
    memcpy(pRG[i].m_TimeGID, (cArray->m_pRtArray)[i].m_TimeGID, sizeof(pRG[i].m_TimeGID));
    pRG[i].m_Mode = cArray->m_pRtArray[i].m_bHolidyValid ? CHECK_HOLIDY : 0;
    pRG[i].m_Mode |= cArray->m_pRtArray[i].m_bActionLock ? ACTION_LOCK : 0;
    pRG[i].m_Mode |= cArray->m_pRtArray[i].m_bActionOutPut ? ACTION_OUTPUT : 0;
  }

  nLen = sizeof(CARRAY) + sizeof(DSPRIGHT) * (pArray->m_Count);
  return send_cmd_packet(pv, pv->m_sHost, M_DSPRIGHT, m_SCmd, 0, pArray, nLen);
}
//功 能 设置权限
//参 数 cszDevAddr 设备地址
//cRTArray 权限
//说 明
BOOL CPM_ULRight(pmdev_t* pv, const DEV_RIGHT_ARRAY* cRTArray)
{
  return CPM_Right_impl(pv, cRTArray, S_SET);
}
//功 能 发送获取权限
//参 数 cszDevAddr 设备地址
//cRTArray 权限
//说 明 当rt_array.m_nCount=0时表示下载所有权限
BOOL CPM_DLRight(pmdev_t* pv, const DEV_RIGHT_ARRAY* cRTArray)
{
  return CPM_Right_impl(pv, cRTArray, S_GET);
}
//功 能 删除权限
//参 数 cszDevAddr 设备地址
//cRTArray 权限
//说 明 当rt_array.m_nCount=0时表示删除所有权限
BOOL CPM_DELRight(pmdev_t* pv, const DEV_RIGHT_ARRAY* cRTArray)
{
  return CPM_Right_impl(pv, cRTArray, S_DEL);
}
#define UG_MAX_SIZE 256
BOOL CPM_UserGroup_impl(pmdev_t* pv, const DEV_USERGROUP_ARRAY* cArray, int m_SCmd)
{
  int i, nLen = 0;
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(CARRAY) + sizeof(USERGROUP)*UG_MAX_SIZE, 4) >> 2] = {0};
  CARRAY* pArray = (CARRAY*)(nData);
  USERGROUP* pUG = (USERGROUP*)(pArray->m_Block);
  DEV_USERGROUP* pTmp = cArray->m_pUGArray;
  ASSERT(cArray->m_nCount <= UG_MAX_SIZE);
  pArray->m_Count = cArray->m_nCount;

  for (i = 0; i < pArray->m_Count; ++i) {
    pUG->m_GroupID = pTmp->m_GroupID;
    pUG->m_GroupFlag = pTmp->m_bGroupOrder ;
    pUG->m_NormalValid = pTmp->m_NormalValid;
    pUG->m_ForceValid = pTmp->m_ForceValid;
    memcpy(pUG->m_NormalUsers, pTmp->m_NormalUsers, sizeof(pUG->m_NormalUsers));
    memcpy(pUG->m_ForceUsers, pTmp->m_ForceUsers, sizeof(pUG->m_ForceUsers));
    pUG++;
    pTmp++;
  }

  nLen = sizeof(NCMD) + sizeof(CARRAY) + sizeof(USERGROUP) * (pArray->m_Count);
  return send_cmd_packet(pv, pv->m_sHost, M_UGROUP, m_SCmd, 0, pArray, nLen);
}
//功 能 设置用户组
//参 数 cszDevAddr 设备地址
//cUGrray 用户组
//说 明
BOOL CPM_ULUserGroup(pmdev_t* pv, const DEV_USERGROUP_ARRAY* cUGrray)
{
  return CPM_UserGroup_impl(pv, cUGrray, S_SET);
}
//功 能 发送获取用户组
//参 数 cszDevAddr 设备地址
//ug_array 用户组
//说 明 当rt_array.m_nCount=0时表示下载所有用户组
BOOL CPM_DLUserGroup(pmdev_t* pv, const DEV_USERGROUP_ARRAY* cUGrray)
{
  return CPM_UserGroup_impl(pv, cUGrray, S_GET);
}
//功 能 删除用户组
//参 数 cszDevAddr 设备地址
//ug_array 用户组
//说 明 当rt_array.m_nCount=0时表示删除所有用户组
BOOL CPM_DELUserGroup(pmdev_t* pv, const DEV_USERGROUP_ARRAY* cUGrray)
{
  return CPM_UserGroup_impl(pv, cUGrray, S_DEL);
}
//功 能 设置开关门状态
//参 数
//cszDevAddr 设备地址
//bEOpen true=紧急开门， false=紧急关门
//说 明
BOOL CPM_ULOpenDoorState(pmdev_t* pv, DEV_DOOR_STATE eState)
{
  return send_cmd_packet(pv, pv->m_sHost, M_DOOR_STATE, S_SET, eState, 0, 0);
}
//功 能 获取开关门状态
//参 数 cszDevAddr 设备地址
//bEOpen true=紧急开门， false=紧急关门
//说 明
BOOL CPM_DLOpenDoorState(pmdev_t* pv)
{
  return send_cmd_packet(pv, pv->m_sHost, M_DOOR_STATE, S_GET, 0, 0, 0);
}
//功 能 远程开门
//参 数 cszDevAddr 设备地址
//bEOpen true=开门， false=关门
//pDoorArray 门点数组
//size 数组大小
//说 明
BOOL CPM_ULRemoteOpenDoor(pmdev_t* pv, BOOL bOpen)
{
  return send_cmd_packet(pv, pv->m_sHost, M_REMOTE_OPEN, S_SET, bOpen ? F_DOOR_OPEN : F_DOOR_CLOSE, 0, 0);
}
//功 能 IO设备控制
//参 数
//cszDevAddr 设备地址
//DEV_IOCTRL 控制信息
//nSeq 命令执行序列号
//说 明 DEV_IOCTRL:m_Source指定要做操作的端口，DEV_IOCTRL:m_Action要执行的动作
//打开门1，m_Source = 0x1f m_Action = DEV_ACT_IO_OPEN
//打辅助1，m_Source = 0x15 m_Action = DEV_ACT_IO_OPEN
//0x1f和015具体怎么解析的，请参考485地址解析
BOOL CPM_IOCtrl(pmdev_t* pv, const DEV_IOCTRL* ioctrl)
{
  return send_cmd_packet(pv, pv->m_sHost, M_IOCTRL, S_SET, 0, ioctrl, sizeof(DEV_IOCTRL));
}
//功 能 格式化设备
//参 数 cszDevAddr 设备地址
//说 明
BOOL CPM_ULFormat(pmdev_t* pv)
{
  return send_cmd_packet(pv, pv->m_sHost, M_FORMAT, S_SET, 0, 0, 0);
}
//功 能 设备程序更新
//参 数 cszDevAddr 设备地址
//cAppData 程序更新结构体
//说 明
BOOL CPM_ULUpdate(pmdev_t* pv, const APPUPDATE* cAppData)
{
  int nLen = sizeof(APPUPDATE) + cAppData->m_Bytes;
  return send_cmd_packet(pv, pv->m_sHost, M_APPUPDATE, S_SET, 0, cAppData, nLen);
}
//功 能 批量上传用户
//参 数
//cszDevAddr 设备地址
//cUsers 批量用户数据
//nFlag DL_DEV_PIC //照片
// DL_DEV_USER_FEAT //人脸特征
// DL_DEV_USER_TEXT //用户文字信息
//说 明: 批量上传用户必须包含用户文字信息，组合如下
//nFlag = DL_DEV_USER_TEXT 上传文字信息
//nFlag = DL_DEV_USER_TEXT|DL_DEV_PIC 文字+注册照
//nFlag = DL_DEV_USER_TEXT|DL_DEV_USER_FEAT 文字+人脸库
//nFlag = DL_DEV_USER_TEXT|DL_DEV_PIC|DL_DEV_USER_FEAT文字+注册照+人脸库
//DevOperResultNotify回调参数对应类型：
//eType = DEV_USER_EX_OPER
//eFlag = DEV_OPER_UPLOAD ，
//pvContent = DEV_BATCH_USER*。
//在OPER_SUCCESS 情况下，批量上传的用户个数与返回的用户个数一致。否则设备保存失败。批量上传用户数设定在5个以下(和网络带宽，主机处理速度有关)。
BOOL CPM_ULUserEx(pmdev_t* pv, const DEV_BATCH_USER* cUsers, int nFlag)
{
  int ret = 0, nLen = 0;
  char* pbBuf = 0;

  if (nFlag & DL_DEV_USER_TEXT) {
    int binLen = 0 , i = 0;

    for (; i < cUsers->m_nCount; ++i) {
      binLen += ALIGN4(sizeof(USER) + cUsers->m_pUserArray[i].m_PicLen + cUsers->m_pUserArray[i].m_FeatLen);
    }

    pbBuf = (char*)malloc(ALIGN4(OFFSET_SIZE + sizeof(NCMD) + sizeof(CARRAY) + binLen));
    nLen = sizeof(NCMD) + sizeof(CARRAY);

    if (pbBuf) {
      CARRAY* pArray = (CARRAY*)(pbBuf);
      USER* pData = (USER*)(pArray->m_Block);
      DEV_USER* pCurUser = cUsers->m_pUserArray;
      pArray->m_Count = cUsers->m_nCount;

      for (i = 0; i < cUsers->m_nCount; ++i) {
        CopyDevUser2DspUser(pData , pCurUser + i);
        nLen += USER_STEP(*pData);
        pData = (USER*)((char*)pData + USER_STEP(*pData));
      }

      ret = send_cmd_packet(pv, pv->m_sHost, M_USER_EX, S_SET, USER_TEXT, pbBuf, nLen);
      free(pbBuf);
    }
  }
  else {
    ASSERT(FALSE);
  }

  return ret;
}
//功 能 批量下载用户
//参 数
//cszDevAddr 设备地址
//pUserIDArray 用户ID数组首地址
//nIDCount 数组元素个数
//nFlag 同CPM_ULUserEx
//说 明 DevOperResultNotify回调参数对应类型：
//eType = DEV_USER_EX_OPER
//eFlag = DEV_OPER_DOWNLOAD ，
//pvContent= DEV_BATCH_USER*。
//OPER_SUCCESS 情况下，如果批量下载的用户数与要求下载的用户数不一致，可以通过
//CPM_DLAllUser(pmdev_t* pv) 来确认用户是否真的存在于设备上。 批量下载用户数设定在5个以下(和网络带宽，主机处理速度有关)。
BOOL CPM_DLUserEx(pmdev_t* pv, const DEV_CID* pUserIDArray, int nIDCount, int nFlag)
{
  enum {max_n = 5};
  char pbBuf[sizeof(CARRAY) + sizeof(CID) * max_n] = {0};
  int ret = 0, nLen = sizeof(CARRAY) + sizeof(CID) * max_n;
  int i, j, m_CFlag = 0;
  CARRAY* pArray = (CARRAY*)(pbBuf);
  CID* pID = (CID*)(pArray->m_Block);

  if (nFlag & DL_DEV_USER_TEXT) {
    m_CFlag |= USER_TEXT;
  }

  if (nFlag & DL_DEV_USER_FEAT) {
    m_CFlag |= DL_FEAT;
  }

  if (nFlag & DL_DEV_PIC) {
    m_CFlag |= DL_IMAGE;
  }

  for (j = 0; j < nIDCount;) {
    pArray->m_Count = MIN(max_n, nIDCount - j);

    for (i = 0; i < pArray->m_Count; ++i) {
      strncpy(pID[i].m_ID , pUserIDArray[j + i].m_ID , sizeof(CID) - 1);
    }

    ret = send_cmd_packet(pv, pv->m_sHost, M_USER_EX, S_GET, m_CFlag, pbBuf, nLen);
    j += pArray->m_Count;
  }

  return ret;
}
//功 能 批量删除用户
//参 数
//cszDevAddr 设备地址
//pUserIDArray 待删除用户的编号数组
//nIDCount 数组元素个数
//说 明 DevOperResultNotify回调参数对应类型：
//eType = DEV_USER_EX_OPER
//eFlag = DEV_OPER_DELETE，
//pvContent = DEV_BATCH_USER*。
//在OPER_SUCCESS 情况下，批量删除的用户个数与返回的用户个数不一致，可以通过CPM_DLAllUser(pmdev_t* pv)
//来确认用户是否真的存在于设备上。单次批量删除用户数设定在10个以下为好。
BOOL CPM_DELUserEx(pmdev_t* pv, const DEV_CID* pUserIDArray, int nIDCount)
{
  char* pbBuf = (char*)malloc(ALIGN4(OFFSET_SIZE + sizeof(NCMD) + sizeof(CARRAY) + sizeof(CID) * nIDCount));
  int i, ret = 0, nLen = sizeof(CARRAY) + sizeof(CID) * nIDCount;
  int m_CFlag = 0;

  if (pbBuf) {
    CARRAY* pArray = (CARRAY*)(pbBuf);
    CID* pID = (CID*)(pArray->m_Block);
    const DEV_CID* pIDTmp = pUserIDArray;
    pArray->m_Count = nIDCount;

    for (i = 0; i < nIDCount; ++i) {
      ZeroMemory(pID , sizeof(CID));
      strncpy(pID->m_ID , pIDTmp->m_ID , sizeof(CID) - 1);
      ++pID;
      ++pIDTmp;
    }

    ret = send_cmd_packet(pv, pv->m_sHost, M_USER_EX, S_DEL, m_CFlag, pbBuf, nLen);
    free(pbBuf);
  }

  return ret;
}
//功 能 批量下载记录
//参 数 cszDevAddr 设备地址
//cRange ID区间
//说 明 包括验证记录、报警记录，操作日志、变更注册照
BOOL CPM_DLRecEx(pmdev_t* pv, const DEV_DL_RECORD_RANGE* cRange)
{
  return send_cmd_packet(pv, pv->m_sHost, M_RANGE_EX, S_GET, F_DL_PIC, cRange, sizeof(RECORDRANGE));
}
//功 能 重启设备
//参 数 cszDevAddr 设备地址
//说 明
BOOL CPM_RebootDev(pmdev_t* pv)
{
  return send_cmd_packet(pv, pv->m_sHost, M_REBOOT, S_SET, 0, 0, 0);
}
//功 能 获得/设置 设备网络信息
//参 数 cszDevAddr 设备地址
// pNetPro NULL表示获得，不为空表示设置
//说 明
BOOL CPM_NetPro(pmdev_t* pv, DEV_NETPRO* pNetPro)
{
  return send_cmd_packet(pv, pv->m_sHost, M_NETPRO, pNetPro ? S_SET : S_GET, 0, pNetPro, pNetPro ? sizeof(DEV_NETPRO) : 0);
}


void DspRec2UspRec(RECORD* dsp, DEV_RECORD* usp, BOOL realTime)
{
  usp->m_RecType = (DEV_REC_TYPE)(dsp->m_RecType);
  usp->m_bRealTime = realTime;
  usp->m_Score = dsp->m_Score;
  usp->m_PicLen = dsp->m_PicBytes;
  usp->m_VerifyMode = dsp->m_VerifyMode;
  usp->m_ConcretSource = dsp->m_Source;
  usp->m_Source = DEV_DOOR_SOURCE;
  usp->m_PicData = dsp->m_Block;
  memcpy(usp->m_SerialID.m_ID, dsp->m_SerialID.m_ID, sizeof(CID));
  memcpy(usp->m_UserID.m_ID, dsp->m_UserID.m_ID, sizeof(CID));
  memcpy(usp->m_ChangID.m_ID, dsp->m_ChangID.m_ID, sizeof(CID));
  memcpy(usp->m_AppendID.m_ID, dsp->m_AppendID.m_ID, sizeof(CID));
  memcpy(usp->m_UserName.m_ID, dsp->m_UserName.m_ID, sizeof(CID));
  memcpy(usp->m_Department.m_ID, dsp->m_Department.m_ID, sizeof(CID));
  memcpy(&(usp->m_RecTime), &(dsp->m_RecTime), sizeof(DATETIME));
}


int send_msg(pmdev_t* pv, const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  int i, MCmd = pCMD->m_MCmd;

  if (pCMD->m_CFlag & F_DL_ID) {
    switch (MCmd) {
    case M_USER: {
      DEV_DL_USER_RANGE Range = {0};

      if (0 == GET_ERR(pCMD->m_CFlag)) {
        CARRAY* pArray = (CARRAY*)(pCMD + 1);
        Range.m_Count = pArray->m_Count;
        Range.m_pArray = (DEV_VUSER*)(pArray->m_Block);
      }

      pv->m_eventfun(pv, cszIP, NULL, DEV_USER_RANGE_OPER, DEV_OPER_DOWNLOAD, &Range, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
    }
    break;

    case M_VERIFY:
    case M_ALARM:
    case M_OPTLOG:
    case M_CIMAGE:
    case M_RANGE: {
      DEV_OPER_TYPE type = DEV_RECORD_RANGE_OPER;
      DEV_DL_RECORD_ARRAY temp[1] = {0};

      switch (pCMD->m_MCmd) {
      case M_VERIFY:
        type = DEV_RECORD_RANGE_OPER;
        break;

      case M_ALARM:
        type = DEV_ALARM_RANGE_OPER;
        break;

      case M_OPTLOG:
        type = DEV_LOG_RANGE_OPER;
        break;

      case M_CIMAGE:
        type = DEV_CIMAGE_RANGE_OPER;
        break;

      default:
        break;
      }

      if (0 == GET_ERR(pCMD->m_CFlag)) {
        CARRAY* pArray = (CARRAY*)(pCMD + 1);
        temp->m_Count = pArray->m_Count;
        temp->m_pRange = (DEV_DL_RECORD_RANGE*)(pArray->m_Block);
      }

      pv->m_eventfun(pv, cszIP, NULL, type, DEV_OPER_DOWNLOAD, temp, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
    }
    break;

    default:
      ASSERT(0);
      break;
    }
  }
  else {
    switch (MCmd) {
    case M_AUTH:
      pv->m_Author = (pCMD->m_CFlag & F_AUTH_VERIFY);
      pv->m_eventfun(pv, cszIP, NULL, (pCMD->m_CFlag & F_AUTH_VERIFY) ? DEV_AUTH_OPER : DEV_AUTH_SET_OPER, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
      break;

    case M_DEVINFO:
      pv->m_eventfun(pv, cszIP, NULL, DEV_BASEINFO_OPER, DEV_OPER_DOWNLOAD, (DEV_BASEINFO*)(pCMD + 1), pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
      break;

    case M_REBOOT:
      pv->m_eventfun(pv, cszIP, NULL, DEV_REBOOT_OPER, DEV_OPER_DOWNLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
      break;

    case M_NETPRO:
      pv->m_eventfun(pv, cszIP, NULL, DEV_NETPRO_OPER, DEV_OPER_DOWNLOAD, (DEV_NETPRO*)(pCMD + 1), pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
      break;

    case M_CIMAGE: {
      DEV_CHANGEIMAGE Image[1] = {0};
      ASSERT(pCMD);

      if (SUC_EXECUT == GET_ERR(pCMD->m_CFlag)) {
        CHANGEIMAGE* pRecord = (CHANGEIMAGE*)(pCMD + 1);
        Image->m_PicBytes = pRecord->m_PicBytes;
        Image->m_bRTChangeImage = (pCMD->m_SCmd == S_REALTIME_SEND) ? TRUE : FALSE;
        memcpy(Image->m_UserID.m_ID, pRecord->m_UserID.m_ID, sizeof(CID));
        memcpy(Image->m_SerialID.m_ID, pRecord->m_SerialID.m_ID, sizeof(CID));
        memcpy(Image->m_AppendID.m_ID, pRecord->m_AppendID.m_ID, sizeof(CID));
        memcpy(Image->m_UserName.m_ID, pRecord->m_UserName.m_ID, sizeof(CID));
        memcpy(Image->m_Department.m_ID, pRecord->m_Department.m_ID, sizeof(CID));
        memcpy(Image->m_Admin.m_ID, pRecord->m_Admin.m_ID, sizeof(CID));
        memcpy(&(Image->m_RecTime), &(pRecord->m_RecTime), sizeof(DATETIME));
        Image->m_PicData = pRecord->m_Block;
      }
      else if (OPER_ERR_BUSY != GET_ERR(pCMD->m_CFlag)) {
        memcpy(&(Image->m_SerialID), pCMD + 1, sizeof(CID));
      }

      pv->m_eventfun(pv, cszIP, NULL, DEV_CHANGEIMAGE_OPER, DEV_OPER_DOWNLOAD, Image, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
    }
    break;

    case M_OPTLOG: {
      DEV_LOG Log;
      ASSERT(pCMD);
      memset(&Log, 0x0, sizeof(DEV_LOG));

      if (0 == GET_ERR(pCMD->m_CFlag)) {
        DSPLOG* pRecord = (DSPLOG*)(pCMD + 1);
        Log.m_LogType = (DEV_LOG_TYPE)(pRecord->m_LogType);
        Log.m_bRTLog = (pCMD->m_SCmd == S_REALTIME_SEND) ? TRUE : FALSE;
        memcpy(Log.m_Admin.m_ID, pRecord->m_Admin.m_ID, sizeof(CID));
        memcpy(Log.m_BeOptUser.m_ID, pRecord->m_BeOptUser.m_ID, sizeof(CID));
        memcpy(Log.m_SerialID.m_ID, pRecord->m_SerialID.m_ID, sizeof(CID));
        memcpy(&(Log.m_RecTime), &(pRecord->m_RecTime), sizeof(DATETIME));
      }
      else if (OPER_ERR_BUSY != GET_ERR(pCMD->m_CFlag)) {
        memcpy(Log.m_SerialID.m_ID, pCMD + 1, sizeof(CID));
      }

      pv->m_eventfun(pv, cszIP, NULL, DEV_LOG_OPER, DEV_OPER_DOWNLOAD, &Log, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
    }
    break;

    case M_DOOR_STATE: {
      if (pCMD->m_SCmd == S_SET) {
        pv->m_eventfun(pv, cszIP, NULL, DEV_DOOR_STATE_OPER, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
      }
      else if (pCMD->m_SCmd == S_GET) {
        DEV_DOOR_STATE nStatus = DEV_DOOR_NOMAL;

        if (pCMD->m_CFlag & DEV_DOOR_OPEN) {
          nStatus = DEV_DOOR_OPEN;
        }
        else if (pCMD->m_CFlag & DEV_DOOR_CLOSE) {
          nStatus = DEV_DOOR_CLOSE;
        }
        else {
          nStatus = DEV_DOOR_NOMAL;
        }

        pv->m_eventfun(pv, cszIP, NULL, DEV_DOOR_STATE_OPER, DEV_OPER_DOWNLOAD, &nStatus, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
      }
    }
    break;

    case M_REMOTE_OPEN:
      pv->m_eventfun(pv, cszIP, NULL, DEV_REMOTE_OPEN_DOOR, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
      break;

    case M_IOCTRL:
      pv->m_eventfun(pv, cszIP, NULL, DEV_IOCTRL_OPER, DEV_OPER_DOWNLOAD, (DEV_IOCTRL*)(pCMD + 1), pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
      break;

    case M_NOPEN:
      pv->m_eventfun(pv, cszIP, NULL, DEV_NOPEN_OPER, DEV_OPER_DOWNLOAD, (DEV_NOPEN*)(pCMD + 1), pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
      break;

    case M_APPUPDATE:
      pv->m_eventfun(pv, cszIP, NULL, DEV_UPDATE_OPER, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
      break;

    case M_VERIFY: {
      DEV_RECORD Tmp[1] = {0};
      ASSERT(pCMD);

      if (0 == GET_ERR(pCMD->m_CFlag)) {
        DspRec2UspRec((RECORD*)(pCMD + 1), Tmp, pCMD->m_SCmd == S_REALTIME_SEND);
      }
      else if (OPER_ERR_BUSY != GET_ERR(pCMD->m_CFlag)) {
        memcpy(Tmp->m_SerialID.m_ID, pCMD + 1, sizeof(CID));//??
      }

      pv->m_eventfun(pv, cszIP, NULL, DEV_RECORD_OPER, DEV_OPER_DOWNLOAD, Tmp, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
    }
    break;

    case M_ALARM: {
      DEV_RECORD Tmp[1];
      ASSERT(pCMD);
      memset(Tmp, 0x0, sizeof(DEV_RECORD));

      if (0 == GET_ERR(pCMD->m_CFlag)) {
        DspRec2UspRec((RECORD*)(pCMD + 1), Tmp, pCMD->m_SCmd == S_REALTIME_SEND);
      }
      else {
        memcpy(Tmp->m_SerialID.m_ID, pCMD + 1, sizeof(CID));
      }

      pv->m_eventfun(pv, cszIP, NULL, DEV_ALARM_OPER, DEV_OPER_DOWNLOAD, Tmp, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
    }
    break;

    case M_RANGE_EX:

    {
      ASSERT(pCMD);

#define BATCH_MAX 200

      if (SUC_EXECUT == GET_ERR(pCMD->m_CFlag)) {
        CARRAY* pArray = (CARRAY*)(pCMD + 1);
        int i;

        switch (pCMD->m_SCmd) {
        case M_VERIFY:
        case M_ALARM:
          if (1) {
            RECORD* pData = (RECORD*)(pArray->m_Block);
            DEV_BATCH_RECORD records = {0};
            DEV_RECORD record[BATCH_MAX];
            records.m_nCount = pArray->m_Count;
            records.m_pRecordArray = record;

            for (i = 0; i < pArray->m_Count; ++i) {
              DspRec2UspRec(pData, record + i, FALSE);
              pData = (RECORD*)((char*)pData + RECORD_STEP(*pData));
            }

            if (pCMD->m_CFlag & BATCH_END) {
              pCMD->m_CFlag = OPER_BATCH_DL_COMPLETE;
            }

            pv->m_eventfun(pv, cszIP, NULL, DEV_RECORD_EX_OPER, DEV_OPER_DOWNLOAD, &records, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
          }

          break;

        case M_OPTLOG: {
          DSPLOG* pRecord = (DSPLOG*)(pArray->m_Block);
          DEV_BATCH_LOG records = {0};
          DEV_LOG record[BATCH_MAX];
          records.m_nCount = pArray->m_Count;
          records.m_pLogArray = record;

          for (i = 0; i < pArray->m_Count; ++i) {
            DEV_LOG* Log = record + i;
            Log->m_LogType = (DEV_LOG_TYPE)(pRecord->m_LogType);
            Log->m_bRTLog = FALSE;
            memcpy(Log->m_Admin.m_ID, pRecord->m_Admin.m_ID, sizeof(CID));
            memcpy(Log->m_BeOptUser.m_ID, pRecord->m_BeOptUser.m_ID, sizeof(CID));
            memcpy(Log->m_SerialID.m_ID, pRecord->m_SerialID.m_ID, sizeof(CID));
            memcpy(&(Log->m_RecTime), &(pRecord->m_RecTime), sizeof(DATETIME));
            pRecord = (DSPLOG*)((char*)pRecord + DSPLOG_STEP(*pRecord));
          }

          if (pCMD->m_CFlag & BATCH_END) {
            pCMD->m_CFlag = OPER_BATCH_DL_COMPLETE;
          }

          pv->m_eventfun(pv, cszIP, NULL, DEV_LOG_EX_OPER, DEV_OPER_DOWNLOAD, &records, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
        }
        break;

        case M_CIMAGE: {
          CHANGEIMAGE* pRecord = (CHANGEIMAGE*)(pArray->m_Block);
          DEV_BATCH_CIMAGE records = {0};
          DEV_CHANGEIMAGE record[BATCH_MAX];
          records.m_nCount = pArray->m_Count;
          records.m_pCImageArray = record;

          for (i = 0; i < pArray->m_Count; ++i) {
            DEV_CHANGEIMAGE* Image = record + i;
            Image->m_PicBytes = pRecord->m_PicBytes;
            Image->m_PicData = pRecord->m_Block;
            Image->m_bRTChangeImage = (pCMD->m_SCmd == S_REALTIME_SEND) ? TRUE : FALSE;
            memcpy(&(Image->m_UserID), &(pRecord->m_UserID), sizeof(CID));
            memcpy(&(Image->m_SerialID), &(pRecord->m_SerialID), sizeof(CID));
            memcpy(Image->m_AppendID.m_ID, pRecord->m_AppendID.m_ID, sizeof(CID));
            memcpy(Image->m_UserName.m_ID, pRecord->m_UserName.m_ID, sizeof(CID));
            memcpy(Image->m_Department.m_ID, pRecord->m_Department.m_ID, sizeof(CID));
            memcpy(Image->m_Admin.m_ID, pRecord->m_Admin.m_ID, sizeof(CID));
            memcpy(&(Image->m_RecTime), &(pRecord->m_RecTime), sizeof(DATETIME));
            pRecord = (CHANGEIMAGE*)((char*)pRecord + CGIMG_STEP(*pRecord));
          }

          if (pCMD->m_CFlag & BATCH_END) {
            pCMD->m_CFlag = OPER_BATCH_DL_COMPLETE;
          }

          pv->m_eventfun(pv, cszIP, NULL, DEV_CIMAGE_EX_OPER, DEV_OPER_DOWNLOAD, &records, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
        }
        break;

        default:
          break;
        }
      }
      else {
        switch (pCMD->m_SCmd) {
        case M_VERIFY:
        case M_ALARM:
          pv->m_eventfun(pv, cszIP, NULL, DEV_RECORD_EX_OPER, DEV_OPER_DOWNLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
          break;

        case M_OPTLOG:
          pv->m_eventfun(pv, cszIP, NULL, DEV_LOG_EX_OPER, DEV_OPER_DOWNLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
          break;

        case M_CIMAGE:
          pv->m_eventfun(pv, cszIP, NULL, DEV_CIMAGE_EX_OPER, DEV_OPER_DOWNLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
          break;

        default:
          break;
        }
      }
    }
    break;

    case M_MCAST: {
      ASSERT(pCMD);

      switch (pCMD->m_SCmd) {
      case S_SET:
        pv->m_eventfun(pv, cszIP, NULL, DEV_REGION_OPER, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
        break;

      case S_GET:
        //GetRgn(cszIP, pCMD, bServer, pvClientData);
        break;

      default:
        break;
      }

    }
    break;

    case M_DSPRIGHT: {
      ASSERT(pCMD);

      switch (pCMD->m_SCmd) {
      case S_SET:
        pv->m_eventfun(pv, cszIP, NULL, DEV_RIGHT_OPER, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
        break;

      case S_GET: {
        int i;
#define TG_MAX_SIZE 256
        DEV_RIGHT_ARRAY r_array = {0};
        DEV_RIGHT RG[TG_MAX_SIZE] = {0};

        if (0 == GET_ERR(pCMD->m_CFlag)) {
          CARRAY* pArray = (CARRAY*)(pCMD + 1);
          DSPRIGHT* pRG = (DSPRIGHT*)(pArray->m_Block);
          r_array.m_nCount = pArray->m_Count > TG_MAX_SIZE ? TG_MAX_SIZE : pArray->m_Count;
          r_array.m_pRtArray = RG;
          ASSERT(pArray->m_Count <= TG_MAX_SIZE);

          for (i = 0; i < r_array.m_nCount; ++i) {
            RG[i].m_RightID = pRG[i].m_RightID;
            memcpy(RG[i].m_TimeGID, pRG[i].m_TimeGID, sizeof(pRG[i].m_TimeGID));
            RG[i].m_bHolidyValid = (BOOL)(pRG[i].m_Mode & CHECK_HOLIDY);
            RG[i].m_bActionLock = (BOOL)(pRG[i].m_Mode & ACTION_LOCK);
            RG[i].m_bActionOutPut = (BOOL)(pRG[i].m_Mode & ACTION_OUTPUT);
          }
        }

        pv->m_eventfun(pv, cszIP, NULL, DEV_RIGHT_OPER, DEV_OPER_DOWNLOAD, &r_array, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
      }
      break;

      case S_DEL:
        pv->m_eventfun(pv, cszIP, NULL, DEV_RIGHT_OPER, DEV_OPER_DELETE, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
        break;
      }
    }

    break;

    case M_DEVSET: {
      ASSERT(pCMD);

      switch (pCMD->m_SCmd) {
      case S_SET:
        pv->m_eventfun(pv, cszIP, NULL, DEV_WORKATT_OPER, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
        break;

      case S_GET: {
        DEV_WORKATT att[1] = {0};

        if (0 == GET_ERR(pCMD->m_CFlag)) {
          DEVSET* pSet = (DEVSET*)(pCMD + 1);
          memcpy(att->m_TimeGID, pSet->m_DevCtx, sizeof(pSet->m_DevCtx));
          att->m_BaseSet = pSet->m_BaseSet;
          att->m_DoorMangetTime = pSet->m_DoorMangetTime;
          att->m_LockTime = pSet->m_LockTime;
          att->m_VerifyMode = pSet->m_VerifyMode;
          att->m_bWGOutPut = (BOOL)(pSet->m_OutputWG & OUTPUT_WG_YES);
          att->m_nWGType = (BOOL)(pSet->m_OutputWG & OUTPUT_WG_34);
          att->m_nWGOutType = (BOOL)(pSet->m_OutputWG & OUTPUT_WG_CTX);
          att->m_nWGOutIDType = (BOOL)(pSet->m_OutputWG & OUTPUT_WG_UID);
          att->m_nWGOutContent = pSet->m_OutputCtx;
          att->m_DefaultRight = (DEV_DEFAULT_RIGHT)(pSet->m_DefaultRight);
          memcpy(att->m_szSuperPWD.m_ID, pSet->m_SuperPassword.m_ID, sizeof(DEV_CID));
        }

        pv->m_eventfun(pv, cszIP, NULL, DEV_WORKATT_OPER, DEV_OPER_DOWNLOAD, att, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
      }
      break;

      default:
        break;
      }
    }
    break;

    case M_STATIS:
      pv->m_eventfun(pv, cszIP, NULL, DEV_STATISINFO_OPER, DEV_OPER_DOWNLOAD, (DEV_STATIS*)(pCMD + 1), pCMD->m_Param1, GET_ERR(pCMD->m_CFlag) , pvClientData);
      break;

    case M_TIME: {
      ASSERT(pCMD);

      switch (pCMD->m_SCmd) {
      case S_SET:
        pv->m_eventfun(pv, cszIP, NULL, DEV_SYS_TIME_OPER, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
        break;

      case S_GET:
        pv->m_eventfun(pv, cszIP, NULL, DEV_SYS_TIME_OPER, DEV_OPER_DOWNLOAD, (DEV_DATETIME*)(pCMD + 1), pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
        break;

      default:
        break;
      }
    }
    break;

    case M_TGROUP: {
      ASSERT(pCMD);

      switch (pCMD->m_SCmd) {
      case S_SET:
        pv->m_eventfun(pv, cszIP, NULL, DEV_TIME_GROUP_OPER, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
        break;

      case S_GET: {
#define TG_MAX_SIZE 256

        if (OPER_SUCCESS == GET_ERR(pCMD->m_CFlag)) {
          CARRAY* pArray = (CARRAY*)(pCMD + 1);
          DSPTIMEG* pTG = (DSPTIMEG*)(pArray->m_Block);
          DEV_TIMEGROUP_ARRAY t_array = {0};
          DEV_TIMEGROUP TG[TG_MAX_SIZE] = {0};
          ASSERT(pArray->m_Count <= TG_MAX_SIZE);
          t_array.m_nCount = pArray->m_Count > TG_MAX_SIZE ? TG_MAX_SIZE : pArray->m_Count;
          t_array.m_pTGArray = TG;

          for (i = 0; i < t_array.m_nCount; ++i) {
            TG[i].m_TGID = pTG[i].m_TGID;
            memcpy(&(TG[i].m_Start), &(pTG[i].m_Start), sizeof(DATETIME));
            memcpy(&(TG[i].m_End), &(pTG[i].m_End), sizeof(DATETIME));
            TG[i].m_TGType = (DEV_TIMEGROUP_TYPE)(pTG[i].m_TGType);
            TG[i].m_CheckFlag = pTG[i].m_CheckFlag;
            TG[i].m_WeekFlag = pTG[i].m_WeekFlag;
          }

          pv->m_eventfun(pv, cszIP, NULL, DEV_TIME_GROUP_OPER, DEV_OPER_DOWNLOAD, &t_array, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
        }
        else {
          pv->m_eventfun(pv, cszIP, NULL, DEV_TIME_GROUP_OPER, DEV_OPER_DOWNLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
        }

#undef TG_MAX_SIZE
      }
      break;

      case S_DEL:
        pv->m_eventfun(pv, cszIP, NULL, DEV_TIME_GROUP_OPER, DEV_OPER_DELETE, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
        break;
      }
    }
    break;

    case M_USER: {
      ASSERT(pCMD);

      switch (pCMD->m_SCmd) {
      case S_SET: {
        DEV_USER user = {0};
        memcpy(user.m_UserID.m_ID, pCMD + 1, sizeof(CID));
        pv->m_eventfun(pv, cszIP, NULL, DEV_USER_OPER, DEV_OPER_UPLOAD, &user, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
      }
      break;

      case S_DEL: {
        DEV_USER user = {0};
        memcpy(user.m_UserID.m_ID, pCMD + 1, sizeof(CID));
        pv->m_eventfun(pv, cszIP, NULL, DEV_USER_OPER, DEV_OPER_DELETE, &user, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
      }
      break;

      case S_REALTIME_LOAD: {
        DEV_USER_REAL_LOAD mode = {0};

        if (pCMD->m_CFlag & F_DL_PIC) {
          mode.m_LoadFlag |= DL_DEV_PIC;
        }

        if (pCMD->m_CFlag & F_DL_FEAT) {
          mode.m_LoadFlag |= DL_DEV_USER_FEAT;
        }

        mode.m_ReqSource = pCMD->m_Param1;
        mode.m_bApeendID = (pCMD->m_CFlag & F_DL_APPENDID) ? TRUE : FALSE;
        memcpy(mode.m_ID.m_ID, pCMD + 1, sizeof(CID));
        pv->m_eventfun(pv, cszIP, NULL, DEV_USER_RT_OPER, DEV_OPER_UPLOAD, &mode, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
      }
      break;

      case S_GET:
      case S_REALTIME_SEND:
      case S_UPDATE_SEND: {
        DEV_USER user = {0};

        if (0 == GET_ERR(pCMD->m_CFlag)) {
          USER* pUser = (USER*)(pCMD + 1);
          memcpy(&user, pUser, OFF_SET(USER, m_UserType));
          user.m_UserType = (DEV_USER_TYPE)pUser->m_UserType;
          user.m_FeatData = pUser->m_Block; //特征数据
          user.m_PicData = pUser->m_Block + pUser->m_FeatBytes; //照片数据

          switch (pCMD->m_SCmd) {
          case S_REALTIME_SEND:
            user.m_bRTUser = 1;
            break;

          case S_UPDATE_SEND:
            user.m_bRTUser = 2;
            break;
          }
        }
        else if (OPER_ERR_BUSY != GET_ERR(pCMD->m_CFlag)) {
          memcpy(user.m_UserID.m_ID, pCMD + 1, sizeof(CID));
        }

        pv->m_eventfun(pv, cszIP, NULL, DEV_USER_OPER, DEV_OPER_DOWNLOAD, &user, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
      }
      break;

      case S_REALTIME_CAP: {
        DEV_USER user = {0};

        if (0 == pCMD->m_Param1) {
          if (0 == GET_ERR(pCMD->m_CFlag)) {
            USER* pUser = (USER*)(pCMD + 1);
            memcpy(&user, pUser, OFF_SET(USER, m_UserType));
            user.m_UserType = (DEV_USER_TYPE)pUser->m_UserType;
            user.m_FeatData = pUser->m_Block; //特征数据
            user.m_PicData = pUser->m_Block + pUser->m_FeatBytes; //照片数据
            user.m_bRTUser = 1;
          }
        }

        pv->m_eventfun(pv, cszIP, NULL, DEV_USER_REMOTE_CAP, DEV_OPER_UPLOAD, &user, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
      }
      break;

      default:
        break;
      }
    }
    break;

    case M_USER_EX: {
      switch (pCMD->m_SCmd) {
      case S_SET: {
        DEV_BATCH_USER users = {0};

        if (OPER_ERR_BUSY != GET_ERR(pCMD->m_CFlag)) {
          CARRAY* pArray = (CARRAY*)(pCMD + 1);
          users.m_nCount = pArray->m_Count;

          if (users.m_nCount) {
            CID* pID;
            DEV_USER* pUser;
            users.m_pUserArray = (DEV_USER*)malloc(ALIGN4(sizeof(DEV_USER) * users.m_nCount));
            pID = (CID*)(pArray->m_Block);
            pUser = users.m_pUserArray;

            for (i = 0; i < pArray->m_Count; ++i) {
              ZeroMemory(pUser , sizeof(DEV_USER));
              strncpy(pUser->m_UserID.m_ID , pID->m_ID , sizeof(CID) - 1);
              ++pUser;
              ++pID;
            }
          }
        }

        pv->m_eventfun(pv, cszIP, NULL, DEV_USER_EX_OPER, DEV_OPER_UPLOAD, &users, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);

        if (users.m_pUserArray) {
          free((char*)users.m_pUserArray);
        }
      }
      break;

      case S_DEL: {
        DEV_BATCH_USER users;

        if (OPER_ERR_BUSY != GET_ERR(pCMD->m_CFlag)) {
          CARRAY* pArray = (CARRAY*)(pCMD + 1);
          users.m_nCount = pArray->m_Count;

          if (users.m_nCount) {
            CID* pID;
            DEV_USER* pUser;

            users.m_pUserArray = (DEV_USER*)malloc(ALIGN4(sizeof(DEV_USER) * users.m_nCount));
            pID = (CID*)(pArray->m_Block);
            pUser = users.m_pUserArray;

            for (i = 0; i < pArray->m_Count; ++i) {
              ZeroMemory(pUser, sizeof(DEV_USER));
              strncpy(pUser->m_UserID.m_ID , pID->m_ID , sizeof(CID) - 1);
              ++pUser;
              ++pID;
            }
          }
        }

        pv->m_eventfun(pv, cszIP, NULL, DEV_USER_EX_OPER, DEV_OPER_DELETE, &users, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
        free((char*)users.m_pUserArray);
      }
      break;

      case S_GET: {
        DEV_BATCH_USER users = {0};
        CARRAY* pArray = (CARRAY*)(pCMD + 1);

        if (OPER_SUCCESS == GET_ERR(pCMD->m_CFlag)) {
          users.m_nCount = pArray->m_Count;
          users.m_pUserArray = (DEV_USER*)malloc(ALIGN4(sizeof(DEV_USER) * pArray->m_Count));

          if (users.m_pUserArray) {
            if (pCMD->m_CFlag & USER_TEXT) {
              USER* pData = (USER*)(pArray->m_Block);
              DEV_USER* pCurUser = users.m_pUserArray;

              for (i = 0; i < pArray->m_Count; ++i) {
                memcpy(pCurUser, pData, OFF_SET(USER , m_UserType));
                pCurUser->m_UserType = (DEV_USER_TYPE)pData->m_UserType;
                pCurUser->m_FeatData = pData->m_Block;  //特征数据
                pCurUser->m_PicData = pData->m_Block + pData->m_FeatBytes; //照片数据
                pData = (USER*)((char*)pData + USER_STEP(*pData));
                ++pCurUser;
              }
            }
            else {
              ASSERT(FALSE);
            }
          }

          if (pCMD->m_CFlag & BATCH_END) {
            pCMD->m_CFlag = OPER_BATCH_DL_COMPLETE;
          }
        }

        pv->m_eventfun(pv, cszIP, NULL, DEV_USER_EX_OPER, DEV_OPER_DOWNLOAD, &users, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);

        if (users.m_pUserArray) {
          free((char*)users.m_pUserArray);
        }
      }
      break;
      }
    }
    break;

    case M_UGROUP: {
      ASSERT(pCMD);

      switch (pCMD->m_SCmd) {
      case S_SET:
        pv->m_eventfun(pv, cszIP, NULL, DEV_USERGROUP_OPER, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
        break;

      case S_DEL:
        pv->m_eventfun(pv, cszIP, NULL, DEV_RIGHT_OPER, DEV_OPER_DELETE, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
        break;

      case S_GET: {
#define TG_MAX_SIZE 256
        DEV_USERGROUP_ARRAY u_array = {0};
        DEV_USERGROUP UG[TG_MAX_SIZE] = {0};

        if (OPER_SUCCESS == GET_ERR(pCMD->m_CFlag)) {
          CARRAY* pArray = (CARRAY*)(pCMD + 1);
          USERGROUP* pUG = (USERGROUP*)(pArray->m_Block);
          ASSERT(pArray->m_Count <= TG_MAX_SIZE);
          u_array.m_nCount = pArray->m_Count > TG_MAX_SIZE ? TG_MAX_SIZE : pArray->m_Count;
          u_array.m_pUGArray = UG;

          for (i = 0; i < u_array.m_nCount; ++i) {
            UG[i].m_GroupID = pUG[i].m_GroupID;
            UG[i].m_bGroupOrder = pUG[i].m_GroupFlag ;
            UG[i].m_NormalValid = pUG[i].m_NormalValid;
            UG[i].m_ForceValid = pUG[i].m_ForceValid;
            memcpy(UG[i].m_NormalUsers, pUG[i].m_NormalUsers, sizeof(pUG[i].m_NormalUsers));
            memcpy(UG[i].m_ForceUsers, pUG[i].m_ForceUsers, sizeof(pUG[i].m_ForceUsers));
          }
        }

        pv->m_eventfun(pv, cszIP, NULL, DEV_USERGROUP_OPER, DEV_OPER_DOWNLOAD, &u_array, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
      }

#undef TG_MAX_SIZE
      break;

      }
    }
    break;

    case M_VIDEO_OUT: {
      switch (pCMD->m_SCmd) {
      case S_SET:
        pv->m_eventfun(pv, cszIP, NULL, DEV_VIDEO_TRANSFER, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
        break;

      case S_GET:
        pv->m_eventfun(pv, cszIP, NULL, DEV_VIDEO_TRANSFER, DEV_OPER_DOWNLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
        break;

      default:
        break;
      }
    }
    break;

    default:
      printf("m_MCmd = %d ???\n", pCMD->m_MCmd);
      break;
    }
  }

  return 0;
}


//接收实时控制数据线程
static DWORD WINAPI RecvRealControlDataThread(pmdev_t* pv)
{
  enum {pack_size = sizeof(SOCK_DATA_HEADER) + PACK_SIZE};
  int i, nLen = 0;
  int nCmd = 0, err = 0;

  enum { maxbufnum = 32 };
  databuf_t databuf[maxbufnum] = {0};

  while (!pv->m_exitThread) {
    //解析网络的数据包
    databuf_t* it = recv_pack_data(pv->m_sHost, databuf, maxbufnum, 1, &err, 0, 0, 0);

    //printf("recv len %d\n", nLen);
    if (err < 0) {
      pv->m_eventfun(pv, pv->m_strDevIP, NULL, DEV_CONNECT_CUT, 0, 0, 0, err, pv->userdata);
      break;
    }

    if (it && it->buf) {
      NCMD* pCMD = (NCMD*)(it->buf);

      //心跳
      if (M_HEART == pCMD->m_MCmd) {
        if (SUC_EXECUT == GET_ERR(pCMD->m_CFlag)) {
          //发送封包
          send_cmd_packet(pv, pv->m_sHost, M_HEART, S_SET, 0, 0, 0);
        }
      }
      else {
        send_msg(pv, pv->m_strDevIP, pCMD, 0, pv->userdata);
      }
    }

    sleep(50);
  }

  for (i = 0; i < maxbufnum; ++i) {
    if (databuf[i].buf) {
      free(databuf[i].buf);
      databuf[i].buf = NULL;
    }
  }

  pv->m_exitThread = FALSE;
  return FALSE;
}

int _Connect(SOCKET s, const struct sockaddr FAR* name, int namelen, int ms)
{
  //set the socket in non-blocking mode
  int nResult;

  if (ms > 0) {
    struct timeval timeout = {ms / 1000, ms % 1000};
    unsigned long argp = 1;
    FD_SET fd = { 1, s };
    ioctlsocket(s, FIONBIO, &argp);
    nResult = connect(s, name, namelen);
    select(0, NULL, &fd, NULL, &timeout);

    //set the socket in blocking mode again.
    argp = 0;
    ioctlsocket(s, FIONBIO, &argp);
  }
  else {
    nResult = connect(s, name, namelen);
  }

  return nResult;
}

//初始化
int CPM_Connect(pmdev_t* pv, const char* strDevIP, const char* sLoginName, const char* sPassword, event_fun_t eventfun, void* userdata)
{
  int nServAddlen, retVal;
  //与设备建立连接
  SOCKADDR_IN servAddr; //服务器地址

  netinit();

  //初始化YVU转RGB的表，显示视频必须先初始化

  pv->m_nIndex = 0;//命令包序号
  pv->m_Author = FALSE; // 登陆设备权限验证通过标志
  pv->m_VideoClose = FALSE;// 关闭视频标志
  pv->m_exitThread = FALSE;

  pv->userdata = userdata;
  pv->m_eventfun  = eventfun;
  strcpy(pv->m_strDevIP, strDevIP);

  //创建套接字
  pv->m_sHost = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if (INVALID_SOCKET == pv->m_sHost) {
    printf("socket failed!\n");
    return 0;
  }

  //设置服务器地址
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = inet_addr(pv->m_strDevIP);
  servAddr.sin_port = htons((short)TCPPORT);
  nServAddlen = sizeof(servAddr);

  //连接服务器
  retVal = connect(pv->m_sHost, (SOCKADDR*)&servAddr, sizeof(servAddr));
  //retVal = _Connect(pv->m_sHost, (SOCKADDR*)&servAddr, sizeof(servAddr), 500);

  if (SOCKET_ERROR == retVal) {
    closesocket(pv->m_sHost); //关闭套接字
    pv->m_eventfun(pv, pv->m_strDevIP, NULL, DEV_CONNECT_FAILUE, 0, NULL, 0, OPER_ERR_EXECUT, pv->userdata);
    return 0;
  }

  pv->m_eventfun(pv, pv->m_strDevIP, NULL, DEV_CONNECT_SUCCESS, 0, NULL, 0, OPER_SUCCESS, pv->userdata);

  CPM_ULDevAuth(pv, sLoginName, sPassword, 1);
  //启动接收实时控制数据线程
  thread_create(pv->m_hTCPThread, (thread_cb)RecvRealControlDataThread, (void*)pv);
  return retVal;
}

const char* getmyip()
{
  struct hostent* pHost;
  char szHost[256];
  static char strClientIP[20];

  //取得本地主机名称
  if (gethostname(szHost, 256) == 0) {
    printf("host: %s\n", szHost);

    if ((pHost = gethostbyname(szHost)) != NULL) { //这些就是获得IP的函数
      char* ip = inet_ntoa(*(struct in_addr*)*pHost->h_addr_list);
      printf("IP: %s\n", ip);
      strcpy(strClientIP, ip);
    }
  }

  return strClientIP;
}

/***********************************************************************************/
/*功 能 启动视频
/*参 数 cszDevAddr 设备地址
hwnd 视频显示窗口句柄
/*说 明
/***********************************************************************************/
//typedef int (*frame_callback_t)(const char* cszIP, const unsigned char* cpbImage, int w, int h, int step, int nImageType, void* user);
int CPM_StartVideo(pmdev_t* pv, frame_fun_t framefun1, int cn, int flipud)
{
  const char* strClientIP = getmyip();

  if (!pv->m_Author) { //通过权限验证
    return FALSE;
  }

  pv->cn = cn;
  pv->flipud = flipud;

  pv->m_framefun  = framefun1;

  {
    //打开视频，包括启动视频数据接收线程
    //创建接收视频数据的SOKECT,已经启动接收视频数据包的线程。
    int nLen;
    //创建接收视频数据的SOKECT
    SOCKADDR_IN addrSrv;
    struct sockaddr_in addr = {0};
    pv->m_sVVIDEOSck = socket(AF_INET, SOCK_DGRAM, 0);
    //inet_ntoa(m_address.sin_addr);
    addrSrv.sin_addr.s_addr = inet_addr(strClientIP);
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_port = htons(0);//随机获取一个可用的UDP端口
    bind(pv->m_sVVIDEOSck, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));

    nLen = sizeof(struct sockaddr_in);

    if (SOCKET_ERROR == getsockname(pv->m_sVVIDEOSck, (struct sockaddr*)&addr, &nLen)) {
      return 0;
    }

    pv->m_nUDPPort = ntohs(addr.sin_port); //获得已经绑定的端口号
    //启动接收视频数据线程
    thread_create(pv->m_hUDPThread, (thread_cb)RecvVideoDataThread, (void*)pv);
  }

  {
    int retVal;
    SOCK_ADDRESS Addr[1] = {0};
    //向设备发起打开视频的命令
    //组命令数据
    strcpy(Addr->IP_Address, strClientIP);
    Addr->Port = pv->m_nUDPPort;
    retVal = send_cmd_packet(pv, pv->m_sHost, M_VIDEO_OUT, S_SET, 0, Addr, sizeof(SOCK_ADDRESS));
  }

  return TRUE;
}

//停止视频
int CPM_StopVideo(pmdev_t* pv)
{
  int retVal;
  const char* strClientIP = getmyip();
  SOCK_ADDRESS Addr[1] = {0};
  //关闭视频

  //组命令数据
  strcpy(Addr->IP_Address, strClientIP);
  Addr->Port = pv->m_nUDPPort;

  retVal = send_cmd_packet(pv, pv->m_sHost, M_VIDEO_OUT, S_GET, F_REQ_FLAG, Addr, sizeof(SOCK_ADDRESS));

  if (SOCKET_ERROR == retVal) {
    printf("send StopVideo failed!\n");
    closesocket(pv->m_sHost); //关闭套接字
    return FALSE;
  }

  //关闭视频UDP套接字
  if (pv->m_sVVIDEOSck) {
    closesocket(pv->m_sVVIDEOSck);
    pv->m_sVVIDEOSck = 0;
  }

  //关闭视频数据处理线程
  if (pv->m_hUDPThread) {
    pv->m_VideoClose = TRUE;

    thread_join(pv->m_hUDPThread, 5000);
    thread_detach(pv->m_hUDPThread);
  }

  return TRUE;
}

//断开连接
void CPM_DisConnect(pmdev_t* pv)
{
  //关闭和设备的连接
  if (pv->m_sHost) {
    closesocket(pv->m_sHost); //关闭套接字
    pv->m_sHost = 0;
  }

  //关闭实时控制数据线程
  if (pv->m_hTCPThread) {
    pv->m_exitThread = TRUE;

    thread_join(pv->m_hTCPThread, 5000);
    thread_detach(pv->m_hTCPThread);
  }

  pv->m_Author = FALSE;
}

#if 0
static DWORD WINAPI start_sdk_thd(pmdev_t* pv)
{
  pv->run = 1;

  for (; pv->run;) {
    sleep(1000);
  }

  CPM_StopVideo(pv);//关闭视频
  CPM_DisConnect(pv);//关闭连接
  netexit();//释放套接字资源
  return 0;
}

int start_sdk(pmdev_t* pv, thread_t* hthread, const char* strDevIP, const char* strClientIP, frame_fun_t framefun1, void* userdata, int cn, int flipud)
{
  CPM_Init(pv, strDevIP, strClientIP, framefun1, userdata, cn, flipud);
  CPM_Connect(pv);
  CPM_StartVideo(pv);
  thread_create(hthread, (thread_cb)start_sdk_thd, (void*)pv);
  return 0;
}
#endif