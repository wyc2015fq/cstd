// DevManager.h: interface for the DevManager class.
//
#if !defined(AFX_DEVMANAGER_H__EF52C8CA_C60E_484E_B595_6B9FFF3A316A__INCLUDED_)
#define AFX_DEVMANAGER_H__EF52C8CA_C60E_484E_B595_6B9FFF3A316A__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Net.h"
#include "common.h"
#include "video.h"
#define SYN_REQUES_CONNECT 0x01 //同步连接请求
#define SYN_ACK_CONNECT 0x10 //同步连接确认
#define SYN_REQUES_CONNECT 0x01 //同步连接请求
#define SYN_ACK_CONNECT 0x10 //同步连接确认
#define MULTYGROUP_PORT 13332 //多播组端口
#define MULTYGROUP_IP "224.0.1.100" //多播组地址
#define MAX_MEMRESERVER_SIZE (1024*1024)
#define SERVER_PORT 13333
#define VIDEO_PORT 23334
#define BINDE_TIMES 20
#define SEND_TIMEOUT 1
#define SEND_BUF (64*1024)
#define RECV_BUF (64*1024)
class DspDevManager : public CThread
{
  friend class DevData;
  friend class CNet;
public:
  DspDevManager();
  virtual ~DspDevManager();
public:
  void RegDevOPNotify(DevOperResultNotify pfnOpNotify, void* pvContext) { //数据操作结果回调
    //    AoutLock lock( &m_syn );
    m_pfnOpNotify      = pfnOpNotify;
    m_pOpNotifyContext = pvContext;
  }
  void RegNetCNNotify(DevConnectStatus pfnCN, void* pCNContext) { //注册设备连接状态提醒回调
    //    AoutLock lock( &m_syn );
    m_pfnCN      = pfnCN;
    m_pCNContext = pCNContext;
  }
public:
  BOOL StartWork(BOOL bFlag);
  BOOL Serch(BOOL bFlag, const char* cszIP);
  void TimeOut(int nDelay = MAX_TIME_INTERVAL);

  BOOL TransMode(void) {
    return m_Trans;
  }
  void RecvNotify(SOCKET hSOCK, const char* pbData, int nLen, void* user);
  void RecvFromNotify(SOCKET hSOCK, const char* pbData, int nLen, const addr_in* Addr, void* user);

  BOOL NetDCN(CSock* s, E_CONNECT_STATUS eFlag, BOOL tri = FALSE);
  BOOL NetCN_Imp(const char* cszIP, SOCKET rSock, BOOL tri = true);
  int NetCN_Imp_Real(const char* cszIP, BOOL srv);
  BOOL SerchBegin(const char* cszIP);
  BOOL SerchEnd(const char* cszIP);
  void SerchNotify(const CSock* s);
  void CallSrvSB(const char* ip, PEERADR* addr);
  CSock* FindTranSrv(const PEERADR& addr);
public:
  BOOL InitSys(BOOL bFlag);
  BOOL DevSerch(BOOL bFlag, const char* cszGroupIP = DEV_REGION_ADDR);
  //设备工作模式
  BOOL SetMode(BOOL srvMode, QuerySrvCB pfn,  void* pvContext);
  //连接/断开中转服务器
  BOOL CnSrv(BOOL bFlag, const char* ip, const char* name, const char* psw);
  //连接断开设备
  BOOL CNDev(const char* cszIP);
  void DCNDev(const char* cszIP);
protected:
  virtual void Run();
public:
  friend void DevCMDNotify(const CSock* s,
      const addr_inx* cszClientAddr,
      DEV_OPER_TYPE eType,
      DEV_OPER_FLAG eFlag,
      void* pvContent,
      int nSeq,
      DEV_OPER_RESULT rt,
      void* pvClientData
                          );
  friend BOOL InternalSend(const CSock* s, const char* pbBuf, unsigned int uLen, void* pvClientData);
  friend void CNNotify(const CSock* s, E_CONNECT_STATUS eFlag, BOOL bServer, void* pvClientData);
public:
  int  OnCNNotify(const char* sIP, E_CONNECT_STATUS eFlag, BOOL bServer = TRUE);
  void OnRecvNotify(const CSock* s, const char* cpbBuf, unsigned int uLen, BOOL bServer = TRUE);

  void OnDevCMDNotify(const CSock* s,
      const char* cszClientAddr,
      DEV_OPER_TYPE eType,
      DEV_OPER_FLAG eFlag,
      void* pvContent,
      int nSeq,
      DEV_OPER_RESULT eResult
                     );
private:
  DevConnectStatus    m_pfnCN;
  void*               m_pCNContext;
  DevOperResultNotify m_pfnOpNotify;
  void*               m_pOpNotifyContext;
public:
  typedef std::map<std::string, CSock*> SOCK_MAP;
  CSock* getSock(const char* ip) {
    std::string ss(ip);
    SOCK_MAP::iterator it = m_map.find(ss);

    if (it != m_map.end()) {
      return it->second;
    }

    return NULL;
  }
  SOCK_MAP m_map;
  CriticalSection m_syn;
  CriticalSection m_SynSrvCb;
  CriticalSection m_synLink;
  BOOL m_bInit;
  BOOL m_Trans;
  QuerySrvCB m_SrvCb;
  void*      m_SrvCtx;

  //  CriticalSection m_syn;
};
#endif // !defined(AFX_DEVMANAGER_H__EF52C8CA_C60E_484E_B595_6B9FFF3A316A__INCLUDED_)

