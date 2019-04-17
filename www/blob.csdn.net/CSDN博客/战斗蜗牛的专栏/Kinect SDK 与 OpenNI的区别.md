# Kinect SDK 与 OpenNI的区别 - 战斗蜗牛的专栏 - CSDN博客





2012年08月07日 16:16:13[vbskj](https://me.csdn.net/vbskj)阅读数：5990








#### Microsoft’s Kinect SDK (Beta)

##### pro: 优点
- support for audio 支持音频
- support for motor/tilt 支持马达
- full body tracking: 全身追踪- does not need a calibration pose 不需要标定姿势（投降姿势）
- includes head, hands, feet, clavicles 包括头，手，脚和锁骨
- seems to deal better with occluded joints 看起来处理关节闭塞更好些

- supports multiple sensors 支持多传感器（多台Kinect）
- single no-fuss installer 简化安装（安装更容易）
- SDK has events for when a new Video or new Depth frame is available 当新的视频或深度图有效时，SDK会有可用事件

##### con: 缺点
- licensed for non-commercial use only 非商用（商业需要付费）
- only tracks full body (no mode for hand only tracking)  只能追踪全身（不包含特定的追踪模式：例如只追踪手）
- does not offer alignment of the color&depth image streams to one another yet- although there are features to align individual coordinates
- and there are hints that support may come later

- full body tracking:  全身追踪- only calculates positions for the joints, not rotations 关节只有坐标数据，没有旋转数据
- only tracks the full body, no upperbody or hands only mode 只能追踪全身，不包含特定的追踪模式：例如只追踪手或上半身
- seems to consume more CPU power than OpenNI/NITE (not properly benchmarked)  和OpenNI/NITE相比，看起来更消耗CPU（没有采用适当的基准）

- no gesture recognition system  不包含手势识别系统
- no support for the PrimeSense and the ASUS WAVI Xtion sensors? (can anyone confirm this?)  不支持PrimeSense和华硕的WAVI Xtion硬件平台
- only supports Win7 (x86 & x64)   只支持Win7(32位和64位）
- no support for Unity3D game engine  不支持Unity3D游戏引擎
- no built in support for record/playback to disk  不支持数据记录或回放到硬盘
- no support to stream the raw InfraRed video data  不支持红外线视频数据流
- SDK does not have events for when new user enters frame, leaves frame etc  SDK没有此类发生事件，例如当一个用户被侦测到或用户丢失等等。

#### PrimeSense OpenNI/NITE

##### pro: 优点
- license includes commercial use 可以商用（不需要付费）
- includes a framework for hand tracking 包含手部追踪框架
- includes a framework for hand gesture recognition 包含手势识别框架
- can automatically align the depth image stream to the color image  可以自动对齐深度图数据到彩色图数据
- full body tracking:  全身追踪- also calculates rotations for the joints 包含坐标数据和旋转数据
- support for hands only mode 支持特殊跟踪模式：例如：只追踪手和头或上半身
- seems to consume less CPU power than Microsoft Kinect SDK’s tracker (not properly benchmarked) 和微软的SDK相比消耗的CPU更少

- also supports the Primesense and the ASUS WAVI Xtion sensors 支持Primesense和华硕的WAVI Xtion硬件平台
- supports multiple sensors although setup and enumeration is a bit quirky 支持多传感器但是需要安装和枚举，这一点有点古怪。
- supports Windows (including Vista&XP), Linux and Mac OSX 支持Windows（包括Vista&XP&WIN7）,Linux系统和苹果操作系统（翻者：也支持Android）
- comes with code for full support in Unity3D game engine  自带的代码全面支持Unity3D游戏引擎（翻者：也支持Ogre）
- support for record/playback to/from disk 支持数据记录到硬盘或从硬盘回放数据
- support to stream the raw InfraRed video data 支持红外数据流
- SDK has events for when new User enters frame, leaves frame etc SDK有此类发生事件，例如：当一个用户被侦测到或者用户丢失。（提供回调函数供开发者使用）

##### con: 缺点
- no support for audio 不支持音频
- no support for motor/tilt (although you can simultaneously use the CL-NUI motor drivers) 不支持马达（翻者：马达是微软的专利，所以primesense公司不想惹微软）
- full body tracking:  全身追踪- lacks rotations for the head, hands, feet, clavicles 缺乏以下关节：头，手，脚，和锁骨
- needs a calibration pose to start tracking (although it can be saved/loaded to/from disk for reuse) 需要一个标定姿势（投降姿势）才能开始追踪骨骼（注意:标定数据是可以存储和提取的方便重用）
- occluded joints are not estimated 关节闭塞没有被估算

- supports multiple sensors although setup and enumeration is a bit quirky 支持多感应器但是需要安装和枚举，这一点有点古怪。
- three separate installers and a NITE license string (although the process can be automated with my auto driver installer)  需要单独安装NITE
- SDK does not have events for when new Video or new Depth frames is available SDK没有此类发生事件，例如：当新的视频或者深度图数据有效时。（翻者：OpenNI提供了类似功能的函数可使用，虽然不是回调函数，但是也很好用）

#### (Personal) conclusion:

Microsoft seems to have the edge when working with skeletons and/or audio.

微软在骨骼识别和音频方面有优势。（翻者：本人非常认同，微软的音频识别将会在未来的体感游戏里发挥重要的作用！）

OpenNI seems to be best suited when working on colored pointclouds, on non-Win7 platforms and/or for commercial projects.

OpenNI似乎更适合做一些带颜色的点云的工作，和在非Win7平台来开发商业项目。

When working with gestures in specific: 手势识别
- If your sensor only sees the upperbody/hands and/or you want an existing framework to start with use OpenNI/NITE.

如果你想开发基于上半身或手识别的项目，可以使用OpenNI和NITE
- When your sensor can see the full body the more stable Microsoft skeleton may be the best to use, however you’ll have to code your own gesture recognitions. (You’d also have to extend OpenNI/NITE for fullbody
 gestures btw)

全身识别毋庸置疑微软的SDK是最好的，然而你必须自己编写你自己的手势识别代码。

原文地址 http://www.cnblogs.com/TravelingLight/archive/2011/06/20/2085149.html



