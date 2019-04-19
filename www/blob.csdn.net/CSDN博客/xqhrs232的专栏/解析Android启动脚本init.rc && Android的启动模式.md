# 解析Android启动脚本init.rc && Android的启动模式 - xqhrs232的专栏 - CSDN博客
2012年03月16日 17:34:58[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1195
原文地址::[http://blog.csdn.net/zhandoushi1982/article/details/5767634](http://blog.csdn.net/zhandoushi1982/article/details/5767634)
相关网帖::Android启动脚本init.rc----[http://hi.baidu.com/donghaozheng/blog/item/e2f5045577cb73c8b645aec0.html](http://hi.baidu.com/donghaozheng/blog/item/e2f5045577cb73c8b645aec0.html)----摘自《Android系统原理及开发要点详解》Page 70
init.rc介绍说明性文件----Androidsourcecode/system/core/init/readme.txt
在Android中使用启动脚本init.rc，可以在系统的初始化过程中进行一些简单的初始化操作。这个脚本被直接安装到目标系统的根文件系统中，被 init可执行程序解析。
      init.rc是在init启动后被执行的启动脚本，其语法主要包含了以下内容：
Commands：命令
Actions： 动作
Triggers：触发条件
Services：服务
Options：选项
Propertise：属性
（1） Commands是一些基本的操作，例如：
    mkdir /sdcard 0000 system system
    mkdir /system
    mkdir /data 0771 system system
    mkdir /cache 0770 system cache
    mkdir /config 0500 root root
    mkdir /sqlite_stmt_journals 01777 root root
    mount tmpfs tmpfs /sqlite_stmt_journals size=4m
这些命令在init可执行程序中被解析，然后调用相关的函数来实现。
（2）Actions(动作)表示一系列的命令，通常在Triggers（触发条件）中调用，动作和触发条件的形式为：
   on <trigger>
      <command>
      <command>
      <command>
动作的使用示例如下：
    on init
    export PATH /sbin:/system/sbin:/system/bin:/system/xbin
    mkdir /system
init表示一个触发条件，这个触发事件发生后，进行设置环境变量和建立目录的操作称为一个“动作”。
（3）Services（服务）通常表示启动一个可执行程序，Options（选项）是服务的附加内容，用于配合服务使用。
service vold /system/bin/vold
    socket vold stream 0660 root mount
service bootsound /system/bin/playmp3
    user media
    group audio
    oneshot
vold和bootsound分别是两个服务的名称，/system/bin/vold和/system /bin/playmp3分别是他们所对应的可执行程序。socket、user、group、oneshot就是配合服务使用的选项。其中oneshot选项表示该服务只启动一次，而如果没有oneshot选项，这个可执行程序会一直存在--如果可执行程序被杀死，则会重新启动。
（4） Properties（属性）是系统中使用的一些值，可以进行设置和读取。
    setprop ro.FOREGROUND_APP_MEM 1536
    setprop ro.VISIBLE_APP_MEM 2048
    on property:ro.kernel.qemu=1
    start adbd
setprop 用于设置属性，on property可以用于判断属性，这里的属性在整个Android系统运行中都是一致的。init脚本的关键字可以参考init进程的system/core/init/keyword.h文件。init.rc的使用方法，可以参考说明文件system/core/init/readme.txt。如果想要修改启动过程只需要修改init.c（system/core/init）或者init.rc里的内容即可.
参考原文：[http://hi.baidu.com/donghaozheng/blog/item/e2f5045577cb73c8b645aec0.html](http://hi.baidu.com/donghaozheng/blog/item/e2f5045577cb73c8b645aec0.html)
 =========================================================================================================================
        一般启动模式（normal mode）的功能是正常启动手机，方法为关机状态下按电源键启动。 
　　安全模式（safe mode）。此模式和正常启动一样，但没有登记Google，所以不能访问Market或使用你的Google账号。操作方法为按住“menu”键，按电源键启动手机，直至手机启动完成松开“menu”键。
　　恢复模式（recovery mode）可打开命令解释程序（shell），刷新映像文件（flash image），执行备份等。当然这一切取决于你手机上的recovery image版本。使用方法很简单，就是home键和电源键一起按。
　　引导模式（bootloader mode）的功能是从SD卡上安装新的系统映像（DREAIMG.NBH），只需再按一次电源键。值得一提的事为Android手机获取root权限，就是使用这个模式。操作方法为照相键和电源键一起按。
　　fastboot模式主要是在电脑上使用fastboot命令来刷新映像文件。使用方法为按住返回键，按电源键启动手机，直至屏幕出现FASTBOOT字样后松开返回键。
　　诊断模式（diagnostic mode）是为了测试手机各项功能的模式，刻意通过轨迹球中心键和电源键打开。
