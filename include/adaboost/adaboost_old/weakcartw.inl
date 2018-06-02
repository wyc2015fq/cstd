
#include "featcartf.inl"
int CARTW_GEN_geterr(int snum, int* labels, const double* f, const double* wi, const int* idx,
    double* err, double* lerr, double* rerr, double posSum, double negSum)
{
  // 得到正样本的权重和，负样本的权重和
  const double sumw = posSum + negSum;
  const double sumwy = posSum - negSum;
  double min_error = sumw;   // 用来保存最后选择出来的阈值的错误率
  double cur_lerr, cur_rerr, cur_err, wyl = 0.f, wl = 0.f;
  int k = 0, i;

  for (i = 0; i < snum; ++i) {
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

      // +1正样本 -1负样本
      wyl += (labels[idx[ i ]] > 0) ? (wi[ idx[ i ] ]) : (-wi[ idx[ i ] ]);
    }
  }

  //printf(FLOATFMT "  ", min_error);
  // 如果k=0，代表该样本前面已经没有有效样本
  //threshold = (ff!=k) ? (k[0].first+k[-1].first)/2. : k[0].first;
  //min_error /= sumw;
  *err = min_error;
  return k;
}

//#undef DO_PRINT_PRO
#define DO_PRINT_PRO1(_PRO, _ALL, _TIME) { if ((_ALL) < 200 || !((_PRO) % ((_ALL) / MIN(_ALL, 10000))) || ((_ALL) - (_PRO)) < 3) \
    { printf("%6d %3.8lf %3.8lf %3.8lf [%4.1lf%%]\r", (int)(*dim), (double)(*spliterror), (double)(*thd), (double)(_TIME), ((_PRO + 1) * 100.f / (_ALL)) ); } }


int get_carttreenum(carnode* nodes)
{
  int n = 1, l = 0, r = 0;

  if (nodes[n].chr[0]) {
    l = get_carttreenum(nodes + nodes[n].chr[0]);
  }

  if (nodes[n].chr[1]) {
    r = get_carttreenum(nodes + nodes[n].chr[1]);
  }

  n = MAX(n, l);
  n = MAX(n, r);
  return n;
}

int onelearn(BOOST* hh, WORKINFO* pwi, int snum, double* subData, char* mask, int* labels)
{
  double e, lval, rval;
  const double* wi = hh->wi;
  int* index = MALLOC(int, snum);
  double* tmpSamp;
  int FeatNum = pwi->end - pwi->beg;
  int i, j, k, knum;
  double FinalThreshold;
  double posSum = 0;
  double negSum = 0;
  utime_start(time);

  for (i = 0; i < snum; ++i) {
    if (mask == NULL || mask[i]) {
      if (labels[i] > 0) {
        posSum += wi[i];
      }
      else if (labels[i] < 0) {
        negSum += wi[i];
      }
    }
  }

  pwi->id = -1;
  pwi->Threshold = 0;
  pwi->Error = 10000;

  for (i = pwi->beg; i < pwi->end; ++i) {
    DO_PRINT_PRO((i - pwi->beg), FeatNum, (int) utime_elapsed(time));
    //tmpSamp = getfeat( hh, i );
    e = 10000.f;
    knum = 0;
    tmpSamp = getfeat(hh, i);

    for (j = 0; j < snum; ++j) {
      if (mask == NULL || mask[j]) {
        index[ knum ] = j;
        ++knum;
      }
    }

    if (knum == 0) {
      break;
    }

    ASSERT(knum > 0);
#define INDEX_LESS(a, b)  ((tmpSamp[a]) < (tmpSamp[b]))

    QSORT(index, index + knum, INDEX_LESS, int);
#undef INDEX_LESS

    k = CARTW_GEN_geterr(knum, labels, tmpSamp, wi, index, &e, &lval, &rval, posSum, negSum);

    FinalThreshold = (k > 0) ? 0.5f * (tmpSamp[ index[ k ] ] + tmpSamp[ index[ k - 1 ] ]) : tmpSamp[ index[ k ] ];

    if (e < pwi->Error) {
      pwi->id = i;
      pwi->Error = e;
      pwi->Threshold = FinalThreshold;
    }
  }

  FREE(index);
  //logprintf("%6d %3.8lf %3.8lf %5.1lf\r\n", (pwi->id), (pwi->Error), (pwi->Threshold), utime_elapsed( time ) );
  return 0;
}


