// DspDevManager.cpp: implementation of the DspDevManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DspDevManager.h"

#include "./dev/DevUser.h"
#include "./dev/DevID.h"
#include "./dev/DevAuth.h"
#include "./dev/DevBaseInfo.h"
#include "./dev/DevSysTime.h"
#include "./dev/DevRecord.h"
#include "./dev/DevStatis.h"
#include "./dev/DevSet.h"
#include "./dev/DevTimeGroup.h"
#include "./dev/DevRight.h"
#include "./dev/DevOpenDoor.h"
#include "./dev/DevRgn.h"
#include "./dev/DevFormat.h"
#include "./dev/DevLog.h"
#include "./dev/DevProUpdate.h"
#include "./dev/DevUserGroup.h"
#include "./dev/DevCImage.h"
#include "./dev/DevCN.h"
#include "./dev/DevVideo.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void DevCMDNotify(const char* cszDevAddr,
    const char* cszClientAddr,
    DEV_OPER_TYPE eType,
    DEV_OPER_FLAG eFlag,
    void* pvContent,
    int nSeq,
    DEV_OPER_RESULT rt,
    void* pvClientData
                 )
{
  DspDevManager* pManager = (DspDevManager*)pvClientData;
  pManager->OnDevCMDNotify(cszDevAddr, cszClientAddr, eType, eFlag, pvContent, nSeq, rt);
}

void CNNotify(const char* cszIP, E_CONNECT_STATUS eFlag, BOOL bServer, void* pvClientData)
{
  DspDevManager* pManager = (DspDevManager*)pvClientData;
  pManager->OnCNNotify(cszIP, eFlag);
}

BOOL InternalSend(const char* cszIP, const char* pbBuf, unsigned int uLen, void* pvClientData)
{
  DspDevManager* pManager = (DspDevManager*)pvClientData;
  return pManager->Send(cszIP, pbBuf, &uLen);
}


DspDevManager::DspDevManager() :
  m_pfnCN(NULL),
  m_pCNContext(NULL),
  m_pfnOpNotify(NULL),
  m_pOpNotifyContext(NULL)
{
    m_Net.SetManager(this);
    m_UDisk.SetManager(this);
    m_Video.SetNet(&m_Net);
}

DspDevManager::~DspDevManager()
{
}

#ifndef WIN32
int Sleep(int ms)
{
  return sleep(ms);
}
#endif

void DspDevManager::process()
{
  uutime m_Inval;

  while (m_bRunning) {
    Sleep(2 * 1000);

    if (m_Inval.elapsed() > 6) {
      m_Net.TimeOut();
      m_Inval.restart();
    }
  }
}


void DspDevManager::OnCNNotify(const char* cszIP, E_CONNECT_STATUS eFlag, BOOL bServer)
{
  //AoutLock lock( &m_syn );
  DEBUG_INFO3(_T("1 %s , %d , %d") , cszIP , (int)eFlag , (int) m_pCNContext);

  if (m_pfnCN) {
    (*m_pfnCN)(cszIP , eFlag, m_pCNContext);
  }

  DEBUG_INFO3(_T("2 %s , %d , %d") , cszIP , (int)eFlag , (int) m_pCNContext);
}

void DspDevManager::OnDevCMDNotify(const char* cszDevAddr,
    const char* cszClientAddr,
    DEV_OPER_TYPE eType,
    DEV_OPER_FLAG eFlag,
    void* pvContent,
    int nSeq,
    DEV_OPER_RESULT eResult
                                  )
{
  //AoutLock lock( &m_syn );
  DEBUG_INFO4(_T("1 %s , %d , %d , %d") , cszDevAddr , (int)eType , (int)m_pOpNotifyContext , (int)eResult);

  if (m_pfnOpNotify) {
    m_pfnOpNotify(cszDevAddr, eType, eFlag, pvContent, nSeq, eResult, m_pOpNotifyContext);
  }

  DEBUG_INFO4(_T("2 %s , %d , %d , %d") , cszDevAddr , (int)eType , (int)m_pOpNotifyContext , (int)eResult);
}

void DspDevManager::OnRecvNotify(const char* cszIP, const char* cpbBuf, unsigned int uLen, BOOL bServer)
{
  if (uLen < sizeof(NCMD)) {
    return;
  }

  NCMD* pNCMD = (NCMD*)cpbBuf;
  SuperCmd* pCMDObj = NULL;

  if (pNCMD->m_CFlag & F_DL_ID) {
    pCMDObj = GET_OBJECT(CMD_M_ID);
  }
  else {
    pCMDObj = GET_OBJECT(pNCMD->m_MCmd);
  }

  if (pCMDObj) {
    pCMDObj->Recv(cszIP, pNCMD, bServer, this);
  }
}

