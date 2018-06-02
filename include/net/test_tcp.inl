
#define TEST_TCP_PORT 6666 

#include "cmath.h"
#include "math/rand.inl"

int test_tcp_client1();
int test_tcp_server1();
int test_tcp_server() 
{
  int ser_sockfd,cli_sockfd; 
  int err,n, len;
  char recvline[200],sendline[200];
  addr_in addr[1] = {0};
  addr_in cli_addr[1] = {0};
  //return test_tcp_server1();
  
  ser_sockfd = sock_open(NULL, TEST_TCP_PORT, SOCK_TCP, addr);
  if(ser_sockfd==-1) {
    printf("socket error:%s\n", sys_errmsg(0));
    return -1; 
  }
  err = sock_bind(ser_sockfd, addr);
  if(err==-1) {
    printf("bind error:%s\n", sys_errmsg(0));
    return -1; 
  }
  err = sock_listen(ser_sockfd, 5);
  if(err==-1) { 
    printf("listen error:%s\n", sys_errmsg(0));
    return -1; 
  }
  printf("listen the port:\n");
  while(1) {
    cli_sockfd = sock_accept(ser_sockfd, cli_addr); 
    if(cli_sockfd==-1) { 
      printf("accept error:%s\n",sys_errmsg(0)); 
    }

    while(1) {
      printf("waiting for client...\n"); 
      n = sock_recv(cli_sockfd, recvline, 1024, 0); 
      if(n<=0) {
        //printf("recv error:%s\n",sys_errmsg(0));
        //break;
        n = 0;
      }
      recvline[n]='\0';
      printf("recv data is:%s\n",recvline); 
      printf("Input your words:");
      len = mygetline(sendline, countof(sendline));
      //len = randstr(NULL, sendline, 10, NULL);
      sock_send(cli_sockfd, sendline, len, 0); 
    } 
    sock_close(cli_sockfd); 
  }
  sock_close(ser_sockfd); 
  return 0; 
}

int test_tcp_client() 
{
  int sockfd;
  int err,n, len;
  addr_in addr_ser[1] = {0};
  char sendline[20],recvline[20]; 
  //return test_tcp_client1();
  //sockfd = sock_open("192.168.1.3", TEST_TCP_PORT, SOCK_TCP, addr_ser);
  sockfd = sock_open("10.1.106.106", 8333, SOCK_TCP, addr_ser);
  if(sockfd==-1) { 
    printf("socket error\n");
    return -1; 
  }
  err = sock_connect(sockfd, addr_ser);
  if(err==-1) {
    printf("connect error:%s\n",sys_errmsg(0)); 
    return -1; 
  }
  printf("connect with server...\n");
  while(1) { 
    printf("Input your words:"); 
    len = mygetline(sendline, countof(sendline));
    //len = randstr(NULL, sendline, 10, NULL);
    sock_send(sockfd, sendline, len, 0);
    printf("waiting for server...\n");
    n = sock_recv(sockfd,recvline,100,0);
    if (n<=0) {
      //break;
      n = 0;
    }
    recvline[n]='\0';
    printf("recv data is:%s\n",recvline); 
  }
  sock_close(sockfd);
  return 0; 
} 

// UDP
int test_udp_server() 
{
  int sockfd; 
  int err,n, len; 
  addr_in addr_ser[1], addr_cli[1]; 
  char recvline[200],sendline[200]; 
  
  sockfd=sock_open(NULL,TEST_TCP_PORT, SOCK_UDP, addr_ser); 
  if(sockfd==-1) {
    printf("socket error:%s\n",sys_errmsg(0)); 
    return -1; 
  }
  
  err=sock_bind(sockfd, addr_ser);
  if(err==-1) { 
    printf("bind error:%s\n",sys_errmsg(0)); 
    return -1; 
  }

  while(1) {
    printf("waiting for client......\n"); 
    n=sock_recvfrom(sockfd, recvline, 200, 0, addr_cli); 
    if(n==-1) { 
      printf("recvfrom error:%s\n",sys_errmsg(0)); 
      return -1; 
    } 
    recvline[n]='\0'; 
    printf("recv data is:%s\n", recvline); 
    
    printf("Input your words: \n"); 
    len = mygetline(sendline, countof(sendline));
    
    n=sock_sendto(sockfd,sendline,200,0, addr_cli);
    if(n==-1) {
      printf("sendto error:%s\n",sys_errmsg(0)); 
      return -1; 
    } 
  } 
  sock_close(sockfd);
  return 0; 
} 

int test_udp_client() 
{
  int sockfd; 
  int n, len;
  addr_in addr_ser[1]; 
  char recvline[200],sendline[200]; 
  
  sockfd=sock_open(NULL,TEST_TCP_PORT, SOCK_UDP, addr_ser); 
  if(sockfd==-1) {
    printf("socket error:%s\n",sys_errmsg(0)); 
    return -1;
  }
  
  while(1) 
  { 
    printf("Input your words: ");
    len = mygetline(sendline, countof(sendline));
    
    n=sock_sendto(sockfd,sendline,200,0, addr_ser); 
    if(n==-1) 
    { 
      printf("sendto error:%s\n",sys_errmsg(0)); 
      return -1; 
    } 
    
    printf("waiting for server......\n"); 
    n = sock_recvfrom(sockfd,recvline,200, 0, addr_ser); 
    if(n==-1) 
    { 
      printf("recvfrom error:%s\n",sys_errmsg(0)); 
      return -1; 
    } 
    recvline[n]='\0'; 
    printf("recv data is:%s\n",recvline); 
  } 
  sock_close(sockfd);
  return 0; 
} 

int test_tcp() {
  return 0;
}
