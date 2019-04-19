# Android Camera 架构简析 - xqhrs232的专栏 - CSDN博客
2011年06月17日 13:58:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：784标签：[android																[框架																[硬件驱动																[server																[通讯																[平台](https://so.csdn.net/so/search/s.do?q=平台&t=blog)](https://so.csdn.net/so/search/s.do?q=通讯&t=blog)](https://so.csdn.net/so/search/s.do?q=server&t=blog)](https://so.csdn.net/so/search/s.do?q=硬件驱动&t=blog)](https://so.csdn.net/so/search/s.do?q=框架&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://rnmichelle.blog.51cto.com/2729211/491743](http://rnmichelle.blog.51cto.com/2729211/491743)
**1.前言**
最近整体看了一下Android Camera
部分代码，从Framework的Camera 
应用框架到 Hardware的Camera HAL 再到 Kernel的 Camera Driver。把对Camera部分的学习整理成笔记，方便理解。
**2.概述**
要了解Android Camera架构，还是得先从Android本身的架构说起。[](http://img1.51cto.com/attachment/201102/111309223.jpg)
![](http://img1.51cto.com/attachment/201102/111553209.jpg)
图1. Android
架构图
从Android框架看，分为四层：应用层、应用框架层、库层、内核层。
应用层主要工作在Android SDK之上，利用Android
提供的API进行开发，生成APK包。
应用框架层整合了Android自带各种控件和类，为应用开发提供高效、方便的API接口。这个对上实现统一的接口，对下也提供统一的标准方便各种库的移入。
库层是Android与底层硬件通信接口，它封装底层硬件接口实现该模块的具体逻辑，并以服务的形式通过Binder通讯机制暴露给应用框架。
内核层则是直接与硬件联系的一层，可以理解为设备驱动。
通过对上面框架的简要说明，我们大致明白一个应用自上而下的各个层起什么作用，这样有助于我们对代码的理解和修改。如果要做基于Android porting工作，就要明白在哪个层次的porting，怎么porting。
**3.Camera层次简析**
说完Android整体框架，我们以Camera为例来说明一下Camera在各层是如何实现。下图是Camera部分在Android上各层的分布图
![](http://img1.51cto.com/attachment/201102/111709557.jpg)
[](http://img1.51cto.com/attachment/201102/111513589.jpg)
图2. Camera结构分布图
l**应用层**
Camera的应用层在Android上表现为直接调用SDK API开发的一个Camera
应用APK包。代码在/android/packages/apps/Camera下。主要是Java写的基于android.hardware.Camera类调用的封装，并且实现Camera应用的业务逻辑和UI显示。android.hardware.Camera就是Android提供给上层调用的Camera类。这个类用来连接或断开一个Camera服务，设置拍摄参数，开始、停止预览，拍照等。它也是Android
 Camera应用框架封装暴露出来的接口。一个Android应用中若要使用这个类，需要在Manifest文件声明Camera的权限，另外还需要添加一些<uses-feature>元素来声明应用中的Camera特性，如自动对焦等。具体做法可如下：
<uses-permissionandroid:name="android.permission.CAMERA"/><uses-featureandroid:name="android.hardware.camera"/><uses-featureandroid:name="android.hardware.camera.autofocus"/>
l**应用框架层**
Camera框架层将应用与底层的实现隔离开来，实现了一套Android定义的对上对下接口规范，方便应用及底层硬件的开发和移植。这一层对上以Java类的形式包装出android.hardware.Camera，提供给应用层调用；对下在CameraHardwareInterface.h头文件中定义了Camera硬件抽象层的接口，这是一个包含纯虚函数的类，必须被实现类继承才能使用。这个实现类也即是下层中将讲到的用户库层，它继承CameraHardwareInterface接口，实例化对底层硬件驱动的封装，最终生成libcamera.so供框架的libcameraservice.so调用。这样做的好处是让Camera的应用框架代码独立，不受底层硬件驱动改变的影响，方便在不同平台上porting
驱动代码，而保持上层的代码不用变化。
从代码上看，这一层包含Java到JNI到C++的代码。源代码主要在以下路径：
/android/frameworks/base/core/java/android/hardware/Camera.java
这个类作为Android SDK Camera部分提供给上层应用，并通过JNI的方式调用本地C++代码。
/android/frameworks/base/core/jni/android_hardware_Camera.cpp
Camera 的JAVA本地调用部分，是承接JAVA代码到C++代码的桥梁。编译生成libandroid_runtime.so。
/android/frameworks/base/libs/ui
包含文件：
Camera.cpp
CameraParameters.cpp
ICamera.cpp
ICameraClient.cpp
ICameraService.cpp
它们的头文件在/android/frameworks/base/include/ui目录下。
这部分的内容编译生成libui.so。在Camera模块的各个库中，libui.so位于核心的位置，作为Camera框架的Client客户端部分，与另外一部分内容服务端
libcameraservice.so通过进程间通讯（即Binder机制）的方式进行通讯。
/android/frameworks/base/camera/libcameraservice
CameraService是Camera服务，Camera框架的中间层，用于链接CameraHardwareInterface
和 Client，它通过调用实际的Camera硬件接口来实现功能。这部分内容被编译成库libcameraservice.so。
libandroid_runtime.so和libui.so两个库是公用的,其中除了Camera还有其他方面的功能。整个Camera在运行的时候，可以大致上分成Client和Server两个部分，它们分别在两个进程中运行，它们之间使用Binder机制实现进程间通讯。这样在client调用接口，功能则在server中实现，但是在client中调用就好像直接调用server中的功能，进程间通讯的部分对上层程序不可见。
以上应用层到框架层的分析可以用下图来表示调用流程。
![](http://img1.51cto.com/attachment/201102/113339175.jpg)
图3. Camera调用流程图
当Camera Client端通过Binder机制与Camera Server端通讯，Server端的实现传递到Client端。而Server端的实现又是调用硬件接口来实现。这就延伸到我们要介绍的Camera的下一个层次――库层。
st1/:*{behavior:url(#ieooui) } 
l**库层（硬件抽象层HAL Hardware Abstraction Layer）**
这个层次其实就是用户空间的驱动代码。前面有介绍过框架层对下在CameraHardwareInterface.h头文件中定义了Camera硬件抽象层的接口，它是包含纯虚函数的类，必须被实现类继承才能使用。HAL层正好继承CameraHardwareInterface接口，依据V4l2规范实例化底层硬件驱动，使用ioctl方式调用驱动，最终生成libcamera.so供框架的libcameraservice.so调用。
这层的代码在/android/hardware/XXX/libcamera目录下（也有可能在vendor目录中对应的libcamera下）。注意这里的XXX是不同厂商为不同产品（板子）而建的目录，以高通msm平台为例，这里XXX用msm7k表示，这样高通msm平台下这个HAL的目录即为/android/hardware/msm7k/libcamera。不难看出，如果要在某硬件平台上运行Android，也就主要在这一层进行修改，因为它是直接和底层硬件驱动相关的。上面也讲过，应用框架层对上对下都定义的标准接口，这样做的目的也就是使上层的代码独立，在porting中不受影响。所以我们现在可以基本确定，如果要改Camera的硬件，框架层以上的部分都可以不动，要改就改HAL到内核层的部分，这也是Android底层开发的主要工作。这个在后续的文章中会具体介绍。st1/:*{behavior:url(#ieooui)
 } 
l**内核层**
这一层主要是基于Linux的设备驱动。对Camera来说，一般是按V4l2规范将Camera原子功能以ioctl的形式暴露出来供HAL层调用的实现。主要功能的实现代码在/android/kernel/drivers/media/video/XXX下。跟HAL层目录一样，XXX是不同厂商不同平台的目录，以高通msm平台为例，这个目录就是/android/kernel/drivers/media/video/msm。所以要在Android平台上添加硬件功能，首先考虑将它的驱动加到Android的Linux内核中。
**后记**
      至此，已将Camera各层在Android中的分布做了简单介绍。依此类推，Android上其他模块也跟Camera一样有类似的结构分层，如MediaPlayer。在了解了这样的结构后，对我们开发有一定的指导意义，使我们清楚如果对已有模块的功能添加和修改一般应在哪些个层次进行，哪些代码不用改，哪些代码要改。
      鉴于本人水平有限，如有写的错误或不周全的地方，请包含。
