#include "libCPMDev.inl"
//#include "netcmdhandle.h"
//#include "netcmdhandle.inl"
#include "libCPMServer.inl"
#include "./CPMServer/CNetS.inl"
const char* str_ip = NULL;
#define USERNAME ("admin")
#define PASSWORD ("201031")
unsigned int strhash(const char* str)
{
  unsigned int hash = 0;
  while (*str) {
    hash = hash * 33 + (*str++);
  }
  return hash;
}
int myrealloc(void** p, int n)
{
  *p = realloc(*p, n);
  return 0;
}
int strlist_count(char** sl)
{
  int n = 0;
  if (sl) {
    for (; sl[n]; ++n);
  }
  return n;
}
int strlist_free(char** * psl)
{
  if (*psl) {
    char** sl = *psl;
    for (; *sl; ++sl) {
      free(*sl);
    }
    free(sl);
    *psl = NULL;
  }
  return 0;
}
int strlist_set(char** sl, int i, const char* s)
{
  int n = strlist_count(sl);
  if (i < 0) {
    i = 0;
  }
  else if (i >= n) {
    i = n - 1;
  }
  free(sl[i]);
  sl[i] = strdup(s);
  return 0;
}
int strlist_insert(char** * sl, int i, const char* s)
{
  int n = strlist_count(*sl);
  if (i < 0) {
    i = 0;
  }
  else if (i > n) {
    i = n;
  }
  //i = BOUND(i, 0, n);
  ++n;
  myrealloc((void**)sl, sizeof(char*) * (n + 1));
  memmove((*sl) + i + 1, (*sl) + i, (n - i)*sizeof(char*));
  (*sl)[i] = strdup(s);
  (*sl)[n] = 0;
  return 0;
}
int strlist_insert_fmt(char** * sl, int i, const char* fmt, ...)
{
  va_list ARGLIST;
  char buf[1024];
  va_start(ARGLIST, fmt);
  _vsnprintf(buf, 1024, fmt, ARGLIST);
  va_end(ARGLIST);
  return strlist_insert(sl, i, buf);
}
int strlist_find(char** sl, int i, const char* s)
{
  int n = strlist_count(sl);
  for (; i < n; ++i) {
    if (0 == strcmp(sl[i], s)) {
      return i;
    }
  }
  return -1;
}
int strlist_del(char** sl, const char* s)
{
  int i = 0, k = 0, n = strlist_count(sl);
  while ((i = strlist_find(sl, i, s)) >= 0) {
    free(sl[i]);
    memmove(sl + i, sl + i + 1, (n - i - 1)*sizeof(char*));
    --n;
    sl[n] = 0;
    ++k;
  }
  return k;
}
char** m_dev = NULL;
char** m_log = NULL;
int CPMDEV_CALL DevConnectNotify(const char* dev_addr, DEV_CONNECT_STATUS flag, void* pvContext)
{
  char ip[16] = {0};
  BYTE field0 = 0, field1 = 0, field2 = 0, field3 = 0;
  //m_IPServer.GetAddress(field0, field1, field2, field3);
  //sprintf(ip, "%d.%d.%d.%d", field0, field1, field2, field3);
  switch (flag) {
  case DEV_CONNECT_NONE: {
    CPM_CNDev(dev_addr);
  }
  break;
  case DEV_CONNECT_FAILUE:
    DEBUG_INFO1(_T("%s 连接失败"), dev_addr);
    break;
  case DEV_CONNECT_SUCCESS: {
    DEBUG_INFO1(_T("%s 建立连接"), dev_addr);
    DEV_CID user = {0};
    DEV_CID pswd = {0};
    memcpy(user.m_ID , USERNAME , sizeof(USERNAME));
    memcpy(pswd.m_ID , PASSWORD , sizeof(PASSWORD));
    CPM_ULDevAuth(dev_addr, user, pswd, 1);
  }
  break;
  case DEV_CONNECT_CUT: {
    strlist_del(m_dev, dev_addr);
    DEBUG_INFO1(_T("%s 断开连接"), dev_addr);
  }
  break;
  case DEV_QUERY_ENCRYPT: {
    //when we connect to an unit, the SDK will ask whether or not to encrypt through this callback.
    //0 : don't encrypt
    //1 : need encrypt and it is server-side
    //2 : need encrypt and it is client-side
    return 0;
  }
  break;
  case SRV_QUERY_ENCRYPT: {
    //when we connect to transit server, the SDK will ask whether or not to encrypt through this callback.
    //0 : don't encrypt
    //1 : need encrypt and it is server-side
    //2 : need encrypt and it is client-side
  }
  return 0;
  default:
    break;
  }
  return 0;
}
#ifndef WIN32
#define Sleep usleep
#endif
#include "aaa.inl"
//void DevOperResult( const char* cszServerAddr, const char* cszDevAddr, NET_OPER_CMD eCMD, NET_OPER_RESULT eRt, void* pvContent, int nSN, void* param )
BOOL CPMDEV_CALL DevOperResult(const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult, void* pvContext)
{
#define OperResult(a, b) case a: return b( cszDevAddr, eType, eFlag , pvContent, nSeq , eResult );
  switch (eType) {
    OperResult(DEV_BASEINFO_OPER, DevBaseInfoOper);
    OperResult(DEV_AUTH_OPER, DevAuthOper);
    //OperResult(CMD_AUTH_SET, DevAuthOper);
    // OperResult(CMD_ADD_DEV, DevAddOper);
    OperResult(DEV_FORMAT_OPER, DevFormatOper);
    //用户
    OperResult(DEV_USER_OPER, DevUserInfoOper);
    OperResult(DEV_USER_EX_OPER, DevUserExOper);
    // OperResult(CMD_RTCAP_USER, DevUserInfoOper);
    // OperResult(CMD_RTUPDATE_USER, DevUserInfoOper);
    // OperResult(CMD_GET_USER_COUNT , DevUserInfoOper);
    OperResult(DEV_USER_RANGE_OPER, DevUserNumsOper);
    OperResult(DEV_USER_RT_OPER, DevRealLoadUser);
    //记录
    OperResult(DEV_RECORD_OPER, DevRecordInfoOper);
    OperResult(DEV_ALARM_OPER, DevRecordInfoOper);
    OperResult(DEV_RECORD_RANGE_OPER, DevRecNums);
    OperResult(DEV_ALARM_RANGE_OPER, DevRecNums);
    OperResult(DEV_LOG_OPER, DevLogInfoOper);
    OperResult(DEV_LOG_EX_OPER, DevLogExOper);
    OperResult(DEV_LOG_RANGE_OPER, DevLogNumsOper);
    OperResult(DEV_CIMAGE_EX_OPER, DevCImageExOper);
    OperResult(DEV_CHANGEIMAGE_OPER, DevChangeImageOper);
    OperResult(DEV_CIMAGE_RANGE_OPER, DevChangeImageNumsOper);
  default:
    printf("%s eType=%d\n", cszDevAddr, eType);
    break;
  }
  return TRUE;
}
#ifdef WIN32
#include "ui/window.inl"
int CPMDEV_CALL VideoProCB1(const struct VideoPro* pro)
{
#if 1
  const char* winname = (const char*)(pro->m_hWnd);
  BITMAPINFO m_pBitmapInfo[1] = {0};
  RECT m_Rect;
  CvWindow* w = icvFindWindowByName(winname);
  //HWND hWnd = (HWND)(pro->m_hWnd);
  HWND hWnd = w->hwnd;
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
#else
  printf("w=%d h=%d\n", pro->biWidth, pro->biHeight);
#endif
  return 0;
}
#else
#define cvNamedWindow(a, b)
#define cvWaitKey(a) usleep(50)
int VideoProCB1(const struct VideoPro* pro)
{
  //printf("w=%d h=%d\n", pro->biWidth, pro->biHeight);
  return 0;
}
#endif
//static int DotIpToLong(const char* ip, addr_inx* out)
int test_CPMDev()
{
  if (0) {
    addr_inx out;
    char buf[256];
    str_ip = "2001:a:2::3";
    DotIpToLong(str_ip, &out);
    LongToDotIp(buf, 256, &out);
    LongToDotIp(buf, 256, &out);
  }
  if (0) {
    SOCKET s = ::socket(AF_INET6, SOCK_STREAM, 0);
    str_ip = "2001:a:2::3";
    addr_in addr[1] = {0};
    net_addr(str_ip, SERVER_PORT, addr);
    int len = CPM_Connect(s, addr);
    if (len != 0) {
      fprintf(stderr, "connect() failed with error %d: %s\n", errno, strerror(errno));
    }
  }
  char winname[64] = "test_CPMDev";
  str_ip = "2001:a:2::27";
  str_ip = "192.168.1.123";
  str_ip = "2001:a:2::107";
  str_ip = "2001:a:2::3";
  str_ip = "ns3.mydebian.org";
  // rj.ch = 192.168.1.118 ry.ch = 192.168.1.107 csp.ch = 192.168.1.150
#if 0
  ipv4
  yw.ch 192.168.1.116
  rj.ch 192.168.1.106
  tdl.ch 192.168.1.117
  ry.ch 192.168.1.118
  ipv6
  ry.ch6
  rj.ch6
#endif
  str_ip = "csp.ch";
  str_ip = "192.168.1.150";
  str_ip = "rj.ch";
  str_ip = "ry.ch6";
  str_ip = "yw.ch";
  str_ip = "ry.ch6";
  str_ip = "192.168.1.107";
  str_ip = "tdl.ch";
  str_ip = "ry.ch";
  str_ip = "192.168.1.3";
  str_ip = "192.168.1.118";
  str_ip = "192.168.1.116";
  str_ip = "192.168.1.152";
  str_ip = "192.168.1.212";
  str_ip = "192.168.1.158";
  str_ip = "192.168.1.153";
  int cmd[256] = {0};
  int i = 0;
  CPM_InitSys(TRUE);
  CPM_RegDevConnectStatusCB(DevConnectNotify, NULL);
  CPM_RegOperResultNotifyCB(DevOperResult, NULL);
  CPM_CNDev(str_ip);
  cvNamedWindow(winname, 0);
  //CvWindow* w = icvFindWindowByName(winname);
  //HWND hWnd = (HWND)(pro->m_hWnd);
  //HWND hWnd = w->hwnd;
  CPM_StartVideo(str_ip, winname, VideoProCB1);
  for (i = 0;; ++i) {
    int ch = cvWaitKey(150);
    if ('q' == ch) {
      break;
    }
    if (ch > 0 && ch < 256) {
      cmd[ch] = !cmd[ch];
      switch (ch) {
      case 'c':
        CPM_CNDev(str_ip);
        break;
      case 'b':
        CPM_DLDevBaseInfo(str_ip);
        break;
      case 'd':
        CPM_DCNDev(str_ip);
        break;
      case 'v':
        CPM_StartVideo(str_ip, winname, VideoProCB1);
        break;
      case 'V':
        CPM_StopVideo(str_ip);
        break;
      }
    }
    if (0 == i % 1000) {
      printf("i = %d\n", i);
      CPM_DLDevBaseInfo(str_ip);
    }
  }
  CPM_DCNDev(str_ip);
  CPM_InitSys(0);
  return 0;
}
int test_CPMServer()
{
  CNetS nets;
  //InitSocket();
  nets.NetInit(AF_INET);
  nets.NetStart(true);
  cvNamedWindow("test_CPMServer", 0);
  for (;;) {
    cvWaitKey(-1);
  }
  return 0;
}

