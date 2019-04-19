# gSOAP 源码分析(一) - 建建的博客 - CSDN博客
2017年02月17日 15:44:57[纪建](https://me.csdn.net/u013898698)阅读数：165
个人分类：[Onvif学习](https://blog.csdn.net/u013898698/article/category/6708886)

http://blog.csdn[.NET](http://lib.csdn.net/base/dotnet)/flyfish1986/article/details/7593224
gSOAP 源码分析(一)
邵盛松 2012-5-22
一 SOAP消息结构
SOAP消息包括以下元素
必需的 Envelope 元素，可把此 XML 文档标识为一条 SOAP 消息，XML文件的顶层元素，代表该文件为SOAP消息
可选的 Header 元素，包含头部信息
必需的 Body 元素，包含所有的调用和响应信息
可选的 Fault 元素，提供有关在处理此消息所发生错误的信息
可选的Attachment，主要用于传递附件，扩展的SOAP消息
Envelope是SOAP消息中的根节点，是SOAP消息中必需的部分；Header是SOAP消息中可选部分，如果SOAP消息中含有它，那么它一定要是soap:Envelope>中的第一个元素节点；Body是SOAP中必需部分，如果SOAP消息中没Header，那么Body必须是SOAP中第一个元素节点。
在默认生成的代码中关于Header 元素定义
/* SOAP Header: */
struct SOAP_ENV__Header
{
#ifdef WITH_NOEMPTYSTRUCT
private:
char dummy; /* dummy member to enable compilation */
#endif
};
下图为SOAP消息的结构
![](https://img-my.csdn.net/uploads/201205/23/1337733250_1595.png)

Fault结构
![](https://img-my.csdn.net/uploads/201205/23/1337733273_4856.png)
<faultcode>供识别故障的代码
<faultstring>这里的错误是为人设定的，让人读懂，而不是为程序处理设定的。
<faultactor>有关是谁引发故障的信息
<detail>Body元素中的内容不能被成功地处理的时候，它就出现了。
代码中fault结构体如下
struct SOAP_ENV__Fault
{
public:
char *faultcode; /* optional element of type xsd:QName */
char *faultstring; /* optional element of type xsd:string */
char *faultactor; /* optional element of type xsd:string */
struct SOAP_ENV__Detail *detail; /* optional element of type SOAP-ENV:Detail */
struct SOAP_ENV__Code *SOAP_ENV__Code; /* optional element of type SOAP-ENV:Code */
struct SOAP_ENV__Reason *SOAP_ENV__Reason; /* optional element of type SOAP-ENV:Reason */
char *SOAP_ENV__Node; /* optional element of type xsd:string */
char *SOAP_ENV__Role; /* optional element of type xsd:string */
struct SOAP_ENV__Detail *SOAP_ENV__Detail; /* optional element of type SOAP-ENV:Detail */
};
Fault是Body的一个子元素主要用于用于报告错误。
SOAP命名空间
SOAP_NMAC struct Namespace namespaces[] =
{
{"SOAP-ENV", "http://schemas.xmlsoap.org/soap/envelope/", "http://www.w3.org/*/soap-envelope", NULL},
{"SOAP-ENC", "http://schemas.xmlsoap.org/soap/encoding/", "http://www.w3.org/*/soap-encoding", NULL},
{"xsi", "http://www.w3.org/2001/XMLSchema-instance", "http://www.w3.org/*/XMLSchema-instance", NULL},
{"xsd", "http://www.w3.org/2001/XMLSchema", "http://www.w3.org/*/XMLSchema", NULL},
{"ns", "urn:calc", NULL, NULL},
{NULL, NULL, NULL, NULL}
};
这里的URL并不是指向文件，而只是一个名字。如果一个SOAP应用程序接收了一个消息，而该消息的SOAP Envelope元素使用和上述不同的名称空间，则该应用程序就将其视为版本错误并忽略该消息。
二 gSOAP Keep-Alive和超时管理
gSOAP是绑定Http协议来对xml数据进行传输，一个SOAP请求实际上就是一个HTTP POST请求。
消息从发送方到接受方方是单向传送，即以请求/应答的方式实现的。这也就是为什么生成的xml文件都是req，res成对出现的
gSOAP运行环境实例是一个struct soap类型的变量，struct SOAP_STD_API soap
。
 gSOAP支持HTTP Keep-Alive，生成代码中默认没有使用Keep-Alive
calcService_init(SOAP_IO_DEFAULT, SOAP_IO_DEFAULT);
如果参数更改SOAP_IO_KEEPALIVE将使用Keep-Alive，默认最大连接100
以下代码在soap结构体中
  short tcp_keep_alive; /* enable SO_KEEPALIVE */
  unsigned int tcp_keep_idle;  /* set TCP_KEEPIDLE */
  unsigned int tcp_keep_intvl;  /* set TCP_KEEPINTVL */
  unsigned int tcp_keep_cnt;  /* set TCP_KEEPCNT */
  unsigned int max_keep_alive;  /* maximum keep-alive session (default=100) */
TCP_KEEPIDLE  空闲多久开始发送keepalive包
TCP_KEEPCNT 总共发送多少个
TCP_KEEPINTVL 每两个keepalive包的发送时间间隔
![](https://img-my.csdn.net/uploads/201205/23/1337733316_8165.png)
这种非阻塞管理需要设置超时时间
接收超时时间
发送超时时间
连接超时时间
接受请求超时时间
代码如下，这也是soap结构体的一部分
int recv_timeout;/* when > 0, gives socket recv timeout in seconds, < 0 in usec */
int send_timeout;/* when > 0, gives socket send timeout in seconds, < 0 in usec */
int connect_timeout;/* when > 0, gives socket connect() timeout in seconds, < 0 in usec */
int accept_timeout;/* when > 0, gives socket accept() timeout in seconds, < 0 in usec */
正值以秒为单位。负值以微秒为单位。
三 提高速度技巧
1 通过更改在stdsoap2.h文件中SOAP_BUFLEN宏，增加缓存大小
2 如果客户端需要多次连接相同的服务器，那么客户端使用HTTP keep-alive。服务端也要支持HTTP keep-alive，这样能够最大的增强服务端和客户端的性能。
3 使用HTTP分块传输
4 不要使用gzip压缩
