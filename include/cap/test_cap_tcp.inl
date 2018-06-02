
#include "cstd.h"
#include "cfile.h"
#include "cap.h"
#include "net.h"

//#include "img/color.inl"
//#include "img/imgopt.inl"
//#include "cv1/cv.inl"

#define PORT 6665
#if 1
#define HEIGHT 720
#define WIDTH 1280
#else
#define HEIGHT 480
#define WIDTH  640
#endif

int test_cap_tcp_ser() {
  //ip = "10.1.60.222";
  capdev cap[1] = {0};
  if (capdshow_open(cap, 0, WIDTH, HEIGHT)) {
    int faceindex=0;
    img_t im[1] = {0};
    int ret;
    addr_in addr[1] = {0};
    int ser_sockfd = sock_open_tcp_server(NULL, PORT, SOCK_TCP, addr);
    if (ser_sockfd>0) {
      char buf[256];
      int len, ch='s';
      int err = sock_listen(ser_sockfd, 5);
      if(err==-1) { 
        printf("listen error:%s\n", sys_errmsg(0));
        return -1; 
      }
      printf("listen in port %d\n", PORT);
      addr_in cli_addr[1] = {0};
      int cli_sockfd = sock_accept(ser_sockfd, cli_addr); 
      if(cli_sockfd==-1) { 
        printf("accept error:%s\n",sys_errmsg(0)); 
      }
      addr_tostr(cli_addr, buf, countof(buf));
      printf("accept %s\n", buf);
      for (;1;) {
        cap_getframe(cap, im, CAP_RGB);
        printf("%d %d\n", im->h, im->w);
        int n = sock_send_safe(cli_sockfd, im->tt.data, im->s*im->h, NULL);
        if (n<0) {
          sock_close(cli_sockfd);
          break;
        }
        //printf("%d\n", n);
        imshow_(im);
        ch = WaitKey(10);
        //select_run(s, 30);
        if ('s'==ch) {
          ++faceindex;
        }
      }
    }
  }
  return 0;
}
int test_cap_tcp_cli() {
  int sockfd;
  int err,n, len;
  char aa[10] = "";
  addr_in addr_ser[1] = {0};
  img_t im[1] = {0};
  char* ip = "10.1.60.222";
  ip = "10.1.60.120";
  ip = "10.1.60.83";
  ip = "127.0.0.1";
  //return test_tcp_client1();
  sockfd = sock_open(ip, PORT, SOCK_TCP, addr_ser);
  if(sockfd==-1) { 
    printf("socket error\n");
    return -1; 
  }
//#define LINE printf("%d\n", __LINE__)
  err = sock_connect(sockfd, addr_ser);
  if(err==-1) {
    printf("connect error:%s\n", sys_errmsg(0)); 
    return -1;
  }
  printf("connect with server...\n");
  imsetsize(im, HEIGHT, WIDTH, 4, 1);
  int frame = 0;
  while(1) {
    //printf("Input your words:"); 
    //len = mygetline(sendline, countof(sendline));
    //len = randstr(NULL, sendline, 10, NULL);
    int n = sock_recv_safe(sockfd, im->tt.data, im->s*im->h, 0);
    if (n<=0) {
      sock_close(sockfd);
      break;
    }
    //printf("%d\n", n);
    printf("%5d %d %d\n", frame, im->h, im->w);
    ++frame;
    imshow_(im);
    WaitKey(10);
  }
  sock_close(sockfd);
  imfree(im);
  return 0;
}
