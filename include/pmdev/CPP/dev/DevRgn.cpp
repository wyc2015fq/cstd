// DevConnect.cpp: implementation of the DevConnect class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "DevRgn.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
REG_OBJECT(CMD_M_MCAST, DevRgn, false);

DevRgn::DevRgn(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  ProcessNCMD(cszIP, pCMD, bServer, pvClientData);
}

DevRgn::DevRgn(const char* cszIP, const char* cszGroupIP, int nSeq, char* pbBuf, unsigned int& nLen)//ÉèÖÃ
{
  assert(pbBuf);
  NCMD* pCMD     = (NCMD*)pbBuf;
  pCMD->m_MCmd   = CMD_M_MCAST;
  pCMD->m_SCmd   = CMD_S_SET;
  pCMD->m_Param1 = nSeq;
  pCMD->m_Param2 = DotIpToLong(cszIP);
  pCMD->m_CFlag  = 0;
  memcpy(pCMD + 1, cszGroupIP, strlen(cszGroupIP));
  nLen = sizeof(NCMD) + sizeof(CID);
}

DevRgn::DevRgn(const char* cszIP, int nSeq, char* pbBuf, unsigned int& nLen)//»ñÈ¡
{
  assert(pbBuf);
  NCMD* pCMD     = (NCMD*)pbBuf;
  pCMD->m_MCmd   = CMD_M_MCAST;
  pCMD->m_SCmd   = CMD_S_GET;
  pCMD->m_Param1 = nSeq;
  pCMD->m_Param2 = DotIpToLong(cszIP);
  pCMD->m_CFlag  = 0;
  nLen = sizeof(NCMD);
}

void DevRgn::ProcessNCMD(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  assert(pCMD);

  switch (pCMD->m_SCmd) {
  case CMD_S_SET:
    SetRgn(cszIP, pCMD, bServer, pvClientData);
    break;

  case CMD_S_GET:
    GetRgn(cszIP, pCMD, bServer, pvClientData);
    break;

  default:
    break;
  }

}

void DevRgn::SetRgn(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  OnDevCmd(cszIP, NULL, DEV_REGION_OPER, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
}

void DevRgn::GetRgn(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{

}