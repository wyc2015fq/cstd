#include "StdAfx.h"
#include "SelectSocket.h"


CSelectSocket::CSelectSocket() :
m_Handle(*this),
m_lRecvTimeout(TIME_OUT),
m_lSendTimeout(TIME_OUT),
m_buf(NULL),
m_len(0)
{
	m_buf = new char[DATA_BUF];
	memset( m_buf, 0x0, DATA_BUF );
}

CSelectSocket::~CSelectSocket()
{
	delete [] m_buf;
	m_buf = NULL;
}

bool CSelectSocket::SetRecvTimeOut( int r_time )
{
    m_lRecvTimeout = r_time;
	return true;
}

bool CSelectSocket::SetSendTimeOut( int s_time )
{
    m_lSendTimeout = s_time;
	return true;
}

bool CSelectSocket::SetSendBufSize( int size )
{
	size = (size > (sizeof(SOCK_DATA_HEADER) + DATAPACK_SIZE) ) ? (sizeof(SOCK_DATA_HEADER) + DATAPACK_SIZE) : size;
	return CCPMSocket::SetSendBufSize(size);
}

bool CSelectSocket::SetRecvBufSize( int size )
{
	size = (size > (sizeof(SOCK_DATA_HEADER) + DATAPACK_SIZE) ) ? (sizeof(SOCK_DATA_HEADER) + DATAPACK_SIZE) : size;
	return CCPMSocket::SetRecvBufSize(size);
}

SOCKET CSelectSocket::accept(  PSOCKADDR lpSockAddr /*= NULL*/ )
{
   fd_set frds;
   FD_ZERO(&frds);
   FD_SET(m_hSocket, &frds);
   timeval rtval;
   rtval.tv_sec = m_lRecvTimeout;
   rtval.tv_usec = 0;
   if ( ::select(m_hSocket+1, &frds, NULL, NULL, &rtval) <= 0 ) return INVALID_SOCKET;
   return CCPMSocket::accept(lpSockAddr) ;
}

bool CSelectSocket::connect( IN PSOCKADDR addr )
{
	unsigned long argp = 1;  
	ioctlsocket(m_hSocket, FIONBIO , &argp );
	fd_set fwds;
	FD_ZERO(&fwds);
	FD_SET(m_hSocket, &fwds);
	timeval rtval;
	rtval.tv_sec  = 3;
	rtval.tv_usec = 0;
	CCPMSocket::connect(addr );
	return (::select(m_hSocket+1, NULL, &fwds, NULL, &rtval) <= 0 ) ? false : true;
}


bool CSelectSocket::recv(  char* buf,  int* lpLen, int flag )
{
    fd_set frds;
    FD_ZERO(&frds);
    FD_SET(m_hSocket, &frds);
    timeval rtval;
    rtval.tv_sec = m_lRecvTimeout;
    rtval.tv_usec = 0;
    if ( ::select(m_hSocket+1, &frds, NULL, NULL, &rtval) <= 0 ) return false;
    return CCPMSocket::recv(buf, lpLen, flag);
}

bool CSelectSocket::recvfrom(PSOCKADDR peer_addr, char* buf, int* len, int flag)
{
    fd_set frds;
    FD_ZERO(&frds);
    FD_SET(m_hSocket, &frds);
    timeval rtval;
    rtval.tv_sec = m_lRecvTimeout;
    rtval.tv_usec = 0;
    if ( ::select(m_hSocket+1, &frds, NULL, NULL, &rtval) <= 0 ) return false;
    return CCPMSocket::recvfrom(peer_addr, buf, len, flag);
}

bool CSelectSocket::sendto(  PSOCKADDR peer_addr, char* buf, int* len, int flag)
{
    fd_set fwds;
    FD_ZERO(&fwds);
    FD_SET(m_hSocket, &fwds);
    timeval wtval;
    wtval.tv_sec = m_lSendTimeout;
    wtval.tv_usec = 0;
    if ( ::select(m_hSocket+1, NULL, &fwds, NULL, &wtval) <= 0 ) return false;
	return CCPMSocket::sendto(peer_addr, buf, len, flag);
}

bool CSelectSocket::send( IN const char* buf, IN OUT int* lpLen, int flag )
{
	PACKGE_HEADR header;
	memset( &header, 0x0, sizeof(header) );
	*lpLen = ( *lpLen > DATA_BUF) ? DATA_BUF : *lpLen;
	header.len = *lpLen;
	header.fcs = 0;
	header.fcs = CCPMSocket::checksum( (unsigned short*)&header, sizeof(header) );
	
	char tmp[DATA_BUF+sizeof(PACKGE_HEADR)] = {0};
	memcpy( tmp, &header, sizeof(header) );
	memcpy( tmp+sizeof(header), buf, *lpLen );
	
	int nSendBytes = *lpLen + sizeof(header);
	int nTotalSends = 0;
	bool ret = false;
	int send_counts = 0;
	
	while ( send_counts++ < MAX_SEND_COUNTS )
	{
		if ( Send( tmp+nTotalSends, &nSendBytes, flag ) )
		{
			nTotalSends += nSendBytes;
			if ( nTotalSends < (*lpLen + sizeof(header)) )
			{
				nSendBytes = (*lpLen + sizeof(header)) - nTotalSends;
			}
			else
			{
				ret = true;
				break;
			}
			
		}
		else
		{
			if ( WSAECONNABORTED==CCPMSocket::GetLastError() || WSAECONNRESET==CCPMSocket::GetLastError() || *lpLen <= 0 )
			{
				ret = false;
				break;
			}
		}
	}
	*lpLen = ret ? *lpLen : -1;
	return ret;
}

bool CSelectSocket::Send( const char* buf, int* lpLen, int flag )
{
	fd_set fwds;
	FD_ZERO(&fwds);
	FD_SET(m_hSocket, &fwds);
	timeval wtval;
	wtval.tv_sec = m_lSendTimeout;
	wtval.tv_usec = 0;
	if ( ::select(m_hSocket+1, NULL, &fwds, NULL, &wtval) <= 0 ) return false;
	return CCPMSocket::send(buf, lpLen, flag);
}

bool CSelectSocket::Recv()
{	
	bool ret = true;
	int recv_len = DATA_BUF - m_len;
	if ( recv(m_buf+m_len, &recv_len ) && recv_len > 0 )
	{
		m_len += recv_len;		
	}
	else
	{
		if ( (WSAECONNRESET == CCPMSocket::GetLastError()) || (recv_len <= 0) ) //??
		{	
			ret = false;
		}
	}
	return ret;
}

int CSelectSocket::send(const char* buf, int len)
{
	return m_Handle.Send(buf, (unsigned int*)&len) ? len : 0;
}

bool CSelectSocket::recv( SOCK_DATA_NODE** pNode )
{	
	*pNode = NULL;
	if ( m_len  <= sizeof( PACKGE_HEADR) ) return false;
	PACKGE_HEADR header = {0,0};
	memcpy( &header, m_buf, sizeof(PACKGE_HEADR) );

	if ( 0 != CCPMSocket::checksum( (unsigned short*)&header, sizeof(PACKGE_HEADR)) )
	{
		memset( m_buf, 0x0, DATA_BUF );
		m_len = 0;
		return false;
	}
	if ( m_len < header.len+sizeof(PACKGE_HEADR) ) return false;
	*pNode = m_Handle.Recv( m_buf+sizeof(PACKGE_HEADR), header.len );
	m_len -= header.len+sizeof(PACKGE_HEADR);
	memcpy( m_buf, m_buf+header.len+sizeof(PACKGE_HEADR), m_len );
	return true;	
}