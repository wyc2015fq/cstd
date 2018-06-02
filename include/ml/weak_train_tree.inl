{
  int i, j, k = 0, K = 1;
  worktype* tmp_feat = fi;
  int feat_num = pwi->end - pwi->beg;
  double cdf0[256], cdf1[256], e0 = 1, e1 = 0, e;
  int nBins = countof(cdf0), thr;
  uchar* data = 0;
  double* wts[1 << (forest_weak_level + 1)] = {0};
  int id[1 << (forest_weak_level + 1)] = {0};
  int child[1 << (forest_weak_level + 1)] = {0};
  int depth[1 << (forest_weak_level + 1)] = {0};
  double thrs[1 << (forest_weak_level + 1)] = {0};
  double errs[1 << (forest_weak_level + 1)] = {0};
  double hs[1 << (forest_weak_level + 1)] = {0};
  DRANGE ra;
  double binval;
  double sumw[2], prior, sum;
  data = MALLOC(uchar, samp_num);
  wts[0] = MALLOC(double, samp_num);

  for (j = 0; j < samp_num; ++j) {
    wts[0][j] = wi[j];
  }

  for (k = 0; k < K; ++k) {
    double* wt = wts[k];
    sumw[0] = sumw[1] = 0;

    for (j = 0; j < samp_num; ++j) {
      if (wt[j] > 0) {
        sumw[j < pos_num] += wt[j];
      }
    }

    sum = sumw[0] + sumw[1];

    if (sum > 0) {
      for (j = 0; j < samp_num; ++j) {
        if (wt[j] > 0) {
          wt[j] /= sum;
        }
      }

      prior = sumw[1] / sum;
      errs[k] = min(prior, 1 - prior);
      hs[k] = .5 * log(prior / (1 - prior));
      hs[k] = BOUND(hs[k], -4, 4);
    }

    if (depth[k] < forest_weak_level) { //%限制了树的深度
      double thd, mine = 100000, minminval = 0, minbinval = 1;
      depth[K] = depth[K + 1] = depth[k] + 1;
      child[k] = K;

      for (i = pwi->beg; i < pwi->end; ++i) {
        if (flag && flag[ i ]) {
          continue;
        }

        get_feat_macro(i, tmp_feat);
        ra = minmax_64f(samp_num, tmp_feat);
        binval = 255. / (ra.r - ra.l);

        for (j = 0; j < samp_num; ++j) {
          data[j] = (uchar)((tmp_feat[j] - ra.l) * binval);
        }

        constructCdf(data, wt, nBins, pos_num, cdf0);
        constructCdf(data + pos_num, wt + pos_num, nBins, neg_num, cdf1);

        for (j = 0; j < nBins; j++) {
          e = prior - cdf1[j] + cdf0[j];

          if (e < e0) {
            e0 = e, e1 = 1 - e, thr = j;
          }
          else if (e > e1) {
            e0 = 1 - e, e1 = e, thr = j;
          }
        }

        if (e0 < mine) {
          mine = e0;
          id[k] = i;
          minminval = ra.l;
          minbinval = binval;
        }
      }

      ASSERT(k < 3);
      get_feat_macro(id[k], tmp_feat);
      thrs[k] = thd = minminval + (thr + 0.5) / minbinval;
      wts[K + 0] = MALLOC(double, samp_num);
      wts[K + 1] = MALLOC(double, samp_num);

      for (j = 0; j < samp_num; ++j) {
        if (wt[j] < 0) {
          wts[K + 0][j] = wts[K + 1][j] = -1;
        }
        else {
          int tt = tmp_feat[j] < thd;
          wts[K + !tt][j] = wt[j];
          wts[K + tt][j] = 0;
        }
      }

      K += 2;
    }
  }

  pwi->Error = 0;

  for (k = 0; k < K; ++k) {
    if (wts[k]) {
      FREE(wts[k]);
    }

    if (k < 3) {
      weak->id[k] = id[k];
      weak->thd[k] = thrs[k];
    }
    else {
      weak->val[k - 3] = hs[k];
      pwi->Error += errs[k];
    }
  }

  FREE(data);
  {
    worktype* fi0 = MALLOC(worktype, 3 * samp_num);
    worktype* fi1 = fi0 + samp_num;
    worktype* fi2 = fi0 + samp_num * 2;
    int yy, kk;
    double ht, beta, alpha, err;
    get_feat_macro(weak->id[0], fi0);
    get_feat_macro(weak->id[1], fi1);
    get_feat_macro(weak->id[2], fi2);
    err = BOUND(pwi->Error, x_esp, 1 - x_esp);
    alpha = (double) 0.5 * log((1 - err) / err);
    beta = exp(-alpha);

    for (i = 0; i < samp_num; ++i) {
      if (wi[ i ] < 0.f) {
        continue;
      }

      FOREST_VAL(fi0[i], fi1[i], fi2[i], weak, ht);
      vi[ i ] += ht;
      yy = i < pos_num ? 1 : -1;
      kk = ht > 0.f ? 1 : -1;

      if (kk == yy) {
        wi[ i ] *= beta;
      }
    }

    FREE(fi0);
  }
}