#define mod(a, b)   ((a)%(b))

int IIVhog(int m, int n, uchar* img, int al, double* allfeature)
{
  //得到方向图和模值图
  //是否需要加标准化gamma空间操作？
  double bin[33 * 33 * BINNUM];
  int bl = 33 * BINNUM;
  carrect* fzb = 0;
  int i, j, num = 0;
  int stp = 2;
  int mincx = 4, mincy = 4;
  ASSERT(m == 32 && n == 32);
  num = get_hog_feat(m, n, mincx, mincy, stp, stp, 0);

  if (allfeature) {
    double* feature;
    fzb = MALLOC(carrect, num);
    FILL(33 * 33 * BINNUM, bin, 0.);
    CARTF_integral(m, n, img, al, bin, bl, 9);
    num = get_hog_feat(m, n, mincx, mincy, stp, stp, fzb);
    //计算特征
    logtime(0);

    for (i = 0; i < num; ++i) {
      int p1, p2, p3, p4, a, b, c;
      //logprintf("%4d: %2d,%2d,%2d,%2d,\n", i, fzb[i].x, fzb[i].y, fzb[i].w, fzb[i].h);
      a = fzb[i].y * bl + fzb[i].x * BINNUM;
      b = fzb[i].w * BINNUM;
      c = fzb[i].h * bl;
      p1 = a;
      p2 = a + b;
      p3 = a + c;
      p4 = a + b + c;
      feature = allfeature + i * BINNUM;

      for (j = 0; j < BINNUM; ++j) {
        feature[j] = bin[p1 + j] + bin[p4 + j] - bin[p2 + j] - bin[p3 + j];
      }
    }

    logtime(1);
    FREE(fzb);
  }

  return num * BINNUM;
}

