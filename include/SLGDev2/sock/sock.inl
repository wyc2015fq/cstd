#include "NetManager.h"
#include "Socket.h"
CNetManager* gNetManager = NULL;
volatile BOOL gInitialized = false;
#ifdef WIN32
BOOL InitSocket()
{
  if (!gInitialized) {
    WORD wVersionRequested = MAKEWORD(2, 2);
    WSADATA WSAData = { 0 };

    if (WSAStartup(wVersionRequested, &WSAData)) {
      return false;
    }

    if (LOBYTE(WSAData.wVersion) != 2 || HIBYTE(WSAData.wVersion) != 2) {
      return false;
    }
  }

  return true ;
}
void UnInitSocket()
{
  if (gInitialized) {
    gInitialized = false;
    WSACleanup() ;
  }
}
#endif
#ifdef WIN32
#define INITSOCK (InitSocket())
#define UINITSOCK (UnInitSocket())
#else
#define INITSOCK 1
#define UINITSOCK 1
#endif
BOOL CPM_InitSock(NET_MODEL NetModel, unsigned int uMemPoolSize, unsigned int uMemPoolUnit)
{
  if (gInitialized) {
    return true;
  }

  if (!INITSOCK) {
    return false;
  }

  assert(NULL == gNetManager);
  gNetManager = new CNetManager();

  if (gNetManager) {
    if (uMemPoolSize <= 0) {
      uMemPoolSize = MEMRESERVER_SIZE;
    }

    if (uMemPoolUnit <= 0) {
      uMemPoolUnit = MEMRESERVER_UNIT;
    }

    gInitialized = gNetManager->Init(NetModel, uMemPoolSize, uMemPoolUnit);

    if (!gInitialized) {
      delete gNetManager;
      gNetManager = NULL;
    }
  }

  return gInitialized;
}
void CPM_UnInitSock()
{
  if (gInitialized) {
    delete gNetManager;
    gNetManager = NULL;
    gInitialized = false;
    UINITSOCK;
  }
}
void CPM_SetListenCallBack(ListenCallBack lpfnListen, void* pvContext)
{
  if (gNetManager) {
    gNetManager->RegListenCB(lpfnListen, pvContext);
  }
}
void CPM_SetDataRecvCallBack(DataRecvCallBack lpfnRecv, void* pvContext)
{
  if (gNetManager) {
    gNetManager->RegRecvCB(lpfnRecv, pvContext);
  }
}
void CPM_SetDataRecvFromCallBack(DataRecvFromCallBack lpfnRecv, void* pvContext)
{
  if (gNetManager) {
    gNetManager->RegRecvFromCB(lpfnRecv, pvContext);
  }
}
BOOL CPM_StartWork(BOOL bStartUp)
{
  if (!gNetManager) {
    return false;
  }

  return gNetManager->StartWork(bStartUp);
}
SOCKET CPM_CreateSock(int af, int nSOCKType)
{
  if (!gNetManager) {
    return false;
  }

  return (gNetManager->CreateSocket(af, nSOCKType));
}
void CPM_ReleaseSock(SOCKET psock)
{
  if (gNetManager) {
    gNetManager->ReleaseSocket(psock);
  }
}
BOOL CPM_Bind(SOCKET hSOCK, const SOCK_ADDRESS& Addr)
{
  if (!gNetManager) {
    return false;
  }

  return gNetManager->Bind(hSOCK, Addr);
}
BOOL CPM_Connect(SOCKET hSOCK, const SOCK_ADDRESS& Addr)
{
  if (!gNetManager) {
    return false;
  }

  return gNetManager->Connect(hSOCK, Addr);
}
BOOL CPM_AddInIPGroup(SOCKET hSOCK, int af, const SOCK_ADDRESS& GroupIPAddr)
{
  if (!gNetManager) {
    return false;
  }

  return gNetManager->JoinIPGroup(hSOCK, af, GroupIPAddr);
}
void CPM_DropOutIPGroup(SOCKET hSOCK, const SOCK_ADDRESS& GroupIPAddr)
{
  if (gNetManager) {
    gNetManager->DropIPGroup(hSOCK, GroupIPAddr);
  }
}
void CPM_Listen(SOCKET hSOCK)
{
  if (gNetManager) {
    gNetManager->Listen(hSOCK);
  }
}
void CPM_SetSendBufSize(SOCKET hSOCK, int nSize)
{
  if (gNetManager) {
    gNetManager->SetSendBuf(hSOCK, nSize);
  }
}
void CPM_SetRecvBufSize(SOCKET hSOCK, int nSize)
{
  if (gNetManager) {
    gNetManager->SetRecvBuf(hSOCK, nSize);
  }
}
BOOL CPM_Send(SOCKET hSOCK, const char* pbData, int* lpLen)
{
  if (!gNetManager || NULL == pbData || NULL == lpLen) {
    return false;
  }

  int32 nLen = gNetManager->Send(hSOCK, pbData, *lpLen);
  BOOL ret = (nLen == *lpLen);
  *lpLen = nLen;
  return ret;
}
BOOL CPM_SendTo(SOCKET hSOCK, const SOCK_ADDRESS& addr, const char* pbData, int* lpLen)
{
  if (!gNetManager || NULL == pbData || NULL == lpLen) {
    return false;
  }

  int32 nLen = gNetManager->SendTo(hSOCK, pbData, *lpLen, (SOCK_ADDRESS*)&addr);
  BOOL ret = (nLen == *lpLen);
  *lpLen = nLen;
  return ret;
}
void CPM_Recv(SOCKET hSOCK , int type)
{
  if (gNetManager && type >= 0) {
    gNetManager->Recv(hSOCK , type);
  }
}
BOOL CPM_GetPeerAddr(SOCKET hSOCK, PSOCK_ADDRESS pAddr)
{
  if (!gNetManager) {
    return false;
  }

  return gNetManager->GetPeerAddr(hSOCK, pAddr);
}
BOOL CPM_GetLocalIP(SOCKET hSOCK, PSOCK_ADDRESS pAddr)
{
  if (!gNetManager) {
    return false;
  }

  return gNetManager->GetLocalIP(hSOCK, pAddr);
}
BOOL CPM_GetSockName(SOCKET hSOCK, PSOCK_ADDRESS pAddr)
{
  if (!gNetManager) {
    return false;
  }

  return gNetManager->GetSockName(hSOCK, pAddr);
}
int CPM_GetLastError()
{
  if (!gNetManager) {
    return false;
  }

  return sock_error();
}

