# iOS下音视频通信的实现-基于WebRTC - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年03月07日 12:22:05[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：2240
![](http://upload-images.jianshu.io/upload_images/2702646-68b506cc55887d9e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
#### 本文来自简书，原文地址:[http://www.jianshu.com/p/c49da1d93df4](http://www.jianshu.com/p/c49da1d93df4)
#### 前言：
WebRTC，名称源自网页实时通信（Web Real-Time Communication）的缩写，简而言之它是一个支持网页浏览器进行实时语音对话或视频对话的技术。
它为我们提供了视频会议的核心技术，包括音视频的采集、编解码、网络传输、显示等功能，并且还支持跨平台：windows，linux，mac，android，iOS。
它在2011年5月开放了工程的源代码，在行业内得到了广泛的支持和应用，成为下一代视频通话的标准。
本文将站在巨人的肩膀上，基于WebRTC去实现不同客户端之间的音视频通话。这个不同的客户端，不局限于移动端和移动端，还包括移动端和Web浏览器之间。
![](http://upload-images.jianshu.io/upload_images/2702646-1e74a6b70c80e577.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
#### 目录：
- 一.WebRTC的实现原理。
- 二.iOS下WebRTC环境的搭建。
- 三.介绍下WebRTC的API，以及实现点对点连接的流程。
- 四.iOS客户端的详细实现，以及服务端信令通道的搭建。
#### 正文：
##### 一.WebRTC的实现原理。
WebRTC的音视频通信是基于P2P，那么什么是P2P呢？
它是点对点连接的英文缩写。
###### 1.我们从P2P连接模式来讲起：
一般我们传统的连接方式，都是以服务器为中介的模式：
- 类似`http`协议：客户端⇋服务端（当然这里服务端返回的箭头仅仅代表返回请求数据）。
- 我们在进行即时通讯时，进行文字、图片、录音等传输的时候：客户端A⇨服务器⇨客户端B。
而点对点的连接恰恰数据通道一旦形成，中间是不经过服务端的，数据直接从一个客户端流向另一个客户端：
> 
客户端A⇋客户端B ... 客户端A⇋客户端C ...（可以无数个客户端之间互联）
这里可以想想音视频通话的应用场景，我们服务端确实是没必要去获取两者通信的数据，而且这样做有一个最大的一个优点就是，大大的减轻了服务端的压力。
而`WebRTC`就是这样一个基于P2P的音视频通信技术。
###### 2.WebRTC的服务器与信令。
讲到这里，可能大家觉得`WebRTC`就不需要服务端了么？这是显然是错误的认识，严格来说它仅仅是不需要服务端来进行数据中转而已。
WebRTC提供了浏览器到浏览器（点对点）之间的通信，但并不意味着WebRTC不需要服务器。暂且不说基于服务器的一些扩展业务，WebRTC至少有两件事必须要用到服务器：
- 浏览器之间交换建立通信的元数据（信令）必须通过服务器。
- 为了穿越NAT和防火墙。
第1条很好理解，我们在A和B需要建立P2P连接的时候，至少要服务器来协调，来控制连接开始建立。而连接断开的时候，也需要服务器来告知另一端P2P连接已断开。这些我们用来控制连接的状态的数据称之为信令，而这个与服务端连接的通道，对于`WebRTC`而言就是信令通道。
![](http://upload-images.jianshu.io/upload_images/2702646-9d654ab68af48919.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
图中signalling就是往服务端发送信令，然后底层调用`WebRTC`，`WebRTC`通过服务端得到的信令，得知通信对方的基本信息，从而实现虚线部分`Media`通信连接。
###### 当然信令能做的事还有很多，这里大概列了一下：
- 用来控制通信开启或者关闭的连接控制消息
- 发生错误时用来彼此告知的消息
- 媒体流元数据，比如像解码器、解码器的配置、带宽、媒体类型等等
- 用来建立安全连接的关键数据
- 外界所看到的的网络上的数据，比如IP地址、端口等
在建立连接之前，客户端之间显然没有办法传递数据。所以我们需要通过服务器的中转，在客户端之间传递这些数据，然后建立客户端之间的点对点连接。但是WebRTC API中并没有实现这些，这些就需要我们来实现了。
而第2条中的NAT这个概念，我们之前在[iOS即时通讯，从入门到“放弃”？](http://www.jianshu.com/p/2dbb360886a8)，中也提到过，不过那个时候我们是为了应对NAT超时，所造成的TCP连接中断。在这里我们就不展开去讲了，感兴趣的可以看看：[NAT百科](http://baike.baidu.com/link?url=V6oQ2RZi1uDc8kXNQnsZ6pjzHVQ3XXWqzIre1O-dVpr8XMXVVON1c7MLp_PfEfyUeyoeaBMlqknEDmHSMgYAIK)
这里我简要说明一下，NAT技术的出现，其实就是为了解决IPV4下的IP地址匮乏。举例来说，就是通常我们处在一个路由器之下，而路由器分配给我们的地址通常为192.168.0.1 、192.168.0.2如果有n个设备，可能分配到192.168.0.n，而这个IP地址显然只是一个内网的IP地址，这样一个路由器的公网地址对应了n个内网的地址，通过这种使用少量的公有IP 地址代表较多的私有IP 地址的方式，将有助于减缓可用的IP地址空间的枯竭。
但是这也带来了一系列的问题，例如这里点对点连接下，会导致这样一个问题：
如果客户端A想给客户端B发送数据，则数据来到客户端B所在的路由器下，会被NAT阻拦，这样B就无法收到A的数据了。
但是A的NAT此时已经知道了B这个地址，所以当B给A发送数据的时候，NAT不会阻拦，这样A就可以收到B的数据了。这就是我们进行NAT穿越的核心思路。
于是我们就有了以下思路：
我们借助一个公网IP服务器,a,b都往公网IP/PORT发包,公网服务器就可以获知a,b的IP/PORT，又由于a,b主动给公网IP服务器发包，所以公网服务器可以穿透NAT A,NAT B送包给a,b。
所以只要公网IP将b的IP/PORT发给a,a的IP/PORT发给b。这样下次a和b互相消息，就不会被NAT阻拦了。
###### 而WebRTC的NAT/防火墙穿越技术，就是基于上述的一个思路来实现的：
> 
建立点对点信道的一个常见问题，就是NAT穿越技术。在处于使用了NAT设备的私有TCP/IP网络中的主机之间需要建立连接时需要使用NAT穿越技术。以往在VoIP领域经常会遇到这个问题。目前已经有很多NAT穿越技术，但没有一项是完美的，因为NAT的行为是非标准化的。这些技术中大多使用了一个公共服务器，这个服务使用了一个从全球任何地方都能访问得到的IP地址。在RTCPeeConnection中，使用ICE框架来保证RTCPeerConnection能实现NAT穿越
![](http://upload-images.jianshu.io/upload_images/2702646-bc4449bffdb08df5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
这里提到了ICE协议框架，它大约是由以下几个技术和协议组成的：STUN、NAT、TURN、SDP，这些协议技术，帮助ICE共同实现了NAT/防火墙穿越。
小伙伴们可能又一脸懵逼了，一下子又出来这么多名词，没关系，这里我们暂且不去管它们，等我们后面实现的时候，还会提到他们，这里提前感兴趣的可以看看这篇文章：[WebRTC
 protocols](https://developer.mozilla.org/zh-CN/docs/Web/API/WebRTC_API/Protocols)
##### 二.iOS下WebRTC环境的搭建：
首先，我们需要明白的一点是：WebRTC已经在我们的浏览器中了。如果我们用浏览器，则可以直接使用js调用对应的`WebRTC`的API，实现音视频通信。
然而我们是在iOS平台，所以我们需要去官网下载指定版本的源码，并且对其进行编译，大概一下，其中源码大小10个多G，编译过程会遇到一系列坑，而我们编译完成最终形成的`webrtc`的`.a`库大概有300多m。
这里我们不写编译过程了，感兴趣的可以看看这篇文章：
[WebRTC(iOS)下载编译](http://www.cnblogs.com/fulianga/p/5868823.html)
最终我们编译成功的文件如下`WebRTC`：
![](http://upload-images.jianshu.io/upload_images/2702646-605ef136279acb26.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
其中包括一个`.a`文件，和`include`文件夹下的一些头文件。(大家测试的时候可以直接使用这里编译好的文件，但是如果以后需要WebRTC最新版，就只能自己动手去编译了)
接着我们把整个`WebRTC`文件夹添加到工程中，并且添加以下系统依赖库：
![](http://upload-images.jianshu.io/upload_images/2702646-a071ef39a12f3ee2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
依赖库
至此，一个`iOS`下的`WebRTC`环境就搭建完毕了
##### 三.介绍下WebRTC的API，以及实现点对点连接的流程。
###### 1.WebRTC主要实现了三个API，分别是:
- `MediaStream`：通过`MediaStream`的API能够通过设备的摄像头及话筒获得视频、音频的同步流
- `RTCPeerConnection`：`RTCPeerConnection`是WebRTC用于构建点对点之间稳定、高效的流传输的组件
- `RTCDataChannel`：`RTCDataChannel`使得浏览器之间（点对点）建立一个高吞吐量、低延时的信道，用于传输任意数据。
其中`RTCPeerConnection`是我们`WebRTC`的核心组件。
###### 2.WebRTC建立点对点连接的流程:
我们在使用`WebRTC`来实现音视频通信前，我们必须去了解它的连接流程，否则面对它的`API`将无从下手。
我们之前讲到过WebRTC用`ICE`协议来保证NAT穿越，所以它有这么一个流程：我们需要从
```
STUN
 Server
```
中得到一个`ice candidate`，这个东西实际上就是公网地址，这样我们就有了客户端自己的公网地址。而这个
```
STUN
 Server
```
所做的事就是之前所说的，把保存起来的公网地址，互相发送数据包，防止后续的`NAT`阻拦。
而我们之前讲过，还需要一个自己的服务端，来建立信令通道，控制A和B什么时候建立连接，建立连接的时候告知互相的
```
ice
 candidate
```
（公网地址）是什么、`SDP`是什么。还包括什么时候断开连接等等一系列信令。
对了，这里补充一下`SDP`这个概念，它是会话描述协议[Session
 Description Protocol (SDP)](http://en.wikipedia.org/wiki/Session_Description_Protocol) 是一个描述多媒体连接内容的协议，例如分辨率，格式，编码，加密算法等。所以在数据传输时两端都能够理解彼此的数据。本质上，这些描述内容的元数据并不是媒体流本身。
###### 讲到这我们来捋一捋建立P2P连接的过程：
- A和B连接上服务端，建立一个TCP长连接（任意协议都可以，WebSocket/MQTT/Socket原生/XMPP），我们这里为了省事，直接采用`WebSocket`，这样一个信令通道就有了。
- A从`ice server`（STUN
 Server）获取`ice candidate`并发送给Socket服务端，并生成包含
```
session
 description
```
（SDP）的offer，发送给Socket服务端。
- Socket服务端把A的offer和
```
ice
 candidate
```
转发给B，B会保存下A这些信息。
- 然后B发送包含自己
```
session
 description
```
的`answer`(因为它收到的是offer，所以返回的是`answer`，但是内容都是SDP)和
```
ice
 candidate
```
给Socket服务端。
- Socket服务端把B的`answer`和
```
ice
 candidate
```
给A，A保存下B的这些信息。
至此A与B建立起了一个P2P连接。
这里理解整个P2P连接的流程是非常重要的，否则后面代码实现部分便难以理解。
##### 四.iOS客户端的详细实现，以及服务端信令通道的搭建。
###### 聊天室中的信令
上面是两个用户之间的信令交换流程，但我们需要建立一个多用户在线视频聊天的聊天室。所以需要进行一些扩展，来达到这个要求
###### 用户操作
首先需要确定一个用户在聊天室中的操作大致流程：
- 打开页面连接到服务器上
- 进入聊天室
- 与其他所有已在聊天室的用户建立点对点的连接，并输出在页面上
- 若有聊天室内的其他用户离开，应得到通知，关闭与其的连接并移除其在页面中的输出
- 若又有其他用户加入，应得到通知，建立于新加入用户的连接，并输出在页面上
- 离开页面，关闭所有连接
###### 从上面可以看出来，除了点对点连接的建立，还需要服务器至少做如下几件事：
- 新用户加入房间时，发送新用户的信息给房间内的其他用户
- 新用户加入房间时，发送房间内的其他用户信息给新加入房间的用户
- 用户离开房间时，发送离开用户的信息给房间内的其他用户
###### 实现思路
以使用WebSocket为例，上面用户操作的流程可以进行以下修改：
- 客户端与服务器建立WebSocket连接
- 发送一个加入聊天室的信令（join），信令中需要包含用户所进入的聊天室名称
- 服务器根据用户所加入的房间，发送一个其他用户信令（peers），信令中包含聊天室中其他用户的信息，客户端根据信息来逐个构建与其他用户的点对点连接
- 若有用户离开，服务器发送一个用户离开信令（remove_peer），信令中包含离开的用户的信息，客户端根据信息关闭与离开用户的信息，并作相应的清除操作
- 若有新用户加入，服务器发送一个用户加入信令（new_peer），信令中包含新加入的用户的信息，客户端根据信息来建立与这个新用户的点对点连接
- 用户离开页面，关闭WebSocket连接
###### 这样有了基本思路，我们来实现一个基于`WebRTC`的视频聊天室。
我们首先来实现客户端实现，先看看`WebRTCHelper.h`：
```
@protocol WebRTCHelperDelegate;
@interface WebRTCHelper : NSObject<SRWebSocketDelegate>
+ (instancetype)sharedInstance;
@property (nonatomic, weak)id<WebRTCHelperDelegate> delegate;
/**
 *  与服务器建立连接
 *
 *  @param server 服务器地址
 *  @param room   房间号
 */
- (void)connectServer:(NSString *)server port:(NSString *)port room:(NSString *)room;
/**
 *  退出房间
 */
- (void)exitRoom;
@end
@protocol WebRTCHelperDelegate <NSObject>
@optional
- (void)webRTCHelper:(WebRTCHelper *)webRTChelper setLocalStream:(RTCMediaStream *)stream userId:(NSString *)userId;
- (void)webRTCHelper:(WebRTCHelper *)webRTChelper addRemoteStream:(RTCMediaStream *)stream userId:(NSString *)userId;
- (void)webRTCHelper:(WebRTCHelper *)webRTChelper closeWithUserId:(NSString *)userId;
@end
```
这里我们对外的接口很简单，就是一个生成单例的方法，一个代理，还有一个与服务器连接的方法，这个方法需要传3个参数过去，分别是server的地址、端口号、以及房间号。还有一个退出房间的方法。
说说代理部分吧，代理有3个可选的方法，分别为：
- 本地设置流的回调，可以用来显示本地的视频图像。
- 远程流到达的回调，可以用来显示对方的视频图像。
- `WebRTC`连接关闭的回调，注意这里关闭仅仅与当前`userId`的连接关闭，而如果你除此之外还与聊天室其他的人建立连接，是不会有影响的。
接着我们先不去看如何实现的，先运行起来看看效果吧:
`VideoChatViewController.m`:
```
[WebRTCHelper sharedInstance].delegate = self;
[[WebRTCHelper sharedInstance]connectServer:@"192.168.0.7" port:@"3000" room:@"100"];
```
仅仅需要设置代理为自己，然后连接上`socket`服务器即可。
我们来看看我们对代理的处理：
```
- (void)webRTCHelper:(WebRTCHelper *)webRTChelper setLocalStream:(RTCMediaStream *)stream userId:(NSString *)userId
{
    RTCEAGLVideoView *localVideoView = [[RTCEAGLVideoView alloc] initWithFrame:CGRectMake(0, 0, KVedioWidth, KVedioHeight)];
    //标记本地的摄像头
    localVideoView.tag = 100;
    _localVideoTrack = [stream.videoTracks lastObject];
    [_localVideoTrack addRenderer:localVideoView];
    [self.view addSubview:localVideoView];
    NSLog(@"setLocalStream");
}
- (void)webRTCHelper:(WebRTCHelper *)webRTChelper addRemoteStream:(RTCMediaStream *)stream userId:(NSString *)userId
{
    //缓存起来
    [_remoteVideoTracks setObject:[stream.videoTracks lastObject] forKey:userId];
    [self _refreshRemoteView];
    NSLog(@"addRemoteStream");
}
- (void)webRTCHelper:(WebRTCHelper *)webRTChelper closeWithUserId:(NSString *)userId
{
    //移除对方视频追踪
    [_remoteVideoTracks removeObjectForKey:userId];
    [self _refreshRemoteView];
    NSLog(@"closeWithUserId");
}
- (void)_refreshRemoteView
{
    for (RTCEAGLVideoView *videoView in self.view.subviews) {
        //本地的视频View和关闭按钮不做处理
        if (videoView.tag == 100 ||videoView.tag == 123) {
            continue;
        }
        //其他的移除
        [videoView removeFromSuperview];
    }
    __block int column = 1;
    __block int row = 0;
    //再去添加
    [_remoteVideoTracks enumerateKeysAndObjectsUsingBlock:^(id  _Nonnull key, RTCVideoTrack *remoteTrack, BOOL * _Nonnull stop) {
        RTCEAGLVideoView *remoteVideoView = [[RTCEAGLVideoView alloc] initWithFrame:CGRectMake(column * KVedioWidth, 0, KVedioWidth, KVedioHeight)];
        [remoteTrack addRenderer:remoteVideoView];
        [self.view addSubview:remoteVideoView];
        //列加1
        column++;
        //一行多余3个在起一行
        if (column > 3) {
            row++;
            column = 0;
        }
    }];
}
```
代码很简单，基本核心的是调用了`WebRTC`的API的那几行：
这里我们得到本地流和远程流的时候，就可以用这个流来设置视频图像了，而音频是自动输出的（远程的音频会输出，自己本地的音频则不会）。
基本上显示视频图像只需要下面3步：
- 创建一个`RTCEAGLVideoView`类型的实例。
- 从代理回调中拿到`RTCMediaStream`类型的`stream`，从`stream`中拿到`RTCVideoTrack`实例：
`_localVideoTrack = [stream.videoTracks lastObject];`
- 用这个`_localVideoTrack`为`RTCEAGLVideoView`实例设置渲染:
`[_localVideoTrack addRenderer:localVideoView];`
这样一个视频图像就呈现在`RTCEAGLVideoView`实例上了，我们只需要把它添加到`view`上显示即可。
这里切记需要注意的是`RTCVideoTrack`实例我们必须持有它（这里我们本机设置为属性了，而远程的添加到数组中，都是为了这么个目的）。否则有可能会导致视频图像无法显示。
就这样，一个简单的`WebRTC`客户端就搭建完了，接下来我们先忽略掉`Socket`服务端（先当作已实现），和`WebRTCHelper`的实现，我们运行运行demo看看效果：
![](http://upload-images.jianshu.io/upload_images/2702646-5e3f733a921fb94f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
这是我用手机截的图，因为模拟器无法调用mac摄像头，第一个
