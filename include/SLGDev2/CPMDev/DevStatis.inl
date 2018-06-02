// DevStatisCommand.cpp: implementation of the DevStatisCommand class.
//
#include "DevStatis.h"
REG_OBJECT(CMD_M_STATIS, DevStatis, false);
DevStatis::DevStatis(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  ProcessNCMD(s, pCMD, bServer, pvClientData);
}
DevStatis::DevStatis(const CSock* s, int nSeq, char* pbBuf, unsigned int& nLen)
{
  assert(pbBuf);
  NCMD* pCMD = (NCMD*)pbBuf;
  s->setNCMD(pCMD, CMD_M_STATIS, CMD_S_GET, 0, nSeq);
  nLen = sizeof(NCMD);
}
void DevStatis::ProcessNCMD(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  OnDevCmd(s, NULL,
      DEV_STATISINFO_OPER, DEV_OPER_DOWNLOAD,
      (DEV_STATIS*)(pCMD + 1), pCMD->m_Param1,
      GET_ERR(pCMD->m_CFlag) , pvClientData);
}

