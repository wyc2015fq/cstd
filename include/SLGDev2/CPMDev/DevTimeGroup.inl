// DevTimeGroupCommand.cpp: implementation of the DevTimeGroupCommand class.
//
#include "DevTimeGroup.h"
REG_OBJECT(CMD_M_TGROUP, DevTimeGroup, false);
DevTimeGroup::DevTimeGroup(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  ProcessNCMD(s, pCMD, bServer, pvClientData);
}
DevTimeGroup::DevTimeGroup(const CSock* s, const DEV_TIMEGROUP_ARRAY& cArray, int nType, DEV_OPER_FLAG eFlag, int nSeq, char* pbBuf, unsigned int& nLen)
{
  assert(pbBuf);
  NCMD* pCMD = (NCMD*)pbBuf;
  int m_SCmd = 0, m_CFlag = 0;

  switch (eFlag) {
  case DEV_OPER_UPLOAD:
    m_SCmd = CMD_S_SET;
    break;

  case DEV_OPER_DOWNLOAD:
    m_SCmd = CMD_S_GET;
    break;

  case DEV_OPER_DELETE:
    m_SCmd = CMD_S_DEL;
    break;
  }

  switch (nType) {
  case DEV_NORMAL:
    m_CFlag = F_DL_NORMAL_TG;
    break;

  case DEV_HOLIDY:
    m_CFlag = F_DL_HOLIDAY_TG;
    break;

  case DEV_ANY_TIME:
    m_CFlag = F_DL_NORMAL_TG | F_DL_HOLIDAY_TG;
    break;

  default:
    break;
  }

  s->setNCMD(pCMD, CMD_M_TGROUP, m_SCmd, m_CFlag, nSeq);
  CARRAY* pArray = (CARRAY*)(pCMD + 1);
  pArray->m_Count = cArray.m_nCount;
  DSPTIMEG* pTG = (DSPTIMEG*)(pArray->m_Block);

  for (int i = 0; i < pArray->m_Count; ++i) {
    pTG[i].m_TGID = (cArray.m_pTGArray)[i].m_TGID;
    memcpy(&(pTG[i].m_Start), &((cArray.m_pTGArray)[i].m_Start), sizeof(DATETIME));
    memcpy(&(pTG[i].m_End), &((cArray.m_pTGArray)[i].m_End), sizeof(DATETIME));
    pTG[i].m_TGType = (cArray.m_pTGArray)[i].m_TGType;
    pTG[i].m_CheckFlag = (cArray.m_pTGArray)[i].m_CheckFlag;
    pTG[i].m_WeekFlag = (cArray.m_pTGArray)[i].m_WeekFlag;
  }

  nLen = sizeof(NCMD) + sizeof(CARRAY) + sizeof(DSPTIMEG) * (pArray->m_Count);
}
void DevTimeGroup::ProcessNCMD(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  assert(pCMD);

  switch (pCMD->m_SCmd) {
  case CMD_S_SET:
    TimeGroupAdd(s, pCMD, bServer, pvClientData);
    break;

  case CMD_S_GET:
    TimeGroupDown(s, pCMD, bServer, pvClientData);
    break;

  case CMD_S_DEL:
    TimeGroupDel(s, pCMD, bServer, pvClientData);
    break;
  }
}
void DevTimeGroup::TimeGroupDel(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  OnDevCmd(s, NULL, DEV_TIME_GROUP_OPER, DEV_OPER_DELETE, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
}
void DevTimeGroup::TimeGroupAdd(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  OnDevCmd(s, NULL, DEV_TIME_GROUP_OPER, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
}
void DevTimeGroup::TimeGroupDown(const CSock* s, NCMD* pCMD, BOOL isServer, void* pvClientData)
{
#define TG_MAX_SIZE 256

  if (OPER_SUCCESS == GET_ERR(pCMD->m_CFlag)) {
    CARRAY* pArray = (CARRAY*)(pCMD + 1);
    assert(pArray->m_Count <= TG_MAX_SIZE);
    DSPTIMEG* pTG = (DSPTIMEG*)(pArray->m_Block);
    DEV_TIMEGROUP_ARRAY t_array = {0};
    DEV_TIMEGROUP TG[TG_MAX_SIZE] = {0};
    t_array.m_nCount = pArray->m_Count > TG_MAX_SIZE ? TG_MAX_SIZE : pArray->m_Count;
    t_array.m_pTGArray = TG;

    for (int i = 0; i < t_array.m_nCount; ++i) {
      TG[i].m_TGID = pTG[i].m_TGID;
      memcpy(&(TG[i].m_Start), &(pTG[i].m_Start), sizeof(DATETIME));
      memcpy(&(TG[i].m_End), &(pTG[i].m_End), sizeof(DATETIME));
      TG[i].m_TGType = DEV_TIMEGROUP_TYPE(pTG[i].m_TGType);
      TG[i].m_CheckFlag = pTG[i].m_CheckFlag;
      TG[i].m_WeekFlag = pTG[i].m_WeekFlag;
    }

    OnDevCmd(s, NULL, DEV_TIME_GROUP_OPER, DEV_OPER_DOWNLOAD, &t_array, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
  }
  else {
    OnDevCmd(s, NULL, DEV_TIME_GROUP_OPER, DEV_OPER_DOWNLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
  }
}

