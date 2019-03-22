# HTTP笔记

# HTTP请求方法对照表

根据HTTP标准，HTTP请求可以使用多种请求方法。 
HTTP1.0定义了三种请求方法： GET, POST 和 HEAD方法。 
HTTP1.1新增了五种请求方法：OPTIONS, PUT, DELETE, TRACE 和 CONNECT 方法。



### HTTP Request Method共计15种

| 序号 | 方法             | 描述                                                         |
| :--- | :--------------- | :----------------------------------------------------------- |
| 1    | GET              | 请求指定的页面信息，并返回实体主体。                         |
| 2    | HEAD             | 类似于get请求，只不过返回的响应中没有具体的内容，用于获取报头 |
| 3    | POST             | 向指定资源提交数据进行处理请求（例如提交表单或者上传文件）。数据被包含在请求体中。POST请求可能会导致新的资源的建立和/或已有资源的修改。 |
| 4    | PUT              | 从客户端向服务器传送的数据取代指定的文档的内容。             |
| 5    | DELETE           | 请求服务器删除指定的页面。                                   |
| 6    | CONNECT          | HTTP/1.1协议中预留给能够将连接改为管道方式的代理服务器。     |
| 7    | OPTIONS          | 允许客户端查看服务器的性能。                                 |
| 8    | TRACE            | 回显服务器收到的请求，主要用于测试或诊断。                   |
| 9    | PATCH            | 实体中包含一个表，表中说明与该URI所表示的原内容的区别。      |
| 10   | MOVE             | 请求服务器将指定的页面移至另一个网络地址。                   |
| 11   | COPY             | 请求服务器将指定的页面拷贝至另一个网络地址。                 |
| 12   | LINK             | 请求服务器建立链接关系。                                     |
| 13   | UNLINK           | 断开链接关系。                                               |
| 14   | WRAPPED          | 允许客户端发送经过封装的请求。                               |
| 15   | Extension-mothed | 在不改动协议的前提下，可增加另外的方法。                     |



HTTP请求头提供了关于请求，响应或者其他的发送实体的信息。HTTP的头信息包括通用头、请求头、响应头和实体头四个部分。每个头域由一个域名，冒号（:）和域值三部分组成。

- **通用头标**：即可用于请求，也可用于响应，是作为一个整体而不是特定资源与事务相关联。
- **请求头标**：允许客户端传递关于自身的信息和希望的响应形式。
- **响应头标**：服务器和于传递自身信息的响应。
- **实体头标**：定义被传送资源的信息。即可用于请求，也可用于响应。

根据以上分类的HTTP请求头介绍可以

参考此文

，本工具根据请求和输出分为Request和Response两部分。







### HTTP Request Header 请求头

| Header              | 解释                                                         | 示例                                                    |
| :------------------ | :----------------------------------------------------------- | :------------------------------------------------------ |
| Accept              | 指定客户端能够接收的内容类型                                 | Accept: text/plain, text/html                           |
| Accept-Charset      | 浏览器可以接受的字符编码集。                                 | Accept-Charset: iso-8859-5                              |
| Accept-Encoding     | 指定浏览器可以支持的web服务器返回内容压缩编码类型。          | Accept-Encoding: compress, gzip                         |
| Accept-Language     | 浏览器可接受的语言                                           | Accept-Language: en,zh                                  |
| Accept-Ranges       | 可以请求网页实体的一个或者多个子范围字段                     | Accept-Ranges: bytes                                    |
| Authorization       | HTTP授权的授权证书                                           | Authorization: Basic QWxhZGRpbjpvcGVuIHNlc2FtZQ==       |
| Cache-Control       | 指定请求和响应遵循的缓存机制                                 | Cache-Control: no-cache                                 |
| Connection          | 表示是否需要持久连接。（HTTP 1.1默认进行持久连接）           | Connection: close                                       |
| Cookie              | HTTP请求发送时，会把保存在该请求域名下的所有cookie值一起发送给web服务器。 | Cookie: $Version=1; Skin=new;                           |
| Content-Length      | 请求的内容长度                                               | Content-Length: 348                                     |
| Content-Type        | 请求的与实体对应的MIME信息                                   | Content-Type: application/x-www-form-urlencoded         |
| Date                | 请求发送的日期和时间                                         | Date: Tue, 15 Nov 2010 08:12:31 GMT                     |
| Expect              | 请求的特定的服务器行为                                       | Expect: 100-continue                                    |
| From                | 发出请求的用户的Email                                        | From: user@email.com                                    |
| Host                | 指定请求的服务器的域名和端口号                               | Host: www.zcmhi.com                                     |
| If-Match            | 只有请求内容与实体相匹配才有效                               | If-Match: “737060cd8c284d8af7ad3082f209582d”            |
| If-Modified-Since   | 如果请求的部分在指定时间之后被修改则请求成功，未被修改则返回304代码 | If-Modified-Since: Sat, 29 Oct 2010 19:43:31 GMT        |
| If-None-Match       | 如果内容未改变返回304代码，参数为服务器先前发送的Etag，与服务器回应的Etag比较判断是否改变 | If-None-Match: “737060cd8c284d8af7ad3082f209582d”       |
| If-Range            | 如果实体未改变，服务器发送客户端丢失的部分，否则发送整个实体。参数也为Etag | If-Range: “737060cd8c284d8af7ad3082f209582d”            |
| If-Unmodified-Since | 只在实体在指定时间之后未被修改才请求成功                     | If-Unmodified-Since: Sat, 29 Oct 2010 19:43:31 GMT      |
| Max-Forwards        | 限制信息通过代理和网关传送的时间                             | Max-Forwards: 10                                        |
| Pragma              | 用来包含实现特定的指令                                       | Pragma: no-cache                                        |
| Proxy-Authorization | 连接到代理的授权证书                                         | Proxy-Authorization: Basic QWxhZGRpbjpvcGVuIHNlc2FtZQ== |
| Range               | 只请求实体的一部分，指定范围                                 | Range: bytes=500-999                                    |
| Referer             | 先前网页的地址，当前请求网页紧随其后,即来路                  | Referer: http://www.zcmhi.com/archives/71.html          |
| TE                  | 客户端愿意接受的传输编码，并通知服务器接受接受尾加头信息     | TE: trailers,deflate;q=0.5                              |
| Upgrade             | 向服务器指定某种传输协议以便服务器进行转换（如果支持）       | Upgrade: HTTP/2.0, SHTTP/1.3, IRC/6.9, RTA/x11          |
| User-Agent          | User-Agent的内容包含发出请求的用户信息                       | User-Agent: Mozilla/5.0 (Linux; X11)                    |
| Via                 | 通知中间网关或代理服务器地址，通信协议                       | Via: 1.0 fred, 1.1 nowhere.com (Apache/1.1)             |
| Warning             | 关于消息实体的警告信息                                       | Warn: 199 Miscellaneous warning                         |

### HTTP Responses Header 响应头

| Header             | 解释                                                         | 示例                                                  |
| :----------------- | :----------------------------------------------------------- | :---------------------------------------------------- |
| Accept-Ranges      | 表明服务器是否支持指定范围请求及哪种类型的分段请求           | Accept-Ranges: bytes                                  |
| Age                | 从原始服务器到代理缓存形成的估算时间（以秒计，非负）         | Age: 12                                               |
| Allow              | 对某网络资源的有效的请求行为，不允许则返回405                | Allow: GET, HEAD                                      |
| Cache-Control      | 告诉所有的缓存机制是否可以缓存及哪种类型                     | Cache-Control: no-cache                               |
| Content-Encoding   | web服务器支持的返回内容压缩编码类型。                        | Content-Encoding: gzip                                |
| Content-Language   | 响应体的语言                                                 | Content-Language: en,zh                               |
| Content-Length     | 响应体的长度                                                 | Content-Length: 348                                   |
| Content-Location   | 请求资源可替代的备用的另一地址                               | Content-Location: /index.htm                          |
| Content-MD5        | 返回资源的MD5校验值                                          | Content-MD5: Q2hlY2sgSW50ZWdyaXR5IQ==                 |
| Content-Range      | 在整个返回体中本部分的字节位置                               | Content-Range: bytes 21010-47021/47022                |
| Content-Type       | 返回内容的MIME类型                                           | Content-Type: text/html; charset=utf-8                |
| Date               | 原始服务器消息发出的时间                                     | Date: Tue, 15 Nov 2010 08:12:31 GMT                   |
| ETag               | 请求变量的实体标签的当前值                                   | ETag: “737060cd8c284d8af7ad3082f209582d”              |
| Expires            | 响应过期的日期和时间                                         | Expires: Thu, 01 Dec 2010 16:00:00 GMT                |
| Last-Modified      | 请求资源的最后修改时间                                       | Last-Modified: Tue, 15 Nov 2010 12:45:26 GMT          |
| Location           | 用来重定向接收方到非请求URL的位置来完成请求或标识新的资源    | Location: http://www.zcmhi.com/archives/94.html       |
| Pragma             | 包括实现特定的指令，它可应用到响应链上的任何接收方           | Pragma: no-cache                                      |
| Proxy-Authenticate | 它指出认证方案和可应用到代理的该URL上的参数                  | Proxy-Authenticate: Basic                             |
| refresh            | 应用于重定向或一个新的资源被创造，在5秒之后重定向（由网景提出，被大部分浏览器支持） | Refresh: 5; url=http://www.zcmhi.com/archives/94.html |
| Retry-After        | 如果实体暂时不可取，通知客户端在指定时间之后再次尝试         | Retry-After: 120                                      |
| Server             | web服务器软件名称                                            | Server: Apache/1.3.27 (Unix) (Red-Hat/Linux)          |
| Set-Cookie         | 设置Http Cookie                                              | Set-Cookie: UserID=JohnDoe; Max-Age=3600; Version=1   |
| Trailer            | 指出头域在分块传输编码的尾部存在                             | Trailer: Max-Forwards                                 |
| Transfer-Encoding  | 文件传输编码                                                 | Transfer-Encoding:chunked                             |
| Vary               | 告诉下游代理是使用缓存响应还是从原始服务器请求               | Vary: *                                               |
| Via                | 告知代理客户端响应是通过哪里发送的                           | Via: 1.0 fred, 1.1 nowhere.com (Apache/1.1)           |
| Warning            | 警告实体可能存在的问题                                       | Warning: 199 Miscellaneous warning                    |
| WWW-Authenticate   | 表明客户端请求实体应该使用的授权方案                         | WWW-Authenticate: Basic                               |



### HTTP response codes 状态码

当浏览者访问一个网页时，浏览者的浏览器会向网页所在服务器发出请求。当浏览器接收并显示网页前，此网页所在的服务器会返回一个包含HTTP状态码的信息头（server header）用以响应浏览器的请求。

