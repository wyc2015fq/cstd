// DevLogCommand.h: interface for the DevLogCommand class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEVLOGCOMMAND_H__826BFC46_620C_4F3F_AE72_19D981AFF04E__INCLUDED_)
#define AFX_DEVLOGCOMMAND_H__826BFC46_620C_4F3F_AE72_19D981AFF04E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DevCmd.h"

class DevLog :  public DevData
{
public:
  DevLog(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
  DevLog(const char* cszIP, const DEV_DL_RECORD_RANGE& cRange, int nFlag, int nSeq, char* pbBuf, unsigned int& nLen);  //обть
private:
  void ProcessNCMD(const char* cszIP, NCMD* pCMD, BOOL isServer, void* pvClientData);
};

#endif // !defined(AFX_DEVLOGCOMMAND_H__826BFC46_620C_4F3F_AE72_19D981AFF04E__INCLUDED_)
