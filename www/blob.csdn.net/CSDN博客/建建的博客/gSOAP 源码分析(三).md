# gSOAP 源码分析(三) - 建建的博客 - CSDN博客
2017年02月17日 15:47:28[纪建](https://me.csdn.net/u013898698)阅读数：496
gSOAP 源码分析(三)
2012-5-25 flyfish
一 HTTP请求方法
GET     请求获取Request-URI所标识的资源
POST    在Request-URI所标识的资源后附加新的数据
HEAD    请求获取由Request-URI所标识的资源的响应消息报头
PUT     请求服务器存储一个资源，并用Request-URI作为其标识
DELETE  请求服务器删除Request-URI所标识的资源
TRACE   请求服务器回送收到的请求信息，主要用于[测试](http://lib.csdn.net/base/softwaretest)或诊断
CONNECT 保留将来使用
OPTIONS 请求查询服务器的性能，或者查询与资源相关的选项和需求
gSOAP对应的函数如下
  int (*fpost)(struct soap*, const char*, const char*, int, const char*, const char*, size_t);
  int (*fget)(struct soap*); /* HTTP GET hook (not set by default) */
  int (*fput)(struct soap*); /* HTTP PUT hook (handled as POST by default) */
  int (*fdel)(struct soap*); /* HTTP DELETE hook (not set by default) */
  int (*fopt)(struct soap*); /* HTTP OPTIONS hook (not set by default) */
  int (*fhead)(struct soap*); /* HTTP HEAD hook (not set by default) */
  int (*fform)(struct soap*); /* HTTP/HTML form handler for plugins */
二 http的处理
static soap_wchar soap_getchunkchar(struct soap*);
static const char *http_error(struct soap*, int);
static int http_get(struct soap*);
static int http_405(struct soap*);
static int http_200(struct soap*);
static int http_post(struct soap*, const char*, const char*, int, const char*, const char*, size_t);
static int http_send_header(struct soap*, const char*);
static int http_post_header(struct soap*, const char*, const char*);
static int http_response(struct soap*, int, size_t);
static int http_parse(struct soap*);
static int http_parse_header(struct soap*, const char*, const char*);
三 请求与应答的消息说明
下面是截获gSOAP的数据包
 1)gSOAP客户端的请求，请求的是add 参数是 1和2，即让服务端计算1+2=？
POST / HTTP/1.1
Host: localhost:8080
User-Agent: gSOAP/2.8
Content-Type: text/xml; charset=utf-8
Content-Length: 448
Connection: close
<?xml version="1.0" encoding="UTF-8"?>
<SOAP-ENV:Envelope xmlns:SOAP-ENV="http://schemas.xmlsoap.org/soap/envelope/" xmlns:SOAP-ENC="http://schemas.xmlsoap.org/soap/encoding/" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:ns="urn:calc"><SOAP-ENV:Body SOAP-ENV:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/"><ns:add><a>1</a><b>2</b></ns:add></SOAP-ENV:Body></SOAP-ENV:Envelope>
Request消息说明
POST / HTTP/1.1 表示 POST方法传输数据和 http协议的版本号
Host: localhost:8080表示 服务端的主机 名称和端口号
User-Agent: gSOAP/2.8 表示客户端的信息
请求内容和自动生成的calc.add.req.xml文件除了参数变了，其他内容一样
2)gSOAP服务端的应答
HTTP/1.1 200 OK
Server: gSOAP/2.8
Content-Type: text/xml; charset=utf-8
Content-Length: 466
Connection: close
<?xml version="1.0" encoding="UTF-8"?>
<SOAP-ENV:Envelope xmlns:SOAP-ENV="http://schemas.xmlsoap.org/soap/envelope/" xmlns:SOAP-ENC="http://schemas.xmlsoap.org/soap/encoding/" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:ns="urn:calc"><SOAP-ENV:Body SOAP-ENV:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/"><ns:addResponse><result>3</result></ns:addResponse></SOAP-ENV:Body></SOAP-ENV:Envelope>
Response消息说明
协议版本号： HTTP/1.1 
状态码：200 
状态消息：OK
Server: gSOAP/2.8 表示gSOAP服务端的软件信息
Content-Type: text/xml; charset=utf-8
gSOAP服务端响应的对象的类型和字符集
Connection: close 表示一个客户端的Request完成，客户端与服务端的TCP连接已关闭
应答内容和calc.add.res.xml文件中的内容除了结果值以外，其他内容一样
四 gSOAP的使用的网络模型
gSOAP使用了传统的select模型
tcp_select(struct soap *soap, SOAP_SOCKET s, int flags, int timeout)
{
......
}
select(
    __in int nfds,
    __inout_opt fd_set FAR * readfds,
    __inout_opt fd_set FAR * writefds,
    __inout_opt fd_set FAR * exceptfds,
    __in_opt const struct timeval FAR * timeout
);
第一个参数会被系统忽略的。
第二个参数是用来检查套接字上是否有数据可读
第三个参数用来检查数据是否可以发出。
最后一个是检查是否有带外数据可读取
Windows下最多支持的并发连接数为64，该连接数由宏FD_SETSIZE控制的，该值默认是64
gSOAP重新定义了该宏为1024
#ifndef FD_SETSIZE
# define FD_SETSIZE (1024)
#endif
select工作流程
1 用FD_ZERO宏来初始化感兴趣的fd_set，用FD_SET宏来将套接字句柄分配给相应的fd_set。
gSOAP中感兴趣的fd_set
一句FD_ZERO 接着一句FD_SET
 if (flags & SOAP_TCP_SELECT_RCV)
  { rfd = &fd[0];
    FD_ZERO(rfd);
    FD_SET(s, rfd);
  }
  if (flags & SOAP_TCP_SELECT_SND)
  { sfd = &fd[1];
    FD_ZERO(sfd);
    FD_SET(s, sfd);
  }
  if (flags & SOAP_TCP_SELECT_ERR)
  { efd = &fd[2];
    FD_ZERO(efd);
    FD_SET(s, efd);
  }
SOAP_TCP_SELECT_ERR这个是外带数据，用来表示错误
2调用Select函数，用FD_ISSET对套接字句柄进行检查
 r = select((int)s + 1, rfd, sfd, efd, &tv);
  if (r > 0)
  { r = 0;
    if ((flags & SOAP_TCP_SELECT_RCV) && FD_ISSET(s, rfd))
      r |= SOAP_TCP_SELECT_RCV;
    if ((flags & SOAP_TCP_SELECT_SND) && FD_ISSET(s, sfd))
      r |= SOAP_TCP_SELECT_SND;
    if ((flags & SOAP_TCP_SELECT_ERR) && FD_ISSET(s, efd))
      r |= SOAP_TCP_SELECT_ERR;
  }
3 剩下的是accept，recv，closesocket.
tcp_accept(struct soap *soap, SOAP_SOCKET s, struct sockaddr *a, int *n)
{
   ......
  fd = accept(s, a, (SOAP_SOCKLEN_T*)n);
}
 frecv(struct soap *soap, char *s, size_t n)
{
   ......
  recv(soap->socket, soap->tmpbuf, 1, MSG_PEEK) < 0))
