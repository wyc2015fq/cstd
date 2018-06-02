
#include <math.h>
#include "sort.h"

#define GEN_VAL(x, gen)  ((gen)->val[(x) > (gen)->thd])
double GEN_value(double x, GENWEAK* gen, int i)
{
  return GEN_VAL(x, gen + i);
}

STATIC int GEN_print(FILE* pf, const GENWEAK* gen)
{
#if 1
  fprintf(pf, _DF() _DF() _DF(), gen->thd, gen->val[ 0 ], gen->val[ 1 ]);
#else

  int flag = gen[ 0 ].val[ 0 ] > gen[ 0 ].val[ 1 ] ? 1 : 0;
  double ww = gen[ 0 ].val[ 0 ] + gen[ 0 ].val[ 1 ];

  fprintf(pf, _D(6d) _D(1d), ptr->id[ i ], flag);
#endif

  return 0;
}

STATIC int GEN_scanf(FILE* pf, GENWEAK* gen)
{
  fscanf(pf, _F() _F() _F(), &gen[ 0 ].thd, &gen[ 0 ].val[ 0 ], &gen[ 0 ].val[ 1 ]);
  return 0;
}

// for i=1:n(对每个特征)
//   found=0
//   for j=1:m  eval[j]=Value[i][j]  (存放特征i在所有样本上的特征值)
//   对eval数组按从小到大排序
//   for j=1:m (对于每个排序后的样本) {
//     wl=sum[k=1:j](w[k]), wr=sum[k=j+1:m](w[k])
//     wyl=sum[k=1:j](w[k]*y[k]), wyr=sum[k=j+1:m](w[k]*y[k])
//       (正样本y[k]=1，负样本y[k]=-1)
//     curleft=wyl/wl, curright=wyr/wr
//     curlerror=sum[k=1:j](w[k]*(y[k]-curleft)^2)
//     currerror=sum[k=j+1:m](w[k]*(y[k]-curright)^2)
//     if (curlerror+currerror) < FAULT then
//       FAULT = (curlerror+currerror)
//       分类器阈值zeta=eval[j]
//       a[1]=curlerror, a[2]=curright
//       found=1
//   }
//   if found=1 ghen selection=1 表示特征i被选择

int FindStumpThreshold(int SampNum, int PosNum, const double* wdata, int* index, double* error, double* left, double* right)
{
  double wyl = 0.0F, wl = 0.0F, wyr = 0.0F, wr = 0.0F, wyyl = 0.0F;
  double curleft = 0.0F, curright = 0.0F;
  double* prevval = NULL, *curval = NULL;
  double curlerror = 0.0F, currerror = 0.0F;
  double wposl, wposr;
  int i = 0, idx = 0, found = -1;
  double lerror = FLT_MAX, rerror = FLT_MAX;
  double sumw = 0.0F, sumwy = 0.0F, sumwyy = 0.0F;
  int cnt = 0;
  *error = FLT_MAX;
  wposl = wposr = 0.0F;

  //if( sumw == FLT_MAX )
  {
    /* calculate sums */
    for (i = 0; i < SampNum; i++) {
      if (wdata[ i ] < 0) {
        continue;
      }

      sumw += wdata[ i ];
      sumwy += i < PosNum ? (wdata[ i ]) : (-wdata[ i ]);
      sumwyy += wdata[ i ];
    }
  }

  for (i = 0; i < SampNum; i++) {
    idx = index[ i ];
    cnt += idx < PosNum;

    if (wdata[ idx ] < 0) {
      continue;
    }

    /* for debug purpose */
    wyr = sumwy - wyl;
    wr = sumw - wl;

    if (wl > 0.0) {
      curleft = wyl / wl;
    }
    else {
      curleft = 0.0F;
    }

    if (wr > 0.0) {
      curright = wyr / wr;
    }
    else {
      curright = 0.0F;
    }

    curlerror = wyyl + curleft * curleft * wl - 2.0F * curleft * wyl;
    currerror = (sumwyy) - wyyl + curright * curright * wr - 2.0F * curright * wyr;

    if (curlerror + currerror < *error) {
      (lerror) = curlerror;
      (rerror) = currerror;
      *error = lerror + rerror;

      if (*error < 0.f) {
        int asdf = 0;
      }

      *left = curleft;
      *right = curright;
      found = i;
    }

    wl += wdata[ idx ];
    wyl += idx < PosNum ? (wdata[ idx ]) : (-wdata[ idx ]);
    wyyl += wdata[ idx ];
  } /* for each value */

  return found;
}

