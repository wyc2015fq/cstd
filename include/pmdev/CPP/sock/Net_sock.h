#ifndef __MY_NET_H__
#define __MY_NET_H__

#include "common.h"
#if 0
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
  BOOL      m_bRunning;
  T_HANDLE    m_hTread;
};
#endif

class CNetManager;
class CNet_sock : public CThread
{

public:
  CNet_sock(CNetManager& mgr);
  ~CNet_sock();
public:
  void SerchMember(SOCKET sock, const SOCK_ADDRESS& GroupIPAddr);
  void StopSerch(SOCKET sock);
  void Listen(SOCKET sock, BOOL bStartUp);
  void JoinGroup(SOCKET sock, BOOL bJoin);
protected:
  virtual void process();
private:
  BOOL Insert(SOCKET sock,  SOCK_CLASS sc);
  BOOL Erase(SOCKET sock);
  void HandleOper(SOCKET sock);
  void ListenOper(SOCKET sock);
  void SerchOper(SOCKET sock);
private:
  typedef std::map<SOCKET, SOCK_CLASS> SOCK_MAP;
  SOCK_MAP m_map;
  CriticalSection m_syn;
private:
  CNetManager& m_Mgr;
};
#endif