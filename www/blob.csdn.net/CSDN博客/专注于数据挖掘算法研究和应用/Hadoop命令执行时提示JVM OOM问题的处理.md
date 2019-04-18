# Hadoop命令执行时提示JVM OOM问题的处理 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年04月25日 16:17:20[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3057
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









                
1、问题：执行Hadoop命令时提示java.lang.OutOfMemoryError: unable to create new native thread

         就是OOM问题。



2、分析：

   为什么会抛OOM Error呢？因为java线程池启动太多会造成OOM。

   在java语言里， 当你创建一个线程的时候，虚拟机会在JVM内存创建一个Thread对象同时创建一个操作系统线程，而这个系统线程的内存用的不是JVMMemory，而是系统中剩下的内存(MaxProcessMemory - JVMMemory - ReservedOsMemory)。




   能创建的线程数的具体计算公式如下：(MaxProcessMemory - JVMMemory - ReservedOsMemory) / (ThreadStackSize) = Number of threads

   1）MaxProcessMemory   进程最大的寻址空间，不能超过虚拟内存和物理内存的总和。Linux 64 bit一般是3G；      

   2）JVMMemory         JVM内存，Heap + PermGen，即堆内存和永久代内存和（不包括本地内存）。

   3）ReservedOsMemory  JVM的本地内存，Native heap，即JNI调用方法所占用的内存。

   4）ThreadStackSize   线程栈的大小，JDK5.0以后每个线程堆栈大小默认为1M,以前每个线程堆栈大小为256K；可以通过jvm参数-Xss来设置；注意-Xss是jvm的非标准参数，不强制所有平台的jvm都支持。




   如果程序中有bug，导致创建大量不需要的线程或者线程没有及时回收，那么必须解决这个bug，修改参数是不能解决问题的。

   如果程序需要大量的线程，现有的设置不能达到要求，那么可以通过修改MaxProcessMemory、JVMMemory、ThreadStackSize这三个因素，来增加能创建的线程数：

   1）MaxProcessMemory 使用64位操作系统

   2）JVMMemory   减少JVMMemory的分配

   3）ThreadStackSize  减小单个线程的栈大小




   具体研究JVM内存设置。


3、解决：因为是Hadoop客户端执行命令遇到问题，所以JVM内存设置是一方面，还可通过ulimit -a设置。



```
core file size          (blocks, -c) 0
data seg size           (kbytes, -d) unlimited
scheduling priority             (-e) 0
file size               (blocks, -f) unlimited
pending signals                 (-i) 514830
max locked memory       (kbytes, -l) 64
max memory size         (kbytes, -m) unlimited
open files                      (-n) 32768
pipe size            (512 bytes, -p) 8
POSIX message queues     (bytes, -q) 819200
real-time priority              (-r) 0
stack size              (kbytes, -s) 10240
cpu time               (seconds, -t) unlimited
max user processes              (-u) 4096
virtual memory          (kbytes, -v) unlimited
file locks                      (-x) unlimited
```





   重点设置两个参数：

   1)max user processes：ulimit -u 65536 

   2)open files：ulimit -n 32768

   可以通过\etc\profile设置全局生效，也可以在具体会话中临时执行命令生效。




