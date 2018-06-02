
#ifndef _GMM3DTRACK_H_
#define _GMM3DTRACK_H_
#ifdef __cplusplus
extern "C" {;
#endif
enum {
  GT_START,  // 开始视频捕获和高斯背景检测 void*
    GT_STOP,  // 结束视频捕获和高斯背景检测 void*
    GT_GETHEIGHT,  // 获取视频高 int*
    GT_GETWIDTH,   // 获取视频高 int*
    GT_GETPICH,    // 获取视频每行字节数 int*
    GT_GETCHANNEL, // 获取视频通道数 int*
    GT_GETTRANSN,  // 获取视频方向 [0~4] 表示视频旋转n*90度 int*
    GT_GETDATA,    // 获取视频字节流 unsigned char*
    GT_GETNEXT,    // 获取下一帧并检测 unsigned char*
    GT_GETBK,    // 获取视频背景字节流 unsigned char*
    GT_GETFG,    // 获取视频前景字节流 unsigned char*
    GT_GETSHOWWINDOW,    // 获取是否显示窗口 int*
    GT_GETMINPIX,    // 获取区域的最小像素个数，小于此个数的区域将被忽略 int*
    GT_GEWAITMS,    // 设置视频每帧时间间隔毫秒 int*
    GT_GETRECT,    // 获取 目标区域坐标 int*  左上点x,左上点y,右下点x,右下点y,目标包含像素个数
    GT_GETRECTN,    // 获取 目标区域个数 int*
    GT_GETFGPIX,    // 获取 前景像素个数 int*
};

void* GMM3DTrack(int mode);
int GMM3DTrackInt(int mode);
int saveToImg(const char* fn, int h, int w, const unsigned char* img, int al, int ai);
int GMM3DTrackStart();
int GMM3DTrackGetNext();
int GMM3DTrackGetfgpix();
int GMM3DTrackStop();


#ifdef TEST_GMM3DTRACK
#include <stdio.h>
#include <stdlib.h>
// 例子代码
int test_GMM3DTrack() {
  char buf[256];
  int i, j, h, w, al, ai, n, fgpix;
  unsigned char* img;
  int* rt;

  GMM3DTrack(GT_START);
  *(int*)GMM3DTrack(GT_GETSHOWWINDOW)=1; // 显示窗口
  *(int*)GMM3DTrack(GT_GETMINPIX)=20; // 设置最小像素个数为20
  *(int*)GMM3DTrack(GT_GEWAITMS)=10; // 
  
  for (i=0;;++i) {
    if (0==GMM3DTrack(GT_GETNEXT)) {
      break;
    }
    fgpix = GMM3DTrackInt(GT_GETFGPIX);
    printf("%3d : 前景像素个数\n", fgpix);
    n = GMM3DTrackInt(GT_GETRECTN);
    rt = (int*)GMM3DTrack(GT_GETRECT);
    for (j=1; j<(n>2?2:n); ++j) {
      printf("%3d: (%3d, %3d)-(%3d, %3d) pix:%3d\n", j, rt[j*5], rt[j*5+1], rt[j*5+2], rt[j*5+3], rt[j*5+4]);
    }
    h = GMM3DTrackInt(GT_GETHEIGHT);
    w = GMM3DTrackInt(GT_GETWIDTH);
    al = GMM3DTrackInt(GT_GETPICH);
    ai = GMM3DTrackInt(GT_GETCHANNEL);
    // 保存视频原图片
    img = (unsigned char*)GMM3DTrack(GT_GETDATA);
    _snprintf(buf, 256, "im%d.jpg", i);
    saveToImg(buf, h, w, img, al, ai);
    // 保存前景图片
    img = (unsigned char*)GMM3DTrack(GT_GETFG);
    _snprintf(buf, 256, "fg%d.jpg", i);
    saveToImg(buf, h, w, img, al, ai);
    // 保存背景图片
    img = (unsigned char*)GMM3DTrack(GT_GETBK);
    _snprintf(buf, 256, "bk%d.jpg", i);
    saveToImg(buf, h, w, img, al, ai);
  }
  GMM3DTrack(GT_STOP);
  return 0;
}

int test_GMM3DTrack2() {
  int i, fgpix;
  
  if (0== GMM3DTrackStart()) {
    printf("启动失败\n"); return 0;
  }
  for (i=0;;++i) {
    if (0==GMM3DTrackGetNext()) {
      printf("获取图像失败\n");
      break;
    }
    fgpix = GMM3DTrackGetfgpix();
    printf("%3d : 前景像素个数\n", fgpix);
  }
  GMM3DTrackStop();
  return 0;
}

#endif

#ifdef __cplusplus
}
#endif
#endif // _GMM3DTRACK_H_