BOOL DspDevManager::Send(const char* cszIP, const char* pbBuf, unsigned int* puLen)
{
  assert(cszIP && puLen);

  if (StrIsIP(cszIP)) {
    return m_Net.Send(cszIP, pbBuf, puLen);
  }
  else {
    return m_UDisk.Send(cszIP, pbBuf, puLen);
  }
}

BOOL DspDevManager::DevCN(const char* cszIP, BOOL bFlag)
{
  assert(cszIP);

  if (StrIsIP(cszIP)) {
    return m_Net.NetCN(bFlag, cszIP);
  }
  else {
    return m_UDisk.UDiskCN(bFlag, cszIP);
  }
}

BOOL DspDevManager::SetMode(BOOL srvMode, QuerySrvCB pfn,  void* pvContext)
{
  return m_Net.SetMode(srvMode, pfn, pvContext);
}

BOOL DspDevManager::CnSrv(BOOL bFlag, const char* ip, const char* name, const char* psw)
{
  return m_Net.CnSrv(bFlag, ip, name, psw);
}

void DspDevManager::OnRecvFromNotify(const SOCK_ADDRESS& Addr, const char* cpbBuf, unsigned int uLen)
{
  if (uLen < sizeof(NCMD)) {
    return ;
  }

  NCMD* pNCMD = (NCMD*)cpbBuf;

  if (pNCMD->m_MCmd = CMD_M_VIDEO_OUT) {
    DEVIMAGE* pImage = (DEVIMAGE*)(pNCMD + 1);
    m_Video.ShowVideo(Addr.IP_Address, pImage->m_Block, pImage->m_W, pImage->m_H, pImage->m_Step, pImage->m_Type);
  }
}

BOOL DspDevManager::InitSys(BOOL bFlag)
{
  BOOL ret = TRUE;

  if (bFlag) {
    ret = m_Net.StartWork(TRUE);

    if (ret) {
      CMemPool::GetInstance().CreateMemPool(1024 * 1024 * 2, MEMRESERVER_UNIT);
      CThread::StartThread();
      m_UDisk.StartWork();
    }
  }
  else {
    CThread::StopThread();
    m_Net.StartWork(FALSE);
  }

  return ret;
}

BOOL DspDevManager::DevSerch(BOOL bFlag, const char* cszGroupIP)
{
  return m_Net.Serch(bFlag, cszGroupIP);
}


BOOL DspDevManager::CNDev(const char* cszIP)
{
  return DevCN(cszIP, true);
}

void DspDevManager::DCNDev(const char* cszIP)
{
  DevCN(cszIP, false);
}

BOOL DspDevManager::Auth(const char* cszIP, const DEV_CID& cName, const DEV_CID& cPws, int nFlag, int nSeq)
{
  DEV_AUTH tmp   = {0};
  tmp.m_UserName = cName;
  tmp.m_Password = cPws;
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(DEV_AUTH), 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) + sizeof(DEV_AUTH);
  DevAuth auth(cszIP, tmp, (nFlag == 2), nSeq, buf, len);
  return Send(cszIP, buf, &len);
}

BOOL DspDevManager::SetDevRgn(const char* cszIP, const char* cszGroupIP, int nSeq)
{
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(DEV_CID), 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) + sizeof(DEV_CID);
  DevRgn rgn(cszIP, cszGroupIP, nSeq, buf, len);
  return Send(cszIP, buf, &len);
}

BOOL DspDevManager::GetDevRgn(const char* cszIP, int nSeq)
{
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD), 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD);
  DevRgn rgn(cszIP, nSeq, buf, len);
  return Send(cszIP, buf, &len);
}

BOOL DspDevManager::SetDevTime(const char* cszIP, const DEV_DATETIME& cTime, int nSeq)
{
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(DATETIME), 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) + sizeof(DATETIME);
  DevTime Time(cszIP, cTime, nSeq, buf, len);
  return Send(cszIP, buf, &len);
}

BOOL DspDevManager::GetDevTime(const char* cszIP, int nSeq)
{
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD), 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) ;
  DevTime Time(cszIP, nSeq, buf, len);
  return Send(cszIP, buf, &len);
}

