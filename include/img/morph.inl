
// 形态学
int median(int h, int w, const uchar* A, int al, uchar* B, int bl, int kk)
{
  int i, j, k, kn;
  int pos[256] = {0};
  k = 0;
  ASSERT(kk < 16);

  for (i = 0; i < kk; ++i) {
    for (j = 0; j < kk; ++j) {
      pos[k++] = (i - kk / 2) * al + (j - kk / 2);
    }
  }

  kn = k;

  for (i = 0; i < h; ++i) {
    for (j = 0; j < w; ++j) {
      int sv = 0, sn = 0;

      for (k = 0; k < kn; ++k) {
        int pos1 = i * al + j + pos[k];

        if (pos1 >= 0 && pos1 < h * w) {
          sv += A[pos1];
          ++sn;
        }
      }

      ASSERT(sn > 0);
      B[i * bl + j] = sv / sn;
    }
  }

  return 0;
}

int immedian(const img_t* im, img_t* out, int kk)
{
  img_t im2[1] = {0};
  imsetsamesize(im2, im);
  median(im->h, im->w, im->tt.data, im->s, im2->tt.data, im2->s, kk);
  imswap(out, im2), imfree(im2);
  return 0;
}

int dilate(int h, int w, const uchar* A, int al, uchar* B, int bl,
    int kh, int kw, const uchar* K, int kl, int kx, int ky, int is_dilate)
{
  int kstep[256] = {0};
  int kn = 0, i, j, k, n = h * w;
  ASSERT(kh * kw < 256);

  for (i = 0; i < kh; ++i) {
    for (j = 0; j < kw; ++j) {
      if (K[i * kl + j]) {
        kstep[kn] = (i - ky) * al + (j - kx);
        ++kn;
      }
    }
  }

  if (is_dilate) {
    for (i = 0; i < h; ++i) {
      for (j = 0; j < w; ++j) {
        int pos = i * al + j;
        int t = A[pos];

        for (k = 0; k < kn; ++k) {
          int pos1 = pos + kstep[k];

          if (pos1 >= 0 && pos1 < n && t < A[pos1]) {
            t = A[pos1];
          }
        }

        B[i * bl + j] = t;
      }
    }
  }
  else {
    for (i = 0; i < h; ++i) {
      for (j = 0; j < w; ++j) {
        int pos = i * al + j;
        int t = A[pos];

        for (k = 0; k < kn; ++k) {
          int pos1 = pos + kstep[k];

          if (pos1 >= 0 && pos1 < n && t > A[pos1]) {
            t = A[pos1];
          }
        }

        B[i * bl + j] = t;
      }
    }
  }

  return 0;
}

int dilate_rect(buf_t* bf, int h, int w, const uchar* A, int al, uchar* B, int bl,
    int kh, int kw, int kx, int ky, int is_dilate)
{
  uchar K[256];
  uchar* C;
  int n = MAX(kh, kw);
  BFMALLOC(bf, C, h * w);
  ASSERT(n < sizeof(K));
  memset(K, 1, n);
  dilate(h, w, A, al, C, w, 1, kw, K, 1, 0, ky, is_dilate);
  dilate(h, w, C, w, B, bl, kh, 1, K, 1, kx, 0, is_dilate);
  BFFREE(bf, C, h * w);
  return 0;
}

// minareas 邻域搜索 4或者8
// minareas 最小面积 去掉小于minareas的块
int bwlabel(buf_t* bf, int h, int w, const uchar* A, int al, int* tmp, int bl, int neighbours,
    int* areas, int areanum, int minareas, const uchar* map, uchar* C, int cl)
{
  int* queue;
  int m = h, n = w, i, j, k;
  const uchar* s = A;
  int label = 1;
  int neighbour[8];
  int neighbour8[][2] = { -1, 0, 0, -1, 0, 1, 1, 0, -1, -1, -1, 1, 1, -1, 1, 1}; //和当前像素坐标相加得到八个邻域坐标
  ASSERT(neighbours <= 8);
  BFMALLOC(bf, queue, h * w);

  for (k = 0; k < neighbours; ++k) {
    neighbour[k] = neighbour8[k][0] * al + neighbour8[k][1];
  }

  for (i = 0; i < m; ++i) {
    for (j = 0; j < n; ++j) {
      tmp[i * bl + j] = 0;
    }
  }

  for (i = 1; i < m - 1; ++i) {
    for (j = 1; j < n - 1; ++j) {
      if (s[i * al + j] && tmp[i * bl + j] == 0) {
        int* queue_head = queue;     //队列头
        int* queue_tail = queue;     //队列尾
        tmp[i * bl + j] = label;
        *queue_tail++ = i * al + j;  //用元组模拟队列，当前坐标入列

        while (queue_head != queue_tail) {
          int pix = *--queue_tail;

          for (k = 0; k < neighbours; ++k) {          //n邻域搜索
            int pix1 = pix + neighbour[k];

            if (pix1 >= 0 && pix1 < h * w) {
              int pix2 = (pix1 / al) * bl + (pix1 % al);

              if (s[pix1] && tmp[pix2] == 0) { //如果当前像素邻域像素为1并且标记图像的这个邻域像素没有被标记，那么标记
                tmp[pix2] = label;
                *queue_tail++ = pix1;
                ASSERT(queue_tail - queue_head < h * w);
              }
            }
          }
        }

        label = label + 1;
      }
    }
  }

  if (areas) {
    for (i = 0; i < areanum; ++i) {
      areas[i] = 0;
    }

    for (i = 0; i < m; ++i) {
      for (j = 0; j < n; ++j) {
        int t = tmp[i * bl + j];

        if (t < areanum) {
          areas[t]++;
        }
      }
    }

    if (minareas > 0) {
      k = 1;

      for (i = 0; i < label + 1; ++i) {
        queue[i] = 0;
      }

      for (i = 1; i < areanum; ++i) {
        queue[i] = 0;

        if (areas[i] > minareas) {
          queue[i] = k++;
        }
      }

      for (i = 0; i < m; ++i) {
        for (j = 0; j < n; ++j) {
          int t = tmp[i * bl + j];
          tmp[i * bl + j] = queue[t];
          ASSERT(tmp[i * bl + j] < 300);
        }
      }
    }

    if (map) {
      for (i = 0; i < m; ++i) {
        for (j = 0; j < n; ++j) {
          int t = tmp[i * bl + j];
          C[i * cl + j] = map[t];
        }
      }
    }
  }

  BFFREE(bf, queue, h * w);
  return label;
}


