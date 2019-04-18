# Linux网络编程 - ljx0305的专栏 - CSDN博客
2009年03月26日 08:56:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：670标签：[网络																[linux																[include																[buffer																[socket																[服务器](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=socket&t=blog)](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)](https://so.csdn.net/so/search/s.do?q=include&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=网络&t=blog)
个人分类：[Linux程序及使用](https://blog.csdn.net/ljx0305/article/category/394700)
Linux网络编程
2. 初等网络函数介绍（TCP）
 Linux系统是通过提供套接字(socket)来进行网络编程的.网络程序通过
socket和其它几个函数的调用,会返回一个 通讯的文件描述符, 我们可以将这
个描述符看成普通的文件的描述符来操作,这就是linux的设备无关性的 好处.
我们可以通过向描述符读写操作实现网络之间的数据交流. 
2.1 socket 
int socket(int domain, int type,int protocol) 
  domain: 说明我们网络程序所在的主机采用的通讯协族(AF_UNIX和AF_INET等).
 AF_UNIX只能够用于单一的Unix系统进程间通信,而 AF_INET是针对Internet的,
因而可以允许在远程 主机之间通信(当我们 man socket时发现 domain可选项是
 PF_*而不是AF_*,因为glibc是posix的实现 所以用PF代替了AF,不过我们都可以
使用的). 
  type:我们网络程序所采用的通讯协议(SOCK_STREAM,SOCK_DGRAM等)
 SOCK_STREAM表明我们用的是TCP协议,这样会提供按顺序的,可靠,双向, 面向
连接的比特流. SOCK_DGRAM 表明我们用的是UDP协议,这样只会提供定长的,不可靠,
无连接的通信. 
  protocol:由于我们指定了type,所以这个地方我们一般只要用0来代替就可以
了 socket为网络通讯做基本的准备.成功时返回文件描述符,失败时返回-1,
看errno可知道出错的详细情况. 
2.2 bind 
int bind(int sockfd, struct sockaddr *my_addr, int addrlen) 
  sockfd:是由socket调用返回的文件描述符. 
  addrlen:是sockaddr结构的长度. 
  my_addr:是一个指向sockaddr的指针. 在中有 sockaddr的定义 
        struct sockaddr{
                unisgned short  as_family;
                char            sa_data[14];
                };
  不过由于系统的兼容性,我们一般不用这个头文件,而使用另外一个结构
(struct sockaddr_in) 来代替.在中有sockaddr_in的定义 
        struct sockaddr_in{
                unsigned short          sin_family;     
                unsigned short int      sin_port;
                struct in_addr          sin_addr;
                unsigned char           sin_zero[8];
   我们主要使用Internet所以sin_family一般为AF_INET,sin_addr设置为INADDR_ANY
表示可以 和任何的主机通信, sin_port是我们要监听的端口号.sin_zero[8]是用来
填充的. bind将本地的端口同socket返回的文件描述符捆绑在一起.成功是返回0,
失败的情况和socket一样 
2.3 listen 
int listen(int sockfd,int backlog) 
sockfd:是bind后的文件描述符. 
backlog:设置请求排队的最大长度.当有多个客户端程序和服务端相连时, 
使用这个表示可以介绍的排队长度. listen函数将bind的文件描述符变为
监听套接字.返回的情况和bind一样. 
2.4 accept 
int accept(int sockfd, struct sockaddr *addr,int *addrlen) 
sockfd:是listen后的文件描述符. 
addr, addrlen是用来给客户端的程序填写的,服务器端只要传递指针就可以了.
 bind,listen和accept是服务器端用的函数,accept调用时,服务器端的程序
会一直阻塞到有一个 客户程序发出了连接. accept成功时返回最后的服务器端
的文件描述符,这个时候服务器端可以向该描述符写信息了. 失败时返回-1 
2.5 connect 
int connect(int sockfd, struct sockaddr * serv_addr,int addrlen) 
sockfd:socket返回的文件描述符. 
serv_addr:储存了服务器端的连接信息.其中sin_add是服务端的地址 
addrlen:serv_addr的长度 
connect函数是客户端用来同服务端连接的.成功时返回0,sockfd
是同服务端通讯的文件描述符 失败时返回-1. 
2.6 实例 
服务器端程序
/******* 服务器程序  (server.c) ************/
＃i nclude 
＃i nclude 
＃i nclude 
＃i nclude 
＃i nclude 
＃i nclude 
＃i nclude 
＃i nclude 
int main(int argc, char *argv[])
{
 int sockfd,new_fd;
 struct sockaddr_in server_addr;
 struct sockaddr_in client_addr;
 int sin_size,portnumber;
 char hello[]="Hello! Are You Fine?/n";
 if(argc!=2)
 {
  fprintf(stderr,"Usage:%s portnumber/a/n",argv[0]);
  exit(1);
 }
 if((portnumber=atoi(argv[1]))<0)
 {
  fprintf(stderr,"Usage:%s portnumber/a/n",argv[0]);
  exit(1);
 }
/* 服务器端开始建立socket描述符 */
 if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)  
 {
        fprintf(stderr,"Socket error:%s/n/a",strerror(errno));
        exit(1);
 }
/* 服务器端填充 sockaddr结构  */ 
 bzero(&server_addr,sizeof(struct sockaddr_in));
 server_addr.sin_family=AF_INET;
 server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
 server_addr.sin_port=htons(portnumber);
/* 捆绑sockfd描述符  */ 
 if(bind(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1)
 {
        fprintf(stderr,"Bind error:%s/n/a",strerror(errno));
        exit(1);
 }
/* 监听sockfd描述符  */
 if(listen(sockfd,5)==-1)
 {
        fprintf(stderr,"Listen error:%s/n/a",strerror(errno));
        exit(1);
 }
 while(1)
 {
        /* 服务器阻塞,直到客户程序建立连接  */
        sin_size=sizeof(struct sockaddr_in);
        if((new_fd=accept(sockfd,(struct sockaddr *)(&client_addr),&sin_size))==-1)
        {
                fprintf(stderr,"Accept error:%s/n/a",strerror(errno));
                exit(1);
    }
        fprintf(stderr,"Server get connection from %s/n",
                                        inet_ntoa(client_addr.sin_addr));
        if(write(new_fd,hello,strlen(hello))==-1)
        {
         fprintf(stderr,"Write Error:%s/n",strerror(errno));
         exit(1);
        }
/* 这个通讯已经结束     */
        close(new_fd);
/* 循环下一个     */  
  }
 close(sockfd);
 exit(0);
}
客户端程序
/******* 客户端程序  client.c ************/
＃i nclude 
＃i nclude 
＃i nclude 
＃i nclude 
＃i nclude 
＃i nclude 
＃i nclude 
＃i nclude 
int main(int argc, char *argv[])
{
 int sockfd;
 char buffer[1024];
 struct sockaddr_in server_addr;
 struct hostent *host;
 int portnumber,nbytes;
 if(argc!=3)
 {
  fprintf(stderr,"Usage:%s hostname portnumber/a/n",argv[0]);
  exit(1);
 }
 if((host=gethostbyname(argv[1]))==NULL)
 {
        fprintf(stderr,"Gethostname error/n");
        exit(1);
 }
 if((portnumber=atoi(argv[2]))<0)
 {
        fprintf(stderr,"Usage:%s hostname portnumber/a/n",argv[0]);
        exit(1);
 }
/* 客户程序开始建立 sockfd描述符  */
 if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
 {
        fprintf(stderr,"Socket Error:%s/a/n",strerror(errno));
        exit(1);
 }
/* 客户程序填充服务端的资料       */
 bzero(&server_addr,sizeof(server_addr));
 server_addr.sin_family=AF_INET;
 server_addr.sin_port=htons(portnumber);
 server_addr.sin_addr=*((struct in_addr *)host->h_addr);
/* 客户程序发起连接请求         */ 
 if(connect(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1)
{
  fprintf(stderr,"Connect Error:%s/a/n",strerror(errno));
  exit(1);
}
/* 连接成功了           */
if((nbytes=read(sockfd,buffer,1024))==-1)
{
        fprintf(stderr,"Read Error:%s/n",strerror(errno));
        exit(1);
 }
 buffer[nbytes]='/0';
 printf("I have received:%s/n",buffer);
/* 结束通讯     */
 close(sockfd);
 exit(0);
}
MakeFile
这里我们使用GNU 的make实用程序来编译. 关于make的详细说明见 Make 使用介绍 
#########  Makefile       ###########
all:server client
server:server.c
        gcc $^ -o $@
client:client.c
        gcc $^ -o $@
运行make后会产生两个程序server(服务器端)和client(客户端) 
先运行./server portnumber&  (portnumber随便取一个大于1204
且不在/etc/services中出现的号码 就用8888好了),
然后运行  ./client localhost 8888 看看有什么结果. (你也可以用telnet
和netstat试一试.) 上面是一个最简单的网络程序,不过是不是也有点烦.上面
有许多函数我们还没有解释. 我会在下一章进行的详细的说明. 
2.7 总结 
总的来说网络程序是由两个部分组成的--客户端和服务器端.它们的建立步骤一般是: 
服务器端
socket-->bind-->listen-->accept 
客户端
socket-->connect 
Linux网络编程--3. 服务器和客户机的信息函数 
--------------------------------------------------------------------------------
作者：hoyt 2001-05-08 11:20:14 来自：http://linuxc.51.net 
这一章我们来学习转换和网络方面的信息函数. 
3.1 字节转换函数 
在网络上面有着许多类型的机器,这些机器在表示数据的字节顺序是不同的, 比如i386芯片是低字节在内存地址的低端,高字节在高端,而alpha芯片却相反. 为了统一起来,在Linux下面,有专门的字节转换函数. 
unsigned long int htonl(unsigned long int hostlong)
unsigned short int htons(unisgned short int hostshort)
unsigned long int ntohl(unsigned long int netlong)
unsigned short int ntohs(unsigned short int netshort)
在 这四个转换函数中,h 代表host, n 代表 network.s 代表short l 代表long 第一个函数的意义是将本机器上的long数据转化为网络上的long. 其他几个函数的意义也差不多. 
3.2 IP和域名的转换 
在网络上标志一台机器可以用IP或者是用域名.那么我们怎么去进行转换呢? 
struct hostent *gethostbyname(const char *hostname)
struct hostent *gethostbyaddr(const char *addr,int len,int type)
在中有struct hostent的定义
struct hostent{
char *h_name; /* 主机的正式名称 */
char *h_aliases; /* 主机的别名 */
int h_addrtype; /* 主机的地址类型 AF_INET*/
int h_length; /* 主机的地址长度 对于IP4 是4字节32位*/
char **h_addr_list; /* 主机的IP地址列表 */
}
#define h_addr h_addr_list[0] /* 主机的第一个IP地址*/
gethostbyname可以将机器名(如 linux.yessun.com)转换为一个结构指针.在这个结构里面储存了域名的信息 
gethostbyaddr可以将一个32位的IP地址(C0A80001)转换为结构指针. 
这两个函数失败时返回NULL 且设置h_errno错误变量,调用h_strerror()可以得到详细的出错信息 
3.3 字符串的IP和32位的IP转换. 
在 网络上面我们用的IP都是数字加点(192.168.0.1)构成的, 而在struct in_addr结构中用的是32位的 IP, 我们上面那个32位IP(C0A80001)是的192.168.0.1 为了转换我们可以使用下面两个函数 
int inet_aton(const char *cp,struct in_addr *inp)
char *inet_ntoa(struct in_addr in)
函 数里面 a 代表 ascii n 代表network.第一个函数表示将a.b.c.d的IP转换 为32位的IP,存储在 inp指针里面.第二个是将32位IP转换为a.b.c.d的格式. 
3.4 服务信息函数 
在网络程序里面我们有时候需要知道端口.IP和服务信息.这个时候我们可以使用以下几个函数 
int getsockname(int sockfd,struct sockaddr *localaddr,int *addrlen)
int getpeername(int sockfd,struct sockaddr *peeraddr, int *addrlen)
struct servent *getservbyname(const char *servname,const char *protoname)
struct servent *getservbyport(int port,const char *protoname)
struct servent
{
char *s_name; /* 正式服务名 */
char **s_aliases; /* 别名列表 */ 
int s_port; /* 端口号 */
char *s_proto; /* 使 用的协议 */ 
}
一 般我们很少用这几个函数.对应客户端,当我们要得到连接的端口号时在connect调用成功后使用可得到系统分配的端口号.对于服务端,我们 用INADDR_ANY填充后,为了得到连接的IP我们可以在accept调用成功后使用而得到IP地址. 
在网络上有许多的默认端口和服务,比如端口21对ftp80对应WWW.为了得到指定的端口号的服务我们可以调用第四个函数,相反为了得到端口号可以调用第三个函数. 
3.5 一个例子 
＃i nclude 
＃i nclude 
＃i nclude 
＃i nclude 
＃i nclude 
int main(int argc ,char **argv)
{
struct sockaddr_in addr;
struct hostent *host;
char **alias;
if(argc<2)
{
fprintf(stderr,"Usage:%s hostname|ip../n/a",argv[0]);
exit(1);
}
argv++;
for(;*argv!=NULL;argv++)
{
/* 这里我们假设是IP*/ 
if(inet_aton(*argv,&addr.sin_addr)!=0)
{
host=gethostbyaddr((char *)&addr.sin_addr,4,AF_INET); 
printf("Address information of Ip %s/n",*argv); 
} 
else 
{
/* 失败,难道是域名?*/
host=gethostbyname(*argv); printf("Address information
of host %s/n",*argv); 
}
if(host==NULL)
{
/* 都不是 ,算了不找了*/
fprintf(stderr,"No address information of %s/n",*argv);
continue;
}
printf("Official host name %s/n",host->h_name);
printf("Name aliases:");
for(alias=host->h_aliases;*alias!=NULL;alias++)
printf("%s ,",*alias);
printf("/nIp address:");
for(alias=host->h_addr_list;*alias!=NULL;alias++)
printf("%s ,",inet_ntoa(*(struct in_addr *)(*alias)));
}
}
在这个例子里面,为了判断用户输入的是IP还是域名我们调用了两个函数,第一次我们假设输入的是IP所以调用inet_aton, 失败的时候,再调用gethostbyname而得到信息. 
Linux网络编程--4. 完整的读写函数 
--------------------------------------------------------------------------------
作者：hoyt 2001-05-08 11:20:52 来自：http://linuxc.51.net 
一旦我们建立了连接,我们的下一步就是进行通信了.在Linux下面把我们前面建立的通道看成是文件描述符,这样服务器端和客户端进行通信时候,只要往文件描述符里面读写东西了. 就象我们往文件读写一样. 
4.1 写函数write 
ssize_t write(int fd,const void *buf,size_t nbytes)
write函数将buf中的nbytes字节内容写入文件描述符fd.成功时返回写的字节数.失败时返回-1. 并设置errno变量. 在网络程序中,当我们向套接字文件描述符写时有俩种可能. 
1)write的返回值大于0,表示写了部分或者是全部的数据. 
2)返回的值小于0,此时出现了错误.我们要根据错误类型来处理. 
如果错误为EINTR表示在写的时候出现了中断错误. 
如果为EPIPE表示网络连接出现了问题(对方已经关闭了连接). 
为了处理以上的情况,我们自己编写一个写函数来处理这几种情况. 
int my_write(int fd,void *buffer,int length)
{
int bytes_left;
int written_bytes;
char *ptr;
ptr=buffer;
bytes_left=length;
while(bytes_left>0)
{
/* 开始写*/
written_bytes=write(fd,ptr,bytes_left);
if(written_bytes<=0) /* 出错了*/
{ 
if(errno==EINTR) /* 中断错误 我们继续写*/
written_bytes=0;
else /* 其他错误 没有办法,只好撤退了*/
return(-1);
}
bytes_left-=written_bytes;
ptr+=written_bytes; /* 从剩下的地方继续写 */
}
return(0);
}
4.2 读函数read 
ssize_t read (int fd,void *buf,size_t nbyte) read函数是负责从fd中读取内容.当读成功时,read返回实际所读的字节数,如 果返回的值是0 表示已经读到文件的结束了,小于0表示出现了错误.如果错误为EINTR说明读是由中断引起的, 如果是ECONNREST表示网络连接出了问题. 和上面一样,我们也写一个自己的读函数. 
int my_read(int fd,void *buffer,int length)
{
int bytes_left;
int bytes_read;
char *ptr;
bytes_left=length;
while(bytes_left>0)
{
bytes_read=read(fd,ptr,bytes_read);
if(bytes_read<0)
{
if(errno==EINTR)
bytes_read=0;
else
return(-1);
}
else if(bytes_read==0)
break;
bytes_left-=bytes_read;
ptr+=bytes_read;
}
return(length-bytes_left);
}
4.3 数据的传递 
有了上面的两个函数,我们就可以向客户端或者是服务端传递数据了.比如我们要传递一个结构.可以使用如下方式 
/* 客户端向服务端写 */
struct my_struct my_struct_client;
write(fd,(void *)&my_struct_client,sizeof(struct my_struct);
/* 服务端的读*/ 
char buffer[sizeof(struct my_struct)];
struct *my_struct_server;
read(fd,(void *)buffer,sizeof(struct my_struct)); 
my_struct_server=(struct my_struct *)buffer; 
在网络上传递数据时我们一般都是把数据转化为char类型的数据传递.接收的时候也是一样的注意的是我们没有必要在网络上传递指针(因为传递指针是没有任何意义的,我们必须传递指针所指向的内容) 
Linux网络编程--5. 用户数据报发送 
--------------------------------------------------------------------------------
作者：hoyt 2001-05-08 11:21:31 来自：http://linuxc.51.net 
我们前面已经学习网络程序的一个很大的部分,由这个部分的知识,我们实际上可以写出大部分的基于TCP协议的网络程序了.现在在Linux下的大部分程序都是用我们上面所学的知识来写的.我们可以去找一些源程序来参考一下.这一章,我们简单的学习一下基于UDP协议的网络程序. 
5.1 两个常用的函数 
int recvfrom(int sockfd,void *buf,int len,unsigned int flags,struct sockaddr * from int *fromlen)
int sendto(int sockfd,const void *msg,int len,unsigned int flags,struct sockaddr *to int tolen)
sockfd,buf,len的意义和read,write一样,分别表示套接字描述符,发送或接收的缓冲区及大小.recvfrom负责从sockfd接收数据,如果from不是NULL,那么在from里面存储了信息来源的情况,如果对信息的来源不感兴趣,可以将from和fromlen设置为NULL.sendto负责向to发送信息.此时在to里面存储了收信息方的详细资料. 
5.2 一个实例 
/* 服务端程序 server.c */
＃i nclude 
＃i nclude 
＃i nclude 
＃i nclude 
＃i nclude 
#define SERVER_PORT 8888
#define MAX_MSG_SIZE 1024
void udps_respon(int sockfd)
{
struct sockaddr_in addr;
int addrlen,n;
char msg[MAX_MSG_SIZE];
while(1)
{ /* 从网络上度,写到网络上面去 */
n=recvfrom(sockfd,msg,MAX_MSG_SIZE,0,
(struct sockaddr*)&addr,&addrlen);
msg[n]=0;
/* 显示服务端已经收到了信息 */
fprintf(stdout,"I have received %s",msg);
sendto(sockfd,msg,n,0,(struct sockaddr*)&addr,addrlen);
}
}
int main(void)
{
int sockfd;
struct sockaddr_in addr;
sockfd=socket(AF_INET,SOCK_DGRAM,0);
if(sockfd<0)
{
fprintf(stderr,"Socket Error:%s/n",strerror(errno));
exit(1);
}
bzero(&addr,sizeof(struct sockaddr_in));
addr.sin_family=AF_INET;
addr.sin_addr.s_addr=htonl(INADDR_ANY);
addr.sin_port=htons(SERVER_PORT);
if(bind(sockfd,(struct sockaddr *)&ddr,sizeof(struct sockaddr_in))<0)
{
fprintf(stderr,"Bind Error:%s/n",strerror(errno));
exit(1);
}
udps_respon(sockfd);
close(sockfd);
}
/* 客户端程序 */
＃i nclude 
＃i nclude 
＃i nclude 
＃i nclude 
＃i nclude 
＃i nclude 
#define MAX_BUF_SIZE 1024
void udpc_requ(int sockfd,const struct sockaddr_in *addr,int len)
{
char buffer[MAX_BUF_SIZE];
int n;
while(1) 
{ /* 从键盘读入,写到服务端 */
fgets(buffer,MAX_BUF_SIZE,stdin);
sendto(sockfd,buffer,strlen(buffer),0,addr,len);
bzero(buffer,MAX_BUF_SIZE);
/* 从网络上读,写到屏幕上 */
n=recvfrom(sockfd,buffer,MAX_BUF_SIZE,0,NULL,NULL);
buffer[n]=0;
fputs(buffer,stdout);
}
}
int main(int argc,char **argv)
{
int sockfd,port;
struct sockaddr_in addr;
if(argc!=3)
{
fprintf(stderr,"Usage:%s server_ip server_port/n",argv[0]);
exit(1);
}
if((port=atoi(argv[2]))<0)
{
fprintf(stderr,"Usage:%s server_ip server_port/n",argv[0]);
exit(1);
}
sockfd=socket(AF_INET,SOCK_DGRAM,0);
if(sockfd<0)
{
fprintf(stderr,"Socket Error:%s/n",strerror(errno));
exit(1);
} 
/* 填充服务端的资料 */
bzero(&addr,sizeof(struct sockaddr_in));
addr.sin_family=AF_INET;
addr.sin_port=htons(port);
if(inet_aton(argv[1],&addr.sin_addr)<0)
{
fprintf(stderr,"Ip error:%s/n",strerror(errno));
exit(1);
}
udpc_requ(sockfd,&addr,sizeof(struct sockaddr_in));
close(sockfd);
}
########### 编译文件 Makefile ##########
all:server client
server:server.c
gcc -o server server.c
client:client.c
gcc -o client client.c
clean:
rm -f server
rm -f client
rm -f core
上面的实例如果大家编译运行的话,会发现一个小问题的. 在我机器上面,我先运行服务端,然后运行客户端.在客户端输入信息,发送到服务端, 在服务端显示已经收到信息,但是客户端没有反映.再运行一个客户端,向服务端发出信息却可以得到反应.我想可能是第一个客户端已经阻塞了.如果谁知道怎么解决的话,请告诉我,谢谢. 由于UDP协议是不保证可靠接收数据的要求,所以我们在发送信息的时候,系统并不能够保证我们发出的信息都正确无误的到达目的地.一般的来说我们在编写网络程序的时候都是选用TCP协议的.
