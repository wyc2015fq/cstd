
typedef struct picpos_t {
  int i, x, y, ready;
  double zoom;
  img_t* im;
} picpos_t;

int hog_boost_gentle_pass_8u(const cascade* ca, const uchar* x, double* pscore)
{
  typedef gentle_weak_t weak_type;
  int ispass = 1;
  double score = 0;
#define weak_macro(wk, v)  GEN_VAL((x)[wk->id], wk, v)
#include "ml/boost_pass_impl.inl"
  *pscore = score;
  return ispass;
}

int hog_boost_find_samp(const cascade* ca, const vstr_t* s, picpos_t* pp, uchar* data, double* pscore, int type)
{
  img_t* im = pp->im;
  img_t* im2 = pp->im + 1;
  int ispass = 0;

  if (type) {
    int asdf = 0;
  }

  if (!pp->ready) {
    pp->x = pp->y = 0;

    if (pp->i >= s->n) {
      if (pp->zoom >= 1) {
        return -1;
      }
      else {
        pp->i = 0;
        pp->zoom *= 1.1;
      }
    }

    imread(s->v[pp->i].s, 3, 1, im);
    imresize_1(im, (int)(im->h * pp->zoom), (int)(im->w * pp->zoom), im);
    imsetsize(im2, im->h / sell_size, im->w / sell_size, 10, 1);
    luvhog_feat(im->h, im->w, im->tt.data, im->s, im->c, im2->tt.data, im2->s, 10);
    pp->i++;
    pp->ready = 1;
  }

  for (pp->y; pp->y + 20 <= im2->h; pp->y++) {
    for (; pp->x + 20 <= im2->w; pp->x++) {
      memcpy2d(data, 200, im2->tt.data + pp->y * im2->s + pp->x * im2->c, im2->s, 20, 200);
      ispass = hog_boost_gentle_pass_8u(ca, data, pscore);

      if (ispass) {
        pp->x++;

        if (pp->y + 20 >= im2->h && pp->x + 20 >= im2->w) {
          pp->ready = 0;
        }

        return 1;
      }
    }
  }

  pp->ready = 0;
  return 0;
}

int hog_boost_train()
{
  typedef gentle_weak_t weak_type;
  str_t str[2] = {0};
  vstr_t strlist[2] = {0};
  char* fn[] = {"listpos.txt", "listneg.txt"};
  int i, j = 0;
  img_t im[2] = {0};
  picpos_t pp0[1] = {0};
  picpos_t pp1[1] = {0};
  cascade ca[1] = {0};
  uchar* samp = 0;
  int pos_num = 6000;
  int samp_num = 15000;
  int feat_num = 20 * 20 * 10;
  int sampstep = feat_num;
  pp0->im = pp1->im = im;
  pp0->zoom = 1, pp1->zoom = 0.3;

  for (i = 0; i < countof(fn); ++i) {
    str_load(fn[i], str + i);
    strv_setstr(strlist + i, str[i], '\n', "\r\n\t ");
  }

  samp = MALLOC(uchar, samp_num * feat_num);
  //pos_num=strlist[0].n;
  {
    typedef double worktype;
    int maxstagelen = 100;
    int maxweaklen = 10000;
    double ha = 0.999;
    double fa = 0.5;
    double min_neg_rate = 0.6;
    int flag_unreselect = 0;  // 不重选特征
    int neg_num = samp_num - pos_num;
    int score = 0;
#define print_macro  logprintf
#define get_pos_macro(ispass, score, has_pos, data)  {int ret = hog_boost_find_samp(ca, strlist+0, pp0, data, &score, 0); if (has_pos = (ret>=0)) { ispass = ret; } }
#define get_neg_macro(ispass, score, has_neg, data)  {int ret = hog_boost_find_samp(ca, strlist+1, pp1, data, &score, 1); if (has_neg = (ret>=0)) { ispass = ret; } }
#define get_feat_macro(i, tmp_feat)    {int _j; for (_j=0; _j<samp_num; ++_j) fi[_j] = samp[_j*sampstep+i]; tmp_feat = fi;}
#define printweak(pwi, weak, end)    ("%5.3f %5.3f|%5.3f %5.3f %6d %5.3f" end, pwi->Ha, pwi->Fa, pwi->Error, pwi->Time, weak->id, weak->thd)
#include "ml/boost_train_beg.inl"
#include "ml/weak_train_gentle.inl"
    cascade_save(ca, "cas.dat");
#include "ml/boost_train_end.inl"
  }
  cascade_save(ca, "cas.dat");
  freeims(im, 2);
  FREE(samp);

  for (i = 0; i < countof(fn); ++i) {
    str_free(str + i);
    strv_free(strlist + i);
  }

  cascade_free(ca);
  return 0;
}

int hog_boost_detect(const cascade* ca, const img_t* im, IRECT* rcout, int maxrclen)
{
  img_t im1[2] = {0};
  img_t* im2 = im1 + 1;
  int x, y, ispass = 0, rclen = 0;
  double score = 0, zoom = 1;
  uchar data[4000];

  for (zoom = 1; zoom > 0.1 && rclen < maxrclen; zoom *= 0.9) {
    imresize_1(im, (int)(im->h * zoom), (int)(im->w * zoom), im1);
    imsetsize(im2, im1->h / sell_size, im1->w / sell_size, 10, 1);
    luvhog_feat(im1->h, im1->w, im1->tt.data, im1->s, im1->c, im2->tt.data, im2->s, 10);

    for (y = 0; y + 20 <= im2->h && rclen < maxrclen; ++y) {
      for (x = 0; x + 20 <= im2->w && rclen < maxrclen; ++x) {
        memcpy2d(data, 200, im2->tt.data + y * im2->s + x * im2->c, im2->s, 20, 200);
        ispass = hog_boost_gentle_pass_8u(ca, data, &score);

        if (ispass) {
          rcout[rclen++] = cRECT2((int)(x * sell_size / zoom), (int)(y * sell_size / zoom), (int)(80 / zoom), (int)(80 / zoom));
        }
      }
    }
  }

  freeims(im1, 2);
  return rclen;
}

int test_hog_boost()
{
  str_t str[1] = {0};
  vstr_t strlist[1] = {0};
  int i, j;
  img_t im[1] = {0};
  cascade ca[1];
  IRECT rc[1000];
  cascade_load(ca, "cas1.dat", 0, 0, 0);
  str_load("listtest2.txt", str);
  strv_setstr(strlist, str[0], '\n', "\r\n\t ");

  for (i = 0; i < strlist->n; ++i) {
    if (imread(strlist->v[i].s, 3, 1, im)) {
      int rclen = hog_boost_detect(ca, im, rc, countof(rc));

      for (j = 0; j < rclen; ++j) {
        imdraw_rect(im, rc[j], 0, rgb(1, 0, 0), 1);
      }

      imshow2("test_hog_boost", im);
      cvWaitKey(-1);
    }
  }

  imfree(im);
  str_free(str);
  strv_free(strlist);
  return 0;
}
