# 刨根问底 HTTP 和 WebSocket 协议（上） - 文章 - 伯乐在线
原文出处： [TheAlchemist](http://www.jianshu.com/p/0e5b946880b4#)
![1966024-fa6c24ef78a381b5](http://jbcdn2.b0.upaiyun.com/2016/09/7dbe1ebc9647fc753bd9c14305e3965e.png)
*HTTP vs WebSocket*
那天和boss聊天，不经意间提到了Meteor，然后聊到了WebSocket，然后就有了以下对话，不得不说，看问题的方式不同，看到的东西也会大不相同。
A：Meteor是一个很新的开发框架，我觉得它设计得十分巧妙。
B：怎么个巧妙之处？
A：它的前后端全部使用JS，做到了真正的前后端统一；前端浏览器里存有一份后台开放出来的数据库的拷贝，快；使用WebSocket协议来做数据传输协议，来同步前后端的数据库，实现了真正的实时同步。
B：哦？WebSocket是什么东西？真实时？那底层是不是还是轮训？和HTTP的长连接有什么不同？
A：（开始心虚）它是一个新的基于TCP的应用层协议，只需要一次连接，以后的数据不需要重新建立连接，可以直接发送，它是基于TCP的，属于和HTTP相同的地位（呃，开始胡诌了），底层不是轮训，和长连接的区别……这个就不清楚了。
B：它的传输过程大致是什么样子的呢？
A：首先握手连接（又是胡诌），好像可以基于HTTP建立连接（之前用过Socket.io，即兴胡诌），建立了连接之后就可以传输数据了，还包括断掉之后重连等机制。
B：看起来和HTTP长连接做的事情差不多嘛，好像就是一种基于HTTP和Socket的协议啊。
A：呃……（我还是回去看看书吧）
有时候看事情确实太流于表面，了解到了每个事物的大致轮廓，但不求甚解，和朋友聊天说出来也鲜有人会刨根问底，导致了很多基础知识并不牢靠，于是回来大致把HTTP和WebSocket协议的RFC文档（RFC2616 和 RFC6455），刚好对HTTP的传输过程一直有点模糊，这里把两个协议的异同总结一下。
## 协议基础
仔细去看这两个协议，其实都非常简单，但任何一个事情想做到完美都会慢慢地变得异常复杂，各种细节。这里只会简单地描述两个协议的结构，并不会深入到很深的细节之处，对于理解http已经足够了。
## HTTP
HTTP的地址格式如下：


```
http_URL = "http:" "//" host [ ":" port ] [ abs_path [ "?" query ]]
协议和host不分大小写
```
## HTTP消息
一个HTTP消息可能是request或者response消息，两种类型的消息都是由开始行（start-line），零个或多个header域，一个表示header域结束的空行（也就是，一个以CRLF为前缀的空行），一个可能为空的消息主体（message-body）。一个合格的HTTP客户端不应该在消息头或者尾添加多余的CRLF，服务端也会忽略这些字符。
header的值不包括任何前导或后续的LWS（线性空白），线性空白可能会出现在域值（filed-value）的第一个非空白字符之前或最后一个非空白字符之后。前导或后续的LWS可能会被移除而不会改变域值的语意。任何出现在filed-content之间的LWS可能会被一个SP（空格）代替。header域的顺序不重要，但建议把常用的header放在前边（协议里这么说的）。
## Request 消息
RFC2616中这样定义HTTP Request 消息：


```
Request = Request-Line
          *(( general-header 
            | request-header（跟本次请求相关的一些header）
            | entity-header ) CRLF)（跟本次请求相关的一些header）
          CRLF
          [ message-body ]
```
一个HTTP的request消息以一个请求行开始，从第二行开始是header，接下来是一个空行，表示header结束，最后是消息体。
请求行的定义如下：


```
//请求行的定义
Request-Line = Method SP Request-URL SP HTTP-Version CRLF
//方法的定义
Method = "OPTIONS" | "GET" | "HEAD"  |"POST" |"PUT" |"DELETE" |"TRACE" |"CONNECT"  | extension-method
//资源地址的定义
Request-URI   ="*" | absoluteURI | abs_path | authotity（CONNECT）
```
Request消息中使用的header可以是general-header或者request-header，request-header（后边会讲解）。其中有一个比较特殊的就是Host，Host会与reuqest Uri一起来作为Request消息的接收者判断请求资源的条件，方法如下：
- 如果Request-URI是绝对地址（absoluteURI），这时请求里的主机存在于Request-URI里。任何出现在请求里Host头域值应当被忽略。
- 假如Request-URI不是绝对地址（absoluteURI），并且请求包括一个Host头域，则主机由该Host头域值决定。
- 假如由规则１或规则２定义的主机是一个无效的主机，则应当以一个400（错误请求）错误消息返回。
## Response 消息
响应消息跟请求消息几乎一模一样，定义如下：


```
Response      = Status-Line              
                   *(( general-header        
                    | response-header       
                    | entity-header ) CRLF)  
                   CRLF
                   [ message-body ]
```
可以看到，除了header不使用request-header之外，只有第一行不同，响应消息的第一行是状态行，其中就包含大名鼎鼎的返回码。
Status-Line的内容首先是协议的版本号，然后跟着返回码，最后是解释的内容，它们之间各有一个空格分隔，行的末尾以一个回车换行符作为结束。定义如下：


```
Status-Line = HTTP-Version SP Status-Code SP Reason-Phrase CRLF
```
## 返回码
返回码是一个3位数，第一位定义的返回码的类别，总共有5个类别，它们是：


```
- 1xx: Informational - Request received, continuing process
  - 2xx: Success - The action was successfully received,
    understood, and accepted
  - 3xx: Redirection - Further action must be taken in order to
    complete the request
  - 4xx: Client Error - The request contains bad syntax or cannot
    be fulfilled
  - 5xx: Server Error - The server failed to fulfill an apparently
    valid request
```
RFC2616中接着又给出了一系列返回码的扩展，这些都是我们平时会用到的，但是那些只是示例，HTTP1.1不强制通信各方遵守这些扩展的返回码，通信各方在返回码的实现上只需要遵守以上边定义的这5种类别的定义，意思就是，返回码的第一位要严格按照文档中所述的来，其他的随便定义。
任何人接收到一个不认识的返回码xyz，都可以把它当做x00来对待。对于不认识的返回码的响应消息，不可以缓存。
## Header
RFC2616中定义了4种header类型，在通信各方都认可的情况下，请求头可以被扩展的（可信的扩展只能等到协议的版本更新），如果接收者收到了一个不认识的请求头，这个头将会被当做实体头。4种头类型如下：
1.通用头（General Header Fields）：可用于request，也可用于response的头，但不可作为实体头，只能作为消息的头。


```
general-header = Cache-Control            ; Section 14.9
              | Connection               ; Section 14.10
              | Date                     ; Section 14.18
              | Pragma                   ; Section 14.32
              | Trailer                  ; Section 14.40
              | Transfer-Encoding        ; Section 14.41
              | Upgrade                  ; Section 14.42
              | Via                      ; Section 14.45
              | Warning                  ; Section 14.46
```
2.请求头（Request Header Fields）：被请求发起端用来改变请求行为的头。


```
request-header = Accept                   ; Section 14.1
               | Accept-Charset           ; Section 14.2
               | Accept-Encoding          ; Section 14.3
               | Accept-Language          ; Section 14.4
               | Authorization            ; Section 14.8
               | Expect                   ; Section 14.20
               | From                     ; Section 14.22
               | Host                     ; Section 14.23
               | If-Match                 ; Section 14.24
               | If-Modified-Since        ; Section 14.25
               | If-None-Match            ; Section 14.26
               | If-Range                 ; Section 14.27
               | If-Unmodified-Since      ; Section 14.28
               | Max-Forwards             ; Section 14.31
               | Proxy-Authorization      ; Section 14.34
               | Range                    ; Section 14.35
               | Referer                  ; Section 14.36
               | TE                       ; Section 14.39
               | User-Agent               ; Section 14.43
```
3.响应头（Response Header Fields）：被服务器用来对资源进行进一步的说明。


```
response-header = Accept-Ranges           ; Section 14.5
                | Age                     ; Section 14.6
                | ETag                    ; Section 14.19
                | Location                ; Section 14.30
                | Proxy-Authenticate      ; Section 14.33
                | Retry-After             ; Section 14.37
                | Server                  ; Section 14.38
                | Vary                    ; Section 14.44
                | WWW-Authenticate        ; Section 14.47
```
4.实体头（Entity Header Fields）：如果消息带有消息体，实体头用来作为元信息；如果没有消息体，就是为了描述请求的资源的信息。


```
entity-header  = Allow                    ; Section 14.7
               | Content-Encoding         ; Section 14.11
               | Content-Language         ; Section 14.12
               | Content-Length           ; Section 14.13
               | Content-Location         ; Section 14.14
               | Content-MD5              ; Section 14.15
               | Content-Range            ; Section 14.16
               | Content-Type             ; Section 14.17
               | Expires                  ; Section 14.21
               | Last-Modified            ; Section 14.29
               | extension-header
```
## 消息体（Message Body）和实体主体（Entity Body）
如果有Transfer-Encoding头，那么消息体解码完了就是实体主体，如果没有Transfer-Encoding头，消息体就是实体主体。


```
message-body = entity-body
                | <entity-body encoded as per Transfer-Encoding>
```
在request消息中，消息头中含有Content-Length或者Transfer-Encoding，标识会有一个消息体跟在后边。如果请求的方法不应该含有消息体（如OPTION），那么request消息一定不能含有消息体，即使客户端发送过去，服务器也不会读取消息体。
在response消息中，是否存在消息体由请求方法和返回码来共同决定。像1xx，204，304不会带有消息体。
## 消息体的长度
消息体长度的确定有一下几个规则，它们顺序执行：
- 所有不应该返回内容的Response消息都不应该带有任何的消息体，消息会在第一个空行就被认为是终止了。
- 如果消息头含有Transfer-Encoding，且它的值不是identity，那么消息体的长度会使用chunked方式解码来确定，直到连接终止。
- 如果消息头中有Content-Length，那么它就代表了entity-length和transfer-length。如果同时含有Transfer-Encoding，则entity-length和transfer-length可能不会相等，那么Content-Length会被忽略。
- 如果消息的媒体类型是multipart/byteranges，并且transfer-length也没有指定，那么传输长度由这个媒体自己定义。通常是收发双发定义好了格式， HTTP1.1客户端请求里如果出现Range头域并且带有多个字节范围（byte-range）指示符，这就意味着客户端能解析multipart/byteranges响应。
- 如果是Response消息，也可以由服务器来断开连接，作为消息体结束。
从消息体中得到实体主体，它的类型由两个header来定义，Content-Type和Content-Encoding（通常用来做压缩）。如果有实体主体，则必须有Content-Type,如果没有，接收方就需要猜测，猜不出来就是用application/octet-stream。
## HTTP 连接
HTTP1.1的连接默认使用持续连接（persistent connection），持续连接指的是，有时是客户端会需要在短时间内向服务端请求大量的相关的资源，如果不是持续连接，那么每个资源都要建立一个新的连接，HTTP底层使用的是TCP，那么每次都要使用三次握手建立TCP连接，将造成极大的资源浪费。
持续连接可以带来很多的好处：
- 使用更少的TCP连接，对通信各方的压力更小。
- 可以使用管道（pipeline）来传输信息，这样请求方不需要等待结果就可以发送下一条信息，对于单个的TCP的使用更充分。
- 流量更小
- 顺序请求的延时更小。
- 不需要重新建立TCP连接就可以传送error，关闭连接等信息。
HTTP1.1的服务器使用TCP的流量控制来控制HTTP的流量，HTTP1.1的客户端在收到服务器连接中发过来的error信息，就要马上关闭此链接。关于HTTP连接还有很多细节，之后再详述。
## WebSocket
只从RFC发布的时间看来，WebSocket要晚近很多，HTTP 1.1是1999年，WebSocket则是12年之后了。WebSocket协议的开篇就说，本协议的目的是为了解决基于浏览器的程序需要拉取资源时必须发起多个HTTP请求和长时间的轮训的问题……而创建的。
## 待续
本来是打算在一篇文章里把HTTP和WebSocket两个协议的大致细节理出来，然后进行对比。可是写着写着就发现篇幅可能会比较长，读起来就不那么友好了，那么刚好就再写第二篇吧。第二篇里会将WebSocket的大致情况描述一下，然后和HTTP适用的场景进行对比。
