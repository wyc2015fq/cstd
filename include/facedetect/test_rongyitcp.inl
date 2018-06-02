
#include "net.h"
#include "rongyitcp.inl"
#include "net/test_tcp.inl"

int test_rongyitcp() {
#ifdef _DEBUG
  CPCODE;
#endif
  if (1) {
    test_tcp_client();
    return 0;
  }
  //ip = "10.1.60.222";
  int faceindex=0;
  img_t im[1] = {0};
  char* ip;
  int ret, port;
  ip = "127.0.0.1";
  ip = "10.1.60.239";
  ip = "10.1.106.106";
  port = 8888;
  port = 60001;
  port = 8333;
  ret = rongyitcp_open(r, ip, port);
  if (ret) {
    char data[256];
    imsetsize(im, 400, 400, 4, 1);
    int len, ch='s';
    addr_tostr(r->addr, data, countof(data));
    printf("connected %s\n", data);
    
    for (;r->nid>0;) {
#ifdef _WIN32
      imshow_(im);
      ch = WaitKey(10);
#else
      ch = 's';
      sys_sleep(1000);
#endif
      select_run(r->s, 30);
      if ('s'==ch) {
        ++faceindex;
        rongyitcp_facedetectinfo(r, faceindex);
      }
    }
  }
  return 0;
}
