# 【Morgan Stanley IKM在线测试】C++ - 三少GG - CSDN博客
2012年04月11日 20:33:05[三少GG](https://me.csdn.net/scut1135)阅读数：8109
**掌握好基础啊！<<Effective C++>>,<<C++ Primer>>**
**![](https://img-my.csdn.net/uploads/201204/11/1334151516_9602.jpg)**
 考核内容包括不限于：
重构，重载，析构函数里（重构，重载，）new,malloc异常 堆和栈
OOP，OSI七层模型（应用层：http,ftp  传输层:tcp）
设计模式，UML图
Xml（DocTYPe）
数据库 sybase
**1. XML DOCTYPE**
2.10.2 The DOCTYPE declaration
An XML file which is valid (as opposed to simply well-formed) must specify a DTD against which its content is to be validated. This is the function of the DOCTYPE declaration.
The DOCTYPE declaration contains, following the DOCTYPE keyword, at least two parts: the name of the root element for the associated document, and a set of declarations for
 all the elements, attributes, notations, entities, etc. which together define the document type declaration (DTD) of that document. Note, incidentally, that the root element name (and hence the DOCTYPE name) may be that of any element whose declaration
 is supplied in this set. The declarations may be supplied explicitly, or by reference to an external entity such as a file, or by a combination of the two.
Taking each of these possibilities in turn, we first present a DOCTYPE declaration in which the declarations for all the elements, attributes, etc. required are given explicitly:
<!DOCTYPE myDoc [
  <!ELEMENT myDoc (p+) >
  <!ATTLIST myDoc n CDATA #IMPLIED>
  <!ELEMENT p (#PCDATA)>
]>
<myDoc n="1">
  <p>This is an instance of a "my.doc" document</p>
</myDoc>Note that the required declarations are enclosed within square brackets inside the DOCTYPE declaration: this part of the declaration is technically
 known as the DTD subset.
More usually, however, the required declarations will be held in a separate entity and invoked by reference, as follows:
<!DOCTYPE myDoc SYSTEM "myDoc.dtd" []>
<myDoc>
  <p>This is another instance of a "myDoc" document.</p>
  <p>It has two paragraphs.</p>
</myDoc>Note the similarity between the syntax used to reference the external entity containing the required declarations and that used to define any other system entity (see [2.7.1
 Entity declarations](http://www.tei-c.org/release/doc/tei-p4-doc/html/SG.html#SG-ents)). The square brackets may be supplied even though they enclose nothing, as in this example, or they may be omitted.
Next, we present a case where declarations are given both within the DTD subset and by reference to an external entity:
<!DOCTYPE myDoc SYSTEM "myDoc.dtd" [
  <!ENTITY tla "three letter acronym">]>
<myDoc>
  <p>This is yet another instance of a "myDoc" document.</p>
  <p>It is surprisingly free of &tla;s.</p>
</myDoc>
Any kind of declaration may be added to a DTD subset; as we have already seen ([2.8.2 Conditional marked
 section](http://www.tei-c.org/release/doc/tei-p4-doc/html/SG.html#SG-cms)), this is the mechanism by which the TEI DTD is customized.
<!DOCTYPE TEI.2 PUBLIC "-//TEI P3//DTD Main Document Type//EN" "tei2.dtd" [
  <!ENTITY % TEI.prose 'INCLUDE'>
  <!ENTITY % TEI.XML   'INCLUDE'>
  <!ENTITY tla "Three Letter Acronym">
  <!ENTITY % x.phrase  'myTag|'>	 
  <!ELEMENT myTag (#PCDATA)    >
  <!-- any other special-purpose declarations or
       re-declarations go  here -->
  ]>
<TEI.2>
  <!-- This is an instance of a modified TEI.2 type document, which
       may contain <myTag>my special tags</myTag> and references 
       to my usual entities such as &tla;. -->
</TEI.2>When, as here, the document type declaration in force includes both the contents of the DTD subset, and the contents of some external entity (in the case above, whatever file is specified by the PUBLIC identifier
 given, tei2.dtd by default), declarations in the DTD subset are always carried out first. As noted above, ([2.7.5
 Parameter entities](http://www.tei-c.org/release/doc/tei-p4-doc/html/SG.html#SG-pe)), the order is important, because in XML only the first declaration of an entity counts. In the above example, therefore, the declaration of the entity tla in
 the DTD subset takes precedence over any declaration of the same entity in the file tei2.dtd. Similarly, the declaration for x.phrasetakes
 precedence over the existing declaration for that entity in the TEI dtd. It is perfectly legal for entities to be declared more than once; elements, by contrast, may not be declared more than once; if a declaration for <myTag> were
 already contained in file tei.dtd, the XML parser would signal an error.
2. **C++new失败的处理**
我们都知道，使用 malloc/calloc 等分配内存的函数时，一定要检查其返回值是否为“空指针”（亦即检查分配内存的操作是否成功），这是良好的编程习惯，也是编写可靠程序所必需的。但是，如果你简单地把这一招应用到 new 上，那可就不一定正确了。我经常看到类似这样的代码：
        int* p = new int[SIZE];
        if ( p == 0 ) // 检查 p 是否空指针
            return -1;
        // 其它代码
    其实，这里的 if ( p == 0 ) 完全是没啥意义的。C++ 里，如果 new 分配内存失败，默认是**抛出异常**的。所以，如果分配成功，p == 0 就绝对不会成立；而如果分配失败了，也不会执行 if ( p == 0 )，因为分配失败时，new 就会**抛出异常跳过后面的代码**。如果你想检查 new 是否成功，应该**捕捉异常**：
        try {
            int* p = new int[SIZE];
            // 其它代码
        } catch ( const bad_alloc& e ) {
            return -1;
        }
    据说一些老的编译器里，new 如果分配内存失败，是不抛出异常的（大概是因为那时 C++ 还没加入异常机制），而是和 malloc 一样，返回空指针。不过我从来都没遇到过 new 返回空指针的情况。
    当然，标准 C++ 亦提供了一个方法来**抑制 new 抛出异常**，而返回空指针：
        int* p = new (std::nothrow) int; // 这样如果 new 失败了，就不会抛出异常，而是返回空指针
        if ( p == 0 ) // 如此这般，这个判断就有意义了
            return -1;
        // 其它代码
===============================详解===================================
首先按c++标准的话，new失败会抛出bad_alloc异常，但是有些编译器对c++标准支持不是很好，比如vc++6.0中new失败不会抛出异常，而返回0
3.stl**container Adapter**
STL 六大组件 功能与运用
1、容器（**Containers**）：各种数据结构，如**Vector,List,Deque,Set,Map,**用来存放数据，STL容器是一种**Class Template**,就体积而言，这一部分很像冰山载海面的比率。
2、算法（**Algorithms**）：各种常用算法如**Sort,Search,Copy,Erase,**从实现的角度来看，STL算法是一种**Function Templates**。
3、迭代器（**Iterators**）：扮演容器与算法之间的胶合剂，是所谓的“泛型指针”，共有五种类型，以及其它衍生变化，从实现的角度来看，迭代器是一种将：**Operators*,Operator->,Operator++,Operator--**等相关操作予以重载的**Class Template**。所有STL容器都附带有自己专属的迭代器——是的，只有容器设计者才知道如何遍历自己的元素，原生指针（**Native pointer**）也是一种迭代器。
4、仿函数（**Functors**）： 行为类似函数，可作为算法的某种策略（**Policy**）,从实现的角度来看，仿函数是一种重载了**Operator()**的**Class** 或 **Class Template**。一般函数指针可视为狭义的仿函数。
5、配接器（**Adapters**）：一种用来修饰容器（**Containers**）或仿函数（**Functors**）或迭代器（**Iterators**）接口的东西，例如：STL提供的**Queue**和**Stack**，虽然看似容器，其实只能算是一种容器配接器，因为 它们的底部完全借助**Deque**，所有操作有底层的**Deque**供应。改变**Functor**接口者，称为**Function Adapter**;改变**Container**接口者，称为**Container Adapter**;改变**Iterator**接口者，称为**Iterator Adapter**。配接器的实现技术很难一言蔽之，必须逐一分析。
6、配接器（**Allocators**）：负责空间配置与管理，从实现的角度来看，配置器是一个实现了动态空间配置、空间管理、空间释放的**Class Template**。
**——**《**STL**源码剖析》
4. **关键字 volatile**
与锁相比，Volatile 变量是一种非常简单但同时又非常脆弱的同步机制，它在某些情况下将提供优于锁的性能和伸缩性。如果严格遵循 volatile 的使用条件 —— 即变量真正独立于其他变量和自己以前的值 —— 在某些情况下可以使用 `volatile` 代替 `synchronized` 来简化代码。然而，使用 `volatile` 的代码往往比使用锁的代码更加容易出错。本文介绍的模式涵盖了可以使用 `volatile` 代替`synchronized` 的最常见的一些用例。遵循这些模式（注意使用时不要超过各自的限制）可以帮助您安全地实现大多数用例，使用
 volatile 变量获得更佳性能。
[http://www.ibm.com/developerworks/cn/java/j-jtp06197.html](http://www.ibm.com/developerworks/cn/java/j-jtp06197.html)
**5. 析构函数 异常问题**
[析构函数中抛异常](http://blog.csdn.net/zdl1016/article/details/4204245)
[构造函数中抛异常](http://blog.csdn.net/zdl1016/article/details/4204149)
1： 构造函数中是否可以抛异常？
可以。
2： 有什么限制吗？
有限制。构造抛异常之前必须把已经申请的资源释放掉。这样，就算你的对象是new出来的，也不会造成内存泄漏。
       因为析构函数不会被调用，所以抛出异常后，你没机会释放资源。
       建议，在构造函数中不要做过多的事情，只是能对成员变量的做初始化工作就好了。真的需要做其他复杂的初始化操作，完全可以提供一个Init或Start函数. 这样在Init或Start抛出异常后，如果对象是在栈上，析构函数仍会被调用。如果是在堆上，需要在捕获Init或Start抛出的异常后，delete对象。
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
网络讨论集锦：[http://bbs.yingjiesheng.com/thread-1242412-1-1.html](http://bbs.yingjiesheng.com/thread-1242412-1-1.html)
网申结束后2天收到邮件，通知进行IKM online
 test，有C++, C#, Java可供选择，选择其一即可。收到邮件后，开始在网上收IKM的相关资料，但很遗憾，没有找到原题。从查到的信息来看，大概是说这个测试很难，题目是adaptive的。另外可以找到考试的说明，大概来说，题目都是选择题，答题的正确率比速度更重要等等。
也不知道怎么准备。。于是开着google，旁边放本C++
 premier就开始做题。选的是C#的题目，一共35分钟。题目范围很广，涉及到多线程（lock等知识点）、内存管理、调试、界面类（与C#对应）、多态继承以及一些设计模式的问题。另外，还有给个程序，让你指出哪里有错误的题目。题目确实很难，而且大部分题一时半会是查不到。我做题速度很慢，一共只做了20多道题，估计要挂。
另外，之前在网上查到一篇博客说，考试的时候可以复制题目，我试着复制了一下，立刻弹出一个警告，说考试题目不允许复制，要是再复制的话，考试将自动终止。所以，题目是不能直接复制的。
题目太难，好多都是我不熟悉的，所以也没记住太多信息。Anyway，希望对大家有帮助！
发表于 2012-3-21 16:26:12
“楼上很厉害啊，我考的JAVA，做了大概20道的样子，坑爹的5选项不定选择，全是考什么异常静态类之类的东西，还有大量的uml题，能把英文看懂就不错了，算了反正没啥希望了，还是等好好复习下再继续参加其他的笔试
发表于
 2012-3-22 17:44:06
 刚做完了，只做了17道。貌似题目都是看上去不难（我的意思是以概念题为主，看来我悲剧了，做的太差，系统默认给我越来越简单了），当然我还是不会做就是了。悲剧啊。。
 选的C++，貌似主要考C++、面向对象和unix线程的一些概念。
发表于
 2012-3-23 10:10:41
我做了java的，做了感觉答的很不好，后来闲着没事又把C++的做了，题目很多都一样啊。不知道什么情况。2周了快，没消息。估计挂了。期待楼主的好消息。
发表于
 2012-3-30 09:55:46
 没有，全是选择题。主要考  C，面向对象和unix操作系统一些东西。
发表于 2011-11-5 14:18:31
我做的Java，知识覆盖挺广的，数据库(Sybase)、OS、UML，被活虐了，做完好久了，一直没有信儿，估计挂掉了
__________________________________________________________
## 大摩IT笔试
(2012-03-29 16:48:57)
![](http://simg.sinajs.cn/blog7style/images/common/sg_trans.gif)
|标签：### [杂谈](http://search.sina.com.cn/?c=blog&q=%D4%D3%CC%B8&by=tag)|分类：[我的求职](http://blog.sina.com.cn/s/articlelist_1661285703_7_1.html)|
昨晚进行的大摩IT的笔试，在此略微描述mark下。
一共三种语言，C++，C#和JAVA，选择我的老本行，JAVA。时间35分钟，全英文，全选择，多选题，每题5个选项，至多3个正确答案。算分规则是：正确答案选中的加一分，不该选的选了减一分，改选的没选减一分。难度根据你前面的答案正确率和做题的速度会改变，做得越好往后越难，反之。
我一共做了30题，感觉难度也没怎么增加，也没怎么减少，一开始做得很慢，总想选的很对，很全，后来觉得实在太慢了，遂加速，答案跟着感觉来。。。估计后面正确率直线下降了。
考了很多方面，OO，JAVA语言，网络，Unix，数据结构，设计模式，最诡异的数据库居然考的Sybase，还考了两题，完全不懂，sigh。。
OO主要考一些概念，比如动态绑定静态绑定，类的aggregation，association等关系。
JAVA语言有考到好几个编程题，主要形式是给代码，让你写答案。
网络，考到了TCP/IP协议，OSI七层中的传输层和物理层。
Unix，一些基本命令，貌似有个关于JVM的题目分类在这里，选择命令的，具体题目……忘了。。
数据结构，记得的不太多了，这悲催的记忆，刚刚一个面试就给忘了，唉。。
设计模式，好像有strategy和bridge什么的比较，具体用来解决哪种问题。还有一题关于facade，大概形式就是给个图，问你哪个模式最适合。
Sybase就不说了，一题直接忽略（还是要扣分的，因为该选的没选啊。。。），还有一题跟着感觉就给选了。。
其他的，记得不太多了。
大体总结下，基础知识还是很重要的，基本都是考的基础，细节啊，基本概念啊。然后就是覆盖面，还是挺广的，别看是JAVA相关，基本覆盖了所有的知识。
最后，说下那个系统。我只是忘了把翻译词典的划词功能关掉，直接给我判定为作弊了，呜呜，还好有两次机会。。
不知道有没有机会电面，感觉挺玄的样子。
END
