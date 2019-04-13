
# Linux网络编程实例详解 - 高科的专栏 - CSDN博客

2011年08月10日 14:41:32[高科](https://me.csdn.net/pbymw8iwm)阅读数：827标签：[编程																](https://so.csdn.net/so/search/s.do?q=编程&t=blog)[linux																](https://so.csdn.net/so/search/s.do?q=linux&t=blog)[网络																](https://so.csdn.net/so/search/s.do?q=网络&t=blog)[socket																](https://so.csdn.net/so/search/s.do?q=socket&t=blog)[struct																](https://so.csdn.net/so/search/s.do?q=struct&t=blog)[服务器																](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=struct&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=socket&t=blog)个人分类：[网络																](https://blog.csdn.net/pbymw8iwm/article/category/893044)
[
																								](https://so.csdn.net/so/search/s.do?q=socket&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=网络&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=网络&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=编程&t=blog)

本文介绍了在Linux环境下的socket编程常用函数用法及socket编程的一般规则和客户/服务器模型的编程应注意的事项和常遇问题的解决方法，并举了具体代 码实例。要理解本文所谈的技术问题需要读者具有一定C语言的编程经验和TCP/IP方面的基本知识。要实习本文的示例，需要Linux下的gcc编译平台支持。
Socket定义
网络的Socket数据传输是一种特殊的I/O，Socket也是一种文件描述符。Socket也具有一个类似于打开文件的函数调用—Socket()，该函数返回一个整型的Socket描述符，随后的连接建立、数据传输等操作都是通过该Socket实现的。常用 的Socket类型有两种：流式Socket—SOCK_STREAM和数据报式Socket—SOCK_DGRAM。流式是一种面向连接的Socket，针对于面向连接的TCP服务应用；数据报式Socket是一种无连接的Socket，对应于无连接的UDP服务应用。
Socket编程相关数据类型定义
计算机数据存储有两种字节优先顺序：高位字节优先和低位字节优先。Intenet上数据以高位字节优先顺序在网络上传输，所以对于在内部是以低位字节优先方式存储数据的机器，在Internet上传输数据时就需要进行转换。
我们要讨论的第一个结构类型是：struct sockaddr，该类型是用来保存socket信息的：
struct sockaddr {
unsigned short sa_family; /* 地址族， AF_xxx */
char sa_data[14]; /* 14 字节的协议地址 */ };
sa_family一般为AF_INET；sa_data则包含该socket的IP地址和端口号。
另外还有一种结构类型：
struct sockaddr_in {
short int sin_family; /* 地址族 */
unsigned short int sin_port;/* 端口号 */
struct in_addr sin_addr; /* IP地址 */
unsigned char sin_zero[8]; /* 填充0 以保持与struct sockaddr同样大
小 */
};
这个结构使用更为方便。sin_zero(它用来将sockaddr_in结构填充到与struct sockaddr同样的长度)应该用bzero()或memset()函数将其置为零。指向sockaddr_in 的指针和指向sockaddr的指针可以相互转换，这意味着如果一个函数所需参数类型是sockaddr时，你可以在函数调用的时候将一个指向sockaddr_in的指针转换为指向sockaddr的指针；或者相反。sin_family通常被赋AF_INET；in_port和sin_addr应该转换成为网络字节优先顺序；而sin_addr则不需要转换。
我们下面讨论几个字节顺序转换函数：
htons()--"Host to Network Short" ;htonl()--"Host to Network long"
ntohs()--"Network to Host Short" ;ntohl()--"Network to Host Long"
在这里， h表示"host" ，n表示"network"，s 表示"short"，l表示 "long"
。
打开socket 描述符、建立绑定并建立连接
socket函数原型为：
int socket(int domain, int type, int protocol);
domain参数指定socket的类型：SOCK_STREAM 或SOCK_DGRAM；protocol通常赋值“0”。Socket()调用返回一个整型socket描述符，你可以在后面的调用使用它。一旦通过socket调用返回一个socket描述符，你应该将该socket与你本机上的一个端口相关联（往往当你在设计服务器端程序时需要调用该函数。随后你就可以在该端口监听服务请求;而客户端一般无须调用该函数）。 Bind函数原型为 ：
int bind(int sockfd,struct sockaddr *my_addr, intaddrlen);
Sockfd是一个socket描述符，my_addr是一个指向包含有本机IP地址及端口号等信息的sockaddr类型的指针;addrlen常被设置为sizeof(struct sockaddr)。
最后，对于bind 函数要说明的一点是，你可以用下面的赋值实现自动获得本机IP地址和随机获取一个没有被占用的端口号：
my_addr.sin_port = 0; /* 系统随机选择一个未被使用的端口号 */
my_addr.sin_addr.s_addr =INADDR_ANY; /* 填入本机IP地址 */
通过将my_addr.sin_port置为0，函数会自动为你选择一个未占用的端口来使用。同样，通过将my_addr.sin_addr.s_addr置为INADDR_ANY，系统会自动填入本机IP地址。Bind()函数在成功被调用时返回0；遇到错误时返回“-1”并将errno置为相应的错误号。另外要注意的是，当调用函数时，一般不要将端口号置为小于1024的值，因为1~1024是保留端口号，你可以使用大于1024中任何一个没有被占用的端口号。
Connect()函数用来与远端服务器建立一个TCP连接，其函数原型为：
int connect(int sockfd, struct sockaddr *serv_addr,int addrlen);
Sockfd是目的服务器的sockt描述符；serv_addr是包含目的机IP地址和端口号的指针。遇到错误时返回-1，并且errno中包含相应的错误码。进行客户端程序设计无须调用bind()，因为这种情况下只需知道目的机器的IP地址，而客户通过哪个端口与服务器建立连接并不需要关心，内核会自动选择一个未被占用的端口供客户端来使用。
Listen()——监听是否有服务请求
在服务器端程序中，当socket与某一端口捆绑以后，就需要监听该端口，以便对到达的服务请求加以处理。
int listen(int sockfd， int backlog);
Sockfd是Socket系统调用返回的socket 描述符；backlog指定在请求队列中允许的最大请求数，进入的连接请求将在队列中等待accept()它们（参考下文）。backlog对队列中等待服务的请求的数目进行了限制，大多数系统缺省值为20。
当listen遇到错误时返回-1，errno被置为相应的错误码。
故服务器端程序通常按下列顺序进行函数调用：
socket(); bind(); listen(); /* accept() goes here */
accept()——连接端口的服务请求。
当某个客户端试图与服务器监听的端口连接时，该连接请求将排队等待服务器accept()它。通过调用accept()函数为其建立一个连接，accept()函数将返回一个新的socket描述符，来供这个新连接来使用。而服务器可以继续在以前的那个socket上监听，同时可以在新的socket描述符上进行数据send()（发送）和recv()（接收）操作：
int accept(int sockfd, void *addr, int *addrlen);
sockfd是被监听的socket描述符，addr通常是一个指向sockaddr_in变量的指针，该变量用来存放提出连接请求服务的主机的信息（某台主机从某个端口发出该请求）；addrten通常为一个指向值为sizeof(struct sockaddr_in)的整型指针变量。错误发生时返回一个-1并且设置相应的errno值。
Send()和recv()——数据传输
这两个函数是用于面向连接的socket上进行数据传输。
Send()函数原型为：
int send(int sockfd, const void *msg, int len, int flags);
Sockfd是你想用来传输数据的socket描述符，msg是一个指向要发送数据的指针。
Len是以字节为单位的数据的长度。flags一般情况下置为0（关于该参数的用法可参照man手册）。
char *msg = "Beej was here!"; int len， bytes_sent; ... ...
len = strlen(msg); bytes_sent = send(sockfd,msg,len,0); ... ...
Send()函数返回实际上发送出的字节数，可能会少于你希望发送的数据。所以需要对send()的返回值进行测量。当send()返回值与len不匹配时，应该对这种情况进行处理。
recv()函数原型为：
int recv(int sockfd,void *buf,int len,unsigned intflags);
Sockfd是接受数据的socket描述符；buf 是存放接收数据的缓冲区；len是缓冲的长度。Flags也被置为0。Recv()返回实际上接收的字节数，或当出现错误时，返回-1并置相应的errno值。
Sendto()和recvfrom()——利用数据报方式进行数据传输
在无连接的数据报socket方式下，由于本地socket并没有与远端机器建立连接，所以在发送数据时应指明目的地址，sendto()函数原型为：
int sendto(int sockfd, const void *msg,intlen,unsigned int flags, const struct sockaddr *to, int tolen);
该函数比send()函数多了两个参数，to表示目地机的IP地址和端口号信息，而tolen常常被赋值为sizeof (struct sockaddr)。Sendto 函数也返回实际发送的数据字节长度或在出现发送错误时返回-1。
Recvfrom()函数原型为：
int recvfrom(int sockfd,void *buf,int len,unsigned intlags,struct sockaddr *from,int *fromlen);
from是一个struct sockaddr类型的变量，该变量保存源机的IP地址及端口号。fromlen常置为sizeof (struct sockaddr)。当recvfrom()返回时，fromlen包含实际存入from中的数据字节数。Recvfrom()函数返回接收到的字节数或当出现错误时返回-1，并置相应的errno。
应注意的一点是，当你对于数据报socket调用了connect()函数时，你也可以利用send()和recv()进行数据传输，但该socket仍然是数据报socket，并且利用传输层的UDP服务。但在发送或接收数据报时，内核会自动为之加上目地和源地址信息。
Close()和shutdown()——结束数据传输
当所有的数据操作结束以后，你可以调用close()函数来释放该socket，从而
停止在该socket上的任何数据操作：close(sockfd);
你也可以调用shutdown()函数来关闭该socket。该函数允许你只停止在某个方向上的数据传输，而一个方向上的数据传输继续进行。如你可以关闭某socket的写操作而允许继续在该socket上接受数据，直至读入所有数据。
int shutdown(int sockfd,int how);
Sockfd的含义是显而易见的，而参数 how可以设为下列值：
·0-------不允许继续接收数据
·1-------不允许继续发送数据
·2-------不允许继续发送和接收数据，均为允许则调用close ()
shutdown在操作成功时返回0，在出现错误时返回-1（并置相应errno）。
DNS——域名服务相关函数
由于IP地址难以记忆和读写，所以为了读写记忆方便，人们常常用域名来表示主机，这就需要进行域名和IP地址的转换。函数gethostbyname()就是完成这种转换的，函数原型为：
struct hostent*gethostbyname(const char *name);
函数返回一种名为hosten的结构类型，它的定义如下：
struct hostent {
char *h_name; /* 主机的官方域名 */
char **h_aliases; /* 一个以NULL结尾的主机别名数组 */
int h_addrtype; /* 返回的地址类型，在Internet环境下为AF-INET */
int h_length; /*地址的字节长度 */
char **h_addr_list; /* 一个以0结尾的数组，包含该主机的所有地址*/
};
\#define h_addr h_addr_list[0]/*在h-addr-list中的第一个地址*/
当 gethostname()调用成功时，返回指向struct hosten的指针，当调用失败时返回-1。当调用gethostbyname时，你不能使用perror()函数来输出错误信息，而应该使用herror()函数来输出。
面向连接的客户/服务器代码实例
这个服务器通过一个连接向客户发送字符串"Hello，world!"。只要在服务器上运行该服务器软件，在客户端运行客户软件，客户端就会收到该字符串。
该服务器软件代码见程序1：
＃i nclude stdio.h
＃i nclude stdlib.h
＃i nclude errno.h
＃i nclude string.h
＃i nclude sys/types.h
＃i nclude netinet/in.h
＃i nclude sys/socket.h
＃i nclude sys/wait.h
\#define MYPORT 3490 /*服务器监听端口号 */
\#define BACKLOG 10 /* 最大同时连接请求数 */
main()
{
intsock fd,new_fd; /* 监听socket: sock_fd,数据传输socket:new_fd*/
struct sockaddr_in my_addr; /* 本机地址信息 */
struct sockaddr_in their_addr;/* 客户地址信息 */
int sin_size;
if ((sockfd = socket(AF_INET,SOCK_STREAM, 0)) == -1) { /*错误检测 */
perror("socket");exit(1); }
my_addr.sin_family=AF_INET;
my_addr.sin_port=htons(MYPORT);
my_addr.sin_addr.s_addr =INADDR_ANY;
bzero(&(my_addr.sin_zero),8);
if (bind(sockfd, (structsockaddr *)&my_addr, sizeof(struct sockaddr))
== -1) {/*错误检测*/
perror("bind");exit(1); }
if (listen(sockfd, BACKLOG) ==-1) {/*错误检测*/
perror("listen");exit(1); }
while(1) { /* main accept()loop */
sin_size = sizeof(structsockaddr_in);
if ((new_fd = accept(sockfd,(struct sockaddr *)&their_addr,
&sin_size)) == -1) {
perror("accept");continue; }
printf("server: gotconnection from %s",inet_ntoa(their_addr.sin_addr));
if (!fork()) { /* 子进程代码段 */
if (send(new_fd, "Hello,world!", 14, 0) == -1)
perror("send");close(new_fd); exit(0); }
close(new_fd); /* 父进程不再需要该socket */
waitpid(-1,NULL,WNOHANG) > 0/*等待子进程结束，清除子进程所占用资源*/
}
}
（程序1）
服务器首先创建一个Socket，然后将该Socket与本地地址/端口号捆绑，成功之后就在相应的socket上监听，当accpet捕捉到一个连接服务请求时，就生成一个新的socket，并通过这个新的socket向客户端发送字符串"Hello，world!"，然后关闭该socket。
fork()函数生成一个子进程来处理数据传输部分，fork()语句对于子进程返回的值为0。所以包含fork函数的if语句是子进程代码部分，它与if语句后面的父进程代码部分是并发执行的。
客户端软件代码部分见程序2：
＃i ncludestdio.h
＃i nclude stdlib.h
＃i nclude errno.h
＃i nclude string.h
＃i nclude netdb.h
＃i nclude sys/types.h
＃i nclude netinet/in.h
＃i nclude sys/socket.h
\#define PORT 3490
\#define MAXDATASIZE 100 /*每次最大数据传输量 */
int main(int argc, char*argv[])
{
int sockfd, numbytes;
char buf[MAXDATASIZE];
struct hostent *he;
struct sockaddr_in their_addr;
if (argc != 2) {
fprintf(stderr,"usage:client hostname"); exit(1); }
if((he=gethostbyname(argv[1]))==NULL){
herror("gethostbyname");exit(1); }
if ((sockfd = socket(AF_INET,SOCK_STREAM, 0)) == -1) {
perror("socket");exit(1); }
their_addr.sin_family=AF_INET;
their_addr.sin_port=htons(PORT);
their_addr.sin_addr = *((structin_addr *)he->h_addr);
bzero(&(their_addr.sin_zero),8);
if (connect(sockfd, (structsockaddr *)&their_addr,
sizeof(struct sockaddr)) ==-1) {/*错误检测*/
perror("connect");exit(1); }
if ((numbytes=recv(sockfd, buf,MAXDATASIZE, 0)) == -1) {
perror("recv");exit(1); }
buf[numbytes] = '';
printf("Received:%s",buf);
close(sockfd);
return 0;
}
（程序2）
客户端代码相对来说要简单一些，首先通过服务器域名获得其IP地址，然后创建一个socket，调用connect函数与服务器建立连接，连接成功之后接收从服务器发送过来的数据，最后关闭socket，结束程序。
无连接的客户/服务器程序的在原理上和连接的客户/服务器是一样的，两者的区别在于无连接的客户/服务器中的客户一般不需要建立连接，而且在发送接收数据时，需要指定远端机的地址。
关于阻塞(blocking)的概念和select()函数当服务器运行到accept语句时，而没有客户连接服务请求到来，那么会发生什么情况?这时服务器就会停止在accept语句上等待连接服务请求的到来；同样，当程序运行到接收数据语句时，如果没有数据可以读取，则程序同样会停止在接收语句上。这种情况称为blocking。但如果你希望服务器仅仅注意检查是否有客户在等待连接，有就接受连接;否则就继续做其他事情，则可以通过将Socke设置为非阻塞方式来实现:非阻塞socket在没有客户在等待时就使accept调用立即返回。
＃i nclude unistd.h
＃i nclude fcntl.h
. . . . ； sockfd =socket(AF_INET,SOCK_STREAM,0);
fcntl(sockfd,F_SETFL,O_NONBLOCK)； . . . . .
通过设置socket为非阻塞方式，可以实现“轮询”若干Socket。当企图从一个没有数据等待处理的非阻塞Socket读入数据时，函数将立即返回，并且返回值置为-1，并且errno置为EWOULDBLOCK。但是这种“轮询”会使CPU处于忙等待方式，从而降低性能。考虑到这种情况，假设你希望服务器监听连接服务请求的同时从已经建立的连接读取数据，你也许会想到用一个accept语句和多个recv()语句，但是由于accept及recv都是会阻塞的，所以这个想法显然不会成功。
调用非阻塞的socket会大大地浪费系统资源。而调用select()会有效地解决这个问题，它允许你把进程本身挂起来，而同时使系统内核监听所要求的一组文件描述符的任何活动，只要确认在任何被监控的文件描述符上出现活动，select()调用将返回指示该文件描述符已准备好的信息，从而实现了为进程选出随机的变化，而不必由进程本身对输入进行测试而浪费CPU开销。Select函数原型为:
int select(int numfds,fd_set *readfds,fd_set *writefds，fd_set *exeptfds,struct timeval*timeout);
其中readfds、writefds、exceptfds分别是被select()监视的读、写和异常处理的文件描述符集合。如果你希望确定是否可以从标准输入和某个socket描述符读取数据，你只需要将标准输入的文件描述符0和相应的sockdtfd加入到readfds集合中；numfds的值是需要检查的号码最高的文件描述符加1，这个例子中numfds的值应为sockfd+1；当select返回时，readfds将被修改，指示某个文件描述符已经准备被读取，你可以通过FD_ISSSET()来测试。为了实现fd_set中对应的文件描述符的设置、复位和测试，它提供了一组宏：
FD_ZERO(fd_set *set)----清除一个文件描述符集；
FD_SET(int fd,fd_set *set)----将一个文件描述符加入文件描述符集中；
FD_CLR(int fd,fd_set *set)----将一个文件描述符从文件描述符集中清除
；
FD_ISSET(int fd,fd_set*set)----试判断是否文件描述符被置位。
Timeout参数是一个指向struct timeval类型的指针，它可以使select()在等待timeout长时间后没有文件描述符准备好即返回。struct timeval数据结构为：
struct timeval {
int tv_sec; /* seconds */
int tv_usec; /* microseconds*/
};
我们通过程序3来说明：
＃i nclude sys/time.h
＃i nclude sys/types.h
＃i nclude unistd.h
\#define STDIN 0 /*标准输入文件描述符*/
main()
{
struct timeval tv;
fd_set readfds;
tv.tv_sec = 2;
tv.tv_usec = 500000;
FD_ZERO(&readfds);
FD_SET(STDIN,&readfds);
/* 这里不关心写文件和异常处理文件描述符集合 */
select(STDIN+1， &readfds， NULL， NULL， &tv);
if (FD_ISSET(STDIN， &readfds)) printf("Akey was pressed!");
else printf("Timedout.");
}
（程序3）
select()在被监视端口等待2.5秒钟以后，就从select返回


