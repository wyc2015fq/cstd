# WebRTC学习与DEMO资源一览 - DoubleLi - 博客园






# 一. WebRTC学习

## 1.1   WebRTC现状

本人最早接触WebRTC是在2011年底，
那时Google已经在Android源码中加入了webrtc源码，放在/external/webrtc/，
但是Android并没有用到它，更没有被浏览器使用。
当时试图在Android 2.3（Gingerbread）高通平台的手机上
用H.264 硬件codec替换掉WebRTC缺省使用的VP8软codec，费了不少劲勉强换掉后效果很差只得放弃。

最近得知Google最新版的Chrome for Android已经支持WebRTC，
应老板的要求搭一个手机浏览器上视频通信的demo，
为此在网上搜集各种资料，发现经过一年多的发展，
国内外研究和使用WebRTC的人明显多起来，可用的demo也很多。
在此做一个笔记，留作日后参考。


目前基于WebRTC的开发其实有两个方向，
一个是基于浏览器的WebRTC应用开发，编程语言主要是JavaScript、HTML等，
这也是WebRTC作为HTML5标准的组成部分原本的目的；


另一个是C层面的移植和开发，作为一款非常强大的开源软件，
很多领域的软件项目都可以利用到WebRTC的音视频通信和处理能力，
这些场合的应用程序可能是C语言写的，也不一定与浏览器有关。
本文是属于前一种方向。



## 1.2 WebRTC基本概念学习


WebRTC的官方资料可以从其官网http://www.webrtc.org/
和W3C网站http://www.w3.org/TR/webrtc/上看到。

学习WebRTC基础知识比较好的网站是《Getting Started with WebRTC》，
网址是http://www.html5rocks.com/en/tutorials/webrtc/basics/，这个也是官网上推荐的。

对浏览器来说，WebRTC其实就是提供了3个API：
. MediaStream (即getUserMedia)，用于获取媒体数据，例如来自摄像头和麦克风的视频流和音频流；
. RTCPeerConnection，用于peer跟peer之间呼叫和建立连接以便传输音视频数据流；
. RTCDataChannel，用于peer跟peer之间传输音视频之外的一般数据。

需要注意的是这几个API的名称在不同浏览器及同一浏览器的不同版本之间略有差异，
比如PeerConnection在FireFox上叫做mozRTCPeerConnection，
而在当前版本的Chrome上叫做webkitRTCPeerConnection，
将来WebRTC标准化完成后会把这些前缀去掉使用统一的名称。

目前网上找到的WebRTC demo都只用到了getUserMedia和RTCPeerConnection这两个API，
另一个API即RTCDataChannel似乎目前还不太成熟。

WebRTC是实现peer to peer的实时通信（可以两个或多个peer之间），
在能够通信前peer跟peer之间必须建立连接，这是RTCPeerConnection的任务，
为此需要借助一个信令服务器（signaling server）来进行，信令包括3种类型的信息：
 . Session control messages: 初始化和关闭通信，及报告错误；
 . Network configuration: 双方的IP地址和端口号（局域网内部IP地址需转换为外部的IP地址）；
 . Media capabilities: 双方的浏览器支持使用何种codecs以及多高的视频分辨率。

WebRTC并未规定使用何种信令机制和消息协议，
象SIP、XMPP、XHR、WebSocket这些技术都可以用作WebRTC的信令通信。

除了信令服务器，peer跟peer建立连接还需要借助另一种服务器
（称为STUN server）实现NAT/Firewall穿越，因为很多peer是处于私有局域网中，
使用私有IP地址，必须转换为公有IP地址才能相互之间传输数据。
这其中涉及到一些专业术语包括STUN、TURN、ICE等，具体的本人还有待学习。
网上找到的WebRTC demo好象都用的是Google提供的STUN server。

peer跟peer之间一旦建立连接就可以直接传输音视频数据流，
并不需要借助第三方服务器中转。



# 二. WebRTC封装库


