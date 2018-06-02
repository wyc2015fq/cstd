

#if 0
typedef gentle_weak_t weak_type;
#define weak_macro  GEN_VEC
#define printweak(pwi, weak)    ("%5.3f %5.3f|%5.3f %5.3f %6d %5.3f\n", pwi->Ha, pwi->Fa, pwi->Error, pwi->Time, weak->id, weak->thd)
#endif
#if 1
typedef lut_weak_t weak_type;
#define weak_macro  LUT_VEC
#define printweak(pwi, weak)    ("%5.3f %5.3f|%5.3f %5.3f %5d %5.3f %5.3f\n", pwi->Ha, pwi->Fa, pwi->Error, pwi->Time, weak->id, weak->minval, weak->binval)
#endif
#if 0
typedef forest_weak_t weak_type;
#define weak_macro  FOREST_VEC
#define printweak(pwi, weak)    ("%5.3f %5.3f|%5.3f %5.3f %3d %3d %3d\n", pwi->Ha, pwi->Fa, pwi->Error, pwi->Time, weak->id[0], weak->id[1], weak->id[2])
#endif
int boost_pass_32s(const cascade* ca, const int* x, double* pscore)
{
  int ispass = 1;
  double score = 0;
#include "ml/boost_pass_impl.inl"
  *pscore = score;
  return ispass;
}

//int gentle_weak_train(gentle_weak_t* weak, const weak_train_info_t* pwi, uchar* feat_flag, int SampNum, const int* samp, int sampstep)

// worktype ha;            // 正样本通过率
// worktype fa;            // 负样本拒识率
// worktype min_neg_rate;  // 最小负样本比例
// int pos_num;            // 正样本数目，小于它的是正样本索引，大于等于的是负样本索引
// int samp_num;           // 总样本数目
int boost_train(cascade* ca, int pos_num, int samp_num, const int* samp, int sampstep)
{
  typedef double worktype;
  int maxstagelen = 10;
  int maxweaklen = 1000;
  double ha = 0.99;
  double fa = 0.5;
  double min_neg_rate = 0.2;
  int feat_num = 2;         // 总特征数目
  int flag_unreselect = 0;  // 不重选特征
  int neg_num = samp_num - pos_num;
  int pos_used = 0;
  int neg_used = 0;
  int score = 0;
#define print_macro  printf
#define get_pos_macro(ispass, score, has_pos, data)  if (has_pos = (pos_used<pos_num)) { ispass = boost_pass_32s(ca, samp+(pos_used++)*sampstep, &score); }
#define get_neg_macro(ispass, score, has_neg, data)  if (has_neg = (neg_used<neg_num)) { ispass = boost_pass_32s(ca, samp+(pos_num+(neg_used++))*sampstep, &score); }
#define get_feat_macro(i, tmp_feat)    {int _j; for (_j=0; _j<samp_num; ++_j) fi[_j] = samp[_j*sampstep+i]; }
#include "ml/boost_train_beg.inl"
  //#include "ml/weak_train_gentle.inl"
  //#include "ml/weak_train_lut.inl"
#include "ml/weak_train_tree.inl"
#include "ml/boost_train_end.inl"
  return 0;
}

int test_boost_train()
{
  enum { dims = 2, cluster_count = 2, class_count = 500, sample_count = cluster_count * class_count};
  COLOR color_tab[cluster_count];
  img_t im[1] = {0};
  CvRNG rng = cvRNG(0xffffffff);
  const char* name = "clusters";

  imsetsize(im, 1000, 1000, 3, 1);

  color_tab[0] = CV_RGB(255, 0, 0);
  color_tab[1] = CV_RGB(0, 255, 0);
  color_tab[2] = CV_RGB(100, 100, 255);
  color_tab[3] = CV_RGB(255, 0, 255);
  color_tab[4] = CV_RGB(255, 255, 0);

  cvNamedWindow(name, 1);

  for (;;) {
    int i, j, k, errcnt = 0;
    cascade ca[1] = {0};
    int points[1000 * dims] = {0};
    int tt[dims] = {0};
    FILL(dims, tt, 1000);
    tt[0] = im->w, tt[1] = im->h;

    /* generate random sample from multigaussian distribution */
    for (k = 0; k < cluster_count; k++) {
      int off[dims];
      int center[dims];

      for (j = 0; j < dims; ++j) {
        off[j] = tt[j] / 5;
        center[j] = tt[j] / 4 + tt[j] * k / cluster_count;
      }

      icvRandn_32s(&rng, points + k * class_count * dims, dims, cvSize(dims, class_count), dims, center, off);
    }

    boost_train(ca, class_count, sample_count, points, dims);
    imzero(im);

    for (i = 0; i < im->h; ++i) {
      for (j = 0; j < im->w; ++j) {
        IPOINT pt = {j, i};
        double score;
        int cluster_idx = boost_pass_32s(ca, &pt.x, &score);
        COLOR clr = RGBBLEN(rgb(1, 1, 1), color_tab[cluster_idx], 20);
        memcpy(im->tt.data + im->s * i + im->c * j, &clr, im->c);
      }
    }

    errcnt = 0;

    for (i = 0; i < sample_count; i++) {
      double score;
      int cluster_idx1 = boost_pass_32s(ca, points + i * dims, &score);
      int cluster_idx = i < class_count;
      //imdraw_circle1(im, pt.x, pt.y, 2, color_tab[cluster_idx], 0);
      imdrawaa_circle(im, points[i * dims + 0], points[i * dims + 1], 2, color_tab[cluster_idx], 0, 0);
      errcnt += cluster_idx1 != cluster_idx;
    }

    printf("err=%f\n", errcnt * 1. / sample_count);

    imshow2(name, im);
    cascade_free(ca);

    if (cvWaitKey(0) == 27) { // 'ESC'
      break;
    }
  }

  return 0;
}
