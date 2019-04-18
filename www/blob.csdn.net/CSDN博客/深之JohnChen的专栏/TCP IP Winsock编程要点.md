# TCP/IP Winsock编程要点 - 深之JohnChen的专栏 - CSDN博客

2005年12月11日 19:26:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1539


利用Winsock编程由同步和异步方式，同步方式逻辑清晰，编程专注于应用，在抢先式的多任务操作系统中(WinNt、Win2K)采用多线程方式效率基本达到异步方式的水平，应此以下为同步方式编程要点。 

　　1、快速通信 

　　Winsock的Nagle算法将降低小数据报的发送速度，而系统默认是使用Nagle算法,使用 
int setsockopt( 

SOCKET s, 

int level, 

int optname, 

const char FAR *optval, 

int optlen 

);函数关闭它 
　　例子： 

SOCKET sConnect; 

sConnect=::socket(AF_INET,SOCK_STREAM,IPPROTO_TCP); 

int bNodelay = 1; 

int err; 

err = setsockopt( 

sConnect, 

IPPROTO_TCP, 

TCP_NODELAY, 

(char *)&bNodelay, 

sizoeof(bNodelay));//不采用延时算法 

if (err != NO_ERROR) 

TRACE ("setsockopt failed for some reason/n");; 
　　2、SOCKET的SegMentSize和收发缓冲 

　　TCPSegMentSize是发送接受时单个数据报的最大长度，系统默认为1460，收发缓冲大小为8192。 

　　在SOCK_STREAM方式下，如果单次发送数据超过1460，系统将分成多个数据报传送，在对方接受到的将是一个数据流，应用程序需要增加断帧的判断。当然可以采用修改注册表的方式改变1460的大小，但MicrcoSoft认为1460是最佳效率的参数，不建议修改。 

　　在工控系统中，建议关闭Nagle算法，每次发送数据小于1460个字节（推荐1400），这样每次发送的是一个完整的数据报，减少对方对数据流的断帧处理。 

　　3、同步方式中减少断网时connect函数的阻塞时间 

　　同步方式中的断网时connect的阻塞时间为20秒左右，可采用gethostbyaddr事先判断到服务主机的路径是否是通的，或者先ping一下对方主机的IP地址。 

　　A、采用gethostbyaddr阻塞时间不管成功与否为4秒左右。 

　　例子： 

LONG lPort=3024; 

struct sockaddr_in ServerHostAddr;//服务主机地址 

ServerHostAddr.sin_family=AF_INET; 

ServerHostAddr.sin_port=::htons(u_short(lPort)); 

ServerHostAddr.sin_addr.s_addr=::inet_addr("192.168.1.3"); 

HOSTENT* pResult=gethostbyaddr((const char *) & 

(ServerHostAddr.sin_addr.s_addr),4,AF_INET); 

if(NULL==pResult) 

{ 

int nErrorCode=WSAGetLastError(); 

TRACE("gethostbyaddr errorcode=%d",nErrorCode); 

} 

else 

{ 

TRACE("gethostbyaddr %s/n",pResult->h_name);; 

} 

　　B、采用PING方式时间约2秒左右 

　　暂略 

4、同步方式中解决recv，send阻塞问题 

　　采用select函数解决，在收发前先检查读写可用状态。 

　　A、读 

　　例子： 

TIMEVAL tv01 = {0, 1};//1ms钟延迟,实际为0-10毫秒 

int nSelectRet; 

int nErrorCode; 

FD_SET fdr = {1, sConnect}; 

nSelectRet=::select(0, &fdr, NULL, NULL, &tv01);//检查可读状态 

if(SOCKET_ERROR==nSelectRet) 

{ 

nErrorCode=WSAGetLastError(); 

TRACE("select read status errorcode=%d",nErrorCode); 

::closesocket(sConnect); 

goto 重新连接（客户方），或服务线程退出（服务方）; 

} 

if(nSelectRet==0)//超时发生，无可读数据 

{ 

继续查读状态或向对方主动发送 

} 

