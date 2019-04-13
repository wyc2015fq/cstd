
# eclipse.ini 内存设置 - 阳光岛主 - CSDN博客

2013年07月23日 23:28:25[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：6094


**Ubuntu 系统下，Eclipse 配置文件：**
vi ~/eclipse/eclipse.ini
-vmargs -Xms128M -Xmx512M -XX:PermSize=64M -XX:MaxPermSize=128M
这里有几个问题：
1. 各个参数的含义什么？
2. 为什么有的机器将-Xmx和-XX:MaxPermSize都设置为512M之后Eclipse可以启动，而有些机器无法启动？
3. 为何将上面的参数写入到eclipse.ini文件Eclipse没有执行对应的设置？
**1. 各个参数的含义什么？**
参数中-vmargs的意思是设置JVM参数，所以后面的其实都是JVM的参数了，我们首先了解一下JVM内存管理的机制，然后再解释每个参数代表的含义。
**堆(Heap)和非堆(Non-heap)内存**
按照官方的说法：“Java 虚拟机具有一个堆，堆是运行时数据区域，所有类实例和数组的内存均从此处分配。堆是在 Java 虚拟机启动时创建的。在JVM中堆之外的内存称为非堆内存(Non-heap memory)”。
可以看出JVM主要管理两种类型的内存：堆和非堆
简单来说：堆就是Java代码可及的内存，是留给开发人员使用的；非堆就是JVM留给自己用的，所以方法区、JVM内部处理或优化所需的内存(如JIT编译后的代码缓存)、每个类结构(如运行时常数池、字段和方法数据)以及方法和构造方法的代码都在非堆内存中。
**堆内存分配**
JVM初始分配的内存由-Xms指定，默认是物理内存的1/64；JVM最大分配的内存由-Xmx指定，默认是物理内存的1/4。默认空余堆内存小于40%时，JVM就会增大堆直到-Xmx的最大限制；空余堆内存大于70%时，JVM会减少堆直到-Xms的最小限制。因此服务器一般设置-Xms、-Xmx相等以避免在每次GC 后调整堆的大小。
**非堆内存分配**
JVM使用-XX:PermSize设置非堆内存初始值，默认是物理内存的1/64；由XX:MaxPermSize设置最大非堆内存的大小，默认是物理内存的1/4。

**JVM内存限制(最大值)**
首先，JVM内存限制于实际的最大物理内存(内存条)，假设物理内存无限大的话，JVM内存的最大值跟操作系统有很大的关系。简单的说就32位处理器虽然可控内存空间有4GB，但是具体的操作系统会给一个限制，这个限制一般是2GB-3GB（一般来说Windows系统下为1.5G-2G，Linux系统下为2G-3G），而64bit以上的处理器就不会有限制了。

**2. 为什么有的机器我将-Xmx和-XX:MaxPermSize都设置为512M之后Eclipse可以启动，而有些机器无法启动？**
通过上面对JVM内存管理的介绍我们已经了解到JVM内存包含两种：堆内存和非堆内存，另外JVM最大内存首先取决于实际的物理内存和操作系统。所以说设置VM参数导致程序无法启动，主要有以下几种原因：
1) 参数中-Xms的值大于-Xmx，或者-XX:PermSize的值大于-XX:MaxPermSize；
2) -Xmx的值和-XX:MaxPermSize的总和超过了JVM内存的最大限制，比如当前操作系统最大内存限制，或者实际的物理内存等等。说到实际物理内存这里需要说明一点的是，如果你的内存是1024MB，但实际系统中用到的并不可能是1024MB，因为有一部分被硬件占用了。

**3. 为何将上面的参数写入到eclipse.ini文件，Eclipse没有执行对应的设置？**
为什么同样的参数在快捷方式或者命令行中有效而在eclipse.ini文件中是无效的呢？这是因为我们没有遵守eclipse.ini文件的设置规则：
参数形如“项 值”这种形式，中间有空格的需要换行书写，如果值中有空格的需要用双引号包括起来。比如我们使用-vm C:\Java\jre1.6.0\bin\javaw.exe参数设置虚拟机，在eclipse.ini文件中要写成这样：
-vm
C:\Java\jre1.6.0\bin\javaw.exe
按照上面所说的，最后参数在eclipse.ini中可以写成这个样子：
-vmargs
-Xms128M
-Xmx512M
-XX:PermSize=64M
-XX:MaxPermSize=128M
实际运行的结果可以通过Eclipse中“Help”-“About Eclipse SDK”窗口里面的“Configuration Details”按钮进行查看。
另外需要说明的是，Eclipse压缩包中自带的eclipse.ini文件内容是这样的：
-showsplash
org.eclipse.platform
--launcher.XXMaxPermSize
256m
-vmargs
-Xms40m
-Xmx256m
其中–launcher.XXMaxPermSize（注意最前面是两个连接线）跟-XX:MaxPermSize参数的含义基本是一样的，我觉得唯一的区别就是前者是eclipse.exe启动的时候设置的参数，而后者是eclipse所使用的JVM中的参数。其实二者设置一个就可以了，所以这里可以把–launcher.XXMaxPermSize和下一行使用\#注释掉。


**4. 其他的启动参数。**
如果你有一个双核的CPU，也许可以尝试这个参数:
`-XX:+UseParallelGC`
让GC可以更快的执行（只是JDK 5里对GC新增加的参数）

**eclipse.ini 示例**（16G物理内存）**：**

```python
-startup
plugins/org.eclipse.equinox.launcher_1.3.0.v20120522-1813.jar
--launcher.library
plugins/org.eclipse.equinox.launcher.gtk.linux.x86_64_1.1.200.v20120913-144807
-product
org.eclipse.epp.package.jee.product
--launcher.defaultAction
openFile
-showsplash
org.eclipse.platform
--launcher.XXMaxPermSize
256m
--launcher.defaultAction
openFile
-vmargs
-Dosgi.requiredJavaVersion=1.5
-Dhelp.lucene.tokenizer=standard
-XX:PermSize=64m
-XX:MaxPermSize=512m
-XX:+UseParallelGC
-Xms512m
-Xmx2048m
```
**eclipse错误日志目录：**找到你所指定的工作空间->.metadata->.log


**参考推荐：**
[eclipse.ini内存设置](http://www.blogjava.net/wilesun/archive/2007/12/15/167915.html)
[eclipse.ini/myeclipse.ini中配置参数 -Xms，-Xmx，-PerSize](http://hi.baidu.com/sunboy_2050/item/3878e644aaedcd2a11ee1ef1)
[Eclipse.ini参数设置（Maven Integration for Eclipse JDK Warning）](http://springsfeng.iteye.com/blog/1097528)
[Java 内存模型及GC原理](http://blog.csdn.net/sunboy_2050/article/details/6252552)（推荐）
[JVM调优总结 -Xms -Xmx -Xmn -Xss](http://unixboy.iteye.com/blog/174173)



