
# Android系统架构开篇 - 嵌入式Linux - CSDN博客

2019年03月31日 00:01:51[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：8



![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Qof5hj3zMPeiauStJMDaoxoicA9rgjdWtkKj2ZZA5bCEcib9Z8bkfqBBZoO2eItzHChicibqZlWc4dP9LHIJIUGGLicg/640?wx_fmt=jpeg)
> Android系统庞大且错综复杂，Gityuan带领大家初探Android系统整体架构，一窥其全貌。


### 一、引言
本文作为Android系统架构的开篇，起到提纲挈领的作用，从系统整体架构角度概要讲解Android系统的核心技术点，带领大家初探Android系统全貌以及内部运作机制。虽然Android系统非常庞大且错综复杂，需要具备全面的技术栈，但整体架构设计清晰。Android底层内核空间以Linux Kernel作为基石，上层用户空间由Native系统库、虚拟机运行环境、框架层组成，通过系统调用(Syscall)连通系统的内核空间与用户空间。对于用户空间主要采用C++和Java代码编写，通过JNI技术打通用户空间的Java层和Native层(C++/C)，从而连通整个系统。
为了能让大家整体上大致了解Android系统涉及的知识层面，先来看一张Google官方提供的经典分层架构图，从下往上依次分为Linux内核、HAL、系统Native库和Android运行时环境、Java框架层以及应用层这5层架构，其中每一层都包含大量的子模块或子系统。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Qof5hj3zMPfFmjpu9eDicO4IN5lG2uxFpwiaSyQkic2m9zXMoq4M6GYhxKXGAeLytXPD9jjDq4YdSg5XWicQES4kNg/640?wx_fmt=jpeg)上图采用静态分层方式的架构划分，众所周知，程序代码是死的，系统运转是活的，各模块代码运行在不同的进程(线程)中，相互之间进行着各种错终复杂的信息传递与交互流，从这个角度来说此图并没能体现Android整个系统的内部架构、运行机理，以及各个模块之间是如何衔接与配合工作的。**为了更深入地掌握Android整个架构思想以及各个模块在Android系统所处的地位与价值，计划以Android系统启动过程为主线，以进程的视角来诠释Android M系统全貌**，全方位的深度剖析各个模块功能，争取各个击破。这样才能犹如庖丁解牛，解决、分析问题则能游刃有余。
### 二、Android架构
Google提供的5层架构图很经典，但为了更进一步透视Android系统架构，本文更多的是以进程的视角，以分层的架构来诠释Android系统的全貌，阐述Android内部的环环相扣的内在联系。
**系统启动架构图**
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Qof5hj3zMPfFmjpu9eDicO4IN5lG2uxFp5peEozqyOGPQIj0hY5JKzum38jrcTBOYw4oCTLn9gibpoN5nOzWjxlA/640?wx_fmt=jpeg)
**图解：**Android系统启动过程由上图从下往上的一个过程是由Boot Loader引导开机，然后依次进入 ->
```python
Kernel
```
->
```python
Native
```
->
```python
Framework
```
->
```python
App
```
，接来下简要说说每个过程：
关于Loader层：
Boot ROM: 当手机处于关机状态时，长按Power键开机，引导芯片开始从固化在ROM里的预设代码开始执行，然后加载引导程序到RAM；
Boot Loader：这是启动Android系统之前的引导程序，主要是检查RAM，初始化硬件参数等功能。

#### 2.1 Linux内核层
Android平台的基础是Linux内核，比如ART虚拟机最终调用底层Linux内核来执行功能。Linux内核的安全机制为Android提供相应的保障，也允许设备制造商为内核开发硬件驱动程序。
启动Kernel的swapper进程(pid=0)：该进程又称为idle进程, 系统初始化过程Kernel由无到有开创的第一个进程, 用于初始化进程管理、内存管理，加载Display,Camera Driver，Binder Driver等相关工作；
启动kthreadd进程（pid=2）：是Linux系统的内核进程，会创建内核工作线程kworkder，软中断线程ksoftirqd，thermal等内核守护进程。kthreadd进程是所有内核进程的鼻祖。

