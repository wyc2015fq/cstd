
# Java堆外内存泄露分析 - 无界 - CSDN博客

2019年02月28日 16:12:10[21aspnet](https://me.csdn.net/21aspnet)阅读数：194


说明：[https://blog.csdn.net/unix21/article/details/79161250](https://blog.csdn.net/unix21/article/details/79161250)本文登录在本人另一个博客，这个博客相对比较精练
查看堆内存占用正常，jvm垃圾回收也没有异常。而top出来显示java占用内存是几个G，那么可能想到了是堆外内存泄漏。
需要安装google-perftools工具进行分析
**1.先安装g++**
不然编译会报错：g++: command not found
\# yum -y install gcc gcc-c++
----
**2.安装libunwind**
不建议版本>0.99据说有问题，这个需要FQ
\# wgethttp://download.savannah.gnu.org/releases/libunwind/libunwind-0.99.tar.gz
\# tar -xzvf libunwind-0.99.tar.gz
\# cd libunwind-0.99
\# ./configure  --prefix=/data0/java/deploy/google-perftools/local/libunwind
\# make && make install
---------
**3.安装gperftools**
\# wget https://github.com/gperftools/gperftools/releases/download/gperftools-2.5/gperftools-2.5.tar.gz
\# tar -xzvf gperftools-2.5.tar.gz
\# cd gperftools-2.5
\# ./configure --prefix=/data0/java/deploy/google-perftools/local/gperftools-2.5/
\# make && make install
-----------
**4.使配置生效**
\# vim /etc/ld.so.conf.d/usr_local_lib.conf
新增以下内容按i
/data0/java/deploy/google-perftools/local/libunwind/lib
按esc再:wq! \#保存退出
\# /sbin/ldconfig  \#执行此命令，使libunwind生效。 需要sudo权限
-------------------
**5.加入环境变量**
\# export LD_PRELOAD=/data0/java/deploy/google-perftools/local/gperftools-2.5/lib/libtcmalloc.so
\# export HEAPPROFILE=/data0/java/deploy/google-perftools/local/tmp/gzip
------
**6.运行java程序**
\# java -classpath /xxx.jar com.xxx.xxx.HttpMain
就会在/data0/java/deploy/google-perftools/local/目录下生成heap文件
![](https://img-blog.csdn.net/20180126154135085?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20180126154206798?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
如果是持续运行的程序需要ctrl+c会生成dump文件。
**7.分析函数调用**
\# /data0/java/deploy/google-perftools/local/gperftools-2.5/bin/pprof --text /usr/lib/jdk/jdk1.8.0_161/bin/java /data0/java/deploy/google-perftools/local/tmp/gzip.0001.heap
![](https://img-blog.csdn.net/20180126154217367?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
下面就是分析的函数：
![](https://img-blog.csdn.net/20180126154239880?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
也可以直接导出完整的函数到文本
\# /data0/java/deploy/google-perftools/local/gperftools-2.5/bin/pprof --text /usr/lib/jdk/jdk1.8.0_161/bin/java /data0/java/deploy/google-perftools/local/tmp/gzip.0001.heap > gzip.0001.heap.log
下面选取一部分：

```python
Total: 21.6 MB
    17.9  82.5%  82.5%     17.9  82.5% os::malloc@9240a0
     3.0  13.9%  96.4%      3.0  13.9% init
     0.5   2.4%  98.8%      0.5   2.4% readCEN
     0.2   0.9%  99.7%      0.2   0.9% updatewindow
     0.0   0.2%  99.8%      0.0   0.2% inflateInit2_
     0.0   0.1%  99.9%      0.0   0.1% _dl_new_object
     0.0   0.0%  99.9%      0.0   0.0% _dl_allocate_tls
     0.0   0.0%  99.9%      0.0   0.0% _nl_intern_locale_data
     0.0   0.0% 100.0%      0.0   0.0% _dl_check_map_versions
     0.0   0.0% 100.0%      0.0   0.0% strdup
     0.0   0.0% 100.0%      0.5   2.4% ZIP_Put_In_Cache0
     0.0   0.0% 100.0%      0.0   0.0% _dl_map_object_deps
     0.0   0.0% 100.0%      0.0   0.0% nss_parse_service_list
     0.0   0.0% 100.0%      0.0   0.0% getpwuid
     0.0   0.0% 100.0%      0.0   0.0% newEntry
     0.0   0.0% 100.0%      0.0   0.2% Java_java_util_zip_Inflater_init
     0.0   0.0% 100.0%      0.0   0.0% __nss_database_lookup
     0.0   0.0% 100.0%      0.0   0.0% expand_dynamic_string_token
     0.0   0.0% 100.0%      0.0   0.0% initLoopbackRoutes
     0.0   0.0% 100.0%      0.0   0.0% JLI_MemAlloc
     0.0   0.0% 100.0%      0.0   0.0% __tzfile_read
     0.0   0.0% 100.0%      0.0   0.1% dl_open_worker
     0.0   0.0% 100.0%      0.0   0.0% add_to_global
     0.0   0.0% 100.0%      0.0   0.0% add_name_to_object
     0.0   0.0% 100.0%      0.0   0.0% tsearch
     0.0   0.0% 100.0%      0.0   0.0% _nl_load_locale_from_archive
     0.0   0.0% 100.0%      0.0   0.0% __nss_lookup_function
     0.0   0.0% 100.0%      0.0   0.0% __tzstring
     0.0   0.0% 100.0%      0.0   0.1% _dlerror_run
     0.0   0.0% 100.0%      0.0   0.0% std::basic_string::_Rep::_S_create
     0.0   0.0% 100.0%      0.0   0.0% ParseLocale
     0.0   0.0% 100.0%      0.0   0.0% new_composite_name
     0.0   0.0% 100.0%      0.0   0.0% __static_initialization_and_destruction_0 (inline)
```

参考此文：

[google-perftools安装](https://www.jianshu.com/p/309c9f61d495)
[使用堆外内存](http://www.raychase.net/1526)(说明什么是堆外内存以及怎么用堆外内存和问题)
[java堆外内存泄漏](http://blog.csdn.net/f529352479/article/details/51908655)
[perftools查看堆外内存并解决hbase内存溢出](http://koven2049.iteye.com/blog/1142768)
===============
下面这个是个人博客 怕没过几天网站不存在了，所以备份下
原文地址：[http://www.dylan326.com/2017/09/28/gperftools/](http://www.dylan326.com/2017/09/28/gperftools/)
perftools wiki地址 ：[https://github.com/gperftools/gperftools/wiki](https://github.com/gperftools/gperftools/wiki)
安装perftools
…
工具主要作用分析程序：heap、cpu等，常用于c/c++程序分析
使用文档如下：
[https://gperftools.github.io/gperftools/heapprofile.html](https://gperftools.github.io/gperftools/heapprofile.html)
[https://gperftools.github.io/gperftools/heap_checker.html](https://gperftools.github.io/gperftools/heap_checker.html)
[https://gperftools.github.io/gperftools/cpuprofile.html](https://gperftools.github.io/gperftools/cpuprofile.html)

## JVM堆外内存分析
本文针对于 Oracle Hotspot jvm 虚拟机
JVM 进程占用大小一般约等于： heap + perm + thread stack + nio directbuffer
当然还有其他占用，一般情况来看native memory跟踪可以使用NMT参数 -XX:NativeMemoryTracking
详情wiki ：[https://docs.oracle.com/javase/8/docs/technotes/guides/vm/nmt-8.html](https://docs.oracle.com/javase/8/docs/technotes/guides/vm/nmt-8.html)
JVM常见内存泄漏检查方式可以按照 oracle提供的文档：[http://www.oracle.com/technetwork/java/javase/memleaks-137499.html](http://www.oracle.com/technetwork/java/javase/memleaks-137499.html)
其中dbx是和perftools类似工具。
除此之外
进程内存在用在linux 面下top,pmap 等工具是较好确定进程内存的手段， 在/proc/${pid}/ 目录下smaps文件等查看RSS，虚拟内存，swap占用等。
pmap 找到内存申请的地址块， 可以使用gdb attach 后 dump 某一段的内存地址 ，后查看内存的数据。 注意gdb attach 进程会暂停， 线上谨慎操作。
如果发现进程内存过高，可能需要注意下程序是否存在堆外内存泄漏问题，java进程出现的堆外内存泄漏常见有如下几个方面：
JNI
NIO directbuffer 泄漏
gzip （本文案例就是此类）
网络编程中程序中，nio directbuffer 有监控工具可以查看， directbuffer使用量、并且可以设置最大直接内存buffer。
## JVM 进程使用perftools 做分析
案例中发现的问题jetty 服务在某版本的JDK（jdk7u80~jdk8u045等 应该7u80后的版本在本文章编写时候都有）出现的内存泄漏问题分析和查找
1、安装perftools 注意配置安装目录， 找到对应的文件路径
2、启动java程序时候携带环境变量（可以参照 perftools wiki帮助 写的较为明确， 文档主要针对正对c/c++程序， Hotspot主要是c++写的）
export LD_PRELOAD=/opt/perftools/lib/libtcmalloc.so \# 安装后的库文件路径
export HEAPPROFILE=/opt/mybin.hprof \#heap分析文件路径和文件
特别注意目录和文件权限是否是进程用户可使用的
3、运行后启动程序后
4、pprof执行程序（注意路径），分析这些heap文件 pprof –help查看使用方式
./perftools/bin/pprof –text *.heap 以文本结果展示heap的分析文件， 结果说明见perftools wiki文档

```python
Total: 55838.9 MB
41320.8 74.0% 74.0% 41320.8 74.0% updatewindow
9018.8 16.2% 90.2% 9018.8 16.2% inflateInit2_
1559.4 2.8% 92.9% 1559.4 2.8% os::malloc@907260
1556.4 2.8% 95.7% 1556.4 2.8% init
587.5 1.1% 96.8% 587.5 1.1% 00007fb480508a66
551.4 1.0% 97.8% 551.4 1.0% 00007f2b1d6f4a66
451.5 0.8% 98.6% 451.5 0.8% 00007fb48177e8e9
183.1 0.3% 98.9% 183.1 0.3% 00007fb46c1b6c91
128.2 0.2% 99.1% 128.2 0.2% 00007fb480508245
120.4 0.2% 99.4% 120.4 0.2% 00007f2b1d6f4245
111.0 0.2% 99.6% 9129.7 16.4% Java_java_util_zip_Inflater_init
102.8 0.2% 99.7% 102.8 0.2% 00007f2b1e96a8e9
74.2 0.1% 99.9% 74.2 0.1% readCEN
45.8 0.1% 100.0% 45.8 0.1% 00007f2b09392c91
7.5 0.0% 100.0% 7.5 0.0% 00007fb4805025b6
4.5 0.0% 100.0% 4.5 0.0% 00007fb48192162d
```

|.对于上述问题表现的jdk1.7.0_76不会出现
|

jdk1.7.0_80,jdk1.8.0_45以上明显出现。
用过pmap分析过 主要都是64m内存块较多， 查看过数据都是jar load相关的数据当时没引起注意
分析主要在updatewindow，Java_java_util_zip_Inflater_init ， 这个是gzip相关， 于是搜索相关资料
结论如下：
‘’’Note that the URLClassLoader was properly disposed of, with a call to close() first, and that the ServiceLoader’s reload() method was also called to flush it’s internal cache.
This is NOT specific to the ServletContainerInitializer, it happens with any class that is loaded via the ServiceLoader.
File handles remain to the *.jar until the app’s process is stopped.’’’
网上查找相关资料
jdk 问题[http://bugs.java.com/bugdatabase/view_bug.do?bug_id=8156014](http://bugs.java.com/bugdatabase/view_bug.do?bug_id=8156014)
jetty issue[https://github.com/eclipse/jetty.project/issues/231](https://github.com/eclipse/jetty.project/issues/231)
处理方法,以下其一
JDK降级到jdk7u76， 使用不会出现问题的JDK版本
等新版本jdk bug修复
jetty contributer 给出的方案 disable url caching wiki[https://github.com/eclipse/jetty.project/issues/575](https://github.com/eclipse/jetty.project/issues/575)
关于堆外内存：《[堆外内存 之 DirectByteBuffer 详解](https://www.jianshu.com/p/007052ee3773)》
DirectByteBuffer是Java用于实现堆外内存的一个重要类，我们可以通过该类实现堆外内存的创建、使用和销毁。
DirectByteBuffer该类本身还是位于Java内存模型的堆中。堆内内存是JVM可以直接管控、操纵。
而DirectByteBuffer中的unsafe.allocateMemory(size);是个一个native方法，这个方法分配的是堆外内存，通过C的malloc来进行分配的。分配的内存是系统本地的内存，并不在Java的内存中，也不属于JVM管控范围，所以在DirectByteBuffer一定会存在某种方式来操纵堆外内存。
在DirectByteBuffer的父类Buffer中有个address属性：
```python
public abstract class Buffer {
    /**
     * The characteristics of Spliterators that traverse and split elements
     * maintained in Buffers.
     */
    static final int SPLITERATOR_CHARACTERISTICS =
        Spliterator.SIZED | Spliterator.SUBSIZED | Spliterator.ORDERED;
    // Invariants: mark <= position <= limit <= capacity
    private int mark = -1;
    private int position = 0;
    private int limit;
    private int capacity;
    // Used only by direct buffers
    // NOTE: hoisted here for speed in JNI GetDirectBufferAddress
    long address;
```
address只会被直接缓存给使用到。之所以将address属性升级放在Buffer中，是为了在JNI调用GetDirectBufferAddress时提升它调用的速率。address表示分配的堆外内存的地址。
======================
## 堆外内存常配合使用System GC
这里说的堆外内存主要针对java.nio.DirectByteBuffer，这些对象的创建过程会通过Unsafe接口直接通过os::malloc来分配内存，然后将内存的起始地址和大小存到java.nio.DirectByteBuffer对象里，这样就可以直接操作这些内存。这些内存只有在DirectByteBuffer回收掉之后才有机会被回收，因此如果这些对象大部分都移到了old，但是一直没有触发CMS GC或者Full GC，那么悲剧将会发生，因为你的物理内存被他们耗尽了，因此为了避免这种悲剧的发生，通过-XX:MaxDirectMemorySize来指定最大的堆外内存大小，当使用达到了阈值的时候将调用System.gc来做一次full gc，以此来回收掉没有被使用的堆外内存，具体堆外内存是如何回收的，其原理机制又是怎样的，还是后面写篇详细的文章吧
《[JVM源码分析之SystemGC完全解读](https://www.cnblogs.com/xll1025/p/6517871.html)》

