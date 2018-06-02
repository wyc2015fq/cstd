
#ifndef _DSP_DATASTRUCT_
#define _DSP_DATASTRUCT_

/*
1.关于MAC地址：MAC共6个字节 前3个字节表示厂商身份，后3个字节由厂商自定义
   BYTE[0-2] = 11-03-31 , 厂商身份代号 , 十六进制表示
   BYTE[3]     = 年份(00-99),十六进制表示
   BYTE[4-5] = 计数(short 型)，支持最大年产量 0xFFFF = 65535台

2 .SID所有计数采用16进制字符表示，例如: 11F0A4-2-20110604221310-FF，解析为
    SN = 11F0A4 , 表示11年第F0A4(61604)台
    TYPE = 2 , M_VERIFY
    DATETIME = 2011-06-04 22:13:10
    ID = FF , 表示数值255
*/

#ifdef __cplusplus
extern "C"
{
#endif

#define ID_LEN 24

  //说明: M表示主命，区间0x01 - 0x80 ，S表示子命令，区间0x81-0xff
#define M_USER        0x01          //用户
#define M_VERIFY      0x02          //验证记录
#define M_ALARM       0x03          //报警记录
#define M_OPTLOG      0x04          //日志记录
#define M_CIMAGE      0x05          //变更注册照记录
#define M_RANGE       0x06          //区间集合数据下载
#define M_AUTH        0x07          //客户端身份验证
#define M_TIME        0x08          //系统时间
#define M_HEART       0x09          //心跳包
#define M_DEVINFO     0x0a          //设备基本信息
#define M_STATIS      0x0b          //统计信息
#define M_DEVSET      0x0c          //设备基本设置
#define M_FORMAT      0x0d          //设备格式化
#define M_DOOR_STATE  0x0e          //门状态设置 ,共3种状态：0正常状态 ， 1常开状态， 2常闭状态
#define M_REMOTE_OPEN 0x0f          //远程开关门,
#define M_TGROUP      0x10          //时间组
#define M_DSPRIGHT    0x11          //权限
#define M_UGROUP      0x12          //用户组
#define M_MCAST       0x13          //多播地址
#define M_APPUPDATE   0x14          //程序更新
#define M_VIDEO_OUT   0x23          //UDP网络视频传输 NET_ADDRESS
#define M_USER_EX     0x24          //批量用户上传现在
#define M_REBOOT      0x25          //设备重启
#define M_RANGE_EX    0x26          //记录批量下载
#define M_SET_IP      0x27          //远程修改IP地址
#define M_NOPEN       0x28          //N+1开门请求
#define M_IOCTRL      0x29          //IO控制

  //子命令
#define S_SET             0x81      //上传\设置
#define S_DEL             0x82      //删除
#define S_GET             0x83      //下载\获得
#define S_REALTIME_SEND   0x84      //采集实时发送
#define S_REALTIME_LOAD   0x85      //实时加载
#define S_REALTIME_CAP    0x86      //实时用户采集
#define S_UPDATE_SEND     0x87      //更新实时发送

  //客户端与中转服务器主命令
#define C_CNDEV       0xB0          //client通过中转服务器设备连接
#define C_VERIFY      0xB1          //中转服务器验证client身份
#define C_HEART       0xB2          //中转服务器与client的心跳包
  //客户端与中转服务器子命令
#define S_CN          0xc0          //建立连接
#define S_DN          0xc1          //断开连接

  //命令执行错误状态
#define SUC_EXECUT      0x00       //成功
#define ERR_EXECUT      0x0A       //失败
#define ERR_BUSY        0x01       //设备忙
#define ERR_LIMIT       0x02       //已达上限
#define ERR_NOFIND      0x03       //没有找到对应数据
#define ERR_SAVE_FAIL   0x04       //数据保存失败
#define ERR_SET_FAIL    0x05       //设置失败
#define ERR_VERIFY_FAIL 0x06       //验证失败
#define ERR_FROMAT      0x07       //格式化失败
#define ERR_PARAM       0x08       //参数错误
#define ERR_DISABLE     0x09       //要求执行的功能没有使能
#define ERR_SUPPORT     0x10       //不支持的命令
#define ERR_INPUTDATA   0x11       //网络端传输的数据有异常

  //设置和获得错误状态
#define SET_ERR( state , err ) (state) = ( (state) & 0xffffff00 )+(err)
#define GET_ERR( state ) ( (state) & 0xff )

#define CMD_DOWN_ID              0x80000000   //下载数据ID集合,优先检测该标记
#define CMD_ID_ALL                    0x00000100   //下载所有数据的区间集合
#define CMD_ID_TIME_RANGE 0x00000200   //下载制定时间段区间集合
#define CMD_ID_RANGE             0x00000400   //确定指定区间的区间集合

  //用户验证
#define AUTH_VERIFY       0x00000100   //用户身份验证

  //门状态: 开，关，不指定为正常状态
#define DOOR_OPEN        0x00000100   //开
#define DOOR_CLOSE      0x00000200   //关

  //数据下载
#define DL_IMAGE             0x00000100   //需要下载照片
#define DL_FEAT                0x00000200   //需要下载特征
#define DL_APPENDID      0x00000400   //实时加载指示CID是编号还是卡号

  //时间组下载
#define DL_NORMAL_TG     0x00000100   //下载正常时间组
#define DL_HOLIDAY_TG     0x00000200   //下载节假日

  //命令结构体，所有上传下载的网络数据,通过此数据结构来辨别数据类型
  typedef struct {
    unsigned int m_MCmd;           //主命令
    unsigned int m_SCmd;           //子命令
    unsigned int m_CFlag;            //特定命令配置参数，最后一个字节为命令执行状态字
    unsigned int m_Param1;         //用户自定义参数1
    unsigned int m_Param2;         //用户自定义参数2
  } NCMD;

  //日期
  typedef struct {
    int   m_Year;
    char  m_Month;
    char  m_Day;
    char  m_Reserve[2];   //结构体4字节对齐
  } NETDATE;

  //时间
  typedef struct {
    int   m_Msec;         //毫秒
    char  m_Hour;
    char  m_Minute;
    char  m_Second;
    char  m_Reserve[1];
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
    NETDATE m_Start;  //记录起始时间
    NETDATE m_End;    //记录结束时间
  } TIMESE;

#define DL_OPENINTERVAL 0x00000001
  //下载指定记录流水号之后的N条记录，包括指定记录本身
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
    int  m_Count;          //数组元素的个数
    char m_Block[4];      //数组地址, 4是为了结构体的字节对齐
  } CARRAY;

