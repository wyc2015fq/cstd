
#ifndef _OMKERNEL_INL_
#define _OMKERNEL_INL_

#if 1
static const double delta1[] = {0.8, 1.2, 1.7, 2.8};
static const int disl1[] = {1, 2, 3, 4, 5, 6, 7, 8};
#else
static const double delta1[] = {0.8};
static const int disl1[] = {1};
#endif

enum {makeom_has2 = 1, makeom_has3 = 1};

#include "omkernel_ok.inl"

static int imfilter_rect(const img_t* im, img_t* result, int nv, const int* kv, int nh, const int* kh, int shift)
{
  int* buf = MALLOC(int, im->w);
  filter_rect(im->h, im->w, im->tt.data, im->s, im->c, result->tt.data, result->s, result->c, buf, nv, kv, nh, kh, shift);
  FREE(buf);
  return 0;
}

static int imcopy_replicate_border(const img_t* im, IRECT rc, img_t* im2)
{
  imsetsize(im2, im->h + rc.t + rc.b, im->w + rc.l + rc.r, im->c, im->f);
  copy_replicate_border(im->h, im->w, im->tt.data, im->s, im2->h, im2->w, im2->tt.data, im2->s, im->c, rc.t, rc.l);
  return 0;
}

static int imfilter_guass(const img_t* im, img_t* im2, double delta)
{
  int* buf;
  int ik[100] = {0}, shift = 8;
  int nk = make_guass_kernel_i(delta, ik, shift);
  imsetsize(im2, im->h, im->w, im->c, im->f);
  buf = MALLOC(int, im->w);
  filter_rect_symmetric(im->h, im->w, im->tt.data, im->s, im->c, im2->tt.data, im2->s, im->c, buf, nk, ik, nk, ik, shift);
  FREE(buf);
  //imshow(im2);cvWaitKey(-1);
  return 0;
}

static int makeom_i1(const uchar* A, uchar* ft, double da, int db)
{
  int buf1[IRIS_W] = {0};
  //double buf1f[IRIS_W*10] = {0};
  int ik[100] = {0}, shift = 8;
  //double ikf[100] = {0};
  int nk;
  enum {ftlen = IRIS_H * IRIS_S};
  uchar* A3 = ft;

  if (ft && A) {
    int A2_i4[IRIS_H * IRIS_W];
    nk = make_guass_kernel_i(da, ik, shift);
    filter_rect_symmetric_i4(IRIS_H, IRIS_W, A, IRIS_W, 1, A2_i4, IRIS_W, 1, buf1, nk, ik, nk, ik, shift);
    //filter_binary_i4(IRIS_H, IRIS_W, A2_i4, IRIS_W, 1, A3, IRIS_W, 1, -db, 0, db, 0, 0);
    filter_binary_bit_i4(IRIS_H, IRIS_W, A2_i4, IRIS_W, 1, A3, IRIS_S, 1, -db, 0, db, 0, 0);
    //{img_t im[1]={0}; IMINIT(im, IRIS_H, IRIS_W/8, A3, IRIS_W/8, 1, 1), imshow(im); cvWaitKey(-1);}
  }

  return ftlen;
}

