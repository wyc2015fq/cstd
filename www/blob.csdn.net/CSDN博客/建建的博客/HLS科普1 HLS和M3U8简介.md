# HLS科普1 HLS和M3U8简介 - 建建的博客 - CSDN博客
2018年02月02日 12:10:47[纪建](https://me.csdn.net/u013898698)阅读数：272
 HLS(HTTP Live Streaming)是苹果公司针对iPhone、iPod、iTouch和iPad等移动设备而开发的基于HTTP协议的流媒体解决方案。在 HLS 技术中 Web 服务器向客户端提供接近实时的音视频流。但在使用的过程中是使用的标准的 HTTP 协议，所以这时，只要使用 HLS 的技术，就能在普通的 HTTP 的应用上直接提供点播和直播。在App Store中的视频相关的应用，基本都是应用的此种技术。该技术基本原理是将视频文件或视频流切分成小片(ts)并建立索引文件(m3u8)。支持的视频流编码为H.264，音频流编码为AAC。
Apple的动态码率自适应技术。主要用于PC和Apple终端的音视频服务。包括一个m3u(8)的索引文件，TS媒体分片文件和key加密串文件。
常用的流媒体协议主要有 HTTP 渐进下载和基于 RTSP/RTP 的实时流媒体协议，这二种基本是完全不同的东西，目前比较方便又好用的是用 HTTP 渐进下载方法。在这个中 apple 公司的 HTTP Live Streaming 是这个方面的代表。它最初是苹果公司针对iPhone、iPod、iTouch和iPad等移动设备而开发的流.现在见到在桌面也有很多应用了，HTML5 是直接支持这个。
但是HLS协议的小切片方式会生成大量的文件，存储或处理这些文件会造成大量资源浪费。如果要实现数天的时移，索引量将会是个巨额数字，并明显影响请求速度。因此，HLS协议对存储I/O要求相当苛刻。对此，也有公司提出了非常好的解决方案。
新型点播服务器系统，独创了内存缓存数据实时切片技术，颠覆了这种传统实现方法，从根本上解决了大量切片的碎片问题，使得单台服务器的切片与打包能力不再是瓶颈。其基本原理如下：
不将TS切片文件存到磁盘，而是存在内存当中，这种技术使得服务器的磁盘上面不再会有“数以吨计”的文件碎片，极大减少了磁盘的I/O次数，延长了服务器磁盘的使用寿命，极大提高了服务器运行的稳定性。同时，由于使用这种技术，使得终端请求数据时直接从服务器的内存中获取，极大提高了对终端数据请求的反应速度，优化了视频观看体验。
**简单归纳起来就是：首先将一个完整视频分成多个TS视频文件，用户下载m3u8文件，通过m3u8文件的索引地址  播放具体的每个小段视频**
**1.   M3U8文件概念**
M3U8文件是指UTF-8编码格式的M3U文件。M3U文件是记录了一个索引纯文本文件，打开它时播放软件并不是播放它，而是根据它的索引找到对应的音视频文件的网络地址进行在线播放。
**2.   M3U8文件举例**
例如使用云图TV点播直播节目时，发送请求：
http://121.199.63.236:7613/m3u8/cckw1/szws.m3u8?from=bab&fun=yes&chk=y&chunk=xax&ppw=yuntutv&auth=yuntutvyuntutvyuntutv&auth=yuntutvyuntutvyuntutv&nwtime=1406515232&sign=033d5483609e6bc87987fc7d2f30a024
返回M3U8文件，文件内容如下
**[cpp]**[view plain](http://blog.csdn.net/cabbage2008/article/details/50522190#)[copy](http://blog.csdn.net/cabbage2008/article/details/50522190#)
- #EXTM3U
- #EXT-X-MEDIA-SEQUENCE:140651513
- #EXT-X-TARGETDURATION:10
- #EXTINF:8,
- http://vapp1.fw.live.cntv.cn/cache/289_/seg0/index140651513[140651513].ts
- #EXTINF:9,
- http://vapp1.fw.live.cntv.cn/cache/289_/seg0/index140651514[140651514].ts
- #EXTINF:11,
- http://vapp1.fw.live.cntv.cn/cache/289_/seg0/index140651515[140651515].ts
- #EXTINF:10,
- http://vapp1.fw.live.cntv.cn/cache/289_/seg0/index140651516[140651516].ts
- #EXTINF:12,
- http://vapp1.fw.live.cntv.cn/cache/289_/seg0/index140651517[140651517].ts
- #EXTINF:8,
- http://vapp1.fw.live.cntv.cn/cache/289_/seg0/index140651518[140651518].ts
- #EXTINF:12,
- http://vapp1.fw.live.cntv.cn/cache/289_/seg0/index140651519[140651519].ts
- #EXTINF:8,
- http://vapp1.fw.live.cntv.cn/cache/289_/seg0/index140651520[140651520].ts
- #EXTINF:9,
- http://vapp1.fw.live.cntv.cn/cache/289_/seg0/index140651521[140651521].ts
- #EXTINF:13,
- http://vapp1.fw.live.cntv.cn/cache/289_/seg0/index140651522[140651522].ts
然后根据索引的中链接请求下载音视频并进行播放
**3.   M3U文件标签及属性说明**
M3U文件中可以包含多个tag，每个tag的功能和属性如下：
**#EXTM3U**
每个M3U文件第一行必须是这个tag，请标示作用
**#EXT-X-MEDIA-SEQUENCE:140651513 **
每一个media URI 在 PlayList中只有唯一的序号，相邻之间序号+1, 一个media URI并不是必须要包含的，如果没有，默认为0
**#EXTINF:**
 duration 指定每个媒体段(ts)的持续时间（秒），仅对其后面的URI有效，title是下载资源的url
**#EXT-X-TARGETDURATION**
指定最大的媒体段时间长（秒）。所以#EXTINF中指定的时间长度必须小于或是等于这个最大值。这个tag在整个PlayList文件中只能出现一 次（在嵌套的情况下，一般有真正ts url的m3u8才会出现该tag）
**#EXT-X-KEY**
表示怎么对media segments进行解码。其作用范围是下次该tag出现前的所有media URI，属性为NONE 或者 AES-128。NONE表示 URI以及IV（Initialization Vector）属性必须不存在， AES-128(Advanced EncryptionStandard)表示URI必须存在，IV可以不存在。
  对于AES-128的情况，keytag和URI属性共同表示了一个key文件，通过URI可以获得这个key，如果没有IV（Initialization Vector）,则使用序列号作为IV进行编解码，将序列号的高位赋到16个字节的buffer中，左边补0；如果有IV，则将改值当成16个字节的16进制数。
**#EXT-X-PROGRAM-DATE-TIME**
将一个绝对时间或是日期和一个媒体段中的第一个sample相关联，只对下一个meida URI有效，格式如#EXT-X-PROGRAM-DATE-TIME:
For example: #EXT-X-PROGRAM-DATE-TIME:2010-02-19T14:54:23.031+08:00
**#EXT-X-ALLOW-CACHE**
是否允许做cache，这个可以在PlayList文件中任意地方出现，并且最多出现一次，作用效果是所有的媒体段。格式如下：#EXT-X-ALLOW-CACHE:
**#EXT-X-PLAYLIST-TYPE**
提供关于PlayList的可变性的信息， 这个对整个PlayList文件有效，是可选的，格式如下：#EXT-X-PLAYLIST-TYPE:：如果是VOD，则服务器不能改变PlayList 文件；如果是EVENT，则服务器不能改变或是删除PlayList文件中的任何部分，但是可以向该文件中增加新的一行内容。
**#EXT-X-ENDLIST**
表示PlayList的末尾了，它可以在PlayList中任意位置出现，但是只能出现一个，格式如下：#EXT-X-ENDLIST
**#EXT-X-MEDIA**
被用来在PlayList中表示相同内容的不用语种/译文的版本，比如可以通过使用3个这种tag表示3中不用语音的音频，或者用2个这个tag表示不同角度的video在PlayLists中。这个标签是独立存在的，属性包含：
  URI：如果没有，则表示这个tag描述的可选择版本在主PlayList的EXT-X-STREAM-INF中存在;
  TYPE:AUDIO and VIDEO;
  GROUP-ID:具有相同ID的MEDIAtag，组成一组样式；
  LANGUAGE：确定使用的主要语言
  NAME：人类可读的语言的翻译
  DEFAULT：YES或是NO，默认是No，如果是YES，则客户端会以这种选项来播放，除非用户自己进行选择。
  AUTOSELECT：YES或是NO，默认是No，如果是YES，则客户端会根据当前播放环境来进行选择（用户没有根据自己偏好进行选择的前提下）。
**#EXT-X-STREAM-INF**
指定一个包含多媒体信息的 media URI 作为PlayList，一般做M3U8的嵌套使用，它只对紧跟后面的URI有效，格式如下：#EXT-X-STREAM-INF:有以下属性：
BANDWIDTH：带宽，必须有。
PROGRAM-ID：该值是一个十进制整数，惟一地标识一个在PlayList文件范围内的特定的描述。一个PlayList 文件中可能包含多个有相同ID的此tag。
CODECS：不是必须的。
RESOLUTION：分辨率。
AUDIO：这个值必须和AUDIO类别的“EXT-X-MEDIA”标签中“GROUP-ID”属性值相匹配。
VIDEO：同上
HLS历史
HLS草案的第一个版本制定在2009年
最新的版18：[https://tools.ietf.org/html/draft-pantos-http-live-streaming-18#userconsent](https://tools.ietf.org/html/draft-pantos-http-live-streaming-18#userconsent)#   2016年5月失效 会更新新的版本
参考文章：M3U8文件简介[http://blog.sina.com.cn/s/blog_6cf7acdf0102v0xv.html](http://blog.sina.com.cn/s/blog_6cf7acdf0102v0xv.html)
                    流媒体开发之--HLS--M3U8解析(1) [http://blog.csdn.net/jwzhangjie/article/details/9743971](http://blog.csdn.net/jwzhangjie/article/details/9743971)
                   流媒体开发之--HLS--M3U8解析(2): HLS草案  [http://blog.csdn.net/jwzhangjie/article/details/9744027](http://blog.csdn.net/jwzhangjie/article/details/9744027)
                    网络直播电视之M3U8解析篇（上）[http://blog.csdn.net/matthew_fan/article/details/8365401](http://blog.csdn.net/matthew_fan/article/details/8365401)
                  网络直播电视之M3U8解析篇 （下）[http://blog.csdn.net/matthew_fan/article/details/843838](http://blog.csdn.net/matthew_fan/article/details/8438384)
