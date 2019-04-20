# JAVA之ClassLoader - Spark高级玩法 - CSDN博客
2017年12月24日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：358
JAVA基础系列之ClassLoader
一，Java类的加载、链接与初始化
1，加载:查找并加载类的二进制数据
• 通过一个类的全限定名来获取定义此类的二进制字节流
• 将这个字节流所代表的静态存储结构转化为方法区的运行时数据结构
• 在内存中生成一个代表这个类的java.lang.Class类对象，作为方法区这个类的各种数据的访问入口。
2，链接
验证:
确保被加载类的正确性
准备:
为类的静态变量分配内存，并将其初始化为默认值
解析:
把类中的符号引用转化为直接引用
3，初始化
为类的静态变量赋予正确的初始值
二，JVM加载类的主要方式
从本地系统中直接加载
通过网络下载.class文件
从zip，jar等归档文件中加载.class文件
从专有数据库中提取.class文件
讲Java的源文件动态编译为.class
三，JVM加载类的种类及功能
JVM通过CLassLoader(类加载器)来动态加载某个class文件到内存当中的。ClassLoader总共分为以下四种：
1，根(Bootstrap)类加载器
该类加载器没有父加载器，他负责加载虚拟机的核心类库，如java.lang.*等。根类加载器从系统属性sun.boot.class.path所指定的目录中加载类库。跟类加载器的实现依赖于底层操作系统属于虚拟机实现的一部分，它并没有继承java.lang.ClassLoader类。
2，扩展(Extension)类加载器
它的父类加载器为根类加载器。它从java.ext.dirs系统属性所指定的目录中加载类库，或者从JDK安装目录的jrelibext子目录(扩展目录加载类库)，如何用户创建的JAR问你件放在这个目录下也会被自动由扩展类加载器加载。扩展类加载器是纯Java类，是java.lang.ClassLoader类的子类。
3，系统类加载器
也称为应用类加载器，它的父加载器为扩展类加载器。他从环境变量classpath或者系统属性java.class.path所指定的目录中加载类，它是用户自定义的类加载器的默认父加载器。系统类加载器是纯Java类，是java.lang.ClassLoader类的子类。
除了以上虚拟机自带的加载器，用户也可以继承java.lang.ClassLoader类实现自定义加载器。
四，类加载器的原理
1，原理介绍
ClassLoader使用的是双亲委托模型来搜索类的，每个ClassLoader实例都有一个父类加载器的引用（不是继承的关系，是一个包含的关系），虚拟机内置的类加载器（Bootstrap ClassLoader）本身没有父类加载器，但可以用作其它ClassLoader实例的的父类加载器。当一个ClassLoader实例需要加载某个类时，它会试图亲自搜索某个类之前，先把这个任务委托给它的父类加载器，这个过程是由上至下依次检查的，首先由最顶层的类加载器Bootstrap ClassLoader试图加载，如果没加载到，则把任务转交给Extension ClassLoader试图加载，如果也没加载到，则转交给App ClassLoader 进行加载，如果它也没有加载得到的话，则返回给委托的发起者，由它到指定的文件系统或网络等URL中加载该类。如果它们都没有加载到这个类时，则抛出ClassNotFoundException异常。否则将这个找到的类生成一个类的定义，并将它加载到内存当中，最后返回这个类在内存中的Class实例对象。
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWmoR6LynOuKaaUxMX1XiczJp1Qb15iank75vibYEZcs1Sc14cZ2icUIPLCBsjJwxWdkhuRejibH0UYrGg/?wx_fmt=png)
2，使用双亲委托模型的原因
因为这样可以避免重复加载，当父亲已经加载了该类的时候，就没有必要子ClassLoader再加载一次。考虑到安全因素，我们试想一下，如果不使用这种委托模式，那我们就可以随时使用自定义的String来动态替代java核心api中定义的类型，这样会存在非常大的安全隐患，而双亲委托的方式，就可以避免这种情况，因为String已经在启动时就被引导类加载器（Bootstrcp ClassLoader）加载，所以用户自定义的ClassLoader永远也无法加载一个自己写的String，除非你改变JDK中ClassLoader搜索类的默认算法。
3，如何判断两个class相同
JVM在判定两个class是否相同时，不仅要判断两个类名是否相同，而且要判断是否由同一个类加载器实例加载的。只有两者同时满足的情况下，JVM才认为这两个class是相同的。就算两个class是同一份class字节码，如果被两个不同的ClassLoader实例所加载，JVM也会认为它们是两个不同class。比如网络上的一个Java类org.classloader.simple.NetClassLoaderSimple，javac编译之后生成字节码文件NetClassLoaderSimple.class，ClassLoaderA和ClassLoaderB这两个类加载器并读取了NetClassLoaderSimple.class文件，并分别定义出了java.lang.Class实例来表示这个类，对于JVM来说，它们是两个不同的实例对象，但它们确实是同一份字节码文件，如果试图将这个Class实例生成具体的对象进行转换时，就会抛运行时异常java.lang.ClassCaseException，提示这是两个不同的类型。
4，常用的方法
(1) loadClass方法
ClassLoader.loadClass()是ClassLoader的入口点。该方法的定义如下：
            Class loadClass(String name,boolean resolve);
         name是加载的类的名称，resolve是告诉方法是不中需要解析类PS:并不是所有的类都需要解析，如果JVM只想知道这个类是否存在或找出该类的超类，那么就不需要解析该类
