# 使用Xcode进行iOS设备无线调试 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年06月07日 10:24:53[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：3409
> 
本文来自简书，原文地址：[http://www.jianshu.com/p/e9c1525cc540](http://www.jianshu.com/p/e9c1525cc540)
设备环境：Mac OSX 10.12.5、iOS11、Xcode9
PS:这是WWDC2017的新功能，iOS11以上，Xcode9这是刚性要求。这个功能不好找，就记下来了
- 手机连接上Xcode，打开Xcode菜单：Windows->Device and Simulators。找到连接上的设备，把
```
Connect
 via network
```
选项打勾。
![](http://upload-images.jianshu.io/upload_images/459563-945328f02d0e369a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
image.png
- 之后Xcode将会转一会圈圈，耐心等待一会就成功了，此时拔掉数据线，可以看到左侧的设备连接列表上手机仍在。
![](http://upload-images.jianshu.io/upload_images/459563-7fe9c118197a8bab.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
image.png
- 
运行我们的项目，在设备列表中不出意外会看到我们的无线连接的手机。选择手机后运行
![](http://upload-images.jianshu.io/upload_images/459563-a4e3f9d339fa7bb3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
image.png
- 
不出意外的话，此时是跑不起来的。还需要最后一步，回到刚刚的设备列表中，选中手机右键，在出来的选项卡中选择一个Connect via IP Address项。选择之后输入手机的局域网ip地址。
![](http://upload-images.jianshu.io/upload_images/459563-17b474736b997be5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
image.png
![](http://upload-images.jianshu.io/upload_images/459563-a47703b31af7719f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
image.png
- 完成后可以看到在连接的手机右边有一个地球连接标志，长这样：
![](http://upload-images.jianshu.io/upload_images/459563-4501412e92f35d3b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
image.png
- 以后项目运行，在下拉列表如果手机在局域网中也会有这个标志：
![](http://upload-images.jianshu.io/upload_images/459563-a7fa9ffe0bd97902.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
image.png
- 
最后一步，选择后直接运行，不出意外的话，项目无需数据线就跑起来了，
![](http://upload-images.jianshu.io/upload_images/459563-3123b1461dc2f8b3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
image.png
- 
另外：此方法是支持无线调试的，连一次以后就不需要数据线了了～
![](http://upload-images.jianshu.io/upload_images/459563-a8ac56b0329f1085.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
image.png
> 
再一次温馨提示：此方式仅适用局域网内，同时软件有硬性要求，开题已注明。
