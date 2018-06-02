
#define _WIN32_WINNT 0x0500 // this is needed for the WS_EX_LAYERED

#include "cstd.h"

#include "pmdev.inl"
//#include "win.h"
//#include "test_udp.inl"
//#include "ui/window.inl"

BOOL eventfun(pmdev_t* pv, const char* sIP, const char* sClientIP, int eType, int eFlag, void* pvContent, int nSerialNo, int eResult, void* pvClientData)
{
#define PRINTF printf
#include "eventfun_impl.inl"
  return TRUE;
}

#include "test_pmdevui.inl"


int frame_1(pmdev_t* vp, int h, int w, const unsigned char* img, int step, int cn) {
  printf("%dx%d\n", w, h);
  return 0;
}

int test_pmdev()
{
  int n = sizeof(NETPRO);
  pmdev_t pv[1] = {0};
  pmdev_t pv2[1] = {0};
  int a = 1;
  unsigned char* b = (unsigned char*)(&a);
  //thread_t th[1] = {0};
  const char* ip;
  short aa = -3614;
  char* aaa = (char*)&aa;
  //unsigned char aa[20] = {117, 9, 0, 0, 0, 0, 0, 0, 20, 0, 0, 0, 20, 0, 0, 0, 98, -10, 0, 0};
  //int checksum = sock_checksum((UINT16*)(aa), 20);
  return test_pmdevui();
  ip = "192.168.1.155";
  //cvNamedWindow("im", 0);
  //登陆设备 admin : 201031
  CPM_Connect(pv, ip, "admin", "201031", eventfun, 0);

  while (pv->m_Author) {
    Sleep(100);
  }
  
  CPM_StartVideo(pv, frame_1, 3, 1);

  for (; 1;) {
    char ch = 0;//cvWaitKey(-1);
    ch = getchar();

    if ('q' == ch || 'Q' == ch) {
      break;
    }

    if ('a' == ch) {
      //CPM_DLAllUser(pv);
    }
  }

  //CPM_StopVideo(pv);//关闭视频
  CPM_DisConnect(pv);//关闭连接
  netexit();//释放套接字资源
  return 0;
}

#ifdef WIN321
#include "win.h"
#include "ui/window.inl"
int show_fun1(int h, int w, const unsigned char* img, int step, int cn)
{
  img_t im[1] = {0};
  int s = 4;
  static int ttt = 0;
  //flip_ud(h, w*3, pPro->m_pRGBTRANS, step);
  IMINIT(im, h / s, w, img, step * s, cn, 1);
  imshow(im);
  printf("%d %d %d\n", ttt++, h, w);
  return 0;
}

int test_VideoSDK3()
{
  pmdev_t pv[1] = {0};
  thread_t h;
  const char* ip;
  int cn = 4;
  ip = "192.168.1.155";
  ip = "192.168.1.189";
  ip = "192.168.1.152";
  h = start_sdk(pv, ip, "192.168.1.3", 0, 0, cn, 0);

  for (; cvWaitKey(100) != 'q';) {
    int aa = 0;
    img_t im[1] = {0};
    IMINIT(im, pv->h, pv->w, pv->pRGBTRANS, pv->w * cn, cn, 1);
    imshow(im);
  }

  pv->run = 0;
  thread_join(h);
  return 0;
}

int test_VideoSDK2()
{
  pmdev_t pv[1] = {0};
  const char* ip;
  ip = "192.168.1.155";
  ip = "192.168.1.189";
  ip = "192.168.1.152";

  cvNamedWindow("im", 0);
  CPM_Init(pv, ip, "192.168.1.3", show_fun1, 0, 4, 0);
  CPM_Connect(pv);
  CPM_StartVideo(pv);

  for (; cvWaitKey(10) != 'q';) {
    int aa = 0;
  }

  CPM_StopVideo(pv);//关闭视频
  CPM_DisConnect(pv);//关闭连接
  WSACleanup();//释放套接字资源
  return 0;
}
#endif
