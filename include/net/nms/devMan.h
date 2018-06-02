#ifndef DEVMAN_H__
#define DEVMAN_H__


#define DEVMANSOFTVER "V1.0.1"
#define DEVMANHARDVER "V1.0.1"

/*
 * 定义可注册的模块号
 */

#define MOD_MAIN            1
#define MOD_CHARGER         10
#define MOD_PRINTERID       11
#define MOD_NFCID           12
#define MOD_PAYID           13
#define MOD_SCANQRCODEID    14
#define MOD_READQRCODEID    15

#define MOD_NMSID           100
#define MOD_TIMEDID         101
#define MOD_LOCALID         102
#define MOD_WIFIPROBE       103
#define MOD_NMSCLIENTID     104
#define MOD_SYSMONITOR      105
#define MOD_REMOTECTRL      106
#define MOD_SCRRENSHOT      107

#define SETDEVID(HOST,MOD,DEV)  ((HOST<<24)|((MOD)<<16)|(DEV))      // 设置DEVID参数
#define GETHOSTID(devid)        (((devid) >> 24) & 0x000000FF)      // 读DEVID中主机地址
#define GETMODID(devid)         (((devid) >> 16) & 0x000000FF)      // 读DEVID中模块地址
#define GETDEVID(devid)         ((devid) & 0x0000FFFF)              // 读DEVID中子设备地址

/*
 * 定义应用层消息结构体
 * ----地址值说明:[0]无效值
 */
typedef enum __MSGTYPE {
  /*
   *  系统级消息
   */
  DEVDPCTRL  = 1,         // 设备控制消息
  SYSEXIT,                // 退出应用程序
  MSG_ACK    = 0x41,
  MSG_ERR    = 0x45,
  MSG_REQACK = 0x51,
  MSG_REQ    = 0x71,
  /*
   *  系统告警消息
   */
  DEVWARNING = 0x100,     // 设备告警消息
  /*
   *  定时模块消息
   */
  MSG_TIMEDADD = 0x200,
  MSG_TIMEDDEL,
} MSGTYPE;

typedef enum __DATATYPE {
  TYPE_NULL = 0,
  TYPE_S8,
  TYPE_U8,
  TYPE_S16,
  TYPE_U16,
  TYPE_S32,
  TYPE_U32,
  TYPE_S64,
  TYPE_U64,
  TYPE_F32,
  TYPE_F64,
  TYPE_DATE,
  TYPE_HEX,
  TYPE_STRING,
  TYPE_JSON,
  TYPE_UDF = 0xFF
} DATATYPE;

#pragma pack(1)
typedef struct __SMSGPACK {
  unsigned int    SrcDevID;       // 源设备地址
  unsigned int    DesDevID;       // 目的设备地址
  MSGTYPE         MsgType;        // 消息类型
  int             Datalen;        // 数据区长度
  DATATYPE        DataType;       // 数据类型
  char            Data[0];        // 数据区
} SMSGPACK;

#pragma pack()

#define MsgPackDebug(pInMsg) \
  do{    \
    MDEBUG(DDEBUG,"[%x]:[%x-%x]:%s\n", \
        pInMsg->MsgType,\
        pInMsg->SrcDevID,\
        pInMsg->DesDevID,\
        (pInMsg->DataType)?pInMsg->Data:"NULL");\
  }while(0)

#define GETMSGPACKLEN(datalen) (datalen+sizeof(SMSGPACK))                       // 获取消息包总长度
//#define MallocMsgPack(datalen) ((SMSGPACK *)malloc(GETMSGPACKLEN(datalen) + 1)) // 分配空间

static SMSGPACK* MallocMsgPack(int datalen)
{
  int len = GETMSGPACKLEN(datalen) + 1;
  SMSGPACK* p = (SMSGPACK*)malloc(len);
  memset(p, 0, len);
  return p;
}

typedef struct __ModuleReg {
  const unsigned int ModeId;      // 注册模块ID
  const char* pName;              // 模块名
  int(*moduleInit)(void*);        // 模块初始化
  int(*msgProc)(void*, int);      // 数据处理，该接口能收到和模块相关的所有消息
  int(*moduleExit)(void);         // 模块退出
} ModuleReg;

/*
 * 模块消息发送到上层处理
 */
//int SendMsg2Main(char* pData,int len);

/*
 * 模块消息直接发送到网络
 */
//extern int32_t NMSClientSendMsg(SMSGPACK *pMsgPack);

#endif

