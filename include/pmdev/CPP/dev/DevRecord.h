// DevRecordCommand.h: interface for the DevRecordCommand class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEVRECORDCOMMAND_H__D01AC178_8EFE_47D9_93D7_64FD763BCB81__INCLUDED_)
#define AFX_DEVRECORDCOMMAND_H__D01AC178_8EFE_47D9_93D7_64FD763BCB81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DevCmd.h"

class DevRecord : public DevData
{
public:
  DevRecord(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData) ;
  DevRecord(const char* cszIP, const DEV_DL_RECORD_RANGE& cRange, int nFlag, int nSeq, char* pbBuf, unsigned int& nLen);  //обть
private:
  void ProcessNCMD(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
};

class DevAlarm: public DevData
{
public:
  DevAlarm(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData) ;
  DevAlarm(const char* cszIP, const DEV_DL_RECORD_RANGE& cRange, int nFlag, int nSeq, char* pbBuf, unsigned int& nLen);  //обть
private:
  void ProcessNCMD(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
};

class DevRecordEx : public DevData
{
public:
  DevRecordEx(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData) ;
  DevRecordEx(const char* cszIP, const DEV_DL_RECORD_RANGE& cRange, int nSeq, char* pbBuf, unsigned int& nLen);  //обть
private:
  void ProcessNCMD(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
};

#endif // !defined(AFX_DEVRECORDCOMMAND_H__D01AC178_8EFE_47D9_93D7_64FD763BCB81__INCLUDED_)
