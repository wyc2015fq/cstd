// DevInfoCommand.cpp: implementation of the DevInfoCommand class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "DevBaseInfo.h"

REG_OBJECT(CMD_M_DEVINFO, DevInfo  , false);
REG_OBJECT(CMD_M_REBOOT , DevReboot, false);
REG_OBJECT(CMD_M_NETPRO , CNetPro  , false);

DevInfo::DevInfo(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  ProcessNCMD(cszIP, pCMD, bServer, pvClientData);
}

DevInfo::DevInfo(const char* cszIP, int nSeq, char* pbBuf, unsigned int& nLen)
{
  assert(pbBuf);
  NCMD* pCMD     = (NCMD*)pbBuf;
  pCMD->m_MCmd   = CMD_M_DEVINFO;
  pCMD->m_SCmd   = CMD_S_GET ;
  pCMD->m_Param1 = nSeq;
  pCMD->m_Param2 = DotIpToLong(cszIP);
  pCMD->m_CFlag  = 0;
  nLen = sizeof(NCMD);
}

void DevInfo::ProcessNCMD(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  OnDevCmd(cszIP, NULL,
      DEV_BASEINFO_OPER, DEV_OPER_DOWNLOAD,
      (DEV_BASEINFO*)(pCMD + 1), pCMD->m_Param1,
      GET_ERR(pCMD->m_CFlag), pvClientData);
}

//***************************************************************************

DevReboot::DevReboot(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  ProcessNCMD(cszIP, pCMD, bServer, pvClientData);
}

DevReboot::DevReboot(const char* cszIP, int nSeq, char* pbBuf, unsigned int& nLen)
{
  assert(pbBuf);
  NCMD* pCMD     = (NCMD*)pbBuf;
  pCMD->m_MCmd   = CMD_M_REBOOT;
  pCMD->m_SCmd   = CMD_S_SET ;
  pCMD->m_Param1 = nSeq;
  pCMD->m_Param2 = DotIpToLong(cszIP);
  pCMD->m_CFlag  = 0;
  nLen = sizeof(NCMD);
}

void DevReboot::ProcessNCMD(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  OnDevCmd(cszIP, NULL, DEV_REBOOT_OPER, DEV_OPER_DOWNLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
}

//***************************************************************************

CNetPro::CNetPro(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  ProcessNCMD(cszIP, pCMD, bServer, pvClientData);
}

CNetPro::CNetPro(const char* cszIP, DEV_NETPRO* pNetPro, int nSeq  , char* pbBuf , unsigned int& nLen)
{
  assert(pbBuf);

  NCMD* pCMD     = (NCMD*)pbBuf;
  pCMD->m_MCmd   = CMD_M_NETPRO;
  pCMD->m_SCmd   = pNetPro ? CMD_S_SET : CMD_S_GET;
  pCMD->m_Param1 = nSeq;
  pCMD->m_Param2 = DotIpToLong(cszIP);
  pCMD->m_CFlag  = 0;

  if (pNetPro) {
    memcpy(pCMD + 1, pNetPro, sizeof(DEV_NETPRO));
    nLen = sizeof(NCMD) + sizeof(DEV_NETPRO);
  }
  else {
    nLen = sizeof(NCMD);
  }
}

void CNetPro::ProcessNCMD(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  OnDevCmd(cszIP, NULL,
      DEV_NETPRO_OPER, DEV_OPER_DOWNLOAD,
      (DEV_NETPRO*)(pCMD + 1), pCMD->m_Param1,
      GET_ERR(pCMD->m_CFlag), pvClientData);
}

