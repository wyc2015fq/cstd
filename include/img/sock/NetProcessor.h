#ifndef __MY_CNET_H__
#define __MY_CNET_H__

#include "common.h"

class CDataProcess;
class CThreadPool;

class CNetProcessor 
{
public:
	CNetProcessor( uint32 uMemPoolSize, uint32 uMemPoolUnit );
	virtual ~CNetProcessor();
public:
	virtual bool  StartUpWork( bool bStartUp ) = 0;
	virtual void  StartRecv( SOCKET sock ) = 0;
	virtual void  StartListen( SOCKET sock ) = 0;
	virtual int32 Send( SOCKET sock, const int8* pbData, int32 nLen, const SOCK_ADDRESS* pAddr = NULL) = 0;
	virtual void  StopListen( SOCKET sock ) ;
	virtual void  StopRecv( SOCKET sock ) ;	
protected:
	void Accept( SOCKET sock, const SOCK_ADDRESS& Addr );
protected:
	CThreadPool* m_pThreadPool;
protected:
	typedef std::map<SOCKET, void*> SOCKMap;
	SOCKMap		    m_SOCKMap;
    CriticalSection m_synSOCKMap;
};

#endif
