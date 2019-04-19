# Android  WebRTC开发入门 - starRTC免费im直播会议一对一视频 - CSDN博客
2019年04月17日 16:19:02[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：11
在学习 WebRTC 的过程中，学习的一个基本步骤是先通过 JS 学习 WebRTC的整体流程，在熟悉了整体流程之后，再学习其它端如何使用 WebRTC 进行互联互通。
申请权限
- Camera 权限
- Record Audio 权限
- Intenet 权限
在Android中，申请权限分为静态权限申请和动态权限申请，这对于做 Android 开发的同学来说已经是习以为常的事情了。下面我们就看一下具体如何申请权限：
静态权限申请
在 Android 项目中的 AndroidManifest.xml 中增加以下代码:
... <uses-feature android:name="android.hardware.camera" /> <uses-feature android:glEsVersion="0x00020000" android:required="true" /> <uses-permission android:name="android.permission.CAMERA" /> <uses-permission android:name="android.permission.RECORD_AUDIO" /> <uses-permission android:name="android.permission.INTENET" /> ...
动态权限申请
随着 Android 的发展，对安全性要求越来越高。除了申请静态权限之外，还需要动态申请权限。代码如下：
void requestPermissions(String[] permissions, intrequestCode);
实际上，对于权限这块的处理真正做细了要写不少代码，好在 Android 官方给我们又提供了一个非常好用的库 EasyPermissions , 有了这个库我们可以少写不少代码。使用 EasyPermissions 非常简单，在MainActivity中添加代码如下：
... protected void onCreate ( Bundle savedInstanceState ) { ... String[] perms = { Manifest.permission.CAMERA, Manifest.permission.RECORD_AUDIO }; if (!EasyPermissions.hasPermissions(this, perms)) { EasyPermissions.requestPermissions(this, "Need permissions for camera & microphone", 0, perms); } } @Override public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) { super.onRequestPermissionsResult(requestCode, permissions, grantResults); EasyPermissions.onRequestPermissionsResult(requestCode, permissions, grantResults, this); } ...
通过添加以上代码，就将权限申请好了，是不是非常简单？
第二步，看在 Android 下如何引入 WebRTC 库。
第一个是 WebRTC 库，第二个是 [socket.io](http://socket.io/) 库，用它来与信令服务器互联。
首先我们看一下如何引入 WebRTC 库（我这里使用的是最新 Android Studio 3.3.2）。在 Module 级别的 build.gradle 文件中增加以下代码：
... dependencies { ... implementation 'org.webrtc:google-webrtc:1.0.+' ... }
接下来要引入 [socket.io](http://socket.io/) 库，用它来与 Nodejs 搭建的信令服务器进行对接。再加上前面用到的EasyPermissions库，所以真正的代码应写成下面的样子：
... dependencies { ... implementation 'io.socket:socket.io-client:1.0.0' implementation 'org.webrtc:google-webrtc:1.0.+' implementation 'pub.devrel:easypermissions:1.1.3' }
WebRTC程序的起源就是PeerConnectionFactory。这也是与使用 JS 开发 WebRTC 程序最大的不同点之一，因为在 JS 中不需要使用 PeerConnectionFactory 来创建 PeerConnection 对象。
而在 Android/iOS 开发中，我们使用的 WebRTC 中的大部分对象基本上都是通过 PeerConnectionFactory 创建出来的。下面这张图就清楚的表达了 PeerConnectionFactory 在 WebRTC 中的地位。
PeerConnectionFactory.png824×618 33.8 KB
通过该图我们可以知道，WebRTC中的核心对象 PeerConnection、LocalMediaStream、LocalVideoTrack、LocalAudioTrack都是通过 WebRTC 创建出来的。
PeerConnectionFactory的初始化与构造
在 WebRTC 中使用了大量的设计模式，对于 PeerConnectionFactory 也是如此。它本身就是工厂模式，而这个构造 PeerConnection 等核心对象的工厂又是通过 builder 模式构建出来的。
下面我们就来看看如何构造 PeerConectionFactory。在我们构造 PeerConnectionFactory 之前，首先要对其进行初始化，其代码如下：
PeerConnectionFactory.initialize(...);
初始化之后，就可以通过 builder 模式来构造 PeerConnecitonFactory 对象了。
... PeerConnectionFactory.Builder builder = PeerConnectionFactory.builder() .setVideoEncoderFactory(encoderFactory) .setVideoDecoderFactory(decoderFactory); ... return builder.createPeerConnectionFactory();
通过上面的代码，大家也就能够理解为什么 WebRTC 要使用 buider 模式来构造 PeerConnectionFactory 了吧？主要是方便调整建造 PeerConnectionFactory的组件，如编码器、解码器等。
从另外一个角度我们也可以了解到，要更换WebRTC引警的编解码器该从哪里设置了哈！
音视频数据源
有了PeerConnectionFactory对象，我们就可以创建数据源了。实际上，数据源是 WebRTC 对音视频数据的一种抽象，表示数据可以从这里获取。
使用过 JS WebRTC API的同学都非常清楚，在 JS中 VideoTrack 和 AudioTrack 就是数据源。而在 Android 开发中我们可以知道 Video/AudioTrack 就是 Video/AudioSouce的封装，可以认为他们是等同的。
创建数据源的方式如下：
... VideoSource videoSource = mPeerConnectionFactory.createVideoSource(false); mVideoTrack = mPeerConnectionFactory.createVideoTrack( VIDEO_TRACK_ID, videoSource); ... AudioSource audioSource = mPeerConnectionFactory.createAudioSource(new MediaConstraints()); mAudioTrack = mPeerConnectionFactory.createAudioTrack( AUDIO_TRACK_ID, audioSource); ...
数据源只是对数据的一种抽象，它是从哪里获取的数据呢？对于音频来说，在创建 AudioSource时，就开始从音频设备捕获数据了。对于视频来说我们可以指定采集视频数据的设备，然后使用观察者模式从指定设备中获取数据。
接下来我们就来看一下如何指定视频设备。
视频采集
在 Android 系统下有两种 Camera，一种称为 Camera1, 是一种比较老的采集视频数据的方式，别一种称为 Camera2, 是一种新的采集视频的方法。它们之间的最大区别是 Camera1使用同步方式调用API，Camera2使用异步方式，所以Camera2更高效。
我们看一下 WebRTC 是如何指定具体的 Camera 的：
private VideoCapturer createVideoCapturer() { if (Camera2Enumerator.isSupported(this)) { return createCameraCapturer(new Camera2Enumerator(this)); } else { return createCameraCapturer(new Camera1Enumerator(true)); } } private VideoCapturer createCameraCapturer(CameraEnumerator enumerator) { final String[] deviceNames = enumerator.getDeviceNames(); // First, try to find front facing camera Log.d(TAG, "Looking for front facing cameras."); for (String deviceName : deviceNames) { if (enumerator.isFrontFacing(deviceName)) { Logging.d(TAG, "Creating front facing camera capturer."); VideoCapturer videoCapturer = enumerator.createCapturer(deviceName, null); if (videoCapturer != null) { return videoCapturer; } } } // Front facing camera not found, try something else Log.d(TAG, "Looking for other cameras."); for (String deviceName : deviceNames) { if (!enumerator.isFrontFacing(deviceName)) { Logging.d(TAG, "Creating other camera capturer."); VideoCapturer videoCapturer = enumerator.createCapturer(deviceName, null); if (videoCapturer != null) { return videoCapturer; } } } return null; }
上面代码的逻辑也比较简单：
- 首先看 Android 设备是否支持 Camera2.
- 如果支持就使用 Camera2, 如果不支持就使用 Camera1.
- 在获到到具体的设备后，再看其是否有前置摄像头，如果有就使用
- 如果没有有效的前置摄像头，则选一个非前置摄像头。
通过上面的方法就可以拿到使用的摄像头了，然后将摄像头与视频源连接起来，这样从摄像头获取的数据就源源不断的送到 VideoTrack 里了。
下面我们来看看 VideoCapture 是如何与 VideoSource 关联到一起的：
... mSurfaceTextureHelper = SurfaceTextureHelper.create("CaptureThread", mRootEglBase.getEglBaseContext()); mVideoCapturer.initialize(mSurfaceTextureHelper, getApplicationContext(), videoSource.getCapturerObserver()); ... mVideoTrack.setEnabled(true); ...
上面的代码中，在初始化 VideoCaptuer 的时候，可以过观察者模式将 VideoCapture 与 VideoSource 联接到了一起。因为 VideoTrack 是 VideoSouce 的一层封装，所以此时我们开启 VideoTrack 后就可以拿到视频数据了。
当然，最后还要调用一下 VideoCaptuer 对象的 startCapture 方法真正的打开摄像头，这样 Camera 才会真正的开始工作哈，代码如下：
@Override protected void onResume() { super.onResume(); mVideoCapturer.startCapture(VIDEO_RESOLUTION_WIDTH, VIDEO_RESOLUTION_HEIGHT, VIDEO_FPS); }
拿到了视频数据后，我们如何将它展示出来呢？
渲染视频
Android 下 WebRTC 使用OpenGL ES 进行视频渲染，用于展示视频的控件是 WebRTC 对 Android 系统控件 SurfaceView 的封装。
WebRTC 封装后的 SurfaceView 类为 org.webrtc.SurfaceViewRenderer。在界面定义中应该定义两个SurfaceViewRenderer，一个用于显示本地视频，另一个用于显示远端视频。
其定义如下：
... <org.webrtc.SurfaceViewRenderer android:id="@+id/LocalSurfaceView" android:layout_width="wrap_content" android:layout_height="wrap_content" android:layout_gravity="center" /> <org.webrtc.SurfaceViewRenderer android:id="@+id/RemoteSurfaceView" android:layout_width="120dp" android:layout_height="160dp" android:layout_gravity="top|end" android:layout_margin="16dp"/> ...
通过上面的代码我们就将显示视频的 View 定义好了。光定义好这两个View 还不够，还要对它做进一步的设置:
... mLocalSurfaceView.init(mRootEglBase.getEglBaseContext(), null); mLocalSurfaceView.setScalingType(RendererCommon.ScalingType.SCALE_ASPECT_FILL); mLocalSurfaceView.setMirror(true); mLocalSurfaceView.setEnableHardwareScaler(false /* enabled */); ...
其含义是：
- 使用 OpenGL ES 的上下文初始化 View。
- 设置图像的拉伸比例。
- 设置图像显示时反转，不然视频显示的内容与实际内容正好相反。
- 是否打开便件进行拉伸。
通过上面的设置，我们的 view 就设置好了，对于远端的 Veiw 与本地 View 的设置是一样的，
接下来将从摄像头采集的数据设置到该view里就可以显示了。设置非常的简单，代码如下：
... mVideoTrack.addSink(mLocalSurfaceView); ...
对于远端来说与本地视频的渲染显示是类似的，只不过数据源是从网络获取的。
通过以上讲解，大家应该对 WebRTC 如何采集数据、如何渲染数据有了基本的认识。下面我们再看来下远端的数据是如何来的。
创建 PeerConnection
要想从远端获取数据，我们就必须创建 PeerConnection 对象。该对象的用处就是与远端建立联接，并最终为双方通讯提供网络通道。
我们来看下如何创建 PeerConnecion 对象。
... PeerConnection.RTCConfiguration rtcConfig = new PeerConnection.RTCConfiguration(iceServers); ... PeerConnection connection = mPeerConnectionFactory.createPeerConnection(rtcConfig, mPeerConnectionObserver); ... connection.addTrack(mVideoTrack, mediaStreamLabels); connection.addTrack(mAudioTrack, mediaStreamLabels); ...
PeerConnection 对象的创建还是要使用我们之前讲过的 PeerConnectionFactory 来创建。WebRTC 在建立连接时使用 ICE 架构，一些参数需要在创建 PeerConnection 时设置进去。
另外，当 PeerConnection 对象创建好后，我们应该将本地的音视频轨添加进去，这样 WebRTC 才能帮我们生成包含相应媒体信息的 SDP，以便于后面做媒体能力协商使用。
通过上面的方式，我们就将 PeerConnection 对象创建好了。
与 JS 中的 PeerConnection 对象一样，当其创建好之后，可以监听一些我们感兴趣有事件了，如收到 Candidate 事件时，我们要与对方进行交换。
PeerConnection 事件的监听与 JS 还是有一点差别的。在 JS 中，监听 PeerConnection的相关事件非常直接，直接实现peerconnection.onXXX就好了。而 Android 中的方式与 JS 略有区别，它是通过观察者模式来监听事件的。大家这点一定要注意！
双方都创建好 PeerConnecton 对象后，就会进行媒体协商，协商完成后，数据在底层就开始传输了。
信令驱动
双方交互的过程中，其业务逻辑的核心是信令， 所有的模块都是通过信令串联起来的。
以 PeerConnection 对象的创建为例，该在什么时候创建 PeerConnection 对象呢？最好的时机当然是在用户加入房间之后了 。
下面我们就来看一下，对于两人通讯的情况，信令该如何设计。在我们这个例子中，可以将信令分成两大类。第一类为客户端命令；第二类为服务端命令；
客户端命令有：
- join: 用户加入房间
- leave: 用户离开房间
- message: 端到端命令（offer、answer、candidate）
服务端命令:
- joined: 用户已加入
- leaved: 用户已离开
- other_joined:其它用户已加入
- bye: 其它用户已离开
- full: 房间已满
通过以上几条信令就可以实现一对一实时互动的要求
在本例子中我们仍然是通过socket.io与之前搭建的信令服备器互联的。由于 [socket.io](http://socket.io/) 是跨平台的，所以无论是在 js 中，还是在 Android 中，我们都可以使用其客户端与服务器相联，非常的方便。
下面再来看一下，收到不同信令后，客户端的状态变化：
直播系统客户端状态机.png715×558 27.9 KB
客户端一开始的时候处于 Init/Leave 状态。当发送 join 消息，并收到服务端的 joined 后，其状态变为 joined。
此时，如果第二个用户加入到房间，则客户端的状态变为了 joined_conn， 也就是说此时双方可以进行实时互动了。
如果此时，该用户离开，则其状态就变成了 初始化状态。其它 case 大家可以根据上面的图自行理解。
IM和视频聊天的，可以参考下这个 https://github.com/starrtc/starrtc-android-demo
[https://rtcdeveloper.com/t/topic/14040/1](https://rtcdeveloper.com/t/topic/14040/1)
