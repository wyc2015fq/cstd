// DevProUpdateCommand.cpp: implementation of the DevProUpdateCommand class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "DevProUpdate.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
REG_OBJECT(CMD_M_APPUPDATE, DevProUpdate, false);

DevProUpdate::DevProUpdate(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  ProcessNCMD(cszIP, pCMD, bServer, pvClientData);
}

DevProUpdate::DevProUpdate(const char* cszIP, const DEV_APPUPDATE& cData, int nSeq, char* pbBuf, unsigned int& nLen)  //更新设备
{
  assert(pbBuf);
  NCMD* pCMD     = (NCMD*)pbBuf;
  pCMD->m_MCmd   = CMD_M_APPUPDATE;
  pCMD->m_SCmd   = CMD_S_SET;
  pCMD->m_Param1 = nSeq;
  pCMD->m_Param2 = DotIpToLong(cszIP);
  pCMD->m_CFlag  = 0;
  APPUPDATE* pData = (APPUPDATE*)(pCMD + 1);
  memcpy(pData, &cData, sizeof(APPUPDATE));
  memcpy(pData->m_Block, cData.m_Buf, cData.m_Bytes);
  nLen = sizeof(NCMD) + sizeof(APPUPDATE) + cData.m_Bytes;
}

void DevProUpdate::ProcessNCMD(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  OnDevCmd(cszIP, NULL, DEV_UPDATE_OPER, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
}