BOOL DspDevManager::GetDevInfo(const char* cszIP, int nSeq)
{
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD), 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) ;
  DevInfo info(cszIP, nSeq, buf, len);
  return Send(cszIP, buf, &len);
}

BOOL DspDevManager::GetDevStatis(const char* cszIP, int nSeq)
{
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD), 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) ;
  DevStatis statis(cszIP, nSeq, buf, len);
  return Send(cszIP, buf, &len);
}

BOOL DspDevManager::SetDevWorkAtt(const char* cszIP, const DEV_WORKATT& cAtt, int nSeq)
{
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(DEVSET), 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) + sizeof(DEVSET) ;
  DevSet att(cszIP, cAtt, nSeq, buf, len);
  return Send(cszIP, buf, &len);
}

BOOL DspDevManager::GetDevWorkAtt(const char* cszIP, int nSeq)
{
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD), 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) ;
  DevSet att(cszIP, nSeq, buf, len);
  return Send(cszIP, buf, &len);
}

BOOL DspDevManager::FormatDev(const char* cszIP, int nSeq)
{
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD), 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) ;
  DevFormat format(cszIP, nSeq, buf, len);
  return Send(cszIP, buf, &len);
}

BOOL DspDevManager::UpdateApp(const char* cszIP, const DEV_APPUPDATE& cData, int nSeq)
{
  char* pbBuf = (char*)(new int[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(APPUPDATE) + cData.m_Bytes, 4) >> 2]);
  unsigned int len = sizeof(NCMD) + sizeof(APPUPDATE) + cData.m_Bytes;
  memset(pbBuf, 0x0, OFFSET_SIZE + sizeof(NCMD) + sizeof(APPUPDATE));
  DevProUpdate update(cszIP, cData, nSeq, pbBuf + OFFSET_SIZE, len);
  BOOL ret = Send(cszIP, pbBuf + OFFSET_SIZE, &len);
  delete [] pbBuf;
  return ret;
}

BOOL DspDevManager::AddDevUser(const char* cszIP, const DEV_USER& cUser, int nSeq)
{
  assert((cUser.m_FeatLen <=  FEAT_SIZE) && (cUser.m_PicLen <=  USER_JPEG_SIZE));

  if (!((cUser.m_FeatLen <=  FEAT_SIZE) && (cUser.m_PicLen <=  USER_JPEG_SIZE))) {
    return FALSE;
  }

  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(USER) + FEAT_SIZE + USER_JPEG_SIZE, 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) + sizeof(USER) + FEAT_SIZE + USER_JPEG_SIZE ;
  DevUser dev_user(cszIP, cUser, TRUE, nSeq, buf, len);
  return Send(cszIP, buf, &len);
}

BOOL DspDevManager::AddDevRealTimeUser(const char* cszIP, const DEV_USER& cUser, const DEV_USER_REAL_LOAD& cLoad)
{
  assert((cUser.m_FeatLen <=  FEAT_SIZE) && (cUser.m_PicLen <=  USER_JPEG_SIZE));

  if (!((cUser.m_FeatLen <=  FEAT_SIZE) && (cUser.m_PicLen <=  USER_JPEG_SIZE))) {
    return FALSE;
  }

  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(USER) + FEAT_SIZE + USER_JPEG_SIZE, 4) >> 2] = {0};
  char* buf = (char*)nData;
  unsigned int len = sizeof(NCMD) + sizeof(USER) + FEAT_SIZE + USER_JPEG_SIZE ;
  DevUser dev_user(cszIP, cUser, cLoad, buf + OFFSET_SIZE, len);
  return Send(cszIP, buf + OFFSET_SIZE, &len);
}

BOOL DspDevManager::RealTimeUserCap(const char* cszDevAddr, const DEV_USER& cUser, int nSeq)
{
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(USER), 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) + sizeof(USER) ;
  DevUser dev_user(cszDevAddr, cUser, FALSE, nSeq, buf, len);
  return Send(cszDevAddr, buf, &len);
}

