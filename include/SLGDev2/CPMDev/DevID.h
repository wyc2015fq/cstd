///////////////////////////////////////////////////////////////////////////////////////
//名    称:用户ID或记录ID命令类
//功    能:发送或获取用户ID或记录ID
///////////////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_DEVIDCOMMAND_H__F9F40177_A526_4956_AF4E_2D03B8F9425F__INCLUDED_)
#define AFX_DEVIDCOMMAND_H__F9F40177_A526_4956_AF4E_2D03B8F9425F__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "DevCmd.h"
class DevID : public DevData
{
public:
  DevID(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData);
public:
  DevID(const CSock* s, int nClass, int nSeq, char* pbBuf, unsigned int& nLen); //下载所有
  DevID(const CSock* s, int nClass, const DEV_TIMESEG& cTimeSeg, int nSeq, char* pbBuf, unsigned int& nLen);  //时间段下载
  DevID(const CSock* s, int nClass, int nFlag, const DEV_DL_RECORD_RANGE& cRange, int nSeq, char* pbBuf, unsigned int& nLen); //区间段下载
private:
  void ProcessNCMD(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData);
  void DlUserID(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData);
  void DlRecID(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData);
};
#endif // !defined(AFX_DEVIDCOMMAND_H__F9F40177_A526_4956_AF4E_2D03B8F9425F__INCLUDED_)

