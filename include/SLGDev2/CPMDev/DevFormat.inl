// DevFormatCommand.cpp: implementation of the DevFormatCommand class.
//
#include "DevFormat.h"
REG_OBJECT(CMD_M_FORMAT, DevFormat, false);
DevFormat::DevFormat(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  ProcessNCMD(s, pCMD, bServer, pvClientData);
}
DevFormat::DevFormat(const CSock* s, int nSeq, char* pbBuf, unsigned int& nLen)
{
  assert(pbBuf);
  NCMD* pCMD = (NCMD*)pbBuf;
  s->setNCMD(pCMD, CMD_M_FORMAT, CMD_S_SET, 0, nSeq);
  nLen = sizeof(NCMD);
}
void DevFormat::ProcessNCMD(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  OnDevCmd(s, NULL, DEV_FORMAT_OPER, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
}

