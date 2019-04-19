# 2011年Android Camera学习笔记之一 - 2019跟着小虎玩着去软考 - CSDN博客
2011年04月08日 10:29:00[littletigerat](https://me.csdn.net/littletigerat)阅读数：2066
# 2011年Android Camera学习笔记之一
**一.代码所在目录**frameworks/base/libs/camera
frameworks/base/services/camera
frameworks/base/include/camera
frameworks/base/core/jni
frameworks/base/graphics/java/android/graphics/Camera.java
frameworks/base/core/java/android/hardware/Camera.java
**二.用到哪些库**1.libui.so ；
2.libandroid_runtime.so ；
3.libcameraservice.so ；
4.libcamera.so；
5.libcamerastub.so；
6.libcamera_client.so；
7.libcameraextensionclient.so；
8.libcameraextensionservice.so；
9.libcameraextensionclientjni.so；
10.libcald_omxcamera.so；
11.liboemcamera.so
12.libcameralight.so
13.libcald_omxcamera_plugin.so
**三.进程间的通讯机制**Binder机制
**四.哪个类提供Camera主要功能的接口**ICamera
**五.整个Camera在运行的时候**分成Client和Server端
**六.两大功能：**
1.取景：preview；
2.拍摄照片：take picture；
**七.程序的架构**1.Client;
2.Server;
在 client调用接口，功能在server中实现；
**八.MediaServer process进程注册三大Server：**
AudioFlinger
MediaPlayerServer
CameraService
AndroidRuntime
JNI
CameraHardwareStub
CameraService
**九****.BaseController**1.CameraController；
2.VideoController；
**十.Java application process拥有的对象：**
Camera.jar
Camera.cpp
ICamera
ICameraService
**十一.****两大****Processs**1.Native Process:
Native process姑且认为是以c,c++语言编写服务程序（是独立的一个进程）。
2.JAVA Process:
JAVA process是通常说的Android Application
**十二.****文件**
frameworks/base/libs/camera/ICameraService.cpp
frameworks/base/include/camera/ICameraService.h
**十三.****头文件**
frameworks/base/include/camera
1.Camera.h;
2.ICamera.h;
3.ICameraClient.h;
4.ICameraService.h;
5.CameraParameters.h;
6.CameraHardwareInterface.h;