BOOL DspDevManager::GetDevUser(const char* cszIP, const DEV_DATA_DL& cDlData, int nSeq)
{
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(DEV_CID) + sizeof(DEV_TIMESEG), 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) + sizeof(DEV_CID) + sizeof(DEV_TIMESEG) ;

  switch (cDlData.m_DLType) {
  case DL_SINGLE : {
    DevUser user(cszIP, *(CID*)(cDlData.m_DLContent), cDlData.m_DLFlag, nSeq, buf, len);
  }
  break;

  case DL_ALL: {
    DevID id(cszIP, CMD_M_USER, nSeq, buf, len);
  }
  break;

  case DL_SEGTIME: {
    DevID id(cszIP, CMD_M_USER, *(DEV_TIMESEG*)(cDlData.m_DLContent), nSeq, buf, len);
  }
  break;

  default:
    len = 0;
    break;
  }

  return Send(cszIP, buf, &len);
}

BOOL DspDevManager::DelDevUser(const char* cszIP, const DEV_CID& cID, int nSeq)
{
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(DEV_CID), 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) + sizeof(DEV_CID);
  DevUser user(cszIP, (CID&)cID, nSeq, buf, len);
  return Send(cszIP, buf, &len);
}

BOOL DspDevManager::GetDevRecord(const char* cszIP, int nClass, const DEV_DATA_DL& cDlData, int nSeq)
{
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(RECORDRANGE) + sizeof(DEV_TIMESEG), 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) + sizeof(RECORDRANGE) + sizeof(DEV_TIMESEG) ;

  switch (cDlData.m_DLType) {
  case DL_SINGLE: {
    DevID id(cszIP, nClass, cDlData.m_DLFlag | 0x80000000, *(DEV_DL_RECORD_RANGE*)(cDlData.m_DLContent), nSeq, buf, len);
  }
  break;

  case DL_RANGE: {
    DevID id(cszIP, nClass, 0, *(DEV_DL_RECORD_RANGE*)(cDlData.m_DLContent), nSeq, buf, len);
  }
  break;

  case DL_ALL: {
    DevID id(cszIP, nClass, nSeq, buf, len);
  }
  break;

  case DL_SEGTIME: {
    DevID id(cszIP, nClass, *(DEV_TIMESEG*)(cDlData.m_DLContent), nSeq, buf, len);
  }
  break;

  default:
    len = 0;
    break;
  }

  return Send(cszIP, buf, &len);
}

BOOL DspDevManager::SetOpenDoorState(const char* cszIP, DEV_DOOR_STATE eState, int nSeq)
{
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD), 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) ;
  DevEOpenDoor opendoor(cszIP, eState, nSeq, buf, len);
  return Send(cszIP, buf, &len);
}

BOOL DspDevManager::GetOpenDoorState(const char* cszIP, int nSeq /*= -1 */)
{
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD), 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) ;
  DevEOpenDoor opendoor(cszIP, nSeq, buf, len);
  return Send(cszIP, buf, &len);
}

BOOL DspDevManager::RemoteOpenDoor(const char* cszIP, BOOL bOpen, int nSeq)
{
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD), 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD);
  DevROpenDoor opendoor(cszIP, bOpen, nSeq, buf, len);
  return Send(cszIP, buf, &len);
}

BOOL DspDevManager::IOCtrl(const char* cszDevAddr, const DEV_IOCTRL& ioctrl, int nSeq)
{
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(DEV_IOCTRL), 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) + sizeof(DEV_IOCTRL);
  DevIOCtrl ioCtrl(cszDevAddr, ioctrl, nSeq, buf, len);
  return Send(cszDevAddr, buf, &len);
}

#define TG_MAX_SIZE 256
BOOL DspDevManager::SetDevTG(const char* cszIP, const DEV_TIMEGROUP_ARRAY& cArray, int nSeq)
{
  assert(cArray.m_nCount <= TG_MAX_SIZE);
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(CARRAY) + sizeof(DSPTIMEG)*TG_MAX_SIZE, 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) + sizeof(CARRAY) + sizeof(DSPTIMEG) * TG_MAX_SIZE;
  DevTimeGroup group(cszIP, cArray, DEV_ANY_TIME, DEV_OPER_UPLOAD, nSeq, buf, len);
  return Send(cszIP, buf, &len);
}

BOOL DspDevManager::GetDevTG(const char* cszIP, const DEV_TIMEGROUP_ARRAY& cArray, DEV_TIMEGROUP_TYPE eType, int nSeq)
{
  assert(cArray.m_nCount <= TG_MAX_SIZE);
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(CARRAY) + sizeof(DSPTIMEG)*TG_MAX_SIZE, 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) + sizeof(CARRAY) + sizeof(DSPTIMEG) * TG_MAX_SIZE;
  DevTimeGroup group(cszIP, cArray, eType, DEV_OPER_DOWNLOAD, nSeq, buf, len);
  return Send(cszIP, buf, &len);
}

