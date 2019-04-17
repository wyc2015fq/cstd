# 深度相机（四）--Realsense概览 - 工作笔记 - CSDN博客





2016年09月25日 14:57:48[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：11469
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









本文参考下文，做一点个人使用补充。﻿﻿

转自：

[http://blog.csdn.net/dfdfdsfdfdfdf/article/details/49990835](http://blog.csdn.net/dfdfdsfdfdfdf/article/details/49990835)

Intel RealSense(实感技术)概览



前段时间CES的报道满天飞，网上逛了几圈，感觉料最猛的还数Intel：老树开新花，推出14nm的第五代酷睿处理器；在智能可穿戴设备及物联网上雄起；RealSense实感技术开启未来人机交互模式。之前总听老哥讲，鼠标键盘这些传统交互模式统治了二三十年，目前跟踪识别爆火且技术趋于成熟，新的交互革命快要打响了。看RealSense这阵势，估计不出三五年新的交互便会普及开来。


英特尔早在2012年左右就着重研发实感技术，当时叫Perceptual Computing，即感知计算，并开放英特尔® 感知计算软件开发套件 2013 版（Intel® Perceptual Computing Software Development Kit, SDK 2013），设重奖举办因特尔感知计算挑战赛，吸引众多开发者参与。随着技术完善与成熟，2014年初更名为RealSense，即实感技术，而后发布了新的[Intel®
 RealSense™ SDK 2014](https://software.intel.com/en-us/intel-realsense-sdk)，同时举行[2014英特尔®RealSense™应用挑战赛](https://zh.realsenseappchallenge.intel.com/)。


基于此技术的应用在IDF2014及今年CES上大放异彩。


Intel® RealSense™ SDK的架构：SDK core,I/O module和Capability modules组成整个SDK堆栈的基础。SDK core管理I/O module和Capability modules同时组织并管理管线的执行。I/O module捕获设备的输入数据而后发送数据到输出设备或Capability modules。Capability modules也叫algorithm modules，主要包括各种模式检测和识别算法（面部跟踪和检测、手部跟踪、手势识别、语音识别及合成等）。

![01](http://planckscale.info/wp-content/uploads/2015/01/01.png)


各功能：手部和手指跟踪、面部分析、语音识别、背景移除、目标跟踪、增强现实、3D扫描。

1. 手


SDK将手抽象出骨架，并从背景中剥离出来。允许在照相机的0.2–1.2米范围内跟踪手上的22个点的位置和方向，如图。左右手是区分的，因而可以双手进行交互。

![02](http://planckscale.info/wp-content/uploads/2015/01/02.png)


手势识别：包括静态手势识别和动态手势识别。SDK中内嵌了一系列手势如下图。你可以用内嵌的这些手势组合出新的手势，也可以根据手骨架上那22个点位置创建出新的手势。



![03](http://planckscale.info/wp-content/uploads/2015/01/03.png)![04](http://planckscale.info/wp-content/uploads/2015/01/04.png)

![05](http://planckscale.info/wp-content/uploads/2015/01/05.png)


动态手势识别：静态手势可以单独使用，当然也可以组合使用形成一定的动作。组合时要求开始的手势和结束的手势都已在手势识别系统中注册。

![06](http://planckscale.info/wp-content/uploads/2015/01/06.png)


SDK给出了一系列动作如下：

![07](http://planckscale.info/wp-content/uploads/2015/01/07.png)



2．脸


脸部检测：SDK提供精确的3D脸部检测和跟踪，且可以同时跟踪4张人脸。每张人脸用长方形来标记，你可以获得长方形的XYZ坐标。与2D跟踪相比，3D头部跟踪在头部运动方面更给力。

![QQ图片20150118224438](http://planckscale.info/wp-content/uploads/2015/01/QQ%E5%9B%BE%E7%89%8720150118224438.png)


脸部识别：SDK提供识别特定人脸的能力。特定ID对应注册的特定人脸，并将此人脸的信息存储到人脸库的内存中。如果同张人脸被注册多次，那这张人脸被正确识别的机会将会增大。当没被识别的人脸出现时，识别模块将与数据库中的可能数据进行比对，如果找到匹配则返回此人脸对应的ID。


用户不用担心自己的头像被存储，因为存储的只是算法从图像中提取的特征的集合。


头的运动：SDK提供头部运动的3D方向：俯仰、左右转动、左右偏转，如图。

![08](http://planckscale.info/wp-content/uploads/2015/01/08.png)


所以可以轻松获得用户头部指向哪里。也可以以此做粗略的眼神跟踪，下一版将会推出更精细的眼神跟踪。


标记点跟踪：SDK提供脸部78个标记点的跟踪以提高脸部识别和分析的精确度。在图像和坐标系中给出这78个标记点的位置。脸部标记点跟踪支持头像创建、脸部动画、拟态及简单的表情识别。可以直接用这些点或这些点的相对位置来作分析。不管你有没有刘海、戴不戴眼镜，这种标记点跟踪都支持。但用户头部在屏幕30度内效果最好。

![09](http://planckscale.info/wp-content/uploads/2015/01/09.png)


面部表情识别：SDK也包括更高级的面部表情识别。这使得你创建卡通头像更简单。每一种表情有1到100的强度等级使得你做的动画更平滑自然。SDK中表情：

![001](http://planckscale.info/wp-content/uploads/2015/01/001.png)


情感识别：SDK中的情感识别算法用的是2D RGB数据。情感模块是独立的模块，并非脸部模块的一部分。为保障情感识别正常工作，图像中的人脸至少要有48×48个像素。此算法并不局限于RGB数据，灰度数据同样可行。利用SDK，你可以检测并估计以下六种原始情感的强度。

![002](http://planckscale.info/wp-content/uploads/2015/01/002.png)


头像控制：SDK通过结合面部表情及78个标记点提供简单的头像控制功能。SDK提供动画角色的示例代码，使你的应用可以适应任何脸型并把用户头像动画化。


应用场景：

![003](http://planckscale.info/wp-content/uploads/2015/01/003.png)



3. 语音


主要包括语音命令和控制、听写、从文字转译成语音等功能。只支持英语，语音识别对成年人效果最好。


语音识别：分为命令模式和听写模式。命令模式需提前设定命令列表，特定命令绑定特定动作。听写模式内置了一个通用的词典，包括50k个常用单词。如果你觉得不够用，也可以自己添加词典中没有的单词。听写模式限时30秒。命令和听写模式不可同时开启。

![004](http://planckscale.info/wp-content/uploads/2015/01/004.png)


语音分析：SDK也可以根据文本动态的生成语音，由女声读出。

![005](http://planckscale.info/wp-content/uploads/2015/01/005.png)

4. 背景移除


可以实时的移除背景并替换成新的背景。

![006](http://planckscale.info/wp-content/uploads/2015/01/006.png)


应用场景：

![007](http://planckscale.info/wp-content/uploads/2015/01/007.png)

5. 目标跟踪


Metaio*3D目标跟踪模块提供基于光学的跟踪技术，可以跟踪视频序列或场景中的目标物。Metaio工具箱可以训练、创建并编辑3D模型，这些模型可以传给各种目标检测及跟踪算法。


跟踪技术支持平面的2D目标跟踪、基于特征的3D跟踪、CAD模型的基于边界的3D跟踪及即时3D跟踪。

![](http://planckscale.info/wp-content/uploads/2015/01/008.png)

![009](http://planckscale.info/wp-content/uploads/2015/01/009.png)

6. 增强现实


用音频、视频、图像或其他信息来添加、增强或补充英特尔实感技术游戏的内容。

![QQ图片20150118210357](http://planckscale.info/wp-content/uploads/2015/01/QQ%E5%9B%BE%E7%89%8720150118210357.png)

7. 三维扫描和打印


可以扫描、编辑、打印和分享三维物体，并与 3D Systems展开合作。

![000](http://planckscale.info/wp-content/uploads/2015/01/000.png)


目前因特尔已经与腾讯合作创立游戏创新实验室，推进实感技术，并打造了《轩辕传奇》。与京东合作建立“京东因特尔联合创新实验室”，推出虚拟试衣、3D物品展示等，使实感技术落地电商平台。同时因特尔自己也在不断推出自己的实感技术产品。这不前两天在北京推出其全新的第五代酷睿处理器家族，搭载此处理器的多款产品还配备了英特尔实感技术，看来这项技术很快就会遍地开花。


只要你的处理器是第四代及四代以后英特尔® 酷睿™处理器，操作系统满足Microsoft* Windows 8.1（仅限64 bit），再外购一个英特尔® RealSense™ 3D 摄像头就可以下载SDK玩起来。估计不久联想、戴尔、华硕、宏基、惠普等等搭载第五代酷睿处理器并内嵌因特尔3D摄像头的超极本、二合一及一体机设备就会上市，那时玩起来会更爽。


资料来源：[RealSenseSDKdesignGuidelinesGold](https://software.intel.com/sites/default/files/managed/0b/0a/RealSenseSDKdesignGuidelinesGold.pdf)


补充一点自己的实际体会：


1.基于结构光方案的深度相机，受光线影响很大，下午太阳光照进来，深度数据残缺严重，这个就是所谓的激光散斑受强光淹没掉了。


2.手势识别中，静态手势，手型识别效果不错。同时，对着深度相机方向的手势运动，识别效果也不错。但左右方向的运动手势，效果很差。主要是视场角小，深度数据残缺严重。


3.SDK可支持多个设备同时链接 。采用一台设备，一个线程来管理维护。线程内部，SDK所有功能都支持。也就是多个设备互不影响。


4.手势识别，在20~55cm范围内可用。其实，整体来看，只能是小范围运动情况了。


5.手势，采用22点骨架模型。


6.SR300是 F200的升级版。SR300近距应用，实际范围 大概在20~150cm。R200是远距离应用，大概是40~280cm.两者都支持RGB([1080P@30](mailto:1080P@30)、[720P@60),D](mailto:720P@60),D)([VGA@60](mailto:VGA@60)),但RGB
 16:9，D 4:3，蛋疼的一点是，R200 4th CPU， SR300 6th CPU....




