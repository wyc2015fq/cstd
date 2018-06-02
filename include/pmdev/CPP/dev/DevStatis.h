// DevStatisCommand.h: interface for the DevStatisCommand class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEVSTATISCOMMAND_H__BDF9604C_FC86_43CB_81D6_10F2000C0AE6__INCLUDED_)
#define AFX_DEVSTATISCOMMAND_H__BDF9604C_FC86_43CB_81D6_10F2000C0AE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DevCmd.h"

class DevStatis : public DevData
{
public:
  DevStatis(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData) ;
  DevStatis(const char* cszIP, int nSeq, char* pbBuf, unsigned int& nLen); //ªÒ»°
private:
  void ProcessNCMD(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
};


#endif // !defined(AFX_DEVSTATISCOMMAND_H__BDF9604C_FC86_43CB_81D6_10F2000C0AE6__INCLUDED_)
