# TCP收发图片 - Orisun - 博客园







# [TCP收发图片](https://www.cnblogs.com/zhangchaoyang/articles/1958172.html)





recvpic.c

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<netinet/in.h>
#include<memory.h>
#include<arpa/inet.h>

#define PORT 3214
#define MAXLOG 100

main()
{
	int sockfd;
	struct sockaddr_in server,client;
	socklen_t len;
	int clientfd;
	int num;
	int MAXDATASIZE=32*1024;
	char buf[MAXDATASIZE];
	FILE *file;
	 if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
	 {
	 	perror("socket");
		exit(1);
	}
	bzero(&server,sizeof(server));
	server.sin_family=AF_INET;
	server.sin_port=htons(PORT);
	server.sin_addr.s_addr=htonl(INADDR_ANY);
	setsockopt(sockfd,SOL_SOCKET,SO_RCVBUF,(const char*)&MAXDATASIZE,sizeof(int));

	if(bind(sockfd,(struct sockaddr *)&server,sizeof(server))==-1)
	{
		perror("bind");
		close(sockfd);
		exit(2);
	}
	
	if((listen(sockfd,MAXLOG))==-1){
		perror("listen");
		close(sockfd);
		exit(1);
	}
	
	len=sizeof(client);
	if((clientfd=accept(sockfd,(struct sockaddr *)&client,&len))==-1){
		perror("accept");
		close(sockfd);
		exit(1);
	}

	if((file=fopen("/home/orisun/recvpic.jpg","wb"))==NULL){
		perror("fopen");
		close(sockfd);
		exit(1);
	}
	while(1)
	{
		num=recv(clientfd,buf,MAXDATASIZE,0);
		if(num<0)
		{
			perror("recvfrom");
			close(sockfd);
			fclose(file);
			exit(3);
		}
		fwrite(buf,1,num,file);
		if(num<MAXDATASIZE)
			break;
	}
	close(sockfd);
	fclose(file);
	return 0;
}


sendpic.c

#include<stdio.h>
#include<memory.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<sys/socket.h>

#define PORT 3214

int main()
{
	int sockfd,num;
	int MAXDATASIZE=32*1024;
	char buf[MAXDATASIZE];
	struct hostent *he;
	struct sockaddr_in server,peer;
	FILE *file;

	if((he=gethostbyname("192.168.1.111"))==NULL)
	{
		perror("getnostbyname");
		exit(2);
	}
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		perror("socket");
		exit(3);
	}

	bzero(&server,sizeof(server));
	server.sin_family=AF_INET;
	server.sin_port=htons(PORT);
	server.sin_addr=*((struct in_addr*)he->h_addr);
	setsockopt(sockfd,SOL_SOCKET,SO_SNDBUF,(const char*)&MAXDATASIZE,sizeof(int));
	
	if(connect(sockfd,(struct sockaddr*)&server,sizeof(struct sockaddr))==-1){
		perror("connect");
		close(sockfd);
		exit(1);
	}
	
	if((file=fopen("/home/orisun/Develop/Rental_Service/housepic/rental1.jpg","rb"))==NULL){
		perror("fopen");
		close(sockfd);
		exit(1);
	}
	
	bzero(buf,sizeof(buf));
	while(!feof(file)){
		num=fread(buf,1,MAXDATASIZE,file);
		if(num!=send(sockfd,buf,num,0)){
			perror("send");
			break;
		}
	}
	close(sockfd);
	fclose(file);
	return 0;
}













