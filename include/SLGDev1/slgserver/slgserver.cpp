
// SLGServer.cpp : Defines the entry point for the DLL application.
//
#include "SLGServer.h"
#include "SockManager.h"
SockManager* gServer = NULL;
BOOL SLG_S_Init(int af, unsigned int size, SERVER_MODEL model)
{
    if (gServer)
    {
        return true;
    }

    gServer = new SockManager(af);
    return (NULL == gServer) ? FALSE : gServer->Init(size, model);
}

void SLG_S_UnInit()
{
    if (gServer)
    {
        gServer->UnInit();
        delete gServer;
        gServer = NULL;
    }

}

void SLG_S_SetRecvCallBack(SRecvCallBack cb, void* param)
{
    if (gServer)
    {
        gServer->SetRecvCallBack(cb, param);
    }

}

void SLG_S_SetRecvFromCallBack(SRecvFromCallBack lpfnRecv, void* pvContext)
{
    if (gServer)
    {
        gServer->SetRecvFromCallBack(lpfnRecv, pvContext);
    }

}

void SLG_S_SetCNCallBack(SCNCallBack cb, void* param)
{
    if (gServer)
    {
        gServer->SetConnectCallBack(cb, param);
    }

}

BOOL SLG_S_AddInMultyGroup(BOOL flag, const addr_in* addr, CSockBase* user)
{
    return (NULL == gServer) ? FALSE : gServer->AddInIpGroup(flag, addr, user);
}

BOOL SLG_S_StartListen(BOOL flag)
{
    return (NULL == gServer) ? FALSE : gServer->StartListen(flag);
}

BOOL SLG_S_StartWork(BOOL flag)
{
    return (NULL == gServer) ? FALSE : gServer->StartWork(flag);
}

BOOL SLG_S_SendData(SOCKET hSock, const char* buf, unsigned int* lpLen)
{
    return (NULL == gServer) ? FALSE : gServer->Send(hSock, buf, lpLen);
}

BOOL SLG_S_DCN(SOCKET hSock)
{
    return (NULL == gServer) ? FALSE : gServer->DisConnect(hSock);
}

BOOL SLG_S_Address(SOCKET hSock, addr_in* addr)
{
    if (NULL == gServer)
    {
        return FALSE;
    }

    memset(&addr, 0x0, sizeof(addr_in));

    if (!(gServer->GetPeerAddr(hSock, addr)))
    {
        return FALSE;
    }

    return true;
}

SOCKET SLG_S_CreateUDPSock(const addr_in* pAddr, CSockBase* user)
{
    if (NULL == gServer)
    {
        return 0;
    }

    return gServer->CreateUDPSock(pAddr, user);
}

BOOL SLG_S_SendDataTo(SOCKET hSock, const char* pbData, unsigned int* lpLen, const addr_in* Addr)
{
    if (NULL == gServer)
    {
        return FALSE;
    }

    return gServer->Send(hSock, pbData, lpLen, Addr);
}