else 

{ 

读数据 

} 
　　B、写 

TIMEVAL tv01 = {0, 1};//1ms钟延迟,实际为9-10毫秒 

int nSelectRet; 

int nErrorCode; 

FD_SET fdw = {1, sConnect}; 

nSelectRet=::select(0, NULL, NULL,&fdw, &tv01);//检查可写状态 

if(SOCKET_ERROR==nSelectRet) 

{ 

nErrorCode=WSAGetLastError(); 

TRACE("select write status errorcode=%d",nErrorCode); 

::closesocket(sConnect); 

//goto 重新连接（客户方），或服务线程退出（服务方）; 

} 

if(nSelectRet==0)//超时发生，缓冲满或网络忙 

{ 

//继续查写状态或查读状态 

} 

else 

{ 

//发送 

} 
　　5、改变TCP收发缓冲区大小 

　　系统默认为8192，利用如下方式可改变。 

SOCKET sConnect; 

sConnect=::socket(AF_INET,SOCK_STREAM,IPPROTO_TCP); 

int nrcvbuf=1024*20; 

int err=setsockopt( 

sConnect, 

SOL_SOCKET, 

SO_SNDBUF,//写缓冲，读缓冲为SO_RCVBUF 

(char *)&nrcvbuf, 

sizeof(nrcvbuf)); 

if (err != NO_ERROR) 

{ 

TRACE("setsockopt Error!/n"); 

} 

在设置缓冲时，检查是否真正设置成功用 

int getsockopt( 

SOCKET s, 

int level, 

int optname, 

char FAR *optval, 

int FAR *optlen 

); 
　　6、服务方同一端口多IP地址的bind和listen 

　　在可靠性要求高的应用中，要求使用双网和多网络通道，再服务方很容易实现，用如下方式可建立客户对本机所有IP地址在端口3024下的请求服务。 

SOCKET hServerSocket_DS=INVALID_SOCKET; 

struct sockaddr_in HostAddr_DS;//服务器主机地址 

LONG lPort=3024; 

HostAddr_DS.sin_family=AF_INET; 

HostAddr_DS.sin_port=::htons(u_short(lPort)); 

HostAddr_DS.sin_addr.s_addr=htonl(INADDR_ANY); 

hServerSocket_DS=::socket( AF_INET, SOCK_STREAM,IPPROTO_TCP); 

if(hServerSocket_DS==INVALID_SOCKET) 

{ 

AfxMessageBox("建立数据服务器SOCKET 失败!"); 

return FALSE; 

} 

if(SOCKET_ERROR==::bind(hServerSocket_DS,(struct 

sockaddr *)(&(HostAddr_DS)),sizeof(SOCKADDR))) 

{ 

int nErrorCode=WSAGetLastError (); 

TRACE("bind error=%d/n",nErrorCode); 

AfxMessageBox("Socket Bind 错误!"); 

return FALSE; 

} 

if(SOCKET_ERROR==::listen(hServerSocket_DS,10))//10个客户 

{ 

AfxMessageBox("Socket listen 错误!"); 

return FALSE; 

} 

AfxBeginThread(ServerThreadProc,NULL,THREAD_PRIORITY_NORMAL); 
　　在客户方要复杂一些，连接断后，重联不成功则应换下一个IP地址连接。也可采用同时连接好后备用的方式。 

　　7、用TCP/IP Winsock实现变种Client/Server 

　　传统的Client/Server为客户问、服务答，收发是成对出现的。而变种的Client/Server是指在连接时有客户和服务之分，建立好通信连接后，不再有严格的客户和服务之分，任何方都可主动发送，需要或不需要回答看应用而言，这种方式在工控行业很有用，比如RTDB作为I/O Server的客户，但I/O Server也可主动向RTDB发送开关状态变位、随即事件等信息。在很大程度上减少了网络通信负荷、提高了效率。 

　　采用1-6的TCP/IP编程要点，在Client和Server方均已接收优先，适当控制时序就能实现。

