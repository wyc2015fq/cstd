
#include <math.h>
#include "sort.h"

#define SAMPSTEP (33*33*9)
#define INTSTEP (33*9)

#define CARTW_value 0
#define CARTF_init 0


// h_feat
typedef struct _CARTF_FEAT_T {
  double* samp;
  carrect* allfeat;        // 所有的特征数据
  int* Labels;
}
CARTF_FEAT_T;

#define CARTF_CN  (9)
#define CARTF_setoffset  (0)
#define CARTF_GETRECT(C, A, al, x)  { \
    int p1 = ((C).x*CARTF_CN+(C).y*al+(C).id);\
    int b = ((C).w*CARTF_CN);\
    int c = ((C).h*al);\
    int p2 = p1+b, p3 = p1+c, p4 = p2+c; \
    x = A[p1]+A[p4]-A[p2]-A[p3]; \
  }

typedef CARTFFEAT CARTWWEAK;
#define CARTF_print CARTW_print
#define CARTF_scanf CARTW_scanf
STATIC int CARTW_scanf(FILE* pf, CARTFFEAT* cart)
{
  int i;
  const carnode* Trees = cart[0].nodes;

  for (i = 0; i < CARTMAXSPLIT; ++i) {
    fprintf(pf, "%lf %d %d %lf %lf ", Trees[i].constrain,
        Trees[i].chr[0], Trees[i].chr[1], Trees[i].coeffs[0], Trees[i].coeffs[1]);
  }

  return 0;
}
STATIC int CARTW_print(FILE* pf, const CARTFFEAT* cart)
{
  int i;
  const carnode* Trees = cart[0].nodes;

  for (i = 0; i < CARTMAXSPLIT; ++i) {
    fscanf(pf, "%+3.16lf %4d %4d %+3.16lf %+3.16lf ", Trees[i].constrain,
        Trees[i].chr[0], Trees[i].chr[1], Trees[i].coeffs[0], Trees[i].coeffs[1]);
  }

  return 0;
}

double CARTF_feat_val(double* A, int al, CARTFFEAT* cart, int idim, double iv)
{
  int i = 0, j;
  double x;
  carnode* nodes = cart[idim].nodes;

  for (; 1;) {
    CARTF_GETRECT(nodes[i], A, al, x);
    j = x < nodes[i].constrain;

    if (nodes[i].chr[j]) {
      i = nodes[i].chr[j];
    }
    else {
      //return i<<1|j;
      return nodes[i].coeffs[j];
    }
  }

  assert(0);
  return 0;
}
int CARTF_integral(int h, int w, const uchar* A, int al, double* gradscalebin, int sl, int si)
{
  int i, j, k, oi, x, y;
  double o, gradscale, gradorient;
  double chushu = 2 * PI / CARTF_CN;

  //char asdf[32*32]={0};
  for (i = 0; i < h; ++i) {
    for (j = 0; j < w; ++j) {
      int l, r, u, d;
      //A[(i)*al+(j)]
      l = j > 0 ? A[(i) * al + (j - 1)] : A[(i) * al + (j)];
      r = j < w - 1 ? A[(i) * al + (j + 1)] : A[(i) * al + (j)];
      u = i > 0 ? A[(i - 1) * al + (j)] : A[(i) * al + (j)];
      d = i < h - 1 ? A[(i + 1) * al + (j)] : A[(i) * al + (j)];
      x = r - l;
      y = d - u;
      gradscale = sqrt(x * x + y * y); // 全是正的 梯度的模
      //asdf[(i)*sl+(j)] = (int)gradscale[(i)*sl+(j)];
      // orientation region is 0 to 2*pi.
      o = atan2(y, x);

      if (o < 0) {
        o += 2 * PI;
      }

      ASSERT(o >= 0 && o <= 2 * PI);
      gradorient = o;
      oi = (int)floor(gradorient / chushu);
      ASSERT(oi < CARTF_CN);

      for (k = 0; k < CARTF_CN; ++k) {
        double* p = gradscalebin + (i + 1) * sl + (j + 1) * CARTF_CN + k;
        *p = p[-sl] + p[-CARTF_CN] - p[-sl - CARTF_CN];
      }

      gradscalebin[(i + 1)*sl + (j + 1)*CARTF_CN + oi] += gradscale;
    }
  }

  //FPRINT1D("img.txt", "%4d\r\n", 32*32, A, 1);
  //FPRINT1D("gradx.txt", "%4d\r\n", 32*32, gradx, 1);
  //FPRINT1D("grady.txt", "%4d\r\n", 32*32, grady, 1);
  //FPRINT1D("gradscale.txt", "%16.16lf\r\n", 32*32, gradscale, 1);
  //FPRINT1D("gradorient.txt", "%16.16lf\r\n", 32*32, gradorient, 1);
  //cvShowImage("asdf", 32, 32, asdf, 32, 1);
  //cvWaitKey(-1);
  return 0;
}