HTTP状态码的英文为HTTP Status Code。 下面是常见的HTTP状态码：

- 200 - 请求成功
- 301 - 资源（网页等）被永久转移到其它URL
- 404 - 请求的资源（网页等）不存在
- 500 - 内部服务器错误





### HTTP状态码的分类

HTTP状态码由三个十进制数字组成，第一个十进制数字定义了状态码的类型，后两个数字没有分类的作用。HTTP状态码共分为5种类型：

| 分类 | 分类描述                                       |
| :--- | :--------------------------------------------- |
| 1**  | 信息，服务器收到请求，需要请求者继续执行操作   |
| 2**  | 成功，操作被成功接收并处理                     |
| 3**  | 重定向，需要进一步的操作以完成请求             |
| 4**  | 客户端错误，请求包含语法错误或无法完成请求     |
| 5**  | 服务器错误，服务器在处理请求的过程中发生了错误 |

### HTTP状态码表（版本1） 此表含状态码英文名称

| 状态码        | 状态码英文名称                  | 中文描述                                                     |
| :------------ | :------------------------------ | :----------------------------------------------------------- |
| 1开头的状态码 |                                 |                                                              |
| 100           | Continue                        | 继续。客户端应继续其请求                                     |
| 101           | Switching Protocols             | 切换协议。服务器根据客户端的请求切换协议。只能切换到更高级的协议，例如，切换到HTTP的新版本协议 |
| 2开头的状态码 |                                 |                                                              |
| 200           | OK                              | 请求成功。一般用于GET与POST请求                              |
| 201           | Created                         | 已创建。成功请求并创建了新的资源                             |
| 202           | Accepted                        | 已接受。已经接受请求，但未处理完成                           |
| 203           | Non-Authoritative Information   | 非授权信息。请求成功。但返回的meta信息不在原始的服务器，而是一个副本 |
| 204           | No Content                      | 无内容。服务器成功处理，但未返回内容。在未更新网页的情况下，可确保浏览器继续显示当前文档 |
| 205           | Reset Content                   | 重置内容。服务器处理成功，用户终端（例如：浏览器）应重置文档视图。可通过此返回码清除浏览器的表单域 |
| 206           | Partial Content                 | 部分内容。服务器成功处理了部分GET请求                        |
| 3开头的状态码 |                                 |                                                              |
| 300           | Multiple Choices                | 多种选择。请求的资源可包括多个位置，相应可返回一个资源特征与地址的列表用于用户终端（例如：浏览器）选择 |
| 301           | Moved Permanently               | 永久移动。请求的资源已被永久的移动到新URI，返回信息会包括新的URI，浏览器会自动定向到新URI。今后任何新的请求都应使用新的URI代替 |
| 302           | Found                           | 临时移动。与301类似。但资源只是临时被移动。客户端应继续使用原有URI |
| 303           | See Other                       | 查看其它地址。与301类似。使用GET和POST请求查看               |
| 304           | Not Modified                    | 未修改。所请求的资源未修改，服务器返回此状态码时，不会返回任何资源。客户端通常会缓存访问过的资源，通过提供一个头信息指出客户端希望只返回在指定日期之后修改的资源 |
| 305           | Use Proxy                       | 使用代理。所请求的资源必须通过代理访问                       |
| 306           | Unused                          | 已经被废弃的HTTP状态码                                       |
| 307           | Temporary Redirect              | 临时重定向。与302类似。使用GET请求重定向                     |
| 4开头的状态码 |                                 |                                                              |
| 400           | Bad Request                     | 客户端请求的语法错误，服务器无法理解                         |
| 401           | Unauthorized                    | 请求要求用户的身份认证                                       |
| 402           | Payment Required                | 保留，将来使用                                               |
| 403           | Forbidden                       | 服务器理解请求客户端的请求，但是拒绝执行此请求               |
| 404           | Not Found                       | 服务器无法根据客户端的请求找到资源（网页）。通过此代码，网站设计人员可设置"您所请求的资源无法找到"的个性页面 |
| 405           | Method Not Allowed              | 客户端请求中的方法被禁止                                     |
| 406           | Not Acceptable                  | 服务器无法根据客户端请求的内容特性完成请求                   |
| 407           | Proxy Authentication Required   | 请求要求代理的身份认证，与401类似，但请求者应当使用代理进行授权 |
| 408           | Request Time-out                | 服务器等待客户端发送的请求时间过长，超时                     |
| 409           | Conflict                        | 服务器完成客户端的PUT请求是可能返回此代码，服务器处理请求时发生了冲突 |
| 410           | Gone                            | 客户端请求的资源已经不存在。410不同于404，如果资源以前有现在被永久删除了可使用410代码，网站设计人员可通过301代码指定资源的新位置 |
| 411           | Length Required                 | 服务器无法处理客户端发送的不带Content-Length的请求信息       |
| 412           | Precondition Failed             | 客户端请求信息的先决条件错误                                 |
| 413           | Request Entity Too Large        | 由于请求的实体过大，服务器无法处理，因此拒绝请求。为防止客户端的连续请求，服务器可能会关闭连接。如果只是服务器暂时无法处理，则会包含一个Retry-After的响应信息 |
| 414           | Request-URI Too Large           | 请求的URI过长（URI通常为网址），服务器无法处理               |
| 415           | Unsupported Media Type          | 服务器无法处理请求附带的媒体格式                             |
| 416           | Requested range not satisfiable | 客户端请求的范围无效                                         |
| 417           | Expectation Failed              | 服务器无法满足Expect的请求头信息                             |
| 5开头的状态码 |                                 |                                                              |
| 500           | Internal Server Error           | 服务器内部错误，无法完成请求                                 |
| 501           | Not Implemented                 | 服务器不支持请求的功能，无法完成请求                         |
| 502           | Bad Gateway                     | 充当网关或代理的服务器，从远端服务器接收到了一个无效的请求   |
| 503           | Service Unavailable             | 由于超载或系统维护，服务器暂时的无法处理客户端的请求。延时的长度可包含在服务器的Retry-After头信息中 |
| 504           | Gateway Time-out                | 充当网关或代理的服务器，未及时从远端服务器获取请求           |
| 505           | HTTP Version not supported      | 服务器不支持请求的HTTP协议的版本，无法完成处理               |

### HTTP状态码列表（版本2） 此表的描述更详细些