#### 2.2 硬件抽象层 (HAL)
硬件抽象层 (HAL) 提供标准接口，HAL包含多个库模块，其中每个模块都为特定类型的硬件组件实现一组接口，比如WIFI/蓝牙模块，当框架API请求访问设备硬件时，Android系统将为该硬件加载相应的库模块。
#### 2.3 Android Runtime & 系统库
每个应用都在其自己的进程中运行，都有自己的虚拟机实例。ART通过执行DEX文件可在设备运行多个虚拟机，DEX文件是一种专为Android设计的字节码格式文件，经过优化，使用内存很少。ART主要功能包括：预先(AOT)和即时(JIT)编译，优化的垃圾回收(GC)，以及调试相关的支持。
这里的Native系统库主要包括init孵化来的用户空间的守护进程、HAL层以及开机动画等。启动init进程(pid=1),是Linux系统的用户进程，
```python
init
```
```python
进程是所有用户进程的鼻祖
```
。
init进程会孵化出ueventd、logd、healthd、installd、adbd、lmkd等用户守护进程；
init进程还启动servicemanager(binder服务管家)、bootanim(开机动画)等重要服务
init进程孵化出Zygote进程，Zygote进程是Android系统的第一个Java进程(即虚拟机进程)，Zygote是所有Java进程的父进程，Zygote进程本身是由init进程孵化而来的。

#### 2.4 Framework层
Zygote进程，是由init进程通过解析init.rc文件后fork生成的，Zygote进程主要包含：
加载ZygoteInit类，注册Zygote Socket服务端套接字
加载虚拟机
提前加载类preloadClasses
提前加载资源preloadResouces
System Server进程，是由Zygote进程fork而来，SystemServer是Zygote孵化的第一个进程，System Server负责启动和管理整个Java framework，包含ActivityManager，WindowManager，PackageManager，PowerManager等服务。
Media Server进程，是由init进程fork而来，负责启动和管理整个C++ framework，包含AudioFlinger，Camera Service等服务。

#### 2.5 App层
Zygote进程孵化出的第一个App进程是Launcher，这是用户看到的桌面App；
Zygote进程还会创建Browser，Phone，Email等App进程，每个App至少运行在一个进程上。
所有的App进程都是由Zygote进程fork生成的。

#### 2.6 Syscall && JNI
Native与Kernel之间有一层系统调用(SysCall)层，见Linux系统调用(Syscall)原理;
Java层与Native(C/C++)层之间的纽带JNI，见Android JNI原理分析。

### 三、通信方式
无论是Android系统，还是各种Linux衍生系统，各个组件、模块往往运行在各种不同的进程和线程内，这里就必然涉及进程/线程之间的通信。对于IPC(Inter-Process Communication, 进程间通信)，Linux现有管道、消息队列、共享内存、套接字、信号量、信号这些IPC机制，Android额外还有Binder IPC机制，Android OS中的Zygote进程的IPC采用的是Socket机制，在上层system server、media server以及上层App之间更多的是采用Binder IPC方式来完成跨进程间的通信。对于Android上层架构中，很多时候是在同一个进程的线程之间需要相互通信，例如同一个进程的主线程与工作线程之间的通信，往往采用的Handler消息机制。
想深入理解Android内核层架构，必须先深入理解Linux现有的IPC机制；对于Android上层架构，则最常用的通信方式是Binder、Socket、Handler，当然也有少量其他的IPC方式，比如杀进程Process.killProcess()采用的是signal方式。下面说说Binder、Socket、Handler：
#### 3.1 Binder
Binder作为Android系统提供的一种IPC机制，无论从系统开发还是应用开发，都是Android系统中最重要的组成，也是最难理解的一块知识点，想了解为什么Android要采用Binder作为IPC机制？ 可查看我在知乎上的回答。深入了解Binder机制，最好的方法便是阅读源码，借用Linux鼻祖Linus Torvalds曾说过的一句话：Read The Fucking Source Code。下面简要说说Binder IPC原理。
**Binder IPC原理**
Binder通信采用c/s架构，从组件视角来说，包含Client、Server、ServiceManager以及binder驱动，其中ServiceManager用于管理系统中的各种服务。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Qof5hj3zMPfFmjpu9eDicO4IN5lG2uxFpBumsSO38ayf3ia0y7pqWAIEkyzgmpjzwaYJYTZIVZFd8Mz86FpQy2hw/640?wx_fmt=jpeg)想进一步了解Binder，可查看Binder系列—开篇，Binder系列花费了13篇文章的篇幅，从源码角度出发来讲述Driver、Native、Framework、App四个层面的整个完整流程。根据有些读者反馈这个系列还是不好理解，这个binder涉及的层次跨度比较大，知识量比较广，建议大家先知道binder是用于进程间通信，有个大致概念就可以先去学习系统基本知识，等后面有一定功力再进一步深入研究Binder机制。
**Binder原理篇**
|序号|文章名|概述|
|---|---|---|
|0|Binder系列—开篇|Binder概述|
|1|Binder系列3—启动Service Manager|ServiceManager守护进程 注册和查询服务|
|2|Binder系列4—获取Service Manager|获取代理对象BpServiceManager|
|3|Binder系列5—注册服务(addService)|注册Media服务|
|4|Binder系列6—获取服务(getService)|获取Media代理，以及DeathRecipient|
|5|Binder系列7—framework层分析|framework层服务注册和查询，Binder注册|
|6|理解Binder线程池的管理|Binder的startThreadPool过程|
|7|彻底理解Android Binder通信架构|startService为主线|
|8|Binder系列10—总结|Binder的简单总结|
|9|Binder IPC的权限控制|clearCallingIdentity/restoreCallingIdentity|
|10|Binder死亡通知机制之linkToDeath|Binder死亡通知机制|
**Binder驱动篇:**
|1|Binder Driver初探|驱动open/mmap/ioctl，以及binder结构体|
|2|Binder Driver再探|Binder通信协议，内存机制|
**Binder使用篇:**
|1|如何使用Binder|Native层、Framwrok层自定义Binder服务|
|2|如何使用AIDL|App层自定义Binder服务|

