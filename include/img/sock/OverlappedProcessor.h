#ifndef __OVERLAPPEDNET_H__
#define __OVERLAPPEDNET_H__

#include "NetProcessor.h"

#ifdef WIN32

#define MAX_TREADS 8
class COverlappedProcessor : public CNetProcessor
{
public:
	COverlappedProcessor( uint32 uMemPoolSize, uint32 uMemPoolUnit );
	virtual ~COverlappedProcessor();
public:
	virtual bool  StartUpWork( bool bStartUp );
	virtual void  StartRecv( SOCKET sock );
	virtual void  StartListen( SOCKET sock );
	virtual void  StopRecv( SOCKET sock );	
	virtual void  StopListen( SOCKET sock );
	virtual int32 Send( SOCKET sock, const int8* pbData, int32 nLen, const SOCK_ADDRESS* pAddr = NULL);
private:
	static void process( void* param );
	void process();
	void HandleIO(SOCKET sock, WSAOVERLAPPED* pIOOper , DWORD dwTrans, int nError);
	void HandleListen(SOCKET sock, WSAOVERLAPPED* pIOOper, DWORD dwTrans, int nError);
	void Recv( SOCKET sock, WSAOVERLAPPED* pIOOper );
	void RecvFrom( SOCKET sock, WSAOVERLAPPED* pIOOper );
	bool ErrDeal( SOCKET sock, WSAOVERLAPPED* pIOOper, DWORD dwTrans, BOOL bOK );
	void StartRecv( SOCKET sock, bool bListen );
private:
	T_HANDLE m_hCompletionPort;
	T_HANDLE m_hIOCPThread;
	T_HANDLE m_hWorkThread[MAX_TREADS];
	int32    m_nTreadCount;
	int32    m_bWorking;
};

#endif //WIN32

#endif