| 状态码 | 含义                                                         |
| :----- | :----------------------------------------------------------- |
| 100    | 客户端应当继续发送请求。这个临时响应是用来通知客户端它的部分请求已经被服务器接收，且仍未被拒绝。客户端应当继续发送请求的剩余部分，或者如果请求已经完成，忽略这个响应。服务器必须在请求完成后向客户端发送一个最终响应。 |
| 101    | 服务器已经理解了客户端的请求，并将通过Upgrade 消息头通知客户端采用不同的协议来完成这个请求。在发送完这个响应最后的空行后，服务器将会切换到在Upgrade 消息头中定义的那些协议。 　　只有在切换新的协议更有好处的时候才应该采取类似措施。例如，切换到新的HTTP 版本比旧版本更有优势，或者切换到一个实时且同步的协议以传送利用此类特性的资源。 |
| 102    | 由WebDAV（RFC 2518）扩展的状态码，代表处理将被继续执行。     |
| 200    | 请求已成功，请求所希望的响应头或数据体将随此响应返回。       |
| 201    | 请求已经被实现，而且有一个新的资源已经依据请求的需要而建立，且其 URI 已经随Location 头信息返回。假如需要的资源无法及时建立的话，应当返回 '202 Accepted'。 |
| 202    | 服务器已接受请求，但尚未处理。正如它可能被拒绝一样，最终该请求可能会也可能不会被执行。在异步操作的场合下，没有比发送这个状态码更方便的做法了。 　　返回202状态码的响应的目的是允许服务器接受其他过程的请求（例如某个每天只执行一次的基于批处理的操作），而不必让客户端一直保持与服务器的连接直到批处理操作全部完成。在接受请求处理并返回202状态码的响应应当在返回的实体中包含一些指示处理当前状态的信息，以及指向处理状态监视器或状态预测的指针，以便用户能够估计操作是否已经完成。 |
| 203    | 服务器已成功处理了请求，但返回的实体头部元信息不是在原始服务器上有效的确定集合，而是来自本地或者第三方的拷贝。当前的信息可能是原始版本的子集或者超集。例如，包含资源的元数据可能导致原始服务器知道元信息的超级。使用此状态码不是必须的，而且只有在响应不使用此状态码便会返回200 OK的情况下才是合适的。 |
| 204    | 服务器成功处理了请求，但不需要返回任何实体内容，并且希望返回更新了的元信息。响应可能通过实体头部的形式，返回新的或更新后的元信息。如果存在这些头部信息，则应当与所请求的变量相呼应。 　　如果客户端是浏览器的话，那么用户浏览器应保留发送了该请求的页面，而不产生任何文档视图上的变化，即使按照规范新的或更新后的元信息应当被应用到用户浏览器活动视图中的文档。 　　由于204响应被禁止包含任何消息体，因此它始终以消息头后的第一个空行结尾。 |
| 205    | 服务器成功处理了请求，且没有返回任何内容。但是与204响应不同，返回此状态码的响应要求请求者重置文档视图。该响应主要是被用于接受用户输入后，立即重置表单，以便用户能够轻松地开始另一次输入。 　　与204响应一样，该响应也被禁止包含任何消息体，且以消息头后的第一个空行结束。 |
| 206    | 服务器已经成功处理了部分 GET 请求。类似于 FlashGet 或者迅雷这类的 HTTP 下载工具都是使用此类响应实现断点续传或者将一个大文档分解为多个下载段同时下载。 　　该请求必须包含 Range 头信息来指示客户端希望得到的内容范围，并且可能包含 If-Range 来作为请求条件。 　　响应必须包含如下的头部域： 　　Content-Range 用以指示本次响应中返回的内容的范围；如果是 Content-Type 为 multipart/byteranges 的多段下载，则每一 multipart 段中都应包含 Content-Range 域用以指示本段的内容范围。假如响应中包含 Content-Length，那么它的数值必须匹配它返回的内容范围的真实字节数。 　　Date 　　ETag 和/或 Content-Location，假如同样的请求本应该返回200响应。 　　Expires, Cache-Control，和/或 Vary，假如其值可能与之前相同变量的其他响应对应的值不同的话。 　　假如本响应请求使用了 If-Range 强缓存验证，那么本次响应不应该包含其他实体头；假如本响应的请求使用了 If-Range 弱缓存验证，那么本次响应禁止包含其他实体头；这避免了缓存的实体内容和更新了的实体头信息之间的不一致。否则，本响应就应当包含所有本应该返回200响应中应当返回的所有实体头部域。 　　假如 ETag 或 Last-Modified 头部不能精确匹配的话，则客户端缓存应禁止将206响应返回的内容与之前任何缓存过的内容组合在一起。 　　任何不支持 Range 以及 Content-Range 头的缓存都禁止缓存206响应返回的内容。 |
| 207    | 由WebDAV(RFC 2518)扩展的状态码，代表之后的消息体将是一个XML消息，并且可能依照之前子请求数量的不同，包含一系列独立的响应代码。 |
| 300    | 被请求的资源有一系列可供选择的回馈信息，每个都有自己特定的地址和浏览器驱动的商议信息。用户或浏览器能够自行选择一个首选的地址进行重定向。 　　除非这是一个 HEAD 请求，否则该响应应当包括一个资源特性及地址的列表的实体，以便用户或浏览器从中选择最合适的重定向地址。这个实体的格式由 Content-Type 定义的格式所决定。浏览器可能根据响应的格式以及浏览器自身能力，自动作出最合适的选择。当然，RFC 2616规范并没有规定这样的自动选择该如何进行。 　　如果服务器本身已经有了首选的回馈选择，那么在 Location 中应当指明这个回馈的 URI；浏览器可能会将这个 Location 值作为自动重定向的地址。此外，除非额外指定，否则这个响应也是可缓存的。 |
| 301    | 被请求的资源已永久移动到新位置，并且将来任何对此资源的引用都应该使用本响应返回的若干个 URI 之一。如果可能，拥有链接编辑功能的客户端应当自动把请求的地址修改为从服务器反馈回来的地址。除非额外指定，否则这个响应也是可缓存的。 　　新的永久性的 URI 应当在响应的 Location 域中返回。除非这是一个 HEAD 请求，否则响应的实体中应当包含指向新的 URI 的超链接及简短说明。 　　如果这不是一个 GET 或者 HEAD 请求，因此浏览器禁止自动进行重定向，除非得到用户的确认，因为请求的条件可能因此发生变化。 　　注意：对于某些使用 HTTP/1.0 协议的浏览器，当它们发送的 POST 请求得到了一个301响应的话，接下来的重定向请求将会变成 GET 方式。 |
| 302    | 请求的资源现在临时从不同的 URI 响应请求。由于这样的重定向是临时的，客户端应当继续向原有地址发送以后的请求。只有在Cache-Control或Expires中进行了指定的情况下，这个响应才是可缓存的。 　　新的临时性的 URI 应当在响应的 Location 域中返回。除非这是一个 HEAD 请求，否则响应的实体中应当包含指向新的 URI 的超链接及简短说明。 　　如果这不是一个 GET 或者 HEAD 请求，那么浏览器禁止自动进行重定向，除非得到用户的确认，因为请求的条件可能因此发生变化。 　　注意：虽然RFC 1945和RFC 2068规范不允许客户端在重定向时改变请求的方法，但是很多现存的浏览器将302响应视作为303响应，并且使用 GET 方式访问在 Location 中规定的 URI，而无视原先请求的方法。状态码303和307被添加了进来，用以明确服务器期待客户端进行何种反应。 |
| 303    | 对应当前请求的响应可以在另一个 URI 上被找到，而且客户端应当采用 GET 的方式访问那个资源。这个方法的存在主要是为了允许由脚本激活的POST请求输出重定向到一个新的资源。这个新的 URI 不是原始资源的替代引用。同时，303响应禁止被缓存。当然，第二个请求（重定向）可能被缓存。 　　新的 URI 应当在响应的 Location 域中返回。除非这是一个 HEAD 请求，否则响应的实体中应当包含指向新的 URI 的超链接及简短说明。 　　注意：许多 HTTP/1.1 版以前的 浏览器不能正确理解303状态。如果需要考虑与这些浏览器之间的互动，302状态码应该可以胜任，因为大多数的浏览器处理302响应时的方式恰恰就是上述规范要求客户端处理303响应时应当做的。 |
| 304    | 如果客户端发送了一个带条件的 GET 请求且该请求已被允许，而文档的内容（自上次访问以来或者根据请求的条件）并没有改变，则服务器应当返回这个状态码。304响应禁止包含消息体，因此始终以消息头后的第一个空行结尾。 　　该响应必须包含以下的头信息： 　　Date，除非这个服务器没有时钟。假如没有时钟的服务器也遵守这些规则，那么代理服务器以及客户端可以自行将 Date 字段添加到接收到的响应头中去（正如RFC 2068中规定的一样），缓存机制将会正常工作。 　　ETag 和/或 Content-Location，假如同样的请求本应返回200响应。 　　Expires, Cache-Control，和/或Vary，假如其值可能与之前相同变量的其他响应对应的值不同的话。 　　假如本响应请求使用了强缓存验证，那么本次响应不应该包含其他实体头；否则（例如，某个带条件的 GET 请求使用了弱缓存验证），本次响应禁止包含其他实体头；这避免了缓存了的实体内容和更新了的实体头信息之间的不一致。 　　假如某个304响应指明了当前某个实体没有缓存，那么缓存系统必须忽视这个响应，并且重复发送不包含限制条件的请求。 　　假如接收到一个要求更新某个缓存条目的304响应，那么缓存系统必须更新整个条目以反映所有在响应中被更新的字段的值。 |
| 305    | 被请求的资源必须通过指定的代理才能被访问。Location 域中将给出指定的代理所在的 URI 信息，接收者需要重复发送一个单独的请求，通过这个代理才能访问相应资源。只有原始服务器才能建立305响应。 　　注意：RFC 2068中没有明确305响应是为了重定向一个单独的请求，而且只能被原始服务器建立。忽视这些限制可能导致严重的安全后果。 |
| 306    | 在最新版的规范中，306状态码已经不再被使用。                  |
| 307    | 请求的资源现在临时从不同的URI 响应请求。由于这样的重定向是临时的，客户端应当继续向原有地址发送以后的请求。只有在Cache-Control或Expires中进行了指定的情况下，这个响应才是可缓存的。 　　新的临时性的URI 应当在响应的 Location 域中返回。除非这是一个HEAD 请求，否则响应的实体中应当包含指向新的URI 的超链接及简短说明。因为部分浏览器不能识别307响应，因此需要添加上述必要信息以便用户能够理解并向新的 URI 发出访问请求。 　　如果这不是一个GET 或者 HEAD 请求，那么浏览器禁止自动进行重定向，除非得到用户的确认，因为请求的条件可能因此发生变化。 |
| 400    | 1、语义有误，当前请求无法被服务器理解。除非进行修改，否则客户端不应该重复提交这个请求。 　　2、请求参数有误。 |
| 401    | 当前请求需要用户验证。该响应必须包含一个适用于被请求资源的 WWW-Authenticate 信息头用以询问用户信息。客户端可以重复提交一个包含恰当的 Authorization 头信息的请求。如果当前请求已经包含了 Authorization 证书，那么401响应代表着服务器验证已经拒绝了那些证书。如果401响应包含了与前一个响应相同的身份验证询问，且浏览器已经至少尝试了一次验证，那么浏览器应当向用户展示响应中包含的实体信息，因为这个实体信息中可能包含了相关诊断信息。参见RFC 2617。 |
| 402    | 该状态码是为了将来可能的需求而预留的。                       |
| 403    | 服务器已经理解请求，但是拒绝执行它。与401响应不同的是，身份验证并不能提供任何帮助，而且这个请求也不应该被重复提交。如果这不是一个 HEAD 请求，而且服务器希望能够讲清楚为何请求不能被执行，那么就应该在实体内描述拒绝的原因。当然服务器也可以返回一个404响应，假如它不希望让客户端获得任何信息。 |
| 404    | 请求失败，请求所希望得到的资源未被在服务器上发现。没有信息能够告诉用户这个状况到底是暂时的还是永久的。假如服务器知道情况的话，应当使用410状态码来告知旧资源因为某些内部的配置机制问题，已经永久的不可用，而且没有任何可以跳转的地址。404这个状态码被广泛应用于当服务器不想揭示到底为何请求被拒绝或者没有其他适合的响应可用的情况下。 |
| 405    | 请求行中指定的请求方法不能被用于请求相应的资源。该响应必须返回一个Allow 头信息用以表示出当前资源能够接受的请求方法的列表。 　　鉴于 PUT，DELETE 方法会对服务器上的资源进行写操作，因而绝大部分的网页服务器都不支持或者在默认配置下不允许上述请求方法，对于此类请求均会返回405错误。 |
| 406    | 请求的资源的内容特性无法满足请求头中的条件，因而无法生成响应实体。 　　除非这是一个 HEAD 请求，否则该响应就应当返回一个包含可以让用户或者浏览器从中选择最合适的实体特性以及地址列表的实体。实体的格式由 Content-Type 头中定义的媒体类型决定。浏览器可以根据格式及自身能力自行作出最佳选择。但是，规范中并没有定义任何作出此类自动选择的标准。 |
| 407    | 与401响应类似，只不过客户端必须在代理服务器上进行身份验证。代理服务器必须返回一个 Proxy-Authenticate 用以进行身份询问。客户端可以返回一个 Proxy-Authorization 信息头用以验证。参见RFC 2617。 |
| 408    | 请求超时。客户端没有在服务器预备等待的时间内完成一个请求的发送。客户端可以随时再次提交这一请求而无需进行任何更改。 |
| 409    | 由于和被请求的资源的当前状态之间存在冲突，请求无法完成。这个代码只允许用在这样的情况下才能被使用：用户被认为能够解决冲突，并且会重新提交新的请求。该响应应当包含足够的信息以便用户发现冲突的源头。 　　冲突通常发生于对 PUT 请求的处理中。例如，在采用版本检查的环境下，某次 PUT 提交的对特定资源的修改请求所附带的版本信息与之前的某个（第三方）请求向冲突，那么此时服务器就应该返回一个409错误，告知用户请求无法完成。此时，响应实体中很可能会包含两个冲突版本之间的差异比较，以便用户重新提交归并以后的新版本。 |
| 410    | 被请求的资源在服务器上已经不再可用，而且没有任何已知的转发地址。这样的状况应当被认为是永久性的。如果可能，拥有链接编辑功能的客户端应当在获得用户许可后删除所有指向这个地址的引用。如果服务器不知道或者无法确定这个状况是否是永久的，那么就应该使用404状态码。除非额外说明，否则这个响应是可缓存的。 　　410响应的目的主要是帮助网站管理员维护网站，通知用户该资源已经不再可用，并且服务器拥有者希望所有指向这个资源的远端连接也被删除。这类事件在限时、增值服务中很普遍。同样，410响应也被用于通知客户端在当前服务器站点上，原本属于某个个人的资源已经不再可用。当然，是否需要把所有永久不可用的资源标记为'410 Gone'，以及是否需要保持此标记多长时间，完全取决于服务器拥有者。 |
| 411    | 服务器拒绝在没有定义 Content-Length 头的情况下接受请求。在添加了表明请求消息体长度的有效 Content-Length 头之后，客户端可以再次提交该请求。 |
| 412    | 服务器在验证在请求的头字段中给出先决条件时，没能满足其中的一个或多个。这个状态码允许客户端在获取资源时在请求的元信息（请求头字段数据）中设置先决条件，以此避免该请求方法被应用到其希望的内容以外的资源上。 |
| 413    | 服务器拒绝处理当前请求，因为该请求提交的实体数据大小超过了服务器愿意或者能够处理的范围。此种情况下，服务器可以关闭连接以免客户端继续发送此请求。 　　如果这个状况是临时的，服务器应当返回一个 Retry-After 的响应头，以告知客户端可以在多少时间以后重新尝试。 |
| 414    | 请求的URI 长度超过了服务器能够解释的长度，因此服务器拒绝对该请求提供服务。这比较少见，通常的情况包括： 　　本应使用POST方法的表单提交变成了GET方法，导致查询字符串（Query String）过长。 　　重定向URI “黑洞”，例如每次重定向把旧的 URI 作为新的 URI 的一部分，导致在若干次重定向后 URI 超长。 　　客户端正在尝试利用某些服务器中存在的安全漏洞攻击服务器。这类服务器使用固定长度的缓冲读取或操作请求的 URI，当 GET 后的参数超过某个数值后，可能会产生缓冲区溢出，导致任意代码被执行[1]。没有此类漏洞的服务器，应当返回414状态码。 |
| 415    | 对于当前请求的方法和所请求的资源，请求中提交的实体并不是服务器中所支持的格式，因此请求被拒绝。 |
| 416    | 如果请求中包含了 Range 请求头，并且 Range 中指定的任何数据范围都与当前资源的可用范围不重合，同时请求中又没有定义 If-Range 请求头，那么服务器就应当返回416状态码。 　　假如 Range 使用的是字节范围，那么这种情况就是指请求指定的所有数据范围的首字节位置都超过了当前资源的长度。服务器也应当在返回416状态码的同时，包含一个 Content-Range 实体头，用以指明当前资源的长度。这个响应也被禁止使用 multipart/byteranges 作为其 Content-Type。 |
| 417    | 在请求头 Expect 中指定的预期内容无法被服务器满足，或者这个服务器是一个代理服务器，它有明显的证据证明在当前路由的下一个节点上，Expect 的内容无法被满足。 |
| 421    | 从当前客户端所在的IP地址到服务器的连接数超过了服务器许可的最大范围。通常，这里的IP地址指的是从服务器上看到的客户端地址（比如用户的网关或者代理服务器地址）。在这种情况下，连接数的计算可能涉及到不止一个终端用户。 |
| 422    | 从当前客户端所在的IP地址到服务器的连接数超过了服务器许可的最大范围。通常，这里的IP地址指的是从服务器上看到的客户端地址（比如用户的网关或者代理服务器地址）。在这种情况下，连接数的计算可能涉及到不止一个终端用户。 |
| 422    | 请求格式正确，但是由于含有语义错误，无法响应。（RFC 4918 WebDAV）423 Locked 　　当前资源被锁定。（RFC 4918 WebDAV） |
| 424    | 由于之前的某个请求发生的错误，导致当前请求失败，例如 PROPPATCH。（RFC 4918 WebDAV） |
| 425    | 在WebDav Advanced Collections 草案中定义，但是未出现在《WebDAV 顺序集协议》（RFC 3658）中。 |
| 426    | 客户端应当切换到TLS/1.0。（RFC 2817）                        |
| 449    | 由微软扩展，代表请求应当在执行完适当的操作后进行重试。       |
| 500    | 服务器遇到了一个未曾预料的状况，导致了它无法完成对请求的处理。一般来说，这个问题都会在服务器的程序码出错时出现。 |
| 501    | 服务器不支持当前请求所需要的某个功能。当服务器无法识别请求的方法，并且无法支持其对任何资源的请求。 |
| 502    | 作为网关或者代理工作的服务器尝试执行请求时，从上游服务器接收到无效的响应。 |
| 503    | 由于临时的服务器维护或者过载，服务器当前无法处理请求。这个状况是临时的，并且将在一段时间以后恢复。如果能够预计延迟时间，那么响应中可以包含一个 Retry-After 头用以标明这个延迟时间。如果没有给出这个 Retry-After 信息，那么客户端应当以处理500响应的方式处理它。 　　注意：503状态码的存在并不意味着服务器在过载的时候必须使用它。某些服务器只不过是希望拒绝客户端的连接。 |
| 504    | 作为网关或者代理工作的服务器尝试执行请求时，未能及时从上游服务器（URI标识出的服务器，例如HTTP、FTP、LDAP）或者辅助服务器（例如DNS）收到响应。 　　注意：某些代理服务器在DNS查询超时时会返回400或者500错误 |
| 505    | 服务器不支持，或者拒绝支持在请求中使用的 HTTP 版本。这暗示着服务器不能或不愿使用与客户端相同的版本。响应中应当包含一个描述了为何版本不被支持以及服务器支持哪些协议的实体。 |
| 506    | 由《透明内容协商协议》（RFC 2295）扩展，代表服务器存在内部配置错误：被请求的协商变元资源被配置为在透明内容协商中使用自己，因此在一个协商处理中不是一个合适的重点。 |
| 507    | 服务器无法存储完成请求所必须的内容。这个状况被认为是临时的。WebDAV (RFC 4918) |
| 509    | 服务器达到带宽限制。这不是一个官方的状态码，但是仍被广泛使用。 |
| 510    | 获取资源所需要的策略并没有没满足。（RFC 2774）               |







