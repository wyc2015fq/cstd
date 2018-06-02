#include "StdAfx.h"

#include "Net.h"
#include "NetManager.h"
#include "socket.h"

CThread::CThread() :
m_hTread(0),
m_bRunning(false)
{
}

CThread::~CThread()
{
	StopThread();
}

void CThread::StartThread()
{
	if ( !m_bRunning )
	{
		m_bRunning = true;
#ifdef WIN32	
		m_hTread = (HANDLE)_beginthread(CThread::process, 0, this);
#else
		pthread_create(&m_hTread, NULL, CThread::process, this) ;
#endif
	}
}

void CThread::StopThread()
{
	if ( m_bRunning )
	{
		m_bRunning = false;
#if 0
#ifdef WIN32
		if ( WaitForSingleObject(m_hTread, 1000*TIME_OUT) == STATUS_TIMEOUT )
		{
			TerminateThread(m_hTread, 0);
		}
		m_hTread = NULL;
#else
		pthread_join(m_hTread, NULL);
#endif
#endif
		m_hTread = 0;
	}
}

T_VOID CThread::process(void* param)
{
	CThread* pthread = (CThread*)param;
	if ( pthread ) 
	{
		pthread->process();
	}
#ifndef WIN32
	return pthread;
#endif
}


CNet::CNet(CNetManager& mgr) :
m_Mgr(mgr)
{	
	m_map.clear();
}

CNet::~CNet()
{
	SOCK_MAP::iterator it = m_map.begin();
	for (; it != m_map.end(); ++it)
	{
		sock_close( it->first );
	}
	m_map.clear();
	StopThread();
}

void CNet::SerchMember( SOCKET sock, const SOCK_ADDRESS& GroupIPAddr )
{
	Insert(sock, eSERCHMEM) ;
	//这里模拟TCP的三次握手中的前两次
	//当对等方接收到此消息后立即发送SYN_ACK确认
	int32 len = sizeof(char);
	char flag = SYN_REQUES_CONNECT;
	sock_send( sock, (char*)&flag, len, GroupIPAddr.IP_Address, GroupIPAddr.Port);	
}

void CNet::StopSerch( SOCKET sock )
{
	Erase(sock);
}

void CNet::Listen( SOCKET sock, bool bStartUp )
{
	bStartUp ? Insert(sock, eLISTEN) : Erase(sock);
}

void CNet::JoinGroup( SOCKET sock, bool bJoin )
{
	bJoin ? Insert(sock, eRESPSERCH) : Erase(sock);
}

bool CNet::Insert( SOCKET sock, SOCK_CLASS sc )
{
	AoutLock lock( &m_syn );
	SOCK_MAP::iterator it = m_map.find( sock );
	if ( it == m_map.end() )
	{
		m_map.insert( std::pair<SOCKET,SOCK_CLASS>(sock, sc) );
		if ( !m_bRunning )
			StartThread();
		return true;
	}
	return false;
}

bool CNet::Erase( SOCKET sock )
{
	AoutLock lock( &m_syn );
	SOCK_MAP::iterator it = m_map.find( sock );
	if ( it != m_map.end() )
	{
		m_map.erase(it);
		if ( m_map.empty() )
			StopThread();
		return true;
	}
	return false;
}

void CNet::process()
{
	while ( m_bRunning )
	{
		m_syn.Lock();
		SOCKET MaxSock = 0;
		SOCKET Array[FD_SETSIZE] = {0};	
		SOCK_MAP::iterator it = m_map.begin();		
		int32 nSize = 0;
		for (; it != m_map.end(); ++it)
		{
			Array[nSize++] = it->first;
			MaxSock = it->first > MaxSock ? it->first : MaxSock;
		}
		if ( 0 == MaxSock )
		{
			SLEEP(MILLISEC*300);
			m_syn.Unlock();
			continue;
		}
		READ_ARRAY_TIME_OUT(Array, nSize, 300)
		if ( bTimeOut )
		{
			m_syn.Unlock();
			continue;	
		}
		m_syn.Unlock();

		i = 0;
		for (; i < nSize; ++i)
		{
			if ( FD_ISSET(Array[i], &frds) )
				HandleOper( Array[i] );
		}
	}
}

void CNet::HandleOper( SOCKET sock )
{
	SOCK_CLASS sc = eNONE;
	m_syn.Lock();
	do 
	{
		SOCK_MAP::iterator it = m_map.find( sock );
		if ( it == m_map.end() )
			break;
		sc = it->second;
	} while (0);
	m_syn.Unlock();

	switch ( sc )
	{
		case eLISTEN:
			ListenOper(sock);
			break;
		case eSERCHMEM:
		case eRESPSERCH:
			SerchOper(sock);
			break;
		default:
			break;
	}
}

void CNet::ListenOper( SOCKET sock )
{
	SOCK_ADDRESS PeerAddr = {0};
	SOCKET NewSock = sock_accept( sock, PeerAddr.IP_Address, PeerAddr.Port );
	if ( INVALID_SOCKET != NewSock )
		m_Mgr.OnListen( NewSock, PeerAddr );
}

void CNet::SerchOper( SOCKET sock )
{
	SOCK_ADDRESS PeerAddr = {0};
	int8 flag = 0;
	if ( sock_recv( sock, (char*)&flag, sizeof(char), PeerAddr.IP_Address, &(PeerAddr.Port) ) == sizeof(char)  )
	{
		if ( SYN_REQUES_CONNECT == flag  )
		{
			flag = SYN_ACK_CONNECT;
			sock_send( sock, (char*)&flag, sizeof(char), PeerAddr.IP_Address, PeerAddr.Port);
		}
		if ( SYN_ACK_CONNECT == flag  )
		{
			m_Mgr.OnScan( PeerAddr.IP_Address );
		}
	}
}
