#define VIDEO_API
#include <windows.h>
#include "EroadVideo.h"
#include "utime.hpp"
#include "DebugAssistant.h"
#include "CriticalSection.h"
#include "CyAPI.h"
#include <process.h>
#include "Raw2Rgb.h"
#pragma comment(lib, "rpcrt4.lib")
#pragma comment(lib, "CyAPI.lib")
#pragma comment(lib, "Raw2Rgb.lib")
#include <math.h>
CriticalSection crEvent;
CriticalSection crEventRead;
CriticalSection crExit;
DWORD VideoWidth = 640;
DWORD VideoHeight = 480;
DWORD WndWidth = 640;
DWORD WndHeight = 480;
LPVOID m_param = NULL;
//DWORD SumFrameCount=0;
DWORD number = VideoWidth * VideoHeight;
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
//检查设备是否连接就需
BOOL CheckSensor()
{
  WORD deviceNum = 0;
  CCyUSBDevice* CheckSensorDev = new CCyUSBDevice(NULL);
  int devices = CheckSensorDev->DeviceCount();
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
  long Readlength = 0;
  BYTE* pSensorDataReplica = new BYTE[614400];//n
  if (pSensorData == NULL) {
    pSensorData = new BYTE[614400]; //n
  }
  ZeroMemory(pSensorDataReplica, 614400);
  ZeroMemory(pSensorData, 614400);
  CCyUSBDevice* ThreadUSBDevice = new CCyUSBDevice(NULL);
  int devices = ThreadUSBDevice->DeviceCount();
  WORD deviceNum = 0;
  Readlength = (number << 1); //n
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
        BYTE buf1[64];
        ZeroMemory(buf1, 64);
        LONG bytesToSend = 64;
        buf1[0] = 0xa2;
        buf1[1] = 0x06;
        ept->Write((PUCHAR)buf1, bytesToSend);
        OVERLAPPED inOvLap;
        UCHAR* inContext;
        inOvLap.hEvent = CreateEvent(NULL, false, false, _T("CYUSB_IN"));
        breakOk = AutoPlayFlag;
        ThreadUSBDevice->BulkInEndPt->SetXferSize(Readlength);
        inContext = ThreadUSBDevice->BulkInEndPt->BeginDataXfer(pSensorDataReplica, Readlength, &inOvLap);
        uutime L_Timer;
        if (!ThreadUSBDevice->BulkInEndPt->WaitForXfer(&inOvLap, 40)) {
          break;
        }
        // ThreadUSBDevice->BulkInEndPt->TimeOut = 40;
        // SumFrameCount++;
        while (AutoPlayFlag && ThreadUSBDevice) {
          Readlength = (number << 1); //n
          long nTemp = Readlength;//n
          L_Timer.restart();
          bool rs = ThreadUSBDevice->BulkInEndPt->XferData(pSensorDataReplica, nTemp);
          double dbl = L_Timer.elapsed();
          DEBUG_INFO4(_T(" XferData = %f, %d %d %d"), dbl, (int)rs, nTemp, Readlength);
          //crEvent.Lock();
          if (pSensorDataReplica != NULL && (nTemp == Readlength)) { //n
            ZeroMemory(pSensorData, sizeof(pSensorData));
            CopyMemory(pSensorData, pSensorDataReplica, nTemp); //n
            ZeroMemory(pSensorDataReplica, sizeof(pSensorDataReplica));
          }
          //crEvent.Unlock();
        }
        DEBUG_INFO1(_T("%s "), _T("out while"));
        if (ThreadUSBDevice && breakOk) {
          //bool bResult=ThreadUSBDevice->BulkInEndPt->FinishDataXfer(pSensorDataReplica, (long&)number, &inOvLap,inContext);
          bool bResult = ThreadUSBDevice->BulkInEndPt->FinishDataXfer(pSensorDataReplica, (long&)Readlength, &inOvLap, inContext);
          DEBUG_INFO1(_T("%s "), _T("FinishDataXfer"));
        }
        CloseHandle(inOvLap.hEvent);
        inOvLap.hEvent = NULL;
        buf1[1] = (char)0x07;
        ept->Write((PUCHAR)buf, bytesToSend);
        DEBUG_INFO1(_T("%s "), _T("ept->Write((PUCHAR)buf, bytesToSend)"));
      }
      ThreadUSBDevice -> Close();
      delete ThreadUSBDevice;
      ThreadUSBDevice = NULL;
      if (breakOk) {
        DEBUG_INFO1(_T("%s "), _T(" ReadOV3620ForThread breakOk)"));
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
  DEBUG_INFO1(_T("%s "), _T("ReadOV3620ForThread End!!!"));
  return 0 ;
}
#define CLP(_X) ((_X)<0?0 : ((_X)>255?255:(_X)))
// YUV转换为RGB
// R = Y + 1.4075 *（V-128）
// G = Y –0.3455 *（U –128）–0.7169 *（V –128）
// B = Y + 1.779 *（U – 128）
#define YUV2RGB(Y, U, V, R, G, B) \
  do { \
    int y, u, v; \
    y = (Y); \
    u = (U) - 128; \
    v = (V) - 128; \
    (R) = CLP(y + ((v * 360) >> 8) ); \
    (G) = CLP(y - ((u * 88 + v * 184) >> 8)); \
    (B) = CLP(y + ((u * 455) >> 8) ); \
  } while(0)
