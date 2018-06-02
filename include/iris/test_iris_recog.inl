
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "iris_recog.inl"
#include "img/imgio.inl"
void free_image(void* data);
void* load_image(const char* fn, int cn_req, int* ph, int* pw, int* pstep);
int save_image(const char* fn, int h, int w, const void* data, int step, int cn);
// 返回 -1 未注册
// 返回 0 没检测到
// 返回 1 检测到
int iris_recog(int h, int w, const void* data, int step, int cn, const char* s_pixtype, const char* s_mode, int is_debug, double scale,
               int* xywh, void* stdiris, void* feature, const void* featlib, int featstep, int featnum, int* pid, int* pdis);

void* load_image(const char* fn, int cn_req, int* ph, int* pw, int* pstep) {
  img_t im[1] = {0};
  if (imread(fn, cn_req, 1, im)) {
    int sz = im->h * im->w * cn_req;
    *ph = im->h;
    *pw = im->w;
    *pstep = im->s;
    return im->tt.data;
  }
  return NULL;
}

int save_image(const char* fn, int h, int w, const void* data, int step, int cn) {
  return imwrite4(fn, h, w, (uchar*)data, step, cn);
}
void free_image(void* data) {
  free(data);
}

int irisfeature(const char* pic_100x100_file_name, void* out_feature) {
  img_t im[1] = {0};
  imread(pic_100x100_file_name, 1, 1, im);
  if (100==im->h && 100==im->w, 1==im->c) {
    iris_recog(0, 0, 0, 0, 0, "gray", "feat", 0, 0, 0, im->tt.data, out_feature, 0, 0, 0, 0, 0);
  }
  imfree(im);
  return 1;
}

#ifndef _WIN32
#define _snprintf snprintf
#endif

int test_iris_recog() {
  char* path = NULL;
  char buf[256];
  int i, j, k, men_numb=3;
  int maxpics = 10; // 每个人最多采集10张图片，可根据需要设置。
  int is_trans = 0; //图像是否需要旋转90度。
  double scale = 0.2; // 第一次图像缩放比例
  int xywh[12] = {0}; // 人脸 左眼 右眼 矩形框位置 [x, y, w, h]
  char stdiris[IRIS_H*IRIS_W]; // 归一化图片
  typedef struct myfeat_t {
    char feat[5400];
    char userid[100];
  } myfeat_t;
  int ret, sz = sizeof(myfeat_t)*(men_numb*maxpics);
  myfeat_t* feats = (myfeat_t*)malloc(sz); // 特征库。
  char* data = NULL;
  int h=0, w=0, step=0, cn = 3;
  char* s_pixtype = "BGR"; // cn = 3
  int is_debug = 1;
  memset(feats, 0, sz);
  path = "E:/pub/bin/iris/iris_recog";
  {
    // 采集过程(采用文件图片的方式演示原理。实际使用中可以采用视频图片)
    for (i=0; i<men_numb; ++i) {
      for (j=0, k=0; k<maxpics; ++j) {
        myfeat_t* feature = feats + i * maxpics + k;
        _snprintf(buf, 256, "%s/%03d/L/S2%03dL%02d.jpg", path, i+1, i+1, j+1);
        _snprintf(feature->userid, 64, "%03d", i); // 设置用户id
        data = (char*)load_image(buf, cn, &h, &w, &step); // 读取图片文件
        if (NULL==data) {
          printf("图片读取失败\n");
        }
        // 检测人脸，并从提取人脸特征
        ret = iris_recog(h, w, data, step, cn, s_pixtype, "REG", is_debug, scale,
          xywh, stdiris, feature->feat, NULL, 0, 0, 0, 0); // 采集过程中不需要设置特征库参数。
        printf("%d %d ", i, j);
        if (ret<0) {
          printf("未注册\n");
          free(feats);
          return 0;
        }
        if (ret) {
          _snprintf(buf, 256, "%s/std/std_%03d_%02d.bmp", path, i, j);
          save_image(buf, IRIS_H, IRIS_W, stdiris, IRIS_W, 1);
          _snprintf(buf, 256, "%s/std/pic_%03d_%02d.bmp", path, i, j);
          save_image(buf, h, w, data, step, cn);
          ++k;
          printf("OK\n");
        } else {
          printf("检测失败\n");
        }
        free_image(data);
      }
    }
  }
  {
    int id, dis;
    myfeat_t feattmp[1];
    int thds[] = {450, 470, 490}; // 阈值 [严格, 一般, 宽松]
    // 识别过程
    for (i=0; i<men_numb; ++i) {
      for (j=maxpics; j<20; ++j) {
        _snprintf(buf, 256, "%s/%03d/L/S2%03dL%02d.jpg", path, i+1, i+1, j+1);
        data = (char*)load_image(buf, cn, &h, &w, &step);
        if (NULL==data) {
          printf("没图片了\n");
          break;
        }
        // 检测人脸，并从提取人脸特征
        ret = iris_recog(h, w, data, step, cn, s_pixtype, "REC", is_trans, scale,
          xywh, stdiris, feattmp->feat, feats->feat, sizeof(*feats), men_numb*maxpics, &id, &dis);
        if (ret) {
          printf("file_%02d id = %3d userid = %s 距离 = %d %s\n", i, id, feats[id].userid, dis, dis<thds[1] ? "通过" : "           拒绝");
        } else {
          printf("检测失败\n");
        }
      }
    }
  }
  free(feats);
  return 0;
}
