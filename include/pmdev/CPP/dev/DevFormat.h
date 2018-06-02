// DevFormatCommand.h: interface for the DevFormatCommand class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEVFORMATCOMMAND_H__A730A7B5_E1CF_446F_80FC_373CD9F0B929__INCLUDED_)
#define AFX_DEVFORMATCOMMAND_H__A730A7B5_E1CF_446F_80FC_373CD9F0B929__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DevCmd.h"

class DevFormat :  public DevData
{
public:
  DevFormat(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
  DevFormat(const char* cszIP, int nSeq, char* pbBuf, unsigned int& nLen); //∏Ò ΩªØ
private:
  void ProcessNCMD(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
};

#endif // !defined(AFX_DEVFORMATCOMMAND_H__A730A7B5_E1CF_446F_80FC_373CD9F0B929__INCLUDED_)
