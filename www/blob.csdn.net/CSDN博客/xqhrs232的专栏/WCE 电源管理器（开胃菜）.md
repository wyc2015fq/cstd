# WCE 电源管理器（开胃菜） - xqhrs232的专栏 - CSDN博客
2014年11月04日 10:13:56[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：784
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
原文地址::[http://blog.csdn.net/wzy198852/article/details/38492759](http://blog.csdn.net/wzy198852/article/details/38492759)
相关文章
1、
## WM/WINCE代码研读系列之 Power Management(1)
 ----[http://blog.sina.com.cn/s/blog_4ef8be9f0100e77r.html](http://blog.sina.com.cn/s/blog_4ef8be9f0100e77r.html)
2、
WM/WINCE代码研读系列之 Power Management(2)----[http://blog.sina.com.cn/s/blog_4ef8be9f0100e77s.htm](http://blog.sina.com.cn/s/blog_4ef8be9f0100e77s.html)
[l](http://blog.sina.com.cn/s/blog_4ef8be9f0100e77s.html)
3、
## WM/WINCE代码研读系列之 Power Management(3)
 ----[http://blog.sina.com.cn/s/blog_4ef8be9f0100e77u.html](http://blog.sina.com.cn/s/blog_4ef8be9f0100e77u.html)
4、
## WM/WINCE代码研读系列之Power Management(4)
 ----[http://blog.sina.com.cn/s/blog_4ef8be9f0100e77v.html](http://blog.sina.com.cn/s/blog_4ef8be9f0100e77v.html)
5、
## WM/WINCE代码研读系列之Power Management(5)
 ----[http://blog.sina.com.cn/s/blog_4ef8be9f0100e77x.html](http://blog.sina.com.cn/s/blog_4ef8be9f0100e77x.html)
6、
## WM/WINCE代码研读系列之Power Management(6)
 ----[http://blog.sina.com.cn/s/blog_4ef8be9f0100e77z.html](http://blog.sina.com.cn/s/blog_4ef8be9f0100e77z.html)
7、[关于SystemIdleTimerReset函数](http://blog.csdn.net/aaa_tnt/article/details/6588397)----[http://blog.csdn.net/aaa_tnt/article/details/6588397](http://blog.csdn.net/aaa_tnt/article/details/6588397)
8、为什么"PowerManager/ActivityTimer/UserActivity"这个输入事件有的时候捕捉不到？----[http://bbs.csdn.net/topics/370216625](http://bbs.csdn.net/topics/370216625)
9、关于系统自动关闭背光的问题----[http://bbs.csdn.net/topics/300045846](http://bbs.csdn.net/topics/300045846)
研究了将近一个月的 NDIS、RDIS以及电源管理器，分析了无数的源代码。赶快记一下，不然弄完其他模块，这部分又忘了。电源管理器的简单介绍网友已经写得很清楚了，感谢各位同仁。补存一句：csdn的文章发布编辑器真心做的很烂。
电源管理的目的是节能,基本的节能方法是使系统适时的进出休眠状态.比如用户按下On/Off按钮,或者监视用户活动的定时器超时,或者应用呼叫api都可以使得系统休眠,用户再次按下On/Off或者其他唤醒中断将使得系统退出休眠.从而可见,电源管理模块和用户活动情况密不可分,电源管理是用户活动所驱动的. WinCE中处理用户与系统交互的部分是GWES,所以早期电源管理工作是由GWES来实现.( GWES:Graphics,Windows and Events
 Subsystem.图形,窗口和事件子系统.主要负责图形输出和用户交互). 但GWES提供的电源管理模块功能过于粗糙死板:所有子设备只能有On和Suspend状态,应用程序无法得到任何状态转换通知,等等……直到WinCE4.0才引入了电源管理模块用以替代GWES中的电源管理功能.(进一步的,为了方便电源管理模块的集中管理,还需要关闭原来GWES对电源管理功能.方法是注册表HKLM/SYSTEM/CurrentControlSet/Control/Power设置DisableGwesPowerOff=1来禁止GWES插手电源管理.系统是默认禁止的.此外,一些用户活动情况仍旧依赖GWES获得,设置注册表HKLM/system/GWE下的ActivityEvent=PowerManager/ActivityTimer/UserActivity.从而告诉GWES,当鼠标,键盘,触摸屏等输入发生时候,GWES要SetEvent这个全局事件以通知电源管理模块.)
新的电源管理模块提供更完整和灵活的功能,系统电源可以自由灵活设定,子设备电源状态可以单独设定,应用可以获得电源通知等等.
### [系统电源]
OEM可以依据需要任意定义系统电源状态,比如On,ScreenOff,UserIdle,SystemIdle,Suspend等.系统电源状态更多的是代表系统电源的一种配置方案,它是各个子设备电源配置的集合.它设定一种可能出现的情景,并且事先拟定了此情景下电力分配策略(哪些子设备打开,哪些子设备关闭).比如,也许On可以代表常规工作的情景,所有子设备打开的状态; ScreenOff可以代表LCD被用户请求关闭的情景,LCD背灯电源被关闭的状态; UserIdle可以代表用户一段时间没有操作的情景,cpu/soc将进入low
 power的状态; Suspend可以代表设备空闲很久了可以挂起的情景,所有非必要供电的子设备电源关闭的状态;等等…系统的电源状态的定义很灵活而且自由. 可以在注册表定义系统电源状态.比如:
[HKEY_LOCAL_MACHINE/SYSTEM/CurrentControlSet/Control/Power/State/On]
"Default"=dword:0 ; D0
"Flags"=dword:10000 ; POWER_STATE_ON
上面定义了On状态,Flags是附加的状态信息(hints),对应pm.h中的宏定义POWER_STATE_ON.defaule表示在这个状态下所有子设备的默认状态.
电源管理模块的重点之一是制订系统电源管理策略,这包括定义系统电源状态,决定状态间转换的条件.以默认的版本为例子,简单图示如下:
子,简单图示如下:
![clip_image001[6]](http://byfiles.storage.msn.com/y1pJ5-6bpC7J942cDyptkXwrzNPQ0yU7QAEI-ByrxDUl_uRm-f4iqT9KGrJoPmzRCZyuxnb6w7utvw?PARTNER=WRITER)
On:用户与系统交互时候的状态.
UserIdle: 代表用户停止输入,但可能仍然在使用的情景,比如阅读文件.
SystemIdle: 代表用户停止使用设备,但处理器仍然工作的情景,比如,后台文件传输.
Suspend: 代表休眠状态.
用户在使用时候,系统处于On状态,用户停止输入,系统自动转入UserIdle状态,持续没有输入时间后,进入SystemIdle状态,持续一段时间后,系统将自动进入Suspend状态.应用程序也可以调用SetSystemPowerState()来进行状态切换.
**在这个基础上,根据自己的平台特点,增加新的策略就基本可以满足常规产品需要.**
1. On/Off按键. (A).电源管理模块已经支持了电源按键功能,最直接的办法可以在pdd中增加电源按键定义,按键io的初始化,检测等等,(B).从外部发送消息给电源管理模块来通知按键事件.(C).使用api直接转换状态.即不使用电源管理模块提供的按键功能,直接调用SetSystemPowerState使得系统进入Suspend状态.这是很常见的做法,我们设计一个电源按键的流驱动,检测到按键时候,呼叫api将系统电源转换到Suspend.
2. 加入背灯控制.比如在On状态下打开请求显示驱动打开背灯,在UserIdle和SystemIdle状态下请求显示驱动关闭背灯.
### [设备电源]
支持电源管理的设备驱动的实现,存在有大量的例子.简单介绍如下:
电源管理模块并不直接实现对子设备的电源开关控制,子设备的电源控制是由各个设备驱动来控制的.电源管理模块透过设备驱动的IOCTLs来请求设备控制自身电源.系统电源状态是灵活自由设定的,而设备电源状态是固定的,最多有5个:D0,D1,D2,D3,D4代表Full on,Low on, Standby, Sleep, Off这5个状态.
不是所有的设备驱动都支持电源管理(至少,在电源管理出现前的早期的设备驱动不会支持).电源管理模块对设备驱动提出了一个规范和架构,满足规范的驱动纳入电源管理.对于流驱动控制的设备,要支持电源管理要满足的条件,简单来说有:1.声明自己是支持电源管理的(Iclass值).2.驱动中实现电源管理模块所要求的IOCTLs.3.驱动加载时候要汇报所支持的电源状态和相关特征.4.***_PowerDown和***_PowerUp接口接收系统休眠和唤醒通知.此外,设计驱动还应该了解:设备不一定具备所有5种状态,但至少可以工作在D0;电源管理模块可能会要求设备进入任何设备电源状态,并不仅仅是设备所汇报自己支持的那几个;如果被要求进入不支持的状态,应该进入另一个它所支持的更高功耗的状态;当前状态不需要重复设置;设备电源状态不一定和系统的电源状态同步.除了流驱动外,还有许多内建驱动需要支持电源管理功能.简单总结:1.显示驱动通过ExtCode接口(SETPOWERMANAGEMENT命令,类似IOCTLs)来控制显示驱动的电源,还控制背灯.2键盘驱动的接口KeybdDriverPowerHandler.3.触摸屏是TouchPanelPowerHandler.4.内建网络miniport驱动是MiniportReset接口.5.PCMCIA驱动是PowerUp和PowerDown.还有打印机,红外等一些内建驱动.
### [OAL对电源管理的支持]
[系统的 idle状态]
当没有线程准备运行时候,内核就调用OEMIdle().这个函数在bsp中,可以由OEM来修改定制.一般我们在这个函数里面会要求cpu进入low power状态节省电流消耗.一般的cpu/soc都提供了对应idle的睡眠模式.当中断发生或者唤醒事件发生时候,要保证cpu快速离开idle状态,返回运行状态.
系统idle状态和前面说的UserIdle状态是不同概念,前者是cpu负荷情况驱动,代表系统空闲;后者是用户活动驱动,代表用户空闲.
一个OEMIdle()的推荐流程:
根据dwReschedTime变量来计算下次唤醒时间
判断sleep类型,假如需要,调整唤醒时间
Idle处理器和时钟
中断发生
判断唤醒源
更新CurMSec, idle计数值.
[系统suspend状态]
当用户按下OFF按钮或者应用调用api进入suspend状态时候,内核会调用OEMPowerOff()函数.在OEMPowerOff()函数里面实现系统挂起,并且系统唤醒后继续从OEMPowerOff()被挂起处执行. OEMPowerOff()时候要进入睡眠模式,睡眠模式根据cpu芯片的sleep模式来选择,要选择最低功耗的模式.如果cpu芯片提供的最低功耗模式是PowerDown模式,处理工作比较复杂,因为唤醒后是从reset处开始执行,要恢复挂起时候的环境,使得应用程序不知道自己被挂起过.一般按照这样流程来处理:关屏,清framebuffer,
 保存必须的寄存器到内存, 设置io, 保存通用寄存器, 保存wakeup地址, 静止中断,清除cache, 使能唤醒源中断, 设置sdram自刷新, cpu进入PowerDown. 唤醒后的流程相反即可. 对于PowerDown模式之外的其他模式,比如慢时钟模式, 处理则简单很多,最重要的是设置唤醒源(一般是任何中断可唤醒), sdram进入自刷新状态.
[SDRAM的控制]
SDRAM的耗电比较大,一般是系统里面除了lcd背光外,sdram是最大的电力消耗设备.常见有mobile sdram和normal sdram这2种,mobile sdram相对于normal sdram增加了温度补偿自刷新,局部阵列自刷新,深度休眠特性,更加适合功耗限制设备,(但mobile sdram工作在更低电压(1.8~2.5v),我想,对有些3.3v总线的cpu未必适合,因为总线会增加很多电平转换的电路.)
在OEMPowerOff()函数里面,保存好当前环境到sdram,然后使得sdram进入自刷新状态,cpu就可以进入最低功耗的sleep模式.唤醒后需要退出自刷新状态.
### [应用层于电源管理]
电源管理模块也提供了应用层接口,使得应用程序也可以参与到电源管理.
应用层可以通过SetSystemPowerState()来设置系统电源状态,可以通过SetDevicePower来设置子设备电源状态,可以通过SetPowerRequirement通知电源管理模块将子设备设置在特殊电源状态下,不随系统电源改变.此外,电源管理还提供了消息队列,应用层还可以通过RequestPowerNotifications函数请求电源管理模块发送相关消息(PBT_RESUME, PBT_POWERSTATUSCHANGE, PBT_TRANSITION,
 PBT_POWERINFOCHANGE).
设计应用程序也许有几点值得考虑:不要无谓占用cpu,尽可能快的让出cpu.比如一个很小的动画,哪怕只占1%的cpu也会导致一些系统无法进入低功耗.这里是2点建议:(1)当应用不在foreground时候,停止占用cpu.(2)用户没有和应用交互时候,停止应用对cpu的占用.另外一些应用也许是相反情况的,播放媒体文件时候,当开始播放时候,不希望自动进入suspend模式.可以(1)每隔一些时间就reset一次定时器.(2)或者设置所有定时器为0,停止电源管理(tcpmp就是这样的).
### [电源管理的系统实现]
电源管理模块实体是一个动态链接库pm.dll来实现的.可以在pb的catalog窗口中选择电源管理组件添加到os中.如下图,微软提供了2个选择(二选一).第一个代表完整功能,所有api全功能实现,第二个代表空实现(形式上提供接口,但空函数).
![clip_image010](http://byfiles.storage.msn.com/y1pJ5-6bpC7J97vk2tkuRfy-G1tLQRJUjmDuGf49w0LJNj-CiV9wysld8LiB-DeYWp29Ak047sBk9U?PARTNER=WRITER)
电源管理模块的代码结构是分层的,MDD+PDD.MDD是抽象公共库,不需要改动,PDD是平台相关,主要改动都在PDD.针对平台特性,微软提供了2种类型PDD示例.一种是default,另外一种是pda版本的.默认的情况,使用的是default.如果要使用pda版本的,需要在系统中指定环境变量**SYSGEN_PM_PDA**. default和pda版本的主要区别:
default版本定义了4种状态:On, UserIdle, SystemIdle, Suspend;
PDA版本定义了On, ScreenOff, Unattended, Resume, Suspend.
default版本的简单描述:UserIdle状态是描述用户在使用但没有操作,比如阅读.SystemIdle状态描述用户停止使用,但系统仍然工作,比如文件传输.
PDA版本简单描述:ScreenOff状态描述用户请求把屏幕背灯关闭.是用户主动关闭的情况,区别于UserIdle,UserIdle是自动的.Unattended状态表示后台工作,用户不会对其察觉的情景,比如ActiveSync每5分钟唤醒系统同步,然后继续suspend; Resume状态描述唤醒后情景,比如唤醒后在指定时间内决定转到哪个状态,否则继续suspend.
### [定制电源管理模块的方法]
Pm.dll是由device.exe加载的,首先device.exe当然是必须的,在pb的catalog中检查Device Manager组件,或者检查SYSGEN_DEVICE变量.其次,仍旧应该选择上图的电源管理组件power management full.
方案一(推荐方案):在bsp的驱动目录中新建一个pm目录,在这里完成电源管理模块PDD部分的实现,并链接MDD最终生成一个pm.dll替代原来系统的pm.dll.
PDD参考微软提供的代码platform.cpp,主要修改是增加状态转换的动作执行单元.
方案二:完全不修改电源管理部分,因为默认的PDD在状态转换时候虽然没有动作,但是广播了PBT_TRANSITION消息,可以截获这个消息来进行状态转换.这样作法不如方案一直接.如果是进程实现,还浪费一个宝贵进程资源.
### [影响系统功耗各方面考虑]
1.系统时钟周期
典型的WinCE系统时钟周期是1ms,增加时钟周期有助进一步降低设备功耗.在OEMInit()àOALTimerInit()修改系统时钟.
2.可变系统时钟节拍Variable Tick Scheduler
典型设计里wince每毫秒产生系统时钟中断,那么每隔1ms都会使得idle退出,如果发现没有线程就绪时候继续idle. 对有功耗限制的设计,可以考虑改变系统时钟节拍后进入idle状态.这样在预期的时间段里,idle状态不会被无谓的系统时钟中断唤醒.
3.LCD背灯的调节策略
早期的设计使用一个独立的驱动来实现背灯的控制和调节策略.简单介绍背灯驱动原理:背灯驱动启动一个监视工作线程,不停等待3个事件:
1. BackLightChangeEvent
2. PowerChangedEvent(供电电源发生变化,比如插手了AC电源,会获得了这个事件)
3. PowerManager/ActivityTimer/UserActivity(用户输入事件)
从注册表中读取超时值,当超时事件发生,则将系统背灯关闭.背灯关闭期间,用户重新活动时候,发生第3个事件,则打开背灯.注册表的超时值决定了背灯工作时间.类同pc上设置屏幕保护时间.此外,背灯驱动也需要提供对系统电源状态切换的支持.power down时候要关闭背灯,power up时候打开背灯.
电源管理模块可以定义一种系统电源状态来描述背灯关闭的情景(比如在UserIdle或者ScreenOff状态时候关闭背灯,On状态时候打开背灯)所以,背灯驱动可以被取消.
4.IO口的漏电流
空载IO避免设置成为输入口,考虑悬空输入导致门电路开关,造成电流消耗.负载IO依照情况设定,一般设置输出低.
5.电池驱动
电池驱动最主要的功能是监视系统电力.它提供了其他模块和应用对系统电源状态的查询,查询是AC,还是battary供电,查询电池电量等.
附：WinCE里面有关Resume状态的一些介绍
When a resume occurs the kernel is the first to execute. A device will only resume from a halted state if an interrupt occurs and the CPU has been programmed to wake when an interrupt occurs.
**To resume a device**
- The kernel performs the following clean-up tasks before resuming normal scheduling:
- If this is an SHx, it calls [OEMFlushCache](http://blog.csdn.net/wzy198852/article/details/38492759).
- Calls [InitClock](http://blog.csdn.net/wzy198852/article/details/38492759) to re-initialize timer hardware to
 a 1 ms tick.
- Calls Filesys.exe with a power on notification.
- Calls GWES with a power on notification.
- Initializes the KITL interrupt if one is in use.
- Power manager then calls [FileSystemPowerFunction](http://blog.csdn.net/wzy198852/article/details/38492759) with
 the (FSNOTIFY_POWER_ON) argument.
- On the resume, GWES performs the following tasks:
- Restores video memory from RAM.
- Powers on the Window Manager.
- Sets the screen contrast.
- Shows the Startup UI screen, if required.
- Posts a message to the Taskbar to tell it the device has resumed by send a WM_POWERBROADCAST message with PBT_APMRESUMESUSPEND parameter.
- Sends the same message to the User notification subsystem.
- Triggers applications that have requested to be triggered on resume.
//===================================================================================================
备注：：
1》WINCE设备下控制面板的电源挂起的注册表的位置
**HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Power**
**下的ExtPowerOff和BattPowerOff-----------------对应为时间，如果设置为0就是禁止休眠的意思！！！**

