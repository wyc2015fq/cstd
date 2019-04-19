# Android系统服务启动分析-binder - xqhrs232的专栏 - CSDN博客
2012年05月22日 15:25:26[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：783
原文地址::[http://www.linuxidc.com/Linux/2011-05/36491.htm](http://www.linuxidc.com/Linux/2011-05/36491.htm)
### 1.System Services
首先我要声明一下，我讲的System Services并非[Android](http://www.linuxidc.com/topicnews.aspx?tid=11) 开发应用程序时，所涉及的Service（后台应用服务程序）的概念。
我要讲的System Services是[Android](http://www.linuxidc.com/topicnews.aspx?tid=11)操作系统Java应用程序下层的，伴随操作系统启动而运行的系统后台服务程序。
它是[Android](http://www.linuxidc.com/topicnews.aspx?tid=11)系统运行的基石，它配合binder（Android多进程通讯方法）、dalvik虚拟机和Android应用程序构成了一个多进程
交互通讯，交互服务的[Android](http://www.linuxidc.com/topicnews.aspx?tid=11)系统。
### 2.浏览一下[Android](http://www.linuxidc.com/topicnews.aspx?tid=11)系统的service
启动shell
**adb shell**
执行下面指令
**#service list**
Found 47 services:
0    phone: [com.[Android](http://www.linuxidc.com/topicnews.aspx?tid=11).internal.telephony.ITelephony]
1    iphonesubinfo: [com.[Android](http://www.linuxidc.com/topicnews.aspx?tid=11).internal.telephony.IPhoneSubInfo]
2    simphonebook: [com.[Android](http://www.linuxidc.com/topicnews.aspx?tid=11).internal.telephony.IIccPhoneBook]
3    isms: [com.[Android](http://www.linuxidc.com/topicnews.aspx?tid=11).internal.telephony.ISms]
4    appwidget: [com.[Android](http://www.linuxidc.com/topicnews.aspx?tid=11).internal.appwidget.IAppWidgetService]
42    SurfaceFlinger: [[Android](http://www.linuxidc.com/topicnews.aspx?tid=11).ui.ISurfaceComposer]
43    media.audio_policy: [[Android](http://www.linuxidc.com/topicnews.aspx?tid=11).media.IAudioPolicyService]
46    media.audio_flinger: [[Android](http://www.linuxidc.com/topicnews.aspx?tid=11).media.IAudioFlinger]
#
从结果看来[Android](http://www.linuxidc.com/topicnews.aspx?tid=11)后台有很多的service，他们是分散在不同进程中的线程实体（有点绕嘴，但是我认为这样说比较确切）。
### 3.什么是Service Manager
里面理解一下就可以了，Service manager是管理以上services的一个进程，他是实际存在的。
您可以在adb shell中运行ps看看进程列表就知道了。
源代码位于：
frameworks/base/cmds/servicemanager
执行方式：
他是用c和c++语言编写的natvie可以执行文件。在[Android](http://www.linuxidc.com/topicnews.aspx?tid=11)中称之为EXECUTABLE，这个名称很重要因为Android.mk文件中
用这个名字来确定他是可以执行的二进制文件。
### 4.探究一下Service Manager的启动过程和方法
开始有点复杂了，也该开始进入真正的Linux阶段了。
众所周知Linux的启动和文件系统的加载需要一个ramdisk，ramdisk负责让Linux kernel加载第一个进程**init进程**
在[Android](http://www.linuxidc.com/topicnews.aspx?tid=11)的ramdisk中就有这样一个可执行文件init，
在深入一下，我们可以去看一下
system/core/init/init.c
int main(int argc, char **argv)
{
    。。。。。。
    parse_config_file("/init.rc");
    。。。。。。
对的，没看错。这个文件会编译出一个init的二进制可执行文件，并且去读init.rc文件。
至此，我们称init.rc文件为[Android](http://www.linuxidc.com/topicnews.aspx?tid=11)启动配置脚本。
现在我们打开init.rc文件，（如果您不知道init.rc，请参考google吧）
## Daemon processes to be run by init.
##
service servicemanager /system/bin/servicemanager
    user system
    critical
onrestart restart zygote
onrestart restart media
看到吗，servicemanager 是init通过init.rc加载的第一个进程
接下来启动了zygote和media
### 4.system server进程
继续阅读init.rc
servicemanager进程运行起来以后，我们就可以应用binder来应用servicemanager提供的服务函数去创建
system-server和mediaserver了，下面是init.rc中的代码
service zygote /system/bin/app_process -Xzygote /system/bin --zygote --start-system-server
#system-server的创建是通过app_process这个二进制程序去加载的
    socket zygote stream 666
    onrestart write /sys/[Android](http://www.linuxidc.com/topicnews.aspx?tid=11)_power/request_state wake
    onrestart write /sys/power/state on
    onrestart restart media
service media /system/bin/mediaserver   #mediaserver的启动代码比较简单，看看就知道了不用参数就创建了
    user media
    group system audio camera graphics inet net_bt net_bt_admin
### 5.回过头再看系统的进程列表
# ps
USER     PID   PPID  VSIZE  RSS     WCHAN    PC         NAME
root      1     0     296    204   c009a694 0000c93c S /init
root      2     0     0      0     c004dea0 00000000 S kthreadd
root      25    1     728    316   c003d444 afe0d6ac S /system/bin/sh
system    26    1     796    256   c019a810 afe0ca7c S /system/bin/servicemanager
**root      30    1     82860  26580 c009a694 afe0cba4 S zygote**
media     31    1     20944  3184  ffffffff afe0ca7c S /system/bin/mediaserver
root      32    1     784    280   c0209468 afe0c7dc S /system/bin/installd
keystore  33    1     1616   396   c01a65a4 afe0d40c S /system/bin/keystore
root      34    1     728    272   c003d444 afe0d6ac S /system/bin/sh
root      35    1     824    332   c00b7dd0 afe0d7fc S /system/bin/qemud
root      37    1     1308   152   ffffffff 0000eca4 S /sbin/adbd
root      44    34    780    304   c0209468 afe0c7dc S /system/bin/qemu-props
**system    52    30    158356 37804 ffffffff afe0ca7c S system_server**
app_1     92    30    108640 20580 ffffffff afe0da04 S com.[Android](http://www.linuxidc.com/topicnews.aspx?tid=11).inputmethod.pinyin
radio     93    30    122852 23340 ffffffff afe0da04 S com.[Android](http://www.linuxidc.com/topicnews.aspx?tid=11).phone
app_1     98    30    143244 34888 ffffffff afe0da04 S [Android](http://www.linuxidc.com/topicnews.aspx?tid=11).process.acore
有点复杂了，请大家跟上思路。我们注意观察进程列表的PID和PPID，我们要通过实际的列表去理清他们的亲缘关系。
servicemanager是init的子进程
mediaserver是init的子进程
zygote是init的子进程
system_server和所有的java应用程序是zygote的子进程
休息一下我们看看他们的应用程序代码方式
Java script caller (executable)
frameworks/base/cmds/app_process/
app_main.cpp
app_process是[Android](http://www.linuxidc.com/topicnews.aspx?tid=11)系统下面基于命令行的java的应用程序的调用工具
**system_server executable（c/c++写的程序）**
frameworks/base/cmds/system_server/
system_main.cpp
library/system_init.cpp
**SystemServer （java程序）**
frameworks/base/services/java/com/[Android](http://www.linuxidc.com/topicnews.aspx?tid=11)/server/
SystemServer.java
**Zygote （java程序）**
frameworks/base/core/java/com/[Android](http://www.linuxidc.com/topicnews.aspx?tid=11)/internal/os/
ZygoteInit.java
### 6.分析具体的调用过程（很痛苦）
**app_main 调用 zygoteInit**
p { margin-bottom: 0.21cm; }
/ Next arg is startup classname or "--zygote"
if ( i < argc ) {
arg = argv [ i ++];
if ( 0 == strcmp ( "--zygote" , arg )) {
bool startSystemServer = ( i < argc ) ?
strcmp ( argv [ i ], "--start-system-server" ) == 0 : **false **;
setArgv0 ( argv0 , "zygote" );
set_process_name ( "zygote" );
**runtime.****start****(****"com.[Android](http://www.linuxidc.com/topicnews.aspx?tid=11).internal.os.ZygoteInit"****,**
**startSystemServer****);**
} else {
set_process_name ( argv0 );
runtime. mClassName = arg ;
// Remainder of args get passed to startup class main ()
runtime. mArgC = argc - i ;
runtime. mArgV = argv + i ;
LOGV ( "App process is starting with pid=%d, class=%s. **\n **" ,
getpid (), runtime. getClassName ());
runtime. start ();
}
}
**Zygote 分裂出 system_server**
p { margin-bottom: 0.21cm; }
**public ****static ****void **main ( **String **argv []) {
**try **{
*// Start profiling the zygote initialization.*
**if **( argv [ 1 ] . equals ( "true" )) {
**startSystemServer****();**
}
Log. i ( TAG, "Accepting command socket connections" );
**if **( ZYGOTE_FORK_MODE ) {
runForkMode ();
} **else **{
runSelectLoopMode ();
}
}
frameworks/base/core/java/com/[Android](http://www.linuxidc.com/topicnews.aspx?tid=11)/internal/os/
ZygoteInit.java
p { margin-bottom: 0.21cm; }
**private ****static ****boolean **startSystemServer()
**throws **MethodAndArgsCaller, **RuntimeException **{
*/* Hardcoded command line to start the system server */*
**try **{
*/* Request to fork the system server process */*
**pid = Zygote.****forkSystemServer****(**
**parsedArgs.****uid****, parsedArgs.****gid****,**
**parsedArgs.****gids****, debugFlags, ****null ****);**
} **catch **( **IllegalArgumentException **ex)
 {
**throw ****new ****RuntimeException **(ex);
}
*/* For child process */*
**if **(pid == 0 )
 {
handleSystemServerProcess(parsedArgs);
}
**return ****true **;
}
**1.如果大家还能看到这里请复习一下 Linux的两个系统调用 fork和exec**
frameworks/base/services/java/com/[Android](http://www.linuxidc.com/topicnews.aspx?tid=11)/server/SystemService.java
p { margin-bottom: 0.21cm; }
**public ****static ****void **main ( **String **[] args ) {
**if **( SamplingProfilerIntegration. isEnabled ()) {
SamplingProfilerIntegration. start ();
**timer **= **new ****Timer **();
**timer **. schedule ( **new ****TimerTask **() {
@ **Override**
**public ****void **run () {
SamplingProfilerIntegration. writeSnapshot ( "system_server" );
}
} , SNAPSHOT_INTERVAL, SNAPSHOT_INTERVAL );
}
VMRuntime. getRuntime () . setTargetHeapUtilization ( 0 .8f );
**System ****.****loadLibrary****(****"[Android](http://www.linuxidc.com/topicnews.aspx?tid=11)_servers"****);**
**init1****(****args****);------------------>**
}
**public ****static ****final ****void **init2 () {
Log. i ( TAG, "Entered
 the [Android](http://www.linuxidc.com/topicnews.aspx?tid=11) system server!" );
**Thread **thr = **new **ServerThread ();
**thr. setName ( "[Android](http://www.linuxidc.com/topicnews.aspx?tid=11).server.ServerThread" );-------------->**
thr.start();
}
**红色的部分最后会执行我们上面列出的jni代码**
**frameworks/base/cmds/system_server/library/system_init.cpp**
Init2 这里启动了java的system service
p { margin-bottom: 0.21cm; }
**class**ServerThread **extends****Thread**{
@**Override**
**public****void**run(){
Log.i(TAG, "System
 Content Providers");
ActivityManagerService.installSystemProviders();
Log.i(TAG, "Battery
 Service");
battery = **new**BatteryService(**context**);
ServiceManager.addService("battery",
 battery);
Log.i(TAG, "Hardware
 Service");
hardware = **new**HardwareService(**context**);
ServiceManager.addService("hardware",
 hardware);
Log.i(TAG, "Alarm
 Manager");
AlarmManagerService alarm = **new**AlarmManagerService(**context**);
ServiceManager.addService(**Context**.ALARM_SERVICE,
 alarm);
}
}
原创文章欢迎转载，转载请注明住处rickleaf
### **7. 整理[Android](http://www.linuxidc.com/topicnews.aspx?tid=11)系统启动流程**
至此System Service的服务环境启动起来了
![](http://www.linuxidc.com/upload/2011_05/110526061669911.gif)
**8. 最后引用霍金的一句话：“懂与不懂都是收获”**
