// __NMS_FORWARDINGSERVER_HH__
//#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if 0
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#endif

#include "cstd.h"
#include "net.h"
#include "net/net.inl"
//#include <auto_stdint.h>
#include "common.h"
//#include "cJSON.h"
#include "cjson.inl"
#include "devMan.h"
//#include "nmsProtocolProc.h"
//#include "jsDevParamConfig.h"
//#include "cJSON.h"
#include "datalist.h"
#include "queue.h"

typedef enum _CLIENTSTA {
  TC_INIT = 1,
  TC_CONNECTING,
  TC_LOGIN,
  TC_CONNECTED
} CLIENTSTA;

static char cTC_Sta      = TC_INIT;
static int32_t  gTC_clifd    = -1;
static DList* pNmsClientList = NULL;

static pthread_t th_TCMain;
static pthread_t th_TCRecv;
static pthread_mutex_t gCMsgSendLock;

static int32_t  gTransaction = 0;

static const char gServerGuid[16] = {0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x01
                                    };

static const char gLocalGuid[16]  = {0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
                                    };

typedef int (*ProtocolMsgProc)(struct ry_sock* s, SMSGBUFF* pInMsg);

typedef struct ry_sock {
  int hostid;
  int modid;
  ProtocolMsgProc proc;
  void* user;
} ry_sock;


static void _CloseSock(void)
{
  int32_t fd;

  if (gTC_clifd > 0) {
    fd = gTC_clifd;
    gTC_clifd = -1;
    //shutdown(fd,SHUT_RDWR);
    closesocket(fd);
  }
}

