
#include "net/net_w32.h"

// 服务器端
int test_server()
{
  //加载WinSock库
  WORD wVersionRequested;
  WSADATA wsaData;
  int err;
  SOCKET lSocket, aSocket;
  SOCKADDR_IN AddrSer;
  SOCKADDR_IN AddrCli;//客户端的地址信息
  int len;
  char recBuf[1024];
  char senBuf[1024];

  wVersionRequested = MAKEWORD(2, 2);
  err = WSAStartup(wVersionRequested, &wsaData);

  if (err != 0) {
    return 0;
  }

  if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
    WSACleanup();
    return 0;
  }

  AddrSer.sin_family = AF_INET;
  AddrSer.sin_port = htons(5000);
  AddrSer.sin_addr.S_un.S_addr = htonl(INADDR_ANY); //INADDR_ANY表示接受任何机器的请求
  lSocket = socket(AF_INET, SOCK_STREAM, 0);

  //绑定服务器地址
  if (bind(lSocket, (SOCKADDR*)&AddrSer, sizeof(SOCKADDR)) == SOCKET_ERROR) {
    printf("绑定套接字失败");
    return 0;
  }

  listen(lSocket, 1); //开始监听客户端的连接请求
  len = sizeof(SOCKADDR);
  aSocket = accept(lSocket, (SOCKADDR*)&AddrCli, &len); //连接成功时，产生新的socket用来与客户端通信

  while (1) {
    //TCP下为recv与send，UDP下为recefrom与sendto
    recv(aSocket, recBuf, 1024, 0);

    if (recBuf[0] == '#') {
      printf("客户端请求断开\n");
      printf("正在断开...");
      Sleep(3000);
      break;
    }

    printf("客户端：%s\n", recBuf);
    printf("服务器:");
    gets(senBuf);//warming:gets()is unsafe

    if (senBuf[0] == '#') {
      send(aSocket, senBuf, 1024, 0);
      printf("正在请求断开...\n");
      Sleep(3000);
      return 0;
    }

    //printf("服务器：%s\n",senBuf);
    send(aSocket, senBuf, 1024, 0);
    memset(recBuf, ' ', 1024);
    memset(senBuf, ' ', 1024);
  }

  closesocket(aSocket);
  closesocket(lSocket);
  WSACleanup();
  return 0;
}

int test_client()
{
  WORD wVersionRequested;
  WSADATA wsaData;
  int err;
  SOCKET cSocket;
  SOCKADDR_IN addrSer;
  char senBuf[1024];
  char recBuf[1024];

  wVersionRequested = MAKEWORD(2, 2);
  err = WSAStartup(wVersionRequested, &wsaData);

  if (err != 0) {
    return 0;
  }

  if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
    WSACleanup();
    return 0;
  }

  cSocket = socket(AF_INET, SOCK_STREAM, 0);
  addrSer.sin_family = AF_INET;
  addrSer.sin_port = htons(5000);
  addrSer.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

  if (connect(cSocket, (SOCKADDR*)&addrSer, sizeof(SOCKADDR)) == SOCKET_ERROR) {
    printf("连接服务器失败");
    return 0;
  }


  while (1) {
    printf("客户端：");
    gets(senBuf);

    if (senBuf[0] == '#') {
      send(cSocket, senBuf, 1024, 0);
      printf("正在请求断开...\n");
      Sleep(3000);
      return 0;
    }

    //printf("客户端：%s\n",senBuf);
    send(cSocket, senBuf, 1024, 0);
    recv(cSocket, recBuf, 1024, 0);

    if (recBuf[0] == '#') {
      printf("与服务器断开");
      printf("正在断开...");
      Sleep(3000);
      break;
    }

    printf("服务器：%s\n", recBuf);
    memset(recBuf, ' ', 1024);
    memset(senBuf, ' ', 1024);
  }

  closesocket(cSocket);
  WSACleanup();
  return 0;
}


#include "test_save_net_pic.h"
#define SERVER_IP "192.168.1.187"

int saferecv( SOCKET s, void* buf0, int n, int flags ) {
  int i = 0, re;
  char* buf = (char*)buf0;
  for ( i = 0; i < n; ) {
    if ( (re = recv( s, buf + i, n - i, flags )) <=0 ) {
      return re;
    }
    i += re;
  }
  
  return i;
}

int thd_save(int id) {
  SOCKADDR_IN addrSer;
  int h = 576, w = 720, cn = 1;
  int step = w * cn, size = h * step;
  int csock = socket(AF_INET, SOCK_STREAM, 0);
  uchar* recBuf = MALLOC(uchar, size);
  char buf[256];
  int frame=0;
  addrSer.sin_family = AF_INET;
  addrSer.sin_port = htons(port[id]);
  addrSer.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);
  
  printf("%d连接服务器 %s ...\n", id, SERVER_IP);
  
  if (connect(csock, (SOCKADDR*)&addrSer, sizeof(SOCKADDR)) == SOCKET_ERROR) {
    printf("%d连接服务器失败", id);
    return 0;
  }

  while (1) {
    int len=0;
    //printf("客户端：%s\n",senBuf);
    len = saferecv(csock, recBuf, size, 0);
    if (len==size) {
      _snprintf(buf, 256, "./out/pic%05d_ch%d.jpg", frame, recBuf[0]);
      imwrite4(buf, h, w, recBuf, step, cn);
      printf("%d, recv %d bytes, %s\n", id, len, buf);
    } else {
      printf("%d, error %d\n", id, len);
      break;
    }
  }
  
  closesocket(csock);
  free(recBuf);
  return 0;
}

int test_save_net_pic()
{
  WORD wVersionRequested;
  WSADATA wsaData;
  int err;
  ttt_t tt[2]={0};
  int frame = 0;
  int i;

  wVersionRequested = MAKEWORD(2, 2);
  err = WSAStartup(wVersionRequested, &wsaData);

  if (err != 0) {
    return 0;
  }

  if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
    WSACleanup();
    return 0;
  }
  
  for (i=0; i<2; ++i) {
    DWORD targetThreadID;
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thd_save, (LPVOID)i,0,&targetThreadID); 
  }
  
  while(getchar()!='q')
  {
    Sleep(1000);
  } 
  _mkdir("out");

  WSACleanup();
  return 0;
}