#define CHECK_TIME   0x01     //检测时间
#define CHECK_WEEK   0x02     //检测星期
#define CHECK_DAY    0x04     //检测日期
#define CHECK_MONTH  0x08     //检测月份
#define CHECK_YEAR   0x10     //检测年份

#define WEEK_1       0x01     //星期一有效
#define WEEK_2       0x02     //星期二有效
#define WEEK_3       0x04     //星期三有效
#define WEEK_4       0x08     //星期四有效
#define WEEK_5       0x10     //星期五有效
#define WEEK_6       0x20     //星期六有效
#define WEEK_7       0x40     //星期天有效

  //任意时间有效 , 正常时间组，节假日时间组
  enum { ANY_TIME = -1 , TG_NORMAL = 1, TG_HOLIDY = 2 };

  //时间组
  typedef struct {
    int      m_TGID;      //时间组ID
    DATETIME m_Start;     //时间组开始时间
    DATETIME m_End;       //时间组结束时间
    char     m_TGType;    //时间组类型, TG_NORMAL,TG_HOLIDY
    char     m_CheckFlag; //时间检测标记例：m_CheckFlag = CHECK_TIME|CHECK_WEEK
    char     m_WeekFlag;  //检测星期时，标记那些星期有效。例：m_WeekFlag = WEEK_1|WEEK_5
    char     m_Reserve;   //保留字节
  } DSPTIMEG;

  //没有指定用户权限,用户默认权限ID为DEFAULT_NO_RIGHT或者DEFAULT_SINGLE_OPEN,DSP上采集的用户，
  //超级管理员可以统一配置为DSP_DEFAULT_SINGLE_OPEN
  //默认无权限，默认单一开门权限，组合用户最大组用户数，权限支持最大时间组
  //DEFAULT_NO_RIGHT: DSP,管理端新增用户,默认值之一无权限
  //DEFAULT_SINGLE_OPEN: DSP,管理端新增用户，默认值之二单一开门权限，此权限没有时间组概念
  enum { DEFAULT_NO_RIGHT = -2, DEFAULT_SINGLE_OPEN = -1, MAX_GROUPUSER = 6, TGROUP_NUM = 8 };


