
#ifndef _HCNET_INL_
#define _HCNET_INL_
#if 1
#include <windows.h>
#include "HCNetSDK.h"
#include "plaympeg4.h"
#include <time.h>
#undef T_UYVY


#pragma comment(lib,"HCCore.lib")
#pragma comment(lib,"HCNetSDK.lib")
#pragma comment(lib,"PlayCtrl.lib")

//int hcnet_capnum();

typedef struct hcnet_t {
  LONG lUserID;
  LONG lRealPlayHandle;
  uchar* data;
  FRAME_INFO fi[1];
  LONG nPort;
} hcnet_t;



int hcnet_close(capdev* s)
{
  hcnet_t* x = (hcnet_t*)(s->x);

  //关闭预览
  if (!NET_DVR_StopRealPlay(x->lRealPlayHandle)) {
    printf("NET_DVR_StopRealPlay error! Error number: %d\n", NET_DVR_GetLastError());
    return 0;
  }

  x->lRealPlayHandle = 0;
  //注销用户
  NET_DVR_Logout(x->lUserID);
  NET_DVR_Cleanup();
  FREE(x->data);
  FREE(x);
  s->x = 0;
  return 0;
}

#include "img/color.inl"

int YV12_RGB(int height, int width, const uchar* src, int srcstep, uchar* dst, int dststep, int srctype, int dsttype, int dstcn, int blue_idx)
{
  const int numpix = width * height;
  int i, j, y00, y01, y10, y11, u, v;
  const unsigned char* pY = src;
  const unsigned char* pV = pY + numpix;
  const unsigned char* pU = pY + numpix + numpix / 4;
  unsigned char* pOut = dst;
  
  for (j = 0; j <= height - 2; j += 2) {
    for (i = 0; i <= width - 2; i += 2) {
      y00 = *pY;
      y01 = *(pY + 1);
      y10 = *(pY + width);
      y11 = *(pY + width + 1);
      u = (*pU++) - 128;
      v = (*pV++) - 128;
      
      move_420_block(y00, y01, y10, y11, u, v, dststep, dstcn, blue_idx, pOut);
      
      pY += 2;
      pOut += 2 * dstcn;
    }
    
    pY += width;
    pOut += dststep;
  }
  
  return 0;
}

int hcnet_getframe(capdev* s, img_t* im, int flag)
{
  hcnet_t* x = (hcnet_t*)(s->x);
  FRAME_INFO* pFrameInfo = x->fi;
  if (x->data) {
    int h = pFrameInfo->nHeight, w = pFrameInfo->nWidth;
    imsetsize(im, h, w, 3, 1);
    YV12_RGB(h, w, (uchar*)x->data, 0, im->tt.data, im->s, 0, 0, im->c, 0);
  }
  return 0;
}
int hcnet_set_sensitometry(capdev* s0, int Saturation);


void CALLBACK g_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void* pUser)
{
  char tempbuf[256] = {0};
  capdev* s = (capdev*)pUser;
  hcnet_t* x = (hcnet_t*)(s->x);

  switch (dwType) {
  case EXCEPTION_RECONNECT:    //预览时重连
    printf("----------reconnect--------%d\n", time(NULL));
    break;

  default:
    break;
  }
}

//解码回调 视频为YUV数据(YV12)，音频为PCM数据
void CALLBACK DecCBFun(long nPort, char* pBuf, long nSize, FRAME_INFO* pFrameInfo, long nReserved1, long nReserved2)
{
  capdev* s = (capdev*)nReserved1;
  hcnet_t* x = (hcnet_t*)(s->x);
  long lFrameType = pFrameInfo->nType;
  *x->fi = *pFrameInfo;

  if (lFrameType == T_YV12) {
    x->data = (uchar*)realloc(x->data, nSize);
    memcpy(x->data, pBuf, nSize);
    //此时是YV12格式的视频数据，保存在pBuf中，可以fwrite(pBuf,nSize,1,Videofile);
    //fwrite(pBuf,nSize,1,fp);
  }
  else if (lFrameType == T_AUDIO16) {
    //此时是音频数据，数据保存在pBuf中，可以fwrite(pBuf,nSize,1,Audiofile);
  }
  else {
  }
  return ;
}

