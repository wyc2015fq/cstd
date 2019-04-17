# VS2013远程调试ffmpeg - 枫林港的博客 - CSDN博客





2018年07月24日 23:32:11[枫林港](https://me.csdn.net/u013601132)阅读数：56








# VS2013远程调试

**主机**：VS2013运行的机器，在主机上用VS2013连接目标机上的程序进行调试。

**目标机：**被调试的程序在此机器上运行。

## 目标机上：

1、将主机上C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\IDE\Remote Debugger拷贝到目标机上。

2、运行Remote Debugger下面的msvsmon.exe，64位的机器用x64下面的，32位机器用x86下面的。

3、msvsmon.exe启动后，菜单->工具->选项设置如下（msvsmon.exe每次重启后都需要重新设置）： 
![这里写图片描述](https://img-blog.csdn.net/20180724233055995?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 主机上：

1、以ffmpeg-3.3工程为例，工程属性配置如下： 
![这里写图片描述](https://img-blog.csdn.net/20180724233112271?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
首先选择”Remote Windows Debugger”，然后设置其下的Remote command、working directory、Remote Server Name,Connection、deployment Directory等，图中的路径D:\test是目标机上被调试程序ffmpeg.exe所在的目录。

VS2010下这个配置不一样，其需要将目标机上将被调试的D:\test目录共享出来，调试的路径形式：\192.18.35.164\test。

2、工具栏选择”Remote Windows Debugger”： 
![这里写图片描述](https://img-blog.csdn.net/20180724233130591?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
3、按F5就可以开始调试了，正常的话目标机上的msvsmon.exe会提示已经连接成功了。






