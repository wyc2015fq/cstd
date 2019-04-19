# 在 Linux 上实现基于 Socket 的多进程实时通信 - xqhrs232的专栏 - CSDN博客
2011年06月02日 10:40:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：669标签：[socket																[linux																[server																[module																[dst																[struct](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=dst&t=blog)](https://so.csdn.net/so/search/s.do?q=module&t=blog)](https://so.csdn.net/so/search/s.do?q=server&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=socket&t=blog)
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://blogold.chinaunix.net/u/27131/showart_343197.html](http://blogold.chinaunix.net/u/27131/showart_343197.html)
套接口（Socket）为目前 Linux 上最为广泛使用的一种的进程间通信机制。但是它不能直接用来多进程之间的相互实时通信。本文提出一个基于 Socket 的多进程之间通信的实现方法，并给出样本程序的实现和说明。
套接口（Socket）为目前Linux上最为广泛使用的一种的进程间通信机制，与其他的Linux通信机制不同之处在于除了它可用于单机内的进程间通信以外，还可用于不同机器之间的进程间通信。但是由于Socket本身不支持同时等待和超时处理，所以它不能直接用来多进程之间的相互实时通信。
本文提出一个基于Socket的多进程之间通信的实现方法。原理是建立一个进程专门用来做为通信服务器(server)来中转各个进程之间的通信。它首先启动一个用来监视连接要求的listening Socket，并把它的描述（Descriptor）号加入到一个事先定义好的fd_set的集合中，这个fd_set的集合用来存放listening Socket和后来生成的通信Socket的描述号。Server运用system call select来实时检查是否有数据到达这个集合中的任何一个socket,如果有数据到达listening
 Socket，则这一定是客户端发起的连接请求，于是生成一个新的通信Socket与该客户端连接，将生成的Socket描述号加入到fd_set的集合中，将客户端的ID号和与之对应的Socket的描述号记录在ID登记表中。如果有数据到达某个通信Socket，则这一定是某个客户端发起的通信请求，读出数据并取出收信客户端ID号，在ID登记表中找到与之对应的Socket描述号，将数据通过对应Socket传送到收信客户端。
其他各进程作为客户端(client)。客户端的动作是首先建立通信Socket连接服务器端，然后通过通信Socket进行送信和收信。
下面给出具体的程序实现和说明，
首先给出Server端的程序，在这里假设有两个客户端要进行实时通信，ClientA向ClientB发送字符1，ClientB向ClientA发送字符2。
**QUOTE:**#include 
#include 
#include 
#include 
#include 
#include 
#include 
#include 
int main()
{
int rcd ;
struct sockaddr_un server_sockaddr ;
int backlog ;
ushort ci ;
int watch_fd_list[3] ;
fd_set catch_fd_set ;
fd_set watchset ;
int new_cli_fd ;
int maxfd;
int socklen ,server_len;
struct sockaddr_un cli_sockaddr ;
struct {
char module_id ; /* Module ID */
int cli_sock_fd ; /* Socket ID */
} cli_info_t[2] ;
for (ci=0;ci<=1;ci++)
cli_info_t[ci].cli_sock_fd=-1;
for (ci=0;ci<=2;ci++)
watch_fd_list[ci]=-1; 
int server_sockfd,client_sockfd;
server_sockfd = socket( AF_UNIX, SOCK_STREAM, 0 ) ;
server_sockaddr.sun_family = AF_UNIX ;
strcpy( server_sockaddr.sun_path, "server_socket" ) ;
server_len=sizeof(server_sockaddr);
rcd = bind( server_sockfd, ( struct sockaddr * )&server_sockaddr, server_len ) ;
backlog = 5 ;
rcd = listen( server_sockfd, backlog ) ;
printf("SERVER::Server is waitting on socket=%d /n",server_sockfd);
watch_fd_list[0]=server_sockfd;
FD_ZERO( &watchset ) ;
FD_SET( server_sockfd, &watchset ) ;
maxfd=watch_fd_list[0];
在上面的程序中，Server生成listening Socket（server_sockfd）,初始化Socket监视集合（watchset），并将listening Socket放入Socket监视集合中。
**QUOTE:**while (1){
char ch;
int fd;
int nread;
catch_fd_set=watchset;
rcd = select( maxfd+1, &catch_fd_set, NULL, NULL, (struct timeval *)0 ) ;
在上面的程序中，Server运用系统调用函数 select来实时检查是否有数据到达Socket监视集合中的任何一个socket。
**QUOTE:**if ( rcd < 0 ) {
printf("SERVER::Server 5 /n");
exit(1);
}
if ( FD_ISSET( server_sockfd, &catch_fd_set ) ) {
socklen = sizeof( cli_sockaddr ) ;
new_cli_fd = accept( server_sockfd, ( struct sockaddr * )&( cli_sockaddr ), &socklen ) ;
printf(" SERVER::open communication with Client %s on socket %d/n", cli_sockaddr.sun_path,new_cli_fd);
for (ci=1;ci<=2;ci++){
if(watch_fd_list[ci] != -1) continue;
else{ 
watch_fd_list[ci] = new_cli_fd;
break;
} 
} 
FD_SET(new_cli_fd , &watchset ) ;
if ( maxfd < new_cli_fd ) {
maxfd = new_cli_fd ;
}
for ( ci=0;ci<=1;ci++){
if(cli_info_t[ci].cli_sock_fd == -1) {
cli_info_t[ci].module_id=cli_sockaddr.sun_path[0];
cli_info_t[ci].cli_sock_fd=new_cli_fd;
break;
} 
}
continue; 
}
在上面的程序中，Server运用系统调用函数FD_ISSET来检查是否有客户端的连接请求到达Listening Socket, 如果返回值大于0，Server生成一个新的通信Socket （new_cli_fd）与客户端连接。将新生成的通信Socket放入Socket监视集合中（FD_SET）。将客户端的信息（ID号和Socket 描述号）保存在注册表cli_info_t中
**QUOTE:**for ( ci = 1; ci<=2 ; ci++ ) {
int dst_fd = -1 ;
char dst_module_id;
char src_module_id;
int i;
if (watch_fd_list[ ci ]==-1) continue;
if ( !FD_ISSET( watch_fd_list[ ci ], &catch_fd_set ) ) {
continue ;
}
ioctl(watch_fd_list[ ci ],FIONREAD,&nread);
if (nread==0){
continue;
} 
read( watch_fd_list[ ci ], &dst_module_id, 1 ) ;
for (i=0;i<=1;i++){
if(cli_info_t*.module_id == dst_module_id) dst_fd= cli_info_t*.cli_sock_fd; if(cli_info_t*.cli_sock_fd==watch_fd_list[ ci ]) src_module_id= cli_info_t*.module_id; }read( watch_fd_list[ ci ], &ch, 1 ) ;printf("SERVER::char=%c to Client %c on socket%d/n",ch, dst_module_id,dst_fd); write(dst_fd,&src_module_id, 1 ) ;write(dst_fd,&ch, 1 ) ;}} }****
在上面的程序中，如果有数据到达某个通信Socket，Server则读出数据并取出收信客户端ID号。在ID登记表中找到收信客户端对应的Socket描述号。并将数据通过对应Socket传送到收信客户端
给出客户端 ClientA的程序
ClientB的程序只需将 char dst_module_id='B'; 改为char dst_module_id='A'; char ch='1'; 改为char char ch='2';既可。
**QUOTE:**#include 
#include 
#include 
#include 
#include 
#include 
#include 
int main(){
int client_sockfd;
int len;
struct sockaddr_un server_sockaddr,cli_sockaddr;
int result;
char dst_module_id='B';
char ch='1';
char src_module_id;
client_sockfd= socket(AF_UNIX,SOCK_STREAM,0);
cli_sockaddr.sun_family = AF_UNIX ;
strcpy( cli_sockaddr.sun_path, "A" ) ;
bind(client_sockfd,(struct sockaddr * )&cli_sockaddr, sizeof( cli_sockaddr ) ) ;
server_sockaddr.sun_family=AF_UNIX;
strcpy( server_sockaddr.sun_path, "server_socket" ) ;
len=sizeof(server_sockaddr);
result = connect(client_sockfd,( struct sockaddr * )&server_sockaddr,len);
if (result <0){
printf("ClientA::error on connecting /n"); 
exit(1);
}
printf("ClientA::succeed in connecting with server/n");
sleep(10);
write(client_sockfd,&dst_module_id,1);
write(client_sockfd,&ch,1); 
read (client_sockfd,&src_module_id,1); 
read (client_sockfd,&ch,1); 
printf("ClientA::char from Client %c =%c/n", src_module_id,ch); 
close (client_sockfd);
}
下面是样本程序的执行结果
[root@zhou test]# ./server &
[3] 4301
[root@zhou test]# SERVER::Server is waitting on socket=3
./clientA & ./clientB &
[4] 4302
[5] 4303
ClientA::succeed in connecting with server
SERVER::open communication with Client A on socket 4
[root@zhou test]# SERVER::open communication with Client B on socket 5
ClientB::succeed in connecting with server
SERVER::char=1 to Client B on socket5
ClientB::char from Client A =1
SERVER::char=2 to Client A on socket4
ClientA::char from Client B =2
程序清单下载：server.c：[http://www-128.ibm.com/developer ... socket-ipc/server.c](http://www-128.ibm.com/developerworks/cn/linux/l-socket-ipc/server.c)
clientA.c：[http://www-128.ibm.com/developer ... ocket-ipc/clientA.c](http://www-128.ibm.com/developerworks/cn/linux/l-socket-ipc/clientA.c)
关于作者
周欣，北京邮电大学计算机系硕士毕业, 现在日本从事软件开发，通过 [jones_zhou@yahoo.com.cn](mailto:jones_zhou@yahoo.com.cn) 可以和他联系。
SERVER.c
#include  <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <netinet/in.h>
int main()
{
	int				rcd ;
	struct sockaddr_un	server_sockaddr ;
	int				backlog ;
	ushort				ci ;
	int				watch_fd_list[3] ;
	fd_set				catch_fd_set ;
	fd_set				watchset ;
	int	new_cli_fd ;
	int 	maxfd;
	int	socklen,server_len;
	struct sockaddr_un	cli_sockaddr ;
	struct {
		char		module_id ;		/* Module ID			*/
		int		cli_sock_fd ;	/* Socket ID		*/
	} cli_info_t[2] ;
	
	for (ci=0;ci<=1;ci++)
		cli_info_t[ci].cli_sock_fd=-1;
		
	for (ci=0;ci<=2;ci++)
		watch_fd_list[ci]=-1;	
		
	int server_sockfd,client_sockfd;
	
	server_sockfd = socket( AF_UNIX, SOCK_STREAM, 0 ) ;
	server_sockaddr.sun_family = AF_UNIX ;
//	server_sockaddr.sin_addr.s_addr=htonl(INADDR_ANY);
//	server_sockaddr.sin_port=htons(9734);
	strcpy( server_sockaddr.sun_path, "server_socket" ) ;
	server_len=sizeof(server_sockaddr);
	rcd = bind( server_sockfd, ( struct sockaddr * )&server_sockaddr, server_len ) ;
	
	backlog = 5 ;
	rcd = listen( server_sockfd, backlog ) ;
	printf("SERVER::Server is  waitting on socket=%d /n",server_sockfd);
	watch_fd_list[0]=server_sockfd;
	FD_ZERO( &watchset ) ;
	FD_SET( server_sockfd, &watchset ) ;
	maxfd=watch_fd_list[0];
	
	while (1){
		char ch;
		int fd;
		int nread;
		
		catch_fd_set=watchset;
		rcd = select( maxfd+1, &catch_fd_set, NULL, NULL, (struct timeval *)0 ) ;
		if ( rcd < 0 ) {
			printf("SERVER::Server 5 /n");
			exit(1);
		}
		if ( FD_ISSET( server_sockfd, &catch_fd_set ) ) {
			socklen = sizeof( cli_sockaddr ) ;
			new_cli_fd = accept( server_sockfd, ( struct sockaddr * )&( cli_sockaddr ), &socklen ) ;
			printf(" SERVER::open communication with  Client %s on socket %d/n", cli_sockaddr.sun_path,new_cli_fd);	
	
			for (ci=1;ci<=2;ci++){
				if(watch_fd_list[ci] != -1) continue;
				else{	
					watch_fd_list[ci] = new_cli_fd;
					break;
				}	
			}	
			FD_SET(new_cli_fd , &watchset ) ;
			if ( maxfd < new_cli_fd ) {
				maxfd = new_cli_fd ;
			}
			
			for ( ci=0;ci<=1;ci++){
				if(cli_info_t[ci].cli_sock_fd == -1) {
					cli_info_t[ci].module_id=cli_sockaddr.sun_path[0];
					cli_info_t[ci].cli_sock_fd=new_cli_fd;
					break;
				}		
			}
				
			continue;	
		}
		for ( ci = 1; ci<=2 ; ci++ ) {
			int				dst_fd = -1 ;
			char			dst_module_id;
			char 			src_module_id;
			int		i;
			if (watch_fd_list[ ci ]==-1) continue;
			if ( !FD_ISSET( watch_fd_list[ ci ], &catch_fd_set ) ) {
				continue ;
			}
			ioctl(watch_fd_list[ ci ],FIONREAD,&nread);
			if (nread==0){
				continue;
			}	
			read( watch_fd_list[ ci ], &dst_module_id, 1 ) ;
			for (i=0;i<=1;i++){
				if(cli_info_t[i].module_id == dst_module_id) 
					dst_fd=	cli_info_t[i].cli_sock_fd;	
				if(cli_info_t[i].cli_sock_fd == watch_fd_list[ ci ]) 
					src_module_id=	cli_info_t[i].module_id;		
			}
			read( watch_fd_list[ ci ], &ch, 1 ) ;
			printf("SERVER::char=%c to  Client %c on socket%d/n",ch, dst_module_id,dst_fd);	
			write(dst_fd,&src_module_id, 1 ) ;
			write(dst_fd,&ch, 1 ) ;
		}
	}	
}
CLIENT.c
#include  <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
int main(){
	
	int client_sockfd;
	int len;
	struct sockaddr_un server_sockaddr,cli_sockaddr;
	int result;
	char dst_module_id='B';
	char ch='1';
	char src_module_id;
	
	
	client_sockfd= socket(AF_UNIX,SOCK_STREAM,0);
	
//	memset( &cli_sockaddr, 0, sizeof( cli_sockaddr) ) ;
	cli_sockaddr.sun_family = AF_UNIX ;
	strcpy( cli_sockaddr.sun_path, "A" ) ;
	bind( client_sockfd, ( struct sockaddr * )&cli_sockaddr, sizeof( cli_sockaddr ) ) ;
	server_sockaddr.sun_family=AF_UNIX;
//	server_sockaddr.sin_addr.s_addr=htonl(INADDR_ANY);
//	server_sockaddr.sin_port=htons(9734);
	strcpy( server_sockaddr.sun_path, "server_socket" ) ;
	len=sizeof(server_sockaddr);
	
	result = connect(client_sockfd,( struct sockaddr * )&server_sockaddr,len);
	if (result <0){
		printf("ClientA::error on connecting /n");	
//		exit(1);
	}
	
	printf("ClientA::succeed in connecting with server/n");
	sleep(10);
	write(client_sockfd,&dst_module_id,1);
	write(client_sockfd,&ch,1); 
	read (client_sockfd,&src_module_id,1); 
	read (client_sockfd,&ch,1); 
	printf("ClientA::char from  Client %c =%c/n", src_module_id,ch);	
	close (client_sockfd);
	
}	
