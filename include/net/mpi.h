
#ifndef __MPI_H__
#define __MPI_H__
#include "chcsock.inl"
#include "memexec.h"
#define MPI_SERVER_JOB_ID 112
#define MPI_SERVER_PORT  (4455)
#define MPI_WORK_PORT  (4477)
#define RecvVec(s, pBuf, nSize, flags, ms, ret) {ret = CPM_Recv(sClient, &nSize, sizeof(int), 0, ms); \
    if (ret>0) {pBuf=(char*)malloc(nSize); ret = CPM_Recv(sClient, pBuf, nSize, 0, ms);} }

typedef struct MPI_CMD_info {
  char* exebuf;
  int exelen;
  int thdnum;
  char cmd[32];
} MPI_CMD_info;

typedef struct MPI_Comm_Info {
  int rank;
  int size;
} MPI_Comm_Info;

int MPI_Server_fun(SOCKET sClient, void* userdata)
{
  int ret = 0;
  int ms = -1;
  unsigned long ulProcessId = 0;
  MPI_CMD_info info = {0};
  ret = CPM_Recv(sClient, &info, sizeof(info), 0, ms);

  if (ret > 0) {
    info.exebuf = (char*)malloc(info.exelen);
    ret = CPM_Recv(sClient, info.exebuf, info.exelen, 0, ms);
  }

  if (ret > 0) {
    ret = CPM_Send(sClient, &info, sizeof(info), 0, 0);
  }

  if (ret > 0) {
    int i;
    //savefile("aaa.exe", info.exebuf, info.exelen);
    printf("exelen=%d %s\n", info.exelen, info.cmd);

    for (i = 0; i < info.thdnum; ++i) {
      //test_memexec();
      MemExecute(info.exebuf, info.exelen, info.cmd, &ulProcessId);
    }
  }

  if (info.exebuf) {
    free(info.exebuf);
  }

  return ret;
}
int MPI_Server()
{
  int nPort = MPI_SERVER_PORT; // nPort 此服务器监听的端口号
  SOCKET sListen;
  // 注册服务
  joblist_op(MPI_SERVER_JOB_ID, MPI_Server_fun);
  sListen = CPM_StartServer(nPort);

  if (INVALID_SOCKET == sListen) {
    printf("MPI服务开启失败。port=%d\n", nPort);
    return 0;
  }

  printf("MPI服务开启了。port=%d\n", nPort);
  StartSelect(sListen, 0);
  return 0;
}

/* Communicators */
typedef int MPI_Comm;
#define MPI_COMM_WORLD ((MPI_Comm)0x44000000)
#define MPI_COMM_SELF  ((MPI_Comm)0x44000001)

/* Groups */
typedef int MPI_Group;
#define MPI_GROUP_EMPTY ((MPI_Group)0x48000000)


typedef int MPI_Win;
#define MPI_WIN_NULL ((MPI_Win)0x20000000)

