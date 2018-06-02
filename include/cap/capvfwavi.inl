
#ifndef _CAPVFW_H_
#define _CAPVFW_H_
#include <windows.h>
#include <assert.h>
#include <stdlib.h>
#include <vfw.h>
#include "cstd.h"
#include "img\imgopt.inl"


#pragma comment(lib, "vfw32.lib")

#if _MSC_VER >= 1200
#pragma warning( disable: 4711 )
#endif

#ifdef __GNUC__
#define WM_CAP_FIRSTA              (WM_USER)
#define capSendMessage(hwnd,m,w,l) (IsWindow(hwnd)?SendMessage(hwnd,m,w,l):0)
#endif

#if (defined WIN64 || defined _WIN64) && defined _MSC_VER && !defined __ICL
#pragma optimize("",off)
#endif

// Capturing video from AVI via VFW
static void icvInitCapture_VFW()
{
  static int isInitialized = 0;

  if (!isInitialized) {
    AVIFileInit();
    isInitialized = 1;
  }
}

typedef struct capvfwavi {
  PAVIFILE avifile;
  PAVISTREAM avistream;
  PGETFRAME getframe;
  AVISTREAMINFO aviinfo;
  BITMAPINFOHEADER* bmih;
  int start_index, end_index;
  double fps;
  int pos;
  //CvSize size;
}
capvfwavi;

int capvfwavi_close(capdev* s0)
{
  capvfwavi* s = (capvfwavi*)s0->x;

  if (s) {
    if (s->getframe) {
      AVIStreamGetFrameClose(s->getframe);
    }

    if (s->avistream) {
      AVIStreamRelease(s->avistream);
    }

    if (s->avifile) {
      AVIFileRelease(s->avifile);
    }

    free(s);
    s0->x = 0;
  }

  //CoUninitialize();
  return 0;
}

int coinit()
{
  static int coinited = 0;

  if (coinited == 0) {
    coinited = 1;
    CoInitialize(NULL);
    //atexit(CoUninitialize);
  }

  return 0;
}

#define matflip2(a, b, c)  (b) = (a), matflip(s->height, s->width*3, s->data, s->width*3)
int capvfwavi_getframe(capdev* s0, img_t* im, int flag)
{
  capvfwavi* s = (capvfwavi*)s0->x;
  
  if (s) {
    s->bmih = (BITMAPINFOHEADER*) AVIStreamGetFrame(s->getframe, s->pos++);
    
    if (s->bmih != 0) {
      uchar* src;
      int step = (s->bmih->biWidth * 3+3)&~3;
      img_t im1[1] = {0};
      imsetsize(im, s->bmih->biHeight, s->bmih->biWidth, 3, 1);
      src = (unsigned char*)(s->bmih + 1);
      // src.widthStep
      memcpy2d(im->tt.data, im->s, src, step, s->bmih->biHeight, s->bmih->biWidth * 3);
      return 1;
    }
  }
  return 0;
}

int asdfasdf() {
  PAVISTREAM pavi;
  const char* szFile = "D:/dt/dt.avi";
  //TCHAR   title[100];                                         // Will Hold The Modified Window Title  
  int width, height, lastframe, mpf;
  AVISTREAMINFOA psi;
  BITMAPINFOHEADER bmih = {0};
  PGETFRAME pgf;
  
  AVIFileInit();                                              // Opens The AVIFile Library  
  // Opens The AVI Stream  
  if (AVIStreamOpenFromFile(&pavi, szFile, streamtypeVIDEO, 0, OF_READ, NULL) !=0)  
  {  
    // An Error Occurred Opening The Stream  
    MessageBox (HWND_DESKTOP, "Failed To Open The AVI Stream", "Error", MB_OK | MB_ICONEXCLAMATION);  
  }  
  AVIStreamInfo(pavi, &psi, sizeof(psi));                     // Reads Information About The Stream Into psi  
  width=psi.rcFrame.right-psi.rcFrame.left;                   // Width Is Right Side Of Frame Minus Left  
  height=psi.rcFrame.bottom-psi.rcFrame.top;                  // Height Is Bottom Of Frame Minus Top  
  
  lastframe=AVIStreamLength(pavi);                            // The Last Frame Of The Stream  
  
  mpf=AVIStreamSampleToTime(pavi,lastframe)/lastframe;        // Calculate Rough Milliseconds Per Frame  
  
  bmih.biSize = sizeof (BITMAPINFOHEADER);                    // Size Of The BitmapInfoHeader  
  bmih.biPlanes = 1;                                          // Bitplanes      
  bmih.biBitCount = 24;                                       // Bits Format We Want (24 Bit, 3 Bytes)  
  bmih.biWidth = width;                                         // Width We Want (256 Pixels)  
  bmih.biHeight = height;                                        // Height We Want (256 Pixels)  
  bmih.biCompression = BI_RGB;                                // Requested Mode = RGB  
  
  pgf=AVIStreamGetFrameOpen(pavi, (PBITMAPINFOHEADER)(&bmih));                        // Create The PGETFRAME Using Our Request Mode  
  if (pgf==NULL)  
  {  
    // An Error Occurred Opening The Frame  
    MessageBox (HWND_DESKTOP, "Failed To Open The AVI Frame", "Error", MB_OK | MB_ICONEXCLAMATION);  
  }  
  
  return 0;
}

