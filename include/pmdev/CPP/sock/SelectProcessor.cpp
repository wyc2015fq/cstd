//#include "StdAfx.h"
#include "SelectProcessor.h"
#include "Socket.h"
#include "SockHandle.h"
#include "MemPool.h"

#include "NetManager.h"
extern CNetManager* gNetManager ;

typedef struct {
  int32            bListen;
  void*            lpContext;
} SELECT_DATA;

CSelectProcessor::CSelectProcessor(uint32 uMemPoolSize, uint32 uMemPoolUnit) :
  CNetProcessor(uMemPoolSize, uMemPoolUnit),
  m_hWorkTread(NULL),
  m_bWorkRuning(false)
{
}

CSelectProcessor::~CSelectProcessor()
{
  StartUpWork(false);
}

void CSelectProcessor::StartRecv(SOCKET sock)
{
  StartRecv(sock, false);
}

void CSelectProcessor::StartListen(SOCKET sock)
{
  StartRecv(sock, true);
}

void CSelectProcessor::StartRecv(SOCKET sock, BOOL bListen)
{
  AoutLock lock(&m_synSOCKMap);
  SOCKMap::iterator it =  m_SOCKMap.find(sock);

  if (it == m_SOCKMap.end()) {
    int8* pRetMem = CMemPool::GetInstance().GetMem(sizeof(SELECT_DATA), true);
    SELECT_DATA* pSelect = (SELECT_DATA*)pRetMem;
    memset(pSelect, 0x0, sizeof(SELECT_DATA));
    pSelect->lpContext = new CDataRecvProcess(sock, m_pThreadPool);
    pSelect->bListen  = bListen;
    m_SOCKMap.insert(std::pair<SOCKET, void*>(sock, pSelect));
  }
}

void CSelectProcessor::StopRecv(SOCKET sock)
{
  SELECT_DATA* pData = NULL;
  m_synSOCKMap.Lock();
  SOCKMap::iterator it = m_SOCKMap.find(sock);

  if (it != m_SOCKMap.end()) {
    pData = (SELECT_DATA*)(it->second);
    m_SOCKMap.erase(it);
  }

  m_synSOCKMap.Unlock();

  if (pData) {
    CDataRecvProcess* pObj = (CDataRecvProcess*)(pData->lpContext);
    pObj->Release();
    CMemPool::GetInstance().FreeMem((int8*)(pData));
    CNetProcessor::StopRecv(sock);
  }
}

void CSelectProcessor::StopListen(SOCKET sock)
{
  StopRecv(sock);
}

int32 CSelectProcessor::Send(SOCKET sock, const int8* pbData, int32 nLen, const SOCK_ADDRESS* pAddr)
{
  CDataRecvProcess* pObj = NULL;
  m_synSOCKMap.Lock();
  SOCKMap::iterator it = m_SOCKMap.find(sock);

  if (it != m_SOCKMap.end()) {
    SELECT_DATA* pData = (SELECT_DATA*)(it->second);

    if (pData) {
      pObj = (CDataRecvProcess*)pData->lpContext;

      if (pObj) {
        pObj->AddRef();
      }
    }
  }

  m_synSOCKMap.Unlock();

  if (pObj) {
    int32 rs = pObj->Send(pbData, nLen, pAddr);
    pObj->Release();
    return rs;
  }

  return 0;
}

BOOL CSelectProcessor::StartUpWork(BOOL flag)
{
  BOOL ret = false;

  if (flag) {
    if (!m_bWorkRuning) {
      m_bWorkRuning = true;
#ifdef WIN32
      m_hWorkTread = (HANDLE)_beginthread(CSelectProcessor::process, 0, this);
      ret = (m_hWorkTread != NULL);
#else
      ret = (pthread_create(&m_hWorkTread, NULL, CSelectProcessor::process, this) == 0);
#endif
    }
  }
  else {
    if (m_bWorkRuning) {
      m_bWorkRuning = false;
#ifdef WIN32

      if (WaitForSingleObject(m_hWorkTread, 1000 * TIME_OUT * 2) == WAIT_TIMEOUT) {
        TerminateThread(m_hWorkTread, 100);
      }

      m_hWorkTread = NULL;
      ret = true;
#else
      pthread_join(m_hWorkTread, NULL);
      m_hWorkTread = 0;
      ret = true;
#endif
    }
  }

  return ret;
}

T_VOID CSelectProcessor::process(void* param)
{
  assert(param != NULL);
  CSelectProcessor* pthis = (CSelectProcessor*)param;
  pthis->process();
#ifndef WIN32
  return pthis;
#endif
}

void CSelectProcessor::process()
{
  while (m_bWorkRuning) {
    SOCKET Array[FD_SETSIZE] = {0};
    int32 sType[FD_SETSIZE] = {1};
    int32 nSize = 0;
    {
      AoutLock lock(&m_synSOCKMap);
      SOCKMap::iterator it = m_SOCKMap.begin();

      for (; it != m_SOCKMap.end(); ++it) {
        Array[nSize] = it->first;
        sType[nSize] = ((SELECT_DATA*)(it->second))->bListen;
        nSize++;
      }
    }

    if (nSize <= 0) {
      SLEEP(MILLISEC * 300);
      continue;
    }

    READ_ARRAY_TIME_OUT(Array, nSize, 300)

    if (bTimeOut) {
      SLEEP(MILLISEC * 300);
      continue;
    }

    for (i = 0; i < nSize; ++i) {
      if (FD_ISSET(Array[i], &frds)) {
        sType[i] ? HandleListen(Array[i]) : HandleIO(Array[i]);
      }
    }
  }
}

void CSelectProcessor::HandleListen(SOCKET sock)
{
  SOCK_ADDRESS Addr = {0};
  SOCKET accepted = sock_accept(sock, Addr.IP_Address, Addr.Port);

  if (INVALID_SOCKET != accepted) {
    CNetProcessor::Accept(accepted, Addr);
  }
}

void CSelectProcessor::HandleIO(SOCKET sock)
{
  BOOL bClosed = false;
  m_synSOCKMap.Lock();
  SOCKMap::iterator it = m_SOCKMap.find(sock);

  if (it != m_SOCKMap.end()) {
    SELECT_DATA* pSelectData = (SELECT_DATA*)(it->second);
    CDataRecvProcess* pCurPrecessor = (CDataRecvProcess*)(pSelectData->lpContext);

    if (sock_type(sock) == SOCK_STREAM) {
      if (! pCurPrecessor->Recv()) {
        sock_close(sock);
        bClosed = true;
      }
      else {
        pCurPrecessor->CheckTimeOut(RECV_TIME_OUT);
      }
    }
    else if (sock_type(sock) == SOCK_DGRAM) {
      pCurPrecessor->RecvFrom();
      pCurPrecessor->CheckTimeOut(RECV_TIME_OUT);
    }
  }

  m_synSOCKMap.Unlock();

  if (bClosed) {
    StopRecv(sock);
  }
}
