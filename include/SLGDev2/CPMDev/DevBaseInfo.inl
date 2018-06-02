// DevInfoCommand.cpp: implementation of the DevInfoCommand class.
//
#include "DevBaseInfo.h"
REG_OBJECT(CMD_M_DEVINFO, DevInfo , false);
REG_OBJECT(CMD_M_REBOOT , DevReboot, false);
REG_OBJECT(CMD_M_NETPRO , CNetPro , false);
DevInfo::DevInfo(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  ProcessNCMD(s, pCMD, bServer, pvClientData);
}
DevInfo::DevInfo(const CSock* s, int nSeq, char* pbBuf, unsigned int& nLen)
{
  assert(pbBuf);
  NCMD* pCMD = (NCMD*)pbBuf;
  s->setNCMD(pCMD, CMD_M_DEVINFO, CMD_S_GET, 0, nSeq);
  nLen = sizeof(NCMD);
}
void DevInfo::ProcessNCMD(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  OnDevCmd(s, NULL,
      DEV_BASEINFO_OPER, DEV_OPER_DOWNLOAD,
      (DEV_BASEINFO*)(pCMD + 1), pCMD->m_Param1,
      GET_ERR(pCMD->m_CFlag), pvClientData);
}
/////////////////////////////////////////////////////////////////////////////
DevReboot::DevReboot(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  ProcessNCMD(s, pCMD, bServer, pvClientData);
}
DevReboot::DevReboot(const CSock* s, int nSeq, char* pbBuf, unsigned int& nLen)
{
  assert(pbBuf);
  NCMD* pCMD = (NCMD*)pbBuf;
  s->setNCMD(pCMD, CMD_M_REBOOT, CMD_S_SET, 0, nSeq);
  nLen = sizeof(NCMD);
}
void DevReboot::ProcessNCMD(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  OnDevCmd(s, NULL, DEV_REBOOT_OPER, DEV_OPER_DOWNLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
}
/////////////////////////////////////////////////////////////////////////////
CResetScreen::CResetScreen(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  ProcessNCMD(s, pCMD, bServer, pvClientData);
}
CResetScreen::CResetScreen(const CSock* s, int nSeq, char* pbBuf, unsigned int& nLen)
{
  assert(pbBuf);
  NCMD* pCMD = (NCMD*)pbBuf;
  s->setNCMD(pCMD, CMD_M_RESETSCREEN, CMD_S_SET, 0, nSeq);
  nLen = sizeof(NCMD);
}
void CResetScreen::ProcessNCMD(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  OnDevCmd(s, NULL, DEV_SCREEN_OPER, DEV_OPER_DOWNLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
}
/////////////////////////////////////////////////////////////////////////////
CNetPro::CNetPro(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  ProcessNCMD(s, pCMD, bServer, pvClientData);
}
CNetPro::CNetPro(const CSock* s, DEV_NETPRO* pNetPro, int nSeq , char* pbBuf , unsigned int& nLen)
{
  assert(pbBuf);
  NCMD* pCMD = (NCMD*)pbBuf;
  s->setNCMD(pCMD, CMD_M_NETPRO, pNetPro ? CMD_S_SET : CMD_S_GET, 0, nSeq);

  if (pNetPro) {
    memcpy(pCMD + 1, pNetPro, sizeof(DEV_NETPRO));
    nLen = sizeof(NCMD) + sizeof(DEV_NETPRO);
  }
  else {
    nLen = sizeof(NCMD);
  }
}
void CNetPro::ProcessNCMD(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  OnDevCmd(s, NULL,
      DEV_NETPRO_OPER, DEV_OPER_DOWNLOAD,
      (DEV_NETPRO*)(pCMD + 1), pCMD->m_Param1,
      GET_ERR(pCMD->m_CFlag), pvClientData);
}

