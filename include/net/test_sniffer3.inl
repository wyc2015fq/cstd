#include <winsock2.h> /*windows socket的头文件，系统定义的*/
#include <windows.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#pragma comment(lib,"ws2_32.lib") /*链接API相关连的Ws2_32.lib静态库*/
#define MAX_HOSTNAME_LAN 255
#define SIO_RCVALL _WSAIOW(IOC_VENDOR,1)
#define MAX_ADDR_LEN 16
struct ipheader {
  unsigned char ip_hl: 4; /*header length(报头长度）*/
  unsigned char ip_v: 4; /*version(版本)*/
  unsigned char ip_tos; /*type os service服务类型*/
  unsigned short int ip_len; /*total length (总长度)*/
  unsigned short int ip_id; /*identification (标识符)*/
  unsigned short int ip_off; /*fragment offset field(段移位域)*/
  unsigned char ip_ttl; /*time to live (生存时间)*/
  unsigned char ip_p; /*protocol(协议)*/
  unsigned short int ip_sum; /*checksum(校验和)*/
  unsigned int ip_src; /*source address(源地址)*/
  unsigned int ip_dst; /*destination address(目的地址)*/
}; /* total ip header length: 20 bytes (=160 bits) */
typedef struct tcpheader {
  unsigned short int sport; /*source port (源端口号)*/
  unsigned short int dport; /*destination port(目的端口号)*/
  unsigned int th_seq; /*sequence number(包的序列号)*/
  unsigned int th_ack; /*acknowledgement number(确认应答号)*/
  unsigned char th_x: 4; /*unused(未使用)*/
  unsigned char th_off: 4; /*data offset(数据偏移量)*/
  unsigned char Flags; /*标志全*/
  unsigned short int th_win; /*windows(窗口)*/
  unsigned short int th_sum; /*checksum(校验和)*/
  unsigned short int th_urp; /*urgent pointer(紧急指针)*/
} TCP_HDR;
typedef struct udphdr {
  unsigned short sport; /*source port(源端口号)*/
  unsigned short dport; /*destination port(目的端口号)*/
  unsigned short len; /*udp length(udp长度)*/
  unsigned short cksum; /*udp checksum(udp校验和)*/
} UDP_HDR;
void main()
{
  SOCKET sock;
  WSADATA wsd;
  DWORD dwBytesRet;
  unsigned int optval = 1;
  unsigned char* dataudp, *datatcp;
  int i, pCount = 0, lentcp, lenudp;
  SOCKADDR_IN sa, saSource, saDest;
  struct hostent FAR* pHostent;
  char FAR name[MAX_HOSTNAME_LAN];
  char szSourceIP[MAX_ADDR_LEN], szDestIP[MAX_ADDR_LEN], RecvBuf[65535] = {0};
  struct udphdr* pUdpheader;
  struct ipheader* pIpheader;
  struct tcpheader* pTcpheader;
  WSAStartup(MAKEWORD(2, 1), &wsd);
  if ((sock = socket(AF_INET, SOCK_RAW, IPPROTO_IP)) == SOCKET_ERROR) {
    exit(1);
  }
  gethostname(name, MAX_HOSTNAME_LAN);
  pHostent = gethostbyname(name);
  sa.sin_family = AF_INET;
  sa.sin_port = htons(6000);
  memcpy(&sa.sin_addr.S_un.S_addr, pHostent->h_addr_list[0], pHostent->h_length);
  bind(sock, (SOCKADDR*)&sa, sizeof(sa)); /*bind()设定自己主机的IP地址和端口号*/
  if ((WSAGetLastError()) == 10013) {
    exit(1);
  }
  WSAIoctl(sock, SIO_RCVALL, &optval, sizeof(optval), NULL, 0, &dwBytesRet, NULL, NULL);
  pIpheader = (struct ipheader*)RecvBuf;
  pTcpheader = (struct tcpheader*)(RecvBuf + sizeof(struct ipheader));
  pUdpheader = (struct udphdr*)(RecvBuf + sizeof(struct ipheader));
  while (1) {
    memset(RecvBuf, 0, sizeof(RecvBuf));
    recv(sock, RecvBuf, sizeof(RecvBuf), 0);
    saSource.sin_addr.s_addr = pIpheader->ip_src;
    strncpy(szSourceIP, inet_ntoa(saSource.sin_addr), MAX_ADDR_LEN);
    saDest.sin_addr.s_addr = pIpheader->ip_dst;
    strncpy(szDestIP, inet_ntoa(saDest.sin_addr), MAX_ADDR_LEN);
    lentcp = (ntohs(pIpheader->ip_len) - (sizeof(struct ipheader) + sizeof(struct tcpheader)));
    lenudp = (ntohs(pIpheader->ip_len) - (sizeof(struct ipheader) + sizeof(struct udphdr)));
    if ((pIpheader->ip_p) == IPPROTO_TCP && lentcp != 0) {
      printf("*******************************************\n");
      pCount++;
      datatcp = (unsigned char*) RecvBuf + sizeof(struct ipheader) + sizeof(struct tcpheader);
      printf("-TCP-\n");
      printf("\n目的IP地址：%s\n", szDestIP);
      printf("\n目的端口：%i\n", ntohs(pTcpheader->dport));
      printf("datatcp address->%x\n", datatcp);
      printf("size of ipheader->%i\n", sizeof(struct ipheader));
      printf("size of tcpheader->%i\n", sizeof(struct tcpheader));
      printf("size of the hole packet->%i\n", ntohs(pIpheader->ip_len));
      printf("\nchar Packet%i [%i]=\"", pCount, lentcp - 1);
      for (i = 0; i < lentcp; i++) {
        printf("\\x%.2x", *(datatcp + i));
        if (i % 10 == 0) {
          printf("\"\n\"");
        }
      }
      printf("\";\n\n\n");
      for (i = 0; i < lentcp; i++) {
        if (*(datatcp + i) <= 127 && *(datatcp + i) >= 20) {
          printf("%c", *(datatcp + i));
        }
        else {
          printf(".");
        }
      }
      printf("\n\n*******************************************\n");
    }
    if ((pIpheader->ip_p) == IPPROTO_UDP && lentcp != 0) {
      pCount++;
      dataudp = (unsigned char*) RecvBuf + sizeof(struct ipheader) + sizeof(struct udphdr);
      printf("-UDP-\n");
      printf("\n目的IP地址：%s\n", szDestIP);
      printf("\n目的端口：%d\n", ntohs(pTcpheader->dport));
      printf("UDP数据地址：%x\n", dataudp);
      printf("IP头部长度：%i\n", sizeof(struct ipheader));
      printf("UDP头部长度：%i\n", sizeof(struct udphdr));
      printf("包的大小：%i\n", ntohs(pIpheader->ip_len));
      printf("\nchar Packet%i [%i]=\"", pCount, lenudp - 1);
      for (i = 0; i < lenudp; i++) {
        printf("\\x%.2x", *(dataudp + i));
        if (i % 10 == 0) {
          printf("\"\n\"");
        }
      }
      printf("\";\n\n\n");
      for (i = 0; i < lenudp; i++) {
        if (*(dataudp + i) <= 127 && *(dataudp + i) >= 20) {
          printf("%c", *(dataudp + i));
        }
        else {
          printf(".");
        }
      }
      printf("\n\n*******************************************\n");
    }
  }
}