#### 3.2 Socket
Socket通信方式也是C/S架构，比Binder简单很多。在Android系统中采用Socket通信方式的主要有：
zygote：用于孵化进程，system_server创建进程是通过socket向zygote进程发起请求；
installd：用于安装App的守护进程，上层PackageManagerService很多实现最终都是交给它来完成；
lmkd：lowmemorykiller的守护进程，Java层的LowMemoryKiller最终都是由lmkd来完成；
adbd：这个也不用说，用于服务adb；
logcatd:这个不用说，用于服务logcat；
vold：即volume Daemon，是存储类的守护进程，用于负责如USB、Sdcard等存储设备的事件处理。
等等还有很多，这里不一一列举，Socket方式更多的用于Android framework层与native层之间的通信。Socket通信方式相对于binder比较简单，这里省略。
#### 3.3 Handler
**Binder/Socket用于进程间通信，而Handler消息机制用于同进程的线程间通信**，Handler消息机制是由一组MessageQueue、Message、Looper、Handler共同组成的，为了方便且称之为Handler消息机制。
有人可能会疑惑，为何Binder/Socket用于进程间通信，能否用于线程间通信呢？答案是肯定，对于两个具有独立地址空间的进程通信都可以，当然也能用于共享内存空间的两个线程间通信，这就好比杀鸡用牛刀。接着可能还有人会疑惑，那handler消息机制能否用于进程间通信？答案是不能，Handler只能用于共享内存地址空间的两个线程间通信，即同进程的两个线程间通信。很多时候，Handler是工作线程向UI主线程发送消息，即App应用中只有主线程能更新UI，其他工作线程往往是完成相应工作后，通过Handler告知主线程需要做出相应地UI更新操作，Handler分发相应的消息给UI主线程去完成，如下图：
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Qof5hj3zMPfFmjpu9eDicO4IN5lG2uxFpMccCpt0ZAhf3JCowX47VR2LNziaiaWB6UDfcqDXQWkCnkUTSiccjM5tibQ/640?wx_fmt=jpeg)由于工作线程与主线程共享地址空间，即Handler实例对象mHandler位于线程间共享的内存堆上，工作线程与主线程都能直接使用该对象，只需要注意多线程的同步问题。工作线程通过mHandler向其成员变量MessageQueue中添加新Message，主线程一直处于loop()方法内，当收到新的Message时按照一定规则分发给相应的handleMessage()方法来处理。所以说，Handler消息机制用于同进程的线程间通信，其核心是线程间共享内存空间，而不同进程拥有不同的地址空间，也就不能用handler来实现进程间通信。
上图只是Handler消息机制的一种处理流程，是不是只能工作线程向UI主线程发消息呢，其实不然，可以是UI线程向工作线程发送消息，也可以是多个工作线程之间通过handler发送消息。更多关于Handler消息机制文章：
Android消息机制-Handler(framework篇)
Android消息机制-Handler(native篇)
Android消息机制3-Handler(实战)
要理解framework层源码，掌握这3种基本的进程/线程间通信方式是非常有必要，当然Linux还有不少其他的IPC机制，比如共享内存、信号、信号量，在源码中也有体现，如果想全面彻底地掌握Android系统，还是需要对每一种IPC机制都有所了解。
### 四、核心提纲
博主对于Android从系统底层一路到上层都有自己的理解和沉淀，通过前面对系统启动的介绍，相信大家对Android系统有了一个整体观。接下来需抓核心、理思路，争取各个击破。后续将持续更新和完善整个大纲，不限于进程、内存、IO、系统服务架构以及分析实战等文章。
当然本站有一些文章没来得及进一步加工，有时间根据大家的反馈，不断修正和完善所有文章，争取给文章，再进一步精简非核心代码，增加可视化图表以及文字的结论性分析。基于**Android 6.0的源码**，专注于分享Android系统原理、架构分析的原创文章。
**建议阅读群体**： 适合于正从事或者有兴趣研究Android系统的工程师或者技术爱好者，也适合Android App高级工程师；对于尚未入门或者刚入门的App工程师阅读可能会有点困难，建议先阅读更基础的资料，再来阅读本站博客。
看到Android整个系统架构是如此庞大的, 该问如何学习Android系统, 以下是我自己的Android的学习和研究论，仅供参考如何自学Android。
从整理上来列举一下Android系统的核心知识点概览：
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Qof5hj3zMPfFmjpu9eDicO4IN5lG2uxFp8Cg4WCcJnY7zR4BWCqZZzBPMUesTNn2lzbGjqTHLYjVn0B5ZVNBHwA/640?wx_fmt=jpeg)4.1 系统启动系列
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Qof5hj3zMPfFmjpu9eDicO4IN5lG2uxFpx7hz1knOGGFtfJ8Ze5PTlpooUloWUIHuUx7GZx2mHIQnohIlE2zATA/640?wx_fmt=jpeg)
Android系统启动-概述:
Android系统中极其重要进程：init, zygote, system_server, servicemanager 进程:
|序号|进程启动|概述|
|---|---|---|
|1|init进程|Linux系统中用户空间的第一个进程, Init.main|
|2|zygote进程|所有Ａpp进程的父进程, ZygoteInit.main|
|3|system_server进程(上篇)|系统各大服务的载体, forkSystemServer过程|
|4|system_server进程(下篇)|系统各大服务的载体, SystemServer.main|
|5|servicemanager进程|binder服务的大管家, 守护进程循环运行在binder_loop|
|6|App进程|通过Process.start启动App进程, ActivityThread.main|

