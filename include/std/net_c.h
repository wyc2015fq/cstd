
#ifndef _NET_H_
#define _NET_H_

//#include "cstd.h"
#include "std/list.h"
#include "std/stddef_c.h"
#include "std/inttypes_c.h"
#include "std/thread_c.h"


//#include <Iphlpapi.h>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#define SOCK_LENTH int

#ifdef _WIN32
static int sock_init()
{
  static WSADATA wsaData = {0};
  
  if (0 == wsaData.wVersion)
  {
    int RetVal = 0;
    
    if ((RetVal = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)
    {
      fprintf(stderr, "WSAStartup failed with error %d\n", RetVal);
      WSACleanup();
      return -1;
    }
  }
  return 0;
}
static int WSAStartuped = sock_init();
#endif // _WIN32

#ifdef __linux__
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/wait.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SOCK_LENTH socklen_t
#define closesocket(x)  close(x)
int sock_init() {
  return 1;
}
#endif // __linux__

// net
typedef struct {
  unsigned short family;
  unsigned char data[26];
} addr_in;

typedef enum {  SOCK_TCP,  SOCK_UDP, } SOCK_TYPE;

#define SOCK_SETSIZE   64

typedef struct sock_set {
  unsigned int fd_count;               /* how many are SET? */
  SOCKET fd_array[SOCK_SETSIZE];   /* an array of SOCKETs */
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
int sock_set_add(SOCKET fd, sock_set* set);
int sock_set_clr(SOCKET fd, sock_set* set);

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

////////////////////////////////////////////////////////////////

/*%
 * WARNING: Don't even consider trying to compile this on a system where
 * sizeof(int) < 4. sizeof(int) > 4 is fine; all the world's not a VAX.
 */
/*
 * Copy src to string dst of size siz. At most siz-1 characters
 * will be copied. Always NUL terminates (unless siz == 0).
 * Returns strlen(src); if retval >= siz, truncation occurred.
 */
static size_t my_strlcpy(char* __restrict dst, const char* __restrict src, size_t siz)
{
  char* d = dst;
  const char* s = src;
  size_t n = siz;

  /* Copy as many bytes as will fit */
  if (n != 0) {
    while (--n != 0) {
      if ((*d++ = *s++) == '\0') {
        break;
      }
    }
  }

  /* Not enough room in dst, add NUL and traverse rest of src */
  if (n == 0) {
    if (siz != 0) {
      *d = '\0'; /* NUL-terminate dst */
    }

    while (*s++)
      ;
  }

  return(s - src - 1); /* count does not include NUL */
}

/* const char *
 * inet_ntop4(src, dst, size)
 * format an IPv4 address
 * return:
 * `dst' (as a const)
 * notes:
 * (1) uses no statics
 * (2) takes a char* not an in_addr as input
 * author:
 * Paul Vixie, 1996.
 */
static char* inet_ntop4(const uchar* src, char* dst, int size)
{
  static const char fmt[] = "%u.%u.%u.%u";
  char tmp[sizeof "255.255.255.255"];
  int l;
  l = snprintf(tmp, sizeof(tmp), fmt, src[0], src[1], src[2], src[3]);

  if (l <= 0 || (int) l >= size) {
    return (NULL);
  }

  my_strlcpy(dst, tmp, size);
  return (dst);
}

/* const char *
 * inet_ntop6(src, dst, size)
 * convert IPv6 binary address into presentation (printable) format
 * author:
 * Paul Vixie, 1996.
 */
static char* inet_ntop6(const uchar* src, char* dst, int size)
{
  /*
  * Note that int32_t and int16_t need only be "at least" large enough
  * to contain a value of the specified size. On some systems, like
  * Crays, there is no such thing as an integer variable with 16 bits.
  * Keep this in mind if you think this function should have been coded
  * to use pointer overlays. All the world's not a VAX.
  */
  char tmp[sizeof "ffff:ffff:ffff:ffff:ffff:ffff:255.255.255.255"], *tp;
  struct {
    int base, len;
  } best, cur;
#define NS_IN6ADDRSZ 16
#define NS_INT16SZ 2
  uint words[NS_IN6ADDRSZ / NS_INT16SZ];
  int i;
  /*
  * Preprocess:
  * Copy the input (bytewise) array into a wordwise array.
  * Find the longest run of 0x00's in src[] for :: shorthanding.
  */
  memset(words, '\0', sizeof words);

  for (i = 0; i < NS_IN6ADDRSZ; i++) {
    words[i / 2] |= (src[i] << ((1 - (i % 2)) << 3));
  }

  best.base = -1;
  best.len = 0;
  cur.base = -1;
  cur.len = 0;

  for (i = 0; i < (NS_IN6ADDRSZ / NS_INT16SZ); i++) {
    if (words[i] == 0) {
      if (cur.base == -1) {
        cur.base = i, cur.len = 1;
      }

      else {
        cur.len++;
      }

    }

    else {
      if (cur.base != -1) {
        if (best.base == -1 || cur.len > best.len) {
          best = cur;
        }

        cur.base = -1;
      }

    }

  }

  if (cur.base != -1) {
    if (best.base == -1 || cur.len > best.len) {
      best = cur;
    }

  }

  if (best.base != -1 && best.len < 2) {
    best.base = -1;
  }

  /*
  * Format the result.
  */
  tp = tmp;

  for (i = 0; i < (NS_IN6ADDRSZ / NS_INT16SZ); i++) {
    /* Are we inside the best run of 0x00's? */
    if (best.base != -1 && i >= best.base &&
        i < (best.base + best.len)) {
      if (i == best.base) {
        *tp++ = ':';
      }

      continue;
    }

    /* Are we following an initial run of 0x00s or any real hex? */
    if (i != 0) {
      *tp++ = ':';
    }

    /* Is this address an encapsulated IPv4? */
    if (i == 6 && best.base == 0 && (best.len == 6 ||
        (best.len == 7 && words[7] != 0x0001) ||
        (best.len == 5 && words[5] == 0xffff))) {
      if (!inet_ntop4(src + 12, tp, sizeof tmp - (tp - tmp))) {
        return (NULL);
      }

      tp += strlen(tp);
      break;
    }

    tp += sprintf(tp, "%x", words[i]);
  }

  /* Was it a trailing run of 0x00's? */
  if (best.base != -1 && (best.base + best.len) ==
      (NS_IN6ADDRSZ / NS_INT16SZ)) {
    *tp++ = ':';
  }

  *tp++ = '\0';

  /*
  * Check for overflow, copy, and we're done.
  */
  if ((int)(tp - tmp) > size) {
    return (NULL);
  }

  strcpy(dst, tmp);
  return (dst);
}

/* char *
 * inet_ntop(af, src, dst, size)
 * convert a network format address to presentation format.
 * return:
 * pointer to presentation format address (`dst'), or NULL (see errno).
 * author:
 * Paul Vixie, 1996.
 */
static char* _inet_ntop(int af, const void* src, char* dst, int size)
{
  switch (af) {
  case AF_INET:
    return (inet_ntop4((const unsigned char*)src, dst, size));

  case AF_INET6:
    return (inet_ntop6((const unsigned char*)src, dst, size));

  default:
    return (NULL);
  }

  /* NOTREACHED */
}

int addr_tostr(const void* addr, char* dst, int size)
{
  int af = *(const short*)addr;
  const struct sockaddr_in* addr4 = (const struct sockaddr_in*)addr;
  const struct sockaddr_in6* addr6 = (const struct sockaddr_in6*)addr;
  char* ret = NULL;
  int port = 0;
  sock_init();

  switch (af) {
  case AF_INET:
    port = ntohs(addr4->sin_port);
    ret = (inet_ntop4((const unsigned char*)(&addr4->sin_addr), dst, size));
    break;

  case AF_INET6:
    port = ntohs(addr6->sin6_port);
    ret = (inet_ntop6((const unsigned char*)(&addr6->sin6_addr), dst, size));
    break;
  }

  return ret ? port : 0;
}

/*%
 * WARNING: Don't even consider trying to compile this on a system where
 * sizeof(int) < 4. sizeof(int) > 4 is fine; all the world's not a VAX.
 */
/* int
 * inet_pton4(addr, dst)
 * like inet_aton() but without all the hexadecimal and shorthand.
 * return:
 * 1 if `src' is a valid dotted quad, else 0.
 * notice:
 * does not touch `dst' unless it's returning 1.
 * author:
 * Paul Vixie, 1996.
 */
static int inet_pton4(const char* src, uchar* dst)
{
  static const char digits[] = "0123456789";
  int saw_digit, octets, ch;
#define NS_INADDRSZ 4
  char tmp[NS_INADDRSZ], *tp;
  saw_digit = 0;
  octets = 0;
  *(tp = tmp) = 0;

  while ((ch = *src++) != '\0') {
    const char* pch;

    if ((pch = strchr(digits, ch)) != NULL) {
      uint newt = *tp * 10 + (pch - digits);

      if (saw_digit && *tp == 0) {
        return (0);
      }

      if (newt > 255) {
        return (0);
      }

      *tp = newt;

      if (!saw_digit) {
        if (++octets > 4) {
          return (0);
        }

        saw_digit = 1;
      }

    }

    else if (ch == '.' && saw_digit) {
      if (octets == 4) {
        return (0);
      }

      *++tp = 0;
      saw_digit = 0;
    }

    else {
      return (0);
    }

  }

  if (octets < 4) {
    return (0);
  }

  memcpy(dst, tmp, NS_INADDRSZ);
  return (1);
}

/* int
 * inet_pton6(src, dst)
 * convert presentation level address to network order binary form.
 * return:
 * 1 if `src' is a valid [RFC1884 2.2] address, else 0.
 * notice:
 * (1) does not touch `dst' unless it's returning 1.
 * (2) :: in a full address is silently ignored.
 * credit:
 * inspired by Mark Andrews.
 * author:
 * Paul Vixie, 1996.
 */
static int inet_pton6(const char* src, uchar* dst)
{
  static const char xdigits_l[] = "0123456789abcdef", xdigits_u[] = "0123456789ABCDEF";
#define NS_IN6ADDRSZ 16
#define NS_INT16SZ 2
  uchar tmp[NS_IN6ADDRSZ], *tp, *endp, *colonp;
  const char* xdigits, *curtok;
  int ch, seen_xdigits;
  uint val;
  memset((tp = tmp), '\0', NS_IN6ADDRSZ);
  endp = tp + NS_IN6ADDRSZ;
  colonp = NULL;

  /* Leading :: requires some special handling. */
  if (*src == ':')
    if (*++src != ':') {
      return (0);
    }

  curtok = src;
  seen_xdigits = 0;
  val = 0;

  while ((ch = *src++) != '\0') {
    const char* pch;

    if ((pch = strchr((xdigits = xdigits_l), ch)) == NULL) {
      pch = strchr((xdigits = xdigits_u), ch);
    }

    if (pch != NULL) {
      val <<= 4;
      val |= (pch - xdigits);

      if (++seen_xdigits > 4) {
        return (0);
      }

      continue;
    }

    if (ch == ':') {
      curtok = src;

      if (!seen_xdigits) {
        if (colonp) {
          return (0);
        }

        colonp = tp;
        continue;
      }

      else if (*src == '\0') {
        return (0);
      }

      if (tp + NS_INT16SZ > endp) {
        return (0);
      }

      *tp++ = (char)(val >> 8) & 0xff;
      *tp++ = (char) val & 0xff;
      seen_xdigits = 0;
      val = 0;
      continue;
    }

    if (ch == '.' && ((tp + NS_INADDRSZ) <= endp) && inet_pton4(curtok, tp) > 0) {
      tp += NS_INADDRSZ;
      seen_xdigits = 0;
      break; /*%< '\\0' was seen by inet_pton4(). */
    }

    return (0);
  }

  if (seen_xdigits) {
    if (tp + NS_INT16SZ > endp) {
      return (0);
    }

    *tp++ = (char)(val >> 8) & 0xff;
    *tp++ = (char) val & 0xff;
  }

  if (colonp != NULL) {
    /*
    * Since some memmove()'s erroneously fail to handle
    * overlapping regions, we'll do the shift by hand.
    */
    const int n = tp - colonp;
    int i;

    if (tp == endp) {
      return (0);
    }

    for (i = 1; i <= n; i++) {
      endp[- i] = colonp[n - i];
      colonp[n - i] = 0;
    }

    tp = endp;
  }

  if (tp != endp) {
    return (0);
  }

  memcpy(dst, tmp, NS_IN6ADDRSZ);
  return (1);
}

/* int
 * inet_pton(af, src, dst)
 * convert from presentation format (which usually means ASCII printable)
 * to network format (which is usually some kind of binary format).
 * return:
 * 1 if the address was valid for the specified address family
 * 0 if the address wasn't valid (`dst' is untouched in this case)
 * -1 if some other error occurred (`dst' is untouched in this case, too)
 * author:
 * Paul Vixie, 1996.
 */
static int _inet_pton(int af, const char* src, void* dst)
{
  //printf("_inet_pton\n");
  switch (af) {
  case AF_INET:
    return (inet_pton4(src, (unsigned char*)dst));

  case AF_INET6:
    return (inet_pton6(src, (unsigned char*)dst));

  default:
    return (-1);
  }

  /* NOTREACHED */
}

int addr_set(void* addr, const char* ip, int port)
{
  struct addrinfo hints, *res, *res0;
  char portStr[32] = {0};
  int ret = 0;
  sock_init();
  snprintf(portStr, 32, "%d", port);
  memset(&hints, 0, sizeof(hints));
  hints.ai_family   = PF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags    = AI_PASSIVE;
  
  ret = getaddrinfo(ip, portStr, &hints, &res0);
  
  if (ret != 0)
  {
    //DEBUG_INFO("getaddrinfo(ret=%d) err: %s", ret, gai_strerror(ret));
    return 0;
  }
  
  if (1)
  {
    int i = 0;
    char buf[256];
    //DEBUG_INFO("getaddrinfo(ret=%d) ret: %s", ret, gai_strerror(ret));
    
    for (res = res0; res; res = res->ai_next)
    {
      addr_tostr(res->ai_addr, buf, 256);
      //            DEBUG_INFO("addr=%s flags=%d family=%d type=%d protocol=%d len=%d", buf,
      //                       res->ai_flags, res->ai_family,
      //                       res->ai_socktype, res->ai_protocol,
      //                       res->ai_addrlen);
      ++i;
    }
  }
  ret = 0;
  for (res = res0; res; res = res->ai_next) {
    if (res->ai_family == AF_INET) {
      memcpy(addr, res->ai_addr, sizeof(struct sockaddr_in));
      ret = sizeof(struct sockaddr_in);
      break;
    } else if (res->ai_family == AF_INET6) {
      memcpy(addr, res->ai_addr, sizeof(struct sockaddr_in6));
      ret = sizeof(struct sockaddr_in6);
      break;
    }
  }
  freeaddrinfo(res0);
  return ret;
}

#define READ_ARRAY_TIME_OUT(sockArray, sizeArray, time, bTimeOut, frds) do {\
  SOCKET sock = 0;\
  struct timeval rtval;\
  int i = 0;\
  FD_ZERO(&frds);\
  for (; i < (int)sizeArray; ++i) {\
    FD_SET(sockArray[i], &frds);\
    if( sockArray[i] > sock ) sock = sockArray[i];\
  }\
  rtval.tv_sec = 0;\
  rtval.tv_usec = (time*1000);\
  bTimeOut = select(sock+1, &frds, NULL, NULL, &rtval) <= 0; } while(0)

int select_run(select_thd_t* s, int delay)
{
  SOCKET Array[FD_SETSIZE] = {0};
  socket_info_t* sis[FD_SETSIZE] = {0};
  uint32 nSize = 0, i;
  bool bTimeOut = 0;
  fd_set frds;
  {
    socket_info_t* si = s->si;
    sys_lock_lock(s->m_synSOCKMap);
    for (; si; si = si->next) {
      Array[nSize] = si->sock;
      sis[nSize] = si;
      nSize++;
    }
    sys_lock_unlock(s->m_synSOCKMap);
  }
  
  if (nSize == 0) {
    //sys_sleep(delay);
    return 0;
  }
  
  READ_ARRAY_TIME_OUT(Array, nSize, delay, bTimeOut, frds);
  if (bTimeOut) {
    //sys_sleep(delay);
    return 0;
  }
  
  for (i = 0; i < nSize; ++i) {
    if (FD_ISSET(Array[i], &frds)) {
      if (sis[i]->fun) {
        sis[i]->fun(Array[i], sis[i]->user);
      }
    }
  }
  return 0;
}
int select_start() {
  static int inited = 0;
  static select_thd_t st[1] = {0};
  if (0==inited) {
    //sys_thread_create(st->th, NULL, );
  }
  return 0;
}

int sock_recv(int s, void* buf, int len, int flag) {
  return recv(s, (char*)buf, (SOCK_LENTH)len, flag);
}
int sock_send(int s, const void* buf, int len, int flags) {
  return send(s, (const char*)buf, (SOCK_LENTH)len, flags);
}
int sock_sendto(int s, const void* buf, int len, int flags, const addr_in* to) {
  return sendto(s, (const char*)buf, (SOCK_LENTH)len, flags, (const struct sockaddr*)to, sizeof(addr_in));
}
int sock_recvfrom(int sock, void* buf, int len, int flags, addr_in* from) {
  SOCK_LENTH size = sizeof(addr_in);
  len = recvfrom(sock, (char*)buf, len, flags, (struct sockaddr*)from, &size) ;
  return len;
}
int sock_set_add(SOCKET fd, sock_set* set) {
  fd_set* fds = (fd_set*)set;
  FD_SET(fd, fds);
  return 0;
}
int sock_set_clr(SOCKET fd, sock_set* set) {
  fd_set* fds = (fd_set*)set;
  FD_CLR(fd, fds);
  return 0;
}
int sock_select(int nfds, sock_set* readfds, sock_set* writefds, sock_set *exceptfds, const timeval_t *timeout) {
  struct timeval tv[1] = {0};
  if (timeout) {tv->tv_sec = timeout->tv_sec, tv->tv_usec = tv->tv_usec; }
  if (sizeof(sock_set)<sizeof(fd_set)) {
    printf("err: sock_select sizeof(sock_set)=%d sizeof(fd_set)=%d\n", (int)sizeof(sock_set), (int)sizeof(fd_set));
  }
  return select(nfds, (fd_set*)readfds, (fd_set*)writefds, (fd_set*)exceptfds, timeout ? tv : NULL);
}
int sock_shutdown(int s, int how) {
  return shutdown(s, how);
}
int sock_accept(int sock, addr_in* addr)
{
  SOCK_LENTH len = sizeof(addr_in);
  int sock_new = accept(sock, (struct sockaddr*)addr, &len);
  return sock_new;
}
//int sock_ioctl(int sock, int cmd, ulong* argp) {  return ioctlsocket(sock, cmd, argp);}
int sock_listen(int s, int backlog) {
  return listen(s, backlog);
}
//#define ANYIP(af) (AF_INET6==(af)) ? "::0" : "0.0.0.0"
int sock_open(const char* ip, int port, SOCK_TYPE type, addr_in* addr) {
  int sock = -1;
  addr_in _addr[1] = {0};
  addr = addr ? addr : _addr;
  ip = ip ? ip : "0.0.0.0";
  //sock_init();
  if (addr_set(addr, ip, port)) {
    int type1 = SOCK_TCP==type ? SOCK_STREAM : SOCK_DGRAM;
    sock = socket(addr->family, type1, 0);
    if (sock>0) {
      int size = 1<<16;
      u_long ul=1;
      setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (const char*)&size, sizeof(size));
      setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (const char*)&size, sizeof(size));
      //ioctlsocket(sock, FIONBIO, &ul); // no block
    }
  }
  return sock;
}
int sock_connect(int s, const addr_in* addr) {
  //sock_ioctl (sock, FIONBIO , true ); //此设置导致SSL握手失败
  int ret = connect(s, (const struct sockaddr*)addr, sizeof(addr_in));
  return ret;
}
int sock_bind(int s, const addr_in* addr)
{
  int on = 1;
  int ret;
  setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const int8*)&on, sizeof(on));
  ret = ::bind(s, (const struct sockaddr*)addr, sizeof(addr_in));
  //printf("%s", strerror(GetLastError()));
  return ret;
}
int sock_close(int s) {
  return closesocket(s);
}

//#include "test_sys_net.inl"


////////////////////////////////////////////////////////////////
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
  socket_info_t* si = (socket_info_t*)malloc(sizeof(socket_info_t));
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
    memset(s->readBuf, 0, HTTP_READ_BUF_LEN + 1);
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

