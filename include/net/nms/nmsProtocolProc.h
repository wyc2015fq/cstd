#ifndef _nmsPROTOCOLPROC_H_
#define _nmsPROTOCOLPROC_H_

#include "stdint.h"

#define TSPORT               60000                  // 直连服务器端口
#define NETCONNECT_TIMEOUT  (3*60*1000)             // 网络连接超时时间
#define SOCKETBUFFSIZE      (800)
#define MSGHEADLEN          sizeof(SMSGBUFF)
#define DEVMANMODID         1


typedef enum _PACKETTYPE {
  TYPE_REQ    = 0x71,
  TYPE_REQACK = 0x51,
  TYPE_ACK    = 0x41,
  TYPE_ERROR  = 0x45,
} PACKETTYPE;

typedef enum _PACKETERR {
  ERR_PACKFORMAT  = 0x01,
  ERR_DEVOFFLINE,
  ERR_MODOFFLINE,
  ERR_DATAFORMAT,
  ERR_LOGIN
} PACKETERR;

#pragma pack(1)
typedef struct __SMSGBUFF {
  uint8_t         head[2];
  uint8_t         type;
  uint8_t         reserved[5];
  uint8_t         SrcDevID[16];   // 源设备地址
  uint8_t         DesDevID[16];   // 目的设备地址
  uint8_t         SrcModID[2];    // 源模块
  uint8_t         DesModID[2];    // 目的模块
  uint8_t         Index[4];       // 消息ID
  uint8_t         DataLen[4];     // 数据区长度
  char            Data[0];        // 数据区
} SMSGBUFF;
#pragma pack()


#define SETMSGBUFFVALUE(pmsg,msgtype,pSguid,pDguid,smod,dmod,index,datalen,pData) {\
    char *p = (char *)pData; \
    memset((char *)(pmsg),0,sizeof(SMSGBUFF)); \
    (pmsg)->head[0] = 0x52; (pmsg)->head[1] = 0x31; \
    (pmsg)->type    = (msgtype);   \
    if((pSguid) != NULL)memcpy((pmsg)->SrcDevID,(pSguid),16); \
    if((pDguid) != NULL)memcpy((pmsg)->DesDevID,(pDguid),16); \
    (pmsg)->SrcModID[0]  = ((smod) >> 8) & 0xff; \
    (pmsg)->SrcModID[1]  = (smod) & 0xff; \
    (pmsg)->DesModID[0]  = ((dmod) >> 8) & 0xff; \
    (pmsg)->DesModID[1]  = (dmod) & 0xff; \
    (pmsg)->Index[0] = ((index) >> 24) & 0xff;  \
    (pmsg)->Index[1] = ((index) >> 16) & 0xff;  \
    (pmsg)->Index[2] = ((index) >> 8) & 0xff;  \
    (pmsg)->Index[3] = (index) & 0xff;  \
    (pmsg)->DataLen[0] = ((datalen) >> 24) & 0xff;  \
    (pmsg)->DataLen[1] = ((datalen) >> 16) & 0xff;  \
    (pmsg)->DataLen[2] = ((datalen) >> 8) & 0xff;  \
    (pmsg)->DataLen[3] = (datalen) & 0xff;  \
    if(p && datalen) memcpy((pmsg)->Data,p,datalen); \
  }

#define MsgBuffDebug(dir,pInMsg) \
  do{    \
    int index = ((pInMsg)->Index[0]<<24L) + ((pInMsg)->Index[1]<<16L) + ((pInMsg)->Index[2]<<8L) + (pInMsg)->Index[3]; \
    int datalen = ((pInMsg)->DataLen[0]<<24L) + ((pInMsg)->DataLen[1]<<16L) + ((pInMsg)->DataLen[2]<<8L) + (pInMsg)->DataLen[3]; \
    MDEBUG(DDEBUG,"%s:[%X]%c:[%x...%x]-[%x...%x] [%d]-[%d]:datalen=%d\n", \
        dir,index, \
        (pInMsg)->type,\
        (pInMsg)->SrcDevID[0],(pInMsg)->SrcDevID[15],\
        (pInMsg)->DesDevID[0],(pInMsg)->DesDevID[15],\
        ((pInMsg)->SrcModID[0]<<8L) + (pInMsg)->SrcModID[1],\
        ((pInMsg)->DesModID[0]<<8L) + (pInMsg)->DesModID[1],\
        datalen);\
  }while(0)

extern int32_t  gTransaction;
extern const char gServerGuid[16];
extern const char gLocalGuid[16];


int32_t ProtocolMsgProc(SMSGBUFF* pInMsg, SMSGBUFF** pOutMsg);

int32_t MsgPackHeadSwap(SMSGBUFF* pInMsg, SMSGBUFF* pOutMsg);
int32_t AckMsgPacketEncode(SMSGBUFF* pInMsg, char* pOutBuff, int bufsize);
int32_t ErrMsgPacketEncode(SMSGBUFF* pInMsg, int errcode, char* pOutBuff, int bufsize);



#endif

