{
  int FeatNum = pwi->end - pwi->beg;
  int i, j;
  double e;
  double poswi[ LUT_BIN ] = { 0 };
  double negwi[ LUT_BIN ] = { 0 };
  double* tmp_feat = fi;
  utime_start(time);
  weak->id = -1;
  pwi->Error = 100000.f;

  //tmp_feat = MALLOC( double, samp_num );
  for (i = pwi->beg; i < pwi->end; ++i) {
    if (i % 100 == 0) {
      printf printweak(pwi, weak, "");
      printf(" %5.f %%%d          \r", utime_elapsed(time), i * 100 / FeatNum);
    }

    //DO_PRINT_PRO(pro, FeatNum, (int) utime_elapsed(time));

    if (flag && flag[ i ]) {
      continue;
    }

    get_feat_macro(i, tmp_feat);
    get_min_map(samp_num, tmp_feat, wi, pos_num, poswi, negwi, LUT_BIN, NULL);
    e = 0.;

    for (j = 0; j < LUT_BIN; ++j) {
      e += (2. * sqrt(poswi[ j ] * negwi[ j ]));
    }

    if (e < pwi->Error) {
      weak->id = i;
      pwi->Error = e;
    }
  }

  printf printweak(pwi, weak, "              \r");

  //SAFEFREE( perr );
  pwi->Time = (double) utime_elapsed(time);
  {
    //self& ww = *this;
    get_feat_macro(weak->id, tmp_feat);
    get_min_map(samp_num, tmp_feat, wi, pos_num, poswi, negwi, LUT_BIN, weak);

    for (i = 0; i < LUT_BIN; ++i) {
      weak->val[ i ] = 0.5 * log((poswi[ i ] + x_esp) / (negwi[ i ] + x_esp));
    }

    // 更新权重
    for (i = 0; i < samp_num; ++i) {
      if (wi[ i ] > 0.f) {
        double ht, yy;
        LUT_VAL(fi[ i ], weak, ht);       // - thd;
        vi[ i ] += ht;

        // if bb>1. 表示被当成正样本
        yy = i < pos_num ? 1.0f : -1.0f;
#if 1
        wi[ i ] *= exp(-1.0f * (yy) * (ht));
#else
        wi[ i ] *= (10. * exp(-1.0 * (yy) * (ht)) + 1) / (10. + 1.);
#endif
      }
    }
  }
}
