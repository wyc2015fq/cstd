// DspDevManager.cpp: implementation of the DspDevManager class.
//
#include "DspDevManager.h"
//#include <process.h>
//#include <io.h>
#include "DevUser.h"
#include "DevID.h"
#include "DevAuth.h"
#include "DevBaseInfo.h"
#include "DevSysTime.h"
#include "DevRecord.h"
#include "DevStatis.h"
#include "DevSet.h"
#include "DevTimeGroup.h"
#include "DevRight.h"
#include "DevOpenDoor.h"
#include "DevRgn.h"
#include "DevFormat.h"
#include "DevLog.h"
#include "DevProUpdate.h"
#include "DevUserGroup.h"
#include "DevCImage.h"
#include "DevCN.h"
#include "DevVideo.h"
#define MCMDDEFDEF(MCMDDEF, TT) \
  MCMDDEF( TT, M_USER ) \
  MCMDDEF( TT, M_VERIFY ) \
  MCMDDEF( TT, M_ALARM ) \
  MCMDDEF( TT, M_OPTLOG ) \
  MCMDDEF( TT, M_CIMAGE ) \
  MCMDDEF( TT, M_RANGE ) \
  MCMDDEF( TT, M_AUTH ) \
  MCMDDEF( TT, M_TIME ) \
  MCMDDEF( TT, M_HEART ) \
  MCMDDEF( TT, M_DEVINFO ) \
  MCMDDEF( TT, M_STATIS ) \
  MCMDDEF( TT, M_DEVSET ) \
  MCMDDEF( TT, M_FORMAT ) \
  MCMDDEF( TT, M_DOOR_STATE ) \
  MCMDDEF( TT, M_REMOTE_OPEN ) \
  MCMDDEF( TT, M_TGROUP ) \
  MCMDDEF( TT, M_DSPRIGHT ) \
  MCMDDEF( TT, M_UGROUP ) \
  MCMDDEF( TT, M_MCAST ) \
  MCMDDEF( TT, M_APPUPDATE ) \
  MCMDDEF( TT, M_IMGCHANGE ) \
  MCMDDEF( TT, M_VIDEO_OUT ) \
  MCMDDEF( TT, M_USER_EX ) \
  MCMDDEF( TT, M_REBOOT ) \
  MCMDDEF( TT, M_RANGE_EX ) \
  MCMDDEF( TT, M_SET_IP ) \
  MCMDDEF( TT, M_NOPEN ) \
  MCMDDEF( TT, M_IOCTRL ) \
  MCMDDEF( TT, M_NETPRO ) \
  MCMDDEF( TT, M_RESETSCREEN )
