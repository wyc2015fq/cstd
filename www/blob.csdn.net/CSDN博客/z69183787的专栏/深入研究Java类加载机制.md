# 深入研究Java类加载机制 - z69183787的专栏 - CSDN博客
2014年06月07日 21:47:14[OkidoGreen](https://me.csdn.net/z69183787)阅读数：967
个人分类：[Java-知识																[Jvm-内存模型分析](https://blog.csdn.net/z69183787/article/category/2275953)](https://blog.csdn.net/z69183787/article/category/2175279)
类加载是Java程序运行的第一步，研究类的加载有助于了解JVM执行过程，并指导开发者采取更有效的措施配合程序执行。
研究类加载机制的第二个目的是让程序能动态的控制类加载，比如热部署等，提高程序的灵活性和适应性。
**一、简单过程**
Java程序运行的场所是内存，当在命令行下执行：
java HelloWorld
命令的时候，JVM会将HelloWorld.class加载到内存中，并形成一个Class的对象HelloWorld.class。
其中的过程就是类加载过程：
1、寻找jre目录，寻找jvm.dll，并初始化JVM；
2、产生一个Bootstrap Loader（启动类加载器）；
3、Bootstrap Loader自动加载Extended Loader（标准扩展类加载器），并将其父Loader设为Bootstrap Loader。
4、Bootstrap Loader自动加载AppClass Loader（系统类加载器），并将其父Loader设为Extended Loader。
5、最后由AppClass Loader加载HelloWorld类。
以上就是类加载的最一般的过程。
**二、类加载器各自搜索的目录**
为了弄清楚这个问题，首先还要看看System类的API doc文档。
|**键**|**相关值的描述**|
|----|----|
|java.version|Java 运行时环境版本|
|java.vendor|Java 运行时环境供应商|
|java.vendor.url|Java 供应商的 URL|
|java.home|Java 安装目录|
|java.vm.specification.version|Java 虚拟机规范版本|
|java.vm.specification.vendor|Java 虚拟机规范供应商|
|java.vm.specification.name|Java 虚拟机规范名称|
|java.vm.version|Java 虚拟机实现版本|
|java.vm.vendor|Java 虚拟机实现供应商|
|java.vm.name|Java 虚拟机实现名称|
|java.specification.version|Java 运行时环境规范版本|
|java.specification.vendor|Java 运行时环境规范供应商|
|java.specification.name|Java 运行时环境规范名称|
|java.class.version|Java 类格式版本号|
|java.class.path|Java 类路径|
|java.library.path|加载库时搜索的路径列表|
|java.io.tmpdir|默认的临时文件路径|
|java.compiler|要使用的 JIT 编译器的名称|
|java.ext.dirs|一个或多个扩展目录的路径|
|os.name|操作系统的名称|
|os.arch|操作系统的架构|
|os.version|操作系统的版本|
|file.separator|文件分隔符（在 UNIX 系统中是“/”）|
|path.separator|路径分隔符（在 UNIX 系统中是“:”）|
|line.separator|行分隔符（在 UNIX 系统中是“/n”）|
|user.name|用户的账户名称|
|user.home|用户的主目录|
|user.dir|用户的当前工作目录|
可惜这个帮助文档并不全，直接用程序打印出来如下：
for (Map.Entry<Object, Object> entry : System.getProperties().entrySet()) { 
                        System.out.println(entry.getKey()+"\t"+entry.getValue()); 
                }
|java.runtime.name|Java(TM) SE Runtime Environment|
|----|----|
|sun.boot.library.path|Q:\jdk6\jre\bin|
|java.vm.version|14.0-b16|
|java.vm.vendor|Sun Microsystems Inc.|
|java.vendor.url|http://java.sun.com/|
|path.separator|;|
|idea.launcher.port|7532|
|java.vm.name|Java HotSpot(TM) Client VM|
|file.encoding.pkg|sun.io|
|sun.java.launcher|SUN_STANDARD|
|user.country|CN|
|sun.os.patch.level|Service Pack 3|
|java.vm.specification.name|Java Virtual Machine Specification|
|user.dir|E:\projects\testScanner|
|java.runtime.version|1.6.0_14-b08|
|java.awt.graphicsenv|sun.awt.Win32GraphicsEnvironment|
|java.endorsed.dirs|Q:\jdk6\jre\lib\endorsed|
|os.arch|x86|
|java.io.tmpdir|C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\|
|line.separator| |
|java.vm.specification.vendor|Sun Microsystems Inc.|
|user.variant| |
|os.name|Windows XP|
|sun.jnu.encoding|GBK|
|java.library.path|Q:\jdk6\bin;.;C:\WINDOWS\Sun\Java\bin;C:\WINDOWS\system32;C:\WINDOWS;Q:\jdk6\bin;Q:\JavaFX\javafx-sdk1.2\bin;Q:\JavaFX\javafx-sdk1.2\emulator\bin;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;C:\MySQL Server 5.1\bin;C:\Program Files\StormII\Codec;C:\Program Files\StormII|
|java.specification.name|Java Platform API Specification|
|java.class.version|50|
|sun.management.compiler|HotSpot Client Compiler|
|os.version|5.1|
|user.home|d:\我的文档|
|user.timezone| |
|java.awt.printerjob|sun.awt.windows.WPrinterJob|
|idea.launcher.bin.path|C:\IDEA8\bin|
|file.encoding|UTF-8|
|java.specification.version|1.6|
|java.class.path|Q:\jdk6\jre\lib\alt-rt.jar;Q:\jdk6\jre\lib\charsets.jar;Q:\jdk6\jre\lib\deploy.jar;Q:\jdk6\jre\lib\javaws.jar;Q:\jdk6\jre\lib\jce.jar;Q:\jdk6\jre\lib\jsse.jar;Q:\jdk6\jre\lib\management-agent.jar;Q:\jdk6\jre\lib\plugin.jar;Q:\jdk6\jre\lib\resources.jar;Q:\jdk6\jre\lib\rt.jar;Q:\jdk6\jre\lib\ext\dnsns.jar;Q:\jdk6\jre\lib\ext\localedata.jar;Q:\jdk6\jre\lib\ext\sunjce_provider.jar;Q:\jdk6\jre\lib\ext\sunmscapi.jar;Q:\jdk6\jre\lib\ext\sunpkcs11.jar;E:\projects\testScanner\out\production\testScanner;C:\IDEA8\lib\idea_rt.jar|
|user.name|Administrator|
|java.vm.specification.version|1|
|java.home|Q:\jdk6\jre|
|sun.arch.data.model|32|
|user.language|zh|
|java.specification.vendor|Sun Microsystems Inc.|
|awt.toolkit|sun.awt.windows.WToolkit|
|java.vm.info|mixed mode, sharing|
|java.version|1.6.0_14|
|java.ext.dirs|Q:\jdk6\jre\lib\ext;C:\WINDOWS\Sun\Java\lib\ext|
|sun.boot.class.path|Q:\jdk6\jre\lib\resources.jar;Q:\jdk6\jre\lib\rt.jar;Q:\jdk6\jre\lib\sunrsasign.jar;Q:\jdk6\jre\lib\jsse.jar;Q:\jdk6\jre\lib\jce.jar;Q:\jdk6\jre\lib\charsets.jar;Q:\jdk6\jre\classes|
|java.vendor|Sun Microsystems Inc.|
|file.separator|\|
|java.vendor.url.bug|http://java.sun.com/cgi-bin/bugreport.cgi|
|sun.io.unicode.encoding|UnicodeLittle|
|sun.cpu.endian|little|
|sun.desktop|windows|
|sun.cpu.isalist| |
1、Bootstrap Loader（启动类加载器）：加载System.getProperty("sun.boot.class.path")所指定的路径或jar。
2、Extended Loader（标准扩展类加载器ExtClassLoader）：加载System.getProperty("java.ext.dirs")所指定的路径或jar。在使用Java运行程序时，也可以指定其搜索路径，例如：java -Djava.ext.dirs=d:\projects\testproj\classes HelloWorld
3、AppClass Loader（系统类加载器AppClassLoader）：加载System.getProperty("java.class.path")所指定的路径或jar。在使用Java运行程序时，也可以加上-cp来覆盖原有的Classpath设置，例如： java -cp ./lavasoft/classes HelloWorld
ExtClassLoader和AppClassLoader在JVM启动后，会在JVM中保存一份，并且在程序运行中无法改变其搜索路径。如果想在运行时从其他搜索路径加载类，就要产生新的类加载器。
**三、类加载器的特点**
1、运行一个程序时，总是由AppClass Loader（系统类加载器）开始加载指定的类。
2、在加载类时，每个类加载器会将加载任务上交给其父，如果其父找不到，再由自己去加载。
3、Bootstrap Loader（启动类加载器）是最顶级的类加载器了，其父加载器为null.
**四、类加载器的获取**
很容易，看下面例子
publicclass HelloWorld { 
publicstaticvoid main(String[] args) { 
                HelloWorld hello = new HelloWorld(); 
                Class c = hello.getClass(); 
                ClassLoader loader = c.getClassLoader(); 
                System.out.println(loader); 
                System.out.println(loader.getParent()); 
                System.out.println(loader.getParent().getParent()); 
        } 
}
打印结果：
sun.misc.Launcher$AppClassLoader@19821f 
sun.misc.Launcher$ExtClassLoader@addbf1 
null
Process finished with exit code 0
从上面的结果可以看出，并没有获取到ExtClassLoader的父Loader，原因是Bootstrap Loader（启动类加载器）是用C语言实现的，找不到一个确定的返回父Loader的方式，于是就返回null。
**五、类的加载**
类加载有三种方式：
1、命令行启动应用时候由JVM初始化加载
2、通过Class.forName()方法动态加载
3、通过ClassLoader.loadClass()方法动态加载
三种方式区别比较大，看个例子就明白了：
publicclass HelloWorld { 
publicstaticvoid main(String[] args) throws ClassNotFoundException { 
                ClassLoader loader = HelloWorld.class.getClassLoader(); 
                System.out.println(loader); 
//使用ClassLoader.loadClass()来加载类，不会执行初始化块 
                loader.loadClass("Test2"); 
//使用Class.forName()来加载类，默认会执行初始化块 
//                Class.forName("Test2"); 
//使用Class.forName()来加载类，并指定ClassLoader，初始化时不执行静态块 
//                Class.forName("Test2", false, loader); 
        } 
}
publicclass Test2 { 
static { 
                System.out.println("静态初始化块执行了！"); 
        } 
}
分别切换加载方式，会有不同的输出结果。
**六、自定义ClassLoader**
为了说明问题，先看例子：
package test; 
import java.net.MalformedURLException; 
import java.net.URL; 
import java.net.URLClassLoader; 
/** 
* 自定义ClassLoader 
* 
* @author leizhimin 2009-7-29 22:05:48 
*/
publicclass MyClassLoader { 
publicstaticvoid main(String[] args) throws MalformedURLException, ClassNotFoundException, IllegalAccessException,
 InstantiationException { 
                URL url = new URL("file:/E:\\projects\\testScanner\\out\\production\\testScanner"); 
                ClassLoader myloader = new URLClassLoader(new URL[]{url}); 
                Class c = myloader.loadClass("test.Test3"); 
                System.out.println("----------"); 
                Test3 t3 = (Test3) c.newInstance(); 
        } 
}
publicclass Test3 { 
static { 
                System.out.println("Test3的静态初始化块执行了！"); 
        } 
}
运行后：
---------- 
Test3的静态初始化块执行了！ 
Process finished with exit code 0
可以看出自定义了ClassLoader myloader = new URLClassLoader(new URL[]{url});已经成功将类Test3加载到内存了，并通过默认构造方法构造了对象Test3 t3 = (Test3) c.newInstance();
有关ClassLoader还有很重要一点：
同一个ClassLoader加载的类文件，只有一个Class实例。但是，如果同一个类文件被不同的ClassLoader载入，则会有两份不同的ClassLoader实例（前提是着两个类加载器不能用相同的父类加载器）。
-----------------------
推荐几篇优秀的文章：
[http://nonopo.javaeye.com/blog/208007](http://nonopo.javaeye.com/blog/208007)
[http://nonopo.javaeye.com/blog/208012](http://nonopo.javaeye.com/blog/208012)
