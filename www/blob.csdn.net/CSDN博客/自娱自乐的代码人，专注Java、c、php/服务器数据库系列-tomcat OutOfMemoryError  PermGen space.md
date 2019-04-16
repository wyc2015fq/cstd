# 服务器数据库系列 - tomcat OutOfMemoryError: PermGen space - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年07月18日 18:50:11[initphp](https://me.csdn.net/initphp)阅读数：863
个人分类：[服务器数据库系列](https://blog.csdn.net/initphp/article/category/2609107)









PermGen space 

PermGen space的全称是Permanent Generation space,是指内存的永久保存区域OutOfMemoryError: PermGen space从表面上看就是内存益出，解决方法也一定是加大内存。说说为什么会内存益出：这一部分用于存放Class和Meta的信息,Class在被 Load的时候被放入PermGen space区域，它和和存放Instance的Heap区域不同,GC(Garbage Collection)不会在主程序运行期对PermGen space进行清理，所以如果你的APP会LOAD很多CLASS的话,就很可能出现PermGen
 space错误。这种错误常见在web服务器对JSP进行pre compile的时候。 如果你的WEB APP下都用了大量的第三方jar, 其大小 超过了jvm默认的大小(4M)那么就会产生此错误信息了。 

解决方法： 手动设置MaxPermSize大小 

改正方法：-Xms256m -Xmx256m -XX:MaxNewSize=256m -XX:MaxPermSize=256m 



修改TOMCAT_HOME/bin/catalina.sh 

JAVA_OPTS="-server -XX:PermSize=64M -XX:MaxPermSize=128m 

建议：将相同的第三方jar文件移置到tomcat/shared/lib目录下，这样可以达到减少jar 文档重复占用内存的目的。 




Sun文档是这样解释的：

java.lang.OutOfMemoryError: PermGen space

The detail message PermGen space indicates that the permanent generation is full. The permanent generation is the area of the heap where class and method objects are stored. If an application loads a very large number of classes, then the size of the permanent
 generation might need to be increased using the -XX:MaxPermSize option.

Interned java.lang.String objects are also stored in the permanent generation. The java.lang.String class maintains a pool of strings. When the intern method is invoked, the method checks the pool to see if an equal string is already in the pool. If there
 is, then the intern method returns it; otherwise it adds the string to the pool. In more precise terms, the java.lang.String.intern method is used to obtain the canonical representation of the string; the result is a reference to the same class instance that
 would be returned if that string appeared as a literal. If an application interns a huge number of strings, the permanent generation might need to be increased from its default setting.

When this kind of error occurs, the text String.intern or ClassLoader.defineClass might appear near the top of the stack trace that is printed.

The jmap -permgen command prints statistics for the objects in the permanent generation, including information about internalized String instances. See 2.6.4 Getting Information on the Permanent Generation.

下面是某人遇到的问题：

SUN JDK+Tomcat 5.5.20运行服务的时候遇到问题，服务器跑几天后就会挂掉，并报java.lang.OutOfMemoryError: PermGen space异常。

发现很多人把问题归因于： spring,hibernate,tomcat，因为他们动态产生类,导致JVM中的permanent heap溢出 。然后解决方法众说纷纭，有人说升级 tomcat版本到最新甚至干脆不用tomcat。还有人怀疑spring的问题，在spring论坛上讨论很激烈，因为spring在AOP时使用CBLIB会动态产生很多类。




但问题是为什么这些王牌的开源会出现同一个问题呢，那么是不是更基础的原因呢？tomcat在Q&A很隐晦的回答了这一点。（Why does the memory usage increase when I redeploy a web application? Because the Classloader (and the Class objects it loaded) cannot be recycled. They are stored in the permanent heap generation
 by the JVM, and when you redepoy a new class loader is created, which loads another copy of all these classes. This can cause OufOfMemoryErrors eventually. ） 

于是有人对更基础的JVM做了检查，发现了问题的关键。原来SUN 的JVM把内存分了不同的区，其中一个就是permanent区用来存放用得非常多的类和类描述。本来SUN设计的时候认为这个区域在JVM启动的时候就固定了，但他没有想到现在动态会用得这么广泛。而且这个区域有特殊的垃圾收回机制，现在的问题是动态加载类到这个区域后，gc根本没办法回收！

对这个bug最彻底的解决办法就是不要用SUN的JDK，而改用BEA的 JRokit.



在tomcat中redeploy时出现outofmemory的错误. 




可以有以下几个方面的原因: 




１,使用了proxool,因为proxool内部包含了一个老版本的cglib. 




2, log4j,最好不用,只用common-logging 




3, 老版本的cglib,快点更新到最新版。 




４，更新到最新的hibernate3.2 

3、 这里以tomcat环境为例，其它WEB服务器如jboss,weblogic等是同一个道理。 



二、java.lang.OutOfMemoryError: Java heap space 

Heap size 设置 

JVM堆的设置是指java程序运行过程中JVM可以调配使用的内存空间的设置.JVM在启动的时候会自动设置Heap size的值， 

其初始空间(即-Xms)是物理内存的1/64，最大空间(-Xmx)是物理内存的1/4。可以利用JVM提供的-Xmn -Xms -Xmx等选项可 

进行设置。Heap size 的大小是Young Generation 和Tenured Generaion 之和。 

提示：在JVM中如果98％的时间是用于GC且可用的Heap size 不足2％的时候将抛出此异常信息。 

提示：Heap Size 最大不要超过可用物理内存的80％，一般的要将-Xms和-Xmx选项设置为相同，而-Xmn为1/4的-Xmx值。 

解决方法：手动设置Heap size 

修改TOMCAT_HOME/bin/catalina.sh 

在“echo "Using CATALINA_BASE:   $CATALINA_BASE"”上面加入以下行： 

JAVA_OPTS="-server -Xms800m -Xmx800m   -XX:MaxNewSize=256m" 




三、实例，以下给出1G内存环境下java jvm 的参数设置参考： 




JAVA_OPTS="-server -Xms800m -Xmx800m  -XX:PermSize=64M -XX:MaxNewSize=256m -XX:MaxPermSize=128m -Djava.awt.headless=true " 




四、java.lang.OutOfMemoryError: Direct buffer memory




可以设置-XX:MaxDirectMemorySize







针对Tomcat，如果Tomcat下面有多个应用，尽可能的把lib下共用的jar文件放到Tomcat的lib下，发布速度和运行速度上也有所提升。



