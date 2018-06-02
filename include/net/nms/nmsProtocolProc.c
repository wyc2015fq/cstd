#include <ctype.h>
#include <time.h>
#include <sys/reboot.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "common.h"
#include "cJSON.h"
#include "devMan.h"
#include "nmsProtocolProc.h"
#include "taskLocalMibee.h"
#include "jsDevParamConfig.h"

#define R_ROOT  0x01
#define R_ADMIN 0x02
#define R_USER  0x04

#define FILEMAXLEN (10*1024*1024)

int32_t  gTransaction = 0;

const char gServerGuid[16] = {0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x01
                             };

const char gLocalGuid[16]  = {0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
                             };

#define JsonSafeCheack(value,del,ret) if(!(value)) { \
    MDEBUG(DDEBUG,"Check false\n"); \
    if(del) { cJSON_Delete(del);} \
    return (ret);}

typedef struct __SNetCmd {
  char*    pCmd;
  int32_t(*MsgPro)(SMSGBUFF* pInMsg, cJSON* pJsInput, SMSGBUFF** pOutMsg);
} SNetCmd;

static int32_t mRebootapp(SMSGBUFF* pInMsg, cJSON* pJsInput, SMSGBUFF** pOutPut)
{
  SMSGPACK* pDpmsg;

  if (!*pOutPut) {
    *pOutPut = (SMSGBUFF*)malloc(MSGHEADLEN + 200);

    if (!*pOutPut) {
      MDEBUG(DERR, "malloc fail.\n");
      return -1;
    }
  }

  sprintf((*pOutPut)->Data, "{\"rebootapp\":\"ok\"}");

  pDpmsg = MallocMsgPack(0);
  pDpmsg->SrcDevID    = SETDEVID(HOSTID_SERVER, MOD_NMSCLIENTID, 0);
  pDpmsg->DesDevID    = SETDEVID(HOSTID_LOCAL, MOD_MAIN, 0);
  pDpmsg->MsgType     = SYSEXIT;
  pDpmsg->Datalen     = 0;
  pDpmsg->DataType    = TYPE_NULL;
  SendMsg2Main((char*)pDpmsg, GETMSGPACKLEN(pDpmsg->Datalen));
  FreePointer(pDpmsg);

  return strlen((*pOutPut)->Data);
}
/*
static int32_t mReboot(SMSGBUFF *pInMsg, cJSON *pJsInput, SMSGBUFF **pOutPut)
{
    system("reboot");
    return 0;
}*/

static int32_t mPrinter1(SMSGBUFF* pInMsg, cJSON* pJsInput, SMSGBUFF** pOutPut)
{
  SMSGPACK* pDpmsg;

  pDpmsg = MallocMsgPack(0);//strlen(pJsItem->valuestring)
  pDpmsg->SrcDevID    = SETDEVID(HOSTID_SERVER, MOD_NMSCLIENTID, 0);
  pDpmsg->DesDevID    = SETDEVID(HOSTID_LOCAL, MOD_PRINTERID, 1);
  pDpmsg->MsgType       = DEVDPCTRL;
  pDpmsg->DataType    = TYPE_STRING;
  //sprintf(pDpmsg->Data,"\"%s\"",pJsItem->valuestring);
  pDpmsg->Datalen     = strlen(pDpmsg->Data);

  SendMsg2Main((char*)pDpmsg, GETMSGPACKLEN(pDpmsg->Datalen));
  FreePointer(pDpmsg);
  return 0;
}

