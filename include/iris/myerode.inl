
// ¸¯Ê´
#define SE5 0
#define SE3 1
int myerode(buf_t* bf, const img_t* im, int canshu, img_t* I)
{
  int rc, row_se = 0, i, ii, jj, sumse = 0;
  int* off = 0;
#if 0
  static const uchar se5[] = {//
    0, 0, 1, 1, 1, 1, 1, 0, 0, //
    0, 1, 1, 1, 1, 1, 1, 1, 0, //
    1, 1, 1, 1, 1, 1, 1, 1, 1, //
    1, 1, 1, 1, 1, 1, 1, 1, 1, //
    1, 1, 1, 1, 1, 1, 1, 1, 1, //
    1, 1, 1, 1, 1, 1, 1, 1, 1, //
    1, 1, 1, 1, 1, 1, 1, 1, 1, //
    0, 1, 1, 1, 1, 1, 1, 1, 0, //
    0, 0, 1, 1, 1, 1, 1, 0, 0 //
  };
  static const uchar se3[] = {//
    1, 1, 1, 1, 1, //
    1, 1, 1, 1, 1, //
    1, 1, 1, 1, 1, //
    1, 1, 1, 1, 1, //
    1, 1, 1, 1, 1 //
  };
  const uchar* seee = se5;
#endif
  row_se = 9;

  if (canshu == SE3) {
    //seee = se3;
    row_se = 5;
  }

  rc = (int)(row_se / 2);
  sumse = 0;
  BFMALLOC(bf, off, row_se * row_se);

  for (ii = 0; ii < row_se; ++ii) {
    for (jj = 0; jj < row_se; ++jj) {
      off[sumse++] = (ii - rc) * im->s + (jj - rc);
    }
  }

  for (ii = 0; ii < im->h; ++ii) {
    for (jj = 0; jj < im->w; ++jj) {
      I->tt.data[ii * I->s + jj] = 0;
    }
  }

  for (ii = rc ; ii < im->h - rc - 1; ++ii) {
    uchar* iout = I->tt.data + (ii) * I->s;

    for (jj = rc ; jj < im->w - rc - 1; ++jj) {
      //int sum_juanji = sum(sum ( im(ii-rc:ii+rc, jj-rc:jj+rc).* se));
      const uchar* pos0 = im->tt.data + (ii) * im->s + (jj);

      if (*pos0) {
        iout[jj] = 255;

        for (i = 0; i < sumse; ++i) {
          if (!pos0[off[i]]) {
            iout[jj] = 0;
            break;
          }
        }
      }
    }
  }

  BFFREE(bf, off, row_se * row_se);

  return sumse;
}
