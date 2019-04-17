# WebRTC学习资料大全 - DoubleLi - 博客园






在学习WebRTC，找了些资料，记录一下，供以后查询。

> 
有些需要FQ才能看


# WebRTC 介绍

官网在这里：[https://webrtc.org/](https://webrtc.org/)。然后这里有一个官方的Getting Started：[https://webrtc.org/start/](https://webrtc.org/start/)。

Google关于WebRTC的幻灯片：
- [http://io13webrtc.appspot.com/](http://io13webrtc.appspot.com/)

然后是WebRTC的SPEC：
- [https://www.w3.org/TR/webrtc/](https://www.w3.org/TR/webrtc/)

WebRTC项目源码地址：[https://chromium.googlesource.com/external/webrtc](https://chromium.googlesource.com/external/webrtc)。

Native开发文档：[https://webrtc.org/native-code/development/](https://webrtc.org/native-code/development/)。

# 教程

入门的，首选codelabs的Real time communication with WebRTC：
- [https://codelabs.developers.google.com/codelabs/webrtc-web](https://codelabs.developers.google.com/codelabs/webrtc-web)

然后是html5rocks上的基础教程：
- [https://www.html5rocks.com/en/tutorials/webrtc/basics/](https://www.html5rocks.com/en/tutorials/webrtc/basics/)

开发文档、入门教程，参考这里：
- [https://developer.mozilla.org/en-US/docs/Web/Guide/API/WebRTC/Peer-to-peer_communications_with_WebRTC](https://developer.mozilla.org/en-US/docs/Web/Guide/API/WebRTC/Peer-to-peer_communications_with_WebRTC)

这里，[http://piratefsh.github.io/projects/2015/08/27/webrtc-stun-turn-servers.html](http://piratefsh.github.io/projects/2015/08/27/webrtc-stun-turn-servers.html)，是一个人部署stun和turn server的记录。

进阶一点的，看这个，比较完整的介绍和实践：
- [http://blog.mgechev.com/2014/12/26/multi-user-video-conference-webrtc-angularjs-yeoman/](http://blog.mgechev.com/2014/12/26/multi-user-video-conference-webrtc-angularjs-yeoman/)

还有这个，介绍如何用WebRTC一步一步实现视频会议：
- [https://www.cleveroad.com/blog/webrtc-step-by-step-implementation-of-video-conference](https://www.cleveroad.com/blog/webrtc-step-by-step-implementation-of-video-conference)

还找到一个中文版的教程，通过WebRTC实现实时视频通信：
- [通过WebRTC实现实时视频通信（一）](http://www.gbtags.com/gb/share/3909.htm)
- [通过WebRTC实现实时视频通信（二）](http://www.gbtags.com/gb/share/3930.htm)
- [通过WebRTC实现实时视频通信（三）](http://www.gbtags.com/gb/share/3929.htm)

# STUN/TURN/Signaling解决方案

WebRTC需要Signaling、STUN、TURN等Server，Google有自己的，还有很多开源的，也有收费的。

免费的：
- Signaling Server需要自己实现，如果你用Nodejs和Socket.io的话，比较容易做。[https://codelabs.developers.google.com/codelabs/webrtc-web](https://codelabs.developers.google.com/codelabs/webrtc-web)这里就有一个示例。
- Google的STUN服务器：`stun:stun.l.google.com:19302`
- restund：[http://www.creytiv.com/restund.html](http://www.creytiv.com/restund.html)。
- rfc5766-turn-server: [https://code.google.com/p/rfc5766-turn-server](https://code.google.com/p/rfc5766-turn-server)。挪到这里了：[https://github.com/coturn/rfc5766-turn-server/](https://github.com/coturn/rfc5766-turn-server/)，支持STUN和TURN
- [http://www.pjsip.org/](http://www.pjsip.org/), PJSIP，支持STUN、TURN、ICE。
- [https://nice.freedesktop.org/wiki/](https://nice.freedesktop.org/wiki/)，libnice，支持ICE和STUN。
- [http://www.stunprotocol.org/](http://www.stunprotocol.org/)，STUNTMAN
- [https://sourceforge.net/projects/stun/](https://sourceforge.net/projects/stun/)，STUN client and server
- [https://github.com/coturn/coturn](https://github.com/coturn/coturn)，C++实现的STUN和TURN服务器，这里有一个安装指南：[https://www.webrtc-experiment.com/docs/TURN-server-installation-guide.html](https://www.webrtc-experiment.com/docs/TURN-server-installation-guide.html)

这里有一个WebRTC服务器搭建的文档：[http://io.diveinedu.com/2015/02/05/%E7%AC%AC%E5%85%AD%E7%AB%A0-WebRTC%E6%9C%8D%E5%8A%A1%E5%99%A8%E6%90%AD%E5%BB%BA.html](http://io.diveinedu.com/2015/02/05/%E7%AC%AC%E5%85%AD%E7%AB%A0-WebRTC%E6%9C%8D%E5%8A%A1%E5%99%A8%E6%90%AD%E5%BB%BA.html)。

收费的解决方案：
- [https://www.onsip.com/blog/webrtc-server](https://www.onsip.com/blog/webrtc-server)

# 开源示例

还有一些开源的示例代码，这个比较全了：
- [https://github.com/webrtc/samples](https://github.com/webrtc/samples)

对应的demo在这里（Chrome打开可以看效果）：
- [https://webrtc.github.io/samples](https://webrtc.github.io/samples)

回退一级，还有更多资源：
- [https://github.com/webrtc](https://github.com/webrtc)

还有一个，SimpleWebRTC：
- [https://github.com/andyet/SimpleWebRTC](https://github.com/andyet/SimpleWebRTC)

还有：
- [https://github.com/mgechev/angular-webrtc](https://github.com/mgechev/angular-webrtc)

嗯，接下来这个更赞，支持多人视频会议，开源的：
- [https://github.com/jitsi](https://github.com/jitsi)，对应的演示地址[https://meet.jit.si/](https://meet.jit.si/)

世界上第一个基于[HTML5](http://lib.csdn.net/base/html5)的SIP客户端：[https://www.doubango.org/sipml5/](https://www.doubango.org/sipml5/)。他们的GitHub主页：[https://github.com/DoubangoTelecom/doubango](https://github.com/DoubangoTelecom/doubango)。

# 在线演示

搜集了一些在线演示的示例：
- [https://apprtc.appspot.com](https://apprtc.appspot.com/)
- [http://www.simpl.info/getusermedia](http://www.simpl.info/getusermedia)
- [https://webrtc.github.io/samples](https://webrtc.github.io/samples)
- [http://webcamtoy.com/app/](http://webcamtoy.com/app/)
- [http://www.shinydemos.com/facekat/](http://www.shinydemos.com/facekat/)
- [http://idevelop.ro/ascii-camera/](http://idevelop.ro/ascii-camera/)
- [https://meet.jit.si/](https://meet.jit.si/)，多人的视频会议

# 围绕WebRTC的框架和服务

框架，视频通信的：
- [https://github.com/webrtc/adapter](https://github.com/webrtc/adapter)，封装了浏览器差异
- [https://github.com/henrikjoreteg/SimpleWebRTC](https://github.com/henrikjoreteg/SimpleWebRTC)，前面说过这个链接了
- [https://github.com/priologic/easyrtc](https://github.com/priologic/easyrtc)
- [https://github.com/webRTC/webRTC.io](https://github.com/webRTC/webRTC.io)

Peer间传递数据的：
- [http://peerjs.com/](http://peerjs.com/)
- [https://github.com/peer5/sharefest](https://github.com/peer5/sharefest)

服务：
- [http://www.tokbox.com/](http://www.tokbox.com/)
- [http://www.vline.com/](http://www.vline.com/)

# 图书
- 《Real-Time Communication with WebRTC》，[https://bloggeek.me/book-webrtc-salvatore-simon/](https://bloggeek.me/book-webrtc-salvatore-simon/)
- [https://bloggeek.me/best-webrtc-book/](https://bloggeek.me/best-webrtc-book/)，这里介绍了5本书。

# 课程


- [https://bloggeek.me/course/webrtc-architecture/](https://bloggeek.me/course/webrtc-architecture/)

原文链接：[http://blog.csdn.net/foruok/article/details/53005728](http://blog.csdn.net/foruok/article/details/53005728)

from:http://blog.csdn.net/caoshangpa/article/details/53084797

在学习WebRTC，找了些资料，记录一下，供以后查询。

> 
有些需要FQ才能看


# WebRTC 介绍

官网在这里：[https://webrtc.org/](https://webrtc.org/)。然后这里有一个官方的Getting Started：[https://webrtc.org/start/](https://webrtc.org/start/)。

Google关于WebRTC的幻灯片：
- [http://io13webrtc.appspot.com/](http://io13webrtc.appspot.com/)

然后是WebRTC的SPEC：
- [https://www.w3.org/TR/webrtc/](https://www.w3.org/TR/webrtc/)

WebRTC项目源码地址：[https://chromium.googlesource.com/external/webrtc](https://chromium.googlesource.com/external/webrtc)。

Native开发文档：[https://webrtc.org/native-code/development/](https://webrtc.org/native-code/development/)。

# 教程

入门的，首选codelabs的Real time communication with WebRTC：
- [https://codelabs.developers.google.com/codelabs/webrtc-web](https://codelabs.developers.google.com/codelabs/webrtc-web)

然后是html5rocks上的基础教程：
- [https://www.html5rocks.com/en/tutorials/webrtc/basics/](https://www.html5rocks.com/en/tutorials/webrtc/basics/)

开发文档、入门教程，参考这里：
- [https://developer.mozilla.org/en-US/docs/Web/Guide/API/WebRTC/Peer-to-peer_communications_with_WebRTC](https://developer.mozilla.org/en-US/docs/Web/Guide/API/WebRTC/Peer-to-peer_communications_with_WebRTC)

这里，[http://piratefsh.github.io/projects/2015/08/27/webrtc-stun-turn-servers.html](http://piratefsh.github.io/projects/2015/08/27/webrtc-stun-turn-servers.html)，是一个人部署stun和turn server的记录。

进阶一点的，看这个，比较完整的介绍和实践：
- [http://blog.mgechev.com/2014/12/26/multi-user-video-conference-webrtc-angularjs-yeoman/](http://blog.mgechev.com/2014/12/26/multi-user-video-conference-webrtc-angularjs-yeoman/)

还有这个，介绍如何用WebRTC一步一步实现视频会议：
- [https://www.cleveroad.com/blog/webrtc-step-by-step-implementation-of-video-conference](https://www.cleveroad.com/blog/webrtc-step-by-step-implementation-of-video-conference)

还找到一个中文版的教程，通过WebRTC实现实时视频通信：
- [通过WebRTC实现实时视频通信（一）](http://www.gbtags.com/gb/share/3909.htm)
- [通过WebRTC实现实时视频通信（二）](http://www.gbtags.com/gb/share/3930.htm)
- [通过WebRTC实现实时视频通信（三）](http://www.gbtags.com/gb/share/3929.htm)

# STUN/TURN/Signaling解决方案

WebRTC需要Signaling、STUN、TURN等Server，Google有自己的，还有很多开源的，也有收费的。

免费的：
- Signaling Server需要自己实现，如果你用Nodejs和Socket.io的话，比较容易做。[https://codelabs.developers.google.com/codelabs/webrtc-web](https://codelabs.developers.google.com/codelabs/webrtc-web)这里就有一个示例。
- Google的STUN服务器：`stun:stun.l.google.com:19302`
- restund：[http://www.creytiv.com/restund.html](http://www.creytiv.com/restund.html)。
- rfc5766-turn-server: [https://code.google.com/p/rfc5766-turn-server](https://code.google.com/p/rfc5766-turn-server)。挪到这里了：[https://github.com/coturn/rfc5766-turn-server/](https://github.com/coturn/rfc5766-turn-server/)，支持STUN和TURN
- [http://www.pjsip.org/](http://www.pjsip.org/), PJSIP，支持STUN、TURN、ICE。
- [https://nice.freedesktop.org/wiki/](https://nice.freedesktop.org/wiki/)，libnice，支持ICE和STUN。
- [http://www.stunprotocol.org/](http://www.stunprotocol.org/)，STUNTMAN
- [https://sourceforge.net/projects/stun/](https://sourceforge.net/projects/stun/)，STUN client and server
- [https://github.com/coturn/coturn](https://github.com/coturn/coturn)，C++实现的STUN和TURN服务器，这里有一个安装指南：[https://www.webrtc-experiment.com/docs/TURN-server-installation-guide.html](https://www.webrtc-experiment.com/docs/TURN-server-installation-guide.html)

这里有一个WebRTC服务器搭建的文档：[http://io.diveinedu.com/2015/02/05/%E7%AC%AC%E5%85%AD%E7%AB%A0-WebRTC%E6%9C%8D%E5%8A%A1%E5%99%A8%E6%90%AD%E5%BB%BA.html](http://io.diveinedu.com/2015/02/05/%E7%AC%AC%E5%85%AD%E7%AB%A0-WebRTC%E6%9C%8D%E5%8A%A1%E5%99%A8%E6%90%AD%E5%BB%BA.html)。

收费的解决方案：
- [https://www.onsip.com/blog/webrtc-server](https://www.onsip.com/blog/webrtc-server)

# 开源示例

还有一些开源的示例代码，这个比较全了：
- [https://github.com/webrtc/samples](https://github.com/webrtc/samples)

对应的demo在这里（Chrome打开可以看效果）：
- [https://webrtc.github.io/samples](https://webrtc.github.io/samples)

回退一级，还有更多资源：
- [https://github.com/webrtc](https://github.com/webrtc)

还有一个，SimpleWebRTC：
- [https://github.com/andyet/SimpleWebRTC](https://github.com/andyet/SimpleWebRTC)

还有：
- [https://github.com/mgechev/angular-webrtc](https://github.com/mgechev/angular-webrtc)

嗯，接下来这个更赞，支持多人视频会议，开源的：
- [https://github.com/jitsi](https://github.com/jitsi)，对应的演示地址[https://meet.jit.si/](https://meet.jit.si/)

世界上第一个基于[HTML5](http://lib.csdn.net/base/html5)的SIP客户端：[https://www.doubango.org/sipml5/](https://www.doubango.org/sipml5/)。他们的GitHub主页：[https://github.com/DoubangoTelecom/doubango](https://github.com/DoubangoTelecom/doubango)。

# 在线演示

搜集了一些在线演示的示例：
- [https://apprtc.appspot.com](https://apprtc.appspot.com/)
- [http://www.simpl.info/getusermedia](http://www.simpl.info/getusermedia)
- [https://webrtc.github.io/samples](https://webrtc.github.io/samples)
- [http://webcamtoy.com/app/](http://webcamtoy.com/app/)
- [http://www.shinydemos.com/facekat/](http://www.shinydemos.com/facekat/)
- [http://idevelop.ro/ascii-camera/](http://idevelop.ro/ascii-camera/)
- [https://meet.jit.si/](https://meet.jit.si/)，多人的视频会议

# 围绕WebRTC的框架和服务

框架，视频通信的：
- [https://github.com/webrtc/adapter](https://github.com/webrtc/adapter)，封装了浏览器差异
- [https://github.com/henrikjoreteg/SimpleWebRTC](https://github.com/henrikjoreteg/SimpleWebRTC)，前面说过这个链接了
- [https://github.com/priologic/easyrtc](https://github.com/priologic/easyrtc)
- [https://github.com/webRTC/webRTC.io](https://github.com/webRTC/webRTC.io)

Peer间传递数据的：
- [http://peerjs.com/](http://peerjs.com/)
- [https://github.com/peer5/sharefest](https://github.com/peer5/sharefest)

服务：
- [http://www.tokbox.com/](http://www.tokbox.com/)
- [http://www.vline.com/](http://www.vline.com/)

# 图书
- 《Real-Time Communication with WebRTC》，[https://bloggeek.me/book-webrtc-salvatore-simon/](https://bloggeek.me/book-webrtc-salvatore-simon/)
- [https://bloggeek.me/best-webrtc-book/](https://bloggeek.me/best-webrtc-book/)，这里介绍了5本书。

# 课程


- [https://bloggeek.me/course/webrtc-architecture/](https://bloggeek.me/course/webrtc-architecture/)

原文链接：[http://blog.csdn.net/foruok/article/details/53005728](http://blog.csdn.net/foruok/article/details/53005728)









