// Video.cpp: implementation of the CVideo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IPCameraDebug.h"
#include "Video.h"
#include "EagleEye2_imgprocess.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVideo::CVideo(): m_Wnd(NULL) , m_ImageW(1280) , m_ImageH(960) , m_CatchSize(0)
{
  ZeroMemory(&m_ImageBase , sizeof(m_ImageBase));
  ZeroMemory(&m_CurImage , sizeof(m_CurImage));
  ZeroMemory(&m_Ddsd2 , sizeof(m_Ddsd2));
}

CVideo::~CVideo()
{
  delete[] m_CurImage.m_pIMage;
  delete[] m_ImageBase.pBuffer[0];
  delete[] m_ImageBase.pBuffer[1];
  delete[] m_ImageBase.pBuffer[2];
  delete[] m_ImageBase.pBuffer[3];
}

void CVideo::DebugVideo(const IP_CMD& cmd , const IP_IMAGE& image)
{
  static uutime timee;
  char msg[128] = { 0 };
  memcpy(msg , cmd.m_Cmd , 6);
  sprintf(msg + 6 , "%x , %x , %x , %x" ,
      cmd.m_Cmd[6] , cmd.m_Cmd[7] , cmd.m_Cmd[8] , cmd.m_Cmd[9]);
  sprintf(msg + 10, " Image: %d , %d , %d , %d  timer = %f\n",
      image.m_Format , image.m_W , image.m_H , image.m_Bytes , timee.elapsed() * 1000);
  //  DEBUG_INFO1( "%s" , msg );
  timee.restart();
}

void CVideo::AllocImage(IP_IMAGE& iMage)
{
  if (iMage.m_pIMage != NULL) {
    if (iMage.m_Bytes > m_CatchSize) {
      ASSERT(FALSE);
      delete[] iMage.m_pIMage;
      iMage.m_pIMage = NULL;
    }
    else {
      return;
    }
  }

  switch (iMage.m_Format) {
  case JPG_F:
    m_CatchSize    = WIDTH_STEP(iMage.m_W , 3 , 4) * iMage.m_H;
    iMage.m_pIMage = new char[ m_CatchSize ];
    break;

  default:
    m_CatchSize    = iMage.m_Bytes;
    iMage.m_pIMage = new char[ m_CatchSize ];
    break;
  }
}

BOOL CVideo::CmdHandle(SOCKET s , IP_CMD& cmd)
{
  ASSERT(memcmp(cmd.m_Cmd , VINFO_HEARD , sizeof(VINFO_HEARD) - 1) == 0);

  if (m_CurImage.m_Format != cmd.m_Cmd[ IMAGE_FROMAT ]) {
    delete[] m_ImageBase.pBuffer[0];
    delete[] m_ImageBase.pBuffer[1];
    delete[] m_ImageBase.pBuffer[2];
    delete[] m_ImageBase.pBuffer[3];
    delete[] m_CurImage.m_pIMage;
    ZeroMemory(&m_ImageBase , sizeof(m_ImageBase));
    ZeroMemory(&m_CurImage , sizeof(m_CurImage));
  }

  m_CurImage.m_Format = cmd.m_Cmd[ IMAGE_FROMAT ];
  m_CurImage.m_Bytes  = *((int*)(cmd.m_Cmd + IMAGE_FROMAT + 1));

  switch (m_CurImage.m_Format) {
  case JPG_F:
    m_CurImage.m_W     = m_ImageW;
    m_CurImage.m_H     = m_ImageH;
    break;

  case YUV422_F:
    m_CurImage.m_W     = LOWORD(m_CurImage.m_Bytes);
    m_CurImage.m_H     = HIWORD(m_CurImage.m_Bytes);
    m_CurImage.m_Bytes = m_CurImage.m_W * m_CurImage.m_H * 2;
    break;

  case RGB24_F:
    m_CurImage.m_W     = LOWORD(m_CurImage.m_Bytes);
    m_CurImage.m_H     = HIWORD(m_CurImage.m_Bytes);
    m_CurImage.m_Bytes = WIDTH_STEP(m_CurImage.m_W, 3, 4) * m_CurImage.m_H;
    break;

  case BAYER_F:
  case YUV420_F:
    m_CurImage.m_W     = LOWORD(m_CurImage.m_Bytes);
    m_CurImage.m_H     = HIWORD(m_CurImage.m_Bytes);
    m_CurImage.m_Bytes = m_CurImage.m_W * m_CurImage.m_H * 1.5;
    break;

  default:
    m_CurImage.m_Format = ERR_F;
    break;
  }

  //DebugVideo( cmd , m_CurImage );
  if (m_CurImage.m_Format != ERR_F && m_CurImage.m_W <= m_ImageW && m_CurImage.m_H <= m_ImageH) {
    AllocImage(m_CurImage);
    int readBytes = CNetTools::NT_Read(m_NetSocket , m_CurImage.m_pIMage , m_CurImage.m_Bytes , 500);

    if (readBytes == m_CurImage.m_Bytes) {
      if (cmd.m_Cmd[6]) {
        CString jpgPath;
        jpgPath.Format("%d_%d.jpg" , cmd.m_Cmd[11] , cmd.m_Cmd[13]);
        FILE* pFIle = ::fopen(jpgPath.GetBuffer(0) , "wb");

        if (pFIle) {
          ::fwrite(m_CurImage.m_pIMage , 1 , m_CurImage.m_Bytes , pFIle);
          ::fclose(pFIle);
        }

        //        DEBUG_INFO1( "%s" , jpgPath.GetBuffer( 0 ) );
      }

      cmd.m_pParam = (char*)&m_CurImage;

      if (m_ReplyCB) {
        (*m_ReplyCB)(cmd , m_UserData);
      }

      PreView(m_CurImage);
    }
  }

  char commd[] = { 0xa0 , 0xa0  };
  CNetTools::NT_Write(m_NetSocket , commd , sizeof(commd));
  return TRUE;
}

