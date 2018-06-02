
#include "macro.h"
#include "imdilate.inl"
#include "depthIrepair.inl"
#include "Rainclusteredit.inl"
#include "LayerAreaFilter.inl"
#include "TrackingAndCaculate.inl"
#include "..\net\chcsock\chcsock.inl"
#include "libheadcounts.h"

// buflen=6*h*w
int FrontAreaSeg(int h, int w, uchar* BI, int BI_step, int ImerodeThresh, int ImdilateThresh, int AreaThresh,
    unsigned char* FrontI, int FrontI_step, unsigned char* buf, int buflen)
{
  BUFUSEBEGIN(buf, buflen);
  unsigned char* img1 = (unsigned char*)BUFMALLOC(unsigned char, h * w);
  unsigned char* img3 = (unsigned char*)BUFMALLOC(unsigned char, h * w);
  int* label = (int*)BUFMALLOC(int, h * w);
  int areas[1000];
  unsigned char map[1000];
  memset(map, 255, sizeof(map));
  map[0] = 0;
  memset(img1, 0, h * w);
  memset(img3, 0, h * w);

  BUFBOUNDCHECK();
  imdilate_rect(h, w, BI, BI_step, img1, w, ImerodeThresh, ImerodeThresh, ImerodeThresh / 2, ImerodeThresh / 2, 0);

  //memcpy(img1, img, h*w);
  bwlabel(h, w, img1, w, label, w, 4, areas, 1000, AreaThresh, map, img3, w);
  imdilate_rect(h, w, img3, w, FrontI, FrontI_step, ImdilateThresh, ImdilateThresh, ImdilateThresh / 2, ImdilateThresh / 2, 1);

  //free(img1);
  //free(img3);
  //free(label);
  BUFUSEEND();
  return 0;
}

int maxv = 5000 / 250;
int cvShowImage_u16(const char* name, int h, int w, const unsigned short* A, int step, int cn)
{
  unsigned char* tmp = (unsigned char*)malloc(h * w * cn);
  int i, j, tmpstep = w * cn;
  maxv = 0;

  for (i = 0; i < h; ++i) {
    for (j = 0; j < w; ++j) {
      if (A[i * step + j] > maxv) {
        maxv = A[i * step + j];
      }
    }
  }

  //if (0==maxv) return 0;
  ++maxv;

  for (i = 0; i < h; ++i) {
    for (j = 0; j < w; ++j) {
      tmp[i * tmpstep + j] = A[i * step + j] * 255 / maxv;
    }
  }

  cvShowImage(name, h, w, tmp, tmpstep, cn);
  free(tmp);
  return 0;
}

int ptid_findid(ptid_t* p, int n, int id)
{
  int i, j;

  for (i = 0; i < n; ++i) {
    for (j = 0; j < p[i].ptn; ++j) {
      if (id == p[i].id[j]) {
        return i;
      }
    }
  }

  return -1;
}

int headcounts_setptr(headcounts_t* ph)
{
  int h = ph->h, w = ph->w;
  BUFUSEBEGIN(ph->buf, ph->buflen);
  unsigned char* FrontI = (unsigned char*)BUFMALLOC(unsigned char, h * w);
  unsigned char* FAI = (unsigned char*)BUFMALLOC(unsigned char, h * w);
  unsigned short* RDI = (unsigned short*)BUFMALLOC(unsigned char, 2 * h * w);
  unsigned short* FRDI = (unsigned short*)BUFMALLOC(unsigned char, 2 * h * w);
  unsigned short* g = (unsigned short*)BUFMALLOC(unsigned char, 2 * h * w);
  ph->FrontI = FrontI;
  ph->FAI = FAI;
  ph->RDI = RDI;
  ph->FRDI = FRDI;
  ph->g = g;
  BUFUSEEND();
  return 0;
}
int headcounts_init(headcounts_t* ph)
{
  int h = ph->h, w = ph->w;
  ph->buflen = (8 + 12) * h * w;
  ph->buf = (unsigned char*)malloc(ph->buflen);
  memset(ph->buf, 0, ph->buflen);
  ph->idmax = 0;
  memset(ph->pi, 0, sizeof(ptid_t)*COUNTOF_PI);
  headcounts_setptr(ph);
  return 0;
}

