// DevSetCommand.h: interface for the DevSetCommand class.
//
#if !defined(AFX_DEVSETCOMMAND_H__C737FA00_F075_4526_9222_FABD927C7454__INCLUDED_)
#define AFX_DEVSETCOMMAND_H__C737FA00_F075_4526_9222_FABD927C7454__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "DevCmd.h"
class DevSet : public DevData
{
public:
  DevSet(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData) ;
  DevSet(const CSock* s, int nSeq, char* pbBuf, unsigned int& nLen); //ªÒ»°
  DevSet(const CSock* s, const DEV_WORKATT& cInfo, int nSeq, char* pbBuf, unsigned int& nLen); //…Ë÷√
private:
  void ProcessNCMD(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData);
  void SetAtt(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData);
  void GetAtt(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData);
};
#endif // !defined(AFX_DEVSETCOMMAND_H__C737FA00_F075_4526_9222_FABD927C7454__INCLUDED_)

