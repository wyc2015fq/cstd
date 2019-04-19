# JavaGC(2)-监控Java垃圾回收 - 零度的博客专栏 - CSDN博客
2016年05月18日 16:24:13[零度anngle](https://me.csdn.net/zmx729618)阅读数：1711
       本文是Java GC系列文章的第二篇。在第一篇中我们学习了不同GC算法的执行过程，GC是如何工作的，什么是新生代和老年代，你应该了解的JDK7中的5种GC类型，以及这5种类型对于应用性能的影响。在本文中，我将解释**JVM到底是如何执行垃圾回收处理的**。
**1、什么是GC监控？**
      垃圾回收收集监控指的是搞清楚JVM如何执行GC的过程，例如，我们可以查明：
1.        何时一个新生代中的对象被移动到老年代时，所花费的时间。
2.        Stop-the-world 何时发生的，持续了多长时间。
           GC监控是为了鉴别JVM是否在高效地执行GC，以及是否有必要进行额外的性能调优。基于以上信息，我们可以修改应用程序或者调整GC算法（GC优化）。
**2、如何监控GC**
       有很多种方法可以监控GC，但其差别仅仅是GC操作通过何种方式展现而已。GC操作是由JVM来完成，而GC监控工具只是将JVM提供的GC信息展现给你，因此，不论你使用何种方式监控GC都将得到相同的结果。所以你也就不必去学习所有的监控GC的方法。但是因为学习每种监控方法不会占用太多时间，了解多一点可以帮助你根据不同的场景选择最为合适的方式。
        下面所列的工具以及JVM参数并不适用于所有的HVM供应商。这是因为并没有关于GC信息的强制标准。本文我们将使用HotSpot JVM (Oracle JVM)。因为NHN 一直在使用Oracle (Sun) JVM，所以用它作为示例来解释我们提到的工具和JVM参数更容易些。
首先，GC监控方法根据访问的接口不同，可以分成CUI 和GUI 两大类。CUI GC监控方法使用一个独立的叫做”jstat”的CUI应用，或者在启动JVM的时候选择JVM参数”verbosegc”。GUI GC监控由一个单独的图形化应用来完成，其中三个最常用的应用是”jconsole”, “jvisualvm” 和 “Visual GC”。下面我们来详细学习每种方法。
**3、jstat**
        jstat 是HotSpot JVM提供的一个监控工具。其他监控工具还有jps 和jstatd。有些时候，你可能需要同时使用三种工具来监控你的应用。jstat 不仅提供GC操作的信息，还提供类装载操作的信息以及运行时编译器操作的信息。本文将只涉及jstat能够提供的信息中与监控GC操作信息相关的功能。
jstat 被放置在$JDK_HOME/bin。因此只要java 和 javac能执行，jstat 同样可以执行。
你可以在命令行环境下执行如下语句。
```
```bash
$> jstat –gc  $<vmid$> 1000
```
```bash
S0C       S1C       S0U    S1U      EC         EU          OC         OU         PC         PU         YGC     YGCT    FGC      FGCT     GCT
```
```bash
3008.0   3072.0    0.0     1511.1   343360.0   46383.0     699072.0   283690.2   75392.0    41064.3    2540    18.454    4      1.133    19.588
```
```bash
3008.0   3072.0    0.0     1511.1   343360.0   47530.9     699072.0   283690.2   75392.0    41064.3    2540    18.454    4      1.133    19.588
```
```bash
3008.0   3072.0    0.0     1511.1   343360.0   47793.0     699072.0   283690.2   75392.0    41064.3    2540    18.454    4      1.133    19.588
```
```bash
$>
```
```
在上图的例子中，实际的数据会按照如下列输出：
```
```bash
S0C    S1C     S0U     S1U    EC     EU     OC     OU     PC
```
```
        vmid (虚拟机 ID)，正如其名字描述的，它是虚拟机的ID，Java应用不论运行在本地还是远程的机器都会拥有自己独立的vmid。运行在本地机器上的vmid称之为lvmid (本地vmid)，通常是PID。如果想得到PID的值你可以使用ps命令或者windows任务管理器，但我们推荐使用jps来获取，因为PID和lvmid有时会不一致。jps 通过Java PS实现，jps命令会返回vmids和main方法的信息，正如ps命令展现PIDS和进程名字那样。
首先通过jps命令找到你要监控的Java应用的vmid，并把它作为jstat的参数。当几个WAS实例运行在同一台设备上时，如果你只使用jps命令，将只能看到启动（bootstrap）信息。我们建议在这种情况下使用ps -ef | grep java与jps配合使用。
想要得到GC性能相关的数据需要持续不断地监控，因此在执行jstat时，要规则地输出GC监控的信息。
例如，执行”jstat –gc 1000″ (或 1s)会每隔一秒展示GC监控数据。”jstat –gc 1000 10″会每隔1秒展现一次，且一共10次。
|参数名称|描述|
|----|----|
|gc|输出每个堆区域的当前可用空间以及已用空间（伊甸园，幸存者等等），GC执行的总次数，GC操作累计所花费的时间。|
|gccapactiy|输出每个堆区域的最小空间限制（ms）/最大空间限制（mx），当前大小，每个区域之上执行GC的次数。（不输出当前已用空间以及GC执行时间）。|
|gccause|输出-gcutil提供的信息以及最后一次执行GC的发生原因和当前所执行的GC的发生原因|
|gcnew|输出新生代空间的GC性能数据|
|gcnewcapacity|输出新生代空间的大小的统计数据。|
|gcold|输出老年代空间的GC性能数据。|
|gcoldcapacity|输出老年代空间的大小的统计数据。|
|gcpermcapacity|输出持久带空间的大小的统计数据。|
|gcutil|输出每个堆区域使用占比，以及GC执行的总次数和GC操作所花费的事件。|
**        你可以只关心那些最常用的命令，你会经常用到 -gcutil (或-gccause), -gc and –gccapacity。**
·         -gcutil 被用于检查堆间的使用情况，GC执行的次数以及GC操作所花费的时间。
·         -gccapacity以及其他的参数可以用于检查实际分配内存的大小。
**使用-gc 参数你可以看到如下输出：**
```
`S0C      S1C    …   GCT`
`1248.0   896.0  …   1.246`
`1248.0   896.0  …   1.246`
`…        …      …   …`
```
           不同的jstat参数输出不同类型的列，如下表所示，根据你使用的”jstat option”会输出不同列的信息。
|列|说明|Jstat参数|
|----|----|----|
|S0C|输出Survivor0空间的大小。单位KB。|-gc-gccapacity-gcnew-gcnewcapacity|
|S1C|输出Survivor1空间的大小。单位KB。|-gc-gccapacity-gcnew-gcnewcapacity|
|S0U|输出Survivor0已用空间的大小。单位KB。|-gc-gcnew|
|S1U|输出Survivor1已用空间的大小。单位KB。|-gc-gcnew|
|EC|输出Eden空间的大小。单位KB。|-gc-gccapacity-gcnew-gcnewcapacity|
|EU|输出Eden已用空间的大小。单位KB。|-gc-gcnew|
|OC|输出老年代空间的大小。单位KB。|-gc-gccapacity-gcold-gcoldcapacity|
|OU|输出老年代已用空间的大小。单位KB。|-gc-gcold|
|PC|输出持久代空间的大小。单位KB。|-gc-gccapacity-gcold-gcoldcapacity-gcpermcapacity|
|PU|输出持久代已用空间的大小。单位KB。|-gc-gcold|
|YGC|新生代空间GC时间发生的次数。|-gc-gccapacity-gcnew-gcnewcapacity-gcold-gcoldcapacity-gcpermcapacity-gcutil-gccause|
|YGCT|新生代GC处理花费的时间。|-gc-gcnew-gcutil-gccause|
|FGC|full GC发生的次数。|-gc-gccapacity-gcnew-gcnewcapacity-gcold-gcoldcapacity-gcpermcapacity-gcutil-gccause|
|FGCT|full GC操作花费的时间|-gc-gcold-gcoldcapacity-gcpermcapacity-gcutil-gccause|
|GCT|GC操作花费的总时间。|-gc-gcold-gcoldcapacity-gcpermcapacity-gcutil-gccause|
|NGCMN|新生代最小空间容量，单位KB。|-gccapacity-gcnewcapacity|
|NGCMX|新生代最大空间容量，单位KB。|-gccapacity-gcnewcapacity|
|NGC|新生代当前空间容量，单位KB。|-gccapacity-gcnewcapacity|
|OGCMN|老年代最小空间容量，单位KB。|-gccapacity-gcoldcapacity|
|OGCMX|老年代最大空间容量，单位KB。|-gccapacity-gcoldcapacity|
|OGC|老年代当前空间容量制，单位KB。|-gccapacity-gcoldcapacity|
|PGCMN|持久代最小空间容量，单位KB。|-gccapacity-gcpermcapacity|
|PGCMX|持久代最大空间容量，单位KB。|-gccapacity-gcpermcapacity|
|PGC|持久代当前空间容量，单位KB。|-gccapacity-gcpermcapacity|
|PC|持久代当前空间大小，单位KB|-gccapacity-gcpermcapacity|
|PU|持久代当前已用空间大小，单位KB|-gc-gcold|
|LGCC|最后一次GC发生的原因|-gccause|
|GCC|当前GC发生的原因|-gccause|
|TT|老年化阈值。被移动到老年代之前，在新生代空存活的次数。|-gcnew|
|MTT|最大老年化阈值。被移动到老年代之前，在新生代空存活的次数。|-gcnew|
|DSS|幸存者区所需空间大小，单位KB。|-gcnew|
         jstat 的好处是它可以持续的监控GC操作数据，不论Java应用是运行在本地还是远程，只要有控制台的地方就可以使用。当使用–gcutil 会输出如下信息。在GC优化的时候，你需要特别注意YGC, YGCT, FGC, FGCT 和GCT。
```
```bash
S0      S1       E        O        P        YGC    YGCT     FGC    FGCT     GCT
```
```bash
0.00    66.44    54.12    10.58    86.63    217    0.928     2     0.067    0.995
```
```bash
0.00    66.44    54.12    10.58    86.63    217    0.928     2     0.067    0.995
```
```bash
0.00    66.44    54.12    10.58    86.63    217    0.928     2     0.067    0.995
```
```
        这些信息很重要，因为它们展示了GC处理到底花费了多少时间。
在这个例子中，YGC 是217而YGCT 是0.928，这样在简单的计算数据平均数后，你可以知道每次新生代的GC大概需要4ms（0.004秒），而full GC的平均时间为33ms。
但是，只看数据平均数经常无法分析出真正的GC问题。这是主要是因为GC操作时间严重的偏差（换句话说，假如两次full GC的时间是 67ms，那么其中的一次full GC可能执行了10ms而另一个可能执行了57ms。）为了更好地检测每次GC处理时间，最好使用 –verbosegc来替代数据平均数。
**4、-verbosegc**
-verbosegc 是在启动一个Java应用时可以指定的JVM参数之一。而jstat 可以监控任何JVM应用，即便它没有制定任何参数。 -verbosegc 需要在启动的时候指定，因此你可能会认为它没有必要（因为jstat可以替代之）。但是， -verbosegc 会以更浅显易懂的方式展现GC发生的结果，因此他对于监控监控GC信息十分有用。
||jstat|-verbosegc|
|----|----|----|
|监控对象|运行在本机的Java应用可以把日志输出到终端上，或者借助jstatd命令通过网络连接远程的Java应用。|只有那些把-verbogc作为启动参数的JVM。|
|输出信息|堆状态（已用空间，最大限制，GC执行次数/时间，等等）|执行GC前后新生代和老年代空间大小，GC执行时间。|
|输出时间|Every designated time每次设定好的时间。|每次GC发生的时候。|
|何时有用。|当你试图观察堆空间变化情况|当你试图了解单次GC产生的效果。|
下面是-verbosegc 的可用参数
· -XX:+PrintGCDetails
· -XX:+PrintGCTimeStamps
· -XX:+PrintHeapAtGC
· -XX:+PrintGCDateStamps (from JDK 6 update 4)
     如果只是用了 -verbosegc 。那么默认会加上 -XX:+PrintGCDetails。 –verbosgc 的附加参数并不是独立的。而是经常组合起来使用。
使用 –verbosegc后，每次GC发生你都会看到如下格式的结果。
[GC [<collector>: <starting occupancy1> -> <ending occupancy1>, <pause time1> secs] <starting occupancy3> -> <ending occupancy3>, <pause time3> secs]
|收集器|minor gc使用的收集器的名字。|
|----|----|
|starting occupancy1|GC执行前新生代空间大小。|
|ending occupancy1|GC执行后新生代空间大小。|
|pause time1|因为执行minor GC，Java应用暂停的时间。|
|starting occupancy3|GC执行前堆区域总大小|
|ending occupancy3|GC执行后堆区域总大小|
|pause time3|Java应用由于执行堆空间GC（包括major GC）而停止的时间。|
这是-verbosegc 输出的minor GC的例子。
```
`S0    S1     E      O      P        YGC    YGCT    FGC    FGCT     GCT`
`0.00``66.44``54.12`
```
10.58
```
`86.63`
```
217
```
`0.928`
```
2
```
`0.067`
```
0.995
```
`0.00``66.44``54.12`
```
10.58
```
`86.63`
```
217
```
`0.928`
```
2
```
`0.067`
```
0.995
```
`0.00``66.44``54.12`
```
10.58
```
`86.63`
```
217
```
`0.928`
```
2
```
`0.067`
```
0.995
```
```
       这是 Full GC发生时的例子
```
```bash
[Full GC [Tenured: 3485K->4095K(4096K), 0.1745373 secs] 61244K->7418K(63104K), [Perm : 10756K->10756K(12288K)], 0.1762129 secs] [Times: user=0.19
 sys=0.00, real=0.19 secs]
```
```
       如果使用了 CMS collector，那么如下CMS信息也会被输出。
由于 –verbosegc 参数在每次GC事件发生的时候都会输出日志，我们可以很轻易地观察到GC操作对于堆空间的影响。
**5、(Java) VisualVM  + Visual GC**
Java Visual VM是由Oracle JDK提供的图形化的汇总和监控工具。
![](http://www.importnew.com/wp-content/uploads/2012/12/Figure-1-VisualVM-Screenshot..png)
**图1: VisualVM 截图**
        除了JDK中自带的版本，你还可以直接从官网下载Visual VM。出于便利性的考虑，JDK中包含的版本被命名为Java VisualVM (jvisualvm),而官网提供的版本被命名为Visual VM (visualvm)。两者的功能基本相同，只有一些细小的差别，例如安装组件的时候。就个人而言，我更喜欢可以从官网下载的Visual
 VM。
![](http://www.importnew.com/wp-content/uploads/2012/12/Figure-2-Viusal-GC-Installation-Screenshot.png)
**图 2: Viusal GC 安装截图**
**         通过Visual GC，你可以更直观的看到执行jstatd 所得到的信息。**
![](http://www.importnew.com/wp-content/uploads/2012/12/Figure-3-Visual-GC-execution-screenshot.png)
**图3: Visual GC 执行截图**
**6、介绍监控远程JVM**
要进行远程监控, 本机的VisualVM就必须和远程的JVM要进行通信, 
Visualvm目前支持两种remote connection方式.分别是jstatd和JMX方式:
 这里我主要介绍的是通过JMX方式.通过JMX连接远程机器, 需要经过下面的配置:
    1. 修改远程机器JDK配置文件 (我这里远程机器是linux).
       a.进入JAVA_HOME\jre\lib\management\目录
       b.拷贝jmxremote.password.template这个文件到当前目录, 并改名为jmxremote.password
c.打开jmxremote.password文件，去掉 # monitorRole  QED 和
# controlRole  R&D 这两行前面的注释符号
    2. 修改远程机器上需要被监控的程序的配置文件 (我这里是监控Tomcat容器内部署的应用).
       a.进入TOMCAT_HOME\bin目录
b.打开catalina.sh文件，加入如下信息：
            JAVA_OPTS="$JAVA_OPTS -Djava.rmi.server.hostname=192.168.0.237
                                                           -Dcom.sun.management.jmxremote.port=18999
                                                           -Dcom.sun.management.jmxremote.ssl=false
                                                           -Dcom.sun.management.jmxremote.authenticate=false"
       c.重启Tomcat服务.
    3. 客户端VisualVM配置 (我客户端用的是WinXP).
a.直接反键点击Remote，选择Add Remote Host...
b.在弹出的界面中输入远程机器的IP地址(192.168.0.237)，这个IP地址会加入到Remote节点下.
c.反键点击这个IP地址，选择Add JMX Connection, 在弹出的界面中输入刚配置的端口号(18999), 这个连接会加入到该IP节点下.
d.反键点击这个连接，选择Open.
        此时就可以看到监控的界面, 从界面上我们可以看到CPU信息， 内存信息， 统计加载类数量，线程信息.
**7、JMeter**
[HPJMeter](https://h20392.www2.hp.com/portal/swdepot/displayProductInfo.do?productNumber=HPJMETER)
 可以很方便的分析 -verbosegc 输出的结果，如果Visual GC可以视作jstat的图形化版本，那么HPJMeter就相当于 –verbosgc的图形化版本。当然，GC分析只是HPJMeter提供的众多功能之一，HPJMeter是由惠普开发的性能监控工具，他可以支持HP-UX，Linux以及MS Windows。
起初，一个成为HPTune 被设计用来图形化的分析-verbosegc.输出的结果。但是，随着HPTune的功能被集成到HPJMeter 3.0版本之后，就没有必要单独下载HPTune了。但运行一个应用时， -verbosegc 的结果会被输出到一个独立的文件中。
你可以用HPJMeter直接打开这个文件，以便更直观的分析GC性能数据。
![](http://www.importnew.com/wp-content/uploads/2012/12/Figure-4-HPJMeter.png)
 图**4: HPJMeter**
       本文我们主要讲述了如果监控GC操作信息，这将是GC优化的前提。就我个人经验而言，我推荐使用jstat 来监控GC操作，如果你感觉到GC操作的执行时间过长，那就可以使用verbosegc 参数来分析GC。GC优化的大体步骤就是在添加verbosegc 参数后，调整GC参数，分析修改后的结果。在下一篇文章中，我们将通过真实的例子来讲解优化GC的最佳选择。
