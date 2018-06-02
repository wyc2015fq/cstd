#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <float.h>


#define xfloor(x) (int) floor(x)
#if 1
#define CalcLutWeight(yy, ht) exp(-1.0f * (yy) * (ht));
#else
#define CalcLutWeight(yy, ht) (10. * exp(-1.0 * (yy) * (ht)) + 1) / (10. + 1.)
#endif

// LUT adaboost
#undef LUT_VAL
#define LUT_VAL(x, lut)  ( (lut)->val[(int)(((x)-(lut)->minval)*(lut)->binval) & (LUT_BIN-1)] )

//#define I(X)  (int)((X)*(1<<13))
//#define Y(X, _N)  (int)((X)*(1<<(_N)))
double LUT_value(double x, LUTWEAK* lut, int i)
{
  //int id = (int)(((x)-(lut)->minval)*(lut)->binval) & (LUT_BIN-1);
  //int id2 = (int)((I(x)-I(lut->minval)>>13)*Y(lut->binval, 26))>>26 & (LUT_BIN-1);
  return LUT_VAL(x, lut + i);
}

STATIC int LUT_print(FILE* pf, const LUTWEAK* lut)
{
  int j;
  fprintf(pf, _DF() _DF(), lut->minval, lut->binval);

  for (j = 0; j < LUT_BIN; ++j) {
    fprintf(pf, _DF(), lut->val[j]);
  }

  return 0;
}

STATIC int LUT_scanf(FILE* pf, LUTWEAK* lut)
{
  int j;
  fscanf(pf, _F() _F(), &lut[0].minval, &lut[0].binval);

  for (j = 0; j < LUT_BIN; ++j) {
    fscanf(pf, _F(), &lut[0].val[j]);
  }

  return 0;
}

int get_min_map(int n, const double* f, const double* w, int sp, double* poswi, double* negwi, int bin, LUTWEAK* wk)
{
  int i;
  double minval = FLT_MAX, maxval = -FLT_MAX, binval = 0.f;
  int minid = 0;
  FILL(bin, poswi, 0.);
  FILL(bin, negwi, 0.);

  // find min and max value
  for (i = 0; i < n; ++i) {
    if (w[i] > 0.f) {
      if (f[i] < minval) {
        minval = f[i];
        minid = i;
      }

      if (f[i] > maxval) {
        maxval = f[i];
      }
    }
  }

  binval = (bin) / (maxval - minval);

  for (i = 0; i < sp; ++i) {
    if (w[i] > 0.f) {
      int idx = bin - 1;
      idx = (int)((f[i] - minval) * binval);
      idx = MIN(idx, bin - 1);
      idx = MAX(idx, 0);
      poswi[idx] += w[i];
    }
  }

  for (i = sp; i < n; ++i) {
    if (w[i] > 0.f) {
      int idx = bin - 1;
      idx = (int)((f[i] - minval) * binval);
      idx = MIN(idx, bin - 1);
      idx = MAX(idx, 0);
      negwi[idx] += w[i];
    }
  }

  if (wk) {
    //logprintf( "id=%d var=%f min=%f bin=%f \n", minid, hhhh->vvv[minid], minval, binval);
    wk->minval = minval;
    wk->binval = binval;
  }

  return 0;
}

// n  - 样本数目
// f  - 特征值 [n] 维向量
// vi - 分类器在样本上的得分
// wi - 样本权重
void LUT_update_weight_impl(int SampNum, const double* f, double* vi, double* wi, int PosNum, double ha, LUTWEAK* wk)
{
  //self& ww = *this;
  double wsum = 0, wsum2 = 0;
  int i;
  double poswi[LUT_BIN * 2] = { 0 };
  double* negwi = poswi + LUT_BIN;
  double* val = wk->val;
  get_min_map(SampNum, f, wi, PosNum, poswi, negwi, LUT_BIN, wk);

  for (i = 0; i < LUT_BIN; ++i) {
    val[i] = 0.5 * log((poswi[i] + X_ESP) / (negwi[i] + X_ESP));
  }

  // 更新权重
  for (i = 0; i < SampNum; ++i) {
    if (wi[i] > 0.f) {
      double ht, yy;
      ht = LUT_VAL(f[i], wk);       // - thd;
      vi[i] += ht;

      // if bb>1. 表示被当成正样本
      yy = i < PosNum ? 1.0f : -1.0f;
      wi[i] *= CalcLutWeight(yy, ht);
      wsum += wi[i];
    }
  }
}

int LUT_update_weight(BOOST* hh, WORKINFO* pwi)
{
  LUTWEAK* wk = hh->ca->lut + hh->ca->weaklen;
  LUT_update_weight_impl(hh->samp_num, hh->fi, hh->vi, hh->wi, hh->pos_num, hh->ha, wk);
  return 0;
}

int LUT_get_error(BOOST* hh, WORKINFO* pwi)
{
  int FeatNum = pwi->end - pwi->beg;
  //const double * Samp = hh->samp;
  double* wi = hh->wi;
  int i, j, pro, id = -1;
  double Error, MaxError = 1000.f;
  double poswi[LUT_BIN] = { 0.f };
  double negwi[LUT_BIN] = { 0.f };
  //double * negwi = poswi + LUT_BIN;
  //double* perr = MALLOC(double, pwi->end-pwi->beg);
  utime_start(time);
  pwi->id = -1;
  pwi->Error = 1000.f;

  //tmpSamp = MALLOC( double, hh->samp_num );
  for (pro = 0, i = pwi->beg; i < pwi->end; ++i) {
    double* tmpSamp;
    ++pro;
    DO_PRINT_PRO(pro, FeatNum, (int) utime_elapsed(time));

    if (hh->flag && hh->flag[i]) {
      continue;
    }

    tmpSamp = getfeat(hh, i);
    get_min_map(hh->samp_num, tmpSamp, wi, hh->pos_num, poswi, negwi, LUT_BIN, NULL);
    Error = 0.f;

    for (j = 0; j < LUT_BIN; ++j) {
      Error += (2. * sqrt(poswi[j] * negwi[j]));
      //val[j] = 0.5f * ( double ) log( ( poswi[j] + X_ESP ) / ( negwi[j] + X_ESP ) );
    }

    //perr[i-pwi->beg] = e;
    if (Error < MaxError) {
      id = pwi->id = i;
      MaxError = pwi->Error = (double) Error;
    }
  }

  //SAFEFREE( tmpSamp );
  //SAFEFREE( perr );
  pwi->Time = (double) utime_elapsed(time);
  printf("                                                                               \r");

  //SAFEFREE(tmpSamp);
  return pwi->id;
}
