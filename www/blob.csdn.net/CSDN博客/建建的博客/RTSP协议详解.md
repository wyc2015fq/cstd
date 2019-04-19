# rtsp协议详解 - 建建的博客 - CSDN博客
2017年02月22日 14:39:56[纪建](https://me.csdn.net/u013898698)阅读数：198
个人分类：[Rtsp协议](https://blog.csdn.net/u013898698/article/category/6698534)
## [rtsp协议详解](http://www.cppblog.com/tx7do/archive/2013/09/13/203219.html)
转载自:[**http://www.mikewootc.com/wiki/net/protocol/rtsp.html**](http://www.mikewootc.com/wiki/net/protocol/rtsp.html)
目录:
- [概述](http://www.mikewootc.com/wiki/net/protocol/rtsp.html#%E6%A6%82%E8%BF%B0)- [RTSP简介](http://www.mikewootc.com/wiki/net/protocol/rtsp.html#rtsp%E7%AE%80%E4%BB%8B)
- [协议特点](http://www.mikewootc.com/wiki/net/protocol/rtsp.html#%E5%8D%8F%E8%AE%AE%E7%89%B9%E7%82%B9)
- [协议细节](http://www.mikewootc.com/wiki/net/protocol/rtsp.html#%E5%8D%8F%E8%AE%AE%E7%BB%86%E8%8A%82)- [典型的rtsp交互过程](http://www.mikewootc.com/wiki/net/protocol/rtsp.html#%E5%85%B8%E5%9E%8B%E7%9A%84rtsp%E4%BA%A4%E4%BA%92%E8%BF%87%E7%A8%8B)
- [RTSP消息格式](http://www.mikewootc.com/wiki/net/protocol/rtsp.html#rtsp%E6%B6%88%E6%81%AF%E6%A0%BC%E5%BC%8F)
- [方法定义](http://www.mikewootc.com/wiki/net/protocol/rtsp.html#%E6%96%B9%E6%B3%95%E5%AE%9A%E4%B9%89)
- [消息头定义](http://www.mikewootc.com/wiki/net/protocol/rtsp.html#%E6%B6%88%E6%81%AF%E5%A4%B4%E5%AE%9A%E4%B9%89)
- [状态码](http://www.mikewootc.com/wiki/net/protocol/rtsp.html#%E7%8A%B6%E6%80%81%E7%A0%81)
- [rtsp中常用方法举例](http://www.mikewootc.com/wiki/net/protocol/rtsp.html#rtsp%E4%B8%AD%E5%B8%B8%E7%94%A8%E6%96%B9%E6%B3%95%E4%B8%BE%E4%BE%8B)
- [SDP协议概述](http://www.mikewootc.com/wiki/net/protocol/rtsp.html#sdp%E5%8D%8F%E8%AE%AE%E6%A6%82%E8%BF%B0)- [简介](http://www.mikewootc.com/wiki/net/protocol/rtsp.html#%E7%AE%80%E4%BB%8B)
- [SDP协议格式](http://www.mikewootc.com/wiki/net/protocol/rtsp.html#sdp%E5%8D%8F%E8%AE%AE%E6%A0%BC%E5%BC%8F)
- [SDP协议举例说明](http://www.mikewootc.com/wiki/net/protocol/rtsp.html#sdp%E5%8D%8F%E8%AE%AE%E4%B8%BE%E4%BE%8B%E8%AF%B4%E6%98%8E)
## 概述
### RTSP简介
RTSP(Real Time Streaming Protocol), 实时流传输协议, 是TCP/IP协议体系中的一个`应用层`协议, 由哥伦比亚大学, 网景和RealNetworks公司提交的IETF RFC标准. 该协议定义了一对多应用程序如何有效地通过IP网络传送多媒体数据. RTSP在体系结构上位于RTP和RTCP之上, 它使用TCP或RTP完成数据传输.
![流媒体服务协议栈](http://www.mikewootc.com/wiki/net/protocol/Rtsp_Arch.png)
流媒体服务协议栈
RTSP提供了一个可扩展框架, 使实时数据, 如音频与视频的受控点播成为可能. 数据源包括现场数据与存储在剪辑中数据. 该协议目的在于控制多个数据发送连接, 为选择发送通道, 如UDP, 组播UDP与TCP, 提供途径, 并为选择基于RTP上发送机制提供方法.
它的语法和运作跟HTTP 1.1类似, 但并不特别强调时间同步, 所以比较能容忍网络延迟.
HTTP与RTSP相比, * HTTP传送HTML. HTTP请求由客户机发出, 服务器作出响应 * RTSP传送的是多媒体数据. 使用RTSP时, 客户机和服务器都可以发出请求, 即RTSP可以是`双向`的.
RTSP是用来控制声音或影像的多媒体串流协议, 并允许同时多个串流需求控制, 传输时所用的网络通讯协议并不在其定义的范围内, 服务器端可以自行选择使用TCP或UDP来传送串流内容.
而前面提到的允许同时多个串流需求控制(Multicast), 除了可以降低服务器端的网络用量, 更进而支持多方视讯会议(Video Conference). 因为与HTTP1.1的运作方式相似, 所以代理服务器〈Proxy〉的快取功能〈Cache〉也同样适用于RTSP, 并因RTSP具有重新导向功能, 可视实际负载情况来转换提供服务的服务器, 以避免过大的负载集中于同一服务器而造成延迟.
该协议用于C/S模型, 是一个基于文本的协议, 用于在客户端和服务器端建立和协商实时流会话.
实时流协议(RTSP)建立并控制一个或几个时间同步的连续流媒体. 尽管连续媒体流与控制流交换是可能的, 通常它本身并不发送连续流. 换言之, RTSP充当多媒体服务器的网络远程控制. RTSP连接没有绑定到传输层连接, 如TCP. 在RTSP连接期间, RTSP用户可打开或关闭多个对服务器的可传输连接以发出RTSP请求. 此外, 可使用无连接传输协议, 如UDP. RTSP流控制的流可能用到RTP, 但RTSP操作并不依赖用于携带连续媒体的传输机制.
协议支持的操作如下: (1)从媒体服务器上检索媒体: 用户可通过HTTP或其它方法提交一个演示描述. 如演示是组播, 演示式就包含用于连续媒体的的组播地址和端口. 如演示仅通过单播发送给用户, 用户为了安全应提供目的地址. (2)媒体服务器邀请进入会议: 媒体服务器可被邀请参加正进行的会议, 或回放媒体, 或记录其中一部分, 或全部. 这种模式在分布式教育应用上很有用, 会议中几方可轮流按远程控制按钮. (3)将媒体加到现成讲座中: 如服务器告诉用户可获得附加媒体内容, 对现场讲座显得尤其有用. 如HTTP/1.1中类似,
 RTSP请求可由代理, 通道与缓存处理.
### 协议特点
- 可扩展性: 新方法和参数很容易加入RTSP.
- 易解析: RTSP可由标准HTTP或MIME解析器解析.
- 安全: RTSP使用网页安全机制.
- 独立于传输: RTSP可使用不可靠数据报协议(EDP), 可靠数据报协议(RDP); 如要实现应用级可靠, 可使用可靠流协议.
- 多服务器支持: 每个流可放在不同服务器上, 用户端自动与不同服务器建立几个并发控制连接, 媒体同步在传输层执行.
- 记录设备控制: 协议可控制记录和回放设备.
- 流控与会议开始分离: 仅要求会议初始化协议提供, 或可用来创建惟一会议标识号. 特殊情况下, 可用SIP或H.323来邀请服务器入会.
- 适合专业应用: 通过SMPTE时标, RTSP支持帧级精度, 允许远程数字编辑.
- 演示描述中立: 协议没强加特殊演示或元文件, 可传送所用格式类型; 然而, 演示描述至少必须包括一个RTSP URL.
- 代理与防火墙友好: 协议可由应用和传输层防火墙处理. 防火墙需要理解SETUP方法, 为UDP媒体流打开一个“缺口”.
- HTTP友好: 此处, RTSP明智地采用HTTP观念, 使现在结构都可重用. 结构包括Internet内容选择平台(PICS). 由于在大多数情况下控制连续媒体需要服务器状态, RTSP不仅仅向HTFP添加方法.
- 适当的服务器控制: 如用户启动一个流, 必须也可以停止一个流.
- 传输协调: 实际处理连续媒体流前, 用户可协调传输方法.
- 性能协调: 如基本特征无效, 必须有一些清理机制让用户决定哪种方法没生效. 这允许用户提出适合的用户界面.
## 协议细节
### 典型的rtsp交互过程
C表示rtsp客户端, S表示rtsp服务端1. C->S:OPTION request //询问S有哪些方法可用
1. S->C:OPTION response //S回应信息中包括提供的所有可用方法
2. C->S:DESCRIBE request //要求得到S提供的媒体初始化描述信息
2. S->C:DESCRIBE response //S回应媒体初始化描述信息, 主要是sdp
3. C->S:SETUP request //设置会话的属性, 以及传输模式, 提醒S建立会话
3. S->C:SETUP response //S建立会话, 返回会话标识符, 以及会话相关信息
4. C->S:PLAY request //C请求播放
4. S->C:PLAY response //S回应该请求的信息
S->C:发送流媒体数据
5. C->S:TEARDOWN request //C请求关闭会话
5. S->C:TEARDOWN response //S回应该请求
上述的过程是标准的, 友好的rtsp流程, 但实际的需求中并不一定按部就班来. 其中第3和4步是必需的！
第一步, 只要服务器客户端约定好, 有哪些方法可用, 则option请求可以不要.
第二步, 如果我们有其他途径得到媒体初始化描述信息(比如http请求等等), 则我们也不需要通过rtsp中的describe请求来完成.
第五步, 可以根据系统需求的设计来决定是否需要.
### RTSP消息格式
RTSP的消息有两大类: `请求消息(request)`, `回应消息(response)`.
请求消息方法 URI RTSP版本 CR LF
消息头 CR LF
CR LF
消息体 CR LF
其中`方法`包括OPTION回应中所有的命令,URI是接受方的地址,例如:rtsp://192.168.20.136. RTSP版本一般都是 RTSP/1.0. 每行后面的CR LF表示回车换行, 需要接受端有相应的解析, 最后一个`消息头`需要有两个CR LF(即空行)
回应消息RTSP版本 状态码 解释 CR LF
消息头 CR LF
CR LF
消息体 CR LF
其中RTSP版本一般都是RTSP/1.0, 状态码是一个数值, 200表示成功, 解释是与状态码对应的文本解释.
### 方法定义
方法记号表示资源上执行的方法, 它区分大小写. 新方法可在将来定义, 但不能以$开头. 已定义方法如下表所示:
(注: P----演示, S----流, C----用户端, S----服务器端)
| 方法 | 方向 | 对象 | 要求 | 含义 |
|---------------|-----------|------|------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| DESCRIBE | C->S | P,S | 推荐 | 检查演示或媒体对象的描述, 也允许使用接收头指定用户理解的描述格式. DESCRIBE的答复-响应组成媒体RTSP初始阶段 |
| ANNOUNCE | C->S S->C | P,S | 可选 | 当从用户发往服务器时, ANNOUNCE将请求URL识别的演示或媒体对象描述发送给服务器; 反之, ANNOUNCE实时更新连接描述. 如新媒体流加入演示, 整个演示描述再次发送, 而不仅仅是附加组件, 使组件能被删除 |
| `GET_PARAMETER` | C->S S->C | P,S | 可选 | `GET_PARAMETER`请求检查RUL指定的演示与媒体的参数值. 没有实体体时, `GET_PARAMETER`也许能用来测试用户与服务器的连通情况 |
| OPTIONS | C->S S->C | P,S | 要求 | 可在任意时刻发出OPTIONS请求, 如用户打算尝试非标准请求, 并不影响服务器状态 |
| PAUSE | C->S | P,S | 推荐 | PAUSE请求引起流发送临时中断. 如请求URL命名一个流, 仅回放和记录被停止; 如请求URL命名一个演示或流组, 演示或组中所有当前活动的流发送都停止. 恢复回放或记录后, 必须维持同步. 在SETUP消息中连接头超时参数所指定时段期间被暂停后, 尽管服务器可能关闭连接并释放资源, 但服务器资源会被预订 |
| PLAY | C->S | P,S | 要求 | PLAY告诉服务器以SETUP指定的机制开始发送数据; 直到一些SETUP请求被成功响应, 客户端才可发布PLAY请求. PLAY请求将正常播放时间设置在所指定范围的起始处, 发送流数据直到范围的结束处. PLAY请求可排成队列, 服务器将PLAY请求排成队列, 顺序执行 |
| RECORD | C->S | P,S | 可选 | 该方法根据演示描述初始化媒体数据记录范围, 时标反映开始和结束时间; 如没有给出时间范围, 使用演示描述提供的开始和结束时间. 如连接已经启动, 立即开始记录, 服务器数据请求URL或其他URL决定是否存储记录的数据; 如服务器没有使用URL请求, 响应应为201(创建), 并包含描述请求状态和参考新资源的实体与位置头. 支持现场演示记录的媒体服务器必须支持时钟范围格式, smpte格式没有意义 |
| REDIRECT | S->C | P,S | 可选 | 重定向请求通知客户端连接到另一服务器地址. 它包含强制头地址, 指示客户端发布URL请求; 也可能包括参数范围, 以指明重定向何时生效. 若客户端要继续发送或接收URL媒体, 客户端必须对当前连接发送TEARDOWN请求, 而对指定主执新连接发送SETUP请求 |
| SETUP | C->S | S | 要求 | 对URL的SETUP请求指定用于流媒体的传输机制. 客户端对正播放的流发布一个SETUP请求, 以改变服务器允许的传输参数. 如不允许这样做, 响应错误为"455 Method Not Valid In This State”. 为了透过防火墙, 客户端必须指明传输参数, 即使对这些参数没有影响 |
| `SET_PARAMETER` | C->S S->C | P,S | 可选 | 这个方法请求设置演示或URL指定流的参数值. 请求仅应包含单个参数, 允许客户端决定某个特殊请求为何失败. 如请求包含多个参数, 所有参数可成功设置, 服务器必须只对该请求起作用. 服务器必须允许参数可重复设置成同一值, 但不让改变参数值. 注意: 媒体流传输参数必须用SETUP命令设置. 将设置传输参数限制为SETUP有利于防火墙. 将参数划分成规则排列形式, 结果有更多有意义的错误指示 |
| TEARDOWN | C->S | P,S | 要求 | TEARDOWN请求停止给定URL流发送, 释放相关资源. 如URL是此演示URL, 任何RTSP连接标识不再有效. 除非全部传输参数是连接描述定义的, SETUP请求必须在连接可再次播放前发布 |
某些防火墙设计与其他环境可能要求服务器插入RTSP方法和流数据. 由于插入将使客户端和服务器操作复杂, 并增加附加开销, 除非有必要, 应避免这样做. 插入二进制数据仅在RTSP通过TCP传输时才可使用. 流数据(如RTP包)用一个ASCII字符'ʹ封装, 后跟一个一字节通道标识, 其后是封装二进制数据的长度, 两字节整数. 流数据紧跟其后, 没有CRLF, 但包括高层协议头. 每个块包含一个高层协议数据单元.
当传输选择为RTP, RTCP信息也被服务器通过TCP连接插入. 缺省情况下, RTCP包在比RTP通道高的第一个可用通道上发送. 客户端可能在另一通道显式请求RTCP包, 这可通过指定传输头插入参数中的两个通道来做到. 当两个或更多流交叉时, 为取得同步, 需要RTCP. 而且, 这为当网络设置需要通过TCP控制连接透过RTP/RTCP提供了一条方便的途径, 可能时, 在UDP上进行传输.
### 消息头定义
消息头的定义如下表. 表格说明:
- Type:
- 类型 "g" 表示请求和响应中的通用请求头;
- 类型 "R" 表示请求头;
- 类型 "r" 表示响应头;
- 类型 "e" 表示实体头字段.
- Support:
- "req." 表示必须由接收者以特殊的方法实现; 注意, 不是所有 "req." 字段在该类型的每个请求中都会被发送. "req." 只表示客户机(支持响应头)和服务器(支持请求头)必须执行该字段.
- "opt." 表示是可选的.
- 最后一栏列出了关于头字段产生作用的方法; 其中 "entity" 针对于返回一个信息主体的所有方法. )
| Header | Type | Support | Methods |
|--------------------|------|---------|---------------------------|
| Accept | R | opt. | entity |
| Accept-Encoding | R | opt. | entity |
| Accept-Language | R | opt. | all |
| Allow | R | opt. | all |
| Authorization | R | opt. | all |
| Bandwidth | R | opt. | all |
| Blocksize | R | opt. | All but OPTIONS, TEARDOWN |
| Cache-Control | G | opt. | SETUP |
| Conference | R | opt. | SETUP |
| Connection | G | req. | all |
| Content-Base | E | opt. | entity |
| Content-Encoding | E | req. | SET_PARAMETER |
| Content-Encoding | E | req. | DESCRIBE, ANNOUNCE |
| Content-Language | E | req. | DESCRIBE, ANNOUNCE |
| Content-Length | E | req. | SET_PARAMETER, ANNOUNCE |
| Content-Length | E | req. | entity |
| Content-Location | E | opt. | entity |
| Content-Type | E | req. | SET_PARAMETER, ANNOUNCE |
| Content-Type | R | req. | entity |
| CSeq | G | req. | all |
| Date | G | opt. | all |
| Expires | E | opt. | DESCRIBE, ANNOUNCE |
| From | R | opt. | all |
| If-Modified-Since | R | opt. | DESCRIBE, SETUP |
| Last-Modified | E | opt. | entity |
| Proxy-Authenticate | | | |
| Proxy-Require | R | req. | all |
| Public | R | opt. | all |
| Range | R | opt. | PLAY, PAUSE, RECORD |
| Range | R | opt. | PLAY, PAUSE, RECORD |
| Referer | R | opt. | all |
| Require | R | req. | all |
| Retry-After | R | opt. | all |
| RTP-Info | R | req. | PLAY |
| Scale | Rr | opt. | PLAY, RECORD |
| Session | Rr | req. | All but SETUP, OPTIONS |
| Server | R | opt. | all |
| Speed | Rr | opt. | PLAY |
| Transport | Rr | req. | SETUP |
| Unsupported | R | req. | all |
| User-Agent | R | opt. | all |
| Via | G | opt. | all |
| WWW-Authenticate | R | opt. | all |
常用头解析:
| Header | Description |
|-------------------------|---------------------------------------------------------------------------------------|
| CSeq | 命令的序列号, 逐1增加 |
| Content-Length | 这个标记的存在说明后面有实体数据, 而且给出了这个数据块的大小, 单位是byte |
| X-Playlist-Gen-Id | 用来检查播放列表是否有效. 这个标记最初在客户端发送DESCRIBE命令后使用. 客户端在发送“SETUP”命令给服务器时必须回应一样的值 |
| X-Playlist-Seek-Id | 值必须和X-Playlist-Gen-Id 域的值相同, 在PLAY请求命令中使用. |
| Blocksize | 媒体包的总长度，单位是byte |
| Session | Session ID是用作客户端和服务器之间是否是正确的连接。在客户端发送SETUP命令后，服务器会在应答消息头里面发送一个session值给客户端。这算建立的一个会话. |
| X-Accept-Authentication | 允许的authentication 方法. NTLM, Digest 和 Basic 是标准的 |
| X-Broadcast-Id | 是否是实况或者是先期录制的流。0 表示先期录制，其他的值表示是实况。 |
| Range | 暂无中文释义 |
| Speed | 用来调整传输到客户端的流得速度。假如你的带宽可以接受更高速的数据传送，这个域的值可以设置大于1来加速下载数据. i.e. x1 rate |
| Server | 服务器类型和软件版本 |
| EOF | 文件结束标记，也是流的结束标记 |
| Date | 日期时间，下面举个例子：Tue, 18 Nov 2003 15:57:07 GMT |
| Bandwidth | 流需要的最大带宽，bits/秒 |
| Transport | 使用什么协议来传输数据，比如TCP或者UDP等 |
| Etag | 实体标记Entity tag，是一个分配给会话的值，就像”23180160″ |
| Supported | 支持的COM modules , 有的是可选的. |
| Content-Type | 此域用来表示命令或者应答的用意. 下面是常用的几种类型 |
| \/ | application/x-wms-Logconnectstats 这个在SET_PARAMETER命令中用到，表示将客户端的信息登记到服务器上。 |
| \/ | application/sdp 这个表示接下来数据包里面的是sdp数据，它是在服务器对DESCRIBE命令的应答包中。 |
| \/ | application/x-wms-contentdesc 表示紧跟的数据是一个内容描述对象，它设置the layout of the dialog. |
| \/ | application/vnd.ms.wms-hdr.asfv1 表示跟着一个流媒体头信息(ASF header)，可以用BASIC 或者DIGEST来解码。 |
| \/ | application/x-rtsp-packetpair 它被用来确定连接的可用带宽。 |
### 状态码
标准RTSP 消息的状态码(在应答消息的第一行表示)
| value | meaning |
|-------|-------------------------------------|
| ”100” | Continue (all 100 range) |
| “200” | OK |
| ”201” | Created |
| ”250” | Low on Storage Space |
| ”300” | Multiple Choices |
| ”301” | Moved Permanently |
| ”302” | Moved Temporarily |
| ”303” | See Other |
| ”304” | Not Modified |
| ”305” | Use Proxy |
| ”350” | Going Away |
| ”351” | Load Balancing |
| ”400” | Bad Request |
| ”401” | Unauthorized |
| ”402” | Payment Required |
| ”403” | Forbidden |
| ”404” | Not Found |
| ”405” | Method Not Allowed |
| ”406” | Not Acceptable |
| ”407” | Proxy Authentication Required |
| ”408” | Request Time-out |
| ”410” | Gone |
| ”411” | Length Required |
| ”412” | Precondition Failed |
| ”413” | Request Entity Too Large |
| ”414” | Request-URI Too Large |
| ”415” | Unsupported Media Type |
| ”451” | Parameter Not Understood |
| ”452” | reserved |
| ”453” | Not Enough Bandwidth |
| ”454” | Session Not Found |
| ”455” | Method Not Valid in This State |
| ”456” | Header Field Not Valid for Resource |
| ”457” | Invalid Range |
| ”458” | Parameter Is Read-Only |
| ”459” | Aggregate operation not allowed |
| ”460” | Only aggregate operation allowed |
| ”461” | Unsupported transport |
| ”462” | Destination unreachable |
| ”500” | Internal Server Error |
| ”501” | Not Implemented |
| ”502” | Bad Gateway |
| ”503” | Service Unavailable |
| ”504” | Gateway Time-out |
| ”505” | RTSP Version not supported |
| ”551” | Option not supported |
## rtsp中常用方法举例
本节针对上面所述的典型交互过程进行说明
OPTION
目的是得到服务器提供的可用方法:OPTIONS rtsp://192.168.20.136:5000/xxx666 RTSP/1.0
CSeq: 1 //每个消息都有序号来标记, 第一个包通常是option请求消息
User-Agent: VLC media player (LIVE555 Streaming Media v2005.11.10)
服务器的回应信息包括提供的一些方法,例如:RTSP/1.0 200 OK
Server: UServer 0.9.7_rc1
Cseq: 1 //每个回应消息的cseq数值和请求消息的cseq相对应
Public: OPTIONS, DESCRIBE, SETUP, TEARDOWN, PLAY, PAUSE, SCALE,GET_PARAMETER //服务器提供的可用的方法
DESCRIBE
C向S发起DESCRIBE请求,为了得到会话描述信息(SDP):DESCRIBE rtsp://192.168.20.136:5000/xxx666 RTSP/1.0
CSeq: 2
token:
Accept: application/sdp
User-Agent: VLC media player (LIVE555 Streaming Media v2005.11.10)
服务器回应一些对此会话的描述信息(sdp):RTSP/1.0 200 OK
Server: UServer 0.9.7_rc1
Cseq: 2
x-prev-url: rtsp://192.168.20.136:5000
x-next-url: rtsp://192.168.20.136:5000
x-Accept-Retransmit: our-retransmit
x-Accept-Dynamic-Rate: 1
Cache-Control: must-revalidate
Last-Modified: Fri, 10 Nov 2006 12:34:38 GMT
Date: Fri, 10 Nov 2006 12:34:38 GMT
Expires: Fri, 10 Nov 2006 12:34:38 GMT
Content-Base: rtsp://192.168.20.136:5000/xxx666/
Content-Length: 344
Content-Type: application/sdp
v=0 //以下都是sdp信息
o=OnewaveUServerNG 1451516402 1025358037 IN IP4 192.168.20.136
s=/xxx666
u=http:///
e=admin@
c=IN IP4 0.0.0.0
t=0 0
a=isma-compliance:1,1.0,1
a=range:npt=0-
m=video 0 RTP/AVP 96 //m表示媒体描述, 下面是对会话中视频通道的媒体描述
a=rtpmap:96 MP4V-ES/90000
a=fmtp:96 profile-level-id=245;config=000001B0F5000001B509000001000000012000C888B0E0E0FA62D089028307 a=control:trackID=0 //trackID＝0表示视频流用的是通道0
SETUP
客户端提醒服务器建立会话,并确定传输模式:SETUP rtsp://192.168.20.136:5000/xxx666/trackID=0 RTSP/1.0
CSeq: 3
Transport: RTP/AVP/TCP;unicast;interleaved=0-1
User-Agent: VLC media player (LIVE555 Streaming Media v2005.11.10)
//uri中 带有trackID＝0, 表示对该通道进行设置. Transport参数设置了传输模式, 包的结构. 接下来的数据包头部第二个字节位置就是 interleaved, 它的值是每个通道都不同的, trackID＝0的interleaved值有两个0或1, 0表示rtp包, 1表示rtcp包, 接 受端根据interleaved的值来区别是哪种数据包.
服务器回应信息:RTSP/1.0 200 OK
Server: UServer 0.9.7_rc1
Cseq: 3
Session: 6310936469860791894 //服务器回应的会话标识符
Cache-Control: no-cache
Transport: RTP/AVP/TCP;unicast;interleaved=0-1;ssrc=6B8B4567
PLAY
客户端发送播放请求:PLAY rtsp://192.168.20.136:5000/xxx666 RTSP/1.0
CSeq: 4
Session: 6310936469860791894
Range: npt=0.000- //设置播放时间的范围
User-Agent: VLC media player (LIVE555 Streaming Media v2005.11.10)
服务器回应信息:RTSP/1.0 200 OK
Server: UServer 0.9.7_rc1
Cseq: 4
Session: 6310936469860791894
Range: npt=0.000000-
RTP-Info: url=trackID=0;seq=17040;rtptime=1467265309
//seq和rtptime都是rtp包中的信息
TEARDOWN
客户端发起关闭请求:TEARDOWN rtsp://192.168.20.136:5000/xxx666 RTSP/1.0
CSeq: 5
Session: 6310936469860791894
User-Agent: VLC media player (LIVE555 Streaming Media v2005.11.10)
服务器回应:RTSP/1.0 200 OK
Server: UServer 0.9.7_rc1
Cseq: 5
Session: 6310936469860791894
Connection: Close
以上方法都是交互过程中最为常用的, 其它还有一些重要的方法如`get/set_parameter,pause,redirect`等等
## SDP协议概述
### 简介
SDP 完全是一种会话描述格式, 它不属于传输协议.
它使用不同的适当的传输协议，包括会话通知协议（SAP）、会话初始协议（SIP）、 实时流协议（RTSP）、MIME 扩展协议的电子邮件以及超文本传输协议（HTTP）。
SDP协议是也是基于文本的协议，这样就能保证协议的可扩展性比较强, 这样就使其具有广泛的应用范围。SDP 不支持会话内容或媒体编码的协商, 所以在流媒体中只用来描述媒体信息。媒体协商这一块要用RTSP来实现．
### SDP协议格式
SDP描述由许多文本行组成，文本行的格式为<类型>=<值>，<类型>是一个字母，<值>是结构化的文本串，其格式依<类型>而定。
`＜type＞=<value>[CRLF]`
sdp的格式:
```
v=<version>
o=<username> <session id> <version> <network type> <address type> <address>
s=<session name>
i=<session description>
u=<URI>
e=<email address>
p=<phone number>
c=<network type> <address type> <connection address>
b=<modifier>:<bandwidth-value>
t=<start time> <stop time>
r=<repeat interval> <active duration> <list of offsets from start-time>
z=<adjustment time> <offset> <adjustment time> <offset> ....
k=<method>
k=<method>:<encryption key>
a=<attribute>
a=<attribute>:<value>
m=<media> <port> <transport> <fmt list>
v = (协议版本)
o = (所有者/创建者和会话标识符)
s = (会话名称)
i = * (会话信息)
u = * (URI 描述)
e = * (Email 地址)
p = * (电话号码)
c = * (连接信息)
b = * (带宽信息)
z = * (时间区域调整)
k = * (加密密钥)
a = * (0 个或多个会话属性行)
时间描述:
t = (会话活动时间)
r = * (0或多次重复次数)
媒体描述:
m = (媒体名称和传输地址)
i = * (媒体标题)
c = * (连接信息 — 如果包含在会话层则该字段可选)
b = * (带宽信息)
k = * (加密密钥)
a = * (0 个或多个媒体属性行)
```
### SDP协议举例说明
SDP（Session Description Protocol）是一个用来描述多媒体会话的应用层控制协议，它是一个基于文本的协议，用于会话建立过程中的媒体类型和编码方案的协商等。
消息正文格式：
v=0 //该行指示协议的版本
o=mhandley 2890844526 2890842807 IN IP4 126.16.64.4 //o行中包含与会话所有者有关的参数
- 第一个参数表明会话发起者的名称，该参数可不填写，如填写和SIP消息中，from消息头的内容一致。
- 第二个参数为主叫方的会话标识符。
- 第三个参数为主叫方会话的版本，会话数据有改变时，版本号递增。
- 第四个参数定义了网络类型，IN表示Internet网络类型，目前仅定义该网络类型。
- 第五个参数为地址类型，目前支持IPV4和IPV6两种地址类型。
- 第六个参数为地址：表明会话发起者的IP地址，该地址为信令面的IP地址，信令PDP激活时为手机分配。
s=SDP Seminar //表明本次会话的标题，或会话的名称
i=A Seminar on the session description protocol //会话的描述
u=http://www.cs.ucl.ac.uk/staff/M.Handley/sdp.03.ps //会话的URI，通过该地址可以查阅到会话的更多内容
e=mjh@isi.edu (Mark Handley) //会话责任人的EMIAL地址
c=IN IP4 224.2.17.12/127 //C行包含为多媒体会话而建立的连接的信息，其中指出了真正的媒体流使用的IP地址
- 第一个参数为网络类型，目前仅定义INTERNET网络类型。用“IN”表示。
- 第二个参数为地址类型，目前支持两种地址类型：IPV4和IPV6。
- 第三个参数为地址，该地址为多媒体流使用的IP地址。
t=2873397496 2873404696 //表示会话的开始时间和结束时间
- 第一个参数表明会话的开始时间，数字表明从1900年1月1日00：00以来所经过的秒数。
- 第二个参数表明会话的结束时间，数字表明从1900年1月1日00：00以来所经过的秒数。
m=audio 3458 RTP/AVP 0 96 97 // m行又称媒体行，描述了发送方所支持的媒体类型等信息
- 第一个参数为媒体名称：表明支持音频类型。
- 第二个参数为端口号，表明UE在本地端口为3458上发送音频流。
- 第三个参数为传输协议，一般为RTP/AVP协议。
- 第四~七参数为所支持的四种净荷类型编号
a=rtpmap:0 PCMU //a行为媒体的属性行，以属性的名称：属性值的方式表示。
a=rtpmap:96 G726-32/8000
a=rtpmap:97 AMR-WB
格式为：a=rtpmap:<净荷类型><编码名称> * 净荷类型0固定分配给了PCMU， * 净荷类型96对应的编码方案为G.726,为动态分配的。 * 净荷类型97对应的编码方式为自适应多速率宽带编码（AMR-WB），为动态分配的。
m=video 3400 RTP/AVP 98 99 //m行又称媒体行，描述了发送方所支持的媒体类型等信息
- 第一个参数为媒体名称：表明支持视频类型。
- 第二个参数为端口号，表明UE在本地端口为3400上发送视频流。
- 第三个参数为传输协议，一般为RTP/AVP协议。
- 四、五参数给出了两种净荷类型编号
a=rtpmap:98 MPV
a=rtpmap:99 H.261
格式为：a=rtpmap:<净荷类型><编码名称> * 净荷类型98对应的编码方案为MPV,为动态分配的。 * 净荷类型97对应的编码方式为H.261，为动态分配的。
