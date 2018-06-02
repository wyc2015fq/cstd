#ifndef __MY_SELECTNET_H__
#define __MY_SELECTNET_H__

#include "NetProcessor.h"

class CSelectProcessor : public CNetProcessor
{
	//	typedef std::map<SOCKET, SELECT_DATA*> DATAMap;
public:
	CSelectProcessor( uint32 uMemPoolSize, uint32 uMemPoolUnit );
	virtual ~CSelectProcessor();
public:
	virtual bool  StartUpWork( bool bStartUp );
	virtual void  StartRecv( SOCKET sock );
	virtual void  StartListen( SOCKET sock );
    virtual void  StopRecv( SOCKET sock );
	virtual void  StopListen( SOCKET sock );
	virtual int32 Send( SOCKET sock, const int8* pbData, int32 nLen, const SOCK_ADDRESS* pAddr = NULL);
private:
	static T_VOID process( void* param );
	void process();
	void HandleIO( SOCKET sock );
	void HandleListen(SOCKET sock );
	void StartRecv( SOCKET sock, bool bListen );
private:
	T_HANDLE      m_hWorkTread; //工作线程句柄
	int32         m_bWorkRuning;
private:
	//	DATAMap		  m_DataMap;
	//  CriticalSection m_synDataMap;
};

#endif
