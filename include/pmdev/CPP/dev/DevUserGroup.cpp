// DevUserGroupCmd.cpp: implementation of the DevUserGroup class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "DevUserGroup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

REG_OBJECT(CMD_M_UGROUP, DevUserGroup, false);

DevUserGroup::DevUserGroup(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  ProcessNCMD(cszIP, pCMD, bServer, pvClientData);
}

DevUserGroup::DevUserGroup(const char* cszIP, const DEV_USERGROUP_ARRAY& cArray, DEV_OPER_FLAG eFlag, int nSeq, char* pbBuf, unsigned int& nLen)
{
  assert(pbBuf);
  NCMD* pCMD = (NCMD*)pbBuf;
  pCMD->m_MCmd = CMD_M_UGROUP;

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

  CARRAY* pArray = (CARRAY*)(pCMD + 1);
  pArray->m_Count = cArray.m_nCount;
  USERGROUP* pUG = (USERGROUP*)(pArray->m_Block);
  DEV_USERGROUP* pTmp = cArray.m_pUGArray;

  for (int i = 0; i < pArray->m_Count; ++i) {
    pUG->m_GroupID     = pTmp->m_GroupID;
    pUG->m_GroupFlag   = pTmp->m_bGroupOrder ;
    pUG->m_NormalValid = pTmp->m_NormalValid;
    pUG->m_ForceValid  = pTmp->m_ForceValid;
    memcpy(pUG->m_NormalUsers, pTmp->m_NormalUsers, sizeof(pUG->m_NormalUsers));
    memcpy(pUG->m_ForceUsers, pTmp->m_ForceUsers, sizeof(pUG->m_ForceUsers));
    pUG++;
    pTmp++;
  }

  nLen = sizeof(NCMD) + sizeof(CARRAY) + sizeof(USERGROUP) * (pArray->m_Count);
}

void DevUserGroup::ProcessNCMD(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  assert(pCMD);

  switch (pCMD->m_SCmd) {
  case CMD_S_SET:
    UserGroupAdd(cszIP, pCMD, bServer, pvClientData);
    break;

  case CMD_S_GET:
    UserGroupDown(cszIP, pCMD, bServer, pvClientData);
    break;

  case CMD_S_DEL:
    UserGroupDel(cszIP, pCMD, bServer, pvClientData);
    break;
  }
}

void DevUserGroup::UserGroupDel(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  OnDevCmd(cszIP, NULL, DEV_RIGHT_OPER, DEV_OPER_DELETE, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
}

void DevUserGroup::UserGroupAdd(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  OnDevCmd(cszIP, NULL, DEV_USERGROUP_OPER, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
}

void DevUserGroup::UserGroupDown(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
#define TG_MAX_SIZE 256
  DEV_USERGROUP_ARRAY u_array   = {0};
  DEV_USERGROUP UG[TG_MAX_SIZE] = {0};

  if (OPER_SUCCESS == GET_ERR(pCMD->m_CFlag)) {
    CARRAY* pArray = (CARRAY*)(pCMD + 1);
    assert(pArray->m_Count <= TG_MAX_SIZE);
    USERGROUP* pUG = (USERGROUP*)(pArray->m_Block);
    u_array.m_nCount = pArray->m_Count > TG_MAX_SIZE ? TG_MAX_SIZE : pArray->m_Count;
    u_array.m_pUGArray = UG;

    for (int i = 0; i < u_array.m_nCount; ++i) {
      UG[i].m_GroupID     = pUG[i].m_GroupID;
      UG[i].m_bGroupOrder = pUG[i].m_GroupFlag ;
      UG[i].m_NormalValid = pUG[i].m_NormalValid;
      UG[i].m_ForceValid  = pUG[i].m_ForceValid;
      memcpy(UG[i].m_NormalUsers, pUG[i].m_NormalUsers, sizeof(pUG[i].m_NormalUsers));
      memcpy(UG[i].m_ForceUsers, pUG[i].m_ForceUsers, sizeof(pUG[i].m_ForceUsers));
    }
  }

  OnDevCmd(cszIP, NULL, DEV_USERGROUP_OPER, DEV_OPER_DOWNLOAD, &u_array, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
}