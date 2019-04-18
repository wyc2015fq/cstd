# JVM性能调优监控工具jps、jstack、jstat、jmap、jinfo使用详解 - z69183787的专栏 - CSDN博客
2016年06月13日 14:44:52[OkidoGreen](https://me.csdn.net/z69183787)阅读数：773
## jps
查看所有的jvm进程，包括进程ID，进程启动的路径等等。
我自己也用PS，即：ps -ef | grep java
## jstack
观察jvm中当前所有线程的运行情况和线程当前状态。
系统崩溃了？如果java程序崩溃生成core文件，jstack工具可以用来获得core文件的java stack和native stack的信息，从而可以轻松地知道java程序是如何崩溃和在程序何处发生问题。
系统hung住了？jstack工具还可以附属到正在运行的java程序中，看到当时运行的java程序的java stack和native stack的信息, 如果现在运行的java程序呈现hung的状态，jstack是非常有用的。
## jstat
jstat利用JVM内建的指令对Java应用程序的资源和性能进行实时的命令行的监控，包括了对进程的classloader，compiler，gc情况；
特别的，一个极强的监视内存的工具，可以用来监视VM内存内的各种堆和非堆的大小及其内存使用量，以及加载类的数量。
## jmap
监视进程运行中的jvm物理内存的占用情况，该进程内存内，所有对象的情况，例如产生了哪些对象，对象数量；
系统崩溃了？jmap 可以从core文件或进程中获得内存的具体匹配情况，包括Heap size, Perm size等等
## jinfo
观察进程运行环境参数，包括Java System属性和JVM命令行参数
系统崩溃了？jinfo可以从core文件里面知道崩溃的Java应用程序的配置信息。
## 备注
如果能熟练运用这些命令，尤其是在linux下，那么完全可以代替jprofile等监控工具。
用命令的好处就是速度快，并且辅助于其他命令，比如grep gawk sed等，可以组装多种符合自己需求的工具。
# ![雨伞](http://static.oschina.net/uploads/img/201508/13111454_iH41.png)怎么用
====================以下为较常用====================
## jstat
具体参数如下：
```java
```
-class：统计class loader行为信息 
-compile：统计编译行为信息 
-gc：统计jdk gc时heap信息 
-gccapacity：统计不同的generations（包括新生区，老年区，permanent区）相应的heap容量情况 
-gccause：统计gc的情况，（同-gcutil）和引起gc的事件 
-gcnew：统计gc时，新生代的情况 
-gcnewcapacity：统计gc时，新生代heap容量 
-gcold：统计gc时，老年区的情况 
-gcoldcapacity：统计gc时，老年区heap容量 
-gcpermcapacity：统计gc时，permanent区heap容量 
-gcutil：统计gc时，heap情况
```
```
$ jstat -class 17970 1000 10 (每隔1秒监控一次，一共做10次) 
Loaded  Bytes  Unloaded  Bytes     Time   
  7837  8534.0     1069  1005.7      27.87
Loaded：Number of classes loaded.
Bytes：Number of Kbytes loaded.
Unloaded：Number of classes unloaded.
Bytes：Number of Kbytes unloaded.
Time：Time spent performing class load and unload operations.
$ jstat -gc 17970 2000 20 (每隔2秒监控一次，共20次)
$ jstat -gcutil 17970 1000 10 (按百分比显式) 
S0C        S1C       S0U      S1U    EC            EU           OC            OU            PC         PU          YGC    YGCT        FGC  FGCT     GCT   
15936.0 15936.0 9030.8  0.0     127872.0 61813.5   901120.0   544712.5  65536.0 35227.8  42078 2796.028  417   498.596 3294.624
S0C：Current survivor space 0 capacity (KB).
S1C：Current survivor space 1 capacity (KB).
S0U：Current survivor space 0 utilization (KB).
S1U：Current survivor space 1 utilization (KB).
EC：Current eden space capacity (KB).
EU：Eden space utilization (KB).
OC：Current old space capacity (KB).
OU：Old space utilization (KB).
PC：Current permanent space capacity (KB).
PU：Permanent space utilization (KB).
YGC：Number of young generation GC Events.
YGCT：Young generation garbage collection time.
FGC：Number of full GC events.
FGCT：Full garbage collection time.
GCT：Total garbage collection time.
![](http://static.oschina.net/uploads/space/2015/0813/110756_0Mj8_1990220.gif)
$ jstat -compiler 17970  （显示VM实时编译的数量等信息） 
Compiled Failed Invalid   Time   FailedType FailedMethod 
    3780      1       0   120.26          2 com/xxx/xxx run
jstat –gccapacity :可以显示，VM内存中三代（young,old,perm）对象的使用和占用大小，如：PGCMN显示的是最小perm的内存使用量，PGCMX显示的是perm的内存最大使用量，PGC是当前新生成的perm内存占用量，PC是但前perm内存占用量。其他的可以根据这个类推， OC是old内纯的占用量。
[root[@localhost](http://my.oschina.net/u/570656) bin]# jstat -gccapacity 25917
NGCMN  640.0
NGCMX  4992.0
NGC  832.0
S0C  64.0
S1C  64.0
EC  704.0
OGCMN  1408.0
OGCMX  60544.0
OGC  9504.0
OC  9504.0  OC是old内纯的占用量
PGCMN  8192.0  PGCMN显示的是最小perm的内存使用量
PGCMX  65536.0  PGCMX显示的是perm的内存最大使用量
PGC  12800.0  PGC是当前新生成的perm内存占用量
PC  12800.0  PC是但前perm内存占用量
YGC  164
FGC  6
jstat -gcnew pid: new对象的信息
[root[@localhost](http://my.oschina.net/u/570656) bin]# jstat -gcnew 25917
S0C  S1C  S0U  S1U  TT MTT  DSS  EC  EU  YGC  YGCT
64.0  64.0  47.4  0.0  2  15  32.0  704.0  145.7  168  0.254
jstat -gcnewcapacity pid: new对象的信息及其占用量
[root[@localhost](http://my.oschina.net/u/570656) bin]# jstat -gcnewcapacity 25917
NGCMN  NGCMX  NGC  S0CMX  S0C   S1CMX  S1C  ECMX  EC  YGC  FGC
640.0  4992.0  832.0 64.0   448.0 448.0  64.0  4096.0  704.0  168  6
jstat -gcold pid: old对象的信息。
[root[@localhost](http://my.oschina.net/u/570656) bin]# jstat -gcold 25917
PC  PU  OC  OU  YGC  FGC  FGCT  GCT
12800.0  12617.6  9504.0  6561.3  169  6  0.335  0.591
jstat -gcoldcapacity pid:old对象的信息及其占用量。
[root[@localhost](http://my.oschina.net/u/570656) bin]# jstat -gcoldcapacity 25917
OGCMN  OGCMX  OGC  OC  YGC  FGC  FGCT  GCT
1408.0  60544.0  9504.0  9504.0  169  6  0.335  0.591
jstat -gcpermcapacity pid: perm对象的信息及其占用量。
[root@localhost bin]# jstat -gcpermcapacity 25917
PGCMN  PGCMX  PGC  PC  YGC  FGC  FGCT  GCT
8192.0  65536.0  12800.0  12800.0  169  6  0.335  0.591
jstat -printcompilation pid:当前VM执行的信息。
[root@localhost bin]# jstat -printcompilation -h3  25917 1000 5
每1000毫秒打印一次，一共打印5次，还可以加上-h3每三行显示一下标题。
Compiled  Size  Type Method
788  73  1 java/io/File <init>
788  73  1 java/io/File <init>
788  73  1 java/io/File <init>
Compiled  Size  Type Method
788  73  1 java/io/File <init>
788  73  1 java/io/File <init>
## jmap
jmap -dump:format=b,file=dump.bin pid 
jmap 可以从core文件或进程中获得内存的具体匹配情况，包括Heap size, Perm size等等，目前只有在Solaris和Linux的JDK版本里面才有。
观察运行中的jvm物理内存的占用情况
打印出某个java进程（使用pid）内存内的，所有‘对象’的情况（如：产生那些对象，及其数量）。
可以输出所有内存中对象的工具，甚至可以将VM 中的heap，以二进制输出成文本。使用方法 jmap -histo pid。如果连用SHELL jmap -histo pid>a.log可以将其保存到文本中去，在一段时间后，使用文本对比工具，可以对比出GC回收了哪些对象。jmap -dump:format=b,file=String 3024可以将3024进程的内存heap输出出来到String文件里。
[root@localhost bin]# jmap -histo  25917
Attaching to process ID 26221, please wait...
Debugger attached successfully.
Client compiler detected.
JVM version is 1.5.0_08-b03
Iterating over heap. This may take a while...
Unknown oop at 0xaa6e42d0
Oop's klass is null
Object Histogram:
Size  Count  Class description
-------------------------------------------------------
3722768 30467  * ConstMethodKlass
1976480 25334  char[]
1907880 46994  * SymbolKlass
1762088 2947  byte[]
1709536 30467  * MethodKlass
1487816 2600  * ConstantPoolKlass
1009576 2600  * InstanceKlassKlass
904880  2199  * ConstantPoolCacheKlass
741432  30893  java.lang.String
653576  4785  int[]
351760  4397  java.lang.reflect.Method
277824  2894  java.lang.Class
248704  3401  short[]
200888  4411  java.lang.Object[]
193656  4045  java.lang.Object[]
179744  5617  java.util.TreeMap$Entry
175688  1800  java.util.HashMap$Entry[]
165288  6887  java.util.HashMap$Entry
104736  3273  java.lang.ref.SoftReference
104136  4339  java.lang.ref.WeakReference
96096  3521  java.lang.String[]
86160  3590  java.util.Hashtable$Entry
85584  3566  java.util.ArrayList
83472  1206  java.util.Hashtable$Entry[]
82944  1728  java.beans.MethodDescriptor
80560  265  * ObjArrayKlassKlass
69120  1728  java.util.HashMap
52464  3055  java.lang.Class[]
43040  1076  java.util.Hashtable
42496  664  org.apache.commons.modeler.AttributeInfo
37880  947  java.util.TreeMap
33896  557  javax.management.modelmbean.ModelMBeanAttributeInfo[]
33152  518  java.beans.PropertyDescriptor
616  11  org.springframework.aop.framework.ProxyFactory
608  19  java.util.PropertyPermission
608  38  org.springframework.beans.MutablePropertyValues
608  38  org.springframework.beans.factory.support.MethodOverrides
608  2  * ArrayKlassKlass
608  38  org.springframework.beans.factory.config.ConstructorArgumentValues
608  4  org.apache.xerces.impl.XMLDTDScannerImpl
576  24  java.util.Stack
576  36  java.util.regex.Pattern$Category
576  24  org.apache.naming.NamingEntry
560  7  java.net.URL[]
552  23  sun.management.MappedMXBeanType$BasicMXBeanType
552  1  java.util.Locale[]
552  22  java.io.ObjectStreamField[]
544  17  java.util.Collections$SynchronizedMap
176  11  java.util.regex.Pattern$Ctype
8  1  sun.reflect.GeneratedMethodAccessor49
8  1  sun.reflect.GeneratedMethodAccessor6
8  1  sun.reflect.GeneratedConstructorAccessor10
Heap traversal took 12.003 seconds.
## jinfo
可以输出并修改运行时的java 进程的opts。用处比较简单，就是能输出并修改运行时的java进程的运行参数。用法是jinfo -opt  pid 如：查看2788的MaxPerm大小可以用  jinfo -flag MaxPermSize 2788。
# ![雨伞](http://static.oschina.net/uploads/img/201508/13111454_iH41.png)课外补充
## jstatd
[http://docs.oracle.com/javase/1.5.0/docs/tooldocs/share/jstatd.html](http://docs.oracle.com/javase/1.5.0/docs/tooldocs/share/jstatd.html)
启动jvm监控服务。它是一个基于rmi的应用，向远程机器提供本机jvm应用程序的信息。默认端口1099。 
实例：jstatd -J-Djava.security.policy=my.policy 
my.policy文件需要自己建立，内如如下： 
grant codebase "file:$JAVA_HOME/lib/tools.jar" { 
permission java.security.AllPermission; 
};
## jdb
jdb 用来对core文件和正在运行的Java进程进行实时地调试，里面包含了丰富的命令帮助您进行调试，它的功能和Sun studio里面所带的dbx非常相似，但 jdb是专门用来针对Java应用程序的。
用来对core文件和正在运行的Java进程进行实时地调试，里面包含了丰富的命令帮助您进行调试，它的功能和Sun studio里面所带的dbx非常相似，但 jdb是专门用来针对Java应用程序的。
## jconsole
jconsole是基于 (JMX)的实时图形化监测工具，这个工具利用了内建到JVM里面的JMX指令来提供实时的性能和资源的监控，包括了Java程序的内存使用，Heap size, 线程的状态，类的分配状态和空间使用等等。
与jstat相当。
jconsole:一个java GUI监视工具，可以以图表化的形式显示各种数据。并可通过远程连接监视远程的服务器VM。
用java写的GUI程序，用来监控VM，并可监控远程的VM，非常易用，而且功能非常强。命令行里打 jconsole，选则进程就可以了。
