
#define BOOSTJOBID 123
typedef struct get_Error_Net_t {
  BOOST* hh;
  WORKINFO workinfo;
  char addr[ 32 ];
  SOCKET sock;
  HANDLE hThread;
  LPTHREAD_START_ROUTINE fun;
  int can_use;
  int need_init;
}
get_Error_Net_t;

int WINAPI get_Error_Net_th(get_Error_Net_t* gt)
{
  SOCKET s = gt->sock;
  BOOST* hh = gt->hh;
  WORKINFO* workinfo = &gt->workinfo;
  int ret, ok;

  if (gt->need_init) {   //需要初始化
    if (INVALID_SOCKET != s) {
      //sockaddr_in addrRemote;
      //int nAddrLen = sizeof( addrRemote );
      //CPM_GetSockName( s, ( SOCKADDR* ) & addrRemote, &nAddrLen );
      //printf("StartJob: %s:%d\n", inet_ntoa( addrRemote.sin_addr ), ntohs(addrRemote.sin_port));
      ret = StartJob(s, BOOSTJOBID);

      if (0 == ret) {
        //printf("没找到服务!\n");
        closesocket(s);
        s = INVALID_SOCKET;
      }
      else {
        //printf("找到服务!\n");
      }
    }

    CPM_SendT(s, hh, sizeof(*hh), 0, 0);
    ret = CPM_RecvT(s, &ok, sizeof(int), 0, -1);

    if (ret < 0) {
      //printf("没找到服务!\n");
      closesocket(s);
      s = INVALID_SOCKET;
      gt->sock = INVALID_SOCKET;
      logprintf("服务器意外断开: %s\n", gt->addr);
      inisetint("马上扫描", 1);
    }

    gt->need_init = 0;
  }

  {
    char* buf;
    int len;
    samp_data(hh, buf, len);
    CPM_SendX(s, 10000, workinfo, sizeof(*workinfo),
        hh->flag, GetFeatNum(hh) * sizeof(hh->flag[ 0 ]),
        hh->wi, GetSampNum(hh) * sizeof(hh->wi[ 0 ]),
        hh->vi, GetSampNum(hh) * sizeof(hh->vi[ 0 ]),
        hh->fi, GetSampNum(hh) * sizeof(hh->fi[ 0 ]),
        buf, len, 0);
  }

  ret = CPM_RecvT(s, workinfo, sizeof(*workinfo), 0, inigetint("计算超时") * 1000);

  if (ret < 0) {
    //printf("没找到服务!\n");
    closesocket(s);
    s = INVALID_SOCKET;
    gt->sock = INVALID_SOCKET;
    logprintf("服务器意外断开: %s\n", gt->addr);
    //inisetint("马上扫描", 1);
  }

  return 0;
}

int init_boost_mem(BOOST* hh);

int get_Error_Net_Server(SOCKET s, void* userdata)
{
  BOOST boost;
  BOOST* hh = &boost;
  WORKINFO workinfo1;
  WORKINFO* workinfo = &workinfo1;
  int ret;
  //需要初始化
  ret = CPM_RecvT(s, hh, sizeof(*hh), 0, 30000);
  hh->ca = hh->h_feat = 0;

  if (ret < 0) {
    return 0;
  }

  if (!init_boost_mem(hh)) {
    return 0;
  }

  CPM_SendT(s, &ret, sizeof(int), 0, 0);

  while (ret >= 0) {
    char* buf;
    int len;
    samp_data(hh, buf, len);
    ret = CPM_RecvX(s, -1, workinfo, sizeof(*workinfo),
        hh->flag, GetFeatNum(hh) * sizeof(hh->flag[ 0 ]),
        hh->wi, GetSampNum(hh) * sizeof(hh->wi[ 0 ]),
        hh->vi, GetSampNum(hh) * sizeof(hh->vi[ 0 ]),
        hh->fi, GetSampNum(hh) * sizeof(hh->fi[ 0 ]),
        buf, len, 0);

    if (ret < 0) {
      break;
    }

    get_Error(hh, workinfo);
    CPM_SendT(s, workinfo, sizeof(*workinfo), 0, 0);
    printf("\n");
  }

  del_boost_(hh);
  return 0;
}

void get_Error_Net_StartServer()
{
  int nPort = 4567; // nPort 此服务器监听的端口号
  SOCKET sListen;
  // 注册服务
  joblist_op(BOOSTJOBID, get_Error_Net_Server);
  sListen = CPM_StartServer(nPort);

  if (INVALID_SOCKET == sListen) {
    printf("服务开启失败。port=%d\n", nPort);
    return ;
  }

  printf("服务开启了。port=%d\n", nPort);
  StartSelect(sListen, 0);
}
int WINAPI get_Error_Local(get_Error_Net_t* gt)
{
  get_Error(gt->hh, &(gt->workinfo));
  return 0;
}

get_Error_Net_t gt[ 256 ];
int gtinited = 0;

