# 深度相机（八）--OpenNI及与Kinect for windows SDK的比较 - 工作笔记 - CSDN博客





2016年09月26日 11:14:51[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：7844
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









﻿﻿

OpenNI(开放自然交互)是一个多语言，跨平台的框架，它定义了编写应用程序，并利用其自然交互的API。OpenNI API由一组可用来编写通用自然交互应用的接口组成。OpenNI的主要目的是要形成一个标准的API，来搭建视觉和音频传感器与视觉和音频感知中间件通信的桥梁。

OpenNI（开放式的自然交互）是一个由业界领导的非营利组织。该组织专注于提高和改善自然交互设备,应用软件的互操作能力。通过使用这些硬件和中间件（软件）来很方便的访问和使用一些设备。

该组织创建于2010年11月，网站于12月8号正式公开。主要成员之一是PrimeSense公司，Kinect的核心芯片正是这家公司提供的。（其他成员还有：开发ROS的机器人公司Willow Garage，以及游戏公司Side-Kick）。

1、视觉和音频传感器（包括分析周围环境的设备）；

2、视觉和音频感知中间件（用于实时的记录音频和视觉分析的数据并理解它们的软件组件）。

例如，一个中间件可以接收一副有人的图像，计算并返回人手掌在图像中的位置。OpenNI提供了一组基于传感器设备的API，以及一组由中间件组件实现的API。通过打破传感器和中间件之间的依赖，使用OpenNI的API编写应用程序就不需要额外的努力去处理由不同的中间件共存带来的问题（跨平台）。

OpenNI的API还允许开发人员直接基于最原始的数据格式编写中间件上层的算法，而不管这些数据是由哪个传感器设备产生的。同时OpenNI的这种机制给了传感器制造商一个充分的自由空间去制造自己的传感器而不用考虑上层OpenNI兼容的应用程序。

OpenNI API使得自然交互应用开发人员通过传感器输出的标准化了的数据类型来处理真实的三维数据（这些标准化的数据类型可以表示人体全身，手的位置，或者仅仅是一个含有深度信息的像素图等）。编写应用程序的时候不用考虑传感器或中间件供应商相关的细节。

OpenNI是微软的xbox 360配件kinect，在PC上的开源驱动必须安装的一个API。

目前OpenNI的最新版为OpenNI 2.2 Beta，实际上，在2012年7月份，就开始停更了。所以，有kinectV1的驱动，没有V2的驱动。

官网提供的版本说明：[http://www.openni.ru/openni-sdk/openni-sdk-release-notes/index.html](http://www.openni.ru/openni-sdk/openni-sdk-release-notes/index.html)


# **OPENNI SDK RELEASE NOTES**

OpenNI 2.2 Beta Release Notes – **July, 2012**

## Introduction

This document describes the OpenNI 2.2 Beta release.

## Package Content

### Components:
- OpenNI 2.2 Beta
- Package Structure

The folder structure of the OpenNI 2.2 Beta release is as follows:

### **Documentation**
- Contains documentation files in HTML format

### **Driver**
- Contains the USB driver for the PrimeSense Sensor (Windows only)

### **Include**
- Contains the OpenNI header include files for application development

### **Lib**
- Contains the *.lib files for compilation linkage (Windows Only)

### **Redist**
- Contains the OpenNI 2.2 Beta runtime binaries
- The Redist folder must be copied to each application directory

### **Samples**
- Contains nine programming samples demonstrating how to program with OpenNI 2.2 Beta API
- Detailed description of each sample is available in Usage section

### **Tools**
- *NiViewer* tool, which demonstrates most of the features provided by OpenNI 2.2 Beta
- PS1080 Console
- PSLink Console



## Supported Platforms

### **Supported Computer Hardware**
- X86 based computers: Pentium 4, 1.4 GHz and above or AMD Athlon 64/FX 1 GHz and above

### **Supported Operating Systems**
- Windows XP (32/64bits) with SP2 and above, Windows 7 (32/64bits), Win 8 (32/64bits)
- OSX 10.7 and above
- Ubuntu 12.04 (32/64bits) and above

### **Supported Development Environments**
- Microsoft Visual Studio 2008 and 2010. The compiler can be an MSVC compiler or an Intel Compiler 11 and above
- GCC 4.x

# Change Log

## **Changes from OpenNI 2.1**
- Capri support
- Java wrappers
- Looks for the driver in the same path as the DLL file (PS1080/PSLink)
- Allow choosing which streams to open in NiViewer (run “NiViewer –help” for details).
- Image registration support for Kinect driver.
- Added *Sensor connected event* in Linux using “OpenNI::addDeviceConnectedListener” function.
- Changed default compilation location to each Sample folder.
- PS1080 – IR streams support for RGB888 and YUYV from FW 5.8.22
- Visual Studio 2008 and express bug fix.

## **Changes from Open 1.x**
- Refactored API – This release includes the OpenNI 1.x features, but with an improved design. See the*OpenNI 2.0 Migration Guide*
- Algorithms API has been removed, and its functionality is now part of the middleware libraries (such as NiTE)

## **OpenNI 2.x Features**
- Added support for translating depth pixels to color map coordinates
- Added support for turning off Auto Exposure and Auto White Balance of the color CMOS in PS1080 devices
- New deployment model –-a private copy of the OpenNI 2.x runtime binaries (in the Redist folder) for each application (see documentation)
- Added built-in support for Kinect devices via the Kinect SDK (Windows only). Requires a Kinect SDK installation [LINK]

Known Issues
|**1.**|**Listener class **Although a Listener instance can attached to one stream only, if an additional stream was attached to the listener there is no error notification.|
|----|----|
|**2.**|**NiViewer tool **Changing color stream resolution to 720p (from any other resolution) closes the color stream and issues the following error: *“Device Protocol: Bad Parameter sent”*.|
|**3.**|**NiViewer tool (applicable to Linux) **When saving an ONI file to the hard drive you cannot change the name of the file or the file’s location; the default is used.|
|**4.**|**All Samples (applicable to Win64) **When closing the Samples Viewer window with the Windows “X” button – the samples are not closed but continue to run in the console window.|

## Installation

The OpenNI 2.2 Beta installation instructions are located in the following areas:
- [**OpenNI SDK**](http://www.openni.ru/openni-sdk/index.html)
- Local documentation – under the **Documentation** folder

## Usage

### OpenNI 2.2 Beta Samples

The OpenNI 2.2 Beta contains several code samples ready to compile and run:
- **SimpleRead** – Demonstrates how to code a console application that opens a depth stream and read frames
- **MultipleStreamRead** – Demonstrates the same functionality as the SimpleRead sample, but this sample uses both the depth stream and the color stream
- **SimpleViewer** – Demonstrates how to code a graphic interface application that opens the depth and color streams and display them together on the screen simultaneously
- **MultiDepthViewer** – Demonstrates how to code an application that opens multiple depth streams simultaneously from a number of sensors on the same PC
- **EventBasedRead** – Demonstrates the same functionality as the SimpleRead sample but using an event based API
- **MWClosestPoint** – Demonstrates how to code a new middleware library over OpenNI (the sample analyzes frames to find the closest pixel)
- **MWClosestPointApp** – Demonstrates how to code a console application that uses the MWClosestPoint library
- **ClosestPointViewer** – Demonstrates how to code a graphic interface application that uses the MWClosestPoint library
- **Simpleviewer.Java**  – Demonstrates how to code a graphic interface application that opens the depth and color streams and display them together on the screen simultaneously in Java.

### Documentation

You can find the OpenNI 2.2 Beta Documentation at:
- [**OpenNI programmer’s Guide**](http://www.openni.ru/openni-programmers-guide/index.html)
- Local documentation – under “Documentation” folder





OpenNI与KinectV1 SDK的比较。

After playing with both the Microsoft Kinect SDK and the PrimeSense OpenNI SDK here are some of my thoughts,Note that the Microsoft’s SDK version is the Beta version, so things may change when the final one is released)


Microsoft’s Kinect SDK (Beta)pro: 优点

- support for audio 支持音频
- support for motor/tilt 支持马达
- full body tracking: 全身追踪
- does not need a calibration pose 不需要标定姿势（投降姿势）
- includes head, hands, feet, clavicles 包括头，手，脚和锁骨
- seems to deal better with occluded joints 看起来处理关节闭塞更好些 

- supports multiple sensors 支持多传感器（多台Kinect）
- single no-fuss installer 简化安装（安装更容易）
- SDK has events for when a new Video or new Depth frame is available 当新的视频或深度图有效时，SDK会有可用事件 


con: 缺点
- licensed for non-commercial use only 非商用（商业需要付费）
- only tracks full body (no mode for hand only tracking)  只能追踪全身（不包含特定的追踪模式：例如只追踪手）
- does not offer alignment of the color&depth image streams to one another yet 

- although there are features to align individual coordinates
- and there are hints that support may come later
full body tracking:  全身追踪
- only calculates positions for the joints, not rotations 
关节只有坐标数据，没有旋转数据 only tracks the full body, no upperbody or hands only mode 只能追踪全身，不包含特定的追踪模式：例如只追踪手或上半身
- seems to consume more CPU power than OpenNI/NITE (not properly benchmarked)  和OpenNI/NITE相比，看起来更消耗CPU（没有采用适当的基准）
no gesture recognition system  不包含手势识别系统 no support for the PrimeSense and the ASUS WAVI Xtion sensors? (can anyone confirm this?)  不支持PrimeSense和华硕的WAVI Xtion硬件平台 only supports Win7 (x86 & x64)   只支持Win7(32位和64位） no support for Unity3D game engine  不支持Unity3D游戏引擎 no built in support for record/playback to disk  不支持数据记录或回放到硬盘 no support to stream the raw InfraRed video data  不支持红外线视频数据流SDK does not have events for when new user enters frame, leaves frame etc  SDK没有此类发生事件，例如当一个用户被侦测到或用户丢失等等。 

PrimeSense OpenNI/NITEpro: 优点
- license includes commercial use 可以商用（不需要付费）
- includes a framework for hand tracking 包含手部追踪框架
- includes a framework for hand gesture recognition 包含手势识别框架
- can automatically align the depth image stream to the color image  可以自动对齐深度图数据到彩色图数据
- full body tracking:  全身追踪 
- also calculates rotations for the joints 包含坐标数据和旋转数据
- support for hands only mode 支持特殊跟踪模式：例如：只追踪手和头或上半身
- seems to consume less CPU power than Microsoft Kinect SDK’s tracker (not properly benchmarked) 和微软的SDK相比消耗的CPU更少

- also supports the Primesense and the ASUS WAVI Xtion sensors 支持Primesense和华硕的WAVI Xtion硬件平台
- supports multiple sensors although setup and enumeration is a bit quirky 支持多传感器但是需要安装和枚举，这一点有点古怪。
- supports Windows (including Vista&XP), Linux and Mac OSX 支持Windows（包括Vista&XP&WIN7）,Linux系统和苹果操作系统（翻者：也支持Android）
- comes with code for full support in Unity3D game engine  自带的代码全面支持Unity3D游戏引擎（翻者：也支持Ogre）
- support for record/playback to/from disk 支持数据记录到硬盘或从硬盘回放数据
- support to stream the raw InfraRed video data 支持红外数据流
- SDK has events for when new User enters frame, leaves frame etc SDK有此类发生事件，例如：当一个用户被侦测到或者用户丢失。（提供回调函数供开发者使用） 

con: 缺点
- no support for audio 不支持音频 （现在华硕xtion支持音频？http://www.asus.com.cn/Multimedia/Xtion_PRO_LIVE/）
- no support for motor/tilt (although you can simultaneously use the CL-NUI motor drivers) 不支持马达（翻者：马达是微软的专利，所以primesense公司不想惹微软）
- full body tracking:  全身追踪 
- lacks rotations for the head, hands, feet, clavicles 缺乏以下关节：头，手，脚，和锁骨
- needs a calibration pose to start tracking (although it can be saved/loaded to/from disk for reuse) 需要一个标定姿势（投降姿势）才能开始追踪骨骼（注意:标定数据是可以存储和提取的方便重用）
- occluded joints are not estimated 关节闭塞没有被估算

- supports multiple sensors although setup and enumeration is a bit quirky 支持多感应器但是需要安装和枚举，这一点有点古怪。
- three separate installers and a NITE license string (although the process can be automated with my auto driver installer)  需要单独安装NITE
- SDK does not have events for when new Video or new Depth frames is available SDK没有此类发生事件，例如：当新的视频或者深度图数据有效时。（翻者：OpenNI提供了类似功能的函数可使用，虽然不是回调函数，但是也很好用） 


(Personal) conclusion:Microsoft seems to have the edge when working with skeletons and/or audio.
微软在骨骼识别和音频方面有优势。（翻者：本人非常认同，微软的音频识别将会在未来的体感游戏里发挥重要的作用！）
OpenNI seems to be best suited when working on colored pointclouds, on non-Win7 platforms and/or for commercial projects.
OpenNI似乎更适合做一些带颜色的点云的工作，和在非Win7平台来开发商业项目。
When working with gestures in specific: 手势识别 

- - If your sensor only sees the upperbody/hands and/or you want an existing framework to start with use OpenNI/NITE. 
如果你想开发基于上半身或手识别的项目，可以使用OpenNI和NITE
- When your sensor can see the full body the more stable Microsoft skeleton may be the best to use, however you’ll have to code your own gesture recognitions. (You’d also have to extend OpenNI/NITE for fullbody gestures btw)
全身识别毋庸置疑微软的SDK是最好的，然而你必须自己编写你自己的手势识别代码。




平台上比较：
Kinect for Windows SDK 仅支持Windows 7/Windows 8 操作系统（PC平台为主，电视机，机顶盒等嵌入式平台为0）
OpenNI 支持大多数操作系统 

OpenNI各版本之间的比较：
OpenNI 1.X 版本  支持大多数操作系统，支持Kinect/Xtion独立驱动
OpenNI 2.X 版本  支持大多数操作系统，支持Xtion独立驱动，Kinect则要靠Kinect for Windows SDK自带的驱动做桥接。

简单的说：
**Kinect for Winows SDK 不允许非Kinect的摄像头使用。**
**OpenNI 2.X 不允许Kinect在非Windows操作系统上运行，其他体感摄像头支持良好。**