int capvfwavi_open(capdev* s0, const char* filename)
{
  capvfwavi* s = NULL;
  coinit();

  if (!filename) {
    return 0;
  }
  //asdfasdf();
  {
    static int isInitialized = 0;
    
    if (!isInitialized) {
      AVIFileInit();
      isInitialized = 1;
    }
  }
  s = MALLOC(capvfwavi, 1);
  memset(s, 0, sizeof(capvfwavi));

  if (s) {
    HRESULT hr = AVIFileOpen(&s->avifile, filename, OF_READ, NULL);

    if (SUCCEEDED(hr)) {
      hr = AVIFileGetStream(s->avifile, &s->avistream, streamtypeVIDEO, 0);

      if (SUCCEEDED(hr)) {
        int iNumFrames=AVIStreamLength(s->avistream);
        hr = AVIStreamInfo(s->avistream, &s->aviinfo, sizeof(s->aviinfo));
        if (SUCCEEDED(hr)) {
          BITMAPINFOHEADER bmih = {0};
          int width = s->aviinfo.rcFrame.right - s->aviinfo.rcFrame.left;
          int height = s->aviinfo.rcFrame.bottom - s->aviinfo.rcFrame.top;
          bmih = icvBitmapHeader(width, height, 24, BI_RGB);

          s->start_index = (int) s->aviinfo.dwStart;
          s->end_index = s->start_index + (int) s->aviinfo.dwLength;
          s->fps = (double) s->aviinfo.dwRate / s->aviinfo.dwScale;
          s->pos = s->start_index;
          s->getframe = AVIStreamGetFrameOpen(s->avistream, &bmih);
          //s->getframe = AVIStreamGetFrameOpen(s->avistream, (PBITMAPINFOHEADER)(AVIGETFRAMEF_BESTDISPLAYFMT));   
          s0->x = s;
          s0->getframe = capvfwavi_getframe;
          s0->close = capvfwavi_close;
          return 1;
        }
      }
    }
  }

  capvfwavi_close((capdev*)s);
  return 0;
}

/********************* Capturing video from camera via VFW *********************/
#if 0
typedef struct CvCaptureCAM_VFW {
  CAP_HEADER;
  CAPDRIVERCAPS caps;
  HWND capWnd;
  VIDEOHDR* hdr;
  DWORD fourcc;
  HIC hic;
}
CvCaptureCAM_VFW;


void cvCaptureCAM_VFW_closeHIC(CvCaptureCAM_VFW* s)
{
  if (s->hic) {
    ICDecompressEnd(s->hic);
    ICClose(s->hic);
    s->hic = 0;
  }
}


LRESULT PASCAL cvCaptureCAM_VFW_frameCallback(HWND hWnd, VIDEOHDR* hdr)
{
  CvCaptureCAM_VFW* s = 0;

  if (!hWnd) {
    return FALSE;
  }

  s = (CvCaptureCAM_VFW*) capGetUserData(hWnd);
  s->hdr = hdr;

  return (LRESULT) TRUE;
}


void cvCaptureCAM_VFW_close(CvCaptureCAM_VFW* s)
{
  if (s->capWnd) {
    capSetCallbackOnFrame(s->capWnd, NULL);
    capDriverDisconnect(s->capWnd);
    DestroyWindow(s->capWnd);
    cvCaptureCAM_VFW_closeHIC(s);
  }

  FREE(s->data);
  FREE(s);
}