int se_find(headcounts_t* ph, int id)
{
  int i;

  for (i = 0; i < MAXSEID; ++i) {
    if (id == ph->se[i].id) {
      return i;
    }
  }

  for (i = 0; i < MAXSEID; ++i) {
    if (0 == ph->se[i].id) {
      return i;
    }
  }

  return -1;
}

int multiply1(POINT sp, POINT ep, POINT op)
{
  return ((sp.x - op.x) * (ep.y - op.y) - (ep.x - op.x) * (sp.y - op.y));
}

int test_multiply1()
{
  POINT a = {50, 50};
  POINT b = {100, 0};
  POINT c = {0, 0};
  POINT d = {50, -50};
  int t1 = multiply1(a, b, c);
  int t2 = multiply1(d, b, c);
  return 0;
}

// BI 前景二值图
// DI 深度图
// CI 彩色图
int headcounts(headcounts_t* ph)
{
  int h = ph->h, w = ph->w;
  int h2 = ph->h, w2 = ph->w;
  unsigned char* buf = ph->buf;
  BUFUSEBEGIN(ph->buf, ph->buflen);
  ptid_t* pi = ph->pi;
  //unsigned char* BI=pBGStatModel->foreground;
  unsigned char* BI = ph->BI;
  unsigned short* depth_data = ph->depth;
  unsigned char* FrontI = (unsigned char*)BUFMALLOC(unsigned char, h * w);
  unsigned char* FAI = (unsigned char*)BUFMALLOC(unsigned char, h * w);
  unsigned short* RDI = (unsigned short*)BUFMALLOC(unsigned char, 2 * h * w);
  unsigned short* FRDI = (unsigned short*)BUFMALLOC(unsigned char, 2 * h * w);
  unsigned short* g = (unsigned short*)BUFMALLOC(unsigned char, 2 * h * w);
  int i, j;

  for (i = 0; i < h2; ++i) {
    for (j = 0; j < w2; ++j) {
      int i2 = i / 2, j2 = j / 2;
      //BI[i*w2+j]=BI[2*i2*w2+j2*2];
    }
  }

  //memset(RDI, 0, h*w);
  //{utime_start(_start_time);
  depthIrepair(h2, w2, depth_data, w2, ph->BRsize, RDI, w2);
  FrontAreaSeg(h2, w2, BI, w2, ph->ImerodeThresh, ph->ImdilateThresh, ph->AreaThresh, FrontI, w2, GETBUFBEG(), GETBUFLEN());

  for (i = 0; i < h2 * w2; ++i) {
    FRDI[i] = FrontI[i] ? RDI[i] : 0;
  }

  Rainclusteredit(h2, w2, FRDI, w2, ph->RN, ph->RD, ph->beishu, g, w2, GETBUFBEG(), GETBUFLEN());

  ph->lables = LayerAreaFilter(h2, w2, g, w2, ph->FilterLayerThresh, ph->FilterAreaThresh, ph->verson, FAI, w2, pi[0].pt, MAXOBJ, ph->Order_verson, GETBUFBEG(), GETBUFLEN());
  //printf(" %.3f ", utime_elapsed(_start_time));}
  pi[0].ptn = ph->lables;
  {
    int m = 3;
    int wl = w2 / m, wh = w2 * (m - 1) / m;
    int hl = h2 / m, hh = h2 * (m - 1) / m;
    int n2 = ph->lables;
    int i, nMatched, k = pi[1].ptn < pi[2].ptn ? 2 : 1;

    for (j = 0; j < n2; ++j) {
      pi[0].id[j] = 0;
    }

    for (i = 1; i < COUNTOF_PI; ++i) {
      //ptid_t tp=pi[i];
      //nMatched = ptMatch( tp.ptn, tp.pt, tp.id, pi[0].ptn, pi[0].pt, pi[0].id);
      nMatched = ptMatch(pi[i].ptn, pi[i].pt, pi[i].id, pi[0].ptn, pi[0].pt, pi[0].id);
    }

    for (i = 0; i < n2; ++i) {
      if (0 == pi[0].id[i]) {
        int x = pi[0].pt[i].x, y = pi[0].pt[i].y;

        if (y < hl) {
          ph->topIn++;
        }

        if (y > hh) {
          ph->botIn++;
        }

        // 目标出现
        if (y < hl || y > hh) {
          ++ph->idmax;
          pi[0].id[i] = ph->idmax;
          j = se_find(ph, ph->idmax);
          ASSERT(j >= 0);
          ph->se[j].s = pi[0].pt[i];
          ph->se[j].id = ph->idmax;
          ph->se[j].cnted_top = 0;
          ph->se[j].cnted_bot = 0;
          ph->se[j].ptlen = 0;
          ph->se[j].pt[ph->se[j].ptlen].x = pi[0].pt[i].x;
          ph->se[j].pt[ph->se[j].ptlen].y = pi[0].pt[i].y;
          ph->se[j].ptlen++;
        }
      }
    }

    for (i = 0; i < pi[0].ptn; ++i) {
      for (j = 0; j < MAXSEID; ++j) {
        if (ph->se[j].id > 0 && ph->se[j].id == pi[0].id[i] && ph->se[j].ptlen < MAXPT) {
          // 添加坐标序列
          ph->se[j].pt[ph->se[j].ptlen].x = pi[0].pt[i].x;
          ph->se[j].pt[ph->se[j].ptlen].y = pi[0].pt[i].y;
          ph->se[j].ptlen++;

          if (ph->pt1.x > 0) {
            int t1 = multiply1(ph->se[j].pt[ph->se[j].ptlen - 1], ph->pt2, ph->pt1);
            int t2 = multiply1(ph->se[j].pt[ph->se[j].ptlen - 2], ph->pt2, ph->pt1);

            //printf("x=%d y=%d\n", ph->pt1.x, ph->pt1.y);
            //printf("t1=%d t2=%d\n", t1, t2);
            if (t1 * t2 < 0) {
              if (t1 < 0) {
                ph->ct_t1++;
              }
              else {
                ph->ct_t2++;
              }
            }
          }

          // 统计超过n帧的目标
          if (ph->se[j].ptlen > 1) {
            //上面进来
            if (ph->se[j].s.y < hl && !ph->se[j].cnted_top) {
              ph->se[j].cnted_top = 1;
              ph->top2bot++;
            }

            // 下面进来
            if (ph->se[j].s.y > hh && !ph->se[j].cnted_bot) {
              //ph->se[j].cnted_bot=1;
              //ph->bot2top++;
            }
          }
        }
      }
    }

    for (i = 0; i < pi[0].ptn;) {
      if (0 == pi[0].id[i]) {
        pi[0].ptn--;
        memcpy(pi[0].pt + i, pi[0].pt + i + 1, (pi[0].ptn - i)*sizeof(pi[0].pt[0]));
        memcpy(pi[0].id + i, pi[0].id + i + 1, (pi[0].ptn - i)*sizeof(pi[0].id[0]));
      }
      else {
        ++i;
      }
    }

    // 统计出去的目标，不包括前面(超过30帧)已经统计过的
    for (i = 0; i < pi[COUNTOF_PI - 1].ptn; ++i) {
      if (pi[COUNTOF_PI - 1].id[i] > 0) {
        int x = pi[COUNTOF_PI - 1].pt[i].x, y = pi[COUNTOF_PI - 1].pt[i].y;
        j = se_find(ph, pi[COUNTOF_PI - 1].id[i]);
        ASSERT(j >= 0);

        // 下面进来，上面出去
        if (y < hl && !ph->se[j].cnted_bot) {
          ph->topOut++;

          if (ph->se[j].s.y > hh || ph->se[j].cnted_top) {
            ph->se[j].e = pi[COUNTOF_PI - 1].pt[i];
            ph->bot2top++;
          }
        }

        // 上面进来，下面出去
        if (y > hh && !ph->se[j].cnted_top) {
          ph->botOut++;

          if (ph->se[j].s.y < hl || ph->se[j].cnted_bot) {
            ph->se[j].e = pi[COUNTOF_PI - 1].pt[i];
            ph->top2bot++;
          }
        }

        ph->se[j].id = 0;
      }
    }

    for (i = COUNTOF_PI - 1; i > 0; --i) {
      pi[i] = pi[i - 1];
    }
  }
  ph->FrontI = FrontI;
  ph->FAI = FAI;
  ph->RDI = RDI;
  ph->FRDI = FRDI;
  ph->g = g;
  BUFUSEEND();
  return 0;
}


