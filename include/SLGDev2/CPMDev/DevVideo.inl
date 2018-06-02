// DevVideo.cpp: implementation of the DevVideo class.
//
#include "DevVideo.h"
REG_OBJECT(CMD_M_VIDEO_OUT, DevVideo, true);
DevVideo::DevVideo(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  ProcessNCMD(s, pCMD, bServer, pvClientData);
}
DevVideo::DevVideo(const CSock* s, const char* cszVideoIP, unsigned short VideoPort, int nSeq, char* pbBuf, unsigned int& nLen)//ÉèÖÃ
{
  NCMD* pCMD = (NCMD*)pbBuf;
  s->setNCMD(pCMD, CMD_M_VIDEO_OUT, CMD_S_SET, 0, nSeq);
  SOCK_ADDRESS Addr = {0};
  strncpy(Addr.IP_Address, cszVideoIP, sizeof(Addr.IP_Address) - 1);
  Addr.Port = VideoPort;
  memcpy(pCMD + 1, &Addr, sizeof(SOCK_ADDRESS));
  nLen = sizeof(NCMD) + sizeof(SOCK_ADDRESS);
}
DevVideo::DevVideo(const CSock* s, int nSeq, char* pbBuf, unsigned int& nLen)//Í£Ö¹
{
  NCMD* pCMD = (NCMD*)pbBuf;
  s->setNCMD(pCMD, CMD_M_VIDEO_OUT, CMD_S_GET, 0, nSeq);
  nLen = sizeof(NCMD);
}
void DevVideo::ProcessNCMD(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  switch (pCMD->m_SCmd) {
  case CMD_S_SET:
    StartVideo(s, pCMD, bServer, pvClientData);
    break;

  case CMD_S_GET:
    StopVideo(s, pCMD, bServer, pvClientData);
    break;

  default:
    break;
  }
}
void DevVideo::StartVideo(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  OnDevCmd(s, NULL, DEV_VIDEO_TRANSFER, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
}
void DevVideo::StopVideo(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  OnDevCmd(s, NULL, DEV_VIDEO_TRANSFER, DEV_OPER_DOWNLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
}