int cvCaptureCAM_VFW_grabFrame(CvCaptureCAM_VFW* s)
{
  if (s->capWnd) {
    SendMessage(s->capWnd, WM_CAP_GRAB_FRAME_NOSTOP, 0, 0);
    return TRUE;
  }

  return FALSE;
}

unsigned char* cvCaptureCAM_VFW_retrieveFrame(CvCaptureCAM_VFW* s)
{
  BITMAPINFO vfmt;
  BITMAPINFOHEADER* vfmt0;
  int sz, prevWidth, prevHeight;

  coinit();

  memset(&vfmt, 0, sizeof(vfmt));
  vfmt0 = &vfmt.bmiHeader;

  if (!s->capWnd) {
    return 0;
  }

  sz = capGetVideoFormat(s->capWnd, &vfmt, sizeof(vfmt));
  prevWidth = s->data ? s->width : 0;
  prevHeight = s->data ? s->height : 0;

  if (!s->hdr || s->hdr->lpData == 0 || sz == 0) {
    return 0;
  }

  if (!(s->data) || s->width != vfmt0->biWidth || s->height != vfmt0->biHeight) {
    FREE(s->data);
    s->data = MALLOC(uchar, vfmt0->biWidth * vfmt0->biHeight * 3);
    s->width = vfmt0->biWidth;
    s->height = vfmt0->biHeight;
    s->channel = 3;
  }

  if (vfmt.bmiHeader.biCompression != BI_RGB ||
      vfmt.bmiHeader.biBitCount != 24) {
    BITMAPINFOHEADER vfmt1 = icvBitmapHeader(vfmt0->biWidth, vfmt0->biHeight, 24, BI_RGB);

    if (s->hic == 0 || s->fourcc != vfmt0->biCompression ||
        prevWidth != vfmt0->biWidth || prevHeight != vfmt0->biHeight) {
      cvCaptureCAM_VFW_closeHIC(s);
      s->hic = ICOpen(MAKEFOURCC('V', 'I', 'D', 'C'),
          vfmt0->biCompression, ICMODE_DECOMPRESS);

      if (s->hic) {
        if (ICDecompressBegin(s->hic, vfmt0, &vfmt1) != ICERR_OK) {
          cvCaptureCAM_VFW_closeHIC(s);
          return 0;
        }
      }
    }

    if (!s->hic || ICDecompress(s->hic, 0, vfmt0, s->hdr->lpData,
        &vfmt1, s->data) != ICERR_OK) {
      cvCaptureCAM_VFW_closeHIC(s);
      return 0;
    }

    matflip2(s->data, s->data, 0);
  }
  else {
    unsigned char* src;
    cvInitImageHeader(s, vfmt0->biWidth, vfmt0->biHeight,
        IPL_DEPTH_8U, 3, IPL_ORIGIN_BL, 4);
    cvSetData(src, s->hdr->lpData, src.widthStep);
    matflip2(src, s->data, 0);
  }

  return s->data;
}


double cvCaptureCAM_VFW_getProperty(CvCaptureCAM_VFW* s, int property_id)
{
  switch (property_id) {
  case CV_CAP_PROP_FRAME_WIDTH:
    return s->data ? s->width : 0;

  case CV_CAP_PROP_FRAME_HEIGHT:
    return s->data ? s->height : 0;

  case CV_CAP_PROP_FOURCC:
    return s->fourcc;
  }

  return 0;
}

