# STM32Cube_FW_F1_V1.0.0固件库学习（一）环境准备 - xqhrs232的专栏 - CSDN博客
2018年03月16日 10:32:21[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：929
原文地址::[https://www.cnblogs.com/nety0403/p/4503419.html](https://www.cnblogs.com/nety0403/p/4503419.html)
相关文章
1、STM32Cube_FW_F1_V1.6.0----[https://download.csdn.net/download/hacker1994/10212542?utm_source=bbsseo](https://download.csdn.net/download/hacker1994/10212542?utm_source=bbsseo)
2、stm32cube_fw_f1_v160.zip STM32CubeF4 V1.16.0----[https://download.csdn.net/download/caiyunfreedom/10020486](https://download.csdn.net/download/caiyunfreedom/10020486)
之前很是惭愧，写了几篇，虽然嘴上说太忙，还是懒病发作，就停了。没有养成好习惯。 还有一个想吐槽下，这个传图片好麻烦啊啊啊 估计之前也是因为这个停了。。。
    虽然还是对之前公司的项目非常感兴趣，因为地域缘故，今年来还是换了工作，这边项目还没开始，陆陆续续到现在也有好几个月时间，
也看了很多东西。回头看看，啥记录也没有，也不知道是不是现在年纪大了，感觉啥东西也没记住。看了点啥也不知道了。这次重新下决心，
争取能够每天记录一下。最少也得把这个事情做完，以此勉励自己。
    本人之前ARM7 M0 M3 430 CY68013等等也都有接触过，板子也买了一堆，不过原来那边因为各种原因，基本上都是一个人在弄，事情
太多，软件上的一直没有系统的学习。430还算用的最多了，写了一个生产检测治具的代码。其他相当于就是入了个门。这次项目下来，很可能是基于STM32 UCOS系统。重新学习下。争取跟上时代。
虽然STM32已经很久，真的相当于是从0开始。这次一定要完完整整啃下来。
    网上现在的资料基本上都是基于STM32官方固件库V3.5的版本。各种视频各种资料。这个是后面才看到的版本。
    小白起手就直奔ST官网下来一堆资料，结果发现官方已经更新了固件库，还有配套软件帮助你自动生成代码。但是固件库已经是新版的V1.0 ，姑且称他为V4.0吧。与原来的结构V3.5还是有一定的区别。
各种命名都有了一些变化。为了逼迫自己这次不要再各种参考代码，还是觉得从最新的版本入手。多花一些时间，搞清楚为什么这么写吧。
    翻箱很久，找到一块N年前买的基于STM32F107VCT 神州IV号的开发板。就基于这块平台做我的STM32学习吧。
    下载器原来的JLINK找不到了，公司有配ulink2，那就用ulink2吧。
    背景介绍完毕，下面来讲碰到的第一个心碎的问题，直接搞了半天，那个揪心。。。
    问题1.1：开发板连接Ulink2后，只能下载一次程序，第二次就识别不了ULINK2，下载不了程序
为什么这么设置，是因为新手小白没用过ULINK2啊，所以设置是参考ULINK2官方给的帮助文档设置的。结果。。。。
 将就解决:将BOOT0的跳线帽从FLASH启动改成从ROM启动，就不会出现上面问题。但是总不能老是这样啊。然后各种查，各种试，各种崩溃
    最终解决：把下面红框那个从normail改成under Reset就好了。。。血泪教训啊
那这个到底是什么意思，参考官方文档，如下：
# Debug
The Debug section controls caching, downloading, and device resetting options applied each time a debugging session is started.
![Target Driver Setup - Connect and Reset Options](https://www.cnblogs.com/ulink2_target_ctx_driver5.png)
Connect & Reset Options
control the instructions executed while connecting to target.
Connect controls the connection conditions.
- Connect — Normal stops the CPU at the currently executed instruction after connecting.
- Connect — with Pre-reset applies a hardware reset(HW RESET) before connecting to the device.
- Connect — under Reset holds
 the hardware reset(HW RESET) signal active while connecting to the device.Use this option when the user program disables the JTAG/SW interface.
Reset after Connect, if enabled, performs a reset operation as defined in the Reset drop-down list (see below), after connecting to the target. If disabled, the debugger
 just stops the CPU at the currently executed instruction after connecting. Refer also to [Start Sequence](http://www.cnblogs.com/ulink2_hw_start_sequence.htm) for details.
Reset controls the RESET operations.
- Reset — HW RESET performs a hardware reset by asserting the hardware reset (HW RESET) signal.
- Reset — SYSRESETREQ performs a software reset by setting the SYSRESETREQ bit. The Cortex-M core and on-chip peripherals are reset.
- Reset — VECTRESET performs a software reset by setting the VECTRESET bit. Only the Cortex-M core is reset. The on-chip peripherals are not affected. For
 some Cortex-M devices, VECTRESET is the only way to reset the core. VECTRESET is not supported on Cortex-M0 and Cortex-M1 cores.
- Reset — Autodetect selects one of the above reset methods based on the target device. The SYSRESETREQ method is used if an unknown device is detected.
Stop after Bootloader stops the CPU after executing the bootloader instructions.
看上面的信息，就是Under Reset 与Normal区别就是一个只是停了CPU 一个在按整版的复位键。不知道是不是因为我的神州IV板子设计的缘故。。。
网上也有找到有人跟我碰到一样的问题，如果亲也是什么都试了不行的话，可以改这个看一下。。。
至于为什么。。。我也不知道。。。先Next吧。。。回头理完再来看看。
如果有知道的大牛，还请多多指点。
——————————————————————————————————————————————————
。。因为未知的原因，又一次识别不了了。这次改什么设置都不能用。
然后最后用串口下载的方式，擦除整个FLASH，又可以了！
如果亲们也再碰到，可以一试
