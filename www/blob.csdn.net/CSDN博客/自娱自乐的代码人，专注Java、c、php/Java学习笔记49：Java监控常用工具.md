# Java学习笔记49：Java监控常用工具 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年06月17日 10:23:50[initphp](https://me.csdn.net/initphp)阅读数：1543
个人分类：[Java学习笔记系列](https://blog.csdn.net/initphp/article/category/1230072)

所属专栏：[Java学习笔记](https://blog.csdn.net/column/details/javago.html)









Java的安装包自带了很多优秀的工具，善用这些工具对于监控和调试Java程序非常有帮助。常用工具如下：

## jps 

用途：jps用来查看JVM里面所有进程的具体状态, 包括进程ID，进程启动的路径等等。

常用参数：

-l: 输出完成的包名称；

-m: 输出程序的命令行输入参数；

-v: 输出完整的JVM参数。

## jstack 

用途：1）查看java程序崩溃生成core文件，获得core文件的java stack和native stack的信息；2）查看正在运行的java程序的java stack和native stack的信息：a) 查看运行的java程序呈现hung的状态；b) 跟踪Java的调用栈，剖析程序。

## jinfo

用途：jinfo可观察运行中的java程序的运行环境参数：参数包括Java System属性和JVM命令行参数；也可从core文件里面知道崩溃的Java应用程序的配置信息。

## jstat 

**用途**：jstat利用了JVM内建的指令对Java应用程序的资源和性能进行实时的命令行的监控，包括了对Heap size和垃圾回收状况的监控等等。

**语法结构：**

Usage: jstat -help|-options

       jstat -<option> [-t] [-h<lines>] <vmid> [<interval> [<count>]]

**参数解释：**

Options — 选项，我们一般使用 -gcutil 查看gc情况

vmid    — VM的进程号，即当前运行的java进程号

interval– 间隔时间，单位为秒或者毫秒

count   — 打印次数，如果缺省则打印无数次

**具体option参数如下：**

-class：统计class loader行为信息

-compile：统计编译行为信息

-gc：统计jdk gc时heap信息

-gccapacity：统计不同的generations（不知道怎么翻译好，包括新生区，老年区，permanent区）相应的heap容量情况

-gccause：统计gc的情况，（同-gcutil）和引起gc的事件

-gcnew：统计gc时，新生代的情况

-gcnewcapacity：统计gc时，新生代heap容量

-gcold：统计gc时，老年区的情况

-gcoldcapacity：统计gc时，老年区heap容量

-gcpermcapacity：统计gc时，permanent区heap容量

-gcutil：统计gc时，heap情况

**输出内容含义如下：**

S0  — Heap上的 Survivor space 0 区已使用空间的百分比

S1  — Heap上的 Survivor space 1 区已使用空间的百分比

E   — Heap上的 Eden space 区已使用空间的百分比

O   — Heap上的 Old space 区已使用空间的百分比

P   — Perm space 区已使用空间的百分比

YGC — 从应用程序启动到采样时发生 Young GC 的次数

YGCT– 从应用程序启动到采样时 Young GC 所用的时间(单位秒)

FGC — 从应用程序启动到采样时发生 Full GC 的次数

FGCT– 从应用程序启动到采样时 Full GC 所用的时间(单位秒)

GCT — 从应用程序启动到采样时用于垃圾回收的总时间(单位秒) 

**示例**

实例使用1：

[root@localhost bin]# jstat -gcutil 25444

  S0     S1     E      O      P     YGC     YGCT    FGC    FGCT     GCT

 11.63   0.00   56.46  66.92  98.49 162    0.248    6      0.331    0.579 

实例使用2：

[root@localhost bin]# jstat -gcutil 25444 1000 5

  S0     S1     E      O      P     YGC     YGCT    FGC    FGCT     GCT

 73.54   0.00  99.04  67.52  98.49    166    0.252     6    0.331    0.583

 73.54   0.00  99.04  67.52  98.49    166    0.252     6    0.331    0.583

 73.54   0.00  99.04  67.52  98.49    166    0.252     6    0.331    0.583

 73.54   0.00  99.04  67.52  98.49    166    0.252     6    0.331    0.583

 73.54   0.00  99.04  67.52  98.49    166    0.252     6    0.331    0.583 

我们可以看到，5次young gc之后，垃圾内存被从Eden space区(E)放入了Old space区(O)，并引起了百分比的变化，导致Survivor space使用的百分比从73.54%(S0)降到0%(S1)。有效释放了内存空间。绿框中，我们可以看到，一次full gc之后，Old space区(O)的内存被回收，从99.05%降到67.52%。

图中同时打印了young gc和full gc的总次数、总耗时。而，每次young gc消耗的时间，可以用相间隔的两行YGCT相减得到。每次full gc消耗的时间，可以用相隔的两行FGCT相减得到。例如红框中表示的第一行、第二行之间发生了1次young gc，消耗的时间为0.252-0.252＝0.0秒。

常驻内存区(P)的使用率，始终停留在98.49%左右，说明常驻内存没有突变，比较正常。

如果young gc和full gc能够正常发生，而且都能有效回收内存，常驻内存区变化不明显，则说明java内存释放情况正常，垃圾回收及时，java内存泄露的几率就会大大降低。但也不能说明一定没有内存泄露。

GCT 是YGCT 和FGCT的时间总和。

以上，介绍了Jstat按百分比查看gc情况的功能。其实，它还有功能，例如加载类信息统计功能、内存池信息统计功能等，那些是以绝对值的形式打印出来的，比较少用，在此就不做介绍。 

[root@localhost bin]# ps -ef | grep java

