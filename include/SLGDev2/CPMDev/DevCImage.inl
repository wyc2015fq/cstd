// ChangeImageCmd.cpp: implementation of the DevChangeImage class.
//
#include "DevCImage.h"
REG_OBJECT(CMD_M_CIMAGE, DevCImage, true);
DevCImage::DevCImage(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  ProcessNCMD(s, pCMD, bServer, pvClientData);
}
DevCImage::DevCImage(const CSock* s, const DEV_DL_RECORD_RANGE& cRange, int nFlag, int nSeq, char* pbBuf, unsigned int& nLen)
{
  assert(pbBuf);
  NCMD* pCMD = (NCMD*)pbBuf;
  s->setNCMD(pCMD, CMD_M_CIMAGE, CMD_S_GET, 0, nSeq);

  if (nFlag & DL_DEV_PIC) {
    pCMD->m_CFlag |= F_DL_PIC;
  }

  memcpy(pCMD + 1, &cRange, sizeof(DEV_DL_RECORD_RANGE));
  nLen = sizeof(NCMD) + sizeof(RECORDRANGE);
}
void DevCImage::ProcessNCMD(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  assert(pCMD);
  DEV_CHANGEIMAGE Image = {0};

  if (SUC_EXECUT == GET_ERR(pCMD->m_CFlag)) {
    CHANGEIMAGE* pRecord = (CHANGEIMAGE*)(pCMD + 1);
    Image.m_PicBytes = pRecord->m_PicBytes;
    Image.m_bRTChangeImage = (pCMD->m_SCmd == CMD_S_REALTIME_SEND) ? TRUE : FALSE;
    memcpy(Image.m_UserID.m_ID, pRecord->m_UserID.m_ID, sizeof(CID));
    memcpy(Image.m_SerialID.m_ID, pRecord->m_SerialID.m_ID, sizeof(CID));
    memcpy(Image.m_AppendID.m_ID, pRecord->m_AppendID.m_ID, sizeof(CID));
    memcpy(Image.m_UserName.m_ID, pRecord->m_UserName.m_ID, sizeof(CID));
    memcpy(Image.m_Department.m_ID, pRecord->m_Department.m_ID, sizeof(CID));
    memcpy(Image.m_Admin.m_ID, pRecord->m_Admin.m_ID, sizeof(CID));
    memcpy(&(Image.m_RecTime), &(pRecord->m_RecTime), sizeof(DATETIME));
    Image.m_PicData = pRecord->m_Block;
  }
  else if (OPER_ERR_BUSY != GET_ERR(pCMD->m_CFlag)) {
    memcpy(&(Image.m_SerialID), pCMD + 1, sizeof(CID));
  }

  OnDevCmd(s, NULL, DEV_CHANGEIMAGE_OPER, DEV_OPER_DOWNLOAD, &Image, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
}

