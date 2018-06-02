// CPMServer.cpp : Defines the entry point for the DLL application.
//
#include "CPMServer.h"
#include "SockManager.h"
SockManager* gServer = NULL;
BOOL CPM_S_Init(int af, unsigned int size, SERVER_MODEL model)
{
  if (gServer) {
    return true;
  }
  gServer = new SockManager(af);
  return (NULL == gServer) ? FALSE : gServer->Init(size, model);
}
void CPM_S_UnInit()
{
  if (gServer) {
    gServer->UnInit();
    delete gServer;
    gServer = NULL;
  }
}
void CPM_S_SetRecvCallBack(SRecvCallBack cb, void* param)
{
  if (gServer) {
    gServer->SetRecvCallBack(cb, param);
  }
}
void CPM_S_SetRecvFromCallBack(SRecvFromCallBack lpfnRecv, void* pvContext)
{
  if (gServer) {
    gServer->SetRecvFromCallBack(lpfnRecv, pvContext);
  }
}
void CPM_S_SetCNCallBack(SCNCallBack cb, void* param)
{
  if (gServer) {
    gServer->SetConnectCallBack(cb, param);
  }
}
BOOL CPM_S_AddInMultyGroup(BOOL flag, const addr_in* addr)
{
  return (NULL == gServer) ? FALSE : gServer->AddInIpGroup(flag, addr);
}
BOOL CPM_S_StartListen(BOOL flag)
{
  return (NULL == gServer) ? FALSE : gServer->StartListen(flag);
}
BOOL CPM_S_StartWork(BOOL flag)
{
  return (NULL == gServer) ? FALSE : gServer->StartWork(flag);
}
BOOL CPM_S_SendData(SOCKET hSock, const char* buf, unsigned int* lpLen)
{
  return (NULL == gServer) ? FALSE : gServer->Send(hSock, buf, lpLen);
}
BOOL CPM_S_DCN(SOCKET hSock)
{
  return (NULL == gServer) ? FALSE : gServer->DisConnect(hSock);
}
BOOL CPM_S_Address(SOCKET hSock, addr_in* addr)
{
  if (NULL == gServer) {
    return FALSE;
  }
  memset(&addr, 0x0, sizeof(addr_in));
  if (!(gServer->GetPeerAddr(hSock, addr))) {
    return FALSE;
  }
  return true;
}
SOCKET CPM_S_CreateUDPSock(const addr_in* pAddr)
{
  if (NULL == gServer) {
    return NULL;
  }
  return gServer->CreateUDPSock(pAddr);
}
BOOL CPM_S_SendDataTo(SOCKET hSock, const char* pbData, unsigned int* lpLen, const addr_in* Addr)
{
  if (NULL == gServer) {
    return FALSE;
  }
  return gServer->Send(hSock, pbData, lpLen, Addr);
}

