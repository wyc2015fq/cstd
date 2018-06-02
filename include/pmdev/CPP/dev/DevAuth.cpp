// DevValidCommand.cpp: implementation of the DevValidCommand class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "DevAuth.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
REG_OBJECT(CMD_M_AUTH, DevAuth, false);

DevAuth::DevAuth(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  ProcessNCMD(cszIP, pCMD, bServer, pvClientData);
}

DevAuth::DevAuth(const char* cszIP, const DEV_AUTH& cAuth, BOOL bSet, int nSeq, char* pbBuf, unsigned int& nLen)
{
  assert(pbBuf);
  NCMD* pCMD     = (NCMD*)pbBuf;
  pCMD->m_MCmd   = CMD_M_AUTH;
  pCMD->m_SCmd   = CMD_S_SET ;
  pCMD->m_CFlag  = bSet ? 0 : F_AUTH_VERIFY;
  pCMD->m_Param1 = nSeq;
  pCMD->m_Param2 = DotIpToLong(cszIP);
  memcpy(pCMD + 1, &cAuth, sizeof(AUTH));
  nLen = sizeof(NCMD) + sizeof(AUTH);
}

void DevAuth::ProcessNCMD(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  assert(pCMD || CMD_S_SET == pCMD->m_SCmd);
  OnDevCmd(cszIP, NULL,
      (pCMD->m_CFlag & F_AUTH_VERIFY) ? DEV_AUTH_OPER : DEV_AUTH_SET_OPER,
      DEV_OPER_UPLOAD, NULL, pCMD->m_Param1,
      GET_ERR(pCMD->m_CFlag), pvClientData
          );
}