再来看看守护进程(也就是进程名一般以d为后缀，比如logd，此处d是指daemon的简称), 下面介绍部分守护进程：
debuggerd
installd
lmkd
logd

#### 4.2 系统稳定性系列
Android系统稳定性主要是异常崩溃(crash)和执行超时(timeout),:
|序号|文章名|概述|
|---|---|---|
|1|理解Android ANR的触发原理|触发ANR的场景以及机理|
|2|Input系统—ANR原理分析|input触发ANR的原理|
|3|理解Android ANR的信息收集过程|AMS.appNotResponding过程分析,收集traces|
|4|解读Java进程的Trace文件|kill -3 信息收集过程|
|5|Native进程之Trace原理|debuggerd -b 信息收集过程|
|6|WatchDog工作原理|WatchDog触发机制|
|7|理解Java Crash处理流程|AMS.handleApplicationCrash过程分析|
|8|理解Native Crash处理流程|debuggerd守护进程|
|9|global reference限制策略|global reference|

#### 4.3 Android进程系列
进程/线程是操作系统的魂，各种服务、组件、子系统都是依附于具体的进程实体。深入理解进程机制对于掌握Android系统整体架构和运转机制是非常有必要的，是系统工程师的基本功，下面列举进程相关的文章：
|序号|文章名|概述|
|---|---|---|
|1|理解Android进程创建流程|Process.start过程分析|
|2|理解杀进程的实现原理|Process.killProcess过程分析|
|3|Android四大组件与进程启动的关系|AMS.startProcessLocked过程分析组件与进程|
|4|Android进程绝杀技--forceStop|force-stop过程分析彻底移除组件与杀进程|
|5|理解Android线程创建流程|3种不同线程的创建过程|
|6|彻底理解Android Binder通信架构|以start-service为线,阐述进程间通信机理|
|7|理解Binder线程池的管理|Zygote fork的进程都默认开启binder线程池|
|8|Android进程生命周期与ADJ|进程adj, processState以及lmk|
|9|Android LowMemoryKiller原理分析|lmk原理分析|
|10|进程优先级|进程nice,thread priority以及scheduler|
|11|Android进程调度之adj算法|updateOomAdjLocked过程|
|12|Android进程整理|整理系统的所有进程/线程|

