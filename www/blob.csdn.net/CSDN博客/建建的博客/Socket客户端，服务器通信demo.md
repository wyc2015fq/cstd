# Socket客户端，服务器通信demo - 建建的博客 - CSDN博客
2017年02月02日 14:58:36[纪建](https://me.csdn.net/u013898698)阅读数：929
socket.h
```
#ifndef SOCKET_H
#define SOCKET_H
#include <WinSock2.h>
#include <string>
enum TypeSocket {BlockingSocket, NonBlockingSocket};
class Socket 
{
public:
    virtual ~Socket();
    Socket(const Socket&);
    Socket& operator=(Socket&);
    std::string ReceiveLine();
    std::string ReceiveBytes();
    void   Close();
    // The parameter of SendLine is not a const reference
    // because SendLine modifes the std::string passed.
    //参数SendLine不是一个const引用
    //因为SendLine modifes的std :: string传递。
    void   SendLine (std::string);
    // The parameter of SendBytes is a const reference
    // because SendBytes does not modify the std::string passed 
    // (in contrast to SendLine).
    //参数SendBytes的是一个const引用
    //，因为SendBytes不修改通过的std :: string
    //（在对比SendLine）。
    void   SendBytes(const std::string&);
protected:
    friend class SocketServer;
    friend class SocketSelect;
    Socket(SOCKET s);
    Socket();
    SOCKET s_;
    int* refCounter_;
private:
    static void Start();
    static void End();
    static int  nofSockets_;
};
class SocketClient : public Socket 
{
public:
    SocketClient(const std::string& host, int port);
};
class SocketServer : public Socket 
{
public:
    SocketServer(int port, int connections, TypeSocket type=BlockingSocket);
    Socket* Accept();
};
// http://msdn.microsoft.com/library/default.asp?url=/library/en-us/winsock/wsapiref_2tiq.asp
class SocketSelect 
{
public:
    SocketSelect(Socket const * const s1, Socket const * const s2=NULL, TypeSocket type=BlockingSocket);
    bool Readable(Socket const * const s);
private:
    fd_set fds_;
}; 
#endif
```
socket.cpp
```
#include "Socket.h"
#include <iostream>
using namespace std;
int Socket::nofSockets_= 0;
void Socket::Start() 
{
    if (!nofSockets_) 
    {
        WSADATA info;
        if (WSAStartup(MAKEWORD(2,0), &info)) 
        {
            throw "Could not start WSA";
        }
    }
    ++nofSockets_;
}
void Socket::End() 
{
    WSACleanup();
}
Socket::Socket() : s_(0) 
{
    Start();
    // UDP: use SOCK_DGRAM instead of SOCK_STREAM
    s_ = socket(AF_INET,SOCK_STREAM,0);
    if (s_ == INVALID_SOCKET) 
    {
        throw "INVALID_SOCKET";
    }
    refCounter_ = new int(1);
}
Socket::Socket(SOCKET s) : s_(s) 
{
    Start();
    refCounter_ = new int(1);
};
Socket::~Socket() 
{
    if (! --(*refCounter_)) 
    {
        Close();
        delete refCounter_;
    }
    --nofSockets_;
    if (!nofSockets_) End();
}
Socket::Socket(const Socket& o) 
{
    refCounter_=o.refCounter_;
    (*refCounter_)++;
    s_  =o.s_;
    nofSockets_++;
}
Socket& Socket::operator=(Socket& o) 
{
    (*o.refCounter_)++;
    refCounter_=o.refCounter_;
    s_ =o.s_;
    nofSockets_++;
    return *this;
}
void Socket::Close() 
{
    closesocket(s_);
}
std::string Socket::ReceiveBytes() 
{
    std::string ret;
    char buf[1024];
    while (1) 
    {
        u_long arg = 0;
        if (ioctlsocket(s_, FIONREAD, &arg) != 0)
            break;
        if (arg == 0)
            break;
        if (arg > 1024) arg = 1024;
        int rv = recv (s_, buf, arg, 0);
        if (rv <= 0) break;
        std::string t;
        t.assign (buf, rv);
        ret += t;
    }
    return ret;
}
std::string Socket::ReceiveLine() 
{
    std::string ret;
    while (1) 
    {
        char r;
        switch(recv(s_, &r, 1, 0)) 
        {
      case 0: // not connected anymore;
          // ... but last line sent
          // might not end in \n,
          // so return ret anyway.
          return ret;
      case -1:
          return "";
          //      if (errno == EAGAIN) {
          //        return ret;
          //      } else {
          //      // not connected anymore
          //      return "";
          //      }
        }
        ret += r;
        if (r == '\n')  return ret;
    }
}
void Socket::SendLine(std::string s) 
{
    s += '\n';
    send(s_,s.c_str(),s.length(),0);
}
void Socket::SendBytes(const std::string& s) 
{
    send(s_,s.c_str(),s.length(),0);
}
SocketServer::SocketServer(int port, int connections, TypeSocket type) 
{
    sockaddr_in sa;
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = PF_INET;             
    sa.sin_port = htons(port);          
    s_ = socket(AF_INET, SOCK_STREAM, 0);
    if (s_ == INVALID_SOCKET) {
        throw "INVALID_SOCKET";
    }
    if(type==NonBlockingSocket) {
        u_long arg = 1;
        ioctlsocket(s_, FIONBIO, &arg);
    }
    /* bind the socket to the internet address */
    if (bind(s_, (sockaddr *)&sa, sizeof(sockaddr_in)) == SOCKET_ERROR) {
        closesocket(s_);
        throw "INVALID_SOCKET";
    }
    listen(s_, connections);                               
}
Socket* SocketServer::Accept() 
{
    SOCKET new_sock = accept(s_, 0, 0);
    if (new_sock == INVALID_SOCKET) 
    {
        int rc = WSAGetLastError();
        if(rc==WSAEWOULDBLOCK) 
        {
            return 0; // non-blocking call, no request pending
        }
        else 
        {
            throw "Invalid Socket";
        }
    }
    Socket* r = new Socket(new_sock);
    return r;
}
SocketClient::SocketClient(const std::string& host, int port) : Socket() 
{
    std::string error;
    hostent *he;
    if ((he = gethostbyname(host.c_str())) == 0) 
    {
        error = strerror(errno);
        throw error;
    }
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr = *((in_addr *)he->h_addr);
    memset(&(addr.sin_zero), 0, 8); 
    if (::connect(s_, (sockaddr *) &addr, sizeof(sockaddr))) 
    {
        error = strerror(WSAGetLastError());
        throw error;
    }
}
SocketSelect::SocketSelect(Socket const * const s1, Socket const * const s2, TypeSocket type) 
{
    FD_ZERO(&fds_);
    FD_SET(const_cast<Socket*>(s1)->s_,&fds_);
    if(s2) 
    {
        FD_SET(const_cast<Socket*>(s2)->s_,&fds_);
    }     
    TIMEVAL tval;
    tval.tv_sec  = 0;
    tval.tv_usec = 1;
    TIMEVAL *ptval;
    if(type==NonBlockingSocket) 
    {
        ptval = &tval;
    }
    else 
    { 
        ptval = 0;
    }
    if (select (0, &fds_, (fd_set*) 0, (fd_set*) 0, ptval) == SOCKET_ERROR) 
        throw "Error in select";
}
bool SocketSelect::Readable(Socket const* const s) 
{
    if (FD_ISSET(s->s_,&fds_)) return true;
    return false;
}
```
一个简单的客户端
下面这个简单的客户端连接到[www.google.ch](http://www.google.ch/)并得到其前面的网站：
```
#include "Socket.h"
#include <iostream>
using namespace std;
int main() 
{
    try 
    {
        SocketClient s("www.google.com", 80);
        s.SendLine("GET / HTTP/1.0");
        s.SendLine("Host: www.google.com");
        s.SendLine("");
        while (1) 
        {
            string l = s.ReceiveLine();
            if (l.empty()) break;
            cout << l;
            cout.flush();
        }
    } 
    catch (const char* s) 
    {
        cerr << s << endl;
    } 
    catch (std::string s) 
    {
        cerr << s << endl;
    } 
    catch (...) 
    {
        cerr << "unhandled exception\n";
    }
    return 0;
}
```
一个简单的echo服务器
以下简单的服务器上打开2000端口，并等待传入的连接。每个连接与写（呼应）同一行的回答。如果你想测试的服务器，使用telnet本地主机2000
```
/* 
EchoServer.cpp
Copyright (C) 2002-2004 René Nyffenegger
This source code is provided 'as-is', without any express or implied
warranty. In no event will the author be held liable for any damages
arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:
1. The origin of this source code must not be misrepresented; you must not
claim that you wrote the original source code. If you use this source code
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original source code.
3. This notice may not be removed or altered from any source distribution.
René Nyffenegger rene.nyffenegger@adp-gmbh.ch
*/
#include "Socket.h"
#include <process.h>
#include <string>
unsigned __stdcall Answer(void* a) 
{
    Socket* s = (Socket*) a;
    while (1) 
    {
        std::string r = s->ReceiveLine();
        if (r.empty()) break;
        s->SendLine(r);
    }
    delete s;
    return 0;
}
int main(int argc, char* argv[]) 
{
    SocketServer in(2000,5);
    while (1) 
    {
        Socket* s=in.Accept();
        unsigned ret;
        _beginthreadex(0,0,Answer,(void*) s,0,&ret);
    }
    return 0;
}
```
使用Socket类的代理
您需要编译这个代理变色龙类。用法：的代理<port proxy><addr服务器><port服务器>。
此代理将听取<port proxy>每当它接收到一个连接器，继电器<port<addr服务器>服务器>交通。这使得它非常适合看什么一个SMTP服务器，或同等NNTP客户端的交流与NNTP服务器的SMTP客户端交流。
```
/* 
Proxy.cpp
Copyright (C) 2002-2004 René Nyffenegger
This source code is provided 'as-is', without any express or implied
warranty. In no event will the author be held liable for any damages
arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:
1. The origin of this source code must not be misrepresented; you must not
claim that you wrote the original source code. If you use this source code
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original source code.
3. This notice may not be removed or altered from any source distribution.
René Nyffenegger rene.nyffenegger@adp-gmbh.ch
*/
#include "Socket.h"
#include <iostream>
#include <process.h>
#include <sstream>
int         portProxy;
std::string addrServer;
int         portServer;
unsigned __stdcall RunProxyThread (void* a) 
{
    Socket*      s = (Socket*) a;
    SocketClient c(addrServer, portServer);
    while (1) 
    {
        SocketSelect sel(s, &c);
        bool still_connected = true;
        if (sel.Readable(s)) 
        {
            std::string bytes = s->ReceiveBytes();
            c.SendBytes(bytes);
            std::cout << "Server: " << bytes << std::endl;
            if (bytes.empty()) still_connected=false;
        }
        if (sel.Readable(&c)) 
        {
            std::string bytes = c.ReceiveBytes();
            s->SendBytes(bytes);
            std::cout << "Client: " << bytes << std::endl;
            if (bytes.empty()) still_connected=false;
        }
        if (! still_connected) 
        {
            break;
        }
    }
    delete s;
    return 0;
}
int main(int argc, char* argv[]) 
{
    if (argc < 4) 
    {
        std::cout << "Usage:" << std::endl;
        std::cout << "  proxy <port proxy> <addr server> <port server>" << std::endl;
        std::cout << std::endl;
        std::cout << "  This proxy will then listen on <port proxy> and whenever it receives" << std::endl;
        std::cout << "  a connection, relays the traffic to the <port server> of <addr server>." << std::endl;
        std::cout << "  This makes it ideal to see what an SMTP Client exchanges with a SMTP Server," << std::endl;
        std::cout << "  or equally what a NNTP client exchanges with an NNTP Server." << std::endl << std::endl;
        return -1;
    }
    std::stringstream s; 
    s<<argv[1]; s>>portProxy; s.clear();
    addrServer=argv[2];
    s<<argv[3]; s>>portServer;
    SocketServer in(portProxy,5);
    while (1) 
    {
        Socket* s=in.Accept();
        unsigned ret;
        _beginthreadex(0, 0, RunProxyThread,(void*) s,0,&ret);
    }
    return 0;
}
```
另见：提琴手：记录您的计算机和互联网之间的所有HTTP流量的HTTP调试代理。提琴手可以让你观看的HTTP流量，设置断点，以及“小提琴”传入或传出数据。提琴手的设计是比使用Netmon或跟腱简单得多。
消息经销商
消息经销商是一个服务器侦听端口2000。任何数量的客户端可以连接到该服务器。当客户端发送消息到服务器，服务器将播出这个消息，所有其他客户端，但不发送消息。
```
/* 
MsgDistributor.cpp
Copyright (C) 2002-2004 René Nyffenegger
This source code is provided 'as-is', without any express or implied
warranty. In no event will the author be held liable for any damages
arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:
1. The origin of this source code must not be misrepresented; you must not
claim that you wrote the original source code. If you use this source code
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original source code.
3. This notice may not be removed or altered from any source distribution.
René Nyffenegger rene.nyffenegger@adp-gmbh.ch
*/
#include "Socket.h"
#include <process.h>
#include <string>
#include <list>
typedef std::list<Socket*> socket_list;
socket_list g_connections;
unsigned __stdcall Connection(void* a) 
{
    Socket* s = (Socket*) a;
    g_connections.push_back(s);
    s->SendLine("Welcome to the Message Distributor");
    while (1) 
    {
        std::string r = s->ReceiveLine();
        if (r.empty()) break;
        for (socket_list::iterator os =g_connections.begin();
            os!=g_connections.end(); 
            os++) 
        {
                if (*os != s) (*os)->SendLine(r);
        }
    }
    g_connections.remove(s);
    delete s;
    return 0;
}
int main() 
{
    SocketServer in(2000,5);
    while (1) 
    {
        Socket* s=in.Accept();
        unsigned ret;
        _beginthreadex(0,0,Connection,(void*) s,0,&ret);
    }
    return 0;
}
```
## Download the files
You can download the source code and the exefiles as a
[zip file](http://www.adp-gmbh.ch/win/misc/socket.zip)consisting of- socket.h
- Chameleon.h
- Chameleon.cpp
- EchoServer.cpp
- MsgDistributor.cpp
- Proxy.cpp
- Socket.cpp
- EchoServer.exe
- Proxy.exe
- makefile
I was able to compile the sources with [mingw](http://www.adp-gmbh.ch/win/misc/mingw/index.html) using the supplied makefile.
