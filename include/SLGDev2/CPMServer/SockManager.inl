// lientManager.cpp: implementation of the SockManager class.
//
//////////////////////////////////////////////////////////////////////
#include "SockManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void CPMDEV_CALL RecvCB(SOCKET hSock, const char* buf, int len, void* param, void* user)
{
  SockManager* pManager = (SockManager*)param;
  pManager->RecvCB(hSock, buf, len, NULL, user);
}
void CPMDEV_CALL RecvFromCB(SOCKET hSock, const char* buf, int len, const addr_in* addr, void* param, void* user)
{
  SockManager* pManager = (SockManager*)param;
  pManager->RecvCB(hSock, buf, len, addr, user);
}
void CPMDEV_CALL ListenCB(SOCKET hSock, const addr_in* addr, void* param, void* user)
{
  SockManager* pManager = (SockManager*)param;
  pManager->ListenCB(hSock, addr, user);
}
SockManager::SockManager(int af) :
  m_af(af),
  m_pConnCB(NULL),
  m_pConnContext(NULL),
  m_pRecvCB(NULL),
  m_pRecvContext(NULL),
  m_pRecvFromCB(NULL),
  m_pRecvFromContext(NULL),
  m_bInit(FALSE),
  m_hListenSock(0),
  m_hMCastSock(0)
{
}
SockManager::~SockManager()
{
  UnInit();
}
BOOL SockManager::Init(unsigned int size, SERVER_MODEL model)
{
  if (!m_bInit) {
    m_bInit = CPM_InitSock((NET_MODEL)model, size) ;
    CPM_SetDataRecvCallBack(::RecvCB, this);
    CPM_SetDataRecvFromCallBack(::RecvFromCB, this);
  }
  return m_bInit;
}
void SockManager::UnInit()
{
  if (m_bInit) {
    m_bInit = FALSE;
    CPM_ReleaseSock(m_hListenSock, NULL);
    CPM_ReleaseSock(m_hMCastSock, NULL);
    CPM_StartWork(FALSE);
    CPM_UnInitSock();
    m_hListenSock = NULL;
    m_hMCastSock = NULL;
  }
}
void SockManager::SetConnectCallBack(SCNCallBack cb, void* param)
{
  if (!m_bInit) {
    return ;
  }
  m_pConnCB = cb;
  m_pConnContext = param;
}
void SockManager::SetRecvCallBack(SRecvCallBack cb, void* param)
{
  if (!m_bInit) {
    return ;
  }
  m_pRecvCB = cb;
  m_pRecvContext = param;
}
void SockManager::SetRecvFromCallBack(SRecvFromCallBack cb, void* param)
{
  if (!m_bInit) {
    return ;
  }
  m_pRecvFromCB = cb;
  m_pRecvFromContext = param;
}
BOOL SockManager::StartListen(BOOL flag)
{
  if (!m_bInit) {
    return FALSE;
  }
  BOOL ret = FALSE;
  do {
    if (flag) {
      addr_in Addr = {0};
      net_addr(ANYIP(m_af), SERVER_PORT, &Addr);
      m_hListenSock = CPM_CreateSock(m_af, SOCK_STREAM);
      int i = 0;
      while (i++ < BINDE_TIMES) {
        if (CPM_Bind(m_hListenSock, &Addr)) {
          ret = true;
          break;
        }
        SLEEP(MILLISEC * 1000 * 10);
      }
      if (!ret) {
        break;
      }
      CPM_SetListenCallBack(::ListenCB, this);
      CPM_Listen(m_hListenSock, NULL);
    }
    else {
      ret = true;
      CPM_ReleaseSock(m_hListenSock, NULL);
      m_hListenSock = NULL;
    }
  }
  while (0);
  return ret;
}
BOOL SockManager::StartWork(BOOL flag)
{
  return m_bInit ? CPM_StartWork(flag) : FALSE;
}
void SockManager::ListenCB(SOCKET hSock, const addr_in* pAddr, void* user)
{
  if (!m_bInit) {
    CPM_ReleaseSock(hSock, user);
    return ;
  }
  CPM_SetSendBufSize(hSock, SEND_BUF);
  CPM_SetRecvBufSize(hSock, RECV_BUF);
  int type = 0;
  if (m_pConnCB) {
    type = m_pConnCB(hSock, true, m_pConnContext);
  }
  CPM_Recv(hSock , type, this);
}
#define ALIGN(bytes, alignments) (((bytes)+((alignments)-1))&~((alignments)-1))
void SockManager::RecvCB(SOCKET hSock, const char* buf, int len, const addr_in* pAddr, void* user)
{
  if (!m_bInit) {
    return ;
  }
  if (0 == len && 0 == buf) {
    if (hSock != m_hListenSock && hSock != m_hMCastSock && m_pConnCB) {
      m_pConnCB(hSock, FALSE, m_pConnContext);
    }
  }
  else if (hSock == m_hMCastSock) {
    if (SYN_REQUES_CONNECT == buf[0]) {
      int szData[ ALIGN(OFFSET_SIZE + 2, 4) >> 2 ] = {0};
      char* tmp = (char*)szData;
      tmp[OFFSET_SIZE] = SYN_ACK_CONNECT;
      len = 2;
      CPM_SendTo(m_hMCastSock, pAddr, tmp + OFFSET_SIZE, &len);
    }
  }
  else {
    if (pAddr) {
      if (m_pRecvFromCB) {
        m_pRecvFromCB(hSock, buf, len, pAddr, m_pRecvFromContext);
      }
    }
    else {
      if (m_pRecvCB) {
        m_pRecvCB(hSock, buf, len, m_pRecvContext);
      }
    }
  }
}
BOOL SockManager::DisConnect(SOCKET hSock)
{
  if (!m_bInit) {
    return FALSE;
  }
  CPM_ReleaseSock(hSock, NULL);
  return true;
}
BOOL SockManager::Send(SOCKET hSock, const char* buf, unsigned int* lpLen, const addr_in* pAddr /*= NULL*/)
{
  if (NULL == pAddr) {
    return m_bInit ? CPM_Send(hSock, buf, (int*)lpLen) : FALSE;
  }
  else {
    return m_bInit ? CPM_SendTo(hSock, pAddr, buf, (int*)lpLen) : FALSE;
  }
  return FALSE;
}
BOOL SockManager::GetPeerAddr(SOCKET hSock, addr_in* pAddr)
{
  return m_bInit ? CPM_GetPeerAddr(hSock, pAddr) : FALSE;
}
BOOL SockManager::AddInIpGroup(BOOL flag, const addr_in* addr)
{
  if (!m_bInit) {
    return FALSE;
  }
  BOOL ret = FALSE;
  addr_in temp = *addr;
  do {
    if (flag) {
      m_hMCastSock = CPM_CreateSock(m_af, SOCK_DGRAM);
      if (0 == m_hMCastSock) {
        break;
      }
      ret = CPM_AddInIPGroup(m_hMCastSock, &temp);
      if (ret) {
        CPM_Recv(m_hMCastSock , 0, this);
      }
    }
    else {
      ret = true;
      CPM_DropOutIPGroup(m_hMCastSock, &temp);
      CPM_ReleaseSock(m_hMCastSock, NULL);
      m_hMCastSock = NULL;
    }
  }
  while (0);
  return ret;
}
SOCKET SockManager::CreateUDPSock(const addr_in* pAddr)
{
  SOCKET hSock = CPM_CreateSock(m_af, SOCK_DGRAM);// return;
  if (pAddr) {
    int i = 0;
    while (i++ < BINDE_TIMES) {
      if (CPM_Bind(hSock, pAddr)) {
        break;
      }
      SLEEP(MILLISEC * 1000 * 10);
    }
  }
  CPM_SetSendBufSize(hSock, SEND_BUF);
  CPM_SetRecvBufSize(hSock, RECV_BUF);
  CPM_Recv(hSock , 0, this);
  return hSock;
}

