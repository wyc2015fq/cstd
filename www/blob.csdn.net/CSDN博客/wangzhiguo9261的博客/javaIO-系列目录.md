
# javaIO-系列目录 - wangzhiguo9261的博客 - CSDN博客


2018年02月24日 15:14:24[码里偷闲王果果](https://me.csdn.net/wangzhiguo9261)阅读数：138个人分类：[java基础](https://blog.csdn.net/wangzhiguo9261/article/category/7367704)


[java io系列01之 "目录"](http://www.cnblogs.com/skywang12345/p/io_01.html)
java io 系列目录如下：
**01.****java io系列01之  "目录"**
**02.****java io系列02之 ByteArrayInputStream的简介,源码分析和示例(包括InputStream)**
**03.****java io系列03之 ByteArrayOutputStream的简介,源码分析和示例(包括OutputStream)**
**04.****java io系列04之 管道(PipedOutputStream和PipedInputStream)的简介,源码分析和示例**
**05.****java io系列05之 ObjectInputStream 和 ObjectOutputStream**
**06.****java io系列06之 序列化总结(Serializable 和 Externalizable)**
**07.****java io系列07之 FileInputStream和FileOutputStream**
**08.****java io系列08之 File总结**
**09.****java io系列09之 FileDescriptor总结**
**10.****java io系列10之 FilterInputStream**
**11.****java io系列11之 FilterOutputStream**
**12.****java io系列12之 BufferedInputStream(缓冲输入流)的认知、源码和示例**
**13.****java io系列13之 BufferedOutputStream(缓冲输出流)的认知、源码和示例**
**14.****java io系列14之 DataInputStream(数据输入流)的认知、源码和示例**
**15.****java io系列15之 DataOutputStream(数据输出流)的认知、源码和示例**
**16.****java io系列16之 PrintStream(打印输出流)详解**
**17.****java io系列17之 System.out.println("hello world")原理**
**18.****java io系列18之 CharArrayReader(字符数组输入流)**
**19.****java io系列19之 CharArrayWriter(字符数组输出流)**
**20.****java io系列20之 PipedReader和PipedWriter**
**21.****j****ava io系列21之 InputStreamReader和OutputStreamWriter**
**22.****java io系列22之 FileReader和FileWriter**
**23.****java io系列23之 BufferedReader(字符缓冲输入流)**
**24.****java io系列24之 BufferedWriter(字符缓冲输出流)**
**25.****java io系列25之 PrintWriter (字符打印输出流)**
**26.****java io系列26之 RandomAccessFile**

### 前言
关于java的io部分，以前学习过。当时学习的节奏是，首先了解io的相关介绍，再查看io的使用示例，最后才是打开API参考文档并参考文档通过写demo来学习。结果，当时是了解了API的使用方法；可是一段时间之后，只能零零碎碎的记得一些片段，再要自己讲出个所以然来，已经非常之艰难。之所以造成这种困境，我想主要是：一，学习的时候，理解的不够彻底，没有真正理解它的思想和框架。二，加之后来使用的较少，加速了遗忘的速度。在这里面，最根本的问题还是理解，使用的目的也是为了更加深入的理解。
最近，抽空整理以前所学的java基础。整理也就是重新学习的过程，在这个学习过程中，我一直尝试着站在一个更高的角度来看问题，将这些类和接口的原理和思想理解的更加透彻一些！在这里，将所学的东西分享出来，一是共同提高；再者，有不足的地方或好的建议，还希望各位读者能指出。谢谢！
转载请注明出处：**http://www.cnblogs.com/skywang12345/p/io_01.html**
### java io简介
java io系统的设计初衷，就是为了实现“文件、控制台、网络设备”这些io设置的通信。例如，对于一个文件，我们可以打开文件，然后进行读取和写入。
在java 1.0中，java提供的类都是以字节(byte)为单位，例如，FileInputStream和FileOutputStream。而到了java 1.1，为了与国际化进行接轨，在java io中添加了许多以字符(Unicode)为单位进行操作的类。
在java io的称呼中，我们经常会提到“输入流”、“输出流”等等概念。首先，什么是流呢？
所谓**“流”，就是一种抽象的数据的总称，它的本质是能够进行传输。**
a) 按照“流”的数据流向，可以将其化分为：**输入流**和**输出流**。
b) 按照“流”中处理数据的单位，可以将其区分为：**字节流**和**字符流**。在java中，字节是占1个Byte，即8位；而字符是占2个Byte，即16位。而且，需要注意的是，java的字节是有符号类型，而字符是无符号类型！

