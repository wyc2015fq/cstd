#include "geo/packing.inl"
// 矩形a减去矩形b 会得到0~4个新矩形
// c=a-b
int rec_sub(const REC* a, const REC* b, REC* c)
{
  int i = 0;
  if (COVER2(b->y, RECY2(*b), a->y, RECY2(*a))) {
    if (COVER1(b->x, a->x, RECX2(*a))) {
      RECSET(c[i], a->x, a->y, b->x - a->x, a->h), ++i;
    }
    if (COVER1(RECX2(*b), a->x, RECX2(*a))) {
      RECSET(c[i], RECX2(*b), a->y, RECX2(*a) - RECX2(*b), a->h), ++i;
    }
  }
  if (COVER2(b->x, RECX2(*b), a->x, RECX2(*a))) {
    if (COVER1(b->y, a->y, RECY2(*a))) {
      RECSET(c[i], a->x, a->y, a->w, b->y - a->y), ++i;
    }
    if (COVER1(RECY2(*b), a->y, RECY2(*a))) {
      RECSET(c[i], a->x, RECY2(*b), a->w, RECY2(*a) - RECY2(*b)), ++i;
    }
  }
  return i;
}
REC le[10000000] = {0};
// 剩余矩形法
int packing_11(int width, int n, REC* rec)
{
  int i, j, k, y = 0, x = 0, w = width, max_y = 0, m = 0;
  int min_w = 10000000, min_h = 10000000;
  RECSET(le[0], 0, 0, width, 100 * width), m = 1;
  for (i = 0; i < n; ++i) {
    min_w = MIN(min_w, rec[i].w);
    min_h = MIN(min_h, rec[i].h);
  }
  for (i = 0; i < n; ++i) {
    int min_y = 100000000;
    int jj = m;
    for (j = 0; j < m; ++j) {
      if (rec[i].h < le[j].h && rec[i].w < le[j].w) {
        int t = le[j].y;//*width+le[j].x;
        if (t < min_y) {
          min_y = t;
          jj = j;
        }
      }
    }
    j = jj;
    if (j < m) {
      rec[i].x = le[j].x, rec[i].y = le[j].y;
      k = m;
      max_y = MAX(max_y, RECY2(rec[i]));
      for (j = 0; j < m; ++j) {
        int c, d, t = rec_sub(le + j, rec + i, le + k);
        for (d = c = 0; c < t; ++c) {
          if (le[k + c].w > min_w && le[k + c].h > min_h) {
            le[k + d++] = le[k + c];
          }
        }
        if (0 == d && t > 0) {
          le[j].w = 0;
        }
        t = d;
        if (t > 0) {
          k += t - 1;
          le[j] = le[k];
        }
      }
      if (k > countof(le)) {
        int asdf = 0;
      }
      if (0) {
        printf("i %d: %d %d %d %d\n", i, rec[i].x, rec[i].y, rec[i].w, rec[i].h);
        printf("%d ==========================\n", k);
        for (j = 0; j < k; ++j) {
          //le[j - m] = le[j];
          printf("j%d: %d %d %d %d\n", j, le[j].x, le[j].y, le[j].w, le[j].h);
        }
        printf("%d ==========================\n", m);
      }
      m = k;
    }
    else {
      return 0;
    }
  }
  printf("%d ==========================\n", m);
  return max_y;
}
typedef int (*packing_fun)(int width, int n, REC* rec);
int test_packing2_impl(packing_fun fun)
{
  REC rec[50];
  int i, j;
  double rate1 = 0, time1, max_y1 = 0;
  int n = countof(rec), width = 3000, t = 100;
  srand(time(0));
  {
    utime_start(_start_time);
    for (j = 0; j < t; ++j) {
      for (i = 0; i < n; ++i) {
        rec[i].h = 100 + rand() % 300;
        rec[i].w = 100 + rand() % 300;
      }
      packing_sorth(n, rec);
      max_y1 += fun(width, n, rec);
      rate1 += packing_rate(n, rec);
    }
    time1 = utime_elapsed(_start_time);
  }
  printf("n=%d, max_y=%.f, rate=%.3f, time=%.3f\n", n, max_y1 / t, rate1 / t, time1);
  return 0;
}
int test_packing2()
{
  //test_packing2_impl(packing_BL);
  test_packing2_impl(packing_FFD);
  test_packing2_impl(packing_FFD2);
  return 0;
}
int test_packing()
{
  DWORD colortable[] = {
#define COLDEF(R, G, B, NAME) _RGB(R, G, B),
    //#include "colortable.txt"
#undef COLDEF
    0
  };
  DWORD clrText = _RGB(0, 0, 0);
  int i, width; //板材宽度
  int max_y;
  IRECT rc;
  //char buf[256];
  REC* rec = NULL;
  int rec_num = 0;
  double rate, time;
  if (0) {
    rec_num = 10;
    MYREALLOC(rec, rec_num);
    rec[0].h = 420, rec[0].w = 594;
    rec[1].h = 841, rec[1].w = 1189;
    rec[2].h = 420, rec[2].w = 594;
    rec[3].h = 210, rec[3].w = 297;
    rec[4].h = 841, rec[4].w = 594;
    rec[5].h = 841, rec[5].w = 594;
    rec[6].h = 841, rec[6].w = 1189;
    rec[7].h = 210, rec[7].w = 297;
    rec[8].h = 420, rec[8].w = 594;
    rec[9].h = 210, rec[9].w = 297;
  }
  if (0) {
    int MIN_SUBRECT = 15;
    int MAX_SUBRECT = 50;
    srand((0));
    //srand(time(0));
    rec_num = 1000;
    MYREALLOC(rec, rec_num);
    for (i = 0; i < rec_num; ++i) {
      rec[i].h = (rand() % MAX_SUBRECT) + MIN_SUBRECT;
      rec[i].w = (rand() % MAX_SUBRECT) + MIN_SUBRECT;
    }
  }
  width = 1024;
#if 1
  if (1) {
    font_t fo[1] = {0};
    int i, n = 0;
    font_bit_t bit[1] = {0};
    rec_num = 7000;
    font_init_memraster(fo, "songti", songti_12);
    MYREALLOC(rec, rec_num);
    for (i = 0; i < (1 << 16) && n < rec_num; ++i) {
      if (fo->getbit(fo, i, bit)) {
        //font_bit_print(i, bit);
        rec[n].w = bit->bx;
        rec[n].h = bit->by;
        rec[n].i = i;
        ++n;
      }
    }
    rec_num = n;
    if (0) {
      char* p = NULL;
      int n;
      savefile("C:\\aaa.dat", songti_12, sizeof(songti_12));
      MYREALLOC(p, sizeof(songti_12));
      n = HUFF_Encode(songti_12, sizeof(songti_12), p, sizeof(songti_12));
      //n = LZWEncode((uchar*)songti_12, sizeof(songti_12), (uchar*)p);
      savefile("C:\\bbb.dat", p, n);
    }
    if (1) {
      img_t im[1] = {0};
      {utime_start(_start_time);
      packing_sortw(rec_num, rec);
      packing_sorth(rec_num, rec);
printf("%f\n", utime_elapsed(_start_time));}
      max_y = packing_FFD2(width, rec_num, rec);
      imsetsize(im, max_y, width, 1, 1);
      //imdraw_fill(im, Color_White);
      for (n=0;n<rec_num; ++n) {
        i = rec[n].i;
        if (fo->getbit(fo, i, bit)) {
          font_bit_copy(bit, im->tt.data + rec[n].x*im->c + rec[n].y*im->s, im->s);
        }
      }
      imwrite("C:\\aaa.png", im);
      imshow(im);
      cvWaitKey(-1);
    }
  }
#endif
  //packing_sorth(n, rec);
  {
    utime_start(_start_time);
    packing_sortw(rec_num, rec);
    packing_sorth(rec_num, rec);
    //max_y = packing_BL(width, rec_num, rec);
    //max_y = packing_FFD(width, rec_num, rec);
    max_y = packing_FFD2(width, rec_num, rec);
    //max_y = packing_11(width, rec_num, rec);
    time = utime_elapsed(_start_time);
  }
  if (1) {
    img_t im[1] = {0};
    int ss = 1;
    if (max_y>1024) {
      ss=2;
    }
    max_y/=ss;
    width/=ss;
    imsetsize(im, max_y, width, 4, 1);
    imdraw_fill(im, Color_White);
    if (1) {
      rate = packing_rate(rec_num, rec);
      printf("max_y=%d, rate=%.3f, time=%.5f", max_y, rate, time);
      //rc = iRECT(0, 0, width / ss, max_y / ss);
      //imdraw_rect(im, NULL, rc, 0, clrText);
      //rc = iRECT(0, max_y / ss, width / ss, rc.bottom + 20);
      //imdraw_text(im, buf, -1, &rc, DT_CENTER | DT_VCENTER, clrText);
    }
    for (i = 0; i < rec_num; i++) {
      rc = iRECT2(rec[i].x / ss, rec[i].y / ss, rec[i].w / ss, rec[i].h / ss);
      imdraw_rect(im, 0, rc, 0, Color_Red, 1, 0);
      //_snprintf(buf, 256, "%d", i);
      //imdraw_text(dc, buf, -1, &rc, DT_CENTER | DT_VCENTER, clrText);
    }
    imshow(im);
    cvWaitKey(-1);
  }
  return 0;
}