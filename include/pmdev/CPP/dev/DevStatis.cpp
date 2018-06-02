// DevStatisCommand.cpp: implementation of the DevStatisCommand class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "DevStatis.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
REG_OBJECT(CMD_M_STATIS, DevStatis, false);

DevStatis::DevStatis(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  ProcessNCMD(cszIP, pCMD, bServer, pvClientData);
}

DevStatis::DevStatis(const char* cszIP, int nSeq, char* pbBuf, unsigned int& nLen)
{
  assert(pbBuf);
  NCMD* pCMD     = (NCMD*)pbBuf;
  pCMD->m_MCmd   = CMD_M_STATIS;
  pCMD->m_SCmd   = CMD_S_GET ;
  pCMD->m_Param1 = nSeq;
  pCMD->m_Param2 = DotIpToLong(cszIP);
  pCMD->m_CFlag  = 0;
  nLen = sizeof(NCMD);
}

void DevStatis::ProcessNCMD(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  OnDevCmd(cszIP, NULL,
      DEV_STATISINFO_OPER, DEV_OPER_DOWNLOAD,
      (DEV_STATIS*)(pCMD + 1), pCMD->m_Param1,
      GET_ERR(pCMD->m_CFlag) , pvClientData);
}

