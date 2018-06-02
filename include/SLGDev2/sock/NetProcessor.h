#ifndef __MY_CNET_H__
#define __MY_CNET_H__
#include "common.h"
class CDataProcess;
class CThreadPool;
class CNetProcessor
{
public:
  CNetProcessor(uint32 uMemPoolSize, uint32 uMemPoolUnit);
  virtual ~CNetProcessor();
public:
  virtual BOOL  StartUpWork(BOOL bStartUp)        = 0;
  virtual void  StartRecv(SOCKET sock , int type, void* user) = 0;
  virtual void  StartListen(SOCKET sock, void* user)          = 0;
  virtual int32 Send(SOCKET sock, const int8* pbData, int32 nLen, const addr_in* pAddr) = 0;
  virtual void  StopListen(SOCKET sock, void* user) ;
  virtual void  StopRecv(SOCKET sock, void* user) ;
protected:
  void Accept(SOCKET sock, const addr_in* Addr, void* user);
protected:
  CThreadPool* m_pThreadPool;
protected:
  typedef std::map<SOCKET, void*> SOCKMap;
  SOCKMap     m_SOCKMap;
  CriticalSection m_synSOCKMap;
};
#endif

