
#include <stdio.h>
#ifdef WIN32
#include <direct.h>
#else
#include <unistd.h>
#define _chdir chdir
#endif

//  int xywh[12] = {0};
//  h 图像高 w 图像宽 data图像数据 step图像每行字节数 cn图像通道数
// s_pixtype GRAY 灰度图 BGR|RGB|BGRA|BGR555|BGR565 彩色图 YUYV|UYVY 其他格式
// is_trans 是否翻转90度
// scale 图像预缩放
// xywh 输出 人脸[x, y, w, h] 左眼[x, y, w, h] 右眼[x, y, w, h]
//  face_detect_defaut(im->h, im->w, im->tt.data, im->s, im->c, "gray", trans, 1, xywh);
//  printf("%d %d\n", xywh[0], xywh[1]);
int face_detect_defaut(int h, int w, const void* data, int step, int cn, const char* s_pixtype, int is_trans, double scale, int* xywh);
int test_face_detect_defaut(const char* pic_file_name, int is_trans, double scale, int* xywh);

int main() {
  int xywh[12] = {0}, i;
  _chdir("E:/pub/bin/face/facedata");
  test_face_detect_defaut("adfasdf.bmp", 1, 1, xywh);
  printf("xywh=");
  for (i=0; i<12; ++i) {
    printf(" %d", i, xywh[i]);
  }
  return 0;
}