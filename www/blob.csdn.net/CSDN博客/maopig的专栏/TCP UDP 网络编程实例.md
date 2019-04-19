# TCP/UDP 网络编程实例 - maopig的专栏 - CSDN博客
2011年08月18日 23:43:36[maopig](https://me.csdn.net/maopig)阅读数：1632
```
1 
#include <stdlib.h>
  2 #include<stdio.h>
  3 #include<errno.h>
  4 #include<string.h>
  5 #include<netdb.h>
  6 #include<sys/types.h>
  7 #include<arpa/inet.h>
  8 #include<sys/socket.h>
  9 
 10 #define SERVER_PORT 8888
 11 #define MAX_BUF_SIZE 1024
 12 
 13 void udpc_requ(int sockfd,conststruct
sockaddr_in*addr,int len)
 14 {
 15    char buffer[MAX_BUF_SIZE];
 16    int n;
 17    while(1)
 18    {
 19       printf("please input char:\n");
 20       fgets(buffer,MAX_BUF_SIZE+1,stdin);
 21       sendto(sockfd,buffer,strlen(buffer),0,(structsockaddr
*)addr,len);
 22       bzero(buffer,MAX_BUF_SIZE);
 23    }
 24 }
 25 
 26 
 27 int main(int argc,char**argv)
 28 {
 29    int sockfd;
 30    struct sockaddr_in addr;
 31    struct hostent*host;
 32 
 33    if(argc!=2)
 34    {
 35       fprintf(stderr,"Usage:%s server_ip\n",argv[0]);
 36       exit(1);
 37    }
 38 
 39    if((host=gethostbyname(argv[1]))==NULL)
 40    {
 41       fprintf(stderr,"Gethostname error\n");
 42       exit(1);
 43    }
 44 
 45 //socket
 46    sockfd=socket(AF_INET,SOCK_DGRAM,0);
 47    if(sockfd<0)
 48    {  
 49       fprintf(stderr,"Socket Error:%s\n",strerror(errno));
 50       exit(1);
 51    }
 52 
 53 //ip port
 54    bzero(&addr,sizeof(structsockaddr_in));
 55    addr.sin_family=AF_INET;
 56    addr.sin_port=htons(SERVER_PORT);
 57    /* if(inet_aton(argv[1],&addr.sin_addr)<0)
 58    {
 59    fprintf(stderr,"IP error:%s\n",strerror(errno));
 60    exit(1);
 61    } */
 62    addr.sin_addr=*((structin_addr
*)host->h_addr);
 63 
 64 
 65    udpc_requ(sockfd,&addr,sizeof(structsockaddr_in));
 66    close(sockfd);
 67 }
```
