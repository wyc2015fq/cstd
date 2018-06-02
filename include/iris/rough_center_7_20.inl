
IPOINT rough_center_7_20(buf_t* bf, const img_t* I)
{
  int ii, jj, n, ind = 0, row, col, x1, x2, y1, y2;
  IPOINT pt;
  // 7_12 应该找到第一个波峰与第二个波峰之间的值，暂时定为30/255 = 0.1171
  img_t Ibw[1] = {0}, Ier[1] = {0}, Ifi[1] = {0}, I_ind[1] = {0};
  int h = I->h, w = I->w, ithd;
  int buflen = bf->len;
  int* hist = 0;
  bf_imsetsize(bf, Ibw, h, w, 1, 1);
  bf_imsetsize(bf, Ier, h, w, 1, 1);
  bf_imsetsize(bf, Ifi, h, w, 1, 1);
  bf_imsetsize(bf, I_ind, h, w, 1, 1);
  BFMALLOC(bf, hist, 256);

  im2bw_adapt(I, 29, Ibw, 255, 0); // (int)(255*0.1161)
  //memset(Ibw->tt.data, 255, h*w);
  ithd = myerode(bf, Ibw, SE5, Ier);
  //imshow(I); imshow(Ibw); imshow(Ier); cvWaitKey(-1);
  my_fill(bf, Ier, Ifi);
  n = I->h * I->w;
  row = I->h, col = I->w;
  //imshow(I); imshow(Ibw); imshow(Ier); cvWaitKey(-1);
  // 统计最后的为1的区域 找到最大区域
  my_connect_domain(Ifi, I_ind);

  for (ii = 0; ii < 256; ++ii) {
    hist[ii] = 0;
  }

  for (jj = 0; jj < n; ++jj) {
    int tt = I_ind->tt.data[jj];
    hist[tt]++;
  }

  ind = 1;

  for (ii = 2; ii < 256; ++ii) {
    if (hist[ii] > hist[ind]) {
      ind = ii;
    }
  }

  x1 = 0, x2 = col, y1 = 0, y2 = row;
#if 0

  for (ii = 0; ii < row; ++ii) {
    for (jj = 0; jj < col; ++jj) {
      if (I_ind->tt.data[ii * I_ind->s + jj] == ind) {
        if (jj < x1) {
          x1 = ii;
          y1 = jj;
        }

        if (jj > x2) {
          x2 = ii;
          y2 = jj;
        }
      }
    }
  }

#else

  for (jj = 0; jj < col; ++jj) {
    for (ii = 0; ii < row; ++ii) {
      if (I_ind->tt.data[ii * I_ind->s + jj] == ind) {
        y1 = ii;
        x1 = jj;
        break;
      }
    }
  }

  for (jj = 0; jj < col; ++jj) {
    for (ii = 0; ii < row; ++ii) {
      if (I_ind->tt.data[(row - 1 - ii) * I_ind->s + col - 1 - jj] == ind) {
        y2 = row - 1 - ii;
        x2 = col - 1 - jj;
        break;
      }
    }
  }

  //imshow(I_ind);cvWaitKey(-1);
#endif

  pt.x = 1 + ((x1 + x2 + col) / 4);
  pt.y = 1 + ((y1 + y2 + row) / 4);
  //imdraw_circle(I, pt.x, pt.y, 4, CV_RGB(255, 0, 0), 0, 0);
  //imshow(Ibw); imshow(Ier); imshow(Ifi); imshow(I); cvWaitKey(-1);
  //pt.x = hist[ind]; pt.y = ithd;
  BFFREE(bf, hist, 256);
  bf_imfree(bf, I_ind);
  bf_imfree(bf, Ifi);
  bf_imfree(bf, Ier);
  bf_imfree(bf, Ibw);

  ASSERT(buflen == bf->len);
  return pt;
}
