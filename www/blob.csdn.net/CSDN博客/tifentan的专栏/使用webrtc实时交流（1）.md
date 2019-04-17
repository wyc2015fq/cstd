# 使用webrtc实时交流（1） - tifentan的专栏 - CSDN博客





2017年09月06日 10:59:18[露蛇](https://me.csdn.net/tifentan)阅读数：265








本系列文章翻译自webrtc官网链接codelab。 
[https://codelabs.developers.google.com/codelabs/webrtc-web/#0](https://codelabs.developers.google.com/codelabs/webrtc-web/#0)

本人翻译水平有限，本着翻译过程就是学习过程的目的进行翻译。若有不当之处，欢迎指出。
# 介绍

```
webrtc是一个开源项目，提供web层、（native）源码层API，实现实时的音频、视频、数据交流。
JavaScript主要有以下API,点击链接查看例子
```
- **[getUserMedia()](https://webrtc.github.io/samples/src/content/getusermedia/gum/)：获取音视频**
- **[MediaRecorder](https://webrtc.github.io/samples/src/content/getusermedia/record/)：录制音视频**
- **[RTCPeerConnection](https://webrtc.github.io/samples/src/content/peerconnection/pc1/)：建立媒体流**
- **[RTCDataChannel](https://webrtc.github.io/samples/src/content/datachannel/basic/)：传输用户数据**

```
有意思的例子2，能录像还能把录像下载。
```

## webrtc可以使用在哪里

安卓和桌面系统的Firefox, Opera， Chrome。在ios和安卓上支持native apps（原生开发）。

## 信令传输（signaling）

webrtc使用RTCPeerConnection在浏览器间传输媒体流，但同时还需要一个协调沟通，传输控制消息的机制，这个流程叫signaling。在codelab这里我们使用Node实现，但还有很多[其他选择](https://github.com/muaz-khan/WebRTC-Experiment/blob/master/Signaling.md)

## 什么是STUN和TURN

webrtc基于p2p实现，用户间尽量选择最直接的路由连接（例如局域网）。但是现实世界中的网络充满了NAT网关和防火墙，在复杂的网络情况下，p2p连接不能保证连接成功，所以需要后手。STUN服务器提供这样一种容错机制，通过webrtc APIS交换连接双方所有的ip地址，尽可能的尝试连接。一旦所有的连接尝试都不成功，TURN作为一个中转服务器的角色保证传输。详细可看下面链接： 
[在互联网中的webrtc](http://www.html5rocks.com/en/tutorials/webrtc/infrastructure/)

## webrtc安全吗

webrtc的组件都是加密的（估计指的是web api），只能通过https或者localhost调用。信令传输是你自己定义的，安不安全关我鸟事，你可以选择安全性高的协议。




