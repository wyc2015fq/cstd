// DevFormatCommand.cpp: implementation of the DevFormatCommand class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "DevFormat.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
REG_OBJECT(CMD_M_FORMAT, DevFormat, false);

DevFormat::DevFormat(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  ProcessNCMD(cszIP, pCMD, bServer, pvClientData);
}

DevFormat::DevFormat(const char* cszIP, int nSeq, char* pbBuf, unsigned int& nLen)
{
  assert(pbBuf);
  NCMD* pCMD     = (NCMD*)pbBuf;
  pCMD->m_MCmd   = CMD_M_FORMAT;
  pCMD->m_SCmd   = CMD_S_SET ;
  pCMD->m_Param1 = nSeq;
  pCMD->m_Param2 = DotIpToLong(cszIP);
  pCMD->m_CFlag  = 0;
  nLen = sizeof(NCMD);
}

void DevFormat::ProcessNCMD(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  OnDevCmd(cszIP, NULL, DEV_FORMAT_OPER, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
}