static int makeom1(const uchar* A, uchar* ft)
{
  int buf1[IRIS_W] = {0};
  double buf1f[IRIS_W * 10] = {0};
  int ik[100] = {0}, shift = 8;
  double ikf[100] = {0};
  int i, j, nk;
  double A2[IRIS_H * IRIS_W];
  uchar* A3 = ft;

  for (j = 0; j < countof(delta1); ++j) {
    //nk = make_guass_kernel_i(delta1[j], ik, shift);
    //filter_rect_symmetric_i4(IRIS_H, IRIS_W, A, IRIS_W, 1, A2_i4, IRIS_W, 1, buf1, nk, ik, nk, ik, shift);
    //nk = make_guass_kernel(delta1[j], ikf);
    nk = make_guass_kernel_2d(delta1[j], buf1f, 8);
    filter_f8(IRIS_H, IRIS_W, A, IRIS_W, 1, A2, IRIS_W, 1, nk, nk, buf1f, nk);
    //imwrite("C:\\ttt.bmp", IRIS_H, IRIS_W, A2, IRIS_W, 1);

    //{img_t im[1]={0}; IMINIT(im, IRIS_H, IRIS_W, A2, IRIS_W, 1, 1), imshow(im); cvWaitKey(-1);}
    for (i = 0; i < countof(disl1); ++i) {
      filter_binary_f8(IRIS_H, IRIS_W, A2, IRIS_W, 1, A3, IRIS_W, 1, -disl1[i], 0, disl1[i], 0, 0);
      //{img_t im[1]={0}; IMINIT(im, IRIS_H, IRIS_W, A3, IRIS_W, 1, 1), imshow(im); cvWaitKey(-1);}
      A3 += IRIS_H * IRIS_W;
    }
  }

  return 0;
}

static int makeom_i(const uchar* A, uchar* ft)
{
  int buf1[IRIS_W] = {0};
  double buf1f[IRIS_W * 10] = {0};
  int ik[100] = {0}, shift = 8;
  double ikf[100] = {0};
  int i, j, nk;
  int A2_i4[IRIS_H * IRIS_W];
  uchar* A3 = ft;

  for (j = 0; j < countof(delta1); ++j) {
    nk = make_guass_kernel_i(delta1[j], ik, shift);
    filter_rect_symmetric_i4(IRIS_H, IRIS_W, A, IRIS_W, 1, A2_i4, IRIS_W, 1, buf1, nk, ik, nk, ik, shift);

    //{img_t im[1]={0}; IMINIT(im, IRIS_H, IRIS_W, A2, IRIS_W, 1, 1), imshow(im); cvWaitKey(-1);}
    for (i = 0; i < countof(disl1); ++i) {
      filter_binary_i4(IRIS_H, IRIS_W, A2_i4, IRIS_W, 1, A3, IRIS_W, 1, -disl1[i], 0, disl1[i], 0, 0);
      //{img_t im[1]={0}; IMINIT(im, IRIS_H, IRIS_W, A3, IRIS_W, 1, 1), imshow(im); cvWaitKey(-1);}
      A3 += IRIS_H * IRIS_W;
    }
  }

  return 0;
}

