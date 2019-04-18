# JVM调优命令-jmap - z69183787的专栏 - CSDN博客
2019年04月04日 17:49:34[OkidoGreen](https://me.csdn.net/z69183787)阅读数：36
[https://www.cnblogs.com/myna/p/7573843.html](https://www.cnblogs.com/myna/p/7573843.html)
**jmap**
JVM Memory Map命令用于生成heap dump文件，如果不使用这个命令，还可以使用-XX:+HeapDumpOnOutOfMemoryError参数来让虚拟机出现OOM的时候自动生成dump文件。 jmap不仅能生成dump文件，还可以查询finalize执行队列、Java堆和永久代的详细信息，如当前使用率、当前使用的是哪种收集器等。【内存分析】
|1234567891011121314151617181920212223242526272829|`[root@localhost jdk1.7.0_79]``# jmap --help``Usage:``    ``jmap [option] <pid>``        ``(to connect to running process)``    ``jmap [option] <executable <core>``        ``(to connect to a core ``file``)``    ``jmap [option] [server_id@]<remote server IP or ``hostname``>``        ``(to connect to remote debug server)``where <option> is one of:``    ``<none>               to print same info as Solaris pmap``    ``-heap                to print java heap summary``    ``-histo[:live]        to print histogram of java object heap; ``if``the ``"live"``                         ``suboption is specified, only count live objects``    ``-permstat            to print permanent generation statistics``    ``-finalizerinfo       to print information on objects awaiting finalization``    ``-dump:<dump-options> to dump java heap ``in``hprof binary ``format``                         ``dump-options:``                           ``live         dump only live objects; ``if``not specified,``                                        ``all objects ``in``the heap are dumped.``                           ``format``=b     binary ``format``                           ``file``=<``file``>  dump heap to <``file``>``                         ``Example: jmap -dump:live,``format``=b,``file``=heap.bin <pid>``    ``-F                   force. Use with -dump:<dump-options> <pid> or -histo``                         ``to force a heap dump or histogram when <pid> does not``                         ``respond. The ``"live"``suboption is not supported``                         ``in``this mode.``    ``-h | -help           to print this help message``    ``-J<flag>             to pass <flag> directly to the runtime system`|
命令格式
|123|`jmap [ option ] pid``jmap [ option ] executable core``jmap [ option ] [server-``id``@]remote-``hostname``-or-IP`|
**参数**
option：选项参数，不可同时使用多个选项参数
pid：java进程id，命令ps -ef | grep java获取
executable：产生核心dump的java可执行文件
core：需要打印配置信息的核心文件
remote-hostname-or-ip：远程调试的主机名或ip
server-id：可选的唯一id，如果相同的远程主机上运行了多台调试服务器，用此选项参数标识服务器
**options参数**
heap : 显示Java堆详细信息
histo : 显示堆中对象的统计信息
permstat :Java堆内存的永久保存区域的类加载器的统计信息
finalizerinfo : 显示在F-Queue队列等待Finalizer线程执行finalizer方法的对象
dump : 生成堆转储快照
F : 当-dump没有响应时，强制生成dump快照
**示例**
**-dump**
dump堆到文件,format指定输出格式，live指明是活着的对象,file指定文件名
|123|`[root@localhost jdk1.7.0_79]``# jmap -dump:live,format=b,file=dump.hprof 24971``Dumping heap to ``/usr/local/java/jdk1``.7.0_79``/dump``.hprof ...``Heap dump ``file``created`|
**-heap**
打印heap的概要信息，GC使用的算法，heap的配置及使用情况，可以用此来判断内存目前的使用情况以及垃圾回收情况
|123456789101112131415161718192021222324252627282930313233343536373839404142434445464748495051|`[root@localhost jdk1.7.0_79]``# jmap -heap 24971``Attaching to process ID 24971, please wait...``Debugger attached successfully.``Server compiler detected.``JVM version is 24.79-b02``using thread-``local``object allocation.``Parallel GC with 4 thread(s)``Heap Configuration:``   ``MinHeapFreeRatio = 0``   ``MaxHeapFreeRatio = 100``   ``MaxHeapSize      = 4146069504 (3954.0MB)``   ``NewSize          = 1310720 (1.25MB)``   ``MaxNewSize       = 17592186044415 MB``   ``OldSize          = 5439488 (5.1875MB)``   ``NewRatio         = 2``   ``SurvivorRatio    = 8``   ``PermSize         = 21757952 (20.75MB)``   ``MaxPermSize      = 85983232 (82.0MB)``   ``G1HeapRegionSize = 0 (0.0MB)``Heap Usage:``PS Young Generation``Eden Space:``   ``capacity = 517996544 (494.0MB)``   ``used     = 151567520 (144.54605102539062MB)``   ``free``= 366429024 (349.4539489746094MB)``   ``29.26033421566612% used``From Space:``   ``capacity = 41943040 (40.0MB)``   ``used     = 0 (0.0MB)``   ``free``= 41943040 (40.0MB)``   ``0.0% used``To Space:``   ``capacity = 40370176 (38.5MB)``   ``used     = 0 (0.0MB)``   ``free``= 40370176 (38.5MB)``   ``0.0% used``PS Old Generation``   ``capacity = 115343360 (110.0MB)``   ``used     = 32927184 (31.401809692382812MB)``   ``free``= 82416176 (78.59819030761719MB)``   ``28.54709972034801% used``PS Perm Generation``   ``capacity = 85983232 (82.0MB)``   ``used     = 54701200 (52.16712951660156MB)``   ``free``= 31282032 (29.832870483398438MB)``   ``63.6184506300019% used``20822 interned Strings occupying 2441752 bytes.`|
**-finalizerinfo**
打印等待回收的对象信息，
|123456|`[root@localhost jdk1.7.0_79]``# jmap -finalizerinfo 24971``Attaching to process ID 24971, please wait...``Debugger attached successfully.``Server compiler detected.``JVM version is 24.79-b02``Number of objects pending ``for``finalization: 0`|
**`Number of objects pending ``for``finalization: 0 `****`说明当前F-QUEUE队列中并没有等待Fializer线程执行final`**
**`izer方法的对象。`**
**-histo**
打印堆的对象统计，包括对象数、内存大小等等。jmap -histo:live 这个命令执行，JVM会先触发gc，然后再统计信息
|123456789101112131415161718192021222324252627282930313233|`[root@localhost jdk1.7.0_79]``# jmap -histo:live 24971 | more`` ``num     ``#instances         #bytes  class name``----------------------------------------------``   ``1:        100134       14622728  <constMethodKlass>``   ``2:        100134       12830128  <methodKlass>``   ``3:         88438       12708392  [C``   ``4:          8271       10163584  <constantPoolKlass>``   ``5:         27806        9115784  [B``   ``6:          8271        6225312  <instanceKlassKlass>``   ``7:          6830        5632192  <constantPoolCacheKlass>``   ``8:         86717        2081208  java.lang.String``   ``9:          2264        1311720  <methodDataKlass>``  ``10:         10880         870400  java.lang.reflect.Method``  ``11:          8987         869888  java.lang.Class``  ``12:         13330         747264  [[I``  ``13:         11808         733872  [S``  ``14:         20110         643520  java.util.concurrent.ConcurrentHashMap$HashEntry``  ``15:         18574         594368  java.util.HashMap$Entry``  ``16:          3668         504592  [Ljava.util.HashMap$Entry;``  ``17:         30698         491168  java.lang.Integer``  ``18:          2247         486864  [I``  ``19:          7486         479104  java.net.URL``  ``20:          8032         453616  [Ljava.lang.Object;``  ``21:         10259         410360  java.util.LinkedHashMap$Entry``  ``22:           699         380256  <objArrayKlassKlass>``  ``23:          5782         277536  org.apache.catalina.loader.ResourceEntry``  ``24:          8327         266464  java.lang.ref.WeakReference``  ``25:          2374         207928  [Ljava.util.concurrent.ConcurrentHashMap$HashEntry;``  ``26:          3440         192640  java.util.LinkedHashMap``  ``27:          4779         191160  java.lang.ref.SoftReference``  ``28:          3576         171648  java.util.HashMap``  ``29:         10080         161280  java.lang.Object`|
jmap -histo:live 24971 | grep com.yuhuo 查询类名包含com.yuhuo的信息
jmap -histo:live 24971 | grep com.yuhuo > histo.txt 保存信息到histo.txt文件
**附 - jmap输出中class name非自定义类的说明：**
|BaseType Character|Type|Interpretation|
|----|----|----|
|B|byte|signed byte|
|C|char|Unicode character|
|D|double|double-precision floating-point value|
|F|float|single-precision floating-point value|
|I|int|integer|
|J|long|long integer|
|L;|reference|an instance of class|
|S|short|signed short|
|Z|boolean|true or false|
|[|reference|one array dimension，[I表示int[]|
**-permstat**
打印Java堆内存的永久区的类加载器的智能统计信息。对于每个类加载器而言，它的名称、活跃度、地址、父类加载器、它所加载的类的数量和大小都会被打印。此外，包含的字符串数量和大小也会被打印。
|1234567891011121314151617181920|`[root@localhost jdk1.7.0_79]``# jmap -permstat 24971``Attaching to process ID 24971, please wait...``Debugger attached successfully.``Server compiler detected.``JVM version is 24.79-b02``finding class loader instances ..``done``.``computing per loader stat ..``done``.``please wait.. computing liveness....................................................liveness analysis may be inaccurate ...``class_loader    classes bytes   parent_loader   alive?  ``type``<bootstrap>   3034    18149440      null      live    <internal>``0x000000070a88fbb8  1   3048      null      dead    sun``/reflect/DelegatingClassLoader``@0x0000000703c50b58``0x000000070a914860  1   3064    0x0000000709035198  dead    sun``/reflect/DelegatingClassLoader``@0x0000000703c50b58``0x000000070a9fc320  1   3056    0x0000000709035198  dead    sun``/reflect/DelegatingClassLoader``@0x0000000703c50b58``0x000000070adcb4c8  1   3064    0x0000000709035198  dead    sun``/reflect/DelegatingClassLoader``@0x0000000703c50b58``0x000000070a913760  1   1888    0x0000000709035198  dead    sun``/reflect/DelegatingClassLoader``@0x0000000703c50b58``0x0000000709f3fd40  1   3032      null      dead    sun``/reflect/DelegatingClassLoader``@0x0000000703c50b58``0x000000070923ba78  1   3088    0x0000000709035260  dead    sun``/reflect/DelegatingClassLoader``@0x0000000703c50b58``0x000000070a88fff8  1   3048      null      dead    sun``/reflect/DelegatingClassLoader``@0x0000000703c50b58``0x000000070adcbc58  1   1888    0x0000000709035198  dead    sun``/reflect/DelegatingClassLoader``@0x0000000703c50b58`|
**-F**
强制模式。如果指定的pid没有响应，请使用jmap -dump或jmap -histo选项。此模式下，不支持live子选项。
