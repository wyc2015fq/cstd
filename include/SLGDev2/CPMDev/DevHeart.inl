// DevHeartCommand.cpp: implementation of the DevHeartCommand class.
//
#include "DevHeart.h"
REG_OBJECT(CMD_M_HEART, DevHeart, false);
DevHeart::DevHeart(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  ProcessNCMD(s, pCMD, bServer, pvClientData);
}
void DevHeart::ProcessNCMD(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD), 4) >> 2] = {0};
  NCMD* pTMP = (NCMD*)((char*)nData + OFFSET_SIZE);
  s->setNCMD(pTMP, CMD_M_HEART, CMD_S_SET);
  unsigned int uLen = sizeof(NCMD);
  s->Send((char*)pTMP, &uLen);
}

