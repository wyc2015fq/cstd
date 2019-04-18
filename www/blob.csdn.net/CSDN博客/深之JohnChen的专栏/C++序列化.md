# C++序列化 - 深之JohnChen的专栏 - CSDN博客

2017年08月24日 10:19:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1940


一、序列化

       在编写应用程序的时候往往需要将程序的某些数据存储在内存中，然后将其写入某个文件或是将它传输到网络中的另一台计算机上以实现通讯。这个将 程序数据转化成能被存储并传输的格式的过程被称为“序列化”（Serialization），而它的逆过程则可被称为“反序列化” （Deserialization）。

简单来说，序列化就是将对象实例的状态转换为可保持或传输的格式的过程。与序列化相对的是反序列化，它根据流重构对象。这两个过程结合起来，可以轻 松地存储和传输数据。例如，可以序列化一个对象，然后使用 HTTP 通过 Internet 在客户端和服务器之间传输该对象。

序列化：将对象变成字节流的形式传出去。

反序列化：从字节流恢复成原来的对象。

简单来说，对象序列化通常用于两个目的：

1）将对象存储于硬盘上 ，便于以后反序列化使用。

2）在网络上传送对象的字节序列。

二、C++对象序列化方法

1、Boost.Serialization

Boost.Serialization可以创建或重建程序中的等效结构，并保存为二进制数据、文本数据、XML或者有用户自定义的其他文件。该库具有以下吸引人的特性：

1). 代码可移植（实现仅依赖于ANSI C++）

2). 深度指针保存与恢复

3). 可以序列化STL容器和其他常用模版库

4). 数据可移植

5). 非入侵性

boost::serialization 基于 boost::archive 来完成任意复杂数据结构的序列化，boost::archive提供两个实现类来完成序列化、反序列化操作：

boost::archive::text_oarchive     序列化数据，也称为：输出、保存（save）

boost::archive::text_iarchive      反序列化数据，也称为：输入、载入（load）

也可以使用二进制格式:binary_oarchive, binary_iarchive

&操作符 

序列化操作使用 << 或者 & 操作符将数据存入text_oarchive中：

ar << data;

ar & data;

反序列化操作使用 >> 或者 & 操作符从text_iarchive中读取数据：

ar >> data;

ar & data;

为什么要引入&操作符？很简单，&操作符可以同时用于序列化和反序列化操作，这样就只需要提供一个 serialize 模板函数就可以同时用于两种操作，具体执行哪种由ar的类型（模版参数类型）决定；如果ar是text_oarchive类型则是序列化，如果ar是text_iarchive类型则是反序列化。

2、protobuf

   Google Protocol Buffers (GPB)是Google内部是用的数据编码方式，旨在用来代替XML进行数据交换。可用于数据序列化与反序列化。主要特性有：

1). 高效

2). 语言中立（Cpp, Java, Python）

3). 可扩展

  官网地址：http://code.google.com/p/protobuf/

3、MFC Serialization

   Windows平台下可使用MFC中的序列化方法。MFC 对 CObject 类中的序列化提供内置支持。因此，所有从 CObject 派生的类都可利用 CObject 的序列化协议。

   继承CObject的类，实现序列化方法Serialize(CArchive& ar)，添加序列化宏DECLARE_SERIAL。编写序列化与反序列化的对象。

几种方法对比：  

Google Protocol Buffers效率较高，但是数据对象必须预先定义，并使用protoc编译，适合要求效率，允许自定义类型的内部场合使用。Boost.Serialization 使用灵活简单，而且支持标准C++容器。相比而言，MFC的效率较低，但是结合VisualStdio平台使用最为方便。

参考资料

http://www.cnblogs.com/lanxuezaipiao/p/3703988.html

http://blog.csdn.net/liyongofdm/article/details/7650380

