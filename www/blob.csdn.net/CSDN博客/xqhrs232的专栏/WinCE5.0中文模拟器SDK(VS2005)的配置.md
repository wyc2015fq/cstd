# WinCE5.0中文模拟器SDK(VS2005)的配置 - xqhrs232的专栏 - CSDN博客
2014年02月21日 22:14:01[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：889
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
原文地址::[http://www.cnblogs.com/we-hjb/archive/2009/11/26/1610953.html](http://www.cnblogs.com/we-hjb/archive/2009/11/26/1610953.html)
相关文章
1、【最新图文教程】WinCE5.0中文模拟器SDK(VS2008)的配置----[http://www.blogbus.com/antiblood-logs/204402631.html](http://www.blogbus.com/antiblood-logs/204402631.html)

WinCE5.0中文模拟器SDK的安装过程不细说了，一路默认即可，下面主要介绍如何配置，使其能在VS2005中正常使用。
     安装完成后，打开VS2005，点击菜单“工具”——“选项”——“设备工具”——“设备”，选择“Windows CE 5.0 ARMV4I Emulator”，点击“属性”按钮，如下图所示。
![image](http://images.cnblogs.com/cnblogs_com/we-hjb/WindowsLiveWriter/WinCE5.0SDKVS2005_1474D/image_thumb_2.png)
     在弹出的对话框中，点击“仿真器选项”，如下图所示。
![image](http://images.cnblogs.com/cnblogs_com/we-hjb/WindowsLiveWriter/WinCE5.0SDKVS2005_1474D/image_thumb_3.png)
    在弹出的对话框中，设置“Flash Memory File”和RAM Size如下图所示。
![image](http://images.cnblogs.com/cnblogs_com/we-hjb/WindowsLiveWriter/WinCE5.0SDKVS2005_1474D/image_thumb_7.png)
     在Display下，设置显示属性，如下图所示，
![image](http://images.cnblogs.com/cnblogs_com/we-hjb/WindowsLiveWriter/WinCE5.0SDKVS2005_1474D/image_thumb_5.png)
点击“OK”——“确定”——“确定”保存设置。
打开Device Emulator Manager，连接“Windows CE 5.0 ARMV4I Emulator”，启动模拟器。如果需要保存文件及注册表设置，点击菜单“Flash”——“Save”即可，如下图所示。
![image](http://images.cnblogs.com/cnblogs_com/we-hjb/WindowsLiveWriter/WinCE5.0SDKVS2005_1474D/image_thumb_6.png)
     该SDK的下载地址:
[http://download.csdn.net/source/1846785](http://download.csdn.net/source/1846785)
[http://download.csdn.net/source/1846812](http://download.csdn.net/source/1846812)
//================================================================================
备注：
1》一度系统还跑不起来，后来又可以了，奇了怪！！！

