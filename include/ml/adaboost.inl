

#define SET_FEAT_FLAT(_FEAT_NUM, _FLAG, _ID, _K) if ((_ID) >= 0 && (_ID) < _FEAT_NUM) { *(_FLAG + (_ID)) = (_K); }

int get_samp_num(const double* f, const double* l)
{
  int cnt = 0;

  for (; f != l; ++f) {
    cnt += (*f >= 0.f);   // 如果此样本的权值为负，表示此样本已经被丢弃
  }

  return cnt;
}

// pWeight: 权重数组
// m:  正样本数目
// n:  负样本数目
// c1: 正样本初始权重
// c2: 负样本初始权重
int weight_fill(double* f, double* l, double c)
{
  for (; f != l; ++f) {
    if (*f >= 0.f) {
      *f = c;
    }
  }

  return 1;
}

// 权重规一
double weight_div(double* f, double* l, double init)
{
  for (; f != l; ++f) {
    if (*f > 0.f) {
      *f /= init;
    }
  }

  return init;
}

double weight_guiyi(double* f, double* l)
{
  double init = 0.f;

  for (; f != l; ++f) {
    if (*f > 0.f) {
      init += *f;
    }
  }

  for (; f != l; ++f) {
    if (*f > 0.f) {
      *f /= init;
    }
  }

  return init;
}

// [f, m] 正样本权重数组
// [m, l] 负样本权重数组
// min_neg_rate 最小负样本率
// ha  通过率
int weight_inialize_impl1(double* f, double* m, double* l, double min_neg_rate, double ha, double Hr)
{
  //st_size = 0;
  int pos_num = get_samp_num(f, m);     // 正样本数目
  int neg_num = get_samp_num(m, l);     // 负样本数目
  double neg_rate = (double) neg_num / (l - m);
  logprintf("正样本数目 = %5d, 负样本数目 = %5d\n", pos_num, neg_num);

  if (neg_rate < min_neg_rate || (!neg_num)) {
    logprintf("负样本数目太少了，要添加样本!\n");
    return 0;
  }

  if (!pos_num) {
    logprintf("正样本数目太少了，要添加样本!\n");
    return 0;
  }

  {
    enum {TYPE1, TYPE2, TYPE3, TYPE4, TYPE5};
    enum {TYPE = TYPE5};
    // c0 正样本初始权重
    // c1 负样本初始权重
    double c0 = 0.f;
    double c1 = 0.f;

    if (TYPE == TYPE1) {
      c1 = 1.f / (pos_num * Hr + neg_num);
      c0 = ha * c1;
    }
    else if (TYPE == TYPE2) {
      int samp_num = pos_num + neg_num;
      c0 = ha;
      c1 = (1.f - ha);
    }
    else if (TYPE == TYPE3) {
      int samp_num = pos_num + neg_num;
      c0 = ha / pos_num;
      c1 = (1.f - ha) / neg_num;
    }
    else if (TYPE == TYPE4) {
      int samp_num = pos_num + neg_num;
      c0 = 1.f / samp_num;
      c1 = c0;
    }
    else if (TYPE == TYPE5) {
      int samp_num = pos_num + neg_num;
      c0 = 0.5f / pos_num;
      c1 = 0.5f / neg_num;
    }

    weight_fill(f, m, c0);
    weight_fill(m, l, c1);
  }

  return 1;
}

#define weight_inialize( hh )  weight_inialize_impl1( hh->wi, hh->wi + hh->pos_num, hh->wi + hh->samp_num, hh->min_neg_rate, hh->ha, hh->Hr )

int find_samp(BOOST* hh)
{
  int ret, samp_send_type = 0;        // 样本发送方式
  int pos_num, neg_num, nPosNum, nNegNum;
  double pos_rate, neg_rate;
  void freeblockcol(void);
  //freeblockcol();
  nPosNum = hh->pos_num;
  nNegNum = hh->samp_num - hh->pos_num;
  pos_num = get_samp_num(hh->wi, hh->wi + hh->pos_num);
  neg_num = get_samp_num(hh->wi + hh->pos_num, hh->wi + hh->samp_num);
  pos_rate = pos_num * 1. / nPosNum;  // 正样本比例
  neg_rate = neg_num * 1. / nNegNum;  // 负样本比例

  logprintf("正样本数目 = %5d, 负样本数目 = %5d\n", pos_num, neg_num);

  // 添加正样本
  if (pos_rate < 0.01f) {
    findsamp(hh, 1);
  }

  // 添加负样本
  if (neg_rate <= hh->min_neg_rate) {
    double* wi = hh->wi + hh->pos_num;
    //FILL(nNegNum, wi, -1);
    findsamp(hh, -1);
  }

  neg_rate = neg_num * 1. / nNegNum;  // 负样本比例
  ret = weight_inialize(hh);

  if (!ret) {
    logprintf("没样本了！\n");
  }

  return ret;
}

