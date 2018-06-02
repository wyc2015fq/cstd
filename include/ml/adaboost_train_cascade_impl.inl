{
  BOOST hh[1] = {0};

  //char buf[ 256 ];
  boosttrain_init(hh);

  hh->vi = MALLOCSET(double, hh->samp_num, 0);
  hh->fi = MALLOCSET(double, hh->samp_num, 0);
  hh->wi = MALLOCSET(double, hh->samp_num, 0);      //-1表示没有样本
  //hh->vvv = MALLOCSET( double, hh->samp_num, 0 );  //-1表示没有样本
  FILL(hh->samp_num, hh->vi, 0.f);
  FILL(hh->samp_num, hh->fi, 0.f);
  FILL(hh->samp_num, hh->wi, -1.f);

  if (hh->feat_num > 0) {
    int i;
    hh->flag = MALLOCSET(unsigned char, hh->feat_num, 0);

    if (hh->ca != NULL) {
      const weaktype* weak = (const weaktype*)hh->ca->weak;

      // 去掉被选过的特征
      for (i = 0; i < hh->ca->weaklen; ++i) {
        SET_FEAT_FLAT(hh->feat_num, hh->flag, weak[i].id, 1);
      }
    }
  }

  {
    static const double esp = X_ESP;    //1.0/(2*bin);
    int i = 0, k, stagecnt, weakcnt;
    const int SampNum = hh->samp_num;
    const int PosNum = hh->pos_num;
    int is_continue_weak;
    int continue_runing = 0;  //表示是继续训练

    for (stagecnt = 0; (!hh->ca || stagecnt < hh->ca->maxstagelen) && (continue_runing || find_samp(hh)); ++stagecnt) {
      int is_continue_stage = 0;
      int baseweakcnt = (hh->ca && hh->ca->is_nesting && (hh->ca->stagelen) > 0) ? 1 : 0;
      logprintf("--------------------------------- Stage %2d ------------------------------------\n",
      (hh->ca) ? hh->ca->stagelen : stagecnt);
      logprintf("  Id  |     ha      Fa    |   Error    Threshold       Time\n");
      {
        WORKINFO pwi[1] = { 0 };
        pwi->beg = 0;
        pwi->end = hh->feat_num;
        pwi->id = -1;

        if (!continue_runing) {
          FILL(SampNum, hh->fi, 0.f);

          if (hh->ca->is_nesting && (hh->ca->stagelen) > 0) {
            COPY(SampNum, hh->vi, hh->fi);
            FILL(SampNum, hh->vi, 0);
            is_continue_weak = update_weight1(hh, pwi);

            if (!is_continue_weak) {
              // 同时满足Ha Fa
              break;
            }
          }
          else {
            FILL(SampNum, hh->vi, 0.f);
          }
        }
        else {
          int nw = getweakcnt(hh->ca);
          baseweakcnt += hh->ca->weaklen - nw;
        }

        continue_runing = 0;

        for (weakcnt = baseweakcnt; GET_ERROR(hh, pwi);) {
          double* fi;
          is_continue_weak = 0;

          if (hh->flag && pwi->id >= 0 && pwi->id < hh->feat_num) {
            hh->flag[ pwi->id ] = 1;
          }

          fi = getfeat(hh, pwi->id);
          COPY(SampNum, fi, hh->fi);
          is_continue_weak = update_weight1(hh, pwi);
          ++weakcnt;

          if (!is_continue_weak) {
            // 同时满足Ha Fa
            break;
          }
        }

        is_continue_stage = weakcnt;

        if (is_continue_stage) {
          hh->ca->stage[ hh->ca->stagelen ].thd = pwi->Threshold;
          hh->ca->stage[ hh->ca->stagelen ].len = weakcnt;

          for (k = 0; k < hh->samp_num; ++k) {
            if (hh->vi[ k ] < pwi->Threshold) {
              // 没有通过强分类器的样本的权值赋-1，表示抛弃样本
              hh->wi[ k ] = -1.f;
            }
          }

          ++(hh->ca->stagelen);
          //fseek( hh->cas, 0, SEEK_SET );
          //cascade_printf( hh->ca, hh->cas );
          cascade_save(hh->ca, "cas.dat");
          //cascade_save_txt(hh->ca, "cas.txt");
        }
        else {
          break;
        }
      }
    }
  }

  logprintf("训练完毕!\n");
  cascade_free(hh->ca);

  uninit_feat(hh);

  // 销毁样本容器对象
  FREE(hh->wi);
  FREE(hh->vi);
  FREE(hh->fi);
  //FREE(hh->Feat);
  FREE(hh->flag);
}
