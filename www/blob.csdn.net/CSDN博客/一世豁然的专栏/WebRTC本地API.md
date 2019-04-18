# WebRTC本地API - 一世豁然的专栏 - CSDN博客





2017年08月06日 14:56:00[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1903








WebRTC Native API实现基于W3C的WebRTC 1.0：浏览器之间的实时通信。




实现WebRTC Native API（包括Stream和PeerConnection API）的代码在[这里](https://chromium.googlesource.com/external/webrtc/+/master/webrtc/api/)可用。 还提供了一个示例客户端应用程序。





本文档的目标受众是希望使用WebRTC Native API来实现WebRTC JavaScript API或开发本机RTC应用程序的用户。








一、框图


![](https://img-blog.csdn.net/20170806144705732?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)








二、调用序列


1、建立一个call

![](https://img-blog.csdn.net/20170806145018967?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





2、接收一个call

![](https://img-blog.csdn.net/20170806145103911?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





3、关闭一个call

![](https://img-blog.csdn.net/20170806145145812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


三、线程模型

WebRTC Native API使用两个全局可用线程：信令线程和工作线程。 根据PeerConnection工厂的创建方式，应用程序可以提供这两个线程，也可以在内部创建它们。





对Stream API和PeerConnection API的调用将被代理到信令线程，这意味着应用程序可以从任何线程调用这些API。





所有回调将在信令线程上进行。 应用程序应尽可能快地返回回调，以避免阻塞信令线程。 资源密集型流程应该发布到不同的线程。





工作线程用于处理更多的资源密集型进程，如数据流。


1、Stream APIs([mediastreaminterface.h](https://chromium.googlesource.com/external/webrtc/+/master/webrtc/api/mediastreaminterface.h))

2、PeerConnection APIs ([peerconnectioninterface.h](https://chromium.googlesource.com/external/webrtc/+/master/webrtc/api/peerconnectioninterface.h))