//实时流回调
void CALLBACK fRealDataCallBack(LONG lRealHandle, DWORD dwDataType, BYTE* pBuffer, DWORD dwBufSize, void* pUser)
{
  DWORD dRet;
  capdev* s = (capdev*)pUser;
  hcnet_t* x = (hcnet_t*)(s->x);
  HWND hWnd = NULL;

  switch (dwDataType) {
  case NET_DVR_SYSHEAD:    //系统头
    if (!PlayM4_GetPort(&x->nPort)) { //获取播放库未使用的通道号
      break;
    }

    if (dwBufSize > 0) {
      if (!PlayM4_OpenStream(x->nPort, pBuffer, dwBufSize, 1024 * 1024)) {
        dRet = PlayM4_GetLastError(x->nPort);
        break;
      }

      //设置解码回调函数 只解码不显示
      if (!PlayM4_SetDecCallBackMend(x->nPort, DecCBFun, (long)pUser))
        //if (!PlayM4_SetDecCallBack(x->nPort, DecCBFun))
      {
        dRet = PlayM4_GetLastError(x->nPort);
        break;
      }

      //设置解码回调函数 解码且显示
      //if (!PlayM4_SetDecCallBackEx(nPort,DecCBFun,NULL,NULL))
      //{
      //  dRet=PlayM4_GetLastError(nPort);
      //  break;
      //}

      //打开视频解码
      if (!PlayM4_Play(x->nPort, hWnd)) {
        dRet = PlayM4_GetLastError(x->nPort);
        break;
      }

      //打开音频解码, 需要码流是复合流
      if (!PlayM4_PlaySound(x->nPort)) {
        dRet = PlayM4_GetLastError(x->nPort);
        break;
      }
    }

    break;

  case NET_DVR_STREAMDATA:   //码流数据
    if (dwBufSize > 0 && x->nPort != -1) {
      BOOL inData = PlayM4_InputData(x->nPort, pBuffer, dwBufSize);

      while (!inData) {
        Sleep(10);
        inData = PlayM4_InputData(x->nPort, pBuffer, dwBufSize);
        printf("PlayM4_InputData failed \n");
      }
    }

    break;
  }
}

int hcnet_open(capdev* s, char* sDVRIP, WORD wDVRPort, char* sUserName, char* sPassword)
{
  NET_DVR_DEVICEINFO_V30 struDeviceInfo;
  hcnet_t* x = 0;
  x = MALLOC(hcnet_t, 1);
  x->nPort = -1;
  MEMSET(x, 0, 1);
  // 初始化
  NET_DVR_Init();
  //设置连接时间与重连时间
  NET_DVR_SetConnectTime(2000, 1);
  NET_DVR_SetReconnect(10000, true);

  // 获取控制台窗口句柄
  //HMODULE hKernel32 = GetModuleHandle((LPCWSTR)"kernel32");
  //GetConsoleWindow = (PROCGETCONSOLEWINDOW)GetProcAddress(hKernel32,"GetConsoleWindow");

  // 注册设备
  x->lUserID = NET_DVR_Login_V30(sDVRIP, wDVRPort, sUserName, sPassword, &struDeviceInfo);

  if (x->lUserID < 0) {
    printf("Login error, %d\n", NET_DVR_GetLastError());
    NET_DVR_Cleanup();
    FREE(x);
    return 0;
  }

  //设置异常消息回调函数
  NET_DVR_SetExceptionCallBack_V30(0, NULL, g_ExceptionCallBack, s);

  //cvNamedWindow("IPCamera");
  //启动预览并设置回调数据流
  NET_DVR_CLIENTINFO ClientInfo;
  ClientInfo.lChannel = 1;        //Channel number 设备通道号
  ClientInfo.hPlayWnd = NULL;     //窗口为空，设备SDK不解码只取流
  ClientInfo.lLinkMode = 0;       //Main Stream
  ClientInfo.sMultiCastIP = NULL;

  x->lRealPlayHandle = NET_DVR_RealPlay_V30(x->lUserID, &ClientInfo, fRealDataCallBack, s, TRUE);

  if (x->lRealPlayHandle < 0) {
    printf("NET_DVR_RealPlay_V30 failed! Error number: %d\n", NET_DVR_GetLastError());
    FREE(x);
    return 0;
  }

  s->x = (void*)x;
  s->close = hcnet_close;
  s->getframe = hcnet_getframe;
  return 0;
}
#endif

#endif // _HCNET_INL_