/* misclassification error
 * err = MIN( wpos, wneg );
 */
#define ICV_DEF_FIND_STUMP_THRESHOLD_MISC(suffix, type) ICV_DEF_FIND_STUMP_THRESHOLD(misc_##suffix, type, wposl = 0.5F * (wl + wyl); wposr = 0.5F * (wr + wyr); curleft = 0.5F * (1.0F + curleft); curright = 0.5F * (1.0F + curright); curlerror = MIN(wposl, wl - wposl); currerror = MIN(wposr, wr - wposr);)
/* gini error
 * err = 2 * wpos * wneg /(wpos + wneg)
 */
#define ICV_DEF_FIND_STUMP_THRESHOLD_GINI(suffix, type) ICV_DEF_FIND_STUMP_THRESHOLD(gini_##suffix, type, wposl = 0.5F * (wl + wyl); wposr = 0.5F * (wr + wyr); curleft = 0.5F * (1.0F + curleft); curright = 0.5F * (1.0F + curright); curlerror = 2.0F * wposl * (1.0F - curleft); currerror = 2.0F * wposr * (1.0F - curright);)
#define CV_ENTROPY_THRESHOLD                            FLT_MIN

/* entropy error
 * err = - wpos * log(wpos / (wpos + wneg)) - wneg * log(wneg / (wpos + wneg))
 */
#define ICV_DEF_FIND_STUMP_THRESHOLD_ENTROPY(suffix, type) \
  ICV_DEF_FIND_STUMP_THRESHOLD(entropy_##suffix, type, wposl = 0.5F * (wl + wyl); wposr = 0.5F * (wr + wyr); curleft = 0.5F * (1.0F + curleft); curright = 0.5F * (1.0F + curright); curlerror = currerror = 0.0F; if (curleft > CV_ENTROPY_THRESHOLD) \
      curlerror -= wposl * logf(curleft); if (curleft < 1.0F - CV_ENTROPY_THRESHOLD) \
        curlerror -= (wl - wposl) * logf(1.0F - curleft); if (curright > CV_ENTROPY_THRESHOLD) \
          currerror -= wposr * logf(curright); if (curright < 1.0F - CV_ENTROPY_THRESHOLD) \
            currerror -= (wr - wposr) * logf(1.0F - curright);)
/* least sum of squares error */
#define ICV_DEF_FIND_STUMP_THRESHOLD_SQ(suffix, type) ICV_DEF_FIND_STUMP_THRESHOLD(sq_##suffix, type, /* calculate error (sum of squares)          */ /* err = sum( w * (y - left(rigt)Val)^2 )    */ curlerror = wyyl + curleft * curleft * wl - 2.0F * curleft * wyl; currerror = (*sumwyy) - wyyl + curright * curright * wr - 2.0F * curright * wyr;)
double getWeightSum(const double* f, const double* l)
{
  double sum = 0.f;

  for (; f != l; ++f) {
    if (*f > 0.f) {
      // 如果此样本的权值为负，表示此样本已经被丢弃
      sum += *f;
    }
  }

  return sum;
}

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