#define CHECK_HOLIDY    0x01     //权限检测节假日
#define ACTION_LOCK       0x02      //电锁输出
#define ACTION_OUTPUT  0x04     //辅助输出
#define ACTION_N_1           0x08     //该权限需要N+1
  //权限
  typedef struct {
    //权限ID
    int m_RightID;

    //时间组ID , (m_TimeGID[0]==ANY_TIME)未指定时间组，开门方式不受时间限制，任意时间段验证成功执行开门方式.
    int m_TimeGID[TGROUP_NUM];

    //BIT(0):1 检测节假日，0不检测节假日
    int m_Mode;
  } DSPRIGHT;

  //组合有序
#define G_ORDER 0x01
  //DEFAULT_NO_GROUP表示用户不参与任何组
  enum { DEFAULT_NO_GROUP = -1 };
  //用户组合
  //有序组合开门: 同一组用户必须有序验证，必须先验证完强制用户，在验证普通用户
  //无序组合开门: 不管以何种顺序验证，只要最后验证总结果打到组合验证的要求，验证通过.
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
    CID  m_NormalUsers[MAX_GROUPUSER];
    //强制用户组合，优先级高
    CID  m_ForceUsers[MAX_GROUPUSER];
  } USERGROUP;


  typedef struct {
    int         m_UserType;                  //终端设备上的用户类型(普通，管理，超管 -- 0 , 1 , 2)
    CID      m_UserID;                       //用户ID
    DATETIME m_TypeUpdTime;  //用户类型更改时间
    DATETIME m_RegistTime;       //人脸注册时间，即特征采集时间，采集或者重新采集要更新此时间
    DATETIME m_LastUpdTime;    //最后更新的时间，识别时特征发生更新或者重新采集要更新此时间
  } VUSER;

  //用户信息