#if 0


#include "cvcanny.inl"

int test_bwlabel3()
{
  //BW=edge(I,'sobel',0.01); % 采用sobel求取边缘，阈值为0.01
  //[L N]= bwlabel(BW,8); % 8连通标记，L为标记后的边缘，N为标记区域数目
  img_t im[1] = {0};
  img_t im2[1] = {0};
  int h, w, al, ai, i = 0;
  _chdir("D:\\pub\\bin\\imgproc\\pic");
  imread("test_bwlabel.bmp", 1, 1, im);
  imclone(im, im2);
  h = im->h, w = im->w, al = im->s, ai = im->c;
  memset(im2->tt.data, 0, h * w);
  cvCanny(h, w, im->tt.data, al, 1, im2->tt.data, al, 1, 50, 101);
  imshow2("asdf", im2);
  cvWaitKey(-1);
  imfree(im);
  return 0;
}
int test_bwlabel2()
{
  //BW=edge(I,'sobel',0.01); % 采用sobel求取边缘，阈值为0.01
  //[L N]= bwlabel(BW,8); % 8连通标记，L为标记后的边缘，N为标记区域数目
  img_t im[1] = {0};
  img_t im2[1] = {0};
  int h, w, al, ai, i, n;
  int* tmp;
  int* tmp1;
  int* areas;
  _chdir("D:\\pub\\bin\\imgproc\\pic");
  imread("test_bwlabel_bw.bmp", 1, 1, im);
  imclone(im, im2);
  h = im->h, w = im->w, al = im->s, ai = im->c;
  memset(im2->tt.data, 0, h * w);
  tmp = (int*)pmalloc(h * w * sizeof(int));
  tmp1 = (int*)pmalloc(h * w * sizeof(int));
  areas = (int*)pmalloc(h * w * sizeof(int));

  for (i = 0; i < 100; ++i) {
    {
      utime_start(_start_time);
      n = bwlabel(h, w, im->tt.data, w, tmp, w, 8, areas, h * w, 0, 0, 0, 0);
      printf("%f\n", utime_elapsed(_start_time));
    }
  }

  for (i = 0; i < n; ++i) {
    tmp1[i] = i;
  }

#define TEST_BWLABEL2_LESS(a, b)  (areas[a]>areas[b])
  QSORT(tmp1, tmp1 + n, TEST_BWLABEL2_LESS, int);
#undef TEST_BWLABEL2_LESS

  for (i = 0; i < h * w; ++i) {
    if (tmp[i] == tmp1[1]) {
      im2->tt.data[i] = 55;
    }

    if (tmp[i] == tmp1[2]) {
      im2->tt.data[i] = 155;
    }
  }

  imshow2("asdf", im2);
  cvWaitKey(-1);
  pfree(tmp);
  pfree(tmp1);
  pfree(areas);
  imfree(im);
  return 0;
}

int test_bwlabel()
{
  uchar s[9 * 17] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0,
    0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0,
    0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0,
    0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0,
    0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  };
  int h = 9, w = 17, i, j;
  int tmp[9 * 17] = {0};
  bwlabel(9, 17, s, 17, tmp, 17, 4, 0, 0, 0, 0, 0, 0);

  for (i = 0; i < h; ++i) {
    for (j = 0; j < w; ++j) {
      printf("%1d", tmp[i * w + j]);
    }

    printf("\n");
  }

  //cvShowImage( "img", h, w, img, w, 1 );
  //cvWaitKey ( -1 );
  return 0;
}