#define initnode(n) ((n).constrain=0, (n).coeffs[0]=0, (n).coeffs[1]=0, (n).chr[0]=0, (n).chr[1]=0, (n).w=0)

int cartoutput2(carnode* nodes, double* A, int al)
{
  int i = 0, j;
  double x;

  for (; 1;) {
    CARTF_GETRECT(nodes[i], A, al, x);
    j = x < nodes[i].constrain;

    if (nodes[i].chr[j]) {
      i = nodes[i].chr[j];
    }
    else {
      return i << 1 | j;
    }
  }

  assert(0);
  return 0;
}

int get_cartout(int snum, double* subData, int* labels, double* weight, carnode* nodes, double* pos, double* neg, int* out)
{
  int i = 0, j;

  for (i = 0; i < snum; ++i) {
    j = cartoutput2(nodes, subData + i * SAMPSTEP, INTSTEP);
    out[i] = j;

    if (labels[i] > 0) {
      pos[j] += weight[i];
    }
    else if (labels[i] < 0) {
      neg[j] += weight[i];
    }
  }

  return 0;
}
int get_tj(int snum, double* subData, int* labels, double* weight, int k, carnode* nodes, double* pos, double* neg)
{
  int i = 0, j, m;
  int sampstep = 33 * 33 * 9;
  int intstep = 33 * 9;
  pos[0] = pos[1] = neg[0] = neg[1] = 0;

  for (; i < snum; ++i) {
    j = cartoutput2(nodes, subData + i * sampstep, intstep);

    if (j >> 1 == k) {
      m = (j & 1);

      if (labels[i] > 0) {
        pos[m] += weight[i];
      }
      else if (labels[i] < 0) {
        neg[m] += weight[i];
      }
    }
  }

  return 0;
}
// intstep 积分图步进 == 33*9
// step 积分图样本步进 ==33*33*9
int get_mask(int snum, double* subData, int* labels, double* weight, int k, carnode* nodes, char* mask0, char* mask1)
{
  int i = 0, j;
  int sampstep = 33 * 33 * 9;
  int intstep = 33 * 9;
  memset(mask0, 0, snum);
  memset(mask1, 0, snum);

  for (; i < snum; ++i) {
    j = cartoutput2(nodes, subData + i * sampstep, intstep);

    if (j >> 1 == k) {
      //mask[i] = inv ? !(j&1):(j&1);
      mask0[i] = (j & 1);
      mask1[i] = !(j & 1);
    }
  }

  return 0;
}

static double* CARTF_feat(BOOST* hh, int i)
{
  CARTF_FEAT_T* ft = (CARTF_FEAT_T*)(hh->h_feat);
  double* samp = (ft->samp), x;
  carrect cr = ft->allfeat[i / CARTF_CN];
  int k;
  ASSERT(i < hh->feat_num);
  cr.id = i % CARTF_CN;

  for (k = 0; k < hh->samp_num; ++k, samp += SAMPSTEP) {
    CARTF_GETRECT(cr, samp, INTSTEP, x);
    hh->fi[k] = x;
  }

  return hh->fi;
}

#define GETHOGFEAT  IIVhog

#define MAKE_IRECT(IRECTW, X0, Y0, CX0, CY0) \
  do \
  { \
    (IRECTW).x = X0; \
    (IRECTW).y = Y0; \
    (IRECTW).w = CX0; \
    (IRECTW).h = CY0; \
  } while (0)

#define SETRECT4(a, b, x0, y0, cx0, cy0) \
  if (testx[a] && testy[b]) {\
    if (out) {MAKE_IRECT((*f), x0, y0, cx0, cy0);}; ++(f);\
    if (out) {MAKE_IRECT((*f), x0+cx0, y0, cx0, cy0);}; ++(f);\
    if (out) {MAKE_IRECT((*f), x0, y0+cy0, cx0, cy0);}; ++(f);\
    if (out) {MAKE_IRECT((*f), x0+cx0, y0+cy0, cx0, cy0);}; ++(f);\
  }

int get_hog_feat(int h, int w, int mincx, int mincy, int a, int b, carrect* out)
{
  carrect* f = out;
  // 在这计算19中Haar特征在特定的长宽下的特征数目
  // 19中特征可以分为12中情况计算
  // 开始计算特征!
  int x, y, dx, dy, len = 0;

  for (y = 0; y < h; y += a) {
    for (x = 0; x < w; x += a) {
      for (dy = mincy; dy <= h; dy += b) {
        for (dx = mincx; dx <= w; dx += b) {
          int dx2 = dx * 2, dx3 = dx * 3, dx4 = dx * 4;
          int dy2 = dy * 2, dy3 = dy * 3, dy4 = dy * 4;
          int testx[ 5 ] = { x <= w, x + dx <= w, x + dx2 <= w, x + dx3 <= w, x + dx4 <= w };
          int testy[ 5 ] = { y <= h, y + dy <= h, y + dy2 <= h, y + dy3 <= h, y + dy4 <= h };

          if (dx == dy || dx == 2 * dy || 2 * dx == dy) {
            SETRECT4(2, 2, x, y, dx, dy);
          }
        }
      }
    }
  }

  return f - out;
}

