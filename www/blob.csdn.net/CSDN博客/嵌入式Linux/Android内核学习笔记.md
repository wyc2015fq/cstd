
# Android内核学习笔记 - 嵌入式Linux - CSDN博客

2018年10月21日 22:57:44[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：35



# 0、android系统启动
《[Android系统启动流程 -- bootloader](http://blog.csdn.net/lizhiguo0532/article/details/7017503)》
《[The Android boot process from power on](http://blog.csdn.net/bonderwu/article/details/5903872)》
《[Android 启动过程介绍](http://blog.csdn.net/yangwen123/article/details/8023654)》
《[Android培训班(86)内核运行之前的引导程序](http://blog.csdn.net/caimouse/article/details/7185040)》 这是一系列文章
《[Android 分区挂载](http://blog.csdn.net/kehyuanyu/article/details/47129437)》

# 1、init
《深入理解android 卷1》 第3章 深入理解init
《Android框架揭秘》第3章 init进程
《[Android的init过程详解（一）](http://www.cnblogs.com/nokiaguy/archive/2013/04/14/3020774.html)》
《[Android的init过程（二）：初始化语言（init.rc）解析](http://www.cnblogs.com/nokiaguy/p/3164799.html)》
《[Android系统初始化开始位置及流程分析](http://wenku.baidu.com/view/837409f1f90f76c661371aa6.html)》
《[Android的初始化语言脚本init.rc解析](http://wenku.baidu.com/view/dcdd2397dd88d0d233d46a3c.html)》
《[Android Init进程源码分析](http://blog.csdn.net/yangwen123/article/details/9029959)》 init.rc解析过程描述详细，图文并茂。
《[Android Init进程分析](http://blog.csdn.net/justmantao/article/details/8751810)》
《[Android情景分析之详解init进程（以启动zygote为例）](http://blog.csdn.net/hu3167343/article/details/38299969)》

# 2、binder
《Android框架揭秘》第六章 -- 第十章 以生动的形式讲述了binder，其中也大量的插图，但是不够详细。值得参考。
《Android系统源代码情景分析》第五章 描叙详细，但是不够生动。值得参考。第六章的6.5小结描述BINDER_TYPE_FD。
《Android_IPC机制与Binder框架》 高焕堂著，形象的描述了从Java层到C++层的binder，偏重应用的实现。
《[Android Binder通信](http://blog.csdn.net/yangwen123/article/category/1609389)》
《[ServiceManager 进程启动源码分析](http://blog.csdn.net/yangwen123/article/details/9092005)》
《[图解Android - Binder 和 Service](http://www.cnblogs.com/samchen2009/p/3316001.html)》 描述别树一帜，形象生动，强烈推荐。
《[Android Binder线程](http://my.oschina.net/wolfcs/blog/209421)》 重点描述了binder线程的创建
《[本地Binder框架通信原理](http://www.cloudchou.com/android/post-534.html)》
《[Binder 机制详解—Binder 系统架构](http://www.cloudchou.com/android/post-507.html)》
《[浅析android中getStrongProxyForHandle函数动态申请索引对应的内存空间](http://blog.chinaunix.net/uid-20564848-id-73515.html)》
《[红茶一杯话Binder（ServiceManager篇）](http://my.oschina.net/youranhongcha/blog/149578)》

# 3、Ashmem（Anonymous Shared Memory） 匿名内存共享
《Android系统源代码情景分析》第六章 描叙详细。
《Android技术内幕》第二章[2.3.2小结](http://tech.it168.com/a2011/0805/1228/000001228471_1.shtml)描述的比较简单，但是对于《Android系统源代码情景分析》第六章的描述可以起到补充的作用。
《[3.2.1　匿名共享内存（1）](http://book.51cto.com/art/201305/395976.htm)》 《Android安全机制解析与应用实践》第3章
《[3.2.1　匿名共享内存（2）](http://book.51cto.com/art/201305/395977.htm)》
《[3.2.1　匿名共享内存（3）](http://book.51cto.com/art/201305/395978.htm)》
《[Android Ashmem相关介绍](http://www.linuxidc.com/Linux/2011-09/42212.htm)》

# 4、activity
《Android系统源代码情景分析》第七章 activity启动过程描叙详细。
《Android的设计与实现》 package manager篇（第8～9章）主要讲解了package manager的机制与实现，以及apk的安装方法与过程；activity manager篇（第10～12章）深入阐述了activitymanagerservice的运行机制、应用程序和进程的启动流程，以及进程管理机制。
《Android内核剖析》第十章 AMS内部原理
《[解开Android应用程序组件Activity的”singleTask”之谜](http://www.cnblogs.com/lovelili/archive/2011/11/21/2258198.html)》
《[Activity启动分析](http://blog.csdn.net/myarrow/article/details/8153489)》
《[android Application Component研究之Activity(一)](http://blog.csdn.net/windskier/article/details/7096521)》比较详细的描述了“task管理”，即startActivityUncheckedLocked函数（主要是处理activity的launchMode和intent的flag）
《[android Application Component研究之Activity(二)](http://blog.csdn.net/windskier/article/details/7172710)》
《[android:clearTaskOnLaunch的用法](http://blog.csdn.net/haiandengbin/article/details/7707267)》
《[Android 笔记之 clearTaskOnLaunch&finishOnTaskLaunch](http://my.eoe.cn/hking/archive/6045.html)》
《[FLAG_ACTIVITY_RESET_TASK_IF_NEEDED](http://blog.csdn.net/new_abc/article/details/13734819)》
《[android:clearTaskOnLaunch](http://80245089.iteye.com/blog/1225229)》
《[Android开发之旅：活动与任务](http://www.cnblogs.com/skynet/archive/2010/04/19/1715320.html)》
《[Activity启动创建 (AcitivtyManageService,ActivityThread,Activity)](http://www.cnblogs.com/lijunamneg/p/3573093.html)》
《[深入理解Activity启动流程](http://www.cloudchou.com/android/post-788.html)》

# 5、service
《Android系统源代码情景分析》第八章 service启动过程描叙详细。
《[android Application Component研究之Service](http://blog.csdn.net/windskier/article/details/7203293)》 有Forground属性的描述，有unbind过程的分析。
《[Android服务之Service(其一)](http://www.cnblogs.com/zhangdongzi/archive/2012/01/08/2316711.html)》 有生命周期图，有例子。
《[Service](http://developer.android.com/reference/android/app/Service.html)》google官方描述。
《[Android 中的 Service 全面总结](http://www.cnblogs.com/newcj/archive/2011/05/30/2061370.html)》
《[android如何保证service不被杀死](http://www.bdqn.cn/news/201305/9246.shtml)》

# 6、BroadcastReceiver
《Android系统源代码情景分析》第九章 broadcast receiver启动过程描叙详细
《[android Application Component研究之BroadcastReceiver](http://blog.csdn.net/windskier/article/details/7251742)》有broadcast发送和分发过程的描述，有Protected Broadcast的讲解，有broadcast超时的过程分析。

# 7、ContentProvider
《[android Application Component研究之ContentProvider](http://blog.csdn.net/windskier/article/details/7221313)》
《[存储文件的ContentProvider](http://ipjmc.iteye.com/blog/1447226)》

# 8、Mashup设计思想
《Android开发精要》3.1 基于Mashup的应用设计

# 9、属性系统(property system)
《[android property system ](http://www.360doc.com/content/13/0521/14/97538_287024765.shtml)》
《[属性系统代码分析](http://www.360doc.com/content/13/0521/14/97538_287024765.shtml)》
《[Android平台上使用属性系统(property system)](http://www.linuxidc.com/Linux/2011-09/43566.htm)》
《[Android Property System | Android属性系统](http://blog.csdn.net/jackyu613/article/details/6136620)》
《深入理解android 卷1》 第3章 3.2.4 属性系统
《Android框架揭秘》第3章 init进程 3.6 属性服务
《[Android 系统属性SystemProperty分析](http://blog.csdn.net/yangwen123/article/details/8936555)》
《[Android情景分析之属性服务](http://blog.csdn.net/hu3167343/article/details/38230271)》
《[SEAndroid安全机制对Android属性访问的保护分析](http://blog.csdn.net/luoshengyang/article/details/38102011)》
《[Android 的系统属性(SystemProperties)分析](http://blog.csdn.net/livingpark/article/details/7454063)》
《[Android 属性系统设计分析](http://blog.csdn.net/andyhuabing/article/details/7406930)》 有__libc_prenit的分析

# 10、创建设备节点文件uevent
《Android框架揭秘》第3章 init进程 3.4创建设备节点文件
《[基于android2.3.5系统：开天辟地Android启动机制[二]](http://blog.csdn.net/wavemcu/article/details/7799001)》
《[基于android2.3.5系统：开天辟地Android启动机制[三]](http://blog.csdn.net/wavemcu/article/details/7820070)》
《[android之init进程-uevent](http://blog.csdn.net/harbordaisy/article/details/7198494)》
《[为Android应用程序读取/dev下设备而提权](http://www.linuxidc.com/Linux/2011-11/47182.htm)》
《[android ueventd 本地native部分源码分析](http://blog.csdn.net/mociml/article/details/6801004)》
《[Android设备节点的动态管理](http://blog.csdn.net/xiyu_1986/article/details/7173075)》
《[android提权漏洞CVE-2010-EASY修复](http://blog.csdn.net/lhj0711010212/article/details/9351131)》
《[Android uevent进程源码分析](http://blog.csdn.net/yangwen123/article/details/9053527)》

# 11、zygote和system_server
《深入理解android 卷1》 第4章 深入理解zygote
《Android框架揭秘》第5章 Zygote
《Android系统源代码情景分析》第11章 Zygote和System进程的启动过程
《Android内核剖析》第15章 15.6 Framework资源
《[Zygote进程启动过程源代码分析](http://blog.csdn.net/yangwen123/article/details/17258023)》
《[SystemServer进程启动过程源码分析](http://blog.csdn.net/yangwen123/article/details/17258089)》
《[图解Android - Zygote, System Server 启动分析](http://www.cnblogs.com/samchen2009/p/3294713.html)》

# 12、进程管理和OOM
《深入理解Android 卷2》 第6章 6.6 AMS中的进程管理 从部分代码角度讲解，不是很详细；
《Android开发精要》第5章 组件生命周期解析 5.2 应用进程托管 -- 从上层角度描述为何要进程托管及托管的算法
《Android内核剖析》第十章 AMS内部原理 10.2 内存管理
《[Android 操作系统的内存回收机制](http://www.ibm.com/developerworks/cn/opensource/os-cn-android-mmry-rcycl/index.html)》
《[android 2.3 应用管理机制分析](http://www.bobbog.com/archives/614)》
《[Android内存管理机制详解](http://blog.csdn.net/chaihuasong/article/details/8289367)》
《[Android内存阀值修改--内存回收机制配置](http://blog.csdn.net/fancsxx/article/details/8548810)》
《[Linux 2.6 中的页面回收与反向映射](http://www.ibm.com/developerworks/cn/opensource/os-cn-android-mmry-rcycl/index.html)》
《[Android内存管理原理](http://www.cnblogs.com/killmyday/archive/2013/06/12/3132518.html)》
《[Android Low memory killer](http://blog.csdn.net/yseternalxuan/article/details/7447419)》
《[Android 系统内存分析](http://blog.csdn.net/kehyuanyu/article/details/47147751)》
《[Android开发:性能最佳实践-管理应用内存](http://www.pocketdigi.com/20140607/1334.html)》
《[Android代码内存优化建议-OnTrimMemory优化](http://www.open-open.com/lib/view/open1437404498474.html)》

# 13、Android编译系统
《Android内核剖析》 第18章 Android编译系统
《Android核心原理与系统应用高效开发》 第2章 编译结构和各种构建
《[Android编译系统参考手册](http://android.cloudchou.com/)》

# 14、WMS
《[Android 4.4(KitKat)窗口管理子系统 - 体系框架](http://www.2cto.com/kf/201407/317148.html)》

# 15、电池
《[android 电池（一）：锂电池基本原理篇](http://blog.csdn.net/xubin341719/article/details/8497830)》
《[android 电池（二）：android关机充电流程、充电画面显示](http://blog.csdn.net/xubin341719/article/details/8498580)》
《[android 电池（三）：android电池系统](http://blog.csdn.net/xubin341719/article/details/8709838)》
《[android电池（四）：电池 电量计(MAX17040)驱动分析篇](http://blog.csdn.net/xubin341719/article/details/8969369)》
《[android电池（五）：电池 充电IC（PM2301）驱动分析篇](http://blog.csdn.net/xubin341719/article/details/8970363)》
《[android 2.3 电量管理](http://blog.csdn.net/z642010820/article/details/7341469)》

# 16、input系统
《[android 物理按键](http://blog.csdn.net/xubin341719/article/details/7881735)》
《[InputMonitor笔记](http://blog.csdn.net/guoqifa29/article/details/38905347)》
《[Android之input系统流程](http://blog.csdn.net/leerobin83/article/details/6873238)》
《[Android之Input子系统事件分发流程](http://blog.csdn.net/tankai19880619/article/details/17019085)》
《[Android Input Framework(一)](http://blog.sina.com.cn/s/blog_89f592f50101394l.html)》
《[Android4.0 input事件输入流程详解(中间层到应用层)](http://blog.csdn.net/wlwl0071986/article/details/8247138)》
《[Android 4.2 Input 流程分析](http://m.blog.csdn.net/blog/lucky_liuxiang/17025137)》
《[安卓4.1: input系统从frameworks到kernel](http://blog.chinaunix.net/uid-27167114-id-3347185.html)》
《[Android输入输出机制之来龙去脉之前生后世](http://daojin.iteye.com/blog/1267890)》
《[android发现之旅之输入事件如何在应用端传递](http://www.tuicool.com/articles/QFbyMv)》

# 17、内存泄露
《[Android中native进程内存泄露的调试技巧](http://www.cnblogs.com/broadview/archive/2013/01/31/2886442.html)》

# 18、intent
《[Android开发之旅: Intents和Intent Filters（理论部分）](http://www.cnblogs.com/skynet/archive/2010/07/20/1781644.html)》

# 20、Dalvik
《[ Dalvik虚拟机进程和线程的创建过程分析](http://blog.csdn.net/luoshengyang/article/details/8923484)》
《[Android开发工具ADB教程之三：Dalvik虚拟机之jdwp线程](http://www.jizhuomi.com/android/environment/220.html)》
《[Android培训班(54)虚拟机的历史](http://blog.csdn.net/caimouse/article/details/6549454)》 这是一个系列文章
《[Android 信号处理面面观 之 trace 文件含义](http://blog.csdn.net/rambo2188/article/details/7017241)》
《[Java中System.loadLibrary() 的执行过程](http://my.oschina.net/wolfcs/blog/129696)》

# 21、View
《[android文本布局引擎](http://my.oschina.net/wolfcs/blog/139346#OSC_h2_11)》
《[Android Text Layout 框架](http://my.oschina.net/wolfcs/blog/106777)》
《[ViewGroup源码分析](http://blog.csdn.net/wulianghuan/article/details/41040149)》
《[Android TextView显示中文不对齐问题](http://blog.csdn.net/xiahao86/article/details/44980885)》
《[Android 属性动画 源码解析 深入了解其内部实现](http://blog.csdn.net/lmj623565791/article/details/42056859)》
《[Android应用程序UI硬件加速渲染的动画执行过程分析](http://blog.csdn.net/luoshengyang/article/details/46449677)》

# 22、AMS
《[ActivityManagerService分析一：AMS的启动](http://blog.csdn.net/lilian0118/article/details/26561835)》

# 23、资源
《[framework资源文件](http://blog.csdn.net/john19871213/article/details/8585418)》
《[如何在Android中写和使用一个自定义的java库](http://blog.chinaunix.net/uid-26669815-id-3092132.html)》
《[如何为Android系统添加一个新的资源包](http://my.oschina.net/kingguary/blog/177502?p=1)》
《[浅谈Android的资源编译过程](http://www.csdn123.com/html/blogs/20130619/24020.htm)》
《[在android中增加公用资源包（类似framework-res.apk）](http://blog.csdn.net/sxote/article/details/8550323)》
《[Android APK应用安装原理(1)-解析AndroidManifest原理-PackageParser.parserPackage](http://blog.csdn.net/zhbinary/article/details/7353739)》
《[Android 资源类型 整理](http://blog.csdn.net/kakaxi1o1/article/details/17515625)》
《[res与res-auto的区别](http://blog.csdn.net/altair86/article/details/34438989)》
《[Android中自定义属性的两种方法](http://blog.csdn.net/ouyangtianhan/article/details/6573957)》
《[Android应用资源---样式资源类型（Style）](http://blog.csdn.net/think_soft/article/details/7521393)》
《[Android应用资源---其他资源类型（More Types）（一）](http://blog.csdn.net/think_soft/article/details/7523498)》
《[Android应用资源---其他资源类型（More Types）（二）](http://blog.csdn.net/think_soft/article/details/7523672)》
《[Android应用资源---其他资源类型（More Types）（三）](http://blog.csdn.net/think_soft/article/details/7524103)》
《[Android xml资源文件中@、@android:type、@*、？、@+含义和区别](http://www.cnblogs.com/greatverve/archive/2011/12/27/Android-xml.html)》
《[android三个特殊的资源目录 /res/xml /res/raw 和 /assets](http://blog.csdn.net/shichexixi/article/details/5985683)》
《[Android字符串进阶之三：字体属性及测量（FontMetrics）](http://mikewang.blog.51cto.com/3826268/871765/)》
《[Styling Views on Android (Without Going Crazy)](http://blog.danlew.net/2014/11/19/styles-on-android/)》
《[Android学习系列(39)--Android主题和样式之系统篇（上）](http://www.cnblogs.com/qianxudetianxia/p/3725466.html)》
《[Android学习系列(40)--Android主题和样式之系统篇（下）](http://www.cnblogs.com/qianxudetianxia/p/3996020.html)》
《[Android系统自带样式（android:theme）](http://www.cnblogs.com/bluestorm/archive/2012/07/12/2588724.html)》
《[浅谈Android的资源编译过程](http://www.csdn123.com/html/blogs/20130619/24020.htm)》

# 24、android:persistent属性
《[android persistent属性研究](http://blog.csdn.net/windskier/article/details/6560925)》
《[Android: HowTo设置app不被系统kill掉](http://blog.csdn.net/zmyde2010/article/details/6756368)》
《[android里面的进程回收机制](http://blog.sina.com.cn/s/blog_514048cb0100wi2j.html)》
《[说说Android应用的persistent属性](http://my.oschina.net/youranhongcha/blog/269591)》

# 25、Handler、Looper和MessageQueue
《[Handler Looper MessageQueue 详解](http://www.cloudchou.com/android/post-388.html)》

# 26、debuggerd
《[Android debuggerd 源码分析](http://www.2cto.com/kf/201408/323895.html)》

# 27、log
《[解读Android LOG机制的实现](http://www.cnblogs.com/hoys/archive/2011/09/30/2196199.html)》
《[Android架构分析之LOG模块](http://blog.csdn.net/liuhaoyutz/article/details/9234911)》
《[Android系统开发中LOG的输出与使用](http://blog.csdn.net/andyhuabing/article/details/8547719)》
《[打开Android C文件中的LOG](http://blog.csdn.net/batoom/article/details/8103054)》
《[Android日志系统驱动程序Logger源代码分析](http://blog.csdn.net/luoshengyang/article/details/6595744)》
《[Android应用程序框架层和系统运行库层日志系统源代码分析](http://blog.csdn.net/luoshengyang/article/details/6598703)》
《[Android日志系统Logcat源代码简要分析](http://blog.csdn.net/luoshengyang/article/details/6606957)》
《[android log 和logcat 分析（一）](http://blog.csdn.net/lb5761311/article/details/43155609)》
《[android log你不知道的小技巧](http://tieba.baidu.com/p/3435484767)》
《[android中BuildConfig.DEBUG的使用](http://dengzhangtao.iteye.com/blog/1846534)》
《[android 代码优化：关闭输出日志](http://blog.csdn.net/jiese1990/article/details/21752159)》

# 28、DropBox
《[介绍 Android DropBoxManager Service](http://blog.csdn.net/ljchlx/article/details/8559963)》

# 29、AppOpsManager
《[Android Kitkat的AppOps](http://zhuanlan.zhihu.com/ccheng/19620324)》
《[Android 4.3 隐藏功能 App Ops 分析](http://blog.mssun.me/security/android-4-3-app-ops-analysis/)》
《[AppOpsManager 一次简单应用](http://blog.csdn.net/langzxz/article/details/45308199)》

# 30、ADB
《[android adb adbd analyse](http://blog.csdn.net/liranke/article/details/4999210)》
《[android2.3-adb源码分析](http://www.open-open.com/lib/view/open1328192447468.html)》
《[Android系统root破解原理分析](http://dengzhangtao.iteye.com/blog/1543494)》

# 31、BackupManagerService
《[第十五篇 Android 的Backup服务管理机制与架构分析](http://blog.csdn.net/goohong/article/details/8026045)》
《[BackupManager的使用与解析](http://blog.csdn.net/wangkaiblog/article/details/9491993)》
《[关于BackupManagerService](http://m.blog.csdn.net/blog/wangkaiblog_11109/9361295)》

# 32、UserManager 多用户机制
《[Android4.2多用户模式的结构与实现](http://itindex.net/detail/41132-android4.2-%E5%A4%9A%E7%94%A8%E6%88%B7-%E7%BB%93%E6%9E%84)》

# 33、DisplayManager 多屏幕机制
《[DisplayContent、StackBox、TaskStack笔记](http://www.cnblogs.com/mengfanrong/p/4022780.html)》
《[android 扩展屏幕 多屏显示](http://blog.sina.com.cn/s/blog_69d06d0c0102vb3x.html)》
《[第十七篇 --ANDROID DisplayManager 服务解析一](http://blog.csdn.net/goohong/article/details/8536102)》
《[android wifi display](http://blog.csdn.net/chinaunixj/article/details/10294143)》
《[Android WifiDisplay分析一：相关Service的启动](http://www.2cto.com/kf/201404/290996.html)》

# 34、Vold
《[Android vold 工作流程分析](http://blog.csdn.net/kehyuanyu/article/details/47131547?ref=myread)》

