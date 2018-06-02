#include "mynet.h"
//#include "thread.h"
#include <locale.h>
#define SERVER_PORT 13333
#define TEST_PORT SERVER_PORT
#define MAXBUF 1024
typedef struct addr_in {unsigned short af; unsigned char data[26];} addr_in;
#if 0
#define TEST_AF AF_UNSPEC
#define TEST_AF AF_INET6
//#define TEST_SOCKADDR_IN struct sockaddr_in6
#define sinx_addr sin6_addr
#define sinx_port sin6_port
#define TEST_IP "2001:a:2::7"
#else
#define TEST_AF AF_INET
//#define TEST_SOCKADDR_IN struct sockaddr_in
#define sinx_addr sin_addr
#define sinx_port sin_port
//#define TEST_IP "192.168.1.3"
#define TEST_IP "ns7.mydebian.org"
#endif
#define ANYIP(af) (AF_INET6==(af)) ? "::0" : "0.0.0.0"
int test_ipv6_tcp_server()
{
  int port, addrlen, sockfd, new_fd;
  socklen_t len;
  /* struct sockaddr_in my_addr, their_addr; */ // IPv4
  addr_in my_addr, their_addr; // IPv6
  unsigned int lisnum = 2;
  char buf[MAXBUF + 1];
  //setlocale(LC_ALL, "chs");
  /* if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) */ // IPv4
  if ((sockfd = socket(TEST_AF, SOCK_STREAM, IPPROTO_TCP)) == -1) { // IPv6
    perror("socket");
    exit(1);
  }
  else {
    printf("socket created\n");
  }
  bzero(&my_addr, sizeof(my_addr));
  //net_addr(TEST_IP, TEST_PORT, &my_addr);
  addrlen = net_addr(ANYIP(TEST_AF), TEST_PORT, &my_addr);
  port = net_ntop(&my_addr, buf, MAXBUF);
  printf("address created %s %d\n", buf, port);
  /* if (bind(sockfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr)) */ // IPv4
  if (-1 == bind(sockfd, (struct sockaddr*) &my_addr, addrlen)) { // IPv6
    perror("bind");
    exit(1);
  }
  else {
    printf("binded\n");
  }
  if (listen(sockfd, lisnum) == -1) {
    perror("listen");
    exit(1);
  }
  else {
    printf("begin listen\n");
  }
  while (1) {
    len = sizeof(their_addr);
    if ((new_fd = accept(sockfd, (struct sockaddr*) &their_addr, &len)) == -1) {
      perror("accept");
      exit(errno);
    }
    else {
      int port = net_ntop(&their_addr, buf, MAXBUF);
      //strcpy(buf, getsocknamestr(new_fd));
      printf("server: got connection from %s, port %d, socket %d\n", buf, port, new_fd); // IPv6
    }
    /* 开始处理每个新连接上的数据收发 */
    bzero(buf, MAXBUF + 1);
    strcpy(buf, "1111111111111111");
    /* 发消息给客户端 */
    len = send(new_fd, buf, strlen(buf), 0);
    if (len < 0) {
      printf("send fail : '%s', errno: %d, errmsg: '%s'\n", buf, errno, strerror(errno));
    }
    else {
      printf("send OK: '%s', %d bytes\n", buf, len);
    }
    bzero(buf, MAXBUF + 1);
    /* 接收客户端的消息 */
    len = recv(new_fd, buf, MAXBUF, 0);
    if (len > 0) {
      printf("recv OK :'%s', %d bytes\n", buf, len);
    }
    else {
      printf("recv fail: errno = %d, errmsg: '%s'\n", errno, strerror(errno));
    }
    /* 处理每个新连接上的数据收发结束 */
  }
  closesocket(sockfd);
  return 0;
}
int test_ipv6_tcp_client()
{
  int port, addrlen, sockfd, len;
  /* struct sockaddr_in dest; */ // IPv4
  addr_in dest; // IPv6
  char buf[MAXBUF + 1];
  //printf("中文useage: ip port\n");
  /* 创建一个 socket 用于 tcp 通信 */
  /* if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) */ // IPv4
  if ((sockfd = socket(TEST_AF, SOCK_STREAM, IPPROTO_TCP)) < 0) { // IPv6
    perror("Socket");
    exit(errno);
  }
  //DomainToIP(TEST_IP, "13333", buf);
  printf("socket created\n");
  // 初始化服务器端（对方）的地址和端口信息
  bzero(&dest, sizeof(dest));
  //net_addr(TEST_AF, NULL, TEST_PORT, &dest);
  addrlen = net_addr(TEST_IP, TEST_PORT, &dest);
  port = net_ntop(&dest, buf, MAXBUF);
  printf("address created %s %d\n", buf, port);
  // 连接服务器
  if ((len = connect(sockfd, (struct sockaddr*) &dest, addrlen)) != 0) {
    fprintf(stderr, "11connect() failed with error %d: %s\n", errno, strerror(errno));
    return 0;
    exit(errno);
  }
  printf("server connected\n");
  // 接收对方发过来的消息, 最多接收 MAXBUF 个字节
  bzero(buf, MAXBUF + 1);
  // 接收服务器来的消息
  len = recv(sockfd, buf, MAXBUF, 0);
  if (len > 0) {
    printf("recv OK: '%s', %d bytes\n", buf, len);
  }
  else {
    printf("recv errno: %d, '%s'\n", errno, strerror(errno));
  }
  bzero(buf, MAXBUF + 1);
  strcpy(buf, "222222222222222222222");
  // 发消息给服务器
  len = send(sockfd, buf, strlen(buf), 0);
  if (len < 0) {
    printf("send fail '%s', errno: %d, errmsg='%s'\n", buf, errno, strerror(errno));
  }
  else {
    printf("send OK: '%s'%d bytes\n", buf, len);
  }
  /* 关闭连接 */
  closesocket(sockfd);
  return 0;
}
int test_ipv6_udp_server()
{
  int mysocket, len;
  int i = 0;
  struct sockaddr_in6 addr;
  int addr_len;
  char msg[200];
  char buf[300];
  if ((mysocket = socket(TEST_AF, SOCK_DGRAM, 0)) < 0) {
    perror("error:");
    return(1);
  }
  else {
    printf("socket created ...\n");
    printf("socket id :%d \n", mysocket);
  }
  addr_len = sizeof(struct sockaddr_in6);
  bzero(&addr, sizeof(addr));
  net_addr(ANYIP(TEST_AF), TEST_PORT, &addr);
  if (bind(mysocket, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
    perror("connect");
    return(1);
  }
  else {
    printf("bink ok .\n");
    printf("local port : %d\n", TEST_PORT);
  }
  while (1) {
    bzero(msg, sizeof(msg));
    len = recvfrom(mysocket, msg, sizeof(msg), 0, (struct sockaddr*)&addr, (socklen_t*)&addr_len);
    printf("%d:", i);
    i++;
    inet_ntop(TEST_AF, &addr.sin6_addr, buf, sizeof(buf));
    printf("message from ip %s", buf);
    printf("Received message : %s\n", msg);
    if (sendto(mysocket, msg, len, 0, (struct sockaddr*)&addr, addr_len) < 0) {
      printf("error");
      return(1);
    }
  }
  return 0;
}
// "::1"相当于ipv4下的lo，即127网段
int test_ipv6_udp_client()
{
  int mysocket, len;
  int i = 0;
  struct sockaddr_in6 addr;
  int addr_len;
  char msg[200];
  if ((mysocket = socket(TEST_AF, SOCK_DGRAM, 0)) < 0) {
    perror("error:");
    return(1);
  }
  else {
    printf("socket created ...\n");
    printf("socket id :%d \n", mysocket);
    printf("rmote ip : %s\n", TEST_IP);
    printf("remote port :%d \n", TEST_PORT);
  }
  addr_len = sizeof(struct sockaddr_in6);
  bzero(&addr, sizeof(addr));
  net_addr(TEST_IP, TEST_PORT, &addr);
  while (i < 10) {
    bzero(msg, sizeof(msg));
    sprintf(msg, "send %d", i);
    len = strlen(msg);
    if (sendto(mysocket, msg, sizeof(msg), 0, (struct sockaddr*)&addr, addr_len) < 0) {
      printf("error");
      return(1);
    }
    len = recvfrom(mysocket, msg, sizeof(msg), 0, (struct sockaddr*)&addr, (socklen_t*)&addr_len);
    printf("%d:", i);
    i++;
    printf("Received message : %s\n", msg);
  }
  return 0;
}
#define TEST_TCP 1
int test_ipv6_server()
{
  if (TEST_TCP) {
    return test_ipv6_tcp_server();
  }
  else {
    return test_ipv6_udp_server();
  }
  return 0;
}
int test_ipv6_client()
{
  if (TEST_TCP) {
    return test_ipv6_tcp_client();
  }
  else {
    return test_ipv6_udp_client();
  }
  return 0;
}

