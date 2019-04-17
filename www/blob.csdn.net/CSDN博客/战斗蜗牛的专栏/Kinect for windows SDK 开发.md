# Kinect for windows SDK 开发 - 战斗蜗牛的专栏 - CSDN博客





2012年07月20日 14:19:11[vbskj](https://me.csdn.net/vbskj)阅读数：5546








# [[译]Kinect for Windows SDK开发入门(一)：开发环境配置](http://www.cnblogs.com/yangecnu/archive/2012/03/30/KinectSDK_Geting_Started.html)






前几天无意中看到微软发布了Kinect for windows sensor，进去看了一下Kinect应用的例子，发现Kinect除了作为xbox360游戏的外设外还能开发一些很酷的应用，而且微软也发布可针对Kinect开发的Kinect for windows SDK1.0正式版本，原本想买一个Kinect for windows sensor来进行开发玩一玩的，可是那个出来没多久，淘宝上只有代购的，而且比Kinect for Xbox360 sensor贵了很多，而且只能进行开发，考虑到以后开发完了，想玩Xbox的话直接就再买个主机，连上这个就可以玩体感游戏了，所以在淘宝上买了一个Kinect
 for Xbox360 sensor，这两个sensor的最大区别是for windows版本的有近景模式，能够进行近景识别。


首先来看一下Kinect设备：



#### 1. Kinect设备


黑色的Kinect设备如下图：基座和感应器之间有一个电动的马达，通过程序能够调整俯仰角度，在上面的感应器中有一个红外投影仪，两个摄像头，四个麦克风和一个风扇。打开外面的盖子可以看到里面的构造：这些感应器用来捕捉RGB和深度数据，面对Kinect，从左往右看。最左边是红外光源，其次是LED指示灯，再次是彩色摄像头，用来收集RGB数据，最右边是红外摄像头用才采集景深数据。彩色摄像头最大支持1280*960分辨率成像，红外摄像头最大支持640*480成像。

![image_thumb3](http://images.cnblogs.com/cnblogs_com/yangecnu/201203/201203301759593730.png)![image_thumb5](http://images.cnblogs.com/cnblogs_com/yangecnu/201203/201203301800006729.png)


在感应器的下方是麦克风阵列，他包括四个不同的麦克风，一个在左边的红外发射器下面，另外3个在右边景深摄像头下面。


初步了解了Kinect构造后，接下来看看开发环境的搭建：



#### 2. Kinect for Windows SDK软硬件环境


Kinect for Windows SDK是一些列的类库，他能够使得开发者能够将Kinect作为输入设备开发各种应用程序。就像名字所显示的那样，Kinect for Windows SDK只能运行在32位或者64位的windows7及以上版本的操作系统上。
- 
硬件配置
- 
一台电脑，当然内存越大越好(推荐2G或者以上)，处理器频率(2.66GHz及以上)越高越好。
- 
Windows 7操作系统，以及支持Microsoft DirectX9.0c兼容的显卡
- 
Kinect for Xbox360 Sensor（如果单买的话，自带有USB Power Adapter，如果是和Xbox360套装一起捆绑的话，因为Xbox主机能够直接为Kinect外设提供电源，所以没有电源，可能需要另买USB Power Adapter）


使用Visual Studio 2010 Express版本或者专业版进行开发时，需要安装最新版本的Kinect for Windows SDK,SDK中包含有对Kinect的硬件驱动。
- 
软件配置


1. Visual Studio 2010 Express或者Visual Studio 2010专业版或其他版本


2. .NET Framework 4.0


3. Kinect for Windows SDK,最新版本为1.0版本，下载地址：[http://www.microsoft.com/en-us/kinectforwindows/develop/overview.aspx](http://www.microsoft.com/en-us/kinectforwindows/develop/overview.aspx)



#### 3. 安装步骤


安装SDK之前，需要断开Kinect与电脑的连接，并关闭Visual Studio。安装过程很简单，等SDK安装完成之后，将Kinect电源线插上并连接到电脑上，Win7会自动寻找和安装驱动，安装完成后就可以识别Kinect，这是Kinect上面LED指示灯会变成绿色。


看驱动是否安装成功，可以到电脑的设备管理器中查看，如下图：在Microsoft Kinect节点下应该有3个项，分别是Microsoft Kinect Audio Array Control，Microsoft Kinect Camera, 和 Microsoft Kinect Security Control.


查看Kinect麦克风是否安装成功可以在设备管理器的声音视频游戏控制器节点下查看，如下图，Kinect USB Audio 应该在这个节点下面：

![image_thumb8](http://images.cnblogs.com/cnblogs_com/yangecnu/201203/201203301800002236.png)![image_thumb9](http://images.cnblogs.com/cnblogs_com/yangecnu/201203/201203301800006107.png)



#### 4. 创建第一个Kinect 项目


创建一个Kincet项目通常需要：


1. 创建一个VS项目，一般为了展示通常创建一个wpf项目。


2. 添加Microsoft.Kinect.dll引用，如果是早期版本的SDK，这个名称可能不同。


3. 引入Kinect命名空间。


Kinect支持3中类型的托管应用程序，分别是：控制台应用程序，WPF以及Windows Form应用程序。首先来创建一个Windows 控制台应用程序，然后在Main函数所在的代码中引入Kinect命名控件，代码如下：

> using Microsoft.Kinect;static void Main(string[] args)
{
    //初始化sensor实例
    KinectSensor sensor = KinectSensor.KinectSensors[0];
            
    //初始化照相机
    sensor.DepthStream.Enable();
    sensor.DepthFrameReady += new EventHandler<DepthImageFrameReadyEventArgs>(sensor_DepthFrameReady);
            
    Console.ForegroundColor=ConsoleColor.Green;

    //打开数据流
    sensor.Start();

    while (Console.ReadKey().Key != ConsoleKey.Spacebar)
    { 
            
    }
}

static void sensor_DepthFrameReady(object sender, DepthImageFrameReadyEventArgs e)
{
    using (var depthFrame=e.OpenDepthImageFrame())
    {
        if (depthFrame == null) return;
        short[] bits = new short[depthFrame.PixelDataLength];
        depthFrame.CopyPixelDataTo(bits);
        foreach (var bit in bits)
            Console.Write(bit);
    }
}


在上面的代码中，为了从摄像头中获取数据流，需要初始化KinectSensor对象，然后启动他。为了获取景深数据，我们给sensor的DepthFrameReady注册了时事件。上面的代码中数据通过DepthFrameReady事件触发。在Kinect应用中，这些获取数据的事件有DepthFrameReady,ColorFrameReady以及SkeletonFrameReady。可以通过这些事件获取丰富的数据来完成各种有趣的应用。在上面的代码中，我们简单的将景深摄像头获取的数据输出打印到控制台上。运行程序，然后站在Kinect前面一段距离，你就会看到控制台上输出的信息，酷吧。

![](http://pic002.cnblogs.com/images/2012/94031/2012033120310585.png)



#### 5.Kinect SDK示例程序


安装完Kinect for Windows SDK后，快捷菜单列表中有一个Kinect SDK Sample Browser，自带有一些示例应用程序，打开后如下图：

![image_thumb13](http://images.cnblogs.com/cnblogs_com/yangecnu/201203/201203301800011614.png)


如果安装好了Kinect，就可以直接点击运行了：我试了一下，挺有趣的：

Kinect Explorer：这是一WPF程序，界面分为左右两个图像区域，左边通过彩色摄像头获取图像信息，并通过骨骼追踪将骨骼叠加在图像上，右边图像是通过景深摄像头获取的景深信息，也在图上叠加了骨骼信息，在这两幅图像下面是一些列对成像参数的设置。这个例子主要是用来展示这几个传感器的应用以及如何获取传感器的信息，还包括获取骨骼数据。

Shape Game：这个一个简单的游戏，界面上的人是通过骨骼追踪绘制出来的，在Kinect前面晃动时，界面上的人也会跟着动，当碰到图形时能够得分。

Kinect Audio Demo：这个是语音识别的例子上面展示的是声音的方向，下面是语音识别的结果，有Red，Green，Blue三个单词，当站在Kinect前面说某个单词时，如果识别正确，横线会显示相应的颜色。试了一下，还是挺灵敏的。后的的例子是针对Kinect
 for Windows sensor设备的例子，我的Kinect for xbox Sensor不能用。



#### 6.结语


本文简要介绍了Kinect传感器的结构，开发所需要的软硬件环境，并通过一个小例子展示了如何从Kinect获取数据，最后简单介绍了Kinect SDK所带的例子，希望本文能帮助你熟悉Kinect for windows SDK。





