# Linux c++学习大概步骤（转） - writeeee的专栏 - CSDN博客
2016年03月14日 23:31:07[writeeee](https://me.csdn.net/writeeee)阅读数：5499
阅读源码 ，makefile, doxygen
**Module01 - Linux系统基础**
```
由于本系列课程基于Linux（或UNIX），熟悉Linux操作系统是必要的前提。 该模块的课程包含以下方面的内容：
常用Unix/Linux命令
熟悉文件管理、文本处理、进程管理、网络、系统管理等各个方面大约100个常用的命令。
深入了解bash
了解Linux默认shell: bash 的语法、命令执行、I/O重定向、任务控制等。
正则表达式基础
由于UNIX/Linux中很多强大的文本处理命令如：grep、awk、sed，还有vi编辑器等工具配合正则表达式将产生强大的威力，所以熟悉正则表达式语法是十分必要的。
find、grep、sed、awk
四个强大的UNIX工具，特别是sed、awk在文本处理方面的能力非常强大，在Linux下工作应该掌握这几个命令。
```
**Module02 - Linux开发环境**
```
不同系统平台下的开发工具、开发环境各有不同。该模块课程关注的是Linux C++/C开发所必需的一系列工具：
vi(vim)文本编辑器
一个UNIX世界标准的文本编辑器，简约而强大，不论作为开发人员还是系统管理员，熟练使用vi是一项基本的、且不可或缺的技能。
gcc/g++ C/C++编译器
通过具体的示例讲解使用gcc/g++编译单个、多个文件、共享库、静态库等。
gdb 调试器
通过具体的示例来熟悉通过gdb来调试C/C++应用程序、修正应用程序运行期的错误。
make和makefile
学习编写makefile，使用make来构建一个完整的项目。
CVS - 版本控制
运用CVS来进行代码的版本控制。
Eclipse CDT
一个非常强大的C/C++ IDE，强大的文本编辑器、与GCC工具链的无缝结合、方便的gdb前端、集成CVS/Subversion等版本控制等，提供众多的便利，大大减轻开发者的负担。
```
**Module03 - C++编程语言**
```
深入讲解C++编程语言的各个方面，即完整的C++语法讲解：
语言基础
详细介绍变量、表达式、语句、指针、数组、流程控制、函数、文件组织等。
抽象机制 - 面向对象编程
深入讲解C++的抽象机制，封装（类）、继承、多态；操作符重载、函数对象、异常处理等。
模板 - 泛型编程
详细介绍C++的模板机制，类模板、函数模板、模板特化等方面的内容。
```
**Module04 - C++标准库**
```
完整地讲解STL各大组件：容器、算法、函数对象、容器适配器、迭代器、迭代器适配器等；另外还包括string、I/O stream；为了更好地了解STL容器的特性、排序算法，额外安排：数据结构简介和常用排序算法简介2个部分的内容。
常用数据结构简介
介绍动态数组、linked-list、binary search tree、rb-tree、hash table、stack、queue、heap等常用的数据结构。
STL容器详细介绍
vector、list、deque、stack、queue、priority_queue、map、set等容器的特性和用法。
常见排序算法介绍
简要介绍各种常见排序算法的原理，及其实现。
STL算法和预定义函数对象
标准库提供了多达60多个算法函数，涉及排序、搜索、排列组合等多个方面，其中多数算法会使用如greater、less、binder2nd等函数对象，该单元的课程详细介绍了上述算法的使用和相关函数对象的具体作用。
迭代器
STL容器和算法高度解耦，而算法之所以能方便的作用于STL容器，维系二者的就是迭代器。
在这个单元的课程中讲介绍C++迭代器的类别、各类迭代器适配器的用法。
string - 字符串
相对于C风格的字符串处理，C++提供了更安全和方便的字符串类型 string，给class提供类众多的方法确保针对字符串处理的安全、便捷性。该单元的课程讲完整地介绍string的使用。
I/O stream
C++标准库提供的一个强大的I/O流框架。本单元详细介绍标准输入/输出、文件输入/输出、字符串输入/输出流的运用，利用操控符来控制输入/输出的格式。
 数值
介绍C++数值运算的算法，如valarray、4组数值算法函数、随机数等方面的内容。
```
**Module05 - C++ Boost**
```
Boost是由C++标准委员会成员发起、众多C++业界高人参与设计并实现的一个涉及面广、质量高且业已广泛使用的C++标准后备库，其中 TR1已经被纳入C++0x标准库。不论从风格和内容组织上讲，都可以认为Boost项目是C++标准库的延伸。截止到boost 1.43版本，boost项目拥有大约100个用途广泛的实用库。这部分课程将介绍服务器端开发所需要的几个组件：
容器与数据结构
介绍boost.any, boost.tuple, boost.array, boost.unordered(基于hash table，即hash_map和hash_set)等组件。
字符串算法和文字处理
介绍boost.lexical_cast, boost.format, boost.string_algo等组件。
正则表达式
正则表达式语法（perl正则表达式语法）的讲解，boost.regex库的使用。
智能指针
详细介绍shared_ptr、scoped_ptr、weak_ptr等智能指针的使用。
函数对象相关
详细介绍boost.bind, boost.mem_fn, boost.function, boost.ref, boost.lambda等组件。
序列化
通过实例熟悉boost.serialization库的用法。
日期与时间
boost.date_time库的详细介绍。
多线程
作为服务器端开发必不可少的内容之一：多线程支持，boost提供了一个跨平台的线程库：boost.thread。
本单元的课程详细介绍boost.thread。
网络编程
boost.asio库，通过对各类操作系统原生的socket API以及事件多路分离、异步I/O API的封装，构成了一个性能优秀、便于编程的网络编程框架，使复杂的网络编程任务变得简单、安全、并且高效。
本单元的课程通过一系列的示例来展示：通过asio来构建TCP同步/异步服务器和客户端、UDP同步/异步服务器和客户端应用，从而熟悉asio的接口和编程套路。
```
**Module06 - C++ ACE**
```
ACE是一个被广泛使用、设计优雅、高性能的C++通信框架（不仅仅是通信框架），其设计及实现被众多开源框架所借鉴。是构建稳定、高性能、高吞吐量、跨平台的服务器端程序的优秀框架。本模块的课程包含以下几个方面： 
ACE基础网络I/O相关对象
详细介绍ACE_SOCK、ACE_SOCK_Acceptor、ACE_SOCK_Connector、ACE_INET_Addr等class的使用。
Reactor框架
ACE Reactor框架简化事件驱动程序的开发，而事件驱动是很多网络化应用的基本特征，这些应用常见的事件源包括I/O事件、Posix信号或 Windows句柄激发以及定时器到期等。
本单元介绍ACE_Event_Handler、Timer、ACE_Reactor等类的使用，并使用该框架构建一个简易的多人聊天室应用。
Task框架
ACE Task框架提供了强大而可扩展的面向对象并发能力，如在基于对象的上下文(context)中派生线程，以及在执行不同线程中的对象之间传递消息和对消息进行排队。
Acceptor-Connector框架
ACE Acceptor-Connector框架实现了Acceptor-Connector模式，这种模式通过解除：1，网络化应用中相互协作的对等服务的连接和初始化所需的活动、2，以及它们一旦连接和初始化后所执行的处理的耦合，增强了软件复用和可扩展性。
Proactor框架
Proactor框架引入异步I/O机制，既保留了Reactor框架的事件多路分离，避免多线程的开销，同时还缓和了反应式的同步I/O的瓶颈效应。
```
**Module07 - 数据库开发**
```
数据库作为服务器端应用数据持久化的最重要的部件，在服务器端应用开发中占有非常重要的地位。本模块主要针对Oracle 10g和MySQL 5.1两种关系型数据库管理系统。本模块包含以下内容：
SQL语言
详细介绍DML、DDL语句的语法和使用。
Oracle PL/SQL
全面介绍Oracle PL/SQL语法，以及使用PL/SQL编写存储过程、函数、触发器。
MySQL存储过程
编写MySQL存储过程、函数、触发器。
C++ OTL
OTL：Oracle、ODBC and DB2-CLI Template Library。通过OTL，可以方便、高效的与各类主流的关系型数据库如DB2、Oracle、Infomix、Sybase、MySQL等通信。
```
