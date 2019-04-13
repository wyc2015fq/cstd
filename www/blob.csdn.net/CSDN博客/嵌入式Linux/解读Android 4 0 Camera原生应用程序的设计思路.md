
# 解读Android 4.0 Camera原生应用程序的设计思路 - 嵌入式Linux - CSDN博客

2015年05月04日 14:48:03[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：815


# 解读Android 4.0 Camera原生应用程序的设计思路
一篇很不错的文章：[http://my.oschina.net/jerikc/blog/90791](http://my.oschina.net/jerikc/blog/90791)1. 设置摄像头方向
2. 打开线程与预览线程
3. 设置参数
4. Camera外设按键
5. 自动对焦与触摸对焦
6. 拍照
7. 人脸检测
8. 位置管理
9. 旋转管理
10. 变焦
11. 录像
![](https://img-my.csdn.net/uploads/201209/11/1347340776_4077.PNG)

Camera的架构为典型的C/S架构，Client端，用户的行为，是为应用程序进程，Server端，设备的功能，是为Camera服务守护进 程，客户端进程承载用户的需求，由Binder进程间通信送往服务端实现设备的功能，服务端由回调函数和消息机制反馈数据返还给用户。
ps查看进程，类似 com.android.camera是为客户端Camera进程，/system/bin/mediaserver是为服务端守护进程，由系统启动时开启。

1. 设置摄像头方向
Framework框架层的Camera对象(camera.java)里有一个类class CameraInfo，里面存放了两个公有成员变量facing和orientation，即我们要讨论的前后置和方向。 程序第一次初始化时initializeFirstTime()，通过getCameraInfo()得到前后置和方向的信息，客户端发送请求 getCameraInfo()询问服务端，服务端调用抽象层拿数据，抽象层参考底层camera sensor驱动的数据facing和orientation，这两个值在驱动里是写死的，不能由用户改变，camera程序启动以后就把它们作为全局变
 量存放起来。
1.1 前置与后置
后置back camera背对手机屏幕，朝外，像素高，前置front camera，面对自己，朝内，像素低。
1.2 方向
摄像头模组有长边和短边，比如采集图像的比例为4:3，那么4为长边，3为短边。设备屏幕也有长边和短边，理论上，摄像头模组的长边不能与屏幕的长边垂直，至于为什么呢，我语文水平太差，没办法很好地表达出来...总之目的就是为了显示效果。

2 打开线程与预览线程
onCreate()里会先后开启CameraOpenThread和CameraPreviewThread。
打开camera还需要线程？CameraOpenThread名为打开，实为C/S connect连接服务端，binder进程间通信，开销较大。预览线程必须在打开线程完成以后执行，它贯穿始终直到进程消亡为止，整个预览过程相对复 杂，在抽象层和底层驱动实现，概括讲，预览线程再开启两个线程，一个拿sensor的frame，一个送往framebuffer经 surfaceflinger显示出来。

3 设置参数
预览拍照录像之前，用户需要设置很多参数，比如闪光，白平衡，场景，对比度等。
程序里这些参数保存在SharedPreferences共享优选项和Parameters两个地方，Preferences包含 Parameters，打开程序读取优选项参数，关闭程序保存优选项参数，考虑到用户经常会调整参数，引入Parameters来保存从打开以后到关闭以 前这个中间过程的参数变量，Parameters的键值由抽象层根据硬件sensor的能力决定。

4. Camera外设按键
Manifest里注册broadcast receiver，
<receiver android:name="com.android.camera.CameraButtonIntentReceiver">
<intent-filter>
<action android:name="android.intent.action.CAMERA_BUTTON"/>
</intent-filter>
</receiver>
有些手机上有camera按键，用户按下按键，android输入系统有两种实现方法，
1）发送广播CAMERA_BUTTON，收到广播后开启主Activity。
2）上报键值KEYCODE_CAMERA，程序收到消息，可自定义实现功能，比如拍照。
public boolean onKeyDown(int keyCode, KeyEvent event) {
switch (keyCode) {
case KeyEvent.KEYCODE_CAMERA:
if (mFirstTimeInitialized && event.getRepeatCount() == 0) {
onShutterButtonClick();
}
return true;

5. 自动对焦与触摸对焦
外界事物由光线经凸透镜聚焦到sensor上成像，camera模组开启马达前后平移镜头取得最佳成像效果，这个过程称之为对焦。
5.1 自动对焦
1) camera模组会因感光强度变化而自动开启对焦，驱动控制。
2) 用户长按快门，软件控制自动对焦。
3) 用户按下快门拍照，拍摄前自动对焦。
程序里，Camera对象实现类ShutterButton的接口OnShutterButtonListener里的方法 onShutterButtonFocus(boolean pressed)和onShutterButtonClick()，后者是拍照，下节讨论，先看 onShutterButtonFocus(boolean pressed)，这个pressed判断是否为一次有效的长按，如果是的话，执行autoFocus()，这个autoFocus()也是Camera 对象实现类FocusManager的接口Listener里的方法，由binder交给camera
 service，最后在底层驱动实现自动对焦。
5.2 触摸对焦
自动对焦的对焦区域位于屏幕正中，用户也可触摸特定区域对焦。
Camera对象实现类View的接口OnTouchListener里的方法onTouch()，输入系统上报MotionEvent的xy坐标，保存在Parameters，执行autoFocus()，抽象层读取Paramters的触摸点坐标，实现区域对焦。

6. 拍照
拍照分四步，对焦，拍照，接收图片，保存图片。
mCameraDevice.takePicture(mShutterCallback, mRawPictureCallback,
mPostViewPictureCallback, new JpegPictureCallback(loc));
1）对焦
拍照前如果已经区域对焦，则取消自动对焦，反之，开启一次自动对焦。对焦完成后，底层发送对焦成功与否的消息给camera对 象，FocusManager把状态mState保存起来，如果正在对焦未完成(mState == STATE_FOCUSING)则不可拍照，直到对焦完毕。
2）拍照
onShutterButtonClick() -> doSnap() -> capture() -> takePicture()，具体实现在抽象层和底层驱动，实质就是拿一张预览的图像，抽象层读取拍照时的Parameters参数配置，包括用户选择的 照片大小。
3）接收图片
通过回调，由底层发送图片给camera对象。
RawPictureCallback，得到原始图片，需要软件压缩Jpeg。（YUV转Jpeg）
JpegPictureCallback，直接得到Jpeg图片，需要硬件压缩Jpeg。
PostViewPictureCallback，拍完后预览图片。
4）保存图片
交由线程ImageSaver保存图片和生成thumbnails。
默认路径位于/mnt/sdcard/DCIM/Camera/

7. 人脸检测
人脸检测可以软件实现，可以硬件实现，软件实现增加CPU开销，硬件实现增大耗电，鼓励硬件实现...
上层Camera对象实现了 framework层Camera的接口FaceDetectionListener的方法onFaceDetection(Face[] faces, Camera camera)，回调机制同上，硬件sensor识别脸部信息，发送face给camera对象，framework定义face的特征，比如眼睛，嘴 巴，上层保存mFaces数据，更新UI。

8. 位置管理
位置管理LocationManager用来记录拍摄图片的GPS位置信息（经维度），存入JPEG头部插入的Exif里。
用户在菜单“相机设置”里的"保存所在位置"选择打开（前提是GPS已开启），屏幕左上方会出现一个GPS图标，表示现在可以记录GPS信息了。
程序里，Camera对象实现了位置管理监听器LocationManager.Listener的接口showGpsOnScreenIndicator()和hideGpsOnScreenIndicator()，显示或者隐藏GPS图标。
程序第一次初始化时initializeFirstTime()，通过读取优选项Preference得到bool值 recordLocation，判断是否需要记录GPS信息，如果需要，在拍照capture()里调用LocationManager的方法得到 Location loc，并将其存入Exif。

9. 旋转管理
假设一台手机，camera正常安装，竖直方向作为默认方向(orientation == 0)拍摄照片，即拍摄“肖像照”(portrait)，得到的照片显示在屏幕上也是竖直方向。
如果把手机旋转90度水平过来拍摄“山水照”(landscape)，对于camera sensor来说，没有旋转的概念，所以软件上要把图片旋转90度回来。
![](https://img-my.csdn.net/uploads/201209/13/1347520901_9805.PNG)
![](https://img-my.csdn.net/uploads/201209/13/1347520930_6557.PNG)
软件上，需要借助方向监听器随时更新方向信息，并保存在Parameters里，抽象层实现拍照功能时从Parameters里读取方向。
具体的，camera对象内部类MyOrientationEventListener的方法onOrientationChanged()保存方 向orientation的值，MyOrientationEventListener继承 OrientationEventListener，OrientationEventListener的onSensorChanged()把从 sensorManager拿到的xyz坐标转换成方向。
程序启动，注册sensor监听器并使能，sensorManager不断上报底层sensor数据，通过消息机制发送到camera对象，后者计 算坐标数据得到方向orientation的值(实际外包给orientationListener完成)，最后保存Parameters。

10. 变焦
用户拖动Zoom横条可放大缩小预览画面连续变焦，另一种所谓状态变焦，其原理是一样的。
camera对象的内部类ZoomChangeListener实现ZoomControl的方法，实质是把变焦的任务全权交给 ZoomControl。ZoomControl监听处理用户的触摸事件dispatchTouchEvent()，用来得到并处理变焦倍数 mListener.onZoomValueChanged(index)，它由mCameraDevice.startSmoothZoom()通过 binder交给camera service，camera service再通过sendComand命令机制交给抽象层实现变焦，抽象层开启变焦线程，变焦改变预览，通过回调机制发送消息
 CAMERA_MSG_ZOOM把变焦倍数返还给camera对象，最终camera对象收到消息 后，ZoomListener.onZoomChange()把变焦倍数保存到Parameters.

11. 录像
ModePicker负责切换模式，一共有三种模式，普通模式，录像模式和全景模式，Manifest里依次声明这三个activity。
切换模式，销毁原有activity，开启新activity，伴随关闭preview，重启preview，保存配置，读取配置，开销很大。
录像VideoCamera.java同预览Camera.java的思路类似，按下录像按钮，程序监听用户事件，开启录像，录像交给MediaRecoder,StagefrightRecorder负责。
1. Overview
1.1 物理架构
1.2 Android架构
2. CameraService
3. HAL
4. Overlay
5. Video for Linux
1. Overview
本文以Freescale IMX为例剖析camera摄像头的系统架构。
1.1 物理架构
硬件方面，camera系统分为主控制器和摄像头设备，功能上主要有preview预览，takePicture拍照和recording录像。
![](http://hi.csdn.net/attachment/201103/16/0_13002748120774.gif)
1) IPU - Image Process Unit 图像处理单元，用于控制摄像机和显示屏。
2）图像采集 - 由camera采集的图像数据信息通过IPU的CSI接口控制。
3）DMA映射到内存 - IPU将采集到得数据通过DMA映射到一段内存。
4）队列机制 - 为了更高效地传送数据，将内存中的数据取出加入一队列，并传送到另一队列。
5）视频输出 - 将视频数据从队列中取出，通过IPU控制这段独立显存，最终将视频显示出来。
1.2 Android架构
![](http://hi.csdn.net/attachment/201103/15/0_1300200060imsm.gif)
Android的camera系统架构自上而下分别为应用层-框架层-硬件抽象层-linux驱动层。
1) APP - Framework
应用层与java框架层的间主要由Binder机制进行通信。
系统初始化时会开启一个CameraService的守护进程，为上层应用提供camera对的功能接口。
2) Framework - HAL
框架层与硬件抽象层间通过回调函数传递数据。
3) Overlay
Overlay层由ServiceFlinger和OverlayHal组成，实现视频输出功能，只有camera框架层或者视频框架层能调用它，上层无法直接调用。
4) HAL - driver
抽象层位于用户空间，通过系统调用如open(),read(),ioctl()等与内核空间进行数据传递。
2 CameraService
Camera的主要功能有取景Preview，拍照takePicture和摄影Recording，下文以取景为例，剖析camera系统架构。
![](http://hi.csdn.net/attachment/201103/16/0_1300275344E2z7.gif)
要实现取景Preview功能，主要须调用CameraService::Client::startPreview()和 CameraService::Client::setOverlay()，前者通过mHardware->startPreview();调用 cameraHal硬件抽象层以实现取景的整个流程，后者通过mSurface->createOverlay();调用 surfaceFlinger层创建overlay_object对象。
3 HAL
![](http://hi.csdn.net/attachment/201103/16/0_1300275721e762.gif)
startPreview主要完成三项任务，配置图象，配置内存，开启两个存取buf队列的线程。
1) cameraPreviewConfig()配置预览图象参数
CameraOpen() - 通过打开设备节点/dev/video0得以由系统接口与设备驱动交互。
S_FMT - ioctl()的指令，设置图象像素格式，将数据由硬件抽象层传递至Linux驱动，这里也就是v4l2。
G_FMT - 得到图象像素格式，将数据由底层驱动v4l2返回至硬件抽象层。
S_PARM - 设置模式的指令，这个指令传到底层后，将会实现对camera硬件的控制。
2) cameraPreviewStart()开启预览，实际上配置了内存
REQBUFS - 申请内存，通过dma_alloc_coherent()为camera申请一端连续的dma内存。
QUERYBUF - 询问内存，将申请到内存的物理地址，虚拟地址等数据从内核空间传递到用户空间。
QBUF - 加入队列，将通过询问得到的buf加入一个队列。
3) PreviewShowFrameThread()和PreviewShowFrameThread()
PreviewCaptureFrameThread()捕捉一帧数据的线程，通过DQBUF，从队列中取出一个buf数据，这里，一个buf即一帧数据即一张图片。注意，如果camera没有采集到图片，这个线程会在DQBUF阻塞。
PreviewShowFrameThread()显示一帧数据的线程。
![](http://hi.csdn.net/attachment/201103/17/0_1300361657RRNn.gif)
mDataCb() - 回调函数，将采集到的图象数据传回CameraService，再由CameraService传递给上层应用。
mOverlay->dequeueBuffer() - 调用Overlay层，从Overlay层得到一个空闲的overlaybuffer，将图象数据拷贝到这个buffer里。至于这个buffer后续的工作，即视频输出，则交给了Overlay去完成。
QUERYBUF & QBUF - 由于已经从队列里取出了一个buf，需要再询问并加入另一个buf到队列里。
4) Overlay
![](http://hi.csdn.net/attachment/201103/16/0_1300276335f1r1.gif)
CameraService::Client::startPreview()完成mHardware->startPreview();后 便去执行CameraService::Client::setOverlay()，如果没有任何overlay，则创建一个新的，通过 mHardware->setOverlay(new Overlay(mOverlayRef))调用到SurfaceFlinger层，再由 overlay_dev->createOverlay();调用到overlay的硬件抽象层，抽象层创建并初始化overlay对象，与
 cameraHal类似，通过ioctl()指令与底层v4l2通信，配置视频参数和内存空间。随后开启一个overlay线程，用于存取队列中的视频数 据。
注意，SurfaceFlinger里也会开启一个处理overlay的surfaceFlinger线程，用于等待用户事件，作相应的overlay控制。
![](http://hi.csdn.net/attachment/201103/17/0_1300362583CVuu.gif)
5 Video for Linux
![](http://hi.csdn.net/attachment/201103/16/0_1300276482ceVR.gif)
v4l2 - video for linux 2是linux为视频驱动抽象出的一层统一的接口，数据结构如下，
v4l2作为master主设备由(*attach)与camera从设备进行绑定。
![](http://hi.csdn.net/attachment/201103/19/0_1300519979XHTc.gif)
初始化函数probe()如下，
![](http://hi.csdn.net/attachment/201103/19/0_1300519640223f.gif)
1) init_camera_struct()初始化v4l2主设备的数据结构，实现open(), read(), ioctl(), mmap()等操作。
2) v4l2_int_device_register()，注册v4l2主设备，绑定camera从设备。
3) video_register_device()注册linux video设备，建立/dev/video0设备节点。