static int makeom(const uchar* A, uchar* ft)
{
  double kk[500] = {0};
  double A2[IRIS_H * IRIS_W];
  int i, nk = 13, nn = IRIS_H * IRIS_W;
  static const double buf1f[] = {
#if 0
    0, 0, 0, 0.000000000000007, 0.000000000000069, 0.000000000000145, 0, -0.000000000000145, -0.000000000000069, -0.000000000000007, -0, 0, 0,
    0, 0.000000000000003, 0.000000000000724, 0.000000000035984, 0.000000000374273, 0.000000000783081, 0, -0.000000000783081, -0.000000000374273, -0.000000000035984, -0.000000000000724, -0.000000000000003, 0,
    0.000000000000003, 0.000000000003454, 0.000000000819066, 0.000000040714244, 0.000000423469082, 0.000000886012825, 0, -0.000000886012825, -0.000000423469082, -0.000000040714244, -0.000000000819066, -0.000000000003454, -0.000000000000003,
    0.000000000000724, 0.000000000819069, 0.000000194252546, 0.000009655934722, 0.000100431431937, 0.000210129949428, 0, -0.000210129949428, -0.000100431431937, -0.000009655934722, -0.000000194252546, -0.000000000819069, -0.000000000000724,
    0.000000000035987, 0.000000040717691, 0.000009656717803, 0.000480017577679, 0.004992665554418, 0.010446018146174, 0, -0.010446018146174, -0.004992665554418, -0.000480017577679, -0.000009656717803, -0.000000040717691, -0.000000000035987,
    0.000000000374997, 0.000000424288081, 0.00010062531021, 0.005001898020058, 0.052024769743212, 0.108850008650629, 0, -0.108850008650629, -0.052024769743212, -0.005001898020058, -0.00010062531021, -0.000000424288081, -0.000000000374997,
    0.000000000819069, 0.00000092673037, 0.00021978588415, 0.010925149711028, 0.113632544255619, 0.237750277151991, 0, -0.237750277151991, -0.113632544255619, -0.010925149711028, -0.00021978588415, -0.00000092673037, -0.000000000819069,
    0.000000000374997, 0.000000424288081, 0.00010062531021, 0.005001898020058, 0.052024769743212, 0.108850008650629, 0, -0.108850008650629, -0.052024769743212, -0.005001898020058, -0.00010062531021, -0.000000424288081, -0.000000000374997,
    0.000000000035987, 0.000000040717691, 0.000009656717803, 0.000480017577679, 0.004992665554418, 0.010446018146174, 0, -0.010446018146174, -0.004992665554418, -0.000480017577679, -0.000009656717803, -0.000000040717691, -0.000000000035987,
    0.000000000000724, 0.000000000819069, 0.000000194252546, 0.000009655934722, 0.000100431431937, 0.000210129949428, 0, -0.000210129949428, -0.000100431431937, -0.000009655934722, -0.000000194252546, -0.000000000819069, -0.000000000000724,
    0.000000000000003, 0.000000000003454, 0.000000000819066, 0.000000040714244, 0.000000423469082, 0.000000886012825, 0, -0.000000886012825, -0.000000423469082, -0.000000040714244, -0.000000000819066, -0.000000000003454, -0.000000000000003,
    0, 0.000000000000003, 0.000000000000724, 0.000000000035984, 0.000000000374273, 0.000000000783081, 0, -0.000000000783081, -0.000000000374273, -0.000000000035984, -0.000000000000724, -0.000000000000003, 0,
    0, 0, 0, 0.000000000000007, 0.000000000000069, 0.000000000000145, 0, -0.000000000000145, -0.000000000000069, -0.000000000000007, -0, 0, 0,
#else
    0.001089742349768, 0.00108701874046, 0.001007446426193, 0.000847186386537, 0.000612892768579, 0.000321849252747, 0, -0.000321849252747, -0.000612892768579, -0.000847186386537, -0.001007446426193, -0.00108701874046, -0.001089742349768,
    0.001307032726535, 0.001303766040147, 0.001208327316586, 0.00101611204971, 0.000735101197599, 0.000386024739188, 0, -0.000386024739188, -0.000735101197599, -0.00101611204971, -0.001208327316586, -0.001303766040147, -0.001307032726535,
    0.001516674037749, 0.001512883391705, 0.001402136788898, 0.001179091184139, 0.00085300764004, 0.000447941117287, 0, -0.000447941117287, -0.00085300764004, -0.001179091184139, -0.001402136788898, -0.001512883391705, -0.001516674037749,
    0.00170271201639, 0.001698456402851, 0.001574125421586, 0.001323720639825, 0.000957639098856, 0.000502886384323, 0, -0.000502886384323, -0.000957639098856, -0.001323720639825, -0.001574125421586, -0.001698456402851, -0.00170271201639,
    0.001849410448425, 0.001844788189307, 0.001709745379012, 0.001437766785294, 0.001040145214339, 0.000546212938292, 0, -0.000546212938292, -0.001040145214339, -0.001437766785294, -0.001709745379012, -0.001844788189307, -0.001849410448425,
    0.001943428514185, 0.001938571274312, 0.001796663322843, 0.001510858214123, 0.001093022844204, 0.000573980643398, 0, -0.000573980643398, -0.001093022844204, -0.001510858214123, -0.001796663322843, -0.001938571274312, -0.001943428514185,
    0.001975818245859, 0.001970880053847, 0.001826607024148, 0.001536038606299, 0.001111239473413, 0.000583546767847, 0, -0.000583546767847, -0.001111239473413, -0.001536038606299, -0.001826607024148, -0.001970880053847, -0.001975818245859,
    0.001943428514185, 0.001938571274312, 0.001796663322843, 0.001510858214123, 0.001093022844204, 0.000573980643398, 0, -0.000573980643398, -0.001093022844204, -0.001510858214123, -0.001796663322843, -0.001938571274312, -0.001943428514185,
    0.001849410448425, 0.001844788189307, 0.001709745379012, 0.001437766785294, 0.001040145214339, 0.000546212938292, 0, -0.000546212938292, -0.001040145214339, -0.001437766785294, -0.001709745379012, -0.001844788189307, -0.001849410448425,
    0.00170271201639, 0.001698456402851, 0.001574125421586, 0.001323720639825, 0.000957639098856, 0.000502886384323, 0, -0.000502886384323, -0.000957639098856, -0.001323720639825, -0.001574125421586, -0.001698456402851, -0.00170271201639,
    0.001516674037749, 0.001512883391705, 0.001402136788898, 0.001179091184139, 0.00085300764004, 0.000447941117287, 0, -0.000447941117287, -0.00085300764004, -0.001179091184139, -0.001402136788898, -0.001512883391705, -0.001516674037749,
    0.001307032726535, 0.001303766040147, 0.001208327316586, 0.00101611204971, 0.000735101197599, 0.000386024739188, 0, -0.000386024739188, -0.000735101197599, -0.00101611204971, -0.001208327316586, -0.001303766040147, -0.001307032726535,
    0.001089742349768, 0.00108701874046, 0.001007446426193, 0.000847186386537, 0.000612892768579, 0.000321849252747, 0, -0.000321849252747, -0.000612892768579, -0.000847186386537, -0.001007446426193, -0.00108701874046, -0.001089742349768,

#endif
  };
  nk = make_dist_guass_kernel_2d(0.8, kk, 8, 1, 0);
  //ASSERT(countof(buf1f)==nk*nk);
  filter_f8(IRIS_H, IRIS_W, A, IRIS_W, 1, A2, IRIS_W, 1, nk, nk, kk, nk);

  for (i = 0; i < nn; ++i) {
    ft[i] = A2[i] > 0;
  }

  //{img_t im[1]={0}; IMINIT(im, IRIS_H, IRIS_W, ft, IRIS_W, 1, 1), imshow(im); cvWaitKey(-1);}
  return 0;
}

