# HTTP协议学习【转】 - weixin_33985507的博客 - CSDN博客
2014年09月06日 13:29:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
    面试过程中又一个常见的问题，http协议，因为做服务器开发如果用http协议的话，现在各种开源软件都封装好了，python中只需要简单的继承定义好的类，重写get或者post等方法，几行代码就可以搭建起来一个简单的http服务器，导致底层对程序员来说都是透明的了。但是面试中追求这个底层的问题还不少，所以最近入手了一本据说一天入门http协议的书籍《图解http》阅读一番，才觉http协议原来是这个样子~这里总结一下自己的学习心得吧。
    现在理解http协议其实只是一些规定好的通信标准，其实发送的均为字符串，去处理字符串即可。（不知道这句话能不能理解，后面会再次解释一下）。
    HTTP协议在TCP/IP协议栈中属于应用层协议，如果基于SSL或者TSL，这个协议就演变为更加安全的HTTPS协议。默认HTTP端口号80，HTTPS默认端口号443。
    与HTTP协议协同工作的重要协议DNS，TCP，IP等协议，其中DNS同为应用层协议。
![](https://images0.cnblogs.com/blog/342726/201409/061109100945761.jpg)
    HTTP协议规定，请求从客户端发出，最后服务器端响应该请求并返回。这是目前HTTP协议的规定，服务器不支持主动响应，所以目前的HTTP协议版本都是基于请求，然后响应的这种模型。
![](https://images0.cnblogs.com/blog/342726/201409/061105412199473.jpg)
    另外，HTTP协议是一种无状态的应用层协议，即使同一个客户端的两次连续请求，在协议规定中也没有对应关系。所以为了解决无状态的这个问题，存在其他的技术解决方案进行补充。
**HTTP请求报文和响应报文：**
    请求报文： 请求方法 ， 请求URI ， 协议版本 ， 可选的请求首部字段和内容实体。（严格定义的字符串）
![](https://images0.cnblogs.com/blog/342726/201409/061116203138427.jpg)
    响应报文：协议版本 ， 状态码 ， 用以解释状态码的原因短语 ， 可选的响应首部字段以及实体主体。（严格定义的字符串）
![](https://images0.cnblogs.com/blog/342726/201409/061118452506247.jpg)
    以上两个截图自己利用socket写了一个简单的服务端和客户端程序，分别用浏览器发送请求和向百度发送请求得到的字符串打印。HTTP具体还是基于TCP协议的，所以各种HTTP服务器底层应该是同样的机制，不过它们处理了很多关于HTTP协议处理的部分，这部分更多的感觉是协议规定好的字符串的内容，各种方法，各种头的属性。
**HTTP请求，响应的模拟**
    另外除了自己利用socket去编辑，更简单观察相应的方法可以利用telnet去发送http请求头至百度。
        telenet 开启方法 控制面板-程序-window程序开启与关闭-开启telnet客户端，这样就能够在cmd中使用telnet命令了。使用起来不是很方便。
    异或是利用wireshark直接去抓取你访问某个网站整个过程的数据包也可以观察~。以下是抓取的请求www.baidu.com的数据包。
![](https://images0.cnblogs.com/blog/342726/201409/061131441103167.jpg)
    整个请求，响应过程包过滤后如下：
![](https://images0.cnblogs.com/blog/342726/201409/061242328137866.jpg)
   其中可以可到tcp的三次握手，以及请求，响应的过程。但是其中有几个包内容是TCP segment of a reassembled PDU不是很清楚，可以看到，tcp每次发送均有响应的ACK回应。具体的TCP得参考TCP协议的知识。
   另外HTTP1.0和HTTP1.1的有一个主要区别就是HTTP1.1引入了持续连接的概念，不然完成一次请求必然导致TCP三次握手成本太高，引入持续连接的概念提高传输的效率。
**HTTP请求支持的方法**
GET   请求获取Request-URI所标识的资源
POST 在Request-URI所标识的资源后附加新的数据
HEAD 请求获取由Request-URI所标识的资源的响应消息报头
PUT    请求服务器存储一个资源，并用Request-URI作为其标识
DELETE 请求服务器删除Request-URI所标识的资源
OPTIONS 请求查询服务器的性能，或者查询与资源相关的选项和需求
TRACE   请求服务器回送收到的请求信息，主要用于测试或诊断
CONNECT 隧道机制
**HTTP的状态响应码**
1XX:指示信息，请求收到，继续处理
2XX:成功，操作成功收到，分析，接受
3XX:完成请求必须进一步处理，重定向
4XX:请求包含一个错误语法，不能完成。指示客户端错误
5XX:服务器执行一个有效请求失败，指示服务端错误
**其他HTTP重要的知识就是报头信息中的具体字段信息：**
**报头字段的格式为 （名字）“：”（空格）（值）；**
通用报头字段：
Cache-Control
Date
Connection
请求报头字段：
Accept
Accept-Charset
Accept-Encoding
Accept-Language
Authorization
Host
User-Agent
响应报头字段：
Location
Server
实体报头字段：
Content-Encoding
Content-Language
Content-Length
Content-Type
Last-Modified
Expires
最后贴出自己利用winsock实现的测试http内容的简单程序：
实现流程图如下：
![](https://images0.cnblogs.com/blog/342726/201409/061303252506005.jpg)
|12345678910111213141516171819202122232425262728293031323334353637383940414243|`//TCP 客户端``#include <iostream>``#include <stdio.h>``#include <stdlib.h>``#include <windows.h>``#include <string.h>``#include <string>``using``namespace``std;``#pragma comment(lib,"ws2_32.lib")``#define PORT 8080``#define BACKLOG 10``int``main(``void``) {``    ``//client module ``    ``WSADATA r_wsadata;``    ``if``( WSAStartup(MAKEWORD(2,2),&r_wsadata) ) {``        ``cout<<``"WSAStartup error"``<<endl;``        ``WSACleanup();``    ``}``    ``SOCKET client_s;``    ``if``( (client_s = socket(AF_INET,SOCK_STREAM,0)) == INVALID_SOCKET ) {``        ``cout<<``"socket error"``<<endl;``        ``WSACleanup();``    ``}``    ``sockaddr_in server_addr;``    ``server_addr.sin_family = AF_INET;``    ``server_addr.sin_port = htons(80);``    ``server_addr.sin_addr.s_addr = inet_addr(``"202.108.22.5"``);``    ``if``( connect(client_s,(sockaddr *)&server_addr,``sizeof``(server_addr)) ) {``        ``cout<<``"connect error"``<<endl;``        ``WSACleanup();``    ``}``    ``char``*sbuf =``"GET / HTTP/1.1\r\nHost:www.baidu.com\r\nUser-Agent: Mozilla/5.0 (Windows NT 5.1; rv:10.0.2) Gecko/20100101 Firefox/10.0.2\r\nAccept-Language: zh-cn,zh;q=0.5\r\nAccept-Encoding: gzip, deflate\r\nConnection:Keep-Alive\r\n\r\n"``;``    ``char``*sbuf1 =``"GET / HTTP/1.1\r\nHost:www.baidu.com\r\nUser-Agent:Mozilla/5.0 (Windows;U;Windows NT 5.1; en-US; rv:1.7.6)\r\nGecko/20050225 Firefox/1.0.1\r\nConnection: Keep Alive\r\n\r\n"``;``    ``int``len = ``strlen``(sbuf) + 1;``    ``int``sendlen = 0;``    ``int``res = send(client_s,sbuf,len,0);``    ``char``buf[10240];``    ``int``recvlen = recv(client_s,buf,``sizeof``(buf),0);``    ``if``(recvlen > 0)``        ``buf[recvlen] = ``'\0'``;``    ``cout<<string(buf)<<endl;``    ``system``(``"pause"``);``}`|
|123456789101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657585960616263|`//TCP``//服务器端程序``#include <iostream>``#include <stdio.h>``#include <stdlib.h>``#include <windows.h>``#include <string.h>``#include <string>``using``namespace``std;``#pragma comment(lib,"ws2_32.lib")``#define PORT 8080``#define BACKLOG 10``int``main(``void``) {``    ``    ``//server module ``    ``WSADATA r_wsadata;``    ``if``( WSAStartup(MAKEWORD(2,2),&r_wsadata) ) {``        ``cout<<``"WSAStartup error"``<<endl;``        ``WSACleanup();``        ``exit``(0);``    ``}``    ``SOCKET server_s;``    ``if``( ( server_s = socket(AF_INET,SOCK_STREAM,0) ) == INVALID_SOCKET ) {``        ``cout<<``"socket error"``<<endl;``        ``WSACleanup();``        ``exit``(0);``    ``}``    ``sockaddr_in server_s_addr;``    ``server_s_addr.sin_family = AF_INET;``    ``server_s_addr.sin_port = htons(PORT);``    ``server_s_addr.sin_addr.s_addr = INADDR_ANY;``    ``if``( bind(server_s,(sockaddr *)&server_s_addr,``sizeof``(server_s_addr)) ) {``        ``cout<<``"bind error"``<<endl;``        ``WSACleanup();``        ``exit``(0);``    ``}``    ``if``( listen(server_s,BACKLOG) ) {``        ``cout<<``"listen error"``<<endl;``        ``WSACleanup();``        ``exit``(0);``    ``}``    ``while``(``true``) {   ``        ``SOCKET accept_socket;``        ``sockaddr_in client_s_addr;``        ``int``addr_len = ``sizeof``(client_s_addr);``        ``if``( (accept_socket = accept(server_s,(sockaddr *)&client_s_addr,&addr_len)) == -1 ) {``            ``cout<<``"accept error"``<<endl;``            ``WSACleanup();``            ``exit``(0);``        ``}``        ``cout<<``"request from host:"``<<string( inet_ntoa(client_s_addr.sin_addr) )<<endl;``        ``cout<<``"-----------------------http request----------------------"``<<endl;``        ``//receive``        ``char``buf[1024];``        ``int``readLen = -1;``        ``while``(readLen < 0)``            ``readLen = recv(accept_socket,buf,``sizeof``(buf),0);``        ``buf[readLen] = ``'\0'``;``        ``cout<<string(buf)<<endl;``        ``closesocket(accept_socket);``    ``}``}`完美世界 http://www.23cat.com/Contents_51864.html 　　戮仙 http://www.23cat.com/Book_51960.html|
