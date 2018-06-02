#ifndef __MY_NET_H__
#define __MY_NET_H__

#include "common.h"
class CThread
{
public:
	CThread();
	virtual ~CThread();
protected:
	void StartThread();
	void StopThread();
protected:
	static T_VOID process(void* param);
	virtual void process() = 0 ;
protected:
	bool      m_bRunning;
	T_HANDLE    m_hTread;
};

class CNetManager;
class CNet : public CThread
{

public:
	CNet( CNetManager& mgr );
	~CNet();
public:
	void SerchMember( SOCKET sock, const SOCK_ADDRESS& GroupIPAddr );
	void StopSerch( SOCKET sock );
	void Listen( SOCKET sock, bool bStartUp );
	void JoinGroup( SOCKET sock, bool bJoin );
protected:
	virtual void process();
private:
	bool Insert( SOCKET sock,  SOCK_CLASS sc);
	bool Erase( SOCKET sock );
	void HandleOper( SOCKET sock );
	void ListenOper( SOCKET sock );
	void SerchOper( SOCKET sock );
private:
	typedef std::map<SOCKET, SOCK_CLASS> SOCK_MAP;
	SOCK_MAP m_map;
	CriticalSection m_syn;
private:
	CNetManager& m_Mgr;
};
#endif