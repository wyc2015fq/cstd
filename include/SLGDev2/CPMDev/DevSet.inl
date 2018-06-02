// DevSetCommand.cpp: implementation of the DevSetCommand class.
//
#include "DevSet.h"
REG_OBJECT(CMD_M_DEVSET, DevSet, false);
DevSet::DevSet(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  ProcessNCMD(s, pCMD, bServer, pvClientData);
}
DevSet::DevSet(const CSock* s, int nSeq, char* pbBuf, unsigned int& nLen)
{
  assert(pbBuf);
  NCMD* pCMD = (NCMD*)pbBuf;
  s->setNCMD(pCMD, CMD_M_DEVSET, CMD_S_GET, 0, nSeq);
  nLen = sizeof(NCMD);
}
DevSet::DevSet(const CSock* s, const DEV_WORKATT& cAtt, int nSeq, char* pbBuf, unsigned int& nLen)
{
  assert(pbBuf);
  NCMD* pCMD = (NCMD*)pbBuf;
  s->setNCMD(pCMD, CMD_M_DEVSET, CMD_S_SET, 0, nSeq);
  DEVSET* pSet = (DEVSET*)(pCMD + 1);
  memcpy(pSet->m_DevCtx, cAtt.m_TimeGID, sizeof(cAtt.m_TimeGID));
  pSet->m_DefaultRight = cAtt.m_DefaultRight;
  pSet->m_BaseSet = cAtt.m_BaseSet;
  memcpy(pSet->m_SuperPassword.m_ID, cAtt.m_szSuperPWD.m_ID, sizeof(CID));
  pSet->m_DoorMangetTime = cAtt.m_DoorMangetTime;
  pSet->m_LockTime = cAtt.m_LockTime;
  pSet->m_VerifyMode = cAtt.m_VerifyMode;
  pSet->m_OutputWG = cAtt.m_bWGOutPut ? OUTPUT_WG_YES : 0x00;
  pSet->m_OutputWG |= cAtt.m_nWGType ? OUTPUT_WG_34 : 0x00;
  pSet->m_OutputWG |= cAtt.m_nWGOutType ? OUTPUT_WG_CTX : 0x00;
  pSet->m_OutputWG |= cAtt.m_nWGOutIDType ? OUTPUT_WG_UID : 0x00;
  pSet->m_OutputCtx = cAtt.m_nWGOutContent;
  nLen = sizeof(NCMD) + sizeof(DEVSET);
}
void DevSet::ProcessNCMD(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  assert(pCMD);

  switch (pCMD->m_SCmd) {
  case CMD_S_SET:
    SetAtt(s, pCMD, bServer, pvClientData);
    break;

  case CMD_S_GET:
    GetAtt(s, pCMD, bServer, pvClientData);
    break;

  default:
    break;
  }
}
void DevSet::SetAtt(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  OnDevCmd(s, NULL, DEV_WORKATT_OPER, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
}
void DevSet::GetAtt(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  DEV_WORKATT att = {0};

  if (0 == GET_ERR(pCMD->m_CFlag)) {
    DEVSET* pSet = (DEVSET*)(pCMD + 1);
    memcpy(att.m_TimeGID, pSet->m_DevCtx, sizeof(pSet->m_DevCtx));
    att.m_BaseSet = pSet->m_BaseSet;
    att.m_DoorMangetTime = pSet->m_DoorMangetTime;
    att.m_LockTime = pSet->m_LockTime;
    att.m_VerifyMode = pSet->m_VerifyMode;
    att.m_bWGOutPut = (BOOL)(pSet->m_OutputWG & OUTPUT_WG_YES);
    att.m_nWGType = (BOOL)(pSet->m_OutputWG & OUTPUT_WG_34);
    att.m_nWGOutType = (BOOL)(pSet->m_OutputWG & OUTPUT_WG_CTX);
    att.m_nWGOutIDType = (BOOL)(pSet->m_OutputWG & OUTPUT_WG_UID);
    att.m_nWGOutContent = pSet->m_OutputCtx;
    att.m_DefaultRight = (DEV_DEFAULT_RIGHT)(pSet->m_DefaultRight);
    memcpy(att.m_szSuperPWD.m_ID, pSet->m_SuperPassword.m_ID, sizeof(DEV_CID));
  }

  OnDevCmd(s, NULL, DEV_WORKATT_OPER, DEV_OPER_DOWNLOAD, &att, pCMD->m_Param1, pCMD->m_CFlag, pvClientData);
}

