//#include "StdAfx.h"
#include "sock.h"
#include "NetManager.h"
#include "Socket.h"

CNetManager* gNetManager   = NULL;
volatile BOOL gInitialized = false;

#ifdef WIN32
BOOL APIENTRY DllMain1(HANDLE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
                      )
{
  switch (ul_reason_for_call) {
  case DLL_PROCESS_ATTACH:
  case DLL_THREAD_ATTACH:
  case DLL_THREAD_DETACH:
  case DLL_PROCESS_DETACH:
    break;
  }

  return TRUE;
}

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
    WSACleanup()  ;
  }
}
#endif

#ifdef  WIN32
#define INITSOCK  (InitSocket())
#define UINITSOCK (UnInitSocket())
#else
#define INITSOCK  1
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
    gNetManager  = NULL;
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

HANDLE CPM_CreateSock(int nSOCKType)
{
  if (!gNetManager) {
    return false;
  }

  return (HANDLE)(gNetManager->CreateSocket(nSOCKType));
}

void CPM_ReleaseSock(HANDLE psock)
{
  if (gNetManager) {
    gNetManager->ReleaseSocket((SOCKET)psock);
  }
}

BOOL CPM_Bind(HANDLE hSOCK, const SOCK_ADDRESS& Addr)
{
  if (!gNetManager) {
    return false;
  }

  return gNetManager->Bind((SOCKET)hSOCK, Addr);
}

BOOL CPM_Connect(HANDLE hSOCK, const SOCK_ADDRESS& Addr)
{
  if (!gNetManager) {
    return false;
  }

  return gNetManager->Connect((SOCKET)hSOCK, Addr);
}


BOOL CPM_AddInIPGroup(HANDLE hSOCK, const SOCK_ADDRESS& GroupIPAddr)
{
  if (!gNetManager) {
    return false;
  }

  return gNetManager->JoinIPGroup((SOCKET)hSOCK, GroupIPAddr);
}

void CPM_DropOutIPGroup(HANDLE hSOCK, const SOCK_ADDRESS& GroupIPAddr)
{
  if (gNetManager) {
    gNetManager->DropIPGroup((SOCKET)hSOCK, GroupIPAddr);
  }
}

void CPM_Listen(HANDLE hSOCK)
{
  if (gNetManager) {
    gNetManager->Listen((SOCKET)hSOCK);
  }
}

void CPM_SetSendBufSize(HANDLE hSOCK, int nSize)
{
  if (gNetManager) {
    gNetManager->SetSendBuf((SOCKET)hSOCK, nSize);
  }
}

void CPM_SetRecvBufSize(HANDLE hSOCK, int nSize)
{
  if (gNetManager) {
    gNetManager->SetRecvBuf((SOCKET)hSOCK, nSize);
  }
}

BOOL CPM_Send(HANDLE hSOCK, const char* pbData, int* lpLen)
{
  if (!gNetManager || NULL == pbData || NULL == lpLen) {
    return false;
  }

  int32 nLen = gNetManager->Send((SOCKET)hSOCK, pbData, *lpLen);
  BOOL ret = (nLen == *lpLen);
  
  *lpLen = nLen;
  return ret;
}

BOOL CPM_SendTo(HANDLE hSOCK, const SOCK_ADDRESS& addr, const char* pbData, int* lpLen)
{
  if (!gNetManager || NULL == pbData || NULL == lpLen) {
    return false;
  }

  int32 nLen = gNetManager->SendTo((SOCKET)hSOCK, pbData, *lpLen, (SOCK_ADDRESS*)&addr);
  BOOL ret = (nLen == *lpLen);
  *lpLen = nLen;
  return ret;
}

void CPM_Recv(HANDLE psock)
{
  if (gNetManager) {
    gNetManager->Recv((SOCKET)psock);
  }
}

BOOL CPM_GetPeerAddr(HANDLE psock, PSOCK_ADDRESS pAddr)
{
  if (!gNetManager) {
    return false;
  }

  return gNetManager->GetPeerAddr((SOCKET)psock, pAddr);
}

BOOL CPM_GetLocalIP(HANDLE hSOCK, PSOCK_ADDRESS pAddr)
{
  if (!gNetManager) {
    return false;
  }

  return gNetManager->GetLocalIP((SOCKET)hSOCK, pAddr);
}

BOOL CPM_GetSockName(HANDLE hSOCK, PSOCK_ADDRESS pAddr)
{
  if (!gNetManager) {
    return false;
  }

  return gNetManager->GetSockName((SOCKET)hSOCK, pAddr);

}

int CPM_GetLastError()
{
  if (!gNetManager) {
    return false;
  }

  return sock_error();
}