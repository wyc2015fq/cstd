# C# DirectX.AudioVideoPlayback音频视频播放 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年01月19日 18:27:40[boonya](https://me.csdn.net/boonya)阅读数：725







英文原文：[https://msdn.microsoft.com/en-us/library/windows/desktop/bb324497(v=vs.85).aspx](https://msdn.microsoft.com/en-us/library/windows/desktop/bb324497(v=vs.85).aspx)

AudioVideoPlayback应用程序编程接口（API）提供音频和视频文件的基本播放和简单控制。有关更多信息，请参阅[Microsoft.DirectX.AudioVideoPlayback](https://msdn.microsoft.com/en-us/library/windows/desktop/ms805515.aspx)托管代码参考文档。

使用[Video](https://msdn.microsoft.com/en-us/library/windows/desktop/ms902629.aspx)类播放视频文件，包括包含音频的视频文件。使用[Audio](https://msdn.microsoft.com/en-us/library/windows/desktop/bb324224(v=vs.85).aspx)类来播放纯音频文件。播放视频文件时，您还可以使用“Audio”类来控制音频属性。 [SeekPositionFlags](https://msdn.microsoft.com/en-us/library/windows/desktop/ms902627.aspx)枚举控件查找操作，[StateFlags](https://msdn.microsoft.com/en-us/library/windows/desktop/ms902626.aspx)枚举具有标志来指示媒体文件是运行，暂停还是停止。
注意：Audio类主要为非常简单的播放场景而设计，或者用于Video类。您还可以使用Microsoft DirectSound播放音频文件，这样可以更好地控制音频播放。
- [播放音频文件](https://msdn.microsoft.com/en-us/library/windows/desktop/bb324497(v=vs.85).aspx#dx_avp_playing_an_audio_file)
- [播放视频文件](https://msdn.microsoft.com/en-us/library/windows/desktop/bb324497(v=vs.85).aspx#dx_avp_playing_a_video_file)

## 播放一个视频文件

要播放视频文件，首先创建一个[Video](https://msdn.microsoft.com/en-us/library/windows/desktop/ms902629.aspx)类的实例。您可以在以下C＃代码示例中指定视频构造函数中的文件名，或者使用文件名称调用[Open](https://msdn.microsoft.com/en-us/library/windows/desktop/bb324196(v=vs.85).aspx)方法。



```
[C#]using Microsoft.DirectX.AudioVideoPlayback;
public class MyVideoPlayer : System.Windows.Forms.Form
{
    /* ... */
    private void OpenFile()
    {
        try
        {
            Video ourVideo = new Video("C:\\Example.avi");
            /* ... */
        }
    }
    /* ... */
}
```
如果您尝试打开不包含视频的文件，[Video](https://msdn.microsoft.com/en-us/library/windows/desktop/ms902629.aspx)对象将引发异常。 接下来，在应用程序中指定一个父窗口来保存Video对象的视频窗口，如下所示：
`[C#]ourVideo.Owner = this;  // 'this' refers to the application's Form object.`通过调用[Play](https://msdn.microsoft.com/en-us/library/windows/desktop/bb324202(v=vs.85).aspx)，[Pause](https://msdn.microsoft.com/en-us/library/windows/desktop/bb324201(v=vs.85).aspx)和[Stop](https://msdn.microsoft.com/en-us/library/windows/desktop/bb324213(v=vs.85).aspx)方法来控制播放。 例如，以下事件处理程序停止播放。


```
[C#]private void mnuStop_Click(object sender, System.EventArgs e)
{
    if (ourVideo != null)
    {
        ourVideo.Stop();
        }
}
```
要设置播放窗口的大小，请设置Size属性，该属性采用System.Drawing.SizeLeave Site对象，如下所示：

`[C#]ourVideo.Size = new Size(480, 320);`您可以通过检查[DefaultSize](https://msdn.microsoft.com/en-us/library/windows/desktop/ms902634.aspx)属性来获取原生视频大小。 如果视频文件包含音频，[Video](https://msdn.microsoft.com/en-us/library/windows/desktop/ms902629.aspx).[Audio](https://msdn.microsoft.com/en-us/library/windows/desktop/ms902630.aspx)属性返回一个[Audio](https://msdn.microsoft.com/en-us/library/windows/desktop/bb324224(v=vs.85).aspx)对象。 您可以使用此对象设置音频的音量或立体声平衡。 如果文件不包含音频，则设置这些属性会导致异常。 围绕代码使用try块，如下所示：


```
[C#]try
{
    Video.Audio.Volume = 100;
}
```

## 播放音频文件


“[Audio](https://msdn.microsoft.com/en-us/library/windows/desktop/bb324224(v=vs.85).aspx)”对象与“[Vedio](https://msdn.microsoft.com/en-us/library/windows/desktop/ms902629.aspx)”对象类似，但支持与音频相关的属性，例如[音量](https://msdn.microsoft.com/en-us/library/windows/desktop/bb324235(v=vs.85).aspx)和[平衡](https://msdn.microsoft.com/en-us/library/windows/desktop/bb324225(v=vs.85).aspx)。 要播放音频文件，请在[Audio](https://msdn.microsoft.com/en-us/library/windows/desktop/bb324236(v=vs.85).aspx)构造函数中指定文件名，如下面的C＃代码示例，或者使用文件名称调用[Open](https://msdn.microsoft.com/en-us/library/windows/desktop/bb324247(v=vs.85).aspx)方法。

`[C#]Audio ourAudio = new Audio("C:\MyAudioFile.wav");`注：有兴趣的可以参考[http://www.cnblogs.com/zhangjun1130/archive/2009/09/15/1566814.html](http://www.cnblogs.com/zhangjun1130/archive/2009/09/15/1566814.html)