// 返回定序图张数
static int makeom_bit(const uchar* A, uchar* ft)
{
  enum { ftlen = countof(delta1) * countof(disl1) * IRIS_OMLEN * (makeom_has2 + makeom_has3) };

  if (ft && A) {
    int buf1[IRIS_W] = {0};
    double buf1f[IRIS_W * 10] = {0};
    int ik[100] = {0}, shift = 8;
    double ikf[100] = {0};
    int i, j, nk;
    int A2[IRIS_H * IRIS_W];
    //uchar A4[IRIS_H * IRIS_W];
    uchar* A3 = ft;

    for (j = 0; j < countof(delta1); ++j) {
      nk = make_guass_kernel_i(delta1[j], ik, shift);
      filter_rect_symmetric_i4(IRIS_H, IRIS_W, A, IRIS_W, 1, A2, IRIS_W, 1, buf1, nk, ik, nk, ik, shift);

      for (i = 0; i < countof(disl1); ++i) {
        if (makeom_has2) {
          filter_binary_bit_i4(IRIS_H, IRIS_W, A2, IRIS_W, 1, A3, IRIS_S, 1, -disl1[i], 0, disl1[i], 0, 0);
          A3 += IRIS_OMLEN;
        }

        if (makeom_has3) {
          filter_binary_bit_i4(IRIS_H, IRIS_W, A2, IRIS_W, 1, A3, IRIS_S, 1, -disl1[i], 0, disl1[i], 0, 1);
          A3 += IRIS_OMLEN;
        }

        //filter_binary_i4(IRIS_H, IRIS_W, A2, IRIS_W, 1, A4, IRIS_W, 1, -disl1[i], 0, disl1[i], 0, 0);
        //{img_t im[1]={0}; IMINIT(im, IRIS_H, IRIS_W, A4, IRIS_W, 1, 1), imshow(im); cvWaitKey(-1);}
        //filter_binary_bit_i4(IRIS_H, IRIS_W, A2, IRIS_W, 1, A3, IRIS_W/8, 1, -disl1[i], 0, disl1[i], 0, 1);
        //A3 += IRIS_OMLEN;
      }
    }
  }

  return ftlen;
}

