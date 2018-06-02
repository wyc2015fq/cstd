/*--------------------------- demo_DCTdenoising -------------------------*/
// This code implements "DCT image denoising: a simple and effective image
// denoising algorithm".
// http://www.ipol.im/pub/algo/ys_dct_denoising
// Copyright, Guoshen Yu, Guillermo Sapiro, 2010.
// Please report bugs and/or send comments to Guoshen Yu
// yu@cmap.polytechnique.fr
/*---------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <vector>
#include <iostream>
using namespace std;
//#include "img/imgio.inl"
int io_png_write_f32(const char* fn, const float* data, size_t nxp, size_t nyp, size_t ncp)
{
  img_t im[1] = {0};
  int x, y, c;
  imsetsize(im, nyp, nxp, ncp, 1);
  for (y = 0; y < im->h; ++y) {
    for (x = 0; x < im->w; ++x) {
      for (c = 0; c < im->c; ++c) {
        im->tt.data[y * im->s + x * im->c + c] = (int)(data[c * (im->h * im->w) + y * im->w + x]);
      }
    }
  }
  imwrite(fn, im);
  imfree(im);
  return 0;
}
float* io_png_read_f32(const char* fn, size_t* nxp, size_t* nyp, size_t* ncp)
{
  img_t im[1] = {0};
  float* data = NULL;
  if (imread(fn, 0, 1, im)) {
    int x, y, c;
    MYREALLOC(data, im->h * im->w * im->c);
    for (y = 0; y < im->h; ++y) {
      for (x = 0; x < im->w; ++x) {
        for (c = 0; c < im->c; ++c) {
          data[c * (im->h * im->w) + y * im->w + x] = im->tt.data[y * im->s + x * im->c + c];
        }
      }
    }
    *nxp = im->w, *nyp = im->h, *ncp = im->c;
    //io_png_write_f32("C:/aaa.bmp", data, *nxp, *nyp, *ncp);
    imfree(im);
  }
  return data;
}
#include "DCTdenoising.inl"
#include "mt19937ar.inl"
#ifndef M_PI
/**
 * M_PI is a POSIX definition for Pi
 */
#define M_PI 3.14159265358979323846
#endif /* !M_PI */
void addnoise(vector<float>&, vector<float>&, int, float);
int tool_DCTdenoising(const char* ImClean, float sigma, const char* ImNoisy, const char* ImDenoised, int flag_dct16x16)
{
  //////////////////////////////////////////////// Input
  // Read image
  float* iarr1;
  size_t w1, h1, c1;
  if (NULL == (iarr1 = io_png_read_f32(ImClean, &w1, &h1, &c1))) {
    std::cerr << "Unable to load image file " << ImClean << std::endl;
    return 1;
  }
  std::vector<float> ipixels(iarr1, iarr1 + w1 * h1 * c1);
  FREE(iarr1);
  if ((c1 != 1) && (c1 != 3)) {
    printf("Error: Input image should have 1 channel (gray-level) or 3 \
 channels (color). \n");
    exit(1);
  }
  int size = w1 * h1 * c1;
  // Read standard deviation of Gaussian white noise
  ////////// Add Gaussian white noise
  std::vector<float> npixels;
  npixels.resize(w1 * h1 * c1);
  addnoise(ipixels, npixels, size, sigma);
  ////////////////////////////////////////////////// Denoising
  std::vector<float> opixels;
  opixels.resize(w1 * h1 * c1);
  DCTdenoising(npixels, opixels, w1, h1, c1, sigma, flag_dct16x16);
  // Save output noisy image
  float* out = new float[w1 * h1 * c1];
  int i;
  for (i = 0; i < size; i++) {
    out[i] = npixels[i];
  }
  io_png_write_f32(ImNoisy, out, w1, h1, c1);
  // Save output denoised image
  for (i = 0; i < size; i++) {
    out[i] = opixels[i];
  }
  io_png_write_f32(ImDenoised, out, w1, h1, c1);
  delete[] out; /*memcheck*/
  return 0;
}
int test_DCTdenoising(int argc, char** argv)
{
  sys_chdir("E:/pub/bin/img2");
  tool_DCTdenoising("chilli.png", 2, "chilliNoisy.png", "chilliDenoised.png", 0);
  return 0;
}
int test_DCTdenoising1(int argc, char** argv)
{
  if ((argc != 5) && (argc != 6)) {
    std::cerr << " ************************************************************************** "
        << std::endl
        << " *************************** DCT image denoising ************************ "
        << std::endl
        << " ************************************************************************** "
        << std::endl
        << "Usage: " << argv[0] << " ImClean.png sigma ImNoisy.png ImDenoised.png "
        << std::endl
        << "Input" << std::endl
        << "ImClean: clean image, gray-level (1 channel) or color (3 channels) in PNG. "
        << std::endl
        << "sigma: standard deviation of i.i.d. Gaussian white noise to be added to ImClean. "
        << std::endl
        << "Output" << std::endl
        << "ImNoisy: noisy image in PNG. " << std::endl
        << "ImDenoised: denoised image in PNG. " << std::endl
        << "[optional 1/0]. 0: DCT window size 16x16 (default). 1: DCT window size 8x8."
        << std::endl
        << " ************************************************************************** "
        << std::endl
        << " ********************* Guoshen Yu, Guillermo Sapiro 2010 **************** "
        << std::endl
        << " ************************************************************************** "
        << std::endl;
    return 1;
  }
  int flag_dct16x16 = 0;
  if (argc == 6) {
    flag_dct16x16 = atoi(argv[5]);
  }
  tool_DCTdenoising(argv[1], atof(argv[2]), argv[3], argv[4], flag_dct16x16);
  return 0;
}
// Add Gaussian iid white noise of std sigma to the image ipixels
// of size height*width*channel. The output is stored in npixels.
void addnoise(vector<float>& ipixels, vector<float>& npixels, int size,
    float sigma)
{
  mt_init_genrand((unsigned long int) time(NULL));
  for (int i = 0; i < size; i++)
    npixels[i] = ipixels[i] +
        (float)((double) sigma
            * sqrt(-2.0 * log(mt_genrand_res53()))
            * cos(2.0 * M_PI * mt_genrand_res53()));
}

