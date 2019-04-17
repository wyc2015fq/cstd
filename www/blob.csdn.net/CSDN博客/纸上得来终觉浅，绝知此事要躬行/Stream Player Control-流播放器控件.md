# Stream Player Control-流播放器控件 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年12月15日 14:37:41[boonya](https://me.csdn.net/boonya)阅读数：1262










原文地址：[https://www.codeproject.com/articles/885869/stream-player-control](https://www.codeproject.com/articles/885869/stream-player-control)




在本文中，您将会找到一个流播放器控件的实现。


- [Download WPF demo - 12.9 MB](https://www.codeproject.com/KB/audio-video/885869/WpfDemo.zip)
- [Download WinForms demo - 12.6 MB](https://www.codeproject.com/KB/audio-video/885869/WinFormsDemo.zip)
- [Download WPF source - 10.2 KB](https://www.codeproject.com/KB/audio-video/885869/WpfSource.zip)
- [Download WinForms source - 11.7 KB](https://www.codeproject.com/KB/audio-video/885869/WinFormsSource.zip)
- [Download FFmpeg Facade source - 17.8 KB](https://www.codeproject.com/KB/audio-video/885869/FFmpegFacadeSources.zip)




![](https://img-blog.csdn.net/20171215140944823)





## 介绍




这篇文章是我前一篇文章的一个延续，它展示了一个网络摄像头控件的实现。 最近我创建了另一个控件，并希望与社区分享我的经验。 这是一个基于FFmpeg的流播放器控件，可以执行以下操作：
- 播放[RTSP](http://en.wikipedia.org/wiki/Real_Time_Streaming_Protocol)/[RTMP](http://en.wikipedia.org/wiki/Real_Time_Messaging_Protocol)视频流或本地视频文件
- 检索控件显示的当前帧
该控件没有额外的依赖和一个简约的界面。




## 必要条件


- 该控件的WinForms版本是使用.NET Framework 2.0实现的
- 该控件的WPF版本是使用.NET Framework 4 Client Profile实现的
该控件支持x86和x64平台目标。






## 背景

现在通过互联网流式传输音频，视频和数据是非常平常的事情。 但是，当我试图找到一个.NET控件来播放通过网络发送的视频流时，我几乎找不到任何东西。 这个项目试图填补这个空白。


## 实现细节

如果你对实现细节不感兴趣，那么你可以跳过这一节。


实施分为三层。


- 底层被实现为本地DLL模块，它将我们的调用转发给FFmpeg框架。
- 为了便于分发，原生DLL模块作为资源嵌入到控件的程序集中。 在运行时阶段，DLL模块将被提取到磁盘上的临时文件，并通过后期绑定技术使用。 一旦控制权被处置，临时文件将被删除。 换句话说，该控件是作为一个单独的文件分发的。 所有这些操作都是由中间层来实现的。
- 顶层实现控制类本身。
下图显示了实现的逻辑结构。



![](https://img-blog.csdn.net/20171215141517836)


只有顶层应该被客户使用。


## 底层

底层使用[facade](http://en.wikipedia.org/wiki/Facade_pattern)模式来为FFmpeg框架提供一个简化的接口。 门面由三个类组成：StreamPlayer类，它实现了流播放功能



///<summary>/// The StreamPlayer class implements a stream playback functionality.
///</summary>class StreamPlayer : private boost::noncopyable
{
public:

    ///<summary>/// Initializes a new instance of the StreamPlayer class.
///</summary>    StreamPlayer();

    ///<summary>/// Initializes the player.
///</summary>///<paramname="playerParams">The StreamPlayerParams object that contains the information that is used to initialize the player.</param>void Initialize(StreamPlayerParams playerParams);

    ///<summary>/// Asynchronously plays a stream.
///</summary>///<paramname="streamUrl">The url of a stream to play.</param>void StartPlay(std::string const& streamUrl);
    
    ///<summary>/// Retrieves the current frame being displayed by the player.
///</summary>///<paramname="bmpPtr">Address of a pointer to a byte that will receive the DIB.</param>void GetCurrentFrame(uint8_t **bmpPtr);

    ///<summary>/// Retrieves the unstretched frame size, in pixels.
///</summary>///<paramname="widthPtr">A pointer to an int that will receive the width.</param>///<paramname="heightPtr">A pointer to an int that will receive the height.</param>void GetFrameSize(uint32_t *widthPtr, uint32_t *heightPtr);

    ///<summary>/// Uninitializes the player.
///</summary>void Uninitialize();
};Stream类将视频流转换为一系列帧




///<summary>/// A Stream class converts a stream into series of frames. 
///</summary>class Stream : private boost::noncopyable
{
public:
    ///<summary>/// Initializes a new instance of the Stream class.
///</summary>///<paramname="streamUrl">The url of a stream to decode.</param>    Stream(std::string const& streamUrl);

    ///<summary>/// Gets the next frame in the stream.
///</summary>///<returns>The next frame in the stream.</returns>    std::unique_ptr<Frame> GetNextFrame();

    ///<summary>/// Gets an interframe delay, in milliseconds.
///</summary>    int32_t InterframeDelayInMilliseconds() const;

    ///<summary>/// Releases all resources used by the stream.
///</summary>    ~Stream();
};Frame类，它是一组框架相关的工具。




///<summary>/// The Frame class implements a set of frame-related utilities. 
///</summary>class Frame : private boost::noncopyable
{
public:
    ///<summary>/// Initializes a new instance of the Frame class.
///</summary>    Frame(uint32_t width, uint32_t height, AVPicture &avPicture);

    ///<summary>/// Gets the width, in pixels, of the frame.
///</summary>    uint32_t Width() const { return width_; }

    ///<summary>/// Gets the height, in pixels, of the frame.
///</summary>    uint32_t Height() const { return height_; }

    ///<summary>/// Draws the frame.
///</summary>///<paramname="window">A container window that frame should be drawn on.</param>void Draw(HWND window);

    ///<summary>/// Converts the frame to a bitmap.
///</summary>///<paramname="bmpPtr">Address of a pointer to a byte that will receive the DIB.</param>void ToBmp(uint8_t **bmpPtr);

    ///<summary>/// Releases all resources used by the frame.
///</summary>    ~Frame();
};这些树类是FFmpeg Facade DLL模块的核心。



## 中间层

中间层由StreamPlayerProxy类实现，该类用作FFmpeg Facade DLL模块的代理。


首先，我们应该从资源中提取FFmpeg Facade DLL模块并将其保存到一个临时文件中。



_dllFile = Path.GetTempFileName();
using (FileStream stream = new FileStream(_dllFile, FileMode.Create, FileAccess.Write))
{
    using (BinaryWriter writer = new BinaryWriter(stream))
    {
        writer.Write(Resources.StreamPlayer);
    }
}然后我们将DLL模块加载到调用进程的地址空间中。




_hDll = LoadLibrary(_dllFile);
if (_hDll == IntPtr.Zero)
{
    thrownew Win32Exception(Marshal.GetLastWin32Error());
}并将DLL模块函数绑定到类实例方法。




privatedelegateInt32 StopDelegate();
private StopDelegate _stop;

// ...IntPtr procPtr = GetProcAddress(_hDll, "Stop");
_stop =
    (StopDelegate)Marshal.GetDelegateForFunctionPointer(procPtr, 
     typeof(StopDelegate));在处理控件时，我们卸载DLL模块并将其删除。




privatevoid Dispose()
{    
    if (_hDll != IntPtr.Zero)
    {
        FreeLibrary(_hDll);
        _hDll = IntPtr.Zero;
    }

    if (File.Exists(_dllFile))
    {
        File.Delete(_dllFile);
    }    
}





## 顶层

顶层由具有以下接口的StreamPlayerControl类实现。



///<summary>/// Asynchronously plays a stream.
///</summary>///<paramname="uri">The url of a stream to play.</param>///<exceptioncref="ArgumentException">An invalid string is passed as an argument.</exception>///<exceptioncref="Win32Exception">Failed to load the FFmpeg facade dll.</exception>///<exceptioncref="StreamPlayerException">Failed to play the stream.</exception>publicvoid StartPlay(Uri uri)

///<summary>/// Retrieves the image being played.
///</summary>///<returns>The current image.</returns>///<exceptioncref="InvalidOperationException">The control is not playing a video stream.</exception>///<exceptioncref="StreamPlayerException">Failed to get the current image.</exception>public Bitmap GetCurrentFrame();

///<summary>/// Stops a stream.
///</summary>///<exceptioncref="InvalidOperationException">The control is not playing a stream.</exception>///<exceptioncref="StreamPlayerException">Failed to stop a stream.</exception>publicvoid Stop();

///<summary>/// Gets a value indicating whether the control is playing a video stream.
///</summary>publicBoolean IsPlaying { get; }

///<summary>/// Gets the unstretched frame size, in pixels.
///</summary>public Size VideoSize  { get; }

///<summary>/// Occurs when the first frame is read from a stream.
///</summary>publicevent EventHandler StreamStarted;

///<summary>/// Occurs when there are no more frames to read from a stream.
///</summary>publicevent EventHandler StreamStopped;

///<summary>/// Occurs when the player fails to play a stream.
///</summary>publicevent EventHandler StreamFailed;
## 使用



打开程序[包管理器控制台](http://docs.nuget.org/docs/start-here/using-the-package-manager-console)，并将Nuget程序包添加到您的项目中：



Install-Package WebEye.Controls.WinForms.StreamPlayerControl首先，我们需要将控件添加到Visual Studio Designer工具箱，使用右键单击，然后选择“选择项目...”菜单项。 然后，我们将控件放置在所需的位置并具有所需的大小。 控件实例变量的默认名称将是streamPlayerControl1。


以下代码使用提供的地址异步播放流。




streamPlayerControl1.StartPlay(new Uri("rtsp://184.72.239.149/vod/mp4:BigBuckBunny_115k.mov"));



还有一个选项可以指定连接超时和底层传输协议。



streamPlayerControl1.StartPlay(new Uri("rtsp://184.72.239.149/vod/mp4:BigBuckBunny_115k.mov"),
    TimeSpan.FromSeconds(15), RtspTransport.UdpMulticast);要获得正在播放的帧，只需调用GetCurrentFrame（）方法即可。 帧的分辨率和质量取决于流的质量。




using (Bitmap image = streamPlayerControl1.GetCurrentFrame())
{
    // image processing...
}要停止流，使用Stop（）方法。




streamPlayerControl1.Stop();您可以随时使用以下代码检查播放状态。




if (streamPlayerControl1.IsPlaying)
{
    streamPlayerControl1.Stop();
}此外，StreamStarted，StreamStopped和StreamFailed事件可用于监视播放状态。


要报告错误，使用异常，所以不要忘记将代码包装在try / catch块中。 这就是使用它。 要查看完整的示例，请查看演示应用程序来源。



## WPF版本

FFmpeg外观需要一个WinAPI窗口句柄（HWND）才能将其用作渲染目标。 问题是，在WPF世界的窗户不再有处理。 VideoWindow类解决了这个问题。



<UserControlx:Class="WebEye.StreamPlayerControl"xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"xmlns:mc="http://schemas.openxmlformats.org/markup-compatibility/2006"xmlns:d="http://schemas.microsoft.com/expression/blend/2008"mc:Ignorable="d"d:DesignHeight="300"d:DesignWidth="300"xmlns:local="clr-namespace:WebEye"><local:VideoWindowx:Name="_videoWindow"HorizontalAlignment="Stretch"VerticalAlignment="Stretch"/></UserControl>要将WPF版本的控件添加到项目中，请使用以下nuget命令：




Install-Package WebEye.Controls.Wpf.StreamPlayerControl



## Github

该项目在下一页提供了一个GitHub仓库。[https://github.com/jacobbo/WebEye/tree/master/StreamPlayerControl](https://github.com/jacobbo/WebEye/tree/master/StreamPlayerControl)


任何问题，评论和意见都是值得欢迎的。


## 外部许可


- [FFmpeg](http://www.ffmpeg.org/)facade源与FFmpeg框架相同，在[LGPL](http://www.gnu.org/licenses/lgpl-3.0.html)许可下获得许可。
- .NET控件的源代码和演示源在“[代码项目开放许可证](https://www.codeproject.com/articles/885869/www.codeproject.com/info/cpol10.aspx)”（CPOL）下获得许可。
您可以在您的商业产品中使用该控件，唯一的一点是您应该提及您的产品使用FFmpeg库，这里是详细信息。



## 历史 


- March 19th, 2015 - The initial version.
- August 22nd, 2015 - Added the x64 platform support.
- October 25th, 2015 - Added asyncronous stream start and stream status events.
- November 8th, 2015 - Added support for local files playback.
- November 30th, 2015 - Added stream connection timeout.
- October 17th, 2017 - Use new FFmpeg decoding API.

## 本文许可
本文以及任何相关的源代码和文件均已获得“[代码项目开放许可证](http://www.codeproject.com/info/cpol10.aspx)”（CPOL）的许可。








