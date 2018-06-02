// DevTimeSynCommand.h: interface for the DevTimeSynCommand class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEVTIMESYNCOMMAND_H__49CE06EE_0963_46F7_9D57_A159A1619DBF__INCLUDED_)
#define AFX_DEVTIMESYNCOMMAND_H__49CE06EE_0963_46F7_9D57_A159A1619DBF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DevCmd.h"

class DevTime : public DevData
{
public:
  DevTime(const char* cszIP,  NCMD* pCMD, BOOL bServer, void* pvClientData);
  DevTime(const char* cszIP, const DEV_DATETIME& cTime, int nSeq, char* pbBuf, unsigned int& nLen);//…Ë÷√
  DevTime(const char* cszIP, int nSeq, char* pbBuf, unsigned int& nLen);//ªÒ»°
private:
  void ProcessNCMD(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
  void SetTime(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
  void GetTime(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
};

#endif // !defined(AFX_DEVTIMESYNCOMMAND_H__49CE06EE_0963_46F7_9D57_A159A1619DBF__INCLUDED_)
