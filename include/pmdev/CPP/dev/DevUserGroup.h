// DevUserGroupCmd.h: interface for the DevUserGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEVUSERGROUPCMD_H__04411C2E_0AE1_4BAB_82A3_E41E3F270576__INCLUDED_)
#define AFX_DEVUSERGROUPCMD_H__04411C2E_0AE1_4BAB_82A3_E41E3F270576__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DevCmd.h"

class DevUserGroup : public DevData
{
public:
  DevUserGroup(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
  DevUserGroup(const char* cszIP, const DEV_USERGROUP_ARRAY& cArray, DEV_OPER_FLAG eFlag, int nSeq, char* pbBuf, unsigned int& nLen);  //…Ë÷√
private:
  void ProcessNCMD(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
  void UserGroupAdd(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
  void UserGroupDel(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
  void UserGroupDown(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
};


#endif // !defined(AFX_DEVUSERGROUPCMD_H__04411C2E_0AE1_4BAB_82A3_E41E3F270576__INCLUDED_)
