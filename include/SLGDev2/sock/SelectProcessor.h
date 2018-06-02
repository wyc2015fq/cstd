#ifndef __MY_SELECTNET_H__
#define __MY_SELECTNET_H__
#include "NetProcessor.h"
class CSelectProcessor : public CNetProcessor
{
  //  typedef std::map<SOCKET, SELECT_DATA*> DATAMap;
public:
  CSelectProcessor(uint32 uMemPoolSize, uint32 uMemPoolUnit);
  virtual ~CSelectProcessor();
public:
  virtual BOOL  StartUpWork(BOOL bStartUp);
  virtual void  StartRecv(SOCKET sock , int type, void* user);
  virtual void  StartListen(SOCKET sock, void* user);
  virtual void  StopRecv(SOCKET sock, void* user);
  virtual void  StopListen(SOCKET sock, void* user);
  virtual int32 Send(SOCKET sock, const int8* pbData, int32 nLen, const addr_in* pAddr);
private:
  static T_VOID CDECL_CALL process(void* param);
  void process();
  void HandleIO(SOCKET sock, void* user);
  void HandleListen(SOCKET sock, void* user);
  void StartRecv(SOCKET sock, int type , BOOL bListen, void* user);
private:
  T_HANDLE      m_hWorkTread; //工作线程句柄
  int32         m_bWorkRuning;
private:
  //  DATAMap     m_DataMap;
  //  CriticalSection m_synDataMap;
};
#endif