### HTTP Content-type 对照表

Content-Type，内容类型，一般是指网页中存在的Content-Type，用于定义网络文件的类型和网页的编码，决定浏览器将以什么形式、什么编码读取这个文件，比如用PHP输出图片文件、JSON数据、XML文件等非HTML内容时，就必须用header函数来指定Content-Type，才能达到输出一张图片或是其它指定内容类型的需求。





### Application Type

| 文件扩展名 | Content-Type(Mime-Type)                 | 描述                                                         |
| :--------- | :-------------------------------------- | :----------------------------------------------------------- |
| .          | application/x-                          |                                                              |
| .*         | application/octet-stream                | 二进制流，不知道下载文件类型                                 |
| .pdf       | application/pdf                         | PDF（Portable Document Format的简称，意为“便携式文件格式”）  |
| .ai        | application/postscript                  | PostScript（PS）是主要用于电子产业和桌面出版领域的一种页面描述语言和编程语言。 |
| .xml       | application/atom+xml                    | Atom feeds                                                   |
| .js        | application/ecmascript                  | 相当于application/javascript但是严格的处理规则               |
| .edi       | application/EDI-X12                     | EDI ANSI ASC X12数据                                         |
| .edi       | application/EDIFACT                     | EDI EDIFACT数据                                              |
| .json      | application/json                        | JSON（JavaScript Object Notation）                           |
| .js        | application/javascript                  | ECMAScript/JavaScript（相当于application/ecmascript但是宽松的处理规则） |
| .ogg       | application/ogg                         | Ogg, 视频文件格式                                            |
| .rdf       | application/rdf+xml                     | 资源描述框架（Resource Description Framework，缩写 RDF），是万维网联盟（W3C）提出的一组标记语言的技术标准，以便更为丰富地描述和表达网络资源的内容与结构。 |
| .xml       | application/rss+xml                     | RSS（Really Simple Syndication,简易信息聚合）是一种消息来源格式规范，用以聚合经常发布更新数据的网站，例如博客文章、新闻、音频或视频的网摘 |
| .xml       | application/soap+xml                    | 简单对象访问协议（SOAP，全写为Simple Object Access Protocol）是交换数据的一种协议规范，使用在计算机网络Web服务（web service）中，交换带结构信息。 |
| .woff      | application/font-woff                   | Web开放字体格式（Web Open Font Format，简称WOFF）是一种网页所采用的字体格式标准。（推荐使用；使用application/x-font-woff直到它变为官方标准） |
| .xhtml     | application/xhtml+xml                   | 可扩展超文本标记语言（eXtensible HyperText Markup Language，XHTML），是一种标记语言，表现方式与超文本标记语言（HTML）类似，不过语法上更加严格。 |
| .xml       | application/xml                         | 可扩展标记语言（英语：eXtensible Markup Language，简称: XML），是一种标记语言。 |
| .dtd       | application/xml-dtd                     | XML文件的文件型别定义（Document Type Definition）可以看成一个或者多个XML文件的模板，在这里可以定义XML文件中的元素、元素的属性、元素的排列方式、元素包含的内容等等。 |
| .xml       | application/xop+xml                     | 二进制优化封装协议(Xmlbinary Optimized Packaging)            |
| .zip       | application/zip                         | ZIP压缩文件                                                  |
| .gzip      | application/gzip                        | Gzip是若干种文件压缩程序的简称，通常指GNU计划的实现，此处的gzip代表GNU zip。 |
| .xls       | application/x-xls                       | XLS 就是 Microsoft Excel 工作表，是一种非常常用的电子表格格式。 |
| .001       | application/x-001                       |                                                              |
| .301       | application/x-301                       |                                                              |
| .906       | application/x-906                       |                                                              |
| .a11       | application/x-a11                       |                                                              |
| .awf       | application/vnd.adobe.workflow          |                                                              |
| .bmp       | application/x-bmp                       |                                                              |
| .c4t       | application/x-c4t                       |                                                              |
| .cal       | application/x-cals                      |                                                              |
| .cdf       | application/x-netcdf                    |                                                              |
| .cel       | application/x-cel                       |                                                              |
| .cg4       | application/x-g4                        |                                                              |
| .cit       | application/x-cit                       |                                                              |
| .bot       | application/x-bot                       |                                                              |
| .c90       | application/x-c90                       |                                                              |
| .cat       | application/vnd.ms-pki.seccat           |                                                              |
| .cdr       | application/x-cdr                       |                                                              |
| .cer       | application/x-x509-ca-cert              |                                                              |
| .cgm       | application/x-cgm                       |                                                              |
| .cmx       | application/x-cmx                       |                                                              |
| .crl       | application/pkix-crl                    |                                                              |
| .csi       | application/x-csi                       |                                                              |
| .cut       | application/x-cut                       |                                                              |
| .dbm       | application/x-dbm                       |                                                              |
| .cmp       | application/x-cmp                       |                                                              |
| .cot       | application/x-cot                       |                                                              |
| .crt       | application/x-x509-ca-cert              |                                                              |
| .dbf       | application/x-dbf                       |                                                              |
| .dbx       | application/x-dbx                       |                                                              |
| .dcx       | application/x-dcx                       |                                                              |
| .dgn       | application/x-dgn                       |                                                              |
| .dll       | application/x-msdownload                |                                                              |
| .dot       | application/msword                      |                                                              |
| .der       | application/x-x509-ca-cert              |                                                              |
| .dib       | application/x-dib                       |                                                              |
| .doc       | application/msword                      |                                                              |
| .drw       | application/x-drw                       |                                                              |
| .dwf       | application/x-dwf                       |                                                              |
| .dxb       | application/x-dxb                       |                                                              |
| .edn       | application/vnd.adobe.edn               |                                                              |
| .dwg       | application/x-dwg                       |                                                              |
| .dxf       | application/x-dxf                       |                                                              |
| .emf       | application/x-emf                       |                                                              |
| .epi       | application/x-epi                       |                                                              |
| .eps       | application/postscript                  |                                                              |
| .exe       | application/x-msdownload                |                                                              |
| .fdf       | application/vnd.fdf                     |                                                              |
| .eps       | application/x-ps                        |                                                              |
| .etd       | application/x-ebx                       |                                                              |
| .fif       | application/fractals                    |                                                              |
| .frm       | application/x-frm                       |                                                              |
| .gbr       | application/x-gbr                       |                                                              |
| .g4        | application/x-g4                        |                                                              |
| .gl2       | application/x-gl2                       |                                                              |
| .hgl       | application/x-hgl                       |                                                              |
| .hpg       | application/x-hpgl                      |                                                              |
| .hqx       | application/mac-binhex40                |                                                              |
| .hta       | application/hta                         |                                                              |
| .gp4       | application/x-gp4                       |                                                              |
| .hmr       | application/x-hmr                       |                                                              |
| .hpl       | application/x-hpl                       |                                                              |
| .hrf       | application/x-hrf                       |                                                              |
| .icb       | application/x-icb                       |                                                              |
| .ico       | application/x-ico                       |                                                              |
| .ig4       | application/x-g4                        |                                                              |
| .iii       | application/x-iphone                    |                                                              |
| .ins       | application/x-internet-signup           |                                                              |
| .iff       | application/x-iff                       |                                                              |
| .igs       | application/x-igs                       |                                                              |
| .img       | application/x-img                       |                                                              |
| .isp       | application/x-internet-signup           |                                                              |
| .jpe       | application/x-jpe                       |                                                              |
| .js        | application/x-javascript                |                                                              |
| .jpg       | application/x-jpg                       |                                                              |
| .lar       | application/x-laplayer-reg              |                                                              |
| .latex     | application/x-latex                     |                                                              |
| .lbm       | application/x-lbm                       |                                                              |
| .ls        | application/x-javascript                |                                                              |
| .ltr       | application/x-ltr                       |                                                              |
| .man       | application/x-troff-man                 |                                                              |
| .mdb       | application/msaccess                    |                                                              |
| .mac       | application/x-mac                       |                                                              |
| .mdb       | application/x-mdb                       |                                                              |
| .mfp       | application/x-shockwave-flash           |                                                              |
| .mi        | application/x-mi                        |                                                              |
| .mil       | application/x-mil                       |                                                              |
| .mocha     | application/x-javascript                |                                                              |
| .mpd       | application/vnd.ms-project              |                                                              |
| .mpp       | application/vnd.ms-project              |                                                              |
| .mpt       | application/vnd.ms-project              |                                                              |
| .mpw       | application/vnd.ms-project              |                                                              |
| .mpx       | application/vnd.ms-project              |                                                              |
| .mxp       | application/x-mmxp                      |                                                              |
| .nrf       | application/x-nrf                       |                                                              |
| .out       | application/x-out                       |                                                              |
| .p12       | application/x-pkcs12                    |                                                              |
| .p7c       | application/pkcs7-mime                  |                                                              |
| .p7r       | application/x-pkcs7-certreqresp         |                                                              |
| .pc5       | application/x-pc5                       |                                                              |
| .pcl       | application/x-pcl                       |                                                              |
| .pdx       | application/vnd.adobe.pdx               |                                                              |
| .pgl       | application/x-pgl                       |                                                              |
| .pko       | application/vnd.ms-pki.pko              |                                                              |
| .p10       | application/pkcs10                      |                                                              |
| .p7b       | application/x-pkcs7-certificates        |                                                              |
| .p7m       | application/pkcs7-mime                  |                                                              |
| .p7s       | application/pkcs7-signature             |                                                              |
| .pci       | application/x-pci                       |                                                              |
| .pcx       | application/x-pcx                       |                                                              |
| .pdf       | application/pdf                         |                                                              |
| .pfx       | application/x-pkcs12                    |                                                              |
| .pic       | application/x-pic                       |                                                              |
| .pl        | application/x-perl                      |                                                              |
| .plt       | application/x-plt                       |                                                              |
| .png       | application/x-png                       |                                                              |
| .ppa       | application/vnd.ms-powerpoint           |                                                              |
| .pps       | application/vnd.ms-powerpoint           |                                                              |
| .ppt       | application/x-ppt                       |                                                              |
| .prf       | application/pics-rules                  |                                                              |
| .prt       | application/x-prt                       |                                                              |
| .ps        | application/postscript                  |                                                              |
| .pwz       | application/vnd.ms-powerpoint           |                                                              |
| .ra        | audio/vnd.rn-realaudio                  | RealAudio是一种由RealNetworks发展的RealMedia多媒体音频文件格式，仅指RealPlayer中能够识别的音频文件，也可以理解为real格式的音频文件。 |
| .ras       | application/x-ras                       |                                                              |
| .pot       | application/vnd.ms-powerpoint           |                                                              |
| .ppm       | application/x-ppm                       |                                                              |
| .ppt       | application/vnd.ms-powerpoint           |                                                              |
| .pr        | application/x-pr                        |                                                              |
| .prn       | application/x-prn                       |                                                              |
| .ps        | application/x-ps                        |                                                              |
| .ptn       | application/x-ptn                       |                                                              |
| .red       | application/x-red                       |                                                              |
| .rjs       | application/vnd.rn-realsystem-rjs       |                                                              |
| .rlc       | application/x-rlc                       |                                                              |
| .rm        | application/vnd.rn-realmedia            |                                                              |
| .rat       | application/rat-file                    |                                                              |
| .rec       | application/vnd.rn-recording            |                                                              |
| .rgb       | application/x-rgb                       |                                                              |
| .rjt       | application/vnd.rn-realsystem-rjt       |                                                              |
| .rle       | application/x-rle                       |                                                              |
| .rmf       | application/vnd.adobe.rmf               |                                                              |
| .rmj       | application/vnd.rn-realsystem-rmj       |                                                              |
| .rmp       | application/vnd.rn-rn_music_package     |                                                              |
| .rmvb      | application/vnd.rn-realmedia-vbr        |                                                              |
| .rnx       | application/vnd.rn-realplayer           |                                                              |
| .rpm       | audio/x-pn-realaudio-plugin             |                                                              |
| .rms       | application/vnd.rn-realmedia-secure     |                                                              |
| .rmx       | application/vnd.rn-realsystem-rmx       |                                                              |
| .rsml      | application/vnd.rn-rsml                 |                                                              |
| .rtf       | application/msword                      |                                                              |
| .rv        | video/vnd.rn-realvideo                  |                                                              |
| .sat       | application/x-sat                       |                                                              |
| .sdw       | application/x-sdw                       |                                                              |
| .slb       | application/x-slb                       |                                                              |
| .rtf       | application/x-rtf                       |                                                              |
| .sam       | application/x-sam                       |                                                              |
| .sdp       | application/sdp                         |                                                              |
| .sit       | application/x-stuffit                   |                                                              |
| .sld       | application/x-sld                       |                                                              |
| .smi       | application/smil                        |                                                              |
| .smk       | application/x-smk                       |                                                              |
| .smil      | application/smil                        |                                                              |
| .spc       | application/x-pkcs7-certificates        |                                                              |
| .spl       | application/futuresplash                |                                                              |
| .ssm       | application/streamingmedia              |                                                              |
| .stl       | application/vnd.ms-pki.stl              |                                                              |
| .sst       | application/vnd.ms-pki.certstore        |                                                              |
| .tdf       | application/x-tdf                       |                                                              |
| .tga       | application/x-tga                       |                                                              |
| .sty       | application/x-sty                       |                                                              |
| .swf       | application/x-shockwave-flash           |                                                              |
| .tg4       | application/x-tg4                       |                                                              |
| .tif       | application/x-tif                       |                                                              |
| .vdx       | application/vnd.visio                   |                                                              |
| .vpg       | application/x-vpeg005                   |                                                              |
| .vsd       | application/x-vsd                       |                                                              |
| .vst       | application/vnd.visio                   |                                                              |
| .vsw       | application/vnd.visio                   |                                                              |
| .vtx       | application/vnd.visio                   |                                                              |
| .torrent   | application/x-bittorrent                |                                                              |
| .vda       | application/x-vda                       |                                                              |
| .vsd       | application/vnd.visio                   |                                                              |
| .vss       | application/vnd.visio                   |                                                              |
| .vst       | application/x-vst                       |                                                              |
| .vsx       | application/vnd.visio                   |                                                              |
| .wb1       | application/x-wb1                       |                                                              |
| .wb3       | application/x-wb3                       |                                                              |
| .wiz       | application/msword                      |                                                              |
| .wk4       | application/x-wk4                       |                                                              |
| .wks       | application/x-wks                       |                                                              |
| .wb2       | application/x-wb2                       |                                                              |
| .wk3       | application/x-wk3                       |                                                              |
| .wkq       | application/x-wkq                       |                                                              |
| .wmf       | application/x-wmf                       |                                                              |
| .wmd       | application/x-ms-wmd                    |                                                              |
| .wp6       | application/x-wp6                       |                                                              |
| .wpg       | application/x-wpg                       |                                                              |
| .wq1       | application/x-wq1                       |                                                              |
| .wri       | application/x-wri                       |                                                              |
| .ws        | application/x-ws                        |                                                              |
| .wmz       | application/x-ms-wmz                    |                                                              |
| .wpd       | application/x-wpd                       |                                                              |
| .wpl       | application/vnd.ms-wpl                  |                                                              |
| .wr1       | application/x-wr1                       |                                                              |
| .wrk       | application/x-wrk                       |                                                              |
| .ws2       | application/x-ws                        |                                                              |
| .xdp       | application/vnd.adobe.xdp               |                                                              |
| .xfd       | application/vnd.adobe.xfd               |                                                              |
| .xfdf      | application/vnd.adobe.xfdf              |                                                              |
| .xls       | application/vnd.ms-excel                |                                                              |
| .xwd       | application/x-xwd                       |                                                              |
| .sis       | application/vnd.symbian.install         |                                                              |
| .x_t       | application/x-x_t                       |                                                              |
| .apk       | application/vnd.android.package-archive |                                                              |
| .x_b       | application/x-x_b                       |                                                              |
| .sisx      | application/vnd.symbian.install         |                                                              |
| .ipa       | application/vnd.iphone                  |                                                              |
| .xap       | application/x-silverlight-app           |                                                              |
| .xlw       | application/x-xlw                       |                                                              |
| .xpl       | audio/scpls                             |                                                              |
| .anv       | application/x-anv                       |                                                              |
| .uin       | application/x-icq                       |                                                              |

