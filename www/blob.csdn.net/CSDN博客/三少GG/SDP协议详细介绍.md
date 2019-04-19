# SDP协议详细介绍 - 三少GG - CSDN博客
2012年03月18日 11:48:08[三少GG](https://me.csdn.net/scut1135)阅读数：9341
# [SDP 协议分析](http://www.cnblogs.com/qingquan/archive/2011/08/02/2125585.html)[http://www.cnblogs.com/qingquan/archive/2011/08/02/2125585.html](http://www.cnblogs.com/qingquan/archive/2011/08/02/2125585.html)
#### 一、SDP协议介绍
SDP 完全是一种会话描述格式 ― 它不属于传输协议 ― 它只使用不同的适当的传输协议，包括会话通知协议（SAP）、会话初始协议（SIP）、[实时流协议（RTSP）、](http://www.cnblogs.com/qingquan/archive/2011/07/14/2106834.html)MIME
 扩展协议的电子邮件以及超文本传输协议（HTTP）。SDP协议是也是基于文本的协议，这样就能保证协议的可扩展性比较强，这样就使其具有广泛的应用范围。SDP 不支持会话内容或媒体编码的协商，所以在流媒体中只用来描述媒体信息。媒体协商这一块要用[RTSP](http://www.cnblogs.com/qingquan/archive/2011/07/14/2106834.html)来实现．
#### 二、SDP协议格式
SDP描述由许多文本行组成，文本行的格式为<类型>=<值>，<类型>是一个字母，<值>是结构化的文本串，其格式依<类型>而定。
＜type＞=<value>[CRLF]
常见的fields有：
![table1](http://images.cnblogs.com/cnblogs_com/qingquan/201108/201108022241526065.jpg)
![tabl2](http://images.cnblogs.com/cnblogs_com/qingquan/201108/20110802224153393.jpg)
![tabl3](http://images.cnblogs.com/cnblogs_com/qingquan/201108/201108022241541657.jpg)
#### 三、SDP协议例子：
下面是一个helix 流媒体服务器的RTSP协议中的SDP协议：
v=0 //SDP version
// o field定义的源的一些信息。其格式为：o=<username> <sess-id> <sess-version> <nettype> <addrtype> <unicast-address>
o=- 1271659412 1271659412 IN IP4 10.56.136.37 s=<No title>
i=<No author> <No copyright>  //session的信息
c=IN IP4 0.0.0.0 //connect 的信息，分别描述了：网络协议，地址的类型，连接地址。
c=IN IP4 0.0.0.0
t=0 0 //时间信息，分别表示开始的时间和结束的时间，一般在流媒体的直播的时移中见的比较多。
a=SdpplinVersion:1610641560 //描述性的信息
a=StreamCount:integer;2 //用来描述媒体流的信息，表示有两个媒体流。integer表示信息的格式为整数。
a=control:*
a=DefaultLicenseValue:integer;0 //License信息
a=FileType:string;"MPEG4" ////用来描述媒体流的信息说明当前协商的文件是mpeg4格式的文件
a=LicenseKey:string;"license.Summary.Datatypes.RealMPEG4.Enabled"
a=range:npt=0-72.080000  //用来表示媒体流的长度
m=audio 0 [RTP](http://www.cnblogs.com/qingquan/archive/2011/07/28/2120440.html)/AVP
 96 //做为媒体描述信息的重要组成部分描述了媒体信息的详细内容：表示session的audio是通过[RTP](http://www.cnblogs.com/qingquan/archive/2011/07/28/2120440.html)来格式传送的，其payload值为96传送的端口还没有定。
b=as:24 //audio 的bitrate
b=RR:1800
b=RS:600
a=control:streamid=1  //通过媒体流1来发送音频
a=range:npt=0-72.080000 //说明媒体流的长度。
a=length:npt=72.080000
a=rtpmap:96 MPEG4-GENERIC/32000/2 //rtpmap的信息，表示音频为AAC的其sample为32000
a=fmtp:96 profile-level-id=15;mode=AAC-hbr;sizelength=13;indexlength=3;indexdeltalength=3;config=1210 //config为AAC的详细格式信息
a=mimetype:string;"audio/MPEG4-GENERIC"
a=Helix-Adaptation-Support:1
a=AvgBitRate:integer;48000
a=HasOutOfOrderTS:integer;1
a=MaxBitRate:integer;48000
a=Preroll:integer;1000
a=OpaqueData:buffer;"A4CAgCIAAAAEgICAFEAVABgAAAC7gAAAu4AFgICAAhKIBoCAgAEC"
a=StreamName:string;"Audio Track"
下面是video的信息基本和audio的信息相对称，这里就不再说了。
m=video 0 [RTP](http://www.cnblogs.com/qingquan/archive/2011/07/28/2120440.html)/AVP
 97
b=as:150
b=RR:11250
b=RS:3750
a=control:streamid=2
a=range:npt=0-72.080000
a=length:npt=72.080000
a=rtpmap:97 MP4V-ES/2500
a=fmtp:97 profile-level-id=1;
a=mimetype:string;"video/MP4V-ES"
a=Helix-Adaptation-Support:1
a=AvgBitRate:integer;300000
a=HasOutOfOrderTS:integer;1
a=Height:integer;240 //影片的长度
a=MaxBitRate:integer;300000
a=MaxPacketSize:integer;1400
a=Preroll:integer;1000
a=Width:integer;320  //影片的宽度
a=OpaqueData:buffer;"AzcAAB8ELyARAbd0AAST4AAEk+AFIAAAAbDzAAABtQ7gQMDPAAABAAAAASAAhED6KFAg8KIfBgEC"
a=StreamName:string;"Video Track"
++++++++++++++++++++++++++++++++++++++++++++
## [http://hi.baidu.com/gxgaoliang/blog/item/9bde1619b15f47bd4aedbc9c.html](http://hi.baidu.com/gxgaoliang/blog/item/9bde1619b15f47bd4aedbc9c.html)
注：v,o,s,t,m为必须的,其他项为可选。  
如果SDP语法分析器不能识别某一类型(Type),则整个描述丢失;
如果”a=”的某属性值不理解,则予以丢失
整个协议区分大小写
“=”两侧不允许有空格
会话级的描述就是媒体级描述的缺省值
所有均格式为<type>=<value>
3. SDP在IP电话中的使用
SDP用于构建INVITE和200 OK响应消息的消息体,供主\被叫用户交换媒体信息.
1. 媒体流的配置
1) 主被叫的媒体描述必须完全对应:主被叫的第n个媒体流(“m=”)对应,都包含”a=rtpmap”.这样的目的是易于适应静态净荷类型到动态净荷类型的转换.
2) 如被叫不想接收主叫提出的某个媒体流则在响应中设置该媒体流的端口号为0.并且,必须返回对应的媒体流行.
2. 单播SDP值的设定
1) 对于只发媒体流,端口号无意义,应设为0.
2) 每个媒体流的净载荷类型例表应传送两个信息:能接受/发送的编译码,和用以标识这些编译码的RTP净载荷类型号.
3) 如对于某一媒体流,主/被叫没有公共的媒体格式,被叫仍然要求返回媒体流的”m=”行,端口好为0,同时,不列净载荷类型.
4) 如果所有媒体流均无公共的媒体格式,则被叫回送400响应(坏请求),并加入304警告头字段(无媒体类型)
3. 多播操作
1) 接受和发送的多播地址是相同的
2) 被叫不允许改变媒体流的只发,只收,或收/发特性
3) 如果被叫不支持多播,则回送400响应和330警告(多播不可用)
4. 延时媒体流
由于主叫可能实际上是一个和其他协议(如H.323)互同的协议的网关,与S其互同的协议要求呼叫建立后进行媒体协商.这样,主叫可以先发不带SDP的INVITE,呼叫建立后可以通过ACK或重新发一个INVITE请求修改被叫的会话描述(SDP).
5. 媒体流保持
如果要求对方进入HOLD,即暂时停止发送一个或多个媒体流,这可以用Re-INVITE,其会话描述和原来的请求或响应中的描述相同,只是,”c=”行中的保持媒体流的地址置为”0.0.0.0”,还有就是Re_INVITE中的Cseq得递增.
6. 对应于SIP中有3个实体字段:
1) Content-Type: 指明消息体类型,有两种:
i. Application/sdp:表示是SDP会话描述
ii. Text/html:表示是普通文本或HTML格式的描述
2) Content-Encoding:补充说明消息体类型,使用户可以采用压缩编码编辑消息体
3) Content-Length:给出消息体的字节数
7. SDP各type的详细解释:
协议版本 v = SDP版本目前为0,没有子版本
会话源 o = <用户名>用户在发起主机上登录名,如果主机不支持用户标识的概念,则为”-”
<会话id>一般为数字串,其分配由创建工具决定,建议用网络时间协议(NTP)时戳,以确保唯一性.
<版本>该会话公告的版本,供公告代理服务器检测同一会话的若干个公告哪个是最新公告.基本要求是会话数据修改后该版本值递增,建议用NTP时戳
<网络类型>为文本串”IN”
<地址类型>”IP4”(可为域名或点分十进制)/”IP6”(域名或压缩文本地址形式)
<地址>
会话名 s= ISO 10646字符表示的会话名
会话信息   v= ISO 10646字符表示的会话信息
URI    u= 能提供会议进一步信息的URI地址
Email地址   e= 给出会议负责人的联系信息,他不一定是创建会议公告的人
电话号码   p= 给出会议负责人的联系信息,他不一定是创建会议公告的人(国际通用形式)
连接数据   c=媒体连接数据，会话级为媒体级的摸认值
带宽    b= 给出会话或媒体所用带宽，单位为kbit/s.修饰语：CT(会议总带宽，表示所有
地点所有媒体的总带宽)，AS(应用特定最大带宽，表示一个地点单一媒体带宽)
时间描述   t= 见上
   r= 见上
