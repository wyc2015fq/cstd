#ifndef __OVERLAPPEDNET_H__
#define __OVERLAPPEDNET_H__
#include "NetProcessor.h"
#ifdef WIN32
#define MAX_TREADS 8
class COverlappedProcessor : public CNetProcessor
{
public:
  COverlappedProcessor(uint32 uMemPoolSize, uint32 uMemPoolUnit);
  virtual ~COverlappedProcessor();
public:
  virtual BOOL  StartUpWork(BOOL bStartUp);
  virtual void  StartRecv(SOCKET sock , int type, void* user);
  virtual void  StartListen(SOCKET sock, void* user);
  virtual void  StopRecv(SOCKET sock, void* user);
  virtual void  StopListen(SOCKET sock, void* user);
  virtual int32 Send(SOCKET sock, const int8* pbData, int32 nLen, const addr_in* pAddr);
private:
  static void __cdecl process(void* param);
  void process();
  void HandleIO(SOCKET sock, WSAOVERLAPPED* pIOOper , DWORD dwTrans, int nError, void* user);
  void HandleListen(SOCKET sock, WSAOVERLAPPED* pIOOper, DWORD dwTrans, int nError, void* user);
  void Recv(SOCKET sock, WSAOVERLAPPED* pIOOper, void* user);
  void RecvFrom(SOCKET sock, WSAOVERLAPPED* pIOOper, void* user);
  BOOL ErrDeal(SOCKET sock, WSAOVERLAPPED* pIOOper, DWORD dwTrans, BOOL bOK, void* user);
  void StartRecv(SOCKET sock , int type , BOOL bListen, void* user);
private:
  T_HANDLE m_hCompletionPort;
  T_HANDLE m_hIOCPThread;
  T_HANDLE m_hWorkThread[MAX_TREADS];
  int32    m_nTreadCount;
  int32    m_bWorking;
};
#endif //WIN32
#endif

