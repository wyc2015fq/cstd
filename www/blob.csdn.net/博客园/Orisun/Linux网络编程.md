# Linux网络编程 - Orisun - 博客园







# [Linux网络编程](https://www.cnblogs.com/zhangchaoyang/articles/1940557.html)





先上一个简单例子吧。这一个服务器例程，只做一件：向客户端返回服务器时间：

```
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<time.h>
#include<string.h>
#include<stdlib.h>
#define PORTNUM 13000
#define HOSTLEN 256
#define oops(msg) {perror(msg);exit(1);}

int main(int argc,char *argv[]){
    struct sockaddr_in saddr;       //套接口地址
    struct hostent *hp;     //主机名称
    char hostname[HOSTLEN]; //主机名
    int sock_id,sock_fd;    //套接口描述符，一个调用socket产生的，一个是调用accept产生的
    FILE *sock_fp;          //把套接口当成流来使用
    time_t thetime;
    //请求内核产生套接口
    sock_id=socket(PF_INET,SOCK_STREAM,0);      //产生流式套接口。在创建套接口时使用PF系列宏定义
    if(sock_id==-1)
        oops("socket");
    bzero((void*)&saddr,sizeof(saddr));     //清空套接口
    gethostname(hostname,HOSTLEN);      //获取本地主机名
    hp=gethostbyname(hostname);     //根据主机名获取主机的相关信息
    bcopy((void*)hp->h_addr,(void*)&saddr.sin_addr,hp->h_length);      //获取主机地址
    saddr.sin_family=AF_INET;   //协议簇。在绑定时协议使用AF系统宏定义
    saddr.sin_port=htons(PORTNUM);  //端口
    if(bind(sock_id,(struct sockaddr*)&saddr,sizeof(saddr))!=0)     //绑定
        oops("bind");
    if(listen(sock_id,1)!=0)    //开始监听
        oops("listen");
    //主循环：accept(),write(),close()
    while(1){
        sock_fd=accept(sock_id,NULL,NULL);
        printf("Wow!got a call!\n");
        if(sock_fd==-1)
            oops("accept");
        sock_fp=fdopen(sock_fd,"w");
        if(sock_fp==NULL)
            oops("fdopen");
        thetime=time(NULL);     //获取当前时间
        fprintf(sock_fp,"The time here is ..");
        fprintf(sock_fp,"%s",ctime(&thetime));
        fclose(sock_fp);
    }
}
```

想测试这个程序只需要自己telnet到自己就可以了，注意telnet自己时还要指定端口号为PORTNUM

$ gcc timesrv.c -o timesrv
$ ./timesrv &
[4] 4367
$ telnet `hostname` 13000
Trying 127.0.1.1...
Wow!got a call!
Connected to zcypc.
Escape character is '^]'.
The time here is ..Sun Dec 25 10:53:13 2011
Connection closed by foreign host.




TCP/IP四层的功能：

• 网络接口层:负责将二进制流转换为数据帧,并进行数据帧的发送和接收。要注意的 是数据帧是独立的网络信息传输单元。

• 网络层:负责将数据帧封装成 IP 数据报,并运行必要的路由算法。

• 传输层:
负责端对端之间的通信会话连接与建立。 传输协议的选择根据数据传输方式 而定。

• 应用层:负责应用程序的网络访问,这里通过端口号来识别各个不同的进程。

像ARP（用于获得同一物理网络中的硬件主机地址）工作在网络接口层，而IPv4、IPv6则工作在网络层。



TCP数据报头部分字段：

• 序号:32
位长。标识发送的数据报的顺序。

• 确认号:32
位长。希望收到的下一个数据报的序列号。

• TCP
头长:4 位长。表明 TCP 头中包含多少个 32
位字。

• 窗口大小:16
位长。窗口大小字段表示在确认了字节之后还可以发送多少个字节。





typedef
uint32_t in_addr_t;

struct
in_addr{

	　　in_addr_t
 s_addr;

}

