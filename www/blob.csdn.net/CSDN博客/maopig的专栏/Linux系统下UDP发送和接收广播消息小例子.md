# Linux系统下UDP发送和接收广播消息小例子 - maopig的专栏 - CSDN博客
2013年09月23日 16:37:09[maopig](https://me.csdn.net/maopig)阅读数：1754
个人分类：[Linux](https://blog.csdn.net/maopig/article/category/850267)
分类： [网络通信](http://blog.csdn.net/robertkun/article/category/1365528)2013-01-07 10:54
1336人阅读[评论](http://blog.csdn.net/robertkun/article/details/8475843#comments)(6)[收藏]()[举报](http://blog.csdn.net/robertkun/article/details/8475843#report)
**[cpp]**[view plain](http://blog.csdn.net/robertkun/article/details/8475843#)[copy](http://blog.csdn.net/robertkun/article/details/8475843#)[print](http://blog.csdn.net/robertkun/article/details/8475843#)[?](http://blog.csdn.net/robertkun/article/details/8475843#)
- // 发送端 
- #include <iostream> 
- #include <stdio.h> 
- #include <sys/socket.h> 
- #include <unistd.h> 
- #include <sys/types.h> 
- #include <netdb.h> 
- #include <netinet/in.h> 
- #include <arpa/inet.h> 
- #include <string.h> 
- 
- 
- usingnamespace std;  
- 
- int main()  
- {  
-     setvbuf(stdout, NULL, _IONBF, 0);   
-     fflush(stdout);   
- 
- int sock = -1;  
- if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)   
-     {     
-         cout<<"socket error"<<endl;   
- returnfalse;  
-     }     
- 
- constint opt = 1;  
- //设置该套接字为广播类型， 
- int nb = 0;  
-     nb = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&opt, sizeof(opt));  
- if(nb == -1)  
-     {  
-         cout<<"set socket error..."<<endl;  
- returnfalse;  
-     }  
- 
- struct sockaddr_in addrto;  
-     bzero(&addrto, sizeof(struct sockaddr_in));  
-     addrto.sin_family=AF_INET;  
-     addrto.sin_addr.s_addr=htonl(INADDR_BROADCAST);  
-     addrto.sin_port=htons(6000);  
- int nlen=sizeof(addrto);  
- 
- while(1)  
-     {  
-         sleep(1);  
- //从广播地址发送消息 
- char smsg[] = {"abcdef"};  
- int ret=sendto(sock, smsg, strlen(smsg), 0, (sockaddr*)&addrto, nlen);  
- if(ret<0)  
-         {  
-             cout<<"send error...."<<ret<<endl;  
-         }  
- else
-         {         
-             printf("ok ");    
-         }  
-     }  
- 
- return 0;  
- }  
```cpp
// 发送端
#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
using namespace std;
int main()
{
	setvbuf(stdout, NULL, _IONBF, 0); 
	fflush(stdout); 
	int sock = -1;
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
	{   
		cout<<"socket error"<<endl;	
		return false;
	}   
	
	const int opt = 1;
	//设置该套接字为广播类型，
	int nb = 0;
	nb = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&opt, sizeof(opt));
	if(nb == -1)
	{
		cout<<"set socket error..."<<endl;
		return false;
	}
	struct sockaddr_in addrto;
	bzero(&addrto, sizeof(struct sockaddr_in));
	addrto.sin_family=AF_INET;
	addrto.sin_addr.s_addr=htonl(INADDR_BROADCAST);
	addrto.sin_port=htons(6000);
	int nlen=sizeof(addrto);
	while(1)
	{
		sleep(1);
		//从广播地址发送消息
		char smsg[] = {"abcdef"};
		int ret=sendto(sock, smsg, strlen(smsg), 0, (sockaddr*)&addrto, nlen);
		if(ret<0)
		{
			cout<<"send error...."<<ret<<endl;
		}
		else
		{		
			printf("ok ");	
		}
	}
	return 0;
}
```
**[cpp]**[view plain](http://blog.csdn.net/robertkun/article/details/8475843#)[copy](http://blog.csdn.net/robertkun/article/details/8475843#)[print](http://blog.csdn.net/robertkun/article/details/8475843#)[?](http://blog.csdn.net/robertkun/article/details/8475843#)
- // 接收端 http://blog.csdn.net/robertkun
- 
- #include <iostream> 
- #include <stdio.h> 
- #include <sys/socket.h> 
- #include <unistd.h> 
- #include <sys/types.h> 
- #include <netdb.h> 
- #include <netinet/in.h> 
- #include <arpa/inet.h> 
- #include <string.h> 
- 
- 
- usingnamespace std;  
- 
- int main()  
- {  
-     setvbuf(stdout, NULL, _IONBF, 0);   
-     fflush(stdout);   
- 
- // 绑定地址 
- struct sockaddr_in addrto;  
-     bzero(&addrto, sizeof(struct sockaddr_in));  
-     addrto.sin_family = AF_INET;  
-     addrto.sin_addr.s_addr = htonl(INADDR_ANY);  
-     addrto.sin_port = htons(6000);  
- 
- // 广播地址 
- struct sockaddr_in from;  
-     bzero(&from, sizeof(struct sockaddr_in));  
-     from.sin_family = AF_INET;  
-     from.sin_addr.s_addr = htonl(INADDR_ANY);  
-     from.sin_port = htons(6000);  
- 
- int sock = -1;  
- if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)   
-     {     
-         cout<<"socket error"<<endl;   
- returnfalse;  
-     }     
- 
- constint opt = 1;  
- //设置该套接字为广播类型， 
- int nb = 0;  
-     nb = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&opt, sizeof(opt));  
- if(nb == -1)  
-     {  
-         cout<<"set socket error..."<<endl;  
- returnfalse;  
-     }  
- 
- if(bind(sock,(struct sockaddr *)&(addrto), sizeof(struct sockaddr_in)) == -1)   
-     {     
-         cout<<"bind error..."<<endl;  
- returnfalse;  
-     }  
- 
- int len = sizeof(sockaddr_in);  
- char smsg[100] = {0};  
- 
- while(1)  
-     {  
- //从广播地址接受消息 
- int ret=recvfrom(sock, smsg, 100, 0, (struct sockaddr*)&from,(socklen_t*)&len);  
- if(ret<=0)  
-         {  
-             cout<<"read error...."<<sock<<endl;  
-         }  
- else
-         {         
-             printf("%s\t", smsg);     
-         }  
- 
-         sleep(1);  
-     }  
- 
- return 0;  
- }  
```cpp
// 接收端 http://blog.csdn.net/robertkun
#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
using namespace std;
int main()
{
	setvbuf(stdout, NULL, _IONBF, 0); 
	fflush(stdout); 
	// 绑定地址
	struct sockaddr_in addrto;
	bzero(&addrto, sizeof(struct sockaddr_in));
	addrto.sin_family = AF_INET;
	addrto.sin_addr.s_addr = htonl(INADDR_ANY);
	addrto.sin_port = htons(6000);
	
	// 广播地址
	struct sockaddr_in from;
	bzero(&from, sizeof(struct sockaddr_in));
	from.sin_family = AF_INET;
	from.sin_addr.s_addr = htonl(INADDR_ANY);
	from.sin_port = htons(6000);
	
	int sock = -1;
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
	{   
		cout<<"socket error"<<endl;	
		return false;
	}   
	const int opt = 1;
	//设置该套接字为广播类型，
	int nb = 0;
	nb = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&opt, sizeof(opt));
	if(nb == -1)
	{
		cout<<"set socket error..."<<endl;
		return false;
	}
	if(bind(sock,(struct sockaddr *)&(addrto), sizeof(struct sockaddr_in)) == -1) 
	{   
		cout<<"bind error..."<<endl;
		return false;
	}
	int len = sizeof(sockaddr_in);
	char smsg[100] = {0};
	while(1)
	{
		//从广播地址接受消息
		int ret=recvfrom(sock, smsg, 100, 0, (struct sockaddr*)&from,(socklen_t*)&len);
		if(ret<=0)
		{
			cout<<"read error...."<<sock<<endl;
		}
		else
		{		
			printf("%s\t", smsg);	
		}
		sleep(1);
	}
	return 0;
}
```
自已在Linux虚拟机下测试可以成功, 前提是要把主机设置在同一网段内, 还有就是不要忘记关闭Linux的防火墙.. 可以使用setup命令进行设置。
(我在测试的时候只能发送不收接收，折磨了我半天，后来才想到是Linux防火墙的问题。。)
关于虚拟机的网卡配置，建议选择桥接模式。NAT的模式的话，是受限制的，可能会收不到广播消息。
具体的参考网上的文章吧。。
祝你成功。。
