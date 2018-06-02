// DevOpenDoorCommand.cpp: implementation of the DevOpenDoorCommand class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "DevOpenDoor.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
REG_OBJECT(CMD_M_EOPEN, DevEOpenDoor, false);
REG_OBJECT(CMD_M_REMOTE_OPEN, DevROpenDoor, false);
REG_OBJECT(CMD_M_NOPEN, DevNOpenDoor, false);
REG_OBJECT(CMD_M_IOCTRL, DevIOCtrl, false);

DevEOpenDoor::DevEOpenDoor(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  ProcessNCMD(cszIP, pCMD, bServer, pvClientData);
}

DevEOpenDoor::DevEOpenDoor(const char* cszIP, DEV_DOOR_STATE nStatus, int nSeq, char* pbBuf, unsigned int& nLen)
{
  assert(pbBuf);
  NCMD* pCMD     = (NCMD*)pbBuf;
  pCMD->m_MCmd   = CMD_M_EOPEN;
  pCMD->m_SCmd   = CMD_S_SET ;
  pCMD->m_CFlag  = nStatus;
  pCMD->m_Param1 = nSeq;
  pCMD->m_Param2 = DotIpToLong(cszIP);
  nLen = sizeof(NCMD);
}

DevEOpenDoor::DevEOpenDoor(const char* cszIP, int nSeq, char* pbBuf, unsigned int& nLen)
{
  assert(pbBuf);
  NCMD* pCMD     = (NCMD*)pbBuf;
  pCMD->m_MCmd   = CMD_M_EOPEN;
  pCMD->m_SCmd   = CMD_S_GET ;
  pCMD->m_CFlag  = 0;
  pCMD->m_Param1 = nSeq;
  pCMD->m_Param2 = DotIpToLong(cszIP);
  nLen = sizeof(NCMD);
}

void DevEOpenDoor::ProcessNCMD(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  if (pCMD->m_SCmd == CMD_S_SET) {
    OnDevCmd(cszIP, NULL, DEV_DOOR_STATE_OPER, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
  }
  else if (pCMD->m_SCmd == CMD_S_GET) {
    DEV_DOOR_STATE nStatus = DEV_DOOR_NOMAL;

    if (pCMD->m_CFlag & DEV_DOOR_OPEN) {
      nStatus = DEV_DOOR_OPEN;
    }
    else if (pCMD->m_CFlag & DEV_DOOR_CLOSE) {
      nStatus = DEV_DOOR_CLOSE;
    }
    else {
      nStatus = DEV_DOOR_NOMAL;
    }

    OnDevCmd(cszIP, NULL, DEV_DOOR_STATE_OPER, DEV_OPER_DOWNLOAD, &nStatus, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
  }
}

//-------------------------------------------------

DevROpenDoor::DevROpenDoor(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  ProcessNCMD(cszIP, pCMD, bServer, pvClientData);
}

DevROpenDoor::DevROpenDoor(const char* cszIP, BOOL bOpen, int nSeq, char* pbBuf, unsigned int& nLen)
{
  assert(pbBuf);
  NCMD* pCMD     = (NCMD*)pbBuf;
  pCMD->m_MCmd   = CMD_M_REMOTE_OPEN;
  pCMD->m_SCmd   = CMD_S_SET ;
  pCMD->m_CFlag  = bOpen ? F_DOOR_OPEN : F_DOOR_CLOSE;
  pCMD->m_Param1 = nSeq;
  pCMD->m_Param2 = DotIpToLong(cszIP);
  nLen = sizeof(NCMD);
}

void DevROpenDoor::ProcessNCMD(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  OnDevCmd(cszIP, NULL, DEV_REMOTE_OPEN_DOOR, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
}

//-------------------------------------------------
DevIOCtrl::DevIOCtrl(const char* cszDevAddr, const DEV_IOCTRL& ioctrl, int nSeq, char* pbByf, unsigned int& nLen)
{
  assert(pbByf);
  NCMD* pCMD     = (NCMD*)pbByf;
  pCMD->m_MCmd   = CMD_M_IOCTRL;
  pCMD->m_SCmd   = CMD_S_SET ;
  pCMD->m_Param1 = nSeq;
  pCMD->m_Param2 = DotIpToLong(cszDevAddr);
  memcpy(pCMD + 1, &ioctrl, sizeof(ioctrl));
  nLen = sizeof(NCMD) + sizeof(DEV_IOCTRL);
}

DevIOCtrl::DevIOCtrl(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  ProcessNCMD(cszIP, pCMD, bServer, pvClientData);
}

void DevIOCtrl::ProcessNCMD(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  OnDevCmd(cszIP, NULL, DEV_IOCTRL_OPER, DEV_OPER_DOWNLOAD, (DEV_IOCTRL*)(pCMD + 1), pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
}

//-------------------------------------------------

DevNOpenDoor::DevNOpenDoor(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  ProcessNCMD(cszIP, pCMD, bServer, pvClientData);
}

void DevNOpenDoor::ProcessNCMD(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  OnDevCmd(cszIP, NULL, DEV_NOPEN_OPER, DEV_OPER_DOWNLOAD, (DEV_NOPEN*)(pCMD + 1), pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
}