// Client.h: interface for the CClient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENT_H__CD9B265C_3BEB_4F36_AFF1_7B9E3842A950__INCLUDED_)
#define AFX_CLIENT_H__CD9B265C_3BEB_4F36_AFF1_7B9E3842A950__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define MAX_TIME_INTERVAL 30
#define VSOCKET           ((HANDLE)0xffffffff)

enum { LINK_FAIL = 0, LINK_OK, LINK_ALREADY };

#include "../dev/utime.hpp"
#include "sock.h"

class DspDevManager;
class CNet
{
public:
  CNet(): m_bInit(FALSE), m_Trans(FALSE), m_SrvCb(NULL), m_SrvCtx(NULL), m_Manager(NULL) {}
  virtual ~CNet() {
    StartWork(FALSE);
  }
  int SetManager(DspDevManager* Manager) {
    m_Manager = (Manager);
    ASSERT(Manager);
    return 0;
  }
public:
  BOOL StartWork(BOOL bFlag);
  BOOL Serch(BOOL bFlag, const char* cszIP);
  BOOL NetCN(BOOL bFlag, const char* cszIP);
  BOOL SetMode(BOOL srvMode, QuerySrvCB pfn,  void* pvContext);
  BOOL CnSrv(BOOL bFlag, const char* ip, const char* name, const char* psw);
  BOOL Send(const char* cszIP, const char* pbBuf, unsigned int* puLen);
  void TimeOut(int nDelay = MAX_TIME_INTERVAL);
  HANDLE CreateVideoHandle();
  void ReleaseVideoHanle(HANDLE hVideoHanle);
  BOOL GetVideoHandleAddr(HANDLE hVideoHanle, SOCK_ADDRESS& Addr);
  BOOL TransMode(void) {
    return m_Trans;
  }
public:
  void RecvNotify(HANDLE hSOCK, const char* pbData, int nLen);
  void RecvFromNotify(HANDLE hSOCK, const char* pbData, int nLen, const SOCK_ADDRESS& Addr);
private:
  BOOL NetDCN(const char* cszIP, BOOL tri = true);
  BOOL NetDCN(HANDLE hSOCK);
  BOOL NetCN_Imp(const char* cszIP, HANDLE rSock, BOOL tri = true);
  long NetCN_Imp(const char* cszIP);
  BOOL SerchBegin(const char* cszIP);
  BOOL SerchEnd(const char* cszIP);
  void SerchNotify(const char* cszIP);
  void CallSrvSB(const char* ip, PEERADR& addr);
  HANDLE FindTranSrv(const PEERADR& addr);
private:

  class CSock
  {
  public:
    CSock(HANDLE hSOCK, HANDLE sSOCK, const char* cszIP, USHORT port = 0, BOOL bCalTimeOut = TRUE):
      m_hSOCK(hSOCK), m_sSock(sSOCK), m_bCalTimeOut(bCalTimeOut) {
      memset(&m_Addr, 0x0, sizeof(m_Addr));
      strncpy(m_Addr.IP_Address, cszIP, sizeof(m_Addr.IP_Address) - 1);
      m_Addr.Port = port;
    }
    ~CSock() {}
  public:
    HANDLE GetHandle(void) const {
      return m_hSOCK;
    }
    HANDLE GetRsock(void) const {
      return m_sSock;
    }
    const PEERADR& GetAddr() const {
      return m_Addr;
    }
    BOOL IsActive() {
      return  m_bCalTimeOut ? (m_uutime.elapsed() < MAX_TIME_INTERVAL) : true ;
    }
    void SetActive() {
      m_uutime.restart();
    }
  private:
    HANDLE  m_hSOCK;
    HANDLE  m_sSock;
    PEERADR m_Addr;
    uutime  m_uutime;
    BOOL    m_bCalTimeOut;
  };

private:
  typedef std::map<unsigned long, CSock*> SOCK_MAP;
  SOCK_MAP m_map;
  CriticalSection m_syn;
  CriticalSection m_SynSrvCb;
  CriticalSection m_synLink;
  DspDevManager*  m_Manager;
  BOOL m_bInit;
  BOOL m_Trans;
  QuerySrvCB m_SrvCb;
  void*      m_SrvCtx;
};

#endif // !defined(AFX_CLIENT_H__CD9B265C_3BEB_4F36_AFF1_7B9E3842A950__INCLUDED_)