// pair.first = ha    正样本通过率
// pair.second = fa   负样本通过率
void GEN_weight_update_impl1(int SampNum, const double* f, double* vi, double* wi, int PosNum, double ha, GENWEAK* wk, double b1)
{
  // 更新分类器信息
  double yy, ht, c = (double) log(b1) / 2.f;
  double sampmean[ 2 ] = { 0.f, 0.f };
  int aa, i;

  //FILE* pfsampmean = fopen("sampmean.txt", "a+");
  for (i = 0; i < SampNum; ++i) {
    // 权值为-1表示此样本在前面训练过程中被丢弃了
    if (wi[ i ] <= 0.f) {
      continue;
    }

    ht = GEN_VAL(f[ i ], wk);
    vi[ i ] += ht;

    // ref小于PosNum表示正样本, aa==1表示正样本, aa==0表示负样本
    aa = i < PosNum;
    yy = i < PosNum ? 1.0f : -1.0f;
    wi[ i ] *= (double) exp(-yy * ht);
  }

  //LOGPRINTF(pfsampmean, "posmean=%13.9f negmean=%13.9f\n",
  //          sampmean[1]/SamCnt[1], sampmean[0]/SamCnt[0]);
  //fclose(pfsampmean);
  return ;
}

void GEN_weight_update_impl(int SampNum, const double* f, double* vi, double* wi, int PosNum, double ha, GENWEAK* wk, double beta, double th)
{
  // 更新分类器信息
  double ht;
  int i, yy, kk;

  //FPRINT1D("weight.txt", "%16.16g"ENDL, SampNum, wi, 1);
  for (i = 0; i < SampNum; ++i) {
    // 权值为-1表示此样本在前面训练过程中被丢弃了
    if (wi[ i ] < 0.f) {
      continue;
    }

    ht = GEN_VAL(f[ i ], wk);
    vi[ i ] += ht;

    // ref小于PosNum表示正样本, aa==1表示正样本, aa==0表示负样本
    yy = i < PosNum ? 1 : -1;

    //kk = ht > th ? 1 : -1;
    //kk = vi[i] > th ? 1 : -1;
    kk = ht > 0.f ? 1 : -1;

    if (kk == yy) {
      wi[ i ] *= beta;
    }
  }

  //FPRINT1D("weight_update.txt", "%16.16g"ENDL, SampNum, wi, 1);
  return ;
}

STATIC int GEN_update_weight(BOOST* hh, WORKINFO* pwi)
{
  static const double esp = 0.000001f;
  GENWEAK* wk = (GENWEAK*)hh->ca->weak + hh->ca->weaklen;
  double beta;
  //double b1;
  beta = pwi->Error / (1.f - pwi->Error);
  //b1 = ( wk->val[ 1 ] + esp ) / ( wk->val[ 0 ] + esp );
  //b1 = ( b1 > 1. ) ? b1 : 1.f / b1;
  pwi->Threshold += 0.5f * (pwi->val[ 1 ] + pwi->val[ 0 ]);
  wk->val[ 0 ] = pwi->val[ 0 ];
  wk->val[ 1 ] = pwi->val[ 1 ];
  wk->thd = pwi->val[ 2 ];
  GEN_weight_update_impl(hh->samp_num, hh->fi, hh->vi, hh->wi, hh->pos_num, hh->ha, wk, beta, pwi->Threshold);
  return 0;
}

#define BINNUM 256

STATIC double GEN_geterr_his(int SampNum, int PosNum, const double* f, const double* wi, double* err, double* lerr, double* rerr)
{
  double minval = FLT_MAX, maxval = -FLT_MAX, binval = 0.f;
  int j, MinThreshold = 0, MinSign = 0;
  double his[ BINNUM ];

  for (j = 0; j < BINNUM; ++j) {
    his[ j ] = 0.f;
  }

  for (j = 0; j < SampNum; ++j) {
    if (wi[ j ] > 0.f) {
      if (f[ j ] < minval) {
        minval = f[ j ];
      }

      if (f[ j ] > maxval) {
        maxval = f[ j ];
      }
    }
  }

  binval = (BINNUM) / (maxval - minval);

  for (j = 0; j < SampNum; ++j) {
    int idx = BINNUM - 1;

    if (f[ j ] < maxval) {
      idx = (int)((f[ j ] - minval) * binval);
    }

    his[ idx ] += (j < PosNum) ? (wi[ j ]) : (-wi[ j ]);
  }

  for (j = 0; j < BINNUM - 1; j++) {
    double TempCost;
    int TempSign;

    if (his[ j ] < 0.f) {
      TempCost = 0.5f + his[ j ];
      TempSign = 1;
    }
    else {
      TempCost = 0.5f - his[ j ];
      TempSign = -1;
    }

    if (TempCost < *err) {
      *err = TempCost;
      // the bin with the minimum error
      MinThreshold = j + 1;
      MinSign = TempSign;
    }

    his[ j + 1 ] += his[ j ];
  }

  if (MinSign < 0) {
    *lerr = *err;
    *rerr = 0;
  }
  else {
    *lerr = 0;
    *rerr = *err;
  }

  return MinThreshold * 1.f / binval + minval;
}

