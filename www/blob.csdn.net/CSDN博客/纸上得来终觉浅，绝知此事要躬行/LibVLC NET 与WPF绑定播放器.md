# LibVLC.NET 与WPF绑定播放器 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年12月15日 10:48:36[boonya](https://me.csdn.net/boonya)阅读数：1358








原文地址：[https://libvlcnet.codeplex.com/](https://libvlcnet.codeplex.com/)

![](https://img-blog.csdn.net/20171215104946638)




## 先决条件


LibVLC.NET提供.NET绑定到[VLC媒体播放器](http://www.videolan.org/vlc/)的LibVLC核心库（libVLC）。由于使用了一些更新的LibVLC功能，LibVLC.NET需要VLC版本1.2，可以在[这里](http://nightlies.videolan.org/build/win32/last/)下载。


如果您已将VLC媒体播放器（1.2）安装在其默认位置（例如C：\ Program Files（x86）\ VideoLAN \ VLC），那么LibVLC.NET应该可以直接使用。 要使用libVLC的本地副本，将文件libvlc.dll，libvlccore.dll和文件夹插件复制到名为VLC的文件夹中，以将其应用到您的位置。 您也可以在运行时指定libVLC DLL的目录，并提供您自己的目录。



## 本地绑定


移植到LibVLC.NET / C＃的http://wiki.videolan.org/LibVLC_Tutorial#Sample_LibVLC_Code的libVLC教程如下所示：





```
using System;
using System.Threading;
using LibVLC.NET;

class Program
{
  static void Main()
  {
    LibVLCLibrary library = LibVLCLibrary.Load(null);
    IntPtr inst, mp, m;

    inst = library.libvlc_new();                                      // Load the VLC engine 
    m = library.libvlc_media_new_location(inst, "path/to/your/file"); // Create a new item 
    mp = library.libvlc_media_player_new_from_media(m);               // Create a media player playing environement 
    library.libvlc_media_release(m);                                  // No need to keep the media now 
    library.libvlc_media_player_play(mp);                             // play the media_player 
    Thread.Sleep(10000);                                              // Let it play a bit 
    library.libvlc_media_player_stop(mp);                             // Stop playing 
    library.libvlc_media_player_release(mp);                          // Free the media_player 
    library.libvlc_release(inst);

    LibVLCLibrary.Free(library);
  }
}
```
本教程包含在解决方案文件夹“Tests”中的项目教程中。




## WPF绑定

LibVLC.NET.Presentation提供可用于播放媒体的元素MediaElement：``
xmlns:vlc="clr-namespace:LibVLC.NET.Presentation;assembly=LibVLC.NET.Presentation"



...


<vlc:MediaElementSource="file://path/to/my/file"/>可以通过调用MediaElement实例上的播放，暂停或停止等方法来控制播放。 MediaElement实例的属性当然会在其调度器线程中更改; 但可能不会立即。


WPF示例可以在解决方案文件夹“Tests”的项目TestPresentation中找到。





