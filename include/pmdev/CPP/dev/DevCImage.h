// ChangeImageCmd.h: interface for the ChangeImage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHANGEIMAGECMD_H__FE3D6C3B_5E91_43EE_B6C5_A9829D30D8BF__INCLUDED_)
#define AFX_CHANGEIMAGECMD_H__FE3D6C3B_5E91_43EE_B6C5_A9829D30D8BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DevCmd.h"

class DevCImage : public DevData
{
public:
  DevCImage(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
  DevCImage(const char* cszIP, const DEV_DL_RECORD_RANGE& cRange, int nFlag, int nSeq, char* pbBuf, unsigned int& nLen);  //обть
private:
  void ProcessNCMD(const char* cszIP, NCMD* pCMD, BOOL Server, void* pvClientData);
};

#endif // !defined(AFX_CHANGEIMAGECMD_H__FE3D6C3B_5E91_43EE_B6C5_A9829D30D8BF__INCLUDED_)
