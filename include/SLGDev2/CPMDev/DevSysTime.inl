// DevTimeSynCommand.cpp: implementation of the DevTimeSynCommand class.
//
#include "DevSysTime.h"
REG_OBJECT(CMD_M_TIME, DevTime, false);
DevTime::DevTime(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  ProcessNCMD(s, pCMD, bServer, pvClientData);
}
DevTime::DevTime(const CSock* s, const DEV_DATETIME& cTime, int nSeq, char* pbBuf, unsigned int& nLen)//ÉèÖÃ
{
  assert(pbBuf);
  NCMD* pCMD = (NCMD*)pbBuf;
  s->setNCMD(pCMD, CMD_M_TIME, CMD_S_SET, 0, nSeq);
  memcpy(pCMD + 1, &cTime, sizeof(DATETIME));
  nLen = sizeof(NCMD) + sizeof(DATETIME);
}
DevTime::DevTime(const CSock* s, int nSeq, char* pbBuf, unsigned int& nLen)//»ñÈ¡
{
  assert(pbBuf);
  NCMD* pCMD = (NCMD*)pbBuf;
  s->setNCMD(pCMD, CMD_M_TIME, CMD_S_GET, 0, nSeq);
  nLen = sizeof(NCMD);
}
void DevTime::ProcessNCMD(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  assert(pCMD);

  switch (pCMD->m_SCmd) {
  case CMD_S_SET:
    SetTime(s, pCMD, bServer, pvClientData);
    break;

  case CMD_S_GET:
    GetTime(s, pCMD, bServer, pvClientData);
    break;

  default:
    break;
  }
}
void DevTime::SetTime(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  OnDevCmd(s, NULL, DEV_SYS_TIME_OPER, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
}
void DevTime::GetTime(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  OnDevCmd(s, NULL,
      DEV_SYS_TIME_OPER, DEV_OPER_DOWNLOAD,
      (DEV_DATETIME*)(pCMD + 1), pCMD->m_Param1,
      GET_ERR(pCMD->m_CFlag), pvClientData);
}

