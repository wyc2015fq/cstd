
// lientManager.h: interface for the SockManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIENTMANAGER_H__21F1FDBC_731C_4945_BA15_B71BDCB0D8BF__INCLUDED_)
#define AFX_LIENTMANAGER_H__21F1FDBC_731C_4945_BA15_B71BDCB0D8BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <string>
#include "SLGServer.h"
#include <assert.h>
#include <memory.h>
#include <stdio.h>
#ifdef WIN32
#define SLEEP(Milliseconds) {Sleep(Milliseconds);}
#define MILLISEC       1
#else
#include <sys/socket.h>
#include <unistd.h>
#define SLEEP(Milliseconds)  {struct timeval tval;tval.tv_sec= 0;tval.tv_usec=(Milliseconds); select( 0 , NULL , NULL , NULL , &tval ); }
#define MILLISEC       1000
#endif

using namespace std;

#define SERVER_PORT 13333

#define BINDE_TIMES 20
#define SEND_TIMEOUT 1


#define SEND_BUF (64*1024)
#define RECV_BUF (64*1024)

class SockManager
{
public:
    SockManager(int af);
    virtual ~SockManager();
public:
    BOOL Init(unsigned int size, SERVER_MODEL model);
    void UnInit();
    void SetConnectCallBack(SCNCallBack cb, void* param);
    void SetRecvCallBack(SRecvCallBack cb, void* param);
    void SetRecvFromCallBack(SRecvFromCallBack cb, void* param);
    BOOL StartListen(BOOL flag);
    BOOL StartWork(BOOL flag);
    BOOL Send(SOCKET hSock, const char* buf, unsigned int* lpLen, const addr_in* pAddr = NULL);
    BOOL GetPeerAddr(SOCKET hSOCK, addr_in* pAddr);
    BOOL DisConnect(SOCKET hSOCK);
    BOOL AddInIpGroup(BOOL flag, const addr_in* addr, CSockBase* user);
    SOCKET  CreateUDPSock(const addr_in* pAddr, CSockBase* user);
public:
    void RecvCB(SOCKET hSock, const char* buf, int len, const addr_in* pAddr);
    void ListenCB(SOCKET hSock, const addr_in* pAddr, CSockBase* user);
private:
    SCNCallBack     m_pConnCB;
    void*               m_pConnContext;
    SRecvCallBack       m_pRecvCB;
    void*       m_pRecvContext;
    SRecvFromCallBack   m_pRecvFromCB;
    void*             m_pRecvFromContext;
private:
    int m_af;
    BOOL                m_bInit;
private:
    SOCKET m_hListenSock;
    SOCKET m_hMCastSock;
};

#endif // !defined(AFX_LIENTMANAGER_H__21F1FDBC_731C_4945_BA15_B71BDCB0D8BF__INCLUDED_)