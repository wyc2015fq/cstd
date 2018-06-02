#include "SelectSocket.h"
CSelectSocket::CSelectSocket() :
    m_Handle(*this),
    m_lRecvTimeout(TIME_OUT),
    m_lSendTimeout(TIME_OUT),
    m_buf(NULL),
    m_len(0)
{
    m_buf = new char[DATA_BUF];
    memset(m_buf, 0x0, DATA_BUF);
}

CSelectSocket::~CSelectSocket()
{
    delete [] m_buf;
    m_buf = NULL;
}

BOOL CSelectSocket::SetRecvTimeOut(int r_time)
{
    m_lRecvTimeout = r_time;
    return true;
}

BOOL CSelectSocket::SetSendTimeOut(int s_time)
{
    m_lSendTimeout = s_time;
    return true;
}

BOOL CSelectSocket::SetSendBufSize(int size)
{
    size = (size > (sizeof(SOCK_DATA_HEADER) + DATAPACK_SIZE)) ? (sizeof(SOCK_DATA_HEADER) + DATAPACK_SIZE) : size;
    return CSLGSocket::SetSendBufSize(size);
}

BOOL CSelectSocket::SetRecvBufSize(int size)
{
    size = (size > (sizeof(SOCK_DATA_HEADER) + DATAPACK_SIZE)) ? (sizeof(SOCK_DATA_HEADER) + DATAPACK_SIZE) : size;
    return CSLGSocket::SetRecvBufSize(size);
}

SOCKET CSelectSocket::accept(PSOCKADDR lpSockAddr /*= NULL*/)
{
    fd_set frds;
    FD_ZERO(&frds);
    FD_SET(m_hSocket, &frds);
    timeval rtval;
    rtval.tv_sec = m_lRecvTimeout;
    rtval.tv_usec = 0;

    if (::select(m_hSocket + 1, &frds, NULL, NULL, &rtval) <= 0)
    {
        return INVALID_SOCKET;
    }

    return CSLGSocket::accept(lpSockAddr) ;
}

BOOL CSelectSocket::connect(IN PSOCKADDR addr)
{
    unsigned int argp = 1;
    ioctlsocket(m_hSocket, FIONBIO , &argp);
    fd_set fwds;
    FD_ZERO(&fwds);
    FD_SET(m_hSocket, &fwds);
    timeval rtval;
    rtval.tv_sec = 3;
    rtval.tv_usec = 0;
    CSLGSocket::connect(addr);
    return (::select(m_hSocket + 1, NULL, &fwds, NULL, &rtval) <= 0) ? false : true;
}

BOOL CSelectSocket::recv(char* buf, int* lpLen, int flag)
{
    fd_set frds;
    FD_ZERO(&frds);
    FD_SET(m_hSocket, &frds);
    timeval rtval;
    rtval.tv_sec = m_lRecvTimeout;
    rtval.tv_usec = 0;

    if (::select(m_hSocket + 1, &frds, NULL, NULL, &rtval) <= 0)
    {
        return false;
    }

    return CSLGSocket::recv(buf, lpLen, flag);
}

BOOL CSelectSocket::recvfrom(PSOCKADDR peer_addr, char* buf, int* len, int flag)
{
    fd_set frds;
    FD_ZERO(&frds);
    FD_SET(m_hSocket, &frds);
    timeval rtval;
    rtval.tv_sec = m_lRecvTimeout;
    rtval.tv_usec = 0;

    if (::select(m_hSocket + 1, &frds, NULL, NULL, &rtval) <= 0)
    {
        return false;
    }

    return CSLGSocket::recvfrom(peer_addr, buf, len, flag);
}

BOOL CSelectSocket::sendto(PSOCKADDR peer_addr, char* buf, int* len, int flag)
{
    fd_set fwds;
    FD_ZERO(&fwds);
    FD_SET(m_hSocket, &fwds);
    timeval wtval;
    wtval.tv_sec = m_lSendTimeout;
    wtval.tv_usec = 0;

    if (::select(m_hSocket + 1, NULL, &fwds, NULL, &wtval) <= 0)
    {
        return false;
    }

    return CSLGSocket::sendto(peer_addr, buf, len, flag);
}

BOOL CSelectSocket::send(IN const char* buf, IN OUT int* lpLen, int flag)
{
    PACKGE_HEADR header;
    memset(&header, 0x0, sizeof(header));
    *lpLen = (*lpLen > DATA_BUF) ? DATA_BUF : *lpLen;
    header.len = *lpLen;
    header.fcs = 0;
    header.fcs = CSLGSocket::checksum((unsigned short*)&header, sizeof(header));
    char tmp[DATA_BUF + sizeof(PACKGE_HEADR)] = {0};
    memcpy(tmp, &header, sizeof(header));
    memcpy(tmp + sizeof(header), buf, *lpLen);
    int nSendBytes = *lpLen + sizeof(header);
    int nTotalSends = 0;
    BOOL ret = false;
    int send_counts = 0;

    while (send_counts++ < MAX_SEND_COUNTS)
    {
        if (Send(tmp + nTotalSends, &nSendBytes, flag))
        {
            nTotalSends += nSendBytes;

            if (nTotalSends < (*lpLen + sizeof(header)))
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
            if (WSAECONNABORTED == CSLGSocket::GetLastError() || WSAECONNRESET == CSLGSocket::GetLastError() || *lpLen <= 0)
            {
                ret = false;
                break;
            }

        }

    }

    *lpLen = ret ? *lpLen : -1;
    return ret;
}

BOOL CSelectSocket::Send(const char* buf, int* lpLen, int flag)
{
    fd_set fwds;
    FD_ZERO(&fwds);
    FD_SET(m_hSocket, &fwds);
    timeval wtval;
    wtval.tv_sec = m_lSendTimeout;
    wtval.tv_usec = 0;

    if (::select(m_hSocket + 1, NULL, &fwds, NULL, &wtval) <= 0)
    {
        return false;
    }

    return CSLGSocket::send(buf, lpLen, flag);
}

BOOL CSelectSocket::Recv()
{
    BOOL ret = true;
    int recv_len = DATA_BUF - m_len;

    if (recv(m_buf + m_len, &recv_len) && recv_len > 0)
    {
        m_len += recv_len;
    }

    else
    {
        if ((WSAECONNRESET == CSLGSocket::GetLastError()) || (recv_len <= 0))   //??
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

BOOL CSelectSocket::recv(SOCK_DATA_NODE   // pNode)
{
    *pNode = NULL;

    if (m_len <= sizeof(PACKGE_HEADR))
    {
        return false;
    }

    PACKGE_HEADR header = {0, 0};
    memcpy(&header, m_buf, sizeof(PACKGE_HEADR));

    if (0 != CSLGSocket::checksum((unsigned short*)&header, sizeof(PACKGE_HEADR)))
    {
        memset(m_buf, 0x0, DATA_BUF);
        m_len = 0;
        return false;
    }

    if (m_len < header.len + sizeof(PACKGE_HEADR))
    {
        return false;
    }

    *pNode = m_Handle.Recv(m_buf + sizeof(PACKGE_HEADR), header.len);
    m_len -= header.len + sizeof(PACKGE_HEADR);
    memcpy(m_buf, m_buf + header.len + sizeof(PACKGE_HEADR), m_len);
    return true;
}

