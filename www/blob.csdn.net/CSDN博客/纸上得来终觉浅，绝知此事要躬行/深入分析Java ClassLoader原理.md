# 深入分析Java ClassLoader原理 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年04月09日 20:17:24[boonya](https://me.csdn.net/boonya)阅读数：675标签：[Java																[classloader																[jdk																[class																[path](https://so.csdn.net/so/search/s.do?q=path&t=blog)
个人分类：[Java Practice](https://blog.csdn.net/boonya/article/category/3089055)






## 原文出处： [CSDN-xyang81](http://blog.csdn.net/xyang81/article/details/7292380)

## 一、什么是ClassLoader?


大家都知道，当我们写好一个Java程序之后，不是管是CS还是BS应用，都是由若干个.class文件组织而成的一个完整的Java应用程序，当程序在运行时，即会调用该程序的一个入口函数来调用系统的相关功能，而这些功能都被封装在不同的class文件当中，所以经常要从这个class文件中要调用另外一个class文件中的方法，如果另外一个文件不存在的，则会引发系统异常。而程序在启动的时候，并不会一次性加载程序所要用的所有class文件，而是根据程序的需要，通过Java的类加载机制（ClassLoader）来动态加载某个class文件到内存当中的，从而只有class文件被载入到了内存之后，才能被其它class所引用。所以ClassLoader就是用来动态加载class文件到内存当中用的。

**二、Java默认提供的三个ClassLoader**
- BootStrap ClassLoader：称为启动类加载器，是Java类加载层次中最顶层的类加载器，负责加载JDK中的核心类库，如：rt.jar、resources.jar、charsets.jar等，可通过如下程序获得该类加载器从哪些地方加载了相关的jar或class文件：




```
```java
URL[]
 urls = sun.misc.Launcher.getBootstrapClassPath().getURLs();
```



```java
for
```

```java
(
```

```java
int
```

```java
i
 =
```

```java
0
```

```java
;
 i < urls.length; i++) {
```



```java

```

```java
System.out.println(urls[i].toExternalForm());
```



```java
}
```
```





以下内容是上述程序从本机JDK环境所获得的结果：
file:/C:/Program%20Files/Java/jdk1.6.0_22/jre/lib/resources.jar
file:/C:/Program%20Files/Java/jdk1.6.0_22/jre/lib/rt.jar
file:/C:/Program%20Files/Java/jdk1.6.0_22/jre/lib/sunrsasign.jar
file:/C:/Program%20Files/Java/jdk1.6.0_22/jre/lib/jsse.jar
file:/C:/Program%20Files/Java/jdk1.6.0_22/jre/lib/jce.jar
file:/C:/Program%20Files/Java/jdk1.6.0_22/jre/lib/charsets.jar
file:/C:/Program%20Files/Java/jdk1.6.0_22/jre/classes/

其实上述结果也是通过查找sun.boot.class.path这个系统属性所得知的。



```
```java
System.out.println(System.getProperty(
```

```java
"sun.boot.class.path"
```

```java
));
```
```




打印结果：C:\Program Files\Java\jdk1.6.0_22\jre\lib\resources.jar;C:\Program Files\Java\jdk1.6.0_22\jre\lib\rt.jar;C:\Program Files\Java\jdk1.6.0_22\jre\lib\sunrsasign.jar;C:\Program
 Files\Java\jdk1.6.0_22\jre\lib\jsse.jar;C:\Program Files\Java\jdk1.6.0_22\jre\lib\jce.jar;C:\Program Files\Java\jdk1.6.0_22\jre\lib\charsets.jar;C:\Program Files\Java\jdk1.6.0_22\jre\classes
- Extension ClassLoader：称为扩展类加载器，负责加载Java的扩展类库，默认加载JAVA_HOME/jre/lib/ext/目下的所有jar。
- App ClassLoader：称为系统类加载器，负责加载应用程序classpath目录下的所有jar和class文件。

注意： 除了Java默认提供的三个ClassLoader之外，用户还可以根据需要定义自已的ClassLoader，而这些自定义的ClassLoader都必须继承自java.lang.ClassLoader类，也包括Java提供的另外二个ClassLoader（Extension ClassLoader和App ClassLoader）在内，但是Bootstrap ClassLoader不继承自ClassLoader，因为它不是一个普通的Java类，底层由C++编写，已嵌入到了JVM内核当中，当JVM启动后，Bootstrap
 ClassLoader也随着启动，负责加载完核心类库后，并构造Extension ClassLoader和App ClassLoader类加载器。

## 三、ClassLoader加载类的原理

## **1、原理介绍**


ClassLoader使用的是双亲委托模型来搜索类的，每个ClassLoader实例都有一个父类加载器的引用（不是继承的关系，是一个包含的关系），虚拟机内置的类加载器（Bootstrap ClassLoader）本身没有父类加载器，但可以用作其它ClassLoader实例的的父类加载器。当一个ClassLoader实例需要加载某个类时，它会试图亲自搜索某个类之前，先把这个任务委托给它的父类加载器，这个过程是由上至下依次检查的，首先由最顶层的类加载器Bootstrap
 ClassLoader试图加载，如果没加载到，则把任务转交给Extension ClassLoader试图加载，如果也没加载到，则转交给App ClassLoader 进行加载，如果它也没有加载得到的话，则返回给委托的发起者，由它到指定的文件系统或网络等URL中加载该类。如果它们都没有加载到这个类时，则抛出ClassNotFoundException异常。否则将这个找到的类生成一个类的定义，并将它加载到内存当中，最后返回这个类在内存中的Class实例对象。

**2、为什么要使用双亲委托这种模型呢？**


因为这样可以避免重复加载，当父亲已经加载了该类的时候，就没有必要子ClassLoader再加载一次。考虑到安全因素，我们试想一下，如果不使用这种委托模式，那我们就可以随时使用自定义的String来动态替代java核心api中定义的类型，这样会存在非常大的安全隐患，而双亲委托的方式，就可以避免这种情况，因为String已经在启动时就被引导类加载器（Bootstrcp ClassLoader）加载，所以用户自定义的ClassLoader永远也无法加载一个自己写的String，除非你改变JDK中ClassLoader搜索类的默认算法。

**3、 **但是JVM在搜索类的时候，又是如何判定两个class是相同的呢？

JVM在判定两个class是否相同时，不仅要判断两个类名是否相同，而且要判断是否由同一个类加载器实例加载的。只有两者同时满足的情况下，JVM才认为这两个class是相同的。就算两个class是同一份class字节码，如果被两个不同的ClassLoader实例所加载，JVM也会认为它们是两个不同class。比如网络上的一个Java类org.classloader.simple.NetClassLoaderSimple，javac编译之后生成字节码文件NetClassLoaderSimple.class，ClassLoaderA和ClassLoaderB这两个类加载器并读取了NetClassLoaderSimple.class文件，并分别定义出了java.lang.Class实例来表示这个类，对于JVM来说，它们是两个不同的实例对象，但它们确实是同一份字节码文件，如果试图将这个Class实例生成具体的对象进行转换时，就会抛运行时异常java.lang.ClassCaseException，提示这是两个不同的类型。现在通过实例来验证上述所描述的是否正确：


1）、在web服务器上建一个org.classloader.simple.NetClassLoaderSimple.java类



```
```java
package
```

```java
org.classloader.simple;
```



```java

```



```java
public
```

```java
class
```

```java
NetClassLoaderSimple {
```



```java

```



```java

```

```java
private
```

```java
NetClassLoaderSimple
 instance;
```



```java

```



```java

```

```java
public
```

```java
void
```

```java
setNetClassLoaderSimple(Object obj) {
```



```java

```

```java
this
```

```java
.instance
 = (NetClassLoaderSimple)obj;
```



```java

```

```java
}
```



```java
}
```
```




org.classloader.simple.NetClassLoaderSimple类的setNetClassLoaderSimple方法接收一个Object类型参数，并将它强制转换成org.classloader.simple.NetClassLoaderSimple类型。


2）、测试两个class是否相同（[NetWorkClassLoader.java](http://blog.csdn.net/xyang81/article/details/7292380#NetWorkClassLoader)）



```
```java
package
```

```java
classloader;
```



```java

```



```java
public
```

```java
class
```

```java
NewworkClassLoaderTest {
```



```java

```



```java

```

```java
public
```

```java
static
```

```java
void
```

```java
main(String[] args) {
```



```java

```

```java
try
```

```java
{
```



```java

```

```java
//测试加载网络中的class文件
```



```java

```

```java
String
 rootUrl =
```

```java
"http://localhost:8080/httpweb/classes"
```

```java
;
```



```java

```

```java
String
 className =
```

```java
"org.classloader.simple.NetClassLoaderSimple"
```

```java
;
```



```java

```

```java
NetworkClassLoader
 ncl1 =
```

```java
new
```

```java
NetworkClassLoader(rootUrl);
```



```java

```

```java
NetworkClassLoader
 ncl2 =
```

```java
new
```

```java
NetworkClassLoader(rootUrl);
```



```java

```

```java
Class<?>
 clazz1 = ncl1.loadClass(className);
```



```java

```

```java
Class<?>
 clazz2 = ncl2.loadClass(className);
```



```java

```

```java
Object
 obj1 = clazz1.newInstance();
```



```java

```

```java
Object
 obj2 = clazz2.newInstance();
```



```java

```

```java
clazz1.getMethod(
```

```java
"setNetClassLoaderSimple"
```

```java
,
 Object.
```

```java
class
```

```java
).invoke(obj1,
 obj2);
```



```java

```

```java
}
```

```java
catch
```

```java
(Exception
 e) {
```



```java

```

```java
e.printStackTrace();
```



```java

```

```java
}
```



```java

```

```java
}
```



```java

```



```java
}
```
```




首先获得网络上一个class文件的二进制名称，然后通过自定义的类加载器NetworkClassLoader创建两个实例，并根据网络地址分别加载这份class，并得到这两个ClassLoader实例加载后生成的Class实例clazz1和clazz2，最后将这两个Class实例分别生成具体的实例对象obj1和obj2，再通过反射调用clazz1中的setNetClassLoaderSimple方法。


3）、查看测试结果

![](http://incdn1.b0.upaiyun.com/2015/03/28c8edde3d61a0411511d3b1866f0636.jpg)

结论：从结果中可以看出，虽然是同一份class字节码文件，但是由于被两个不同的ClassLoader实例所加载，所以JVM认为它们就是两个不同的类。

**4、ClassLoader的体系架构：**

![](http://incdn1.b0.upaiyun.com/2015/03/665f644e43731ff9db3d341da5c827e11.png)

### 验证ClassLoader加载类的原理：

测试1：打印ClassLoader类的层次结构，请看下面这段代码：



```
```java
ClassLoader
 loader = ClassLoaderTest.
```

```java
class
```

```java
.getClassLoader();
```

```java
//获得加载ClassLoaderTest.class这个类的类加载器
```



```java
while
```

```java
(loader
 !=
```

```java
null
```

```java
)
 {
```



```java

```

```java
System.out.println(loader);
```



```java

```

```java
loader
 = loader.getParent();
```

```java
//获得父类加载器的引用
```



```java
}
```



```java
System.out.println(loader);
```
```



打印结果：

![](http://incdn1.b0.upaiyun.com/2015/03/38026ed22fc1a91d92b5d2ef93540f20.jpg)


第一行结果说明：ClassLoaderTest的类加载器是AppClassLoader。


第二行结果说明：AppClassLoader的类加器是ExtClassLoader，即parent=ExtClassLoader。


第三行结果说明：ExtClassLoader的类加器是Bootstrap ClassLoader，因为Bootstrap ClassLoader不是一个普通的Java类，所以ExtClassLoader的parent=null，所以第三行的打印结果为null就是这个原因。

测试2：将ClassLoaderTest.class打包成ClassLoaderTest.jar，放到Extension ClassLoader的加载目录下（JAVA_HOME/jre/lib/ext），然后重新运行这个程序，得到的结果会是什么样呢？

![](http://incdn1.b0.upaiyun.com/2015/03/011ecee7d295c066ae68d4396215c3d0.jpg)

打印结果：

![](http://incdn1.b0.upaiyun.com/2015/03/4e44f1ac85cd60e3caa56bfd4afb675e.jpg)

打印结果分析：

为什么第一行的结果是ExtClassLoader呢？


因为ClassLoader的委托模型机制，当我们要用ClassLoaderTest.class这个类的时候，AppClassLoader在试图加载之前，先委托给Bootstrcp ClassLoader，Bootstracp ClassLoader发现自己没找到，它就告诉ExtClassLoader，兄弟，我这里没有这个类，你去加载看看，然后Extension ClassLoader拿着这个类去它指定的类路径（JAVA_HOME/jre/lib/ext）试图加载，唉，它发现在ClassLoaderTest.jar这样一个文件中包含ClassLoaderTest.class这样的一个文件，然后它把找到的这个类加载到内存当中，并生成这个类的Class实例对象，最后把这个实例返回。所以ClassLoaderTest.class的类加载器是ExtClassLoader。


第二行的结果为null，是因为ExtClassLoader的父类加载器是Bootstrap ClassLoader。

测试3：用Bootstrcp ClassLoader来加载ClassLoaderTest.class，有两种方式：


1、在jvm中添加-Xbootclasspath参数，指定Bootstrcp ClassLoader加载类的路径，并追加我们自已的jar（ClassTestLoader.jar）


2、将class文件放到JAVA_HOME/jre/classes/目录下（上面有提到）

方式1：（我用的是[Eclipse](http://res.importnew.com/eclipse)开发工具，用命令行是在java命令后面添加-Xbootclasspath参数）


打开Run配置对话框：

![](http://incdn1.b0.upaiyun.com/2015/03/3d2f8900f2e49c02b481c2f717aa9020.jpg)


配置好如图中所述的参数后，重新运行程序，产的结果如下所示：（类加载的过程，只摘下了一部份）

打印结果：



```
```java
[Loaded
 java.io.FileReader from C:\Program Files\Java\jdk1.
```

```java
6
```

```java
.0_22\jre\lib\rt.jar]
```



```java
[Loaded
 sun.nio.cs.StreamDecoder from C:\Program Files\Java\jdk1.
```

```java
6
```

```java
.0_22\jre\lib\rt.jar]
```



```java
[Loaded
 java.util.ArrayList from C:\Program Files\Java\jdk1.
```

```java
6
```

```java
.0_22\jre\lib\rt.jar]
```



```java
[Loaded
 java.lang.reflect.Array from C:\Program Files\Java\jdk1.
```

```java
6
```

```java
.0_22\jre\lib\rt.jar]
```



```java
[Loaded
 java.util.Locale from C:\Program Files\Java\jdk1.
```

```java
6
```

```java
.0_22\jre\lib\rt.jar]
```



```java
[Loaded
 java.util.concurrent.ConcurrentMap from C:\Program Files\Java\jdk1.
```

```java
6
```

```java
.0_22\jre\lib\rt.jar]
```



```java
[Loaded
 java.util.concurrent.ConcurrentHashMap from C:\Program Files\Java\jdk1.
```

```java
6
```

```java
.0_22\jre\lib\rt.jar]
```



```java
[Loaded
 java.util.concurrent.locks.Lock from C:\Program Files\Java\jdk1.
```

```java
6
```

```java
.0_22\jre\lib\rt.jar]
```



```java
[Loaded
 java.util.concurrent.locks.ReentrantLock from C:\Program Files\Java\jdk1.
```

```java
6
```

```java
.0_22\jre\lib\rt.jar]
```



```java
[Loaded
 java.util.concurrent.ConcurrentHashMap$Segment from C:\Program Files\Java\jdk1.
```

```java
6
```

```java
.0_22\jre\lib\rt.jar]
```



```java
[Loaded
 java.util.concurrent.locks.AbstractOwnableSynchronizer from C:\Program Files\Java\jdk1.
```

```java
6
```

```java
.0_22\jre\lib\rt.jar]
```



```java
[Loaded
 java.util.concurrent.locks.AbstractQueuedSynchronizer from C:\Program Files\Java\jdk1.
```

```java
6
```

```java
.0_22\jre\lib\rt.jar]
```



```java
[Loaded
 java.util.concurrent.locks.ReentrantLock$Sync from C:\Program Files\Java\jdk1.
```

```java
6
```

```java
.0_22\jre\lib\rt.jar]
```



```java
[Loaded
 java.util.concurrent.locks.ReentrantLock$NonfairSync from C:\Program Files\Java\jdk1.
```

```java
6
```

```java
.0_22\jre\lib\rt.jar]
```



```java
[Loaded
 java.util.concurrent.locks.AbstractQueuedSynchronizer$Node from C:\Program Files\Java\jdk1.
```

```java
6
```

```java
.0_22\jre\lib\rt.jar]
```



```java
[Loaded
 java.util.concurrent.ConcurrentHashMap$HashEntry from C:\Program Files\Java\jdk1.
```

```java
6
```

```java
.0_22\jre\lib\rt.jar]
```



```java
[Loaded
 java.lang.CharacterDataLatin1 from C:\Program Files\Java\jdk1.
```

```java
6
```

```java
.0_22\jre\lib\rt.jar]
```



```java
[Loaded
 java.io.ObjectStreamClass from C:\Program Files\Java\jdk1.
```

```java
6
```

```java
.0_22\jre\lib\rt.jar]
```



```java
[Loaded
 sun.net.www.ParseUtil from C:\Program Files\Java\jdk1.
```

```java
6
```

```java
.0_22\jre\lib\rt.jar]
```



```java
[Loaded
 java.util.BitSet from C:\Program Files\Java\jdk1.
```

```java
6
```

```java
.0_22\jre\lib\rt.jar]
```



```java
[Loaded
 java.net.Parts from C:\Program Files\Java\jdk1.
```

```java
6
```

```java
.0_22\jre\lib\rt.jar]
```



```java
[Loaded
 java.net.URLStreamHandler from C:\Program Files\Java\jdk1.
```

```java
6
```

```java
.0_22\jre\lib\rt.jar]
```



```java
[Loaded
 sun.net.www.protocol.file.Handler from C:\Program Files\Java\jdk1.
```

```java
6
```

```java
.0_22\jre\lib\rt.jar]
```



```java
[Loaded
 java.util.HashSet from C:\Program Files\Java\jdk1.
```

```java
6
```

```java
.0_22\jre\lib\rt.jar]
```



```java
[Loaded
 sun.net.www.protocol.jar.Handler from C:\Program Files\Java\jdk1.
```

```java
6
```

```java
.0_22\jre\lib\rt.jar]
```



```java
[Loaded
 sun.misc.Launcher$AppClassLoader from C:\Program Files\Java\jdk1.
```

```java
6
```

```java
.0_22\jre\lib\rt.jar]
```



```java
[Loaded
 sun.misc.Launcher$AppClassLoader$
```

```java
1
```

```java
from
 C:\Program Files\Java\jdk1.
```

```java
6
```

```java
.0_22\jre\lib\rt.jar]
```



```java
[Loaded
 java.lang.SystemClassLoaderAction from C:\Program Files\Java\jdk1.
```

```java
6
```

```java
.0_22\jre\lib\rt.jar]
```



```java
[Path
 C:\Program Files\Java\jdk1.
```

```java
6
```

```java
.0_22\jre\classes]
```



```java
[Loaded
 classloader.ClassLoaderTest from C:\Program Files\Java\jdk1.
```

```java
6
```

```java
.0_22\jre\classes]
```



```java
null
```

```java
//这是打印的结果
```



```java
C:\Program
 Files\Java\jdk1.
```

```java
6
```

```java
.0_22\jre\lib\resources.jar;C:\Program
 Files\Java\jdk1.
```

```java
6
```

```java
.0_22\jre\lib\rt.jar;
```



```java
C:\Program
 Files\Java\jdk1.
```

```java
6
```

```java
.0_22\jre\lib\sunrsasign.jar;C:\Program
 Files\Java\jdk1.
```

```java
6
```

```java
.0_22\jre\lib\jsse.jar;
```



```java
C:\Program
 Files\Java\jdk1.
```

```java
6
```

```java
.0_22\jre\lib\jce.jar;C:\Program
 Files\Java\jdk1.
```

```java
6
```

```java
.0_22\jre\lib\charsets.jar;
```



```java
C:\Program
 Files\Java\jdk1.
```

```java
6
```

```java
.0_22\jre\classes;c:\ClassLoaderTest.jar
```



```java
//这一段是System.out.println(System.getProperty("sun.boot.class.path"));打印出来的。这个路径就是Bootstrcp
 ClassLoader默认搜索类的路径
```



```java
[Loaded
 java.lang.Shutdown from C:\Program Files\Java\jdk1.
```

```java
6
```

```java
.0_22\jre\lib\rt.jar]
```



```java
[Loaded
 java.lang.Shutdown$Lock from C:\Program Files\Java\jdk1.
```

```java
6
```

```java
.0_22\jre\lib\rt.jar]
```
```



方式2：将ClassLoaderTest.jar解压后，放到JAVA_HOME/jre/classes目录下，如下图所示：

提示：jre目录下默认没有classes目录，需要自己手动创建一个

![](http://incdn1.b0.upaiyun.com/2015/03/cd7fd1517e323f26c6f1b0b6b96e3b3d.jpg)

打印结果：

![](http://incdn1.b0.upaiyun.com/2015/03/815e6212def15fe76ed27cec7a393d59.jpg)


从结果中可以看出，两种方式都实现了将ClassLoaderTest.class由Bootstrcp ClassLoader加载成功了。

**四、定义自已的ClassLoader**

既然JVM已经提供了默认的类加载器，为什么还要定义自已的类加载器呢？


因为Java中提供的默认ClassLoader，只加载指定目录下的jar和class，如果我们想加载其它位置的类或jar时，比如：我要加载网络上的一个class文件，通过动态加载到内存之后，要调用这个类中的方法实现我的业务逻辑。在这样的情况下，默认的ClassLoader就不能满足我们的需求了，所以需要定义自己的ClassLoader。

定义自已的类加载器分为两步：


1、继承java.lang.ClassLoader


2、重写父类的findClass方法


读者可能在这里有疑问，父类有那么多方法，为什么偏偏只重写findClass方法？


因为JDK已经在loadClass方法中帮我们实现了ClassLoader搜索类的算法，当在loadClass方法中搜索不到类时，loadClass方法就会调用findClass方法来搜索类，所以我们只需重写该方法即可。如没有特殊的要求，一般不建议重写loadClass搜索类的算法。下图是API中ClassLoader的loadClass方法：

![](http://incdn1.b0.upaiyun.com/2015/03/4c0d13d3ad6cc317017872e51d01b238.jpg)

示例：自定义一个NetworkClassLoader，用于加载网络上的class文件



```
```java
package
```

```java
classloader;
```



```java

```



```java
import
```

```java
java.io.ByteArrayOutputStream;
```



```java
import
```

```java
java.io.InputStream;
```



```java
import
```

```java
java.net.URL;
```



```java

```



```java
/**
```



```java

```

```java
*
 加载网络class的ClassLoader
```



```java

```

```java
*/
```



```java
public
```

```java
class
```

```java
NetworkClassLoader
```

```java
extends
```

```java
ClassLoader
 {
```



```java

```



```java

```

```java
private
```

```java
String
 rootUrl;
```



```java

```



```java

```

```java
public
```

```java
NetworkClassLoader(String
 rootUrl) {
```



```java

```

```java
this
```

```java
.rootUrl
 = rootUrl;
```



```java

```

```java
}
```



```java

```



```java

```

```java
@Override
```



```java

```

```java
protected
```

```java
Class<?>
 findClass(String name)
```

```java
throws
```

```java
ClassNotFoundException
 {
```



```java

```

```java
Class
 clazz =
```

```java
null
```

```java
;
```

```java
//this.findLoadedClass(name);
 // 父类已加载
```



```java

```

```java
//if
 (clazz == null) {  //检查该类是否已被加载过
```



```java

```

```java
byte
```

```java
[]
 classData = getClassData(name);
```

```java
//根据类的二进制名称,获得该class文件的字节码数组
```



```java

```

```java
if
```

```java
(classData
 ==
```

```java
null
```

```java
)
 {
```



```java

```

```java
throw
```

```java
new
```

```java
ClassNotFoundException();
```



```java

```

```java
}
```



```java

```

```java
clazz
 = defineClass(name, classData,
```

```java
0
```

```java
,
 classData.length);
```

```java
//将class的字节码数组转换成Class类的实例
```



```java

```

```java
//}
```



```java

```

```java
return
```

```java
clazz;
```



```java

```

```java
}
```



```java

```



```java

```

```java
private
```

```java
byte
```

```java
[]
 getClassData(String name) {
```



```java

```

```java
InputStream
 is =
```

```java
null
```

```java
;
```



```java

```

```java
try
```

```java
{
```



```java

```

```java
String
 path = classNameToPath(name);
```



```java

```

```java
URL
 url =
```

```java
new
```

```java
URL(path);
```



```java

```

```java
byte
```

```java
[]
 buff =
```

```java
new
```

```java
byte
```

```java
[
```

```java
1024
```

```java
*
```

```java
4
```

```java
];
```



```java

```

```java
int
```

```java
len
 = -
```

```java
1
```

```java
;
```



```java

```

```java
is
 = url.openStream();
```



```java

```

```java
ByteArrayOutputStream
 baos =
```

```java
new
```

```java
ByteArrayOutputStream();
```



```java

```

```java
while
```

```java
((len
 = is.read(buff)) != -
```

```java
1
```

```java
)
 {
```



```java

```

```java
baos.write(buff,
```

```java
0
```

```java
,len);
```



```java

```

```java
}
```



```java

```

```java
return
```

```java
baos.toByteArray();
```



```java

```

```java
}
```

```java
catch
```

```java
(Exception
 e) {
```



```java

```

```java
e.printStackTrace();
```



```java

```

```java
}
```

```java
finally
```

```java
{
```



```java

```

```java
if
```

```java
(is
 !=
```

```java
null
```

```java
)
 {
```



```java

```

```java
try
```

```java
{
```



```java

```

```java
is.close();
```



```java

```

```java
}
```

```java
catch
```

```java
(IOException
 e) {
```



```java

```

```java
e.printStackTrace();
```



```java

```

```java
}
```



```java

```

```java
}
```



```java

```

```java
}
```



```java

```

```java
return
```

```java
null
```

```java
;
```



```java

```

```java
}
```



```java

```



```java

```

```java
private
```

```java
String
 classNameToPath(String name) {
```



```java

```

```java
return
```

```java
rootUrl
 +
```

```java
"/"
```

```java
+
 name.replace(
```

```java
"."
```

```java
,
```

```java
"/"
```

```java
)
 +
```

```java
".class"
```

```java
;
```



```java

```

```java
}
```



```java

```



```java
}
```
```



测试类：



```
```java
package
```

```java
classloader;
```



```java

```



```java
public
```

```java
class
```

```java
ClassLoaderTest {
```



```java

```



```java

```

```java
public
```

```java
static
```

```java
void
```

```java
main(String[] args) {
```



```java

```

```java
try
```

```java
{
```



```java

```

```java
/*ClassLoader
 loader = ClassLoaderTest.class.getClassLoader();  //获得ClassLoaderTest这个类的类加载器
```



```java

```

```java
while(loader
 != null) {
```



```java

```

```java
System.out.println(loader);
```



```java

```

```java
loader
 = loader.getParent();    //获得父加载器的引用
```



```java

```

```java
}
```



```java

```

```java
System.out.println(loader);*/
```



```java

```



```java

```



```java

```

```java
String
 rootUrl =
```

```java
"http://localhost:8080/httpweb/classes"
```

```java
;
```



```java

```

```java
NetworkClassLoader
 networkClassLoader =
```

```java
new
```

```java
NetworkClassLoader(rootUrl);
```



```java

```

```java
String
 classname =
```

```java
"org.classloader.simple.NetClassLoaderTest"
```

```java
;
```



```java

```

```java
Class
 clazz = networkClassLoader.loadClass(classname);
```



```java

```

```java
System.out.println(clazz.getClassLoader());
```



```java

```



```java

```

```java
}
```

```java
catch
```

```java
(Exception
 e) {
```



```java

```

```java
e.printStackTrace();
```



```java

```

```java
}
```



```java

```

```java
}
```



```java

```



```java
}
```
```



打印结果：

![](http://incdn1.b0.upaiyun.com/2015/03/8d8e353b98d5191d5ceea1aa3eb05d43.jpg)


下图是我机器上web服务器的目录结构：

![](http://incdn1.b0.upaiyun.com/2015/03/7bfc85c0d74ff05806e0b5a0fa0c1df1.jpg)


目前常用web服务器中都定义了自己的类加载器，用于加载web应用指定目录下的类库（jar或class），如：Weblogic、Jboss、tomcat等，下面我以Tomcat为例，展示该web容器都定义了哪些个类加载器：


1、新建一个web工程httpweb


2、新建一个ClassLoaderServletTest，用于打印web容器中的ClassLoader层次结构



```
```java
import
```

```java
java.io.IOException;
```



```java
import
```

```java
java.io.PrintWriter;
```



```java

```



```java
import
```

```java
javax.servlet.ServletException;
```



```java
import
```

```java
javax.servlet.http.HttpServlet;
```



```java
import
```

```java
javax.servlet.http.HttpServletRequest;
```



```java
import
```

```java
javax.servlet.http.HttpServletResponse;
```



```java

```



```java
public
```

```java
class
```

```java
ClassLoaderServletTest
```

```java
extends
```

```java
HttpServlet
 {
```



```java

```



```java

```

```java
public
```

```java
void
```

```java
doGet(HttpServletRequest request, HttpServletResponse response)
```



```java

```

```java
throws
```

```java
ServletException,
 IOException {
```



```java

```



```java

```

```java
response.setContentType(
```

```java
"text/html"
```

```java
);
```



```java

```

```java
PrintWriter
 out = response.getWriter();
```



```java

```

```java
ClassLoader
 loader =
```

```java
this
```

```java
.getClass().getClassLoader();
```



```java

```

```java
while
```

```java
(loader
 !=
```

```java
null
```

```java
)
 {
```



```java

```

```java
out.write(loader.getClass().getName()+
```

```java
""
```

```java
);
```



```java

```

```java
loader
 = loader.getParent();
```



```java

```

```java
}
```



```java

```

```java
out.write(String.valueOf(loader));
```



```java

```

```java
out.flush();
```



```java

```

```java
out.close();
```



```java

```

```java
}
```



```java

```



```java

```

```java
public
```

```java
void
```

```java
doPost(HttpServletRequest request, HttpServletResponse response)
```



```java

```

```java
throws
```

```java
ServletException,
 IOException {
```



```java

```

```java
this
```

```java
.doGet(request,
 response);
```



```java

```

```java
}
```



```java

```



```java
}
```
```




3、配置Servlet，并启动服务



```
```java
<?xml
 version=
```

```java
"1.0"
```

```java
encoding=
```

```java
"UTF-8"
```

```java
?>
```



```java
<web-app
 version=
```

```java
"2.4"
```



```java

```

```java
xmlns=
```

```java
"http://java.sun.com/xml/ns/j2ee"
```



```java

```

```java
xmlns:xsi=
```

```java
"http://www.w3.org/2001/XMLSchema-instance"
```



```java

```

```java
xsi:schemaLocation="http:
```

```java
//java.sun.com/xml/ns/j2ee
```





```java
http:
```

```java
//java.sun.com/xml/ns/j2ee/web-app_2_4.xsd">
```





```java

```

```java
<servlet>
```



```java

```

```java
<servlet-name>ClassLoaderServletTest</servlet-name>
```



```java

```

```java
<servlet-
```

```java
class
```

```java
>ClassLoaderServletTest</servlet-
```

```java
class
```

```java
>
```



```java

```

```java
</servlet>
```



```java

```



```java

```

```java
<servlet-mapping>
```



```java

```

```java
<servlet-name>ClassLoaderServletTest</servlet-name>
```



```java

```

```java
<url-pattern>/servlet/ClassLoaderServletTest</url-pattern>
```



```java

```

```java
</servlet-mapping>
```



```java

```

```java
<welcome-file-list>
```



```java

```

```java
<welcome-file>index.jsp</welcome-file>
```



```java

```

```java
</welcome-file-list>
```



```java
</web-app>
```
```




4、访问Servlet，获得显示结果

![](http://incdn1.b0.upaiyun.com/2015/03/c8b2f17833a4c73bb20f88876219ddcd.jpg)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=jdk&t=blog)](https://so.csdn.net/so/search/s.do?q=classloader&t=blog)](https://so.csdn.net/so/search/s.do?q=Java&t=blog)




