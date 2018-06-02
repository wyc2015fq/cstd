/****************************************************************************
Filename:
Description:
Author:
Date:
Modified History:
*****************************************************************************/

#include "common.h"
#include "queue.h"
#include "devMan.h"
#include "taskDebug.h"
#include "nmsClient.h"
#include "taskLocalMibee.h"
#include "jsDevParamConfig.h"
#include "taskTimed.h"
#include "taskPrinter.h"
#include "taskWifiProbe.h"
#include "taskMonitor.h"
#include "taskRemoteCtrl.h"
#include "taskScreenShot.h"

unsigned char g_cHostID = 0; // 主机ID

static pthread_mutex_t gMainMsgSendLock;
static queue_t* gpMsgList = NULL;


static ModuleReg gModule[] = {
  /*{ModeId,            pName,          moduleInit,             msgProc,                    moduleExit          },*/
  //{MOD_PRINTERID,     "Printer",      PrintModuleInit,        PrintModuleMsgProc,       PrintModuleExit     },
  //{MOD_SCANQRCODEID,  "ScanQRCode",   ScanQRCodeModuleInit,   ScanQRCodeModuleMsgProc,  ScanQRCodeModuleExit},
  {MOD_NMSCLIENTID,   "NMSClient",    NMSClientInit,          NMSClientMsgProc,         NMSClientExit       },
  //{MOD_LOCALID,       "LocalMibee",   LocalMibeeModuleInit,   NULL,                     LocalMibeeModuleExit},
  {MOD_TIMEDID,       "TimedTask",    TimedModuleInit,        TimedModuleMsgProc,       TimedModuleExit     },
  {MOD_WIFIPROBE,     "WifiProbe",    WifiProbeModuleInit,    NULL,                     WifiProbeModuleExit },
  {MOD_SYSMONITOR,    "SysMonitor",   MonitorModuleInit,      NULL,                     MonitorModuleExit },
  {MOD_REMOTECTRL,    "RemoteCtrl",   RemoteCtrlModuleInit,   RemoteCtrlModuleMsgProc,  RemoteCtrlModuleExit },
  {MOD_SCRRENSHOT,    "ScreenShot",   ScreenShotModuleInit,   ScreenShotModuleMsgProc,  ScreenShotModuleExit },
};
static int gModCnt = (sizeof(gModule) / sizeof(ModuleReg));

static int SendDpMsg2Dev(char* pMsg, int len)
{
  SMSGPACK* pDpMsg = (SMSGPACK*)pMsg;
  int i;

  for (i = 0; i < gModCnt; i++) {
    if (gModule[i].ModeId == GETMODID(pDpMsg->DesDevID)) {
      if (gModule[i].msgProc) {
        gModule[i].msgProc((void*)pMsg, len);
      }

      break;
    }
  }

  return 0;
}


int main(int argc, char* argv[])
{
  pthread_t th_Debug;
  SMSGPACK* pDpMsg = NULL;
  char pStr[4];
  memset(pStr, 0x00, 4);
  int ret;
  int i;
  (void)argc;
  (void)argv;

  /*
   * 设置调试打印等级
   */
  gcDbugLevel = DDEBUG;

  ret = JSDevParam_Open(CFGDIR"/DevParamConfig.json");

  if (ret < 0) {
    MDEBUG(DERR, "JSDevParam_Open return %d\n", ret);
  }

  ret = JSTimed_Open(CFGDIR"/TimedTask.json");

  if (ret < 0) {
    MDEBUG(DERR, "JSTimed_Open return %d\n", ret);
  }

  /*
   * 参数初始化
   */
  ret = JSDevParam_Get(0, "debuglevel", pStr, 4);

  if (ret >= 0) {
    gcDbugLevel = atoi(pStr);
    MDEBUG(DINFO, "Set gcDbugLevel = %d\n", gcDbugLevel);
  }

  signal(SIGPIPE, SIG_IGN);
  /*
   * 创建消息队列
   */
  gpMsgList = queue_new(1000);

  if (!gpMsgList) {
    MDEBUG(DALERT, "queue_new failed\n");
    exit(-1);
  }

  /*
   *  创建互斥锁
   */
  if (pthread_mutex_init(&gMainMsgSendLock, NULL) != 0) {
    MDEBUG(DERR, "pthread_mutex_init:%s\n", strerror(errno));
    exit(-1);
  }

  MDEBUG(DINFO, "Software version : %s\nHardware version : %s\nCompile time     : %s %s\n\n", DEVMANSOFTVER, DEVMANHARDVER, __DATE__, __TIME__);

  /*
   *  创建调试线程
   */
  if (pthread_create(&th_Debug, NULL, DebugThread, 0) != 0) {
    MDEBUG(DERR, "pthread_create:%s\n", strerror(errno));
  }

  /*
   *  初始化注册模块
   */
  for (i = 0; i < gModCnt; i++) {
    gModule[i].moduleInit(NULL);
  }

  while (1) {
    FreePointer(pDpMsg);
    /*接收消息队列*/
    ret = queue_pop(gpMsgList, (void**)&pDpMsg);

    if (ret < 0) {
      MDEBUG(DERR, "queue_pop error\n");
      continue;
    }

    MsgPackDebug(pDpMsg);

    if (GETMODID(pDpMsg->DesDevID) == MOD_MAIN) {
      switch (pDpMsg->MsgType) {
      case SYSEXIT:
        MDEBUG(DINFO, "App Exit Now...\n");
        sleep(1);
        goto HostExit;
        break;

      default:
        break;
      }
    }
    else if (pDpMsg->DesDevID) {
      SendDpMsg2Dev((char*)pDpMsg, GETMSGPACKLEN(pDpMsg->Datalen));
    }
  }

HostExit:

  if (0 != pthread_kill(th_Debug, SIGUSR1)) {
    MDEBUG(DERR, "Error cancelling thread.\r\n");
  }

  pthread_join(th_Debug, NULL);

  // 退出模块
  for (i = 0; i < gModCnt; i++) {
    if (gModule[i].moduleExit) {
      gModule[i].moduleExit();
    }
  }

  pthread_mutex_destroy(&gMainMsgSendLock);
  queue_delete(gpMsgList);
  JSDevParam_Close();

  return 0;
}


int SendMsg2Main(char* pData, int len)
{
  SMSGPACK* pMsg;
  int ret;

  pthread_mutex_lock(&gMainMsgSendLock);
  pMsg = malloc(len + 1);
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

  pthread_mutex_unlock(&gMainMsgSendLock);

  return ret;
}


