// Client.h: interface for the CClient class.
//
#if !defined(AFX_CLIENT_H__CD9B265C_3BEB_4F36_AFF1_7B9E3842A950__INCLUDED_)
#define AFX_CLIENT_H__CD9B265C_3BEB_4F36_AFF1_7B9E3842A950__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define MAX_TIME_INTERVAL 30
#define VSOCKET           ((SOCKET)0xffffffff)
enum { LINK_FAIL = 0, LINK_OK, LINK_ALREADY };
class DspDevManager;
#include "yuv2bmp.h"
#include "dspdatestruct.h"
class CSock
{
public:
  addr_in addr;
  SOCKET  m_hSOCK;
  SOCKET  m_sSock;
  PEERADR m_Addr;
  uutime  m_uutime;
  BOOL    m_bCalTimeOut;
  VideoPro* m_Pro;
  CriticalSection m_syn;
public:
  CSock(SOCKET hSOCK, SOCKET sSOCK, addr_in* paddr, const char* cszIP, USHORT port = 0, BOOL bCalTimeOut = TRUE):
    m_hSOCK(hSOCK), m_sSock(sSOCK), m_bCalTimeOut(bCalTimeOut) {
    memset(&m_Addr, 0x0, sizeof(m_Addr));
    strncpy(m_Addr.IP_Address, cszIP, sizeof(m_Addr.IP_Address) - 1);
    m_Addr.Port = port;
    addr = *paddr;
    m_Pro = NULL;
  }
  ~CSock() {}
public:
  SOCKET GetHandle(void) const {
    return m_hSOCK;
  }
  SOCKET GetRsock(void) const {
    return m_sSock;
  }
  const PEERADR& GetAddr() const {
    return m_Addr;
  }
  BOOL IsActive() {
    //return true;
    return  m_bCalTimeOut ? (m_uutime.elapsed() < MAX_TIME_INTERVAL) : true ;
  }
  void SetActive() {
    m_uutime.restart();
  }
  BOOL NetDCN() {
    int temp[ ALIGN4(OFFSET_SIZE + sizeof(NCMD)) >> 2 ] = { 0 };
    NCMD* pCMD = (NCMD*)((char*)temp + OFFSET_SIZE);
    pCMD->m_MCmd = C_CNDEV;
    pCMD->m_SCmd = S_DN;
    addr_to_addrx(&pCMD->m_Param2);
    int nLen = sizeof(NCMD);
    return CPM_Send(m_hSOCK, (char*)pCMD, &nLen);
  }
  BOOL NetCN_Imp() {
    int temp[ ALIGN4(OFFSET_SIZE + sizeof(NCMD)) >> 2 ] = { 0 };
    NCMD* pCMD = (NCMD*)((char*)temp + OFFSET_SIZE);
    pCMD->m_MCmd = C_CNDEV;
    pCMD->m_SCmd = S_CN;
    addr_to_addrx(&pCMD->m_Param2);
    int nLen = sizeof(NCMD);
    return CPM_Send(m_hSOCK, (char*)pCMD, &nLen);
  }
  void setNCMD(NCMD* pTMP, int m_MCmd, int m_SCmd, int m_CFlag = 0, int m_Param1 = 0) const {
    //NCMD pTMP[1] = {0};
    pTMP->m_MCmd = m_MCmd;
    pTMP->m_SCmd = m_SCmd;
    pTMP->m_CFlag = m_CFlag;
    pTMP->m_Param1 = m_Param1;
    addr_to_addrx(&pTMP->m_Param2);
  }
  void addr_to_addrx(addr_inx* pout) const {
#ifdef USE_IPV6
    *pout = addr;
#else

    if (AF_INET == addr.family) {
      struct sockaddr_in* p = (struct sockaddr_in*)&addr;
      //unsigned int aa = 0;
      //DotIpToLong1("192.168.1.150", &aa);
      *pout = p->sin_addr.s_addr;
    }

#endif // USE_IPV6
  }

  BOOL Send(const char* pbBuf, unsigned int* puLen) const {
    return CPM_Send(m_sSock, pbBuf, (int*)puLen);
  }
  void OnRecvFromNotify(const addr_in* Addr, const char* cpbBuf, unsigned int uLen) {
    if (uLen < sizeof(NCMD)) {
      return ;
    }

    NCMD* pNCMD = (NCMD*)cpbBuf;

    if (pNCMD->m_MCmd == CMD_M_VIDEO_OUT) {
      DEVIMAGE* pImage = (DEVIMAGE*)(pNCMD + 1);
      ShowVideo(pImage->m_Block, pImage->m_W, pImage->m_H, pImage->m_Step, pImage->m_Type);
    }
  }