static int32_t NMSC_Connect(const char* server_ip, const char* server_port)
{
  struct sockaddr_in server_addr;
  struct timeval timev = {3, 0};

  memset(&server_addr, 0, sizeof(server_addr));

  gTC_clifd = socket(AF_INET, SOCK_STREAM, 0);

  if (gTC_clifd < 0) {
    MDEBUG(DERR, "Create Socket Failed!\n");
    gTC_clifd = -1;
    return -1;
  }

  setsockopt(gTC_clifd, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timev, sizeof(timev));

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(atoi(server_port));

  //if(inet_aton(server_ip, &server_addr.sin_addr) == 0)
  if (inet_pton4(server_ip, (unsigned char*)&server_addr.sin_addr) == 0) {
    struct hostent* he;
    he = gethostbyname(server_ip);

    if (he == NULL) {
      _CloseSock();
      MDEBUG(DERR, "Server IP Address(%s) Error!\n", server_ip);
      return -1;
    }

    memcpy(&server_addr.sin_addr, he->h_addr, sizeof(struct in_addr));
  }

  if (connect(gTC_clifd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
    MDEBUG(DINFO, "Can Not Connect To %s:%s!\n", server_ip, server_port);
    _CloseSock();
    return -1;
  }

  return 0;
}

static int32_t mNMSC_Recv(char* buffer, int32_t len, int32_t timeout_ms)
{
  int32_t ret;
  struct timeval tv;
  fd_set watchset;

  if (gTC_clifd <= 0) {
    return -1;
  }

  FD_ZERO(&watchset);
  FD_SET(gTC_clifd, &watchset);

  if (timeout_ms) {
    tv.tv_sec  = timeout_ms / 1000;
    tv.tv_usec = (timeout_ms % 1000) * 1000;
  }
  else {
    tv.tv_sec  = 60;
    tv.tv_usec = 0;
  }

  ret = select(gTC_clifd + 1, &watchset, NULL, NULL, &tv);

  if (ret < 0) {
    MDEBUG(DERR, "Select error.\n");
  }
  else if (ret == 0) {
    MDEBUG(DERR, "Select time out.\n");
  }
  else {
    ret = recv(gTC_clifd, buffer, len, 0);
    MDEBUG(DINFO, "ret=%d.\n", ret);
  }

  if (ret < 0) {
    _CloseSock();
    cTC_Sta = TC_INIT;
  }

  return ret;
}

static int32_t mNMSC_RecvHead(SMSGBUFF* pMsgBuff)
{
  int32_t ret, index;
  char* pbuff = (char*)pMsgBuff;

  index = 0;

  while (1) {
    ret = mNMSC_Recv(pbuff + index, MSGHEADLEN - index, 60000);

    if (ret <= 0) {
      return ret;
    }

    index += ret;

    if (index < MSGHEADLEN) {
      continue;
    }

    for (index = 0; index < (MSGHEADLEN - 1); index++) {
      if ((pbuff[index] == 0x52) && (pbuff[index + 1] == 0x31)) {
        break;
      }
    }

    if (!index) {
      break;
    }

    if (pbuff[index] == 0x52) {
      memcpy(pbuff, pbuff + index, MSGHEADLEN - index);
      index = MSGHEADLEN - index;
    }
    else {
      index = 0;
    }
  }

  return MSGHEADLEN;
}
static int32_t mNMSC_RecvData(char* databuff, int32_t len)
{
  int32_t ret, index;

  index = 0;

  while (1) {
    ret = mNMSC_Recv(databuff + index, len - index, 60000);

    if (ret <= 0) {
      return ret;
    }

    index += ret;

    if (index >= len) {
      break;
    }
  }

  return index;
}

int32_t _Send(char* pData, int len)
{
  int32_t ret, sendlen;

  if (0 != pthread_mutex_lock(&gCMsgSendLock)) {
    return -1;
  }

  sendlen = 0;

  while (sendlen < len) {
    if (gTC_clifd <= 0) {
      sendlen = -1;
      break;
    }

    if (len - sendlen > SOCKETBUFFSIZE) {
      ret = send(gTC_clifd, pData + sendlen, SOCKETBUFFSIZE, 0);
    }
    else {
      ret = send(gTC_clifd, pData + sendlen, len - sendlen, 0);
    }

    if (ret <= 0) {
      sendlen = -1;
      break;
    }

    sendlen += ret;
  }

  pthread_mutex_unlock(&gCMsgSendLock);

  return sendlen;
}

static int LoginMsgProc(SMSGBUFF* pMsg)
{
  cJSON* pJsData, *pJsItem;
  int ret = -1;

  pJsData = cJSON_Parse(pMsg->Data);

  if (!pJsData) {
    return -1;
  }

  do {
    pJsItem = cJSON_GetObjectItem(pJsData, "cmd");

    if (!pJsItem || !pJsItem->string) {
      break;
    }

    if (strcmp(pJsItem->valuestring, "login")) {
      break;
    }

    ret = 0;
  }
  while (0);

  cJSON_Delete(pJsData);
  return ret;
}

static int32_t SendData(ry_sock* s, uint8_t MsgType, const char* pDesGUID, int32_t DesModID, char* pData, int32_t datalen)
{
  char* pBuff = (char*)malloc(MSGHEADLEN + datalen + 1);
  int32_t ret;

  if (!pBuff) {
    MDEBUG(DERR, "malloc(%d) return fail.\n", (int32_t)(MSGHEADLEN + datalen));
    return -1;
  }

  SETMSGBUFFVALUE((SMSGBUFF*)pBuff, MsgType, gLocalGuid, pDesGUID, s->modid, DesModID, gTransaction, datalen, pData);
  gTransaction++;
  MsgBuffDebug("SEND", (SMSGBUFF*)pBuff);
  ret = _Send(pBuff, datalen + MSGHEADLEN);
  free(pBuff);

  return ret;
}

static int SendLoginMsg(ry_sock* s)
{
  char cdata[100];

  sprintf(cdata, "{\"cmd\":\"login\",\"model\":\"%d\"}", s->modid);
  return SendData(s, TYPE_REQ, gLocalGuid, 0, cdata , strlen(cdata));
}

#define SendErrMsg(srcMsg, errType) \
  { \
    int32_t errmsglen = ErrMsgPacketEncode(srcMsg, errType, cSendBuffer, SOCKETBUFFSIZE); \
    _Send(cSendBuffer, errmsglen); \
    MsgBuffDebug("SEND ERR",(SMSGBUFF *)cSendBuffer); \
    MDEBUG(DERR,"ERR DATA:%s\n",((SMSGBUFF *)cSendBuffer)->Data); \
  }

int32_t MsgPackHeadSwap(SMSGBUFF* pInMsg, SMSGBUFF* pOutMsg)
{
  memcpy((char*)pOutMsg, (char*)pInMsg, MSGHEADLEN);
  memcpy(pOutMsg->SrcDevID, pInMsg->DesDevID, 16);
  memcpy(pOutMsg->DesDevID, pInMsg->SrcDevID, 16);
  memcpy(pOutMsg->SrcModID, pInMsg->DesModID, 2);
  memcpy(pOutMsg->DesModID, pInMsg->SrcModID, 2);
  memset(pOutMsg->DataLen, 0, 4);
  return 0;
}

int32_t AckMsgPacketEncode(SMSGBUFF* pInMsg, char* pOutBuff, int bufsize)
{
  SMSGBUFF* pOutMsg = (SMSGBUFF*)pOutBuff;

  if (bufsize < MSGHEADLEN) {
    return -1;
  }

  MsgPackHeadSwap(pInMsg, pOutMsg);
  pOutMsg->type = TYPE_ACK;

  return MSGHEADLEN;
}

int32_t ErrMsgPacketEncode(SMSGBUFF* pInMsg, int errcode, char* pOutBuff, int bufsize)
{
  SMSGBUFF* pOutMsg = (SMSGBUFF*)pOutBuff;
  int32_t iTemp, datalen;
  static char* errstr[] = {
    "",
    "Message format error",
    "Device not online",
    "Module not online",
    "DATA format error",
    "Connect not login",
    "",
  };

  if (bufsize < MSGHEADLEN + 100) {
    return -1;
  }

  MsgPackHeadSwap(pInMsg, pOutMsg);
  pOutMsg->type = TYPE_ERROR;
  /*
   * data
   */
  sprintf(pOutMsg->Data, "{\"errcode\":%d,\"errstring\":\"%s\"}", errcode, errstr[errcode]);

  /*
   * len
   */
  datalen = strlen(pOutMsg->Data);
  iTemp = htonl(datalen);
  memcpy(pOutMsg->DataLen, (char*)&iTemp, 4);

  return MSGHEADLEN + datalen;
}


typedef struct __SNetCmd {
  char*    pCmd;
  int32_t(*MsgPro)(ry_sock* s, int dst_modid, cJSON* pJsInput);
  void* user;
} SNetCmd;

//static SNetCmd* g_RegistNetCmd = NULL;
//int g_nRegistNetCmd = 0;
static int32_t runProtocolMsgProc(ry_sock* s, SMSGBUFF* pInMsg, SNetCmd* RegistNetCmd, int g_nRegistNetCmd, int* pmodid)
{
  //SNetCmd* RegistNetCmd = g_RegistNetCmd;
  cJSON* pJsData, *pJsItem;
  int i;
  int ret = -1;
  int errcode = 0;

  if (NULL == pInMsg) {
    return -1;
  }

  pJsData = cJSON_Parse(pInMsg->Data);

  if (!pJsData) {
    return -1;
  }

  int dst_modid = (pInMsg->SrcModID[0] << 8L) + pInMsg->SrcModID[1];
  if (pmodid) {
    *pmodid = dst_modid;
  }
  if (pJsItem = cJSON_GetObjectItem(pJsData, "errcode")) {
    ret = pJsItem->valueint;
    return ret;
  }
  do {
    pJsItem = cJSON_GetObjectItem(pJsData, "cmd");

    if (!pJsItem || !pJsItem->string) {
      break;
    }

    for (i = 0; i < g_nRegistNetCmd; i++) {
      if (strcmp(pJsItem->valuestring, RegistNetCmd[i].pCmd)) {
        continue;
      }

      ret = RegistNetCmd[i].MsgPro(s, dst_modid, pJsData);
      break;
    }
  }
  while (0);

  if (pJsData) {
    cJSON_Delete(pJsData);
  }

#if 0
  if ((ret > 0)) {
    uint32_t u32temp;
    MsgPackHeadSwap(pInMsg, *ppOutMsg);
    (*ppOutMsg)->Index[0] = (gTransaction >> 24) & 0xff;
    (*ppOutMsg)->Index[1] = (gTransaction >> 16) & 0xff;
    (*ppOutMsg)->Index[2] = (gTransaction >> 8) & 0xff;
    (*ppOutMsg)->Index[3] = gTransaction & 0xff;
    gTransaction++;
    u32temp = htonl(ret);
    memcpy((*ppOutMsg)->DataLen, (char*)&u32temp, 4);
    return (ret + MSGHEADLEN);
  }
#endif

  return ret;
}

static queue_t* gpMsgList = NULL;
static int SendMsg2Main(char* pData, int len)
{
  SMSGPACK* pMsg;
  int ret;

  //pthread_mutex_lock(&gMainMsgSendLock);
  pMsg = (SMSGPACK*)malloc(len + 1);
  *((char*)pMsg + len) = '\0';

  if (pMsg) {
    memcpy((char*)pMsg, (char*)pData, len);
    ret = queue_push(gpMsgList, pMsg);

    if (ret < 0) {
      MDEBUG(DERR, "queue_push:%d\n", ret);
    }
  }
  else {
    ret = -1;
    MDEBUG(DERR, "malloc failed\n");
  }

  //pthread_mutex_unlock(&gMainMsgSendLock);

  return ret;
}


static void* NMSC_RecvThread(void* t)
{
  static char cRecvBuffer[SOCKETBUFFSIZE + 1];
  static char cSendBuffer[SOCKETBUFFSIZE + 1];
  SMSGBUFF* pMsgBuff = (SMSGBUFF*)cRecvBuffer;
  int32_t ret, desmodid, datalen;
  int8_t timeout_cnt = 0;
  ry_sock* s = (ry_sock*)t;

  //signal(SIGUSR1, ThreadExitHandle);
  datalen = 0;
  pMsgBuff = (SMSGBUFF*)cRecvBuffer;

  while (1) {
    if ((cTC_Sta < TC_LOGIN) || (cTC_Sta > TC_CONNECTED)) {
      usleep(10000);
      datalen = 0;
      timeout_cnt = 0;
      continue;
    }

    while (datalen > 0) {
      if (datalen < SOCKETBUFFSIZE) {
        ret = mNMSC_RecvData(cRecvBuffer, datalen);
      }
      else {
        ret = mNMSC_RecvData(cRecvBuffer, SOCKETBUFFSIZE);
      }

      if (ret <= 0) {
        break;
      }

      datalen -= ret;
      cRecvBuffer[ret] = 0;
      MDEBUG(DDEBUG, "Drop(len=%d):%s\n", ret, cRecvBuffer);
    }

    ret = mNMSC_RecvHead(pMsgBuff);

    if (0 > ret) {
      MDEBUG(DDEBUG, "mNMSC_RecvHead return %d.\n", ret);
      continue;
    }
    else if (0 == ret) {
      if (++timeout_cnt > 5) {
        _CloseSock();
        cTC_Sta = TC_INIT;
        usleep(10000);
      }
      else {
        MDEBUG(DDEBUG, "Send heart.\n");
        SendData(s, TYPE_REQACK, gLocalGuid, 0, NULL , 0);
      }

      continue;
    }
    else {
      timeout_cnt = 0;
    }

    MsgBuffDebug("RECV", pMsgBuff);
    desmodid = (pMsgBuff->DesModID[0] << 8L) + pMsgBuff->DesModID[1];
    datalen  = ntohl(*(int32_t*)pMsgBuff->DataLen);

    if (desmodid != s->modid) {
      MDEBUG(DDEBUG, "DesModID error.\n");
      continue;
    }

    if (pMsgBuff->type == TYPE_REQACK) {
      SMSGBUFF msgsend;
      MsgPackHeadSwap(pMsgBuff, &msgsend);
      msgsend.type = TYPE_ACK;
      _Send((char*)&msgsend, MSGHEADLEN);
      MDEBUG(DDEBUG, "SENDACK.\n");
    }

    if (cTC_Sta == TC_LOGIN) {
      if (pMsgBuff->type == TYPE_ACK) {
        continue;
      }

      if ((datalen <= 0) || (datalen > (SOCKETBUFFSIZE - MSGHEADLEN))) {
        MDEBUG(DERR, "datalen = %d.\n", datalen);
        SendErrMsg(pMsgBuff, ERR_LOGIN);
        continue;
      }

      if (datalen != mNMSC_RecvData(cRecvBuffer + MSGHEADLEN, datalen)) {
        MDEBUG(DERR, "mNMSC_RecvData return fail.\n");
        SendErrMsg(pMsgBuff, ERR_LOGIN);
        continue;
      }

      cRecvBuffer[datalen + MSGHEADLEN] = 0;
      MDEBUG(DDEBUG, "RECV DATA:%s.\n", cRecvBuffer + MSGHEADLEN);

      if (0 == LoginMsgProc(pMsgBuff)) {
        cTC_Sta = TC_CONNECTED;
        MDEBUG(DDEBUG, "TC_CONNECTED\n");
      }
      else {
        SendErrMsg(pMsgBuff, ERR_LOGIN);
      }

      datalen = 0;
    }
    else if (cTC_Sta == TC_CONNECTED) {
      if ((pMsgBuff->type == TYPE_ACK) || (pMsgBuff->type == TYPE_ERROR)) {
        ListNode* pNode;
        int index = ntohl(*((int32_t*)pMsgBuff->Index));

        if (datalen > (SOCKETBUFFSIZE - MSGHEADLEN)) {
          MDEBUG(DERR, "datalen = %d.\n", datalen);
          continue;
        }

        if (datalen) {
          if (datalen != mNMSC_RecvData(cRecvBuffer + MSGHEADLEN, datalen)) {
            MDEBUG(DERR, "mNMSC_RecvData return fail.\n");
            continue;
          }

          cRecvBuffer[datalen + MSGHEADLEN] = 0;
          MDEBUG(DDEBUG, "RECV DATA:%s.\n", cRecvBuffer + MSGHEADLEN);
          s->proc(s, (SMSGBUFF*)cRecvBuffer);
        }

        for (pNode = pNmsClientList->head; pNode ; pNode = pNode->next) {
          if (((int64_t)pNode & 0xFFFFFFFF) == index) {
            break;
          }
        }

        if (pNode) {
          SMSGPACK* pMsg = MallocMsgPack(ntohl(*(int32_t*)pMsgBuff->DataLen));

          memcpy((char*)pMsg, (char*)pNode->data, sizeof(SMSGPACK));
          pMsg->SrcDevID = ((SMSGPACK*)(pNode->data))->DesDevID;
          pMsg->DesDevID = ((SMSGPACK*)(pNode->data))->SrcDevID;;
          pMsg->MsgType = (MSGTYPE)pMsgBuff->type;
          pMsg->Datalen = ntohl(*(int32_t*)pMsgBuff->DataLen);

          if (pMsg->Datalen) {
            memcpy(pMsg->Data, pMsgBuff->Data, pMsg->Datalen);
          }

          MDEBUG(DDEBUG, "xxxpMsg->Data [%s]\n", pMsg->Data);
          SendMsg2Main((char*)pMsg, GETMSGPACKLEN(pMsg->Datalen));
          free(pMsg);
        }
      }
      else {
        int len;
        char* pMsg = NULL;

        do {
          if (datalen <= 0) {
            MDEBUG(DERR, "datalen = %d\n", datalen);
            break;
          }

          pMsg = (char*)malloc(MSGHEADLEN + datalen);

          if (!pMsg) {
            MDEBUG(DERR, "malloc(%d) fail.\n", (int32_t)(MSGHEADLEN + datalen));
            break;
          }

          memcpy(pMsg, cRecvBuffer, MSGHEADLEN);
          ret = mNMSC_RecvData(pMsg + MSGHEADLEN, datalen);

          if (ret != datalen) {
            free(pMsg);
            MDEBUG(DERR, "mNMSC_RecvData return %d\n", ret);
            break;
          }

          MDEBUG(DDEBUG, "RECV DATA:%-.200s.\n", pMsg + MSGHEADLEN);
          len = s->proc(s, (SMSGBUFF*)pMsg);

          if (pMsg) {
            free(pMsg);
          }
        }
        while (0);

        datalen = 0;
      }

      listDelTimeOutNote(pNmsClientList, 180);
    }
  }

  return 0;
}

const char* cServerip = "127.0.0.1";
const char* cServerport = "60000";
static void* NMSC_MainThread(void* t)
{
  uint8_t errcnt = 0;
  ry_sock* s = (ry_sock*)t;

  //signal(SIGUSR1, ThreadExitHandle);
  while (1) {
    switch (cTC_Sta) {
    case TC_INIT:
      MDEBUG(DDEBUG, "TC_INIT\n");
      MDEBUG(DDEBUG, "Server Addr:%s:%s\n", cServerip, cServerport);
      cTC_Sta = TC_CONNECTING;
      break;

    case TC_CONNECTING:
      if (0 > NMSC_Connect(cServerip, cServerport)) {
        MDEBUG(DERR, "CONNECT Fail.\n");
        cTC_Sta = TC_INIT;
        sleep(5);
      }
      else {
        cTC_Sta = TC_LOGIN;
        errcnt = 0;
      }

      break;

    case TC_LOGIN:
      MDEBUG(DDEBUG, "TC_LOGIN\n");

      if (0 > SendLoginMsg(s)) {
        cTC_Sta = TC_INIT;
        _CloseSock();
      }

      errcnt++;

      if (errcnt >= 10) {
        cTC_Sta = TC_INIT;
        _CloseSock();
        MDEBUG(DDEBUG, "TC_LOGIN TimeOut\n");
      }

      sleep(3);
      break;

    case TC_CONNECTED:
      sleep(1);
      break;

    default:
      break;
    }
  }

  return NULL;
}

// pMsgPack->DesDevID:
// byte3: 主机ID,HOSTID_SERVER/HOSTID_LOCAL
// byte2: 应用模块
// byte1-0: 网络模块
int32_t NMSC_SendMsg(ry_sock* s, SMSGPACK* pMsgPack)
{
  SMSGBUFF* pMsgBuff;
  ListNode* pNode;
  const char* pDesGUID;
  int ret;
  int32_t index, desmod;

  //if(GETMODID(pMsgPack->DesDevID) != MOD_NMSCLIENTID) { return -1;}
  if ((pMsgPack->MsgType != MSG_ACK)
      && (pMsgPack->MsgType != MSG_ERR)
      && (pMsgPack->MsgType != MSG_REQACK)
      && (pMsgPack->MsgType != MSG_REQ)) {
    //MDEBUG(DWARNING,"MsgType(%d) is not suport.\n",pMsgPack->MsgType);
    //return -1;
  }

  pNode = listPushFront(pNmsClientList, (void*)pMsgPack, GETMSGPACKLEN(0));
  index = ((int64_t)pNode & 0xFFFFFFFF);
  pMsgBuff = (SMSGBUFF*)malloc(MSGHEADLEN + pMsgPack->Datalen + 1);
#if 0

  if (GETHOSTID(pMsgPack->DesDevID) == s->hostid) {
    pDesGUID = gServerGuid;
  }
  else
#endif
  {
    pDesGUID = gLocalGuid;
  }

  desmod = GETMODID(pMsgPack->DesDevID);

  if (!desmod) {
    desmod = 1;
  }

  SETMSGBUFFVALUE(pMsgBuff, (uint8_t)pMsgPack->MsgType,
      gLocalGuid, pDesGUID,
      s->modid, desmod,
      index,
      pMsgPack->Datalen, pMsgPack->Data);
  ret = _Send((char*)pMsgBuff, MSGHEADLEN + pMsgPack->Datalen);
  free((char*)pMsgBuff);

  if (ret != MSGHEADLEN + pMsgPack->Datalen) {
    MDEBUG(DERR, "_Send fail:%d\n", ret);
    listDelNote(pNmsClientList, pNode);
    return -1;
  }

  return 0;
}

// 初始化，设备打开，线程创建
ry_sock* NMSC_Init(int hostid, int modid, ProtocolMsgProc proc, void* user)
{
  MDEBUG(DINFO, "NMSC_ Init\n");
  ry_sock* s = NULL;
  s = (ry_sock*)malloc(sizeof(ry_sock));
  memset(s, 0, sizeof(ry_sock));
  s->hostid = hostid;
  s->modid = modid;
  s->proc = proc;
  s->user = user;

  // 创建互斥锁
  if (pthread_mutex_init(&gCMsgSendLock, NULL) != 0) {
    MDEBUG(DERR, "pthread_mutex_init:%s\n", strerror(errno));
    free(s);
    return NULL;
  }

  // 创建模块主线程
  if (pthread_create(&th_TCMain, NULL, NMSC_MainThread, s) != 0) {
    MDEBUG(DERR, "pthread_create:%s\n", strerror(errno));
    free(s);
    return NULL;
  }

  // 创建接收线程
  if (pthread_create(&th_TCRecv, NULL, NMSC_RecvThread, s) != 0) {
    MDEBUG(DERR, "pthread_create:%s\n", strerror(errno));
    free(s);
    return NULL;
  }

  pNmsClientList = listInit();
  return s;
}

#define SIGUSR1 0
// 退出
int32_t NMSC_Close(ry_sock* s)
{
  if (0 != pthread_kill(th_TCMain, SIGUSR1)) {
    MDEBUG(DERR, "Error cancelling thread.\r\n");
  }

  if (0 != pthread_kill(th_TCRecv, SIGUSR1)) {
    MDEBUG(DERR, "Error cancelling thread.\r\n");
  }

  pthread_join(th_TCMain, NULL);
  pthread_join(th_TCRecv, NULL);
  //模块退出销毁锁
  pthread_mutex_destroy(&gCMsgSendLock);
  listDestroy(pNmsClientList);
  cTC_Sta = TC_INIT;
  _CloseSock();
  free(s);
  return 0;
}

static int32_t NMSC_SendTo(ry_sock* s, int dst_hostid, int dst_modid, const char* data, int datalen)
{
  SMSGPACK* pDpmsg;
  MDEBUG(DINFO, "NMSC_SendTo %d %d\n", dst_hostid, dst_modid);
  pDpmsg = MallocMsgPack(datalen);
  memcpy(pDpmsg->Data, data, datalen);
  pDpmsg->SrcDevID    = SETDEVID(s->hostid, s->modid, 0);
  pDpmsg->DesDevID    = SETDEVID(dst_hostid, dst_modid, 0);
  pDpmsg->MsgType     = DEVDPCTRL;
  pDpmsg->Datalen     = datalen;
  pDpmsg->DataType    = TYPE_STRING;
  NMSC_SendMsg(s, pDpmsg);
  FreePointer(pDpmsg);
  return 0;
}
static int32_t NMSC_SendCmd(ry_sock* s, int dst_hostid, int dst_modid, const char* cmd) {
  int datalen = strlen(cmd);
  return NMSC_SendTo(s, dst_hostid, dst_modid, cmd, datalen);
}
