
// n个数中第k(1<=k<=10)小的数
int nth_element(int n, uchar* a, int k)
{
  if (n > k) {
    int i, j, pos[10];
    pos[0] = 0;

    for (i = 1; i < n; ++i) {
      for (j = MIN(k, i); j-- > 0 && !(a[pos[j]] < a[i]);) {
        pos[j + 1] = pos[j];
      }

      pos[j + 1] = i;
    }

    return pos[k - 1];
  }

  return -1;
}

// 比较耗时
int horizontal_filter(buf_t* bf, const img_t* img, int istop, img_t* I5)
{
  img_t I[1] = {0}, I2[1] = {0}, orient_img[1] = {0}, I3[1] = {0}, I4[1] = {0};
  // 进行去除睫毛  水平滤波 horizontal rank filter
  int i, j, ROI_row = img->h, ROI_col = img->w;
  double scaling = 1.0,    sigma = 2,    vert = 1.0,    horz = 8.0;
  bf_imsetsize(bf, I5, ROI_row, ROI_col, 1, 1);
  bf_imsetsize(bf, I, ROI_row, ROI_col, 1, 1);
  imclone(img, I);

#if 0

  //{utime_start(_start_time);
  for (i = 1; i < ROI_row; ++i) {
    for (j = 4; j < ROI_col - 4; ++j) {
      uchar* d =  img->tt.data + img->s * i + j - 3; //
#if 0
      I->tt.data[I->s * i + j] = d[nth_element(7, d, 6)];
#else
      int k = 0, t = 0;

      for (k = 1; k < 7; ++k) {
        if (d[t] < d[k]) {
          t = k;
        }
      }

      I->tt.data[I->s * i + j] = d[t];
#endif
    }
  }

#else
#endif

  //imshow(I); cvWaitKey(-1);
  //printf(" %.4f", utime_elapsed(_start_time));}

  //imshow(I);
  mat2gray_u8(I);
  //imshow(I); cvWaitKey(-1);
  scaling = 1.0, sigma = 2, vert = 1.0, horz = 8.0;
  // generate the edge image
  {
#if 0
    canny(I, sigma, scaling, vert, horz, I2, orient_img);    // edge detection 4-19
    //{utime_start(_start_time);
    //printf(" %.4f", utime_elapsed(_start_time));}
    adjgamma(I2, 1.9, I3);       // 将I2 增强 并normalize to [0, 1]  4-19
    nonmaxsup(I3, orient_img, 1.5, I4);
    //I5 = false(size(I4));
    FPRINT2D("I2.txt", "%8.3f, ", "\n", I2->h, I2->w, I2->tt.f8, I2->s / 8, 1);
    FPRINT2D("I3.txt", "%8.3f, ", "\n", I3->h, I3->w, I3->tt.f8, I3->s / 8, 1);
    FPRINT2D("I4.txt", "%8.3f, ", "\n", I4->h, I4->w, I4->tt.f8, I4->s / 8, 1);

    // 5_30 上下眼睑的定位不同
    if (0 == strcmp(canshu, "top")) {
      for (i = 1; i < round(3 * ROI_row / 5); ++i) {
        for (j = 5; j < ROI_col - 5 ; ++j) {
          if (I4->tt.f8[I4->w * i + j] > 0.32) {
            I5->tt.data[I5->s * i + j] = 255;
          }
          else {
            I5->tt.data[I5->s * i + j] = 0;
          }
        }
      }
    }
    else if (0 == strcmp(canshu, "bottom")) {
      //     for i = 1 : round(3*ROI_row/5)
      for (i = round(ROI_row / 2); i < ROI_row; ++i) {
        for (j = 5; j < ROI_col - 5; ++j) {
          // I5(i,j) = 0;
          if (I4->tt.f8[I4->w * i + j] > 0.25) {
            I5->tt.data[I5->s * i + j] = 255;
          }
          else {
            I5->tt.data[I5->s * i + j] = 0;
          }
        }
      }
    }

#else
    //I5 = false(size(I4));
    canny_fix(bf, I, sigma, scaling, vert, horz, 1.9, 1.5, I4);    // edge detection 4-19
#if 0
    {
      int i;
      double d = 0;
      img_t I44[1] = {0};
      canny(I, sigma, scaling, vert, horz, I2, orient_img);    // edge detection 4-19
      //FPRINT2D("I.txt", "%5.1f, ", "\n", I2->h, I2->w, I2->tt.f8, I2->s/8, 1);
      //{utime_start(_start_time);
      //printf(" %.4f", utime_elapsed(_start_time));}
      adjgamma(I2, 1.9, I3);       // 将I2 增强 并normalize to [0, 1]  4-19
      nonmaxsup(I3, orient_img, 1.5, I44);

      //FPRINT2D("or.txt", "%f\r\n", inimage->w, or->tt.f8, 1);
      //imshow(I44);imshow(I4);cvWaitKey(-1);
      for (i = 0; i < I->h * I->w; ++i) {
        d += ABSSUB(I44->tt.f8[i], I4->tt.u1[i] / 256.);
      }

      printf("d = %f\n", d);
      imfree(I44);
    }
#endif

    // 5_30 上下眼睑的定位不同
    if (istop) {
      for (i = 1; i < round(3 * ROI_row / 5); ++i) {
        for (j = 5; j < ROI_col - 5 ; ++j) {
          if (I4->tt.u1[I4->w * i + j] > (0.22 * 255)) {
            I5->tt.data[I5->s * i + j] = 255;
          }
          else {
            I5->tt.data[I5->s * i + j] = 0;
          }
        }
      }
    }
    else {
      //     for i = 1 : round(3*ROI_row/5)
      for (i = round(ROI_row / 2); i < ROI_row; ++i) {
        for (j = 5; j < ROI_col - 5; ++j) {
          // I5(i,j) = 0;
          if (I4->tt.u1[I4->w * i + j] > (0.25 * 255)) {
            I5->tt.data[I5->s * i + j] = 255;
          }
          else {
            I5->tt.data[I5->s * i + j] = 0;
          }
        }
      }
    }

#endif
  }

  //imread("C:\\11.bmp", 1, 1, I5);
#if 0
  imshow(I2);
  imshow(I3);
  imshow(I4);
  imshow(I5);
  //cvWaitKey(-1);
#endif

  //imshow(img);imshow(I5);imshow(I);cvWaitKey(-1);

  bf_imfree(bf, I4);
  bf_imfree(bf, I);
  bf_imfree(bf, I2);
  bf_imfree(bf, orient_img);
  bf_imfree(bf, I3);
  return 0;
}

