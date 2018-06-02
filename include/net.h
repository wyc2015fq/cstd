
#ifndef _NET_H_
#define _NET_H_

#include "cstd.h"

// net
typedef struct {
  unsigned short family;
  unsigned char data[26];
} addr_in;

typedef enum {  SOCK_TCP,  SOCK_UDP, } SOCK_TYPE;

#define SOCK_SETSIZE   64

typedef struct sock_set {
  unsigned int fd_count;               /* how many are SET? */
  int fd_array[SOCK_SETSIZE];   /* an array of SOCKETs */
} sock_set;

typedef struct timeval_t {
  unsigned int tv_sec;         /* seconds */
  unsigned int tv_usec;        /* and microseconds */
} timeval_t;

int addr_tostr(const void* addr, char* dst, int size);
int addr_set(void* addr, const char* ip, int port);
int sock_open(const char* ip, int port, SOCK_TYPE type, addr_in* addr);
int sock_listen(int s, int backlog);
int sock_close(int s);
int sock_connect(int s, const addr_in* addr);
int sock_bind(int s, const addr_in* addr);
int sock_accept(int sock, addr_in* addr);
int sock_recv(int s, void* data, int len, int flag);
int sock_send(int s, const void* buf, int len, int flags);
int sock_sendto(int s, const void* buf, int len, int flags, const addr_in* to);
int sock_recvfrom(int sock, void* buf, int len, int flags, addr_in* from);
int sock_select(int nfds, sock_set* readfds, sock_set* writefds, sock_set *exceptfds, const timeval_t *timeout);
int sock_shutdown(int s, int how);

int sock_set_add(int fd, sock_set* set);
int sock_set_clr(int fd, sock_set* set);

static int sock_send_safe(int s, const void* data, int len, int flag) {
  int i=0;
  for (;i<len;) {
    int ret = sock_send(s, (char*)data + i, len-i, flag);
    //printf("sock_send ret %d / %d = %f\n", ret, len, ret*1./len);
    if (ret<=0) {
      return ret;
    }
    i += ret;
  }
  return len;
}

static int sock_recv_safe(int s, void* data, int len, int flag) {
  int i=0;
  for (;i<len;) {
    int ret = sock_recv(s, (char*)data + i, len-i, flag);
    //printf("sock_recv ret %d / %d = %f\n", ret, len, ret*1./len);
    if (ret<=0) {
      return ret;
    }
    i += ret;
  }
  return len;
}

////////////////////////////////////////////////////////////////

typedef int (*net_callback_f)(int sock, void* user);

typedef struct socket_info_t {
  int sock;
  net_callback_f fun;
  void* user;
  struct socket_info_t* prev;
  struct socket_info_t* next;
} socket_info_t;

typedef struct select_thd_t {
  bool m_bWorkRuning;
  lock_t m_synSOCKMap[1];
  socket_info_t* si;
  thread_t th[1];
} select_thd_t;

int select_run(select_thd_t* s, int delay);
//socket_info_t* select_add(select_thd_t* s, int sock, net_callback_f fun, void* user);
//int sock_ioctl(int sock, int cmd, ulong* argp);

static int select_init(select_thd_t* s) {
  s->m_bWorkRuning = 1;
  sys_lock_init(s->m_synSOCKMap);
  return 0;
}
static int select_exit(select_thd_t* s) {
  s->m_bWorkRuning = 0;
  sys_lock_destroy(s->m_synSOCKMap);
  return 0;
}
static socket_info_t* select_add(select_thd_t* s, int sock, net_callback_f fun, void* user) {
  socket_info_t* si = MALLOC(socket_info_t, 1);
  memset(si, 0, sizeof(socket_info_t));
  si->sock = sock;
  si->user = user;
  si->fun = fun;
  sys_lock_lock(s->m_synSOCKMap);
  LIST_ADDFRONT(s->si, si);
  sys_lock_unlock(s->m_synSOCKMap);
  return si;
}
static int select_loop(select_thd_t* s) {
  while (s->m_bWorkRuning) {
    select_run(s, 300);
  }
  return 0;
}

static int select_listen_fun(int sock, void* user) {
  return 0;//select_add(s, sock, );
}
static socket_info_t* select_listen(select_thd_t* s, int sock, net_callback_f fun, void* user) {
  return 0;//select_add(s, sock, );
}
static int select_del(select_thd_t* s, socket_info_t* si) {
  sys_lock_lock(s->m_synSOCKMap);
  LIST_DEL(s->si, si);
  sys_lock_unlock(s->m_synSOCKMap);
  return 0;
}
static int sock_open_tcp_server(const char* ip, int port, SOCK_TYPE type, addr_in* addr) {
  int sock = sock_open(ip, port, type, addr);
  if (sock>0) {
    int err = sock_bind(sock, addr);
    if(err==-1) {
      printf("bind error:%s\n", strerror(errno));
      return -1;
    }
    {
      //unsigned long ul=1;
      //ioctlsocket(sock, FIONBIO, &ul);
    }
  }
  return sock;
}
static socket_info_t* select_client_create(select_thd_t* s, const char* ip, int port, net_callback_f fun, void* user, addr_in* addr) {
  int sock = sock_open(NULL, 0, SOCK_TCP, addr);
  socket_info_t* si = NULL;
  if (sock>=0) {
    addr_set(addr, ip, port);
    if (0==sock_connect(sock, addr)) {
      select_init(s);
      si = select_add(s, sock, fun, user);
    }
  }
  return si;
}

#define HTTP_READ_BUF_LEN     4096
typedef struct sockbuf_t {
  int sock;
  int readBufRemain;
  char readBuf[HTTP_READ_BUF_LEN + 1];
  char *readBufPtr;
} sockbuf_t;

static int sockbuf_readChar(sockbuf_t* s, char* cp)
{
  if (s->readBufRemain == 0) {
    bzero(s->readBuf, HTTP_READ_BUF_LEN + 1);
    s->readBufRemain = sock_recv(s->sock, s->readBuf, HTTP_READ_BUF_LEN, 0);
    if (s->readBufRemain < 1) {
      return(0);
    }
    s->readBuf[s->readBufRemain] = 0;
    s->readBufPtr = s->readBuf;
  }
  *cp = *s->readBufPtr++;
  s->readBufRemain--;
  return(1);
}

static int sockbuf_readLine(sockbuf_t* s, char* destBuf, int len)
{
  char curChar, *dst;
  int count;
  count = 0;
  dst = destBuf;
  while (count < len) {
    if (sockbuf_readChar(s, &curChar) < 1) {
      return(0);
    }
    if (curChar == '\n') {
      *dst = 0;
      return(1);
    }
    if (curChar == '\r') {
      continue;
    }
    else {
      *dst++ = curChar;
      count++;
    }
  }
  *dst = 0;
  return(1);
}
static int sockbuf_readBuf(sockbuf_t* s, char* destBuf, int len)
{
  char curChar, *dst;
  int count;
  count = 0;
  dst = destBuf;
  while (count < len) {
    if (sockbuf_readChar(s, &curChar) < 1) {
      return(0);
    }
    *dst++ = curChar;
    count++;
  }
  return(1);
}
#endif // _NET_H_

