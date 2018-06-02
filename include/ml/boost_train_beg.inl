{
  worktype* vi;           // [samp_num] 样本得分
  worktype* wi;           // [samp_num] 样本权重
  worktype* fi;           // [samp_num] 特征数据权重
  uchar* flag = 0;  // [feat_num] 特征索引，特征没有用过=0，特征已经用过=1
  int i, k = 0, len = 0, pos_cnt = 0, neg_cnt = 0, ispass = 0;
  int has_pos = 0, has_neg = 0;
  double neg_rate, score;
  double x_esp = (0.00001f);
  weak_type* weak = 0;
  stage_32f* stage = 0;
  int stage_beg = ca->stage_len;

  stage = ca->stage + ca->stage_len;
  weak = ((weak_type*)ca->weak) + ca->weak_len;
  vi = MALLOC(worktype, samp_num);
  wi = MALLOC(worktype, samp_num);
  fi = MALLOC(worktype, samp_num);

  if (flag_unreselect) {
    flag = MALLOC(uchar, feat_num);
    MEMSET(flag, 0, feat_num);
  }

  for (i = 0; i < samp_num; ++i) {
    vi[i] = 0;
    wi[i] = -1;
    fi[i] = 0;
  }

  for (; ca->stage_len < maxstagelen; ca->stage_len++, ++stage) {
    int weaklen = ca->weak_len;

    for (i = 0; i < samp_num; ++i) {
      vi[i] = 0;
    }

    if (stage_beg == ca->stage_len) {
      int allcnt = 0, cnt = 0;

      for (i = 0; i < pos_num;) {
        ++allcnt;

        if (wi[i] < 0) {
          has_pos = ispass = 0;
          get_pos_macro(ispass, score, has_pos, samp + i * sampstep);

          if (!has_pos) {
            break;
          }

          if (ispass) {
            wi[ i ] = 1.;
            vi[ i ] = score;
            ++i;
          }

          ++cnt;
          printf("+%d/%d/%d/%d/%d        \r", 0, pos_num, i, cnt, allcnt);
        }
        else {
          ++i;
        }
      }

      printf("+%d/%d/%d/%d/%d        \n", 0, pos_num, i, cnt, allcnt);
    }

    neg_cnt = 0;

    for (i = pos_num; i < samp_num; ++i) {
      neg_cnt += wi[i] >= 0;
    }

    neg_rate = (double)neg_cnt / neg_num;

    if (neg_rate < min_neg_rate) {
      int allcnt = 0, cnt = 0;

      for (i = pos_num; i < samp_num;) {
        ++allcnt;

        if (wi[i] < 0) {
          has_neg = ispass = 0;
          get_neg_macro(ispass, score, has_neg, samp + i * sampstep);

          if (!has_neg) {
            break;
          }

          if (ispass) {
            wi[ i ] = 1.;
            vi[ i ] = score;
            ++i;
          }

          ++cnt;
          printf("-%d/%d/%d/%d/%d      \r", neg_cnt, neg_num, i - pos_num, cnt, allcnt);
        }
        else {
          ++i;
        }
      }

      printf("-%d/%d/%d/%d/%d      \n", neg_cnt, neg_num, i - pos_num, cnt, allcnt);
    }

    pos_cnt = 0;
    neg_cnt = 0;

    for (i = 0; i < pos_num; ++i) {
      pos_cnt += wi[i] >= 0;
    }

    for (i = pos_num; i < samp_num; ++i) {
      neg_cnt += wi[i] >= 0;
    }

    print_macro("+%d +%d\n", pos_cnt, neg_cnt);
    neg_rate = (double)neg_cnt / neg_num;

    if (neg_rate < min_neg_rate) {
      logprintf("负样本数目太少了，要添加样本!\n");
      break;
    }

    {
      // c0 正样本初始权重
      // c1 负样本初始权重
      double c0, c1;
      c0 = 0.5 / pos_cnt;
      c1 = 0.5 / neg_cnt;

      for (i = 0; i < pos_num; ++i) {
        if (wi[i] >= 0) {
          wi[i] = c0;
        }
      }

      for (i = pos_num; i < samp_num; ++i) {
        if (wi[i] >= 0) {
          wi[i] = c1;
        }
      }
    }

    print_macro("--------------------------------- Stage %2d ------------------------------------\n", ca->stage_len);
    print_macro("  ha   Fa  |Error  Time\n");

    for (; ca->weak_len < maxweaklen;) {
      weak_train_info_t pwi[1] = {0};
      int samp_cnt[2] = {0};
      int pass_cnt[2] = {0};
      double sumw = 0;

      for (i = 0; i < samp_num; ++i) {
        if (wi[i] > 0) {
          sumw += wi[i];
        }
      }

      for (i = 0; i < samp_num; ++i) {
        if (wi[i] > 0) {
          wi[i] /= sumw;
        }
      }

      pwi->beg = 0, pwi->end = feat_num;