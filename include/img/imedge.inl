
// sobel垂直边缘检测
int abssobel_v(const img_t* im, img_t* sob)
{
  // h=[-1 0 1 ;-2 0 2 ;-1 0 1];      //sobel垂直算子
  img_t sob16[1] = {0};
  unsigned char* A0 = im->tt.data;
  int i, j, h = im->h, w = im->w, al = im->s;
  int maxpix = 0;

  imsetsize(sob, im->h, im->w, 1, 1);
  imsetsize(sob16, im->h, im->w, sizeof(short), 1);

  for (i = 1; i < h - 1; ++i) {
    for (j = 1; j < w - 1; ++j) {
      unsigned char* A = A0 + i * al + j;
      int ss = A[-al + 1] + A[1] * 2 + A[al + 1] - A[-al - 1] - A[-1] * 2 - A[al - 1];
      ss = abs(ss);
      sob16->tt.s2[i * w + j] = ss;

      if (ss > maxpix) {
        maxpix = ss;
      }
    }
  }

  //归一化至0-255范围内
  if (maxpix > 0) {
    for (i = 0; i < h * w; ++i) {
      sob->tt.data[i] = sob16->tt.s2[i] * 255 / maxpix;
    }
  }

  imfree(sob16);
  return 0;
}