#define USER_CARD_INVALID 0x01   //卡挂失
#define USER_DLine_VALID      0x02   //失效时间有效
#define USER_BLACK                 0x04   //黑名单
#define USER_MIX1N                  0x08   //混合模式下1N用户
  typedef struct {
    int     m_FeatBytes;         //人脸特征字节数，==0 表示没有人脸特征
    int     m_PicBytes;          //注册图像的字节数，==0 表示没有图片
    int     m_RightID;           //用户权限ID,三种可能值(DEFAULT_NO_RIGHT,DEFAULT_SINGLE_OPEN,用户指定)
    CID  m_ImageSID;      //上传注册照，对应的变更ID，全局唯一
    CID  m_UserID;            //用户标识
    CID  m_Admin;             //人脸注册管理员,标识此用户的人脸特征是哪个管理员采集
    CID  m_AppendID;          //卡号或者其他用于1:1的附加身份确认信息
    CID  m_UserName;          //用户名
    CID  m_Department;         //部门名称
    DATETIME m_DeadLine;      //失效时间
    DATETIME m_RegistTime;      //人脸注册时间，即特征采集时间，采集或者重新采集要更新此时间
    DATETIME m_LastUpdTime;   //最后更新的时间，识别时特征发生更新或者重新采集要更新此时间
    DATETIME m_TypeUpdTime;  //用户类型变更时间
    char m_UserFlag;                       //用户状态标记
    char m_UserType;          //终端设备上的用户类型(普通，管理，超管 -- 0 , 1 , 2)
    char m_Block[2];           //注册图片,必须JPG,人脸特征是与m_UserID唯一绑定的.m_Block的前m_FeatBytes个字节表示人脸特征, 后m_PicBytes个字节表示图片.
  } USER;

  //记录类型:
  //0x01 - 0x20: 正常事件
  //0x20 - 0x40: 硬件报警事件
  //0x40 - 0x50: 辅助输入
  //0x50 - 0x60: 辅助输出
  //0xf0 - 0xff: 软件报警事件
  //其他: 未定义
  enum {
    VERIFY_SUCC = 0x01,          //验证成功
    VERIFY_FAIL    = 0x02,          //验证失败
    ADMIN_SUCC  = 0x03,           //管理员验证成功
    EMER_OPEN   = 0x04,            //紧急开门
    RIGHT_OPEN   = 0x05,           //权限开门
    GROUP_OPEN = 0x06,           //组合开门
    BUTTON_OPEN = 0x07,         //按钮开门

    ALARM_HD_MANGET_TIMEOUT = 0x20,     //门磁超时
    ALARM_HD_MANGET_ILLOPEN = 0x21,      //门磁非法开门
    ALARM_HD_OFFLINE  = 0x22,                         //前端掉线报警

    ALARM_HD_BREAK    = 0x30,       //防拆报警
    ALARM_HD_SHOCK    =  0x31,      //震动报警
    ALARM_HD_FPOWR    = 0x36,      //前端供电异常报警
    ALARM_HD_UPS_ON  = 0x37,      //UPS备用电池开启
    ALARM_HD_UPS_OFF = 0x38,     //UPS备用电池关闭

    ALARM_HD_ASSIST = 0x40,      //辅助输入触发

    ALARM_HD_WL_DOOR_MAGNETIC = 0x41, //无线
    ALARM_HD_WL_SMOKE = 0x42,
    ALARM_HD_WL_GAS = 0x43,
    ALARM_HD_WL_BUTTON1 = 0x44,
    ALARM_HD_WL_BUTTON2 = 0x45,
    ALARM_HD_WL_BUTTON3 = 0x46,
    ALARM_HD_WL_BUTTON4 = 0x47,
    ALARM_HD_WL_AREA1      = 0x48,
    ALARM_HD_WL_AREA2      = 0x49,
    ALARM_HD_WL_AREA3      = 0x4A,
    ALARM_HD_WL_AREA4      = 0x4B,
    ALARM_HD_WL_AREA5      = 0x4C,
    ALARM_HD_WL_AREA6      = 0x4D,
    ALARM_HD_WL_AREA7      = 0x4E,

    ALARM_SF_BACKLIST = 0xF0,    //黑名单验证报警
    ALARM_SF_ILLCARD = 0xF1,     //无效卡(挂失)
    ALARM_SF_ILLTIME = 0xF2,     //非法时间识别
    ALARM_SF_DEADLINE = 0xF3,    //失效时间
    ALARM_SF_DANGER_OPEN = 0xF4, //胁迫开门
    ALARM_SF_SUPER_OPEN = 0xF5,  //超级密码开门
    LINK_EVENT = 0xF6,  //联动事件
  };

  //记录信息,识别记录，报警记录
  typedef struct {
    int  m_RecType;               //记录类型
    int  m_Score;                    //识别得分 , 联动事件为联动事件的LINKID
    int  m_PicBytes;               //记录图像的字节数，==0 表示没有记录图片
    CID  m_ChangID;             //注册照记录号
    CID  m_UserID;                //用户ID,如果为空，表示非法记录 , 联动事件为事件描述Text
    CID  m_SerialID;              //识别记录流水号ID
    CID  m_AppendID;           //卡号或者其他用于1:1的附加身份确认信息
    CID  m_UserName;          //用户名
    CID  m_Department;        //部门名称
    DATETIME m_RecTime;  //记录时间
    char m_VerifyMode;          //验证模式,例:VERIFY_CARD|VERIFY_FACE_1N,刷卡+人脸
    char m_Source;                //事件源设备的485地址
    char m_Block[2];               //记录图像,必须JPG , 2是为了结构体的字节对齐
  } RECORD;

  //操作日志类型
  enum {
    DSPLOG_ADDUSER = 0x01,      //增加用户
    DSPLOG_DELUSER = 0x02,       //删除用户
    DSPLOG_REREGIST = 0x03,       //重新采集
    DSPLOG_CAHNGETYPE = 0x04,     //改变用户类型
    DSPLOG_UDISK_ULUSER = 0x05,   //U盘上传用户
    DSPLOG_UDISK_DLUSER = 0x06,   //U盘下载用户
    DSPLOG_UDISK_DLRECORD = 0x07 ,//U盘下载记录
    DSPLOG_UDISK_UPDATEAPP = 0x08, //U盘更新程序
  };

  //设备操作日志
  typedef struct {
    int m_LogType;      //日志类型
    CID m_SerialID;     //流水号ID
    CID m_Admin;        //操作员
    CID m_BeOptUser;    //被操作员
    DATETIME m_RecTime; //记录时间
  } DSPLOG;

  //变更注册照
  typedef struct {
    int  m_PicBytes;              //注册照大小
    CID  m_UserID;              //用户ID
    CID  m_SerialID;              //流水号ID
    CID  m_AppendID;          //卡号或者其他用于1:1的附加身份确认信息
    CID  m_UserName;         //用户名
    CID  m_Department;        //部门名称
    CID  m_Admin;                 //人脸注册管理员,标识此用户的人脸特征是哪个管理员采集
    DATETIME m_RecTime;  //记录时间
    char m_Block[4];               //注册照内存
  } CHANGEIMAGE;

  //DSP相关统计信息
  typedef struct {
    int  m_TotalUsers;                   //当前设备的总用户数
    int  m_NoFeatUser;                //没有采集人脸特征的用户数
    int  m_TotalDays;                     //识别记录保存总天数
    int  m_TotalRecords;               //总记录数
    int  m_TotalAlarm;                    //总报警记录数
    int  m_TotalDspLog;                //总操作日志数目
    int  m_TotalChangeImage;     //总变更注册照
  } DEVSTATIS;

  //验证信息
  typedef struct {
    CID m_UserName;      //用户名
    CID m_Password;        //密码
  } AUTH;

  //设备基本信息
  typedef struct {
    int m_DevType;        //设备类型
    int m_LimitUser;       //总人数上限
    int m_Limit1NUser;  //1N用户上限
    CID m_SN;                //设备编号
    CID m_Ver;                //DSP软件版本
    CID m_Space;           //磁盘容量信息
  } DEVINFO;

  //2.0版支持的功能
