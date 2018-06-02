// DevConnect.cpp: implementation of the DevConnect class.
//
#include "DevRgn.h"
REG_OBJECT(CMD_M_MCAST, DevRgn, false);
DevRgn::DevRgn(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  ProcessNCMD(s, pCMD, bServer, pvClientData);
}
DevRgn::DevRgn(const CSock* s, const char* cszGroupIP, int nSeq, char* pbBuf, unsigned int& nLen)//ÉèÖÃ
{
  assert(pbBuf);
  NCMD* pCMD = (NCMD*)pbBuf;
  s->setNCMD(pCMD, CMD_M_MCAST, CMD_S_SET, 0, nSeq);
  memcpy(pCMD + 1, cszGroupIP, strlen(cszGroupIP));
  nLen = sizeof(NCMD) + sizeof(CID);
}
DevRgn::DevRgn(const CSock* s, int nSeq, char* pbBuf, unsigned int& nLen)//»ñÈ¡
{
  assert(pbBuf);
  NCMD* pCMD = (NCMD*)pbBuf;
  s->setNCMD(pCMD, CMD_M_MCAST, CMD_S_GET, 0, nSeq);
  nLen = sizeof(NCMD);
}
void DevRgn::ProcessNCMD(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  assert(pCMD);

  switch (pCMD->m_SCmd) {
  case CMD_S_SET:
    SetRgn(s, pCMD, bServer, pvClientData);
    break;

  case CMD_S_GET:
    GetRgn(s, pCMD, bServer, pvClientData);
    break;

  default:
    break;
  }
}
void DevRgn::SetRgn(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  OnDevCmd(s, NULL, DEV_REGION_OPER, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
}
void DevRgn::GetRgn(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
}