int GrabFrame_CAM_VFW(CvCaptureCAM_VFW* s)
{
  return cvCaptureCAM_VFW_grabFrame(s) && cvCaptureCAM_VFW_retrieveFrame(s);
}
typedef void (*Close_F)(struct capdev*);
typedef int (*Close_F2)(struct capdev*);
// Initialize camera input
CvCaptureCAM_VFW* cvCaptureCAM_VFW_open(int wIndex)
{
  CvCaptureCAM_VFW* s = NULL;
  char szDeviceName[ 80 ];
  char szDeviceVersion[ 80 ];
  HWND hWndC = 0;

  if ((unsigned) wIndex >= 10) {
    wIndex = 0;
  }

  for (; wIndex < 10; wIndex++) {
    if (capGetDriverDescription(wIndex, szDeviceName,
        sizeof(szDeviceName), szDeviceVersion,
        sizeof(szDeviceVersion))) {
      hWndC = capCreateCaptureWindow("My Own Capture Window",
          WS_POPUP | WS_CHILD, 0, 0, 320, 240, 0, 0);

      if (capDriverConnect(hWndC, wIndex)) {
        break;
      }

      DestroyWindow(hWndC);
      hWndC = 0;
    }
  }

  if (hWndC) {
    CAPTUREPARMS p;
    s = MALLOC(CvCaptureCAM_VFW, 1);
    MEMSET(s, 0, 1);

    s->data = 0;
    s->capWnd = hWndC;
    s->hdr = 0;
    s->hic = 0;
    s->fourcc = (DWORD) - 1;

    memset(&s->caps, 0, sizeof(s->caps));
    capDriverGetCaps(hWndC, &s->caps, sizeof(&s->caps));
    MoveWindow(hWndC, 0, 0, 320, 240, TRUE);
    capSetUserData(hWndC, (size_t) s);
    capSetCallbackOnFrame(hWndC, cvCaptureCAM_VFW_frameCallback);
    capCaptureGetSetup(hWndC, &p, sizeof(CAPTUREPARMS));
    p.dwRequestMicroSecPerFrame = 66667 / 2;
    capCaptureSetSetup(hWndC, &p, sizeof(CAPTUREPARMS));
    //capPreview( hWndC, 1 );
    capPreviewScale(hWndC, FALSE);
    capPreviewRate(hWndC, 1);
    s->Close = (Close_F) cvCaptureCAM_VFW_close;
    s->GrabFrame = (Close_F2) GrabFrame_CAM_VFW;
  }

  return s;
}



/*************************** writing AVIs ******************************/

typedef struct CvVideoWriter_VFW {
  CAP_HEADER;
  PAVIFILE avifile;
  PAVISTREAM compressed;
  PAVISTREAM uncompressed;
  double fps;
  unsigned char* tempFrame;
  long pos;
  int fourcc;
}
CvVideoWriter_VFW;


void CvVideoWriter_VFW_close(CvVideoWriter_VFW* s)
{
  if (s) {
    if (s->uncompressed) {
      AVIStreamRelease(s->uncompressed);
    }

    if (s->compressed) {
      AVIStreamRelease(s->compressed);
    }

    if (s->avifile) {
      AVIFileRelease(s->avifile);
    }

    FREE(s->tempFrame);
    FREE(s);
  }
}


// philipg.  Made this code capable of writing 8bpp gray scale bitmaps
typedef struct BITMAPINFO_8Bit {
  BITMAPINFOHEADER bmiHeader;
  RGBQUAD bmiColors[ 256 ];
}
BITMAPINFO_8Bit;


int CvVideoWriter_VFW_createStreams(CvVideoWriter_VFW* s, CvSize frameSize, int isColor)
{
  int i;
  AVISTREAMINFO aviinfo;
  BITMAPINFO_8Bit bmih;

  if (!s->avifile) {
    return FALSE;
  }

  bmih.bmiHeader = icvBitmapHeader(frameSize.w, frameSize.h, isColor ? 24 : 8, BI_RGB);

  for (i = 0; i < 256; i++) {
    bmih.bmiColors[ i ].rgbBlue = (BYTE) i;
    bmih.bmiColors[ i ].rgbGreen = (BYTE) i;
    bmih.bmiColors[ i ].rgbRed = (BYTE) i;
    bmih.bmiColors[ i ].rgbReserved = 0;
  }

  memset(&aviinfo, 0, sizeof(aviinfo));
  aviinfo.fccType = streamtypeVIDEO;
  aviinfo.fccHandler = 0;
  // use highest possible accuracy for dwRate/dwScale
  aviinfo.dwScale = (DWORD)((double) 0x7FFFFFFF / s->fps);
  aviinfo.dwRate = cvRound(s->fps * aviinfo.dwScale);
  aviinfo.rcFrame.top = aviinfo.rcFrame.left = 0;
  aviinfo.rcFrame.right = frameSize.w;
  aviinfo.rcFrame.bottom = frameSize.h;

  if (AVIFileCreateStream(s->avifile, &s->uncompressed, &aviinfo) == AVIERR_OK) {
    AVICOMPRESSOPTIONS copts, *pcopts = &copts;
    copts.fccType = streamtypeVIDEO;
    copts.fccHandler = s->fourcc != -1 ? s->fourcc : 0;
    copts.dwKeyFrameEvery = 1;
    copts.dwQuality = 10000;
    copts.dwBytesPerSecond = 0;
    copts.dwFlags = AVICOMPRESSF_VALID;
    copts.lpFormat = &bmih;
    copts.cbFormat = (isColor ? sizeof(BITMAPINFOHEADER) : sizeof(bmih));
    copts.lpParms = 0;
    copts.cbParms = 0;
    copts.dwInterleaveEvery = 0;

    if (s->fourcc != -1 || AVISaveOptions(0, 0, 1, &s->uncompressed, &pcopts) == TRUE) {
      if (AVIMakeCompressedStream(&s->compressed, s->uncompressed, pcopts, 0) == AVIERR_OK &&
          AVIStreamSetFormat(s->compressed, 0, &bmih, sizeof(bmih)) == AVIERR_OK) {
        s->fps = s->fps;
        s->fourcc = (int) copts.fccHandler;
        frameSize = frameSize;
        s->tempFrame = MALLOC(uchar, frameSize.w * frameSize.h * (isColor ? 3 : 1));
        return TRUE;
      }
    }
  }

  return FALSE;
}

