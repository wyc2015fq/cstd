// DevValidCommand.cpp: implementation of the DevValidCommand class.
//
#include "DevAuth.h"
REG_OBJECT(CMD_M_AUTH, DevAuth, false);
DevAuth::DevAuth(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  ProcessNCMD(s, pCMD, bServer, pvClientData);
}
DevAuth::DevAuth(const CSock* s, const DEV_AUTH& cAuth, BOOL bSet, int nSeq, char* pbBuf, unsigned int& nLen)
{
  assert(pbBuf);
  NCMD* pCMD = (NCMD*)pbBuf;
  s->setNCMD(pCMD, CMD_M_AUTH, CMD_S_SET, bSet ? 0 : F_AUTH_VERIFY, nSeq);
  memcpy(pCMD + 1, &cAuth, sizeof(AUTH));
  nLen = sizeof(NCMD) + sizeof(AUTH);
}
void DevAuth::ProcessNCMD(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  assert(pCMD || CMD_S_SET == pCMD->m_SCmd);
  OnDevCmd(s, NULL,
      (pCMD->m_CFlag & F_AUTH_VERIFY) ? DEV_AUTH_OPER : DEV_AUTH_SET_OPER,
      DEV_OPER_UPLOAD, NULL, pCMD->m_Param1,
      GET_ERR(pCMD->m_CFlag), pvClientData
          );
}