static int makeom_bit_select(const uchar* A, uchar* ft)
{
  static int index[] = {
#include "txt/byte.txt"
  };
  enum {nn = IRIS_H * IRIS_S * 4 * 8};
  int i;
  static int inited = 1;

  if (!inited) {
    inited = 1;
    srand(10);

    for (i = 0; i < countof(index); ++i) {
      index[i] = rand() % nn;
    }
  }

  if (ft) {
    uchar ft1[nn] = {0};
    makeom_bit(A, ft1);

    for (i = 0; i < countof(index); ++i) {
      ft[i] = ft1[index[i]];
    }
  }

  return countof(index);
}

static int make_block(int w, int h, int x0, int y0, int cx, int cy, int tx, int ty, IRECT* rc, int maxlen)
{
  int y, x, k = 0;

  for (y = y0; y <= h - cy; y += ty) {
    for (x = x0; x <= w - cx; x += tx) {
      ASSERT(k < maxlen);
      //printf("%d %d\n", x, y);
      rc[k++] = iRECT(x, y, x + cx, y + cy);
    }
  }

  return k;
}


// 14:57 2015-4-8
// 66个块特征的 featlen=8448
// "txt/block_select66.txt" (3956[5740 5741] 36914[404 416])(14773[1819 2219] 42709[2357 7155]
// 42个块特征的 featlen 5376
// "txt/block_select42.txt" (3431[4432 4433] 22891[1158 4312])(13332[1819 2219] 24766[2833 4710]
// 20个块特征的 featlen 2560
// "txt/block_select20.txt"  (2481[6502 6503] 11457[560 562])(6983[5585 7207] 11723[1784 5367]


