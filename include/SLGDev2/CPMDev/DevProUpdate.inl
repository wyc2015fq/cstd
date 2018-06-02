// DevProUpdateCommand.cpp: implementation of the DevProUpdateCommand class.
//
#include "DevProUpdate.h"
REG_OBJECT(CMD_M_APPUPDATE, DevProUpdate, false);
DevProUpdate::DevProUpdate(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  ProcessNCMD(s, pCMD, bServer, pvClientData);
}
DevProUpdate::DevProUpdate(const CSock* s, const DEV_APPUPDATE& cData, int nSeq, char* pbBuf, unsigned int& nLen) //更新设备
{
  assert(pbBuf);
  NCMD* pCMD = (NCMD*)pbBuf;
  s->setNCMD(pCMD, CMD_M_APPUPDATE, CMD_S_SET, 0, nSeq);
  APPUPDATE* pData = (APPUPDATE*)(pCMD + 1);
  pData->m_Bytes = cData.m_Bytes;
  pData->m_Type = cData.m_Type;
  memcpy(&pData->m_CtxInfo , &cData.m_CtxInfo, sizeof(CID));
  memcpy(&pData->m_FileName , &cData.m_FileName, sizeof(CID));
  //memcpy(pData, &cData, sizeof(APPUPDATE));
  memcpy(pData->m_Block, cData.m_Buf, cData.m_Bytes);
  nLen = sizeof(NCMD) + sizeof(APPUPDATE) + cData.m_Bytes;
}
void DevProUpdate::ProcessNCMD(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  OnDevCmd(s, NULL, DEV_UPDATE_OPER, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
}