### Text Type

| 文件扩展名 | Content-Type(Mime-Type) | 描述                                                         |
| :--------- | :---------------------- | :----------------------------------------------------------- |
| .323       | text/h323               |                                                              |
| .biz       | text/xml                |                                                              |
| .cml       | text/xml                |                                                              |
| .asa       | text/asa                |                                                              |
| .asp       | text/asp                |                                                              |
| .css       | text/css                | 层叠样式表（英语：Cascading Style Sheets，简写CSS），又称串样式列表、层次结构式样式表文件，一种用来为结构化文档（如HTML文档或XML应用）添加样式（字体、间距和颜色等）的计算机语言，由W3C定义和维护。 |
| .csv       | text/csv                | 逗号分隔值（Comma-Separated Values，CSV，有时也称为字符分隔值，因为分隔字符也可以不是逗号），其文件以纯文本形式存储表格数据（数字和文本）。 |
| .dcd       | text/xml                |                                                              |
| .dtd       | text/xml                |                                                              |
| .ent       | text/xml                |                                                              |
| .fo        | text/xml                |                                                              |
| .htc       | text/x-component        |                                                              |
| .html      | text/html               |                                                              |
| .htx       | text/html               |                                                              |
| .htm       | text/html               |                                                              |
| .htt       | text/webviewhtml        |                                                              |
| .jsp       | text/html               |                                                              |
| .math      | text/xml                |                                                              |
| .mml       | text/xml                |                                                              |
| .mtx       | text/xml                |                                                              |
| .plg       | text/html               |                                                              |
| .rdf       | text/xml                |                                                              |
| .rt        | text/vnd.rn-realtext    |                                                              |
| .sol       | text/plain              |                                                              |
| .spp       | text/xml                |                                                              |
| .stm       | text/html               |                                                              |
| .svg       | text/xml                |                                                              |
| .tld       | text/xml                |                                                              |
| .txt       | text/plain              | 纯文字内容                                                   |
| .uls       | text/iuls               |                                                              |
| .vml       | text/xml                |                                                              |
| .tsd       | text/xml                |                                                              |
| .vcf       | text/x-vcard            |                                                              |
| .vxml      | text/xml                |                                                              |
| .wml       | text/vnd.wap.wml        |                                                              |
| .wsdl      | text/xml                |                                                              |
| .wsc       | text/scriptlet          |                                                              |
| .xdr       | text/xml                |                                                              |
| .xql       | text/xml                |                                                              |
| .xsd       | text/xml                |                                                              |
| .xslt      | text/xml                |                                                              |
| .xml       | text/xml                |                                                              |
| .xq        | text/xml                |                                                              |
| .xquery    | text/xml                |                                                              |
| .xsl       | text/xml                |                                                              |
| .xhtml     | text/html               |                                                              |
| .odc       | text/x-ms-odc           |                                                              |
| .r3t       | text/vnd.rn-realtext3d  |                                                              |
| .sor       | text/plain              |                                                              |