#define IPLISTFILE "iplist.txt"
typedef struct Net_t {
  char addr[ 32 ];
  SOCKET sock;
}
Net_t;
MPI_Comm_Info g_comm_info;
Net_t g_net_t[100] = {0};
int g_net_len = 0;
Net_t* findip(const char* ip)
{
  int i;

  for (i = 1; i < 100; ++i) {
    if (strstr(g_net_t[i].addr, ip)) {
      return g_net_t + i;
    }
  }

  return 0;
}
Net_t* puship(const char* ip)
{
  int i = g_net_len;
  strcpy(g_net_t[g_net_len].addr, ip);
  g_net_len++;
  return g_net_t + i;
}
Net_t* popip()
{
  --g_net_len;
  return g_net_t + g_net_len;
}
int MPI_Connect()
{
  int ret = 0;
  CPM_SOCKET s;
  struct sockaddr_in clientAddr;
  int nAddrLen = sizeof(struct sockaddr_in);
  int i = g_net_len;

  if (g_net_t[0].sock == 0 || g_net_t[0].sock == INVALID_SOCKET) {
    g_net_t[0].sock = CPM_StartServer(MPI_WORK_PORT);
  }

  s = g_net_t[0].sock;

  if (s == INVALID_SOCKET) {
    return 0;
  }

  if (g_net_t[i].sock == 0 || g_net_t[i].sock == INVALID_SOCKET) {
    char* ipstr;
    ret = CPM_LISTEN(s, 1);
    IF_ERR(ret, "监听失败!\n");
    g_net_t[i].sock = CPM_ACCEPT(s, clientAddr, nAddrLen);

    if (g_net_t[i].sock != INVALID_SOCKET) {
      CPM_GETPEERNAME(g_net_t[i].sock, clientAddr, nAddrLen);
      ipstr = inet_ntoa(clientAddr.sin_addr);
      strcpy(g_net_t[i].addr, ipstr);
      printf("收到连接: %s\n", ipstr);

      if (ret > 0) {
        MPI_Comm_Info comm_info;
        comm_info.rank = i;
        comm_info.size = g_net_len;
        //ret = CPM_Send(g_net_t[i].sock, &comm_info, sizeof(comm_info), 0, 0);
      }
    }
  }

  return ret;
}
int Scan_Net(const char* exepath)
{
  DWORD dwTimeOut = 10;
  char buf[256];
  char buf2[256];
  int ret, thdnum;
  MPI_CMD_info info = {0};
  char* hostip;
  __INITSOCKET22();
  hostip = gethostip();
  puship(hostip);
  _snprintf(info.cmd, 32, "svrip=%s:%d", hostip, MPI_WORK_PORT);
  //while(1)
  {
    FILE* pf = fopen(IPLISTFILE, "rb");

    if (pf != NULL) {
      info.exebuf = filemap(exepath, &info.exelen);

      for (; fgets(buf, 256, pf) > 0;) {
        strtrim(buf, " \t\r\n");
        sscanf(buf, "%d;%s", &thdnum, buf);

        if (IsIpString(buf) && NULL == findip(buf)) {
          SOCKET sock;
          dwTimeOut = 2000;
          _snprintf(buf2, 256, "%s:%d", buf, MPI_SERVER_PORT);
          printf("扫描: %s\n", buf2);
          sock = ScanConnect(buf2, dwTimeOut);

          if (sock != INVALID_SOCKET) {
            ret = StartJob(sock, MPI_SERVER_JOB_ID);
            info.thdnum = thdnum;

            if (info.thdnum < 1) {
              info.thdnum = 1;
            }

            if (ret > 0) {
              ret = CPM_Send(sock, &info, sizeof(info), 0, 0);
            }

            if (ret > 0) {
              ret = CPM_Send(sock, info.exebuf, info.exelen, 0, 0);
            }

            if (ret > 0) {
              MPI_CMD_info info2;
              ret = CPM_Recv(sock, &info2, sizeof(info2), 0, 3000);
            }

            if (ret > 0) {
              int i;
              closesocket(sock);

              for (i = 0; i < info.thdnum; ++i) {
                if (MPI_Connect()) {
                  puship(buf);
                  //printf( "连接到服务器: %s\n", buf );
                }
              }
            }
            else {
              //printf("没找到服务!\n");
              closesocket(sock);
            }
          }
        }
      }

      free(info.exebuf);
      fclose(pf);
    }
  }

  if (0 && g_net_len > 1) {
    CPM_SOCKET s;
    int nClientNum = g_net_len;
    struct sockaddr_in clientAddr;
    int nAddrLen = sizeof(struct sockaddr_in), i;
    g_net_t[0].sock = CPM_StartServer(MPI_WORK_PORT);
    s = g_net_t[0].sock;
    ret = CPM_LISTEN(s, nClientNum - 1);
    printf("开始监听\n");
    IF_ERR(ret, "监听失败!\n");

    for (i = 1; i < nClientNum; ++i) {
      char* ipstr;
      g_net_t[i].sock = CPM_ACCEPT(s, clientAddr, nAddrLen);

      if (g_net_t[i].sock != INVALID_SOCKET) {
        CPM_GETPEERNAME(g_net_t[i].sock, clientAddr, nAddrLen);
        ipstr = inet_ntoa(clientAddr.sin_addr);
        strcpy(g_net_t[i].addr, ipstr);
        printf("收到连接: %s\n", ipstr);

        if (ret > 0) {
          MPI_Comm_Info comm_info;
          comm_info.rank = i;
          comm_info.size = g_net_len;
          ret = CPM_Send(g_net_t[i].sock, &comm_info, sizeof(comm_info), 0, 0);
        }
      }
    }
  }
  else {
    int i;

    for (i = 1; i < g_net_len; ++i) {
      MPI_Comm_Info comm_info;
      comm_info.rank = i;
      comm_info.size = g_net_len;
      ret = CPM_Send(g_net_t[i].sock, &comm_info, sizeof(comm_info), 0, 0);
    }
  }

  g_comm_info.rank = 0;
  g_comm_info.size = g_net_len;
  return ret;
}
typedef int MPI_Comm;
#define MPI_COMM_WORLD ((MPI_Comm)0x44000000)
#define MPI_COMM_SELF  ((MPI_Comm)0x44000001)

