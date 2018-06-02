{
  int i, j, k;
  double e, lval, rval, posSum = 0, negSum = 0;
  int feat_num = pwi->end - pwi->beg;
  int* index = MALLOC(int, samp_num);
  utime_start(time);
  worktype* tmp_feat = fi;
  pwi->Error = 1000000.;

  for (i = 0; i < pos_num; ++i) {
    if (wi[i] > 0.) {
      posSum += wi[i];
    }
  }

  for (; i < samp_num; ++i) {
    if (wi[i] > 0.) {
      negSum += wi[i];
    }
  }

  for (i = pwi->beg; i < pwi->end; ++i) {
    if (flag && flag[ i ]) {
      continue;
    }

    get_feat_macro(i, tmp_feat);

    for (j = 0; j < samp_num; ++j) {
      index[ j ] = j;
    }

#define INDEX_LESS(a, b)  ((tmp_feat[a]) < (tmp_feat[b]))

    QSORT(index, index + samp_num, INDEX_LESS, int);
#undef INDEX_LESS
    e = 10000.f;
    k = GEN_geterr(samp_num, pos_num, tmp_feat, wi, index, &e, &lval, &rval, posSum, negSum);

    if (e <= pwi->Error) {
      pwi->Error = e;

      if (e < 0.0000001) {
        int asdf = 0;
      }

      weak->id = i;
      weak->val[ 0 ] = lval;
      weak->val[ 1 ] = rval;
      weak->thd = (k > 0) ? 0.5f * (tmp_feat[ index[ k ] ] + tmp_feat[ index[ k - 1 ] ]) : tmp_feat[ index[ k ] ];
    }
  }

  pwi->Time = (double) utime_elapsed(time);
  FREE(index);
  {
    int yy, kk;
    double ht, beta, alpha, err;
    err = BOUND(pwi->Error, x_esp, 1 - x_esp);
    alpha = (double) 0.5 * log((1 - err) / err);
    beta = exp(-alpha);

    //printf(FLOATFMT " %+1d ", alpha, pwi->val[0]<pwi->val[1]?-1:1);
    if (weak->val[ 0 ] < weak->val[ 1 ]) {
      weak->val[ 0 ] = 0.;
      weak->val[ 1 ] = alpha;
    }
    else {
      weak->val[ 0 ] = alpha;
      weak->val[ 1 ] = 0.;
    }

    get_feat_macro(weak->id, tmp_feat);

    // 更新分类器信息
    for (i = 0; i < samp_num; ++i) {
      // 权值为-1表示此样本在前面训练过程中被丢弃了
      if (wi[ i ] < 0.f) {
        continue;
      }

      GEN_VAL(tmp_feat[ i ], weak, ht);
      vi[ i ] += ht;

      // ref小于PosNum表示正样本, aa==1表示正样本, aa==0表示负样本
      yy = i < pos_num ? 1 : -1;
      kk = ht > 0.f ? 1 : -1;

      if (kk == yy) {
        wi[ i ] *= beta;
      }
    }
  }
  {
    int tt_end = 0;
  }
}
