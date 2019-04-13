
# Android ANR 实例分析 - 嵌入式Linux - CSDN博客

2018年12月30日 16:44:55[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：162



## 什么是ANR？
以下四个条件都可以造成ANR发生：
InputDispatching Timeout：5秒内无法响应屏幕触摸事件或键盘输入事件
BroadcastQueue Timeout ：在执行前台广播（BroadcastReceiver）的onReceive()函数时10秒没有处理完成，后台为60秒。
Service Timeout ：前台服务20秒内，后台服务在200秒内没有执行完毕。
ContentProvider Timeout ：ContentProvider的publish在10s内没进行完。
[https://www.jianshu.com/p/388166988cef](https://www.jianshu.com/p/388166988cef)
实例分析
我们测试过程中跑monkey测试的时候，发生了ANR，LOG非常多。但是分析原因的时候还是非常考验思路。
需要三个文件
traces.txt traces状态
threath.txt 这个是最后的进程状态
logcat日志
日志链接，想分析的自行下载
链接：[https://pan.baidu.com/s/1i0Lf2luKbbEwXMATKErI8w](https://pan.baidu.com/s/1i0Lf2luKbbEwXMATKErI8w)
提取码：wutm
1.搜索ANR in这个关键字
12-27 01:18:59.865   562   591 E ActivityManager: ANR in com.android.music
12-27 01:18:59.865   562   591 E ActivityManager: PID: 5563
12-27 01:18:59.865   562   591 E ActivityManager: Reason: Broadcast of Intent { act=android.intent.action.MEDIA_BUTTON flg=0x10 cmp=com.android.music/.MediaButtonIntentReceiver (has extras) }
12-27 01:18:59.865   562   591 E ActivityManager: Load: 0.0 / 0.0 / 0.0
12-27 01:18:59.865   562   591 E ActivityManager: CPU usage from 5870ms to 375ms ago (2018-12-27 01:18:53.949 to 2018-12-27 01:18:59.444):
12-27 01:18:59.865   562   591 E ActivityManager:   105% 562/system_server: 103% user + 2.1% kernel / faults: 435 minor
12-27 01:18:59.865   562   591 E ActivityManager:   100% 30/kconsole: 0% user + 100% kernel
12-27 01:18:59.865   562   591 E ActivityManager:   3.6% 204/healthd: 0.5% user + 3% kernel
12-27 01:18:59.865   562   591 E ActivityManager:   2.3% 164/ueventd: 1.2% user + 1% kernel / faults: 1 minor
12-27 01:18:59.865   562   591 E ActivityManager:   2.3% 186/logd: 1% user + 1.2% kernel
12-27 01:18:59.865   562   591 E ActivityManager:   2% 235/log-watch: 1% user + 0.9% kernel
12-27 01:18:59.865   562   591 E ActivityManager:   0.9% 132/irq/78-0-006a: 0% user + 0.9% kernel
12-27 01:18:59.865   562   591 E ActivityManager:   0.9% 5579/kworker/1:1: 0% user + 0.9% kernel
12-27 01:18:59.865   562   591 E ActivityManager:   0.7% 230/netd: 0.3% user + 0.3% kernel
12-27 01:18:59.865   562   591 E ActivityManager:   0.5% 1125/com.android.phone: 0.3% user + 0.1% kernel / faults: 8 minor
12-27 01:18:59.865   562   591 E ActivityManager:   0.1% 287/logcatext: 0% user + 0% kernel
12-27 01:18:59.865   562   591 E ActivityManager:   0.1% 5112/com.android.commands.monkey: 0.1% user + 0% kernel / faults: 5 minor
12-27 01:18:59.865   562   591 E ActivityManager:   0.1% 7/rcu_preempt: 0% user + 0.1% kernel
12-27 01:18:59.865   562   591 E ActivityManager:   0% 24/ksoftirqd/3: 0% user + 0% kernel
12-27 01:18:59.865   562   591 E ActivityManager:   0.1% 194/vold: 0% user + 0.1% kernel
12-27 01:18:59.865   562   591 E ActivityManager:   0.1% 205/displayd: 0.1% user + 0% kernel
12-27 01:18:59.865   562   591 E ActivityManager:   0% 209/servicemanager: 0% user + 0% kernel
12-27 01:18:59.865   562   591 E ActivityManager:   0.1% 210/surfaceflinger: 0% user + 0.1% kernel
12-27 01:18:59.865   562   591 E ActivityManager: 54% TOTAL: 27% user + 27% kernel
12-27 01:18:59.865   562   591 E ActivityManager: CPU usage from 50244340ms to 50244340ms ago (1970-01-01 02:00:00.000 to 1970-01-01 02:00:00.000) with 0% awake:
12-27 01:18:59.865   562   591 E ActivityManager: 0% TOTAL: 0% user + 0% kernel
12-27 01:18:59.865     0     0 W KERNEL  : [50232.754404] healthd: battery l=66 v=4082 t=30.9 h=2 st=2 chg=
12-27 01:18:59.990   562   594 W BatteryStatsService: Timeout reading modem stats
12-27 01:18:59.998  1125  1125 D RILJ    : [9781]> RIL_REQUEST_GET_ACTIVITY_INFO [SUB0]
12-27 01:18:59.999  1125  1125 D RilRequest: [9781]< RIL_REQUEST_GET_ACTIVITY_INFO error: com.android.internal.telephony.CommandException: RADIO_NOT_AVAILABLE ret=
12-27 01:18:59.999  1125  1125 E PhoneInterfaceManager: [PhoneIntfMgr] queryModemActivityInfo: Empty response
12-27 01:18:59.999  1125  1522 E JavaBinder: !!! FAILED BINDER TRANSACTION !!!  (parcel size = 276)
12-27 01:18:59.999   562   594 E BatteryStatsService: no controller energy info supplied
12-27 01:18:59.999   562   594 E BatteryStatsService: no controller energy info supplied
12-27 01:19:00.000     0     0 W KERNEL  : [50232.884468] healthd: battery l=66 v=4082 t=30.9 h=2 st=2 chg=a
12-27 01:19:00.059   562  8694 ?然后这里可以看到ANR 的PID是5563
回到traces.txt找一下这个com.android.music
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018122917444815.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXFpZmEw,size_16,color_FFFFFF,t_70)
干他大爷的，发现没有找到~~~~~~
不着急，
我们刚才看那个日志的时候比较早，traces.txt可能会后面的日志冲掉了原因是
Wrote stack traces to '/data/anr/traces.txt'这个是每次发生anr的时候，想traces文件写入数据的日志
我们找最后一次写入这个文件的日志位置
12-27 03:02:54.175     0     0 W KERNEL  : [56327.543907]   .cpu_load[0]                   : 84
12-27 03:02:54.178   562  1364 I Process : Sending signal. PID: 562 SIG: 3
12-27 03:02:54.179   562   567 I art     : Thread[2,tid=567,WaitingInMainSignalCatcherLoop,Thread*=0xa6d0c000,peer=0x12c010d0,"Signal Catcher"]: reacting to signal 3
12-27 03:02:54.179   562   567 I art     : 
12-27 03:02:54.180     0     0 W KERNEL  : [56327.543921]   .cpu_load[1]                   : 54
12-27 03:02:54.641   562   567 I art     : Wrote stack traces to '/data/anr/traces.txt'
12-27 03:02:54.641   562  1364 I Process : Sending signal. PID: 1125 SIG: 3
12-27 03:02:54.641  1125  1128 I art     : Thread[3,tid=1128,WaitingInMainSignalCatcherLoop,Thread*=0xa433fe00,peer=0x12c00670,"Signal Catcher"]: reacting to signal 3
12-27 03:02:54.641     0     0 W KERNEL  : [56327.543932]   .cpu_load[2]                   : 30
12-27 03:02:54.641  1125  1128 I art     : 
12-27 03:02:54.653     0     0 W KERNEL  : [56327.543945]   .cpu_load[3]                   : 16
12-27 03:02:54.693  1125  1128 I art     : Wrote stack traces to '/data/anr/traces.txt'
12-27 03:02:54.698     0     0 W KERNEL  : [56327.543958]   .cpu_load[4]                   : 9
12-27 03:02:54.698   193   193 W         : debuggerd: handling request: pid=210 uid=1000 gid=1003 tid=210
12-27 03:02:54.698     0     0 W KERNEL  : [56327.543972]   .yld_count                     : 570694
12-27 03:02:54.791   193   193 W         : debuggerd: resuming target 210
12-27 03:02:54.791     0     0 W KERNEL  : [56327.543986]   .sched_count                   : 127372328
12-27 03:02:54.792   193   193 W         : debuggerd: handling request: pid=221 uid=1041 gid=1005 tid=221
12-27 03:02:54.792     0     0 W KERNEL  : [56327.544001]   .sched_goidle                  : 25588776
12-27 03:02:54.870   193   193 W         : debuggerd: resuming target 221
12-27 03:02:54.871     0     0 W KERNEL  : [56327.544014]   .avg_idle                      : 1000000
12-27 03:02:54.871   193   193 W         : debuggerd: handling request: pid=222 uid=1047 gid=1005 tid=222
12-27 03:02:54.871     0     0 W KERNEL  : [56327.544022]   .max_idle_balance_cost         : 500000
12-27 03:02:54.904   193   193 W         : debuggerd: resuming target 222
12-27 03:02:54.905     0     0 W KERNEL  : [56327.544030]   .ttwu_count                    : 68703282
12-27 03:02:54.905   193   193 W         : debuggerd: handling request: pid=223 uid=1019 gid=1019 tid=223
12-27 03:02:54.905     0     0 W KERNEL  : [56327.544037]   .ttwu_local                    : 31888109
12-27 03:02:54.917   193   193 W         : debuggerd: resuming target 223
12-27 03:02:54.917     0     0 W KERNEL  : [56327.544045] \x0acfs_rq[1]:/bg_non_interactive
12-27 03:02:54.917   193   193 W         : debuggerd: handling request: pid=226 uid=1046 gid=1006 tid=226
12-27 03:02:54.917     0     0 W KERNEL  : [56327.544057]   .exec_clock                    : 1419463.649513
12-27 03:02:54.968   193   193 W         : debuggerd: resuming target 226
12-27 03:02:54.968     0     0 W KERNEL  : [56327.544065]   .MIN_vruntime                  : 0.000001
12-27 03:02:54.968   193   193 W         : debuggerd: handling request: pid=227 uid=1013 gid=1031 tid=227
12-27 03:02:54.968     0     0 W KERNEL  : [56327.544072]   .min_vruntime                  : 10007865.678185
12-27 03:02:54.982   193   193 W         : debuggerd: resuming target 227Process : Sending signal. PID: 1125 SIG: 3
PID 1125  被发送了一个SIG3 然后我们查一下这个SIG 3
说明1125 被杀死了，信号值对应的数值如下
信号 值 处理动作 发出信号的原因 
---------------------------------------------------------------------- 
SIGHUP 1 A 终端挂起或者控制进程终止 
SIGINT 2 A 键盘中断（如break键被按下） 
SIGQUIT 3 C 键盘的退出键被按下 
SIGILL 4 C 非法指令 
SIGABRT 6 C 由abort(3)发出的退出指令 
SIGFPE 8 C 浮点异常 
SIGKILL 9 AEF Kill信号 
SIGSEGV 11 C 无效的内存引用 
SIGPIPE 13 A 管道破裂: 写一个没有读端口的管道 
SIGALRM 14 A 由alarm(2)发出的信号 
SIGTERM 15 A 终止信号 
SIGUSR1 30,10,16 A 用户自定义信号1 
SIGUSR2 31,12,17 A 用户自定义信号2 
SIGCHLD 20,17,18 B 子进程结束信号 
SIGCONT 19,18,25 进程继续（曾被停止的进程） 
SIGSTOP 17,19,23 DEF 终止进程 
SIGTSTP 18,20,24 D 控制终端（tty）上按下停止键 
SIGTTIN 21,21,26 D 后台进程企图从控制终端读 
SIGTTOU 22,22,27 D 后台进程企图从控制终端写 
下面的信号没在POSIX.1中列出，而在SUSv2列出 
信号 值 处理动作 发出信号的原因链接：[www.cnblogs.com/taobataoma/archive/2007/08/30/875743.html](https://www.cnblogs.com/taobataoma/archive/2007/08/30/875743.html)
然后我们继续搜下
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181229180211343.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXFpZmEw,size_16,color_FFFFFF,t_70)
从这个图片看到了，这个1125不断的被杀死，因为是系统进程，自己又无耻的启动，反复如此，就出现了上面的日志。
然后继续回到traces.txt里面分析
看thread.txt里面查看1125这个进程的名字是android.phone.
1125  1125 radio    20   0   0% S 1024340K  87388K  fg m.android.phone com.android.phone
 1125  1127 radio    29   9   0% S 1024340K  87388K  fg Jit thread pool com.android.phone
 1125  1128 radio    20   0   0% S 1024340K  87388K  fg Signal Catcher  com.android.phone
 1125  1129 radio    20   0   0% S 1024340K  87388K  fg JDWP            com.android.phone然后在看thread.txt里面查看562这个进程的名字是system_server，如下图
562   562 system   18  -2   0% S 1851968K 408908K  fg system_server   system_server
  562   567 system   20   0   0% S 1851968K 408908K  fg Signal Catcher  system_server
  562   569 system   20   0   0% S 1851968K 408908K  fg ReferenceQueueD system_server
  562   570 system   20   0   0% S 1851968K 408908K  fg FinalizerDaemon system_server
  562   571 system   20   0   0% S 1851968K 408908K  fg FinalizerWatchd system_server
  562   572 system   20   0   0% S 1851968K 408908K  fg HeapTaskDaemon  system_server
  562   588 system   20   0   0% S 1851968K 408908K  fg Binder:562_1    system_server然后再在traces.txt里面看
----- pid 562 at 2018-12-27 03:03:32 -----
Cmd line: system_server
Build fingerprint: 'Android/rk3288_pad/rk3288_pad:7.1.2/NHG47K/52:userdebug/test-keys'
ABI: 'arm'这是system_server出现问题的时候记录下来的日志，然后继续往下看
"main" prio=5 tid=1 Native
  | group="main" sCount=1 dsCount=0 obj=0x73d514a0 self=0xae105400
  | sysTid=562 nice=-2 cgrp=default sched=0/0 handle=0xb0eb2534
  | state=S schedstat=( 265574536000 322529001630 703903 ) utm=21655 stm=4901 core=1 HZ=100
  | stack=0xbe0b4000-0xbe0b6000 stackSize=8MB
  | held mutexes=
  native: \#00 pc 00017418  /system/lib/libc.so (syscall+28)
  native: \#01 pc 00046eab  /system/lib/libc.so (_ZL24__pthread_cond_timedwaitP23pthread_cond_internal_tP15pthread_mutex_tbPK8timespec+102)
  native: \#02 pc 000846d7  /system/lib/libmedia.so (_ZN7android11MediaPlayer7prepareEv+82)
  native: \#03 pc 0002b683  /system/lib/libmedia_jni.so (???)
  native: \#04 pc 00a5780f  /data/dalvik-cache/arm/system@framework@boot.oat (Java_android_media_MediaPlayer__1prepare__+74)
  at android.media.MediaPlayer._prepare(Native method)
  at android.media.MediaPlayer.prepare(MediaPlayer.java:1365)
  at android.media.Ringtone.setUri(Ringtone.java:298)
  at android.media.Ringtone.setAudioAttributes(Ringtone.java:134)
  at android.media.Ringtone.setStreamType(Ringtone.java:105)
  at com.android.server.power.Notifier.playWirelessChargingStartedSound(Notifier.java:702)
  at com.android.server.power.Notifier.-wrap0(Notifier.java:-1)
  at com.android.server.power.Notifier$NotifierHandler.handleMessage(Notifier.java:728)
  at android.os.Handler.dispatchMessage(Handler.java:102)
  at android.os.Looper.loop(Looper.java:154)
  at com.android.server.SystemServer.run(SystemServer.java:358)
  at com.android.server.SystemServer.main(SystemServer.java:225)
  at java.lang.reflect.Method.invoke!(Native method)
  at com.android.internal.os.ZygoteInit$MethodAndArgsCaller.run(ZygoteInit.java:912)
  at com.android.internal.os.ZygoteInit.main(ZygoteInit.java:802)这是出现问题的栈指针和函数调用的情况，可以看到是
at android.media.MediaPlayer._prepare(Native method)
at android.media.MediaPlayer.prepare(MediaPlayer.java:1365)
这里出现了问题，具体原因需要根据日志去分析了
总结梳理一下流程
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181230164050555.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXFpZmEw,size_16,color_FFFFFF,t_70)
1125 这个是com.android.phone 是android 拨打电话的进程
562 这个是android_server 系统进程
这两个在日志里面不断的相爱相杀了很多次，从traces.txt的原因上来看是MediaPlayer.prepare阻塞的原因，但是android_server正常没人惹他，他是不会发生这么多次崩溃的，所以最终导致的原因应该是com.android.phone调用了system_server的东西。
然后为什么com.android.phone会有问题呢？
继续摸索了一下日志
12-27 02:39:44.726  1125  1125 D RILJ    : [8577]> RIL_REQUEST_GET_ACTIVITY_INFO [SUB0]
12-27 02:39:44.727  1125  1125 D RilRequest: [8577]< RIL_REQUEST_GET_ACTIVITY_INFO error: com.android.internal.telephony.CommandException: RADIO_NOT_AVAILABLE ret=
12-27 02:39:44.727  1125  1125 E PhoneInterfaceManager: [PhoneIntfMgr] queryModemActivityInfo: Empty response
12-27 02:39:44.732   562   594 E BatteryStatsService: modem info is invalid: ModemActivityInfo{ mTimestamp=0 mSleepTimeMs=0 mIdleTimeMs=0 mTxTimeMs[]=[0, 0, 0, 0, 0] mRxTimeMs=0 mEnergyUsed=0}
12-27 02:39:44.735   562   594 E BatteryStatsService: no controller energy info supplied
12-27 02:39:44.735   562   594 E BatteryStatsService: no controller energy info supplied
12-27 02:39:44.736  1125  1125 D RILJ    : [8578]> RIL_REQUEST_GET_ACTIVITY_INFO [SUB0]
12-27 02:39:44.736  1125  1125 D RilRequest: [8578]< RIL_REQUEST_GET_ACTIVITY_INFO error: com.android.internal.telephony.CommandException: RADIO_NOT_AVAILABLE ret=
12-27 02:39:44.736  1125  1125 E PhoneInterfaceManager: [PhoneIntfMgr] queryModemActivityInfo: Empty response
12-27 02:39:44.741   562   594 E BatteryStatsService: modem info is invalid: ModemActivityInfo{ mTimestamp=0 mSleepTimeMs=0 mIdleTimeMs=0 mTxTimeMs[]=[0, 0, 0, 0, 0] mRxTimeMs=0 mEnergyUsed=0}
12-27 02:39:44.745   562   594 E BatteryStatsService: no controller energy info supplied
12-27 02:39:44.745   562   594 E BatteryStatsService: no controller energy info supplied1125 调用了modem相关的东西，也就是在monkey测试过程中拨打了电话和，但是因为我们是平板项目，硬件上根本是没有modem的，原声的系统是也是去掉了这个apk的，我们是为了做暗码组合键功能才调出来的。
实际分析如上，如果有不正确的，请留言指正，谢谢
欢迎关注微信公众号-嵌入式Linux
觉得不错，请帮忙转发，点赞，您的每一次支持，我都将铭记于心

[
  ](https://img-blog.csdnimg.cn/20181230164050555.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXFpZmEw,size_16,color_FFFFFF,t_70)