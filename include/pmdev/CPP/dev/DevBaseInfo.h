/*************************************************************************************/
/*名    称:设备基本信息获取命令类
/*功    能:发送或获取设备基本信息
/*************************************************************************************/

#if !defined(AFX_DEVINFOCOMMAND_H__77B73CF1_7AC4_4720_9C86_BC5A752DD67A__INCLUDED_)
#define AFX_DEVINFOCOMMAND_H__77B73CF1_7AC4_4720_9C86_BC5A752DD67A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DevCmd.h"

class DevInfo : public DevData
{
public:
  DevInfo(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
  DevInfo(const char* cszIP, int nSeq, char* pbBuf, unsigned int& nLen);    //获取
private:
  void ProcessNCMD(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
};

class DevReboot : public DevData
{
public:
  DevReboot(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
  DevReboot(const char* cszIP, int nSeq, char* pbBuf, unsigned int& nLen);    //获取
private:
  void ProcessNCMD(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
};

class CNetPro : public DevData
{
public:
  // 接收完数据，构造处理对象
  CNetPro(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
  // 发送数据时，构造处理对象
  CNetPro(const char* cszIP, DEV_NETPRO* pNetPro, int nSeq  , char* pbBuf , unsigned int& nLen);
private:
  void ProcessNCMD(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
};

#endif // !defined(AFX_DEVINFOCOMMAND_H__77B73CF1_7AC4_4720_9C86_BC5A752DD67A__INCLUDED_)
