# WebRTC开源项目一览之二 - DoubleLi - 博客园






、Kurento视频直播系统
4.1  应用实例
搞视频会议就会涉及一对多、多对多、广播、转码、混音、合屏、录制，这就需要用到流媒体服务器，而kurento就具有这些功能。
他主要用来作为webrtc的流媒体服务器，因为BUG多，目前不适于商用，不过前景可期， 
图1:
![](http://blog.chinaunix.net/attachment/201507/28/26000296_1438081429NW3D.png)
说明：
1、看到这里您可不要讲他的功能和ICE服务器的功能给搞混了哦，后者主要用来做NAT穿透和转发的。

4.1.1 、kurento架构
图2:
![](http://blog.chinaunix.net/attachment/201507/28/26000296_14380814413CXB.png)

说明：
1、客户端对音视频数据的采集和播放等是通过webrtc来处理的，传输模块就是kurento的。
2、流媒体服务是他的核心服务，可以进行编解码，混音，录制，计算机视觉，视觉增强等等。

4.1.2  、特色功能---计算机视觉
图3:
![](http://blog.chinaunix.net/attachment/201507/28/26000296_1438081453W559.png)

说明：
1、服务端可以对收到的视频流进行处理，如人脸识别，这些扩展下去应用前景就很广泛了，期待！
2、因为他对图像进行了处理，所以延迟会比较大，识别率还存在些问题，而且会造成图像闪动（可能也是跟延迟有关）。
3、其他功能如一对一，广播就不重复了，很多其他流媒体服务都具有这些功能。

最后：虽然kurento目前问题很多，但我看好他，后面会继续分享相关内容，也会和他们一起去完善这个东西。


4.2  Kureto官网
链接：http://www.kurento.org/whats-kurento
What's Kurento?
Kurento is a WebRTC media server and a set of client APIs making simple the development 
of advanced video applications for WWW and smartphone platforms. 
Kurento Media Server features include group communications, transcoding, 
recording, mixing, broadcasting and routing of audiovisual flows.


Kurento 是一个 WebRTC 流媒体服务器以及一些客户端API，有了它，
开发WWW及智能手机平台的高级视频应用就变得更加容易。
可以利用Kurento开发的应用类型包括，视频会议，音视频广播，音视频录制、转码等。


As a differential feature, Kurento Media Server also provides advanced media 
processing capabilities involving computer vision, video indexing, 
augmented reality and speech analysis. Kurento modular architecture 
makes simple the integration of third party media processing algorithms 
(i.e. speech recognition, sentiment analysis, face recognition, etc.), 
which can be transparently used by application developers as the rest of Kurento built-in features.
.

Kurento Media Server adds advanced media processing to the capabilities of common media servers.


五、一个WebRTC的视频直播试验项目
链接：
https://github.com/muaz-khan/WebRTC-Experiment/tree/master/broadcast
WebRTC One-to-Many video sharing/broadcasting / Demo
1. This WebRTC experiment is for one-to-many transmission of audio/video streams.
2. It sets up multiple peer connections to support the multi-user connectivity feature.
 Rememebr, WebRTC doesn't supports 3-way handshake!
3. Multi-peers establishment opens many RTP-ports according to the 
number of media streamas referenced to each peer connection.
4. Multi-ports establishment causes huge CPU and bandwidth usage!


六、一个基于WebRTC的混合CDN/P2P直播开源项目
大家好，


最近在GitHub看到一个基于WebRTC的混合CDN/P2P直播开源项目，觉得很有前景。
相比传统P2P直播软件而言，该项目的优势是不需要安装任何插件，而且主流浏览器都支持。
出于好奇，我也在服务器上创建了一个实例，看看效果如何。


如果你稍微有那么一点的感兴趣，我邀请你一起参与测试。这里是一些注意事项：
1. 测试地址 http://www.zoptv.com/player/p2p/
2. 支持的浏览器 Chrome, Firefox, Opera
3. 测试页面和视频，之后可能被删掉
4. 期待你的建议，我会不定期的在此帖回复


P.S 感谢原作者 http://github.com/bemtv (貌似github被墙了？)


官方链接：
https://github.com/bemtv


http://bem.tv/
BemTV is an attempt to scale live video streaming using peer-to-peer
 without the need of an external plug-in. It uses the powers of WebRTC to 
build swarms, enabling the possibility to flow chunks between users.



from:http://blog.csdn.net/fireroll/article/details/47109757









