// DevOpenDoorCommand.h: interface for the DevOpenDoorCommand class.
//
#if !defined(AFX_DEVOPENDOORCOMMAND_H__A453D337_2A95_49DE_B8F1_8F8C332CC7C2__INCLUDED_)
#define AFX_DEVOPENDOORCOMMAND_H__A453D337_2A95_49DE_B8F1_8F8C332CC7C2__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "DevCmd.h"
class DevEOpenDoor : public DevData
{
public:
  DevEOpenDoor(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData) ;
public:
  DevEOpenDoor(const CSock* s, DEV_DOOR_STATE nStatus, int nSeq, char* pbBuf, unsigned int& nLen);   //设置开关门状态
  DevEOpenDoor(const CSock* s, int nSeq, char* pbBuf, unsigned int& nLen);   //获取开关门状态
  //DevEOpenDoor(const CSock* s, BOOL bFlag, int* pDoorArray, int size, int nSeq, char* pbByf, unsigned int& nLen );
private:
  void ProcessNCMD(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData);
};
class DevROpenDoor : public DevData
{
public:
  DevROpenDoor(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData) ;
  DevROpenDoor(const CSock* s, BOOL bOpen, int nSeq, char* pbBuf, unsigned int& nLen);   //远程开关门
private:
  void ProcessNCMD(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData);
};
class DevIOCtrl : public DevData
{
public:
  DevIOCtrl(const CSock* s, const DEV_IOCTRL& ioctrl, int nSeq, char* pbByf, unsigned int& nLen);
  DevIOCtrl(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData) ;
private:
  void ProcessNCMD(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData);
};
class DevNOpenDoor : public DevData
{
public:
  DevNOpenDoor(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData) ;
private:
  void ProcessNCMD(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData);
};
#endif // !defined(AFX_DEVOPENDOORCOMMAND_H__A453D337_2A95_49DE_B8F1_8F8C332CC7C2__INCLUDED_)

