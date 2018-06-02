// DevLogCommand.cpp: implementation of the DevLogCommand class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "DevLog.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
REG_OBJECT(CMD_M_OPTLOG, DevLog, false);

DevLog::DevLog(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  ProcessNCMD(cszIP, pCMD, bServer, pvClientData);
}

DevLog::DevLog(const char* cszIP, const DEV_DL_RECORD_RANGE& cRange, int nFlag, int nSeq, char* pbBuf, unsigned int& nLen)
{
  assert(pbBuf);
  NCMD* pCMD     = (NCMD*)pbBuf;
  pCMD->m_MCmd   = CMD_M_OPTLOG;
  pCMD->m_SCmd   = CMD_S_GET;
  pCMD->m_Param1 = nSeq;
  pCMD->m_Param2 = DotIpToLong(cszIP);
  pCMD->m_CFlag  = 0;

  if (nFlag & DL_DEV_PIC) {
    pCMD->m_CFlag |= F_DL_PIC;
  }

  memcpy(pCMD + 1, &cRange, sizeof(DEV_DL_RECORD_RANGE));
  nLen = sizeof(NCMD) + sizeof(RECORDRANGE);
}

void DevLog::ProcessNCMD(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  assert(pCMD);
  DEV_LOG Log ;
  memset(&Log, 0x0, sizeof(DEV_LOG));

  if (0 == GET_ERR(pCMD->m_CFlag)) {
    DSPLOG* pRecord = (DSPLOG*)(pCMD + 1);
    Log.m_LogType   = DEV_LOG_TYPE(pRecord->m_LogType);
    Log.m_bRTLog    = (pCMD->m_SCmd == CMD_S_REALTIME_SEND) ? TRUE : FALSE;
    memcpy(Log.m_Admin.m_ID, pRecord->m_Admin.m_ID, sizeof(CID));
    memcpy(Log.m_BeOptUser.m_ID, pRecord->m_BeOptUser.m_ID, sizeof(CID));
    memcpy(Log.m_SerialID.m_ID, pRecord->m_SerialID.m_ID, sizeof(CID));
    memcpy(&(Log.m_RecTime), &(pRecord->m_RecTime), sizeof(DATETIME));
  }
  else if (OPER_ERR_BUSY != GET_ERR(pCMD->m_CFlag)) {
    memcpy(Log.m_SerialID.m_ID, pCMD + 1, sizeof(CID));
  }

  OnDevCmd(cszIP, NULL, DEV_LOG_OPER, DEV_OPER_DOWNLOAD, &Log, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
}