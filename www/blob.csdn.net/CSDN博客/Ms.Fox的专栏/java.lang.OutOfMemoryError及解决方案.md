# java.lang.OutOfMemoryError及解决方案 - Ms.Fox的专栏 - CSDN博客
2018年11月27日 19:41:08[lulu-lu](https://me.csdn.net/smbluesky)阅读数：51
# [java.lang.OutOfMemoryError及解决方案](https://www.cnblogs.com/penghongwei/p/3603326.html)
主要有3种比较常见的OutOfMemory Error：
- java.lang.OutOfMemoryError: Java heap space
- java.lang.OutOfMemoryError: PermGen space
- java.lang.OutOfMemoryError: GC overhead limit exceeded
# **1. java.lang.OutOfMemoryError: Java heap space**
     Java heap space，Java应用程序创建的对象存放在这片区域，垃圾回收（Garbage Collection）也发生在这块区域。通常一些比较“重型”的操作可能会导致该异常，比如：需要创建大量的对象，层次比较深的递归操作等。
     解决方案有两种，一是优化应用，找到消耗大量内存的地方，然后优化代码或者算法。这种方式比较推荐，但是难度比较大，尤其是在产品环境中出现这种问题，开发人员不能很好的重现问题。第二种方案是提升Java heap size，这种方式虽然感觉有点治标不治本，但是可行性非常高，操作简单。
     对于一般的应用，采用如下方式即可（数字根据自己的需要调整）：
```
1 -Xms<size> - Set initial Java heap size
2 -Xmx<size> - Set maximum Java heap size
3  
4 java -Xms512m -Xmx1024m JavaApp
```
     如果是在tomcat中，出现的这种问题，解决办法是在{tomcat_dir}/bin/catalina.bat中找到如下几行：
```
1 set MAINCLASS=org.apache.catalina.startup.Bootstrap
2 set ACTION=start
3 set SECURITY_POLICY_FILE=
4 set DEBUG_OPTS=
5 set JPDA=
```
     在后面加上一行（数字根据自己的需要调整）：
`1 set CATALINA_OPTS=-Xms512m   -Xmx512m`
# **2. java.lang.OutOfMemoryError: PermGen space**
     Perm Gen Size（Permanent Generation Size），用来存储被加载的类的定义（class definition）和元数据（metadata），比如：Class Object和Method Object等。这是内存中的一块永久保存区域，JVM的垃圾回收不会触及这块区域。通常在加载一个非常大的项目的时候才会出现该异常。
     对于一般的应用，采用如下方式即可（数字根据自己的需要调整）：
```
1 -XX:PermSize<size> - Set initial PermGen Size.
2 -XX:MaxPermSize<size> - Set the maximum PermGen Size.
3  
4 java -XX:PermSize=64m -XX:MaxPermSize=128m JavaApp
```
     如果是在tomcat中出现这个问题，解决办法是在{tomcat_dir}/bin/catalina.bat中添加如下一行：
`1 set CATALINA_OPTS=-server -Xms256m -Xmx1024m -XX:PermSize=512m -XX:MaxPermSize=512m`
# **3. java.lang.OutOfMemoryError: GC overhead limit exceeded**
     这个错误会出现在这个场景中：GC占用了多余98%（默认值）的CPU时间却只回收了少于2%（默认值）的堆空间。目的是为了让应用终止，给开发者机会去诊断问题。**一般是应用程序在有限的内存上创建了大量的临时对象或者弱引用对象，从而导致该异常。**虽然加大内存可以暂时解决这个问题，但是还是强烈建议去优化代码，后者更加有效。
     首先，你可以关闭JVM这个默认的策略：
`1 java -XX:-UseGCOverheadLimit JavaApp`
      其次，你也可以尝试去加大Heap Size：
`1 java -Xmx512m JavaApp`
     注意：在修改Tomcat的catalina.bat(*.sh)中的内容时，网上有很多都是说直接修改JAVA_OPTS，按照Apache官方的说法是：
**Note: Do not use JAVA_OPTS to specify memory limits. You do not need much memory for a small process that is used to stop Tomcat. Those settings belong to CATALINA_OPTS.**
- **Reference**
- http://wiki.apache.org/tomcat/OutOfMemory
- http://www.mkyong.com/java/find-out-your-java-heap-memory-size/