STATIC int GEN_get_error(BOOST* hh, WORKINFO* pwi, get_feat_f getfeat)
{
  double e, lval, rval, thd;
  int SampNum = hh->samp_num;
  int PosNum = hh->pos_num;
  int FeatNum = pwi->end - pwi->beg;
  unsigned char* FeatFlag = hh->flag;
  const double* wi = hh->wi;
  int* index = MALLOC(int, SampNum);
  int i;
  double FinalThreshold;
  const double posSum = getWeightSum(wi, wi + PosNum);
  const double negSum = getWeightSum(wi + PosNum, wi + SampNum);
  utime_start(time);

  //static FILE* pf = 0xff;
  pwi->id = -1;
  pwi->Error = 10000.f;

  for (i = pwi->beg; i < pwi->end; ++i) {
    double* tmpSamp;
    DO_PRINT_PRO((i - pwi->beg), FeatNum, (int) utime_elapsed(time));

    if (FeatFlag && FeatFlag[ i ]) {
      continue;
    }

    {
      tmpSamp = getfeat(hh, i);
      e = 10000.f;
#if 1

      {
        int k, j;

        for (j = 0; j < SampNum; ++j) {
          index[ j ] = j;
        }

#define INDEX_LESS(a, b)  ((tmpSamp[a]) < (tmpSamp[b]))

        QSORT(index, index + SampNum, INDEX_LESS, int);
#undef INDEX_LESS
#if 1

        k = GEN_geterr(SampNum, PosNum, tmpSamp, wi, index, &e, &lval, &rval, posSum, negSum);
#else

        k = FindStumpThreshold(SampNum, PosNum, wi, index, &e, &lval, &rval);
#endif

        FinalThreshold = (k > 0) ? 0.5f * (tmpSamp[ index[ k ] ] + tmpSamp[ index[ k - 1 ] ]) : tmpSamp[ index[ k ] ];
      }
#else
      FinalThreshold = GEN_geterr_his(SampNum, PosNum, tmpSamp, wi, &e, &lval, &rval);
#endif

      if (e <= pwi->Error) {
        pwi->id = i;
        pwi->Error = e;
        pwi->val[ 0 ] = lval;
        pwi->val[ 1 ] = rval;

        //thd = (k<SampNum-1) ? 0.5f*(tmpSamp[index[k]]+tmpSamp[index[k+1]]) : tmpSamp[index[k]];
        thd = FinalThreshold;
        pwi->val[ 2 ] = thd;
      }
    }
  }

  pwi->Time = (double) utime_elapsed(time);
  FREE(index);
  {
    double beta, alpha;
    static const double esp = 0.000001;

    if (pwi->Error < esp) {
      pwi->Error = esp;
    }

    beta = (pwi->Error) / (1.f - pwi->Error);
    alpha = (double) log(1.f / beta);

    //printf(FLOATFMT " %+1d ", alpha, pwi->val[0]<pwi->val[1]?-1:1);
    if (pwi->val[ 0 ] < pwi->val[ 1 ]) {
      pwi->val[ 0 ] = 0.;
      pwi->val[ 1 ] = alpha;
    }
    else {
      pwi->val[ 0 ] = alpha;
      pwi->val[ 1 ] = 0.;
    }
  }

  pwi->val[ 2 ] = thd;

  //if (pf) {fclose(pf); pf = NULL;}
  return pwi->id;
}