#define RECORD_SAVEFAIL       0x00000001    //是否保存识别失败记录
#define SUPER_PASSWORD      0x00000002     //超级密码开门有效
#define HDBEEP_OPEN               0x00000004    //撤防/布防(硬件报警蜂鸣器开启)
#define REALTIME_RECORD       0x00000010    //实时动态记录显示
#define REALTIME_USERLOAD  0x00000020    //输入编号或者卡号识别时，如果无法在当前dsp中获得人脸特征,在网络连接状态下，将从管理端实时加载人脸特征
#define REALTIME_USERSEND  0x00000040    //采集用户同步
#define DOORMANGET_OPEN    0x00000080    //开启门磁检测报警
#define DOORFORCE_OPEN       0x00000100    //开启胁迫开门功能
#define REMOTE_CAP_SAVE      0x00000200    //远程控制采集的用户是否保存到DSP
#define GPRS_OPEN                    0x00000400    //开启GPRS模块
#define UPDATE_USERSEND     0x00000800    //特征更新时，是否实时发送用户特征
#define REALTIME_USERLOAD_FORCE 0x00001000  //网络连接情况下，所有用户强制远程加载
#define AB_LOCK                           0x00002000    //AB互锁
#define DOOR1_NOPEN                0x00004000    //门一N+1
#define DOOR2_NOPEN                0x00008000    //门二N+1

  //验证模式优先级低-高:UID<CARD<11<MIX<1N
