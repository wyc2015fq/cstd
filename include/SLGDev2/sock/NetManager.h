#ifndef __MY_NETMANAGER_H__
#define __MY_NETMANAGER_H__
#include "common.h"
class CNetProcessor;
class CNetManager
{
  friend class CDataRecvJob;
  friend class CDataRecvProcess;
  friend class CNetProcessor;
public:
  CNetManager();
  ~CNetManager();
public:
  BOOL Init(NET_MODEL eModel, uint32 uMemPoolSize, uint32 uMemPoolUnit);
  void UnInit();
public:
  void RegListenCB(ListenCallBack lpfnCB, void* lpContext) {
    m_lpfnListen = lpfnCB;
    m_lpListenContext = lpContext;
  }
  void RegRecvCB(DataRecvCallBack lpfnCB, void* lpContext) {
    m_lpfnRecv = lpfnCB;
    m_lpRecvContext = lpContext;
  }
  void RegRecvFromCB(DataRecvFromCallBack lpfnCB, void* lpContext) {
    m_lpfnRecvFrom = lpfnCB;
    m_lpRecvFromContext = lpContext;
  }
public:
  SOCKET CreateSocket(int af, int32 nType);
  void   ReleaseSocket(SOCKET sock);
  BOOL   Connect(SOCKET sock, const SOCK_ADDRESS& PeerAddr);
  BOOL   Bind(SOCKET sock, const SOCK_ADDRESS& PeerAddr);
  void   Listen(SOCKET sock);
  BOOL   JoinIPGroup(SOCKET sock, int af, const SOCK_ADDRESS& GroupIPAddr);  //加入多播组
  void   DropIPGroup(SOCKET sock, const SOCK_ADDRESS& GroupIPAddr);  //退出多播组
  void   SetRecvBuf(SOCKET sock, int32 nSize);
  void   SetSendBuf(SOCKET sock, int32 nSize);
  BOOL   GetPeerAddr(SOCKET sock, PSOCK_ADDRESS lpPeerAddr);
  BOOL   GetLocalIP(SOCKET sock, PSOCK_ADDRESS lpLocalIP);
  BOOL   GetSockName(SOCKET sock, PSOCK_ADDRESS lpLocalIP);
  int32  Send(SOCKET sock, const int8* pbData, int32 nLen);
  int32  SendTo(SOCKET sock, const int8* pbData, int32 nLen, PSOCK_ADDRESS lpAddr);
  void   Recv(SOCKET sock , int type);
  BOOL   StartWork(BOOL bStartUp);
protected:
  void   OnScan(const int8* IP);
  void   OnListen(SOCKET sock, const SOCK_ADDRESS& Addr);
  void   OnRecv(SOCKET sock, const int8* pbData, int32 nLen, PSOCK_ADDRESS lpAddr = NULL);
protected:
  CNetProcessor*       m_pProc;
protected:
  ListenCallBack       m_lpfnListen;
  void*            m_lpListenContext;
  DataRecvCallBack     m_lpfnRecv;
  void*            m_lpRecvContext;
  DataRecvFromCallBack m_lpfnRecvFrom;
  void*            m_lpRecvFromContext;
};
#endif

