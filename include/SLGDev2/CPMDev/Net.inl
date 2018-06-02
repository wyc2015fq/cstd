// Client.cpp: implementation of the DspDevManager class.
//
#include "Net.h"
#include "DspDevManager.h"
BOOL CSock::Auth(const DEV_CID& cName, const DEV_CID& cPws, int nFlag, int nSeq)
{
  DEV_AUTH tmp = {0};
  tmp.m_UserName = cName;
  tmp.m_Password = cPws;
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(DEV_AUTH), 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) + sizeof(DEV_AUTH);
  DevAuth auth(this, tmp, (nFlag == 2), nSeq, buf, len);
  return Send(buf, &len);
}
BOOL CSock::SetDevRgn(const char* cszGroupIP, int nSeq)
{
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(DEV_CID), 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) + sizeof(DEV_CID);
  DevRgn rgn(this, cszGroupIP, nSeq, buf, len);
  return Send(buf, &len);
}
BOOL CSock::GetDevRgn(int nSeq)
{
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD), 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD);
  DevRgn rgn(this, nSeq, buf, len);
  return Send(buf, &len);
}
BOOL CSock::SetDevTime(const DEV_DATETIME& cTime, int nSeq)
{
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(DATETIME), 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) + sizeof(DATETIME);
  DevTime Time(this, cTime, nSeq, buf, len);
  return Send(buf, &len);
}
BOOL CSock::GetDevTime(int nSeq)
{
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD), 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) ;
  DevTime Time(this, nSeq, buf, len);
  return Send(buf, &len);
}
BOOL CSock::GetDevInfo(int nSeq)
{
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD), 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) ;
  DevInfo info(this, nSeq, buf, len);
  return Send(buf, &len);
}
BOOL CSock::GetDevStatis(int nSeq)
{
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD), 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) ;
  DevStatis statis(this, nSeq, buf, len);
  return Send(buf, &len);
}
BOOL CSock::SetDevWorkAtt(const DEV_WORKATT& cAtt, int nSeq)
{
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(DEVSET), 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) + sizeof(DEVSET) ;
  DevSet att(this, cAtt, nSeq, buf, len);
  return Send(buf, &len);
}
BOOL CSock::GetDevWorkAtt(int nSeq)
{
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD), 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) ;
  DevSet att(this, nSeq, buf, len);
  return Send(buf, &len);
}
BOOL CSock::FormatDev(int nSeq)
{
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD), 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) ;
  DevFormat format(this, nSeq, buf, len);
  return Send(buf, &len);
}
BOOL CSock::UpdateApp(const DEV_APPUPDATE& cData, int nSeq)
{
  char* pbBuf = (char*)(new int[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(APPUPDATE) + cData.m_Bytes, 4) >> 2]);
  unsigned int len = sizeof(NCMD) + sizeof(APPUPDATE) + cData.m_Bytes;
  memset(pbBuf, 0x0, OFFSET_SIZE + sizeof(NCMD) + sizeof(APPUPDATE));
  DevProUpdate update(this, cData, nSeq, pbBuf + OFFSET_SIZE, len);
  BOOL ret = Send(pbBuf + OFFSET_SIZE, &len);
  delete [] pbBuf;
  return ret;
}
BOOL CSock::AddDevUser(const DEV_USER& cUser, int nSeq)
{
  assert((cUser.m_FeatLen <= FEAT_SIZE) && (cUser.m_PicLen <= USER_JPEG_SIZE));

  if (!((cUser.m_FeatLen <= FEAT_SIZE) && (cUser.m_PicLen <= USER_JPEG_SIZE))) {
    return FALSE;
  }

  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(USER) + FEAT_SIZE + USER_JPEG_SIZE, 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) + sizeof(USER) + FEAT_SIZE + USER_JPEG_SIZE ;
  DevUser dev_user(this, cUser, TRUE, nSeq, buf, len);
  return Send(buf, &len);
}
BOOL CSock::AddDevRealTimeUser(const DEV_USER& cUser, const DEV_USER_REAL_LOAD& cLoad)
{
  assert((cUser.m_FeatLen <= FEAT_SIZE) && (cUser.m_PicLen <= USER_JPEG_SIZE));

  if (!((cUser.m_FeatLen <= FEAT_SIZE) && (cUser.m_PicLen <= USER_JPEG_SIZE))) {
    return FALSE;
  }

  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(USER) + FEAT_SIZE + USER_JPEG_SIZE, 4) >> 2] = {0};
  char* buf = (char*)nData;
  unsigned int len = sizeof(NCMD) + sizeof(USER) + FEAT_SIZE + USER_JPEG_SIZE ;
  DevUser dev_user(this, cUser, cLoad, buf + OFFSET_SIZE, len);
  return Send(buf + OFFSET_SIZE, &len);
}
BOOL CSock::RealTimeUserCap(const DEV_USER& cUser, int nSeq)
{
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(USER) + FEAT_SIZE + USER_JPEG_SIZE, 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) + sizeof(USER) ;
  DevUser dev_user(this, cUser, FALSE, nSeq, buf, len);
  return Send(buf, &len);
}
BOOL CSock::GetDevUser(const DEV_DATA_DL& cDlData, int nSeq)
{
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(DEV_CID) + sizeof(DEV_TIMESEG), 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) + sizeof(DEV_CID) + sizeof(DEV_TIMESEG) ;

  switch (cDlData.m_DLType) {
  case DL_SINGLE : {
    DevUser user(this, *(CID*)(cDlData.m_DLContent), cDlData.m_DLFlag, nSeq, buf, len);
  }
  break;

  case DL_ALL: {
    DevID id(this, CMD_M_USER, nSeq, buf, len);
  }
  break;

  case DL_SEGTIME: {
    DevID id(this, CMD_M_USER, *(DEV_TIMESEG*)(cDlData.m_DLContent), nSeq, buf, len);
  }
  break;

  default:
    len = 0;
    break;
  }

  return Send(buf, &len);
}
BOOL CSock::DelDevUser(const DEV_CID& cID, int nSeq)
{
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(DEV_CID), 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) + sizeof(DEV_CID);
  DevUser user(this, (CID&)cID, nSeq, buf, len);
  return Send(buf, &len);
}
BOOL CSock::GetDevRecord(int nClass, const DEV_DATA_DL& cDlData, int nSeq)
{
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(RECORDRANGE) + sizeof(DEV_TIMESEG), 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) + sizeof(RECORDRANGE) + sizeof(DEV_TIMESEG) ;

  switch (cDlData.m_DLType) {
  case DL_SINGLE: {
    DevID id(this, nClass, cDlData.m_DLFlag | 0x80000000, *(DEV_DL_RECORD_RANGE*)(cDlData.m_DLContent), nSeq, buf, len);
  }
  break;

  case DL_RANGE: {
    DevID id(this, nClass, 0 , *(DEV_DL_RECORD_RANGE*)(cDlData.m_DLContent), nSeq, buf, len);
  }
  break;

  case DL_ALL: {
    DevID id(this, nClass, nSeq, buf, len);
  }
  break;

  case DL_SEGTIME: {
    DevID id(this, nClass, *(DEV_TIMESEG*)(cDlData.m_DLContent), nSeq, buf, len);
  }
  break;

  default:
    len = 0;
    break;
  }

  return Send(buf, &len);
}
BOOL CSock::SetOpenDoorState(DEV_DOOR_STATE eState, int nSeq)
{
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD), 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) ;
  DevEOpenDoor opendoor(this, eState, nSeq, buf, len);
  return Send(buf, &len);
}
BOOL CSock::GetOpenDoorState(int nSeq /*= -1 */)
{
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD), 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) ;
  DevEOpenDoor opendoor(this, nSeq, buf, len);
  return Send(buf, &len);
}
BOOL CSock::RemoteOpenDoor(BOOL bOpen, int nSeq)
{
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD), 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD);
  DevROpenDoor opendoor(this, bOpen, nSeq, buf, len);
  return Send(buf, &len);
}
BOOL CSock::IOCtrl(const DEV_IOCTRL& ioctrl, int nSeq)
{
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(DEV_IOCTRL), 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) + sizeof(DEV_IOCTRL);
  DevIOCtrl ioCtrl(this, ioctrl, nSeq, buf, len);
  return Send(buf, &len);
}
#define TG_MAX_SIZE 256
BOOL CSock::SetDevTG(const DEV_TIMEGROUP_ARRAY& cArray, int nSeq)
{
  assert(cArray.m_nCount <= TG_MAX_SIZE);
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(CARRAY) + sizeof(DSPTIMEG)*TG_MAX_SIZE, 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) + sizeof(CARRAY) + sizeof(DSPTIMEG) * TG_MAX_SIZE;
  DevTimeGroup group(this, cArray, DEV_ANY_TIME, DEV_OPER_UPLOAD, nSeq, buf, len);
  return Send(buf, &len);
}
BOOL CSock::GetDevTG(const DEV_TIMEGROUP_ARRAY& cArray, DEV_TIMEGROUP_TYPE eType, int nSeq)
{
  assert(cArray.m_nCount <= TG_MAX_SIZE);
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(CARRAY) + sizeof(DSPTIMEG)*TG_MAX_SIZE, 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) + sizeof(CARRAY) + sizeof(DSPTIMEG) * TG_MAX_SIZE;
  DevTimeGroup group(this, cArray, eType, DEV_OPER_DOWNLOAD, nSeq, buf, len);
  return Send(buf, &len);
}
BOOL CSock::DelDevTG(const DEV_TIMEGROUP_ARRAY& cArray, DEV_TIMEGROUP_TYPE eType, int nSeq)
{
  assert(cArray.m_nCount <= TG_MAX_SIZE);
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(CARRAY) + sizeof(DSPTIMEG)*TG_MAX_SIZE, 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) + sizeof(CARRAY) + sizeof(DSPTIMEG) * TG_MAX_SIZE;
  DevTimeGroup group(this, cArray, eType, DEV_OPER_DELETE, nSeq, buf, len);
  return Send(buf, &len);
}
#define RT_MAX_SIZE 256
BOOL CSock::SetDevRT(const DEV_RIGHT_ARRAY& cArray, int nSeq)
{
  assert(cArray.m_nCount <= RT_MAX_SIZE);
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(CARRAY) + sizeof(DSPRIGHT)*RT_MAX_SIZE, 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) + sizeof(CARRAY) + sizeof(DSPRIGHT) * RT_MAX_SIZE;
  DevRight right(this, cArray, DEV_OPER_UPLOAD, nSeq, buf, len);
  return Send(buf, &len);
}
BOOL CSock::GetDevRT(const DEV_RIGHT_ARRAY& cArray, int nSeq)
{
  assert(cArray.m_nCount <= RT_MAX_SIZE);
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(CARRAY) + sizeof(DSPRIGHT)*RT_MAX_SIZE, 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) + sizeof(CARRAY) + sizeof(DSPRIGHT) * RT_MAX_SIZE;
  DevRight right(this, cArray, DEV_OPER_DOWNLOAD, nSeq, buf, len);
  return Send(buf, &len);
}
BOOL CSock::DelDevRT(const DEV_RIGHT_ARRAY& cArray, int nSeq)
{
  assert(cArray.m_nCount <= RT_MAX_SIZE);
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(CARRAY) + sizeof(DSPRIGHT)*RT_MAX_SIZE, 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) + sizeof(CARRAY) + sizeof(DSPRIGHT) * RT_MAX_SIZE;
  DevRight right(this, cArray, DEV_OPER_DELETE, nSeq, buf, len);
  return Send(buf, &len);
}
#define UG_MAX_SIZE 256
BOOL CSock::SetDevUserGroup(const DEV_USERGROUP_ARRAY& cArray, int nSeq)
{
  assert(cArray.m_nCount <= UG_MAX_SIZE);
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(CARRAY) + sizeof(USERGROUP)*UG_MAX_SIZE, 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) + sizeof(CARRAY) + sizeof(USERGROUP) * UG_MAX_SIZE;
  DevUserGroup UserGroup(this, cArray, DEV_OPER_UPLOAD, nSeq, buf, len);
  return Send(buf, &len);
}
BOOL CSock::GetDevUserGroup(const DEV_USERGROUP_ARRAY& cArray, int nSeq)
{
  assert(cArray.m_nCount <= UG_MAX_SIZE);
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(CARRAY) + sizeof(USERGROUP)*UG_MAX_SIZE, 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) + sizeof(CARRAY) + sizeof(USERGROUP) * UG_MAX_SIZE;
  DevUserGroup UserGroup(this, cArray, DEV_OPER_DOWNLOAD, nSeq, buf, len);
  return Send(buf, &len);
}
BOOL CSock::DelDevUserGroup(const DEV_USERGROUP_ARRAY& cArray, int nSeq)
{
  assert(cArray.m_nCount <= UG_MAX_SIZE);
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(CARRAY) + sizeof(USERGROUP)*UG_MAX_SIZE, 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) + sizeof(CARRAY) + sizeof(USERGROUP) * UG_MAX_SIZE;
  DevUserGroup UserGroup(this, cArray, DEV_OPER_DELETE, nSeq, buf, len);
  return Send(buf, &len);
}
//用户扩展操作
BOOL CSock::ULUserEx(const DEV_BATCH_USER& cUsers, int nFlag, int nSeq)
{
  char* buf = 0;
  unsigned int len = 0;
  DevUserEx users(this, cUsers, nFlag, nSeq, buf, len);

  if (NULL == buf) {
    return FALSE;
  }

  BOOL ret = Send(buf + OFFSET_SIZE, &len);
  CMemPool::GetInstance().FreeMem(buf);
  return ret;
}
BOOL CSock::DLUserEx(DEV_CID* pUserIDArray, int nIDCount, int nFlag, int nSeq)
{
  char* buf = 0;
  unsigned int len = 0;
  DevUserEx users(this, pUserIDArray, nIDCount, nFlag, nSeq, buf, len);

  if (NULL == buf) {
    return FALSE;
  }

  BOOL ret = Send(buf + OFFSET_SIZE, &len);
  CMemPool::GetInstance().FreeMem(buf);
  return ret;
}
BOOL CSock::DELUserEx(DEV_CID* pUserIDArray, int nIDCount, int nSeq)
{
  char* buf = 0;
  unsigned int len = 0;
  DevUserEx users(this, pUserIDArray, nIDCount, nSeq, buf, len);

  if (NULL == buf) {
    return FALSE;
  }

  BOOL ret = Send(buf + OFFSET_SIZE, &len);
  CMemPool::GetInstance().FreeMem(buf);
  return ret;
}
BOOL CSock::DLRecEx(const DEV_DL_RECORD_RANGE& cRange, int nSeq /*= -1*/)
{
  char buf[OFFSET_SIZE + sizeof(NCMD) + sizeof(DEV_DL_RECORD_RANGE)] = {0};
  unsigned int len = sizeof(NCMD) + sizeof(DEV_DL_RECORD_RANGE);
  DevRecordEx record(this, cRange, nSeq, buf + OFFSET_SIZE, len);
  return Send(buf + OFFSET_SIZE, &len);
}
BOOL CSock::RebootDev(int nSeq /*= -1*/)
{
  char buf[ OFFSET_SIZE + sizeof(NCMD) ] = {0};
  unsigned int len = sizeof(NCMD);
  DevReboot ins(this, nSeq, buf + OFFSET_SIZE, len);
  return Send(buf + OFFSET_SIZE, &len);
}
BOOL CSock::ResetScreen(int nSeq /*= -1*/)
{
  char buf[ OFFSET_SIZE + sizeof(NCMD) ] = {0};
  unsigned int len = sizeof(NCMD);
  CResetScreen ins(this, nSeq, buf + OFFSET_SIZE, len);
  return Send(buf + OFFSET_SIZE, &len);
}
BOOL CSock::DevNetPro(DEV_NETPRO* pNetPro , int nSeq)
{
  char buf[ OFFSET_SIZE + sizeof(NCMD) + sizeof(DEV_NETPRO) ] = {0};
  unsigned int len = sizeof(NCMD);
  CNetPro ins(this, pNetPro , nSeq, buf + OFFSET_SIZE, len);
  return Send(buf + OFFSET_SIZE, &len);
}
int VideoProCB_defult(const struct VideoPro* pro)
{
  const char* winname = (const char*)(pro->m_hWnd);
  BITMAPINFO m_pBitmapInfo[1] = {0};
  RECT m_Rect;
  HWND hWnd = (HWND)(pro->m_hWnd);
  ::GetWindowRect(hWnd, &(m_Rect));
  m_pBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFO);
  m_pBitmapInfo->bmiHeader.biPlanes = 1;
  m_pBitmapInfo->bmiHeader.biBitCount = 24;
  m_pBitmapInfo->bmiHeader.biCompression = BI_RGB;
  m_pBitmapInfo->bmiHeader.biSizeImage = 0;
  m_pBitmapInfo->bmiHeader.biXPelsPerMeter = 11811;
  m_pBitmapInfo->bmiHeader.biYPelsPerMeter = 11811;
  m_pBitmapInfo->bmiHeader.biClrUsed = 0;
  m_pBitmapInfo->bmiHeader.biClrImportant = 0;
  m_pBitmapInfo->bmiHeader.biHeight = pro->biHeight;
  m_pBitmapInfo->bmiHeader.biWidth = pro->biWidth;
  int lStillWidth = m_Rect.right - m_Rect.left;
  int lStillHeight = m_Rect.bottom - m_Rect.top;
  HDC hdc = ::GetDC(hWnd);
  ::SetStretchBltMode(hdc, COLORONCOLOR);
  ::StretchDIBits(
      hdc, 0, 0,
      lStillWidth, lStillHeight,
      0, 0,
      pro->biWidth,
      pro->biHeight,
      pro->m_pRGBTRANS,
      m_pBitmapInfo,
      DIB_RGB_COLORS,
      SRCCOPY);
  ::ReleaseDC(hWnd, hdc);
  //printf("w=%d h=%d\n", pro->biWidth, pro->biHeight);
  return 0;
}
BOOL CSock::StartVideo(void* hwnd, VideoPro_CallBack VideoCB, int nSeq)
{
  addr_in addrLocal = {0};

  if (NULL == m_Pro) {
    if (!CPM_GetLocalIP(&addr, 0, &addrLocal)) {
      return false;
    }

    SOCKET hVideo = CPM_CreateSock(addrLocal.family, SOCK_DGRAM);

    if (!CPM_Bind(hVideo, &addrLocal)) {
      CPM_ReleaseSock(hVideo, NULL);
      return false;
    }

    CPM_GetSockName(hVideo, &addrLocal);
    CPM_Recv(hVideo , 0, this);
    VideoPro* pro = new VideoPro;
    pro->m_hVideoHandle = hVideo;
    pro->m_Addr = addrLocal;
    pro->m_pRGBTMP = NULL;
    pro->m_pRGBTRANS = NULL;
    pro->m_hWnd = hwnd;
    pro->VideoCB = VideoCB;
    m_Pro = pro;
    //DEBUG_INFO3("StartVideo HWND = %d , W = %d , H = %d" , (int)hwnd , pro->m_Rect.right - pro->m_Rect.left , pro->m_Rect.bottom - pro->m_Rect.top);
  }
  else {
    m_Pro->m_hWnd = hwnd;
    addrLocal = m_Pro->m_Addr;
  }

  SOCK_ADDRESS Addr[1] = {0};
  Addr->Port = net_ntop(&addrLocal, Addr->IP_Address, IP_LEN);
  char buf[OFFSET_SIZE + sizeof(NCMD) + sizeof(SOCK_ADDRESS)] = {0};
  unsigned int len = sizeof(NCMD) + sizeof(SOCK_ADDRESS);
  DevVideo Video(this, Addr->IP_Address, Addr->Port, nSeq, buf + OFFSET_SIZE, len);
  return Send(buf + OFFSET_SIZE, &len);
}
void CSock::ShowVideo(const char* cpbImage, int w, int h, int step, int nImageType)
{
  AoutLock lock(&m_syn);
  VideoPro* pro = m_Pro;
  static int inited = 0;

  if (!inited) {
    InitialTable();
  }

  if (NULL == pro) {
    return;
  }

  if (NULL == pro->m_pRGBTMP || w != pro->biWidth || h != pro->biHeight) {
    delete [] pro->m_pRGBTMP;
    delete [] pro->m_pRGBTRANS;
    pro->m_pRGBTMP = new BYTE[w * h * 3];
    pro->m_pRGBTRANS = new BYTE[w * h * 3];
  }

  if (IMAGE_UYVY == nImageType) {
    pro->biWidth = h;
    pro->biHeight = w;
    YUV422_C_RGB((BYTE*)cpbImage, pro->m_pRGBTMP, h, w);
    RGB_EDDY(pro->m_pRGBTMP, h, w, pro->m_pRGBTRANS);
  }
  else if (IMAGE_YUYV == nImageType) {
    pro->biWidth = w;
    pro->biHeight = h;
    YUYV_C_RGB((BYTE*)cpbImage, pro->m_pRGBTRANS, h, w);
  }

#if _WIN32

  if (IsWindow((HWND)pro->m_hWnd)) {
    VideoProCB_defult(pro);
  }

#endif

  if (pro->VideoCB) {
    pro->VideoCB(pro);
  }
}
BOOL CSock::StopVideo(int nSeq)
{
  char buf[ OFFSET_SIZE + sizeof(NCMD) ] = {0};
  unsigned int len = sizeof(NCMD);
  DevVideo Video(this, nSeq, buf + OFFSET_SIZE, len);
  BOOL ret = Send(buf + OFFSET_SIZE, &len);
  AoutLock lock(&m_syn);

  if (m_Pro) {
    CPM_ReleaseSock(m_Pro->m_hVideoHandle, NULL);
    delete [] m_Pro->m_pRGBTMP;
    m_Pro->m_pRGBTMP = NULL;
    delete [] m_Pro->m_pRGBTRANS;
    m_Pro->m_pRGBTRANS = NULL;
    //::ReleaseDC( it->second->m_hWnd, it->second->m_hDC );
    delete m_Pro;
    m_Pro = NULL;
  }

  return ret;
}