### Audio Type

| 文件扩展名 | Content-Type(Mime-Type)   | 描述                                                         |
| :--------- | :------------------------ | :----------------------------------------------------------- |
| .acp       | audio/x-mei-aac           |                                                              |
| .aif       | audio/aiff                |                                                              |
| .aiff      | audio/aiff                |                                                              |
| .aifc      | audio/aiff                |                                                              |
| .au        | audio/basic               |                                                              |
| .la1       | audio/x-liquid-file       |                                                              |
| .lavs      | audio/x-liquid-secure     |                                                              |
| .lmsff     | audio/x-la-lms            |                                                              |
| .m3u       | audio/mpegurl             |                                                              |
| .midi      | audio/mid                 |                                                              |
| .mid       | audio/mid                 |                                                              |
| .mp2       | audio/mp2                 |                                                              |
| .mp3       | audio/mp3                 |                                                              |
| .mp4       | audio/mp4                 | MP4，全称MPEG-4 Part 14，是一种使用MPEG-4的多媒体计算机文件格式，扩展名为.mp4，以存储数字音频及数字视频为主。 |
| .mnd       | audio/x-musicnet-download |                                                              |
| .mp1       | audio/mp1                 |                                                              |
| .mns       | audio/x-musicnet-stream   |                                                              |
| .mpga      | audio/rn-mpeg             |                                                              |
| .pls       | audio/scpls               |                                                              |
| .ram       | audio/x-pn-realaudio      |                                                              |
| .rmi       | audio/mid                 |                                                              |
| .rmm       | audio/x-pn-realaudio      |                                                              |
| .snd       | audio/basic               |                                                              |
| .wav       | audio/wav                 |                                                              |
| .wax       | audio/x-ms-wax            |                                                              |
| .wma       | audio/x-ms-wma            |                                                              |

### Video Type

| 文件扩展名 | Content-Type(Mime-Type) | 描述                                                         |
| :--------- | :---------------------- | :----------------------------------------------------------- |
| .asf       | video/x-ms-asf          |                                                              |
| .asx       | video/x-ms-asf          |                                                              |
| .avi       | video/avi               |                                                              |
| .IVF       | video/x-ivf             |                                                              |
| .m1v       | video/x-mpeg            |                                                              |
| .m2v       | video/x-mpeg            |                                                              |
| .m4e       | video/mpeg4             |                                                              |
| .movie     | video/x-sgi-movie       |                                                              |
| .mp2v      | video/mpeg              |                                                              |
| .mp4       | video/mpeg4             | MP4，全称MPEG-4 Part 14，是一种使用MPEG-4的多媒体计算机文件格式，扩展名为.mp4，以存储数字音频及数字视频为主。 |
| .mpa       | video/x-mpg             |                                                              |
| .mpe       | video/x-mpeg            |                                                              |
| .mpg       | video/mpg               |                                                              |
| .mpeg      | video/mpg               |                                                              |
| .mps       | video/x-mpeg            |                                                              |
| .mpv       | video/mpg               |                                                              |
| .mpv2      | video/mpeg              |                                                              |
| .wm        | video/x-ms-wm           |                                                              |
| .wmv       | video/x-ms-wmv          |                                                              |
| .wmx       | video/x-ms-wmx          |                                                              |
| .wvx       | video/x-ms-wvx          |                                                              |

### Image Type

| 文件扩展名 | Content-Type(Mime-Type) | 描述                                                         |
| :--------- | :---------------------- | :----------------------------------------------------------- |
| .tif       | image/tiff              | 标签图像文件格式（Tagged Image File Format，简写为TIFF）是一种主要用来存储包括照片和艺术图在内的图像的文件格式。它最初由Aldus公司与微软公司一起为PostScript打印开发。 |
| .fax       | image/fax               |                                                              |
| .gif       | image/gif               | 图像互换格式（GIF，Graphics Interchange Format）是一种位图图形文件格式，以8位色（即256种颜色）重现真彩色的图像。 |
| .ico       | image/x-icon            |                                                              |
| .jfif      | image/jpeg              |                                                              |
| .jpe       | image/jpeg              |                                                              |
| .jpeg      | image/jpeg              | JPEG是一种针对相片图像而广泛使用的一种有损压缩标准方法。     |
| .jpg       | image/jpeg              |                                                              |
| .net       | image/pnetvue           |                                                              |
| .png       | image/png               | 便携式网络图形（Portable Network Graphics，PNG）是一种无损压缩的位图图形格式，支持索引、灰度、RGB三种颜色方案以及Alpha通道等特性。 |
| .rp        | image/vnd.rn-realpix    |                                                              |
| .tif       | image/tiff              |                                                              |
| .tiff      | image/tiff              |                                                              |
| .wbmp      | image/vnd.wap.wbmp      |                                                              |

### Message Type

| 文件扩展名 | Content-Type(Mime-Type) | 描述 |
| :--------- | :---------------------- | :--- |
| .eml       | message/rfc822          |      |
| .mht       | message/rfc822          |      |
| .mhtml     | message/rfc822          |      |
| .nws       | message/rfc822          |      |

### Drawing Type

| 文件扩展名 | Content-Type(Mime-Type) | 描述 |
| :--------- | :---------------------- | :--- |
| .907       | drawing/907             |      |
| .slk       | drawing/x-slk           |      |
| .top       | drawing/x-top           |      |

### Java Type