#define VERIFY_USERID   0x01    //编号
#define VERIFY_CARD       0x02    //刷卡
#define VERIFY_FACE_11  0x04    //11人脸加载
#define VERIFY_FACE_MIX 0x08    //混合人脸加载
#define VERIFY_FACE_1N  0x10    //1N人脸加载
  //#define REALTIME_USERLOAD  0x00000020 , 也作为一种验证模式看待

#define OUTPUT_WG_YES   0x01    //输出伟根协议
#define OUTPUT_WG_34    0x02    //指定输出伟根34，不指定输出伟根26
#define OUTPUT_WG_CTX   0x04    //伟根输出指定内容
#define OUTPUT_WG_UID   0x08    //不标记OUTPUT_WG_CTX，输出用户ID，默认输出卡号

  //设备工作属性设置
  typedef struct {
    //设备时间组,(m_TimeGID[0]==ANY_TIME)未指定时间组,设备任意时间有效.设备时间组优先级高于用户权限时间组
    int  m_TimeGID[TGROUP_NUM];
    //设备默认权限: DEFAULT_NO_RIGHT , DEFAULT_SINGLE_OPEN
    int  m_DefaultRight;
    //参见2.0版本定义的标记
    int m_BaseSet;
    //超级密码, 将产生一条报警记录，并且会抓拍彩色全景图.
    CID  m_SuperPassword;
    //门磁延时时间，单位秒
    char m_DoorMangetTime;
    //电锁持续时间，单位秒
    char m_LockTime;
    //用户身份验证模式,例如:VERIFY_USERID|VERIFY_CARD|VERIFY_FACE_11
    char m_VerifyMode;
    //BIT(0):0不输出扩展韦根信号,1输出.
    //BIT(1):0输出韦根26,1输出韦根34.
    //BIT(2):0按照BIT(4)输出 , 1输出m_OutputCtx中指定内容
    //BIT(4):0直接输出卡号,1输出编号
    char m_OutputWG;
    //韦根协议输出内容
    int m_OutputCtx;
  } DEVSET;

  //程序更新结构体
  typedef struct {
    int  m_Bytes;           //文件大小
    CID  m_FileName;    //文件名
    char m_TotalFiles;     //需要更新的文件总数
    char m_FileIndex;     //当前更新到第几个
    char m_Block[2];       //文件数据
  } APPUPDATE;

  //IO设备状态
#define IO_MODE_NORMAL   0x00   //正常状态
#define IO_MODE_OPEN        0x01   //常开状态
#define IO_MODE_CLOSE      0x02   //常关状态
  //门磁状态
#define IO_STATE_CLOSE      0x00   //门磁关
#define IO_STATE_OPEN        0x01   //门磁开
  //执行动作