int CvVideoWriter_VFW_writeFrame(CvVideoWriter_VFW* s, const unsigned char* image, int widthStep)
{
  int result = FALSE;
  int width = s->width;
  int height = s->height;
  int nChannels = s->channel;
  CV_FUNCNAME("CvVideoWriter_VFW_writeFrame");

  __BEGIN__;

  if (!image) {
    EXIT;
  }

  if (!s->compressed && !CvVideoWriter_VFW_createStreams(s, cvSize(width, height), nChannels > 1)) {
    EXIT;
  }

  //if ( width != s->width || height != s->height )
  //  CV_ERROR( CV_StsUnmatchedSizes, "image size is different from the currently set s->data size" );

  if (nChannels != s->channel ||
      widthStep != cvAlign(width * nChannels, 4)) {
    //cvConvertImage( image, tempFrame, image->origin == 0 ? CV_CVTIMG_FLIP : 0 );
    //image = (const unsigned char*)tempFrame;
  }

  // µßµ¹
  MEMCPY2D(height, width * nChannels, (image + widthStep * (height - 1)), -widthStep, s->data, width * nChannels);

  result = AVIStreamWrite(s->compressed, s->pos++, 1, s->data,
      s->imageSize, AVIIF_KEYFRAME, 0, 0) == AVIERR_OK;

  __END__;

  return result;
}
typedef int (*capcam_F)(struct capdev*, const unsigned char*, int);
CvVideoWriter_VFW* CvVideoWriter_VFW_open(const char* filename, int _fourcc, double _fps, CvSize frameSize, int nChannels)
{
  CvVideoWriter_VFW* s = NULL;
  int isColor = nChannels > 1;
  icvInitCapture_VFW();
  s = MALLOC(CvVideoWriter_VFW, 1);
  MEMSET(s, 0, 1);

  if (s) {
    if (AVIFileOpen(&s->avifile, filename, OF_CREATE | OF_WRITE, 0) == AVIERR_OK) {
      s->fourcc = _fourcc;
      s->fps = _fps;

      if (frameSize.w > 0 && frameSize.h > 0 &&
          !CvVideoWriter_VFW_createStreams(s, frameSize, isColor)) {
        CvVideoWriter_VFW_close(s);
        return 0;
      }

      s->height = frameSize.h;
      s->width = frameSize.w;
      s->channel = nChannels;
      s->imageSize = frameSize.w * frameSize.h * nChannels;
      s->data = MALLOC(unsigned char, s->imageSize);
      s->writeFrame = (capcam_F)CvVideoWriter_VFW_writeFrame;
      s->Close = (Close_F)CvVideoWriter_VFW_close;
    }
  }

  return s;
}

capdev* cap_create_avi(const char* filename, int height, int width, int nChannels)
{
  CvVideoWriter_VFW* cap = NULL;
  int fcc = -1;
  fcc = CV_FOURCC('c', 'v', 'i', 'd'); // Cinepak Codec by Radius
  //fcc = CV_FOURCC('i', 'v', '3', '2'); // Intel Indeo (R) Video R3.2
  //fcc = CV_FOURCC('i', 'v', '4', '1'); // Intel Indeo (R) Video R3.2
  //fcc = CV_FOURCC('i', 'y', 'u', 'v'); // Intel Indeo (R) Video R3.2
  cap = CvVideoWriter_VFW_open(filename, fcc, 5, cvSize(width, height), nChannels);
  // height, width, channel;
  return (capdev*)cap;
}

#endif

#endif // _CAPVFW_H_