| 文件扩展名 | Content-Type(Mime-Type) | 描述 |
| :--------- | :---------------------- | :--- |
| .class     | java/*                  |      |
| .java      | java/*                  |      |

### Other Type

| 文件扩展名 | Content-Type(Mime-Type) | 描述 |
| :--------- | :---------------------- | :--- |
| .dwf       | Model/vnd.dwf           |      |





# HTML/XML转义字符对照表

HTML/XML转义字符对照表包含符号、数学符号、希腊字母 、重要的国际标志、ISO 8859-1 (Latin-1)字符集、特殊符号等。





### 特殊字符转义表

| 字符                           | 十进制 | 转义字符 |
| :----------------------------- | :----- | :------- |
| "                              | &#34;  | &quot;   |
| &                              | &#38;  | &amp;    |
| <                              | &#60;  | &lt;     |
| >                              | &#62;  | &gt;     |
| 不断开空格(non-breaking space) | &#160; | &nbsp;   |

### 最常用的转义字符列表

| 显示 | 说明           | 实体名称 | 十进制编号 |
| :--- | :------------- | :------- | :--------- |
|      | 半方大的空白   | &ensp;   | &#8194;    |
|      |                |          |            |
|      | 全方大的空白   | &emsp;   | &#8195;    |
|      |                |          |            |
|      | 不断行的空白格 | &nbsp;   | &#160;     |
| <    | 小于           | &lt;     | &#60;      |
| >    | 大于           | &gt;     | &#62;      |
| &    | &符号          | &amp;    | &#38;      |
| "    | 双引号         | &quot;   | &#34;      |
| ©    | 版权           | &copy;   | &#169;     |
| ®    | 已注册商标     | &reg;    | &#174;     |
| ™    | 商标（美国）   | &trade;  | &#8482;    |
|      |                |          |            |
| ×    | 乘号           | &times;  | &#215;     |
| ÷    | 除号           | &divide; | &#247;     |

### ISO 8859-1 (Latin-1)字符集

| 显示 | 名称     | 十进制 | 显示 | 名称     | 十进制 | 显示 | 名称     | 十进制 | 显示 | 名称     | 十进制 | 显示 | 名称     | 十进制 |
| :--- | :------- | :----- | :--- | :------- | :----- | :--- | :------- | :----- | :--- | :------- | :----- | :--- | :------- | :----- |
|      | &nbsp;   | &#160; | ¡    | &iexcl;  | &#161; | ¢    | &cent;   | &#162; | £    | &pound;  | &#163; | ¤    | &curren; | &#164; |
| ¥    | &yen;    | &#165; | ¦    | &brvbar; | &#166; | §    | &sect;   | &#167; | ¨    | &uml;    | &#168; | ©    | &copy;   | &#169; |
| ª    | &ordf;   | &#170; | «    | &laquo;  | &#171; | ¬    | &not;    | &#172; |      | &shy;    | &#173; | ®    | &reg;    | &#174; |
| ¯    | &macr;   | &#175; | °    | &deg;    | &#176; | ±    | &plusmn; | &#177; | ²    | &sup2;   | &#178; | ³    | &sup3;   | &#179; |
| ´    | &acute;  | &#180; | µ    | &micro;  | &#181; | ¶    | &para;   | &#182; | ·    | &middot; | &#183; | ¸    | &cedil;  | &#184; |
| ¹    | &sup1;   | &#185; | º    | &ordm;   | &#186; | »    | &raquo;  | &#187; | ¼    | &frac14; | &#188; | ½    | &frac12; | &#189; |
| ¾    | &frac34; | &#190; | ¿    | &iquest; | &#191; | À    | &Agrave; | &#192; | Á    | &Aacute; | &#193; | Â    | &Acirc;  | &#194; |
| Ã    | &Atilde; | &#195; | Ä    | &Auml;   | &#196; | Å    | &Aring;  | &#197; | Æ    | &AElig;  | &#198; | Ç    | &Ccedil; | &#199; |
| È    | &Egrave; | &#200; | É    | &Eacute; | &#201; | Ê    | &Ecirc;  | &#202; | Ë    | &Euml;   | &#203; | Ì    | &Igrave; | &#204; |
| Í    | &Iacute; | &#205; | Î    | &Icirc;  | &#206; | Ï    | &Iuml;   | &#207; | Ð    | &ETH;    | &#208; | Ñ    | &Ntilde; | &#209; |
| Ò    | &Ograve; | &#210; | Ó    | &Oacute; | &#211; | Ô    | &Ocirc;  | &#212; | Õ    | &Otilde; | &#213; | Ö    | &Ouml;   | &#214; |
| ×    | &times;  | &#215; | Ø    | &Oslash; | &#216; | Ù    | &Ugrave; | &#217; | Ú    | &Uacute; | &#218; | Û    | &Ucirc;  | &#219; |
| Ü    | &Uuml;   | &#220; | Ý    | &Yacute; | &#221; | Þ    | &THORN;  | &#222; | ß    | &szlig;  | &#223; | à    | &agrave; | &#224; |
| á    | &aacute; | &#225; | â    | &acirc;  | &#226; | ã    | &atilde; | &#227; | ä    | &auml;   | &#228; | å    | &aring;  | &#229; |
| æ    | &aelig;  | &#230; | ç    | &ccedil; | &#231; | è    | &egrave; | &#232; | é    | &eacute; | &#233; | ê    | &ecirc;  | &#234; |
| ë    | &euml;   | &#235; | ì    | &igrave; | &#236; | í    | &iacute; | &#237; | î    | &icirc;  | &#238; | ï    | &iuml;   | &#239; |
| ð    | &eth;    | &#240; | ñ    | &ntilde; | &#241; | ò    | &ograve; | &#242; | ó    | &oacute; | &#243; | ô    | &ocirc;  | &#244; |
| õ    | &otilde; | &#245; | ö    | &ouml;   | &#246; | ÷    | &divide; | &#247; | ø    | &oslash; | &#248; | ù    | &ugrave; | &#249; |
| ú    | &uacute; | &#250; | û    | &ucirc;  | &#251; | ü    | &uuml;   | &#252; | ý    | &yacute; | &#253; | þ    | &thorn;  | &#254; |
| ÿ    | &yuml;   | &#255; |      |          |        |      |          |        |      |          |        |      |          |        |

### 符号、数学符号和希腊字母  symbols, mathematical symbols, and Greek letters

| 显示 | 名称       | 编号    | 显示 | 名称     | 编号    | 显示 | 名称     | 编号    | 显示 | 名称      | 编号    | 显示 | 名称      | 编号    |
| :--- | :--------- | :------ | :--- | :------- | :------ | :--- | :------- | :------ | :--- | :-------- | :------ | :--- | :-------- | :------ |
| ƒ    | &fnof;     | &#402;  | Α    | &Alpha;  | &#913;  | Β    | &Beta;   | &#914;  | Γ    | &Gamma;   | &#915;  | Δ    | &Delta;   | &#916;  |
| Ε    | &Epsilon;  | &#917;  | Ζ    | &Zeta;   | &#918;  | Η    | &Eta;    | &#919;  | Θ    | &Theta;   | &#920;  | Ι    | &Iota;    | &#921;  |
| Κ    | &Kappa;    | &#922;  | Λ    | &Lambda; | &#923;  | Μ    | &Mu;     | &#924;  | Ν    | &Nu;      | &#925;  | Ξ    | &Xi;      | &#926;  |
| Ο    | &Omicron;  | &#927;  | Π    | &Pi;     | &#928;  | Ρ    | &Rho;    | &#929;  | Σ    | &Sigma;   | &#931;  | Τ    | &Tau;     | &#932;  |
| Υ    | &Upsilon;  | &#933;  | Φ    | &Phi;    | &#934;  | Χ    | &Chi;    | &#935;  | Ψ    | &Psi;     | &#936;  | Ω    | &Omega;   | &#937;  |
| α    | &alpha;    | &#945;  | β    | &beta;   | &#946;  | γ    | &gamma;  | &#947;  | δ    | &delta;   | &#948;  | ε    | &epsilon; | &#949;  |
| ζ    | &zeta;     | &#950;  | η    | &eta;    | &#951;  | θ    | &theta;  | &#952;  | ι    | &iota;    | &#953;  | κ    | &kappa;   | &#954;  |
| λ    | &lambda;   | &#955;  | μ    | &mu;     | &#956;  | ν    | &nu;     | &#957;  | ξ    | &xi;      | &#958;  | ο    | &omicron; | &#959;  |
| π    | &pi;       | &#960;  | ρ    | &rho;    | &#961;  | ς    | &sigmaf; | &#962;  | σ    | &sigma;   | &#963;  | τ    | &tau;     | &#964;  |
| υ    | &upsilon;  | &#965;  | φ    | &phi;    | &#966;  | χ    | &chi;    | &#967;  | ψ    | &psi;     | &#968;  | ω    | &omega;   | &#969;  |
| ?    | &thetasym; | &#977;  | ?    | &upsih;  | &#978;  | ?    | &piv;    | &#982;  | •    | &bull;    | &#8226; | …    | &hellip;  | &#8230; |
| ′    | &prime;    | &#8242; | ″    | &Prime;  | &#8243; | ‾    | &oline;  | &#8254; | ⁄    | &frasl;   | &#8260; | ℘    | &weierp;  | &#8472; |
| ℑ    | &image;    | &#8465; | ℜ    | &real;   | &#8476; | ™    | &trade;  | &#8482; | ℵ    | &alefsym; | &#8501; | ←    | &larr;    | &#8592; |
| ↑    | &uarr;     | &#8593; | →    | &rarr;   | &#8594; | ↓    | &darr;   | &#8595; | ↔    | &harr;    | &#8596; | ↵    | &crarr;   | &#8629; |
| ⇐    | &lArr;     | &#8656; | ⇑    | &uArr;   | &#8657; | ⇒    | &rArr;   | &#8658; | ⇓    | &dArr;    | &#8659; | ⇔    | &hArr;    | &#8660; |
| ∀    | &forall;   | &#8704; | ∂    | &part;   | &#8706; | ∃    | &exist;  | &#8707; | ∅    | &empty;   | &#8709; | ∇    | &nabla;   | &#8711; |
| ∈    | &isin;     | &#8712; | ∉    | &notin;  | &#8713; | ∋    | &ni;     | &#8715; | ∏    | &prod;    | &#8719; | ∑    | &sum;     | &#8721; |
| −    | &minus;    | &#8722; | ∗    | &lowast; | &#8727; | √    | &radic;  | &#8730; | ∝    | &prop;    | &#8733; | ∞    | &infin;   | &#8734; |
| ∠    | &ang;      | &#8736; | ∧    | &and;    | &#8743; | ∨    | &or;     | &#8744; | ∩    | &cap;     | &#8745; | ∪    | &cup;     | &#8746; |
| ∫    | &int;      | &#8747; | ∴    | &there4; | &#8756; | ∼    | &sim;    | &#8764; | ∝    | &cong;    | &#8773; | ≈    | &asymp;   | &#8776; |
| ≠    | &ne;       | &#8800; | ≡    | &equiv;  | &#8801; | ≤    | &le;     | &#8804; | ≥    | &ge;      | &#8805; | ⊂    | &sub;     | &#8834; |
| ⊃    | &sup;      | &#8835; | ⊄    | &nsub;   | &#8836; | ⊆    | &sube;   | &#8838; | ⊇    | &supe;    | &#8839; | ⊕    | &oplus;   | &#8853; |
| ⊗    | &otimes;   | &#8855; | ⊥    | &perp;   | &#8869; | ⋅    | &sdot;   | &#8901; | ?    | &lceil;   | &#8968; | ?    | &rceil;   | &#8969; |
| ?    | &lfloor;   | &#8970; | ?    | &rfloor; | &#8971; | ?    | &lang;   | &#9001; | ?    | &rang;    | &#9002; | ◊    | &loz;     | &#9674; |
| ♠    | &spades;   | &#9824; | ♣    | &clubs;  | &#9827; | ♥    | &hearts; | &#9829; | ♦    | &diams;   | &#9830; |      |           |         |

### 重要的国际标记 markup-significant and internationalization characters

| 显示 | 名称     | 编号    | 显示 | 名称     | 编号    | 显示 | 名称     | 编号    | 显示 | 名称     | 编号    | 显示 | 名称     | 编号    |
| :--- | :------- | :------ | :--- | :------- | :------ | :--- | :------- | :------ | :--- | :------- | :------ | :--- | :------- | :------ |
| "    | &quot;   | &#34;   | &    | &amp;    | &#38;   | <    | &lt;     | &#60;   | >    | &gt;     | &#62;   | Œ    | &OElig;  | &#338;  |
| œ    | &oelig;  | &#339;  | Š    | &Scaron; | &#352;  | š    | &scaron; | &#353;  | Ÿ    | &Yuml;   | &#376;  | ˆ    | &circ;   | &#710;  |
| ˜    | &tilde;  | &#732;  |      | &ensp;   | &#8194; |      | &emsp;   | &#8195; |      | &thinsp; | &#8201; | ‌     | &zwnj;   | &#8204; |
| ‍‍     | &zwj;    | &#8205; | ‎‎     | &lrm;    | &#8206; | ‏‏     | &rlm;    | &#8207; | –    | &ndash;  | &#8211; | —    | &mdash;  | &#8212; |
| ‘    | &lsquo;  | &#8216; | ’    | &rsquo;  | &#8217; | ‚    | &sbquo;  | &#8218; | “    | &ldquo;  | &#8220; | ”    | &rdquo;  | &#8221; |
| „    | &bdquo;  | &#8222; | †    | &dagger; | &#8224; | ‡    | &Dagger; | &#8225; | ‰    | &permil; | &#8240; | ‹    | &lsaquo; | &#8249; |
| ›    | &rsaquo; | &#8250; | €    | &euro;   | &#8364; |      |          |         |      |          |         |      |          |         |

### 为什么要用转义字符串？

HTML中<，>，&等有特殊含义（<，>，用于链接签，&用于转义），不能直接使用。这些符号是不显示在我们最终看到的网页里的，那如果我们希望在网页中显示这些符号，该怎么办呢？

这就要说到HTML转义字符串（Escape Sequence）了。

转义字符串（Escape Sequence）也称字符实体(Character Entity)。在HTML中，定义转义字符串的原因有两个：第一个原因是像“<”和“>”这类符号已经用来表示HTML标签，因此就不能直接当作文本中的符号来使用。为了在HTML文档中使用这些符号，就需要定义它的转义字符串。当解释程序遇到这类字符串时就把它解释为真实的字符。在输入转义字符串时，要严格遵守字母大小写的规则。第二个原因是，有些字符在ASCII字符集中没有定义，因此需要使用转义字符串来表示。

### 转义字符串的组成

转义字符串（Escape Sequence），即字符实体（Character Entity）分成三部分：第一部分是一个&符号，英文叫ampersand；第二部分是实体（Entity）名字或者是#加上实体（Entity）编号；第三部分是一个分号。

比如，要显示小于号（<），就可以写 &lt; 或者 &#60; 。

用实体（Entity）名字的好处是比较好理解，一看lt，大概就猜出是less than的意思，但是其劣势在于并不是所有的浏览器都支持最新的Entity名字。而实体(Entity)编号，各种浏览器都能处理。

提示：实体名称（Entity）是区分大小写的。

备注：同一个符号，可以用“实体名称”和“实体编号”两种方式引用，“实体名称”的优势在于便于记忆，但不能保证所有的浏览器都能顺利识别它，而“实体编号”则没有这种担忧，但它实在不方便记忆。

### 如何显示空格？

通常情况下，HTML会自动截去多余的空格。不管你加多少空格，都被看做一个空格。比如你在两个字之间加了10个空格，HTML会截去9个空格，只保留一个。为了在网页中增加空格，你可以使用&nbsp;表示空格。

## PS：需要对字符串进行转义操作的朋友可使用此工具【[在线HTML转义/反转义工具](http://tools.jb51.net/transcoding/html_transcode)】





# JavaScript特殊字符转义字符对照表

JavaScript编程中如果遇到下表中的这些特殊字符，需要使用反斜杠来向文本字符串添加这些特殊字符，这就是JavaScript的转义字符。

| Unicode 字符值 | 转义序列 | 含义           | 类别     |
| :------------- | :------- | :------------- | :------- |
| \u0008         | \b       | Backspace      |          |
| \u0009         | \t       | Tab            | 空白     |
| \u000A         | \n       | 换行符（换行） | 行结束符 |
| \u000B         | \v       | 垂直制表符     | 空白     |
| \u000C         | \f       | 换页           | 空白     |
| \u000D         | \r       | 回车           | 行结束符 |
| \u0020         |          | 空格           | 空白     |
| \u0022         | \"       | 双引号 (")     |          |
| \u0027         | \'       | 单引号 (')     |          |
| \u005C         | \\       | 反斜杠 (\)     |          |
| \u00A0         |          | 不间断空格     | 空白     |
| \u2028         |          | 行分隔符       | 行结束符 |
| \u2029         |          | 段落分隔符     | 行结束符 |
| \uFEFF         |          | 字节顺序标记   | 空白     |



# 常用浏览器(PC,移动) user-agent

该对照表整理了时下流行的浏览器User-Agent大全，User Agent也简称UA。它是一个特殊字符串头，是一种向访问网站提供你所使用的浏览器类型及版本、操作系统及版本、浏览器内核、等信息的标识。并悬浮提示来访者的user-agent信息。

## PC端
|浏览器|user-agent|
|:----|:----|
|safari 5.1 – MAC|User-Agent,Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10_6_8; en-us) AppleWebKit/534.50 (KHTML, like Gecko) Version/5.1 Safari/534.50|
|safari 5.1 – Windows|User-Agent,Mozilla/5.0 (Windows; U; Windows NT 6.1; en-us) AppleWebKit/534.50 (KHTML, like Gecko) Version/5.1 Safari/534.50|
|IE 9.0|User-Agent,Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Trident/5.0;|
|IE 8.0|User-Agent,Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.0; Trident/4.0)|
|IE 7.0|User-Agent,Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 6.0)|
|IE 6.0|User-Agent, Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)|
|Firefox 4.0.1 – MAC|User-Agent, Mozilla/5.0 (Macintosh; Intel Mac OS X 10.6; rv,2.0.1) Gecko/20100101 Firefox/4.0.1|
|Firefox 4.0.1 – Windows|User-Agent,Mozilla/5.0 (Windows NT 6.1; rv,2.0.1) Gecko/20100101 Firefox/4.0.1|
|Opera 11.11 – MAC|User-Agent,Opera/9.80 (Macintosh; Intel Mac OS X 10.6.8; U; en) Presto/2.8.131 Version/11.11|
|Opera 11.11 – Windows|User-Agent,Opera/9.80 (Windows NT 6.1; U; en) Presto/2.8.131 Version/11.11|
|Chrome 17.0 – MAC|User-Agent, Mozilla/5.0 (Macintosh; Intel Mac OS X 10_7_0) AppleWebKit/535.11 (KHTML, like Gecko) Chrome/17.0.963.56 Safari/535.11|
|傲游（Maxthon）|User-Agent, Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; Maxthon 2.0)|
|腾讯TT|User-Agent, Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; TencentTraveler 4.0)|
|世界之窗（The World） 2.x|User-Agent, Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1)|
|世界之窗（The World） 3.x|User-Agent, Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; The World)|
|搜狗浏览器 1.x|User-Agent, Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; Trident/4.0; SE 2.X MetaSr 1.0; SE 2.X MetaSr 1.0; .NET CLR 2.0.50727; SE 2.X MetaSr 1.0)|
|360浏览器|User-Agent, Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; 360SE)|
|Avant|User-Agent, Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; Avant Browser)|
|Green Browser|User-Agent, Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1)|
|UC浏览器|User-Agent,Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/55.0.2883.87 UBrowser/6.2.4094.1 Safari/537.36|

## 移动设备端
|浏览器|user-agent|
|:----|:----|
|safari iOS 4.33 – iPhone|User-Agent,Mozilla/5.0 (iPhone; U; CPU iPhone OS 4_3_3 like Mac OS X; en-us) AppleWebKit/533.17.9 (KHTML, like Gecko) Version/5.0.2 Mobile/8J2 Safari/6533.18.5|
|safari iOS 4.33 – iPod Touch|User-Agent,Mozilla/5.0 (iPod; U; CPU iPhone OS 4_3_3 like Mac OS X; en-us) AppleWebKit/533.17.9 (KHTML, like Gecko) Version/5.0.2 Mobile/8J2 Safari/6533.18.5|
|safari iOS 4.33 – iPad|User-Agent,Mozilla/5.0 (iPad; U; CPU OS 4_3_3 like Mac OS X; en-us) AppleWebKit/533.17.9 (KHTML, like Gecko) Version/5.0.2 Mobile/8J2 Safari/6533.18.5|
|Android N1|User-Agent, Mozilla/5.0 (Linux; U; Android 2.3.7; en-us; Nexus One Build/FRF91) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1|
|Android QQ浏览器 For android|User-Agent, MQQBrowser/26 Mozilla/5.0 (Linux; U; Android 2.3.7; zh-cn; MB200 Build/GRJ22; CyanogenMod-7) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1|
|Android Opera Mobile|User-Agent, Opera/9.80 (Android 2.3.4; Linux; Opera Mobi/build-1107180945; U; en-GB) Presto/2.8.149 Version/11.10|
|Android Pad Moto Xoom|User-Agent, Mozilla/5.0 (Linux; U; Android 3.0; en-us; Xoom Build/HRI39) AppleWebKit/534.13 (KHTML, like Gecko) Version/4.0 Safari/534.13|
|BlackBerry|User-Agent, Mozilla/5.0 (BlackBerry; U; BlackBerry 9800; en) AppleWebKit/534.1+ (KHTML, like Gecko) Version/6.0.0.337 Mobile Safari/534.1+|
|WebOS HP Touchpad|User-Agent, Mozilla/5.0 (hp-tablet; Linux; hpwOS/3.0.0; U; en-US) AppleWebKit/534.6 (KHTML, like Gecko) wOSBrowser/233.70 Safari/534.6 TouchPad/1.0|
|Nokia N97|User-Agent, Mozilla/5.0 (SymbianOS/9.4; Series60/5.0 NokiaN97-1/20.0.019; Profile/MIDP-2.1 Configuration/CLDC-1.1) AppleWebKit/525 (KHTML, like Gecko) BrowserNG/7.1.18124|
|Windows Phone Mango|User-Agent, Mozilla/5.0 (compatible; MSIE 9.0; Windows Phone OS 7.5; Trident/5.0; IEMobile/9.0; HTC; Titan)|
|UC无|User-Agent, UCWEB7.0.2.37/28/999|
|UC标准|User-Agent, NOKIA5700/ UCWEB7.0.2.37/28/999|
|UCOpenwave|User-Agent, Openwave/ UCWEB7.0.2.37/28/999|
|UC Opera|User-Agent, Mozilla/4.0 (compatible; MSIE 6.0; ) Opera/UCWEB7.0.2.37/28/999|