时区调整   z= 见上
加密密钥    k=已定义的方法有
                 k=clear:<加密密钥>密钥没有变换
                 k=base64:<编码密钥>已编码，因为它含有SDP禁用的字符
                 k=uri:<获得密钥的URI>
                 k=prompt。SDP没有提供密钥但该会话或媒体流是要求加密的。
属性        a=一个m=行可有多个a=行，SDP建议扩展如下：(具体见[1].Page419)
会话级： a=cat:<类别>//给出点分层次式会话分类号,供接收方筛选会话
               a=keywds:<关键词>//供接收方筛选会话
               a=tool:<工具名和版本号>//创建会话描述的工具名和版本号
                a=recvonly/sendrecv/sendonly//收发模式
               a=type:<会议类型>//有:广播,聚会,主席主持,测试,H.323
               a=charset:<字符集>//显示会话名和信息数据的字符集
               a=sdplang:<语言标记>//描述所有语言
              a=lang:<语言标记>//会话描述的缺省语言或媒体描述的语言
               a=framerate:<帧速率>//单位:帧/秒
              a=quality:<质量>//视频的建议质量(10/5/0)
               a=fmtp:<格式>< 格式特定参数>//定义指定格式的附加参数
媒体级（一般在m后）：
             a=ptime:<分组时间>//媒体分组的时长(单位:秒)
             a=recvonly/sendrecv/sendonly//收发模式
             a=orient:<白板方向>//指明白板在屏莫上的方向
             a=sdplang:<语言标记>//描述所有语言
             a=lang:<语言标记>//会话描述的缺省语言或媒体描述的语言