static const char* MCmdtoString(int mcmd)
{
#define MCMDDEF(TT, XX ) if (mcmd==XX) return #XX ;
  MCMDDEFDEF(MCMDDEF, 0);
  return "";
}
void CPMDEV_CALL DevRecvFromCallBack(SOCKET hSock, const char* pbData, int nLen, const addr_in* addr, void* pvContext, void* user)
{
  DspDevManager* pNet = (DspDevManager*)pvContext;
  pNet->RecvFromNotify(hSock, pbData, nLen, addr, user);
}
void CPMDEV_CALL DevRecvCallBack(SOCKET hSock, const char* pbData, int nLen, void* pvContext, void* user)
{
  DspDevManager* pNet = (DspDevManager*)pvContext;
  pNet->RecvNotify(hSock, pbData, nLen, user);
}
void DspDevManager::RecvNotify(SOCKET hSock, const char* pbData, int nLen, void* user)
{
  CSock* s = (CSock*)user;

  if (!s) {
    return ;
  }

  PEERADR l_Addr = s->GetAddr();
  s->SetActive();//重置心跳包

  if (pbData == NULL && nLen < sizeof(NCMD)) {
    NetDCN(s, DEV_CONNECT_CUT);
    return;
  }

  NCMD* pCmd = (NCMD*)pbData;
  DEBUG_INFO3("SOCKET=%d, NCMD len = %d m_MCmd = %s", hSock, nLen, MCmdtoString(pCmd->m_MCmd));

  switch (pCmd->m_MCmd) {
  case C_CNDEV:
    if (S_CN == pCmd->m_SCmd) {
      if (GET_ERR(pCmd->m_CFlag) == SUC_EXECUT) {
        NetCN_Imp(l_Addr.IP_Address, hSock, false);
      }
      else {
        this->OnCNNotify(l_Addr.IP_Address, DEV_CONNECT_FAILUE);
      }
    }
    else if (S_DN == pCmd->m_SCmd) {
      NetDCN(s, DEV_CONNECT_CUT, false);
    }

    return;

  case C_HEART:
    //回发心跳
    CPM_Send(hSock, pbData, &nLen);
    return;

  case C_VERIFY:
    this->OnDevCMDNotify(s, NULL, CLI_AUTH_OPER, DEV_OPER_UPLOAD, NULL, pCmd->m_Param1, (DEV_OPER_RESULT)GET_ERR(pCmd->m_CFlag));
    return;

  default:
    break;
  }

  this->OnRecvNotify(s, pbData, nLen);
}
void DspDevManager::RecvFromNotify(SOCKET hSock, const char* pbData, int nLen, const addr_in* Addr, void* user)
{
  if (!m_bInit) {
    return;
  }

  BOOL bExit = false;
  CSock* s = (CSock*)user;

  if (NULL == s) {
    if (pbData && pbData[0] == SYN_ACK_CONNECT) {
      SerchNotify(s);
    }
  }
  else {
    s->OnRecvFromNotify(Addr, pbData, nLen);
  }
}
void DspDevManager::SerchNotify(const CSock* s)
{
  this->OnCNNotify(s->GetAddr().IP_Address, DEV_CONNECT_NONE);
}
BOOL DspDevManager::StartWork(BOOL bFlag)
{
  BOOL ret = false;

  if (bFlag) {
    if (!m_bInit && CPM_InitSock(USE_NET_MODEL , MAX_MEMRESERVER_SIZE, MEMRESERVER_UNIT)) {
      CPM_SetDataRecvFromCallBack(::DevRecvFromCallBack, this);
      CPM_SetDataRecvCallBack(::DevRecvCallBack, this);
      m_bInit = ret = CPM_StartWork(true);
    }
  }
  else {
    if (m_bInit) {
      m_bInit = false;
      {
        //主动断开所有连接
        AoutLock lock(&m_syn);
        SOCK_MAP::iterator it = m_map.begin();

        for (; it != m_map.end(); ++it) {
          SOCKET sock = it->second->GetHandle();

          if (VSOCKET != sock) {
            CPM_ReleaseSock(sock, it->second);
          }

          delete it->second;
        }

        m_map.clear();
      }
      CPM_StartWork(false);
      CPM_UnInitSock();
    }

    ret = true;
  }

  return ret;
}
void DspDevManager::CallSrvSB(const char* ip, PEERADR* addr)
{
  assert(TransMode());
  AoutLock lock(&m_SynSrvCb);

  if (m_SrvCb) {
    m_SrvCb(ip, addr, m_SrvCtx);
  }
}
BOOL DspDevManager::CnSrv(BOOL bFlag, const char* ip, const char* name, const char* psw)
{
  if (!m_bInit || !TransMode() || !StrIsIP(ip)) {
    return false;
  }

  //断开中转服务
  if (!bFlag) {
    return 0;//Net DCN( s , false);
  }

  //连接
  switch (NetCN_Imp_Real(ip, true)) {
  case LINK_ALREADY:
    return true;

  case LINK_FAIL:
    this->OnCNNotify(ip, DEV_CONNECT_FAILUE);
    return false;

  case LINK_OK:
    this->OnCNNotify(ip, DEV_CONNECT_SUCCESS);
    {
      //发送身份验证
      int temp[ ALIGN4(OFFSET_SIZE + sizeof(NCMD) + sizeof(AUTH)) >> 2 ] = { 0 };
      NCMD* pCmd = (NCMD*)((char*)temp + OFFSET_SIZE);
      AUTH* pAuth = (AUTH*)(pCmd + 1);
      pCmd->m_MCmd = C_VERIFY;
      DotIpToLong(ip, &pCmd->m_Param2);

      if (name && strlen(name) > 0) {
        strncpy(pAuth->m_UserName.m_ID, name, sizeof(CID) - 1);
      }

      if (psw && strlen(psw) > 0) {
        strncpy(pAuth->m_Password.m_ID, psw, sizeof(CID) - 1);
      }

      UINT ulen = sizeof(NCMD) + sizeof(AUTH);
      //Send(ip, (char*)pCmd, &ulen);
      ASSERT(0);
    }
    return true;

  default:
    assert(false);
  }

  return false;
}
CSock* DspDevManager::FindTranSrv(const PEERADR& addr)
{
  assert(TransMode());
  AoutLock lock(&m_syn);
  SOCK_MAP::iterator it = m_map.begin();

  for (; it != m_map.end(); ++it) {
    if (it->second->GetHandle() != VSOCKET) {
      if (strcmp(addr.IP_Address, it->second->GetAddr().IP_Address) == 0) {
        return it->second;
      }
    }
  }

  return NULL;
}
BOOL DspDevManager::NetDCN(CSock* s, E_CONNECT_STATUS eFlag, BOOL tri)
{
  if (!m_bInit) {
    return false;
  }

  BOOL bExit = false;

  if (s) {
    AoutLock lock(&m_syn);
    SOCK_MAP::iterator it = m_map.find(s->GetAddr().IP_Address);

    if (it != m_map.end()) {
      //对方断开连接
      PEERADR l_Addr = s->GetAddr();
      s->StopVideo(0);
      this->OnCNNotify(l_Addr.IP_Address, eFlag);
      m_map.erase(it);
      delete s;
      bExit = true;
    }
  }

  return bExit;
}
BOOL DspDevManager::NetCN_Imp(const char* cszIP, SOCKET rSock, BOOL tri)
{
  if (!m_bInit || !StrIsIP(cszIP)) {
    return false;
  }

  switch (NetCN_Imp_Real(cszIP, false)) {
  case LINK_ALREADY:
    printf("LINK_ALREADY\n");
    return true;

  case LINK_FAIL:
    this->OnCNNotify(cszIP, DEV_CONNECT_FAILUE);
    return false;

  case LINK_OK:
    g_ip_addr_add_or_del(cszIP, &l, NULL);
    this->OnCNNotify(cszIP, DEV_CONNECT_SUCCESS);
    return true;

  default:
    assert(false);
  }

  return false;
}
int DspDevManager::NetCN_Imp_Real(const char* cszIP, BOOL srv)
{
  std::string l(cszIP);
  CSock* s = getSock(cszIP);

  if (s) {
    return LINK_ALREADY;
  }

  int linkCode = LINK_FAIL;
  SOCKET hSock;
  addr_in addr[1] = {0};
  net_addr(cszIP, SERVER_PORT, addr);
  hSock = CPM_CreateSock(addr->family, SOCK_STREAM);

  if (hSock != INVALID_SOCKET) {
    m_synLink.Lock();
    BOOL bRst = CPM_Connect(hSock, addr);
    m_synLink.Unlock();

    if (!bRst) {
      CPM_ReleaseSock(hSock, NULL);
      hSock = NULL;
    }
    else {
      CSock* s = NULL;
      m_syn.Lock();
      m_map.insert(std::pair<std::string, CSock*>(l, s = new CSock(hSock, hSock, addr, cszIP, SERVER_PORT, TRUE)));
      m_syn.Unlock();
      CPM_SetSendBufSize(hSock, SEND_BUF);
      CPM_SetRecvBufSize(hSock, RECV_BUF);
      CPM_Recv(hSock , this->OnCNNotify(cszIP, srv ? SRV_QUERY_ENCRYPT : DEV_QUERY_ENCRYPT), s);
      linkCode = LINK_OK;
    }
  }

  return linkCode;
}
BOOL DspDevManager::SerchBegin(const char* cszIP)
{
  addr_in addr[1] = {0};
  net_addr(cszIP, MULTYGROUP_PORT, addr);
  SOCKET hSock = NULL;
  std::string l(cszIP);
  {
    m_syn.Lock();
    SOCK_MAP::iterator it = m_map.find(l);

    if (it != m_map.end()) {
      hSock = it->second->GetHandle();
    }

    m_syn.Unlock();

    if (!hSock) {
      hSock = CPM_CreateSock(addr->family, SOCK_DGRAM);

      if (hSock) {
        addr_in addrLocal[1] = {0};

        if (CPM_GetLocalIP(addr, 0, addrLocal) && CPM_Bind(hSock, addrLocal)) {
          CSock* s = NULL;
          m_syn.Lock();
          m_map.insert(std::pair<std::string, CSock*>(l, s = new CSock(hSock, hSock, addrLocal, cszIP, MULTYGROUP_PORT, false)));
          m_syn.Unlock();
          CPM_Recv(hSock , 0, s);
        }
        else {
          CPM_ReleaseSock(hSock, NULL);
          hSock = NULL;
        }
      }
    }
  }

  if (hSock) {
    int szData[ ALIGN(OFFSET_SIZE + 2, 4) >> 2 ] = {0};
    char* tmp = (char*)szData;
    tmp[OFFSET_SIZE] = SYN_REQUES_CONNECT;
    int nLen = 2;
    return CPM_SendTo(hSock, addr, tmp + OFFSET_SIZE, &nLen);
  }

  return false;
}
BOOL DspDevManager::SerchEnd(const char* cszIP)
{
  std::string l(cszIP);
  SOCKET hSock = NULL;
  {
    AoutLock lock(&m_syn);
    SOCK_MAP::iterator it = m_map.find(l);

    if (it != m_map.end()) {
      hSock = it->second->GetHandle();
      delete it->second;
      m_map.erase(it);
    }
  }

  if (hSock) {
    CPM_ReleaseSock(hSock, NULL);
  }

  return true;
}
void DspDevManager::TimeOut(int nDelay)
{
  AoutLock lock(&m_syn);
  SOCK_MAP::iterator it = m_map.begin();

  for (; it != m_map.end();) {
    CSock* s = it->second;
    ++it;

    if (s && !s->IsActive()) {
      printf("TimeOut\n");
      assert(VSOCKET != s->GetHandle());
      CPM_ReleaseSock(s->GetHandle(), s);
    }
  }
}
void DevCMDNotify(const CSock* s,
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
  pManager->OnDevCMDNotify(s, cszClientAddr, eType, eFlag, pvContent, nSeq, rt);
}
void CNNotify(const CSock* s, E_CONNECT_STATUS eFlag, BOOL bServer, void* pvClientData)
{
  DspDevManager* pManager = (DspDevManager*)pvClientData;
  pManager->OnCNNotify(s->GetAddr().IP_Address, eFlag);
}
BOOL InternalSend(const CSock* s, const char* pbBuf, unsigned int uLen, void* pvClientData)
{
  DspDevManager* pManager = (DspDevManager*)pvClientData;
  return s->Send(pbBuf, &uLen);
}
DspDevManager::DspDevManager() :
  m_pfnCN(NULL),
  m_pCNContext(NULL),
  m_pfnOpNotify(NULL),
  m_pOpNotifyContext(NULL),
  m_bInit(FALSE), m_Trans(FALSE), m_SrvCb(NULL), m_SrvCtx(NULL)
{
}
DspDevManager::~DspDevManager()
{
  StartWork(FALSE);
}
void DspDevManager::Run()
{
  uutime m_Inval;

  while (m_bRunning) {
    Sleep(2 * 1000);

    if (m_Inval.elapsed() > 6) {
      TimeOut();
      m_Inval.restart();
    }
  }
}
int DspDevManager::OnCNNotify(const char* sIP, E_CONNECT_STATUS eFlag, BOOL bServer)
{
  //AoutLock lock( &m_syn );
  int type = 0;

  if (m_pfnCN) {
    type = (*m_pfnCN)(sIP , eFlag, m_pCNContext);
  }

  return type;
}
void DspDevManager::OnDevCMDNotify(const CSock* s,
    const char* cszClientAddr,
    DEV_OPER_TYPE eType,
    DEV_OPER_FLAG eFlag,
    void* pvContent,
    int nSeq,
    DEV_OPER_RESULT eResult
                                  )
{
  //AoutLock lock( &m_syn );
  //DEBUG_INFO4(_T("1 %s , %d , %x , %d") , s->GetAddr().IP_Address , (int)eType , m_pOpNotifyContext , (int)eResult);
  if (m_pfnOpNotify) {
    m_pfnOpNotify(s->GetAddr().IP_Address, eType, eFlag, pvContent, nSeq, eResult, m_pOpNotifyContext);
  }

  //DEBUG_INFO4(_T("2 %s , %d , %x , %d") , s->GetAddr().IP_Address , (int)eType , m_pOpNotifyContext , (int)eResult);
}
void DspDevManager::OnRecvNotify(const CSock* s, const char* cpbBuf, unsigned int uLen, BOOL bServer)
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
    pCMDObj->Recv(s, pNCMD, bServer, this);
  }
}
BOOL DspDevManager::SetMode(BOOL srvMode, QuerySrvCB pfn, void* pvContext)
{
  AoutLock lock(&m_SynSrvCb);
  m_SrvCb = pfn;
  m_SrvCtx = pvContext;
  m_Trans = srvMode;
  return true;
}
BOOL DspDevManager::InitSys(BOOL bFlag)
{
  BOOL ret = TRUE;

  if (bFlag) {
    ret = StartWork(TRUE);

    if (ret) {
      CMemPool::GetInstance().CreateMemPool(1024 * 1024 * 2, MEMRESERVER_UNIT);
      CThread::StartThread();
    }
  }
  else {
    CThread::StopThread();
    StartWork(FALSE);
  }

  return ret;
}
BOOL DspDevManager::DevSerch(BOOL bFlag, const char* cszIP)
{
  if (!m_bInit || !StrIsIP(cszIP)) {
    return false;
  }

  return bFlag ? SerchBegin(cszIP) : SerchEnd(cszIP);
}
BOOL DspDevManager::CNDev(const char* cszIP)
{
  return NetCN_Imp(cszIP, NULL, true);
}
void DspDevManager::DCNDev(const char* cszIP)
{
  CSock* s = getSock(cszIP);

  if (s) {
    //NetDCN(s, DEV_CONNECT_CUT); //
    CPM_ReleaseSock(s->m_sSock, s);// 由回调调用NetDCN
  }
}

