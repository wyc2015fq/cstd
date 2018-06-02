
enum {
  BOOST_DOPASS
};

typedef struct stage_32f {
  int len;  // 包含弱分类器数目
  float thd; // 强分类器阈值
}
stage_32f;

typedef struct _IRECTW {
  int x, y, w, h, d;
}
IRECTW;

typedef struct gentle_weak_t {
  int id;
  double thd;
  double val[2];
} gentle_weak_t;

typedef struct haar_feat_t {
  IRECTW ft0;
  IRECTW ft1;
} haar_feat_t;

enum {LUT_BIN = 32};

typedef struct lut_weak_t {
  int id;
  double minval;
  double binval; // = bin/(maxval-minval);
  double val[ LUT_BIN ];
}
lut_weak_t;

typedef struct cascade {
  int stage_len;     // 强分类器数
  int weak_len;      // 弱分类器数
  int is_nesting;   // 使用nesting结构
  int elemsize;
  stage_32f* stage;
  uchar* weak;
} cascade;

int cascade_setsize(cascade* ca, int maxstagelen, int maxweaklen, int elemsize)
{
  ca->elemsize = elemsize;

  if (maxstagelen > 0) {
    ca->stage = REALLOC(stage_32f, ca->stage, maxstagelen);
  }

  if (maxweaklen > 0) {
    ca->weak = REALLOC(uchar, ca->weak, ca->elemsize * maxweaklen);
  }

  return 0;
}

int cascade_free(cascade* ca)
{
  FREE(ca->stage);
  FREE(ca->weak);
  return 0;
}

int cascade_save(const cascade* ca, const char* fn)
{
  FILE* pf = fopen(fn, "wb");

  if (pf) {
    fwrite(ca, sizeof(cascade), 1, pf);
    fwrite(ca->stage, sizeof(stage_32f)*ca->stage_len, 1, pf);
    fwrite(ca->weak, ca->elemsize * ca->weak_len, 1, pf);
    fclose(pf);
  }

  return 0;
}

int cascade_load(cascade* ca, const char* fn, int maxstagelen, int maxweaklen, int elemsize)
{
  FILE* pf = fopen(fn, "rb");

  if (pf) {
    int i, weaklen = 0;
    fread(ca, sizeof(cascade), 1, pf);

    if (elemsize > 0) {
      ASSERT(elemsize == ca->elemsize);
    }

    maxstagelen = MAX(ca->stage_len, maxstagelen);
    maxweaklen = MAX(ca->weak_len, maxweaklen);
    ca->stage = MALLOC(stage_32f, maxstagelen);
    ca->weak = MALLOC(uchar, maxweaklen * ca->elemsize);
    fread(ca->stage, sizeof(stage_32f)*ca->stage_len, 1, pf);
    fread(ca->weak, ca->elemsize * ca->weak_len, 1, pf);
    fclose(pf);

    for (i = 0; i < ca->stage_len; ++i) {
      weaklen += ca->stage[i].len;
    }

    ca->weak_len = weaklen;
  }
  else {
    cascade_setsize(ca, maxstagelen, maxweaklen, elemsize);
  }

  return 0;
}

typedef struct weak_train_info_t {
  int beg, end;
  double Error, Ha, Fa, Time;
} weak_train_info_t;

int GEN_geterr(int SampNum, int sp, const double* f, const double* wi, const int* idx, double* err, double* lerr, double* rerr, double posSum, double negSum)
{
  // 得到正样本的权重和，负样本的权重和
  const double sumw = posSum + negSum;
  const double sumwy = posSum - negSum;
  double min_error = sumw;   // 用来保存最后选择出来的阈值的错误率
  double cur_lerr, cur_rerr, cur_err, wyl = 0.f, wl = 0.f;
  int k = 0, i;

  for (i = 0; i < SampNum; ++i) {
    if (wi[ idx[ i ] ] > 0.f) {
      // 如果此样本的权值为负，表示此样本已经被丢弃
      if (0 == i || f[ idx[ i ] ] != f[ idx[ i - 1 ] ]) {
        cur_lerr = negSum + wyl;  // 左正＋右负
        cur_rerr = posSum - wyl;  // 左负＋右正
        cur_err = (cur_rerr <= cur_lerr) ? cur_rerr : cur_lerr;   // 在两个中选择错误率小的

        if (cur_err < min_error) {
          // 用此阈值所产生的错误率与目前得到的最小错误率比较。
          min_error = cur_err;
          *lerr = cur_lerr;
          *rerr = cur_rerr;
          k = i;
        }
      }

      wl += wi[ idx[ i ] ];

      // +正样本 -负样本
      wyl += (idx[ i ] < sp) ? (wi[ idx[ i ] ]) : (-wi[ idx[ i ] ]);
    }
  }

  //printf(FLOATFMT "  ", min_error);
  // 如果k=0，代表该样本前面已经没有有效样本
  //threshold = (ff!=k) ? (k[0].first+k[-1].first)/2. : k[0].first;
  //min_error /= sumw;
  *err = min_error;
  return k;
}


