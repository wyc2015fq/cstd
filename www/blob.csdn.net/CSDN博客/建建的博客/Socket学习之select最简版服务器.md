# Socket学习之select最简版服务器 - 建建的博客 - CSDN博客
2017年01月27日 10:46:04[纪建](https://me.csdn.net/u013898698)阅读数：914
                
首先创建一个Windows网络程序，
定义变量->获得WINDOCK版本->加载WINSOCK库->初始化->创建套接字->设置套接字选项->关闭套接字->
卸载WINSOCK库->释放资源
首先要初始化一个WinSocket
typedef struct WSAData {
        WORD                    wVersion;
        WORD                    wHighVersion;
#ifdef _WIN64
        unsigned short          iMaxSockets;
        unsigned short          iMaxUdpDg;
        char FAR *              lpVendorInfo;
        char                    szDescription[WSADESCRIPTION_LEN+1];
        char                    szSystemStatus[WSASYS_STATUS_LEN+1];
#else
        char                    szDescription[WSADESCRIPTION_LEN+1];
        char                    szSystemStatus[WSASYS_STATUS_LEN+1];
        unsigned short          iMaxSockets;
        unsigned short          iMaxUdpDg;
        char FAR *              lpVendorInfo;
#endif
} WSADATA;
这个结构体的作用是存放windows Socket的初始化信息的
其中 wVersion是使用的windows socket版本号
wHighVersion是当前电脑支持的socket最高版本
szDescription与szSystemStatus暂时没用到过，
iMaxSockets是当前支持的最大并发数，这和硬件有关
iMaxUdpDg数据报所最大长度
WORD version = MAKEWORD(2, 0);获取当前Socket的版本号
其中第一个参数为是低数位，第二个参数位是高数位
高位字节指明副版本，低位字节指明主版本
int ret = WSAStartup(version, &wsaDATA);
操作系统根据请求的Socket版本来搜索相应的Socket库，
然后绑定找到的Socket库到该应用程序中。
以后应用程序就可以调用所请求的Socket库中的其它Socket函数了。
 SOCKET m_hSocket;创建一个socket对象
 SOCKADDR_IN m_addr;
/*
 * Socket address, internet style.
 */
struct sockaddr_in {
        short   sin_family;
        u_short sin_port;
        struct  in_addr sin_addr;
        char    sin_zero[8];
};
sin_family是地址家族的意思通常大多用的是都是AF_INET,代表TCP/IP协议族
 sin_port是网络端口号，数字的话要转为网络字节序
实际上这个网络字节序和普通的数字区别在于内存存储的时候大端位有小端位的替换
sin_addr存储IP地址，需要转为网络字节序，一般windows  socket使用htons转换
m_hSocket = socket(AF_INET, SOCK_STREAM,0);
m_addr.sin_family = AF_INET;
m_addr.sin_addr.S_un.S_addr = INADDR_ANY;
m_addr.sin_port = 1234;
创建套接字
socket函数的函数原型是
int socket(int domain, int type, int protocol);
domain是协议域的意思，他决定了socket的网络地址类型
如AF_INET决定了要用ipv4地址（32位的）与端口号（16位的）的组合、AF_UNIX决定了要用一个绝对路径名作为地址
type指定了socket的类型，主要的有SOCK_STREAM、SOCK_DGRAM、SOCK_RAW、SOCK_PACKET、SOCK_SEQPACKET
其中sock_stream流氏套接字，是专门面向TCP连接的，针对TCP连接专用的，SOCK_DGRAM是面向UDP服务
protocol指定协议类型
常用协议有IPPROTO_TCP、IPPROTO_UDP、IPPROTO_STCP、IPPROTO_TIPC等，分别对应TCP传输协议、UDP传输协议、STCP传输协议、TIPC传输协议。
ret = bind(m_hSocket, (LPSOCKADDR)&m_addr, sizeof(m_addr));
函数原型
int PASCAL FAR bind( SOCKET sockaddr, const struct sockaddr FAR* my_addr,int addrlen);
sockaddr一个socket对象，该对象没有被绑定
my_addr指向一个网络地址
addrlen网络地址的长度
成功返回0，失败返回-1.
listen(m_hSocket, 5);
函数原型
int listen(int sockfd, int backlog)
其实socket默认是主动连接，也就是默认情况下，之调用connect函数连接服务器的
但是在服务器编程中，我们要被动等待其他客户端连接。所有要调用listen告诉这个socket采用被动等待连接
backlog这个参数主要是为了应对并发时候，最大并发数用的。因为连接是一个过程，一个连接可能处于连接完成以及未完成之间，这个时候又有其他客户端来连接，这就要有一个列表来维护。但是列表的长度不能太大。所以这个参数是为了限制列表的大小的
//非堵塞等待客户的连接
SOCKET sock;                                                  //这个用来接受一个连接
fd_set rfd, wfd;                                                  //描述符集 这个将用来测试有没有一个可用的连接
struct timeval timeout;
FD_ZERO(&rfd);                                                //总是这样先清空一个描述符集
timeout.tv_sec = 60;                                          //等下select用到这个
timeout.tv_usec = 0;
u_long ul = 1;
ioctlsocket(m_hSocket, FIONBIO, &ul);                         //用非阻塞的连接
其中fd_set
select()机制中提供一fd_set的数据结构，实际上是一long类型的数组，每一个数组元素都能与一打开的文件句柄（不管是socket句柄，还是其他文件或命名管道或设备句柄）建立联系，建立联系的工作由程序员完成，当调用select()时，由内核根据IO状态修改fd_set的内容，由此来通知执行了select()的进程哪一socket或文件发生了可读或可写事件。
FD_ZERO(&set); /*将set清零使集合中不含任何fd*/
FD_SET(fd, &set); /*将fd加入set集合*/
FD_CLR(fd, &set); /*将fd从set集合中清除*/
FD_ISSET(fd, &set); /*在调用select()函数后，用FD_ISSET来检测fd是否在set集合中，当检测到fd在set中则返回真，否则，返回假（0）*/
ioctlsocket(m_hSocket, FIONBIO, &ul);
函数原型
int ioctlsocket( int s, long cmd, u_long * argp);
s：一个标识套接口的描述字。
cmd：对套接口s的操作命令。
argp：指向cmd命令所带参数的指针。
cmd命令主要有
FIONBIO，允许或禁止阻塞非阻塞模式，如果是非阻塞模式则argp为非0数
FIONREAD，确定套接口s自动读入的数据量。argp指向一个无符号长整型，其中存有ioctlsocket()的返回值。如果s是SOCKET_STREAM类型，则FIONREAD返回在一次recv()中所接收的所有数据量。这通常与套接口中排队的数据总量相同。如果S是SOCK_DGRAM 型，则FIONREAD返回套接口上排队的第一个数据报大小。
SIOCATMARK，
确认是否所有的带外数据都已被读入。这个命令仅适用于SOCK_STREAM类型的套接口，且该套接口已被设置为可以在线接收带外数据（SO_OOBINLINE）。如无带外数据等待读入，则该操作返回TRUE真。否则的话返回FALSE假，下一个recv()或recvfrom()操作将检索“标记”前一些或所有数据。应用程序可用SIOCATMARK操作来确定是否有数据剩下。如果在“紧急”（带外）数据[前有常规数据，则按序接收这些数据（请注意，recv()和recvfrom()操作不会在一次调用中混淆常规数据与带外数]据）。argp指向一个BOOL型数，ioctlsocket()在其中存入返回值。
if(select(m_hSocket + 1, &rfd, 0, 0, &timeout) == 0) 
{//这个大括号接上面的,返回0那么就超过了timeout预定的时间
}
函数原型
int select (int maxfd + 1,fd_set *readset,fd_set *writeset,fd_set *exceptset,const struct timeval * timeout);
参数一：最大的文件描述符加1。
参数二：用于检查可读性，
参数三：用于检查可写性，
参数四：用于检查带外数据，
参数五：一个指向timeval结构的指针，用于决定select等待I/o的最长时间。如果为空将一直等待。timeval结构的定义：struct timeval{
long tv_sec; // seconds
long tv_usec; // microseconds
}
if(FD_ISSET(m_hSocket, &rfd))
{//有一个描述符准备好了
sock = accept(m_hSocket, 0, 0);                           //一个用来测试读 一个用来测试写
if(sock == SOCKET_ERROR)
{
cout << "连接失败：" << (error = WSAGetLastError()) << endl;
system("pause");
return 0;
}
FD_ZERO(&rfd);
FD_ZERO(&wfd);
函数原型
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
sockfd：套接字描述符，该套接口在listen()后监听连接。
addr：（可选）指针，指向一缓冲区，其中接收为通讯层所知的连接实体的地址。Addr参数的实际格式由套接口创建时所产生的地址族确定。
addrlen：（可选）指针，输入参数，配合addr一起使用，指向存有addr地址长度的整型数。
while(1)
{
FD_SET(sock, &rfd);                                   //把sock放入读描述符集
FD_SET(sock, &wfd);                                   //把sock放入写描述符集
if(SOCKET_ERROR != select(0, &rfd, NULL, 0, 0))       //测试这两个描述符集,永不超时 其中rfd只用来测试读 wfd只用来测试写
{//没有错误
if(FD_ISSET(sock, &rfd))                          //sock可读
{
char buff[256];
ret = recv(sock, buff, 256, 0);
if(ret == 0 || ret == SOCKET_ERROR )
{
cout << "接收数据失败：" << WSAGetLastError() << endl;
return 0;
}
buff[ret] = '\0';
cout << buff << endl;
}
}
if(SOCKET_ERROR != select(0, NULL, &wfd, 0, 0))       //测试这两个描述符集,永不超时 其中rfd只用来测试读 wfd只用来测试写
{
if(FD_ISSET(sock, &wfd))                          //sock可写
{
string buf;
cin >> buf;
ret = send(sock, buf.c_str(), buf.size(), 0);
if(ret != buf.size())
{
cout << "发送数据失败：" << WSAGetLastError() << endl;
return 0;
}
}
}
}
}
system("pause");
实际这段处理代码有点问题，只能实现简单的发送和接收，而且只能接收一个客户端的读写。后面还需要改进
附上全部代码
// server3.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include "winsock.h"
#pragma comment(lib,"wsock32")
#include <string>
#include <iostream>
using namespace std;
int _tmain(int argc, _TCHAR* argv[])
{
//初始化Winsock
WSADATA wsaDATA;
WORD version = MAKEWORD(2, 0);
int ret = WSAStartup(version, &wsaDATA);
if(ret != 0)
cout << "初始化失败!" << endl;
    SOCKET m_hSocket;
SOCKADDR_IN m_addr;
//创建新的套接字，这里是创建的流类型的套接字
m_hSocket = socket(AF_INET, SOCK_STREAM,0);
m_addr.sin_family = AF_INET;
m_addr.sin_addr.S_un.S_addr = INADDR_ANY;
m_addr.sin_port = 1234;
//绑定一个套接字到本机的地址
ret = 0;
int error = 0;
ret = bind(m_hSocket, (LPSOCKADDR)&m_addr, sizeof(m_addr));
if(ret == SOCKET_ERROR)
{
cout << "绑定失败：" << (error = WSAGetLastError()) << endl;
system("pause");
return 0;
}
//开始一个侦听过程，等待用户的连接
ret = listen(m_hSocket, 5);
if(ret == SOCKET_ERROR)
{
cout << "侦听失败：" << (error = WSAGetLastError()) << endl;
system("pause");
return 0;
}
//非堵塞等待客户的连接
SOCKET sock;                                                  //这个用来接受一个连接
fd_set rfd, wfd;                                              //描述符集 这个将用来测试有没有一个可用的连接
struct timeval timeout;
FD_ZERO(&rfd);                                                //总是这样先清空一个描述符集
timeout.tv_sec = 60;                                          //等下select用到这个
timeout.tv_usec = 0;
u_long ul = 1;
ioctlsocket(m_hSocket, FIONBIO, &ul);                         //用非阻塞的连接
//现在开始用select
FD_SET(m_hSocket, &rfd);                                      //把sock放入要测试的描述符集 就是说把sock放入了rfd里面 这样下一步调用select对rfd进行
                     //测试的时候就会测试sock了(因为我们将sock放入的rdf) 一个描述符集可以包含多个被测试的描述符
if(select(m_hSocket + 1, &rfd, 0, 0, &timeout) == 0) 
{//这个大括号接上面的,返回0那么就超过了timeout预定的时间
}
if(FD_ISSET(m_hSocket, &rfd))
{//有一个描述符准备好了
sock = accept(m_hSocket, 0, 0);                           //一个用来测试读 一个用来测试写
if(sock == SOCKET_ERROR)
{
cout << "连接失败：" << (error = WSAGetLastError()) << endl;
system("pause");
return 0;
}
FD_ZERO(&rfd);
FD_ZERO(&wfd);
while(1)
{
FD_SET(sock, &rfd);                                   //把sock放入读描述符集
FD_SET(sock, &wfd);                                   //把sock放入写描述符集
if(SOCKET_ERROR != select(0, &rfd, NULL, 0, 0))       //测试这两个描述符集,永不超时 其中rfd只用来测试读 wfd只用来测试写
{//没有错误
if(FD_ISSET(sock, &rfd))                          //sock可读
{
char buff[256];
ret = recv(sock, buff, 256, 0);
if(ret == 0 || ret == SOCKET_ERROR )
{
cout << "接收数据失败：" << WSAGetLastError() << endl;
return 0;
}
buff[ret] = '\0';
cout << buff << endl;
}
}
if(SOCKET_ERROR != select(0, NULL, &wfd, 0, 0))       //测试这两个描述符集,永不超时 其中rfd只用来测试读 wfd只用来测试写
{
if(FD_ISSET(sock, &wfd))                          //sock可写
{
string buf;
cin >> buf;
ret = send(sock, buf.c_str(), buf.size(), 0);
if(ret != buf.size())
{
cout << "发送数据失败：" << WSAGetLastError() << endl;
return 0;
}
}
}
}
}
system("pause");
return 0;
}
while(1)
{
FD_SET(sock, &rfd);                                   //把sock放入读描述符集
FD_SET(sock, &wfd);                                   //把sock放入写描述符集
if(SOCKET_ERROR != select(0, &rfd, NULL, 0, 0))       //测试这两个描述符集,永不超时 其中rfd只用来测试读 wfd只用来测试写
{//没有错误
if(FD_ISSET(sock, &rfd))                          //sock可读
{
char buff[256];
ret = recv(sock, buff, 256, 0);
if(ret == 0 || ret == SOCKET_ERROR )
{
cout << "接收数据失败：" << WSAGetLastError() << endl;
return 0;
}
buff[ret] = '\0';
cout << buff << endl;
}
}
if(SOCKET_ERROR != select(0, NULL, &wfd, 0, 0))       //测试这两个描述符集,永不超时 其中rfd只用来测试读 wfd只用来测试写
{
if(FD_ISSET(sock, &wfd))                          //sock可写
{
string buf;
cin >> buf;
ret = send(sock, buf.c_str(), buf.size(), 0);
if(ret != buf.size())
{
cout << "发送数据失败：" << WSAGetLastError() << endl;
return 0;
}
}
}
}
}
system("pause");while(1)
{
FD_SET(sock, &rfd);                                   //把sock放入读描述符集
FD_SET(sock, &wfd);                                   //把sock放入写描述符集
if(SOCKET_ERROR != select(0, &rfd, NULL, 0, 0))       //测试这两个描述符集,永不超时 其中rfd只用来测试读 wfd只用来测试写
{//没有错误
if(FD_ISSET(sock, &rfd))                          //sock可读
{
char buff[256];
ret = recv(sock, buff, 256, 0);
if(ret == 0 || ret == SOCKET_ERROR )
{
cout << "接收数据失败：" << WSAGetLastError() << endl;
return 0;
}
buff[ret] = '\0';
cout << buff << endl;
}
}
if(SOCKET_ERROR != select(0, NULL, &wfd, 0, 0))       //测试这两个描述符集,永不超时 其中rfd只用来测试读 wfd只用来测试写
{
if(FD_ISSET(sock, &wfd))                          //sock可写
{
string buf;
cin >> buf;
ret = send(sock, buf.c_str(), buf.size(), 0);
if(ret != buf.size())
{
cout << "发送数据失败：" << WSAGetLastError() << endl;
return 0;
}
}
}
}
}
system("pause");while(1)
{
FD_SET(sock, &rfd);                                   //把sock放入读描述符集
FD_SET(sock, &wfd);                                   //把sock放入写描述符集
if(SOCKET_ERROR != select(0, &rfd, NULL, 0, 0))       //测试这两个描述符集,永不超时 其中rfd只用来测试读 wfd只用来测试写
{//没有错误
if(FD_ISSET(sock, &rfd))                          //sock可读
{
char buff[256];
ret = recv(sock, buff, 256, 0);
if(ret == 0 || ret == SOCKET_ERROR )
{
cout << "接收数据失败：" << WSAGetLastError() << endl;
return 0;
}
buff[ret] = '\0';
cout << buff << endl;
}
}
if(SOCKET_ERROR != select(0, NULL, &wfd, 0, 0))       //测试这两个描述符集,永不超时 其中rfd只用来测试读 wfd只用来测试写
{
if(FD_ISSET(sock, &wfd))                          //sock可写
{
string buf;
cin >> buf;
ret = send(sock, buf.c_str(), buf.size(), 0);
if(ret != buf.size())
{
cout << "发送数据失败：" << WSAGetLastError() << endl;
return 0;
}
}
}
}while(1)
{
FD_SET(sock, &rfd);                                   //把sock放入读描述符集
FD_SET(sock, &wfd);                                   //把sock放入写描述符集
if(SOCKET_ERROR != select(0, &rfd, NULL, 0, 0))       //测试这两个描述符集,永不超时 其中rfd只用来测试读 wfd只用来测试写
{//没有错误
if(FD_ISSET(sock, &rfd))                          //sock可读
{
char buff[256];
ret = recv(sock, buff, 256, 0);
if(ret == 0 || ret == SOCKET_ERROR )
{
cout << "接收数据失败：" << WSAGetLastError() << endl;
return 0;
}
buff[ret] = '\0';
cout << buff << endl;
}
}
if(SOCKET_ERROR != select(0, NULL, &wfd, 0, 0))       //测试这两个描述符集,永不超时 其中rfd只用来测试读 wfd只用来测试写
{
if(FD_ISSET(sock, &wfd))                          //sock可写
{
string buf;
cin >> buf;
ret = send(sock, buf.c_str(), buf.size(), 0);
if(ret != buf.size())
{
cout << "发送数据失败：" << WSAGetLastError() << endl;
return 0;
}
}
}
}while(1)
{
FD_SET(sock, &rfd);                                   //把sock放入读描述符集
FD_SET(sock, &wfd);                                   //把sock放入写描述符集
if(SOCKET_ERROR != select(0, &rfd, NULL, 0, 0))       //测试这两个描述符集,永不超时 其中rfd只用来测试读 wfd只用来测试写
{//没有错误
if(FD_ISSET(sock, &rfd))                          //sock可读
{
char buff[256];
ret = recv(sock, buff, 256, 0);
if(ret == 0 || ret == SOCKET_ERROR )
{
cout << "接收数据失败：" << WSAGetLastError() << endl;
return 0;
}
buff[ret] = '\0';
cout << buff << endl;
}
}
if(SOCKET_ERROR != select(0, NULL, &wfd, 0, 0))       //测试这两个描述符集,永不超时 其中rfd只用来测试读 wfd只用来测试写
{
if(FD_ISSET(sock, &wfd))                          //sock可写
{
string buf;
cin >> buf;
ret = send(sock, buf.c_str(), buf.size(), 0);
if(ret != buf.size())
{
cout << "发送数据失败：" << WSAGetLastError() << endl;
return 0;
}
}
}
}



