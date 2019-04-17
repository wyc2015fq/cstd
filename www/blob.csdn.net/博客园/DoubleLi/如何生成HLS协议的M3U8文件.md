# 如何生成HLS协议的M3U8文件 - DoubleLi - 博客园






什么是HLS协议：



HLS（Http Live Streaming）是由Apple公司定义的用于实时流传输的协议，HLS基于HTTP协议实现，传输内容包括两部分，一是M3U8描述文件，二是TS媒体文件。



HLS协议应用：



由于传输层协议只需要标准的 HTTP 协议, HLS 可以方便的透过防火墙或者代理服务器, 而且可以很方便的利用CDN进行分发加速, 这样就可以很方便的解决大规模应用的瓶颈。并且客户端实现起来也容易。



HLS 目前广泛地应用于点播和直播领域，HLS协议是将音视频流通过HTTP协议传输出去，实现视频直播,HLS协议实现是关键。尤其目前直播领域的手机移动端的直播协议，基本上都是采用了苹果公司的HLS协议。可以说这个协议的出现解决了移动端的直播播放问题。



HLS协议[架构](http://lib.csdn.net/base/architecture)图：

![](http://www.800li.net/Uploads/Editor/2017-05-17/591c1665a0fdd.jpg)



M3U8文件的生成和解析：



HLS 协议的主要内容是关于 M3U8 这个文本协议的, 其实生成与解析都非常简单。下面是给出实际应用的例子：



Media Playlist (媒体播放列表)

![](http://www.800li.net/Uploads/Editor/2017-05-17/591c167c3b5bf.jpg)



·         HLS 通过 URI(RFC3986) 指向的一个 Playlist 来表示一个媒体流。

·         一个 Playlist 可以是一个 Media Playlist 或者 Master Playlist, 使用 UTF-8 编码的文本文件, 包含一些 URI 跟描述性的 tags。

·         一个 Media Playlist 包含一个 Media Segments 列表,当顺序播放时, 能播放整个完整的流。

·         要想播放这个 Playlist, 客户端需要首先下载他, 然后播放里面的每一个 Media Segment。

·         更加复杂的情况是, Playlist 是一个 Master Playlist, 包含一个 Variant Stream 集合, 通常每个 Variant Stream 里面是同一个流的多个不同版本(如: 分辨率, 码率不同)。

HLS Media Segments

·         每一个 Media Segment 通过一个 URI 指定, 可能包含一个 byte range。

·         每一个 Media Segment 的 duration 通过 EXTINF tag 指定。

·         每一个 Media Segment 有一个唯一的整数 Media Segment Number。

·         有些媒体格式需要一个 format-specific sequence 来初始化一个 parser, 在 Media Segment 被 parse 之前. 这个字段叫做 Media Initialization Section, 通过 EXT-X-MAP tag 来指定。

HLS 的优势

客户端支持简单, 只需要支持 HTTP 请求即可, HTTP 协议无状态, 只需要按顺序下载媒体片段即可。

使用 HTTP 协议网络兼容性好, HTTP 数据包也可以方便地通过防火墙或者代理服务器, CDN 支持良好。

Apple 的全系列产品支持, 由于 HLS 是苹果提出的, 所以在 Apple 的全系列产品包括 iphone, ipad, safari 都不需要安装任何插件就可以原生支持播放 HLS, 现在, [Android](http://lib.csdn.net/base/android) 也加入了对 HLS 的支持。

自带多码率自适应, Apple 在提出 HLS 时, 就已经考虑了码流自适应的问题。

HLS 的劣势

相比 RTMP 这类长连接协议, 延时较高, 难以用到互动直播场景。

对于点播服务来说, 由于 TS 切片通常较小, 海量碎片在文件分发, 一致性缓存, 存储等方面都有较大挑战。

HLS 延时问题



HLS 理论延时 = 1 个切片的时长 + 0-1个 td (td 是 EXT-X-TARGETDURATION, 可简单理解为播放器取片的间隔时间) + 0-n 个启动切片(苹果官方建议是请求到 3 个片之后才开始播放) + 播放器最开始请求的片的网络延时(网络连接耗时)。为了追求低延时效果, 可以将切片切的更小, 取片间隔做的更小, 播放器未取到 3 个片就启动播放. 但是, 这些优化方式都会增加 HLS 不稳定和出现错误的风险。



如今很多流媒体直播系统厂商都使用了HLS协议来解决安卓手机和苹果手机端的播放问题。自动生成有效的直播视频的M3U8播放链接地址，也就是HLS协议的M3U8的文件以便在第三方支持M3U8的播放器上流畅播放如VLC播放器或者和HTML5的网页技术结合通过移动端的浏览器网页观看直播视频。



![](http://www.800li.net/Uploads/Editor/2017-05-17/591c16a2cf438.jpg)



示例播放代码如下：



嵌入网页的HTML代码
<script type="text/javascript" src="http://192.168.1.44:8083/assets/player.js"></script><script type="text/javascript">p2ps_embed("hls", "live", "http://192.168.1.44:8083/videos/live/22/39/tUabV17nAPCev", "p2ps_video", "640", "480", "10.1.0", "http://192.168.1.44:8083/assets/expressInstall.swf",{}, {allowFullScreen:true, allowScriptAccess: "always"});</script><div id="p2ps_video"><h1>我们需要Flash player 10.1 或以上版本来播放。</h1><p><a href="http://www.adobe.com/go/getflashplayer"><img src="http://www.adobe.com/images/shared/download_buttons/get_flash_player.gif" alt="安装最新的Flash player" /></a></p></div>
移动端M3U8链接
http://192.168.1.44:8083/videos/live/22/39/tUabV17nAPCev/tUabV17nAPCev.M3U8








