// DevRightCommand.cpp: implementation of the DevRightCommand class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "DevRight.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
REG_OBJECT(CMD_M_DSPRIGHT, DevRight, false);

DevRight::DevRight(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  ProcessNCMD(cszIP, pCMD, bServer, pvClientData);
}

DevRight::DevRight(const char* cszIP, const DEV_RIGHT_ARRAY& cArray, DEV_OPER_FLAG eFlag, int nSeq, char* pbBuf, unsigned int& nLen)
{
  assert(pbBuf);
  NCMD* pCMD = (NCMD*)pbBuf;
  pCMD->m_MCmd = CMD_M_DSPRIGHT;

  switch (eFlag) {
  case DEV_OPER_UPLOAD:
    pCMD->m_SCmd = CMD_S_SET;
    break;

  case DEV_OPER_DOWNLOAD:
    pCMD->m_SCmd = CMD_S_GET;
    break;

  case DEV_OPER_DELETE:
    pCMD->m_SCmd = CMD_S_DEL;
    break;
  }

  pCMD->m_Param1 = nSeq;
  pCMD->m_Param2 = DotIpToLong(cszIP);
  pCMD->m_CFlag  = 0;

  CARRAY* pArray  = (CARRAY*)(pCMD + 1);
  DSPRIGHT* pRG   = (DSPRIGHT*)(pArray->m_Block);
  pArray->m_Count = cArray.m_nCount;

  for (int i = 0; i < pArray->m_Count; ++i) {
    pRG[i].m_RightID = (cArray.m_pRtArray)[i].m_RightID;
    memcpy(pRG[i].m_TimeGID, (cArray.m_pRtArray)[i].m_TimeGID, sizeof(pRG[i].m_TimeGID));
    pRG[i].m_Mode  = cArray.m_pRtArray[i].m_bHolidyValid  ? CHECK_HOLIDY : 0;
    pRG[i].m_Mode |= cArray.m_pRtArray[i].m_bActionLock   ? ACTION_LOCK  : 0;
    pRG[i].m_Mode |= cArray.m_pRtArray[i].m_bActionOutPut ? ACTION_OUTPUT : 0;
  }

  nLen = sizeof(NCMD) + sizeof(CARRAY) + sizeof(DSPRIGHT) * (pArray->m_Count);
}

void DevRight::ProcessNCMD(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  assert(pCMD);

  switch (pCMD->m_SCmd) {
  case CMD_S_SET:
    RightAdd(cszIP, pCMD, bServer, pvClientData);
    break;

  case CMD_S_GET:
    RightDown(cszIP, pCMD, bServer, pvClientData);
    break;

  case CMD_S_DEL:
    RightDel(cszIP, pCMD, bServer, pvClientData);
    break;
  }
}

void DevRight::RightDel(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  OnDevCmd(cszIP, NULL, DEV_RIGHT_OPER, DEV_OPER_DELETE, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
}

void DevRight::RightAdd(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  OnDevCmd(cszIP, NULL, DEV_RIGHT_OPER, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
}

void DevRight::RightDown(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
#define TG_MAX_SIZE 256
  DEV_RIGHT_ARRAY r_array   = {0};
  DEV_RIGHT RG[TG_MAX_SIZE] = {0};

  if (0 == GET_ERR(pCMD->m_CFlag)) {
    CARRAY* pArray = (CARRAY*)(pCMD + 1);
    DSPRIGHT* pRG  = (DSPRIGHT*)(pArray->m_Block);
    r_array.m_nCount   = pArray->m_Count > TG_MAX_SIZE ? TG_MAX_SIZE : pArray->m_Count;
    r_array.m_pRtArray = RG;
    assert(pArray->m_Count <= TG_MAX_SIZE);

    for (int i = 0; i < r_array.m_nCount; ++i) {
      RG[i].m_RightID = pRG[i].m_RightID;
      memcpy(RG[i].m_TimeGID,  pRG[i].m_TimeGID, sizeof(pRG[i].m_TimeGID));
      RG[i].m_bHolidyValid  = (BOOL)(pRG[i].m_Mode & CHECK_HOLIDY);
      RG[i].m_bActionLock   = (BOOL)(pRG[i].m_Mode & ACTION_LOCK);
      RG[i].m_bActionOutPut = (BOOL)(pRG[i].m_Mode & ACTION_OUTPUT);
    }
  }

  OnDevCmd(cszIP, NULL, DEV_RIGHT_OPER, DEV_OPER_DOWNLOAD, &r_array, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
}