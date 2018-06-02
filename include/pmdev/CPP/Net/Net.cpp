// Client.cpp: implementation of the CNet class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "Net.h"

#include "../DspDevManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


#define SYN_REQUES_CONNECT    0x01            //同步连接请求
#define SYN_ACK_CONNECT       0x10            //同步连接确认
#define MULTYGROUP_PORT       13332           //多播组端口
#define MULTYGROUP_IP         "224.0.1.100"   //多播组地址
#define MAX_MEMRESERVER_SIZE (1024*1024)
#define SERVER_PORT           13333
#define VIDEO_PORT            23334
#define BINDE_TIMES           20
#define SEND_TIMEOUT          1
#define SEND_BUF              (64*1024)
#define RECV_BUF              (64*1024)

void DevRecvFromCallBack(HANDLE hSOCK, const char* pbData, int nLen, const SOCK_ADDRESS& addr, void* pvContext)
{
  CNet* pNet = (CNet*)pvContext;
  pNet->RecvFromNotify(hSOCK, pbData, nLen, addr);
}

void DevRecvCallBack(HANDLE hSOCK, const char* pbData, int nLen, void* pvContext)
{
  CNet* pNet = (CNet*)pvContext;
  pNet->RecvNotify(hSOCK, pbData, nLen);
}

void CNet::RecvNotify(HANDLE hSOCK, const char* pbData, int nLen)
{
  if (!m_bInit) {
    return;
  }

  if (pbData == NULL && nLen == 0) {
    //对方断开连接
    NetDCN(hSOCK);
    return;
  }

  assert(nLen >= sizeof(NCMD));

  PEERADR l_Addr = { 0 };
  BOOL bFind  = false;
  {
    AoutLock lock(&m_syn);
    SOCK_MAP::iterator it = m_map.begin();

    for (; it != m_map.end(); ++it) {
      if (it->second->GetHandle() == hSOCK) {
        l_Addr = it->second->GetAddr();
        it->second->SetActive();//重置心跳包
        bFind = true;
        break;
      }
    }
  }

  if (!bFind) {
    return;
  }

  NCMD* pCmd = (NCMD*)pbData;

  if (pCmd->m_Param2) {
    memset(&l_Addr, 0, sizeof(l_Addr));
    LongToDotIp(l_Addr.IP_Address, pCmd->m_Param2);
  }

  assert(StrIsIP(l_Addr.IP_Address));

  switch (pCmd->m_MCmd) {
  case C_CNDEV:
    if (S_CN == pCmd->m_SCmd) {
      if (GET_ERR(pCmd->m_CFlag) == SUC_EXECUT) {
        NetCN_Imp(l_Addr.IP_Address, hSOCK, false);
      }
      else {
        m_Manager->OnCNNotify(l_Addr.IP_Address, DEV_CONNECT_FAILUE);
      }
    }
    else if (S_DN == pCmd->m_SCmd) {
      NetDCN(l_Addr.IP_Address, false);
    }

    return;

  case C_HEART:
    //回发心跳
    CPM_Send(hSOCK, pbData, &nLen);
    return;

  case C_VERIFY:
    m_Manager->OnDevCMDNotify(l_Addr.IP_Address, NULL, CLI_AUTH_OPER, DEV_OPER_UPLOAD,
        NULL, pCmd->m_Param1, (DEV_OPER_RESULT)GET_ERR(pCmd->m_CFlag));
    return;

  default:
    break;
  }

  m_Manager->OnRecvNotify(l_Addr.IP_Address,  pbData, nLen);
}

void CNet::RecvFromNotify(HANDLE hSOCK, const char* pbData, int nLen, const SOCK_ADDRESS& Addr)
{
  if (!m_bInit) {
    return;
  }

  BOOL bExit = false;
  {
    AoutLock lock(&m_syn);
    SOCK_MAP::iterator it = m_map.begin();

    for (; it != m_map.end(); ++it) {
      if (it->second->GetHandle() == hSOCK) {
        bExit = true;
        break;
      }
    }
  }

  if (bExit) {
    if (pbData && pbData[0] == SYN_ACK_CONNECT) {
      SerchNotify(Addr.IP_Address);
    }
  }
  else {
    m_Manager->OnRecvFromNotify(Addr, pbData, nLen);
  }
}

void CNet::SerchNotify(const char* cszIP)
{
  BOOL bExit = false;
  {
    //过滤掉已经连接的设备
    unsigned long l = DotIpToLong(cszIP);
    AoutLock lock(&m_syn);
    SOCK_MAP::iterator it = m_map.find(l);
    bExit = (it != m_map.end());
  }

  if (!bExit) {
    m_Manager->OnCNNotify(cszIP, DEV_CONNECT_NONE);
  }
}

