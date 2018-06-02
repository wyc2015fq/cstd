// DevCN.h: interface for the DevCN class.
//
#if !defined(AFX_DEVCN_H__2E4581F0_A73D_4F13_9657_F55D46C0EFAA__INCLUDED_)
#define AFX_DEVCN_H__2E4581F0_A73D_4F13_9657_F55D46C0EFAA__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "DevCmd.h"
class CDevCN : public DevData
{
public:
  CDevCN(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData);
private:
  void ProcessNCMD(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData);
};
#endif // !defined(AFX_DEVCN_H__2E4581F0_A73D_4F13_9657_F55D46C0EFAA__INCLUDED_)

