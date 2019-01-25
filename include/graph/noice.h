

#include "std/img_c.h"
#include "std/gui_c.h"
#include "std/rand_c.h"
//#include "std/draw_c.h"

int addSaltNoise(img_t* im, int n, int aa) {
  int i, j, k;
  if (im->w > 0 && im->h > 0) {
    for (k = 0; k < 2; ++k) {
      for (i = 0; i < n; ++i) {
        int x = rand() % im->w;
        int y = rand() % im->h;
        uchar* p = im->data + y*im->s + x*im->c;
        for (j = 0; j < im->c; ++j) {
          p[j] = aa;
        }
      }
    }
  }
  return 0;
}
// 添加点噪声
int addSaltNoise(img_t* im, int n) {
  addSaltNoise(im, n, 0);
  addSaltNoise(im, n, 255);
  return 0;
}
//生成高斯噪声
double generateGaussianNoise(double mu, double sigma)
{
  //定义小值
  const double epsilon = 0.0000000001;
  static double z0, z1;
  static bool flag = false;
  flag = !flag;
  //flag为假构造高斯随机变量X
  if (!flag)
    return z1 * sigma + mu;
  double u1, u2;
  //构造随机变量
  do
  {
    u1 = rand() * (1.0 / RAND_MAX);
    u2 = rand() * (1.0 / RAND_MAX);
  } while (u1 <= epsilon);
  //flag为真构造高斯随机变量
  z0 = sqrt(-2.0*log(u1))*cos(2 * CC_PI*u2);
  z1 = sqrt(-2.0*log(u1))*sin(2 * CC_PI*u2);
  return z0*sigma + mu;
}

int isContinuous(img_t* im) {
  return im->s == (im->w*im->c);
}
//为图像添加高斯噪声
int addGaussianNoise(img_t* im)
{
  int channels = im->c;
  int rowsNumber = im->h;
  int colsNumber = im->w*channels;
  //判断图像的连续性
  if (isContinuous(im))
  {
    colsNumber *= rowsNumber;
    rowsNumber = 1;
  }
  for (int i = 0; i < rowsNumber; i++) {
    uchar* p = img_row(uchar, im, i);
    for (int j = 0; j < colsNumber; j++) {
      //添加高斯噪声
      int val = p[j] + generateGaussianNoise(2, 0.8) * 32;
      val = BOUND(val, 0, 255);
      p[j] = (uchar)val;
    }
  }
  return 0;
}

#include "PerlinNoise.hpp"

int addPerlinNoise(img_t* im, int Wood, int seed) {
  // Define the size of the image

  // Create a PerlinNoise object with a random permutation vector generated with seed
  PerlinNoise pn(seed);

  unsigned int i, j;
  double n;
  // Visit every pixel of the image and assign a color generated with Perlin noise
  for (i = 0; i < im->h; ++i) {     // y
    uchar* p = img_row(uchar, im, i);
    for (j = 0; j < im->w; ++j) {  // x
      double x = (double)j / ((double)im->w);
      double y = (double)i / ((double)im->h);

      // Wood like structure
      if (Wood) {
        n = 20 * pn.noise(x, y, 0.8);
        n = n - floor(n);
      }
      else {
        // Typical Perlin noise
        n = pn.noise(10 * x, 10 * y, 0.8);
      }

      // Map the values to the [0, 255] interval, for simplicity we use 
      // tones of grey
      int pix = (int)(255 * n);
      pix = BOUND(pix, 0, 255);
      for (int k = 0; k < im->c; ++k) {
        *p++ = pix;
      }
    }
  }

  return 0;
}

int test_noice() {
  img_t im[1] = { 0 };
  imsetsize(im, 600, 800, 1, 1, NULL);
  //addSaltNoise(im, 300);
  while (1) {
    addPerlinNoise(im, 0, rand());
    imshow_(im); waitkey(-1);
  }
  imfree(im);
  return 0;
}