// DevVideo.h: interface for the DevVideo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEVVIDEO_H__AC8317D7_9FA4_4406_914B_4BAABC58D148__INCLUDED_)
#define AFX_DEVVIDEO_H__AC8317D7_9FA4_4406_914B_4BAABC58D148__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DevCmd.h"

class DevVideo : public DevData
{
public:
  DevVideo(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
  DevVideo(const char* cszIP, const char* cszVideoIP, unsigned short VideoPort, /*long hwnd,*/ int nSeq, char* pbBuf, unsigned int& nLen);//…Ë÷√
  DevVideo(const char* cszIP, int nSeq, char* pbBuf, unsigned int& nLen);//Õ£÷π
private:
  void ProcessNCMD(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
  void StartVideo(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
  void StopVideo(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
};

#endif // !defined(AFX_DEVVIDEO_H__AC8317D7_9FA4_4406_914B_4BAABC58D148__INCLUDED_)
