// video.h: interface for the video class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIDEO_H__52F8E63E_D8A3_4C0F_A0C4_9918E3A0EC35__INCLUDED_)
#define AFX_VIDEO_H__52F8E63E_D8A3_4C0F_A0C4_9918E3A0EC35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Net/Net.h"

class CVideo
{
public:
  CVideo();
  virtual ~CVideo() {}
  int SetNet(CNet* net) {
    m_Net = (net);
    ASSERT(net);
    return 0;
  }
public:
  BOOL StartVideo(const char* cszIP, void* hwnd, frame_callback_t frame_rgb);
  void StopVideo(const char* cszIP);
  void GetVideoTransAddr(const char* cszIP, SOCK_ADDRESS& Addr);
  void ShowVideo(const char* cszIP, const char* cpbImage, int w, int h, int step, int nImageType);
private:
  CNet* m_Net;
private:
  struct VideoPro {
    HANDLE m_hVideoHandle;
    BYTE* m_pRGBTMP;
    BYTE* m_pRGBTRANS;
    BITMAPINFO* m_pBitmapInfo;
    void* user;
    frame_callback_t frame_rgb;
    SOCK_ADDRESS m_Addr;
  };
  typedef std::map<unsigned long, VideoPro*> SOCK_MAP;
  SOCK_MAP m_map;
  CriticalSection m_syn;
};

#endif // !defined(AFX_VIDEO_H__52F8E63E_D8A3_4C0F_A0C4_9918E3A0EC35__INCLUDED_)
