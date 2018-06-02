
typedef double haar_inttype;

#include "cv/sumpixels.inl"
#include "cv/convert.inl"
haar_feat_t all_haar_feat[70000];

enum {haar_cx = 20, haar_cy = 20, haar_cx1 = haar_cx + 1, haar_cy1 = haar_cy + 1};

int init_harrfeat()
{
  static int allfeatlen = 0;

  if (!allfeatlen) {
    allfeatlen = make_haar_feat(haar_cy, haar_cx, 2, 2, 1, 1, all_haar_feat, countof(all_haar_feat));
  }

  return allfeatlen;
}

int haar_boost_gentle_pass_64s(const cascade* ca, const int64* data, int step, double invvar, double* pscore)
{
  typedef lut_weak_t weak_type;
  int ispass = 1;
  double score = 0;
  double x;
  init_harrfeat();
#define weak_macro(wk, v)  { int64 t0, t1; RECT_VAL(data, step, all_haar_feat[wk->id].ft0, t0); RECT_VAL(data, step, all_haar_feat[wk->id].ft1, t1); x = invvar*(t0+t1); LUT_VAL(x, wk, v);}
#include "ml/boost_pass_impl.inl"
  *pscore = score;
  return ispass;
}

int haar_boost_find_samp(const cascade* ca, const vstr_t* s, picpos_t* pp, haar_inttype* data, double* pscore, int type)
{
  img_t* im = pp->im;
  img_t* im1 = pp->im + 1;
  img_t* im2 = pp->im + 2;
  int ispass = 0;
  const double invarea = 1. / (haar_cy * haar_cx);

  if (type) {
    int asdf = 0;
  }

  if (!pp->ready) {
    pp->x = pp->y = 0;

    if (pp->i >= s->n) {
      if (pp->zoom >= 100) {
        return -1;
      }
      else {
        pp->i = 0;
        pp->zoom *= 1.1;
      }
    }

    imread(s->v[pp->i].s, 1, 1, im);

    if (type) {
      CvSize size = samsize(im->h, im->w, (int)(haar_cx * pp->zoom), (int)(haar_cy * pp->zoom), -1);
      imresize_1(im, size.h, size.w, im);
    }
    else {
      imresize_1(im, haar_cy, haar_cx, im);
    }

    imsetsize(im1, im->h + 1, im->w + 1, sizeof(int64), 1);
    imsetsize(im2, im->h + 1, im->w + 1, sizeof(int64), 1);
    cvIntegral_8u64s(im->tt.data, im->s, im1->tt.s8, im1->w, im2->tt.s8, im2->w, 0, 0, cvSize(im->w, im->h), 1);
    pp->i++;
    pp->ready = 1;
  }

  for (pp->y; pp->y + 20 <= im2->h; pp->y++) {
    for (; pp->x + 20 <= im2->w; pp->x++) {
      double var = 0, invvar = 0;
      int64* qq1 = im1->tt.s8 + im1->w * pp->y + pp->x;
      int64* qq2 = im2->tt.s8 + im2->w * pp->y + pp->x;
      SKIP_INTEGRAL_VAR(qq1, im1->w * haar_cy, haar_cx, qq2, im1->w * haar_cy, haar_cx, invarea, var);

      if (var < 10.) {
        continue;
      }

      invvar = (var <= 0.) ? 1. : 1. / sqrt(var);
      ispass = haar_boost_gentle_pass_64s(ca, qq1, im1->w, invvar, pscore);

      if (ispass) {
        pp->x++;
        cvScale_64s64f(qq1, im1->w, data, haar_cx1, cvSize(haar_cx1, haar_cy1), invvar, -invvar * qq1[0]);

        if (pp->y + haar_cy >= im2->h && pp->x + haar_cx >= im2->w) {
          pp->ready = 0;
        }

        return 1;
      }
    }
  }

  pp->ready = 0;
  return 0;
}

int get_haarfeat(double* f, int n, haar_inttype* samp, int sampstep, const haar_feat_t* haar)
{
  int i;
  haar_inttype t0, t1;

  for (i = 0; i < n; ++i) {
    haar_inttype* data = samp + i * sampstep;
    RECT_VAL(data, haar_cx1, haar->ft0, t0);
    RECT_VAL(data, haar_cx1, haar->ft1, t1);
    f[i] = (t0 + t1);
  }

  return 0;
}

