// video.cpp : Defines the entry point for the DLL application.
//
#include <windows.h>
#include "Eroadvideo.h"
// video.cpp : Defines the entry point for the DLL application.
//
//#include <winioctl.h>
//#include <time.h>
//#include "ezusbsys.h"
//#include <stdio.h>
//#include <stdlib.h>
//#include <dbt.h>
//#include <Rpcdce.h>
#include "DebugAssistant.h"
#include "CriticalSection.h"
#include "stdafx.h"
bool FlagVerticalMirror = 0;//垂直镜像标志
#include "CyAPI.h"
#include <process.h>
#include "Raw2Rgb.h"
#pragma comment(lib, "rpcrt4.lib")
#pragma comment(lib, "CyAPI.lib")
#pragma comment(lib, "Raw2Rgb.lib")
BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
  switch (ul_reason_for_call) {
  case DLL_PROCESS_ATTACH:
  case DLL_THREAD_ATTACH:
  case DLL_THREAD_DETACH:
  case DLL_PROCESS_DETACH:
    break;
  }
  return TRUE;
}
CriticalSection crEvent;
CriticalSection crEventRead;
CriticalSection crExit;
DWORD VideoWidth = 640;
DWORD VideoHeight = 480;
DWORD WndWidth = 640;
DWORD WndHeight = 480;
LPVOID m_param = NULL;
//DWORD SumFrameCount=0;
DWORD number;
PBITMAPINFO pBitMapInfo = NULL;
BITMAPINFOHEADER bmfInfoHeader;
BOOL AutoPlayFlag = FALSE;
BOOL ImageDisposal_ing = FALSE;
BOOL GetPhotoing = FALSE;
int light = 99;
int contrast = 53;
int saturation = 512;
int uplight = 0;
int upcontrast = 255;
int ThresholdValue = 255;
HANDLE hThread = NULL;
HANDLE hShowThread = NULL;
BYTE* pSensorData = NULL;
PFCALLBACK gCallBack = 0;
//CCriticalSection g_cs;
unsigned __stdcall ReadOV3620ForThread(LPVOID lpParameter);
unsigned __stdcall ShowThread(LPVOID lpParameter);
void RGB_BrightAdjust(BYTE* pRGB_In, BYTE* pRGB_Out, int Bright, long DateSize);
void RGB_ContrastAdjust(BYTE* pRGB_In, BYTE* pRGB_Out, int Contrast, int ThresholdValue, long DateSize);
void RGB_ContrastAdjust(BYTE* pRGB_In, BYTE* pRGB_Out, int Contrast, int ThresholdValue, long DateSize)
{
  int Count, tempContrast, Ytemp;
  if (0 < Contrast && Contrast < 255) {
    tempContrast = 255 * 255 / (256 - Contrast) - 255 ;
  }
  else {
    tempContrast = Contrast;
  }
  for (Count = 0; Count < DateSize * 3; Count++) {
    Ytemp = pRGB_In[Count] + (pRGB_In[Count] - ThresholdValue) * Contrast / 255;
    if (Ytemp > 255) {
      pRGB_Out[Count] = 255;
    }
    else if (Ytemp < 0) {
      pRGB_Out[Count] = 0;
    }
    else {
      pRGB_Out[Count] = (BYTE)(Ytemp);
    }
  }
}
void RGB_BrightAdjust(BYTE* pRGB_In, BYTE* pRGB_Out, int Bright, long DateSize)
{
  int Count, Ytemp;
  for (Count = 0; Count < DateSize * 3; Count++) {
    Ytemp = pRGB_In[Count] + Bright ;
    if (Ytemp > 255) {
      pRGB_Out[Count] = 255;
    }
    else if (Ytemp < 0) {
      pRGB_Out[Count] = 0;
    }
    else {
      pRGB_Out[Count] = (BYTE)(Ytemp);
    }
  }
}
VIDEO_API BOOL CheckSensor()
{
  WORD deviceNum = 0;
  CCyUSBDevice* CheckSensorDev = new CCyUSBDevice(NULL);
  int devices = CheckSensorDev->DeviceCount();
  // if (CheckSensorDev == NULL)
  // {
  // DEBUG_INFO1(_T("%s"),_T("CheckSensorDev is NULL"));
  // }
  // else
  // {
  // DEBUG_INFO1(_T("%s"),_T("CheckSensorDev is not NULL"));
  // }
  // if (devices >0)
  // {
  // DEBUG_INFO1(_T("%s"),_T("devices is >0"));
  // }
  // else
  // {
  // DEBUG_INFO1(_T("%s"),_T("devices is <=0"));
  // }
  //CheckSensorDev->ReConnect();
  for (deviceNum = 0; deviceNum < devices; deviceNum++) {
    if (CheckSensorDev->Open((UCHAR)deviceNum)) {
      CCyControlEndPoint* ept = CheckSensorDev->ControlEndPt;
      ept->Target = TGT_DEVICE;
      ept->ReqType = REQ_VENDOR;
      ept->Direction = DIR_TO_DEVICE;
      ept->ReqCode = 0xa3;
      ept->Value = 0;
      ept->Index = 0;
      char buf[64];
      ZeroMemory(buf, 64);
      LONG bytesToSend = 64;
      ept->Read((PUCHAR)buf, bytesToSend);
      ept->Read((PUCHAR)buf, bytesToSend);
      if (0x02 == buf[11]) {
        DEBUG_INFO1(_T("%s"), _T("our device"));
        CheckSensorDev -> Close();
        delete CheckSensorDev;
        CheckSensorDev = NULL;
        if (pBitMapInfo == NULL) {
          pBitMapInfo = new BITMAPINFO;
        }
        delete CheckSensorDev;
        return TRUE;
      }
      CheckSensorDev -> Close();
      delete CheckSensorDev;
      CheckSensorDev = NULL;
    }
  }
  CheckSensorDev->Close();
  delete CheckSensorDev;
  CheckSensorDev = NULL;
  return FALSE;
}
unsigned __stdcall ReadOV3620ForThread(LPVOID lpParameter)
{
  LONG Readlength = 0;
  BYTE* pSensorDataReplica = new BYTE[307200];
  crEvent.Lock();
  if (pSensorData == NULL) {
    pSensorData = new BYTE[307200];
  }
  crEvent.Unlock();
  for (int a = 0; a < 307200; a++) {
    pSensorDataReplica[a] = 0;
  }
  for (int b = 0; b < 307200; b++) {
    pSensorData[b] = 0;
  }
  CCyUSBDevice* ThreadUSBDevice = new CCyUSBDevice(NULL);
  int devices = ThreadUSBDevice->DeviceCount();
  WORD deviceNum = 0;
  for (deviceNum = 0; deviceNum < devices; deviceNum++) {
    if (ThreadUSBDevice->Open((UCHAR)deviceNum)) {
      CCyControlEndPoint* ept = ThreadUSBDevice->ControlEndPt;
      ept->Target = TGT_DEVICE;
      ept->ReqType = REQ_VENDOR;
      ept->Direction = DIR_TO_DEVICE;
      ept->ReqCode = 0xa3;
      ept->Value = 0;
      ept->Index = 0;
      char buf[64];
      ZeroMemory(buf, 64);
      LONG bytesToSend = 64;
      ept->Read((PUCHAR)buf, bytesToSend);
      ept->Read((PUCHAR)buf, bytesToSend);
      BOOL breakOk = FALSE;
      if (0x02 == buf[11]) {
        CCyControlEndPoint* ept = ThreadUSBDevice->ControlEndPt;
        ept->Target = TGT_DEVICE;
        ept->ReqType = REQ_VENDOR;
        ept->Direction = DIR_TO_DEVICE;
        ept->ReqCode = 0xa2;
        ept->Value = 0;
        ept->Index = 0;
        BYTE buf[64];
        ZeroMemory(buf, 64);
        LONG bytesToSend = 64;
        buf[0] = 0xa2;
        buf[1] = 0x06;
        ept->Write((PUCHAR)buf, bytesToSend);
        OVERLAPPED inOvLap;
        UCHAR* inContext;
        inOvLap.hEvent = CreateEvent(NULL, false, false, _T("CYUSB_IN"));
        breakOk = AutoPlayFlag;
        ThreadUSBDevice->BulkInEndPt->SetXferSize(number);
        inContext = ThreadUSBDevice->BulkInEndPt->BeginDataXfer(pSensorDataReplica, number, &inOvLap);
        if (!ThreadUSBDevice->BulkInEndPt->WaitForXfer(&inOvLap , 1000)) {
          break;
        }
        // SumFrameCount++;
        while (AutoPlayFlag && ThreadUSBDevice) {
          long nTemp = number;
          ThreadUSBDevice->BulkInEndPt->XferData(pSensorDataReplica, nTemp);
          crEvent.Lock();
          if (pSensorData) {
            CopyMemory(pSensorData , pSensorDataReplica , number);
          }
          crEvent.Unlock();
          //delete []pSensorDataReplica;
          //pSensorDataReplica = NULL;
          ZeroMemory(pSensorDataReplica, sizeof(pSensorDataReplica));
        }
        DEBUG_INFO1(_T("%s ") , _T("out while"));
        if (ThreadUSBDevice && breakOk) {
          bool bResult = ThreadUSBDevice->BulkInEndPt->FinishDataXfer(pSensorDataReplica, (long&)number, &inOvLap, inContext);
          DEBUG_INFO1(_T("%s ") , _T("FinishDataXfer"));
        }
        CloseHandle(inOvLap.hEvent);
        inOvLap.hEvent = NULL;
        buf[1] = (char)0x07;
        ept->Write((PUCHAR)buf, bytesToSend);
        DEBUG_INFO1(_T("%s ") , _T("ept->Write((PUCHAR)buf, bytesToSend)"));
      }
      ThreadUSBDevice -> Close();
      delete ThreadUSBDevice;
      ThreadUSBDevice = NULL;
      if (breakOk) {
        DEBUG_INFO1(_T("%s ") , _T(" ReadOV3620ForThread breakOk)"));
        break;
      }
    }
  }
  if (ThreadUSBDevice) {
    delete ThreadUSBDevice;
    ThreadUSBDevice = NULL;
  }
  if (pSensorDataReplica) {
    delete[] pSensorDataReplica;
    pSensorDataReplica = NULL;
  }
  if (pSensorData) {
    delete []pSensorData;
    pSensorData = NULL;
  }
  DEBUG_INFO1(_T("%s ") , _T("ReadOV3620ForThread End!!!"));
  return 0 ;
}
unsigned __stdcall ShowThread(LPVOID lpParameter)
{
  BYTE* pData_RAW_YUV = new BYTE[307200];
  BYTE* pDataRGB = new BYTE[921600];
  for (int a = 0; a < 921600; a++) {
    pDataRGB[a] = 0;
  }
  for (int b = 0; b < 307200; b++) {
    pData_RAW_YUV[b] = 0;
  }
  BYTE m_pLutR[256], m_pLutG[256], m_pLutB[256];
  for (int i = 0; i < 256; i++) {
    m_pLutR[i] = i;
    m_pLutG[i] = i;
    m_pLutB[i] = i;
  }
  pBitMapInfo->bmiHeader.biSize = 40;
  pBitMapInfo->bmiHeader.biWidth = VideoWidth;
  pBitMapInfo->bmiHeader.biHeight = VideoHeight;
  pBitMapInfo->bmiHeader.biPlanes = 1;
  pBitMapInfo->bmiHeader.biBitCount = 24;
  pBitMapInfo->bmiHeader.biCompression = BI_RGB;
  pBitMapInfo->bmiHeader.biSizeImage = VideoWidth * VideoHeight * 3;
  pBitMapInfo->bmiHeader.biXPelsPerMeter = 0;
  pBitMapInfo->bmiHeader.biYPelsPerMeter = 0;
  pBitMapInfo->bmiHeader.biClrUsed = 0;
  pBitMapInfo->bmiHeader.biClrImportant = 0;
  number = VideoWidth * VideoHeight;
  HV_BAYER_CONVERT_TYPE ConvertType = BAYER2RGB_NEIGHBOUR;
  HV_BAYER_LAYOUT Layout = BAYER_GR;
  // Sleep(10);
  uutime L_Timer;
  while (AutoPlayFlag) {
    if (!ImageDisposal_ing && pSensorData) {
      ImageDisposal_ing = TRUE;
      AoutLock lock(&crEvent);
      /*for( int Count=0; Count<number; Count++ )
      {
      if ( pSensorData != NULL )
      pData_RAW_YUV[Count] = pSensorData[number-Count];
      }
      L_Timer.restart();
      ConvertBayer2Rgb(pDataRGB,pData_RAW_YUV,VideoWidth,VideoHeight,ConvertType,m_pLutR,m_pLutG,m_pLutB,true,Layout);
      */
      if (FlagVerticalMirror)
        for (DWORD Count = 0; Count < number; Count++) { //使用ConvertBayer2Rgb时用这个循环来调正方向，否则上下方向反了。
          pData_RAW_YUV[Count] = pSensorData[Count];
        }
      else
        for (DWORD Count = 0; Count < number; Count++) { //使用ConvertBayer2Rgb时用这个循环来调正方向，否则上下方向反了。
          pData_RAW_YUV[Count] = pSensorData[number - Count];
        }
      if (FlagVerticalMirror) {
        Layout = BAYER_BG;
        ConvertBayer2Rgb(pDataRGB, pData_RAW_YUV, VideoWidth, VideoHeight, ConvertType, m_pLutR, m_pLutG, m_pLutB, true, Layout);
      }
      else {
        Layout = BAYER_GR;
        ConvertBayer2Rgb(pDataRGB, pData_RAW_YUV, VideoWidth, VideoHeight, ConvertType, m_pLutR, m_pLutG, m_pLutB, true, Layout);
      }
      DEBUG_INFO1(_T(" ConvertBayer2Rgb = %f ") , L_Timer.elapsed());
      L_Timer.restart();
      RGB_ContrastAdjust(pDataRGB, pDataRGB, upcontrast, ThresholdValue, number);
      DEBUG_INFO1(_T(" RGB_ContrastAdjust = %f ") , L_Timer.elapsed());
      //L_Timer.restart();
      //RGB_BrightAdjust(pDataRGB,pDataRGB,uplight,number);
      //DEBUG_INFO1( _T(" RGB_BrightAdjust = %f ") , L_Timer.elapsed() );
      //L_Timer.restart();
      if (gCallBack != NULL) {
        (*gCallBack)(VideoWidth, VideoHeight, pDataRGB, m_param);
      }
      //DEBUG_INFO1( _T(" StretchDIBits = %f ") , L_Timer.elapsed() );
      ImageDisposal_ing = FALSE;
      for (int a = 0; a < 921600; a++) {
        pDataRGB[a] = 0;
      }
      for (int b = 0; b < 307200; b++) {
        pData_RAW_YUV[b] = 0;
      }
    }
    Sleep(40);
  }
  if (pBitMapInfo) {
    delete pBitMapInfo;
    pBitMapInfo = NULL;
  }
  delete[] pData_RAW_YUV;
  pData_RAW_YUV = NULL;
  delete[] pDataRGB;
  pDataRGB = NULL;
  DEBUG_INFO1(_T("%s ") , _T("ShowThread End!!!"));
  return 0;
}
VIDEO_API BOOL GetPhoto()
{
  return GetPhotoing;
}
VIDEO_API void SethandLight(int light)
{
  uplight = light;
}
VIDEO_API void SethandContrast(int contrast)
{
  upcontrast = contrast;
}
VIDEO_API void SetThresholdValue(int value)
{
  ThresholdValue = value;
}
VIDEO_API int GethandLight()
{
  return uplight;
}
VIDEO_API int GethandContrast()
{
  return upcontrast;
}
VIDEO_API int GetThresholdValue()
{
  return ThresholdValue;
}
VIDEO_API BOOL SetSensorAEWorAEB(BYTE Light)
{
  BYTE Buf[64];
  ZeroMemory(Buf, sizeof(Buf));
  CCyUSBDevice* SetDev = new CCyUSBDevice(NULL);
  CCyControlEndPoint* ept = SetDev->ControlEndPt;
  ept->Target = TGT_DEVICE;
  ept->ReqType = REQ_VENDOR;
  ept->Direction = DIR_TO_DEVICE;
  ept->ReqCode = 0xa2;
  LONG bytesToSend = 64;
  Buf[0x00] = 0xa2;
  Buf[0x01] = 0x01;
  Buf[0x02] = Light;
  Buf[0x07] = Light;
  BOOL ret = ept->Write(Buf, bytesToSend);
  delete SetDev;
  SetDev = NULL;
  return ret;
}
VIDEO_API BOOL SetSensorAGC(BYTE Contrast)
{
  CCyUSBDevice* SetDev = new CCyUSBDevice(NULL);
  CCyControlEndPoint* ept = SetDev->ControlEndPt;
  BYTE Buf[64];
  ZeroMemory(Buf, sizeof(Buf));
  ept->Target = TGT_DEVICE;
  ept->ReqType = REQ_VENDOR;
  ept->Direction = DIR_TO_DEVICE;
  ept->ReqCode = 0xa2;
  LONG bytesToSend = 64;
  Buf[0] = 0xa2;
  Buf[1] = 0X04;
  Buf[2] = 0x02;
  ept->Write(Buf, bytesToSend);
  ZeroMemory(Buf, sizeof(Buf));
  Buf[0x00] = 0xa2;
  Buf[0x01] = 0X03;
  Buf[0x02] = Contrast;
  Buf[0x08] = Contrast;
  BOOL ret = ept->Write(Buf, bytesToSend);
  delete SetDev;
  SetDev = NULL;
  return ret;
}
VIDEO_API BOOL SetSensorAEC(int Saturation)
{
  CCyUSBDevice* SetDev = new CCyUSBDevice(NULL);
  CCyControlEndPoint* ept = SetDev->ControlEndPt;
  BYTE Buf[64];
  ZeroMemory(Buf, sizeof(Buf));
  Buf[0x00] = 0xa2;
  Buf[0x01] = 0X05;
  Buf[0x02] = (BYTE)(Saturation >> 8);
  Buf[0x03] = (BYTE)Saturation;
  Buf[0x08] = (BYTE)(Saturation >> 8);
  Buf[0x09] = (BYTE)Saturation;
  ept->Target = TGT_DEVICE;
  ept->ReqType = REQ_VENDOR;
  ept->Direction = DIR_TO_DEVICE;
  ept->ReqCode = 0xa2;
  LONG bytesToSend = 64;
  BOOL ret = ept->Write(Buf, bytesToSend);
  delete SetDev;
  SetDev = NULL;
  return ret;
}
VIDEO_API int GetSaturation()
{
  return saturation;
}
VIDEO_API int GetContrast()
{
  return contrast;
}
VIDEO_API int GetLight()
{
  return light;
}
VIDEO_API BOOL SetManuallight()
{
  CCyUSBDevice* SetDev = new CCyUSBDevice(NULL);
  CCyControlEndPoint* ept = SetDev->ControlEndPt;
  ept->Target = TGT_DEVICE;
  ept->ReqType = REQ_VENDOR;
  ept->Direction = DIR_TO_DEVICE;
  ept->ReqCode = 0xa2;
  LONG bytesToSend = 64;
  BYTE Buf[64];
  ZeroMemory(Buf, sizeof(Buf));
  Buf[0] = 0xa2;
  Buf[1] = 0X04;
  Buf[2] = 0x01;
  ept->Write(Buf, bytesToSend);
  ZeroMemory(Buf, sizeof(Buf));
  Buf[0] = 0xa2;
  Buf[1] = 0X02;
  Buf[2] = 0x01;
  BOOL ret = ept->Write(Buf, bytesToSend);
  delete SetDev;
  SetDev = NULL;
  return ret;
}
VIDEO_API BOOL SetManualcontrast(BOOL state)
{
  CCyUSBDevice* SetDev = new CCyUSBDevice(NULL);
  CCyControlEndPoint* ept = SetDev->ControlEndPt;
  ept->Target = TGT_DEVICE;
  ept->ReqType = REQ_VENDOR;
  ept->Direction = DIR_TO_DEVICE;
  ept->ReqCode = 0xa2;
  LONG bytesToSend = 64;
  BYTE Buf[64];
  ZeroMemory(Buf, sizeof(Buf));
  Buf[0] = 0xa2;
  Buf[1] = 0X02;
  if (state) {
    Buf[2] = 0x02;
  }
  else {
    Buf[2] = 0x01;
  }
  BOOL ret = ept->Write(Buf, bytesToSend);
  delete SetDev;
  SetDev = NULL;
  return ret;
}
VIDEO_API BOOL SetManualsaturation(BOOL state)
{
  CCyUSBDevice* SetDev = new CCyUSBDevice(NULL);
  CCyControlEndPoint* ept = SetDev->ControlEndPt;
  ept->Target = TGT_DEVICE;
  ept->ReqType = REQ_VENDOR;
  ept->Direction = DIR_TO_DEVICE;
  ept->ReqCode = 0xa2;
  LONG bytesToSend = 64;
  BYTE Buf[64];
  ZeroMemory(Buf, sizeof(Buf));
  Buf[0] = 0xa2;
  Buf[1] = 0x04;
  if (state) {
    Buf[2] = 0x02;
  }
  else {
    Buf[2] = 0x01;
  }
  BOOL ret = ept->Write(Buf, bytesToSend);
  delete SetDev;
  SetDev = NULL;
  return ret;
}
VIDEO_API BOOL SetFrameRate(int rate)
{
  CCyUSBDevice* SetDev = new CCyUSBDevice(NULL);
  CCyControlEndPoint* ept = SetDev->ControlEndPt;
  ept->Target = TGT_DEVICE;
  ept->ReqType = REQ_VENDOR;
  ept->Direction = DIR_TO_DEVICE;
  ept->ReqCode = 0xa5;
  LONG bytesToSend = 64;
  BYTE Buf[64];
  ZeroMemory(Buf, sizeof(Buf));
  Buf[0] = 0xa5;
  Buf[1] = 0x11;
  switch (rate) {
  case 60:
    Buf[2] = 0x00;
    break;
  case 30:
    Buf[2] = 0x01;
    break;
  case 15:
    Buf[2] = 0x03;
    break;
  default:
    Buf[2] = 0x00;
    break;
  }
  BOOL ret = ept->Write(Buf, bytesToSend);
  delete SetDev;
  SetDev = NULL;
  return ret;
}
BOOL bf = TRUE;
VIDEO_API BOOL SetVideoModel(BYTE VideoModel)
{
  CCyUSBDevice* SetDev = new CCyUSBDevice(NULL);
  CCyControlEndPoint* ept = SetDev->ControlEndPt;
  if (bf == TRUE) {
    pBitMapInfo = new BITMAPINFO;
    bf = FALSE;
  }
  ept->Target = TGT_DEVICE;
  ept->ReqType = REQ_VENDOR;
  ept->Direction = DIR_TO_DEVICE;
  ept->ReqCode = 0xa2;
  LONG bytesToSend = 64;
  BYTE Buf[64];
  if (hThread == NULL || hShowThread == NULL) {
    ZeroMemory(Buf, sizeof(Buf));
    Buf[0] = 0xa2;
    Buf[1] = 0x06;
    ept->Write(Buf, bytesToSend);
  }
  ZeroMemory(Buf, sizeof(Buf));
  Buf[0] = 0xa2;
  Buf[1] = 0X08;
  BOOL ret;
  switch (VideoModel) {
  case 0:
    VideoWidth = 640;
    VideoHeight = 480;
    Buf[2] = VideoModel;
    break;
  case 1:
    VideoWidth = 320;
    VideoHeight = 240;
    Buf[2] = VideoModel;
    break;
  default:
    VideoWidth = 640;
    VideoHeight = 480;
    Buf[2] = VideoModel;
    break;
  }
  ret = ept->Write(Buf, bytesToSend);
  pBitMapInfo->bmiHeader.biSize = 40;
  pBitMapInfo->bmiHeader.biWidth = VideoWidth;
  pBitMapInfo->bmiHeader.biHeight = VideoHeight;
  pBitMapInfo->bmiHeader.biPlanes = 1;
  pBitMapInfo->bmiHeader.biBitCount = 24;
  pBitMapInfo->bmiHeader.biCompression = BI_RGB;
  pBitMapInfo->bmiHeader.biSizeImage = VideoWidth * VideoHeight * 3;
  pBitMapInfo->bmiHeader.biXPelsPerMeter = 0;
  pBitMapInfo->bmiHeader.biYPelsPerMeter = 0;
  pBitMapInfo->bmiHeader.biClrUsed = 0;
  pBitMapInfo->bmiHeader.biClrImportant = 0;
  number = VideoWidth * VideoHeight;
  delete SetDev;
  SetDev = NULL;
  return ret;
}
VIDEO_API void ExitVideo()
{
  //AoutLock lock( &crExit );
  AutoPlayFlag = FALSE;
  if (NULL != hThread) {
    if (WaitForSingleObject(hThread , 300) != WAIT_OBJECT_0) {
      TerminateThread(hThread , 0);
      DEBUG_INFO1(_T("%s ") , _T(" WaitForSingsleObject ReadOV3620ForThread end)"));
    }
    //WaitForSingleObject( hThread , INFINITE );
    hThread = NULL;
  }
  if (NULL != hShowThread) {
    if (WaitForSingleObject(hShowThread , 300) != WAIT_OBJECT_0) {
      TerminateThread(hShowThread , 0);
      DEBUG_INFO1(_T("%s ") , _T(" ShowThread TerminateThread)"));
    }
    //WaitForSingleObject( hShowThread , INFINITE );
    hShowThread = NULL;
  }
  AoutLock lock(&crEvent);
  bf = TRUE;
  gCallBack = 0;
  if (pSensorData != NULL) {
    delete []pSensorData;
    pSensorData = NULL;
    DEBUG_INFO1(_T("%s"), _T("Clear pSensorData"));
  }
  if (pBitMapInfo != NULL) {
    delete pBitMapInfo;
    pBitMapInfo = NULL;
    DEBUG_INFO1(_T("%s"), _T("Clear pBitMapInfo"));
  }
  m_param = NULL;
  DEBUG_INFO1(_T("%s"), _T("ExitVideo finish"));
  // CCyUSBDevice *ResetSensor = new CCyUSBDevice(NULL);
  // ResetSensor->ReConnect();
  // delete ResetSensor;
  // ResetSensor = NULL;
}
VIDEO_API BOOL StartVideo(PFCALLBACK Func, LPVOID param)
{
  AutoPlayFlag = TRUE;
  ImageDisposal_ing = FALSE;
  // if(Func==NULL)
  // return FALSE;
  crEvent.Lock();
  m_param = param;
  gCallBack = Func;
  crEvent.Unlock();
  return TRUE;
}
VIDEO_API void ShowVideo(HWND hWnd, DWORD Width, DWORD Height)
{
  WndWidth = Width;
  WndHeight = Height;
  // GUID GUID_CLASS_INPUT;
  // DEV_BROADCAST_DEVICEINTERFACE m_NotificationFilter;
  // ZeroMemory( &m_NotificationFilter, sizeof( m_NotificationFilter ) );
  // UuidFromString( (unsigned char*)"AE18AA60-7F6A-11d4-97DD-00010229B959", &GUID_CLASS_INPUT );
  // m_NotificationFilter.dbcc_size = sizeof( m_NotificationFilter );
  // m_NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
  // m_NotificationFilter.dbcc_classguid = GUID_CLASS_INPUT;
  // RegisterDeviceNotification( hWnd, &m_NotificationFilter,DEVICE_NOTIFY_WINDOW_HANDLE);
  if (NULL != hThread) {
    if (WaitForSingleObject(hThread , 1000) != WAIT_OBJECT_0) {
      TerminateThread(hThread , 100);
    }
    hThread = NULL;
  }
  if (NULL != hShowThread) {
    if (WaitForSingleObject(hShowThread , 1000) != WAIT_OBJECT_0) {
      TerminateThread(hShowThread , 100);
    }
    hShowThread = NULL;
  }
  unsigned int dwThreadID1;
  hThread = (HANDLE)_beginthreadex(NULL, 0, ReadOV3620ForThread, NULL, 0, &dwThreadID1);
  //CloseHandle(hThread);
  //Sleep(10);
  unsigned int dwThreadID2;
  hShowThread = (HANDLE)_beginthreadex(NULL, 0, ShowThread, NULL, 0, &dwThreadID2);
  //CloseHandle(hShowThread);
  //Sleep(10);
}
VIDEO_API BOOL GetBMPPhoto(TCHAR* filename)
{
  if (hThread == NULL || hShowThread == NULL) {
    return FALSE;
  }
  BYTE* pData_RAW_YUV = new BYTE[307200];
  BYTE* pDataRGB = new BYTE[921600];
  for (int a = 0; a < 307200; a++) {
    pData_RAW_YUV[a] = 0;
  }
  for (int b = 0; b < 921600; b++) {
    pDataRGB[b] = 0;
  }
  HV_BAYER_CONVERT_TYPE ConvertType = BAYER2RGB_NEIGHBOUR;
  HV_BAYER_LAYOUT Layout = BAYER_GR;
  BYTE m_pLutR[256];
  BYTE m_pLutG[256];
  BYTE m_pLutB[256];
  for (int i = 0; i < 256; i++) {
    m_pLutR[i] = i;
    m_pLutG[i] = i;
    m_pLutB[i] = i;
  }
  DWORD ytemp;
  for (ytemp = 0; ytemp < VideoWidth * VideoHeight; ytemp++) {
    pData_RAW_YUV[ytemp] = pSensorData[VideoWidth * VideoHeight - ytemp];
  }
  //ConvertBayer2Rgb(pDataRGB,pData_RAW_YUV,VideoWidth,VideoHeight,ConvertType,m_pLutR,m_pLutG,m_pLutB,true,Layout);
  if (FlagVerticalMirror) {
    Layout = BAYER_BG;
    ConvertBayer2Rgb(pDataRGB, pData_RAW_YUV, VideoWidth, VideoHeight, ConvertType, m_pLutR, m_pLutG, m_pLutB, true, Layout);
  }
  else {
    Layout = BAYER_GR;
    ConvertBayer2Rgb(pDataRGB, pData_RAW_YUV, VideoWidth, VideoHeight, ConvertType, m_pLutR, m_pLutG, m_pLutB, true, Layout);
  }
  RGB_ContrastAdjust(pDataRGB, pDataRGB, upcontrast, ThresholdValue, number);
  RGB_BrightAdjust(pDataRGB, pDataRGB, uplight, number);
  HANDLE hf = CreateFile(filename, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, NULL, NULL);
  if (hf == INVALID_HANDLE_VALUE) {
    delete []pData_RAW_YUV;
    delete []pDataRGB;
    return FALSE;
  }
  BITMAPFILEHEADER bfh;
  memset(&bfh, 0, sizeof(bfh));
  bfh.bfType = 'MB';
  bfh.bfSize = sizeof(bfh) + VideoWidth * VideoHeight * 3 + sizeof(BITMAPINFOHEADER);
  bfh.bfOffBits = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);
  DWORD dwWritten = 0;
  WriteFile(hf, &bfh, sizeof(bfh), &dwWritten, NULL);
  BITMAPINFOHEADER bih;
  memset(&bih, 0, sizeof(bih));
  bih.biSize = sizeof(bih);
  bih.biWidth = VideoWidth;
  bih.biHeight = VideoHeight;
  bih.biPlanes = 1;
  bih.biBitCount = 24;
  dwWritten = 0;
  WriteFile(hf, &bih, sizeof(bih), &dwWritten, NULL);
  dwWritten = 0;
  WriteFile(hf, pDataRGB, VideoWidth * VideoHeight * 3, &dwWritten, NULL);
  CloseHandle(hf);
  delete []pData_RAW_YUV;
  delete []pDataRGB;
  return TRUE;
}
VIDEO_API BOOL SendMsg(BYTE msg)
{
  BYTE Buf[64];
  ZeroMemory(Buf, sizeof(Buf));
  CCyUSBDevice* BitControlDev = new CCyUSBDevice(NULL);
  BOOL ret = FALSE;
  if (BitControlDev->IsOpen()) {
    CCyControlEndPoint* ept = BitControlDev->ControlEndPt;
    if (ept != NULL) {
      ept->Target = TGT_DEVICE;
      ept->ReqType = REQ_VENDOR;
      ept->Direction = DIR_TO_DEVICE;
      ept->ReqCode = 0xa2;
      LONG bytesToSend = 64;
      Buf[0] = 0xa2;
      Buf[1] = 0x0f;
      Buf[2] = msg;
      ret = ept->Write(Buf, bytesToSend);
    }
  }
  delete BitControlDev;
  BitControlDev = NULL;
  return ret;
}
VIDEO_API BOOL SetOther(BYTE msg)
{
  BYTE Buf[64];
  ZeroMemory(Buf, sizeof(Buf));
  CCyUSBDevice* BitControlDev = new CCyUSBDevice(NULL);
  CCyControlEndPoint* ept = BitControlDev->ControlEndPt;
  ept->Target = TGT_DEVICE;
  ept->ReqType = REQ_VENDOR;
  //ept->Direction = DIR_TO_DEVICE;
  ept->ReqCode = 0xa2;
  LONG bytesToSend = 64;
  Buf[0] = 0xa2;
  Buf[1] = 0x10;
  Buf[2] = msg;
  BOOL ret = ept->Write(Buf, bytesToSend);
  if (msg & 0x80) {
    FlagVerticalMirror = 1; //垂直镜像标志
  }
  else {
    FlagVerticalMirror = 0; //垂直镜像标志
  }
  delete BitControlDev;
  return ret;
}