WebRTC的目的是为了简化基于浏览器的实时数据通信的开发工作量，
但实际应用编程还是有点复杂，尤其调用RTCPeerConnection必须对如何建立连接、
交换信令的流程和细节有较深入的理解。

因此有高人为我们开发了WebRTC封装库，将WebRTC的调用细节封装起来，包装成更简单的API，
使开发应用程序更简单。
封装库的另一个目的是为了屏蔽不同浏览器之间的差异，例如上面说的API名称的差异。
当然，这些库都是开源的，可以根据自己的需要重新修改。

目前网上找到的有两种WebRTC封装库，
1) webrtc.io，网址是https://github.com/webRTC/webRTC.io，
   上面有详细说明和使用方法，有很多demo使用它；
2) SimpleWebRTC，网址是https://github.com/HenrikJoreteg/SimpleWebRTC，
   貌似比webrtc.io用起来更简单。



# 三、 WebRTC demo试用


网上可以找到一堆WebRTC demo，在code.google.com上也能找到不少WebRTC应用项目的源码。
有些demo是直接调用WebRTC API开发的，但大多数是调用上述两种WebRTC封装库开发的。
由于WebRTC API的名称在不同浏览器及同一浏览器的不同版本之间存在差异，
所以不是所有demo都能运行在所有浏览器上。

为了找到一个可在公司局域网环境中跑在手机上的WebRTC demo，本人试用了不少demo，
下面选几个有代表性的介绍，其中有两个经修改后已在本人公司的局域网环境中运行成功。

先说一下本人的测试环境：手机上的浏览器是Chrome for Android 26.0.1410.49，
运行在Android 4.1.2上，这个Chrome版本本身是beta版，支持WebRTC但缺省是关闭WebRTC功能的，
需要在chrome://flags中使能WebRTC并重启Chrome，
或者在启动Chrome时增加命令行选项--enable-webrtc。
本人在PC上运行WebRTC的浏览器是Chrome 26.0.1410.43，操作系统是Windows 7。



## 3.1  http://www.webrtc.org/demo（https://apprtc.appspot.com/）


这是官方的demo，功能很全，
这个网站 https://apprtc.appspot.com/ 不能直接从国内上去，需要FQ才行，
它的源码原来是在https://code.google.com/p/webrtc-samples/上，
现在下不了了。
此demo没有用任何封装库。

这个demo所使用的信令机制使用了XHR和Google App Engine Channel API ，具体我不懂。
在我的公司局域网环境里无法运行该demo。



## 3.2  爱立信实验室开发的WebRTC demo


据说是第一个基于浏览器的WebRTC视频通信demo，
爱立信为此还开发了一个浏览器用于支持WebRTC，好象也是基于WebKit的，
叫做Bowser browser（当时市场上可能还没有支持WebRTC的浏览器），
该项目网址是https://labs.ericsson.com/apps/bowser。
这个Bowser browser好象只支持Ubuntu 11.04 and 11.10
（见https://labs.ericsson.com/apis/web-real-time-communication/downloads）。

该demo的网址是http://webrtc.labs.ericsson.net:8082。
在我的公司局域网环境里无法运行该demo。



## 3.3   人脸检测识别


利用WebRTC的getUserMedia从摄像头获取图像进行人脸识别的demo，例如这两个：
http://neave.com/webcam/html5/face/
http://www.raymondcamden.com/demos/2012/mar/29/test1.html
前面这个在PC和手机上的Chrome上都可运行，后面那个不行



## 3.4   http://www.simpl.info


这个demo演示HTML, CSS and JavaScript的各种feature和使用方法，
包括WebRTC的3个API：
 . getUserMedia、
 . RTCPeerConnection、
 . RTCDataChannel的演示，
但遗憾的是RTCPeerConnection的演示只是本地camera的画面传回给本地，
并没有实现真正的设备之间音视频通信。
该项目的源码在https://github.com/samdutton/simpl。



## 3.5   Framegrabber


