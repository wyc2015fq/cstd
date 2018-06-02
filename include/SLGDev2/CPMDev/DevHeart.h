///////////////////////////////////////////////////////////////////////////////////////
//名    称:心跳包命令类
//功    能:管理端跟dsp设备端建立连接后dsp设备端会不间断发送心跳，此命令对象收到心跳包
//       消息后将通知外部，外部接受到此消息后视情况回送心跳包
///////////////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_DEVHEARTCOMMAND_H__8EDBF5F4_97D4_4F20_8CC6_A986F8B75AF2__INCLUDED_)
#define AFX_DEVHEARTCOMMAND_H__8EDBF5F4_97D4_4F20_8CC6_A986F8B75AF2__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "DevCmd.h"
class DevHeart : public DevData
{
public:
  DevHeart(const CSock* addr, NCMD* pCMD, BOOL bServer, void* pvClientData);
private:
  void ProcessNCMD(const CSock* addr, NCMD* pCMD, BOOL bServer, void* pvClientData);
};
#endif // !defined(AFX_DEVHEARTCOMMAND_H__8EDBF5F4_97D4_4F20_8CC6_A986F8B75AF2__INCLUDED_)

