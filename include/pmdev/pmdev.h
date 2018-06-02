#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define ASSERT assert
enum {
  TRUE = 1,
  FALSE = 0,
  TCPPORT = 13333, // 服务器端口
  //命令
  M_USER = 0x01, // 用户
  M_VERIFY = 0x02, // 验证记录
  M_ALARM = 0x03, // 报警记录
  M_OPTLOG = 0x04, // 日志记录
  M_CIMAGE = 0x05, // 变更注册照记录
  M_RANGE = 0x06, // 区间集合数据下载
  M_AUTH = 0x07, // 客户端身份验证
  M_TIME = 0x08, // 系统时间
  M_HEART = 0x09, // 心跳包
  M_DEVINFO = 0x0a, // 设备基本信息
  M_STATIS = 0x0b, // 统计信息
  M_DEVSET = 0x0c, // 设备基本设置
  M_FORMAT = 0x0d, // 设备格式化
  M_DOOR_STATE = 0x0e, // 门状态设置 ,共3种状态：0正常状态 ， 1常开状态，
  // 2常闭状态
  M_REMOTE_OPEN = 0x0f, // 远程开关门,
  M_TGROUP = 0x10, // 时间组
  M_DSPRIGHT = 0x11, // 权限
  M_UGROUP = 0x12, // 用户组
  M_MCAST = 0x13, // 多播地址
  M_APPUPDATE = 0x14, // 程序更新
  M_VIDEO_OUT = 0x23, // UDP网络视频传输 NET_ADDRESS
  M_USER_EX = 0x24, // 批量用户上传现在
  M_REBOOT = 0x25, // 设备重启
  M_RANGE_EX = 0x26, // 记录批量下载
  M_SET_IP = 0x27, // 远程修改IP地址
  M_NOPEN = 0x28, // N+1开门请求
  M_IOCTRL = 0x29, // IO控制
  M_NETPRO = 0x30, // 网络设置
  S_SET = 0x81, // 上传\设置
  S_DEL = 0x82, // 删除
  S_GET = 0x83, // 下载\获得
  S_REALTIME_SEND = 0x84, // 采集实时发送
  S_REALTIME_LOAD = 0x85, // 实时加载
  S_REALTIME_CAP = 0x86, // 实时用户采集
  S_REALTIME_CAP_INFO = 0x88, // 远程采集实是进度信息
  S_UPDATE_SEND = 0x87, // 更新实时发送
  S_CLEAR_INOUT = 0x88, // 清空统计计数
  // 客户端与中转服务器主命令
  C_CNDEV = 0xB0, // client通过中转服务器设备连接
  C_VERIFY = 0xB1, // 中转服务器验证client身份
  C_HEART = 0xB2, // 中转服务器与client的心跳包
  // 客户端与中转服务器子命令
  S_CN = 0xc0, // 建立连接
  S_DN = 0xc1, // 断开连接
  // 命令执行错误状态
  SUC_EXECUT = 0x00, // 成功
  ERR_EXECUT = 0x0A, // 失败
  ERR_BUSY = 0x01, // 设备忙
  ERR_LIMIT = 0x02, // 已达上限
  ERR_NOFIND = 0x03, // 没有找到对应数据
  ERR_SAVE_FAIL = 0x04, // 数据保存失败
  ERR_SET_FAIL = 0x05, // 设置失败
  ERR_VERIFY_FAIL = 0x06, // 验证失败
  ERR_FROMAT = 0x07, // 格式化失败
  ERR_PARAM = 0x08, // 参数错误
  ERR_DISABLE = 0x09, // 要求执行的功能没有使能
  ERR_SUPPORT = 0x10, // 不支持的命令
  ERR_INPUTDATA = 0x11, // 网络端传输的数据有异常
  CMD_STATE_REPLY = 0x40000000, // 标记该命令为状态回复命令
  CMD_DOWN_ID = 0x80000000, // 下载数据ID集合,优先检测该标记
  CMD_ID_ALL = 0x00000100, // 下载所有数据的区间集合
  CMD_ID_TIME_RANGE = 0x00000200, // 下载制定时间段区间集合
  CMD_ID_RANGE = 0x00000400, // 确定指定区间的区间集合
  // 数据下载
  DL_IMAGE = 0x00000100, // 需要下载照片
  DL_FEAT = 0x00000200, // 需要下载特征
  DL_APPENDID = 0x00000400, // 实时加载指示CID是编号还是卡号
  LD_ENROLL = 0x00000800, // 需要远程采集
  USER_TEXT = 0x00000400, // 用户文字信息
  BATCH_END = 0x00000800, // 批量结束
  // 用户验证
  AUTH_VERIFY = 0x00000100, // 用户身份验证
  // 门状态: 开，关，不指定为正常状态
  DOOR_OPEN = 0x00000100, // 开
  DOOR_CLOSE = 0x00000200, // 关
  // 时间组下载
  DL_NORMAL_TG = 0x00000100, // 下载正常时间组
  DL_HOLIDAY_TG = 0x00000200, // 下载节假日
  // 组合有序
  // 用户组合
  // 有序组合开门: 同一组用户必须有序验证，必须先验证完强制用户，在验证普通用户
  // 无序组合开门: 不管以何种顺序验证，只要最后验证总结果打到组合验证的要求，验证通过.
  G_ORDER = 0x01,
  CMD_M_ID = 0xA1,
  CMD_M_AUTH = M_AUTH,
  CMD_M_HEART = M_HEART,
  CMD_M_DEVINFO = M_DEVINFO,
  CMD_M_STATIS = M_STATIS,
  CMD_M_DEVSET = M_DEVSET,
  CMD_M_TIME = M_TIME,
  CMD_M_FORMAT = M_FORMAT,
  CMD_M_EOPEN = M_DOOR_STATE,
  CMD_M_REMOTE_OPEN = M_REMOTE_OPEN,
  CMD_M_NOPEN = M_NOPEN,
  CMD_M_USER = M_USER,
  CMD_M_VERIFY = M_VERIFY,
  CMD_M_ALARM = M_ALARM,
  CMD_M_OPTLOG = M_OPTLOG,
  CMD_M_CIMAGE = M_CIMAGE,
  CMD_M_RANGE = M_RANGE,
  CMD_M_TGROUP = M_TGROUP,
  CMD_M_DSPRIGHT = M_DSPRIGHT,
  CMD_M_UGROUP = M_UGROUP,
  CMD_M_MCAST = M_MCAST,
  CMD_M_APPUPDATE = M_APPUPDATE,
  CMD_M_VIDEO_OUT = M_VIDEO_OUT,
  CMD_M_USER_EX = M_USER_EX,
  CMD_M_RECORD_EX = M_RANGE_EX,
  CMD_M_REBOOT = M_REBOOT,
  CMD_M_IOCTRL = M_IOCTRL,
  CMD_M_NETPRO = M_NETPRO,
  CMD_S_SET = S_SET, // 上传\设置
  CMD_S_DEL = S_DEL, // 删除
  CMD_S_GET = S_GET, // 下载\获得
  CMD_S_REALTIME_SEND = S_REALTIME_SEND, // 采集实时发送
  CMD_S_REALTIME_LOAD = S_REALTIME_LOAD, // 实时加载
  CMD_S_REALTIME_CAP = S_REALTIME_CAP, // 实时用户采集
  CMD_S_REALTIME_CAP_INFO = S_REALTIME_CAP_INFO, // 远程采集实是进度信息
  CMD_S_UPDATE_SEND = S_UPDATE_SEND, // 更新实时发送
  CMD_S_CLEAR_INOUT = S_CLEAR_INOUT, // 清空统计计数
  // 状态标记位
  F_ERR_BUSY = ERR_BUSY,
  F_ERR_LIMIT = ERR_LIMIT,
  F_ERR_NOFIND = ERR_NOFIND,
  F_ERR_SAVE_FAIL = ERR_SAVE_FAIL,
  F_ERR_SET_FAIL = ERR_SET_FAIL,
  F_ERR_VERIFY_FAIL = ERR_VERIFY_FAIL,
  // 其他标记位
  F_DL_ID = CMD_DOWN_ID, // 下载ID集合
  F_DL_ID_ALL = CMD_ID_ALL, // 下载所有数据的ID集合
  F_ID_TIME_RANGE = CMD_ID_TIME_RANGE, // 下载制定时间段ID集合
  F_ID_RANGE = F_ID_TIME_RANGE << 1,
  F_AUTH_VERIFY = AUTH_VERIFY, // 用户身份验证
  F_DL_PIC = DL_IMAGE, // 需要下载照片
  F_DL_FEAT = DL_FEAT, // 需要下载人脸特征
  F_DL_APPENDID = DL_APPENDID, // 远程加在时，m_ID表示卡号
  F_DL_NORMAL_TG = DL_NORMAL_TG, // 下载正常时间组
  F_DL_HOLIDAY_TG = DL_HOLIDAY_TG, // 下载节假日
  F_DOOR_CLOSE = DOOR_CLOSE, // 紧急关门，否则为紧急开门
  F_DOOR_OPEN = DOOR_OPEN, // 长开或者长关状态取消
  F_G_ORDER = G_ORDER, // 组合开门有序
  //  F_DEFAULT_NO_GROUP = DEFAULT_NO_GROUP,
  // 没有指定用户权限,用户默认权限ID为DEFAULT_NO_RIGHT或者DEFAULT_SINGLE_OPEN,DSP上采集的用户，
  // 超级管理员可以统一配置为DSP_DEFAULT_SINGLE_OPEN
  // 默认无权限，默认单一开门权限，组合用户最大组用户数，权限支持最大时间组
  // DEFAULT_NO_RIGHT: DSP,管理端新增用户,默认值之一无权限
  // DEFAULT_SINGLE_OPEN: DSP,管理端新增用户，默认值之二单一开门权限，此权限没有时间组概念
  DEFAULT_NO_RIGHT = -2,
  DEFAULT_SINGLE_OPEN = -1,
  MAX_GROUPUSER = 60,
  TGROUP_NUM = 8,
  TIMEGROUP_NUMS = TGROUP_NUM, // 时间组个数
  USER_COMBINS = MAX_GROUPUSER, // 用户组合个数
  DL_OPENINTERVAL = 0x00000001, // 下载指定记录流水号之后的N条记录，包括指定记录本身
  WEB_LOADUSER = 0x0001, // 加载用户
  WEB_WRITEUSER = 0x0002, // 写用户
  WEB_WRITEREC = 0x0004, // 写记录
  WEB_SYNCTIME = 0x0008, // 同步时间
  CHECK_TIME = 0x01, // 检测时间
  CHECK_WEEK = 0x02, // 检测星期
  CHECK_DAY = 0x04, // 检测日期
  CHECK_MONTH = 0x08, // 检测月份
  CHECK_YEAR = 0x10, // 检测年份
  // 星期
  WEEK_1 = 0x01, // 星期一有效
  WEEK_2 = 0x02, // 星期二有效
  WEEK_3 = 0x04, // 星期三有效
  WEEK_4 = 0x08, // 星期四有效
  WEEK_5 = 0x10, // 星期五有效
  WEEK_6 = 0x20, // 星期六有效
  WEEK_7 = 0x40, // 星期天有效
  // 任意时间有效 , 正常时间组，节假日时间组
  ANY_TIME = -1,
  TG_NORMAL = 1,
  TG_HOLIDY = 2,
  // 时间组
  CHECK_HOLIDY = 0x01, // 权限检测节假日
  ACTION_LOCK = 0x02, // 电锁输出
  ACTION_OUTPUT = 0x04, // 辅助输出
  ACTION_N_1 = 0x08, // 该权限需要N+1
  // 表示用户不参与任何组
  INVALID_GROUPID = -1,
  // 用户信息
  USER_CARD_INVALID = 0x01, // 卡挂失
  USER_DLine_VALID = 0x02, // 失效时间有效
  USER_BLACK = 0x04, // 黑名单
  USER_MIX1N = 0x08, // 1N人脸认证
  USER_VIPCARD = 0x10, // VIP用户
  USER_FINGER_PRINT = 0x20, // 指纹认证
  USER_SYNC_FINGEER = 0x40, // 指纹同步
  // 记录类型:
  // 0x01 - 0x20: 正常事件
  // 0x20 - 0x40: 硬件报警事件
  // 0x40 - 0x50: 辅助输入
  // 0x50 - 0x60: 辅助输出
  // 0xf0 - 0xff: 软件报警事件
  // 其他: 未定义
  VERIFY_SUCC = 0x01, // 验证成功
  VERIFY_FAIL = 0x02, // 验证失败
  ADMIN_SUCC = 0x03, // 管理员验证成功
  EMER_OPEN = 0x04, // 紧急开门
  RIGHT_OPEN = 0x05, // 权限开门
  GROUP_OPEN = 0x06, // 组合开门
  BUTTON_OPEN = 0x07, // 按钮开门
  ALARM_HD_MANGET_TIMEOUT = 0x20, // 门磁超时
  ALARM_HD_MANGET_ILLOPEN = 0x21, // 门磁非法开门
  ALARM_HD_OFFLINE = 0x22, // 前端掉线报警
  ALARM_HD_BREAK = 0x30, // 防拆报警
  ALARM_HD_SHOCK = 0x31, // 震动报警
  ALARM_HD_FPOWR = 0x36, // 前端供电异常报警
  ALARM_HD_UPS_ON = 0x37, // UPS备用电池开启
  ALARM_HD_UPS_OFF = 0x38, // UPS备用电池关闭
  ALARM_HD_ASSIST = 0x40, // 辅助输入触发
  ALARM_HD_WL_DOOR_MAGNETIC = 0x41, // 无线
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
  ALARM_SF_BACKLIST = 0xF0, // 黑名单验证报警
  ALARM_SF_ILLCARD = 0xF1, // 无效卡(挂失)
  ALARM_SF_ILLTIME = 0xF2, // 非法时间识别
  ALARM_SF_DEADLINE = 0xF3, // 失效时间
  ALARM_SF_DANGER_OPEN = 0xF4, // 胁迫开门
  ALARM_SF_SUPER_OPEN = 0xF5, // 超级密码开门
  LINK_EVENT = 0xF6, // 联动事件
  // 操作日志类型
  DSPLOG_ADDUSER = 0x01, // 增加用户
  DSPLOG_DELUSER = 0x02, // 删除用户
  DSPLOG_REREGIST = 0x03, // 重新采集
  DSPLOG_CAHNGETYPE = 0x04, // 改变用户类型
  DSPLOG_UDISK_ULUSER = 0x05, // U盘上传用户
  DSPLOG_UDISK_DLUSER = 0x06, // U盘下载用户
  DSPLOG_UDISK_DLRECORD = 0x07, // U盘下载记录
  DSPLOG_UDISK_UPDATEAPP = 0x08, // U盘更新程序
  // 2.0版支持的功能
  RECORD_SAVEFAIL = 0x00000001, // 是否保存识别失败记录
  SUPER_PASSWORD = 0x00000002, // 超级密码开门有效
  HDBEEP_OPEN = 0x00000004, // 撤防/布防(硬件报警蜂鸣器开启)
  REALTIME_RECORD = 0x00000010, // 实时动态记录显示
  REALTIME_USERLOAD = 0x00000020, // 输入编号或者卡号识别时，如果无法在当前dsp中获得人脸特征,在网络连接状态下，将从管理端实时加载人脸特征
  REALTIME_USERSEND = 0x00000040, // 采集用户同步
  DOORMANGET_OPEN = 0x00000080, // 开启门磁检测报警
  DOORFORCE_OPEN = 0x00000100, // 开启胁迫开门功能
  REMOTE_CAP_SAVE = 0x00000200, // 远程控制采集的用户是否保存到DSP
  GPRS_OPEN = 0x00000400, // 开启GPRS模块
  UPDATE_USERSEND = 0x00000800, // 特征更新时，是否实时发送用户特征
  REALTIME_USERLOAD_PRIOR = 0x00001000, // 网络连接情况下，优先网络加载
  AB_LOCK = 0x00002000, // AB互锁
  DOOR1_NOPEN = 0x00004000, // 门一N+1
  DOOR2_NOPEN = 0x00008000, // 门二N+1
  ENABLE_COUNT = 0x00010000, // 在线记录数统计
  ENCRYP_DISPALY = 0x00020000, // 在线记录数统计
  // 验证模式优先级低-高:UID<CARD<11<MIX<1N
  VERIFY_USERID = 0x01, // 编号
  VERIFY_CARD = 0x02, // 刷卡
  VERIFY_FACE_11 = 0x04, // 11人脸加载
  VERIFY_FACE_MIX = 0x08, // 混合人脸加载
  VERIFY_FACE_1N = 0x10, // 1N人脸加载
  VERIFY_FINGER = 0x20, // 指纹认证
  OUTPUT_WG_YES = 0x01, // 输出伟根协议
  OUTPUT_WG_34 = 0x02, // 指定输出伟根34，不指定输出伟根26
  OUTPUT_WG_CTX = 0x04, // 伟根输出指定内容
  OUTPUT_WG_UID = 0x08, // 不标记OUTPUT_WG_CTX，输出用户ID，默认输出卡号
  WG_OLD = 0,
  WG_26 = 1,
  WG_34 = 2,
  WG_35 = 3,
  WG_36 = 4,
  WG_37 = 5,
  WG_44 = 6,
  WG_45 = 7,
  // IO设备状态
  IO_MODE_NORMAL = 0x00, // 正常状态
  IO_MODE_OPEN = 0x01, // 常开状态
  IO_MODE_CLOSE = 0x02, // 常关状态
  // 门磁状态
  IO_STATE_CLOSE = 0x00, // 门磁关
  IO_STATE_OPEN = 0x01, // 门磁开
  // 执行动作
  ACT_IO_OPEN = 0x02, // 执行打开动作
  ACT_IO_OPENEX = 0x04, // 执行辅助动作
  ACT_IO_CLOSE = 0x10, // 执行关闭动作
  ACT_MODE_SET = 0x20, // 设置IO设备工作模式
  ACT_MODE_GET = 0x40, // 获得IO设备工作模式
  ACT_STATE_GET = 0x80, // 获得IO设备当前状态
  // 关于485地址解析:
  // 1.485地址用一个字节表示，[4-7]表示设备索引号1-15 , [0-3]表示支持16种不同型的设备
  // 已经定义的设备类型 0--人脸前端 , 1--读卡器 ,
  // 例如: 0x11 表示1号门人脸识别前端1， INDEX[4-7] == 1 TYPE[0-3] == 1
  // 0x10 表示1号门人脸识别前端0，INDEX[4-7] == 1 TYPE[0-3] == 0
  // 0x12 表示1号门刷卡器0 INDEX[4-7] == 1 TYPE[0-3] == 2
  // 0x13 表示1号门刷卡器1 INDEX[4-7] == 1 TYPE[0-3] == 3
  // 0x1F 表示门点本身 INDEX[4-7] == 1 TYPE[0-3] == F
  // 0x14 表示辅助输入1 INDEX[4-7] == 1 TYPE[0-3] == 4
  // 0x25 表示辅助输出2 INDEX[4-7] == 2 TYPE[0-3] == 5
  // 2. 所有设备的序号从1开始。此协议可支持15张门，16中不同类型的门点输入设备，15个辅助输入和15个辅助输出
  // 3. [0-7] == 0x00 表示非法地址，[0-7] ＝＝ 0x01表示后端板本身
  // 7_______3______0
  // |_INDEX_|__type__|
  DEV_TYPE_FACE1 = 0x0, // 人脸前端1
  DEV_TYPE_FACE2 = 0x1, // 人脸前端2
  DEV_TYPE_CARD1 = 0x2, // 刷卡器1
  DEV_TYPE_CARD2 = 0x3, // 刷卡器2
  DEV_TYPE_IN = 0x4, // 辅助输入
  DEV_TYPE_OUT = 0x5, // 辅助输出
  DEV_TYPE_BUTTON = 0x6, // 出门按钮
  DEV_TYPE_DOOR = 0xF, // 门点本身
  // 图像格式
  IMAGE_UYVY = 0x01,
  IMAGE_YUYV = 0x02,
  IMAGE_RGB24 = 0x03,
  //结构长度
  OFFSET_SIZE = 20, // 包头字节
  NCMD_size = 20, // 命令头字节
  PACK_SIZE = (60 * 1024), // 拆包
  ID_LEN = 24, // id最大长度 比如 用户名 密码 
  URL_LEN = 127, // url长度
  IP_LEN = 64, // IP字符串长度
  NETPRO_size = 128 + 128 + 16 * 5,
  NETDATE_size = 8,
  NETTIME_size = 8,
  DATETIME_size = NETDATE_size + NETTIME_size,
  TIMESE_size = NETDATE_size * 2,
  DSPTIMEG_size = 4 + DATETIME_size * 2 + 4,
  DSPRIGHT_size = (TGROUP_NUM + 2) * 4,
  USERGROUP_size = 8 + 2 * ID_LEN * MAX_GROUPUSER,
  RECORDRANGE_size = ID_LEN + 8,
  IOCTRL_size = 4,
};
#define MIN(a, b)   ((a)<(b) ? (a):(b))
#define MAX(a, b)   ((a)>(b) ? (a):(b))
typedef unsigned char byte;