static const IRECT iris_block_rc[] = {
#include "impl/iris_block_rc.txt"
};
int makeom_bit_block_select(buf_t* bf, const uchar* A, uchar* ft)
{
  static const int ttt[] = {
#include "txt/block_select42.txt"
    //#include "txt/block_select20.txt"
    //#include "txt/block_select69.txt"
    //#include "txt/block_select62.txt"
    //#include "txt/block_select66.txt"
    //#include "txt/block_select175.txt"
  };
  
#if 0
  static IRECT iris_block_rc[IRIS_BLOCK_NUM] = {0};
  static int dinited = 0;
  enum { omlen = countof(delta1) * countof(disl1), nblock = countof(iris_block_rc), nttt = countof(ttt), ftlen = nttt * IRIS_BLOCK_H * IRIS_BLOCK_W };

  if (!dinited) {
    int blocknum = make_block(IRIS_S, IRIS_H, 0, 0, IRIS_BLOCK_W, IRIS_BLOCK_H, IRIS_BLOCK_TX, IRIS_BLOCK_TY, iris_block_rc, IRIS_BLOCK_NUM);
    ASSERT(blocknum == IRIS_BLOCK_NUM);
    dinited = 1;
  }
#else
  
  enum { omlen = countof(delta1) * countof(disl1), nblock = countof(iris_block_rc), nttt = countof(ttt), ftlen = nttt * IRIS_BLOCK_H * IRIS_BLOCK_W };
#endif

#if 0

  if (A) {
    img_t im[1] = {0};
    int i, j, t, k = 1, cnt = 0;
    IRECT rr;
    COLOR clr;
    enum {aa = (makeom_has2 + makeom_has3) * 8};
    imsetsize(im, aa * IRIS_H, IRIS_W, 3, 1);

    for (k = 0; k < countof(delta1); ++k) {
      for (t = 0; t < aa; ++t) {
        for (i = 0; i < IRIS_H; ++i) {
          for (j = 0; j < IRIS_W; ++j) {
            uchar* ptr = im->tt.data + j * 3 + (t * IRIS_H + i) * (IRIS_W * 3);
            ptr[0] = ptr[1] = ptr[2] = A[i * IRIS_W + j];
          }
        }

        rr = iRECT(0, t * IRIS_H, IRIS_W, (t + 1) * IRIS_H);
        clr = t & 1 ? rgba(1, 0, 0, 0.8) : rgba(0, 0, 1, 0.8);
        imdraw_rect(im, rr, rgba(1, 1, 1, 0.05 * i), clr, 1);
        imdraw_text(im, rr, t & 1 ? "三个核" : "两个核", -1, zimo_songti_12, clr, 0, TT_VCENTER | TT_CENTER);
      }

      for (i = 0; i < nttt; ++i) {
        j = ttt[i] / nblock;
        t = ttt[i] % nblock;

        if (j / aa == k) {
          char buf[256];
          IRECT rr = iris_block_rc[t];
          int off = (j % aa) * IRIS_H;
          rr.l *= 8, rr.r *= 8;
          rr = iRectOffset(rr, 0, off, 0, off);
          cnt++;
          imdraw_rect(im, rr, rgba(0, 1, 0, 0.3), rgba(1, 0, 0, 0.8), 1);
          _snprintf(buf, 256, "%d", ttt[i]);
          imdraw_text(im, rr, buf, -1, zimo_songti_12, rgb(0, 0, 0), 0, TT_VCENTER | TT_CENTER);

        }
      }

      imshow(im);
      cvWaitKey(-1);
    }

    imfree(im);
  }

#endif

  if (ft && A) {
    int* buf1 = 0;
    int* ik = 0, shift = 8;
    int i, j, nk, k = 0, i0 = 0, t;
    int* A2 = 0;
    //uchar A4[IRIS_H * IRIS_W];
    uchar A3[IRIS_OMLEN] = {0};
    uchar* A4 = ft;
    int tt[2 * 2][2] = {{0, 0}, {0, 0}, {1, 0}, {0, 1}, };
    BFMALLOC(bf, buf1, IRIS_W);
    BFMALLOC(bf, ik, 100);
    BFMALLOC(bf, A2, IRIS_H * IRIS_W);

    for (j = 0; j < countof(delta1); ++j) {
      nk = make_guass_kernel_i(delta1[j], ik, shift);
      filter_rect_symmetric_i4(IRIS_H, IRIS_W, A, IRIS_W, 1, A2, IRIS_W, 1, buf1, nk, ik, nk, ik, shift);

      for (i = 0; i < countof(disl1); ++i) {
        for (t = 0; t < (makeom_has2 + makeom_has3); ++t) {
          filter_binary_bit_i4(IRIS_H, IRIS_W, A2, IRIS_W, 1, A3, IRIS_S, 1, -disl1[i], 0, disl1[i], 0, t > 0 || !makeom_has2);

          for (i0 = 0; i0 < nttt; ++i0) {
            int ti = ttt[i0];

            if ((k * nblock) <= ti && ti < (k * nblock + nblock)) {
              IRECT r = iris_block_rc[ti % nblock];
              uchar* pA = A3 + r.t * IRIS_S + r.l;
              SKIP_COPY(IRIS_BLOCK_H, IRIS_BLOCK_W, pA, IRIS_S, 1, A4, IRIS_BLOCK_W, 1);
              A4 += IRIS_BLOCK_H * IRIS_BLOCK_W;
            }
          }

          ++k;
        }
      }
    }
    BFFREE(bf, A2, IRIS_H * IRIS_W);
    BFFREE(bf, ik, 100);
    BFFREE(bf, buf1, IRIS_W);

    ASSERT(ftlen == (A4 - ft));
  }

  return ftlen;
}

