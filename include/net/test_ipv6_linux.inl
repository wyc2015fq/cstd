#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/un.h>
#include <netdb.h>
#define DEFAULT_PORT "9999" /* default port number for bind(2) */
#define DEFAULT_BACKLOG 5 /* backlog */
/* structure to include options */
struct s_opt_svr {
  sa_family_t family; /* PF_UNSPEC, PF_INET, PF_INET6 */
  int debug; /* display verbosely */
  char* host; /* nodename for getaddrinfo(3) */
  char* service; /* service name: "pop", "110" */
#ifdef IPV6_V6ONLY
  int v6only;
#endif
  int reuseaddr; /* SO_REUSEADDR */
  int dontuselocal; /* don't use PF_LOCAL socket */
};
// -f 6
void print_help_svr(char* pname)
{
  fprintf(stderr, "%s [option]\n"
      " -e : server side display\n"
      " -f [46]: specify family\n"
      " -a : specify bind address\n"
      " -p : specify port (default 9999)\n"
#ifdef IPV6_V6ONLY
      " -6 : use IPV6_V6ONLY option\n"
#endif
      " -A : use SO_REUSEADDR\n"
      " -l : don't use PF_LOCAL socket\n"
      " -d : display verbosely\n"
      " -h: help\n",
      pname);
}
void read_write(int s, int debug, int echop)
{
  char peer[NI_MAXHOST];
  struct sockaddr_storage ss;
  socklen_t salen = sizeof(ss);
  int n;
  char buf[BUFSIZ];
  if (debug) {
    printf("s=[%d],debug=[%d] {%d}", s, debug, BUFSIZ);
  }
  if (debug) {
    if (getpeername(s, (struct sockaddr*)&ss, &salen) < 0) {
      perror("getpeername");
    }
    if (getnameinfo((struct sockaddr*)&ss, salen,
        peer, sizeof(peer), NULL, 0, NI_NUMERICHOST) < 0) {
      perror("getnameinfo");
    }
  }
  if (debug) {
    buf[0] = 'A';
    buf[1] = '\0';
    printf("BUF:[%s]", buf);
  }
  while ((n = read(s, buf, sizeof(buf))) > 0) {
    if (echop) {
      write(STDOUT_FILENO, buf, n);
    }
    if (write(s, buf, n) < 0) {
      perror("write error.\n");
      close(s);
      exit(1);
    };
    if (debug) {
      printf("BUF:[%s]", buf);
    }
    if (debug) {
      buf[n] = '\0';
      fprintf(stderr, "%s: %s", peer, buf);
    }
  }
  if (debug) {
    printf("Quit from read_write.\n");
  }
  close(s);
}
int test_tcpserv(int argc, char** argv)
{
  int c;
  struct s_opt_svr sopt;
  struct addrinfo hints, *res, *res0;
  int err, echop;
  struct sockaddr_storage ss;
  struct sockaddr* sa = (struct sockaddr*)&ss;
  socklen_t len = sizeof(ss);
  int s[FD_SETSIZE];
  int smax = 0;
  int sock_max = -1;
  fd_set rfd, rfd0;
  int i, m;
  char host[NI_MAXHOST];
  char serv[NI_MAXSERV];
  int backlog = DEFAULT_BACKLOG;
  printf("IPv4/IPv6 Protocol Independent Echo Server, v1.1\n");
  memset(&sopt, 0, sizeof(sopt));
  sopt.family = PF_UNSPEC;
  echop = 0;
  while ((c = getopt(argc, argv, "e:f:a:p:6Aldh")) != EOF) {
    switch (c) {
    case 'e':
      echop = 1;
      break;
    case 'f':
      if (!strncmp("4", optarg, 1)) {
        sopt.family = PF_INET;
      }
      else if (!strncmp("6", optarg, 1)) {
        sopt.family = PF_INET6;
      }
      else {
        print_help_svr(argv[0]);
        exit(EXIT_FAILURE);
      }
      break;
    case 'a':
      sopt.host = (char*)malloc(strlen(optarg) + 1);
      strcpy(sopt.host, optarg);
      break;
    case 'p':
      sopt.service = (char*)malloc(strlen(optarg) + 1);
      strcpy(sopt.service, optarg);
      break;
#ifdef IPV6_V6ONLY
    case '6':
      sopt.v6only = 1;
      break;
#endif
    case 'A':
      sopt.reuseaddr = 1;
      break;
    case 'l':
      sopt.dontuselocal = 1;
      break;
    case 'd':
      sopt.debug = 1;
      break;
    case 'h':
    default:
      print_help_svr(argv[0]);
      exit(EXIT_SUCCESS);
    }
  }
  if (sopt.service == NULL) {
    sopt.service = (char*)malloc(strlen(DEFAULT_PORT) + 1);
    strcpy(sopt.service, DEFAULT_PORT);
  }
  /* hints is used after zero cleared */
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = sopt.family;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE; /* specified in the case of bind(2) */
  err = getaddrinfo(sopt.host, sopt.service, &hints, &res0);
  if (err) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
    exit(EXIT_FAILURE);
  }
  /* bind(2) with getaddrinfo(3) results */
  for (res = res0 ; res && smax < FD_SETSIZE; res = res->ai_next) {
    if (sopt.debug) {
      printf("[%d][%d][%d]\n", res->ai_family, res->ai_socktype, res->ai_protocol);
    }
    s[smax] = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (s[smax] < 0) {
      fprintf(stderr, "socket() failed: %d\n", s[smax]);
      continue;
    }
    /* Some implementation returns UNIX domain protocol(PF_LOCAL) when hints.ai_family = PF_UNSPEC */
    if (res->ai_family == PF_LOCAL) {
      if (sopt.dontuselocal) {
        if (sopt.debug) {
          fprintf(stderr, "skip: ai_family is PF_LOCAL\n");
        }
        continue;
      }
      else {
        struct sockaddr_un* sun;
        sun = (struct sockaddr_un*)res->ai_addr;
        /* deleated because you fail in bind(2) if socket file exists in the case of AF_LOCAL */
        unlink(sun->sun_path);
      }
    }
#ifdef IPV6_V6ONLY /* configure IPV6_V6ONLY socket option */
    if (sopt.v6only && res->ai_family == PF_INET6) {
      int on = 1;
      if (setsockopt(s[smax], IPPROTO_IPV6, IPV6_V6ONLY, &on, sizeof(on)) < 0) {
        perror("setsockopt(IPV6_V6ONLY)");
      }
    }
#endif
    if (sopt.reuseaddr) {
      int on = 1;
      if (setsockopt(s[smax], SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
        perror("setsockopt(SO_REUSEADDR");
      }
    }
    if (sopt.debug) {
      printf("Socket[%d],Len[%d]", s[smax], res->ai_addrlen);
    }
    if (bind(s[smax], (struct sockaddr*)res->ai_addr, res->ai_addrlen) < 0) {
      getnameinfo((struct sockaddr*)res->ai_addr, res->ai_addrlen,
          host, sizeof(host),
          serv, sizeof(serv),
          NI_NUMERICHOST | NI_NUMERICSERV);
      fprintf(stderr, "bind(2):failed: %s: "
          "[%s]:%s\n", strerror(errno), host, serv);
      close(s[smax]);
      s[smax] = -1;
      continue;
    }
    else {
      getnameinfo((struct sockaddr*)res->ai_addr, res->ai_addrlen,
          host, sizeof(host),
          serv, sizeof(serv),
          NI_NUMERICHOST | NI_NUMERICSERV);
      fprintf(stderr, "bind(2):succeed: [%s]:%s\n", host, serv);
    }
    if (listen(s[smax], backlog) < 0) {
      getnameinfo((struct sockaddr*)res->ai_addr, res->ai_addrlen,
          host, sizeof(host),
          serv, sizeof(serv),
          NI_NUMERICHOST | NI_NUMERICSERV);
      fprintf(stderr, "listen(2):failed: [%s]:%s\n", host, serv);
      close(s[smax]);
      s[smax] = -1;
      continue;
    }
    else {
      getnameinfo((struct sockaddr*)res->ai_addr, res->ai_addrlen,
          host, sizeof(host),
          serv, sizeof(serv),
          NI_NUMERICHOST | NI_NUMERICSERV);
      fprintf(stderr, "listen(2):succeed: [%s]:%s\n", host, serv);
    }
    if (s[smax] > sock_max) {
      sock_max = s[smax];
    }
    smax++;
  }
  if (smax == 0) {
    fprintf(stderr, "no sockets\n");
    exit(EXIT_FAILURE);
  }
  freeaddrinfo(res0);
  FD_ZERO(&rfd0);
  for (i = 0; i < smax; i++) {
    FD_SET(s[i], &rfd0);
  }
  while (1) {
    rfd = rfd0;
    m = select(sock_max + 1, &rfd, NULL, NULL, NULL);
    if (m < 0) {
      if (errno == EINTR) {
        continue;
      }
      perror("select");
      exit(EXIT_FAILURE);
    }
    for (i = 0; i < smax; i++) {
      if (FD_ISSET(s[i], &rfd)) {
        int cs;
        cs = accept(s[i], sa, &len);
        if (cs < 0) {
          perror("accept");
          exit(EXIT_FAILURE);
        }
        if (fork() == 0) {
          close(s[i]);
          read_write(cs, sopt.debug, echop);
          close(cs);
          exit(EXIT_SUCCESS);
        }
        close(cs);
      }
    }
  }
  return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
/* structure to include options */
struct s_opt_cli {
  sa_family_t family; /* PF_UNSPEC, PF_INET, PF_INET6 */
  int debug; /* display verbosely */
  char* host; /* nodename for getaddrinfo(3) */
  char* service; /* service name: "pop", "110" */
};
void print_help_cli(char* pname)
{
  fprintf(stderr, "IPv4/v6 Echo client, v1.1.\n");
  fprintf(stderr, "%s [option] host\n"
      " -f [46]: specify family\n"
      " -p : specify port (default 9999)\n"
      " -d : display verbosely\n"
      " -h : help\n",
      pname);
}
int test_tcpclient(int argc, char** argv)
{
  struct s_opt_cli sopt;
  int c, n;
  struct addrinfo hints, *res, *res0;
  int err;
  int s;
  char host[NI_MAXHOST];
  char serv[NI_MAXSERV];
  char rbuf[BUFSIZ];
  char wbuf[BUFSIZ];
  memset(&sopt, 0, sizeof(sopt));
  sopt.family = PF_UNSPEC;
  while ((c = getopt(argc, argv, "p:f:dh")) != EOF) {
    switch (c) {
    case 'p':
      sopt.service = (char*)malloc(strlen(optarg) + 1);
      strcpy(sopt.service, optarg);
      break;
    case 'f':
      if (!strncmp("4", optarg, 1)) {
        sopt.family = PF_INET;
      }
      else if (!strncmp("6", optarg, 1)) {
        sopt.family = PF_INET6;
      }
      else {
        print_help_cli(argv[0]);
        exit(EXIT_FAILURE);
      }
      break;
    case 'd':
      sopt.debug = 1;
      break;
    case 'h':
    default:
      print_help_cli(argv[0]);
      exit(EXIT_SUCCESS);
    }
  }
  if (optind < argc) {
    sopt.host = (char*)malloc(strlen(argv[optind]) + 1);
    strcpy(sopt.host, argv[optind]);
  }
  else {
    print_help_cli(argv[0]);
    exit(EXIT_FAILURE);
  }
  if (sopt.service == NULL) {
    sopt.service = (char*)malloc(strlen(DEFAULT_PORT) + 1);
    strcpy(sopt.service, DEFAULT_PORT);
  }
  /* hints is used after zero cleared */
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = sopt.family;
  hints.ai_socktype = SOCK_STREAM;
  err = getaddrinfo(sopt.host, sopt.service, &hints, &res0);
  if (err) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
    exit(EXIT_FAILURE);
  }
  /* attempt to connect using getaddrinfo(3) results */
  for (res = res0 ; res ; res = res->ai_next) {
    s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (s < 0) {
      fprintf(stderr, "socket() faild: %d\n", s);
      continue;
    }
    if (connect(s, res->ai_addr, res->ai_addrlen) < 0) {
      perror("connect");
      close(s);
      s = -1;
      continue;
    }
    if (sopt.debug) {
      getnameinfo(res->ai_addr, res->ai_addrlen,
          host, sizeof(host),
          serv, sizeof(serv),
          NI_NUMERICHOST | NI_NUMERICSERV);
      fprintf(stderr, "connect: [%s]:%s\n", host, serv);
    }
    break;
  }
  if (s < 0) {
    fprintf(stderr, "can't connect %s\n", sopt.host);
    exit(EXIT_FAILURE);
  }
  n = -1;
  c = 0;
  while (fgets(wbuf, sizeof(wbuf), stdin) != NULL) {
    n = strlen(wbuf);
    if (sopt.debug) {
      printf("Length='%d'.\n", n);
    }
    if (n < 0) {
      fprintf(stderr, "can't read");
      exit(EXIT_FAILURE);
    }
    do {
      n = write(s, wbuf, n + 1);
      if (sopt.debug) {
        fputs(wbuf, stdout);
      }
    }
    while (n < 0 && errno == EINTR);
    if (sopt.debug) {
      printf("Write over.\n");
    }
    do {
      n = read(s, rbuf, sizeof(rbuf) - 1);
      if (sopt.debug) {
        printf("What's read from socket:[%s|%d].", rbuf, n);
      }
      c += n;
    }
    while (errno == EINTR && n <= 0);
    write(STDOUT_FILENO, rbuf, n);
  }
  if (sopt.service) {
    free(sopt.service);
  }
  if (sopt.host) {
    free(sopt.service);
  }
  freeaddrinfo(res0);
  return 0;
}

