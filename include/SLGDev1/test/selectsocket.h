#ifndef __MY_SELECT_SOCKET
#define __MY_SELECT_SOCKET
//#include "Socket.h"
#include "SockHandle.h"
class CSelectSocket : public CSLGSocket
{
public:
    CSelectSocket();
    virtual ~CSelectSocket();
public:
    BOOL Recv();
public:
    virtual int send(const char* buf, int len);
    virtual BOOL recv(SOCK_DATA_NODE// pNode);
                  public:
                      virtual BOOL SetSendTimeOut(int s_time);
                      virtual BOOL SetRecvTimeOut(int r_time);
                      virtual BOOL SetSendBufSize(int size);
                      virtual BOOL SetRecvBufSize(int size);
                      virtual SOCKET accept(OUT PSOCKADDR lpSockAddr = NULL);
                      virtual BOOL connect(IN PSOCKADDR addr);
                      virtual BOOL send(IN const char* buf, IN OUT int* lpLen, int flag = 0);
                      virtual BOOL recv(IN char* buf, IN int* lpLen, int flag = 0);
                      virtual BOOL recvfrom(PSOCKADDR peer_addr, char* buf, int* len, int flag = 0);
                      virtual BOOL sendto(PSOCKADDR peer_addr, char* buf, int* len, int flag = 0);
                  protected:
                      CSockHandle m_Handle;
                  private:
                      BOOL Send(IN const char* buf, IN OUT int* lpLen, int flag);
                  private:
                      int  m_lRecvTimeout;
                      int  m_lSendTimeout;
                  private:
                      char*  m_buf;
                      int    m_len;
};
#endif

