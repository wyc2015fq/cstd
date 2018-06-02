
#define PORT 6666
int test_tcp_server1()
{
  int ser_sockfd,cli_sockfd;  
  int err,n;  
  int addlen;  
  struct sockaddr_in ser_addr;  
  struct sockaddr_in cli_addr;  
  char recvline[200],sendline[200];  
  sock_init();
  
  ser_sockfd=socket(AF_INET,SOCK_STREAM,0);  
  if(ser_sockfd==-1)  
  {  
    printf("socket error:%s\n",strerror(errno));  
    return -1;  
  }  
  
  bzero(&ser_addr,sizeof(ser_addr));  
  ser_addr.sin_family=AF_INET;  
  ser_addr.sin_addr.s_addr=htonl(INADDR_ANY);  
  ser_addr.sin_port=htons(PORT);  
  err=bind(ser_sockfd,(struct sockaddr *)&ser_addr,sizeof(ser_addr));  
  if(err==-1)  
  {  
    printf("bind error:%s\n",strerror(errno));  
    return -1;  
  }  
  
  err=listen(ser_sockfd,5);  
  if(err==-1)  
  {  
    printf("listen error\n");  
    return -1;  
  }  
  
  printf("listen the port:\n");  
  
  while(1)  
  {     
    addlen=sizeof(struct sockaddr);  
    cli_sockfd=accept(ser_sockfd,(struct sockaddr *)&cli_addr,&addlen);  
    if(cli_sockfd==-1)  
    {  
      printf("accept error\n");  
    }  
    while(1)  
    {  
      printf("waiting for client...\n");  
      n=recv(cli_sockfd,recvline,1024,0);  
      if(n==-1)  
      {  
        printf("recv error\n");
        n = 0;
      }  
      recvline[n]='\0';  
      
      printf("recv data is:%s\n",recvline);  
      
      printf("Input your words:");  
      scanf("%s",&sendline);  
      send(cli_sockfd,sendline,strlen(sendline),0);  
    }  
    close(cli_sockfd);  
  }  
  
  close(ser_sockfd);  
  
  return 0;  
}

int test_tcp_client1()
{  
  int sockfd;  
  int err,n;  
  struct sockaddr_in addr_ser;  
  char sendline[20],recvline[20];  
  sock_init();
  sockfd=socket(AF_INET,SOCK_STREAM,0);  
  if(sockfd==-1)  
  {  
    printf("socket error\n");  
    return -1;  
  }  
  
  bzero(&addr_ser,sizeof(addr_ser));  
  addr_ser.sin_family=AF_INET;
  addr_ser.sin_addr.s_addr=htonl(INADDR_ANY);
  addr_ser.sin_addr.s_addr= inet_addr("127.0.0.1");
  addr_ser.sin_port=htons(PORT);
  err=connect(sockfd,(struct sockaddr *)&addr_ser,sizeof(addr_ser));
  if(err==-1)
  {
    printf("connect error\n");
    return -1;
  }
  
  printf("connect with server...\n");
  
  while(1)
  {
    printf("Input your words:");
    scanf("%s",&sendline);
    
    send(sockfd,sendline,strlen(sendline),0);
    
    printf("waiting for server...\n");
    
    n=recv(sockfd,recvline,100,0);
    recvline[n]='\0';
    printf("recv data is:%s\n",recvline);  
  }
  return 0;  
}