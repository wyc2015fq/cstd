#ifndef __MY_SELECTNET_H__
#define __MY_SELECTNET_H__
#include "NetProcessor.h"
class CSelectProcessor : public CNetProcessor
{
    //  typedef std::map<SOCKET, SELECT_DATA*> DATAMap;
public:
    CSelectProcessor(uint32 uMemPoolSize, uint32 uMemPoolUnit);
    virtual ~CSelectProcessor();
public:
    virtual BOOL  StartUpWork(BOOL bStartUp);
    virtual BOOL  StartRecv(SOCKET sock , CSockBase* user);
    virtual BOOL  StartListen(SOCKET sock, CSockBase* user);
    virtual void  StopRecv(SOCKET sock, CSockBase* user);
    virtual void  StopListen(SOCKET sock, CSockBase* user);
    virtual int32 Send(SOCKET sock, const int8* pbData, int32 nLen, const addr_in* pAddr);
private:
    static unsigned long WINAPI process1(void* param);
    void process();
    void HandleIO(SOCKET sock, CSockBase* user);
    void HandleListen(SOCKET sock, CSockBase* user);
    BOOL StartRecv(SOCKET sock, BOOL bListen, CSockBase* user);
private:
    T_HANDLE      m_hWorkTread; //工作线程句柄
    int32         m_bWorkRuning;
private:
    //  DATAMap     m_DataMap;
    //  CriticalSection m_synDataMap;
};
#endif

