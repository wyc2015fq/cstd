# 【FastCGI】FastCGI 协议分析 - Pangda NoSilly - CSDN博客

2018年08月10日 16:43:56[hepangda](https://me.csdn.net/hepangda)阅读数：359


# FastCGI 协议分析

FastCGI（快速通用网关接口）是一种让交互程序与Web服务器通信的协议，可以算是CGI的增强版本。FastCGI减少了网页服务器与CGI程序之间的开销，使得服务器可以同时处理更多的网页请求。目前很多常见的网页服务器都已经支持了该种协议。

## 通信格式

Web服务器与FastCGI程序之间通过流式套接字来通信，既包含Unix域套接字（Unix Domain 

 Socket），也可以使用最常见的TCP/IP协议族套接字。FastCGI程序提供的内容与普通的CGI程序相似。由于与CGI程序具有环境变量、标准输入和标准输出等众多获取信息的渠道不同，FastCGI程序仅仅通过Socket获取信息。

### 消息头

FastCGI规定，任意一个FastCGI数据包必须以一个8字节的消息头开始：

```
struct FCGI_Header {
  unsigned char version;
  unsigned char type;
  unsigned char requestIdB1;
  unsigned char requestIdB0;
  unsigned char contentLengthB1;
  unsigned char contentLengthB0;
  unsigned char paddingLength;
  unsigned char reserved;
};
```

在此消息头中，第一个字节（`version`）表示FastCGI协议的版本，目前一般定义为`1`。第二个字节（`type`）表示数据包的类型，这些类型有：

```
enum FCGI_Type {
  FCGI_BEGIN_REQUEST     = 1,  // (WEB->FastCGI) 表示一次请求的开始
  FCGI_ABORT_REQUEST     = 2,  // (WEB->FastCGI) 表示终止一次请求
  FCGI_END_REQUEST       = 3,  // (FastCGI->WEB) 请求已被处理完毕
  FCGI_PARAMS            = 4,  // (WEB->FastCGI) 表示一个向CGI程序传递的环境变量
  FCGI_STDIN             = 5,  // (WEB->FastCGI) 表示向CGI程序传递的标准输入
  FCGI_STDOUT            = 6,  // (FastCGI->WEB) 表示CGI程序的标准输出   
  FCGI_STDERR            = 7,  // (FastCGI->WEB) 表示CGI程序的标准错误输出
  FCGI_DATA              = 8,  // (WEB->FastCGI) 向CGI程序传递的额外数据
  FCGI_GET_VALUES        = 9,  // (WEB->FastCGI) 向FastCGI程序询问一些环境变量
  FCGI_GET_VALUES_RESULT = 10, // (FastCGI->WEB) 询问环境变量的结果
  FCGI_UNKNOWN_TYPE      = 11  // 未知类型，可能用作拓展
};
```

之后第三、四个字节（`requestIdB1 requestIdB0`）合起来表示本次请求的编号，其中`requestIdB1`是请求编号的高八位，`requestIdB0`是请求编号的低八位。这个字段的存在允许Web服务器在一次连接中向FastCGI服务器发送多个不同的请求，只要使用不同的请求编号即可。然后第五、六个字节（`contentLengthB1 contentLengthB0`）合起来表示消息头后仍有多少字节的数据（数据长度），`contentLengthB1`表示其高八位，`contentLengthB0`表示其低八位。数据长度的表示范围在0~65535之间，因而若数据超过65535字节，则必须将之分为多个数据包来传输。第七个字节则表示“填充长度”，为了使字节对齐，提高网络传输效率，可能需要在数据包后附加无用的数据来保证对齐，这些无用数据的长度就存储在`paddingLength`中，当然，填充长度不可超过255字节。第八个字节为保留字段，目前的FastCGI还不使用这一字段。

### 消息体

在发送完毕消息头之后，应当附加一个消息体。根据消息类型（`type`字段）的不同，发送的格式也略有区别。

#### FCGI_BEGIN_REQUEST

该类型的消息体格式是固定的一个八字节结构体，因此该类型的消息头中表示数据长度的字段值也应当是固定的。格式被定义为：

```
struct FCGI_BeginRequestBody {
  unsigned char roleB1;
  unsigned char roleB0;
  unsigned char flags;
  unsigned char reserved[5];
};
```

第一、二字节合起来表示在本次请求中，Web服务器希望CGI程序担当的角色，其中`roleB1`表示其高八位，`roleB0`则表示其低八位。目前FastCGI协议中规定了三种角色，分别为：

```
enum FCGI_Role {
  FCGI_RESPONDER  = 1,  // 响应器
  FCGI_AUTHORIZER = 2,  // 认证器
  FCGI_FILTER     = 3   // 过滤器
};
```

一般而言，Web服务器基本需要FastCGI做响应器（`FCGI_RESPONDER`）角色。

第三个字节表示Web服务器希望FastCGI所做的一些行为，但目前只有控制FastCGI服务器处理完请求之后是否关闭Socket连接的功能，之后可能会做拓展。

```
enum FCGI_Flags {
  FCGI_KEEP_CONN = 1;
};
```

当`flags`与`FLAGS_KEEP_CONN`按位与结果为`1`时，则表示FastCGI服务器处理完本次请求之后不关闭连接，否则则关闭该Socket连接。

消息体中的其余字节目前尚无作用，留作拓展。

#### FCGI_END_REQUEST

该类型的消息体格式与也是固定的一个八字节结构体，因此该类型的消息头中表示数据长度的字段值也应当是固定的。格式被定义为：

```
struct FCGI_EndRequestBody {
  unsigned char appStatusB3;
  unsigned char appStatusB2;
  unsigned char appStatusB1;
  unsigned char appStatusB0;
  unsigned char protocolStatus;
  unsigned char reserved[3];
};
```

前四字节合起来表示CGI程序的退出状态，与之前相同，此处是一个网络字节序，需要手动转换。第五字节表示FastCGI协议的状态码，可能的取值有：

```
enum FCGI_ProtocolStatus {
  FCGI_REQUEST_COMPLETE = 0,  // 请求正常完成
  FCGI_CANT_MPX_CONN    = 1,  // FastCGI服务器不支持并发处理，请求已被拒绝
  FCGI_OVERLOADED       = 2,  // FastCGI服务器耗尽了资源或达到限制，请求已被拒绝
  FCGI_UNKNOWN_ROLE     = 3   // FastCGI不支持指定的role，请求已被拒绝
};
```

后三字节目前尚无作用。

#### FCGI_PARAMS

该类型的消息体格式为：

```
struct FCGI_ParamsBody {
  unsigned char nameLengthB3;
  unsigned char nameLengthB2;
  unsigned char nameLengthB1;
  unsigned char nameLengthB0;
  unsigned char valueLengthB3;
  unsigned char valueLengthB2;
  unsigned char valueLengthB1;
  unsigned char valueLengthB0;
  unsigned char nameData[NAME_LENGTH];    // NAME_LENGTH与前四字节所表示数字相同
  unsigned char valueData[VALUE_LENGTH];  // VALUE_LENGTH与第五到八字节所表示的数字相同
};
```

消息体的前四字节表示传递给FastCGI的环境参数名的长度，计算方法为`(B3 & 0x7f) << 24) + (B2 << 16) + (B1 << 8) + B0`，第五到八字节则表示环境值的长度，计算方法同上。在这八字节之后，先跟环境参数名，再跟相应的环境值。

#### 其他消息体

其他消息体后直接跟具体内容，没有规定的格式。若`type`为`FCGI_ABORT`，那么不必跟消息体，在消息头中将`contentLength`设为0即可。另，`type`为`FCGI_DATA`、`FCGI_GET_VALUES`和 `FCGI_GET_VALUES_RESULT` 的情况本文不做介绍，可参考[FastCGI Specification](http://www.mit.edu/~yandros/doc/specs/fcgi-spec.html)。

## 通信模式

下面我们模拟一次客户端发起的POST请求，简单说明Web服务器和FastCGI服务之间的通信过程：

```
POST /fastcgi.page HTTP/1.1
Host: a.host.name
Connection: keep-alive
Content-Type: application/x-www-form-urlencoded
Content-Length: 11

a=b&c=d&e=f
```
- Web服务器收到、解析请求，和FastCGI服务器建立Socket连接；
- Web->FastCGI：发送`FCGI_BEGIN_REQUEST`消息；
- Web->FastCGI：发送`FCGI_PARAMS`消息，设置`REQUEST_METHOD`变量为`POST`；
- Web->FastCGI：发送`FCGI_PARAMS`消息，设置`SCRIPT_NAME`变量为`/wwwroot/fastcgi.page`（此处根据CGI程序的不同和请求的不同，填写内容应当不同）；
- Web->FastCGI：发送`FCGI_PARAMS`消息，设置`CONTENT_LENGTH`变量为`11`；
- Web->FastCGI：发送`FCGI_PARAMS`消息，设置`CONTENT_TYPE`变量为`application/x-www-form-urlencoded`（3-6步可以以任意顺序执行）；
- Web->FastCGI：发送`FCGI_PARAMS`消息，消息头的`contentLength`字段应当为0，且不包含消息体；
- Web->FastCGI：发送`FCGI_STDIN`消息，消息头的`contentLength`字段应当为`11`（若本来的`Content-Length`比65535字节更大，应当分次发送，分次发送时应当注意次序），内容即为`a=b&c=d&e=f`；
- Web->FastCGI：发送`FCGI_STDIN`消息，消息头的`contentLength`字段应当设为0，且不包含消息体；
- FastCGI->Web：（假设本次请求不产生错误），发送`FCGI_STDOUT`消息，若CGI程序给出的输出大于65535字节，将分次发送。
- FastCGI->Web：发送`FCGI_STDOUT`消息，本次消息的`contentLength`字段为`0`，且不具有消息体。
- FastCGI->Web：（假设此时请求已完成），按规定的格式发送`FCGI_END_REQEUST`消息。

总结一下，一次请求由一个`FCGI_BEGIN_REQUEST`消息开始，由一个`FCGI_END_REQUEST`消息结束。途中发送的消息，类似于`FCGI_STDIN`、`FCGI_STDERR`之类的消息，可以根据需要分次发送，在结束发送该类消息之后，发送一个同类型的`contentLength`字段为0的消息，表示该类型的消息已经结束。

需要注意的是，若不发送`FCGI_PARAMS`消息设置`CONTENT_LENGTH`参数，那么之后发送的`FCGI_STDIN`消息无效。若发送的`FCGI_STDIN`消息的`contentLength`字段之和与`CONTENT_LENGTH`的值不同，那么该请求也将失败。

## CGI的常见参数

在我们发送`FCGI_PARAMS`消息中，我们常设置以下参数：
|参数名|意义|
|----|----|
|REQUEST_METHOD|信息传输方式（GET/POST/PUT等）|
|QUERY_STRING|查询字符串|
|CONTENT_LENGTH|向CGI标准输入传递的信息长度（应当等于FCGI_STDIN消息contentLength字段之和)|
|CONTENT_TYPE|向CGI标准输入传递的信息类型|
|PATH_INFO|路径信息|
|PATH_TRANSLATED|CGI程序的完成路径名|
|SCRIPT_NAME|要执行的CGI程序的名字|
|GATEWAY_INTERFACE|服务器所实现的CGI版本|
|SERVER_NAME|服务器的IP或名字|
|SERVER_PORT|主机的端口号|
|SERVER_SOFTWARE|调用CGI程序的HTTP服务器的名称和版本号|
|REMOTE_ADDR|客户机的主机名|
|REMOTE_HOST|客户机的IP地址|
|ACCEPT|HTTP请求中Accept字段|
|ACCEPT_ENCODING|HTTP请求中Accept-Encoding字段|
|ACCEPT_LANGUAGE|HTTP请求中Accept-Language字段|
|AUTORIZATION|HTTP请求中Authorization字段|
|IF_MODIFIED_SINCE|HTTP请求中If-Modified-Since字段|
|PRAGMA|HTTP请求中Pragma字段|
|REFERER|HTTP请求中Referer字段|
|USER_AGENT|HTTP请求中User-Agent字段|

## CGI的“标准输出”格式

CGI的标准输出的基本格式为：

```
NAME1: VALUE1\r\n
NAME2: VALUE2\r\n
...
NAMEN: VALUEN\r\n
\r\n
(Content)
```

可以看到，CGI的标准输出类似于一个HTTP Response，除过没有HTTP Response第一行的内容外，其余均十分相似。CGI的标准输出中有一个特别的部分，名为：`Status`，值为相应的HTTP状态码。CGI程序通过这些内容向Web服务器传递头部信息和期望的状态码。例如下面这个标准输出：

```
Status: 404
Content-Type: text/html

<html>
  <body>
    <p>404 Not Found</p>
  </body>
</html>
```

则表示希望Web服务器将之状态码设为404，将`Content-Type`字段设为`text/html`。相应内容则为由`\r\n\r\n`标识的空行后的内容。

