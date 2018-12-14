//ECHO服务器端：
//#include "socket.h"
#include "stdc/thread.h"
//#include "threadpool.h"
//#include "cmath.h"
enum task_type {
  update_data,
  calc_dist,
};
typedef struct result_t {
  int mindis;
  int minid;
  double time;
} result_t;
typedef struct data_t {
  short feat[2000];
  int feat_id;
} data_t;
typedef struct task_t {
  int type;
  int begid, endid;
  int size;
  int maxsize;
  data_t data[1];
} task_t;
int rand_short(int n, short* data)
{
  int i;
  for (i = 0; i < n; ++i) {
    data[i] = rand() % 30;
  }
  return 0;
}
result_t findminid_impl(int n, const void* lib, int step, int i, const short* feats)
{
  int j, minid = -1, featlen = step / 2, mindis = 1000000000;
  char* libs = (char*)lib;
  result_t ret = { -1, -1};
  for (; i < n; ++i) {
    short* libsi = (short*)(libs + i * step);
    int ss = 0;
    for (j = 0; j < featlen; ++j) {
      ss += libsi[j] > feats[j] ? libsi[j] - feats[j] : feats[j] - libsi[j];
    }
    if (ss < mindis) {
      mindis = ss;
      minid = i;
    }
  }
  ret.mindis = mindis;
  ret.minid = minid;
  return ret;
}
result_t findminid(int n, const void* lib, int step, task_t* ta)
{
  int i;
  if (ta) {
    i = MAX(0, ta->begid);
    n = MIN(n, ta->endid);
  }
  return findminid_impl(n, lib, step, i, ta->data->feat);
}
typedef struct findminid_th_t {
  int n, i;
  const void* lib;
  int step;
  task_t* ta;
  result_t ret;
} findminid_th_t;
int WINAPI findminid_th(findminid_th_t* x)
{
  x->ret = findminid_impl(x->n, x->lib, x->step, x->i, x->ta->data->feat);
  //printf("findminid_th %d %d %d\n", x->i, x->n, x->ret.mindis);
  return 0;
}
#define MAXTH 8
result_t findminid_mth(int n, const void* lib, int step, task_t* ta, threadpool_t* p)
{
  result_t ret = { -1, -1};
  thread_t th[MAXTH] = {0};
  findminid_th_t pa[MAXTH] = {0};
  int i, ns = MAXTH;
  for (i = 0; i < ns; ++i) {
    int begid, endid;
    begid = (n + ns - 1) * i / ns;
    endid = (n + ns - 1) * (i + 1) / ns;
    endid = MIN(endid, n);
    pa[i].i = begid;
    pa[i].n = endid;
    pa[i].lib = lib;
    pa[i].step = step;
    pa[i].ta = ta;
  }
  threadpool_create(MAXTH, th, findminid_th, pa, sizeof(findminid_th_t), p);
  threadpool_join(MAXTH, th, -1, p);
  ret = pa[0].ret;
  for (i = 1; i < ns; ++i) {
    if (pa[i].ret.mindis < ret.mindis) {
      ret = pa[i].ret;
    }
  }
  return ret;
}
#define SSS "0123456789 abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define MAX_CLIENT (FD_SETSIZE -1)
int test_select_server(void* x)
{
  data_t* lib = 0;
  int nPort = *(int*)x;
  BOOL bInterface = FALSE;
  socket_t s[2] = {0};
  socket_t* sClient = s + 1;
  int nRet, nLen, i;
  socket_t arrClientSocket[MAX_CLIENT] = {INVALID_SOCKET};
  fd_set fdRead;
  SOCKADDR_IN saRemote;
  int maxsize = 0;
  bInterface = TRUE;
  net_init(2, 2);
  printf("S:socket_listening.....\n");
  if (!socket_listening(s, nPort)) {
    return 1;
  }
  printf("S:listen in port %d was successed\n", nPort);
  for (i = 0; i < MAX_CLIENT; i++) {
    arrClientSocket[i].s = INVALID_SOCKET;
  }
  for (;;) {
    //初始化队列，将所有套接字加入。
    FD_ZERO(&fdRead);
    FD_SET(s->s, &fdRead);
    for (i = 0; i < MAX_CLIENT; i++) {
      if (INVALID_SOCKET != arrClientSocket[i].s) {
        FD_SET(arrClientSocket[i].s, &fdRead);
      }
    }
    nRet = select(0, &fdRead, NULL, NULL, NULL);
    if (SOCKET_ERROR == nRet) {
      printf("S:Select() error:%d\n", WSAGetLastError());
      break;
    }
    if (nRet > 0) {
      //说明有新SOCKET连接到服务器
      if (FD_ISSET(s->s, &fdRead)) {
        nLen = sizeof(saRemote);
        socket_accept(s, (LPSOCKADDR)&saRemote, &nLen, sClient);
        for (i = 0; i < MAX_CLIENT; i++) {
          if (INVALID_SOCKET == arrClientSocket[i].s) {
            arrClientSocket[i] = *sClient;
            break;
          }
        }
        printf("S:Sokcet:%d, was accept()\n", sClient->s);
      }
      else {
        int asdf = 0;
      }
      for (i = 0; i < MAX_CLIENT; i++) {
        if (FD_ISSET(arrClientSocket[i].s, &fdRead)) {
          result_t ret = {0};
          task_t ta[1] = {0};
          nRet = socket_safe_recv(arrClientSocket + i, ta, sizeof(task_t), 0);
          switch (ta->type) {
          case update_data:
            if (maxsize < ta->maxsize) {
              maxsize = ta->maxsize;
              MYREALLOC(lib, maxsize);
            }
            if (ta->begid < maxsize) {
              lib[ta->begid] = ta->data[0];
            }
            //printf("S:%d 收到%d\n", nPort, ta->begid);
            break;
          case calc_dist:
            if (nRet <= 0) {
              socket_close(arrClientSocket + i);
              continue;
            }
            ret = findminid(maxsize, lib, sizeof(data_t), ta);
            //printf("S:port:%d, %d %d\n", nPort, ret.minid, ret.mindis);
            socket_safe_send(arrClientSocket + i, &ret, sizeof(result_t), 0);
            break;
          }
        }
      }
    }
  }
  socket_close(s);
  net_exit();
  return 0;
}
#define BASEPORT 10000
int socket_connect_str(socket_t* s, const char* ip, int port)
{
  SOCKADDR_IN sa;
  int nAddrLen;
  if (!socket_open(s, AF_INET, SOCK_STREAM, IPPROTO_TCP)) {
    printf("C:Socket was created failse:%d\n", WSAGetLastError());
    socket_close(s);
    return 0;
  }
  nAddrLen = sizeof(SOCKADDR_IN);
  memset(&sa, 0, nAddrLen);
  sa.sin_family = AF_INET;
  sa.sin_port = htons(port);
  sa.sin_addr.s_addr = inet_addr(ip);
  if (!socket_connect(s, (LPSOCKADDR)&sa, nAddrLen)) {
    printf("C:socket connect(%s, %d) failse:%d\n", ip, port, WSAGetLastError());
    socket_close(s);
    return 0;
  }
  return 1;
}
int socket_send_all(int n, socket_t* s, const void* buf, int len, int flags, int step)
{
  int i, ret;
  const char* cc = (const char*)buf;
  for (i = 0; i < n; ++i) {
    ret = socket_safe_send(s + i, cc + i * step, len, flags);
  }
  return ret;
}
int socket_recv_all(int n, socket_t* s, void* buf, int len, int flags, int step)
{
  int i, ret;
  char* cc = (char*)buf;
  for (i = 0; i < n; ++i) {
    ret = socket_safe_recv(s + i, cc + i * step, len, flags);
  }
  return ret;
}
#define MAXNS 4
#define MAX_SIZE (20*1000)
// 测试用客户端，目前地址写在了程序中，用户可以自行修改。
int test_select_client(void* x)
{
  data_t* lib;
  int nRet;
  socket_t s[MAXNS] = {0};
  result_t rets[MAXNS] = {0};
  int ns = 0, i;
  int port = 0;
  char buf[256];
  threadpool_t pool[1] = {0};
  lib = MALLOC(data_t, MAX_SIZE);
  net_init(2, 2);
  if (x) {
    for (port = BASEPORT; port < (BASEPORT + MAXNS); ++port) {
      socket_t* s1 = s + ns;
      _snprintf(buf, 256, "192.168.1.3");
      if (socket_connect_str(s + ns, buf, port)) {
        ++ns;
      }
    }
  }
  rand_short(MAX_SIZE * sizeof(data_t) / sizeof(short), lib->feat);
  for (i = 0; i < MAX_SIZE; ++i) {
    task_t ta[1];
    ta->type = update_data;
    ta->begid = i;
    ta->maxsize = MAX_SIZE;
    lib[i].feat_id = 123;
    *ta->data = lib[i];
    nRet = socket_send_all(ns, s, ta, sizeof(task_t), 0, 0);
  }
  for (;;) {
    task_t ta[1];
    result_t ret0 = { -1, -1}, ret1 = { -1, -1}, ret2 = { -1, -1};
    logprintf("============\n");
    rand_short(sizeof(data_t) / 2, ta->data->feat);
    if (ns > 0) {
      utime_start(_start_time);
      ta->type = calc_dist;
      for (i = 0; i < ns; ++i) {
        ta->begid = (MAX_SIZE + ns - 1) * i / ns;
        ta->endid = (MAX_SIZE + ns - 1) * (i + 1) / ns;
        ta->endid = MIN(ta->endid, MAX_SIZE);
        nRet = socket_safe_send(s + i, ta, sizeof(task_t), 0);
      }
      if (SOCKET_ERROR == nRet) {
        printf("C:Socket send() failse:%d\n", WSAGetLastError());
        break;
      }
      if (1) {
        nRet = socket_recv_all(ns, s, rets, sizeof(result_t), 0, sizeof(result_t));
        ret0 = rets[0];
        for (i = 1; i < ns; ++i) {
          if (rets[i].mindis < ret0.mindis) {
            ret0 = rets[i];
          }
        }
      }
      ret0.time = utime_elapsed(_start_time);
    }
    if (1) {
      utime_start(_start_time);
      ta->begid = 0;
      ta->endid = MAX_SIZE;
      ret1 = findminid(MAX_SIZE, lib, sizeof(data_t), ta);
      ret1.time = utime_elapsed(_start_time);
    }
    if (0) {
      utime_start(_start_time);
      ta->begid = 0;
      ta->endid = MAX_SIZE;
      ret2 = findminid_mth(MAX_SIZE, lib, sizeof(data_t), ta, 0);
      ret2.time = utime_elapsed(_start_time);
    }
    if (1) {
      utime_start(_start_time);
      ta->begid = 0;
      ta->endid = MAX_SIZE;
      ret2 = findminid_mth(MAX_SIZE, lib, sizeof(data_t), ta, pool);
      ret2.time = utime_elapsed(_start_time);
    }
    logprintf("图片数%d\n", MAX_SIZE);
    if (ns > 0) {
      logprintf("C: %d服务器 ret0 %d, %d %f\n", ns, ret0.minid, ret0.mindis, ret0.time);
    }
    logprintf("C: 单机 ret1 %d, %d %f\n", ret1.minid, ret1.mindis, ret1.time);
    logprintf("C: 多线程 ret2 %d, %d %f\n", ret2.minid, ret2.mindis, ret2.time);
    Sleep(500);
  }
  socket_close(s);
  net_exit();
  free(lib);
  return 0;
}
int test_select()
{
  thread_t th[MAXNS] = {0};
  int i, port[MAXNS] = {0};
  for (i = 0; i < MAXNS; ++i) {
    port[i] = BASEPORT + i;
  }
  //thread_create(MAXNS, th, test_select_server, port, sizeof(int));
  test_select_client(0);
  return 0;
}

