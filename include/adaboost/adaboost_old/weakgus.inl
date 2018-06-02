
// 带宽

#define GUS_DELTA   (0.0000015)
#define SQRT2PI  2.506628274631000200

double GUS_VAL_IMPL(double x, const double* val)
{
  int j;
  double po = 0, ne = 0;
  double score;

  for (j = 0; j < GUS_BIN_POS; ++j) {
    double ss = x - val[j];
    po += exp(-ss * ss / (2 * GUS_DELTA * GUS_DELTA));
  }

  for (j = GUS_BIN_POS; j < GUS_BIN; ++j) {
    double ss = x - val[j];
    ne += exp(-ss * ss / (2 * GUS_DELTA * GUS_DELTA));
  }

  //po /= GUS_DELTA*SQRT2PI*GUS_BIN_POS;
  //ne /= GUS_DELTA*SQRT2PI*GUS_BIN_NEG;
  score = 0.5 * log((po + EPS) / (ne + EPS));
  return score;
}
#define GUS_VAL(x, wk)  GUS_VAL_IMPL(x, (wk)->val)

// 计算AUC曲线面积
// 错误拒绝率 FRR 把正样本错认为负样本的权重和
// 错误接受率 FAR 把负样本错认为正样本的权重和
double get_AUC_area(const double* A, const double* wi, double posSum, double negSum)
{
  double area = 0;
  double area2 = 0;
  double errpos = 0, errneg = negSum, preerrneg = negSum;
  int id[GUS_BIN];
  double sco[GUS_BIN];
  int i, n = GUS_BIN;

  for (i = 0; i < n; ++i) {
    sco[i] = GUS_VAL_IMPL(A[i], A);
  }

  for (i = 0; i < n; ++i) {
    id[i] = i;
  }

#define INDEX_LESS(a, b)  ((sco[a]) < (sco[b]))
  QSORT(id, id + n, INDEX_LESS, int);
#undef INDEX_LESS

  // 正样本全对，负样本全错
  errpos = 0, errneg = negSum;

  for (i = 0; i < n; ++i) {
    if (id[i] < GUS_BIN_POS) {
      errpos += wi[id[i]]; //是正样本就错了
      //area += errneg*wi[id[i]];
      area += 0.5 * (preerrneg + errneg) * wi[id[i]];
    }
    else {
      preerrneg = errneg;
      errneg -= wi[id[i]]; //是负样本就对了
      //area2 += errpos*wi[id[i]];
    }
  }

  return area;
}

double GUS_value(double x, GUSWEAK* gus, int i)
{
  return GUS_VAL(x, gus + i);
}

STATIC int GUS_print(FILE* pf, const GUSWEAK* gus)
{
  int j;

  for (j = 0; j < GUS_BIN; ++j) {
    fprintf(pf, _DF(), gus->val[ j ]);
  }

  return 0;
}
STATIC int GUS_scanf(FILE* pf, GUSWEAK* gus)
{
  int j;

  for (j = 0; j < GUS_BIN; ++j) {
    fscanf(pf, _F(), &gus[ 0 ].val[ j ]);
  }

  return 0;
}

// n - 样本数目
// f - 特征值 [n]维向量
// vi - 分类器在样本上的得分
// wi - 样本权重
void GUS_update_weight_impl(int SampNum, const double* f, double* vi, double* wi, int PosNum, double ha, GUSWEAK* wk)
{
  //self& ww = *this;
  double wsum = 0, wsum2 = 0;
  int i;
  double* val = wk->val;

  COPY(SampNum, f, val);

  // 更新权重
  for (i = 0; i < SampNum; ++i) {
    if (wi[ i ] > 0.f) {
      double ht, yy;
      ht = GUS_VAL(f[ i ], wk);       // - thd;
      vi[ i ] += ht;

      // if bb>1. 表示被当成正样本
      yy = i < PosNum ? 1.0f : -1.0f;
      wi[ i ] *= CalcLutWeight(yy, ht);
      wsum += wi[ i ];
    }
  }
}

int GUS_update_weight(BOOST* hh, WORKINFO* pwi)
{
  GUSWEAK* wk = hh->ca->gus + hh->ca->weaklen;
  GUS_update_weight_impl(hh->samp_num, hh->fi, hh->vi, hh->wi, hh->pos_num, hh->ha, wk);
  return 0;
}

int GUS_get_error(BOOST* hh, WORKINFO* pwi)
{
  int SampNum = hh->samp_num;
  int PosNum = hh->pos_num;
  int FeatNum = pwi->end - pwi->beg;
  unsigned char* FeatFlag = hh->flag;
  //const double * Samp = hh->samp;
  double* wi = hh->wi;
  int i, j = 0, pro, id = -1;
  double Error, MaxError = 1000.f;
  const double posSum = getWeightSum(wi, wi + PosNum);
  const double negSum = getWeightSum(wi + PosNum, wi + SampNum);
  utime_start(time);
  ASSERT(GUS_BIN_POS == PosNum);
  ASSERT(GUS_BIN == SampNum);
  //double * negwi = poswi + GUS_BIN;
  //double* perr = MALLOC(double, pwi->end-pwi->beg);

  //tmpSamp = MALLOC( double, SampNum );
  for (pro = 0, i = pwi->beg; i < pwi->end; ++i) {
    double* tmpSamp;
    ++pro;
    DO_PRINT_PRO(pro, FeatNum, (int) utime_elapsed(time));

    if (FeatFlag && FeatFlag[ i ]) {
      continue;
    }

    tmpSamp = getfeat(hh, i);
    Error = get_AUC_area(tmpSamp, wi, PosNum, negSum);

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
