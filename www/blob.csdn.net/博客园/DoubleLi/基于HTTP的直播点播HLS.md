# 基于HTTP的直播点播HLS - DoubleLi - 博客园






  HLS（HTTP Live Streaming） 是Apple在2009年发布的，可以通过普通的web服务器进行分发的新型流媒体协议。苹果官方对于视频直播服务提出了 HLS 解决方案，该方案主要适用范围在于：

- 使用 iPhone 、iPod touch、 iPad 以及 Apple TV 进行流媒体直播功能。（MAC 也能用）
- 不使用特殊的服务软件进行流媒体直播。
- 需要通过加密和鉴定（authentication）的视频点播服务。


        HLS 的目的在于，让用户可以在苹果设备（包括MAC OS X）上通过普通的网络服务完成流媒体的播放。 HLS 同时支持流媒体的直播和点播服务，也支持不同 bit 速率的多个备用流（平时根据当前网速去自适应视频的清晰度），这样客户端也好根据当前网络的带宽去只能调整当前使用的视频流。安全方面，HLS 提供了通过 HTTPS 加密对媒体文件进行加密 并 对用户进行验证，允许视频发布者去保护自己的网络。

        相对于常见的流媒体直播协议，例如RTMP协议、RTSP协议、MMS协议等，HLS直播最大的不同在于，直播客户端获取到的，并不是一个完整的数据流。HLS协议在服务器端将直播数据流存储为连续的、很短时长的媒体文件（MPEG-TS格式），而客户端则不断的下载并播放这些小文件，因为服务器端总是会将最新的直播数据生成新的小文件，这样客户端只要不停的按顺序播放从服务器获取到的文件，就实现了直播。由此基本上可以认为，HLS是以点播的技术方式来实现直播，HLS的这种技术特点，决定了它的延迟一般总是会高于普通的流媒体直播协议。

        HLS只请求基本的HTTP报文，与实时传输协议（RTP)不同，HLS可以穿过任何允许HTTP数据通过的防火墙或者代理服务器。它也很容易使用内容分发网络来传输媒体流。由于数据通过HTTP协议传输，所以完全不用考虑防火墙或者代理的问题，而且分段文件的时长很短，客户端可以很快的选择和切换码率，以适应不同带宽条件下的播放。

        苹果对于自家的 HLS 推广也是采取了强硬措施，当你的直播内容持续十分钟 或者每五分钟内超过 5 MB 大小时，你的 APP 直播服务必须采用 HLS 架构，否则不允许上架。

**服务支持**

Adobe Flash Media Server：从4.5开始支持HLS、Protected HLS（PHLS）。5.0改名为Adobe Media Server

RealNetworks的 Helix Universal Server ：2010年4月，版本15.0开始支持iPhone, iPad和iPod的HTTP直播、点播H.264/AAC内容，最新更新在2012年11月。

微软的IIS Media Services：从4.0开始支持HLS。

Nginx RTMP Module：支持直播模式的HLS。

VLC Media Player：从2.0开始支持直播和点播HLS。

Wowza Media Server：2009年12月9日发布2.0，开始全面支持HLS。

VODOBOX Live Server：始支持HLS。





**客户端支持环境**



Native支持

> 
Android 3.0+

iOS 3.0+



flash支持：Adobe Flash Player从11.0开始支持HLS。

          MediaElement.js（MIT）


> 
Flowplayer（GPL ×）

GrindPlayer（MIT）

video-js-swf（Apache License 2.0）

clappr（BSD IE10+ ×）


Windows 10 的 EDGE 浏览器开始支持HLS。