#define ACT_IO_OPEN       0x02   //执行打开动作
#define ACT_IO_OPENEX  0x04   //执行辅助动作
#define ACT_IO_CLOSE     0x10   //执行关闭动作
#define ACT_MODE_SET   0x20   //设置IO设备工作模式
#define ACT_MODE_GET   0x40   //获得IO设备工作模式
#define ACT_STATE_GET   0x80   //获得IO设备当前状态

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
    int          m_CtxID;
    //验证用户数目： 0密码开门，1权限开门，>1组合开门
    int          m_Users;
    //控制信息
    IOCTRL m_XOpen;
    //验证用户数组
    CID        m_IDS[MAX_GROUPUSER * 2];
  } NOPEN;

  //关于485地址解析:
  //1.485地址用一个字节表示，[4-7]表示设备索引号1-15 , [0-3]表示支持16种不同型的设备
  //    已经定义的设备类型 0--人脸前端 , 1--读卡器 ,
  //    例如: 0x11 表示1号门人脸识别前端1，INDEX[4-7] == 1  TYPE[0-3] == 1
  //              0x10 表示1号门人脸识别前端0，INDEX[4-7] == 1  TYPE[0-3] == 0
  //              0x12 表示1号门刷卡器0                INDEX[4-7] == 1  TYPE[0-3] == 2
  //              0x13 表示1号门刷卡器1                INDEX[4-7] == 1  TYPE[0-3] == 3
  //              0x1F 表示门点本身                        INDEX[4-7] == 1  TYPE[0-3] == F
  //              0x14 表示辅助输入1                      INDEX[4-7] == 1  TYPE[0-3] == 4
  //              0x25 表示辅助输出2                      INDEX[4-7] == 2  TYPE[0-3] == 5
  //2. 所有设备的序号从1开始。此协议可支持15张门，16中不同类型的门点输入设备，15个辅助输入和15个辅助输出
  //3. [0-7] == 0x00 表示非法地址，[0-7] ＝＝ 0x01表示后端板本身
  //7_______3______0
  //|_INDEX_|__type__|

#define DEV_TYPE_FACE1    0x0      //人脸前端1
#define DEV_TYPE_FACE2    0x1      //人脸前端2
#define DEV_TYPE_CARD1   0x2      //刷卡器1
#define DEV_TYPE_CARD2   0x3      //刷卡器2
#define DEV_TYPE_IN            0x4      //辅助输入
#define DEV_TYPE_OUT        0x5      //辅助输出
#define DEV_TYPE_DOOR     0xF      //门点本身

  //GET/SET 0－3位数据
#define GET_TYPE_485( x ) ( (x)&0x0F )
#define SET_TYPE_485( x , v ) ( (x) = ( ((x)&0xF0) + (v) )
  //GET/SET 获得4－7位数据
#define GET_INDEX_485( x ) ( ( (x)&0xF0 ) >> 4 )
#define SET_INDEX_485( x , v ) ( (x) = ( ((x)&0x0F) + ((v)<<4)) )

#define IMAGE_UYVY   0x01
#define IMAGE_YUYV   0x02
#define IMAGE_RGB24  0x03
  typedef struct {
    //图像数据总大小BYTES
    int     m_Size;
    //图像宽
    int     m_W;
    //图像高
    int     m_H;
    //行数据步长
    int     m_Step;
    //图像类型
    char m_Type;
    //图像数据
    char m_Block[3];
  } DEVIMAGE;

#define ALIGN4( size ) ( ((size)+3) & ~3 )
#define USER_STEP( obj ) ALIGN4( sizeof(USER)+ (obj).m_FeatBytes+(obj).m_PicBytes )
#define RECORD_STEP( obj ) ALIGN4( sizeof(RECORD)+(obj).m_PicBytes-2 )
#define CGIMG_STEP( obj )  ALIGN4( sizeof(CHANGEIMAGE)+(obj).m_PicBytes-4 )
#define DSPLOG_STEP( obj ) ALIGN4( sizeof(DSPLOG) )

#define USER_RIMAGE        0x00000100   //用户注册照
#define USER_FEAT          0x00000200   //用户特征
#define USER_TEXT          0x00000400   //用户文字信息
#define BATCH_END          0x00000800   //批量结束


#ifdef __cplusplus
}
#endif

#endif
