# Android Camera Framework Stream(一) - xqhrs232的专栏 - CSDN博客
2011年06月17日 13:31:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：881标签：[android																[stream																[jni																[通讯																[service																[java](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=service&t=blog)](https://so.csdn.net/so/search/s.do?q=通讯&t=blog)](https://so.csdn.net/so/search/s.do?q=jni&t=blog)](https://so.csdn.net/so/search/s.do?q=stream&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/miliang1688/archive/2011/01/17/6146262.aspx](http://blog.csdn.net/miliang1688/archive/2011/01/17/6146262.aspx)
现在我们尝试从最开始的启动流程来熟悉android camera的整体framework流程:
首先从上图的各个步骤来逐一分析流程，后续会根据具体的一些点进行内容的添加和扩充：
? Camera.java
packages/apps/camera/src/com/android/
最上层的应用就是从这个文件开始。
该文件集中了整个android上层应用的所有相关内容，当然更多的则为界面的代码实现。
如果出现了camera应用界面的问题(当然除了camera拍摄区域内容外)，可以从android的代码入手。
? Camera.java
frameworks/base/core/java/android/hardware/
该文件中主要是对native函数接口的调用，当然也包括一些本地的函数实现。
也可以认为该文件是实现了从java层调用c++层代码函数接口。
也就是我们需要去了解的一点JNI机制。
? android_hardware_Camera.cpp
该文件就是JNI的c++层的代码实现。
通过camera的类实例来调用camera类的相关接口。
? Camera.cpp/Camera.h
对于上层应用来说，camera.cpp是最为直接的函数调用和实现。
继承于ICameraClient类,典型的Client端的接口实例。
? BnCameraClient/BpCameraClient
IPC通讯所需的函数接口实现，继承于ICameraClient类。
? ICameraClient.cpp/ICameraClient.h
Client/Service模式下的Client端实现
? ICameraService.cpp/ICameraService.h
Client/Service模式下service端实现
? BnCameraService/BpCameraService
IPC通讯所需的函数接口实现，继承于ICameraService类。
? CameraService.cpp/CameraService.h
继承于BnCameraService类。
是对BnCameraService函数接口的实现，其本质也是对CameraService的内部类Client函数接口的调用。
? Client(CameraService内部类)
该类才是真正的底层函数实现，其通过openCameraHardware()得到camera硬件实例对象进行操作。
其继承于ICamera类,是对ICamera类函数接口的实现。
转自：[http://blog.163.com/shawpin@126/blog/static/116663752201092394147937/](http://blog.163.com/shawpin@126/blog/static/116663752201092394147937/)
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/miliang1688/archive/2011/01/17/6146262.aspx](http://blog.csdn.net/miliang1688/archive/2011/01/17/6146262.aspx)

