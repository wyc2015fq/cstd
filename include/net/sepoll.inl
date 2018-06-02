#ifndef _SEPOLL_INL_
#define _SEPOLL_INL_

/* clang-format off */

enum EPOLL_EVENTS {
  EPOLLIN      = 1 <<  0,
  EPOLLPRI     = 1 <<  1,
  EPOLLOUT     = 1 <<  2,
  EPOLLERR     = 1 <<  3,
  EPOLLHUP     = 1 <<  4,
  EPOLLRDNORM  = 1 <<  6,
  EPOLLRDBAND  = 1 <<  7,
  EPOLLWRNORM  = 1 <<  8,
  EPOLLWRBAND  = 1 <<  9,
  EPOLLMSG     = 1 << 10, /* Never reported. */
  EPOLLRDHUP   = 1 << 13,
  EPOLLONESHOT = 1 << 31
};

#define EPOLLIN      ((uint32_t) EPOLLIN)
#define EPOLLPRI     ((uint32_t) EPOLLPRI)
#define EPOLLOUT     ((uint32_t) EPOLLOUT)
#define EPOLLERR     ((uint32_t) EPOLLERR)
#define EPOLLHUP     ((uint32_t) EPOLLHUP)
#define EPOLLRDNORM  ((uint32_t) EPOLLRDNORM)
#define EPOLLRDBAND  ((uint32_t) EPOLLRDBAND)
#define EPOLLWRNORM  ((uint32_t) EPOLLWRNORM)
#define EPOLLWRBAND  ((uint32_t) EPOLLWRBAND)
#define EPOLLMSG     ((uint32_t) EPOLLMSG)
#define EPOLLRDHUP   ((uint32_t) EPOLLRDHUP)
#define EPOLLONESHOT ((uint32_t) EPOLLONESHOT)

#define EPOLL_CTL_ADD 1
#define EPOLL_CTL_MOD 2
#define EPOLL_CTL_DEL 3

/* clang-format on */


typedef union epoll_data {
  void* ptr;
  int fd;
  uint32_t u32;
  uint64_t u64;
  SOCKET sock; /* Windows specific */
  HANDLE hnd;  /* Windows specific */
} epoll_data_t;

struct epoll_event {
  uint32_t events;   /* Epoll events */
  epoll_data_t data; /* User data variable */
};

#if 0
int epoll_create(int size);
int epoll_create1(int flags);
int epoll_close(int ephnd);
int epoll_ctl(int ephnd, int op, SOCKET sock, struct epoll_event* event);
int epoll_wait(int ephnd, struct epoll_event* events, int maxevents, int timeout);
#endif

typedef sock_set* epoll_handle;

static epoll_handle epoll_create(int size) {
  sock_set* ephnd = (sock_set*)malloc(sizeof(sock_set));
  BZERO1(ephnd);
  //MYREALLOC(ephnd->ev, size);
  //ephnd->i=0;
  //ephnd->used = 0;
  return ephnd;
}
//epoll_handle epoll_create1(int flags);
static int epoll_close(epoll_handle ephnd) {
  //fd_set *frds = &ephnd->frds;
  //free(ephnd->ev);
  free(ephnd);
  //FD_ZERO(frds);
  return 0;
}
static int epoll_ctl(epoll_handle ephnd, int op, SOCKET sock, struct epoll_event* event) {
  int i=0, ret=0;
  switch(op) {
  case EPOLL_CTL_MOD:
  case EPOLL_CTL_ADD:
    sock_set_add(sock, ephnd);
    break;
  case EPOLL_CTL_DEL:
    sock_set_clr(sock, ephnd);
    break;
  default:
    break;
  }
  return ret;
}
static int epoll_wait(epoll_handle ephnd, struct epoll_event* events, int maxevents, int timeout) {
  uint32 nSize = ephnd->fd_count;
  int ret = 0, nfds=0;
  int i = 0;//ephnd->i;
  struct timeval_t rtval;
  sock_set frds = *ephnd;
  rtval.tv_sec = 0;
  rtval.tv_usec = (timeout*1000);
  ret = sock_select(nSize, &frds, NULL, NULL, &rtval);
  
  if (ret <= 0) {
    return -1;
  }

  for (i=0; i < nSize; ++i) {
    int fd = ephnd->fd_array[i];
    if (nfds<maxevents && FD_ISSET(fd, &frds)) {
      events[nfds].events = EPOLLIN;
      events[nfds].data.fd = fd;
      nfds++;
    }
  }
  return nfds;
}


#endif // _SEPOLL_INL_
