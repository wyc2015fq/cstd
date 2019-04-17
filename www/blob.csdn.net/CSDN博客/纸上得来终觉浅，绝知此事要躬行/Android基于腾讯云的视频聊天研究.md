# Android基于腾讯云的视频聊天研究 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年07月19日 22:42:41[boonya](https://me.csdn.net/boonya)阅读数：5381











文章转自：[https://cloud.tencent.com/community/article/269778](https://cloud.tencent.com/community/article/269778)


## 简介

最近有个需求是与视频聊天相关，之前有看到过腾讯云有视频直播的产品，不清楚是否也支持视频聊天。经过一番查找，发现除了直播的demo，还专门有一个视频聊天的demo，于是便下下来研究一下。

这是文档：[https://www.qcloud.com/document/product/268/7603](https://cloud.tencent.com/document/product/268/7603)

这是视频直播demo工程：[https://github.com/zhaoyang21cn/ILiveSDK_Android_Suixinbo](https://github.com/zhaoyang21cn/ILiveSDK_Android_Suixinbo)

这是视频聊天demo工程：[https://github.com/zhaoyang21cn/CallSDK_Android_Demo](https://github.com/zhaoyang21cn/CallSDK_Android_Demo)

本文只讨论视频聊天的demo，下载下来简单配置一下，工程就能跑起来。注册两个账号后，就可以互相呼叫了。这里注意一下，需要开启相机权限，否则会是一片黑屏。界面大概是这样的，会有两个渲染窗口，一个自己的，一个对方的，右边一些操作的按钮，可以操作美颜、切换摄像头等等（这里只有自己的窗口）。

![](https://blog-10039692.file.myqcloud.com/1500363298457_2331_1500363298934.jpg)

## 遮罩与蒙层

工程跑起来以后，就要做一些定制的事情了。需求要求有遮罩和模糊的效果（不要问为什么，反正有就是了）。遮罩比较简单，直接盖一层ImageView就好了。模糊的话，如果要做高斯模糊，就要对视频数据进行处理。首先尝试一下用最简单的盖一层黑色蒙层看一下效果如何。为了方便测试，在右侧加了两个操作的按钮。在布局里面添加了对应的item。

![](https://blog-10039692.file.myqcloud.com/1500363041166_8638_1500363041452.jpg)

遮罩的效果：

![](https://blog-10039692.file.myqcloud.com/1500363265404_1503_1500363265780.jpg)

蒙层的效果：（50%的黑色蒙层）

![](https://blog-10039692.file.myqcloud.com/1500363237813_7264_1500363238146.jpg)

## 灰阶、旋转、模糊

从效果上看，蒙层确实差，看来只能从视频数据入手了。翻了一遍文档，终于找到定制视频的方法（[https://www.qcloud.com/document/product/268/7645）。](https://cloud.tencent.com/document/product/268/7645%EF%BC%89%E3%80%82) 有两种方式可以处理视频数据，来达到想要的效果。

1、拦截sdk的相机数据，进行处理后，再传回到sdk。

2、自己采集相机数据，进行处理后，传给sdk进行上传。

尝试使用第一种方式，需要调用setLocalVideoPreProcessCallback来拦截相机回调的数据，注意视频数据格式为I420（视频数据格式相关知识可以参考这篇文章《[图文详解YUV420数据格式](http://www.cnblogs.com/azraelly/archive/2013/01/01/2841269.html)》）， 而且需要在相机初始化以后调用，否则会失败。

针对视频数据，首先尝试了灰阶化和旋转的修改。

**灰阶化：**

![](https://blog-10039692.file.myqcloud.com/1500363548966_8964_1500363549535.jpg)

![](https://blog-10039692.file.myqcloud.com/1500363586362_619_1500363586820.jpg)

**旋转：**(注意宽高也要反过来)

![](https://blog-10039692.file.myqcloud.com/1500363617034_1316_1500363617429.jpg)

![](https://blog-10039692.file.myqcloud.com/1500363647968_6627_1500363648386.jpg)

**模糊：**

真正的高斯模糊性能要求较高，比较难达到实时的要求，需要使用Shader来实现，这里打算先体验一下模糊的效果，所以选择最简单的方式来实现。为了进行模糊操作，需要先转换为RGB格式，处理完了以后再转换为YUV数据。流程是这样的：YUV -> RGB -> blur ->YUV，这里的blur采用近似的算法来代替（网上找到fastBlur方法）。对于数据转换的方法，可以参考这篇文章《[图像RGB2YUV与YUV2RGB格式互转介绍](http://www.cnblogs.com/qiqibaby/p/5260167.html)》，测试了一下，腾讯云的数据是YUV而不是YCbCr，所以这里采用的是2、3的转换。

![](https://blog-10039692.file.myqcloud.com/1500363763135_5896_1500363763783.jpg)

![](https://blog-10039692.file.myqcloud.com/1500363863400_3518_1500363864219.jpg)

![](https://blog-10039692.file.myqcloud.com/1500363874370_2848_1500363874792.jpg)
（模糊半径：10）

![](https://blog-10039692.file.myqcloud.com/1500363890116_1399_1500363890514.jpg)
（模糊半径：30）

模糊处理耗时：150 ~200 ms

## 后续工作

本文使用的都是直接在java层进行处理，数据处理起来性能效率很差（150-200ms），界面会很卡（掉帧）。要真能用于实际的业务，必须要使用第二种方式来进行数据处理，就是自行采集数据，利用硬件GPU进行数据处理，然后提交sdk渲染和传输，方能达到性能与效果的平衡，当然这种方式开发量和质量保证的难度也会相应地增加。

详细代码请查看附件。

## 参考文献

腾讯云文档：[https://www.qcloud.com/document/product/268/7603](https://cloud.tencent.com/document/product/268/7603)
YUV数据格式：[http://www.cnblogs.com/azraelly/archive/2013/01/01/2841269.html](http://www.cnblogs.com/azraelly/archive/2013/01/01/2841269.html)
YUV与RGB转换：[http://www.cnblogs.com/qiqibaby/p/5260167.html](http://www.cnblogs.com/qiqibaby/p/5260167.html)