int sobel_abs(int h, int w, const uchar* A, int al, uchar* B, int bl)
{
  int i, j;

  for (i = 0; i < h; ++i) {
    const uchar* A0, * A1, * A2;
    A0 = A + al * MAX(i - 1, 0);
    A1 = A + al * i;
    A2 = A + al * MIN(i + 1, h - 1);

    for (j = 0; j < w; ++j) {
      int l, r, x, ll, rr;
      l = MAX(j - 1, 0);
      r = MIN(j + 1, w - 1);
      rr = A0[ r ] + (A1[ r ] << 2) + A2[ r ];
      ll = A0[ l ] + (A1[ l ] << 2) + A2[ l ];
      x = ABSSUB(rr, ll);
      B[ i * bl + j ] = MIN(x, 255);
      B[ i * bl + j ] = x < (26) ? 0 : 255;
    }
  }

  return 0;
}

#include "cvutils.inl"
#include "cvsmooth1.inl"
#include "cvderiv1.inl"

int test_bwlabel1()
{
  //BW=edge(I,'sobel',0.01); % 采用sobel求取边缘，阈值为0.01
  //[L N]= bwlabel(BW,8); % 8连通标记，L为标记后的边缘，N为标记区域数目
  img_t* im;
  img_t* im2;
  int h, w, al, ai, i;
  _chdir("D:\\pub\\bin\\imgproc\\pic");
  im = imread2("test_bwlabel.bmp", 1);
  im2 = imclone(im);
  h = im->h, w = im->w, al = im->s, ai = im->c;
  memset(im2->tt.data, 0, h * w);

  if (0) {
    sobel_abs(h, w, im->tt.data, al, im2->tt.data, im2->s);
  }
  else {
    short* out01 = (short*)pmalloc(h * w * 2);
    short* out10 = (short*)pmalloc(h * w * 2);
    CvSize sz = cvSize(w, h);
    cvSobel(im->tt.data, al, out01, w, sz, 0, 1, 3);
    cvSobel(im->tt.data, al, out10, w, sz, 1, 0, 3);

    for (i = 0; i < h * w; ++i) {
      int ff = abs(out01[i]) + abs(out10[i]);
      im2->tt.data[i] = ff < 80 ? 0 : 255;
    }

    pfree(out01);
    pfree(out10);
  }

  imshow2("asdf", im2);
  cvWaitKey(-1);
  imfree(im);
  return 0;
}

int test_imdilate()
{
  FILE* plisttxt;
  //bgmodel bg;
  char line[256];
  int areas[100];
  uchar map[400];
  uchar K1[20 * 20] = {1};
  memset(K1, 1, 400);
  memset(map, 255, 400);
  map[0] = 0;
  //cvCreateTrackbar ( "high", "cam", &high, 300, 0 );
  _chdir("D:/pub/bin/imgproc/depth");
  plisttxt = fopen("list.txt", "rb");

  if (NULL == plisttxt) {
    return 0;
  }

  {
    int fram = 0;
    char ch = 0;
    uchar* img, * img2, * img1, * img3;
    int* label;

    for (; 'q' != ch && 'Q' != ch && fgets(line, 256, plisttxt) > 0; ++fram) {
      int h, w, al, ai;
      img_t* im2;
      strtrim(line, "\n\r");
      im2 = imread2(line, 1);
      img = im2->tt.data;
      h = im2->h, w = im2->w, al = im2->s, ai = im2->c;
      img1 = (uchar*)pmalloc(h * w);
      img2 = (uchar*)pmalloc(h * w);
      img3 = (uchar*)pmalloc(h * w);
      label = (int*)pmalloc(h * w * sizeof(int));
      {
        img_t* im3;
        char buf[256];
        _snprintf(buf, 256, "%s_fg_dep_bwlabel.bmp", line);
        im3 = imread2(buf, 1);

        if (im3) {
          int i, n = 0;

          for (i = 0; i < h * w; ++i) {
            if (0 == im3->tt.data[i]) {
              im2->tt.data[i] = 0;
            }
            else {
              ++n;
            }
          }

          imfree(im3);
        }
      }

      if (0) {
        utime_start(_start_time);
        imdilate_rect(h, w, img, al, img1, w, 10, 10, 5, 5, 0);
        //memcpy(img1, img, h*w);
        bwlabel(h, w, img1, w, label, w, 4, areas, 100, 1500, map, img3, w);
        imdilate_rect(h, w, img3, w, img2, w, 15, 15, 7, 7, 1);
        printf("%f\n", utime_elapsed(_start_time));
      }

      if (1) {
        utime_start(_start_time);
        //Rainclusteredit(h, w, img, al, 25, 5, img1, w);
        printf("%s, %f\n", line, utime_elapsed(_start_time));
      }

      cvShowImage("img", h, w, img, al, ai);
      cvShowImage("img1", h, w, img1, w, 1);
      cvShowImage("img2", h, w, img2, w, 1);
      cvShowImage("img3", h, w, img3, w, 1);
      imfree(im2);
      SAFEFREE(img1);
      SAFEFREE(img2);
      SAFEFREE(img3);
      SAFEFREE(label);
      ch = cvWaitKey(10);
      //ch = cvWaitKey ( -1 );
    }
  }

  return 0;
}
#endif
