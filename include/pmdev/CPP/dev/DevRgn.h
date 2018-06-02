/*************************************************************************************/
/*名    称:设备连接命令类
/*功    能:管理端跟dsp设备端建立连接或断开连接时将发送此命令通知给外部
           外部接受到此命令即可获取连接状态
/*************************************************************************************/

#if !defined(AFX_DEVREGION_H__DFDB31A7_48EF_4101_A0C8_63306CFFAB35__INCLUDED_)
#define AFX_DEVREGION_H__DFDB31A7_48EF_4101_A0C8_63306CFFAB35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DevCmd.h"

class DevRgn : public DevData
{
public:
  DevRgn(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
  DevRgn(const char* cszIP, const char* cszGroupIP, int nSeq, char* pbBuf, unsigned int& nLen);//设置
  DevRgn(const char* cszIP, int nSeq, char* pbBuf, unsigned int& nLen);//获取
private:
  void ProcessNCMD(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
  void SetRgn(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
  void GetRgn(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
};



#endif // !defined(AFX_DEVCONNECT_H__DFDB31A7_48EF_4101_A0C8_63306CFFAB35__INCLUDED_)