static int32_t mPrinter1status(SMSGBUFF* pInMsg, cJSON* pJsInput, SMSGBUFF** pOutPut)
{
  SMSGPACK* pDpmsg;

  pDpmsg = MallocMsgPack(0);//strlen(pJsItem->valuestring)
  pDpmsg->SrcDevID    = SETDEVID(HOSTID_SERVER, MOD_NMSCLIENTID, 0);
  pDpmsg->DesDevID    = SETDEVID(HOSTID_LOCAL, MOD_PRINTERID, 1);
  pDpmsg->MsgType       = DEVDPCTRL;
  pDpmsg->DataType    = TYPE_STRING;
  //sprintf(pDpmsg->Data,"\"%s\"",pJsItem->valuestring);
  pDpmsg->Datalen     = strlen(pDpmsg->Data);

  SendMsg2Main((char*)pDpmsg, GETMSGPACKLEN(pDpmsg->Datalen));
  FreePointer(pDpmsg);
  return 0;
}

static int32_t mPrinter2(SMSGBUFF* pInMsg, cJSON* pJsInput, SMSGBUFF** pOutPut)
{
  SMSGPACK* pDpmsg;


  pDpmsg = MallocMsgPack(0);//strlen(pJsItem->valuestring)
  pDpmsg->SrcDevID    = SETDEVID(HOSTID_SERVER, MOD_NMSCLIENTID, 0);
  pDpmsg->DesDevID    = SETDEVID(HOSTID_LOCAL, MOD_PRINTERID, 2);
  pDpmsg->MsgType       = DEVDPCTRL;
  pDpmsg->DataType    = TYPE_STRING;
  //sprintf(pDpmsg->Data,"\"%s\"",pJsItem->valuestring);
  pDpmsg->Datalen     = strlen(pDpmsg->Data);

  SendMsg2Main((char*)pDpmsg, GETMSGPACKLEN(pDpmsg->Datalen));
  FreePointer(pDpmsg);
  return 0;
}

static int32_t mPrinter2status(SMSGBUFF* pInMsg, cJSON* pJsInput, SMSGBUFF** pOutPut)
{
  SMSGPACK* pDpmsg;

  pDpmsg = MallocMsgPack(0);//strlen(pJsItem->valuestring)
  pDpmsg->SrcDevID    = SETDEVID(HOSTID_SERVER, MOD_NMSCLIENTID, 0);
  pDpmsg->DesDevID    = SETDEVID(HOSTID_LOCAL, MOD_PRINTERID, 2);
  pDpmsg->MsgType       = DEVDPCTRL;
  pDpmsg->DataType    = TYPE_STRING;
  //sprintf(pDpmsg->Data,"\"%s\"",pJsItem->valuestring);
  pDpmsg->Datalen     = strlen(pDpmsg->Data);

  SendMsg2Main((char*)pDpmsg, GETMSGPACKLEN(pDpmsg->Datalen));
  FreePointer(pDpmsg);
  return 0;
}

static int32_t mZbarcam(SMSGBUFF* pInMsg, cJSON* pJsInput, SMSGBUFF** pOutPut)
{
  SMSGPACK* pDpmsg;

  pDpmsg = MallocMsgPack(21);
  //sprintf(pDpmsg->Data,"%d",pJsItem->valueint);
  pDpmsg->SrcDevID    = SETDEVID(HOSTID_SERVER, MOD_NMSCLIENTID, 0);
  pDpmsg->DesDevID    = SETDEVID(HOSTID_LOCAL, MOD_READQRCODEID, 1);
  pDpmsg->MsgType       = DEVDPCTRL;
  pDpmsg->Datalen     = strlen(pDpmsg->Data);
  pDpmsg->DataType    = TYPE_STRING;
  SendMsg2Main((char*)pDpmsg, GETMSGPACKLEN(pDpmsg->Datalen));
  FreePointer(pDpmsg);

  return 0;
}

static int32_t mRebootSys(SMSGBUFF* pInMsg, cJSON* pJsInput, SMSGBUFF** pOutPut)
{
  MDEBUG(DINFO, "mRebootSys Init\n");

  sleep(3);

  sync();
  reboot(RB_AUTOBOOT);

  return 0;
}

