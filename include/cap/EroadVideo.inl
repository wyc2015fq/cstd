#ifndef _EROADVIDEO_INL_
#define _EROADVIDEO_INL_
//#include "DebugAssistant.h"
#include <process.h>
#pragma comment(lib, "CyAPI.lib")
#undef DEBUG_INFO1
#define DEBUG_INFO1 printf
#define RAWSZ (640*480)
#ifndef _T
#define _T(x) (x)
#endif // _T
#include "CyAPI.h"
typedef struct eroad_dev {
  bool FlagVerticalMirror;//垂直镜像标志
  DWORD width;
  DWORD height;
  DWORD number;
  BOOL AutoPlayFlag;
  BOOL ImageDisposal_ing;
  HANDLE hThread;
  int wIndex;
  CCyUSBDevice* pUSBDev;
  unsigned char data[RAWSZ];
  UCHAR* inContext;
} eroad_dev;
// ReadOV3620
unsigned __stdcall EroadVideoThread(LPVOID lpParameter)
{
  LONG Readlength = 0;
  eroad_dev* s = (eroad_dev*)lpParameter;
  for (int a = 0; a < RAWSZ; a++) {
    s->data[a] = 0;
  }
  int devices = s->pUSBDev->DeviceCount();
  WORD deviceNum = 0;
  for (deviceNum = 0; deviceNum < devices; deviceNum++) {
    if (s->wIndex == deviceNum && s->pUSBDev->Open((UCHAR)deviceNum)) {
      CCyControlEndPoint* ept = s->pUSBDev->ControlEndPt;
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
        CCyControlEndPoint* ept = s->pUSBDev->ControlEndPt;
        ept->Target = TGT_DEVICE;
        ept->ReqType = REQ_VENDOR;
        ept->Direction = DIR_TO_DEVICE;
        ept->ReqCode = 0xa2;
        ept->Value = 0;
        ept->Index = 0;
        BYTE buf[64];
        char buf2[64];
        ZeroMemory(buf, 64);
        LONG bytesToSend = 64;
        buf[0] = 0xa2;
        buf[1] = 0x06;
        ept->Write((PUCHAR)buf, bytesToSend);
        OVERLAPPED inOvLap;
        UCHAR* inContext;
        _snprintf(buf2, 64, ("CYUSB_IN%d"), deviceNum);
        inOvLap.hEvent = CreateEvent(NULL, false, false, buf2);
        breakOk = s->AutoPlayFlag;
        s->pUSBDev->BulkInEndPt->SetXferSize(s->number);
        inContext = s->pUSBDev->BulkInEndPt->BeginDataXfer(s->data, s->number, &inOvLap);
        if (!s->pUSBDev->BulkInEndPt->WaitForXfer(&inOvLap, 1000)) {
          break;
        }
        // SumFrameCount++;
        while (s->AutoPlayFlag && s->pUSBDev) {
          long nTemp = s->number;
          s->pUSBDev->BulkInEndPt->XferData(s->data, nTemp);
        }
        if (s->pUSBDev && breakOk) {
          bool bResult = s->pUSBDev->BulkInEndPt->FinishDataXfer(s->data, (long&)s->number, &inOvLap, inContext);
          DEBUG_INFO1(_T("%s "), _T("FinishDataXfer"));
        }
        CloseHandle(inOvLap.hEvent);
        inOvLap.hEvent = NULL;
        buf[1] = (char)0x07;
        ept->Write((PUCHAR)buf, bytesToSend);
        DEBUG_INFO1(_T("%s "), _T("ept->Write((PUCHAR)buf, bytesToSend)"));
      }
      if (breakOk) {
        DEBUG_INFO1(_T("%s "), _T(" EroadVideoThread breakOk)"));
        break;
      }
    }
  }
  DEBUG_INFO1(_T("%s "), _T("EroadVideoThread End!!!"));
  return 0 ;
}
//设置传感器视频亮度，范围是0-255，默认打开为63, SetSensorAEWorAEB
BOOL caperoad_set_brightness(capdev* s0, int Light)
{
  eroad_dev* s = (eroad_dev*)(s0->x);
  BYTE Buf[64];
  ZeroMemory(Buf, sizeof(Buf));
  CCyControlEndPoint* ept = s->pUSBDev->ControlEndPt;
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
  return ret;
}
//设置传感器视频对比度，范围是0-255，默认打开是传感器的自动设置，手动默认为38, SetSensorAGC
BOOL caperoad_set_contrast(capdev* s0, BYTE Contrast)
{
  eroad_dev* s = (eroad_dev*)(s0->x);
  CCyControlEndPoint* ept = s->pUSBDev->ControlEndPt;
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
  return ret;
}
//设置视频曝光度，范围是0-65535，默认打开为传感器自动设置，手动默认为140, SetSensorAEC
int caperoad_set_sensitometry(capdev* s0, int Saturation)
{
  eroad_dev* s = (eroad_dev*)(s0->x);
  CCyControlEndPoint* ept = s->pUSBDev->ControlEndPt;
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
  return ret;
}
//手动控制亮度的开关，由于传感器的原因，亮度没有自动设置一项，
// 要手动设置亮度必须将对比度和曝光度设置为自动
BOOL SetManuallight(capdev* s0)
{
  eroad_dev* s = (eroad_dev*)(s0->x);
  CCyControlEndPoint* ept = s->pUSBDev->ControlEndPt;
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
  return ret;
}
//手动自动设置对比度的开关，state为TRUE是手动，FALSE是自动
BOOL SetManualcontrast(capdev* s0, BOOL state)
{
  eroad_dev* s = (eroad_dev*)(s0->x);
  CCyControlEndPoint* ept = s->pUSBDev->ControlEndPt;
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
  return ret;
}
//手动自动设置曝光度的开关，state为TRUE是手动，FALSE是自动
int caperoad_set_manual_saturation(capdev* s0, int state)
{
  eroad_dev* s = (eroad_dev*)(s0->x);
  CCyControlEndPoint* ept = s->pUSBDev->ControlEndPt;
  ept->Target = TGT_DEVICE;
  ept->ReqType = REQ_VENDOR;
  ept->Direction = DIR_TO_DEVICE;
  ept->ReqCode = 0xa2;
  LONG bytesToSend = 64;
  BYTE Buf[64];
  ZeroMemory(Buf, sizeof(Buf));
  Buf[0] = 0xa2;
  Buf[1] = 0x04;
  Buf[2] = state ? 0x02 : 0x01;
  BOOL ret = ept->Write(Buf, bytesToSend);
  return ret;
}
//设置帧率，rate有60、30、15三个值，其它将设置为60
BOOL SetFrameRate(capdev* s0, int rate)
{
  eroad_dev* s = (eroad_dev*)(s0->x);
  CCyControlEndPoint* ept = s->pUSBDev->ControlEndPt;
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
  return ret;
}
//设置视频显示模式
// VideoModel=0为640*480的分辨率显示
// VideoModel=1为320*240的分辨率显示
// 其它为640*480分辨率显示
BOOL SetVideoModel(capdev* s0, BYTE VideoModel)
{
  eroad_dev* s = (eroad_dev*)(s0->x);
  CCyControlEndPoint* ept = s->pUSBDev->ControlEndPt;
  ept->Target = TGT_DEVICE;
  ept->ReqType = REQ_VENDOR;
  ept->Direction = DIR_TO_DEVICE;
  ept->ReqCode = 0xa2;
  LONG bytesToSend = 64;
  BYTE Buf[64];
  if (s->hThread == NULL) {
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
    s->width = 640;
    s->height = 480;
    Buf[2] = VideoModel;
    break;
  case 1:
    s->width = 320;
    s->height = 240;
    Buf[2] = VideoModel;
    break;
  default:
    s->width = 640;
    s->height = 480;
    Buf[2] = VideoModel;
    break;
  }
  ret = ept->Write(Buf, bytesToSend);
  s->number = s->width * s->height;
  return ret;
}
BOOL SendMsg(capdev* s0, BYTE msg)
{
  BYTE Buf[64];
  BOOL ret = FALSE;
  eroad_dev* s = (eroad_dev*)(s0->x);
  ZeroMemory(Buf, sizeof(Buf));
  if (s->pUSBDev->IsOpen()) {
    CCyControlEndPoint* ept = s->pUSBDev->ControlEndPt;
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
  return ret;
}
BOOL SetOther(capdev* s0, BYTE msg)
{
  BYTE Buf[64];
  eroad_dev* s = (eroad_dev*)(s0->x);
  ZeroMemory(Buf, sizeof(Buf));
  CCyControlEndPoint* ept = s->pUSBDev->ControlEndPt;
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
    s->FlagVerticalMirror = 1; //垂直镜像标志
  }
  else {
    s->FlagVerticalMirror = 0; //垂直镜像标志
  }
  return ret;
}
//image control
// control = b00000000->normal
// control = b00000001->mirror image
// control = b00000010->vertically flip image
// control = b00000011->mirror and vertically flip image
BOOL IMGControl(capdev* s0, BYTE control)
{
  BYTE Buf[64];
  BOOL ret = FALSE;
  eroad_dev* s = (eroad_dev*)(s0->x);
  ZeroMemory(Buf, sizeof(Buf));
  if (s->pUSBDev->IsOpen()) {
    CCyControlEndPoint* ept = s->pUSBDev->ControlEndPt;
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
  return ret;
}
//IR control function
// control = b00000000->close all IR LED
// control = b00000001->open group 1 IR LED
// control = b00000010->open group 2 IR LED
// control = b00000100->open group 3 IR LED
// control = b00000111->open group 1, 2 and 3 IR LED
BOOL IRControl(capdev* s0, BYTE control)
{
  BYTE Buf[64];
  BOOL ret = FALSE;
  eroad_dev* s = (eroad_dev*)(s0->x);
  ZeroMemory(Buf, sizeof(Buf));
  if (s->pUSBDev && s->pUSBDev->IsOpen()) {
    CCyControlEndPoint* ept = s->pUSBDev->ControlEndPt;
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
  return ret;
}
//I2C memory write
// addr->write address
// data->write data
BOOL I2CMemWrite(capdev* s0, WORD addr, BYTE* data)
{
  BYTE Buf[64];
  BOOL ret = FALSE;
  eroad_dev* s = (eroad_dev*)(s0->x);
  ZeroMemory(Buf, sizeof(Buf));
  if (s->pUSBDev->IsOpen()) {
    CCyControlEndPoint* ept = s->pUSBDev->ControlEndPt;
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
  return ret;
}
//I2C memory Read
// addr->read address
// data->read data
BOOL I2CMemRead(capdev* s0, WORD addr, BYTE* data)
{
  BYTE Buf[64];
  BOOL ret = FALSE;
  eroad_dev* s = (eroad_dev*)(s0->x);
  ZeroMemory(Buf, sizeof(Buf));
  if (s->pUSBDev->IsOpen()) {
    CCyControlEndPoint* ept = s->pUSBDev->ControlEndPt;
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
  return ret;
}
BOOL OpenLight(capdev* s0, BOOL openorclose)
{
  BYTE imgValue;
  imgValue = (openorclose == TRUE) ? 7 : 0;
  IRControl(s0, imgValue);
  return TRUE;
}
BOOL VideoWrite(capdev* s0, int addressWrite, BYTE byteV[100], BOOL isOwnVideo)
{
  if (!isOwnVideo) {
    return FALSE;
  }
  I2CMemWrite(s0, addressWrite, byteV);
  return TRUE;
}
int CHC_SetVideoVChange(capdev* s0, BOOL bValue, BOOL isOwnVideo)
{
  BYTE OtherData = 0;
  if (!isOwnVideo) {
    if (bValue) {
      OtherData |= 0x40;
    }
    else {
      OtherData &= ~0x40;
    }
    SetOther(s0, OtherData);
  }
  else {
    if (bValue) {
      OtherData = 0;
    }
    else {
      OtherData = 3;
    }
    IMGControl(s0, OtherData);
  }
  return 0;
}
//关闭视频设备，退出，清理数据
int caperoad_close(capdev* s0)
{
  eroad_dev* s = (eroad_dev*)(s0->x);
  OpenLight(s0, 0);
  s->AutoPlayFlag = FALSE;
  if (NULL != s->hThread) {
    if (WaitForSingleObject(s->hThread, 300) != WAIT_OBJECT_0) {
      TerminateThread(s->hThread, 0);
      DEBUG_INFO1(_T("%s "), _T(" WaitForSingsleObject EroadVideoThread end)"));
    }
    //WaitForSingleObject( s->hThread, INFINITE );
    s->hThread = NULL;
  }
  delete s->pUSBDev;
  s->pUSBDev = NULL;
  DEBUG_INFO1(_T("%s"), _T("ExitVideo finish"));
  delete s;
  return 0;
}
#include "eroad_impl.inl"
#if 0
int test_EroadVideo1()
{
  img_t im2[1] = {0};
  //上位视频对比度
  int upcontrast = 255;
  //设置上位对比度阀值
  int ThresholdValue = 255;
  capdev s[1] = {0};
  cvNamedWindow("asdfasdf", 1);
  if (caperoad_open(s, 0)) {
    for (; 1;) {
      caperoad_getframe(s, im2);
      //RGB_ContrastAdjust(im2->data, im2->data, upcontrast, ThresholdValue, im2->width*im2->height);
      //imshow2("asdfasdf", im2);
      if ('q' == cvWaitKey(1)) {
        break;
      }
    }
    caperoad_close(s);
  }
  imfree(im2);
  cvWaitKey(-1);
  return 0;
}
#endif
#endif // _EROADVIDEO_INL_

