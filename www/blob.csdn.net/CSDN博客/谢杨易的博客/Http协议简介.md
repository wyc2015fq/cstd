
# Http协议简介 - 谢杨易的博客 - CSDN博客

2016年09月04日 12:01:12[谢杨易](https://me.csdn.net/u013510838)阅读数：1503



## 1 URL与资源
1）URI和URL
URI：统一资源标识符。用于标识某一互联网资源名称的字符串。包括URL和URN
URL：统一资源定位符。URI的一种形式。现在基本都是用URL来代表URI。可以将URL和URI等同来看待。
URN：统一资源名称。URI的另一种形式。仅仅处于试验阶段。它用资源名称来代表资源，与路径无关。故资源移动位置时，不用改变。
2）URL格式
scheme://user:password@host:port/path;params?query\#frag
scheme：协议，如http https ftp
host：主机名，标示了server地址，DNS会将host名解析为IP地址。
port：端口号，表示server上的对应的应用程序。http默认端口号80，https默认443
path：server上的文件路径
params：告诉server更详细的信息，如编码格式
query：查询server端数据库可能使用到。如?item=12345, 表示查询数据库中的Item号码为12345的数据
frag 片段（fragment）：只引用HTML的一个章节，而不是所有的HTML文档。server回传的仍然是整个HTML文档，frag供客户端使用
3）相对URL
i.  不完整，相对于base URL来解析它的绝对URL
ii.  如HTML文档为[http://www.test.com/test.html](http://www.test.com/test.html), 其内容中有\< a href=’./hamers.html’ />，此link即为相对URL，它的base URL就是HTML文档的url，故最后解析出的url即为[http://www.test.com/hamers.html](http://www.test.com/hamers.html)
iii. 相对URL减小了数据量，同时增加了可移植性
4)  编码方式
URL字符集只支持us-Ascii
转义字符: 用来表示不安全的字符集，如空格，~。 不使用转义的话，它们可能会被浏览器误解。%7E表示~。转义的表示方法为：%十六进制数
## 2 http报文
1）client发送的request和server回复的response都是通过http报文的形式发送的。
2）http报文结构
![Markdown](http://i2.buimg.com/573600/d2a55bb2622d492b.png)
起始行 start-line：
i. request：method url version
ii. response： version code reason
首部： header（可选）
i. 每行一个key:value, 最后一行是一个空行
ii. 通用首部，request和response都能使用
connection：keep-alive等连接中用到
Date：报文的创建时间，注意不是文档的时间
Trailer：采用分块传输时，这个首部列出位于报文拖挂部分的首部集合。这个首部一般是等body确定了才能确定下来
Transfer-encoding：报文编码方式
Via：报文经过的中间节点，可以做log。如代理，网关等。
cache-control
iii. request 首部，仅仅request可以使用
From（客户端用户的email），Host（server主机和端口号），Referer（文档URL），user-agent（浏览器名称）
Accept（接收文档类型），Accept-charset， Accept-Encoding，
TE（告诉server可以使用哪些扩展传输编码，如分块编码）
Expect（条件请求），If-Match，If-modified-since（除非在某日期后修改过，否则不用发送文档），If-Range（对范围条件请求）
Authorization（client自身的认证信息，如用户名，密码），Cookie（向服务器传送的令牌，用户名，密码可以包含在其中）
Proxy-Authorization（client发送给proxy的认证信息），Proxy-connection（解决盲代理对keep-alive的不支持使用，与connection功能相同）
iv. response首部，仅仅response可以使用
Age（过期时间，相对时间）
Accept-Ranges(服务器可接受的范围类型）
Proxy-Authenticate（来自代理的对客户端的质询列表）
set-cookie（告诉client自己在server上设置了用户，client之后会回传一个cookie）
www-Authenticate(来自服务器的对客户端的质询列表）
v. 实体首部
Allow（列出可以对此实体执行的请求方法）
Location（重定向时用，告诉client文档的url）
content-length， content-type， content-MD5（body的MD5摘要）
Expires（缓存过期时间，server返回给client用的），ETag（与此实体相关的实体标记），Last-modified（实体最后的修改时间）
body（可选）
3）method：request的方法
GET：获取页面内容，请求参数放在了url中
HEAD：只获取响应的首部，不用发送body
可在不获取资源情况下，先查看资源情况，如mine type
可先查看资源是否存在
可查看资源是否被修改了。使用cache验证是否过期时会使用
POST：获取页面内容，参数放在了body中，而不是url中
PUT：上传，让server创建一个request中url命名的文档，如果已经存在，则替换它
DELETE：删除server上的文件，不一定server会支持
OPTIONS：查看server支持哪些method
Get和Post是我们最常用到的两种method，他们的区别是：Get将请求参数放在了url中，别人都能看到，故安全性较差。Post将请求参数放在了http报文的body中，安全性比Get高。
4）状态码：response回复的状态
1开头 info
100：continue，收到了request的初始部分，请client继续发送request。常见场景为，client有个body要发送，想先看看server是否接受这个body。故先发送一个值为100 continue的expect首部。如果服务器接受，会发送一条100 continue的响应。之后client再发送body。
101：协议切换
2开头 成功
200：ok
201：created，put上传request成功执行了，server上文档已经创建好了
202 ： accept，请求已经被接受，但还没有处理
203：non-authoritative information, 响应中的body不是来自源端服务器，而只是一个副本
204：No content，响应中没有body
205：reset content，告诉浏览器清除当前页面中所有HTML表单元素
206：partial content，成功执行了一个部分或者range的请求
3开头 重定向
300： multiple choice， 客户端的请求对应了多个资源，比如此HTML文档有英语version和法语version
301：moved permanently，URL对应资源被移走了，response的location首部会告诉client资源现在的URL
302：found，与301类似，response的location首部也会告诉client资源现在的临时url，但之后的请求还是使用老URL
303：set other， 让client使用另一个url，在location首部中指出
304：not modified，文档未被修改。使用cache时，文档过期后，client会查询文档是否被修改。未被修改，则继续使用cache，而不需要重新请求文档。
305：use proxy，必须通过一个代理来访问资源，代理URL在location中给出
307：temporary redirect，与301类似，临时使用location首部中url，之后的request还是使用老url
4开头 client错误
400：bad request，request格式等可能有误
401：unauthorized，client没有资源的访问权限，必须先认证
403：forbidden，请求被server拒绝了
404：not found，url对应资源在server上没有
405：method not allowed，request中的method有误
406：not acceptable，body不被server接受
407：proxy authentication required，与401类似，用于要求对client认证的代理服务器。有时候代理也需要权限才能访问
408：request timeout
409 ：conflict，请求可能在资源上引发一些冲突
410：gone，与404类似，表示资源被移走了，但server曾经拥有过这个资源
411：length required，要求在request中包含content-length首部
412：prediction failed，客户端发起了条件请求，其中一个条件失败了。包含expect首部的就是条件请求
413：request entity too large：request发送的body太大了，比如上传了一个太大的文件
414：request URI too long：request中的url太长。有的server为了防止恶意攻击，对1KB以上的URL不处理
415：unsupported media type：request中的body的mine type不被server接受
416：request ragne not satisfied，request的是某个资源的某个范围，这个范围无效时发送此状态码
417：expectation failed，无法满足request中的expect首部
5开头 server错误
500：internal server error，server遇到了妨碍它为request提供response的错误，发送此code
501：not implemented，server搞不定request中的方法
502：bad gateway，网关server出错了，比如网关连接不到它的父网关
503：service unavailable，server目前无法提供服务，但将来可以提供服务
504：gateway timeout，网关或代理等待另一个服务器时超时了，此时会回复client这个code
505：http version not supported
3 连接管理
1）连接流程
a. 浏览器通过url解析出host
b. 浏览器查询host的DNS，转换为IP地址
c. 浏览器获得端口号
d. 浏览器发起到IP地址确定端口号的连接（三次握手）
e. 浏览器向server发送一条http request报文
f. 浏览器从server读取返回的http response报文
g. 浏览器关闭连接
2）IP分组：TCP通过名为IP分组的小数据块来发送数据。每个IP数据块包含
IP分组首部（20B）
TCP段首部（20B)
TCP数据块（0B或多个字节）
有用数据即为TCP数据块，故应该尽量将TCP数据块塞满，以提高效率。
3）操作TCP连接的API：socket，Linux driver对socket提供了支持，流程为：
server创建socket，bind到端口80上，允许socket连接（listen），并等待连接（accept）
client获取用户输入的IP和端口号，创建socket，connect到server的port上去。
通知server app有连接到来，读取connect请求
三次握手，连接成功。client发送http request
server处理http request，并回传http response，close连接
client接收http response，close连接。
4）TCP性能考虑
http事务时延
client要去查询DNS server，将host转化为IP地址
TCP连接三次握手时延
传输request和response有时延
TCP连接握手时延
延迟确认
每个TCP都有一个序列号和数据完整性的校验和，TCP会做校验，保证数据传输的正确性。对于每个数据段，都会发送一个确认
延迟确认算法思路为，在发往同一个server的数据中捎带这个确认，从而减少传输事务。搭顺风车的想法。
TCP慢启动
TCP连接刚开始时，会限制它的最大连接速度。随着时间推移，传输速度可以提高。称为TCP慢启动
故新的TCP连接速度会比经过调谐的老连接慢。
解决方法：重用连接，持久连接机制
5）Connection首部
connection首部中包含的首部，不应该被转发出去。比如client传给proxy时，proxy转发到server时，应该删除connection首部和connection首部中包含的首部
connection中的close表示处理完后关闭连接。
6）并行连接
a. 打开多条连接，并行的执行多个事务
b. 串行连接如果不能充分利用信道速度，则并行连接效率要高于串行。反之，并行效率会低。因为建立多条连接需要耗费时间
c. 并行连接可以同时加载多幅图片，给用户感觉也是要快一些。
7）持久连接
一次事务传输结束后仍然保持TCP连接。
优点：减少连接握手时延，避免TCP慢启动。
缺点：管理不当，会出现大量的空闲连接，这些连接没有关闭，仍然耗费client和server的资源。
keep-alive：
request的header中包含 connection：keep-alive
如果server支持并愿意持久连接，则在response中也会包connection：keep-alive。如果response中没有发现此header，则说明server不支持。
哑代理：识别不了keep-alive的代理。代理会在事务结束后等待连接关闭，但是client和server都能理解keep-alive，他们不会关闭连接。形成死循环。解决方法：
proxy必须删除connection首部，不能转发他。如果proxy能理解keep-alive，它自己再入到header中发送给server
使用proxy-connection代替connection。能理解的proxy会将proxy-connection替换为标准的connection首部发给server，不能理解的proxy则继续转发proxy-connection，server不会理会它。

persistent-connection，http1.1持久连接
http1.1默认激活持久连接，不像1.0中默认没有激活keep-alive那样。
如果不想使用持久连接，则必须添加header，connection：close。这样在事务传输完成后，连接会关闭
不发送connection：close，不代表server承诺会一直保持连接。是否关闭的决定权在server。
一个client对server或proxy，最多只能有两条持久连接。
8）管道化连接
在持久化连接基础上，做的又一个优化
可以将多条request放在队列中，第一条request发送后，立即发送第二条，不用等待第一条request的response回复到client
限制
如果不支持持久连接，则不能使用管道连接。管道连接是建立在持久连接基础上的又一个优化
必须按照request的顺序来发送response
client必须能够应对没收到response而连接被关闭的情况，此时一般会重发request
重发request时，应该注意可能有副作用。如post上传，不能重复发送request
9）关闭连接
server可自主决定何时关闭连接，不受keep-alive影响
每条response应该都有准确的content-length。但它也有可能不正确，此时已连接关闭为准
连接关闭可能导致副作用，比如上传文件，提交订单等。之后重发request可能造成重复。要小心此副作用。
4 Http物理结构
1）proxy
client和server的中间人。
代理的两端的网络协议相同。比如都是http。网关则是协议转换器，比如http request经过网关转换后发送给ftp server。
proxy作用
内容过滤，比如儿童过滤器
文档访问控制，通过权限认证来控制
安全防火墙，限制哪些应用层协议的数据可以流入或流出一个组织。有的防火墙只允许http数据流入流出。
web缓存，将缓存可以放在proxy上。此proxy与client距离比较近，访问速度远大于server
反向代理，假扮server，称为替代物。它们接收client的request，然后按需发送给不同的web server（重定向，负载均衡）
内容路由器
转码器，将内容发送给client之前，可以修改内容格式。比如将gzip压缩的server传输的数据，进行unzip，再回传给client
匿名者，proxy将request中的身份特性删除，比如from，referer，cookie等。可以提高保密性
proxy与client的连接
浏览器中直接设置代理，此后所有的request直接发送给此proxy
修改网络，也能拦截代理。拦截网络流量，并将其导入到代理中。通常是在交换机或路由器层级做。
修改DNS命名空间，反向代理放在web server之前，它会假扮服务器的ip和名字。这样通过DNS解析后数据就发送给了proxy。（重定向和负载均衡经常使用）
修改web server。让server发送一条重定向response给client，之后client就会将新的request都发送给proxy了。
2） cache
作用
cache访问速度远远快于server。一般cache配置在离client比较近的地方。如学校机房等。
防止server瞬间拥塞
减小距离时延。有时候server距离很远，比如国外的server。连接时延和传输时延就相当大了。
命中
新鲜度检测，称为缓存再验证，看看cache中保存的副本是不是server上最新的
cache只有在文件过期后，才会做再验证
再验证request：If-modified-since
再验证response
如果再验证发现没有被修改，则server回复304 not modified，称为再验证命中。
再验证未命中，server会将新的文档发送回来，并回复200 ok
在验证，对象被删除，回复404 not found
判断文档是否来自cache：查看Date首部，与当前时间比较。如果Date比较早，那就肯定是cache中的
缓存处理步骤
接收client的报文
解析request报文中的URL和各种header
查询是否有本地副本可用
新鲜度检测，如果文档过期了，发送if-modified-since来检测文档新鲜度
创建response，利用新的header和已缓存的文档创建response。注意不要调整Date首部，它表示的文档最开始创建的时间。
发送response到client
文档过期
server向每个文档添加一个过期日期，通过expires首部和cache-control首部
Expires在http1.0中使用，使用绝对时间。缺点是client和server很有可能时间不同步
cache-control:max-age在http1.1中使用，使用相对时间，即报文response到过期之间的时间。