void CVideo::PreView(const IP_IMAGE& iMage)
{
  AoutLock lock(&m_SynWnd);

  if (!::IsWindow(m_Wnd)) {
    return;
  }

  switch (iMage.m_Format) {
  case YUV420_F: {
    if (NULL == m_ImageBase.pBuffer[0]) {
      m_ImageBase.pBuffer[0] = new BYTE[iMage.m_W * iMage.m_H];
      m_ImageBase.pBuffer[1] = new BYTE[iMage.m_W * iMage.m_H >> 2];
      m_ImageBase.pBuffer[2] = new BYTE[iMage.m_W * iMage.m_H >> 2];
      m_ImageBase.dwLineSize[0] = iMage.m_W;
      m_ImageBase.dwLineSize[1] = iMage.m_W >> 1;
      m_ImageBase.dwLineSize[2] = iMage.m_W >> 1;

      FormatYV12::Build(m_Ddsd2 , iMage.m_W, iMage.m_H);
      m_directDraw.Create(m_Wnd, m_Ddsd2 , FormatYV12::Copy);
    }

    long indexY = 0 , indexU = 0 , indexV = 0 , step = (iMage.m_W * 3) >> 1;

    for (int y = 0; y < iMage.m_H; y += 2) {
      char* pYuy = iMage.m_pIMage + y * step;
      char* pYvy = iMage.m_pIMage + (y + 1) * step;

      for (int u = 0; u < step; u += 3) {
        m_ImageBase.pBuffer[0][indexY++] = *(pYuy + u);
        m_ImageBase.pBuffer[1][indexU++] = *(pYuy + u + 1);
        m_ImageBase.pBuffer[0][indexY++] = *(pYuy + u + 2);
      }

      for (int v = 0; v < step; v += 3) {
        m_ImageBase.pBuffer[0][indexY++] = *(pYvy + v);
        m_ImageBase.pBuffer[2][indexV++] = *(pYvy + v + 1);
        m_ImageBase.pBuffer[0][indexY++] = *(pYvy + v + 2);
      }
    }

    m_directDraw.Draw(m_ImageBase);
  }
  break;

  case BAYER_F:
    if (NULL == m_ImageBase.pBuffer[0]) {
      m_ImageBase.pBuffer[0] = new BYTE[iMage.m_W * iMage.m_H];
      m_ImageBase.pBuffer[1] = new BYTE[iMage.m_W * iMage.m_H / 4];
      m_ImageBase.pBuffer[2] = new BYTE[iMage.m_W * iMage.m_H / 4];
      m_ImageBase.dwLineSize[0] = iMage.m_W;
      m_ImageBase.dwLineSize[1] = iMage.m_W >> 1;
      m_ImageBase.dwLineSize[2] = iMage.m_W >> 1;

      FormatYV12::Build(m_Ddsd2 , iMage.m_W, iMage.m_H);
      m_directDraw.Create(m_Wnd, m_Ddsd2 , FormatYV12::Copy);
    }

    EagleEye2_Bayer2YCrCb420(
        (Uint8*)iMage.m_pIMage ,
        (Uint8*)m_ImageBase.pBuffer[0],
        (Uint8*)m_ImageBase.pBuffer[1],
        (Uint8*)m_ImageBase.pBuffer[2],
        (Uint32)iMage.m_W, (Uint32)iMage.m_H);
    m_directDraw.Draw(m_ImageBase);
    break;

  case YUV422_F: {
    if (NULL == m_ImageBase.pBuffer[0]) {
      m_ImageBase.pBuffer[0]    = (PBYTE)iMage.m_pIMage;
      m_ImageBase.dwLineSize[0] = iMage.m_W * 2;
      FormatUYVY::Build(m_Ddsd2 , iMage.m_W, iMage.m_H);
      m_directDraw.Create(m_Wnd, m_Ddsd2 , FormatUYVY::Copy);
    }

    m_directDraw.Draw(m_ImageBase);
  }
  break;

  case JPG_F: {
    // Load JPEG image from buffer
    m_fPic.LoadJpeg((BYTE*)iMage.m_pIMage , iMage.m_Bytes);
    // Achieve the handle of bitmap
    HBITMAP hBmp = (HBITMAP)m_fPic.GetDDB_Handle();

    // Draw the DDB bitmap
    if (m_DDB.GetHandle() != NULL) {
      ::DeleteObject(m_DDB.GetHandle());
    }

    m_DDB.Attach(hBmp);

    if (m_DDB.GetHandle()) {
      HDC dc = GetWindowDC(m_Wnd);
      RECT rect = { 0 };
      ::GetClientRect(m_Wnd , &rect);
      m_DDB.ScaleDraw(dc, 0, 0, rect.right - rect.left, rect.bottom - rect.top);
      ::ReleaseDC(m_Wnd , dc);
    }
  }
  break;

  case RGB24_F:
    if (NULL == m_ImageBase.pBuffer[0]) {
      m_ImageBase.pBuffer[0]    = (PBYTE)iMage.m_pIMage;
      m_ImageBase.dwLineSize[0] = iMage.m_W * 3;
      FormatBGR24::Build(m_Ddsd2 , iMage.m_W, iMage.m_H);
      m_directDraw.Create(m_Wnd, m_Ddsd2 , FormatBGR24::Copy);
    }

    m_directDraw.Draw(m_ImageBase);
    break;
  }
}
