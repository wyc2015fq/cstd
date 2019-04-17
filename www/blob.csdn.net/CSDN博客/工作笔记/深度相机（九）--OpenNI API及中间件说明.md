# 深度相机（九）--OpenNI API及中间件说明 - 工作笔记 - CSDN博客





2016年10月05日 11:50:20[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：7364
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









﻿﻿

**本文由官方文档翻译而来**



# 总览

## 目的

OpenNI 2.0 API（应用程序编程接口）提供了访问PrimerSense的兼容深度传感器。这就使得一个应用程序能够初始化传感器和从设备接收深度(depth)，彩图(RGB)和红外(IR)视频流，还提供了一个统一的接口给传感器和通过深度传感器创建.oni记录文件。

OpenNI也提供了第三方中间件开发者可以相互使用深度传感器的统一接口。应用程序还能用第三方中间件（如NITE2），也可以使用直接由OpenNI提供的基础的深度和视频数据。

## 高级API一览

获取深度视频流需要使用4个主要的类（class）。

1.     openni::OpenNI提供了一个静态的API进入点。它提供访问设备，设备相关事件，版本和错误信息。当然，首先得确保你连接了设备。

2.     openni::Device提供了一个传感器设备连接系统的接口（个人理解就是通过Device类来访问控制传感器）。在它创建之前需要对OpenNI类进行初始化。Device可以访问流（Streams）。

3.     openni::VideoStream从一个设备（Device）里提取一个视频流，需要获取视频帧引用（VideoFrameRefs）。

4.     openni::VideoFramRef从相关的源数据里提取一个视频帧。这是从一个特定的流里面获取的。



除了这些主要的类以外，还有许多类和结构体用来保持一些特殊类型的数据。Rocorder类就是用来存储OpenNI视频流到文件的。还有Listener类用来监听OpenNI和Stream类产生的事件。

视频流可以通过两种方式来获取数据：轮询和事件。下面会有具体介绍。

# OpenNI 类

## 简介

首先是最主要组成OpenNI2的是openni::OpenNI，这个类提供了一个API的静态入口。可用来访问系统中所有的设备（传感器设备）。也可以生成许多设备连接和断开事件，和提供以轮询方式访问数据流的功能一样。



## 设备的基础访问

OpenNI类提供了API的静态入口，是在OpenNI::initialize()方法。这个方法初始化所有的传感器驱动并且扫描系统中所有可用的传感器设备。所有使用OpenNI的应用程序在使用其他API之前都应该调用此方法。

一旦初始化方法完成，将可能会设备(Device)对象，并使用这些对象来和真实的传感器硬件进行交互。OpenNI::enumerateDevices()方法会返回一个已经连接上系统的可用的传感器设备列表。

当应用程序准备退出时，必须调用OpenNI::shutdown()方法来关闭所有驱动并且正确地清除所有。



## 视频流(Video Streams)的基础访问

流的轮询访问系统是OpenNI::waitForStream()方法实现的。此方法的参数之一就是流的列表。当方法调用时，就会锁定直到列表中的流有新的可用数据。然后返回一个状态码(status code)并指向是哪个流有可用数据了。



## 设备的事件驱动访问

OpenNI类提供了一个在事件驱动方式（event driven manner）中访问设备的框架。OpenNI定义了3个事件：设备连接事件(onDeviceConnected)，设备断开事件(onDeviceDisconnected)，设备状态改变事件(onDeviceStateChanged)。设备连接事件是在一个新的设备连接并通过OpenNI可用时产生的，设备断开事件是在一个设备从系统中移除时产生的。设备状态改变事件是在设备的设置被改变时产生的。



可以用下列方法从事件处理列表中增添或者移除监听器类(Listener classes)：

OpenNI::addDeviceConnectedListener()//添加设备连接事件监听器

OpenNI::addDeviceDisconnectedListener()//添加设备断开事件监听器

OpenNI::addDeviceStateChangedListener()//添加设备状态改变事件监听器

OpenNI::removeDeviceConnectedListener()//移除设备连接事件监听器

OpenNI::removeDeviceDisconnectedListener()//移除设备断开事件监听器

OpenNI::removeDeviceStateChangedListener()//移除设备状态改变事件监听器



3个事件都提供了一个指针指向OpenNI::DeviceInfo对象。这个对象用来获取被事件提交的设备的细节和标识。此外，设备状态改变事件还提供了一个指针指向DeviceState对象，这个对象被用来查看设备新的状态信息。



事件驱动通过视频流类(VideoStream class)来访问真实的视频流



## 错误信息

在SDK中有许多方法都会返回一个类型为“Status”的值。当错误发生，Status就会包含有一个记录或者显示给用户的代码。OpenNI::getExtendedError()方法会返回更多的关于错误的可读信息。



## 版本信息

API的版本信息由OpenNI::getVersion()方法来获取。这个方法返回应用程序目前使用的API的版本信息。







# 设备类（Device Class）

## 简介

openni::Device类提供了一个物理硬件设备的接口（通过驱动）。也通过一个从物理设备得来的ONI记录文件提供了一个模拟硬件设备的接口。

设备的基本目的是提供流。设备对象被用来连接和配置底层文件或者硬件设备，并从设备中创建流。

注意：这里设备对象是和一整个设备（比如一台kinect或者一台xtion）联系的，而不是具体到某个传感器。

## 设备联系的前置条件(这里的设备联系指的是代码里连接设备获取流，物理上的连接必须在此之前)

在设备类能连接到物理硬件设备前，设备必须在物理上正确地连接到主机，并且驱动必须安装完毕。OpenNI2自带PrimeSense传感器驱动。

如果连接的是ONI文件，那要求在系统运行应用程序时ONI记录文件必须可用，而且应用程序有足够的权限去访问。

当然，也需要在联系设备前openni::OpenNI::initialize()方法被调用。这将会初始化驱动，使API知道设备连接了。

## 基本操作

### 构造函数

设备类（Device class）的构造函数没有参数，也不会联系到物理硬件设备。只是简单地创建对象。

### Device::open()

此方法用来联系到物理硬件设备。open()方法有一个参数，设备的URI（统一资源标识符），方法返回一个状态码指示是否成功。

最简单的用法是用常量openni::ANY_DEVICE作为设备的URI，用这个常量会使系统连接所有的硬件设备。当恰好只有一个设备已连接，这招就会非常有用。

如果多个传感器连接了，那就得先调用OpenNI::enumerateDevices()来获取可用的设备列表。然后找到你要找的设备，通过调用DeviceInfo::getUri()来获取URI，用此方法的输出作为Device::open()的参数，然后就能打开对应的设备了。

如果打开文件，那参数就是ONI文件的路径。

### Device::close()

close()方法用来关闭硬件设备。按照惯例，所有打开的设备必须要关闭。这会分离硬件设备和驱动，这样后面的应用程序连接它们就不会有什么麻烦了。



### Device::isValid()

isValid()方法用来确定设备是否正确地和设备对象联系到了



### 从一个设备中获取信息

可以获取关于设备的基础信息。信息包括名称，供应商，uri，USB VID/PID（usb的id有两部分，供应商id即VID，产品id即PID）。openni::DeviceInfo类就提供了相关信息。每个可用信息都有getter方法，从给定的设备对象里获取DeviceInfo就调用Device::getDeviceInfo()方法



一个设备可能由许多传感器组成。比如一个PrimeSense设备就由IR（红外）传感器，一个color（颜色）传感器和一个depth（深度）传感器组成。流的打开必须基于已经有的传感器。也就是说有什么传感器你才能打开什么流



可以从一个设备中得到传感器列表。Device::hasSensor()方法用来查询设备是否有特定的传感器。传感器类型如下：



SENSOR_IR – The IR video sensor 红外视频传感器
SENSOR_COLOR – The RGB-Color video sensor 彩色视频传感器
SENSOR_DEPTH – The depth video sensor 深度视频传感器



如果要找的传感器可用，Device::getSensorInfo()方法就可以用来获取其信息。SensorInfo提供了传感器类型，包含了视频模式的数组的getter方法。个别视频模式被封装进了VideoMode类。



**特殊设备功能**

### 配准（Registration，我个人更喜欢OpenNI1的叫法，视图替换）

一些设备会同时产生深度流和彩图流。通常这些流由不同的物理摄像头来生成。然而它们在实际上的位置是不同，就造成了它们生成的同一画面是从不同角度得来的。这就使得从同一设备对象不同的流得到的图像有所不同。



两个摄像头之间的几何关系和距离对于设备对象来说都是已知的。这就可以通过数学上的变换来使得两幅图像能够一致。让一个叠加在另一个上。比如彩图的每个像素叠加到深度图上。这个过程就是配准(每一个像素配准叠加到另一张图上)



一些设备能够在硬件进行运算，那么可以校准数据。如果这个功能可用，那硬件上有个标示flag来进行开或关。



设备对象提供了isImageRegistrationSupported()方法来测试已连接的设备是否支持配准功能。如果支持，那getImageRegistrationMode()能用来查询这个功能的状态，setImageRegistrationMode()就可以设置它。openni::ImageRegistrationMode枚举提供了以下值用来set或get：



IMAGE_REGISTRATION_OFF – Hardware registration features aredisabled 硬件配准功能被禁用
IMAGE_REGISTRATION_DEPTH_TO_IMAGE – The depth image is transformed tohave the same apparent vantage point as the RGB image 深度图像被变换叠加至彩图上



需要注意的是两个传感器的可视范围有部分不重叠。这就导致部分深度图不会在显示在结果中。在深度图有毛边的地方的可以看到“影子”或者“孔洞”，由于距离摄像头距离的不同而看起来不同数量的物体被“转移变形（shifted）”了。导致远的物体移动大过近的物体。而它们之间留下了一个没有可用的深度信息的空间。（这一段我也有点迷糊了...）



### 帧同步（FramSync)

当深度和彩图流都可用，那可能两个流会出现不同步，会导致轻微的帧率不同，或者是帧到达时间的轻微不同，即使是帧率相同时。

一些设备提供了使两个帧同步的功能，为了在确定的最大时间范围内分别从获取到两个帧，通常这个最大值都小于两帧间隔。这个功能就是帧同步。

启用或禁用此功能用setDepthColorSyncEnable()。



### 通用功能（General Capabilities)

一些设备有功能设置不同于帧同步和配准。OpenNI 2.0提供了setProperty()和getProperty()方法来激活这些功能。setProperty方法用一个属性Id和值来设置它。getProperty方法则返回对应Id的属性的值。



查阅传感器供应商对于特殊附加属性的支持，及对应数字id和属性的值。



**文件设备（File Devices）**

### 总览

OpenNI 2.0 提供了记录设备输出到文件的功能（记录文件是ONI文件，通常扩展名为.oni）。可以选择记录设备里的所有的流，在录像时保证设置都为使能。一旦录像开始，那么录像就可以作为“文件设备”打开。



打开文件设备和打开设备差不多，都是调用Device::open()，只不过文件设备是用文件路径作为URI。



这个功能在运行调试时非常有用。实时场景很难甚至不能再现，而通过录像功能，就使得同一输入能够用于多个[算法](http://lib.csdn.net/base/datastructure)，调试，性能比较。此功能能用于应用的自动化测试，或者是在一个项目中摄像头不足，测试代码就可以用记录文件来替代。最后，录像还可以使得技术支持通过查看用户摄像头的输出文件找出问题实现远程支持。



重放控制类(PlaybackControl class)用于访问文件设备的特殊功能。查看关于此类的章节获取更多信息。为了促进写出通用目的的处理文件和设备的代码，OpenNI提供了Device::isFile()方法，允许应用在尝试使用重放控制之前确定是文件还是设备。



# 重放控制类(PlaybackControl Class)

## 简介

在处理记录文件（recorded file）时可能会有一系列操作。这些操作包括在流里查找，确定记录有多长，循环播放，改变重放速度。这个功能封装在PlaybackControl类中。

## 初始化（Initializing）

在使用PlaybackControl类之前，必须实例化和从文件初始化一个Device类。一旦一个可用的文件设备被创建，你就可以通过调用Device::getPlaybackControl()获得其中的PlaybackControl对象。Device::IsFile()方法被用来确定一个Device是否从一个文件创建的。

## 查找定位（seek）

提供了两个方法从一个记录中查找定位

PlaybackControl::seek()方法用一个视频流指针（VideoStream pointer）和帧ID（frameID）作为输入，然后重放到指定的帧。如果一个记录中有多个流，那所有的流都会被设置到同样的位置上（定位的位置是指定流指定帧ID的位置）。

PlaybackControl::getNumberOfFrames()方法用来确定这个记录有多长。从根本上确定可用目标来查找很有用。此方法以一个流的指针作为输入，返回指定流所包含的帧的数目。需要注意的是同一记录的不同流可能不同的帧总数。因为真不会一直都同步。

## 重放速度（Playback Speed）

此功能在测试一个有很大输入数据集合的算法时很有效。因为可以更快地得到结果。

PlaybackControl::setSpeed()方法使用一个浮点数作为输入。这个输入值作用于记录制作的多种速度。比如记录是一个30fps的流，然后输入值为2.0，那么重放速度为60fps，如果输入值为0.5，那重放速度为15fps。

设置速度为0.0会导致流播放速度为极限速度（系统能运行的最大速度）。设置速度为-1会导致流变成手动读取，即播放会暂停，卡在这一帧，直到应用程序去去读取下一帧。将记录置为手动模式（manual mode），读取将会很紧密地循环，这就和设置速度为0.0很像。设置速度0.0是因为在用事件驱动模式进行数据读取时很有用。

PlaybackControl::getSpeed()方法会返回最近设置的速度值。



## 循环播放（Playback Looping）

一个物理传感器会继续产生数据无法确定，而记录又只有一定数量的帧。这时用一个记录来模拟一个物理传感器就会有问题，应用程序的代码设计是用来处理物理传感器，因此不能拿来处理记录的结束。

为了解决这个问题，API提供了一个循环播放的方法。PlaybackControl::setRepeatEnabled()方法用来开关循环。设置值为true则重复读取，读完最后一帧又读第一帧。如果设置值为false，那么在记录读取完后导致没有数据帧了。

PlaybackControl::getRepeatEnable()可用来获取当前的重复（repeat）值。



# 视频流类(VideoStream Class)

## 简介

由设备类创建的视频流类封装了所有的数据流。这就使得你可以对数据流进行开始，停止，和配置。也被用来进行流一级（和设备一级相对）的参数配置。

## 视频流的基础功能

### 创建和初始化视频流

调用视频流默认的构造函数会创建一个空的未初始化的视频流对象。在使用前，这个对象必须调用VideoStream::create()进行初始化。而create()方法要求一个已经初始化的设备对象。一旦创建，你就可以调用VideoStream::start()方法来产生数据流。VideoStream::stop()方法则会停止产生数据流。

### 基于轮询的数据读取

一旦视频流创建完毕，就可以直接通过VideoStream::readFrame()方法进行读取数据。如果有新的可用数据了，这个方法就会返回一个可以访问由视频流生成的最新的视频帧引用（VideoFrameRef）。如果没有新的帧可用，那就会锁定直到有新的帧可用。

需要注意的是，**如果非循环地从记录中读取，那么在追后一帧读取完毕后程序将永远卡死在此方法**。



### 基于事件的数据读取

在事件驱动方式下（event driven manner）从视频流中读取数据是可以的。首先，需要创建一个类继承自VideoStream::Listener类，此类应该实现方法onNewFrame()。一旦你创建了这个类，实例化了它，就可以通过VideoStream::addListener()方法来添加监听器。当有新的帧到达，自定义的监听器类的onNewFrame()方法就被调用。然后你就可以调用readFrame()读取了。



## 获取关于视频流的信息

### 传感器信息（SensorInfo）和视频模式（VideoMode）

传感器信息和视频模式类可以一直追踪视频流的信息。视频模式封装了视频流的帧率（frame rate），分辨率（resolution）和像素格式（pixel format）。传感器信息包含了产生视频流的传感器的类型和每个流的视频模式对象列表。通过遍历这个列表，那就能确定传感器生成的流的所有可能的模式。

使用VideoStream::getSensorInfo能够得到当前流的传感器信息对象

### 视野（Field of View）

此功能为确定创建了视频流的传感器的视野范围。使用getHorizonFieldOfView()和getVerticalFieldOfView()方法来确定视野。其返回的值是弧度。

### 像素最大最小值（Min and Max PixelValues）

在深度流中，通常知道一个像素可能出现的最大值和最小值是很有用的。用getMinPixelValue()和getMaxPixleValue()方法就能获取这些信息。

## 配置视频流

### 视频模式（Video Mode）

可以设置给定流的帧率（frame rate），分辨率（resolution）和像素格式（pixel type）。设置这些就要用到setVideoMode()方法。在此之前，你首先需要获取已配置视频流的传感器信息（SensorInfo），然后你才能选择一个可用的视频模式。

### 裁剪（Cropping）

如果传感器支持裁剪，视频流会提供方法来控制它。使用VideoStream::isCroppingSupported()方法来确定是否支持。

如果支持，使用setCropping()来使能裁剪和设置裁剪的具体配置。ResetCropping()方法被用来再次关闭裁剪。getCropping()方法用来获取当前的裁剪设置。

### 镜像（Mirroring） 

镜像，顾名思义，就是使视频流所展现的看起来就像在镜子里一样。启用或禁用镜像，使用VideoStream::setMirroringEnable()方法。设置true为启用，设置false为禁用。可用getMirroringEnable()来获取当前设置。

### 通用属性（General Properties）

在固件层，大多数的传感器设置都存储为地址/值的队（address/value pairs，就是一种键值对）。所以可以通过setProperty和getProperty方法直接操作。这些方法被sdk内部用来实现裁剪，镜像，等等。而它们通常不会被应用程序频繁地使用，因为大多数有用的属性都被更加友好的方法封装了。



# 视频帧引用类（VideoFrameRef Class）

## 简介

视频帧引用类封装了从视频流读取的单个帧的所有的相关数据。是视频流用来返回每一个新的帧。它提供了访问包含了帧数据（元数据，工作所需的帧）基础数组。

视频帧引用对象是从VideoStream::readFrame()方法获取的。

视频帧引用数据可以从红外摄像头，RGB摄像头或者深度摄像头获取。getSensorType()方法用来确定产生此帧的传感器类型。它会返回传感器类型，一个枚举值。

## 访问帧数据

VideoFrameRef::getDate()方法返回一个直接指向帧数据的指针。类型为void，这样每个像素的数据类型才能正确地索引。

## 元数据（metadata）

每个帧都会提供一系列的元数据来促进数据本身的工作。

## 数据裁剪（Cropping data）

数据帧引用知道视频流的裁剪设置，因此可以用来确定裁剪框的原点，裁剪框的大小和帧是否启用裁剪功能。实现方法如下：getCropOriginX()，getCropOriginY(),getCroppingEnable().若启用裁剪功能，则裁剪框大小等于帧大小。所以确定这些的方法和确定帧分辨率的方法是一样一样儿的（东北话）。

## 时间戳（TimeStamp）

每帧数据都有个时间戳。这个值是基于任意0值开始的微妙数。是不同于两帧之间时间差。同一设备的所有流用的都是同一0值，所以时间戳的差值可以用来比较不同流的帧。

OpenNI 2.0中，时间戳的0值是第一帧数据的到达时间。然而这无法保证每次都一样，所以程序代码应该使用时间戳增量。时间戳的值本身不应该用作一种绝对的时间指向。

## 帧索引（FrameIndex）

除了时间戳，帧还提供了连续的帧索引号。这在确定已知的两帧之间有多少帧很有用。如果流使用了同步方法Device::setColorDepthSync()，那相应的帧的帧号应该就是一致的。

如果没有同步，那帧号将不一定匹配。这种情况下，用时间戳来确定相关帧的位置更有效。

## 视频模式（Video Modes）

VideoFrameRef::getVideoMode()用来确定生成当前帧的传感器的视频模式。信息包括像素格式，分辨率，帧率。

## 数据大小（Data Size）

getDataSize()用来确定图像数组中所有数据的大小。在分配存储帧的缓冲区时或者确定帧数时很有用。需要注意的是这是整个数组的数据大小。用VideoMode::getPixelFormat()来确定每个数组元素的大小。

## 图像分辨率（Image Resolution）

getHeight()和getWidth()方法来确定帧的分辨率很容易。这个数据数据也可以通过VideoFrameRef::getVideoMode().getResolutionX()和VideoFrameRef::getVideoMode().getResolutionY()来获取，但不适合频繁调用，因为太低效了。

## 数据有效性（Data Validity）

VideoFrameRef::isValid()方法确定当前视频帧引用是否是有效数据。

在视频帧引用初始化结构体和第一次数据加载之间调用会返回false。

## 传感器类型（Sensor Type）

确定产生数据帧的传感器类型用getSensorType()。方法返回传感器类型，为以下的枚举值：

SENSOR_IR– for an image taken with an IR camera 红外传感器
SENSOR_COLOR – for an image taken with an RGB camera 彩图传感器
SENSOR_DEPTH – for an image taken with a depth sensor 深度传感器

## 数组跨度（Array Stride）

包含帧的数组跨度可以用getStrideBytes()来获取。它将返回数组每行数据的大小，单位字节byte。主要用于索引二维图像数据。

# 记录器类（Recorder Class）

## 简介

记录器类用来记录视频数据到ONI文件中。ONI文件是OpenNI记录深度传感器输出的标准记录文件。包含了一到多个流的信息（如从一个PrimeSense传感器中同时记录一个深度和彩图流）。还包含了设备的设置信息。所以可以用来通过文件实例化设备对象。

## 设置记录器

设置一个记录器有三大步。

一，调用默认构造函数构造一个记录器对象。这不同于其他类的实例化。

二，调用Recorder::creat()方法，参数为记录文件的文件名。创建和写入文件出错时返回一个错误码。

三，提供一个数据流进行记录。使用Recorder::attach()方法来联系上给定的视频流。如果你记录多个流，那就多次调用来联系每个视频流，也就是逐个添加（很明显是写API的人偷懒了）。

## 记录

视频流联系上后，调用Recorder::start()方法开始记录。方法一调用，每帧数据都会被写入ONI文件。通过调用Recorder::stop()方法来结束记录。调用Recorder::destroy()方法来让文件存盘，释放所有内存。

## 重放

ONI可以被许多OpenNI程序和公用程序进行重放。程序打开ONI文件都是作为文件设备打开的。重放控制封装在重放控制类里（Playback Class）。

## 支撑类（SupportClasses）

## 简介

除了OpenNI的主要类外，还有一系列的支撑类。这些类主要服务于封装数据，在其他主要类的章节都有所提及。

## 传感器配置类

### 设备信息（DeviceInfo）

此类记录了设备的所有配置，包括设备名，URI，USB VID/PID描述符和供应商。

### 传感器信息（SensorInfo）

此类存储了传感器的所有配置，这里的传感器仅是三大传感器之一的一个。一个设备有多个传感器。

### 视频模式（VideoMode）

此类存储了分辨率，帧率和像素格式。用于视频流的设置和查看设置，由视频帧引用查看这些设置，由传感器信息提供一个视频模式的列表。

### 摄像头设置（CameraSetting）

存储了RGB摄像头的设置，可以启用或禁用自动白平衡和自动曝光。

## 数据存储类/结构体

### 版本

软件版本。由OpenNI发布。一些应用程序需要适配相应版本的时候使用。

### RGB888Pixel

结构体，存储彩色像素值。

### 数组（Array）

OpenNI提供的简单数组类。包含图像数据。

### 坐标转换（Coordinate Conversion）

坐标转换类用来进行真实坐标和深度坐标的转换。详情请参考API。




补充：

基于OpenNI开发的Middleware介绍  

原文 ：[http://blog.csdn.net/yangtrees/article/details/8707603](http://blog.csdn.net/yangtrees/article/details/8707603)

OpenNI是一个简单高效的Kinect开发的开源框架，它有着简单易用的特点，不少大学和研究机构都选择它作为开发平台，而不选择功能强大的Kinect SDK，在这里给大家介绍几款基于OpenNI开发的Middleware：



### 1.      NITE2:

提供完整的手势控制与身体控制API，手势定位和跟踪；人体骨架跟踪……

优点：占用CPU少，简单灵活，多平台支持（Windows、[Linux](http://lib.csdn.net/base/linux)、Mac OS and Andorid）

![](https://img-my.csdn.net/uploads/201303/22/1363948291_1416.JPG)


[](http://www.openni.org/wp-content/uploads/TEST.png)[](http://www.openni.org/wp-content/uploads/test3.png)

       url：[http://www.primesense.com/solutions/nite-middleware/](http://www.primesense.com/solutions/nite-middleware/)



### 2.      3D Hand Tracking Library

提供完整的3D手势跟踪框架的API

优点：无需任何标记跟踪3D手势的位置、方向、关节。

缺点：平台要求较高，

Presequisites：Windows7 64bit or Ubuntu 12.04 64bit workstation. NVidia GPU withCUDA  support.



![](https://img-my.csdn.net/uploads/201303/22/1363948295_9146.JPG)


url:[http://cvrlcode.ics.forth.gr/handtracking/](http://cvrlcode.ics.forth.gr/handtracking/)



### 3.      SigmaNIL Framework

提供非常强大的自然用户交互借口，包括：包括手势重建（能达到手指精度）、手势识别、手势骨架跟踪。

优点：完全开源，框架完整

Presequisites： OpenNI与NITE或KinectSDK，一些module与sample需要Boost、[OpenCV](http://lib.csdn.net/base/opencv)、CUDA Toolkit，sample需要Qt。



![](https://img-my.csdn.net/uploads/201303/22/1363948300_3972.JPG)


url：[http://www.sigmanil.com](http://www.sigmanil.com/)



### 4.      TipTep Skeletonizer

根据手势的深度图提供手势的几何骨架。

Presequisites：Windowsx86/x64, .NET 4.



![](https://img-my.csdn.net/uploads/201303/22/1363948305_8608.JPG)


url：[http://tiptep.com/index.php/products](http://tiptep.com/index.php/products)



### 5.      3D Face Identification

原理：1.利用OpenCV的人脸检测在RGB图像中定位人脸；2.将人脸区域的深度数据转化为提前规范好的深度数据；3.将探测出的数据与[数据库](http://lib.csdn.net/base/mysql)中的数据进行比对。

优点：The program提供探测和识别的基本函数集可以供二次开发。Real-time。

Reference：[1] R. Min, J. Choi, G. Medioni, J.-L. Dugelay, “ Real-Time 3D FaceIdentification from a Depth Camera”, 21st International Conference on PatternRecognition (ICPR), November 11-15, 2012.

Presequisites：支持GPU编程的显卡。



![](https://img-my.csdn.net/uploads/201303/22/1363948310_5608.JPG)


url: [http://iris.usc.edu/USC-Computer-Vision.html](http://iris.usc.edu/USC-Computer-Vision.html)



### 6.      Volumental

Volumental是一种最快捷和简便的方式，他能将扫描和分享任何你想到的对象包括：人、对象或房间。Volumental为你的浏览器提供一个插件来连接你的深度摄像机，使用专门的硬件和[算法](http://lib.csdn.net/base/datastructure)在云端重建3D模型。

Presequisites：Windows平台. Verified to work with Safari, Firefox and Chrome

![](https://img-my.csdn.net/uploads/201303/22/1363948316_6136.JPG)




url：[http://www.volumental.com/](http://www.volumental.com/)



### 7.      KScan3D Middleware

KScan3D提供完整的3D扫描数据的提取和处理方案，包含一个鲁棒性好的API和[LUA-based](http://baike.baidu.com/view/416116.htm)自动化脚本。集成的脚本编辑器提供了一个基于gui控制台日志记录输出和调试脚本的语法方式。还可以通过API dll为你的项目提供来自KScan3D的功能，包括：扫描、对齐、网格处理。

Presequisites：Windows 7 or8（64-bit recommended）

![](https://img-my.csdn.net/uploads/201303/22/1363948322_9021.JPG)




url: [http://www.kscan3d.com/](http://www.kscan3d.com/)

KScan3D与SDK 1.7的Kinect Fusion的功能比较接近，但是只能免费使用14天。




### 8.      VIIM SDK V2.0.0 Beta

在OpenNI与NITE的基础上简化功能，增加高层特征。VIIM SDK的目标是为每位程序员提供更通用的[NUI](http://baike.baidu.com/view/3149156.htm)，它实际上就是增强版的OpenNI/NITE.

Presequisites：Windows 7 orlater ; x64/x86;

![](https://img-my.csdn.net/uploads/201303/22/1363948326_6721.JPG)




url: [http://www.viim.pt/](http://www.viim.pt/)



### 9.      GST API

GST(GlobalSensing Technologies)提供的非常简单的程序识别项目，GSTSkeletonPostures能够非常流畅的标记用户动作。

Presequisites：Windows 7 32or 64 bits, the last corresponding version of OpenNI2 and NiTE2 librariesinstalled. Project have been made using Microsoft SDK 7.1 and Microsoft visualC++ express 2010.



![](https://img-my.csdn.net/uploads/201303/22/1363948332_7444.JPG)


url：[http://globalsensing.eu/](http://globalsensing.eu/)



### 10.  Motion Nexus Plugin

Motion Nexus Plugin通过OpenNI为Flash开发者提供工具和环境来建立真实的浸入式和交互式游戏与应用。

![](https://img-my.csdn.net/uploads/201303/22/1363948337_2381.JPG)




url：[http://www.motionnexus.com/openni.html](http://www.motionnexus.com/openni.html)



### 11.  Starry Night Feature ExtractionDemonstration SDK

特征提取SDK允许用户简单地提供（1）所需的感兴趣对象的模型，和（2）一种包含该对象的场景的三维扫描。该软件识别和定位对象在三维扫描的场景。凡高的星夜证明了这个SDK的特征提取能力，只是实时的三维物体识别和分析软件所提供的许多功能之一。

Presequisites：PC running Windows 7 with OpenNI 2.0 installed, Carmine sensor

![](https://img-my.csdn.net/uploads/201303/22/1363948342_5386.JPG)




url：[http://www.vangoghimaging.com/3d     _scanning_imaging_products.html](http://www.vangoghimaging.com/3d_scanning_imaging_products.html)







这些程序其实开源程度不一样，大家可以移步：http://www.openni.org/做深入了解~

﻿﻿