struct
sockaddr {


	　　unsigned
short sa_family; /*地址族*/


	　　char
sa_data[14]; /*14 字节的协议地址,包含该 socket 的 IP
地址和端口号。*/


};


struct
sockaddr_in {


	　　short
int sin_family; /*地址族*/


	　　unsigned
short int sin_port; /*端口号*/


	　　struct
in_addr sin_addr; /*IP 地址*/


	　　unsigned
char sin_zero[8]; /*填充 0
以保持与 struct sockaddr
同样大小*/


};


sockaddr_in
和sockaddr
两个数据类型是等效的,可以相互转化,通常 sockaddr_in 数据类型使用更为方便。



计算机数据存储有两种字节优先顺序:
高位字节优先和低位字节优先。Internet
上数据以 高位字节优先顺序在网络上传输，在计算机上相反。

uint16_t
htons(unit16_t host16bit)


uint32_t
htonl(unit32_t host32bit)


uint16_t
ntohs(unit16_t net16bit)


uint32_t
ntohs(unit32_t net32bit)




struct
hostent{


	　　char
*h_name;/*正式主机名*/


	　　char
**h_aliases;/*主机别名*/


	　　int
h_addrtype;/*地址类型*/


	　　int
h_length;/*地址长度*/


	　　char
**h_addr_list;/*指向 IPv4 或 IPv6 的地址指针数组*/


　　char
h_addr;/*指向 IPv4 或 IPv6
的地址指针*/

}


#include
<netdb.h>


函数原型 Struct
hostent *gethostbyname(const char *hostname)


函数传入值 Hostname:主机名，实际上就是形如“127.0.0.1”的字符串

函数返回值

成功:hostent
类型指针

出错:−1


在send()的时候，返回的是实际发送出去的字节(同步)或发送到socket缓冲区的字节(异步);系统默认的状态发送和接收一次为8688字节(约为8.5K)；在实际的过程中发送数据和接收数据量比较大，可以设置socket缓冲区，而避免了send(),recv()不断的循环收发：

// 接收缓冲区

int nRecvBuf=32*1024;//设置为32K

setsockopt(sockfd,SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(int));

//发送缓冲区

int nSendBuf=32*1024;//设置为32K

setsockopt(sockfd,SOL_SOCKET,SO_SNDBUF,(const char*)&nSendBuf,sizeof(int));



TCP/IP服务端例程：

int
socket(int family, int type, int protocol)
 	//protocol一般设为0

int
bind(int sockfd,struct sockaddr *server_addr,
int server_addr_len)


int
listen(int sockfd, int backlog)
	//backlog:请求队列中允许的最大请求数,大多数系统缺省值为 20


int
accept(int sockfd, struct sockaddr *client_addr,
socklen_t *client_addr_len)


int
recv(int sockfd,void *buf,int len,unsigned int flags)
	// flags一般设为0

int
close(int sockfd)	//在头文件unistd.h中



TCP/IP客户端例程：

int
socket(int family, int type, int protocol)
 	//protocol一般设为0

int
connect(int sockfd, struct sockaddr *serv_addr,
int servaddrlen)


int
send(int sockfd, const void *msg, int len, int flags)
	// flags一般设为0

int
close(int sockfd)	//在头文件unistd.h中



下面我们来写一个完整的程序



#include<stdio.h>		/*perror(),printf()*/

#include<stdlib.h>		/*exit()*/

#include<unistd.h>		/*close()*/

#include<netinet/in.h>	/*struct
sockaddr_in,struct in_addr*/

#include<sys/socket.h>	/*socket(),bind(),listen(),accept(),recv()*/

#include<string.h>　　/*bzero()*/



#define
PORT 1500

#define
MAXLOG 10

#define
MAXRECV 100



int
main()