#define MAXSPI (CARTMAXSPLIT*3)
int catrain(BOOST* hh, WORKINFO* pwi, int snum, double* subData)
{
  CARTF_FEAT_T* ft = (CARTF_FEAT_T*)(hh->h_feat);
  int i, j, dim, nnum = 0, nn;
  int* labels = ft->Labels;
  int maxsplit = CARTMAXSPLIT;
  carnode nodes[MAXSPI];
  int maxlen = MAXSPI;
  int off = pwi->beg;
  double thd, spiterror;
  double* weight = hh->wi;
  double errors[2][MAXSPI];
  double deltas[MAXSPI];
  int idx[MAXSPI];
  int chr[2][MAXSPI];
  int papa[MAXSPI];
  char isok[MAXSPI];
  double pos[2], neg[2];
  char* mask[2];

  mask[0] = MALLOC(char, snum);
  mask[1] = MALLOC(char, snum);
  memset(isok, 0, MAXSPI);

  for (i = 0; i < MAXSPI; ++i) {
    idx[i] = 0;
    isok[i] = 0;
    deltas[i] = 0;
    errors[0][i] = 0;
    errors[1][i] = 0;
    chr[0][i] = 0;
    chr[1][i] = 0;
    papa[i] = 0;
    initnode(nodes[i]);
  }

#define SETDIM(a, b, c)  (a.x=b.x,a.y=b.y,a.w=b.w,a.h=b.h,a.id=c)

  for (i = 0; i < maxsplit; ++i) {
    initnode(nodes[i]);
  }

  onelearn(hh, pwi, snum, subData, 0, labels);
  dim = pwi->id, thd = pwi->Threshold, spiterror = pwi->Error;
  ASSERT(0 == off % 9);
  SETDIM(nodes[nnum], ft->allfeat[(dim + off) / 9], (dim) % 9), nodes[nnum].constrain = thd, ++nnum;
  isok[0] = 1;
  get_tj(snum, subData, labels, weight, nnum - 1, nodes, pos, neg);

  if (pos[1] == 0 && neg[1] == 0) {
    return 0;
  }

  if (pos[0] == 0 && neg[0] == 0) {
    return 0;
  }

  errors[0][0] = MIN(pos[0], neg[0]);
  errors[1][0] = MIN(pos[1], neg[1]);
  nn = 0; // 待分裂的节点

  for (i = 1; i < maxsplit; ++i) {
    double max_deltas;
    int best_split = 0;
    ASSERT(isok[nn] == 1);
    get_mask(snum, subData, labels, weight, nn, nodes, mask[0], mask[1]);

    //FPRINT1D("mask0.txt", "%d\r\n", snum, mask[0], 1);
    //FPRINT1D("mask1.txt", "%d\r\n", snum, mask[1], 1);
    for (j = 0; j < 2; ++j) {
      onelearn(hh, pwi, snum, subData, mask[j], labels);
      dim = pwi->id, thd = pwi->Threshold, spiterror = pwi->Error;
      assert(nnum < maxlen);
      assert(nodes[nn].chr[j] == 0);
      SETDIM(nodes[nnum], ft->allfeat[(dim + off) / 9], (dim) % 9), nodes[nnum].constrain = thd, nodes[nn].chr[!j] = nnum, papa[nnum] = nn << 1 | (!j), ++nnum;
      get_tj(snum, subData, labels, weight, nnum - 1, nodes, pos, neg);

      if ((pos[1] + neg[1]) == 0 || (pos[0] + neg[0]) == 0) {
        deltas[nnum - 1] = 0;
      }
      else {
        deltas[nnum - 1] = errors[!j][nn] - spiterror; //上一层错误率减去当前错误率
      }

      errors[0][nnum - 1] = MIN(pos[0], neg[0]);
      errors[1][nnum - 1] = MIN(pos[1], neg[1]);
      nodes[nn].chr[!j] = 0;
    }

    max_deltas = -INF;

    // 在叶子节点中，选出最大 deltas 的一个
    for (j = 0; j < nnum; ++j) {
      if (0 == isok[j]) {
        if (deltas[j] > max_deltas) {
          max_deltas = deltas[j];
          best_split = j;
        }
      }
    }

    {
      int m = papa[best_split];
      nodes[m >> 1].chr[(m & 1)] = best_split;
    }

    ASSERT(best_split != 0);
    nn = best_split;// 下一次待分裂的节点
    isok[nn] = 1;
  }

  j = 0;

  for (i = 0; i < nnum; ++i) {
    idx[i] = 0;

    if (isok[i]) {
      idx[i] = j;
      nodes[j++] = nodes[i];
    }
  }

  nnum = j;

  for (i = 0; i < nnum; ++i) {
    nodes[i].chr[0] = idx[nodes[i].chr[0]];
    nodes[i].chr[1] = idx[nodes[i].chr[1]];
  }

  for (i = 0; i < maxsplit; ++i) {
    pwi->cart.nodes[i] = nodes[i];
    logprintf("%+3.16lf %4d %4d %+3.16lf %+3.16lf\r\n", nodes[i].constrain,
        nodes[i].chr[0], nodes[i].chr[1], nodes[i].coeffs[0], nodes[i].coeffs[1]);
  }

  FREE(mask[0]);
  FREE(mask[1]);
  return nnum;
}

