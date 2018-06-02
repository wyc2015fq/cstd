// DevHeartCommand.cpp: implementation of the DevHeartCommand class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "DevHeart.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
REG_OBJECT(CMD_M_HEART, DevHeart, false);

DevHeart::DevHeart(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  ProcessNCMD(cszIP, pCMD, bServer, pvClientData);
}

void DevHeart::ProcessNCMD(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD), 4) >> 2] = {0};
  NCMD* pTMP     = (NCMD*)((char*)nData + OFFSET_SIZE);
  pTMP->m_MCmd   = CMD_M_HEART;
  pTMP->m_SCmd   = CMD_S_SET ;
  pTMP->m_Param2 = DotIpToLong(cszIP);
  unsigned int uLen = sizeof(NCMD);
  InternalSend(cszIP, (char*)pTMP, uLen, pvClientData);
}