int update_weight1(BOOST* hh, WORKINFO* pwi)
{
  int i;
  static const double esp = X_ESP;        //1.0/(2*bin);
  int SampNum = hh->samp_num, PosNum = hh->pos_num;
  int poscntha;
  int samp_cnt[ 2 ] = { 0 };
  int pass_cnt[ 2 ] = { 0 };
  double wisum = 0;
  double* wi = hh->wi;
  updateweight(hh, pwi);

  for (i = 0; i < SampNum; ++i) {
    if (hh->wi[ i ] > 0.f) {
      wisum += hh->wi[ i ];
    }
  }

  weight_div(hh->wi, hh->wi + SampNum, wisum);
  {
    {
      double* p;
      p = MALLOCSET(double, PosNum, 0);

      for (i = 0; i < SampNum; ++i) {
        if (hh->wi[ i ] > 0.f) {
          int aa = i < PosNum;

          if (aa) {
            p[ samp_cnt[ 1 ] ] = hh->vi[ i ];
          }

          samp_cnt[ aa ] ++;
        }
      }

      // 用ha算阀值
      if (1) {
        poscntha = (int) floor(samp_cnt[ 1 ] - hh->ha * samp_cnt[ 1 ]);
        poscntha = MIN(MAX(poscntha, 0), samp_cnt[ 1 ] - 1);
        QSORT(p, p + samp_cnt[ 1 ], LESS, double);
        pwi->Threshold = p[ poscntha ] - esp;
      }

      FREE(p);
    }

    // 算通过数目
    for (i = 0; i < SampNum; ++i) {
      if (hh->wi[ i ] > 0.f) {
        int aa = i < PosNum;
        int cc = hh->vi[ i ] > pwi->Threshold;

        if (cc) {
          // 表示通过了
          pass_cnt[ aa ] ++;
        }
      }
    }

    pwi->Ha = (double) pass_cnt[ 1 ] / samp_cnt[ 1 ];
    pwi->Fa = (double) pass_cnt[ 0 ] / samp_cnt[ 0 ];
  }

  if (hh->flag && hh->flag_unreselect) {
    SET_FEAT_FLAT(hh->feat_num, hh->flag, pwi->id, 1);
  }

#define FFMT "%9.6g"
#define LOGFMT "%6d|"FFMT " "FFMT "| "FFMT "  "FFMT "  "FFMT "\n"
  logprintf(LOGFMT, pwi->id, pwi->Ha, pwi->Fa, pwi->Error, pwi->Threshold, pwi->Time);
  return (pwi->Ha < hh->ha || pwi->Fa > hh->fa);
}

//#define GET_ERROR get_Error_Net
#define GET_ERROR get_Error
int get_Error(BOOST* hh, WORKINFO* pwi)
{
  int AllFeatNum = pwi->end - pwi->beg;
  utime_start(time);
  pwi->Ha = pwi->Fa = pwi->Error = pwi->Time = 0.f;
  pwi->id = -1;
  weight_guiyi(hh->wi, hh->wi + hh->samp_num);
  //hh->get_error(hh, pwi);
  geterror(hh, pwi, getfeat);

  pwi->Time = (double) utime_elapsed(time);

  return pwi->id >= 0;
}
//#include "get_Error.inl"

#if 0
typedef struct get_Error_Net_t {
  WORKINFO workinfo;
}
get_Error_Net_t;

get_Error_Net_t gt[ 10 ] = {0};
int gtinited = 0;