static int32_t mScreenShot(SMSGBUFF* pInMsg, cJSON* pJsInput, SMSGBUFF** pOutPut)
{
  MDEBUG(DINFO, "mScreenShot Init\n");
  SMSGPACK* pDpmsg;

  pDpmsg = MallocMsgPack(0);
  pDpmsg->SrcDevID    = SETDEVID(HOSTID_SERVER, MOD_NMSCLIENTID, 0);
  pDpmsg->DesDevID    = SETDEVID(HOSTID_LOCAL, MOD_SCRRENSHOT, 1);
  pDpmsg->MsgType     = DEVDPCTRL;
  pDpmsg->Datalen     = strlen(pDpmsg->Data);
  pDpmsg->DataType    = TYPE_STRING;
  SendMsg2Main((char*)pDpmsg, GETMSGPACKLEN(pDpmsg->Datalen));
  FreePointer(pDpmsg);

  return 0;
}

static int32_t mFilePush(SMSGBUFF* pInMsg, cJSON* pJsInput, SMSGBUFF** pOutPut)
{
  MDEBUG(DINFO, "mFilePush Init\n");

  SMSGPACK* pDpmsg;

  int datalen = ntohl(*(int32_t*)pInMsg->DataLen);
  pDpmsg = MallocMsgPack(datalen + 1);
  snprintf(pDpmsg->Data, datalen + 1, "%s", pInMsg->Data);
  pDpmsg->SrcDevID    = SETDEVID(HOSTID_SERVER, MOD_NMSCLIENTID, 0);
  pDpmsg->DesDevID    = SETDEVID(HOSTID_LOCAL, MOD_SCRRENSHOT, 2);
  pDpmsg->MsgType     = DEVDPCTRL;
  pDpmsg->Datalen     = datalen;
  pDpmsg->DataType    = TYPE_STRING;
  SendMsg2Main((char*)pDpmsg, GETMSGPACKLEN(pDpmsg->Datalen));
  FreePointer(pDpmsg);

  return 0;
}

static int32_t mTimerPowerOn(SMSGBUFF* pInMsg, cJSON* pJsInput, SMSGBUFF** pOutPut)
{
  MDEBUG(DINFO, "mTimerPowerOn Init\n");
  SMSGPACK* pDpmsg;

  int datalen = ntohl(*(int32_t*)pInMsg->DataLen);
  int srcModID = ((pInMsg)->SrcModID[0] << 8L) + (pInMsg)->SrcModID[1];

  pDpmsg = MallocMsgPack(datalen + 1);
  snprintf(pDpmsg->Data, datalen + 1, "%s", pInMsg->Data);

  if (0 == pInMsg->SrcDevID[15]) {
    pDpmsg->SrcDevID    = SETDEVID(HOSTID_LOCAL, MOD_NMSCLIENTID, srcModID);
  }
  else {
    pDpmsg->SrcDevID    = SETDEVID(HOSTID_SERVER, MOD_NMSCLIENTID, srcModID);
  }

  pDpmsg->DesDevID    = SETDEVID(HOSTID_LOCAL, MOD_REMOTECTRL, 1);
  pDpmsg->MsgType     = DEVDPCTRL;
  pDpmsg->Datalen     = datalen;
  pDpmsg->DataType    = TYPE_JSON;
  SendMsg2Main((char*)pDpmsg, GETMSGPACKLEN(pDpmsg->Datalen));
  FreePointer(pDpmsg);

  return 0;
}

