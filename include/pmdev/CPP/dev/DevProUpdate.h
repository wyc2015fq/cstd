// DevProUpdateCommand.h: interface for the DevProUpdateCommand class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEVPROUPDATECOMMAND_H__B196427F_8ADF_42F4_97F9_FC2CC19AC4B5__INCLUDED_)
#define AFX_DEVPROUPDATECOMMAND_H__B196427F_8ADF_42F4_97F9_FC2CC19AC4B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DevCmd.h"

class DevProUpdate : public DevData
{
public:
  DevProUpdate(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
  DevProUpdate(const char* cszIP, const DEV_APPUPDATE& cData, int nSeq, char* pbBuf, unsigned int& nLen);  //更新设备
private:
  void ProcessNCMD(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
};

#endif // !defined(AFX_DEVPROUPDATECOMMAND_H__B196427F_8ADF_42F4_97F9_FC2CC19AC4B5__INCLUDED_)
