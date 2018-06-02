// DevRightCommand.h: interface for the DevRightCommand class.
//
#if !defined(AFX_DEVRIGHTCOMMAND_H__294328D2_C630_4BDF_945C_5D9E832BC7F4__INCLUDED_)
#define AFX_DEVRIGHTCOMMAND_H__294328D2_C630_4BDF_945C_5D9E832BC7F4__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "DevCmd.h"
class DevRight : public DevData
{
public:
  DevRight(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData);
  DevRight(const CSock* s, const DEV_RIGHT_ARRAY& cArray, DEV_OPER_FLAG eFlag, int nSeq, char* pbBuf, unsigned int& nLen);
private:
  void ProcessNCMD(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData);
  void RightAdd(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData);
  void RightDel(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData);
  void RightDown(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData);
};
#endif // !defined(AFX_DEVRIGHTCOMMAND_H__294328D2_C630_4BDF_945C_5D9E832BC7F4__INCLUDED_)