static int himm_block_dis1(const uchar* fa, const uchar* fb, IRECT rc)
{
  enum {iris_bit_step = IRIS_W / 8};
  int x = rc.l, y = rc.t;
  int i, j, ret = 0, off = y * iris_bit_step + x / 8, t = x & 7;
  const uchar* a = fa + off;
  const uchar* b = fa + off;
  const uchar tt0[] = {0xff, 0xfe, 0xfc, 0xf8, 0xf0, 0xe0, 0xc0, 0x80};
  const uchar tt1[] = {0x00, 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f};

  for (i = 0; i < IRIS_BLOCK_H; ++i, a += iris_bit_step, b += iris_bit_step) {
    ret += poptable[tt0[t] & (a[0] ^ b[0])];

    if (t) {
      ret += poptable[tt1[t] & (a[8] ^ b[8])];
    }

    for (j = 1; j < 8; ++j) {
      ret += poptable[a[j] ^ b[j]];
    }
  }

  return ret;
}

static int himm_block_dis(const uchar* fa, const uchar* fb, IRECT rc)
{
  int i, j, ret = 0, off = rc.t * IRIS_S + rc.l;
  const uchar* a = fa + off;
  const uchar* b = fb + off;

  //#define PAD1(I) poptable[a[I]^b[I]]
  //#define PAD2(I) PAD1(I+0)+PAD1(I+1)
  for (i = 0; i < IRIS_BLOCK_H; ++i, a += IRIS_S, b += IRIS_S) {
    for (j = 0; j < IRIS_BLOCK_W; ++j) {
      ret += poptable[a[j] ^ b[j]];
    }
  }

  return ret;
}

static int feat_select_makeom_bit(int h, int w, const uchar* A, int al, const int* id, double* err)
{
  int i, j, k, nn = h * (h - 1) / 2;
  uchar* B = MALLOC(uchar, h);

  for (k = 0; k < w; ++k) {
    int hist[2][9] = {0};
    int cnt[2] = {0};
    double dhist[2][9] = {0};
    double e = 100;

    for (i = 0; i < h; ++i) {
      B[i] = A[i * al + k];
    }

    for (i = 0; i < h; ++i) {
      for (j = i + 1; j < h; ++j) {
        hist[id[i] == id[j]][poptable[B[i] ^ B[j]]]++;
        cnt[id[i] == id[j]]++;
      }
    }

    for (i = 0; i < 9; ++i) {
      dhist[0][i] = hist[0][i] * 1. / cnt[0];
      dhist[1][i] = hist[1][i] * 1. / cnt[1];
    }

    for (i = 0; i < 8; ++i) {
      double ee = dhist[0][i]  + 1 - dhist[1][i];
      e = MIN(e, ee);
    }

    err[k] = e;
    printf("计算错误率 %4d/%4d\r", k, w);
  }

  FREE(B);
  return 0;
}

static int count_xor_select(int n, const uchar* A, const uchar* B, const int* id)
{
  int i = 0, k = 0;

  for (; i < n - 3; i += 4) {
    k +=
        poptable[(A[id[i + 0]] ^ B[id[i + 0]])] +
        poptable[(A[id[i + 1]] ^ B[id[i + 1]])] +
        poptable[(A[id[i + 2]] ^ B[id[i + 2]])] +
        poptable[(A[id[i + 3]] ^ B[id[i + 3]])];
  }

  for (; i < n; ++i) {
    k += poptable[A[id[i]] ^ B[id[i]]];
  }

  return k;
}

static double count_xor_mask(int n, const uchar* A, const uchar* B, const uchar* MA, const uchar* MB)
{
  int i, k = 0, c = 0;

  for (i = 0; i < n; ++i) {
    uchar mab = MA[i] & MB[i];
    k += poptable[mab & (A[i] ^ B[i])];
    c += poptable[mab];
  }

  return k * 1. / c;
}


#endif // _OMKERNEL_INL_
