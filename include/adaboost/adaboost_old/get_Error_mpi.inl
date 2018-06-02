
#ifndef countof
#define countof(_ARR)  (sizeof(_ARR)/sizeof((_ARR)[0]))
#endif // countof

int init_boost_mem(BOOST* hh);

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
        ret = MPI_Send(hh->flag, GetFeatNum(hh) * sizeof(hh->flag[ 0 ]), MPI_BYTE, i, 0, MPI_COMM_WORLD);
      }

      if (ret > 0) {
        ret = MPI_Send(hh->wi, GetSampNum(hh) * sizeof(hh->wi[ 0 ]), MPI_BYTE, i, 0, MPI_COMM_WORLD);
      }

      if (ret > 0) {
        ret = MPI_Send(hh->vi, GetSampNum(hh) * sizeof(hh->vi[ 0 ]), MPI_BYTE, i, 0, MPI_COMM_WORLD);
      }

      if (ret > 0) {
        ret = MPI_Send(hh->fi, GetSampNum(hh) * sizeof(hh->fi[ 0 ]), MPI_BYTE, i, 0, MPI_COMM_WORLD);
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
    MPI_RECV(hh->flag, GetFeatNum(hh) * sizeof(hh->flag[ 0 ]), MPI_BYTE, 0, 0, MPI_COMM_WORLD);
    MPI_RECV(hh->wi, GetSampNum(hh) * sizeof(hh->wi[ 0 ]), MPI_BYTE, 0, 0, MPI_COMM_WORLD);
    MPI_RECV(hh->vi, GetSampNum(hh) * sizeof(hh->vi[ 0 ]), MPI_BYTE, 0, 0, MPI_COMM_WORLD);
    MPI_RECV(hh->fi, GetSampNum(hh) * sizeof(hh->fi[ 0 ]), MPI_BYTE, 0, 0, MPI_COMM_WORLD);
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

