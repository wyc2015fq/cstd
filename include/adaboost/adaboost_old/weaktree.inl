
#include <math.h>
#include "sort.h"

//#define TREE_VAL(x, tr)  ((tr)->val[(x) > (tr)->thd])
double TREE_value(double x, TREEWEAK* tr, int i)
{
  double y=0;
  tr+=i;
  if (2==TREEWEAK_LV) {
    if (x<tr->thd[0]) {
      y = (tr)->val[(x) > (tr)->thd[1]];
    } else {
      y = ((tr)->val+2)[(x) > (tr)->thd[2]];
    }
  } else if (1==TREEWEAK_LV) {
    y =  ((tr)->val[(x) > (tr)->thd[0]]);
  }
  return y;
}

STATIC int TREE_print(FILE* pf, const TREEWEAK* tr)
{
#if 1
  fprintf(pf, _DF() _DF() _DF(), tr->thd, tr->val[ 0 ], tr->val[ 1 ]);
#else

  int flag = tr[ 0 ].val[ 0 ] > tr[ 0 ].val[ 1 ] ? 1 : 0;
  double ww = tr[ 0 ].val[ 0 ] + tr[ 0 ].val[ 1 ];

  fprintf(pf, _D(6d) _D(1d), ptr->id[ i ], flag);
#endif

  return 0;
}

STATIC int TREE_scanf(FILE* pf, TREEWEAK* tr)
{
  fscanf(pf, _F() _F() _F(), &tr[ 0 ].thd, &tr[ 0 ].val[ 0 ], &tr[ 0 ].val[ 1 ]);
  return 0;
}

// pair.first = ha    正样本通过率
// pair.second = fa   负样本通过率
void TREE_weight_update_impl1(int SampNum, const double* f, double* vi, double* wi, int PosNum, double ha, TREEWEAK* wk, double b1)
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

    ht = TREE_value(f[ i ], wk, 0);
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

void TREE_weight_update_impl(int SampNum, const double* f, double* vi, double* wi, int PosNum, double ha, TREEWEAK* wk, double beta, double th)
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

    ht = TREE_value(f[ i ], wk, 0);
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

STATIC int TREE_update_weight(BOOST* hh, WORKINFO* pwi)
{
  static const double esp = 0.000001f;
  TREEWEAK* wk = hh->ca->tree + hh->ca->weaklen;
  double beta;
  //double b1;
  beta = pwi->Error / (1.f - pwi->Error);
  //b1 = ( wk->val[ 1 ] + esp ) / ( wk->val[ 0 ] + esp );
  //b1 = ( b1 > 1. ) ? b1 : 1.f / b1;
  pwi->Threshold += 0.5f * (pwi->val[ 1 ] + pwi->val[ 0 ]);
  wk->val[ 0 ] = pwi->val[ 0 ];
  wk->val[ 1 ] = pwi->val[ 1 ];
  wk->thd[0] = pwi->val[ 2 ];
  TREE_weight_update_impl(hh->samp_num, hh->fi, hh->vi, hh->wi, hh->pos_num, hh->ha, wk, beta, pwi->Threshold);
  return 0;
}

int TREE_geterr(int num, const char* label, const double* f, const double* wi, const int* idx, double* err, double* lerr, double* rerr, const double* wiSum, double* FinalThreshold)
{
  // 得到正样本的权重和，负样本的权重和
  const double sumw = wiSum[0] + wiSum[1];
  const double sumwy = wiSum[0] - wiSum[1];
  double min_error = sumw;   // 用来保存最后选择出来的阈值的错误率
  double cur_lerr, cur_rerr, cur_err, wyl = 0.f, wl = 0.f;
  int k = 0, i;

  for (i = 0; i < num; ++i) {
    // 如果此样本的权值为负，表示此样本已经被丢弃
    if (0 == i || (label[ idx[ i ] ] != label[ idx[ i - 1 ] ] && f[ idx[ i ] ] != f[ idx[ i - 1 ] ])) {
      cur_lerr = wiSum[1] + wyl;  // 左正＋右负
      cur_rerr = wiSum[0] - wyl;  // 左负＋右正
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
    wyl += (label[idx[ i ]]) ? (-wi[ idx[ i ] ]) : (wi[ idx[ i ] ]);
  }

  //printf(FLOATFMT "  ", min_error);
  // 如果k=0，代表该样本前面已经没有有效样本
  //threshold = (ff!=k) ? (k[0].first+k[-1].first)/2. : k[0].first;
  //min_error /= sumw;
  *err = min_error;
  *FinalThreshold = (k > 0) ? 0.5f * (f[ idx[ k ] ] + f[ idx[ k - 1 ] ]) : f[ idx[ k ] ];
  return k;
}

STATIC int TREE_get_error(BOOST* hh, WORKINFO* pwi)
{
  double e, lval, rval, thd;
  int SampNum = hh->samp_num;
  int PosNum = hh->pos_num;
  int FeatNum = pwi->end - pwi->beg;
  unsigned char* FeatFlag = hh->flag;
  const double* wi = hh->wi;
  int* index = MALLOC(int, SampNum);
  char* label = MALLOC(char, SampNum);
  int i, j, k, num = 0;
  double FinalThreshold;
  double wiSum[2] = {0};
  utime_start(time);

  //static FILE* pf = 0xff;
  pwi->id = -1;
  pwi->Error = 10000.f;

  for (j = 0; j < SampNum; ++j) {
    label[ j ] = -1;

    if (wi[j] > 0) {
      label[ j ] = !(j < PosNum);
      wiSum[label[ j ]] += wi[j];
    }
  }

  for (i = pwi->beg; i < pwi->end; ++i) {
    double* tmpSamp;
    DO_PRINT_PRO((i - pwi->beg), FeatNum, (int) utime_elapsed(time));

    if (FeatFlag && FeatFlag[ i ]) {
      continue;
    }

    {
      tmpSamp = getfeat(hh, i);
      e = 10000.f;
      k = 0;

      for (j = 0; j < SampNum; ++j) {
        if (label[ j ] >= 0) {
          index[ k++ ] = j;
        }
      }

      num = k;
#define INDEX_LESS(a, b)  ((tmpSamp[a]) < (tmpSamp[b]))

      QSORT(index, index + num, INDEX_LESS, int);
#undef INDEX_LESS
      k = TREE_geterr(num, label, tmpSamp, wi, index, &e, &lval, &rval, wiSum, &FinalThreshold);


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
  FREE(label);
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

