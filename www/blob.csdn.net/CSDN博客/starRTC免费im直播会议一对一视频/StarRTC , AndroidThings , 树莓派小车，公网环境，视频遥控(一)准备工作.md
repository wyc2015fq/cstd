# StarRTC , AndroidThings , 树莓派小车，公网环境，视频遥控(一)准备工作 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年06月23日 20:09:25[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：403
原文地址：http://blog.starrtc.com/?p=48

啥也不说，先来个视频看看效果
视频播放器
00:00
00:54
概述
为了体现StarRTC的实时音视频传输能力，我们做了这个视频遥控小车。
小车使用了StarRTC能力中的的IM通讯、互动直播（连麦）、实时数据传输（类教学白板）这三项能力。
大致流程是：
1 手机端通过IM通讯获取对小车的控制权（IM能力）；
2 小车创建一个互动直播间，并通知手机端进行连麦（互动直播能力）；
3 手机端连麦并向小车发送实时指令，控制小车的行动（实时数据流传输能力）；
帖子最后会贴出小车demo的项目源码，供大家交流使用。
1 需要准备哪些？
硬件：树莓派3B,树莓派摄像头，摄像头云台，直流电机驱动板，4驱小车套装；
系统：AndroidThings 1.0；
软件：StarRTC SDK；
2 开始造车
2.1 首先将树莓派3B刷成AndroidThings系统
刷机教程：[https://blog.csdn.net/fu_shuwu/article/details/75737359](https://blog.csdn.net/fu_shuwu/article/details/75737359)
2.2 组装小车
组装小车底盘和车轮》固定树莓派》安装电机驱动板》安装装摄像头云台》接线
云台舵机接线
![云台舵机接线](http://blog.starrtc.com/wp-content/uploads/2018/06/297843509918763789.jpg)
云台由两个舵机控制方向，每个舵机有三根线，棕色接地，橙色接电（这里我接了树莓派3.3v，因为5v的被风扇占了），黄色接PWM信号。
控制上下的舵机接线方式：
棕色>第9引脚、橙色>第1引脚、黄色>第12引脚；
控制左右的舵机接线方式：
棕色>第25引脚、橙色>第17引脚、黄色>第33引脚；
附上一张树莓派3b的引脚说明图,这张图拯救了我，因为从这张图里看到了PWM的引脚。
![](http://blog.starrtc.com/wp-content/uploads/2018/06/raspberrypi_pinout-176x300.png)
驱动电机接线
![驱动电机接线](http://blog.starrtc.com/wp-content/uploads/2018/06/25276792513020578.jpg)
电机接线需要参照您购买的驱动板说明，接线应该都很容易。
安装完成
![安装完成](http://blog.starrtc.com/wp-content/uploads/2018/06/158723229296627950.jpg)
下一篇介绍如何借助StarRTC，开发一个视频遥控小车~
同行的认可是远行最大的动力，欢迎转载本博客文章，转载请注明出处，十分感谢。
[StarRTC , AndroidThings , 树莓派小车，公网环境，视频遥控(一)准备工作](http://blog.starrtc.com/?p=48)
[StarRTC , AndroidThings , 树莓派小车，公网环境，视频遥控(二)小车端](http://blog.starrtc.com/?p=94)
[StarRTC , AndroidThings , 树莓派小车，公网环境，视频遥控(三)手机端](http://blog.starrtc.com/?p=111)
[源码下载地址](https://docs.starrtc.com/download/star_sdk_iot_car.zip)