int get_Error_Net_Impl(BOOST* hh, WORKINFO* pwi)
{
  int AllFeatNum = pwi->end - pwi->beg;
  double SumPower = 0;
  int beg0 = pwi->beg;
  int i, n = 0, ret = 1;
  char* buf;
  int len;
  int rank, size;
  MPI_Status status;
  static int inited = 0;
  utime_start(time);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  //需要初始化
  if (0 == inited) {
    {
      const char* needfile[] = {
        "allimgdata.dat",
        "CHI_FEAT_FILE.dat",
        "featfilecol.dat"
      };
      char buf[256];
      printf("%s\n", getcwd(buf, 256));

      for (i = 0; i < countof(needfile); ++i) {
        MPI_BcastFile(needfile[i], 0, MPI_COMM_WORLD);
      }
    }
    inited = 1;
    ret = MPI_Bcast(hh, sizeof(*hh), MPI_BYTE, 0, MPI_COMM_WORLD);

    if (ret < 0) {
      return 0;
    }

    if (0 == rank) {
    }
    else {
      hh->ca = hh->h_feat = 0;

      if (!init_boost_mem(hh)) {
        return 0;
      }
    }
  }

  samp_data(hh, buf, len);

  if (0 == rank) {
    for (i = 0; i < size; ++i) {
      double Power; // 计算能力

      if (gt[ i ].workinfo.Time > 0.f && gt[ i ].workinfo.end > gt[ i ].workinfo.beg) {
        Power = (gt[ i ].workinfo.end - gt[ i ].workinfo.beg) / gt[ i ].workinfo.Time;
      }
      else {
        Power = 1.f;
      }

      SumPower += Power;
    }

    for (i = 0; i < size; ++i) {
      double Power; // 计算能力

      if (gt[ i ].workinfo.Time > 0.f && gt[ i ].workinfo.end > gt[ i ].workinfo.beg) {
        Power = (gt[ i ].workinfo.end - gt[ i ].workinfo.beg) / gt[ i ].workinfo.Time;
      }
      else {
        Power = 1.f;
      }

      gt[ i ].workinfo.beg = beg0;
      beg0 += (int)(((Power) / SumPower) * AllFeatNum);
      gt[ i ].workinfo.end = beg0;

      if (i == (size - 1)) {
        gt[ i ].workinfo.end = AllFeatNum;
      }

      //logprintf("%2d:[%5d,%5d]%5d %f %f\n", i, gt[ i ].workinfo.beg, gt[ i ].workinfo.end,
      //  gt[ i ].workinfo.end-gt[ i ].workinfo.beg, gt[ i ].workinfo.Time, SumPower);
      //logprintf("Power%d=%f\n", i, Power);
    }

    for (i = 1; i < size; ++i) {
      if (ret > 0) {
        ret = MPI_Send(gt + i, sizeof(gt[0]), MPI_BYTE, i, 0, MPI_COMM_WORLD);
      }

      if (ret > 0) {
        ret = MPI_Send(hh->flag, hh->feat_num * sizeof(hh->flag[ 0 ]), MPI_BYTE, i, 0, MPI_COMM_WORLD);
      }

      if (ret > 0) {
        ret = MPI_Send(hh->wi, hh->samp_num * sizeof(hh->wi[ 0 ]), MPI_BYTE, i, 0, MPI_COMM_WORLD);
      }

      if (ret > 0) {
        ret = MPI_Send(hh->vi, hh->samp_num * sizeof(hh->vi[ 0 ]), MPI_BYTE, i, 0, MPI_COMM_WORLD);
      }

      if (ret > 0) {
        ret = MPI_Send(hh->fi, hh->samp_num * sizeof(hh->fi[ 0 ]), MPI_BYTE, i, 0, MPI_COMM_WORLD);
      }

      if (ret > 0) {
        ret = MPI_Send(buf, len, MPI_BYTE, i, 0, MPI_COMM_WORLD);
      }
    }
  }
  else {
    i = rank;
    //printf("%d\n", GetFeatNum( hh ));
    //printf("%d\n", GetSampNum( hh ));
    //printf("%d\n", len);
    MPI_RECV(gt + i, sizeof(gt[0]), MPI_BYTE, 0, 0, MPI_COMM_WORLD);
    MPI_RECV(hh->flag, hh->feat_num * sizeof(hh->flag[ 0 ]), MPI_BYTE, 0, 0, MPI_COMM_WORLD);
    MPI_RECV(hh->wi, hh->samp_num * sizeof(hh->wi[ 0 ]), MPI_BYTE, 0, 0, MPI_COMM_WORLD);
    MPI_RECV(hh->vi, hh->samp_num * sizeof(hh->vi[ 0 ]), MPI_BYTE, 0, 0, MPI_COMM_WORLD);
    MPI_RECV(hh->fi, hh->samp_num * sizeof(hh->fi[ 0 ]), MPI_BYTE, 0, 0, MPI_COMM_WORLD);
    MPI_RECV(buf, len, MPI_BYTE, 0, 0, MPI_COMM_WORLD);
  }

  get_Error(hh, &gt[rank].workinfo);

  //gt[rank].workinfo.Time = utime_elapsed( time );
  if (0 == rank) {
    int beg = pwi->beg;
    int end = pwi->end;
    *pwi = gt[ 0 ].workinfo;

    for (i = 1; i < size; ++i) {
      MPI_RECV(gt + i, sizeof(gt[0]), MPI_BYTE, i, 0, MPI_COMM_WORLD);

      if (pwi->Error >= gt[ i ].workinfo.Error) {
        *pwi = gt[ i ].workinfo;
      }
    }

    pwi->beg = beg;
    pwi->end = end;
    pwi->Time = utime_elapsed(time);
  }
  else {
    i = rank;

    if (ret > 0) {
      ret = MPI_Send(gt + i, sizeof(gt[0]), MPI_BYTE, 0, 0, MPI_COMM_WORLD);
    }
  }

  return ret;
}

int get_Error_Net(BOOST* hh, WORKINFO* pwi)
{
  get_Error_Net_Impl(hh, pwi);
  return pwi->id >= 0;
}
int get_Error_Net_StartServer()
{
  BOOST* hh = (BOOST*)malloc(sizeof(BOOST));
  WORKINFO workinfo;
  int ret = 1;
  memset(hh, 0, sizeof(BOOST));

  while (ret > 0) {
    ret = get_Error_Net_Impl(hh, &workinfo);

    if (ret <= 0) {
      //printf("err\n");
    }
  }

  del_boost_(hh);
  return 0;
}

#endif

