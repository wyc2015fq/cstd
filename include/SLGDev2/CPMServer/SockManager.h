// lientManager.h: interface for the SockManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIENTMANAGER_H__21F1FDBC_731C_4945_BA15_B71BDCB0D8BF__INCLUDED_)
#define AFX_LIENTMANAGER_H__21F1FDBC_731C_4945_BA15_B71BDCB0D8BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "common.h"

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
  BOOL AddInIpGroup(BOOL flag, const addr_in* addr);
  SOCKET  CreateUDPSock(const addr_in* pAddr);
public:
  void RecvCB(SOCKET hSock, const char* buf, int len, const addr_in* pAddr, void* user);
  void ListenCB(SOCKET hSock, const addr_in* pAddr, void* user);
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