int ClientProc_cmd(SOCKET_T svi, int cmd, headcounts_t* ph0,
    unsigned char* BGRImg, unsigned char* DepthImg)
{
  int ret;

  if (INVALID_SOCKET == svi) {
    return -1;
  }

  ret = CPM_SendT(svi, &cmd, sizeof(int), 0, 0);

  //printf("cmd=%d\n", cmd);
  if (cmd) {
    headcounts_t ttt = {0};
    ret = CPM_RecvT(svi, &ttt, sizeof(headcounts_t), 0, 1000);

    if (ret < 0) {
      printf("2 %d: %s\n", ret, WSAGetLastErrMsg());
      return ret;
    }

#define GETV(a, b)   ph0->a=ttt.a
    GETV(top2bot, 1);
    GETV(bot2top, 1);
    GETV(topIn, 1);
    GETV(topOut, 1);
    GETV(botIn, 1);
    GETV(botOut, 1);
    GETV(cnt_t1, 1);
    GETV(cnt_t2, 1);
#undef GETV
    ret = CPM_SendT(svi, ph0, sizeof(headcounts_t), 0, 0);

    if (0) {
      ttt.buf = (unsigned char*)malloc(ttt.buflen);
      ret = CPM_RecvT(svi, ttt.buf, ttt.buflen, 0, 10000);

      if (ret < 0) {
        printf("3 %d: %s\n", ret, WSAGetLastErrMsg());
        return ret;
      }
    }

    if (cmd & NET_HEADCOUNTS_GETCOLOR) {
      ret = CPM_RecvT(svi, BGRImg, _WW * _HH * 3, 0, 10000);

      if (ret < 0) {
        printf("4 %d: %s\n", ret, WSAGetLastErrMsg());
        return ret;
      }
    }

    if (cmd & NET_HEADCOUNTS_GETDEPTH) {
      ret = CPM_SendT(svi, &ret, sizeof(int), 0, 0);
      ret = CPM_RecvT(svi, DepthImg, _WW * _HH, 0, 10000);

      if (ret < 0) {
        printf("5 %d: %s\n", ret, WSAGetLastErrMsg());
        return ret;
      }
    }
  }

  return ret;
}

int ClientProc_end(SOCKET_T svi)
{
  if (INVALID_SOCKET != svi) {
    closesocket(svi);
  }

  return INVALID_SOCKET;
}
SOCKET_T ClientProc_beg(const char* ipaddrstr)
{
  SOCKET svi = 0;
  int ret;
  int g_svLen = ScanPortList(1, &ipaddrstr, 10, &svi);
  {
    if (INVALID_SOCKET != svi) {
      sockaddr_in addrRemote;
      int nAddrLen = sizeof(addrRemote);
      CPM_GetSockName(svi, (SOCKADDR*) &addrRemote, &nAddrLen);
      printf("StartJob: %s:%d\n", inet_ntoa(addrRemote.sin_addr), ntohs(addrRemote.sin_port));
      ret = StartJob(svi, SERVERPROCID);

      if (0 == ret) {
        printf("没找到服务!\n");
        closesocket(svi);
        svi = INVALID_SOCKET;
      }
      else {
        printf("找到服务!\n");
      }
    }
  }
  return svi;
}
