
#include <string.h>
#include "net.h"

int test_udp()
{
  int port = 4545;
  int aa = 0, i;
  int times = 10;
  netinit();
#ifdef WIN32
  aa = !aa;
#endif

  if (aa) {
    int sock;
    struct sockaddr_in addr;
    char buff[512];
    struct sockaddr_in clientAddr;
    int n;
    int len = sizeof(clientAddr);

    printf("Welcome! This is a UDP server, I can only received message from client and reply with same message\n");

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
      perror("socket");
      return 0;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
      perror("bind");
      return 0;
    }

    //通过套接字获取IP、Port等地址信息
    for (i = 0; i < times; ++i) {
      n = recvfrom(sock, buff, 511, 0, (struct sockaddr*)&clientAddr, &len);

      if (n > 0) {
        buff[n] = 0;
        printf("%s %u says: %s\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), buff);
        n = sendto(sock, buff, n, 0, (struct sockaddr*)&clientAddr, sizeof(clientAddr));

        if (n < 0) {
          perror("sendto");
          break;
        }
      }
      else {
        perror("recv");
        break;
      }

      sleep(100);
    }
  }
  else {
    struct sockaddr_in addr;
    int sock;
    char buff[512];
    int len = sizeof(addr);
    char* ip;
    ip = "192.168.1.3";
#ifdef WIN32
    ip = "192.168.16.139";
#endif
    printf("This is a UDP client\n");

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
      perror("socket");
      return 0;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);

    if (addr.sin_addr.s_addr == INADDR_NONE) {
      printf("Incorrect ip address!");
      closesocket(sock);
      return 0;
    }

    for (i = 0; i < times; ++i) {
      int n;
      snprintf(buff, 256, "ip:%s:%d:%d", ip, port, i);
      n = sendto(sock, buff, strlen(buff), 0, (struct sockaddr*)&addr, sizeof(addr));

      if (n < 0) {
        perror("sendto");
        closesocket(sock);
        break;
      }

      n = recvfrom(sock, buff, 512, 0, (struct sockaddr*)&addr, &len);

      if (n > 0) {
        buff[n] = 0;
        printf("received:%s\n", buff);
      }
      else if (n == 0) {
        printf("server closed\n");
        closesocket(sock);
        break;
      }
      else if (n == -1) {
        perror("recvfrom");
        closesocket(sock);
        break;
      }

      sleep(100);
    }

  }

  netexit();
  return 0;
}