这是一个基于WebRTC实现屏幕共享（screensharing）的Chrome扩展，
源码在https://github.com/samdutton/rtcshare，
有关介绍可参考这篇文章：http://blog.sina.com.cn/s/blog_51396f890102es7k.html。
没试用过。



## 3.6   http://webrtc.dennis.is


这个demo是基于库webrtc.io实现的，是webrtc.io官方的demo，使用WebSocket作为信令手段。
在我的公司局域网环境里无法运行该demo；
在家里无线路由器环境下可成功运行，但只能单向传输视频。



## 3.7   http://v.kainy.cn


国内牛人做的，相当于是汉化版的http://webrtc.dennis.is，
自然也是基于webrtc.io实现的，但使用的webrtc.io版本较老，
不支持新版本Chrome所使用的API名称webkitRTCPeerConnection，
所以无法在新版本Chrome上运行。
具体介绍在http://blog.kainy.cn/2013/01/webrtc实现的视频聊天室应用/。



## 3.8   http://conversat.io


这个demo是基于库SimpleWebRTC实现的，是SimpleWebRTC官方的demo，使用WebSocket作为信令手段。

在我的公司局域网环境里无法运行该demo；
在家里无线路由器环境下可成功运行，且可双向传视频，支持多个peer同时视频通信。


经修改后在本人公司局域网成功运行，
试过两个手机和一个笔记本电脑同时视频通信OK。
修改和运行步骤：
1. 从http://www.nodejs.org/download/下载nodejs最新版并安装，
   我是在Windows7 64位上安装的；
2. 在命令行下依次运行如下命令（安装运行signaling server所需的模块）：
npm install express
npm install yetify
npm install getconfig
npm install node-uuid
npm install socket.io


3. 从https://github.com/andyet/signalmaster下载信令服务器源码，
   该信令服务器是SimpleWebRTC缺省使用的，解开该源码后运行node server.js，
   该服务器监听8888端口，通过WebSocket与浏览器通信。


4. 在同一台PC上运行apache server，
   将从http://conversat.io网站扒下来的文件放到该server上
  （可在Chrome浏览器中打开http://conversat.io然后鼠标右键单击在菜单中选
   “另存为”、“网页，全部”即可），修改其中的 index.html 和 simplewebrtc.js，
    将其中 url 改为'http://10.100.156.83:8888'（其中IP地址是我的PC在公司局域网中的IP地址）。


5. 在同一局域网中的其他设备上打开Chrome浏览器，
   地址栏输入http://10.100.156.83，输入相同的room名称（随便起）即可开始多方视频通信，
   也可修改上述index.html中的“var room”一行，设定为固定的room名称，
   就不需要每次在每个设备上手工输入room名称了。




## 3.9   国内人开发的视频聊天室应用


这个demo是国内人赵书剑开发的视频聊天室，基于webrtc.io实现。
该项目源码和文档下载地址是:
  http://ishare.iask.sina.com.cn/f/35083616.html，
源码在:
  https://github.com/zsj2145676。


1. 从http://ishare.iask.sina.com.cn/f/35083616.html下载webrtc.chatdemo.zip，
解压缩，修改其中public\javascripts\client.js中的rtc.connect一行，
将实际的服务器地址写进去，例如改为：
  rtc.connect("ws://10.100.156.83:8001", room);


2. 安装node.js
从http://www.nodejs.org/download/下载nodejs最新版并安装，
   我是在Windows7 64位上安装的；
在命令行下依次运行如下命令（安装运行signaling server所需的模块）：
npm install express
npm install yetify
npm install getconfig
npm install node-uuid
npm install socket.io


3.运行node app.js，注意该demo本身已包含http server，
  不需要其他的http server,比如apache server


4.在同一局域网中的其他设备上打开Chrome浏览器，
地址栏输入http://10.100.156.83:8000，
输入相同的room名称（随便起）即可开始多方视频通信。


我对照做了安装和修改，测试失败，
能看到房间有人进来和退出，但无法进行文件和视频聊天。









