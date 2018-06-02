#include "StdAfx.h"
#include "OverLappedSocket.h"

#ifdef WIN32
COverLappedSocket::COverLappedSocket() 
{
	memset( &m_RecvIOOper, 0x0, sizeof(m_RecvIOOper) );
	m_RecvIOOper.mOperBuf = (char*)VirtualAlloc( NULL, DATA_BUF, MEM_COMMIT, PAGE_READWRITE );
	memset( m_RecvIOOper.mOperBuf, 0x0,  DATA_BUF );
}

COverLappedSocket::~COverLappedSocket()
{
	if ( m_RecvIOOper.mOperBuf ) VirtualFree(m_RecvIOOper.mOperBuf, DATA_BUF, MEM_DECOMMIT | MEM_RELEASE );
}


bool COverLappedSocket::Recv()
{
	if ( INVALID_SOCKET == m_hSocket  ) return false;
	m_RecvIOOper.WsaBuf.buf = m_RecvIOOper.mOperBuf + m_RecvIOOper.nOperBytes;
	m_RecvIOOper.WsaBuf.len = DATA_BUF - m_RecvIOOper.nOperBytes ;
	m_RecvIOOper.flag = FD_READ;
	DWORD flags = 0; 
	DWORD nRecvBytes = DATA_BUF - m_RecvIOOper.nOperBytes;
	int ret = ::WSARecv(m_hSocket, &(m_RecvIOOper.WsaBuf), 1, (LPDWORD)&nRecvBytes, &flags, &(m_RecvIOOper.OverLapped), NULL);
	int error = CCPMSocket::GetLastError();
	return (SOCKET_ERROR == ret && error != WSA_IO_PENDING) ? false : true;
}

void COverLappedSocket::HandleReadIO( int nRecvBytes )
{
	m_RecvIOOper.nOperBytes += nRecvBytes;
}

int COverLappedSocket::send(const char* buf, int len)
{
	return CSelectSocket::send(buf, len);
}

bool COverLappedSocket::recv( SOCK_DATA_NODE** pNode )
{
	*pNode = NULL;
	if ( m_RecvIOOper.nOperBytes  <= sizeof( PACKGE_HEADR) ) return false;
	PACKGE_HEADR header = {0,0};
	memcpy( &header, m_RecvIOOper.mOperBuf, sizeof(PACKGE_HEADR) );
	if ( 0 != CCPMSocket::checksum( (unsigned short*)&header, sizeof(PACKGE_HEADR)) )
	{
		memset( m_RecvIOOper.mOperBuf, 0x0, DATA_BUF );
		m_RecvIOOper.nOperBytes = 0;
		return false;
	}
	if ( m_RecvIOOper.nOperBytes < header.len+sizeof(PACKGE_HEADR) ) return false;
	*pNode = m_Handle.Recv( m_RecvIOOper.mOperBuf+sizeof(PACKGE_HEADR), header.len );
	m_RecvIOOper.nOperBytes -= header.len+sizeof(PACKGE_HEADR);
	memcpy( m_RecvIOOper.mOperBuf, m_RecvIOOper.mOperBuf+header.len+sizeof(PACKGE_HEADR), m_RecvIOOper.nOperBytes );
	return true;
}

#endif
