
//unsigned char img[512 * 512 * 4];

int resize_line(const uchar* A, int an, uchar* B, int bn)
{
  int i, j, r = 0;

  for (i = 0, j = 0; i < bn; ++i) {
    if (r + an < bn) {
      assert(j < an);
      B[i] = A[j];
      r += an;
      //printf("i=%d j=%d B=%d a=%d\n", i, j, bi, x);
    }
    else {
      int bi = 0;
      assert(j < an);
      bi += A[j++] * (bn - r);
      r = r + an - bn;

      for (; r >= bn; r -= bn) {
        assert(j < an);
        bi += A[j++] * (bn);
      }

      if (r) {
        assert(j < an);
        bi += A[j] * (r);
      }

      B[i] = bi / an;
      //printf("i=%d j=%d B=%d a=%d\n", i, j, bi, x);
    }

    //B[i]=A[j]*(n-k)+A[j+(nk)/m+nk%m]*();
  }

  return 0;
}

// 放大会有锯齿，适合缩小
#define ARROPT(N, A, OP)   {int _I=0; for (; _I<(N); ++_I) {(A)[_I] OP;}}
int resize2D(const uchar* A, int aw, int ah, int al, uchar* B, int bw, int bh, int bl)
{
  int* bi = MALLOC(int, bw);
  int i, j, r = 0;

  for (i = 0, j = 0; i < bh; ++i) {
    uchar* B0 = B + i * bl;

    if (r + ah < bh) {
      assert(j < ah);
      //B[i]=A[j];
      resize_line(A + j * al, aw, B0, bw);
      r += ah;
      //printf("i=%d j=%d B=%d a=%d\n", i, j, bi, x);
    }
    else {
      int bhr = bh - r;
      assert(j < ah);
      //bi=A[j]*(bh-r);
      resize_line(A + j * al, aw, B0, bw);
      ARROPT(bw, bi, = bhr * B0[_I]);
      ++j;
      r = r + ah - bh;

      for (; r >= bh; r -= bh, ++j) {
        assert(j < ah);
        //bi+=A[j]*(bh);
        resize_line(A + j * al, aw, B0, bw);
        ARROPT(bw, bi, += bh * B0[_I]);
      }

      if (r) {
        assert(j < ah);
        //bi+=A[j]*(r);
        resize_line(A + j * al, aw, B0, bw);
        ARROPT(bw, bi, += r * B0[_I]);
      }

      //B[i]=bi/ah;
      ARROPT(bw, B0, = bi[_I] / ah);
      //printf("i=%d j=%d B=%d a=%d\n", i, j, bi, x);
    }

    //B[i]=A[j]*(n-k)+A[j+(nk)/m+nk%m]*();
  }

  pfree(bi);
  return 0;
}


int test_resize()
{
  uchar A[100];
  uchar B[100];
  int i, h, w, ch;
  Image* im;

  for (i = 0; i < 10; ++i) {
    A[i] = i + 1;
  }

  resize_line(A, 10, B, 35);
  _chdir("D:/pub/bin/faceRecog/front/std/");
  loadbmpfile("3007_000_165502070.bmp.bmp", 8, 1, im);
  h = im->h, w = im->w;

  for (;;) {
    Image im2[1] = {0};
    newImage(w, h, 1, im2);
    {
      utime_start(_start_time);
      resize2D(im->data, im->h, im->w, im->s, im2->data, im2->h, im2->w, im2->s);
      printf("%f\n", utime_elapsed(_start_time));
    }
    cvShowImage("asdf", im2->h, im2->w, im2->data, im2->s, 1);
    ch = cvWaitKey(-1);

    if ('+' == ch) {
      h = (h * 11 / 10);
      w = (w * 11 / 10);
      printf("%d\n", h);
    }

    if ('-' == ch) {
      h = (h * 10 / 11);
      w = (w * 10 / 11);
      printf("%d\n", h);
    }

    pfree(im2);
  }

  pfree(im);
  return 0;
}
