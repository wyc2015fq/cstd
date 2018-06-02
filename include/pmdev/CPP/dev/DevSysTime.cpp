// DevTimeSynCommand.cpp: implementation of the DevTimeSynCommand class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "DevSysTime.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
REG_OBJECT(CMD_M_TIME, DevTime, false);

DevTime::DevTime(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  ProcessNCMD(cszIP, pCMD, bServer, pvClientData);
}

DevTime::DevTime(const char* cszIP, const DEV_DATETIME& cTime, int nSeq, char* pbBuf, unsigned int& nLen)//ÉèÖÃ
{
  assert(pbBuf);
  NCMD* pCMD     = (NCMD*)pbBuf;
  pCMD->m_MCmd   = CMD_M_TIME;
  pCMD->m_SCmd   = CMD_S_SET;
  pCMD->m_Param1 = nSeq;
  pCMD->m_Param2 = DotIpToLong(cszIP);
  pCMD->m_CFlag  = 0;
  memcpy(pCMD + 1, &cTime, sizeof(DATETIME));
  nLen = sizeof(NCMD) + sizeof(DATETIME);

}

DevTime::DevTime(const char* cszIP, int nSeq, char* pbBuf, unsigned int& nLen)//»ñÈ¡
{
  assert(pbBuf);
  NCMD* pCMD     = (NCMD*)pbBuf;
  pCMD->m_MCmd   = CMD_M_TIME;
  pCMD->m_SCmd   = CMD_S_GET;
  pCMD->m_Param1 = nSeq;
  pCMD->m_Param2 = DotIpToLong(cszIP);
  pCMD->m_CFlag  = 0;
  nLen = sizeof(NCMD);
}

void DevTime::ProcessNCMD(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  assert(pCMD);

  switch (pCMD->m_SCmd) {
  case CMD_S_SET:
    SetTime(cszIP, pCMD, bServer, pvClientData);
    break;

  case CMD_S_GET:
    GetTime(cszIP, pCMD, bServer, pvClientData);
    break;

  default:
    break;
  }
}

void DevTime::SetTime(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  OnDevCmd(cszIP, NULL, DEV_SYS_TIME_OPER, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
}

void DevTime::GetTime(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  OnDevCmd(cszIP, NULL,
      DEV_SYS_TIME_OPER, DEV_OPER_DOWNLOAD,
      (DEV_DATETIME*)(pCMD + 1), pCMD->m_Param1,
      GET_ERR(pCMD->m_CFlag), pvClientData);
}