// CART 样本
// 每张图片占空间 32*32 字节
// 每张积分图占空间 33*33*9*sizeof(double) 字节
static int CARTF_find_samp(BOOST* hh, int type)
{
  CARTF_FEAT_T* fs = (CARTF_FEAT_T*)(hh->h_feat);
  // 由特征计算样本
  int ispass;
  int nFeatNum = hh->feat_num;
  int nSampNum = GetSampNum(hh);
  int step = nSampNum;
  int offset = (type > 0 ? 0 : GetPosNum(hh));
  double* samp = (fs->samp) + offset * SAMPSTEP;
  int Num = (type > 0) ? (GetPosNum(hh)) : (GetSampNum(hh) - GetPosNum(hh));
  int maxsamp = Num;
  double* wi = hh->wi + offset;
  double* vi = hh->vi + offset;
  int* Labels = fs->Labels + offset;
  int id = type;
  double* tmpFeat = hh->fi;
  double score;
  //double F[SAMPSTEP];
  FILE* plisttxt;
  int cxy;
  int i = 0, j = 0, k = 0, picnum = 0, pics = 0;
  char buf[256];
  img_t im[1] = {0};
  char* fn;
  fn = inigetstr(type > 0 ? "正样本文件" : "负样本文件");
  cxy = inigetint("样本检测步进");
  cxy = MAX(cxy, 1);
  plisttxt = fopen(fn, "rb");

  for (k = 0; fgets(buf, 256, plisttxt) > 0 && k < maxsamp;) {
    int ok = 0;
    strtrim(buf, "\n\r");

    if (imread(buf, 1, 1, im)) {
      ++pics;

      for (i = 0; i < im->h - 31 && k < maxsamp; i += cxy) {
        for (j = 0; j < im->w - 31 && k < maxsamp; j += cxy) {
          unsigned char* img = im->tt.data + i * im->s + j;
          double* F = samp + k * SAMPSTEP;
          CARTF_integral(32, 32, img, im->s, F, INTSTEP, CARTF_CN);
          cascade_pass(hh->ca, F, INTSTEP, 0, &ispass, &score);

          if (ispass) {
            wi[ k ] = 1.f;
            vi[ k ] = score;
            Labels[ k ] = type;
            ++k;
          }
          else {
            wi[ k ] = -1.f;
            Labels[ k ] = 0;
          }
        }
      }

      printf("%d %s\r", k, strrchr(buf, '\\') + 1);
    }
  }

  imfree(im);
  logprintf("找了%d张图片, 找到%d个样本!...\n", pics, k);
  //_snprintf(buf, 256, "%s_%dx%d.dat", fn, fram, *sdim);
  //rename(fn, buf);
  //*snum = fram;

  return 0;
}

int CARTF_save_feat(BOOST* hh, int id)
{
  CARTF_FEAT_T* ft = (CARTF_FEAT_T*)(hh->h_feat);
  return 0;
}
int CARTF_uninit(BOOST* hh)
{
  CARTF_FEAT_T* ft = (CARTF_FEAT_T*)(hh->h_feat);
  FREE(ft->samp);
  FREE(ft->allfeat);
  FREE(ft->Labels);
  FREE(hh->h_feat);
  return 0;
}

int CARTF_samp_data(BOOST* hh, char** buf, int* size)
{
  CARTF_FEAT_T* fs = (CARTF_FEAT_T*)(hh->h_feat);
  *buf = (char*)(fs->allfeat);
  *size = GetFeatNum(hh) * sizeof(carrect);
  return 0;
}

int CARTF_feat_init(BOOST* hh)
{
  CARTF_FEAT_T* ft;
  int stp = 2;
  int mincx = 4, mincy = 4;
  hh->feat_num = get_hog_feat(32, 32, mincx, mincy, stp, stp, 0);
  ft = MALLOC(CARTF_FEAT_T, 1);
  memset(ft, 0, sizeof(CARTF_FEAT_T));
  ft->Labels = MALLOC(int, hh->samp_num);
  FILL(hh->samp_num, ft->Labels, 0);
  ft->allfeat = MALLOC(carrect, hh->feat_num);
  memset(ft->allfeat, 0, sizeof(carrect)*hh->feat_num);
  ASSERT(hh->feat_num == get_hog_feat(32, 32, mincx, mincy, stp, stp, ft->allfeat));
  ft->samp = MALLOC(double, hh->samp_num * SAMPSTEP);
  FILL(hh->samp_num * SAMPSTEP, ft->samp, 0);
  hh->feat_num *= CARTF_CN;
  hh->h_feat = ft;
  return 1;
}