#### 4.4 四大组件系列
对于App来说，Android应用的四大组件Activity，Service，Broadcast Receiver， Content Provider最为核心，接下分别展开介绍：
|序号|文章名|类别|
|---|---|---|
|1|startActivity启动过程分析|Activity|
|2|简述Activity生命周期|Activity|
|3|startService启动过程分析|Service|
|4|bindService启动过程分析|Service|
|5|以Binder视角来看Service启动|Service|
|6|Android Broadcast广播机制分析|Broadcast|
|7|理解ContentProvider原理|ContentProvider|
|8|ContentProvider引用计数|ContentProvider|
|9|Activity与Service生命周期|Activity&&Service|
|10|简述Activity与Window关系|Activity&&Window|
|11|四大组件之综述|AMS|
|12|四大组件之ServiceRecord|Service|
|13|四大组件之BroadcastRecord|Broadcast|
|14|四大组件之ContentProviderRecord|ContentProvider|
|15|理解Android Context|Context|
|16|理解Application创建过程|Application|
|17|unbindService流程分析|Service|
|18|四大组件之ActivityRecord|Activity|
|19|AMS总结(一)|AMS|

#### 4.5 图形系统系列
图形也是整个系统非常复杂且重要的一个系列，涉及WindowManager,SurfaceFlinger服务。
|序号|文章名|类别|
|---|---|---|
|1|WindowManager启动篇|Window|
|2|WMS之启动窗口篇|Window|
|3|以Window视角来看startActivity|Window|
|4|Android图形系统概述|SurfaceFlinger|
|5|SurfaceFlinger启动篇|SurfaceFlinger|
|6|SurfaceFlinger绘图篇|SurfaceFlinger|
|7|Choreographer原理|Choreographer|

#### 4.6 系统服务篇
再则就是在整个架构中有大量的服务，都是基于Binder来交互的，Android系统服务的注册过程也是在此之上的构建的。计划针对部分核心服务来重点分析：
AMS服务
AMS启动过程（一）
更多组件篇[见小节4.3]
Input系统
Input系统—启动篇
Input系统—InputReader线程
Input系统—InputDispatcher线程
Input系统—UI线程
Input系统—进程交互
Input系统—ANR原理分析
PKMS服务
PackageManager启动篇
Installd守护进程
Alarm服务
理解AlarmManager机制
JobScheduler服务
理解JobScheduler机制
BatteryService
Android耗电统计算法
PMS服务
DropBox服务
DropBoxManager启动篇
UserManagerService
多用户管理UserManager
更多系统服务
敬请期待

#### 4.7 内存&&存储篇
内存篇
Android LowMemoryKiller原理分析
Linux内存管理
Android内存分析命令

存储篇
Android存储系统之源码篇
Android存储系统之架构篇

Linux驱动篇
敬请期待

dalvik/art
解读Java进程的Trace文件

#### 4.8 工具篇
再来说说Android相关的一些常用命令和工具以及调试手段.
|序号|文章名|类别|
|---|---|---|
|1|理解Android编译命令|build|
|2|理解Android.bp|build|
|2|性能工具Systrace|systrace|
|3|Android内存分析命令|Memory|
|4|ps进程命令|Process|
|5|Am命令用法|Am|
|6|Pm命令用法|Pm|
|7|调试系列1：bugreport源码篇|bugreport|
|8|调试系列2：bugreport实战篇|bugreport|
|9|dumpsys命令用法|dumpsys|
|10|Android logd日志原理|logd|
|11|介绍gdb调试工具|gdb|
|12|介绍addr2line调试命令|addr2line|

