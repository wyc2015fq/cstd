# vs2013编译ffmpeg之十九  nvenc - 枫林港的博客 - CSDN博客





2018年07月09日 00:15:31[枫林港](https://me.csdn.net/u013601132)阅读数：86








# nvenc

ffmpeg-3.3中，nvenc模块是自动检测的，configure的时候不需要加enable。

针对ffmpeg-3.3.2,需要从[官网上](https://developer.nvidia.com/nvidia-video-codec-sdk)下载SDK，需要注册，从网上下载了5.0的SDK，nvenc_5.0.1_sdk，这个SDK大约是2014年发布的。SDK和N卡驱动需要配套，具体的信息在上面的了解里面有说明，5.0的SDK与368.69-desktop-win8-win7-winvista-64bit-international-whql.exe配套测试过是ok的，368.69以后的驱动应该也是可以的，另外需要安装cuda_7.5.18_windows_64.exe，cuda安装时需要联网，要下载.NET Framework。

没有N卡时，安装cuda_7.5.18_windows_64.exe会告警，直接点继续： 
![这里写图片描述](https://img-blog.csdn.net/20180706233551635?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

建议先安装Visual Studio再安装cuda： 
![这里写图片描述](https://img-blog.csdn.net/20180706233557318?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



