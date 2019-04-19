# SDP协议 - 建建的博客 - CSDN博客
2017年08月29日 18:09:29[纪建](https://me.csdn.net/u013898698)阅读数：155
个人分类：[Rtsp协议](https://blog.csdn.net/u013898698/article/category/6698534)

## 1.SDP协议概述
SDP（Session Description Protocol）是一个用来描述多媒体会话的应用层控制协议，为会话通知、会话邀请和其它形式的**多媒体会话初始化等目的**提供了多媒体会话描述；它是一个基于文本的协议，这样就能保证协议的可扩展性比较强，这样就使其具有广泛的应用范围；SDP
 完全是一种**会话描述格式** ― 它不属于传输协议 ― 它只使用不同的适当的传输协议，包括会话通知协议（SAP）、会话初始协议（SIP）、[实时流协议（RTSP）、](http://www.cnblogs.com/qingquan/archive/2011/07/14/2106834.html)MIME 扩展协议的电子邮件以及超文本传输协议（HTTP）。SDP
 不支持会话内容或媒体编码的协商，所以在流媒体中只用来描述媒体信息。媒体协商这一块要用[RTSP](http://www.cnblogs.com/qingquan/archive/2011/07/14/2106834.html)来实现。
**会话目录**用于协助多媒体会议的通告，并为会话参与者传送相关设置信息。SDP 即用于将这种信息传输到接收端。在因特网组播骨干网（Mbone）中，**会话目录工具**被用于通告多媒体会议，并为参与者传送会议地址和参与者所需的会议特定工具信息，这由 SDP 完成。SDP 连接好会话后，传送足够的信息给会话参与者。SDP
 信息发送利用了会话通知协议（SAP），它周期性地组播通知数据包到已知组播地址和端口处。这些信息是 UDP 数据包，其中包含 SAP 协议头和文本有效载荷（text payload）。这里文本有效载荷指的是 SDP 会话描述。此外信息也可以通过电子邮件或 WWW （World Wide Web） 进行发送。
SDP 文本信息包括：会话名称和意图； 会话持续时间； 构成会话的媒体； 有关接收媒体的信息（地址等）。
## **2.SDP协议结构**
SDP协议的信息是文本信息，采用 UTF-8 编 码中的 ISO 10646 字符集。
SDP描述由许多文本行组成，文本行的格式为<类型>=<值>，<类型>是一个字母，<值>是结构化的文本串，其格式依<类型>而定。＜type＞=<value>[CRLF]
**SDP会话描述如下：（标注 * 符号的表示可选字段）：**
v = （协议版本） 
o = （所有者/创建者和会话标识符） 
s = （会话名称） 
i = * （会话信息） 
u = * （URI 描述） 
e = * （Email 地址） 
p = * （电话号码） 
c = * （连接信息 ― 如果包含在所有媒体中，则不需要该字段） 
b = * （带宽信息） **一个或更多时间描述（如下所示）：**
z = * （时间区域调整） 
k = * （加密密钥） 
a = * （0 个或多个会话属性行） 
0个或多个媒体描述（如下所示） **时间描述**
t = （会话活动时间） 
r = * （0或多次重复次数）**媒体描述**
m = （媒体名称和传输地址） 
i = * （媒体标题） 
c = * （连接信息 — 如果包含在会话层则该字段可选） 
b = * （带宽信息） 
k = * （加密密钥） 
a = * （0 个或多个会话属性行） 
## 三、SDP协议例子：
**消息正文格式：**
v=0                                                                                                      //该行指示协议的版本
**o=mhandley 2890844526 2890842807 IN IP4 126.16.64.4   //o行中包含与会话所有者有关的参数**
> 
l 第一个参数表明会话发起者的名称，该参数可不填写，如填写和SIP消息中，from消息头的内容一致。
l 第二个参数为主叫方的会话标识符。
l 第三个参数为主叫方会话的版本，会话数据有改变时，版本号递增。
l 第四个参数定义了网络类型，IN表示Internet网络类型，目前仅定义该网络类型。
l 第五个参数为地址类型，目前支持IPV4和IPV6两种地址类型。
l 第六个参数为地址：表明会话发起者的IP地址，该地址为信令面的IP地址，信令PDP激活时为手机分配。
s=SDP Seminar //表明本次会话的标题，或会话的名称
i=A Seminar on the session description protocol //会话的描述
u=http://www.cs.ucl.ac.uk/staff/M.Handley/sdp.03.ps //会话的URI，通过该地址可以查阅到会话的更多内容
e=mjh@isi.edu (Mark Handley) //会话责任人的EMIAL地址
**c=IN IP4 224.2.17.12/127 //C行包含为多媒体会话而建立的连接的信息，其中指出了真正的媒体流使用的IP地址**
> 
l 第一个参数为网络类型，目前仅定义INTERNET网络类型。用“IN”表示。
l 第二个参数为地址类型，目前支持两种地址类型：IPV4和IPV6。
l 第三个参数为地址，该地址为多媒体流使用的IP地址。
t=2873397496 2873404696 //表示会话的开始时间和结束时间
l 第一个参数表明会话的开始时间，数字表明从1900年1月1日00：00以来所经过的秒数。
l 第二个参数表明会话的结束时间，数字表明从1900年1月1日00：00以来所经过的秒数。
**m=audio 3458  RTP/AVP  0   96   97   // m行又称媒体行，描述了发送方所支持的媒体类型等信息**
> 
l   第一个参数为媒体名称：表明支持音频类型。
l   第二个参数为端口号，表明UE在本地端口为3458上发送音频流。
l   第三个参数为传输协议，一般为RTP/AVP协议。
l  四-七参数为所支持的四种净荷类型编号
**a=rtpmap:0   PCMU //a行为媒体的属性行，以属性的名称：属性值的方式表示。**
    格式为：a=rtpmap:<净荷类型><编码名称>
> 
l 净荷类型0固定分配给了PCMU，
l 净荷类型96对应的编码方案为G.726,为动态分配的。
l 净荷类型97对应的编码方式为自适应多速率宽带编码（AMR-WB），为动态分配的。           
a=rtpmap:96  G726-32/8000
a=rtpmap:97  AMR-WB
**m=video 3400 RTP/AVP 98  99 //m行又称媒体行，描述了发送方所支持的媒体类型等信息**
> 
l   第一个参数为媒体名称：表明支持视频类型。
l   第二个参数为端口号，表明UE在本地端口为3400上发送视频流。
l   第三个参数为传输协议，一般为RTP/AVP协议。
l   四、五参数给出了两种净荷类型编号
> 
> 
   格式为：a=rtpmap:<净荷类型><编码名称>
> 
      a=rtpmap:98  MPV
> 
      a=rtpmap:99  H.261
> 
l 净荷类型98对应的编码方案为MPV,为动态分配的。
> 
l 净荷类型97对应的编码方式为H.261，为动态分配的。
a=rtpmap:98  MPV
a=rtpmap:99  H.261
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
m=audio 0 [RTP](http://www.cnblogs.com/qingquan/archive/2011/07/28/2120440.html)/AVP 96 //做为媒体描述信息的重要组成部分描述了媒体信息的详细内容：表示session的audio是通过[RTP](http://www.cnblogs.com/qingquan/archive/2011/07/28/2120440.html)来格式传送的，其payload值为96传送的端口还没有定。
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
m=video 0 [RTP](http://www.cnblogs.com/qingquan/archive/2011/07/28/2120440.html)/AVP 97
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
[](http://blog.csdn.net/dxpqxb/article/details/18706471#)[](http://blog.csdn.net/dxpqxb/article/details/18706471#)[](http://blog.csdn.net/dxpqxb/article/details/18706471#)[](http://blog.csdn.net/dxpqxb/article/details/18706471#)[](http://blog.csdn.net/dxpqxb/article/details/18706471#)[](http://blog.csdn.net/dxpqxb/article/details/18706471#)
- 
顶
