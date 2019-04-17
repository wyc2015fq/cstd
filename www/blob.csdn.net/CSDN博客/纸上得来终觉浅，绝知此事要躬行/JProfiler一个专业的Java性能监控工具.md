# JProfiler一个专业的Java性能监控工具 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年06月23日 14:16:51[boonya](https://me.csdn.net/boonya)阅读数：4557









JProfiler是一个全功能的[Java](http://lib.csdn.net/base/javaee)剖析工具（profiler），专用於分析J2SE和J2EE应用程式。它把CPU、线程和记忆体的剖析组合在一个强大的应用中。JProfiler可提供许多IDE整合和应用服务器整合功能。JProfiler直觉式的GUI让你可以找到性能瓶颈、抓住内存泄漏(memory leaks)、并解决多线程的问题。它让你得以对heap walker作资源回收器的root analysis，可以轻易找出内存泄漏；heap快照（snapshot）模式让未被引用（reference）的对象, 稍微被引用的对象、或在终结（finalization）序列的对象都会被移除；整合精灵以便剖析瀏览器的Java外掛功能。

**JProfiler能帮助你快速创建不同内容的监控：**

![](https://img-blog.csdn.net/20160623141514492)
**官方下载地址：[https://www.ej-technologies.com/products/jprofiler/overview.html](https://www.ej-technologies.com/products/jprofiler/overview.html)**

**注：下述内容来自互联网博客，正确与否未做甄别，阅读慎之。**


下载地址:
[http://download.ej-technologies.com/jprofiler/jprofiler_windows_4_2_2.exe](http://download.ej-technologies.com/jprofiler/jprofiler_windows_4_2_2.exe)

解密过程:

将EJ[1].Technologies.JProfiler.v4.2.2.Incl.Keymaker-AGAiN压缩包中的jkgone.jar解压到

根目录或其它目录下运行如下命令.

java -jar jkgone.jar

附其它版本的注册码
[http://download.ej-technologies.com/jprofiler/jprofiler_windows_3_3_1.exe](http://download.ej-technologies.com/jprofiler/jprofiler_windows_3_3_1.exe)

JProfiler 4.0

Name and Company: anything s/n: A-G666#76114F-1olm9mv1i5uuly#0126

JProfiler 3.3.1

s/n: A-XiV7#20128F-1nf9r2z1qepp2e#7120

EJ Technologies JProfiler 2.2.1 

S/N: A-DWP#OWNZ#YOU-212hyr 

JProfiler 3.3

S/N: A-XiV6#62267F-1tfbcghardqqd#16312<br>

JProfiler 3.2.0

S/N: A-GAiN#91584F-vd0mmz13mkf00#181013<br>or<br>A-GAiN#22031F-1giul8u16x7p65#121218<br>or<br>A-GAiN#98900F-1j62dw18rpusn#111117<br>

JProfiler 3.2

S/N: A-GAiN#70503F-l7qte9gtq77c#81111<br>or<br>A-GAiN#19132F-y2fnayai9yu8#141420<br>



目前最新版为4.3,大家可以申请试用10天,如果大家还需要其它版本的注册码回复

官方试用版下载:
[http://www.ej-technologies.com/download/jprofiler/trial.php](http://www.ej-technologies.com/download/jprofiler/trial.php) .

注册文件下载地址:

[http://lyh007.gbaopan.com/files/245c5ad31b314e4abd6bb1c54e86ebbe.gbp](http://lyh007.gbaopan.com/files/245c5ad31b314e4abd6bb1c54e86ebbe.gbp)

由于G宝盘只能传小于10MB文件,所以到主页公布的邮箱进行全部下载.




来自：http://www.anymobile.org


1、摘要..........................................................................1

2、改善服务器的性能...........................................................1

3、分析器原理...................................................................2

4、JProfiler 简介...............................................................2

5、JProfiler 特征...............................................................3

6、本地监控.....................................................................4

7、远程监控.....................................................................7

8、参考..........................................................................9







### 1 、 摘要



       改善 Java 服务器的性能需要模拟负载下的服务器。创建一个模拟环境、搜集数据并且分析结果可能是对许多开发人员的挑战。这篇文章介绍了使用 JProfiler 跟踪分析 Java 服务器的性能。



简单的性能问题很容易分离并解决，然而，大的性能问题，如内存溢出或者系统的罢工，通常在系统处于高负载情况下发生，就不能这么简单的处理了。这些问题需要一个独立的测试环境、一个模拟的负载，并且需要仔细地分析和跟踪。



在这篇文章中，我使用比较通用的工具（ JProfiler 和 JBuilder ）和设备创建了一个性能监控分析环境，跟踪本地和远程的服务器程序，专注于三个性能问题：内存、垃圾回收和多线程运行状况，从而很好的监视 JVM 运行情况及其性能。


### 2、改善服务器的性能



服务器的性能改善是依赖于数据的。没有可靠的数据基础而更改应用或环境会导致更差的结果。分析器提供有用的 Java 服务器应用信息，但由于从单用户负载下的数据与多用户负载下得到的数据是完全不同的，这导致分析器的数据并不精确。在开发阶段使用分析器来优化应用的性能是一个好的方式，但在高负载下的应用分析可以取到更好的效果。

在负载下分析服务器应用的性能需要一些基本的元素：

1、  可控的进行应用负载测试的环境。

2、  可控的人造负载使得应用满负荷运行。

3、  来自监视器、应用和负载测试工具自身的数据搜集。

4、  性能改变的跟踪。

不要低估最后一个需求（性能跟踪）的重要性因为如果不能跟踪性能你就不能实际的管理项目。性能上 10-20% 的改善对单用户环境来说并没有什么不同，但对支持人员来说就不一样了。20% 的改善是非常大的，而且通过跟踪性能的改善，你可以提供重要的反馈和持续跟踪。

虽然性能跟踪很重要，但有时为了使后续的测试更加精确而不得不抛弃先前的测试结果。在性能测试中，改善负载测试的精确性可能需要修改模拟环境，而这些变化是必须的，通过变化前后的负载测试你可以观察到其中的转变。

### 3、 分析器原理



现在几乎所有的分析器都是从同一个起点和约束开始的： Java 虚拟机分析器界面 (JVMPI) ( 参考 "The Java Virtual Machine Profiler Interface") 。 Sun 微系统的 API 允许工具开发商接口或者连接到遵循 JVMPI 的 JVM 上，并且监控运作的方式以及 JVM 运行任何 Java 程序时的关键事件 -- 从单独的应用程序到 Applet 、 Servlet 和企业 JavaBeans
 (EJB) 组件。 





在分析器内启动一个程序意味着生成、捕捉、和观察大量数据，所以所有的分析器都包含着不同的方法来控制数据的流动，在不同的标准以及每一封装包的基础上进行过滤。同样 也可以使用灵活的正规表达式类型模式来完成。

### 4 、 JProfiler 简介

![](http://www.blogjava.net/images/blogjava_net/anymobile/8280/r_jp002.gif) 是一个全功能的 Java 剖析工具（ profiler ），专用于分析 J2SE 和 J2EE 应用程序。它把 CPU 、执行绪和内存的剖析组合在一个强大的 应用中。 JProfiler可提供许多 IDE 整合和应用服务器整合用途。 JProfiler 直觉式的 GUI 让你可以找到效能瓶颈、抓出内存漏失 (memory leaks) 、并解决执行绪的问题。它让你得以对 heap walker 作资源回收器的root analysis ，可以轻易找出内存溢出； heap 快照（ snapshot ）模式让未被参照（ reference ）的对象、稍微被参照的对象、或在终结（ finalization ）队列的对象 都会被移除；整合精灵以便剖析浏览器的 Java 外挂功能。

目前最新的版本是 4.1.2 ，几乎支持所有常用的 IDE 和 Application Server ，可以到其 EJ 官方网站 http://www.ej-technologies.com/ 下载，申请一个十天的试用注册码。

### 5、 JProfiler 特征

#### 5.1 内存剖析 Memory profiler

JProfiler 的内存视图部分可以提供动态的内存使用状况更新视图和显示关于内存分配状况信息的视图。所有的视图都有几个聚集层并且能够显示现有存在的对象和作为垃圾回收的对象。


- **所有对象 **

显示类或在状况统计和尺码信息堆上所有对象的包。你可以标记当前值并显示差异值。


- **记录对象 Record objects **

显示类或所有已记录对象的包。你可以标记出当前值并且显示差异值。


- **分配访问树 Allocation call tree **

显示一棵请求树或者方法、类、包或对已选择类有带注释的分配信息的J2EE组件。


- **分配热点 Allocation hot spots **

显示一个列表，包括方法、类、包或分配已选类的J2EE组件。你可以标注当前值并且显示差异值。对于每个热点都可以显示它的跟踪记录树。



#### 5.2 堆遍历 Heap walker



在JProfiler的堆遍历器(Heap walker)中，你可以对堆的状况进行快照并且可以通过选择步骤下寻找感兴趣的对象。堆遍历器有五个视图：


- **类 Classes **

显示所有类和它们的实例。


- **分配 Allocations **

为所有记录对象显示分配树和分配热点。


- **索引 References **

为单个对象和“显示到垃圾回收根目录的路径”提供索引图的显示功能。还能提供合并输入视图和输出视图的功能。


- **数据 Data **

为单个对象显示实例和类数据。


- **时间 Time **

显示一个对已记录对象的解决时间的柱状图。



#### 5.3 CPU 剖析 CPU profiler



JProfiler 提供不同的方法来记录访问树以优化性能和细节。线程或者线程组以及线程状况可以被所有的视图选择。所有的视图都可以聚集到方法、类、包或J2EE组件等不同层上。CPU视图部分包括：


- **访问树 Call tree **

显示一个积累的自顶向下的树,树中包含所有在JVM中已记录的访问队列。JDBC,JMS和JNDI服务请求都被注释在请求树中。请求树可以根据Servlet和JSP对URL的不同需要进行拆分。


- **热点 Hot spots **

显示消耗时间最多的方法的列表。对每个热点都能够显示回溯树。该热点可以按照方法请求，JDBC，JMS和JNDI服务请求以及按照URL请求来进行计算。


- **访问图 Call graph **

显示一个从已选方法、类、包或J2EE组件开始的访问队列的图。



#### 5.4 线程剖析 Thread profiler



对线程剖析，JProfiler提供以下视图:


- **线程历史 Thread history **

显示一个与线程活动和线程状态在一起的活动时间表。


- **线程监控 Thread monitor **

显示一个列表，包括所有的活动线程以及它们目前的活动状况。


- **死锁探测图表 Deadlock Detection **

显示一个包含了所有在JVM里的死锁图表。


- **目前使用的监测器 Current monitor useage **

显示目前使用的监测器并且包括它们的关联线程。


- **历史检测记录 History usage history **

显示重大的等待事件和阻塞事件的历史记录。


- **监测使用状态 Monitor usage statistics **

显示分组监测，线程和监测类的统计监测数据。



#### 5.5 VM 遥感勘测技术 VM telemetry



观察JVM的内部状态，JProfiler提供了不同的遥感勘测视图，如下所示:


- **堆 Heap **

显示一个堆的使用状况和堆尺寸大小活动时间表。


- **记录的对象 Recorded objects **

显示一张关于活动对象与数组的图表的活动时间表。


- **垃圾回收 Garbage collector **

显示一张关于垃圾回收活动的活动时间表。


- **类 Classes **

显示一个与已装载类的图表的活动时间表。


- **线程 Threads **

显示一个与动态线程图表的活动时间表。



### 6、 本地监控

系统环境 Windows OS ；软件 JBuilderX/2005 和 JProfiler 4.1.2

1 、安装 JBuilderX 和 JProfiler 4.1.2

2 、运行 JProfiler ， Session-> IDE integration tab, IDE 选择Borland JBuilder7 to 2005，点击Integrate按钮，选择JBuilder的安装目录，确认，会看到已经将JProfiler以OpenTool的形式，成功整合到JBuilder中，见下图。





![](http://www.blogjava.net/images/blogjava_net/anymobile/8280/r_jp003.png)




3 、运行 JBuilder ，打开 Run->Configurations ，选择或新建一个 Runtime ，在 Optimize 选项中就可以看到 JProfiler ，可以选择每次运行程序新建一个 JProfiler 窗口的提示设置。

4 、点击 Optimize Project 按钮，运行程序。



![](http://www.blogjava.net/images/blogjava_net/anymobile/8280/r_jp004.png)


5 、弹出如下的 JProfiler 窗口，确认相关的信息即可。



![](http://www.blogjava.net/images/blogjava_net/anymobile/8280/r_jp005.png)


6 、至此，就可以监控本地服务器的各个方面的性能了。
![](http://www.blogjava.net/images/blogjava_net/anymobile/8280/r_jp006.png)

![](http://www.blogjava.net/images/blogjava_net/anymobile/8280/r_jp007.png)

![](http://www.blogjava.net/images/blogjava_net/anymobile/8280/r_jp009.png)

![](http://www.blogjava.net/images/blogjava_net/anymobile/8280/r_jp010.png)


### 7、 远程监控

服务器程序一般运行在远程的服务器设备上，有时候我们还需要远程监控商用的服务器资源。

服务器[操作系统](http://lib.csdn.net/base/operatingsystem)Linux OS ，安装步骤如下：

1、./jprofiler_linux_4_1_2.sh，出现如下提示：

testing JVM in /usr/jdk1.4 ...

Starting Installer ...

注：对于没有安装X Server的机器，需要执行./jprofiler_linux_4_1_2.sh -q，否则会提示，

testing JVM in /usr/jdk1.4 ...

Starting Installer ...

This installer needs access to an X Server.

If this is not possible, you can run the installer in unattended mode

by passing the argument -q to the installer.

2、安装完毕后，会在/opt目录下，找到jprofiler的安装目录，/opt/jprofiler4。


本地操作系统 WindowXP ，相关的配置如下：

1 、本地安装 JProfiler ， Linux 服务器上也安装 JProfiler （只有本地 / 监控者的需要输入序列号）。

2 、打开本地的 JProfiler ， session->Integration wizards-> New Remote integration

3 、选择 on a remote computer ， platform 选择 linux
 x86/AMD64 ，点击 next

4 、输入远程 ip 地址，点击 next

5 、输入远程 JProfiler 的安装目录，默认都安装在 /opt/jprofiler4 下，一路NEXT

6 、出现下面提示框，按照要求配置下服务器的设置，界面如下：

![](http://www.blogjava.net/images/blogjava_net/anymobile/8280/r_jp008.png)

Java 执行语句中加入下列运行参数

 -Xint -Xrunjprofiler:port=8849 -Xbootclasspath/a:/opt/jprofiler4/bin/agent.jar ；

/etc/profile 中加入 export LD_LIBRARY_PATH=/opt/jprofiler4/bin/linux-x86 ，退出、重新登陆。

7 、好了，全部配置完毕，先运行远程服务器程序，再打开本地的 JProfiler 程序，握手成功后，远程程序正常运行了。

服务器信息如下：

[root@ns 55556]# tail -f nohup.out

JProfiler> Protocol version 21

JProfiler> Using JVMPI

JProfiler> 32-bit library

JProfiler> Listening on port: 8849.

JProfiler> Native library initialized

JProfiler> Waiting for a connection from the JProfiler GUI ...

// 以上为本地 JProfiler 连上前的系统提示

JProfiler> Using dynamic instrumentation

JProfiler> Time measurement: elapsed time

JProfiler> CPU profiling enabled

JProfiler> Starting org/anymobile/server/cmwap/CmwapServer ...

2005/12/15 17:05:46 [ INFO] - Starting Cmwap Stand Server ...
2005/12/15 17:05:47 [ INFO] - HandleThread runing ......



### 8 、补充说明及参考

#### 8.1 补充说明

如果你希望动态保存当前 Session 的运行数据的快照，点击 JProfiler 的保存按钮即可；

可以通过 JProfiler Start Center 的 Open snapshot tab 打开保存的 Session ；

你也可以右键点击某个视图，静态保存到 HTML 文件，文字描述加视图图片；

有一些视图的数据只会运行一次，不会动态的刷新，如内存视图中的分配访问树等视图；

WinXP 下 JProfiler 似乎不支持中文， 2K 下支持的，上面有一些图片是在 2K 系统上截取的；

另外， JProfiler 还可以监控某个 Application Server 、 Applet ，功能非常强大，可以参考软件自带的 Help 。

#### 8.2 参考

JProfiler Tutorials             [http://www.ej-technologies.com/products/jprofiler/tutorials.html](http://www.ej-technologies.com/products/jprofiler/tutorials.html)

[What's new in](http://www.ej-technologies.com/products/jprofiler/whatsnew41.html)JProfiler     [http://www.ej-technologies.com/products/jprofiler/whatsnew41.html](http://www.ej-technologies.com/products/jprofiler/whatsnew41.html)

JProfiler help                        [http://resources.ej-technologies.com/jprofiler/help/doc/](http://resources.ej-technologies.com/jprofiler/help/doc/)

Java Virtual Machine Profiler Interface  [http://java.sun.com/j2se/1.3/docs/guide/jvmpi/index.html](http://java.sun.com/j2se/1.3/docs/guide/jvmpi/index.html)




