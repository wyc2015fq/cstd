// DevValidCommand.h: interface for the DevValidCommand class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEVVALIDCOMMAND_H__FB0290B0_9FAE_4C36_90C1_D952C2706E8C__INCLUDED_)
#define AFX_DEVVALIDCOMMAND_H__FB0290B0_9FAE_4C36_90C1_D952C2706E8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DevCmd.h"

class DevAuth : public DevData
{
public:
  DevAuth(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
  DevAuth(const char* cszIP, const DEV_AUTH& cAuth, BOOL bSet, int nSeq, char* pbBuf, unsigned int& uLen);
private:
  void ProcessNCMD(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
};


#endif // !defined(AFX_DEVVALIDCOMMAND_H__FB0290B0_9FAE_4C36_90C1_D952C2706E8C__INCLUDED_)
