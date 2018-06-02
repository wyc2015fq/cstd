// DevTimeGroupCommand.h: interface for the DevTimeGroupCommand class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEVTIMEGROUPCOMMAND_H__DDF223FA_C95B_4E0A_8ED0_C143C6445C61__INCLUDED_)
#define AFX_DEVTIMEGROUPCOMMAND_H__DDF223FA_C95B_4E0A_8ED0_C143C6445C61__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DevCmd.h"

class DevTimeGroup : public DevData
{
public:
  DevTimeGroup(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
  DevTimeGroup(const char* cszIP, const DEV_TIMEGROUP_ARRAY& cArray, int nType, DEV_OPER_FLAG eFlag, int nSeq, char* pbBuf, unsigned int& nLen);  //…Ë÷√
private:
  void ProcessNCMD(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
  void TimeGroupAdd(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
  void TimeGroupDel(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
  void TimeGroupDown(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
};

#endif // !defined(AFX_DEVTIMEGROUPCOMMAND_H__DDF223FA_C95B_4E0A_8ED0_C143C6445C61__INCLUDED_)