### java io框架
有了上面的基本概念之后，我们看看java io的框架。
**1. 以字节为单位的输入流的框架图**
下面，是以字节为单位的输入流的框架图。
![](https://images0.cnblogs.com/blog/497634/201310/20234201-95f7519c9a174cbbb8b3c6e0a076a56d.jpg)
从中，我们可以看出。
(01) InputStream 是以字节为单位的输入流的超类。InputStream提供了read()接口从输入流中读取字节数据。
(02) ByteArrayInputStream 是字节数组输入流。它包含一个内部缓冲区，该缓冲区包含从流中读取的字节；通俗点说，它的内部缓冲区就是一个字节数组，而ByteArrayInputStream本质就是通过字节数组来实现的。
(03) PipedInputStream 是管道输入流，它和PipedOutputStream一起使用，能实现多线程间的管道通信。
(04) FilterInputStream 是过滤输入流。它是DataInputStream和BufferedInputStream的超类。
(05) DataInputStream 是数据输入流。它是用来装饰其它输入流，它“允许应用程序以与机器无关方式从底层输入流中读取基本 Java 数据类型”。
(06) BufferedInputStream 是缓冲输入流。它的作用是为另一个输入流添加缓冲功能。
(07) File 是“文件”和“目录路径名”的抽象表示形式。关于File，注意两点：
a), File不仅仅只是表示文件，它也可以表示目录！
b), File虽然在io保重定义，但是它的超类是Object，而不是InputStream。
(08) FileDescriptor 是“文件描述符”。它可以被用来表示开放文件、开放套接字等。
(09) FileInputStream 是文件输入流。它通常用于对文件进行读取操作。
(10) ObjectInputStream 是对象输入流。它和ObjectOutputStream一起，用来提供对“基本数据或对象”的持久存储。
**2. 以字节为单位的输出流的框架图**
下面，是以字节为单位的输出流的框架图。
![](https://images0.cnblogs.com/blog/497634/201310/20234231-929b2961bb604a05922c9a6ce1348110.jpg)
从中，我们可以看出。以字节为单位的输出流的公共父类是OutputStream。
(01) OutputStream 是以字节为单位的输出流的超类。OutputStream提供了write()接口从输出流中读取字节数据。
(02) ByteArrayOutputStream 是字节数组输出流。写入ByteArrayOutputStream的数据被写入一个 byte 数组。缓冲区会随着数据的不断写入而自动增长。可使用 toByteArray() 和 toString() 获取数据。
(03) PipedOutputStream 是管道输出流，它和PipedInputStream一起使用，能实现多线程间的管道通信。
(04) FilterOutputStream 是过滤输出流。它是DataOutputStream，BufferedOutputStream和PrintStream的超类。
(05) DataOutputStream 是数据输出流。它是用来装饰其它输出流，它“允许应用程序以与机器无关方式向底层写入基本 Java 数据类型”。
(06) BufferedOutputStream 是缓冲输出流。它的作用是为另一个输出流添加缓冲功能。
(07) PrintStream 是打印输出流。它是用来装饰其它输出流，能为其他输出流添加了功能，使它们能够方便地打印各种数据值表示形式。
(08) FileOutputStream 是文件输出流。它通常用于向文件进行写入操作。
(09) ObjectOutputStream 是对象输出流。它和ObjectInputStream一起，用来提供对“基本数据或对象”的持久存储。
**3. 以字节为单位的输入流和输出流关联的框架图**
输入流和输出流都有对应的关系，下面是将以字节为单位的输入流和输出流关联起来的图片。
![](https://images0.cnblogs.com/blog/497634/201310/20234245-b708d62c6397495db7915d8fee6616f7.jpg)
**4. 以字符为单位的输入流的框架图**
下面，是以字符为单位的输入流的框架图。
![](https://images0.cnblogs.com/blog/497634/201310/20234317-f9f030ae18904626b08b8d464e87eed1.jpg)
从中，我们可以看出。以字符为单位的输入流的公共父类是Reader。
(01) Reader 是以字符为单位的输入流的超类。它提供了read()接口来取字符数据。
(02) CharArrayReader 是字符数组输入流。它用于读取字符数组，它继承于Reader。操作的数据是以字符为单位！
(03) PipedReader 是字符类型的管道输入流。它和PipedWriter一起是可以通过管道进行线程间的通讯。在使用管道通信时，必须将PipedWriter和PipedReader配套使用。
(04) FilterReader 是字符类型的过滤输入流。
(05) BufferedReader 是字符缓冲输入流。它的作用是为另一个输入流添加缓冲功能。
(06) InputStreamReader 是字节转字符的输入流。它是字节流通向字符流的桥梁：它使用指定的 charset 读取字节并将其解码为字符。
(07) FileReader 是字符类型的文件输入流。它通常用于对文件进行读取操作。
**5. 以字符为单位的输出流的框架图**
下面，是以字符为单位的输出流的框架图。
![](https://images0.cnblogs.com/blog/497634/201310/20234330-d18eb674e6ba44beb6d65b05c602f065.jpg)
(01) Writer 是以字符为单位的输出流的超类。它提供了write()接口往其中写入数据。
(02) CharArrayWriter 是字符数组输出流。它用于读取字符数组，它继承于Writer。操作的数据是以字符为单位！
(03) PipedWriter 是字符类型的管道输出流。它和PipedReader一起是可以通过管道进行线程间的通讯。在使用管道通信时，必须将PipedWriter和PipedWriter配套使用。
(04) FilterWriter 是字符类型的过滤输出流。
(05) BufferedWriter 是字符缓冲输出流。它的作用是为另一个输出流添加缓冲功能。
(06) OutputStreamWriter 是字节转字符的输出流。它是字节流通向字符流的桥梁：它使用指定的 charset 将字节转换为字符并写入。
(07) FileWriter 是字符类型的文件输出流。它通常用于对文件进行读取操作。
(08) PrintWriter 是字符类型的打印输出流。它是用来装饰其它输出流，能为其他输出流添加了功能，使它们能够方便地打印各种数据值表示形式。
**6. 以字符为单位的输入流和输出流关联的框架图**
下面是将以字符为单位的输入流和输出流关联起来的图片。
![](https://images0.cnblogs.com/blog/497634/201310/20234410-c986ccb259594865ae75f14f19e1179f.jpg)
**7. 字节转换为字符流的框架图**
在java中，字节流能转换为字符流，下面是它们的转换关系图。
![](https://images0.cnblogs.com/blog/497634/201310/20234430-bb419718ff01462c8d94fc2ac3e1aeb6.jpg)
从中，我们可以看出。
(01) FileReader继承于InputStreamReader，而InputStreamReader依赖于InputStream。具体表现在InputStreamReader的构造函数是以InputStream为参数。我们传入InputStream，在InputStreamReader内部通过转码，将字节转换成字符。
(02) FileWriter继承于OutputStreamWriter，而OutputStreamWriter依赖于OutputStream。具体表现在OutputStreamWriter的构造函数是以OutputStream为参数。我们传入OutputStream，在OutputStreamWriter内部通过转码，将字节转换成字符。
**8. 字节和字符的输入流对应关系**
![](https://images0.cnblogs.com/blog/497634/201310/20234451-97f7312056a642ccb58ca02a2803dbb4.jpg)
**9. 字节和字符的输出流对应关系**
![](https://images0.cnblogs.com/blog/497634/201310/20234541-d488f6a75e524979acfe8a77ff14ec78.jpg)
认识了java io的总体框架之后。接下来，我们可以开始对其中的各个类进入学习，包括“它的作用和用法，源码的探索”等方面。
---
**更多内容**
**01.****[java 集合系列目录(Category)](http://www.cnblogs.com/skywang12345/p/3323085.html)**

![img](http://www.cnblogs.com/images/cnblogs_com/skywang12345/552879/t_f1.jpg)|生活的悲欢离合永远在地平线以外，而眺望是一种青春的姿态...
|PS.文章是笔者分享的学习笔记，若你觉得可以、还行、过得去、甚至不太差的话，可以“推荐”一下的哦。就此谢过!|