int CARTW_update_weight(BOOST* hh, WORKINFO* pwi)
{
  CARTF_FEAT_T* ft = (CARTF_FEAT_T*)(hh->h_feat);
  int i, j, k, maxsplit = CARTMAXSPLIT, tlen = CARTMAXSPLIT;
  int snum = hh->samp_num;
  double* Data = ft->samp;
  double* weight = hh->wi;
  double* final_hyp = MALLOC(double, snum);
  int* Labels = ft->Labels;
  carnode* Trees = pwi->cart.nodes;// hh->ca->cart[hh->ca->weaklen].nodes;
  int* cartout = MALLOC(int, snum);
  double Z, invsnum = 1. / snum;
  //carnode Trees[100];
  double pos[100];
  double neg[100];
  FILL(snum, final_hyp, 0.);
  FILL(snum, cartout, 0);
  FILL(100, pos, 0);
  FILL(100, neg, 0);
  get_cartout(snum, Data, Labels, weight, Trees, pos, neg, cartout);

  for (i = 0; i < tlen; ++i) {
    double s1, s2, Alpha;

    for (j = 0; j < 2; ++j) {
      if (0 == Trees[i].chr[j]) {
        int m = i << 1 | j;
        s1 = pos[m];
        s2 = neg[m];

        if (s1 == 0 && s2 == 0) {
          continue;
        }

        Alpha = 0.5 * log((s1 + EPS) / (s2 + EPS));
        //累加得分
        Trees[i].coeffs[j] = Alpha;

        for (k = 0; k < snum; ++k) {
          if (m == cartout[k]) {
            final_hyp[k] += Alpha;
          }
        }
      }
    }
  }

  Z = 0;

  for (i = 0; i < snum; ++i) {
    if (Labels[i]) {
      double x = exp(-1 * (Labels[i] * final_hyp[i]));
      weight[i] = x;
      Z += weight[i];
      logprintf("%lf\r\n", x);
    }
  }

  //Z = sum(weight);
  for (i = 0; i < snum; ++i) {
    if (Labels[i]) {
      weight[i] = weight[i] / Z;
    }
  }

  //hh->ca->cart[hh->ca->weaklen++] = pwi->cart;
  {
    double* Result;
    double* PosResult;
    double x, thresh, Error, Detect, False;
    int FalseNum = 0, DetectNum = 0, ErrorNum = 0, PosNum = 0, NegNum = 0, AllNum = 0, num;
    Result = hh->vi;
    PosResult = MALLOC(double, snum);

    for (i = 0; i < snum; ++i) {
      if (Labels[i]) {
        x = CARTF_feat_val(Data + i * SAMPSTEP, INTSTEP, &pwi->cart, 0, 0);
        Result[i] += x;

        //提取正样本的得分
        if (Labels[i] < 0) {
          ++NegNum;
        }
        else if (Labels[i] > 0) {
          PosResult[PosNum] = Result[i];
          ++PosNum;
        }

        ++AllNum;
      }
    }

#define INDEX_LESS(a, b)  ((a) > (b))

    QSORT(PosResult, PosResult + PosNum, INDEX_LESS, double);
#undef INDEX_LESS

    //计算需要分对的正样本个数
    num = (int)(PosNum * 0.995);
    thresh = PosResult[num] - EPS;

    for (i = 0; i < snum; ++i) {
      int s = Result[i] > thresh ? 1 : -1;

      if (s != Labels[i]) {
        ErrorNum++;
      }

      if (Labels[i] > 0 && s == 1) {
        DetectNum++;
      }

      if (Labels[i] < 0 && s == 1) {
        FalseNum++;
      }
    }

    Error = ErrorNum * 1. / AllNum;
    Detect = DetectNum * 1. / (PosNum);
    False = FalseNum * 1. / (NegNum);

    FREE(PosResult);

    pwi->Ha = Detect;
    pwi->Fa = False;
    logprintf("thresh: %3.8lf\n", thresh);
    logprintf("总体错误率: %3.8lf\n", Error);
    logprintf("正样本的检测率(ha): %3.8lf\n", Detect);
    logprintf("负样本的误检率(fa): %3.8lf\n", False);

  }
  return 0;
}

// maxsplit 每棵树的最大节点数目
int CARTW_get_error(BOOST* hh, WORKINFO* pwi)
{
  CARTF_FEAT_T* ft = (CARTF_FEAT_T*)(hh->h_feat);
  int blockdim = 36;
  int sdim = pwi->end - pwi->beg, d1, tlen;
  int blockid, blocknum = sdim / blockdim;
  int snum = hh->samp_num;
  int maxsplit = CARTMAXSPLIT;
  double* AllData = ft->samp;
  double* subData;
  int* Labels = ft->Labels;
  WORKINFO wi = *pwi;
  //first choose a block, then train a cartree on it
  onelearn(hh, pwi, snum, AllData, 0, Labels);

  blockid = (pwi->id / blockdim);

  d1 = blockid * blockdim;
  subData = AllData;
  wi.beg = d1;
  wi.end = d1 + blockdim;
  tlen = catrain(hh, &wi, snum, subData);
  pwi->cart = wi.cart;
  return tlen;
}


#undef SAMPSTEP
#undef INTSTEP
