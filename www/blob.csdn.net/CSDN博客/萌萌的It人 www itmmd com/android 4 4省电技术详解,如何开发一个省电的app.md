
# android 4.4省电技术详解,如何开发一个省电的app - 萌萌的It人 www.itmmd.com - CSDN博客


2015年02月02日 13:12:06[Jlins](https://me.csdn.net/dyllove98)阅读数：6431


安卓4.4的推出，省电技术方面谷歌又表示进行了改良!让我们一起了解安卓4.4省电技术到底做了什么改变!
测试结论：
1)灭屏待机最省电：
a)任何App包括后台Service应该尽可能减少唤醒CPU的次数，比如IM类业务的长连接心跳、QQ提醒待机闹钟类业务的alarm硬时钟唤醒要严格控制;
b)每次唤醒CPU执行的代码应该尽可能少，从而让CPU迅速恢复休眠，比如申请wake lock的数量和持有时间要好好斟酌;
2)Wi-Fi比蜂窝数据，包括2G(GPRS)、3G更省电：
a)尽量在Wi-Fi下传输数据，当然这是废话，不过可以考虑在有Wi-Fi的时候做预加载，比如应用中心的zip包、手Q web类应用的离线资源等;
b)非Wi-Fi下，尽量减少网络访问，每一次后台交互都要考虑是否必须。虽然WiFi接入方式已经占到移动互联网用户的50%，但是是有些手机设置为待机关闭WiFi连接，即便有Wi-Fi信号也只能切换到蜂窝数据;
测试分析：
1)灭屏的情况：
a)灭屏待机，CPU处于休眠状态，最省电(7mA);
b)灭屏传输，CPU被激活，耗电显著增加，即便是处理1K的心跳包，电量消耗也会是待机的6倍左右(45mA);
c)灭屏传输，高负载download的时候WiFi最省电(70mA)，3G(270mA)和2G(280mA)相当，是WiFi的4倍左右;
![Android手机耗电深度解析！](http://i1.android.265g.com/allimg/131116/10393T1L-0.png)
2)亮屏的情况：
a)亮屏待机，CPU处于激活状态，加上屏幕耗电，整机电量消耗不小(140mA);
b)亮屏传输，如果只是处理1K的心跳包，耗电增加不多(150mA)，即便是很大的心跳包(64K)，消耗增加也不明显(160mA);
c)亮屏传输，高负载download的时候WiFi最省电(280mA)，3G(360mA)和2G(370mA)相当，是WiFi的1.3倍左右;
![Android手机耗电深度解析！](http://i1.android.265g.com/allimg/131116/10393U637-1.png)
3)Alarm唤醒频繁会导致待机耗电增加：
手机灭屏后会进入待机状态，这时CPU会进入休眠状态。Android的休眠机制介绍的文章很多，这里引用一段网络文章：
Early suspend是android引进的一种机制,这种机制在上游备受争议,这里 不做评论。这个机制作用在关闭显示的时候,在这个时候,一些和显示有关的 设备,比如LCD背光,比如重力感应器,触摸屏,这些设备都会关掉,但是系统可能还是在运行状态(这时候还有wake lock)进行任务的处理,例如在扫描SD卡上的文件等.在嵌入式设备中,背光是一个很大的电源消耗,所以android会加入这样一种机制.
Late Resume是和suspend配套的一种机制,是在内核唤醒完毕开始执行的.主要就是唤醒在Early Suspend的时候休眠的设备.
Wake Lock在Android的电源管理系统中扮演一个核心的角色. Wake Lock是一种锁的机制,只要有人拿着这个锁,系统就无法进入休眠,可以被用户态程序和内核获得.这个锁可以是有超时的或者是没有超时的,超时的锁会在时 间过去以后自动解锁.如果没有锁了或者超时了,内核就会启动休眠的那套机制来进入休眠.
当用户写入mem或者standby到/sys/power/state中的时候, state_store()会被调用,然后Android会在这里调用request_suspend_state()而标准的Linux会在这里进入 enter_state()这个函数.如果请求的是休眠,那么early_suspend这个workqueue就会被调用,并且进入 early_suspend
简单的说，当用户按power键，使得手机进入灭屏休眠状态，Android系统其实是做了前面说的一些工作：关闭屏幕、触摸屏、传感器、dump当前用 户态和内核态程序运行上下文到内存或者硬盘、关闭CPU供电，当然为了支持语音通讯，modern等蜂窝信令还是工作的。
这种情况下，应用要唤醒CPU，只有两种可能：
a)通过服务器主动PUSH数据，通过网络设备激活CPU;
b)设置alarm硬件闹钟唤醒CPU;
这里我们重点分析第二种情况。首先来看看什么是alarm硬件闹钟。Google官方提供的解释是：Android提供的alarm services可以帮助应用开发者能够在将来某一指定的时刻去执行任务。当时间到达的时候，Android系统会通过一个Intent广播通知应用去完 成这一指定任务。即便CPU休眠，也不影响alarm services的服务，这种情况下可以选择唤醒CPU。
显然唤醒CPU是有电量消耗的，CPU被唤醒的次数越多，耗电量会越大。现在很多应用为了维持心跳、拉取数据、主动PUSH会不同程度地注册alarm服 务，导致Android系统被频繁唤醒。这就是为什么雷军说Android手机在安装了TOP100的应用后，待机时间会大大缩短的重要原因。
比较简单评测CPU唤醒次数的方法是看dumpsys alarm，这里会详细记录从开机到当前的各个进程和服务唤醒CPU的次数和时间。通过对比唤醒次数和唤醒时间可以帮助我们分析后台进程和服务的耗电情况。Dumpsys alarm的输出看起来像这样：
![Android手机耗电深度解析！](http://i1.android.265g.com/allimg/131116/10393TA7-2.jpg)
其中544代表唤醒次数，38684ms代表唤醒时间。
4)Wake locks持有时间过长会导致耗电增加：
Wake locks是一种锁机制，有些文献翻译成唤醒锁。简单说，前面讲的灭屏CPU休眠还需要做一个判断，就是看是否还有任何应用持有wake locks。如果有，CPU将不会休眠。有些应用不合理地申请wake locks，或者申请了忘记释放，都会导致手机无法休眠，耗电增加。
测试方法：硬件设备提供稳压电源替代手机电池供电，在不同场景下记录手机平均电流。
测试设备：Monsoon公司的Power Monitor TRMT000141
测试机型：Nexus One
![Android手机耗电深度解析！](http://i1.android.265g.com/allimg/131116/10393SZ5-3.jpg)
灭屏benchmark(CPU进入休眠状态)：7mA
![Android手机耗电深度解析！](http://i1.android.265g.com/allimg/131116/10393QR4-4.jpg)
灭屏WiFi：70 mA
![Android手机耗电深度解析！](http://i1.android.265g.com/allimg/131116/10393Q025-5.jpg)
灭屏3G net：270 mA
![Android手机耗电深度解析！](http://i1.android.265g.com/allimg/131116/10393W5F-6.jpg)
灭屏2G net GPRS：280mA
![Android手机耗电深度解析！](http://i1.android.265g.com/allimg/131116/10393Q395-7.jpg)
亮屏benchmark：140mA
![Android手机耗电深度解析！](http://i1.android.265g.com/allimg/131116/10393T263-8.jpg)
亮屏Wi-Fi：280mA
![Android手机耗电深度解析！](http://i1.android.265g.com/allimg/131116/10393WU2-9.jpg)
亮屏3G net：360mA
![Android手机耗电深度解析！](http://i1.android.265g.com/allimg/131116/10393T557-10.jpg)
亮屏2G：370mA
![Android手机耗电深度解析！](http://i1.android.265g.com/allimg/131116/10393UN0-11.jpg)
亮屏待机：140mA
![Android手机耗电深度解析！](http://i1.android.265g.com/allimg/131116/10393R291-12.jpg)
亮屏Wi-Fi ping 1024包：150mA
![Android手机耗电深度解析！](http://i1.android.265g.com/allimg/131116/10393QR5-13.jpg)
亮屏Wi-Fi ping 65500包：160mA
![Android手机耗电深度解析！](http://i1.android.265g.com/allimg/131116/10393QL2-14.jpg)
灭屏 屏1024：45mA
![Android手机耗电深度解析！](http://i1.android.265g.com/allimg/131116/10393V962-15.jpg)
灭屏ping 65500：55mA
![Android手机耗电深度解析！](http://i1.android.265g.com/allimg/131116/10393R441-16.jpg)
关闭所有数据网络待机：7mA
其他android学习教程

## [android学习笔记（57）Handler消息传递机制](http://www.itmmd.com/201502/575.html)
## [android学习笔记（56）android重写onConfigurationChange...](http://www.itmmd.com/201501/570.html)
## [android学习笔记（55）android
 Configuration响应的系统设置的事...](http://www.itmmd.com/201501/568.html)


