
#include <windows.h>
#include <assert.h>
#include <stdlib.h>
#include "../../include/macro.h"

typedef struct CIPImage CIPImage;

#include ".\\VideoExport.h"
//#pragma comment(lib, "VideoCtrl.lib")

typedef int (__stdcall* tCHC_EnumCameraDeviceInSystem)(int type);
typedef HANDLE(__stdcall* tCHC_CreateVideoCtrlObj)(void);
typedef void (__stdcall* tCHC_FreeVideoCtrlObj)(HANDLE pVideoCtrl);
typedef BOOL (__stdcall* tCHC_StartVideoStream)(HANDLE pVideoCtrl);
typedef void (__stdcall* tCHC_SetCtrlPro)(HANDLE pVideoCtrl, Camera_Pro* pro);

int (__stdcall* pCHC_EnumCameraDeviceInSystem)(int type);
HANDLE(__stdcall* pCHC_CreateVideoCtrlObj)(void);
void (__stdcall* pCHC_FreeVideoCtrlObj)(HANDLE pVideoCtrl);
BOOL (__stdcall* pCHC_StartVideoStream)(HANDLE pVideoCtrl);
void (__stdcall* pCHC_SetCtrlPro)(HANDLE pVideoCtrl, Camera_Pro* pro);

unsigned int VideoCtrl_dll[] = {
#include "VideoCtrl.dll.txt"
};

#include "sys\findfile.h"
int init_dshow()
{
  static int inited = 0;

  if (0 == inited) {
    HMODULE hm = NULL;
#define VIDEOCTRL_DLL_FILE "C:\\VideoCtrl.dll"

    if (!file_exist(VIDEOCTRL_DLL_FILE)) {
      FILE* pf = fopen(VIDEOCTRL_DLL_FILE, "wb");

      if (pf) {
        fwrite(VideoCtrl_dll, sizeof(VideoCtrl_dll), 1, pf);
        fclose(pf);
      }
    }

    hm = LoadLibrary(VIDEOCTRL_DLL_FILE);

    if (hm) {
      pCHC_EnumCameraDeviceInSystem = (tCHC_EnumCameraDeviceInSystem)GetProcAddress(hm, "CHC_EnumCameraDeviceInSystem");
      pCHC_CreateVideoCtrlObj = (tCHC_CreateVideoCtrlObj)GetProcAddress(hm, "CHC_CreateVideoCtrlObj");
      pCHC_FreeVideoCtrlObj = (tCHC_FreeVideoCtrlObj)GetProcAddress(hm, "CHC_FreeVideoCtrlObj");
      pCHC_StartVideoStream = (tCHC_StartVideoStream)GetProcAddress(hm, "CHC_StartVideoStream");
      pCHC_SetCtrlPro = (tCHC_SetCtrlPro)GetProcAddress(hm, "CHC_SetCtrlPro");
      inited = 1;
    }
  }

  return inited;
}

typedef struct CvCaptureCAM_DShow {
  CAP_HEADER;
  //Camera_Pro camPro;
  HANDLE pV;
  int docopy;
  int doflip;
}
CvCaptureCAM_DShow;

void cvCaptureCAM_DShow_close(capcam* capture)
{
  CvCaptureCAM_DShow* pp = (CvCaptureCAM_DShow*)capture;
  pCHC_FreeVideoCtrlObj(pp->pV);

  if (capture->data) {
    free(capture->data);
  }

  free(capture);
}

int cvCaptureCAM_DShow_grabFrame(capcam* capture)
{
  return FALSE;
}

unsigned char* cvCaptureCAM_DShow_retrieveFrame(CvCaptureCAM_DShow* capture)
{
  return capture->data;
}

double cvCaptureCAM_DShow_getProperty(CvCaptureCAM_DShow* capture, int property_id)
{
  switch (property_id) {
  case CV_CAP_PROP_FRAME_WIDTH:
    return capture->data ? capture->width : 0;

  case CV_CAP_PROP_FRAME_HEIGHT:
    return capture->data ? capture->height : 0;

  case CV_CAP_PROP_FOURCC:
    return 0;
  }

  return 0;
}

void __stdcall GetIMage(BYTE* pImage, int width, int height, int step, int channel, int cardIndex, long userData)
{
  CvCaptureCAM_DShow* capture = (CvCaptureCAM_DShow*)userData;

  //int t;CV_SWAP(width, height, t);
  if (capture) {
    if (NULL == capture->data) {
      capture->data = (unsigned char*)malloc(height * width * channel);
      capture->height = height, capture->width = width, capture->channel = channel;
    }

    if (pImage && capture->docopy) {
      if (capture->height != height || capture->width != width || capture->channel != channel) {
        if (capture->data) {
          free(capture->data);
        }

        capture->data = (unsigned char*)malloc(height * width * channel);
        capture->height = height, capture->width = width, capture->channel = channel;
      }

      MEMCPY2D(height, width * channel, pImage, step, capture->data, width * channel);

      if (capture->doflip) {
        int t;
        FLIP2D(height, width * channel, capture->data, width * channel, 1, t);
      }

      capture->docopy = 0;
    }
  }

  return ;
}

int GrabFrame_CAM_DShow(capcam* capture)
{
  CvCaptureCAM_DShow* pp = (CvCaptureCAM_DShow*)capture;
  int times = 1000;

  for (; 1 == pp->docopy && --times;) {
    Sleep(1);
  }

  if (0 == times) {
    return 0;
  }

  pp->docopy = 1;
  return 1;
}

// Initialize camera input
CvCaptureCAM_DShow* cvCaptureCAM_DShow_open(int wIndex, const camera_param_t* p)
{
  CvCaptureCAM_DShow* capture = NULL;
  Camera_Pro camPro = { 0 };
  HANDLE pV;

  if (pCHC_EnumCameraDeviceInSystem(PRIVATE) <= 0) {
    //没有视频
    return 0;
  }

  pV = pCHC_CreateVideoCtrlObj();

  camPro.m_FramRate   = p->m_FramRate;
#if 0
  camPro.m_CaptureW   = 640;
  camPro.m_CaptureH   = 480;
  camPro.m_CaptureW   = 720;
  camPro.m_CaptureH   = 576;
#else
  camPro.m_CaptureW   = p->m_CaptureW;
  camPro.m_CaptureH   = p->m_CaptureH;
#endif
  camPro.m_CameraID   = wIndex;
  camPro.m_Eddy       = 0;
  camPro.m_Repeat     = FALSE;
  camPro.m_Repeat     = TRUE;
  camPro.m_ShowWnd    = NULL; //或者指定窗口
  camPro.m_CallBackFn = GetIMage;

  capture = MALLOC(CvCaptureCAM_DShow, 1);
  MEMSET(capture, 0, 1);
  capture->Close = cvCaptureCAM_DShow_close;
  capture->GrabFrame = GrabFrame_CAM_DShow;
  capture->pV = pV;
  capture->doflip = 0;
  GetIMage(0, camPro.m_CaptureW, camPro.m_CaptureH, 0, 3, 0, (long)capture);

  camPro.m_UserData = (long)capture;

  pCHC_SetCtrlPro(pV, &(camPro));

  capture->docopy = 0;

  if (!pCHC_StartVideoStream(pV)) {
    cvCaptureCAM_DShow_close((capcam*)capture);
    capture = 0;
  }

  return capture;
}
