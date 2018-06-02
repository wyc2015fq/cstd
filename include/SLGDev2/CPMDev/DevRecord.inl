// DevRecordCommand.cpp: implementation of the DevRecordCommand class.
//
#include "DevRecord.h"
REG_OBJECT(CMD_M_VERIFY, DevRecord, false);
REG_OBJECT(CMD_M_ALARM, DevAlarm, false);
REG_OBJECT(CMD_M_RECORD_EX, DevRecordEx, false);
void DspRec2UspRec(RECORD& dsp, DEV_RECORD& usp, BOOL realTime)
{
  usp.m_RecType = DEV_REC_TYPE(dsp.m_RecType);
  usp.m_bRealTime = realTime;
  usp.m_Score = dsp.m_Score;
  usp.m_PicLen = dsp.m_PicBytes;
  usp.m_VerifyMode = dsp.m_VerifyMode;
  usp.m_ConcretSource = dsp.m_Source;
  usp.m_Source = DEV_DOOR_SOURCE;
  usp.m_PicData = dsp.m_Block;
  memcpy(usp.m_SerialID.m_ID, dsp.m_SerialID.m_ID, sizeof(CID));
  memcpy(usp.m_UserID.m_ID, dsp.m_UserID.m_ID, sizeof(CID));
  memcpy(usp.m_ChangID.m_ID, dsp.m_ChangID.m_ID, sizeof(CID));
  memcpy(usp.m_AppendID.m_ID, dsp.m_AppendID.m_ID, sizeof(CID));
  memcpy(usp.m_UserName.m_ID, dsp.m_UserName.m_ID, sizeof(CID));
  memcpy(usp.m_Department.m_ID, dsp.m_Department.m_ID, sizeof(CID));
  memcpy(&(usp.m_RecTime), &(dsp.m_RecTime), sizeof(DATETIME));
}
DevRecord::DevRecord(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  ProcessNCMD(s, pCMD, bServer, pvClientData);
}
DevRecord::DevRecord(const CSock* s, const DEV_DL_RECORD_RANGE& cRange, int nFlag, int nSeq, char* pbBuf, unsigned int& nLen)
{
  assert(pbBuf);
  NCMD* pCMD = (NCMD*)pbBuf;
  s->setNCMD(pCMD, CMD_M_VERIFY, CMD_S_GET, 0, nSeq);

  if (nFlag & DL_DEV_PIC) {
    pCMD->m_CFlag |= F_DL_PIC;
  }

  memcpy(pCMD + 1, &cRange, sizeof(RECORDRANGE));
  nLen = sizeof(NCMD) + sizeof(RECORDRANGE);
}
void DevRecord::ProcessNCMD(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  assert(pCMD);
  DEV_RECORD Tmp;
  memset(&Tmp, 0, sizeof(Tmp));

  if (0 == GET_ERR(pCMD->m_CFlag)) {
    DspRec2UspRec(*(RECORD*)(pCMD + 1), Tmp, pCMD->m_SCmd == CMD_S_REALTIME_SEND);
  }
  else if (OPER_ERR_BUSY != GET_ERR(pCMD->m_CFlag)) {
    memcpy(Tmp.m_SerialID.m_ID, pCMD + 1, sizeof(CID));//??
  }

  OnDevCmd(s, NULL, DEV_RECORD_OPER, DEV_OPER_DOWNLOAD, &Tmp, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
}
DevAlarm::DevAlarm(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  ProcessNCMD(s, pCMD, bServer, pvClientData);
}
DevAlarm::DevAlarm(const CSock* s, const DEV_DL_RECORD_RANGE& cRange, int nFlag, int nSeq, char* pbBuf, unsigned int& nLen)
{
  assert(pbBuf);
  NCMD* pCMD = (NCMD*)pbBuf;
  s->setNCMD(pCMD, CMD_M_ALARM, CMD_S_GET, 0, nSeq);

  if (nFlag & DL_DEV_PIC) {
    pCMD->m_CFlag |= F_DL_PIC;
  }

  memcpy(pCMD + 1, &cRange, sizeof(RECORDRANGE));
  nLen = sizeof(NCMD) + sizeof(RECORDRANGE);
}
void DevAlarm::ProcessNCMD(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  assert(pCMD);
  DEV_RECORD Tmp ;
  memset(&Tmp, 0x0, sizeof(Tmp));

  if (0 == GET_ERR(pCMD->m_CFlag)) {
    DspRec2UspRec(*(RECORD*)(pCMD + 1), Tmp, pCMD->m_SCmd == CMD_S_REALTIME_SEND);
  }
  else {
    memcpy(Tmp.m_SerialID.m_ID, pCMD + 1, sizeof(CID));
  }

  OnDevCmd(s, NULL, DEV_ALARM_OPER, DEV_OPER_DOWNLOAD, &Tmp, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
}
DevRecordEx::DevRecordEx(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  ProcessNCMD(s, pCMD, bServer, pvClientData);
}
DevRecordEx::DevRecordEx(const CSock* s, const DEV_DL_RECORD_RANGE& cRange, int nSeq, char* pbBuf, unsigned int& nLen)
{
  assert(pbBuf);
  NCMD* pCMD = (NCMD*)pbBuf;
  s->setNCMD(pCMD, CMD_M_RECORD_EX, CMD_S_GET, F_DL_PIC, nSeq);
  memcpy(pCMD + 1, &cRange, sizeof(RECORDRANGE));
  nLen = sizeof(NCMD) + sizeof(RECORDRANGE);
}
#define BATCH_MAX 200
void DevRecordEx::ProcessNCMD(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  assert(pCMD);

  if (SUC_EXECUT == GET_ERR(pCMD->m_CFlag)) {
    CARRAY* pArray = (CARRAY*)(pCMD + 1);

    switch (pCMD->m_SCmd) {
    case CMD_M_VERIFY:
    case CMD_M_ALARM: {
      RECORD* pData = (RECORD*)(pArray->m_Block);
      DEV_BATCH_RECORD records = {0};
      DEV_RECORD record[BATCH_MAX];
      records.m_nCount = pArray->m_Count;
      records.m_pRecordArray = record;

      for (int i = 0; i < pArray->m_Count; ++i) {
        DspRec2UspRec(*pData, record[i], false);
        pData = (RECORD*)((char*)pData + RECORD_STEP(*pData));
      }

      if (pCMD->m_CFlag & 0x00000800) {
        pCMD->m_CFlag = OPER_BATCH_DL_COMPLETE;
      }

      OnDevCmd(s, NULL, DEV_RECORD_EX_OPER, DEV_OPER_DOWNLOAD, &records, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
    }
    break;

    case CMD_M_OPTLOG: {
      DSPLOG* pRecord = (DSPLOG*)(pArray->m_Block);
      DEV_BATCH_LOG records = {0};
      DEV_LOG record[BATCH_MAX];
      records.m_nCount = pArray->m_Count;
      records.m_pLogArray = record;

      for (int i = 0; i < pArray->m_Count; ++i) {
        DEV_LOG& Log = record[i];
        Log.m_LogType = DEV_LOG_TYPE(pRecord->m_LogType);
        Log.m_bRTLog = FALSE;
        memcpy(Log.m_Admin.m_ID, pRecord->m_Admin.m_ID, sizeof(CID));
        memcpy(Log.m_BeOptUser.m_ID, pRecord->m_BeOptUser.m_ID, sizeof(CID));
        memcpy(Log.m_SerialID.m_ID, pRecord->m_SerialID.m_ID, sizeof(CID));
        memcpy(&(Log.m_RecTime), &(pRecord->m_RecTime), sizeof(DATETIME));
        pRecord = (DSPLOG*)((char*)pRecord + DSPLOG_STEP(*pRecord));
      }

      if (pCMD->m_CFlag & 0x00000800) {
        pCMD->m_CFlag = OPER_BATCH_DL_COMPLETE;
      }

      OnDevCmd(s, NULL, DEV_LOG_EX_OPER, DEV_OPER_DOWNLOAD, &records, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
    }
    break;

    case CMD_M_CIMAGE: {
      CHANGEIMAGE* pRecord = (CHANGEIMAGE*)(pArray->m_Block);
      DEV_BATCH_CIMAGE records = {0};
      DEV_CHANGEIMAGE record[BATCH_MAX];
      records.m_nCount = pArray->m_Count;
      records.m_pCImageArray = record;

      for (int i = 0; i < pArray->m_Count; ++i) {
        DEV_CHANGEIMAGE& Image = record[i];
        Image.m_PicBytes = pRecord->m_PicBytes;
        Image.m_PicData = pRecord->m_Block;
        Image.m_bRTChangeImage = (pCMD->m_SCmd == CMD_S_REALTIME_SEND) ? TRUE : FALSE;
        memcpy(&(Image.m_UserID), &(pRecord->m_UserID), sizeof(CID));
        memcpy(&(Image.m_SerialID), &(pRecord->m_SerialID), sizeof(CID));
        memcpy(Image.m_AppendID.m_ID, pRecord->m_AppendID.m_ID, sizeof(CID));
        memcpy(Image.m_UserName.m_ID, pRecord->m_UserName.m_ID, sizeof(CID));
        memcpy(Image.m_Department.m_ID, pRecord->m_Department.m_ID, sizeof(CID));
        memcpy(Image.m_Admin.m_ID, pRecord->m_Admin.m_ID, sizeof(CID));
        memcpy(&(Image.m_RecTime), &(pRecord->m_RecTime), sizeof(DATETIME));
        pRecord = (CHANGEIMAGE*)((char*)pRecord + CGIMG_STEP(*pRecord));
      }

      if (pCMD->m_CFlag & 0x00000800) {
        pCMD->m_CFlag = OPER_BATCH_DL_COMPLETE;
      }

      OnDevCmd(s, NULL, DEV_CIMAGE_EX_OPER, DEV_OPER_DOWNLOAD, &records, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
    }
    break;

    default:
      break;
    }
  }
  else {
    switch (pCMD->m_SCmd) {
    case CMD_M_VERIFY:
    case CMD_M_ALARM:
      OnDevCmd(s, NULL, DEV_RECORD_EX_OPER, DEV_OPER_DOWNLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
      break;

    case CMD_M_OPTLOG:
      OnDevCmd(s, NULL, DEV_LOG_EX_OPER, DEV_OPER_DOWNLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
      break;

    case CMD_M_CIMAGE:
      OnDevCmd(s, NULL, DEV_CIMAGE_EX_OPER, DEV_OPER_DOWNLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
      break;

    default:
      break;
    }
  }
}