int WINAPI Scan_Net_th(void* h, int MAXIP)
{
  int i;
  DWORD dwTimeOut = 10;

  //while(1)
  if (0) {
    for (i = 1; i < MAXIP; ++i) {
      gt[ i ].fun = 0;
      gt[ i ].hh = 0;
      dwTimeOut = inigetint("扫描超时");

      if (dwTimeOut < 10) {
        dwTimeOut = 10;
      }

      if (gt[ i ].sock == INVALID_SOCKET) {
        printf("扫描%s\r", gt[ i ].addr);
        gt[ i ].sock = ScanConnect(gt[ i ].addr, dwTimeOut);

        if (gt[ i ].sock != INVALID_SOCKET) {
          gt[ i ].need_init = 1; //需要初始化
          logprintf("连接到服务器: %s\n", gt[ i ].addr);
        }
      }

      if (gt[ i ].sock != INVALID_SOCKET) {
        gt[ i ].fun = (LPTHREAD_START_ROUTINE) get_Error_Net_th;
      }
    }

    Sleep(1000);
  }
  else {

  }

  return 0;
}
//#define SVRPORT 4569
int get_Error_Net(BOOST* hh, WORKINFO* pwi)
{
  int AllFeatNum = pwi->end - pwi->beg;
  DWORD threadID;
  double SumTime = 0;
  HANDLE hThreads[ 256 ];
  int beginip = 1;
  int MAXIP = 20;
  int beg0 = pwi->beg;
  int i, j = 0, n = 0;
  int danji = 0;
  char ipfmt[256];
  utime_start(time);
  iniappname(ADABOOSTSECT);
  MAXIP = inigetint("最大IP");
  danji = inigetint("使用单机");
  strcpy(ipfmt, inigetstr("扫描IP段"));

  if (danji) {
    return get_Error(hh, pwi);
  }

  if (0 == gtinited) {
    __INITSOCKET22();
    gt[ 0 ].fun = (LPTHREAD_START_ROUTINE) get_Error_Local;
    gt[ 0 ].workinfo.Time = 1;
    gt[ 0 ].hh = hh;

    for (i = 1; i < MAXIP; ++i) {
      _snprintf(gt[ i ].addr, 32, ipfmt, beginip + i);
      gt[ i ].sock = INVALID_SOCKET;
      gt[ i ].fun = 0;
      gt[ i ].workinfo.Time = 1;
    }

    //hThreadsScan = CreateThread( NULL, 0, Scan_Net_th, ( LPVOID ) ( 0 ), 0, &threadID );
    gtinited = 1;
    Scan_Net_th(0, MAXIP);
  }

  {
    int ifscan;
    ifscan = inigetint("马上扫描");

    if (ifscan) {
      Scan_Net_th(0, MAXIP);
      inisetint("马上扫描", 0);
    }
  }

  if (n <= 1) {
    //logprintf( "没找到任何服务器!\n" );
  }

  for (i = 0, j = 0; i < MAXIP; ++i) {
    if (gt[ i ].fun != NULL) {
      if (gt[ i ].workinfo.Time > 0.f && gt[ i ].workinfo.end > gt[ i ].workinfo.beg) {
        gt[ i ].workinfo.Time = (gt[ i ].workinfo.end - gt[ i ].workinfo.beg) / gt[ i ].workinfo.Time;
        SumTime += gt[ i ].workinfo.Time;
      }
      else {
        gt[ i ].workinfo.Time = 1.f;
        SumTime += gt[ i ].workinfo.Time;
      }
    }
  }

  n = 0;

  for (i = 0, j = 0; i < MAXIP; ++i) {
    if (gt[ i ].fun != NULL) {
      gt[ i ].workinfo.beg = beg0;
      beg0 += (int)(((gt[ i ].workinfo.Time) / SumTime) * AllFeatNum);
      gt[ i ].workinfo.end = beg0;

      if (j == (n - 1)) {
        gt[ i ].workinfo.end = AllFeatNum;
      }

      gt[ i ].hh = hh;
      hThreads[ j ] = CreateThread(NULL, 0, gt[ i ].fun, (LPVOID)(gt + i), 0, &threadID);
      ++j;
      ++n;
    }
  }

  {
    DWORD dwIndex;
    int beg = pwi->beg;
    int end = pwi->end;
    dwIndex = WaitForMultipleObjects(n, hThreads, 1, INFINITE);
    pwi->Error = 10000.f;

    for (j = 0; j < n; ++j) {
      CloseHandle(hThreads[j]);
    }

    for (i = 0, j = 0; i < MAXIP; ++i) {
      if (gt[ i ].fun != NULL) {
        if (gt[ i ].sock == INVALID_SOCKET) {
          gt[ i ].need_init = 1;
          gt[ i ].fun = 0;
          printf("%70s\r", "");
          logprintf("帮%s计算！\n", gt[ i ].addr);
          get_Error(gt[ i ].hh, &(gt[ i ].workinfo));
        }

        if (pwi->Error > gt[ i ].workinfo.Error) {
          *pwi = gt[ i ].workinfo;
        }
      }
    }

    pwi->beg = beg;
    pwi->end = end;
    pwi->Time = utime_elapsed(time);
  }

  return pwi->id >= 0;
}