BOOL CNet::StartWork(BOOL bFlag)
{
  BOOL ret = false;

  if (bFlag) {
    if (!m_bInit && CPM_InitSock(NET_OVERLAPPED, MAX_MEMRESERVER_SIZE, MEMRESERVER_UNIT)) {
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
          HANDLE sock = it->second->GetHandle();

          if (VSOCKET != sock) {
            CPM_ReleaseSock(sock);
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

BOOL CNet::Serch(BOOL bFlag, const char* cszIP)
{
  if (!m_bInit || !StrIsIP(cszIP)) {
    return false;
  }

  return bFlag ? SerchBegin(cszIP) : SerchEnd(cszIP);
}

BOOL CNet::NetCN(BOOL bFlag, const char* cszIP)
{
  if (!m_bInit) {
    return false;
  }

  return bFlag ? NetCN_Imp(cszIP, NULL, true) : NetDCN(cszIP, true);
}

BOOL CNet::SetMode(BOOL srvMode, QuerySrvCB pfn,  void* pvContext)
{
  AoutLock lock(&m_SynSrvCb);
  m_SrvCb  = pfn;
  m_SrvCtx = pvContext;
  m_Trans  = srvMode;
  return true;
}

void CNet::CallSrvSB(const char* ip, PEERADR& addr)
{
  assert(TransMode());
  AoutLock lock(&m_SynSrvCb);

  if (m_SrvCb) {
    m_SrvCb(ip, addr, m_SrvCtx);
  }
}

BOOL CNet::CnSrv(BOOL bFlag, const char* ip, const char* name, const char* psw)
{
  if (!m_bInit || !TransMode() || !StrIsIP(ip)) {
    return false;
  }

  //断开中转服务
  if (!bFlag) {
    return NetDCN(ip , false);
  }

  //连接
  switch (NetCN_Imp(ip)) {
  case LINK_ALREADY:
    return true;

  case LINK_FAIL:
    m_Manager->OnCNNotify(ip, DEV_CONNECT_FAILUE);
    return false;

  case LINK_OK:
    m_Manager->OnCNNotify(ip, DEV_CONNECT_SUCCESS);
    {
      //发送身份验证
      int temp[ ALIGN4(OFFSET_SIZE + sizeof(NCMD) + sizeof(AUTH)) >> 2 ] = { 0 };
      NCMD* pCmd     = (NCMD*)((char*)temp + OFFSET_SIZE);
      AUTH* pAuth    = (AUTH*)(pCmd + 1);
      pCmd->m_MCmd   = C_VERIFY;
      pCmd->m_Param2 = DotIpToLong(ip);

      if (name && strlen(name) > 0) {
        strncpy(pAuth->m_UserName.m_ID, name, sizeof(CID) - 1);
      }

      if (psw && strlen(psw) > 0) {
        strncpy(pAuth->m_Password.m_ID, psw, sizeof(CID) - 1);
      }

      UINT ulen = sizeof(NCMD) + sizeof(AUTH);
      Send(ip, (char*)pCmd, &ulen);
    }
    return true;

  default:
    assert(false);
  }

  return false;
}

HANDLE CNet::FindTranSrv(const PEERADR& addr)
{
  assert(TransMode());
  AoutLock lock(&m_syn);
  SOCK_MAP::iterator it = m_map.begin();

  for (; it != m_map.end(); ++it) {
    if (it->second->GetHandle() != VSOCKET) {
      if (!StrIsIP(addr.IP_Address)) {
        return it->second->GetHandle();
      }
      else if (strcmp(addr.IP_Address, it->second->GetAddr().IP_Address) == 0) {
        return it->second->GetHandle();
      }
    }
  }

  return NULL;
}

BOOL CNet::Send(const char* cszIP, const char* pbBuf, unsigned int* puLen)
{
  if (!m_bInit || !StrIsIP(cszIP)) {
    return false;
  }

  HANDLE hSOCK = 0;
  {
    //操作目的设备是否已经连接
    AoutLock lock(&m_syn);
    SOCK_MAP::iterator it = m_map.find(DotIpToLong(cszIP));

    if (it == m_map.end()) {
      return false;
    }
    else {
      hSOCK = it->second->GetRsock();
    }
  }
  assert(hSOCK);
  return CPM_Send(hSOCK, pbBuf, (int*)puLen);
}

BOOL CNet::NetDCN(HANDLE hSOCK)
{
  if (!m_bInit) {
    return false;
  }

  PEERADR l_Addr = {0};
  BOOL bExit  = false;
  {
    AoutLock lock(&m_syn);
    SOCK_MAP::iterator it = m_map.begin();

    for (; it != m_map.end(); ++it) {
      if (it->second->GetHandle() == hSOCK) {
        l_Addr = it->second->GetAddr();
        delete it->second;
        m_map.erase(it);
        bExit = true;
        break;
      }
    }
  }

  if (bExit) {
    m_Manager->OnCNNotify(l_Addr.IP_Address, DEV_CONNECT_CUT);

    if (TransMode()) {
      //中转服务器断开,断开所有设备
      DEBUG_INFO1(_T("与中转服务连接断开：%s"), l_Addr.IP_Address);
      assert(hSOCK != VSOCKET);
      AoutLock lock(&m_syn);
      SOCK_MAP::iterator it = m_map.begin();

      while (it != m_map.end()) {
        if (it->second->GetRsock() == hSOCK) {
          assert(it->second->GetHandle() == VSOCKET);
          m_Manager->OnCNNotify(it->second->GetAddr().IP_Address, DEV_CONNECT_CUT);
          delete it->second;
          m_map.erase(it++);
          continue;
        }

        ++it;
      }
    }
  }

  return bExit;
}

BOOL CNet::NetDCN(const char* cszIP, BOOL tri)
{
  if (!m_bInit || !StrIsIP(cszIP)) {
    return false;
  }

  unsigned long l = DotIpToLong(cszIP);

  if (TransMode()) {
    if (tri) {
      //发送设备断开命令
      PEERADR l_Addr = {0};
      CallSrvSB(cszIP, l_Addr);
      HANDLE Sock = FindTranSrv(l_Addr);

      if (!Sock) {
        //未找到中转服务器
        m_Manager->OnCNNotify(cszIP, DEV_CONNECT_FAILUE);
        return false;
      }

      int temp[ ALIGN4(OFFSET_SIZE + sizeof(NCMD)) >> 2 ] = { 0 };
      NCMD* pCMD = (NCMD*)((char*)temp + OFFSET_SIZE);
      pCMD->m_MCmd   = C_CNDEV;
      pCMD->m_SCmd   = S_DN;
      pCMD->m_Param2 = l;
      int nLen = sizeof(NCMD);
      return CPM_Send(Sock, (char*)pCMD, &nLen);
    }
  }

  HANDLE temp = 0;
  {
    AoutLock lock(&m_syn);
    SOCK_MAP::iterator it = m_map.find(l);

    if (it != m_map.end()) {
      temp = it->second->GetHandle();

      if (VSOCKET == temp) {
        //伪装socket连接，直接删除
        delete it->second;
        m_map.erase(it);
      }
    }
  }

  if (temp) {
    if (VSOCKET == temp) {
      m_Manager->OnCNNotify(cszIP, DEV_CONNECT_CUT);
    }
    else {
      CPM_ReleaseSock(temp);
    }
  }

  return temp != NULL;
}

BOOL CNet::NetCN_Imp(const char* cszIP, HANDLE rSock, BOOL tri)
{
  if (!m_bInit || !StrIsIP(cszIP)) {
    return false;
  }

  if (TransMode()) {
    unsigned long l = DotIpToLong(cszIP);
    {
      AoutLock lock(&m_syn);
      SOCK_MAP::iterator it = m_map.find(l);

      if (it != m_map.end()) {
        return true;
      }
    }

    if (tri) {
      PEERADR l_Addr = {0};
      CallSrvSB(cszIP, l_Addr);
      HANDLE Sock = FindTranSrv(l_Addr);

      if (!Sock) {
        //未找到中转服务器
        m_Manager->OnCNNotify(cszIP, DEV_CONNECT_FAILUE);
        return false;
      }

      int temp[ ALIGN4(OFFSET_SIZE + sizeof(NCMD)) >> 2 ] = { 0 };
      NCMD* pCMD = (NCMD*)((char*)temp + OFFSET_SIZE);
      pCMD->m_MCmd   = C_CNDEV;
      pCMD->m_SCmd   = S_CN;
      pCMD->m_Param2 = l;
      int nLen = sizeof(NCMD);
      return CPM_Send(Sock, (char*)pCMD, &nLen);
    }
    else {
      m_syn.Lock();
      m_map.insert(std::pair<unsigned long, CSock*>(l, new CSock(VSOCKET, rSock, cszIP, 0, FALSE)));
      m_syn.Unlock();
      m_Manager->OnCNNotify(cszIP, DEV_CONNECT_SUCCESS);
      return true;
    }
  }
  else {
    switch (NetCN_Imp(cszIP)) {
    case LINK_ALREADY:
      return true;

    case LINK_FAIL:
      m_Manager->OnCNNotify(cszIP, DEV_CONNECT_FAILUE);
      return false;

    case LINK_OK:
      m_Manager->OnCNNotify(cszIP, DEV_CONNECT_SUCCESS);
      return true;

    default:
      assert(false);
    }
  }

  return false;
}

long CNet::NetCN_Imp(const char* cszIP)
{
  assert(StrIsIP(cszIP));
  unsigned long l = DotIpToLong(cszIP);
  {
    AoutLock lock(&m_syn);
    SOCK_MAP::iterator it = m_map.find(l);

    if (it != m_map.end()) {
      return LINK_ALREADY;
    }
  }

  long linkCode = LINK_FAIL;
  HANDLE hSOCK = CPM_CreateSock(NET_SOCK_STREAM);

  if (hSOCK != NULL) {
    SOCK_ADDRESS addr = {0};
    addr.Port = SERVER_PORT;
    strncpy(addr.IP_Address, cszIP, sizeof(addr.IP_Address) - 1);

    m_synLink.Lock();
    BOOL bRst = CPM_Connect(hSOCK, addr);
    m_synLink.Unlock();

    if (!bRst) {
      CPM_ReleaseSock(hSOCK);
      hSOCK = NULL;
    }
    else {
      m_syn.Lock();
      m_map.insert(std::pair<unsigned long, CSock*>(l, new CSock(hSOCK, hSOCK, addr.IP_Address, addr.Port, TRUE)));
      m_syn.Unlock();
      CPM_SetSendBufSize(hSOCK, SEND_BUF);
      CPM_SetRecvBufSize(hSOCK, RECV_BUF);
      CPM_Recv(hSOCK);
      linkCode = LINK_OK;
    }
  }

  return linkCode;
}

BOOL CNet::SerchBegin(const char* cszIP)
{
  SOCK_ADDRESS addr = {0};
  addr.Port = MULTYGROUP_PORT;
  HANDLE hSOCK = NULL;
  unsigned long l = DotIpToLong(cszIP);
  {
    m_syn.Lock();
    SOCK_MAP::iterator it = m_map.find(l);

    if (it != m_map.end()) {
      hSOCK = it->second->GetHandle();
    }

    m_syn.Unlock();

    if (!hSOCK) {
      hSOCK = CPM_CreateSock(NET_SOCK_DGRAM);

      if (hSOCK) {
        if (CPM_GetLocalIP(hSOCK, &addr) && CPM_Bind(hSOCK, addr)) {
          m_syn.Lock();
          m_map.insert(std::pair<unsigned long, CSock*>(l, new CSock(hSOCK, hSOCK, cszIP, 0, false)));
          m_syn.Unlock();
          CPM_Recv(hSOCK);
        }
        else {
          CPM_ReleaseSock(hSOCK);
          hSOCK = NULL;
        }
      }
    }
  }

  if (hSOCK) {
    memset(addr.IP_Address, 0x0, sizeof(addr.IP_Address));
    strncpy(addr.IP_Address, cszIP, sizeof(addr.IP_Address) - 1);
    int szData[ ALIGN(OFFSET_SIZE + 2, 4) >> 2 ] = {0};
    char* tmp = (char*)szData;
    tmp[OFFSET_SIZE] = SYN_REQUES_CONNECT;
    int nLen = 2;
    return CPM_SendTo(hSOCK, addr, tmp + OFFSET_SIZE, &nLen);
  }

  return false;
}

BOOL CNet::SerchEnd(const char* cszIP)
{
  unsigned long l = DotIpToLong(cszIP);
  HANDLE hSOCK = NULL;
  {
    AoutLock lock(&m_syn);
    SOCK_MAP::iterator it = m_map.find(l);

    if (it != m_map.end()) {
      hSOCK = it->second->GetHandle();
      delete it->second;
      m_map.erase(it);
    }
  }

  if (hSOCK) {
    CPM_ReleaseSock(hSOCK);
  }

  return true;
}

HANDLE CNet::CreateVideoHandle()
{
  return CPM_CreateSock(NET_SOCK_DGRAM);
}

void CNet::ReleaseVideoHanle(HANDLE hSOCK)
{
  CPM_ReleaseSock(hSOCK);
}

BOOL CNet::GetVideoHandleAddr(HANDLE hVideoHanle, SOCK_ADDRESS& Addr)
{
  SOCK_ADDRESS tmp = {0};

  if (!CPM_GetLocalIP(hVideoHanle, &tmp)) {
    return false;
  }

  strcpy(Addr.IP_Address, tmp.IP_Address);

  if (!CPM_Bind(hVideoHanle, tmp)) {
    return false;
  }

  if (!CPM_GetSockName(hVideoHanle, &tmp)) {
    return false;
  }

  Addr.Port = tmp.Port;
  CPM_Recv(hVideoHanle);
  return true;
}

void CNet::TimeOut(int nDelay)
{
#ifdef _DEBUG
  return;
#endif
  AoutLock lock(&m_syn);
  SOCK_MAP::iterator it = m_map.begin();

  for (; it != m_map.end(); ++it) {
    if (!it->second->IsActive()) {
      assert(VSOCKET != it->second->GetHandle());
      CPM_ReleaseSock(it->second->GetHandle());
    }
  }
}