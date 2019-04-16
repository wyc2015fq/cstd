# 那些曾经记得的C++库 - 我相信...... - CSDN博客





2015年09月29日 09:09:55[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1653
个人分类：[编程语言](https://blog.csdn.net/wireless_com/article/category/2540051)









昨天有个朋友和我探讨了几个有关C++的问题，发现自己居然还能回忆起好多东西，只是不那么清晰罢了。尽管现在只是使用Object C++时和C++ 藕断丝连，但哪些曾经用过的C++ 类库可能只是曾经了。

### 系统和网络编程的C++库

ACE，只使用其基础设施和class wrapper就能开发出与C语言相当的系统应用，进而的Reactor和Proactor就是利器了。

### 内存管理的C++库

C/C++的内存管理是个永恒的话题，如果没有特别的理由，应该在大型项目中使用自动内存管理工具。比较轻量级的是boost：：smart_ptr 智能指针，重一些的实现了完整的GC机制是Hans-Boehm GC。

### 分布式对象中间件的C++库

谁还记得基于corba的orbix呢？ DCOM 或许还有些印象吧，C++/CLI 也支持.net framework 吧。 ICE 可能算是corba的改进版。

### 矩阵计算的C++库

由于template技术的高级应用，C++也可以相对方便的应用于科学计算，MTL功能丰富，性能和成熟度都不错。 C语言的科学算法库也很多，如 GSL，Atlas，NR等。

### 配置管理的C++库

Lua 作为配置脚本，嵌入到C/C++程序中，可以适应复杂的配置管理。

### 密码及安全的C++库

OpenSSL 应该说是业界的标杆了。

### XML的C++库

C++的xml 解析库较多，但好像都是C语言实现的，其中基于DOM的tinyxml，基于SOAP的xerxes。

### 正则表达式的C++库

正则表达式是编程中强有力的工具之一，C语言中有pcre，C++中boost 里的regex 库效率不错。

### GUI 的C++库

Qt 是一个比较完整的可移植应用程序框架，还包含了大量工具如正则，web和socket类，2D和3D，xml解析，SQL类等，跨平台GUI的成熟度非常高。主要特色是元对象模型，实现了signal/slot机制，实际上是C++的一个扩展。



