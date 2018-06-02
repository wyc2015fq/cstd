

#ifndef _WIN32
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<arpa/inet.h>
#endif

#include "test_save_net_pic.h"

int test_send_pic_start(ttt_t* tt, int server_port) {
  if (!tt->netinited) {
    int clen;
    struct sockaddr_in client_addr, server_addr;
    char buf[256];
    
    if ((tt->ssock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
      perror("socket error:");
      return -1;
    }
    
    clen = sizeof(client_addr);
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family     = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(server_port);
    
    if (bind(tt->ssock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
      perror("bind error");
      return -2;
    }
    
    printf("listen ....\n");
    if (listen(tt->ssock, 8) < 0) {
      perror("listen error:");
      return -3;
    }
    tt->csock = accept(tt->ssock, (struct sockaddr*)&client_addr, &clen);
    tt->netinited = 1;
    if (NULL==tt->pGray) {
      tt->pGray = (uchar*)malloc(720 * 576);
    }
  }
  return 0;
}

int test_send_pic_stop(ttt_t* tt) {
  close(tt->csock);
  close(tt->ssock);
  tt->netinited = 0;
  tt->csock = 0;
  tt->ssock = 0;
  free(tt->pGray);
  tt->pGray = 0;
  return 0;
}

int send_picCB(char* pData , int w , int h, int step, int param, int chID)
{
  ttt_t* tt = (ttt_t*)param;
  int size = 720*576, len=0;
  ASSERT(720==w && h==576);
  if (!tt->netinited && tt->id) {
    test_send_pic_start(tt, port[tt->id]);
  }
  if (tt->netinited) {
    chID = chID>0;
    UYVY2GRAY(h, w, pData, step, step / w, tt->pGray, w, 1, 1);
    tt->pGray[0] = tt->id;
    if ((len=send(tt->csock, tt->pGray, size, 0)) <= 0) {
      perror("send error:");
      test_send_pic_stop(tt);
    }
    printf("%d send %d bytes\n", chID, len);
  }
  return 0;
}

int test_send_pic(int argc, char* argv[])
{
  int numMCVIP;
  int debug = 0;
  ttt_t tt[2] = {0};
  int i;
  int start = 1;
  numMCVIP = 1;
  for (i=0; i<2; ++i) {
    tt[i].id = i;
  }
  // return test_iris_detect_arm();
  TVP5158Init(send_picCB, tt+0, send_picCB, tt+1);
  
  
  while (getchar()=='q') {
    if (start) {
      DISPLAY_stop();
      CAPTURE_stop();
      start = 0;
    }
    else {
      DISPLAY_start();
      CAPTURE_start();
      start = 1;
    }
  }
  
  TVP5158UnInit();
  for (i=0; i<2; ++i) {
    test_send_pic_stop(tt+i);
  }
  return 0;
}