int icvYUYV2BGRx_8u_C2C3R1_table(unsigned long width,
    unsigned long height,
    unsigned char* yuvin,
    unsigned char* rgbout)
{
  int src_cn = 2, dst_cn = 3, srcstep = width * src_cn, dststep = width * dst_cn;
  int blue_idx = 2;
  unsigned char* src = yuvin; // 源数据
  unsigned char* dst = rgbout; // 目标数据
  int i, j;
  for (i = 0; i < height; ++i) {
    unsigned int yuv_uv[ 2 ] = {0, 0}; // {u0, v1};
    const unsigned char* src0 = src + i * srcstep;
    //unsigned char *dst0 = dst + i * dststep;
    unsigned char* dst0 = dst + (height - 1 - i) * dststep;
    for (j = 0; j < width; ++j, src0 += src_cn, dst0 += dst_cn) {
      // UYVY标准 [U0 Y0 V0 Y1] [U1 Y2 V1 Y3] [U2 Y4 V2 Y5] 每像素点两个字节，[内]为四个字节
      yuv_uv[(j & 1) ] = src0[ 1 ];
      YUV2RGB(src0[ 0 ], yuv_uv[ 0 ], yuv_uv[ 1 ], dst0[ 2 ^ blue_idx ], dst0[ 1 ], dst0[ blue_idx ]);
    }
  }
  return 0;
}
bool YUV422ToRGB(unsigned long width, unsigned long height, unsigned char* yuvin, unsigned char* rgbout)  ///n
{
  int col, row;
  long Y, U, V;
  int red, blue, green;
  for (row = 0; row < height; row += 1) {
    int idx = ((height - row - 1)) * width * 3;
    int rowptr = row * width * 2;
    for (col = 0; col < width; col += 1) {
      int colhalf = col << 1;
      Y = yuvin[rowptr + colhalf];
      // U=128;
      // V=128;
      U = yuvin[((rowptr + colhalf) & 0xfffffffC) + 3 ];
      V = yuvin[((rowptr + colhalf) & 0xfffffffC) + 1 ];
      red = (int)(1.0 * Y + 8 + 1.402 * (V - 128));
      green = (int)(1.0 * Y - 0.34414 * (U - 128) - 0.71414 * (V - 128));
      blue = (int)(1.0 * Y + 1.772 * (U - 128) + 0);
      if (red > 255) {
        red = 255;
      }
      else if (red < 0) {
        red = 0;
      }
      if (green > 255) {
        green = 255;
      }
      else if (green < 0) {
        green = 0;
      }
      if (blue > 255) {
        blue = 255;
      }
      else if (blue < 0) {
        blue = 0;
      }
      rgbout[idx++] = (BYTE)red;
      rgbout[idx++] = (BYTE)green;
      rgbout[idx++] = (BYTE)blue;
    }
  }
  return true;
}
bool FlagVerticalMirror = 0;//垂直镜像标志
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
      DEBUG_INFO1(_T(" ConvertBayer2Rgb = %f "), L_Timer.elapsed());
      L_Timer.restart();
      RGB_ContrastAdjust(pDataRGB, pDataRGB, upcontrast, ThresholdValue, number);
      DEBUG_INFO1(_T(" RGB_ContrastAdjust = %f "), L_Timer.elapsed());
      //L_Timer.restart();
      //RGB_BrightAdjust(pDataRGB,pDataRGB,uplight,number);
      //DEBUG_INFO1( _T(" RGB_BrightAdjust = %f "), L_Timer.elapsed() );
      //L_Timer.restart();
      if (gCallBack != NULL) {
        (*gCallBack)(VideoWidth, VideoHeight, pDataRGB, m_param);
      }
      //DEBUG_INFO1( _T(" StretchDIBits = %f "), L_Timer.elapsed() );
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
  DEBUG_INFO1(_T("%s "), _T("ShowThread End!!!"));
  return 0;
}
unsigned __stdcall ShowThread1(LPVOID lpParameter)
{
  BYTE* pData_RAW_YUV = new BYTE[614400];//n
  BYTE* pDataRGB = new BYTE[921600];
  ZeroMemory(pData_RAW_YUV, 614400);
  ZeroMemory(pDataRGB, 921600);
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
      for (int Count = 0; Count < (number * 2); Count++) { //n
        if (pSensorData != NULL) {
          pData_RAW_YUV[Count] = pSensorData[(number * 2) - Count];
        }
      }
      icvYUYV2BGRx_8u_C2C3R1_table(VideoWidth, VideoHeight, pData_RAW_YUV, pDataRGB);
      if (gCallBack != NULL) {
        (*gCallBack)(VideoWidth, VideoHeight, pDataRGB, m_param); //0.005
      }
      ImageDisposal_ing = FALSE;
      ZeroMemory(pData_RAW_YUV, 614400);
      ZeroMemory(pDataRGB, 921600);
    }
    Sleep(60); //0.025+0.020
  }
  if (pBitMapInfo) {
    delete pBitMapInfo;
    pBitMapInfo = NULL;
  }
  delete[] pData_RAW_YUV;
  pData_RAW_YUV = NULL;
  delete[] pDataRGB;
  pDataRGB = NULL;
  DEBUG_INFO1(_T("%s "), _T("ShowThread End!!!"));
  return 0;
}
BOOL GetPhoto()
{
  return GetPhotoing;
}
void SethandLight(int light)
{
  uplight = light;
}
void SethandContrast(int contrast)
{
  upcontrast = contrast;
}
void SetThresholdValue(int value)
{
  ThresholdValue = value;
}
int GethandLight()
{
  return uplight;
}
int GethandContrast()
{
  return upcontrast;
}
int GetThresholdValue()
{
  return ThresholdValue;
}
BOOL SetSensorAEWorAEB(BYTE Light)
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
BOOL SetSensorAGC(BYTE Contrast)
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
BOOL SetSensorAEC(int Saturation)
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
int GetSaturation()
{
  return saturation;
}
int GetContrast()
{
  return contrast;
}
int GetLight()
{
  return light;
}
BOOL SetManuallight()
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
BOOL SetManualcontrast(BOOL state)
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
BOOL SetManualsaturation(BOOL state)
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
BOOL SetFrameRate(int rate)
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
BOOL SetVideoModel(BYTE VideoModel)
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
  case 2:
    VideoWidth = 576;
    VideoHeight = 432;
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
void ExitVideo()
{
  //AoutLock lock( &crExit );
  AutoPlayFlag = FALSE;
  if (NULL != hThread) {
    if (WaitForSingleObject(hThread, 300) != WAIT_OBJECT_0) {
      TerminateThread(hThread, 0);
      DEBUG_INFO1(_T("%s "), _T(" WaitForSingsleObject ReadOV3620ForThread end)"));
    }
    //WaitForSingleObject( hThread, INFINITE );
    hThread = NULL;
  }
  if (NULL != hShowThread) {
    if (WaitForSingleObject(hShowThread, 300) != WAIT_OBJECT_0) {
      TerminateThread(hShowThread, 0);
      DEBUG_INFO1(_T("%s "), _T(" ShowThread TerminateThread)"));
    }
    //WaitForSingleObject( hShowThread, INFINITE );
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
BOOL StartVideo(PFCALLBACK Func, LPVOID param)
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
void ShowVideo(HWND hWnd, DWORD Width, DWORD Height)
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
    if (WaitForSingleObject(hThread, 1000) != WAIT_OBJECT_0) {
      TerminateThread(hThread, 100);
    }
    hThread = NULL;
  }
  if (NULL != hShowThread) {
    if (WaitForSingleObject(hShowThread, 1000) != WAIT_OBJECT_0) {
      TerminateThread(hShowThread, 100);
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
//向设备发送消息
BOOL SendMsg(BYTE msg)
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
BOOL SetOther(BYTE msg)
{
  return TRUE;
}
//image control
// control = b00000000 -> normal
// control = b00000001 -> mirror image
// control = b00000010 -> vertically flip image
// control = b00000011 -> mirror and vertically flip image
BOOL IMGControl(BYTE control)
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
      // ept->Direction = DIR_TO_DEVICE;
      ept->ReqCode = 0xb6;
      ept->Value = ((WORD)control);
      ept->Index = 0;
      LONG bytesToSend = 64;
      ept->Read((PUCHAR)Buf, bytesToSend);
    }
  }
  delete BitControlDev;
  BitControlDev = NULL;
  return ret;
}
//IR control function
// control = b00000000 -> close all IR LED
// control = b00000001 -> open group 1 IR LED
// control = b00000010 -> open group 2 IR LED
// control = b00000100 -> open group 3 IR LED
// control = b00000111 -> open group 1, 2 and 3 IR LED
BOOL IRControl(BYTE control)
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
      // ept->Direction = DIR_TO_DEVICE;
      ept->ReqCode = 0xb4;
      ept->Value = ((WORD)control) | 0xf300;
      ept->Index = 0;
      LONG bytesToSend = 64;
      ept->Read((PUCHAR)Buf, bytesToSend);
    }
  }
  delete BitControlDev;
  BitControlDev = NULL;
  return ret;
}
//I2C memory write
// addr -> write address
// data -> write data
BOOL I2CMemWrite(WORD addr, BYTE* data)
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
      // ept->Direction = DIR_TO_DEVICE;
      ept->ReqCode = 0xb0;
      ept->Value = (WORD)data[0];
      ept->Index = addr;
      LONG bytesToSend = 64;
      ept->Read((PUCHAR)Buf, bytesToSend);
    }
  }
  delete BitControlDev;
  BitControlDev = NULL;
  return ret;
}
//I2C memory Read
// addr -> read address
// data -> read data
BOOL I2CMemRead(WORD addr, BYTE* data)
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
      ept->ReqCode = 0xb1;
      ept->Value = (WORD)data;
      ept->Index = addr;
      LONG bytesToSend = 64;
      ept->Read((PUCHAR)Buf, bytesToSend);
    }
    *data = Buf[0];
  }
  delete BitControlDev;
  BitControlDev = NULL;
  return ret;
}
BOOL OpenLight(BOOL openorclose)
{
  BYTE imgValue;
  imgValue = (openorclose == TRUE) ? 7 : 0;
  IRControl(imgValue);
  return TRUE;
}
BOOL VideoWrite(int addressWrite, BYTE byteV[100], BOOL isOwnVideo)
{
  if (!isOwnVideo) {
    return FALSE;
  }
  I2CMemWrite(addressWrite, byteV);
  return TRUE;
}
void CHC_SetVideoVChange(BOOL bValue, BOOL isOwnVideo)
{
  BYTE OtherData = 0;
  if (!isOwnVideo) {
    if (bValue) {
      OtherData |= 0x40;
    }
    else {
      OtherData &= ~0x40;
    }
    SetOther(OtherData);
  }
  else {
    if (bValue) {
      OtherData = 0;
    }
    else {
      OtherData = 3;
    }
    IMGControl(OtherData);
  }
}
BOOL CHC_SetSenseAuto(long lValue, BOOL isOwnVideo)
{
  SetManualcontrast(FALSE);
  SetManualsaturation(FALSE);
  SetManuallight();
  BOOL blState = SetSensorAEWorAEB(lValue);
  return blState;
}

