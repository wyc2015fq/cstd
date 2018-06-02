// DevOpenDoorCommand.cpp: implementation of the DevOpenDoorCommand class.
//
#include "DevOpenDoor.h"
REG_OBJECT(CMD_M_EOPEN, DevEOpenDoor, false);
REG_OBJECT(CMD_M_REMOTE_OPEN, DevROpenDoor, false);
REG_OBJECT(CMD_M_NOPEN, DevNOpenDoor, false);
REG_OBJECT(CMD_M_IOCTRL, DevIOCtrl, false);
DevEOpenDoor::DevEOpenDoor(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  ProcessNCMD(s, pCMD, bServer, pvClientData);
}
DevEOpenDoor::DevEOpenDoor(const CSock* s, DEV_DOOR_STATE nStatus, int nSeq, char* pbBuf, unsigned int& nLen)
{
  assert(pbBuf);
  NCMD* pCMD = (NCMD*)pbBuf;
  s->setNCMD(pCMD, CMD_M_EOPEN, CMD_S_SET, nStatus, nSeq);
  nLen = sizeof(NCMD);
}
DevEOpenDoor::DevEOpenDoor(const CSock* s, int nSeq, char* pbBuf, unsigned int& nLen)
{
  assert(pbBuf);
  NCMD* pCMD = (NCMD*)pbBuf;
  s->setNCMD(pCMD, CMD_M_EOPEN, CMD_S_GET, 0, nSeq);
  nLen = sizeof(NCMD);
}
void DevEOpenDoor::ProcessNCMD(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  if (pCMD->m_SCmd == CMD_S_SET) {
    OnDevCmd(s, NULL, DEV_DOOR_STATE_OPER, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
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

    OnDevCmd(s, NULL, DEV_DOOR_STATE_OPER, DEV_OPER_DOWNLOAD, &nStatus, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
  }
}
//-------------------------------------------------
DevROpenDoor::DevROpenDoor(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  ProcessNCMD(s, pCMD, bServer, pvClientData);
}
DevROpenDoor::DevROpenDoor(const CSock* s, BOOL bOpen, int nSeq, char* pbBuf, unsigned int& nLen)
{
  assert(pbBuf);
  NCMD* pCMD = (NCMD*)pbBuf;
  s->setNCMD(pCMD, CMD_M_REMOTE_OPEN, CMD_S_SET, bOpen ? F_DOOR_OPEN : F_DOOR_CLOSE, nSeq);
  nLen = sizeof(NCMD);
}
void DevROpenDoor::ProcessNCMD(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  OnDevCmd(s, NULL, DEV_REMOTE_OPEN_DOOR, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
}
//-------------------------------------------------
DevIOCtrl::DevIOCtrl(const CSock* s, const DEV_IOCTRL& ioctrl, int nSeq, char* pbByf, unsigned int& nLen)
{
  assert(pbByf);
  NCMD* pCMD = (NCMD*)pbByf;
  s->setNCMD(pCMD, CMD_M_IOCTRL, CMD_S_SET, 0, nSeq);
  memcpy(pCMD + 1, &ioctrl, sizeof(ioctrl));
  nLen = sizeof(NCMD) + sizeof(DEV_IOCTRL);
}
DevIOCtrl::DevIOCtrl(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  ProcessNCMD(s, pCMD, bServer, pvClientData);
}
void DevIOCtrl::ProcessNCMD(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  OnDevCmd(s, NULL, DEV_IOCTRL_OPER, DEV_OPER_DOWNLOAD, (DEV_IOCTRL*)(pCMD + 1), pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
}
//-------------------------------------------------
DevNOpenDoor::DevNOpenDoor(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  ProcessNCMD(s, pCMD, bServer, pvClientData);
}
void DevNOpenDoor::ProcessNCMD(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  OnDevCmd(s, NULL, DEV_NOPEN_OPER, DEV_OPER_DOWNLOAD, (DEV_NOPEN*)(pCMD + 1), pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
}

