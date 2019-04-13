
# C-C++ 获得公网ip地址和内网ip - 高科的专栏 - CSDN博客

2014年12月26日 08:48:27[高科](https://me.csdn.net/pbymw8iwm)阅读数：19230


**获得公网ip：**
```python
bool getPublicIp(string& ip)
{
    int    sock;
    char **pptr = NULL;
    struct sockaddr_in    destAddr;
    struct hostent    *ptr = NULL;
    char destIP[128];
    sock = socket(AF_INET,SOCK_STREAM,0);
    if( -1 == sock ){
        perror("creat socket failed");
        return false;
    }
    bzero((void *)&destAddr,sizeof(destAddr));
    destAddr.sin_family = AF_INET;
    destAddr.sin_port = htons(80);
    ptr = gethostbyname("www.ip138.com");
    if(NULL == ptr){
        perror("gethostbyname error");
        return false;
    }
    for(pptr=ptr->h_addr_list ; NULL != *pptr ; ++pptr){
        inet_ntop(ptr->h_addrtype,*pptr,destIP,sizeof(destIP));
        printf("addr:%s\n",destIP);
        ip = destIP;
        return true;
    }
    return true;
}
```
获得内网ip：
```python
int getlocalip(char* outip)
{
#ifndef WIN32
	int i=0;
	int sockfd;
	struct ifconf ifconf;
	char buf[512];
	struct ifreq *ifreq;
	char* ip;
	//初始化ifconf
	ifconf.ifc_len = 512;
	ifconf.ifc_buf = buf;
	strcpy(outip,"127.0.0.1");
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0))<0)
	{
		return -1;
	}
	ioctl(sockfd, SIOCGIFCONF, &ifconf);    //获取所有接口信息
	close(sockfd);
	//接下来一个一个的获取IP地址
	ifreq = (struct ifreq*)buf;
	for(i=(ifconf.ifc_len/sizeof(struct ifreq)); i>0; i--)
	{
		ip = inet_ntoa(((struct sockaddr_in*)&(ifreq->ifr_addr))->sin_addr);

		if(strcmp(ip,"127.0.0.1")==0)  //排除127.0.0.1，继续下一个
		{
			ifreq++;
			continue;
		}
	}
	strcpy(outip,ip);
	return 0;
#else
	return 0;
#endif
	
}
```