媒体描述     
           m= <媒体>有5种类型：音频/视频/应用(如白板信息)/数据(不向用户显示的)/控制
<端口>媒体流发往传输层的端口。取决于c=行规定的网络类型和接下来的传送层协议：对UDP为1024-65535；对分层编码应用（c=行没有多播地址），要给出多播端口数，如：m=video 49170/2 RTP/AVP 31（表示：端口49170和49171为第一对RTP/RTCP端口，49172和49173为第二对的端口）。
<传送层协议>与c=行的地址类型有关。对大多的媒体在RTP/UDP上传送，定义2种：RTP/AVP：IETF RTP协议，音/视频应用文档。在UDP上传诵。
Udp：UDP协议。
<格式列表>对音/视频，就是音/视频应用文档中规定媒体净荷类型。列表中都
有可能用，但第一个为缺省值，分为静态绑定和动态绑定：静态绑定即使媒体编码方式有净荷类型号完全确定，动态绑定则媒体编码方式
如时钟频率，音频信道数等）没有完全确定，需要进一步的属性说明。分别举例如下：
Alaw的PCM编码单信道Audio，其净荷类型号为8，把它发往UDP端口49232，则：m=audio 49232 RTP/AVP 8 
16bit线性编码，双声道立体声，抽样速率16kHz，其动态净荷类型号98，则：m=audio 49232 RTP/AVP 98
a=rtpmap:98 L16/16000/2
说明：1）a=rtpmap:<净荷类型号><编码名>/<时钟速率>[/<编码参数>]
               对音频，编码参数为音频信道数；对视频没有定义
          2）SDP允许rtpmap规定实验性编码格式，但编码名必须以X-起，表示此格式还没正式登记。