typedef int MPI_Datatype;
#define MPI_CHAR           ((MPI_Datatype)0x4c000101)
#define MPI_SIGNED_CHAR    ((MPI_Datatype)0x4c000118)
#define MPI_UNSIGNED_CHAR  ((MPI_Datatype)0x4c000102)
#define MPI_BYTE           ((MPI_Datatype)0x4c00010d)
#define MPI_WCHAR          ((MPI_Datatype)0x4c00020e)
#define MPI_SHORT          ((MPI_Datatype)0x4c000203)
#define MPI_UNSIGNED_SHORT ((MPI_Datatype)0x4c000204)
#define MPI_INT            ((MPI_Datatype)0x4c000405)
#define MPI_UNSIGNED       ((MPI_Datatype)0x4c000406)
#define MPI_LONG           ((MPI_Datatype)0x4c000407)
#define MPI_UNSIGNED_LONG  ((MPI_Datatype)0x4c000408)
#define MPI_FLOAT          ((MPI_Datatype)0x4c00040a)
#define MPI_DOUBLE         ((MPI_Datatype)0x4c00080b)
#define MPI_LONG_DOUBLE    ((MPI_Datatype)0x4c00080c)
#define MPI_LONG_LONG_INT  ((MPI_Datatype)0x4c000809)
#define MPI_UNSIGNED_LONG_LONG ((MPI_Datatype)0x4c000819)
#define MPI_LONG_LONG      MPI_LONG_LONG_INT

#define MPI_PACKED         ((MPI_Datatype)0x4c00010f)
#define MPI_LB             ((MPI_Datatype)0x4c000010)
#define MPI_UB             ((MPI_Datatype)0x4c000011)

#define MPIDATASIZE(DATATYPE)   ((DATATYPE&0x00000f00)>>8)

typedef struct MPI_Status {
  int count;
  int cancelled;
  int MPI_SOURCE;
  int MPI_TAG;
  int MPI_ERROR;
} MPI_Status;

