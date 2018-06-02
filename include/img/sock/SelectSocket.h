
#ifndef __MY_SELECT_SOCKET
#define __MY_SELECT_SOCKET

//#include "Socket.h"
#include "SockHandle.h"
class CSelectSocket : public CCPMSocket
{
public:
	CSelectSocket();
	virtual ~CSelectSocket();
public:
	bool Recv();
public:
	virtual int send(const char* buf, int len);
	virtual bool recv( SOCK_DATA_NODE** pNode );

public:
    virtual bool SetSendTimeOut( int s_time );
    virtual bool SetRecvTimeOut( int r_time );
	virtual bool SetSendBufSize( int size );
    virtual bool SetRecvBufSize( int size );

	virtual SOCKET accept( OUT PSOCKADDR lpSockAddr = NULL );
	virtual bool connect( IN PSOCKADDR addr );
	virtual bool send ( IN const char* buf, IN OUT int* lpLen, int flag = 0);
	virtual bool recv( IN char* buf, IN int* lpLen, int flag = 0 );
	virtual bool recvfrom(PSOCKADDR peer_addr, char* buf, int* len, int flag = 0);
	virtual bool sendto(  PSOCKADDR peer_addr, char* buf, int* len, int flag = 0);

protected:
	CSockHandle m_Handle;
private:
	bool Send( IN const char* buf, IN OUT int* lpLen, int flag );
private:
    long  m_lRecvTimeout;
    long  m_lSendTimeout;
private:
	char*  m_buf;
	int    m_len;
};

#endif