int haar_boost_train()
{
  typedef lut_weak_t weak_type;
  str_t str[2] = {0};
  vstr_t strlist[2] = {0};
  char* fn[] = {"listpos.txt", "listneg.txt"};
  int i, j = 0;
  img_t im[3] = {0};
  picpos_t pp0[1] = {0};
  picpos_t pp1[1] = {0};
  cascade ca[1] = {0};
  haar_inttype* samp = 0;
  int pos_num = 5000;
  int samp_num = 20000;
  int feat_num = init_harrfeat();
  int sampstep = (haar_cx1) * (haar_cy1);
  pp0->im = pp1->im = im;
  pp0->zoom = 1, pp1->zoom = 10;

  for (i = 0; i < countof(fn); ++i) {
    str_load(fn[i], str + i);
    strv_setstr(strlist + i, str[i], '\n', "\r\n\t ");
  }

  samp = MALLOC(haar_inttype, samp_num * sampstep);
  //pos_num=strlist[0].n;
  {
    typedef double worktype;
    int maxstagelen = 4000;
    int maxweaklen = 10000;
    double ha = 0.999;
    double fa = 0.5;
    double min_neg_rate = 0.6;
    int flag_unreselect = 0;  // 不重选特征
    int neg_num = samp_num - pos_num;
    int score = 0;
#define print_macro  logprintf
#define get_pos_macro(ispass, score, has_pos, data)  {int ret = haar_boost_find_samp(ca, strlist+0, pp0, data, &score, 0); if (has_pos = (ret>=0)) { ispass = ret; } }
#define get_neg_macro(ispass, score, has_neg, data)  {int ret = haar_boost_find_samp(ca, strlist+1, pp1, data, &score, 1); if (has_neg = (ret>=0)) { ispass = ret; } }
#define get_feat_macro(i, tmp_feat)    {get_haarfeat(fi, samp_num, samp, sampstep, all_haar_feat+i); tmp_feat = fi;}
#define printweak(pwi, weak, end)    ("%5.3f %5.3f|%5.3f %5.3f %6d" end, pwi->Ha, pwi->Fa, pwi->Error, pwi->Time, weak->id)
    cascade_load(ca, "cas.dat", maxstagelen, maxweaklen, sizeof(weak_type));
#include "ml/boost_train_beg.inl"
    //#include "ml/weak_train_gentle.inl"
#include "ml/weak_train_lut.inl"
    cascade_save(ca, "cas.dat");
#include "ml/boost_train_end.inl"
  }
  cascade_save(ca, "cas.dat");
  freeims(im, countof(im));
  FREE(samp);

  for (i = 0; i < countof(fn); ++i) {
    str_free(str + i);
    strv_free(strlist + i);
  }

  cascade_free(ca);
  return 0;
}

int haar_boost_detect(const cascade* ca, const img_t* im0, IRECT* rcout, int maxrclen)
{
  img_t im[3] = {0};
  img_t* im1 = im + 1;
  img_t* im2 = im + 2;
  int x, y, ispass = 0, rclen = 0;
  double score = 0, zoom = 1;
  const double invarea = 1. / (haar_cy * haar_cx);
  ASSERT(im0->c == 1);

  for (zoom = 1; zoom > 0.1 && rclen < maxrclen; zoom *= 0.9) {
    imresize_1(im0, (int)(im0->h * zoom), (int)(im0->w * zoom), im);
    imsetsize(im1, im->h + 1, im->w + 1, sizeof(int64), 1);
    imsetsize(im2, im->h + 1, im->w + 1, sizeof(int64), 1);
    cvIntegral_8u64s(im->tt.data, im->s, im1->tt.s8, im1->w, im2->tt.s8, im2->w, 0, 0, cvSize(im->w, im->h), 1);

    for (y = 0; y + haar_cy <= im->h && rclen < maxrclen; ++y) {
      for (x = 0; x + haar_cx <= im->w && rclen < maxrclen; ++x) {
        double var = 0, invvar = 0;
        int64* qq1 = im1->tt.s8 + im1->w * y + x;
        int64* qq2 = im2->tt.s8 + im2->w * y + x;
        SKIP_INTEGRAL_VAR(qq1, im1->w * haar_cy, haar_cx, qq2, im1->w * haar_cy, haar_cx, invarea, var);

        if (var < 10.) {
          continue;
        }

        invvar = (var <= 0.) ? 1. : 1. / sqrt(var);
        ispass = haar_boost_gentle_pass_64s(ca, qq1, im1->w, invvar, &score);

        if (ispass) {
          rcout[rclen++] = cRECT2((int)(x / zoom), (int)(y / zoom), (int)(haar_cx / zoom), (int)(haar_cy / zoom));
        }
      }
    }
  }

  freeims(im, countof(im));
  return rclen;
}

int test_haar_boost()
{
  str_t str[1] = {0};
  vstr_t strlist[1] = {0};
  int i, j, rclen;
  img_t im[2] = {0};
  img_t* gry = im + 1;
  cascade ca[1];
  IRECT rc[10000];
  cascade_load(ca, "cas.dat", 0, 0, 0);
  str_load("listneg.txt", str);
  str_load("listpos.txt", str);
  str_load("listtest.txt", str);
  strv_setstr(strlist, str[0], '\n', "\r\n\t ");

  for (i = 0; i < strlist->n; ++i) {
    if (imread(strlist->v[i].s, 3, 1, im)) {
      //imresize_1(im, haar_cy, haar_cx, im);
      im2gry(im, gry);
      rclen = haar_boost_detect(ca, gry, rc, countof(rc));

      for (j = 0; j < rclen; ++j) {
        imdraw_rect(im, rc[j], 0, rgb(1, 0, 0), 1);
      }

      imshow2("test_haar_boost", im);
      cvWaitKey(-1);
    }
  }

  freeims(im, countof(im));
  str_free(str);
  strv_free(strlist);
  return 0;
}
