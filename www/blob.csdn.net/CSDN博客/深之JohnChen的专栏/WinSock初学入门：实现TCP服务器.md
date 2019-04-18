# WinSock初学入门：实现TCP服务器 - 深之JohnChen的专栏 - CSDN博客

2006年03月29日 14:50:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1426


**WinSock初学入门：实现TCP服务器**

（一）介绍：

　　本文主要采用Winsock API实现了一个Tcp服务器。对初学者而言，相当实用。 

　　（二）准备工作：

　　1、使用vc6向导建立控制台工程（console application），并选择支持[MFC](http://www.yesky.com/key/350/10350.html)。

　　2、在stdafx.h中加入头文件conio.h和winsock2.h。

　　3、在头文件中加入：#pragma comment(lib, "ws2_32.lib")。

　　（三）主程序：主要是启动服务器线程并等待用户退出的命令。

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
int nRetCode = 0; 

cout << "Press ESCAPE to terminate program/r/n";
AfxBeginThread(ServerThread,0); //启动一条线程
while(_getch()!=27); //用户按ESC键则退出

return nRetCode;
}
　　（四）线程代码：一个简单的tcp服务器，返回客户的ip地址。

UINT ServerThread(LPVOID pParam)
{ 
cout << "Starting up TCP [server](http://www.yesky.com/key/4216/19216.html)/r/n";

//A SOCKET is simply a typedef for an unsigned int.
//In Unix, socket handles were just about same as file 
//handles which were again unsigned ints.
//Since this cannot be entirely true under Windows
//a new data type called SOCKET was defined.

//定义一个socket句柄作为服务器
SOCKET server;

//WSADATA is a struct that is filled up by the call 
//to WSAStartup

//这是一个结构体，用于调用函数WSAStartup时作为参数
WSADATA wsaData;

//The sockaddr_in specifies the address of the socket
//for TCP/IP [sockets](http://www.yesky.com/key/4163/19163.html). Other protocols use similar structures.

//存放ip地址的结构体
sockaddr_in local;

//WSAStartup initializes the program for calling WinSock.
//The first parameter specifies the highest version of the 
//WinSock specification, the program is allowed to use.

//初始化winsock，每次必须最先调用
int wsaret=WSAStartup(0x101,&wsaData);

//WSAStartup returns zero on success.
//If it fails we exit.

//测试初始化是否成功
if(wsaret!=0)
{
return 0;
}

//Now we populate the sockaddr_in structure

//填充服务器的ip地址和端口号
local.sin_family=AF_INET; //Address family
local.sin_addr.s_addr=INADDR_ANY; //Wild card IP address
local.sin_port=htons((u_short)8888); //port to use

//the socket function creates our SOCKET

//建立一个tcp socket
server=socket(AF_INET,SOCK_STREAM,0);

//If the socket() function fails we exit
if(server==[INVALID](http://www.yesky.com/key/3829/13829.html)_SOCKET)
{
return 0;
}

//bind links the socket we just created with the sockaddr_in 
//structure. Basically it connects the socket with 
//the local address and a specified port.
//If it returns non-zero quit, as this indicates error

//绑定服务器的ip地址和端口号
if(bind(server,(sockaddr*)&local,sizeof(local))!=0)
{
return 0;
}

//listen instructs the socket to listen for incoming 
//connections from clients. The [second](http://www.yesky.com/key/4225/19225.html) arg is the backlog

//监听客户端请求，最大同时连接数10
if(listen(server,10)!=0)
{
return 0;
}

//we will need variables to hold the client socket.
//thus we declare them here.

//接受请求后，实际同客户端socket进行交互的SOCKET client
SOCKET client;
sockaddr_in from;
int fromlen=sizeof(from);

while(true)//we are looping endlessly
{
char temp[512];

//accept() will accept an incoming
//client connection

//接受一个连接请求，并返回一个同客户端交互的socket给变量client
client=accept(server,
(struct sockaddr*)&from,&fromlen);

//得到客户端的ip地址，并写入缓冲区
sprintf(temp,"Your IP is %s/r/n",inet_ntoa(from.sin_addr));

//we simply send this string to the client

// 发送回缓冲中的内容（即：客户端的ip地址）
send(client,temp,strlen(temp),0);
cout << "Connection from " << inet_ntoa(from.sin_addr) <<"/r/n";

//close the client socket
//关闭连接
closesocket(client);

}

//closesocket() closes the socket and releases the socket descriptor
//释放socket资源
closesocket(server);

//originally this function probably had some use
//currently this is just for backward compatibility
//but it is safer to call it as I still believe some
//implementations use this to terminate use of WS2_32.DLL 
//必须作的最后的事
WSACleanup();

return 0;
}
　　（五）运行：

　　服务器端：

E:/work/Server/Debug>server
Press ESCAPE to terminate program
Starting up TCP server
Connection from 203.200.100.122
Connection from 127.0.0.1
E:/work/Server/Debug>
　　客户端：

E:/work/Server/Debug>telnet 202.89.211.88 8888
Trying 202.89.211.88...
Connected to 202.89.211.88.
Escape character is ’^]’.
Your IP is 203.200.100.122
Connection closed by foreign host.
　　（六）说明：

　　这只是一个极为简单的tcp server程序，目的是帮助初学者学习winsock编程。因此，程序本着简单的原则，对于很多方面考虑都较少。比如，退出时候可能有socket句柄未释放的危险；只有一条线程，客户端很多时不够实用等等。这些都需要进一步的考虑。            

