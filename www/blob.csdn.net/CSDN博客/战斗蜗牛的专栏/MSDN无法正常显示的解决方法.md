# MSDN无法正常显示的解决方法 - 战斗蜗牛的专栏 - CSDN博客





2011年06月28日 13:48:00[vbskj](https://me.csdn.net/vbskj)阅读数：569








    以前就遇到过这样的问题，还以为是IE7导致的。后来重新安装了IE7也没有解决。后来就重新安装MSDN了，非常郁闷。
     今天终于知道原因了。因为开了HijackThis删除了一些注册协议，然后发现MSDN不能用了。所以，相信和注册表信息有关。
     删除的信息为：
     O18 - Protocol: ms-help - {314111C7-A502-11D2-BBCA-00C04F8EC294}
     com组件来的。只要注册一个文件就可以解决问题。

解决方法：
regsvr32 "C:/Program Files/Common Files/Microsoft Shared/Help/hxds.dll"