int get_min_map(int samp_num, const double* fi, const double* wi, int pos_num,
    double* poswi, double* negwi, int bin, lut_weak_t* weak)
{
  int j;
  double minval = FLT_MAX, maxval = -FLT_MAX, binval = 0.f;
  int minid = 0;
  FILL(bin, poswi, 0.);
  FILL(bin, negwi, 0.);

  // find min and max value
  for (j = 0; j < samp_num; ++j) {
    if (wi[ j ] > 0.f) {
      if (fi[ j ] < minval) {
        minval = fi[ j ];
        minid = j;
      }

      if (fi[ j ] > maxval) {
        maxval = fi[ j ];
      }
    }
  }

  binval = (LUT_BIN) / (maxval - minval);

  for (j = 0; j < pos_num; ++j) {
    if (wi[ j ] > 0.f) {
      int idx = (int)((fi[ j ] - minval) * binval);
      idx = BOUND(idx, 0, bin - 1);
      poswi[ idx ] += wi[ j ];
    }
  }

  for (j = pos_num; j < samp_num; ++j) {
    if (wi[ j ] > 0.f) {
      int idx = (int)((fi[ j ] - minval) * binval);
      idx = BOUND(idx, 0, bin - 1);
      negwi[ idx ] += wi[ j ];
    }
  }

  if (weak) {
    weak->minval = minval;
    weak->binval = binval;
  }

  return 0;
}

enum {forest_weak_level = 2, forest_weak_nodes = (1 << forest_weak_level) - 1};

typedef struct forest_weak_t {
  int id[3];
  double thd[3];
  double val[4];
} forest_weak_t;

#define LUT_VAL(x, wk, v)  ( v = (wk)->val[(int)(((x)-(wk)->minval)*(wk)->binval) & (LUT_BIN-1)] )
#define LUT_VEC(x, wk, v)  LUT_VAL((x)[wk->id], wk, v)
#define GEN_VAL(x, wk, v)  (v = ((wk)->val[(x) > (wk)->thd]))
#define GEN_VEC(x, wk, v)  GEN_VAL((x)[wk->id], wk, v)
#define FOREST_VAL(x0, x1, x2, wk, v)  { int t0=x0 < wk->thd[0]; v = t0 ? wk->val[x1 < wk->thd[1]] : wk->val[2+(x2 < wk->thd[2])]; }
#define FOREST_VEC(x, wk, v)  { int t0=(x)[wk->id[0]] > wk->thd[0]; v = (wk->val[(t0<<1)+((x)[wk->id[1+t0]]>wk->thd[1+t0])]); }

// construct cdf given data vector and wts
int constructCdf2(uint8* data, double* wts, int nBins, int N, int M, uint32* ord, double* cdf)
{
  int i;

  for (i = 0; i < nBins; i++) {
    cdf[i] = 0;
  }

  if (M) {
    for (i = 0; i < M; i++) {
      cdf[data[ord[i]]] += wts[i];
    }
  }
  else {
    for (i = 0; i < N; i++) {
      cdf[data[i]] += wts[i];
    }
  }

  for (i = 1; i < nBins; i++) {
    cdf[i] += cdf[i - 1]; //概率累计函数
  }

  return 0;
}

DRANGE minmax_64f(int n, const double* A)
{
  int i;
  DRANGE ra;
  ra.l = ra.r = A[0];

  for (i = 1; i < n; ++i) {
    if (A[i] > ra.r) {
      ra.r = A[i];
    }
    else if (A[i] < ra.l) {
      ra.l = A[i];
    }
  }

  return ra;
}

int cvt_64f_to_8u(int n, const double* A, uchar* B)
{
  if (n > 0) {
    int i;
    DRANGE ra = minmax_64f(n, A);
    double binval = 255.9999 / (ra.r - ra.l);

    for (i = 0; i < n; ++i) {
      B[i] = (uchar)((A[i] - ra.l) * binval);
    }
  }

  return 0;
}