// 可变数组结构体。数组元素具体什么内容，需要根据NCMD设定的命令来解释
struct CARRAY {
  int m_Count; // 数组元素的个数
  byte* m_Block; // 数组地址, 4是为了结构体的字节对齐
};
// 日期
struct NETDATE {
  int m_Year;
  byte m_Month;
  byte m_Day;
};
int NETDATE_tostring(const NETDATE* date, char* str, int len)
{
  _snprintf(str, len, "%d/%d/%d", date->m_Year, date->m_Month, date->m_Day);
  return 0;
}
// 时间
struct NETTIME {
  int m_Msec; // 毫秒
  byte m_Hour;
  byte m_Minute;
  byte m_Second;
};
int NETTIME_tostring(const NETTIME* date, char* str, int len)
{
  _snprintf(str, len, "%d:%d-%d-%d", date->m_Hour, date->m_Minute, date->m_Second, date->m_Msec);
  return 0;
}
struct DATETIME {
  NETDATE m_Date;
  NETTIME m_Time;
};
int DATETIME_tostring(const DATETIME* dt, char* str, int len)
{
  char buf1[32] = {0};
  char buf2[32] = {0};
  NETDATE_tostring(&dt->m_Date, buf1, 32);
  NETTIME_tostring(&dt->m_Time, buf2, 32);
  _snprintf(str, len, "日期:%s 时间:%s", buf1, buf2);
  return 0;
}
// 网络属性
struct NETPRO {
  byte m_WebPro1;
  byte m_WebPro2;
  char m_WebUrl1[URL_LEN];// = new byte[127];
  char m_WebUrl2[URL_LEN];// = new byte[127];
  char m_IPAdr[IP_LEN];// = new byte[16];
  char m_MIPAdr[IP_LEN];// = new byte[16];
  char m_NetMask[IP_LEN];// = new byte[16];
  char m_GateWay[IP_LEN];// = new byte[16];
  char m_Dns[IP_LEN];// = new byte[16];
};
// 获得指定时间段的ID集合
struct TIMESE {
  NETDATE m_Start; // 记录起始时间
  NETDATE m_End; // 记录结束时间
};
void TIMESE_set(TIMESE* s, NETDATE Start, NETDATE End)
{
  s->m_Start = Start;
  s->m_End = End;
}
// 时间组
struct DSPTIMEG {
  int m_TGID; // 时间组ID
  DATETIME m_Start; // 时间组开始时间
  DATETIME m_End; // 时间组结束时间
  byte m_TGType; // 时间组类型, TG_NORMAL,TG_HOLIDY
  byte m_CheckFlag; // 时间检测标记例：m_CheckFlag = CHECK_TIME|CHECK_WEEK
  byte m_WeekFlag; // 检测星期时，标记那些星期有效。例：m_WeekFlag = WEEK_1|WEEK_5
  byte m_Reserve; // 保留字节
};
// 权限
struct DSPRIGHT {
  // 权限ID
  int m_RightID;
  // 时间组ID , (m_TimeGID[0]==ANY_TIME)未指定时间组，开门方式不受时间限制，任意时间段验证成功执行开门方式.
  int m_TimeGID[TGROUP_NUM];
  // BIT(0):1 检测节假日, 不检测节假日
  // BIT(1):1 电锁, 1输出 0不输出
  // BIT(2):1 辅助, 1输出 0不输出
  // BIT(3):1 N＋1, 开启 0关闭
  int m_Mode;
};
struct CID {
  char m_ID[ID_LEN];
};
struct USERGROUP {
  // 组ID
  int m_GroupID;
  // BIT(0):1有序，0无序
  byte m_GroupFlag;
  // 组合成功后，输出参数ACTION_LOCK|ACTION_OUTPUT|ACTION_N_1
  byte m_Param;
  // 普通用户中有效用户数
  byte m_NormalValid;
  // 强制用户中有效用户数
  byte m_ForceValid;
  // 普通用户组合，优先级低
  CID m_NormalUsers[MAX_GROUPUSER];// [MAX_GROUPUSER];
  // 强制用户组合，优先级高
  CID m_ForceUsers[MAX_GROUPUSER]; // [MAX_GROUPUSER];
};
struct GROUPEX {
  int m_GroupID;
  int m_StrSize;
  byte m_GroupFlag;
  byte m_MinNum;
  byte* m_BLock;//
};
struct VUSER {
  int m_UserType; // 终端设备上的用户类型(普通，管理，超管 -- 0 , 1 , 2)
  CID m_UserID; // 用户ID
  DATETIME m_TypeUpdTime; // 用户类型更改时间
  DATETIME m_RegistTime; // 人脸注册时间，即特征采集时间，采集或者重新采集要更新此时间
  DATETIME m_LastUpdTime; // 最后更新的时间，识别时特征发生更新或者重新采集要更新此时间
};
int VUSER_tostring(VUSER* s, char* str, int len)
{
  const char* typestr[] = { "普通", "管理", "超管" };
  char buf1[32] = {0};
  char buf2[32] = {0};
  char buf3[32] = {0};
  DATETIME_tostring(&s->m_TypeUpdTime, buf1, 32);
  DATETIME_tostring(&s->m_RegistTime, buf2, 32);
  DATETIME_tostring(&s->m_LastUpdTime, buf3, 32);
  _snprintf(str, len, "终端设备上的用户类型: (%d, %s)\n用户ID: %s\n用户类型更改时间%s\n人脸注册时间%s\n最后更新的时间%s"
      , s->m_UserType, typestr[s->m_UserType], s->m_UserID.m_ID, buf1, buf2, buf3);
  return 0;
}
struct USER {
  int m_FeatBytes; //人脸特征字节数，==0 表示没有人脸特征
  int m_PicBytes; //注册图像的字节数，==0 表示没有图片
  int m_RightID; // 用户权限ID,三种可能值(DEFAULT_NO_RIGHT,DEFAULT_SINGLE_OPEN,用户指定)
  CID m_ImageSID; // 上传注册照，对应的变更ID，全局唯一
  CID m_UserID; // 用户标识
  CID m_Admin; // 人脸注册管理员,标识此用户的人脸特征是哪个管理员采集
  CID m_AppendID; // 卡号或者其他用于1:1的附加身份确认信息
  CID m_UserName; // 用户名
  CID m_Department; // 部门名称
  DATETIME m_DeadLine; // 失效时间
  DATETIME m_RegistTime; // 人脸注册时间，即特征采集时间，采集或者重新采集要更新此时间
  DATETIME m_LastUpdTime; // 最后更新的时间，识别时特征发生更新或者重新采集要更新此时间
  DATETIME m_TypeUpdTime; // 用户类型变更时间
  byte m_UserFlag; // 用户状态标记
  byte m_UserType; // 终端设备上的用户类型(普通，管理，超管 -- 0 , 1 , 2)
  byte* m_Feat; // 人脸特征，==0 表示没有人脸特征
  byte* m_Pic; // 注册图像 ==NULL 表示没有图片
};
static int ALIGN(int bytes, int alignments)
{
  return (((bytes) + ((alignments) - 1)) & ~((alignments) - 1));
}
// 注册图片,必须JPG,人脸特征是与m_UserID唯一绑定的.m_Block的前m_FeatBytes个字节表示人脸特征,
// 后m_PicBytes个字节表示图片.
int USER_size(USER* s)
{
  return ALIGN(6 * ID_LEN + 4 * DATETIME_size + 2 * 4 + s->m_FeatBytes + s->m_PicBytes + 16, 4);
}
struct RECORDRANGE {
  // 下载多少条,<=0 表示下载m_SID之后产生的所有记录,此场景下m_SID必须有效
  int m_Count;
  // DL_OPENINTERVAL打开开区间下载,否则闭区间下载
  int m_Flag;
  // 从那一条开始，m_SID表示记录精确的流水号,
  // 流水号字符串组成:
  // SN(MAC后3字节) + TYPE(记录类别) + DATETIME(日期) + ID(自定义) = (6BYTE + 1BYTE +
  // 14BYTE + 2BYTE + \0)
  CID m_SID;
};
// 记录信息,识别记录，报警记录
struct RECORD {
  int m_RecType; // 记录类型
  int m_Score; // 识别得分 , 联动事件为联动事件的LINKID
  int m_PicBytes; // 记录图像的字节数，==0 表示没有记录图片
  CID m_ChangID; // 注册照记录号
  CID m_UserID; // 用户ID,如果为空，表示非法记录 , 联动事件为事件描述Text
  CID m_SerialID; // 识别记录流水号ID
  CID m_AppendID; // 卡号或者其他用于1:1的附加身份确认信息
  CID m_UserName; // 用户名
  CID m_Department; // 部门名称
  DATETIME m_RecTime; // 记录时间
  byte m_VerifyMode; // 验证模式,例:VERIFY_CARD|VERIFY_FACE_1N,刷卡+人脸
  byte m_Source; // 事件源设备的485地址
  byte* m_Pic; // 记录图像,必须JPG , 2是为了结构体的字节对齐
};
int RECORD_size(RECORD* s)
{
  return ALIGN(4 * 4 + ID_LEN * 6 + s->m_PicBytes, 4);
}
// CGIMG_STEP( obj ) ALIGN4(
// sizeof(CHANGEIMAGE)+(this).m_PicBytes-4 )
// DSPLOG_STEP( obj ) ALIGN4( sizeof(DSPLOG) )
// 设备操作日志
struct DSPLOG {
  int m_LogType; // 日志类型
  CID m_SerialID; // 流水号ID
  CID m_Admin; // 操作员
  CID m_BeOptUser; // 被操作员
  DATETIME m_RecTime; // 记录时间
};
// 变更注册照
struct CHANGEIMAGE {
  int m_PicBytes; //注册照大小
  CID m_UserID; // 用户ID
  CID m_SerialID; // 流水号ID
  CID m_AppendID; // 卡号或者其他用于1:1的附加身份确认信息
  CID m_UserName; // 用户名
  CID m_Department; // 部门名称
  CID m_Admin; // 人脸注册管理员,标识此用户的人脸特征是哪个管理员采集
  DATETIME m_RecTime; // 记录时间
  byte* m_Pic; // 注册照内存
};
int CHANGEIMAGE_size(CHANGEIMAGE* s)
{
  return ALIGN(6 * ID_LEN + 8 + s->m_PicBytes, 4);
}
// DSP相关统计信息
struct STATIS {
  int m_TotalUsers; // 当前设备的总用户数
  int m_NoFeatUser; // 没有采集人脸特征的用户数
  int m_TotalDays; // 识别记录保存总天数
  int m_TotalRecords; // 总记录数
  int m_TotalAlarm; // 总报警记录数
  int m_TotalDspLog; // 总操作日志数目
  int m_TotalChangeImage; // 总变更注册照
};
int STATIS_tostring(STATIS* s, char* str, int len)
{
  _snprintf(str, len, "当前设备的总用户数:%d\n没有采集人脸特征的用户数:%s\n识别记录保存总天数:%s\n总记录数:%s\n总报警记录数:%s\n总操作日志数目:%s\n总变更注册照",
      s->m_TotalUsers, s->m_NoFeatUser, s->m_TotalDays, s->m_TotalRecords, s->m_TotalAlarm, s->m_TotalDspLog, s->m_TotalChangeImage);
  return 0;
}
// 验证信息
struct AUTH {
  CID m_UserName; // 用户名
  CID m_Password; // 密码
};
// 设备基本信息
struct BASEINFO {
  int m_DevType; // 设备类型
  int m_LimitUser; // 总人数上限
  int m_Limit1NUser; // 1N用户上限
  CID m_SN; // 设备编号
  CID m_Ver; // DSP软件版本
  CID m_Space; // 磁盘容量信息
};
int BASEINFO_tostring(BASEINFO* s, char* str, int len)
{
  _snprintf(str, len, "设备类型:%d\n总使用人数上限:%d\n1N用户上限%d\n设备编号:%s\nDSP软件版本:%s\n磁盘容量信息%s",
      s->m_DevType, s->m_LimitUser, s->m_Limit1NUser, s->m_SN.m_ID, s->m_Ver.m_ID, s->m_Space.m_ID);
  return 0;
}
// 远程采集进度信息
struct RCAPINFO {
  int m_Total; // 采集总数
  int m_Count; // 当前采集数
  CID m_UserID; // 用户登记编号
};
// 设备工作属性设置
struct WORKATT {
  // 设备时间组,(m_TimeGID[0]==ANY_TIME)未指定时间组,设备任意时间有效.设备时间组优先级高于用户权限时间组
  int m_DevCtx[TGROUP_NUM]; // [ TGROUP_NUM ];
  // 设备默认权限: DEFAULT_NO_RIGHT , DEFAULT_SINGLE_OPEN
  int m_DefaultRight;
  // 参见2.0版本定义的标记
  int m_BaseSet;
  // 超级密码, 将产生一条报警记录，并且会抓拍彩色全景图.
  CID m_SuperPassword;
  // 门磁延时时间，单位秒
  byte m_DoorMangetTime;
  // 电锁持续时间，单位秒
  byte m_LockTime;
  // 用户身份验证模式,例如:VERIFY_USERID|VERIFY_CARD|VERIFY_FACE_11
  byte m_VerifyMode;
  // BIT(0) : 0不输出扩展韦根信号,1输出.
  // BIT(1) : 0输出韦根26,1输出韦根34.
  // BIT(2) : 0按照BIT(4)输出 , 1输出m_OutputCtx中指定内容
  // BIT(4) : 0直接输出卡号,1输出编号
  // BIT(4-7): WG输出协议,0-wg26 1-wg34 2-wg35 3-wg36 4-wg37 5-wg44 6-wg45
  byte m_OutputWG;
  // 韦根协议输出内容
  int m_OutputCtx;
};
// 程序更新结构体
struct APPUPDATE {
  int m_Bytes; //文件大小
  CID m_FileName; // 文件名
  byte m_TotalFiles; // 需要更新的文件总数
  byte m_FileIndex; // 当前更新到第几个
  byte* m_File; // 文件数据
};
int APPUPDATE_size(APPUPDATE* s)
{
  return ID_LEN + 6 + s->m_Bytes;
}
struct IOCTRL {
  // IO设备485地址
  byte m_Source;
  // 执行动作
  byte m_Action;
  // IO设备当前模式
  byte m_IOMode;
  // IO设备当前状态状态
  byte m_IOState;
};
// NOPEN:远程请求管理端开门
struct NOPEN {
  // m_Users==1权限ID, m_Users>1组合ID
  int m_CtxID;
  // 验证用户数目： 0密码开门，1权限开门，>1组合开门
  int m_Users;
  // 控制信息
  IOCTRL m_XOpen;
  // 验证用户数组
  CID m_IDS[MAX_GROUPUSER * 2];
};
struct IMAGE {
  int m_Size; // 图像数据总大小BYTES
  int m_W; // 图像宽
  int m_H; // 图像高
  int m_Step; // 行数据步长
  byte m_Type; // 图像类型
  byte* m_Block; // 图像数据
};
struct set_stream {
  int off;
  byte* bytes;
  set_stream(byte* bytes1, int off1) {
    bytes = bytes1;
    off = off1;
  }
  int set_STRING(const char* data, int maxlen) {
    int len = strlen(data);
    len = MIN(len, maxlen);
    memcpy(bytes + off, data, len);
    if (len<maxlen) {
      memset(bytes + off+len, 0, maxlen-len);
    }
    off += maxlen;
    return off;
  }
  int set_BYTE(byte data) {
    bytes[off + 0] = (byte)(data);
    off += 1;
    return off;
  }
  int set_BYTES(byte* data, int n) {
    int i = 0;

    for (; i < n; ++i) {
      bytes[off + i] = data[i];
    }

    off += n;
    return off;
  }
  int set_SHORT(short data) {
    bytes[off + 0] = (byte)(data & 0xff);
    bytes[off + 1] = (byte)((data & 0xff00) >> 8);
    off += 2;
    return off;
  }
  int set_INT(int data) {
    bytes[off + 0] = (byte)(data & 0xff);
    bytes[off + 1] = (byte)((data & 0xff00) >> 8);
    bytes[off + 2] = (byte)((data & 0xff0000) >> 16);
    bytes[off + 3] = (byte)((data & 0xff000000) >> 24);
    off += 4;
    return off;
  }
  int set_INTS(const int* arr, int n) {
    for (int i = 0; i < n; ++i) {
      set_INT(arr[i]);
    }

    return off;
  }
  int set_CID(const CID* str) {
    return set_STRING(str->m_ID, ID_LEN);
  }
  int set_NETDATE(NETDATE* date) {
    if (date != NULL) {
      set_INT(date->m_Year);
      set_BYTE(date->m_Month);
      set_BYTE(date->m_Day);
    }

    off += 8;
    return off;
  }
  int set_NETTIME(NETTIME* time) {
    if (time != NULL) {
      set_INT(time->m_Msec);
      set_BYTE(time->m_Hour);
      set_BYTE(time->m_Minute);
      set_BYTE(time->m_Second);
    }

    off += 8;
    return off;
  }
  int set_DATETIME(DATETIME* datetime) {
    if (datetime != NULL) {
      set_NETDATE(&datetime->m_Date);
      set_NETTIME(&datetime->m_Time);
    }
    else {
      set_NETDATE(NULL);
      set_NETTIME(NULL);
    }

    return off;
  }
  int offset(int add) {
    int i = off;
    off += add;

    for (; i < off; ++i) {
      bytes[i] = 0;
    }

    return off;
  }
  int set_CIDS(const CID* strs, int n) {
    for (int i = 0; i < n; ++i) {
      set_CID(strs + i);
    }

    return off;
  }
  int set_WORKATT(WORKATT* workatt) {
    set_INTS(workatt->m_DevCtx, TGROUP_NUM);
    set_INT(workatt->m_DefaultRight);
    set_INT(workatt->m_BaseSet);
    set_CID(&workatt->m_SuperPassword);
    set_BYTE((byte) workatt->m_DoorMangetTime);
    set_BYTE((byte) workatt->m_LockTime);
    set_BYTE((byte) workatt->m_VerifyMode);
    set_BYTE((byte) workatt->m_OutputCtx);
    set_INT(workatt->m_OutputCtx);
    return off;
  }
  int set_USER(USER* obj) {
    int off1 = off;
    set_INT(obj->m_FeatBytes);
    set_INT(obj->m_PicBytes);
    set_INT(obj->m_RightID);
    set_CID(&obj->m_ImageSID);
    set_CID(&obj->m_UserID);
    set_CID(&obj->m_Admin);
    set_CID(&obj->m_AppendID);
    set_CID(&obj->m_UserName);
    set_CID(&obj->m_Department);
    set_DATETIME(&obj->m_DeadLine);
    set_DATETIME(&obj->m_RegistTime);
    set_DATETIME(&obj->m_LastUpdTime);
    set_DATETIME(&obj->m_TypeUpdTime);
    set_BYTE(obj->m_UserFlag);
    set_BYTE(obj->m_UserType);

    if (obj->m_Feat != NULL) {
      set_BYTES(obj->m_Feat, obj->m_FeatBytes);
    }

    if (obj->m_Pic != NULL) {
      set_BYTES(obj->m_Pic, obj->m_PicBytes);
    }

    off = off1 + USER_size(obj);
    return off;
  }
  int set_RECORDRANGE(RECORDRANGE* obj) {
    int off1 = off;
    set_INT(obj->m_Count);
    set_INT(obj->m_Flag);
    set_CID(&obj->m_SID);
    off = off1 + RECORDRANGE_size;
    return off;
  }
  int set_DSPTIMEG(DSPTIMEG* obj) {
    int off1 = off;
    set_INT(obj->m_TGID);
    set_DATETIME(&obj->m_Start);
    set_DATETIME(&obj->m_End);
    set_BYTE(obj->m_TGType);
    set_BYTE(obj->m_CheckFlag);
    set_BYTE(obj->m_WeekFlag);
    set_BYTE(obj->m_Reserve);
    off = off1 + DSPTIMEG_size;
    return off;
  }
  int set_DSPRIGHT(DSPRIGHT* obj) {
    int off1 = off;
    set_INT(obj->m_RightID);
    set_INTS(obj->m_TimeGID, TGROUP_NUM);
    set_INT(obj->m_Mode);
    off = off1 + DSPTIMEG_size;
    return off;
  }
  int set_USERGROUP(USERGROUP* obj) {
    int off1 = off;
    set_INT(obj->m_GroupID);
    set_BYTE(obj->m_GroupFlag);
    set_BYTE(obj->m_Param);
    set_BYTE(obj->m_NormalValid);
    set_BYTE(obj->m_ForceValid);
    set_CIDS(obj->m_NormalUsers, MAX_GROUPUSER);
    set_CIDS(obj->m_ForceUsers, MAX_GROUPUSER);
    off = off1 + USERGROUP_size;
    return off;
  }
  int set_IOCTRL(IOCTRL* obj) {
    int off1 = off;
    set_BYTE(obj->m_Source);
    set_BYTE(obj->m_Action);
    set_BYTE(obj->m_IOMode);
    set_BYTE(obj->m_IOState);
    off = off1 + USERGROUP_size;
    return off;
  }
  int set_APPUPDATE(APPUPDATE* obj) {
    int off1 = off;
    set_INT(obj->m_Bytes);
    set_CID(&obj->m_FileName);
    set_BYTE(obj->m_TotalFiles);
    set_BYTE(obj->m_FileIndex);

    if (obj->m_File != NULL) {
      set_BYTES(obj->m_File, obj->m_Bytes);
    }

    off = off1 + APPUPDATE_size(obj);
    return off;
  }
  int set_NETPRO(NETPRO* obj) {
    int off1 = off;
    set_BYTE(obj->m_WebPro1);
    set_BYTE(obj->m_WebPro2);
    set_STRING(obj->m_WebUrl1, URL_LEN);
    set_STRING(obj->m_WebUrl2, URL_LEN);
    set_STRING(obj->m_IPAdr, IP_LEN);
    set_STRING(obj->m_MIPAdr, IP_LEN);
    set_STRING(obj->m_NetMask, IP_LEN);
    set_STRING(obj->m_GateWay, IP_LEN);
    set_STRING(obj->m_Dns, IP_LEN);
    off = off1 + NETPRO_size;
    return off;
  }
};
int myrealloc(void* pp1, int n)
{
  void** pp2 = (void**)(pp1);
  *pp2 = realloc(*pp2, n);
  return n;
}
struct get_stream {
  int off;
  byte* bytes;
  get_stream(byte* bytes1, int off1) {
    bytes = bytes1;
    off = off1;
  }
  int get_BYTE(byte* by) {
    *by = (byte)(0xff & bytes[off]);
    off += 1;
    return off;
  }
  int get_INT(int* ret) {
    *ret = (0xff & bytes[off + 0])
        | (0xff00 & (bytes[off + 1] << 8))
        | (0xff0000 & (bytes[off + 2] << 16))
        | (0xff000000 & (bytes[off + 3] << 24));
    off += 4;
    return off;
  }
  int get_BYTES(byte* by, int len) {
    if (len > 0) {
      memcpy(by, bytes + off, len);
      off += len;
    }

    return off;
  }
  int get_STRING(char* s, int len) {
    if (len > 0) {
      memcpy(s, bytes + off, len);
      off += len;
    }

    return off;
  }
  int get_CID(CID* obj) {
    return get_STRING(obj->m_ID, ID_LEN);
  }
  int get_NETDATE(NETDATE* obj) {
    get_INT(&obj->m_Year);
    get_BYTE(&obj->m_Month);
    get_BYTE(&obj->m_Day);
    off += 2;
    return off;
  }
  int get_NETTIME(NETTIME* obj) {
    get_INT(&obj->m_Msec);
    get_BYTE(&obj->m_Hour);
    get_BYTE(&obj->m_Minute);
    get_BYTE(&obj->m_Second);
    off += 1;
    return off;
  }
  int get_DATETIME(DATETIME* obj) {
    get_NETDATE(&obj->m_Date);
    get_NETTIME(&obj->m_Time);
    return off;
  }
  int get_VUSER(VUSER* obj) {
    get_INT(&obj->m_UserType);
    get_CID(&obj->m_UserID);
    get_DATETIME(&obj->m_TypeUpdTime);
    get_DATETIME(&obj->m_RegistTime);
    get_DATETIME(&obj->m_LastUpdTime);
    return off;
  }
  int ALIGN4(int size) {
    return (((size) + 3) & ~3);
  }
  int get_USER(USER* obj) {
    get_INT(&obj->m_FeatBytes);
    get_INT(&obj->m_PicBytes);
    get_INT(&obj->m_RightID);
    get_CID(&obj->m_ImageSID);
    get_CID(&obj->m_UserID);
    get_CID(&obj->m_Admin);
    get_CID(&obj->m_AppendID);
    get_CID(&obj->m_UserName);
    get_CID(&obj->m_Department);
    get_DATETIME(&obj->m_DeadLine);
    get_DATETIME(&obj->m_RegistTime);
    get_DATETIME(&obj->m_LastUpdTime);
    get_DATETIME(&obj->m_TypeUpdTime);
    get_BYTE(&obj->m_UserFlag);
    get_BYTE(&obj->m_UserType);

    if (obj->m_FeatBytes > 0) {
      myrealloc(&obj->m_Feat, obj->m_FeatBytes);
      get_BYTES(obj->m_Feat, obj->m_FeatBytes);
    }

    if (obj->m_PicBytes > 0) {
      myrealloc(&obj->m_Pic, obj->m_PicBytes);
      get_BYTES(obj->m_Pic, obj->m_PicBytes);
    }

    off = ALIGN4(off);
    return off;
  }
  int get_RECORDRANGE(RECORDRANGE* obj) {
    get_INT(&obj->m_Count);
    get_INT(&obj->m_Flag);
    get_CID(&obj->m_SID);
    return off;
  }
  int get_RECORD(RECORD* obj) {
    get_INT(&obj->m_RecType);
    get_INT(&obj->m_Score);
    get_INT(&obj->m_PicBytes);
    get_CID(&obj->m_ChangID);
    get_CID(&obj->m_UserID);
    get_CID(&obj->m_SerialID);
    get_CID(&obj->m_AppendID);
    get_CID(&obj->m_UserName);
    get_CID(&obj->m_Department);
    get_DATETIME(&obj->m_RecTime);
    get_BYTE(&obj->m_VerifyMode);
    get_BYTE(&obj->m_Source);

    if (obj->m_PicBytes > 0) {
      myrealloc(&obj->m_Pic, obj->m_PicBytes);
      get_BYTES(obj->m_Pic, obj->m_PicBytes);
    }

    off = ALIGN4(off);
    return off;
  }
  int get_NETPRO(NETPRO* obj) {
    get_BYTE(&obj->m_WebPro1);
    get_BYTE(&obj->m_WebPro2);
    get_STRING(obj->m_WebUrl1, URL_LEN);
    get_STRING(obj->m_WebUrl2, URL_LEN);
    get_STRING(obj->m_IPAdr, IP_LEN);
    get_STRING(obj->m_MIPAdr, IP_LEN);
    get_STRING(obj->m_NetMask, IP_LEN);
    get_STRING(obj->m_GateWay, IP_LEN);
    get_STRING(obj->m_Dns, IP_LEN);
    return off;
  }
};
