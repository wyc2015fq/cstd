# java.lang.OutOfMemoryError: Java heap space错误及处理办法(收集整理、转) - xqhrs232的专栏 - CSDN博客
2012年03月28日 13:21:06[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：854标签：[java																[jvm																[eclipse																[initialization																[outofmemoryerror																[tomcat](https://so.csdn.net/so/search/s.do?q=tomcat&t=blog)](https://so.csdn.net/so/search/s.do?q=outofmemoryerror&t=blog)](https://so.csdn.net/so/search/s.do?q=initialization&t=blog)](https://so.csdn.net/so/search/s.do?q=eclipse&t=blog)](https://so.csdn.net/so/search/s.do?q=jvm&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/jxzxm1_2/article/details/2499751](http://blog.csdn.net/jxzxm1_2/article/details/2499751)
以下是从网上找到的关于堆空间溢出的错误解决办法：
java.lang.OutOfMemoryError: Java heap space 
===================================================
使用Java程序从数据库中查询大量的数据时出现异常:
java.lang.OutOfMemoryError: Java heap space
在JVM中如果98％的时间是用于GC且可用的 Heap size 不足2％的时候将抛出此异常信息。
JVM堆的设置是指java程序运行过程中JVM可以调配使用的内存空间的设置.
JVM在启动的时候会自动设置Heap size的值，其初始空间(即-Xms)是物理内存的1/64，最大空间(-Xmx)是物理内存的1/4。可以利用JVM提供的-Xmn -Xms -Xmx等选项可进行设置。
例如：java -jar -Xmn16m -Xms64m -Xmx128m MyApp.jar
如果Heap Size设置偏小，除了这些异常信息外，还会发现程序的响应速度变慢了。GC占用了更多的时间，而应用分配到的执行时间较少。
Heap Size 最大不要超过可用物理内存的80％，一般的要将-Xms和-Xmx选项设置为相同，而-Xmn为1/4的-Xmx值。
Heap size的 -Xms -Xmn 设置不要超出物理内存的大小。否则会提示“Error occurred during initialization of VM Could not reserve enough space for object heap”。
==========================================================
经过一个晚上的努力终于完成了一个文件替换指定字符串的程序，但是由于我要替换的全站程序html文件太多，所以eclipse下边老是在一个目录结束后报出java.lang.OutOfMemoryError: Java heap space的异常，然后就崩溃了。
我一想肯定是频繁操作造成来不及回收，于是在每个循环之后加上一个Thread.sleep（1000）,发现还是到那个目录下就死掉，于是把1000改成5000，还是到那里死掉，我想可能不是来不及回收这么简单，或许sun 的JVM里边刚好对于这种情况不释放也有可能。
接着我又把启动的参数添上一个 -Xmx256M,这回就可以了。
想一想，还是对于垃圾回收的原理不太了解，就在网上查了一下，发现了几篇不错的文章。
[http://java.ccidnet.com/art/3539/20060314/476073_1.html](http://java.ccidnet.com/art/3539/20060314/476073_1.html)
[http://www.pconline.com.cn/pcedu/empolder/gj/java/0509/701281.html](http://www.pconline.com.cn/pcedu/empolder/gj/java/0509/701281.html)
还有:Java堆的管理—垃圾回收提到一下几点，很不错，或许可以作为写程序时候的准则：
　　（1）不要试图去假定垃圾收集发生的时间，这一切都是未知的。比如，方法中的一个临时对象在方法调用完毕后就变成了无用对象，这个时候它的内存就可以被释放。
　　（2）Java中提供了一些和垃圾收集打交道的类，而且提供了一种强行执行垃圾收集的方法--调用System.gc()，但这同样是个不确定的方法。Java 中并不保证每次调用该方法就一定能够启动垃圾收集，它只不过会向JVM发出这样一个申请，到底是否真正执行垃圾收集，一切都是个未知数。
　　（3）挑选适合自己的垃圾收集器。一般来说，如果系统没有特殊和苛刻的性能要求，可以采用JVM的缺省选项。否则可以考虑使用有针对性的垃圾收集器，比如增量收集器就比较适合实时性要求较高的系统之中。系统具有较高的配置，有比较多的闲置资源，可以考虑使用并行标记/清除收集器。
　　（4）关键的也是难把握的问题是内存泄漏。良好的编程习惯和严谨的编程态度永远是最重要的，不要让自己的一个小错误导致内存出现大漏洞。
　　（5）尽早释放无用对象的引用。
大多数程序员在使用临时变量的时候，都是让引用变量在退出活动域(scope)后，自动设置为null，暗示垃圾收集器来收集该对象，还必须注意该引用的对象是否被监听，如果有，则要去掉监听器，然后再赋空值。
就是说,对于频繁申请内存和释放内存的操作,还是自己控制一下比较好,但是System.gc()的方法不一定适用，最好使用finallize强制执行或者写自己的finallize方法。
================================================
tomcat
遇到TOMCAT出错：java.lang.OutOfMemoryError: Java heap space，于是查了资料，找到了解决方法：
If Java runs out of memory, the following error occurs:
Exception in thread "main" java.lang.OutOfMemoryError: Java heap space
Java heap size can be increased as follows:
java -Xms<initial heap size> -Xmx<maximum heap size>
Defaults are:
java -Xms32m -Xmx128m
如果你用win
/tomcat/bin/catalina.bat 加上下面的命令:
set JAVA_OPTS=-Xms32m -Xmx256m
如果你用unix/linux
/tomcat/bin/catalina.sh 加上下面的命令:
JAVA_OPTS="-Xms32m -Xmx256m"
=========================================
原因：
使用Java程序从数据库中查询大量的数据时出现异常:
java.lang.OutOfMemoryError: Java heap space 
在JVM中如果98％的时间是用于GC且可用的 Heap size 不足2％的时候将抛出此异常信息。
JVM堆的设置是指java程序运行过程中JVM可以调配使用的内存空间的设置.JVM在启动的时候会自动设置Heap size的值，其初始空间(即-Xms)是物理内存的1/64，最大空间(-Xmx)是物理内存的1/4。可以利用JVM提供的-Xmn -Xms -Xmx等选项可进行设置。
解决办法：
tomcat_home/bin下catalina.bat(win)或catalina.sh(linux)执行代码前加上：
set JAVA_OPTS=%JAVA_OPTS% -Xms128m -Xmx512m
=============================================================
eclipse java.lang.OutOfMemoryError: Java heap space 解决方案 
=========================================================== 
eclipse java.lang.OutOfMemoryError: Java heap space 解决方案
eclipse 有启动参数里设置jvm大小，因为eclipse运行时自己也需要jvm,所以eclipse.ini里设置的jvm大小不是具体某个程序运行时所用jvm的大小，这和具体程序运行的jvm大小无关。
那么怎么才能设置某个程序的jvm大小呢（当然控制台运行的话不会存在这个问题，如：java -Xms256m -Xmx1024m classname，这样就可以把当前程序的jvm大小给设定）？
因为eclipse里默认的一个程序的jvm配置为：-Xms8m -Xmx128m，所以我们的处理耗内存比较大时需要手动调整一下，以便不会内存溢出。具体的设置方法为：
选中被运行的类，点击菜单‘Run as ->Open Run Dialog...’，选择(x)=Argument标签页下的vm arguments框里输入 -Xmx512m, 保存运行就ok了
=======================================================
Eclipse中java.lang.OutOfMemoryError: Java heap space
错误2007-06-12 11:53java.lang.OutOfMemoryError: Java heap space错误
在tomcat in的catalina.bat里加 
set JAVA_OPTS=-Xms256m -Xmx512m -Djava.awt.headless=true 
256和512分别是MIN和MAX内存 
在eclipse中的 
windows->preferences..->tomcat->jvm..->jvm那一个文本框里，添加-Xms256m -Xmx512m 
这样就可以了！
==================================
Eclipse中VM设定方法 - [] - Tag：java.lang.OutOfMemoryError: Java heap space
点击右键，选择run...，弹出窗口如下图，在vm输入框中填写合适的内存值即可。
==================================
eclipse中，黄线是警示。 
private Log log = LogFactory.getLog(AddCateAction.class);//小黄线在log上。 
这里的黄线是指你定义了私有变量log,但没有用到这个变量。 
v.add(o); //这时eclipse下面出现了一个小黄线，表示仍然被引用？ 
这里是指add方法是不推荐的方法。
OutOfMemoryError和怀疑的引用没有关系，可能是循环创建对象导致java内存不足。
用参数指定java虚拟机的内存. 
java -Xms256m -Xmx1024m -XX:MaxPermSize=256M
========================================================
2、如果在启动过程中出现内存溢出问题，抛出类似如下异常信息：
java.lang.OutOfMemoryError: Java heap space
可以尝试办法：
A.修改Tomcat/bin/catalina.bat,添加如下内容
set JAVA_OPTS=-Xms256m -Xmx512m -Djava.awt.headless=true [-XX:MaxPermSize=128M]
B.eclipse->windows->preferences..->tomcat->jvm..->jvm文本框里，添加-Xms256m -Xmx512m
C.eclipse->preference->java->instal jres->edit,增加参数：-Xms256m -Xmx512m
参考原因：JVM中如果98％的时间是用于GC且可用的, Heap size不足2％的时候将抛出此异常信息。
JVM堆的设置是指java程序运行过程中JVM可以调配使用的内存空间的设置.JVM在启动的时候会自动设置Heap size的值，其初始空间(即-Xms)是物理内存的1/64，最大空间(-Xmx)是物理内存的1/4。
可以利用JVM提供的-Xmn -Xms -Xmx等选项可进行设置。Heap Size 最大不要超过可用物理内存的80％，一般的要将-Xms和-Xmx选项设置为相同，而-Xmn为1/4的-Xmx值。
Heap size的 -Xms -Xmn 设置不要超出物理内存的大小。否则会提示“Error occurred during initialization of VM Could not reserve enough space for object heap”。
==================================================================
我的问题原因：
使用Spring+Hibernate从数据库中读取大量数据，使用了单例模式的AppContext的getBean，内存中缓存了大量的Entity Bean……
目前仍没有有效解决办法，通过加大JVM内存大小，以及修改数据读取方式(如：只读取有用信息)缓解了一下。
修改数据读取方式：
例如一个EntityBean有如下字段：id, name, age, address, description。如果程序中只用到了此Bean的id和name字段，那么从数据库中只用读此两字段，而不是读取整个bean以及关联的Bean。