int constructCdf(const uint8* data, const double* wts, int nBins, int N, double* cdf)
{
  int i;

  for (i = 0; i < nBins; i++) {
    cdf[i] = 0;
  }

  for (i = 0; i < N; i++) {
    cdf[data[i]] += wts[i];
  }

  for (i = 1; i < nBins; i++) {
    cdf[i] += cdf[i - 1]; //概率累计函数
  }

  return 0;
}



#define RECT_VAL(A, AL, ft, V)   { int p0=ft.y*AL+ft.x, ph=ft.h*AL; V=ft.d*RECT_VAL2((A+p0), ph, ft.w); }

#define IRECTW_SET(S, X0, Y0, CX0, CY0, W0) ((S).x = X0,(S).y = Y0,(S).w = CX0,(S).h = CY0,(S).d = W0)
// 正
#define SETRECT2(a, b, x0, y0, cx0, cy0, w0, x1, y1, cx1, cy1, w1)  if (testx[a] && testy[b]) { if (out) {ASSERT(i<maxoutlen);IRECTW_SET((out[i]).ft0, x0, y0, cx0, cy0, w0); IRECTW_SET((out[i]).ft1, x1, y1, cx1, cy1, w1);} ++i;}
#define SETRECT2a(w0, h0, x1, y1, w1, h1)  if (testx[w0] && testy[h0]) { if (out) {IRECTW_SET((out[i]).ft0, x, y, w0*dx, h0*dy, -w1*h1); IRECTW_SET((out[i]).ft1, x+x1*dx, y+y1*dy, w1*dx, h1*dy, w0*h0);} ++i;}
#define SETRECT2b(w0, h0, x1, y1, w1, h1)   {SETRECT2a(w0, h0, x1, y1, w1, h1);SETRECT2a(h0, w0, y1, x1, h1, w1);}
// 斜
#define TILRECT2(a, b, x0, y0, cx0, cy0, w0, x1, y1, cx1, cy1, w1)  if (((x + a * dx) <= w && (x - b * dy) >= 0 && (y + a * dx + b * dy) <= h)) { (f++)->init(al, ai, 1, x0, y0, cx0, cy0, w0, x1, y1, cx1, cy1, w1, 0, 0, 0, 0, 0) }