BOOL DspDevManager::DelDevTG(const char* cszIP, const DEV_TIMEGROUP_ARRAY& cArray, DEV_TIMEGROUP_TYPE eType, int nSeq)
{
  assert(cArray.m_nCount <= TG_MAX_SIZE);
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(CARRAY) + sizeof(DSPTIMEG)*TG_MAX_SIZE, 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) + sizeof(CARRAY) + sizeof(DSPTIMEG) * TG_MAX_SIZE;
  DevTimeGroup group(cszIP, cArray, eType, DEV_OPER_DELETE, nSeq, buf, len);
  return Send(cszIP, buf, &len);
}

#define RT_MAX_SIZE 256
BOOL DspDevManager::SetDevRT(const char* cszIP, const DEV_RIGHT_ARRAY& cArray, int nSeq)
{
  assert(cArray.m_nCount <= RT_MAX_SIZE);
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(CARRAY) + sizeof(DSPRIGHT)*RT_MAX_SIZE, 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) + sizeof(CARRAY) + sizeof(DSPRIGHT) * RT_MAX_SIZE;
  DevRight right(cszIP, cArray, DEV_OPER_UPLOAD, nSeq, buf, len);
  return Send(cszIP, buf, &len);
}

BOOL DspDevManager::GetDevRT(const char* cszIP, const DEV_RIGHT_ARRAY& cArray, int nSeq)
{
  assert(cArray.m_nCount <= RT_MAX_SIZE);
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(CARRAY) + sizeof(DSPRIGHT)*RT_MAX_SIZE, 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) + sizeof(CARRAY) + sizeof(DSPRIGHT) * RT_MAX_SIZE;
  DevRight right(cszIP, cArray, DEV_OPER_DOWNLOAD, nSeq, buf, len);
  return Send(cszIP, buf, &len);
}

BOOL DspDevManager::DelDevRT(const char* cszIP, const DEV_RIGHT_ARRAY& cArray, int nSeq)
{
  assert(cArray.m_nCount <= RT_MAX_SIZE);
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(CARRAY) + sizeof(DSPRIGHT)*RT_MAX_SIZE, 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) + sizeof(CARRAY) + sizeof(DSPRIGHT) * RT_MAX_SIZE;
  DevRight right(cszIP, cArray, DEV_OPER_DELETE, nSeq, buf, len);
  return Send(cszIP, buf, &len);
}

#define UG_MAX_SIZE 256
BOOL DspDevManager::SetDevUserGroup(const char* cszIP, const DEV_USERGROUP_ARRAY& cArray, int nSeq)
{
  assert(cArray.m_nCount <= UG_MAX_SIZE);
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(CARRAY) + sizeof(USERGROUP)*UG_MAX_SIZE, 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) + sizeof(CARRAY) + sizeof(USERGROUP) * UG_MAX_SIZE;
  DevUserGroup UserGroup(cszIP, cArray, DEV_OPER_UPLOAD, nSeq, buf, len);
  return Send(cszIP, buf, &len);
}

BOOL DspDevManager::GetDevUserGroup(const char* cszIP, const DEV_USERGROUP_ARRAY& cArray, int nSeq)
{
  assert(cArray.m_nCount <= UG_MAX_SIZE);
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(CARRAY) + sizeof(USERGROUP)*UG_MAX_SIZE, 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) + sizeof(CARRAY) + sizeof(USERGROUP) * UG_MAX_SIZE;
  DevUserGroup UserGroup(cszIP, cArray, DEV_OPER_DOWNLOAD, nSeq, buf, len);
  return Send(cszIP, buf, &len);
}

BOOL DspDevManager::DelDevUserGroup(const char* cszIP, const DEV_USERGROUP_ARRAY& cArray, int nSeq)
{
  assert(cArray.m_nCount <= UG_MAX_SIZE);
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(CARRAY) + sizeof(USERGROUP)*UG_MAX_SIZE, 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) + sizeof(CARRAY) + sizeof(USERGROUP) * UG_MAX_SIZE;
  DevUserGroup UserGroup(cszIP, cArray, DEV_OPER_DELETE, nSeq, buf, len);
  return Send(cszIP, buf, &len);
}

