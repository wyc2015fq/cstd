# 直播http-flv小调研 - 建建的博客 - CSDN博客
2018年02月02日 14:29:27[纪建](https://me.csdn.net/u013898698)阅读数：915
最近打算直播上http-flv，之前用的是rtmp和hls。为什么使用http-flv,它有什么优缺点？
怎么让流媒体服务器支持flv直播？
**一、市场上哪家直播使用了http-flv:**
通过抓包分析: 优酷的pc网页直播使用了http-flv。
斗鱼、熊猫tv、虎牙pc网页上的也使用了http-flv。
**二、http-flv、rtmp和hls直播的优缺点:**
A、三者的延迟性：
http-flv：低延迟，内容延迟可以做到2-5秒。
Rtmp：低延迟，内容延迟可以做到2-5秒。
Hls:：延迟较高。
B、三者的易用性:
rtmp和http-flv：播放端安装率高。只要浏览器支持FlashPlayer就能非常简易的播放。
hls：最大的优点：HTML5可以直接打开播放；这个意味着可以把一个直播链接通过微信
等转发分享，不需要安装任何独立的APP，有浏览器即可。
C、rtmp和http-flv比较：
(1) 穿墙：很多防火墙会墙掉RTMP，但是不会墙HTTP，因此HTTP FLV出现奇怪问题的概率很小。
(2) 调度：RTMP也有个302，可惜是播放器as中支持的，HTTP FLV流就支持302方便CDN纠正DNS的错误。
(3) 容错：SRS的HTTP FLV回源时可以回多个，和RTMP一样，可以支持多级热备。
(4) 简单：FLV是最简单的流媒体封装，HTTP是最广泛的协议，这两个组合在一起维护性更高，比RTMP简单多了。
**三、http-flv技术实现:**
HTTP协议中有个约定：content-length字段，http的body部分的长度
服务器回复http请求的时候如果有这个字段，客户端就接收这个长度的数据然后就认为数据传输完成了，
如果服务器回复http请求中没有这个字段，客户端就一直接收数据，直到服务器跟客户端的socket连接断开。
http-flv直播就是利用第二个原理，服务器回复客户端请求的时候不加content-length字段，在回复了http
内容之后，紧接着发送flv数据，客户端就一直接收数据了。
**四、使用和测试http-flv:**
使用开源的SRS服务器，SRS2.0版本是开源，后续的版本已经闭源。
推流端以rtmp的形式推流到srs服务器，srs再把rtmp流转换成http-flv流。
请求SRS返回的是：
 HTTP/1.1 200 OK
Connection: Keep-Alive
Content-Type: video/x-flv
Server: SRS/2.0.205
Transfer-Encoding: chunked
请求某个电视台http://fms.cntv.lxdns.com/live/flv/channel2.flv，返回的是：
HTTP/1.1 200 OK
Cache-Control: no-cache
Content-Type: video/x-flv
Expires: -1
Pragma: no-cache
欢迎转载，转载请注明出处: [http://blog.csdn.net/ahoustep](http://blog.csdn.net/ahoustep)