static int32_t mTimerPowerOff(SMSGBUFF* pInMsg, cJSON* pJsInput, SMSGBUFF** pOutPuta)
{
  MDEBUG(DINFO, "mTimerPowerOff Init\n");
  SMSGPACK* pDpmsg;
  int datalen = ntohl(*(int32_t*)pInMsg->DataLen);
  int srcModID = ((pInMsg)->SrcModID[0] << 8L) + (pInMsg)->SrcModID[1];

  pDpmsg = MallocMsgPack(datalen + 1);
  snprintf(pDpmsg->Data, datalen + 1, "%s", pInMsg->Data);

  if (0 == pInMsg->SrcDevID[15]) {
    pDpmsg->SrcDevID    = SETDEVID(HOSTID_LOCAL, MOD_NMSCLIENTID, srcModID);
  }
  else {
    pDpmsg->SrcDevID    = SETDEVID(HOSTID_SERVER, MOD_NMSCLIENTID, srcModID);
  }

  pDpmsg->DesDevID    = SETDEVID(HOSTID_LOCAL, MOD_REMOTECTRL, 2);
  pDpmsg->MsgType     = DEVDPCTRL;
  pDpmsg->Datalen     = datalen;
  pDpmsg->DataType    = TYPE_JSON;
  SendMsg2Main((char*)pDpmsg, GETMSGPACKLEN(pDpmsg->Datalen));
  FreePointer(pDpmsg);

  return 0;
}

static SNetCmd RegistNetCmd [] = {
  {"rebootapp",               mRebootapp,      },
  //{"rebootsys",               mReboot,         },
  {"printer1",                mPrinter1,       },
  {"printer1status",          mPrinter1status, },
  {"printer2",                mPrinter2,       },
  {"printer2status",          mPrinter2status, },
  {"zbarcam",                 mZbarcam,        },
  {"rebootsys",               mRebootSys,      },
  {"screenshot",              mScreenShot,     },
  {"timerpoweron",            mTimerPowerOn,   },
  {"timerpoweroff",           mTimerPowerOff,  },
  {"file_push",               mFilePush,       },
};

static int32_t mProtocolMsgProc(SMSGBUFF* pInMsg, SMSGBUFF** ppOutMsg)
{
  if (NULL == pInMsg) {
    return -1;
  }

  cJSON* pJsData, *pJsItem;
  int i;
  int ret = -1;

  pJsData = cJSON_Parse(pInMsg->Data);

  if (!pJsData) {
    return -1;
  }

  do {
    pJsItem = cJSON_GetObjectItem(pJsData, "cmd");

    if (!pJsItem || !pJsItem->string) {
      break;
    }

    for (i = 0; i < (sizeof(RegistNetCmd) / sizeof(SNetCmd)); i++) {
      if (strcmp(pJsItem->valuestring, RegistNetCmd[i].pCmd)) {
        continue;
      }

      ret = RegistNetCmd[i].MsgPro(pInMsg, pJsData, ppOutMsg);
      break;
    }
  }
  while (0);

  if (pJsData) {
    cJSON_Delete(pJsData);
  }

  if ((ret > 0) && *ppOutMsg) {
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

  return ret;
}

int32_t ProtocolMsgProc(SMSGBUFF* pInMsg, SMSGBUFF** ppOutMsg)
{
  int32_t ret;
  static char initsta = 1;
  static pthread_mutex_t ptProcLock;

  if (initsta) {
    pthread_mutex_init(&ptProcLock, NULL);
    initsta = 0;
  }

  pthread_mutex_lock(&ptProcLock);
  ret = mProtocolMsgProc(pInMsg, ppOutMsg);
  pthread_mutex_unlock(&ptProcLock);

  return ret;
}


int32_t MsgPackHeadSwap(SMSGBUFF* pInMsg, SMSGBUFF* pOutMsg)
{
  memcpy((char*)pOutMsg, (char*)pInMsg, MSGHEADLEN);
  /*
   * SGUID
   */
  memcpy(pOutMsg->SrcDevID, pInMsg->DesDevID, 16);
  /*
   * DGUID
   */
  memcpy(pOutMsg->DesDevID, pInMsg->SrcDevID, 16);
  /*
   * SMOD
   */
  memcpy(pOutMsg->SrcModID, pInMsg->DesModID, 2);
  /*
   * DMOD
   */
  memcpy(pOutMsg->DesModID, pInMsg->SrcModID, 2);

  /*
   * len
   */
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


