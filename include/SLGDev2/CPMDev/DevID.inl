// DevIDCommand.cpp: implementation of the DevIDCommand class.
//
#include "DevID.h"
#include "DevUser.h"
#include "DevRecord.h"
#include "DevCImage.h"
#include "DevLog.h"
REG_OBJECT(CMD_M_ID, DevID, true);
DevID::DevID(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  ProcessNCMD(s, pCMD, bServer, pvClientData);
}
DevID::DevID(const CSock* s, int nClass, int nSeq, char* pbBuf, unsigned int& nLen) //下载所有
{
  assert(pbBuf);
  NCMD* pCMD = (NCMD*)pbBuf;
  s->setNCMD(pCMD, nClass, CMD_S_GET, F_DL_ID | F_DL_ID_ALL, nSeq);
  nLen = sizeof(NCMD);
}
DevID::DevID(const CSock* s, int nClass, const DEV_TIMESEG& cTimeSeg, int nSeq, char* pbBuf, unsigned int& nLen) //时间段下载
{
  NCMD* pCMD = (NCMD*)pbBuf;
  s->setNCMD(pCMD, nClass, CMD_S_GET, F_DL_ID | F_ID_TIME_RANGE, nSeq);
  memcpy(pCMD + 1, &cTimeSeg, sizeof(TIMESE));
  nLen = sizeof(NCMD) + sizeof(TIMESE);
}
DevID::DevID(const CSock* s, int nClass, int nFlag, const DEV_DL_RECORD_RANGE& cRange, int nSeq, char* pbBuf, unsigned int& nLen) //区间段下载
{
  NCMD* pCMD = (NCMD*)pbBuf;
  int m_CFlag = (nFlag & 0x80000000) ? 0 : (F_DL_ID | F_ID_RANGE);

  if (nFlag & DL_DEV_PIC) {
    m_CFlag |= F_DL_PIC;
  }

  s->setNCMD(pCMD, CMD_M_RANGE, CMD_S_GET, m_CFlag, nSeq);
  memcpy(pCMD + 1, &cRange, sizeof(RECORDRANGE));
  nLen = sizeof(NCMD) + sizeof(RECORDRANGE);
}
void DevID::ProcessNCMD(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  assert(pCMD);

  switch (pCMD->m_MCmd) {
  case CMD_M_USER:
    DlUserID(s, pCMD, bServer, pvClientData);
    break;

  case CMD_M_VERIFY:
  case CMD_M_ALARM:
  case CMD_M_OPTLOG:
  case CMD_M_CIMAGE:
  case CMD_M_RANGE:
    DlRecID(s, pCMD, bServer, pvClientData);
    break;

  default:
    break;
  }
}
void DevID::DlUserID(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  DEV_DL_USER_RANGE Range = {0};

  if (0 == GET_ERR(pCMD->m_CFlag)) {
    CARRAY* pArray = (CARRAY*)(pCMD + 1);
    Range.m_Count = pArray->m_Count;
    Range.m_pArray = (DEV_VUSER*)(pArray->m_Block);
  }

  OnDevCmd(s, NULL, DEV_USER_RANGE_OPER, DEV_OPER_DOWNLOAD, &Range, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
}
void DevID::DlRecID(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  DEV_OPER_TYPE type = DEV_RECORD_RANGE_OPER;

  switch (pCMD->m_MCmd) {
  case CMD_M_VERIFY:
    type = DEV_RECORD_RANGE_OPER;
    break;

  case CMD_M_ALARM:
    type = DEV_ALARM_RANGE_OPER;
    break;

  case CMD_M_OPTLOG:
    type = DEV_LOG_RANGE_OPER;
    break;

  case CMD_M_CIMAGE:
    type = DEV_CIMAGE_RANGE_OPER;
    break;

  default:
    break;
  }

  DEV_DL_RECORD_ARRAY temp = {0};

  if (0 == GET_ERR(pCMD->m_CFlag)) {
    CARRAY* pArray = (CARRAY*)(pCMD + 1);
    temp.m_Count = pArray->m_Count;
    temp.m_pRange = (DEV_DL_RECORD_RANGE*)(pArray->m_Block);
  }

  OnDevCmd(s, NULL, type, DEV_OPER_DOWNLOAD, &temp, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
}

