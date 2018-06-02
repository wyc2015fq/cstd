// DevUserCommand.h: interface for the DevUserCommand class.
//
#if !defined(AFX_DEVUSERCOMMAND_H__CAB9310C_8A5D_470C_A427_F3A2125DAA5D__INCLUDED_)
#define AFX_DEVUSERCOMMAND_H__CAB9310C_8A5D_470C_A427_F3A2125DAA5D__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "DevCmd.h"
class DevUser : public DevData
{
public:
  DevUser(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData) ;
  DevUser(const CSock* s, const CID& cID, int nFlag, int nSeq, char* pbBuf, unsigned int& nLen);  //用户获取操作
  DevUser(const CSock* s, const CID& cID, int nSeq, char* pbBuf, unsigned int& nLen);  //用户删除操作
  DevUser(const CSock* s, const DEV_USER& cUser, BOOL bAddFlag, int nSeq, char* pbBuf, unsigned int& nLen);  //用户添加或采集操作
  DevUser(const CSock* s, const DEV_USER& cUser, const DEV_USER_REAL_LOAD& cLoad, char* pbBuf, unsigned int& nLen);  //用户实时添加操作
  //  DevUser( const addr_inx* addr, const DEV_USER& cUser, int nSeq, char* pbBuf, unsigned int& nLen ); //实时用户采集
private:
  void ProcessNCMD(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData);
  void UserAdd(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData);
  void UserDel(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData);
  void UserDown(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData);
  void UserRtLoad(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData);
  void UserRemoteCap(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData);
  void UserRemoteReCap(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData);
};
class DevUserEx : public DevData
{
public:
  DevUserEx(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData) ;
  DevUserEx(const CSock* s, const DEV_BATCH_USER& cUsers, int nFlag, int nSeq, char*& pbBuf, unsigned int& nLen); //用户批量上传操作
  DevUserEx(const CSock* s, DEV_CID* pUserIDArray, int nIDCount, int nFlag, int nSeq, char*& pbBuf, unsigned int& nLen); //用户批量下载操作
  DevUserEx(const CSock* s, DEV_CID* pUserIDArray, int nIDCount, int nSeq, char*& pbBuf, unsigned int& nLen); //用户批量删除操作
private:
  void ProcessNCMD(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData);
  void UserAdd(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData);
  void UserDel(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData);
  void UserDown(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData);
  void UserAdd(const CSock* s, const DEV_BATCH_USER& cUsers, int nFlag, int nSeq, char*& pbBuf, unsigned int& nLen);
  void UserDel(const CSock* s, DEV_CID* pUserIDArray, int nIDCount, int nSeq, char*& pbBuf, unsigned int& nLen);
  void UserDown(const CSock* s, DEV_CID* pUserIDArray, int nIDCount, int nFlag, int nSeq, char*& pbBuf, unsigned int& nLen);
};
#endif // !defined(AFX_DEVUSERCOMMAND_H__CAB9310C_8A5D_470C_A427_F3A2125DAA5D__INCLUDED_)

