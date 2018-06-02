// CPMDev.cpp : Defines the entry point for the DLL application.
//
#include "DspDevManager.h"
#include <assert.h>
DspDevManager* gDspDevManager = NULL;
BOOL CPMDEV_CALL CPM_InitSys(BOOL bFlag)
{
  BOOL ret = FALSE;

  if (bFlag) {
    if (NULL == gDspDevManager) {
      gDspDevManager = new DspDevManager;
      assert(gDspDevManager);
      ret = gDspDevManager->InitSys(bFlag);
    }
    else {
      ret = TRUE;
    }
  }
  else {
    ret = TRUE;

    if (gDspDevManager) {
      gDspDevManager->InitSys(bFlag);
      delete gDspDevManager;
      gDspDevManager = NULL;
    }
  }

  return ret;
}
void CPMDEV_CALL CPM_RegDevConnectStatusCB(DevConnectStatus cb, void* param)//注册设备连接状态提醒回调
{
  if (! gDspDevManager) {
    return ;
  }

  assert(gDspDevManager);
  gDspDevManager->RegNetCNNotify(cb, param);
  return ;
}
void CPMDEV_CALL CPM_RegOperResultNotifyCB(DevOperResultNotify cb, void* param)
{
  if (! gDspDevManager) {
    return ;
  }

  assert(gDspDevManager);
  gDspDevManager->RegDevOPNotify(cb, param);
  return ;
}
BOOL CPMDEV_CALL CPM_SetMode(BOOL srvMode, QuerySrvCB pfn, void* pvContext)
{
  if (! gDspDevManager) {
    return FALSE;
  }

  assert(gDspDevManager);
  return gDspDevManager->SetMode(srvMode, pfn, pvContext);
}
BOOL CPMDEV_CALL CPM_CnSrv(BOOL bFlag, const char* ip, const char* name, const char* psw)
{
  if (! gDspDevManager) {
    return FALSE;
  }

  assert(gDspDevManager);
  return gDspDevManager->CnSrv(bFlag, ip, name, psw);
}
BOOL CPMDEV_CALL CPM_StartDevSerch(BOOL flag, const char* dev_region_addr)//启动设备搜索与否
{
  if (! gDspDevManager) {
    return FALSE;
  }

  assert(gDspDevManager);
  return gDspDevManager->DevSerch(flag, dev_region_addr);
}
BOOL CPMDEV_CALL CPM_CNDev(const char* cszDevAddr) //手动连接设备
{
  if (! gDspDevManager) {
    return FALSE;
  }

  assert(gDspDevManager);
  //DEBUG_INFO1("CPM_CNDev IP = %s" , cszDevAddr);
  return gDspDevManager->CNDev(cszDevAddr);
}
void CPMDEV_CALL CPM_DCNDev(const char* cszDevAddr) //断开设备连接
{
  if (! gDspDevManager) {
    return ;
  }

  assert(gDspDevManager);
  gDspDevManager->DCNDev(cszDevAddr);
  return ;
}
#define CPM_APIDEF_IMPL(fun) { CSock* s = gDspDevManager->getSock(cszDevAddr); if (s) { return s->fun; } else {return FALSE;} }
#define CPM_APIDEF(ret, nameargs, fun) ret CPMDEV_CALL nameargs {assert(gDspDevManager); CPM_APIDEF_IMPL(fun) }
CPM_APIDEF(BOOL, CPM_ULDevAuth(const char* cszDevAddr, const DEV_CID& user_name, const DEV_CID& pws, int flag, int nSeq) , Auth(user_name, pws, flag, nSeq));
CPM_APIDEF(BOOL, CPM_ULDevRegionAddr(const char* cszDevAddr, const char* dev_region_addr, int nSeq) , SetDevRgn(dev_region_addr, nSeq));
CPM_APIDEF(BOOL, CPM_ULDevTime(const char* cszDevAddr, const DEV_DATETIME& dev_time, int nSeq) , SetDevTime(dev_time, nSeq)); //设置设备时间
CPM_APIDEF(BOOL, CPM_DLDevTime(const char* cszDevAddr, int nSeq) , GetDevTime(nSeq)); //获取设备时间
CPM_APIDEF(BOOL, CPM_DLDevBaseInfo(const char* cszDevAddr, int nSeq) , GetDevInfo(nSeq)); //获取设备基本信息
CPM_APIDEF(BOOL, CPM_DLDevStatisInfo(const char* cszDevAddr, int nSeq) , GetDevStatis(nSeq)); //获取设备统计信息
CPM_APIDEF(BOOL, CPM_DLDevWorkAttInfo(const char* cszDevAddr, int nSeq) , GetDevWorkAtt(nSeq)); //获取设备工作属性信息
CPM_APIDEF(BOOL, CPM_ULDevWorkAttInfo(const char* cszDevAddr, const DEV_WORKATT& att, int nSeq), SetDevWorkAtt(att, nSeq)); //设置设备工作属性信息
CPM_APIDEF(BOOL, CPM_ULUser(const char* cszDevAddr, const DEV_USER& user, int nSeq), AddDevUser(user, nSeq)); //上传用户
CPM_APIDEF(BOOL, CPM_ULRealTimeUser(const char* cszDevAddr, const DEV_USER& user, const DEV_USER_REAL_LOAD& rt_load), AddDevRealTimeUser(user, rt_load)); //
CPM_APIDEF(BOOL, CPM_ULRealTimeUserCap(const char* cszDevAddr, const DEV_USER& cUser, int nSeq), RealTimeUserCap(cUser, nSeq)); //
CPM_APIDEF(BOOL, CPM_DELUser(const char* cszDevAddr, const DEV_CID& user_id, int nSeq), DelDevUser(user_id, nSeq)); //删除某以用户
BOOL CPMDEV_CALL CPM_DLSingleUser(const char* cszDevAddr, const DEV_CID& user_id, int flag, int nSeq)
{
  if (! gDspDevManager) {
    return FALSE;
  }

  assert(gDspDevManager);
  int szBuf[sizeof(DEV_DATA_DL) + sizeof(DEV_CID)] = {0};
  DEV_DATA_DL& dl = *(DEV_DATA_DL*)szBuf ;
  dl.m_DLType = DL_SINGLE;
  dl.m_DLFlag = flag;
  memcpy(dl.m_DLContent, &user_id, sizeof(DEV_CID));
  CPM_APIDEF_IMPL(GetDevUser(dl, nSeq));
}
BOOL CPMDEV_CALL CPM_DLAllUser(const char* cszDevAddr, int nSeq)//下载所有用户
{
  if (! gDspDevManager) {
    return FALSE;
  }

  assert(gDspDevManager);
  DEV_DATA_DL dl ;
  memset(&dl, 0x0, sizeof(dl)) ;
  dl.m_DLType = DL_ALL;
  CPM_APIDEF_IMPL(GetDevUser(dl, nSeq));
}
BOOL CPMDEV_CALL CPM_DLSegTimeUser(const char* cszDevAddr, const DEV_DATE& start, const DEV_DATE& end , int nSeq)//下载某一时间段内的注册用户
{
  if (! gDspDevManager) {
    return FALSE;
  }

  assert(gDspDevManager);
  int szBuf[sizeof(DEV_DATA_DL) + sizeof(DEV_TIMESEG)] = {0};
  DEV_DATA_DL& dl = *(DEV_DATA_DL*)szBuf;
  dl.m_DLType = DL_SEGTIME;
  DEV_TIMESEG seg = {0};
  seg.m_Start = start;
  seg.m_End = end;
  memcpy(dl.m_DLContent, &seg, sizeof(DEV_TIMESEG));
  CPM_APIDEF_IMPL(GetDevUser(dl, nSeq));
}
BOOL CPMDEV_CALL CPM_DLAllIdentifyRecord(const char* cszDevAddr, int nSeq)//下载所有识别记录
{
  if (! gDspDevManager) {
    return FALSE;
  }

  assert(gDspDevManager);
  DEV_DATA_DL dl;
  memset(&dl, 0x0, sizeof(dl));
  dl.m_DLType = DL_ALL;
  CPM_APIDEF_IMPL(GetDevRecord(CMD_M_VERIFY, dl, nSeq));
}
BOOL CPMDEV_CALL CPM_DLSegTimeIdentifyRecord(const char* cszDevAddr, const DEV_DATE& start, const DEV_DATE& end, int nSeq)//下载指定时间段识别记录
{
  if (! gDspDevManager) {
    return FALSE;
  }

  assert(gDspDevManager);
  int szBuf[sizeof(DEV_DATA_DL) + sizeof(DEV_TIMESEG)] = {0};
  DEV_DATA_DL& dl = *(DEV_DATA_DL*)szBuf;
  dl.m_DLType = DL_SEGTIME;
  dl.m_DLFlag = 0;
  DEV_TIMESEG seg = {0};
  seg.m_Start = start;
  seg.m_End = end;
  memcpy(dl.m_DLContent, &seg, sizeof(DEV_TIMESEG));
  CPM_APIDEF_IMPL(GetDevRecord(CMD_M_VERIFY, dl, nSeq));
}
BOOL CPMDEV_CALL CPM_DLRangeRec(const char* cszDevAddr, const DEV_DL_RECORD_RANGE& cRange , int nFlag, BOOL bKnowRange , int nSeq)
{
  if (! gDspDevManager) {
    return FALSE;
  }

  assert(gDspDevManager);
  int szBuf[sizeof(DEV_DATA_DL) + sizeof(DEV_DL_RECORD_RANGE)] = {0};
  DEV_DATA_DL& dl = *(DEV_DATA_DL*)szBuf;
  dl.m_DLFlag = nFlag;
  dl.m_DLType = bKnowRange ? DL_SINGLE : DL_RANGE;
  memcpy(dl.m_DLContent, &cRange, sizeof(DEV_DL_RECORD_RANGE));
  CPM_APIDEF_IMPL(GetDevRecord(CMD_M_VERIFY, dl, nSeq));
}
BOOL CPMDEV_CALL CPM_DLAllAlarmRecord(const char* cszDevAddr, int nSeq)//下载所有报警记录
{
  if (! gDspDevManager) {
    return FALSE;
  }

  assert(gDspDevManager);
  DEV_DATA_DL dl ;
  memset(&dl, 0x0, sizeof(dl)) ;
  dl.m_DLType = DL_ALL;
  dl.m_DLFlag = 0;
  CPM_APIDEF_IMPL(GetDevRecord(CMD_M_ALARM, dl, nSeq));
}
BOOL CPMDEV_CALL CPM_DLSegTimeAlarmRecord(const char* cszDevAddr, const DEV_DATE& start, const DEV_DATE& end, int nSeq)//下载指定时间段报警记录
{
  if (! gDspDevManager) {
    return FALSE;
  }

  assert(gDspDevManager);
  char szBuf[sizeof(DEV_DATA_DL) + sizeof(DEV_TIMESEG)] = {0};
  DEV_DATA_DL& dl = *(DEV_DATA_DL*)szBuf;
  dl.m_DLType = DL_SEGTIME;
  dl.m_DLFlag = 0;
  DEV_TIMESEG seg = {0};
  seg.m_Start = start;
  seg.m_End = end;
  memcpy(dl.m_DLContent, &seg, sizeof(DEV_TIMESEG));
  CPM_APIDEF_IMPL(GetDevRecord(CMD_M_ALARM, dl, nSeq));
}
BOOL CPMDEV_CALL CPM_DLRangeAlarm(const char* cszDevAddr, const DEV_DL_RECORD_RANGE& cRange , int nFlag, BOOL bKnowRange, int nSeq)
{
  if (! gDspDevManager) {
    return FALSE;
  }

  assert(gDspDevManager);
  char szBuf[sizeof(DEV_DATA_DL) + sizeof(DEV_DL_RECORD_RANGE)] = {0};
  DEV_DATA_DL& dl = *(DEV_DATA_DL*)szBuf;
  dl.m_DLFlag = nFlag;
  dl.m_DLType = bKnowRange ? DL_SINGLE : DL_RANGE;
  memcpy(dl.m_DLContent, &cRange, sizeof(DEV_DL_RECORD_RANGE));
  CPM_APIDEF_IMPL(GetDevRecord(CMD_M_ALARM, dl, nSeq));
}
BOOL CPMDEV_CALL CPM_DLLog(const char* cszDevAddr, int nSeq)
{
  if (! gDspDevManager) {
    return FALSE;
  }

  assert(gDspDevManager);
  int szBuf[sizeof(DEV_DATA_DL) + sizeof(DEV_DL_RECORD_RANGE)] = {0};
  DEV_DATA_DL& dl = *(DEV_DATA_DL*)szBuf;
  dl.m_DLType = DL_ALL;
  CPM_APIDEF_IMPL(GetDevRecord(CMD_M_OPTLOG, dl, nSeq));
}
BOOL CPMDEV_CALL CPM_DLRangeLog(const char* cszDevAddr, const DEV_DL_RECORD_RANGE& cRange , BOOL bKnowRange, int nSeq)
{
  if (! gDspDevManager) {
    return FALSE;
  }

  assert(gDspDevManager);
  int szBuf[sizeof(DEV_DATA_DL) + sizeof(DEV_DL_RECORD_RANGE)] = {0};
  DEV_DATA_DL& dl = *(DEV_DATA_DL*)szBuf;
  dl.m_DLFlag = 0;
  dl.m_DLType = bKnowRange ? DL_SINGLE : DL_RANGE;
  memcpy(dl.m_DLContent, &cRange, sizeof(DEV_DL_RECORD_RANGE));
  CPM_APIDEF_IMPL(GetDevRecord(CMD_M_OPTLOG, dl, nSeq));
}
BOOL CPMDEV_CALL CPM_DLChangeImage(const char* cszDevAddr, int nSeq)
{
  if (! gDspDevManager) {
    return FALSE;
  }

  assert(gDspDevManager);
  char szBuf[sizeof(DEV_DATA_DL) + sizeof(DEV_DL_RECORD_RANGE)] = {0};
  DEV_DATA_DL& dl = *(DEV_DATA_DL*)szBuf;
  dl.m_DLType = DL_ALL;
  CPM_APIDEF_IMPL(GetDevRecord(CMD_M_CIMAGE, dl, nSeq));
}
BOOL CPMDEV_CALL CPM_DLRangeCImage(const char* cszDevAddr, const DEV_DL_RECORD_RANGE& cRange , int nFlag, BOOL bKnowRange, int nSeq)
{
  if (! gDspDevManager) {
    return FALSE;
  }

  assert(gDspDevManager);
  char szBuf[sizeof(DEV_DATA_DL) + sizeof(DEV_DL_RECORD_RANGE)] = {0};
  DEV_DATA_DL& dl = *(DEV_DATA_DL*)szBuf;
  dl.m_DLFlag = nFlag;
  dl.m_DLType = bKnowRange ? DL_SINGLE : DL_RANGE;
  memcpy(dl.m_DLContent, &cRange, sizeof(DEV_DL_RECORD_RANGE));
  CPM_APIDEF_IMPL(GetDevRecord(CMD_M_CIMAGE, dl, nSeq));
}
BOOL CPMDEV_CALL CPM_ULTimeGroup(const char* cszDevAddr, const DEV_TIMEGROUP_ARRAY& tg_array, int nSeq)
{
  if (! gDspDevManager) {
    return FALSE;
  }

  assert(gDspDevManager);
  CPM_APIDEF_IMPL(SetDevTG(tg_array, nSeq));
}
BOOL CPMDEV_CALL CPM_DLTimeGroup(const char* cszDevAddr, const DEV_TIMEGROUP_ARRAY& tg_array, DEV_TIMEGROUP_TYPE tg_type, int nSeq)
{
  if (! gDspDevManager) {
    return FALSE;
  }

  assert(gDspDevManager);
  CPM_APIDEF_IMPL(GetDevTG(tg_array, tg_type, nSeq));
}
BOOL CPMDEV_CALL CPM_DELTimeGroup(const char* cszDevAddr, const DEV_TIMEGROUP_ARRAY& tg_array, DEV_TIMEGROUP_TYPE tg_type, int nSeq)
{
  if (! gDspDevManager) {
    return FALSE;
  }

  assert(gDspDevManager);
  CPM_APIDEF_IMPL(DelDevTG(tg_array, tg_type, nSeq));
}
BOOL CPMDEV_CALL CPM_DLRight(const char* cszDevAddr, const DEV_RIGHT_ARRAY& rt_array, int nSeq)
{
  if (! gDspDevManager) {
    return FALSE;
  }

  assert(gDspDevManager);
  CPM_APIDEF_IMPL(GetDevRT(rt_array, nSeq));
}
BOOL CPMDEV_CALL CPM_ULRight(const char* cszDevAddr, const DEV_RIGHT_ARRAY& rt_array, int nSeq)
{
  if (! gDspDevManager) {
    return FALSE;
  }

  assert(gDspDevManager);
  CPM_APIDEF_IMPL(SetDevRT(rt_array, nSeq));
}
BOOL CPMDEV_CALL CPM_DELRight(const char* cszDevAddr, const DEV_RIGHT_ARRAY& rt_array, int nSeq)
{
  if (! gDspDevManager) {
    return FALSE;
  }

  assert(gDspDevManager);
  CPM_APIDEF_IMPL(DelDevRT(rt_array, nSeq));
}
BOOL CPMDEV_CALL CPM_ULUserGroup(const char* cszDevAddr, const DEV_USERGROUP_ARRAY& ug_array, int nSeq)
{
  if (! gDspDevManager) {
    return FALSE;
  }

  assert(gDspDevManager);
  CPM_APIDEF_IMPL(SetDevUserGroup(ug_array, nSeq));
}
BOOL CPMDEV_CALL CPM_DLUserGroup(const char* cszDevAddr, const DEV_USERGROUP_ARRAY& ug_array, int nSeq)
{
  if (! gDspDevManager) {
    return FALSE;
  }

  assert(gDspDevManager);
  CPM_APIDEF_IMPL(GetDevUserGroup(ug_array, nSeq));
}
BOOL CPMDEV_CALL CPM_DELUserGroup(const char* cszDevAddr, const DEV_USERGROUP_ARRAY& ug_array, int nSeq)
{
  if (! gDspDevManager) {
    return FALSE;
  }

  assert(gDspDevManager);
  CPM_APIDEF_IMPL(DelDevUserGroup(ug_array, nSeq));
}
BOOL CPMDEV_CALL CPM_ULOpenDoorState(const char* cszDevAddr, DEV_DOOR_STATE eState, int nSeq)
{
  if (! gDspDevManager) {
    return FALSE;
  }

  assert(gDspDevManager);
  CPM_APIDEF_IMPL(SetOpenDoorState(eState, nSeq));
}
BOOL CPMDEV_CALL CPM_DLOpenDoorState(const char* cszDevAddr, int nSeq /*= -1*/)
{
  if (! gDspDevManager) {
    return FALSE;
  }

  assert(gDspDevManager);
  CPM_APIDEF_IMPL(GetOpenDoorState(nSeq));
}
BOOL CPMDEV_CALL CPM_ULRemoteOpenDoor(const char* cszDevAddr, BOOL bOpen, int nSeq)
{
  if (! gDspDevManager) {
    return FALSE;
  }

  assert(gDspDevManager);
  CPM_APIDEF_IMPL(RemoteOpenDoor(bOpen, nSeq));
}
BOOL CPMDEV_CALL CPM_IOCtrl(const char* cszDevAddr, const DEV_IOCTRL& ioctrl, int nSeq)
{
  if (! gDspDevManager) {
    return FALSE;
  }

  assert(gDspDevManager);
  CPM_APIDEF_IMPL(IOCtrl(ioctrl, nSeq));
}
BOOL CPMDEV_CALL CPM_ULFormat(const char* cszDevAddr, int nSeq)
{
  if (! gDspDevManager) {
    return FALSE;
  }

  assert(gDspDevManager);
  CPM_APIDEF_IMPL(FormatDev(nSeq));
}
#pragma pack(1)
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
#ifdef WIN32
typedef unsigned __int64 uint64_t;
#else
//typedef unsigned long long uint64_t;
#endif
typedef struct {
  uint8_t pos;
  uint32_t dict_size;
  uint64_t dst_size;
} lzma_header_t;
#pragma pack()
BOOL CPMDEV_CALL CPM_ULUpdate(const char* cszDevAddr, const DEV_APPUPDATE& data, int nSeq)
{
  if (! gDspDevManager) {
    return FALSE;
  }

  assert(gDspDevManager);
  CPM_APIDEF_IMPL(UpdateApp(data, nSeq));
}
BOOL CPMDEV_CALL CPM_StartVideo(const char* cszDevAddr, void* hwnd, VideoPro_CallBack VideoCB, int nSeq)
{
  if (! gDspDevManager) {
    return FALSE;
  }

  assert(gDspDevManager);
  CPM_APIDEF_IMPL(StartVideo(hwnd, VideoCB, nSeq));
}
BOOL CPMDEV_CALL CPM_StopVideo(const char* cszDevAddr, int nSeq)
{
  if (! gDspDevManager) {
    return FALSE;
  }

  assert(gDspDevManager);
  CPM_APIDEF_IMPL(StopVideo(nSeq));
}
BOOL CPMDEV_CALL CPM_ULUserEx(const char* cszDevAddr, const DEV_BATCH_USER& cUsers, int nFlag, int nSeq)
{
  if (! gDspDevManager) {
    return FALSE;
  }

  CPM_APIDEF_IMPL(ULUserEx(cUsers, nFlag, nSeq));
}
BOOL CPMDEV_CALL CPM_DLUserEx(const char* cszDevAddr, DEV_CID* pUserIDArray, int nIDCount, int nFlag, int nSeq)
{
  if (! gDspDevManager) {
    return FALSE;
  }

  CPM_APIDEF_IMPL(DLUserEx(pUserIDArray, nIDCount, nFlag, nSeq));
}
BOOL CPMDEV_CALL CPM_DELUserEx(const char* cszDevAddr, DEV_CID* pUserIDArray, int nIDCount, int nSeq)
{
  if (! gDspDevManager) {
    return FALSE;
  }

  CPM_APIDEF_IMPL(DELUserEx(pUserIDArray, nIDCount, nSeq));
}
BOOL CPMDEV_CALL CPM_RebootDev(const char* cszDevAddr, int nSeq /*= -1*/)
{
  if (! gDspDevManager) {
    return FALSE;
  }

  CPM_APIDEF_IMPL(RebootDev(nSeq));
}
BOOL CPMDEV_CALL CPM_ResetScreen(const char* cszDevAddr, int nSeq /*= -1*/)
{
  if (! gDspDevManager) {
    return FALSE;
  }

  CPM_APIDEF_IMPL(ResetScreen(nSeq));
}
BOOL CPMDEV_CALL CPM_DLRecEx(const char* cszDevAddr, const DEV_DL_RECORD_RANGE& cRange, int nSeq /*= -1*/)
{
  if (! gDspDevManager) {
    return FALSE;
  }

  CPM_APIDEF_IMPL(DLRecEx(cRange, nSeq));
}
BOOL CPMDEV_CALL CPM_NetPro(const char* cszDevAddr, DEV_NETPRO* pNetPro , int nSeq)
{
  if (! gDspDevManager) {
    return FALSE;
  }

  CPM_APIDEF_IMPL(DevNetPro(pNetPro, nSeq));
}