  void RecvNotify(SOCKET hSock, const char* pbData, int nLen, void* user);
  //验证
  BOOL Auth(const DEV_CID& cName, const DEV_CID& cPws, int nFlag, int nSeq = -1);
  //区域
  BOOL SetDevRgn(const char* cszGroupIP = DEV_REGION_ADDR, int nSeq = -1);
  BOOL GetDevRgn(int nSeq = -1);
  //时间
  BOOL SetDevTime(const DEV_DATETIME& cTime, int nSeq = -1);
  BOOL GetDevTime(int nSeq = -1);
  //硬件基本信息
  BOOL GetDevInfo(int nSeq = -1);
  //统计信息
  BOOL GetDevStatis(int nSeq = -1);
  //工作属性
  BOOL SetDevWorkAtt(const DEV_WORKATT& cAtt, int nSeq = -1);
  BOOL GetDevWorkAtt(int nSeq = -1);
  //格式化设备
  BOOL FormatDev(int nSeq = -1);
  //更新设备程序
  BOOL UpdateApp(const DEV_APPUPDATE& cData, int nSeq = -1);
  //用户
  BOOL AddDevUser(const DEV_USER& cUser, int nSeq = -1);
  BOOL AddDevRealTimeUser(const DEV_USER& cUser, const DEV_USER_REAL_LOAD& cLoad);
  BOOL RealTimeUserCap(const DEV_USER& cUser, int nSeq) ;
  BOOL GetDevUser(const DEV_DATA_DL& cDlData, int nSeq = -1);
  BOOL DelDevUser(const DEV_CID& cID, int nSeq = -1);
  //记录
  BOOL GetDevRecord(int nClass, const DEV_DATA_DL& cDlData, int nSeq = -1);
  //紧急开门
  BOOL SetOpenDoorState(DEV_DOOR_STATE eState, int nSeq = -1);
  BOOL GetOpenDoorState(int nSeq = -1);
  BOOL RemoteOpenDoor(BOOL bOpen, int nSeq = -1);
  BOOL IOCtrl(const DEV_IOCTRL& ioctrl, int nSeq = -1);
  //时间组
  BOOL SetDevTG(const DEV_TIMEGROUP_ARRAY& cArray, int nSeq = -1);
  BOOL GetDevTG(const DEV_TIMEGROUP_ARRAY& cArray, DEV_TIMEGROUP_TYPE eType, int nSeq = -1);
  BOOL DelDevTG(const DEV_TIMEGROUP_ARRAY& cArray, DEV_TIMEGROUP_TYPE eType, int nSeq = -1);
  //权限
  BOOL SetDevRT(const DEV_RIGHT_ARRAY& cArray, int nSeq = -1);
  BOOL GetDevRT(const DEV_RIGHT_ARRAY& cArray, int nSeq = -1);
  BOOL DelDevRT(const DEV_RIGHT_ARRAY& cArray, int nSeq = -1);
  //用户组
  BOOL SetDevUserGroup(const DEV_USERGROUP_ARRAY& cArray, int nSeq);
  BOOL GetDevUserGroup(const DEV_USERGROUP_ARRAY& cArray, int nSeq);
  BOOL DelDevUserGroup(const DEV_USERGROUP_ARRAY& cArray, int nSeq);
  //视频传输
  BOOL StartVideo(void* hwnd, VideoPro_CallBack VideoCB, int nSeq);
  BOOL StopVideo(int nSeq);
  void ShowVideo(const char* cpbImage, int w, int h, int step, int nImageType);
  //用户扩展操作
  BOOL ULUserEx(const DEV_BATCH_USER& cUsers, int nFlag, int nSeq);
  BOOL DLUserEx(DEV_CID* pUserIDArray, int nIDCount, int nFlag, int nSeq);
  BOOL DELUserEx(DEV_CID* pUserIDArray, int nIDCount, int nSeq);
  //记录扩展操作
  BOOL DLRecEx(const DEV_DL_RECORD_RANGE& cRange, int nSeq /*= -1*/);
  //设备重启
  BOOL RebootDev(int nSeq /*= -1*/);
  //获得和设置网络属性
  BOOL DevNetPro(DEV_NETPRO* pNetPro , int nSeq);
  //屏幕校验
  BOOL ResetScreen(int nSeq /*= -1*/);

};
#endif // !defined(AFX_CLIENT_H__CD9B265C_3BEB_4F36_AFF1_7B9E3842A950__INCLUDED_)

