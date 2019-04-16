# 全栈必备  Java 基础 - 我相信...... - CSDN博客





2017年12月15日 21:23:40[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：2317
所属专栏：[全栈之路](https://blog.csdn.net/column/details/12818.html)









那一年，从北邮毕业，同一年，在大洋的彼岸诞生了一门对软件业将产生重大影响的编程语言，它就是——Java。1998年的时候，开始学习Java1.2，并在Java Orbix 上做服务，而如今Java 9 已经来了，而且 Java 10 也已经不远了。

对一个全栈而言，Java 是必备的编程语言之一。 而谈到Java，虽万语千言却不知从何开始，老码农从个人的角度看一下Java 语言的编程基础。

### 虚拟机

Java 真正牛X的地方就在于JVM。JVM是一个抽象的计算机，具有指令集、寄存器、垃圾回收堆、栈、存储区、类文件的格式等细节。所有平台上的JVM向上提供给Java字节码的接口完全相同，但向下提供适应不同平台的接口，规定了JVM的统一标准并实现了Java程序的平台无关性。这就是常说的，Java的跨平台，但跨越不同实现的JVM时还是有些许不同的。

JVM是运行java程序的核心虚拟机，而运行java程序不仅需要核心虚拟机，也需要其他的类加载器，字节码校验器以及大量的基础类库。JRE除了包含JVM之外还包含运行Java程序的其他环境支持。

当JVM启动时，由三个类加载器对类进行加载：

bootstrap classloader 是由JVM实现的，不是java.lang.ClassLoader的子类 ，负责加载Java的核心类，其加载的类由 sun.boot.class.path指定，或者在执行java命令时使用-Xbootclasspath选项, 还可以使用-D选项指定sun.boot.class.path系统属性值

extension classloader ，它负责加载JRE的扩展目录中JAR的类包，为引入除Java核心类以外的新功能提供了一个标准机制。

system/application classloader，加载来自-classpath或者java.class.path系统属性以及CLASSPATH操作系统属性所指定的JAR包和类路径。可以通过静态方法ClassLoader.getSystemClassLoader()找到该类加载器。如果没有特别指定，则用户自定义的任何类加载器都将该类加载器作为它的父加载器。

ClassLoader加载Class的一般过程如下： 
![这里写图片描述](https://img-blog.csdn.net/20171215084230187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lyZWxlc3NfY29t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
垃圾回收是JVM 中的一项重要技术。所谓垃圾回收只是针对内存资源，而对于物理资源如数据库连接、IO读写等JVM无能为力，所有程序中都需要显式释放。为了更快回收垃圾，可以将对象的引用变量设为null。垃圾回收具有不可预知性，即使调用了对象的finalize() ，System.gc()方法也不能确定何时回收，只是通知JVM而已。垃圾回收机制能精确标记活着的对象，能精确定位对象之间关系，前者是完全回收的前提，后者实现归并和复制等功能。现在JVM有多种不同的垃圾回收算法实现，不同的垃圾回收算法都有着典型的场景， 根据内存和cpu使用的不同可以对垃圾回收算法进行调整。

### 语法

作为一种编程语言，基本语法都是类似的，包括数据类型，操作符，语句，判断和分支，循环，递归等。

对于Java 的关键字可以做个文字游戏，排列成打油诗。

if volatile default， catch class short,  

abstract package private, throw this protected.

else char break, return super true, 

 instanceof interface long, switch null native.

while boolean case, try final static, 

 extends false transient, throws void public.

import new float, continue for double,  

implements int byte, do synchronized.

finally, goto const……

如果没有记错的话，goto 和 const 是 java 的保留字而不是关键字。弄清楚每个关键字的意义、用法、典型场景等，才算是“磨刀不误砍柴功”。

### 数据

java 中的基本类型有4类8种：整型（int, short, long, byte），浮点型（ float, double），逻辑型 boolean和 文本型 char。

Java中的基本数据结构大多在java.util 中体现，主要分为Collection和map两个主要接口，而程序中最终使用的数据结构则是继承自这些接口的数据结构类。

```
import java.util.Hashtable;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedHashMap;
import java.util.LinkedHashSet;
import java.util.LinkedList;
import java.util.Stack;
import java.util.TreeMap;
import java.util.TreeSet;
import java.util.Vector;
.....
```

一般的，一个空的对象需要占用12字节的堆空间，一个空的String就要占用40字节的堆空间，这或许就是推荐用stringbuilder的一个原因吧。在Java中，类型决定行为，例如byte可以起到限制数据的作用，但是并不能节约内存，在内存中byte和int一样是占用4字节的空间。一个对象的占用堆空间的多少一般与类中非static的基本数据类型和引用变量有关。每一个数组中的元素都是一个对象，每一个对象都有一个16字节的数组对象头。

回忆一下堆栈，Java 的堆是一个运行时数据区,类的对象从中分配空间。只有通过new()方法才能保证每次都创建一个新的对象，它们不需要程序代码来显式的释放。堆是由垃圾回收来负责的。Java的栈存取速度比堆要快，栈数据可以共享，存在栈中的数据大小与生存期必须是确定的，主要存放一些基本类型的变量和对象句柄。 


### 接口

抽象类和接口是Java 的两大利器， 抽象类是OOP 的共性，而接口则简单规范，提高了代码的可维护性和可扩展性，同时是软件松耦合的重要方式。对修改关闭，对扩展（不同的实现implements）开放，接口本身就是对开闭原则的一种体现。

Java接口是一系列方法的声明，是一些方法特征的集合，一个接口只有方法而没有方法的实现。弄一点玄虚，接口是一组规则的集合，它规定了实现本接口的类或接口必须拥有的一组规则，是在一定粒度上同类事物的抽象表示。

```
<修饰符>interface<接口名>{ 
[<常量声明>] 
[<抽象方法声明>] 
}
```

接口是类型转换的前提和动态调用的保证。实现某一接口就完成了类型的转换也就是多重继承，一般用来作为一个类型的等级结构的起点；动态调用则只关心类型，不关心具体类。接口可以为不同类顺利交互提供标准。

　　Java中的类描述了一个实体，包括实体的状态，也包括实体可能发出的动作。而接口定义了一个实体可能发出的动作，但只是定义了这些动作的原型，没有实现，也没有任何状态信息。所以接口有点象一个规范、一个协议，是一个抽象的概念；而类则是实现了这个协议，满足了这个规范的具体实体，是一个具体的概念。

　　从程序角度简单理解，接口就是函数声明，类就是函数实现。需要注意的是同一个声明可能有很多种实现。 　　

### 泛型

所谓“泛型”，就是宽泛的数据类型，任意的数据类型。Java 中的泛型是以C++模板为参照的，本质是参数化类型的应用，主要包括：

泛型类，例如：

```
public class MyGeneric<T,V> {
T obj_a;
V obj_b;
MyGeneric(T obj_1,V obj_2){
this.obj_a = obj_1;
this.obj_b = obj_2;
}
```

泛型接口，例如：

```php
interface MyInterface<T extends Comparable<T>>{
//...
}
```

泛型方法，例如：
`<T extends Comparator<T>, V extends T> boolean MyIn(T x, V[] y)`
泛型中的类型参数只能用来表示引用类型，不能用来表示基本类型，如 int、double、char 等。但是传递基本类型不会报错，因为它们会自动装箱成对应的包装类。类型参数必须是一个合法的标识符，习惯上使用单个大写字母，通常情况下，K 表示键，V 表示值，E 表示异常或错误，T 表示一般意义上的数据类型。

使用有界通配符，可以为参数类型指定上界和下界，从而能够限制方法能够操作的对象类型。最常用的是指定有界通配符上界，使用extends子句创建。 对于实现了`<? extends T>`的集合类只能将它视为生产者向外提供元素（get)，而不能作为消费者来对外获取元素(add)。

Java泛型只能用于在编译期间的静态类型检查，然后编译器生成的代码会擦除相应的类型信息，这样到了运行期间实际上JVM根本就知道泛型所代表的具体类型。在Java中不允许创建泛型数组，无法对泛型代码直接使用instanceof。

使用泛型，可以消除显示的强制类型转换，提高代码复用，还可以提供更强的类型检查，避免运行时的ClassCastException。

### 反射

JAVA反射机制是在运行状态中，对于任意一个类，都能够知道这个类的所有属性和方法；对于任意一个对象，都能够调用它的任意一个方法。普通调用需要在编译前必须了解所有的class，包括成员变量，成员方法，继承关系等。而反射可以于运行时加载、探知、使用编译期间完全未知的类。也就是说，Java程序可以加载一个运行时才得知名称的class，获悉其完整构造。

Java反射的方式主要分为两类：Java.lang.reflect.*和Cg-lib工具包。

因为在反射调用中同样要遵循java的可见性规约，因此Class.getMethod方法只能查找到该类的public方法。如果要获取声明为private的方法对象，则需要通过Class.getDeclaredMethod，而且在invoke前要设置setAccessable(true)才能保证调用成功。如果的确需要调用父类方法，可以通过Class.getInterface方法查找父类，再实例化一个父类对象，然后按照调用private Method的方式进行调用。

反射的应用广泛，例如Spring容器的注入，就是运用了反射的方式，通过配置文件读取欲实例化的类的名称，属性，然后由spring容器统一实例化，既达到了注入的目的，又可以通过容器统一控制bean的作用域、生命周期等。J

在框架和容器中，比较广泛的就是java bean的规范，或者POJO，以及一些作为与数据库交互载体的持久化对象，都会有要求：
- 每个field都要有setXxx/getXxx方法，命名符合驼峰命名法，且需要声明为public的。 
- 含有一个无参的构造方法。

第一条就是为了方便反射属性值，通过get/set方法。另一条是为了保证可以通过cls.newInstance()实例化一个新对象。 另外还有servlet（要有init、service、doGet、doPost方法），filter（要有doFilter方法）。这些组件定义的规范就是为了容器可以通过反射的方式进行统一调用和管理。

ava.lang.reflect包中还自带了代理模式的一个实现，静态代理和动态代理都是有意思的事， 很多插件化开发都使用了代理模式。

### 注解

注解这种机制允许在编写代码的同时可以直接编写元数据。注解就是代码的元数据，包含了代码自身的信息。

注解可以被用在包，类，方法，变量，参数上。自Java8开始，有一种注解几乎可以被放在代码的任何位置，叫做类型注解。被注解的代码并不会直接被注解影响，只会向第三系统提供关于自己的信息以用于不同的需求。注解会被编译至class文件中，而且会在运行时被处理程序提取出来用于业务逻辑。当然，创建在运行时不可用的注解也是可能的，甚至可以创建只在源文件中可用，在编译时不可用的注解。

Java自带的内建注解可以叫元注解，由JVM 对这些注解进行执行。常见的元注解如下：

@Retention：用来说明如何存储已被标记的注解，值包括：SOURCE， CLASS和RUNTIME。

@Target：这个注解用于限制某个元素可以被注解的类型。例如：

ANNOTATION_TYPE ：应用到其他注解上 

CONSTRUCTOR：使用到构造器上 

FIELD：使用到域或属性上 

LOCAL_VARIABLE：使用到局部变量上。 

METHOD：使用到方法级别的注解上。 

PACKAGE：使用到包声明上 

PARAMETER：使用到方法的参数上 

TYPE：使用到一个类的任何元素上。 

@Documented：被注解的元素将会作为Javadoc产生的文档中的内容，都默认不会成为成为文档中的内容。这个注解可以对其它注解使用。
@Inherited：在默认情况下，注解不会被子类继承。被此注解标记的注解会被所有子类继承。

还有 @Deprecated，@SuppressWarnings，@Override等等。

Java反射API包含了许多方法来在运行时从类、方法或者其它元素获取注解的手段。接口AnnotatedElement包含了大部分重要的方法，如下：
- getAnnotations(): 返回该元素的所有注解，包括没有显式定义该元素上的注解。
- isAnnotationPresent(annotation): 检查传入的注解是否存在于当前元素。
- getAnnotation(class): 按照传入的参数获取指定类型的注解。返回null说明当前元素不带有此注解。

自己写个注解，会让代码变得简洁。一些类库如：JAXB, Spring Framework, Findbugs, Log4j, Hibernate, Junit等，使用注解来完成代码质量分析，单元测试，XML解析，依赖注入和许多其它的工作。

### 线程

一个JVM 相当于操作系统的一个进程，Java线程是进程的一个实体，是CPU调度和分派的基本单位，JVM线程调度程序是基于优先级的抢先调度机制。 线程有自己的堆栈和局部变量，但线程之间没有单独的地址空间，一个线程包含以下内容：
- 一个指向当前被执行指令的指令指针
- 一个栈 
- 一个寄存器值的集合，定义了一部分描述正在执行线程的处理器状态的值 
- 一个私有的数据区

在 Java程序中，有两种方法创建线程：对 Thread 类进行派生并覆盖 run方法和通过实现Runnable接口创建。获取当前线程的对象的方法是Thread.currentThread()。实现Runnable接口相对于继承Thread类而言，更适合多个相同的程序代码的线程去处理同一个资源，绕过单继承限制，而且线程池只能放入实现Runable或callable类线程，一般不直接放入继承Thread的类。

线程池的基本思想还是一种对象池的思想，开辟一块内存空间，里面存放了众多（未死亡）的线程，池中线程执行调度由池管理器来处理。当有线程任务时，从池中取一个，执行完成后线程对象归池，这样可以避免反复创建线程对象所带来的性能开销，节省了系统的资源。线程池分好多种：固定尺寸的线程池、单任务线程池、可变尺寸连接池、延迟连接池、自定义线程池等等。

理解Java线程的状态机（新建，就绪，运行，睡眠／阻塞／等待，消亡等）对于线程的使用很有帮助。 
![图片来自<a href=](https://img-blog.csdn.net/20171215085238350?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lyZWxlc3NfY29t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)http://blog.csdn.net/Evankaka/article/details/44153709” title=”” />
在使用任何多线程技术的时候，都要关注线程安全。尽管线程安全类中封装了必要的同步机制，从而客户端无须进一步采取同步措施，但还是要关注一下资源竞争即所谓的竞态条件。竞态条件成立的三个条件： 
- 两个处理共享变量 
- 至少一个处理会对变量进行修改 
- 一个处理未完成前另一个处理会介入进来

只要三个条件有一个不具备，就可以写线程安全的程序了。 规避一，没有共享内存，就不存在竞态条件了，例如利用独立进程和actor模型。 规避二，比如Java中的immutable 规避三,不介入，使用协调模式的线程如coroutine等，也可以使用表示不便介入的标识——锁、mutex、semaphore，实际上是使用中的状态牌。锁的使用问题包括死锁和无法组合，只能寄托于事务内存来奢望解决了。

通过Java多线程技术，可以提高资源利用率，程序拥有更好的响应。

### 排错

Zero Bug 是每个程序员的目标， debug 是项繁重的工作，减少bug一般从Error Handling 开始，在Java 中主要体现在异常处理。

#### 异常处理

Java 中 Exception的继承关系如下图：

![图片来自<a href=](https://img-blog.csdn.net/20171215085503340?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lyZWxlc3NfY29t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)https://www.programcreek.com/2013/09/top-8-diagrams-for-understanding-java/ ” title=”” />

红色部分为必须被捕获，或者在函数中声明为抛出该异常。其中，throwable 是一个有趣的东西， 在某些极端情况下， 直接catch throwable 才能得到想要的效果。

#### 静态代码分析

据说，在整个软件开发生命周期中，30% 至 70% 的代码逻辑设计和编码缺陷是可以通过静态代码分析来发现和修复的。但是，code review 往往要求大量的时间消耗和相关知识的积累，因此使用静态代码分析工具自动化执行代码检查和分析，能够极大地提高软件可靠性并节省软件开发和测试成本。

静态代码分析是指无需运行被测代码，仅通过分析或检查源程序的语法、结构、过程、接口等来检查程序的正确性，找出代码隐藏的错误和缺陷，如参数不匹配，有歧义的嵌套语句，错误的递归，非法计算，可能出现的空指针引用等等。静态代码分析主要是基于缺陷模式匹配，类型推断，模型检查和数据流分析等。

通过静态代码分析工具可以自动执行静态代码分析，快速定位代码隐藏错误和缺陷；帮助我们更专注于分析和解决bug；显著减少在代码逐行检查上花费的时间，提高软件可靠性并节省软件开发和测试成本。

常用的静态代码工具有checkstyle，findbugs，PMD等，其中Checkstyle 更加偏重于代码编写格式检查，而 FindBugs，PMD，Jtest 等着重于发现代码缺陷，但个人还是喜欢Sonar。

#### 内存泄漏

在Java中排错的一个麻烦就是内存泄露。内存泄漏是指无用对象持续占用内存或无用对象的内存得不到及时释放，从而造成内存空间的浪费。内存泄露有时不严重且不易察觉，这样可能不知道存在内存泄露，但有时也会很严重，会引发Out of memory。

常用的Java内存分析工具有VisualVM、jconsole、jhat、JProfiler、Memory Analyzer (MAT)等。考虑能处理的Heapdump大小及速度，网络环境，可视化分析，内存资源限制，是否免费使用等，推荐的工具为jmap + MAT。

Java中内存分析的一般步骤如下：
- 
把Java应用程序使用的堆dump下来，启动时加虚拟机参数： -XX:+HeapDumpOnOutOfMemoryError 

-XX:HeapDumpPath=path，这样在程序发生OOM时，会自动在相关路径下生成dump文件

- 
然后使用Java heap分析工具，找出对象数量或占用内存太多的对象 执行jmap 

-dump:format=b,file=heap.bin pid 其中，format=b，表示dump出来的文件是二进制格式，file=heap.bin，表示dump出来的文件名是heap.bin，pid是进程号。

- 
需要分析嫌疑对象和其他对象的引用关系，结合程序的源代码，找出原因。 

可以将Heapdump拉到本地，使用MAT打开进行分析。如果Heapdump较大，本地内存不够，可以在服务器上执行sh 

ParseHeapDump.sh Heapdumpfile，得到分解后的文件，然后拉到本地，再使用MAT打开，就可以进一步分析了。
### 不是小结的小结

Java 博大精深，技术栈很多，有关Java 的书也是浩如烟海，这里只谈到了个人认为的一些Java基础，包括 虚拟机，语法，数据结构，接口，泛型，反射，注解，线程，排错等几个部分。

“勿在浮沙筑高塔”，基础扎实一点，才可能走的远一点。《深入分布式缓存》一书中的代码实现几乎都是Java的， 有兴趣的朋友可以看一下。 
![这里写图片描述](https://img-blog.csdn.net/20171215085822982?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lyZWxlc3NfY29t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

京东上的购买链接，呵呵。 
![这里写图片描述](https://img-blog.csdn.net/20171215085902812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lyZWxlc3NfY29t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)








