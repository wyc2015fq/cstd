# 利用MemoryAnalyzer进行OutOfMemoryError的诊断分析 - 零度的博客专栏 - CSDN博客
2016年07月18日 15:43:37[零度anngle](https://me.csdn.net/zmx729618)阅读数：924
                
这篇帖子只介绍利用MemoryAnalyzer进行简单的JVM的堆的分析，至于JVM的内部结构是怎么样的，这里不进行分析。好吧，废话不多说；首先如果我们要分析JVM某个时刻的Heap的对象分配情况，我们就必须要dump这个时刻的JVM的heap（堆）；有以下几个办法进行dump某个时刻JVM的heap内容：
         1、 使用$JAVA_HOME/bin/jmap -dump来触发， 
                eg：jmap-dump:format=b,file=/home/longhao/heamdump.out <pid>
         2、 使用$JAVA_HOME/bin/jcosole中的MBean，到MBean>com.sun.management>HotSpotDiagnostic>操作>dumpHeap中，点击 dumpHeap按钮。生成的dump文件在   java应用的根目录下面。
         3、在应用启动时配置相关的参数 -XX:+HeapDumpOnOutOfMemoryError，当应用抛出OutOfMemoryError时生成dump文件。
         4、使用hprof。启动虚拟机加入-Xrunhprof:head=site，会生成java.hprof.txt文件。该配置会导致jvm运行非常的慢，不适合生产环境。
**利用MemoryAnalyzer进行Heap分析**
        去eclipse官网上去下载MemoryAnalyzer，可以下载非插件版的，这样MemoryAnalyzer运行起来比较快，如果是eclipse插件版进行可能会导致eclipse卡死。本人下载的版本是MemoryAnalyzer-1.2.0.20120530-win32.win32.x86_64。
 一、Java代码样例
**[java]**[view
 plain](http://blog.csdn.net/zapldy/article/details/7727572#)[copy](http://blog.csdn.net/zapldy/article/details/7727572#)
- package org.ph.javaee.javaheap;  
- 
- 
- import java.util.Map;  
- 
- import java.util.HashMap;  
- 
- 
- /**
- 
-  * JVMOutOfMemoryErrorSimulator
- 
-  * 
- 
-  * @author PH
- 
-  * 
- 
-  */
- 
- publicclass JVMOutOfMemoryErrorSimulator {  
- 
- privatefinalstaticint NB_ITERATIONS = 500000;  
- 
- // ~1 KB data footprint
- 
- privatefinalstatic String LEAKING_DATA_PREFIX = "datadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadatadata";  
- 
- // Map used to stored our leaking String instances
- 
- privatestatic Map<String, String> leakingMap;  
- 
- static {  
- 
-               leakingMap = new HashMap<String, String>();  
- 
-        }  
- 
- 
- publicstaticvoid main(String[] args) {  
- 
-               System.out.println("JVM OutOfMemoryError Simulator 1.0");  
- 
-               System.out.println("Author: Pierre-Hugues Charbonneau");  
- 
-               System.out.println("http://javaeesupportpatterns.blogspot.com/");  
- 
- try {  
- for (int i = 0; i < NB_ITERATIONS; i++) {  
-                            String data = LEAKING_DATA_PREFIX + i;  
- 
- // Add data to our leaking Map data structure...
- 
-                            leakingMap.put(data, data);  
-                      }  
- 
-               } catch (Throwable any) {  
- 
- if (any instanceof java.lang.OutOfMemoryError) {  
- 
-                             System.out.println("OutOfMemoryError triggered! "
- 
-                                          + any.getMessage() + " [" + any + "]");  
-                      } else {  
- 
-                            System.out.println("Unexpected Exception! " + any.getMessage()  
- 
-                                          + " [" + any + "]");  
-                      }  
-               }  
-               System.out.println("simulator done!");  
- 
-        }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
 二、设置JVM启动参数
         在此我们把JVM堆的最大内存设置为512m，并且让程序运行过程中出现内存溢出的时候会dump当时的JVM对内存的内容，所以需要加上XX:+HeapDumpOnOutOfMemoryError参数；因此按照以下步骤在eclipse中加入启动参数
![](https://img-my.csdn.net/uploads/201207/09/1341764084_1639.png)
![](https://img-my.csdn.net/uploads/201207/09/1341764117_7592.png)
 三、运行程序
        运行程序，然后观察控制台的输出，输出结果如下：
![](https://img-my.csdn.net/uploads/201207/09/1341764167_9241.png)
        从以上的输出结果来看，此程序已经抛出了OutOfMemoryError了，并且生成了一个heap文件，文件名为java_pid3880.hprof，下一步我们就可以拿这个文件在MemoryAnalyzer分析了。
 四、导入heap文件分析
        导入后按照以下步骤执行进行内存泄露的可疑分析；
![](https://img-my.csdn.net/uploads/201207/09/1341764243_2741.png)
![](https://img-my.csdn.net/uploads/201207/09/1341764288_3437.png)
![](https://img-my.csdn.net/uploads/201207/09/1341764320_3690.png)
![](https://img-my.csdn.net/uploads/201207/09/1341764363_3311.png)
 鼠标点击红色的部分会出现菜单，选中菜单的第一行的List objects > With Incoming references，就会出现以下界面
![](https://img-my.csdn.net/uploads/201207/09/1341764400_7859.png)
        此图展现了HashMap$Entry被引用的路径，HashMap$Entry被一个java.util.HashMap的table实例变量引用，而这个HashMap又被JVMOutOfMemberyErrorSimulator类变量leakingMap引用，所以通过这些路径就很容易找到是哪段代码导致的内存溢出。
        通过以上分析我们就发现，类JVMOutOfMemberyErrorSimulator的leakingMap变量的内容太大导致了内存溢出，所以这样就能很快定位到问题。
        其中注意Shallow vs. Retained Heap的区别，这里不进行解释，点击MemoryAnalyzer的Help菜单里面的HelpContents就可以查找到
        这里解释下Java Local，JVM里面的类变量，实例变量的名字在JVM里面都有记录，而局部变量是没有记录的，所以Java Local在这里就代表局部变量。
       总结：以上的这些分析方法是入门级别的，现实中的OOM分析肯定比这更复杂，本人就曾经遇到过很多诡异的OOM。但在一般情况下，如果出现OOM，那么我们肯定需要对JVM的heap进行分析，这篇帖子是一个很好的思维方法；当然你也可以利用其他的工具进行heap分析，但思路大概都差不多；MemoryAnalyzer是一个不错的工具，里面有很多的小工具给我分析，可以花点时间看看。
