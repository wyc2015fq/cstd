# 使用JMAP dump及分析dump文件 - z69183787的专栏 - CSDN博客
2018年06月28日 14:49:30[OkidoGreen](https://me.csdn.net/z69183787)阅读数：195
[https://blog.csdn.net/kevin_luan/article/details/8447896](https://blog.csdn.net/kevin_luan/article/details/8447896)
查看整个JVM内存状态 
jmap -heap [pid]
要注意的是在使用CMS GC 情况下，jmap -heap的执行有可能会导致JAVA 进程挂起
查看JVM堆中对象详细占用情况
jmap -histo [pid]
导出整个JVM 中内存信息
jmap -dump:format=b,file=文件名 [pid]
jhat是sun 1.6及以上版本中自带的一个用于分析JVM 堆DUMP 文件的工具，基于此工具可分析JVM HEAP 中对象的内存占用情况
jhat -J-Xmx1024M [file]
执行后等待console 中输入start HTTP server on port 7000 即可使用浏览器访问 IP：7000
eclipse Memory Analyzer
Eclipse 提供的一个用于分析JVM 堆Dump文件的插件。借助这个插件可查看对象的内存占用状况，引用关系，分析内存泄露等。
[http://www.eclipse.org/mat/](http://www.eclipse.org/mat/)
kill -3 [pid]
在Linux 上找到Java所在的进程号，然后执行以上命令，线程的相关信息就输出到console
jstack
jstack 是sun JDK 自带的工具，通过该工具可以看到JVM 中线程的运行状况，包括锁等待，线程是否在运行
执行 jstack [pid] ,线程的所有堆栈信息
"http-8080-10" daemon prio=10 tid=x0a949bb60 nid=0x884  waiting for monitor entry [...]
"http-8080-10" 这个线程处于等待状态。 waiting for monitor entry 如果在连续几次输出线程堆栈信息都存在于同一个或多个线程上时，则说明系统中有锁竞争激烈，死锁，或锁饿死的想象。
“http-8080-11” daemon prio=10 tix=xxx nid=xxx in object.wait() [...]
 java.lang.Thread.State:waiting (on object monitor)
该表示http-8080-11的线程处于对象的Wait 上，等待其他线程的唤醒，这也是线程池的常见用法。
“Low Memory Detector”daemon prio=10 tix=xx nid=xxx runnable [...] java.lang.Thread.State:runnable
表示“Low Memory Detector” 的线程处于Runable状态，等待获取ＣＰＵ的使用权.
