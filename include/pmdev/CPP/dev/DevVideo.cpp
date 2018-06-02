// DevVideo.cpp: implementation of the DevVideo class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "DevVideo.h"
#include "../Net/sock.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
REG_OBJECT(CMD_M_VIDEO_OUT, DevVideo, true);

DevVideo::DevVideo(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  ProcessNCMD(cszIP, pCMD, bServer, pvClientData);
}

DevVideo::DevVideo(const char* cszIP, const char* cszVideoIP, unsigned short VideoPort, /*long hwnd,*/ int nSeq, char* pbBuf, unsigned int& nLen)//ÉèÖÃ
{
  NCMD* pCMD     = (NCMD*)pbBuf;
  pCMD->m_MCmd   = CMD_M_VIDEO_OUT;
  pCMD->m_SCmd   = CMD_S_SET;
  pCMD->m_Param1 = nSeq;
  pCMD->m_Param2 = DotIpToLong(cszIP);
  pCMD->m_CFlag  = 0;
  SOCK_ADDRESS Addr = {0};
  strncpy(Addr.IP_Address, cszVideoIP, sizeof(Addr.IP_Address) - 1);
  Addr.Port = VideoPort;
  memcpy(pCMD + 1, &Addr, sizeof(SOCK_ADDRESS));
  nLen = sizeof(NCMD) + sizeof(SOCK_ADDRESS);
}

DevVideo::DevVideo(const char* cszIP, int nSeq, char* pbBuf, unsigned int& nLen)//Í£Ö¹
{
  NCMD* pCMD     = (NCMD*)pbBuf;
  pCMD->m_MCmd   = CMD_M_VIDEO_OUT;
  pCMD->m_SCmd   = CMD_S_GET;
  pCMD->m_Param1 = nSeq;
  pCMD->m_Param2 = DotIpToLong(cszIP);
  pCMD->m_CFlag  = 0;
  nLen = sizeof(NCMD);
}

void DevVideo::ProcessNCMD(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  switch (pCMD->m_SCmd) {
  case CMD_S_SET:
    StartVideo(cszIP, pCMD, bServer, pvClientData);
    break;

  case CMD_S_GET:
    StopVideo(cszIP, pCMD, bServer, pvClientData);
    break;

  default:
    break;
  }
}

void DevVideo::StartVideo(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  OnDevCmd(cszIP, NULL, DEV_VIDEO_TRANSFER, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
}

void DevVideo::StopVideo(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  OnDevCmd(cszIP, NULL, DEV_VIDEO_TRANSFER, DEV_OPER_DOWNLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
}