#### 4.9 实战篇
下面列举处理过的部分较为典型的案例，供大家参考
|序号|文章名|类别|
|---|---|---|
|1|Binder Driver缺陷导致定屏的案例|binder|
|2|深度解读ArrayMap优势与缺陷|ArrayMap|
|3|数组越界导致系统重启的案例|数组越界|
|4|一行Log引发多线程并发问题的案例|多线程并发|
|5|跑monkey压力测试过程的冻屏案例|monkey冻屏|
|6|深度剖析APP保活案例|保活|
本文只是提纲挈领的篇章，更多详细文章可点击下方阅读原文，进入我的博客Gityuan.com阅读，博主仍在持续更新。
### 五、结束语
Android系统之博大精深，包括Linux内核、Native、虚拟机、Framework，通过系统调用连通内核与用户空间，通过JNI打通用户空间的Java层和Native层，通过Binder、Socket、Handler等打通跨进程、跨线程的信息交换。只有真正阅读并理解系统核心架构的设计，解决问题和设计方案才能做到心中无剑胜有剑，才能做到知其然知其所以然。当修炼到此，恭喜你对系统有了更高一个层次的理解，正如太极剑法，忘记了所有招式，也就练成了太极剑法。
再回过头去看看那些API，看到的将不再是一行行代码、一个个接口的调用，而是各种信息的传递与交互工作，而是背后成千上万个小蝌蚪的动态执行流。记得《侠客行》里面的龙木二岛主终其一生也无法参透太玄经，石破天却短短数日练成绝世神功，究其根源是龙木二岛主以静态视角去解读太玄经，而石破天把墙壁的图案想象成无数游动的蝌蚪，最终成就绝世神功。一言以蔽之，程序代码是死的，系统运转是活的，要以动态视角去理解系统架构。

推荐阅读：
[Android技术架构演进与未来](https://mp.weixin.qq.com/s?__biz=MzI5NjE3NzA4Mg==&mid=2650359899&idx=1&sn=76a156a24ba4116e19f8a5ef85a9ecaa&chksm=f445b48ec3323d987a1a165d832c2befd79ca5c39711b29f281c8ec5e9ecf69c63a54de53688&token=1566461568&lang=zh_CN&scene=21#wechat_redirect)
[深度解读ArrayMap优势与缺陷](https://mp.weixin.qq.com/s?__biz=MzI5NjE3NzA4Mg==&mid=2650359887&idx=1&sn=e170eedc77b782f3bc02a407627886dd&chksm=f445b49ac3323d8cafdc682bd83369c58d7da4bfecd345440622ea5d4f1a36174b6d0c001022&token=1566461568&lang=zh_CN&scene=21#wechat_redirect)
[解读Android进程优先级ADJ算法](http://mp.weixin.qq.com/s?__biz=MzI5NjE3NzA4Mg==&mid=2650359872&idx=1&sn=0a6d471f98fbf641e316722bae5a4982&chksm=f445b495c3323d835d7c4ca0134868456805824b55865e014ea5afe05185db7c25654eb143a1&scene=21#wechat_redirect)
[Binder Driver缺陷导致定屏的实战分析](http://mp.weixin.qq.com/s?__biz=MzI5NjE3NzA4Mg==&mid=2650359848&idx=1&sn=b22e13fd95e2bd2137d6742ddc5e48a5&chksm=f445b4fdc3323deb2ebeb4e135d56b19db3dbae49ffc0a0b3cedd1f0d713de8bfbfbd75bd260&scene=21#wechat_redirect)


![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Qof5hj3zMPfFmjpu9eDicO4IN5lG2uxFpH6fcYicEXZHnDNgyWFo14icBjY80EHC2IKegk1VKe46rUTPg34dzK9Ug/640?wx_fmt=jpeg)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/Qof5hj3zMPfFmjpu9eDicO4IN5lG2uxFpmmnnYNKsYsoq8Eqw9DNEc6XVx7Pd0rVkkRGspxG4HvdUibWLXCdMg8A/640?wx_fmt=png)



觉得不错
请点在看
↓