int MPI_Send(void* buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm)
{
  int ret, datasize = MPIDATASIZE(datatype);
  ret = CPM_Send(g_net_t[dest].sock, buf, count * datasize, 0, 0);
  return ret;
}
#define MPI_RECV(buf, len, type, src, tag, comm)  ret=MPI_Recv(buf, len, type, src, tag, comm, &status ); if (ret<=0) { printf("MPI_Recv error on file: %s line: %d\n", __FILE__, __LINE__); }
int MPI_Recv(void* buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status* status)
{
  int ret, datasize = MPIDATASIZE(datatype);
  ret = CPM_Recv(g_net_t[source].sock, buf, count * datasize, 0, 0);
  return ret;
}
int MPI_SendFile(const char* filename, int dest, int tag, MPI_Comm comm)
{
  int ret;
  int64 fsz2 = 0, fsz = 0;
  {
    if (!file_exist(filename)) {
      return -1;
    }

    fsz = filesize(filename);
    CPM_Send(g_net_t[dest].sock, &fsz, sizeof(fsz), 0, 0);
    CPM_Recv(g_net_t[dest].sock, &fsz2, sizeof(fsz), 0, 0);
  }

  if (fsz != fsz2) {
    HD_FILE hf = FOPEN(filename, FILE_READ);

    if (INVALID(hf)) {
      return 0;
    }

    ret = CPM_SendFile(g_net_t[dest].sock, hf, 0, 0);
    FCLOSE(hf);
  }
  else {
    printf("%s 文件已存在,size=%ld!\n", filename, fsz);
  }

  return ret;
}
int MPI_RecvFile(const char* filename, int source, int tag, MPI_Comm comm)
{
  int ret;
  int64 fsz2 = 0, fsz = 0;
  {
    if (file_exist(filename)) {
      fsz = filesize(filename);
    }

    CPM_Recv(g_net_t[source].sock, &fsz2, sizeof(fsz), 0, 0);
    CPM_Send(g_net_t[source].sock, &fsz, sizeof(fsz), 0, 0);
  }

  if (fsz != fsz2) {
    HD_FILE hf = FOPEN(filename, FILE_WRITE);

    if (INVALID(hf)) {
      return 0;
    }

    ret = CPM_RecvFile(g_net_t[source].sock, hf, 0, 0);
    FCLOSE(hf);
  }
  else {
    printf("%s 文件已存在,size=%ld!\n", filename, fsz);
  }

  return ret;
}
char* getfilename(const char* fn)
{
  char* p = strrchr(fn, '\\');
  p = p ? p : strrchr(fn, '/');
  return p ? (p + 1) : fn;
}
int MPI_BcastFile(const char* filename, int root, MPI_Comm comm)
{
  int i, ret = 1;

  if (g_comm_info.rank == root) {
    for (i = 0; i < g_net_len; i++) {
      if (i != root) {
        ret = MPI_SendFile(filename, i, 0, MPI_COMM_WORLD);
      }
    }
  }
  else {
    char* fn = getfilename(filename);
    ret = MPI_RecvFile(fn, root, 0, MPI_COMM_WORLD);
  }

  return ret;
}
int MPI_Bcast(void* buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm)
{
  int i, ret = 1;
  int ms = 0;

  if (g_comm_info.rank == root) {
    for (i = 0; i < g_net_len; ++i) {
      if (i != root) {
        ret = MPI_Send(buffer, count, datatype, i, 0, comm);
      }
    }
  }
  else {
    ret = MPI_Recv(buffer, count, datatype, root, 0, comm, 0);
  }

  return ret;
}

int g_argc = 0;
char** g_argv = 0;

int MPI_Init0(int* pargc, char** *pargv)
{
  g_argc = *pargc;
  g_argv = *pargv;
  return 0;
}

int MPI_Init(int* pargc, char** *pargv)
{
  int i, argc = *pargc;
  char** argv = *pargv;
  int is_rank0 = 1;
  char buf[256];

  for (i = 0; i < argc; ++i) {
    if (strstr(argv[i], "svrip=")) {
      sscanf(argv[i], "svrip=%s", buf);
      //printf("%s\n", buf);
      is_rank0 = 0;
    }
  }

  if (is_rank0) {
    Scan_Net(argv[0]);
  }
  else {
    SOCKET sock;
    int i;

    for (i = 0; i < 5; ++i) {
      printf("try connect: %s\n", buf);
      sock = CPM_StartClient(buf);

      if (sock != INVALID_SOCKET) {
        break;
      }

      Sleep(1000);
    }

    if (sock != INVALID_SOCKET) {
      int ret;
      ret = CPM_Recv(sock, &g_comm_info, sizeof(g_comm_info), 0, 0);

      if (ret > 0) {
        Net_t* ps = puship(buf);
        ps->sock = sock;
        printf("connected: %s\n", buf);
      }
    }
  }

  return 0;
}
int MPI_Comm_size(MPI_Comm c, int* psize)
{
  *psize = g_comm_info.size;
  return 1;
}
int MPI_Comm_rank(MPI_Comm c, int* prank)
{
  *prank = g_comm_info.rank;
  return 0;
}
int MPI_Finalize(void)
{
  int i;

  for (i = 0; i < g_net_len; ++i) {
    if (g_net_t[i].sock && g_net_t[i].sock != INVALID_SOCKET) {
      closesocket(g_net_t[i].sock);
      g_net_t[i].sock = 0;
      printf("close: %s\n", g_net_t[i].addr);
    }
  }

  return 0;
}
#endif // __MPI_H__