root     25917     1  2 23:23 pts/2    00:00:05 /usr/local/jdk1.5/bin/java -Djava.endorsed.dirs=/usr/local/jakarta-tomcat-5.0.30/common/endorsed -classpath /usr/local/jdk1.5/lib/tools.jar:/usr/local/jakarta-tomcat-5.0.30/bin/bootstrap.jar:/usr/local/jakarta-tomcat-5.0.30/bin/commons-logging-api.jar -Dcatalina.base=/usr/local/jakarta-tomcat-5.0.30 -Dcatalina.home=/usr/local/jakarta-tomcat-5.0.30 -Djava.io.tmpdir=/usr/local/jakarta-tomcat-5.0.30/temp org.apache.catalina.startup.Bootstrap start

**jstat -class pid:显示加载class的数量，及所占空间等信息。**

实例使用3：

[root@localhost bin]# jstat -class 25917

Loaded  Bytes  Unloaded  Bytes     Time

2629    2916.8       29   24.6     0.90 

**jstat -compiler pid:显示VM实时编译的数量等信息。**

实例使用4：

[root@localhost bin]# jstat -compiler 25917

Compiled Failed Invalid   Time   FailedType FailedMethod

     768      0       0   0.70            0 

**jstat –gccapacity :可以显示，VM内存中三代（young,old,perm）对象的使用和占用大小**，如：PGCMN显示的是最小perm的内存使用量，PGCMX显示的是perm的内存最大使用量，PGC是当前新生成的perm内存占用量，PC是但前perm内存占用量。其他的可以根据这个类推， OC是old内纯的占用量。 

[root@localhost bin]# jstat -gccapacity 25917

NGCMN       640.0

NGCMX       4992.0

NGC         832.0

S0C         64.0

S1C         64.0

EC          704.0

OGCMN       1408.0

OGCMX       60544.0

OGC         9504.0

OC          9504.0                  OC是old内纯的占用量

PGCMN       8192.0                  PGCMN显示的是最小perm的内存使用量

PGCMX       65536.0                 PGCMX显示的是perm的内存最大使用量

PGC         12800.0                 PGC是当前新生成的perm内存占用量

PC          12800.0                 PC是但前perm内存占用量

YGC         164

FGC         6 

**jstat -gcnew pid: new对象的信息**

[root@localhost bin]# jstat -gcnew 25917

 S0C    S1C    S0U    S1U   TT MTT  DSS      EC       EU     YGC     YGCT

 64.0   64.0   47.4   0.0   2  15   32.0    704.0    145.7    168    0.254 

**jstat -gcnewcapacity pid: new对象的信息及其占用量**

[root@localhost bin]# jstat -gcnewcapacity 25917

 NGCMN  NGCMX   NGC   S0CMX  S0C   S1CMX  S1C   ECMX    EC      YGC   FGC

640.0  4992.0  832.0 64.0   448.0 448.0  64.0   4096.0  704.0  168     6

**jstat -gcold pid: old对象的信息。**

[root@localhost bin]# jstat -gcold 25917

   PC       PU        OC          OU       YGC    FGC    FGCT     GCT

 12800.0  12617.6     9504.0      6561.3   169     6    0.335    0.591

**jstat -gcoldcapacity pid:old对象的信息及其占用量。**

[root@localhost bin]# jstat -gcoldcapacity 25917

OGCMN      OGCMX        OGC         OC       YGC   FGC    FGCT     GCT

1408.0     60544.0      9504.0      9504.0   169     6    0.335    0.591 

**jstat -gcpermcapacity pid: perm对象的信息及其占用量。**

[root@localhost bin]# jstat -gcpermcapacity 25917

PGCMN      PGCMX       PGC         PC      YGC   FGC    FGCT     GCT

8192.0    65536.0    12800.0    12800.0   169     6    0.335    0.591 

**jstat -printcompilation pid:当前VM执行的信息。**

[root@localhost bin]# jstat -printcompilation -h3  25917 1000 5

每1000毫秒打印一次，一共打印5次，还可以加上-h3每三行显示一下标题。

Compiled  Size  Type Method

     788     73    1 java/io/File <init>

     788     73    1 java/io/File <init>

     788     73    1 java/io/File <init>

Compiled  Size  Type Method

     788     73    1 java/io/File <init>

     788     73    1 java/io/File <init>

## jmap

**用途**：观察运行中的jvm物理内存的占用情况，包括Heap size, Perm size等等。

**参数如下：**

-heap：打印jvm heap的情况

-histo：打印jvm heap的直方图。其输出信息包括类名，对象数量，对象占用大小。

-histo：live ：同上，但是只答应存活对象的情况

-permstat：打印permanent generation heap情况

**命令使用：**

jmap -heap 2083 ---- 观察到New Generation（Eden Space，From Space，To Space）,tenured generation,Perm Generation的内存使用情况

jmap -histo 2083 ｜ jmap -histo:live 2083   ---- 观察heap中所有对象的情况（heap中所有生存的对象的情况）。包括对象数量和所占空间大小。

jmap -dump:live,format=b,file=heap.bin 2083 ---- dump java heap in hprof binary format。输出文件可用于进一步分析。

class name对应的就是Class文件里的class的标识
B代表byte
C代表char
D代表double
F代表float
I代表int
J代表long
Z代表boolean
前边有[代表数组，[I 就相当于int[]
对象用[L+类名表示


|*BaseType*** Character**|**Type**|**Interpretation**|
|----|----|----|
|B|byte|signed byte|
|C|char|Unicode character|
|D|double|double-precision floating-point value|
|F|float|single-precision floating-point value|
|I|int|integer|
|J|long|long integer|
|L<classname>;|reference|an instance of class de><classname>de>|
|S|short|signed short|
|Z|boolean|de>truede> or de>falsede>|
|**[**|reference|one array dimension|



