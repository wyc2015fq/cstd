#ifndef __MY_CNET_H__
#define __MY_CNET_H__
class CDataProcess;
class CThreadPool;

#include "../slgdev_private.h"
#include "sock.h"

class CNetProcessor
{
public:
    CNetProcessor(uint32 uMemPoolSize, uint32 uMemPoolUnit);
    virtual ~CNetProcessor();
public:
    virtual BOOL  StartUpWork(BOOL bStartUp)        = 0;
    virtual BOOL  StartRecv(SOCKET sock , CSockBase* user) = 0;
    virtual BOOL  StartListen(SOCKET sock, CSockBase* user)          = 0;
    virtual int32 Send(SOCKET sock, const int8* pbData, int32 nLen, const addr_in* pAddr) = 0;
    virtual void  StopListen(SOCKET sock, CSockBase* user) ;
    virtual void  StopRecv(SOCKET sock, CSockBase* user) ;
protected:
    void Accept(SOCKET sock, const addr_in* Addr, CSockBase* user);
protected:
    CThreadPool* m_pThreadPool;
protected:
    typedef std::map<SOCKET, void*> SOCKMap;
    SOCKMap     m_SOCKMap;
    CriticalSection m_synSOCKMap;
};
#endif