{

	struct
sockaddr_in serv_sock,client_sock;	//程序员并不需要设置 client_sock， client_sock作为参数传入accept（）后被赋值

	int
sockfd;				//服务端套接字描述符

	int
clientfd;				//客户端套接字描述符

	socklen_t
addlen;			//客户端套接字长度

	char
buff[MAXRECV];		//接收到的数据

	int
recvlen;				//接收到的数据的实际长度



	serv_sock.sin_family=AF_INET;		//
AF_INET是IPv4一族

	serv_sock.sin_port=htons(PORT);		//把计算机的字节序转换为网络的字节序，因端口号是16位，所以用 htons

serv_sock.sin_addr.s_addr=INADDR_ANY;	//
INADDR_ANY可以自动识别本机IP，并且它是一个网络字节序的in_addr_t类型，不需要再使用htonl转换了

	bzero(&(serv_sock.sin_zero),8);



	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1){	//
SOCK_STREAM表示用TCP/IP

		perror(“socket”);

		exit(1);

	}



	if(bind(sockfd,(struct
sockaddr*)&serv_sock,sizeof(serv_sock))==-1){		//显式地把strcut
sockaddr_in*转换为struct
sockaddr*,这里用sizeof(serv_sock)和sizeof(struct
sockaddr)是等价的

perror(“bind”);

		exit(1);

	}



	if(listen(sockfd,
MAXLOG)==-1){

		perror(“listen”);

		exit(1);

	}



	addlen=sizeof(client_sock);

	if((clientfd=accept(sockfd,(struct
sockaddr*)&client_sock,&addlen))==-1){
		perror(“accept”);

		exit(1);

	}

	if((recvlen=recv(clientfd,buff,
MAXRECV,0))==-1){

		perror(“recv”);

		exit(1);

	}

	buff[recvlen]='\0';

	printf(“Receive
data=%s,length=%d\n”,buff,recvlen);

	close(sockfd);

	return
0;

}



#include<stdio.h>		/*perror(),printf()*/

#include<stdlib.h>		/*exit()*/

#include<unistd.h>		/*close()*/

#include<netinet/in.h>	/*struct
sockaddr_in,struct in_addr*/

#include<sys/socket.h>	/*socket(),connect(),send()*/	

#include<netdb.h>		/*gethostbyname(),struct
hostent*/



#include<string.h>　　/*bzero()*/




#define
PORT 1500		/*服务端和客户端需要在同一端口上建立连接*/

#define
SENDLEN 100	/*要发送的最大字节数*/



int
main()

{

	struct
sockaddr_in serv_sock;

	int
sockfd;

	struct
hostent *host=gethostbyname(“127.0.0.1”);

	char
buff[SENDLEN]=”orisun”;	//后面93个全部以'\0'填充，sizeof(buff)=100

	int
len;			//实际发送的字节数



	
serv_sock.sin_family=AF_INET;

	
serv_sock.sin_port=htons(PORT);

	
serv_sock.sin_addr=*((struct
in_addr*)host->h_addr);		//注意为能写成(struct
in_addr)host->h_addr，即不能把
host->h_addr（char*类型）直接转换为
struct in_addr类型



bzero(&(serv_sock.sin_zero),8);



	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1){

		perror(“socket”);

		exit(1);

	}



	if(connect(sockfd,(struct
sockaddr*)&serv_sock,sizeof(struct sockaddr))==-1){

		perror(“connect”);

		exit(1);

	}



	if((len=send(sockfd,buff,sizeof(buff),0))==-1){	

		perror(“send”);

		exit(1);

	}



	printf(“send
success\n”);

	close(sockfd);

	return
0;

}



发生Program
received signal SIGPIPE, Broken pipe.错误是因为服务端没打开。

bind()发生Address
already in use错误,用netstat
-a -n | grep 1500发现该端口已经在使用，再换一个大一点的端口解决问题。

**UDP传输真是不可靠**

我设置一次最大传送32K，传送31K和47K的图片都没问题，但传个260K的图片就出现这种情况：

1是数据大量丢失，2是到达的次序发生了混乱。

发送的图片：

![](https://pic002.cnblogs.com/images/2011/103496/2011021815432570.jpg)

接收到的图片：

![](https://pic002.cnblogs.com/images/2011/103496/2011021815434235.jpg)
















