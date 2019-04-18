# tomcat内存设置及jvm参数调优 - z69183787的专栏 - CSDN博客
2014年03月22日 20:43:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2705
**设置tomcat的内存**
1、修改 start.bat 中的
call "%EXECUTABLE%" start %CMD_LINE_ARGS%
为 call "%EXECUTABLE%" run %CMD_LINE_ARGS% 
2、 catalina.bat 首行加入
set JAVA_OPTS=-server -Xms512m -Xmx512m -XX:NewSize=64m -XX:MaxNewSize=512m -XX:PermSize=256m 
-XX:MaxPermSize=256m -XX:+UseParallelGC -XX:ParallelGCThreads=2
-server 以server的模式启动
-Xms   初始化堆内存大小（initial heap size）
-Xmx   最大堆内存大小（maximum heap size）
-XX:NewSize   新生代内存大小
-XX:MaxNewSize 最大新生代内存大小
-XX:PermSize   持久带内存初始化大小
-XX:MaxPermSize持久带内存最大大小
下面三个图很好的阐述了堆内存的分布： 新生代eden (young)   老年代tenure（old）  持久代（perm） 分布示意图如下：
![tomcat内存设置及jvm参数调优 - gubaojian - 去创新 去开创 为世界之光](http://img1.ph.126.net/ZlH_rWGt9NuqFVadeN_Myw==/6597816335051310481.jpg)
![tomcat内存设置及jvm参数调优 - gubaojian - 去创新 去开创 为世界之光](http://img3.ph.126.net/u7Xb6r6K7FvKECfpn__qSg==/6597508471795761653.jpg)
![tomcat内存设置及jvm参数调优 - gubaojian - 去创新 去开创 为世界之光](http://img3.ph.126.net/pDZ6WJiqAczYqu_3mNm4Cg==/6597639313679237743.jpg)
 调优的参数：
|Option and Default Value|Description|
|----|----|
|-XX:+AggressiveOpts|Turn on point performance compiler optimizations that are expected to be default in upcoming releases. (Introduced in 5.0 update 6.)|
|-XX:CompileThreshold=10000|Number of method invocations/branches before compiling [-client: 1,500]|
|-XX:LargePageSizeInBytes=4m|Sets the large page size used for the Java heap. (Introduced in 1.4.0 update 1.) [amd64: 2m.]|
|-XX:MaxHeapFreeRatio=70|Maximum percentage of heap free after GC to avoid shrinking.|
|-XX:MaxNewSize=size|Maximum size of new generation (in bytes). Since 1.4, MaxNewSize is computed as a function of NewRatio. [1.3.1 Sparc: 32m; 1.3.1 x86: 2.5m.]|
|-XX:MaxPermSize=64m|Size of the Permanent Generation.  [5.0 and newer: 64 bit VMs are scaled 30% larger; 1.4 amd64: 96m; 1.3.1 -client: 32m.]|
|-XX:MinHeapFreeRatio=40|Minimum percentage of heap free after GC to avoid expansion.|
|-XX:NewRatio=2|Ratio of new/old generation sizes. [Sparc -client: 8; x86 -server: 8; x86 -client: 12.]-client: 4 (1.3) 8 (1.3.1+), x86: 12]|
|-XX:NewSize=2m|Default size of new generation (in bytes) [5.0 and newer: 64 bit VMs are scaled 30% larger; x86: 1m; x86, 5.0 and older: 640k]|
|-XX:ReservedCodeCacheSize=32m|Reserved code cache size (in bytes) - maximum code cache size. [Solaris 64-bit, amd64, and -server x86: 48m; in 1.5.0_06 and earlier, Solaris 64-bit and amd64: 1024m.]|
|-XX:SurvivorRatio=8|Ratio of eden/survivor space size [Solaris amd64: 6; Sparc in 1.3.1: 25; other Solaris platforms in 5.0 and earlier: 32]|
|-XX:TargetSurvivorRatio=50|Desired percentage of survivor space used after scavenge.|
|-XX:ThreadStackSize=512|Thread Stack Size (in Kbytes). (0 means use default stack size) [Sparc: 512; Solaris x86: 320 (was 256 prior in 5.0 and earlier); Sparc 64 bit: 1024; Linux amd64: 1024 (was 0 in 5.0 and earlier); all others 0.]|
|-XX:+UseBiasedLocking|Enable biased locking. For more details, see this [tuning example](http://www.oracle.com/technetwork/java/tuning-139912.html#section4.2.5). (Introduced in 5.0 update 6.) [5.0: false]|
|-XX:+UseFastAccessorMethods|Use optimized versions of Get<Primitive>Field.|
|-XX:-UseISM|Use Intimate Shared Memory. [Not accepted for non-Solaris platforms.] For details, see [Intimate Shared Memory](http://www.oracle.com/technetwork/java/ism-139376.html).|
|-XX:+UseLargePages|Use large page memory. (Introduced in 5.0 update 5.) For details, see [Java Support for Large Memory Pages](http://www.oracle.com/technetwork/java/javase/tech/largememory-jsp-137182.html).|
|-XX:+UseMPSS|Use Multiple Page Size Support w/4mb pages for the heap. Do not use with ISM as this replaces the need for ISM. (Introduced in 1.4.0 update 1, Relevant to Solaris 9 and newer.) [1.4.1 and earlier: false]|
|-XX:+UseStringCache|Enables caching of commonly allocated strings.|
|-XX:AllocatePrefetchLines=1|Number of cache lines to load after the last object allocation using prefetch instructions generated in JIT compiled code. Default values are 1 if the last allocated object was an instance and 3 if it was an array.|
|-XX:AllocatePrefetchStyle=1|Generated code style for prefetch instructions.0 - no prefetch instructions are generate*d*,1 - execute prefetch instructions after each allocation,2 - use TLAB allocation watermark pointer to gate when prefetch instructions are executed.|
|-XX:+UseCompressedStrings|Use a byte[] for Strings which can be represented as pure ASCII. (Introduced in Java 6 Update 21 Performance Release)|
|-XX:+OptimizeStringConcat|Optimize String concatenation operations where possible. (Introduced in Java 6 Update 20)|
相关参考文档：
[http://www.oracle.com/technetwork/java/javase/tech/largememory-jsp-137182.html](http://www.oracle.com/technetwork/java/javase/tech/largememory-jsp-137182.html)
[http://docs.oracle.com/javase/6/docs/technotes/tools/windows/java.html](http://docs.oracle.com/javase/6/docs/technotes/tools/windows/java.html)
[http://docs.oracle.com/javase/6/docs/technotes/guides/vm/gc-ergonomics.html](http://docs.oracle.com/javase/6/docs/technotes/guides/vm/gc-ergonomics.html)
[http://www.oracle.com/technetwork/java/javase/tech/vmoptions-jsp-140102.html](http://www.oracle.com/technetwork/java/javase/tech/vmoptions-jsp-140102.html)
