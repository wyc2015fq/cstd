# JDK工具 java命令详解 - xqhrs232的专栏 - CSDN博客
2012年03月28日 13:32:43[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：577标签：[java																[jdk																[工具																[class																[jni																[search](https://so.csdn.net/so/search/s.do?q=search&t=blog)](https://so.csdn.net/so/search/s.do?q=jni&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=工具&t=blog)](https://so.csdn.net/so/search/s.do?q=jdk&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://java.e800.com.cn/articles/2009/211/1234334178378_1.html](http://java.e800.com.cn/articles/2009/211/1234334178378_1.html)
相关网帖
1.Java命令详解(上)----[http://hubingforever.blog.163.com/blog/static/1710405792011324114234161/](http://hubingforever.blog.163.com/blog/static/1710405792011324114234161/)
java命令也是SCJP必考内容，很有用处，即使你现在用的是IDE.
一、查看用法
C:\>java -help
Usage: java [-options] class [args...]
(to execute a class)
or java [-options] -jar jarfile [args...]
(to execute a jar file)
where options include:
-client to select the "client" VM
-server to select the "server" VM
-hotspot is a synonym for the "client" VM [deprecated]
The default VM is client.
-cp <class search path of directories and zip/jar files>
-classpath <class search path of directories and zip/jar files>
A ; separated list of directories, JAR archives,
and ZIP archives to search for class files.
-D<name>=<value>
set a system property
-verbose[:class|gc|jni]
enable verbose output
-version print product version and exit
-version:<value>
require the specified version to run
-showversion print product version and continue
-jre-restrict-search | -jre-no-restrict-search
include/exclude user private JREs in the version search
-? -help print this help message
-X print help on non-standard options
-ea[:<packagename>...|:<classname>]
-enableassertions[:<packagename>...|:<classname>]
enable assertions
-da[:<packagename>...|:<classname>]
-disableassertions[:<packagename>...|:<classname>]
disable assertions
-esa | -enablesystemassertions
enable system assertions
-dsa | -disablesystemassertions
disable system assertions
-agentlib:<libname>[=<options>]
load native agent library <libname>, e.g. -agentlib:hprof
see also, -agentlib:jdwp=help and -agentlib:hprof=help
-agentpath:<pathname>[=<options>]
load native agent library by full pathname
-javaagent:<jarpath>[=<options>]
load Java programming language agent, see java.lang.instrument 
这个命令帮助是英文的，不知道JDK咋搞的，也不妨碍使用。
另外，这个命令的非标准选项也是很重要的，常常在JVM优化配置方面很关键，可以参看本人的JVM参数配置文章。
C:\myproject>java -X
-Xmixed mixed mode execution (default)
-Xint interpreted mode execution only
-Xbootclasspath:<directories and zip/jar files separated by ;>
set search path for bootstrap classes and resources
-Xbootclasspath/a:<directories and zip/jar files separated by ;>
append to end of bootstrap class path
-Xbootclasspath/p:<directories and zip/jar files separated by ;>
prepend in front of bootstrap class path
-Xnoclassgc disable class garbage collection
-Xincgc enable incremental garbage collection
-Xloggc:<file> log GC status to a file with time stamps
-Xbatch disable background compilation
-Xms<size> set initial Java heap size
-Xmx<size> set maximum Java heap size
-Xss<size> set java thread stack size
-Xprof output cpu profiling data
-Xfuture enable strictest checks, anticipating future default
-Xrs reduce use of OS signals by Java/VM (see documentation)
-Xcheck:jni perform additional checks for JNI functions
-Xshare:off do not attempt to use shared class data
-Xshare:auto use shared class data if possible (default)
-Xshare:on require using shared class data, otherwise fail. 
二、实践 
老规矩，主要看看里面的参数（也叫开关）的使用。环境接着上篇javac的环境。
1、无参数情况
2、-cp
运行Java要使用类的全名来运行。如果遇到文件夹，则需要-cp设置到顶级包下面，例如
3、-D 
设置一个系统属性，在运行时候，可以通过System.getProperties（）来获取到。例如写一段代码：
package com.lavasoft;
import java.util.Properties; 
/** 
* 列举系统的属性 
* User: leizhimin 
* Date: 2008-11-12 21:25:08 
*/ 
public class TestProperty { 
public static void main(String[] args) { 
//获取系统属性 
Properties prop = System.getProperties(); 
//输出所有到一个流上， 
prop.list(System.out); 
} 
}

如果在运行的时候加上一个参数-DmyProp=999999999，注意，中间不要添加任何空格。这样就相当于设置了一个系统属性myProp，其值为999999999.
一旦通过-DmyProp=999999999设置了这个系统属性，在程序中就可以获取到这个属性。
|3、程序入参 是在运行的时候，给main方法传递参数。为了测试先写一个测试类。package com.lavasoft;/** * 测试main方法的入参 * User: leizhimin * Date: 2008-11-12 21:46:21 */ public class TestMainVar { public static void main(String[] args) { System.out.println("入参列表如下："); for(String arg:args){ System.out.println(arg); } } }4、其他的选项有关选择 客户端/服务端VM、版本、运行在哪个版本下、是否使用断言等一些不常用的选项。还可以查看5、-verbose[：class|gc|jni]查看虚拟机内部动作。|package com.lavasoft;/** * 测试main方法的入参 * User: leizhimin * Date: 2008-11-12 21:46:21 */ public class TestMainVar { public static void main(String[] args) { System.out.println("入参列表如下："); for(String arg:args){ System.out.println(arg); } } }|
|----|----|
|package com.lavasoft;/** * 测试main方法的入参 * User: leizhimin * Date: 2008-11-12 21:46:21 */ public class TestMainVar { public static void main(String[] args) { System.out.println("入参列表如下："); for(String arg:args){ System.out.println(arg); } } }| |
4、其他的选项
有关选择 客户端/服务端VM、版本、运行在哪个版本下、是否使用断言等一些不常用的选项。还可以查看
5、-verbose[：class|gc|jni]
查看虚拟机内部动作。

