
# BAT经典面试题精简版（基础知识附答案） - Magician的博客 - CSDN博客


2017年11月04日 13:34:00[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：794



### 文章目录
[目录](#_2)
[J2SE基础](#J2SE_4)
[JVM](#JVM_121)
[操作系统](#_176)
[TCP/IP](#TCPIP_208)
[数据结构与算法](#_317)

# 目录
## J2SE基础
九种基本数据类型的大小，以及他们的封装类。
原始类型封装类
booleanBoolean
charCharacter
byteByte
shortShort
intInteger
longLong
floatFloat
doubleDouble
Switch能否用string做参数？
在switch（expr1）中，expr1只能是一个整数表达式或者枚举常量（更大字体），整数表达式可以是int基本类型或Integer包装类型，由于，byte,short,char都可以隐含转换为int，所以，这些类型以及这些类型的包装类型也是可以的。显然，long和String类型都不符合switch的语法规定，并且不能被隐式转换成int类型，所以，它们不能作用于swtich语句中。
equals与的区别。
操作符专门用来比较两个变量的值是否相等，也就是用于比较变量所对应的内存中所存储的数值是否相同，要比较两个基本类型的数据或两个引用变量是否相等，只能用操作符。
如果一个类没有自己定义equals方法，它默认的equals方法（从Object 类继承的）就是使用操作符，也是在比较两个变量指向的对象是否是同一对象，这时候使用equals和使用==会得到同样的结果，如果比较的是两个独立的对象则总返回false。如果你编写的类希望能够比较该类创建的两个实例对象的内容是否相同，那么你必须覆盖equals方法，由你自己写代码来决定在什么情况即可认为两个对象的内容是相同的。
Object有哪些公用方法？
Java的四种引用，强弱软虚，用到的场景。
Hashcode的作用。
判断两个对象是否相等。
ArrayList、LinkedList、Vector的区别。
List的子类特点
ArrayList:
底层数据结构是数组，查询快，增删慢
线程不安全，效率高
Vector:
底层数据结构是数组，查询快，增删慢
线程安全，效率低
LinkedList:
底层数据结构是链表，查询慢，增删快
线程不安全，效率高
String、StringBuffer与StringBuilder的区别。
String类创建数值不会改变的对象。
StringBuffer类创建数值会改变的对象，并且可以同步，线程安全效率低。
StringBuilder类创建数值会改变的对象不可以同步，线程不安全效率高。
Map、Set、List、Queue、Stack的特点与用法。
List 以特定次序来持有元素，可有重复元素。
Set 无法拥有重复元素,内部排序。
Map 保存key-value值，value可多值。
HashMap和HashTable的区别。
HashMap允许键和值是null，而Hashtable不允许键或者值是null。
Hashtable是同步的，而HashMap不是。因此，HashMap更适合于单线程环境，而Hashtable适合于多线程环境。
HashMap和ConcurrentHashMap的区别，HashMap的底层源码。
TreeMap、HashMap、LindedHashMap的区别。
Collection包结构，与Collections的区别。
Collection是集合类的上级接口，继承与他的接口主要有Set 和List.
Collections是针对集合类的一个帮助类，他提供一系列静态方法实现对各种集合的搜索、排序、线程安全化等操作。
try catch finally，try里有return，finally还执行么？
可以
Excption与Error包结构。OOM你遇到过哪些情况，SOF你遇到过哪些情况。
OOM：内存溢出
SOF：堆溢出
Java面向对象的三个特征与含义。
继承、封装、多态
Override和Overload的含义去区别。
方法的重写Overriding和重载Overloading是Java多态性的不同表现。重写Overriding是父类与子类之间多态性的一种表现，重载O verloading是一个类中多态性的一种表现。如果在子类中定义某方法与其父类有相同的名称和参数，我们说该方法被重写(Overriding)。子类的对象使用这个方法时，将调用子类中的定义，对它而言，父类中的定义如同被”屏蔽”了。如果在一个类中定义了多个同名的方法，它们或有不同的参数个数或有不同的参数类型，则称为方法的重载(Overloading)。Overloaded的方法是可以改变返回值的类型。
Interface与abstract类的区别。
一个类可以实现多个接口，但只能继承一个抽象类。
接口中的成员属性都是公有的，而抽象类中的可以是各种类型。
Static class 与non static class的区别。
静态类可以直接通过类来调用，非静态类则只能通过类的实例来调用。
java多态的实现原理。
实现多线程的两种方法：继承Thread与实现Runable接口。
线程同步的方法：sychronized、lock、reentrantLock等。
锁的等级：方法锁、对象锁、类锁。
写出生产者消费者模式。
ThreadLocal的设计理念与作用。
ThreadPool用法与优势。
Concurrent包里的其他东西：ArrayBlockingQueue、CountDownLatch等等。
wait()和sleep()的区别。
对象执行wait（）时，需要释放对象锁。而执行sleep（）时不用释放对象锁。
foreach与正常for循环效率对比。
Java IO与NIO。
传统的IO在对数据进行处理时需要在操作系统中从内核态转换到用户态然后再从用户态转换为内核态，中间消耗很多时间。
传统IO示意图：
![这里写图片描述](https://img-blog.csdn.net/20180131154303843?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
先将文件内容从磁盘中拷贝到操作系统buffer
再从操作系统buffer拷贝到程序应用buffer
从程序buffer拷贝到socket buffer
从socket buffer拷贝到协议引擎.
NIO是New IO指 的是在对数据进行IO处理时，可以不用做状态的转换。如下图所示：![这里写图片描述](https://img-blog.csdn.net/20180131154344385?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
NIO技术省去了将操作系统的read buffer拷贝到程序的buffer, 以及从程序buffer拷贝到socket buffer的步骤, 直接将 read buffer 拷贝到 socket buffer. java 的 FileChannel.transferTo() 方法就是这样的实现, 这个实现是依赖于操作系统底层的sendFile()实现的.
[
](https://img-blog.csdn.net/20180131154344385?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)反射的作用和原理。
通过字节码文件对象（指的就是类的全称），去使用任意成员变量，构造方法，成员方法
泛型常用特点，List能否转为List。
解析XML的几种方式的原理与特点：DOM、SAX、PULL。
Java与C++对比。
Java1.7与1.8新特性。
设计模式：单例、工厂、适配器、责任链、观察者等等。
JNI的使用。
Java里有很多很杂的东西，有时候需要你阅读源码，大多数可能书里面讲的不是太清楚，需要你在网上寻找答+案。
推荐书籍：《java核心技术卷I》《Thinking in java》《java并发编程》《effictive java》《大话设计模式》
JVM
内存模型以及分区，需要详细到每个区放什么。
Java虚拟机运行时数据区域被分为五个区域：堆(Stack)、栈(Heap)、本地方法栈(Native Stack)、方法区(Method Area)、程序计数器(Program Count Register).
![这里写图片描述](https://img-blog.csdn.net/20180131221042967?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
堆：我们创建的对象和数组就是存放在堆里面（ Java Stack可以处于物理上不连续的内存空间中，只要逻辑上是连续的即可。与Java Stack相关的还有Java的垃圾回收机制（GC）,Java Stack是垃圾回收器管理的主要区域。Java Stack是一块共享的区域，操作共享区域的成员就有了锁和同步。）
[
](https://img-blog.csdn.net/20180131221042967?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)栈：Java Heap是线程私有的，她的生命周期与线程相同。Java Heap描述的是Java方法执行时的内存模型，每个方法执行时都会创建一个栈帧（Heap Frame）用于存储局部变量表、操作数栈、动态链接、方法出口等信息（局部变量表存放了编译期间的各种基本数据类型，对象引用等信息。）
[
](https://img-blog.csdn.net/20180131221042967?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
本地方法栈：本地方法栈（Native Heap）与Java虚拟机站（Java Heap）所发挥的作用非常相似，他们之间的区别在于虚拟机栈为虚拟机栈执行java方法（也就是字节码）服务，而本地方法栈则为使用到Native方法服务。
**方法区：**方法区（Method Area）与堆（Java Stack）一样，是各个线程共享的内存区域，它用于存储虚拟机加载的类信息，常量，静态变量，即时编译器编译后的代码等数据
总结：
1、  线程私有的数据区域有：
Java虚拟机栈（Java Heap）
本地方法栈（Native Heap）
2、  线程共有的数据区域有：
堆（Java Stack）
方法区
堆里面的分区：Eden，survival from to，老年代，各自的特点。
对象创建方法，对象的内存分配，对象的访问定位。
GC的两种判定方法：引用计数与引用链。
GC的三种收集方法：标记清除、标记整理、复制算法的原理与特点，分别用在什么地方，如果让你优化收集方法，有什么思路？
标记-清除算法（Mark-sweep）
标记出所有需要回收的对象，在标记完成后统一回收所有被标记的对象
缺点：一个是效率问题，标记和清除两个过程的效率都不高；
另一个是空间问题，标记清除之后会产生大量不连续的内存碎片，空间碎片太多可能会导致以后在程序运行过程中需要分配较大对象时，无法找到足够的连续内存而不得不提前触发另一次垃圾收集动作。
![这里写图片描述](https://img-blog.csdn.net/20180131220830941?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
复制算法（Copying）
1、将可用内存按容量划分为大小相等的两块，每次只使用其中的一块。
2、当这一块的内存用完了，就将还存活着的对象复制到另外一块上面，然后再把已使用过的内存空间一次清理掉。
优点：这样使得每次都是对整个半区进行内存回收，内存分配时也就不用考虑内存碎片等复杂情况，只要移动堆顶指针，按顺序分配内存即可，实现简单，运行高效。只是这种算法的代价是将内存缩小为了原来的一半，未免太高了一点。
缺点：复制收集算法在对象存活率较高时就要进行较多的复制操作，效率将会变低
![这里写图片描述](https://img-blog.csdn.net/20180131220840792?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
GC收集器有哪些？CMS收集器与G1收集器的特点。
Minor GC与Full GC分别在什么时候发生？
几种常用的内存调试工具：jmap、jstack、jconsole。
类加载的五个过程：加载、验证、准备、解析、初始化。
双亲委派模型：Bootstrap ClassLoader、Extension ClassLoader、ApplicationClassLoader。
分派：静态分派与动态分派。
JVM过去过来就问了这么些问题，没怎么变，内存模型和GC算法这块问得比较多，可以在网上多找几篇博客来看看。
推荐书籍：《深入理解java虚拟机》
操作系统
进程和线程的区别。
进程：可以看做是一个正在执行的应用程序。
线程：进程的执行单元，可以看做是程序的某一个功能。（线程是操作系统能够进行运算调度的最小单位，它被包含在进程之中，是进程中的实际运作单位）
死锁的必要条件，怎么处理死锁。
两个线程互相等待对方的处理结果。线程在获取锁的时候声明获取锁的顺序。
Window内存管理方式：段存储，页存储，段页存储。
进程的几种状态。
（1）运行(running)态：进程占有处理器正在运行。
（2）就绪(ready)态：进程具备运行条件，等待系统分配处理器以便运行。
（3）等待(wait)态：又称为阻塞(blocked)态或睡眠(sleep)态，指进程不具备运行条件，正在等待某个事件的完成。
![这里写图片描述](https://img-blog.csdn.net/20180131231255039?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
除了三态模式外还有七态模式。
[
](https://img-blog.csdn.net/20180131231255039?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
IPC<进程间通信(IPC,Inter-Process Communication)>几种通信方式。
（1）、管道(pipe)：管道可用于具有亲缘关系的进程间的通信，是一种半双工的方式，数据只能单向流动，允许一个进程和另一个与它有共同祖先的进程之间进行通信。
（2）、命名管道(named pipe)：命名管道克服了管道没有名字的限制，同时除了具有管道的功能外（也是半双工），它还允许无亲缘关系进程间的通信。命名管道在文件系统中有对应的文件名。命名管道通过命令mkfifo或系统调用mkfifo来创建。
（3）、信号（signal）：信号是比较复杂的通信方式，用于通知接收进程有某种事件发生了，除了进程间通信外，进程还可以发送信号给进程本身；linux除了支持Unix早期信号语义函数sigal外，还支持语义符合Posix.1标准的信号函数sigaction（实际上，该函数是基于BSD的，BSD为了实现可靠信号机制，又能够统一对外接口，用sigaction函数重新实现了signal函数）。
（4）、消息队列：消息队列是消息的链接表，包括Posix消息队列system V消息队列。有足够权限的进程可以向队列中添加消息，被赋予读权限的进程则可以读走队列中的消息。消息队列克服了信号承载信息量少，管道只能承载无格式字节流以及缓冲区大小受限等缺
（5）、共享内存：使得多个进程可以访问同一块内存空间，是最快的可用IPC形式。是针对其他通信机制运行效率较低而设计的。往往与其它通信机制，如信号量结合使用，来达到进程间的同步及互斥。
（6）、内存映射：内存映射允许任何多个进程间通信，每一个使用该机制的进程通过把一个共享的文件映射到自己的进程地址空间来实现它。
（7）、信号量（semaphore）：主要作为进程间以及同一进程不同线程之间的同步手段。
（8）、套接字（Socket）：更为一般的进程间通信机制，可用于不同机器之间的进程间通信。起初是由Unix系统的BSD分支开发出来的，但现在一般可以移植到其它类Unix系统上：Linux和System V的变种都支持套接字。
什么是虚拟内存。
虚拟地址、逻辑地址、线性地址、物理地址的区别。
推荐书籍：《深入理解现代操作系统》
TCP/IP
OSI与TCP/IP各层的结构与功能，都有哪些协议。
![这里写图片描述](https://img-blog.csdn.net/20180131231501310?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
网络模型7层概述：
1.物理层：主要定义物理设备标准，如网线的接口类型、光纤的接口类型、各种传输介质的传输速率等。它的主要作用是传输比特流（就是由1、0转化为电流强弱来进行传输,到达目的地后在转化为1、0，也就是我们常说的数模转换与模数转换）。这一层的数据叫做比特。
2.数据链路层：主要将从物理层接收的数据进行MAC地址（网卡的地址）的封装与解封装。常把这一层的数据叫做帧。在这一层工作的设备是交换机，数据通过交换机来传输。
3.网络层：主要将从下层接收到的数据进行IP地址（例192.168.0.1)的封装与解封装。在这一层工作的设备是路由器，常把这一层的数据叫做数据包。
4.传输层：定义了一些传输数据的协议和端口号（WWW端口80等），如：TCP（传输控制协议，传输效率低，可靠性强，用于传输可靠性要求高，数据量大的数据），UDP（用户数据报协议，与TCP特性恰恰相反，用于传输可靠性要求不高，数据量小的数据，如QQ聊天数据就是通过这种方式传输的）。 主要是将从下层接收的数据进行分段和传输，到达目的地址后再进行重组。常常把这一层数据叫做段。
5.会话层：通过传输层（端口号：传输端口与接收端口）建立数据传输的通路。主要在你的系统之间发起会话或者接受会话请求（设备之间需要互相认识可以是IP也可以是MAC或者是主机名）
6.表示层：主要是进行对接收的数据进行解释、加密与解密、压缩与解压缩等（也就是把计算机能够识别的东西转换成人能够能识别的东西（如图片、声音等）。
7.应用层： 主要是一些终端的应用，比如说FTP（各种文件下载），WEB（IE浏览），QQ之类的（可以把它理解成我们在电脑屏幕上可以看到的东西．就是终端应用）。TCP与UDP的区别。
UDP:数据打包,有限制,不连接,效率高,不可靠
TCP:建立数据通道,无限制,效率低,可靠
TCP报文结构。
TCP的三次握手与四次挥手过程，各个状态名称与含义，TIMEWAIT的作用。
TCP（Transmission Control Protocol，传输控制协议）是 面向连接的协议，也就是说在收发数据之前，必须先和对方建立连接，一个TCP连接必须要经过三次“对话”才能建立起来，其中的过程非常复杂，只简单的 描述下这三次对话的简单过程：主机A向主机B发出连接请求数据包：“我想给你发数据，可以吗？”，这是第一次对话；主机B向主机A发送同意连接和要求同步 （同步就是两台主机一个在发送，一个在接收，协调工作）的数据包：“可以，你什么时候发？”，这是第二次对话；主机A再发出一个数据包确认主机B的要求同 步：“我现在就发，你接着吧！”，这是第三次对话。三次“对话”的目的是使数据包的发送和接收同步，经过三次“对话”之后，主机A才向主机B正式发送数 据。
需要了解的信息：
ACK ： TCP协议规定，只有ACK=1时有效，也规定连接建立后所有发送的报文的ACK必须为1
SYN(SYNchronization) ： 在连接建立时用来同步序号。当SYN=1而ACK=0时，表明这是一个连接请求报文。对方若同意建立连接，则应在响应报文中使SYN=1和ACK=1. 因此,  SYN置1就表示这是一个连接请求或连接接受报文。
FIN （finis）即完，终结的意思， 用来释放一个连接。当 FIN = 1 时，表明此报文段的发送方的数据已经发送完毕，并要求释放连接。
![这里写图片描述](https://img-blog.csdn.net/20180204170959442?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
三次握手：
![这里写图片描述](https://img-blog.csdn.net/20180204170821139?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
三次握手
**第一次握手：**主机A发送位码为syn＝1,随机产生seq number=10001的数据包到服务器，主机B由SYN=1知道，A要求建立联机，此时状态为SYN_SENT；
**第二次握手：**主机B收到请求后要确认联机信息，向A发送ack number=(主机A的seq+1),syn=1,ack=1,随机产生seq=20001的包，此时状态由LISTEN变为SYN_RECV；
**第三次握手：**主机A收到后检查ack number是否正确，即第一次发送的seq number+1,以及位码ack是否为1，若正确，主机A会再发送ack number=(主机B的seq+1),ack=1，主机B收到后确认seq值与ack=1则连接建立成功，双方状态ESTABLISHED。
完成三次握手，主机A与主机B开始传送数据
各个状态名称与含义
CLOSED: 这个没什么好说的了，表示初始状态。
LISTEN: 这个也是非常容易理解的一个状态，表示服务器端的某个SOCKET处于监听状态，可以接受连接了。
SYN_RECV: 这个状态表示接受到了SYN报文，在正常情况下，这个状态是服务器端的SOCKET在建立TCP连接时的三次握手会话过程中的一个中间状态，很短暂，基本 上用netstat你是很难看到这种状态的，除非你特意写了一个客户端测试程序，故意将三次TCP握手过程中最后一个ACK报文不予发送。因此这种状态 时，当收到客户端的ACK报文后，它会进入到ESTABLISHED状态。
SYN_SENT: 这个状态与SYN_RECV遥想呼应，当客户端SOCKET执行CONNECT连接时，它首先发送SYN报文，因此也随即它会进入到了SYN_SENT状 态，并等待服务端的发送三次握手中的第2个报文。SYN_SENT状态表示客户端已发送SYN报文。
ESTABLISHED：这个容易理解了，表示连接已经建立了。
四次挥手图示（TCP的关闭）：
![这里写图片描述](https://img-blog.csdn.net/20180204171154417?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
TCP连接是全双工的，可以同时发送和接受数据，关闭的时候要关闭这两个方向的通道
[
](https://img-blog.csdn.net/20180204171154417?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
第一次挥手：Client给Server发送FIN，请求关闭连接
第二次挥手：Server收到FIN之后给Client返回确认ACK，同时关闭Receive通道，Client收到对自己的FIN确认后，关闭Send通道
第三次挥手： Server关闭连接，给Client发送FIN
第四次挥手：Client收到后给Server回复ACK确认，同时Client关闭Receive通道，进入TIME_WAIT状态。Server接收到Client对自己的FIN的确认ACK，关闭Send通道
•	FIN_WAIT_1: 这个状态要好好解释一下，其实FIN_WAIT_1和FIN_WAIT_2状态的真正含义都是表示等待对方的FIN报文。而这两种状态的区别 是：FIN_WAIT_1状态实际上是当SOCKET在ESTABLISHED状态时，它想主动关闭连接，向对方发送了FIN报文，此时该SOCKET即 进入到FIN_WAIT_1状态。而当对方回应ACK报文后，则进入到FIN_WAIT_2状态，当然在实际的正常情况下，无论对方何种情况下，都应该马 上回应ACK报文，所以FIN_WAIT_1状态一般是比较难见到的，而FIN_WAIT_2状态还有时常常可以用netstat看到。
FIN_WAIT_2：上面已经详细解释了这种状态，实际上FIN_WAIT_2状态下的SOCKET，表示半连接，也即有一方要求close连接，但另外还告诉对方，我暂时还有点数据需要传送给你，稍后再关闭连接。
TIME_WAIT: 表示收到了对方的FIN报文，并发送出了ACK报文，就等2MSL后即可回到CLOSED可用状态了。如果FIN_WAIT_1状态下，收到了对方同时带 FIN标志和ACK标志的报文时，可以直接进入到TIME_WAIT状态，而无须经过FIN_WAIT_2状态。
CLOSING: 这种状态比较特殊，实际情况中应该是很少见，属于一种比较罕见的例外状态。正常情况下，当你发送FIN报文后，按理来说是应该先收到（或同时收到）对方的 ACK报文，再收到对方的FIN报文。但是CLOSING状态表示你发送FIN报文后，并没有收到对方的ACK报文，反而却也收到了对方的FIN报文。什 么情况下会出现此种情况呢？其实细想一下，也不难得出结论：那就是如果双方几乎在同时close一个SOCKET的话，那么就出现了双方同时发送FIN报 文的情况，也即会出现CLOSING状态，表示双方都正在关闭SOCKET连接。
CLOSE_WAIT: 这种状态的含义其实是表示在等待关闭。怎么理解呢？当对方close一个SOCKET后发送FIN报文给自己，你系统毫无疑问地会回应一个ACK报文给对 方，此时则进入到CLOSE_WAIT状态。接下来呢，实际上你真正需要考虑的事情是察看你是否还有数据发送给对方，如果没有的话，那么你也就可以 close这个SOCKET，发送FIN报文给对方，也即关闭连接。所以你在CLOSE_WAIT状态下，需要完成的事情是等待你去关闭连接。
LAST_ACK: 这个状态还是比较容易好理解的，它是被动关闭一方在发送FIN报文后，最后等待对方的ACK报文。当收到ACK报文后，也即可以进入到CLOSED可用状态了。
•	为什么建立连接协议是三次握手，而关闭连接却是四次握手呢？
这 是因为服务端的LISTEN状态下的SOCKET当收到SYN报文的建连请求后，它可以把ACK和SYN（ACK起应答作用，而SYN起同步作用）放在一 个报文里来发送。但关闭连接时，当收到对方的FIN报文通知时，它仅仅表示对方没有数据发送给你了；但未必你所有的数据都全部发送给对方了，所以你可以未 必会马上会关闭SOCKET,也即你可能还需要发送一些数据给对方之后，再发送FIN报文给对方来表示你同意现在可以关闭连接了，所以它这里的ACK报文 和FIN报文多数情况下都是分开发送的。
•	为什么TIME_WAIT状态还需要等2MSL后才能返回到CLOSED状态？
因为虽然双方都同意关闭连接了，而且握手的4个报文也都发送完毕，按理可以直接回到CLOSED 状态(就好比从SYN_SENT 状态到ESTABLISH 状态那样)，但是我们必须假想网络是不可靠的，你无法保证你(客户端)最后发送的ACK报文一定会被对方收到，就是说对方处于LAST_ACK 状态下的SOCKET可能会因为超时未收到ACK报文，而重发FIN报文，所以这个TIME_WAIT 状态的作用就是用来重发可能丢失的ACK报文。
•	关闭TCP连接一定需要4次挥手吗?
不一定，4次挥手关闭TCP连接是最安全的做法。但在有些时候，我们不喜欢TIME_WAIT 状态(如当MSL数值设置过大导致服务器端有太多TIME_WAIT状态的TCP连接，减少这些条目数可以更快地关闭连接，为新连接释放更多资源)，这时我们可以通过设置SOCKET变量的SO_LINGER标志来避免SOCKET在close()之后进入TIME_WAIT状态，这时将通过发送RST强制终止TCP连接(取代正常的TCP四次握手的终止方式)。但这并不是一个很好的主意，TIME_WAIT 对于我们来说往往是有利的。
TCP拥塞控制。
TCP滑动窗口与回退N针协议。
Http的报文结构。
Http的状态码含义。
Http request的几种类型。
Http1.1和Http1.0的区别
HTTP  协议？
答： HTTP  超文本传输协议，是一个属于应用层的面向对象的协议，由于其简捷、快速的方式，适用于分布式超媒体信息系统。
Http怎么处理长连接。
Cookie与Session的作用于原理。
电脑上访问一个网页，整个过程是怎么样的：DNS、HTTP、TCP、OSPF、IP、ARP。
Ping的整个过程。ICMP报文是什么。
C/S模式下使用socket通信，几个关键函数。
IP地址分类。
常见的三种为ABC
IP地址的组成：网络号段+主机号段
A类：第一号段为网络号段+后三段的主机号段
一个网络号：256256256 = 16777216
B类：前二号段为网络号段+后二段的主机号段
一个网络号：256*256 = 65536
C类：前三号段为网络号段+后一段的主机号段
一个网络号：256
D类 224.0.0.1—239.255.255.254
E类 240.0.0.1—247.255.255.254
路由器与交换机区别。
网络其实大体分为两块，一个TCP协议，一个HTTP协议，只要把这两块以及相关协议搞清楚，一般问题不大。
推荐书籍：《TCP/IP协议族》
数据结构与算法
链表与数组。
队列和栈，出栈与入栈。
链表的删除、插入、反向。
字符串操作。
Hash表的hash函数，冲突解决方法有哪些。
各种排序：冒泡、选择、插入、希尔、归并、快排、堆排、桶排、基数的原理、平均时间复杂度、最坏时间复杂度、空间复杂度、是否稳定。
快排的partition函数与归并的Merge函数。
对冒泡与快排的改进。
二分查找，与变种二分查找。
二叉树、B+树、AVL树、红黑树、哈夫曼树。
二叉树的前中后续遍历：递归与非递归写法，层序遍历算法。
图的BFS与DFS算法，最小生成树prim算法与最短路径Dijkstra算法。
KMP算法。
排列组合问题。
动态规划、贪心算法、分治算法。（一般不会问到）
大数据处理：类似10亿条数据找出最大的1000个数…等等
算法的话其实是个重点，因为最后都是要你写代码，所以算法还是需要花不少时间准备，这里有太多算法题，写不全，我的建议是没事多在OJ上刷刷题（牛客网、leetcode等），剑指offer上的算法要能理解并自己写出来，编程之美也推荐看一看。
推荐书籍：《大话数据结构》《剑指offer》《编程之美》

