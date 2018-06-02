
#include "GlobalFunction.h"
int otsuThreshold1(img_t* frame, img_t* dst)
{
  int width = frame->w;
  int height = frame->h;

  int dd;
  int i, j, pixelSum = width * height, threshold = 0;
  int pixelCount[255] = {0};
  float pixelPro[255] = {0};
  float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax;
  //uchar* data = (uchar*)frame->imageData;

  for (i = 0; i < GrayScale; i++) {
    pixelCount[i] = 0;
    pixelPro[i] = 0;
  }

  dd = 0;

  //统计灰度级中每个像素在整幅图像中的个数
  for (j = 0; j < height; j++) {
    for (i = 0; i < width; i++) {
      //pixelCount[(int)data[i * width + j]]++;
      dd = S(frame, i, j);
      pixelCount[S(frame, i, j)]++;

      if (dd != 255) {
        int kk = 12;
      }
    }
  }

  //计算每个像素在整幅图像中的比例
  for (i = 0; i < GrayScale; i++) {
    pixelPro[i] = (float)pixelCount[i] / pixelSum;
  }

  //遍历灰度级[0,255]
  deltaMax = 0;

  for (i = 0; i < GrayScale; i++) {
    w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;

    for (j = 0; j < GrayScale; j++) {
      if (j <= i) { //背景部分
        w0 += pixelPro[j];
        u0tmp += j * pixelPro[j];
      }
      else { //前景部分
        w1 += pixelPro[j];
        u1tmp += j * pixelPro[j];
      }
    }

    u0 = u0tmp / w0;
    u1 = u1tmp / w1;
    u = u0tmp + u1tmp;
    deltaTmp = w0 * (float)pow((u0 - u), 2) + w1 * (float)pow((u1 - u), 2);

    if (deltaTmp > deltaMax) {
      deltaMax = deltaTmp;
      threshold = i;
    }
  }

  //threshold=threshold+20;
  cvThreshold(frame, dst, threshold, 255, CV_THRESH_BINARY);
  return threshold;
}