## 浅议SDP（会话描述协议）
[http://blog.sina.com.cn/s/blog_8337d0c50100wt1e.html](http://blog.sina.com.cn/s/blog_8337d0c50100wt1e.html)
|标签：### [sdp](http://uni.sina.com.cn/c.php?t=blog&k=sdp&ts=bpost&stype=tag)### [杂谈](http://uni.sina.com.cn/c.php?t=blog&k=%D4%D3%CC%B8&ts=bpost&stype=tag)|分类：[SIP](http://blog.sina.com.cn/s/articlelist_2201473221_4_1.html)|
因为最近常常使用到SDP（会话描述协议Session Description Protocol）写了一些SDP的文本，在linux里使用例如"ffplay test.sdp"来播放媒体流，今天想简单谈谈SDP，做了一个小小的总结和分析，希望对大家有帮助。
一、SDP协议介绍
    SDP是会话描述协议的缩写，是描述流媒体初始化参数的格式，由IETF作为RFC 4566颁布。流媒体是指在传输过程中看到或听到的内容，SDP包通常包括以下信息：
（1）会话信息
      　　· 会话名和目的。
      　　· 会话活动时间。
      　　由于参与会话的资源是受限制的，因此包括以下附加信息是非常有用的。
      　　· 会话使用的带宽信息。
      　　· 会话负责人的联系信息。
（2）媒体信息
      　　· 媒体类型，例如视频和音频。
      　　· 传输协议，例如RTP/UDP/IP和H.320。
      　　· 媒体格式，例如H.261视频和MPEG视频。
      　　· 多播地址和媒体传输端口（IP多播会话）。
      　　· 用于联系地址的媒体和传输端口的远端地址（IP单播会话）。
      　　SDP描述由许多文本行组成，文本行的格式为<类型>=<值>，<类型>是一个字母，<值>是结构化的文本串，其格式依<类型>而定。
二、SDP协议格式
SDP格式:
       Session description
          v=   (protocol version)
          o=   (owner/creator and session identifier)
          s=   (session name)
          i=* (session information)
          u=* (URI of description)
          e=* (email address)
          p=* (phone number)
          c=* (connection information - not required if included in all media)
          b=* (zero or more bandwidth information lines)
          One or more time descriptions ("t=" and "r=" lines, see below)
          z=* (time zone adjustments)
          k=* (encryption key)
          a=* (zero or more session attribute lines)
          Zero or more media descriptions
       Time description
          t=   (time the session is active)
          r=* (zero or more repeat times)
       Media description, if present
          m=   (media name and transport address)
          i=* (media title)
          c=* (connection information - optional if included at
               session-level)
          b=* (zero or more bandwidth information lines)
          k=* (encryption key)
          a=* (zero or more media attribute lines)
以上带"*"号的是可选的,其余的是必须的。一般顺序也按照上面的顺序来排列。
a=*是sdp协议扩展属性定义，除上面以外的，分解时其它的都可以扔掉。
a=charset属性指定协议使用的字符集。一般的是ISO-10646。
示例：
v=<username> <sess-id> <sess-version> <nettype> <addrtype> <unicast-address>
    其中：nettype是IN,代表internet,addrtype是IP4或IP6。unicast-address任务创建计算机的地址。
    整个这个属性，是唯一表示一个任务。
[e=123@126.com](mailto:e=123@126.com) 或 p=+1 616 555-6011
对于一个任务只能两者之中的一个，表示会议控制者的联系方式。邮件地址可以是[email]j.doe@example.com[/email] (Jane Doe)形式，括号里面的是描述联系人的名称，或者Jane Doe <[email]j.doe@example.com[/email]>，前面的是联系人的名称。
c=<nettype> <addrtype> <connection-address>
这个连接数据，可以是传话级别的连接数据，或者是单独一个媒体数据的连接数据。在是多播时，connection-address就该是一个多播组地址，当是单播时，connection-address就该是一个单播地址。对于addrtype是IP4的情况下，connection-address不仅包含IP地址，并且还要包含a time to live value(TTL 0-255)，如：c=IN IP4 224.2.36.42/128，IP6没有这个TTL值。还允许象这样的<base multicast address>[/<ttl>]/<number
 of addresses>格式的connection-address。如：c=IN IP4 224.2.1.1/127/3等同于包含c=IN IP4 224.2.1.1/127, c=IN IP4 224.2.1.2/127, c=IN IP4 224.2.1.3/127三行内容。
b=<bwtype>:<bandwidth> bwtype可以是CT或AS，CT方式是设置整个会议的带宽，AS是设置单个会话的带宽。缺省带宽是千比特每秒。
t=<start-time> <stop-time>，这个可以有行，指定多个不规则时间段，如果是规则的时间段，则r=属性可以使用。start-time和stop-time都遵从NTP(Network Time Protocol),是以秒为单位，自从1900以来的时间。要转换为UNIX时间，减去2208988800。如果stop-time设置为0,则会话没有时间限制。如果start-time也设置为0，则会话被认为是永久的。
r=<repeat-interval> <active duration> <offsets from start-time>重复次数在时间表示里面可以如下表示：
       d - days (86400 seconds)
       h - hours (3600 seconds)
       m - minutes (60 seconds)
       s - seconds (allowed for completeness)
z=<adjustment time> <offset> <adjustment time> <offset> ....
k=<method>
k=<method>:<encryption key>
a=<attribute>
a=<attribute>:<value>
m=<media> <port> <proto> <fmt> ...
m=<media> <port>/<number of ports> <proto> <fmt> ...
其中：<media>可以是，"audio","video", "text", "application" and "message"。<port>是媒体传送的端口号，它依赖于c=和<proto>。<proto> 可以是，udp，RTP/AVP和RTP/SAVP。
a=cat:<category>分类，根据分类接收者隔离相应的会话
a=keywds:<keywords>关键字，根据关键字隔离相应的会话
a=tool:<name and version of tool>创建任务描述的工具的名称及版本号
a=ptime:<packet time>在一个包里面的以毫秒为单位的媒体长度
a=maxptime:<maximum packet time>以毫秒为单位，能够压缩进一个包的媒体量。
a=rtpmap:<payload type> <encoding name>/<clock rate> [/<encoding    parameters>]
a=recvonly
a=sendrecv
a=sendonly
a=inactive，
a=orient:<orientation>其可能的值，"portrait", "landscape" and "seascape" 。
a=type:<conference type>,建议值是，"broadcast", "meeting", "moderated", "test" and "H332"。
a=charset:<character set>
a=sdplang:<language tag>指定会话或者是媒体级别使用的语言
a=framerate:<frame rate>设置最大视频帧速率
a=quality:<quality>值是0-10
a=fmtp:<format> <format specific parameters>
在SIP协议的包含的内容是SDP时，应该把Content-Type设置成application/sdp。
SDP是描述的是流媒体的初始化参数，IETF对其的描述可以在RFC 2327找到，SDP是一个纯文本文档，后缀为.sdp，它的基本内容包括：
# 会话信息：
　　　* 会话名和目的；
　　　* 会话时间；
　　　* 会话使用的带宽；
　　　* 会话的用户信息；
# 媒体信息：
　　　* 媒体类型，例如：视频或音频；
　　　* 传输协议，例如：RTP/UDP/IP；
　　　* 媒体格式，例如：H.263视频或者MPEG视频；
　　　* 多播地址和媒体传输端口（IP多播会话）；
　　　* IP单播会话的联系地址和媒体传输端口
举一个例子来进行分析（live555的testMPEG1or2VideoStreamer里附加的SDP文本）：
----------------------------------------------------------------------------------------------
v=0
o=- 49451 3 IN IP4 127.0.0.1
s=Test MPEG Video session
i=Parameters for the session streamed by "testMPEG1or2VideoStreamer"
t=0 0
a=tool:testMPEG1or2VideoStreamer
a=type:broadcast
m=video 1234 RTP/AVP 32
c=IN IP4 239.255.42.42/127
----------------------------------------------------------------------------------------------
可以发现SDP会话描述由许多文本行组成，它的格式为“类型＝值”。其中v，o，s等等代表了是类型。
第1行v代表了协议版本，例子中为0。
第2行o代表所有者/创建者和会话标识符。
第3行s代表会话名称，例子中为Test MPEG Video session，用户可以自己填写。
第4行t代表会话活动时间。
第5行和第6行a代表会话属性行，可写0个或多个。
第7行m代表代表媒体信息；video代表是视频流；1234代表UDP端口号是1234；RTP/AVP指媒体传输协议使用RTP/AVP；32代表媒体格式使用MPV并且使用90KHz的时钟。关于RTP/AVP可以在RFC 3551 RTP A/V Profile July 2003找到。以下是部分截取：
PT encoding media type clock rate
name (Hz)24 unassigned V
25 CelB V 90,000
26 JPEG V 90,000
27 unassigned V
28 nv V 90,000
29 unassigned V
30 unassigned V
31 H261 V 90,000
32 MPV V 90,000 （这就是例子中的RTP/AVP类型）
33 MP2T AV 90,000
34 H263 V 90,000
35-71 unassigned ?
72-76 reserved N/A N/A
77-95 unassigned ?
96-127 dynamic ?
dyn H263-1998 V 90,000
第8行c代表连接信息。
当然了还可以加上更多的信息描述，例如b=AS:104857，b代表了带宽信息。
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
SIP SAP及SDP协议组合应用的研究 
　　摘 要 SIP、SAP、SDP是NGN与3Tnet中涉及的重要协议。本文在介绍与分析SIP 
、SAP、SDP协议的基础上，给出了一个基于三种协议组合而实现的多媒体会议应用 
实例。 
　　关键词 SIP SAP SDP 
　　1 引 言 
　　SIP（Session Initiation Protocol，会话初始协议）、SAP（Session Annou 
ncement Protocol，会话通告协议）、SDP（Session Description Protocol，会话 
描述协议）是三个与会话（Session）有关的既有联系又有区别的RFC协议。在本文 
中，我们将对这三个协议做简单分析并给出一个利用这三个协议实现的具有一定实 
用价值的多媒体应用。 
　　2 协议分析 
　　2.1 SIP协议介绍与分析 
　　会话初始协议（SIP）是一信令协议用于初始、管理和终止分组网络中的语音和 
视频会话，具体地说就是用来生成、修改和终结一个或多个参与者之间的会话。SI 
P是互联网工程任务组（IETF）多媒体数据和控制体系结构的一个组成部分，因此它 
与IETF的许多其他协议都有联系，例如RTP（实时传输协议）和本文提到的SAP、SD 
P协议。 
　　SIP主要提供了与会话建立和终结相关的五个方面功能，它们是： 
　　（1）用户定位：用于通信的终端系统的决定； 
　　（2）用户可用性：被呼叫方参与通信的意愿的决定； 
　　（3）用户能力：使用的媒体和媒体参数的决定； 
　　（4）会话建立：“振铃”，呼叫和被呼叫方会话参数的建立； 
　　（5）会话管理：包括转移和终结会话，修改会话参数，以及调用业务等。 
　　SIP网络由四种类型的逻辑SIP实体组成。每一实体具有特定的功能，并且作为 
客户机（初始请求），或作为服务器（响应请求），或作为两者的结合参与到SIP通 
信中。一个“物理设备”能够具有多于一个逻辑SIP实体的功能性。例如，作为代理 
服务器的网络服务器可同时具备注册服务器的功能。 
　　下面是四种功能实体： 
　　（1）用户代理 
　　在SIP中，用户代理（UA）是端点实体。用户代理通过交换请求和响应初始和终 
止会话。UA作为一应用程序，它包含用户代理客户机和用户代理服务器，如下： 
　　· 用户代理客户机（UAC）：客户机应用程序，它初始SIP请求。 
　　· 用户代理服务器（UAS）：服务器应用程序，当接收到SIP请求时它联系用户 
并且代表用户返回一响应。 
　　在SIP网络中具有UA功能的设备是：工作站，IP电话，电话网关，呼叫代理，自 
动应答服务。 
　　（2）代理服务器 
　　代理服务器是同时作为服务器和客户机的中间实体，其目的是代表其他客户机 
生成请求。请求被内部处理或可能在翻译之后将其传递到其他服务器。如果需要， 
代理在转发之前可解释和重写请求消息。 
　　（3）重定向服务器 
　　重定向服务器接受SIP请求，并将被呼叫方的SIP地址映射成零个（如果没有可 
知地址）或更多的新地址并且将它们返回客户机。不像代理服务器，重定向服务器 
不传递请求到其他服务器。 
　　（4）注册服务器 
　　注册服务器是接受REGISTER请求的服务器，其目的是根据用户在请求中规定的 
联系信息更新位置数据库。 
　　SIP通过E-mail形式的地址来标明用户地址。每一用户通过一等级化的URL来标 
识，它通过诸如用户电话号码或主机名等元素来构造（例如：SIP：user@company. 
com）。因为它与E-mail地址的相似性，SIP URLs容易与用户的E-mail地址关联。 
　　SIP有两种类型的消息，它们是： 
　　（1）请求：从客户机发到服务器的消息。 
　　（2）响应：从服务器发到客户机的消息。 
　　其中请求消息包括： 
　　· INVITE：初始呼叫，改变所以参数（re-INVITE）。 
　　· ACK：确认INVITE的最终应答。 
　　· BYE：终止呼叫。 
　　· CANCEL：撤消搜索和振玲。 
　　· OPTIONS：查询另一方能力。 
　　· REGISTER：注册位置服务。 
　　· INFO：发送会话中信息而不改变会话状态。 
　　· PRACK：与ACK作用相同，但是用于临时响应。 
　　· SUBSCRIBE：该方法用来向远端端点预订其状态变化的通知。 
　　· NOTIFY：该方法发送消息以通知预订者它所预定的状态的变化。 
　　· UPDATE：允许客户更新一个会话的参数而不影响该会话的当前状态。 
　　· MESSAGE：通过在其请求体中承载即时消息内容实现即时消息。 
　　· REFER：其功能是指示接受方通过使用在请求中提供的联系地址信息联系第 
三方。 
　　响应消息包含数字响应代码。SIP响应代码集部分基于HTTP响应代码。有两种类 
型的响应，它们是： 
　　· 临时响应（1XX）：临时响应被服务器用来指示进程，但是不终结SIP事物。 
　　· 最终响应（2XX，3XX，4XX，5XX，6XX）：最终响应终止SIP事物。 
　　每条SIP消息由以下三部分组成： 
　　（1）起始行（Start Line）：每个SIP消息由起始行开始。起始行传达消息类 
型（在请求中是方法类型，在响应中是响应代码）与协议版本。起始行可以是一请 
求行（请求）或状态行（响应）。 
　　（2）SIP头：用来传递消息属性和修改消息意义。它们在语法和语义上与HTTP 
头域相同（实际上有些头就是借自HTTP），并且总是保持格式：<名字>:<值>。 
　　（3）消息体：用于描述被初始的会话（例如，在多媒体会话中包括音频和视频 
编码类型，采样率等）。消息体能够显示在请求与响应中。SIP清晰区别了在SIP起 
始行和头中传递的信令信息与在SIP范围之外的会话描述信息。可能的体类型就包括 
本文将要描述的SDP会话描述协议。 
　　2.2 SAP协议介绍与分析 
　　SAP的全称是会话通告协议，其目的是为了通知一个多播的多媒体会议或其他多 
播会话而将相关的会话建立信息发送给所期望的会议参与者。SAP协议本身并不建立 
会话，它只是将建立会话所必要的信息，例如所采取的视频或音频编码方式通知给 
其他在一个多播组内的参与者，当参与者接收到该通知数据包后就可以启动相应的 
工具并设置正确的参数向该会议的发起者建立会话了（建立会话可以使用SIP协议） 
。 
　　通知的发起者并不知道各参与者是否收到了会话通知，也就是说每个参与者并 
不向通知发起者回复“我收到了通知”的确认；因此，通知发起者只能够通过周期 
性地发送这个会话通知从而最大可能地使参与者收到通知。 
　　SAP并不是向每个参与者一一发通知数据包，它是通过多播的机制（multicast 
）向一个已知的多播地址和端口一次性发送一个通知数据包，该多播组内的成员如 
果工作正常的化就会收到该通知数据包。因此，为了使会议的参与者都能够接收到 
通知，就要确保其参加到该多播组内。　　一个通知数据报除了可以通知某会话将 
要发起外，还可以通知该会话取消了或该会话的某些通信参数已被修改了。当然， 
这需要相应机制使这几个通知都是针对同一会话的。 
　　那么SAP如何描述会话的相关信息，这就需要借助SDP协议了。在SAP数据包的p 
ayload字段中一般情况下填充的就是SDP数据，它描述了建立会话所必要的基本信息 
。 
　　SDP将在1.3节中介绍。 
　　2.3 SDP协议介绍与分析 
　　上面介绍的两个协议都用到了SDP，实际上SDP就是用来描述多媒体会话通告， 
多媒体会话邀请和其他形式的多媒体会话初始化的协议。SDP包通常包括以下信息： 
　　（1）会话信息 
　　· 会话名和目的。 
　　· 会话活动时间。 
　　由于参与会话的资源是受限制的，因此包括以下附加信息是非常有用的。 
　　· 会话使用的带宽信息。 
　　· 会话负责人的联系信息。 
　　（2）媒体信息 
　　· 媒体类型，例如视频和音频。 
　　· 传输协议，例如RTP/UDP/IP和H.320。 
　　· 媒体格式，例如H.261视频和MPEG视频。 
　　· 多播地址和媒体传输端口（IP多播会话）。 
　　· 用于联系地址的媒体和传输端口的远端地址（IP单播会话）。 
　　SDP描述由许多文本行组成，文本行的格式为<类型>=<值>，<类型>是一个字母 
，<值>是结构化的文本串，其格式依<类型>而定。 
　　3 SAP、SIP、SDP组合应用实例 
　　在这个应用实例中，我们设计了这样一种场景：某公司领导需要与本公司各部 
门负责人召开电视会议。他使用的多媒体会议系统采用了本文所描述的这种SAP、S 
IP、SDP组合应用技术。该公司领导首先通过一台具有Web浏览功能的客户机登陆会 
议服务器，注册一个会议。在注册的同时，他就将各部门负责人纳入了本次会议的 
参与者之列（当然，还需要该公司领导和各部门负责人的客户机在同一个多播组内 
）。然后，他在页面上点击一个“会议通知”按钮，会议服务器就通过SAP协议向本 
次会议的各参与者（包括公司领导和部门负责人）所在客户机发送召开会议的通知 
。此时，若各参与者所在客户机是正常的连接到网络上并且SAP通知接收器正常工作 
的话，它就会以声音或图形的方式通知该参与者，同时准备好本客户机参加此次会 
议的客户端程序。当各参与者发现会议通知后，他在已经准备好的会议客户端程序 
上点击一个“参加会议”的按钮，客户端就通过SIP协议将该客户机轻松地加入到会 
议之中。当参与者到齐后，在会议主席（公司领导）的控制下，此次会议就可以正 
式开始了。 
　　各参与者客户机上的SAP通知接收器接收到会议通知后，它会尽可能地调用客户 
机上的各种资源以满足本次会议的要求，如果实在不能够满足的话，它也可以以书 
面形式通知参与者本机所缺的资源。在整个会议的SAP、SIP交互过程中都用到了SD 
P协议来描述会话和媒体信息。简要流程如附图所示。 
　　4 结束语 
　　SAP、SIP、SDP都不是孤立的协议，只有在相互组合与协调并且与其他协议配合 
的情况下，才能够发挥它们应有的强大作用。本文给出了SAP、SIP、SDP组合应用的 
一个实例，这样的应用其实还很多，例如将本文的技术稍加改动也可用于楼宇的多 
点视频自动监控系统中。随着上述协议的进一步发展和普及，我们坚信SAP、SIP、 
SDP将会广泛地应用于NGN（下一代网络）、3Tnet（高性能宽带信息网）等宽带多媒 
体应用中。(强磊，硕士，中国电信股份有限公司研究院技术部，叶华，中国电信股 
份有限公司研究院技术部副部长 摘自《中国数据通信》） 
+++++++++++++++++++++++++++++++
假设主叫Alice 在她的提供中包含下列SDP 描述。其中包含一个双向音频流和两个双向视频流（视频流分别使用编解码H.261(载荷类型31)和MPEG （载荷类型32 。
v=0
o=alice 2890844526 2890844526 IN IP4 host.anywhere.com （用户名，会话id，版本，。。。。）
s=
c=IN IP4 host.anywhere.com
t=0 0 （起始时间，终止时间）
m=audio 49170 RTP/AVP 0 （净荷类型是0 ，发往UDP协议的49170端口）
a=rtpmap:0 PCMU/8000 （双声道 净荷类型0 时钟频率8000每秒，RTP类型编码）
m=video 51372 RTP/AVP 31
a=rtpmap:31 H261/90000
m=video 53000 RTP/AVP 32
a=rtpmap:32 MPV/90000
--------------------------------------------------------------------------------------------------
被叫Bob 不希望接收或发送第一个视频流，那么他将在应答中返回如下SDP：
v=0
o=bob 2890844730 2890844730 IN IP4 host.example.com
s=
c=IN IP4 host.example.com
t=0 0
m=audio 49920 RTP/AVP 0
a=rtpmap:0 PCMU/8000
m=video 0 RTP/AVP 31 （如果不希望接收其中的一个流，就在返回中把他的端口设置为0，其他的媒体流必须按原来返回）
m=video 53000 RTP/AVP 32
a=rtpmap:32 MPV/90000
----------------------------------------------------------------------------------------------------
之后的某个时刻，Bob 想改变音频流的接收端口（从49920 改为65422）,同时增加另外一个只收音频流，并使用RTP 载荷类型，那么Bob 将在提供中包含如下SDP：
v=0
o=bob 2890844730 2890844731 IN IP4 host.example.com （会话号要自动加一）
s=
c=IN IP4 host.example.com
t=0 0
m=audio 65422 RTP/AVP 0 （把49920改为65422端口）
a=rtpmap:0 PCMU/8000
m=video 0 RTP/AVP 31
m=video 53000 RTP/AVP 32
a=rtpmap:32 MPV/90000
m=audio 51434 RTP/AVP 110 （增加只收的音频流，载荷类型为RTP）
a=rtpmap:110 telephone-events/8000
a=recvonly （只收模式）
---------------------------------------------------------------------------------------------------------
Alice 接收增加的媒体流，将产生如下应答：
v=0
o=alice 2890844526 2890844527 IN IP4 host.anywhere.com （会话号要自动加一）
s=
c=IN IP4 host.anywhere.com 
t=0 0 
m=audio 49170 RTP/AVP 0
a=rtpmap:0 PCMU/8000
m=video 0 RTP/AVP 31
a=rtpmap:31 H261/90000
m=video 53000 RTP/AVP 32
a=rtpmap:32 MPV/90000
m=audio 53122 RTP/AVP 110
a=rtpmap:110 telephone-events/8000
a=sendonly （只发模式）
--------------------------------------------------------
