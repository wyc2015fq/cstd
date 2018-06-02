// DevTimeGroupCommand.cpp: implementation of the DevTimeGroupCommand class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "DevTimeGroup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
REG_OBJECT(CMD_M_TGROUP, DevTimeGroup, false);


DevTimeGroup::DevTimeGroup(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  ProcessNCMD(cszIP, pCMD, bServer, pvClientData);
}

DevTimeGroup::DevTimeGroup(const char* cszIP, const DEV_TIMEGROUP_ARRAY& cArray, int nType, DEV_OPER_FLAG eFlag, int nSeq, char* pbBuf, unsigned int& nLen)
{
  assert(pbBuf);
  NCMD* pCMD   = (NCMD*)pbBuf;
  pCMD->m_MCmd = CMD_M_TGROUP;

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

  switch (nType) {
  case DEV_NORMAL:
    pCMD->m_CFlag = F_DL_NORMAL_TG;
    break;

  case DEV_HOLIDY:
    pCMD->m_CFlag = F_DL_HOLIDAY_TG;
    break;

  case DEV_ANY_TIME:
    pCMD->m_CFlag = F_DL_NORMAL_TG | F_DL_HOLIDAY_TG;
    break;

  default:
    break;
  }

  pCMD->m_Param1 = nSeq;
  pCMD->m_Param2 = DotIpToLong(cszIP);

  CARRAY* pArray = (CARRAY*)(pCMD + 1);
  pArray->m_Count = cArray.m_nCount;
  DSPTIMEG* pTG  = (DSPTIMEG*)(pArray->m_Block);

  for (int i = 0; i < pArray->m_Count; ++i) {
    pTG[i].m_TGID = (cArray.m_pTGArray)[i].m_TGID;
    memcpy(&(pTG[i].m_Start), &((cArray.m_pTGArray)[i].m_Start), sizeof(DATETIME));
    memcpy(&(pTG[i].m_End), &((cArray.m_pTGArray)[i].m_End), sizeof(DATETIME));
    pTG[i].m_TGType    = (cArray.m_pTGArray)[i].m_TGType;
    pTG[i].m_CheckFlag = (cArray.m_pTGArray)[i].m_CheckFlag;
    pTG[i].m_WeekFlag  = (cArray.m_pTGArray)[i].m_WeekFlag;
  }

  nLen = sizeof(NCMD) + sizeof(CARRAY) + sizeof(DSPTIMEG) * (pArray->m_Count);
}

void DevTimeGroup::ProcessNCMD(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  assert(pCMD);

  switch (pCMD->m_SCmd) {
  case CMD_S_SET:
    TimeGroupAdd(cszIP, pCMD, bServer, pvClientData);
    break;

  case CMD_S_GET:
    TimeGroupDown(cszIP, pCMD, bServer, pvClientData);
    break;

  case CMD_S_DEL:
    TimeGroupDel(cszIP, pCMD, bServer, pvClientData);
    break;
  }
}

void DevTimeGroup::TimeGroupDel(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  OnDevCmd(cszIP, NULL, DEV_TIME_GROUP_OPER, DEV_OPER_DELETE, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
}

void DevTimeGroup::TimeGroupAdd(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  OnDevCmd(cszIP, NULL, DEV_TIME_GROUP_OPER, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
}

void DevTimeGroup::TimeGroupDown(const char* cszIP, NCMD* pCMD, BOOL isServer, void* pvClientData)
{
#define TG_MAX_SIZE 256

  if (OPER_SUCCESS == GET_ERR(pCMD->m_CFlag)) {
    CARRAY* pArray = (CARRAY*)(pCMD + 1);
    assert(pArray->m_Count <= TG_MAX_SIZE);
    DSPTIMEG* pTG = (DSPTIMEG*)(pArray->m_Block);
    DEV_TIMEGROUP_ARRAY t_array = {0};
    DEV_TIMEGROUP TG[TG_MAX_SIZE] = {0};
    t_array.m_nCount   = pArray->m_Count > TG_MAX_SIZE ? TG_MAX_SIZE : pArray->m_Count;
    t_array.m_pTGArray = TG;

    for (int i = 0; i < t_array.m_nCount; ++i) {
      TG[i].m_TGID = pTG[i].m_TGID;
      memcpy(&(TG[i].m_Start), &(pTG[i].m_Start), sizeof(DATETIME));
      memcpy(&(TG[i].m_End), &(pTG[i].m_End), sizeof(DATETIME));
      TG[i].m_TGType    = DEV_TIMEGROUP_TYPE(pTG[i].m_TGType);
      TG[i].m_CheckFlag = pTG[i].m_CheckFlag;
      TG[i].m_WeekFlag  = pTG[i].m_WeekFlag;
    }

    OnDevCmd(cszIP, NULL, DEV_TIME_GROUP_OPER, DEV_OPER_DOWNLOAD, &t_array, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
  }
  else {
    OnDevCmd(cszIP, NULL, DEV_TIME_GROUP_OPER, DEV_OPER_DOWNLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
  }

#undef TG_MAX_SIZE
}