BOOL DspDevManager::StartVideo(const char* cszIP, void* hwnd, frame_callback_t frame_rgb, int nSeq)
{
  if (!m_Video.StartVideo(cszIP, hwnd, frame_rgb)) {
    return FALSE;
  }

  SOCK_ADDRESS Addr = {0};
  m_Video.GetVideoTransAddr(cszIP, Addr);
  char buf[OFFSET_SIZE + sizeof(NCMD) + sizeof(SOCK_ADDRESS)] = {0};
  unsigned int len = sizeof(NCMD) + sizeof(SOCK_ADDRESS);
  DevVideo Video(cszIP, Addr.IP_Address, Addr.Port, /*hwnd, */nSeq, buf + OFFSET_SIZE, len);
  return Send(cszIP, buf + OFFSET_SIZE, &len);
}

BOOL DspDevManager::StopVideo(const char* cszIP, int nSeq)
{
  char buf[ OFFSET_SIZE + sizeof(NCMD) ] = {0};
  unsigned int len = sizeof(NCMD);
  DevVideo Video(cszIP, nSeq, buf + OFFSET_SIZE, len);
  BOOL ret = Send(cszIP, buf + OFFSET_SIZE, &len);
  m_Video.StopVideo(cszIP);
  return ret;
}


//用户扩展操作
BOOL DspDevManager::ULUserEx(const char* cszDevAddr, const DEV_BATCH_USER& cUsers, int nFlag, int nSeq)
{
  char* buf = 0;
  unsigned int len = 0;
  DevUserEx users(cszDevAddr, cUsers, nFlag, nSeq, buf, len);

  if (NULL == buf) {
    return FALSE;
  }

  BOOL ret =  Send(cszDevAddr, buf + OFFSET_SIZE, &len);
  CMemPool::GetInstance().FreeMem(buf);
  return ret;
}

BOOL DspDevManager::DLUserEx(const char* cszDevAddr, DEV_CID* pUserIDArray, int nIDCount, int nFlag, int nSeq)
{
  char* buf = 0;
  unsigned int len = 0;
  DevUserEx users(cszDevAddr, pUserIDArray, nIDCount, nFlag, nSeq, buf, len);

  if (NULL == buf) {
    return FALSE;
  }

  BOOL ret =  Send(cszDevAddr, buf + OFFSET_SIZE, &len);
  CMemPool::GetInstance().FreeMem(buf);
  return ret;
}

BOOL DspDevManager::DELUserEx(const char* cszDevAddr, DEV_CID* pUserIDArray, int nIDCount, int nSeq)
{
  char* buf = 0;
  unsigned int len = 0;
  DevUserEx users(cszDevAddr, pUserIDArray, nIDCount, nSeq, buf, len);

  if (NULL == buf) {
    return FALSE;
  }

  BOOL ret =  Send(cszDevAddr, buf + OFFSET_SIZE, &len);
  CMemPool::GetInstance().FreeMem(buf);
  return ret;
}

BOOL DspDevManager::DLRecEx(const char* cszDevAddr, const DEV_DL_RECORD_RANGE& cRange, int nSeq /*= -1*/)
{
  char buf[OFFSET_SIZE + sizeof(NCMD) + sizeof(DEV_DL_RECORD_RANGE)] = {0};
  unsigned int len = sizeof(NCMD) + sizeof(DEV_DL_RECORD_RANGE);
  DevRecordEx record(cszDevAddr, cRange, nSeq, buf + OFFSET_SIZE, len);
  return Send(cszDevAddr, buf + OFFSET_SIZE, &len);
}

BOOL DspDevManager::RebootDev(const char* cszDevAddr, int nSeq /*= -1*/)
{
  char buf[ OFFSET_SIZE + sizeof(NCMD) ] = {0};
  unsigned int len = sizeof(NCMD);
  DevReboot ins(cszDevAddr, nSeq, buf + OFFSET_SIZE, len);
  return Send(cszDevAddr, buf + OFFSET_SIZE, &len);
}

BOOL DspDevManager::DevNetPro(const char* cszDevAddr, DEV_NETPRO* pNetPro , int nSeq)
{
  char buf[ OFFSET_SIZE + sizeof(NCMD) + sizeof(DEV_NETPRO) ] = {0};
  unsigned int len = sizeof(NCMD);
  CNetPro ins(cszDevAddr, pNetPro , nSeq, buf + OFFSET_SIZE, len);
  return Send(cszDevAddr, buf + OFFSET_SIZE, &len);
}