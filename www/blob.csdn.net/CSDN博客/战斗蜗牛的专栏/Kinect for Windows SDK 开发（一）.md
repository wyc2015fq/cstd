# Kinect for Windows SDK 开发（一） - 战斗蜗牛的专栏 - CSDN博客





2012年07月20日 14:51:16[vbskj](https://me.csdn.net/vbskj)阅读数：3791








#### Kinect for Windows SDK开发入门(二)：基础知识 上

发布:2012-06-12 | 作者:—— | 来源: 华强卡卡 | 查看:208次 | 用户关注：[](http://www.hqew.com/tech/fangan/588914.html)

基于Kinect开发的应用程序最开始需要用到的对象就是KinectSensor对象，该对象直接表示Kinect硬件设备。KinectSensor对象是我们想要获取数据，包括彩色影像数据，景深数据和骨骼追踪数据的源头。本文将详细介绍ColorImageStream，后面的文章将详细讨论DepthImageStream和SkeletonStream。 从KinectSensor获取数据最常用的方式是通过监听该对象的一系列事件。每一种数据流都有对应的事件，当改类型数据流可用时，就会触发改时间


## 1. Kinect[**传感器**](http://www.hqew.com/tech/cgq/200010150017/17350.html)

    基于Kinect开发的应用程序最开始需要用到的对象就是KinectSensor对象，该对象直接表示Kinect硬件设备。KinectSensor对象是我们想要获取数据，包括彩色影像数据，景深数据和骨骼追踪数据的源头。本文将详细介绍ColorImageStream，后面的文章将详细讨论Dep[**thI**](http://www.hqew.com/tech/ljq/200010230005/908646.html)mageStream和SkeletonStream。

    从KinectSensor获取数据最常用的方式是通过监听该对象的一[**系列**](http://www.hqew.com/tech/led/200010410003/27268.html)事件。每一种数据流都有对应的事件，当改类型数据流可用时，就会触发改时间。每一个数据流以帧(frame)为单位。例如：ColorImageStream当获取到了新的数据时就会触发ColorFrameReady事件。当在讨论各个具体的传感器数据流是我们将会详细讨论这些事件。

    每一种数据流(Color,Depth,Skeleton)都是以数据点的方式在不同的坐标系中显示的，在后面的讨论中我们能够清楚的看到这一点。将一个数据流中的点数据转换到另一个数据流中是一个很常见的操作，在本文的后面将会讨论如何转换以及为什么这种转换很有必要。KinectSensor对象有一些列的方法能够进行数据流到数据点阵的转换，他们是MapDepthToColorImagePoint，MapDepthToSkeletonPoint以及MapSkeletonPointToDepth。在获取Kinect数据前，我们必须先发现连接的Kinect设备。发现Kinect设备很简单，但是也有需要主注意的地方。



### 1.1 发现连接的Kinect设备

    KinectObject对象没有公共的构造器，应用程序不能直接创建它。相反，该对象是SDK在探测到有连接的Kinect设备时创建的。当有Kinect设备连接到计算机上时，应用程序应该得到通知或者提醒。KinectSeneor对象有一个静态的属性KinectSensors，该属性是一个KinectSensorCollection集合，该集合继承自ReadOnlyCollection，ReadOnlyCollection集合很简单，他只有一个索引器和一个称之为StatusChanged的事件。

    使用集合中的索引器来获取KinectSensor对象。集合中元素的个数就是Kinect设备的个数。也就是说，一台电脑上可以连接多个Kinect设备来从不同的方向获取数据。应用程序可以使用多个Kinect设备来获取多方面的数据，Kinect个数的限制 只有电脑配置的限制。由于每个Kinect是通过[**USB**](http://www.hqew.com/tech/detail/usb.html)来进行[**数据传输**](http://www.hqew.com/tech/gdz/200010250016/16579.html)的，所以每一个Kinect设备需要一条USB线与电脑相连。此外，更多的Kinect设备需要更多的[**CPU**](http://www.hqew.com/tech/detail/CPU.html)和[**内存**](http://www.hqew.com/tech/qtdz/200010160031/1614389.html)消耗。

    查找Kinect设备可以通过简单的遍历集合找到；但是KinectSensor集合中的设备不是都能直接使用，所以KinectSensor对象有一个Status属性，他是一个枚举类型，标识了当前Kinect设备的状态。下表中列出了传感器的状态及其含义：

![image](http://images.cnblogs.com/cnblogs_com/yangecnu/201203/20120331212659142.png)

    只有设备在Connected状态下时，KinectSensor对象才能初始化。在应用的整个生命周期中，传感器的状态可能会发生变化，这意味着我们开发的应用程序必须监控设备的连接状态，并且在设备连接状态发生变化时能够采取相应的措施来提高用户体验。例如，如果连接Kinect的USB线从电脑拔出，那么传感器的连接状态就会变为Disconnected，通常，应用程序在这种情况下应该暂停，并提示用户将Kinect设备插入到电脑上。应用程序不应该假定在一开始时Kinect设备就处于可用状态，也不应该假定在整个程序运行的过程中，Kinect设备会一直与电脑连接。

    下面，首先创建一个WPF应用程序来展示如何发现，获取Kinect传感器的状态。先建按一个WPF项目，并添加Microsoft.Kinect.dll。在MainWindows.xaml.cs中写下如下代码：

> public partial class MainWindow : Window
{
    //私有Kinectsensor对象
    private KinectSensor kinect;

    public KinectSensor Kinect
    {
        get { return this.kinect;}
        [**set**](http://www.hqew.com/tech/dzg/200010140014/711518.html){
            //如果带赋值的传感器和目前的不一样
            if (this.kinect!=value)
            {
                //如果当前的传感对象不为null
                if (this.kinect!=null)
                {
                 //uninitailize当前对象
                    this.kinect=null;
                }
                //如果传入的对象不为空，且状态为连接状态
                if (value!=null&[**amp**](http://www.hqew.com/tech/detail/AMP.html);&value.Status==KinectStatus.Connected)
                {
                    this.kinect=value;
                }
            }
        }
    }

    public MainWindow()
    {
        InitializeComponent();
        this.Loaded += (s, e) => DiscoverKinectSensor();
        this.Unloaded += (s, e) => this.kinect = null;
    }

    private void DiscoverKinectSensor()
    {
        KinectSensor.KinectSensors.StatusChanged += KinectSensors_StatusChanged;
        this.Kinect = KinectSensor.KinectSensors.FirstOrDefau[**lt**](http://www.hqew.com/tech/dzg/200010140014/762478.html)(x => x.Status == KinectStatus.Connected);
    }

    private void KinectSensors_StatusChanged(object sender, StatusChangedEventArgs e)
    {
        switch (e.Status)
        {
            case KinectStatus.Connected:
                if (this.kinect == null)
                    this.kinect = e.Sensor;
                break;
            case KinectStatus.Disconnected:
                if (this.kinect == e.Sensor)
                {
                    this.kinect = null;
                    this.kinect = KinectSensor.KinectSensors.FirstOrDefault(x => x.Status == KinectStatus.Connected);
                    if (this.kinect == null)
                    {
                        //TODO:通知用于Kinect已拔出
                       }
                }
                break;
            //TODO:处理其他情况下的状态
        }
    }
}



    上面的代码注释很详细，首先定义了一个私有变量kinect，应用程序应该定义一个私有的变量来存储对获取到的KincectSensor对象的引用，当应用程序不在需要KinectSensor产生数据时，可以使用这个局部变量来释放对KinectSensor对象的引用从而释放资源。我们还定义了一个Kinect属性来对这个私有变量进行包装，使用属性的目的是保证能够以正确的方式初始化和反初始化KinectSensor对象。在Set方法中我们可以看到，自由待赋值的对象的组航太是Connected的时候我们才进行赋值操作，任何将没有处在Connected状态的传感器对象复制给KinectSensor对象时都会抛出InvalidOperationException异常。

    在构造函数中有两个匿名方法，一个用来监听Loaded事件，一个用来监听Unloaded事件。当卸载时应该将Kinect属性置为空。在窗口的Loaded事件中程序通过DiscoverKinectSensor方法试图调用一个连接了的传感器。在窗体的Loaded和Unloaded事件中注册这两个事件用来初始化和释放Kinect对象，如果应用程序没有找到Kinect对象，将会通知用户。

    DiscoverKinectSensor方法只有两行代码，第一行代码注册StatusChanged事件，第二行代码通过lambda表达式查询集合中第一个处在Connected状态的传感器对象，并将该对象复制给Kinect属性。Kinect属性的set方法确保能都赋值一个合法的Kinect对象。

StatusChanged事件中值得注意的是，当状态为KinectSensor.Connected的时候，if语句限制了应用程序只能有一个kinect传感器，他忽略了电脑中可能连接的其他Kinect传感器。

    以上代码展示了用于发现和引用Kinect设备的最精简的代码，随着应用的复杂，可能需要更多的代码来保证线程安全以及能让垃圾回收器及时释放资源以防止内存泄露。



### 1.2 打开传感器

    一旦发现了传感器，在应用程序能够使用传感器之前必须对其进行初始化。传感器的初始化包括三个步骤。首先，应用程序必须设置需要使用的数据流，并将其状态设为可用。每一中类型的数据流都有一个Enable方法，该方法可以初始化数据流。每一种数据流都完全不同，在使用之前需要进行一些列的设置。在一些情况下这些设置都在Enable方法中处理了。在下面，我们将会讨论如何初始化ColorImageStream数据流，在以后的文章中还会讨论如何初始化DepthImageStream数据流和SkeletonStream数据流。

    初始化之后，接下来就是要确定应用程序如何使用产生的数据流。最常用的方式是使用Kinect对象的一些列事件，每一种数据流都有对应的事件，他们是：ColorImageStream对应ColorFrameReady事件、DepthImageStream对应DepthFrameReady事件、SkeletonStream对象对应SkeletonFrameReady事件。以及AllFramesReady事件。各自对应的事件只有在对应的数据流enabled后才能使用，AllFramesReady事件在任何一个数据流状态enabled时就能使用。

    最后，应用程序调用KinectSensor对象的Start方法后，frame-ready事件就会触发从而产生数据。



### 1.3 停止传感器

    一旦传感器打开后，可以使用KinectSensor对象的Stop方法停止。这样所有的数据产生都会停止，因此在监听frameready事件时要先检查传感器是否不为null。

    KinectSensor对象以及数据流都会使用系统资源，应用程序在不需要使用KinectSensor对象时必须能够合理的释放这些资源。在这种情况下，程序不仅要停止传单器，还用注销frameready事件。注意，不要去调用KinectSensor对象的Dispose方法。这将会阻止应用程序再次获取传感器。应用程序必须从启或者将Kinect从新拔出然后插入才能再次获得并使用对象。



## 2. 彩色影像数据流

    Kinect有两类摄像头，近[**红外**](http://www.hqew.com/tech/gdz/200010250016/16710.html)摄像头和普通的视频摄像头。视频摄像头提供了一般摄像头类似的彩色影像。这种数据流是三中数据流中使用和设置最简单的。因此我将他作为Kinect数据流介绍的例子。

    使用Kinect数据流也有三部。首先是数据流必须可用。一旦数据流可用，应用程序就可以从数据量中读取数据并对数据进行处理和展现。一旦有新的数据帧可用，这两个步骤就会一直进行，下面的代码展现了如何初始化ColorImage对象。

> public KinectSensor Kinect
{
    get { return this.kinect;}
    set {
        //如果带赋值的传感器和目前的不一样
        if (this.kinect!=value)
        {
            //如果当前的传感对象不为null
            if (this.kinect!=null)
            {
              UninitializeKinectSensor(this.kinect);//uninitailize当前对象
                this.kinect=null;
            }
            //如果传入的对象不为空，且状态为连接状态
            if (value!=null&&value.Status==KinectStatus.Connected)
            {
                this.kinect=value;
                InitializeKinectSensor(this.kinect);
            }
        }
    }
}

private void InitializeKinectSensor(KinectSensor kinectSensor)
{
    if (kinectSensor != null)
    {
        kinectSensor.ColorStream.Enable();
        kinectSensor.ColorFrameReady += new EventHandler<ColorImageFrameReadyEventArgs>(kinectSensor_ColorFrameReady);
        kinectSensor.Start();
    }
}

private void UninitializeKinectSensor(KinectSensor kinectSensor)
{
    if (kinectSensor != null)
    {
        kinectSensor.Stop();
        kinectSensor.ColorFrameReady -= new EventHandler<ColorImageFrameReadyEventArgs>(kinectSensor_ColorFrameReady);
    }
}

    上面的代码对之前Kinect属性进行了修改，加粗为修改部分。新添加的两行调用了两个方法，分别初始化和释放KinectSensor和ColorImageStream对象。InitializeKinectSensor对象调用ColorImageStream的Enable方法，注册ColorFrameReady事件并调用start方法。一旦打开了传感器，当新数据帧大道是就会触发frameready事件，该事件触发频率是每秒30次。

在实现Kinect_ColorFrameReady方法前，我们先在XAML窗体中添加一些空间来展现获取到的数据，代码如下:

> <Window x:Class="KinectApplicationFoundation.MainWindow"
        xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
        xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
        Title="ColorImageStreamFromKinect" Height="350" Width="525">
    <Grid>
        <Image x:Name="ColorImageElement"></Image>
    </Grid>
</Window>




    然后，在Kinect_ColorFrameReady方法中，我们首先通过打开或者获取一个frame来提取获Frame数据。ColorImageFrameReadyEventArgs对象的OpenColorImageFrame属性返回一个当前的ColorImageFrame对象。这个对象实现了IDisposable接口。所以可以将这个对象抱在using语句中的原因,在提取像素数据之前需要使用一个Byte数组保存获取到的数据。FrameObject对象的PixelDataLength对象返回数据和序列的具体大小。调用CopyPixelDataTo方法可以填充像素数据，然后将数据展示到image控件上，具体代码如下：

> void kinectSensor_ColorFrameReady(object sender, ColorImageFrameReadyEventArgs e)
{
    using (ColorImageFrame frame = e.OpenColorImageFrame())
    {
        if (frame != null)
        {
            byte[] pixelData = new byte[frame.PixelDataLength];
            frame.CopyPixelDataTo(pixelData);
            ColorImageElement.Source = BitmapImage.Create(frame.Width, frame.Height, 96, 96,
                                                         PixelFormats.Bgr32, null, pixelData,
                                                         frame.Width * frame.BytesPerPixel);

        }
    }
}



    运行程序，就能得到从Kinect获取的视频信息，如下图所示这是从Kinect彩色摄像头获取的我房间的照片。和一般的视频没什麽两样，只不过这个是从Kinect的视频摄像头产生的。

![image](http://images.cnblogs.com/cnblogs_com/yangecnu/201203/201203312127249976.png)



## 3. 结语

    本文简要介绍了Kinect开发会遇到的基本对象，Kinect物理设备的发现，KinectSensor对象的初始化，打开KinectSensor对象以及如何获取数据流，最后以ColorImageStream对象为例展示了如何从Kinect获取数据并展现出来。

    由于Kinect的彩色摄像头默认每秒产生30副ColorImageFrame，所以上面的应用程序会产生30个Bitmap对象，而且这些对象初始化后很快将变成垃圾等待垃圾回收器进行收集，当采集的数据量很大时，将会对性能产生影响。限于篇幅原因，下篇文章将会介绍如何对这一点进行改进，并将讨论获取Kinect传感器产生数据的两种编程模式：基于事件的模式和轮询的模式。