int make_haar_feat(int h, int w, int mincx, int mincy, int stepa, int stepb, haar_feat_t* out, int maxoutlen)
{
  int i = 0;
  // 在这计算19中Haar特征在特定的长宽下的特征数目
  // 19中特征可以分为12中情况计算
  // 开始计算特征!
  int x, y, dx, dy, len = 0;

  for (y = 0; y < h; y += stepa) {
    for (x = 0; x < w; x += stepa) {
      for (dy = mincy; dy <= h; dy += stepb) {
        for (dx = mincx; dx <= w; dx += stepb) {
          int dx2 = dx * 2, dx3 = dx * 3, dx4 = dx * 4;
          int dy2 = dy * 2, dy3 = dy * 3, dy4 = dy * 4;
          int testx[ 5 ] = { x <= w, x + dx <= w, x + dx2 <= w, x + dx3 <= w, x + dx4 <= w };
          int testy[ 5 ] = { y <= h, y + dy <= h, y + dy2 <= h, y + dy3 <= h, y + dy4 <= h };
          SETRECT2(2, 1, x, y, dx2, dy, -1, x + dx, y, dx, dy, 2);    /* 2×1 */
          SETRECT2(1, 2, x, y, dx, dy2, -1, x, y + dy, dx, dy, 2);    /* 1×2 */
          SETRECT2(3, 1, x, y, dx3, dy, -1, x + dx, y, dx, dy, 3);    /* 3×1 */
          SETRECT2(1, 3, x, y, dx, dy3, -1, x, y + dy, dx, dy, 3);    /* 1×3 */
          SETRECT2(4, 1, x, y, dx4, dy, -1, x + dx, y, dx2, dy, 2);   /* 4×1 */
          SETRECT2(1, 4, x, y, dx, dy4, -1, x, y + dy, dx, dy2, 2);   /* 1×4 */
          SETRECT2(3, 3, x, y, dx3, dy3, -1, x + dx, y + dy, dx, dy, 9);              /* 3×3 */

          // 2×2
          if (0) {
            SETRECT2(2, 2, x, y, dx2, dy2, -1, x, y, dx, dy, 4);
            SETRECT2(2, 2, x, y, dx2, dy2, -1, x + dx, y, dx, dy, 4);
            SETRECT2(2, 2, x, y, dx2, dy2, -1, x, y + dy, dx, dy, 4);
            SETRECT2(2, 2, x, y, dx2, dy2, -1, x + dx, y + dy, dx, dy, 4);
          }

          //SETRECT2(2,2,x,y,dx2,dy2,-1,x,y,dx,dy,2,x+dx,y+dy,dx,dy,2);

          // 3×3
          if (0) {
            SETRECT2(3, 3, x, y, dx3, dy3, -4, x, y, dx2, dy2, 9);
            SETRECT2(3, 3, x, y, dx3, dy3, -4, x + dx, y, dx2, dy2, 9);
            SETRECT2(3, 3, x, y, dx3, dy3, -4, x, y + dy, dx2, dy2, 9);
            SETRECT2(3, 3, x, y, dx3, dy3, -4, x + dx, y + dy, dx2, dy2, 9);
          }

          // 3×3
          if (0) {
#if 0
            SETRECT2(x, y, dx3, dy3, -1);
            SETRECT2(x, y, dx, dy, 3);
            SETRECT2(x + dx, y + dy, dx, dy, 3);
            SETRECT2(x + dx2, y + dy2, dx, dy, 3);
            SETRECT2(x, y, dx3, dy3, -1);
            SETRECT2(x, y + dy2, x + dx, dy3, 3);
            SETRECT2(x + dx, y + dy, dx2, dy2, 3);
            SETRECT2(x + dx2, y, dx3, dy, 3);
            SETRECT2(x, y, dx3, dy3, -1);
            SETRECT2(x, y + dy, dx3, dy2, 3);
            SETRECT2(x + dx, y, dx2, dy3, 3);
            SETRECT2(x + dx, y + dy, dx2, dy2, -9);
#endif
          }
        }
      }

      for (dy = 1; dy <= h; dy += stepa) {
        for (dx = 1; dx <= w; dx += stepa) {
#if 0
          int stepa = dx, stepb = dy;
          int dx2 = dx * 2, dx3 = dx * 3, dx4 = dx * 4;
          int dy2 = dy * 2, dy3 = dy * 3, dy4 = dy * 4;

          //x+stepa*dx-stepb*dy;
          //y+stepa*dx+stepb*dy;
          TILRECT2(2, 1, x, y, dx2, dy, -1, x + dx, y + dx, dx, dy, 2);               // 2×1
          TILRECT2(1, 2, x, y, dx, dy2, -1, x - dy, y + dy, dx, dy, 2);               // 1×2
          TILRECT2(3, 1, x, y, dx3, dy, -1, x + dx, y + dx, dx, dy, 3);               // 3×1
          TILRECT2(1, 3, x, y, dx, dy3, -1, x - dy, y + dy, dx, dy, 3);               // 1×3
          TILRECT2(4, 1, x, y, dx4, dy, -1, x + dx, y + dx, dx2, dy, 2);              // 4×1
          TILRECT2(1, 4, x, y, dx, dy4, -1, x - dy, y + dy, dx, dy2, 2);              // 1×4
          TILRECT2(3, 3, x, y, dx3, dy3, -1, x + dx - dy, y + dx + dy, dx, dy, 9);    // 3×3

          // 2×2
          TILRECT2(2, 2, x, y, dx2, dy2, -1, x, y, dx, dy, 4);
          TILRECT2(2, 2, x, y, dx2, dy2, -1, x + dx, y + dx, dx, dy, 4);
          TILRECT2(2, 2, x, y, dx2, dy2, -1, x - dy, y + dy, dx, dy, 4);
          TILRECT2(2, 2, x, y, dx2, dy2, -1, x + dx - dy, y + dx + dy, dx, dy, 4);

          //TILRECT2(2,2,x,y,dx2,dy2,-1,x,y,dx,dy,2,x+dx-dy,y+dx+dy,dx,dy,2);
          // 3×3
          TILRECT2(3, 3, x, y, dx3, dy3, -4, x, y, dx2, dy2, 9);
          TILRECT2(3, 3, x, y, dx3, dy3, -4, x + dx, y + dx, dx2, dy2, 9);
          TILRECT2(3, 3, x, y, dx3, dy3, -4, x - dy, y + dy, dx2, dy2, 9);
          TILRECT2(3, 3, x, y, dx3, dy3, -4, x + dx - dy, y + dx + dy, dx2, dy2, 9);
#endif
        }
      }
    }
  }

  return i;
}



// [errs,thrs] = mexFunction( data0, data1, wts0, wts1,
//  nBins, prior, fids, nThreads, [ord0], [ord1] )