(2) defineClass方法
defineClass方法接受由原始字节组成的数组，并把它转换成Class的对象。原始数组包含如从文件系统或网络装入的数据。defineClass管理JVM的许多复杂的实现层面——它把字节码分析成运行时数据结构、校验有效性等，因为defineClass方法被标记成final的，所以不能覆盖它。
(3) findSytemClass方法
findSystemClass方法就是查找本地类Class文件，然后装入
(4) resolveClass方法
我们在调用编写自己的loadClass方法的时候可以调用resolveClass方法来获得resolve参数
(5) findLoadedClass方法
在调用loadClass方法之前可以调用改方法来查看地ClassLoader是否已经装入了这个类，这样可以避免重新装入这个类
(6) findClass方法
在loadClass默认实现调用这个新方法。findClass的用途包含classLoader的所有特殊代码，而无须复制其他代码
(7) getSystemClassLoader方法
在如果覆盖findClass或loadClass,getSystemClassLoader能以实际的ClassLoader对象访问系统ClassLoader(而不是固定地从findSystemClass调用它)。为了将类请求委托给父类ClassLoader,这个新方法允许ClassLoader获取它的父类ClassLoader.当使用特殊方法，定制的ClassLoader不能找到类时，可以使用这种方法。
  父类ClassLoader被定义成创建该ClassLoader所包含代码的对象的ClassLoader.
(8) forName方法
在Class类中有一个静态方法forName,这个方法和ClassLoader中的loaderClass方法的目的是一样的，都是用来加载Class的，但是两者在作用上却有所区别：
loadClass加载实际上就是加载的时候并不对该类进行解释，因此不会初始化该类。而Class类的forName方法则相反，使用forName加载的时候就会将Class进行解释和初始化
五，类加载器的使用
使用URLClassLoader去加载类
importjava.lang.reflect.Constructor;
import java.lang.reflect.Method;
import java.net.URL;
import java.net.URLClassLoader;
public class ClassLoaderTest {
public static void main(String args[]) {
try {
                URL url = new URL("file:/C:/Users/spark/Desktop/logs-analyzer.jar");
URLClassLoader myClassLoader1 = new URLClassLoader(new URL[]{url}, Thread.currentThread().getContextClassLoader());
Class<?> clazz = myClassLoader1.loadClassQ("study.ClassLoaderTest.TestAction");
Method mainClass = clazz.getMethod("action");
Constructor<?> constructor = clazz.getConstructor();
Object obj = constructor.newInstance();
System.out.println(mainClass.invoke(obj));
} catch (Exception e) {
            e.printStackTrace();
}
    }
}
首先定义好一个类，然后打包成jar
public class TestAction{
public String action()
    {
return "this ActionTest class";
}
}
六，Spark中的URLClassLoader简述
Spark使用内部使用的最多的类加载器就是URLClassloader。
private[spark] class MutableURLClassLoader(urls: Array[URL], parent: ClassLoader)
extends URLClassLoader(urls, parent) {
override def addURL(url: URL): Unit = {
super.addURL(url)
  }
override def getURLs(): Array[URL] = {
super.getURLs()
  }
}
这样取决于Spark分布式计算的特性，后面源码系列讲述到运行环境的时候会详细说道这个问题。
推荐阅读：
1，[大数据基础系列之JAVA引用详解](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484059&idx=1&sn=67122f6fcc6dff5c4c2bd7cfc21e9b3a&chksm=9f38e1b3a84f68a54a1ec90470a39b5c8c99b627abaad0e2d38b818d42ce25eeb8227e34c0c8&scene=21#wechat_redirect)
2，[HBase在滴滴出行的应用场景和最佳实践](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484343&idx=1&sn=cd0c5bd1fbd8d55dd9779d336880bb3d&chksm=9f38e09fa84f6989105d027e60beb463332313724813ea114ec3f4e29707c8da75fc09434269&scene=21#wechat_redirect)
3，[Spark的Ml pipeline](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484270&idx=1&sn=69153063accd105649d4466d938012da&chksm=9f38e046a84f69505d954726c65d940f9a370342755ae2ce7456134ef4b9a6eafb1350a4dfb6&scene=21#wechat_redirect)
4，[Flink系列之时间](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484055&idx=1&sn=4415fc918b0313002303fd9e999a476a&chksm=9f38e1bfa84f68a917fed17ebb28db0951c0c00740d30cae54ee56c24986e4e8b6b373febcd8&scene=21#wechat_redirect)
![640.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yqVAqoZvDibGvvZsCSFg4MtnXJvpwMWqssPsNF7n9JsSib9z44Naja7DrEQls7XabzQXaAfiaceo4mmovPjEzoIjQ/640.png)
**关于Spark学习技巧**
kafka，hbase，spark，Flink等入门到深入源码，spark机器学习，大数据安全，大数据运维，请关注浪尖公众号，看高质量文章。
![640.jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWicEOUZkFLHTzutxwiadmt0ickOuxL2Ml31qdB6W0n5AgPMfpzSib0HVDDNhtm3MwW2yC8qo09ibpeoYA/640.jpeg)
**更多文章，敬请期待**

