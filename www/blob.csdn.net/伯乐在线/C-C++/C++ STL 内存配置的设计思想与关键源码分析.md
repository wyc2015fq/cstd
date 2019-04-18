# C++ STL 内存配置的设计思想与关键源码分析 - 文章 - 伯乐在线
原文出处： [QingLiXueShi，2015-04-28](http://www.cnblogs.com/mengwang024/p/4461116.html)
**说明：**我认为要读懂STL中allocator部分的源码，并汲取它的思想，至少以下几点知识你要了解：operator new和operator delete、handler函数以及一点模板知识。否则，下面你很可能看不大明白，补充点知识再学习STL源码比较好。
下面会结合关键源码分析C++STL(SGI版本)的内存配置器设计思想。关键词既然是“**思想**”，所以重点也就呼之欲出了。
**1、allocator的简短介绍**
我阅读的源码是SGI公司的版本，也是看起来最清楚的版本，各种命名最容易让人看懂。allocator有人叫它**空间配置器**，因为空间不一定是内存，也可以是磁盘或其他辅助存储介质。我说的内存配置就是指的allocator。
C++标准规范了allocator的一些必要接口，由各个厂家实现。SGI的版本与众不同，也与标准规范不同，**它的名称是alloc而不是allocator**，**且不接受任何参数**。
假设你在程序中显示写出allocator，不能像下面这样写：

```
vector<int, std::allocator<int> > iv;        //错误的
```
必须要这样写才对：

```
vector<int, std::alloc> iv                    //好的
```
虽然SGI STL并不符合规范，但我们用起来好像很自然。这是因为我们使用时空间配置器是缺省的，不需要我们自行指定。例如，STL中vector的声明如下：
![](http://jbcdn2.b0.upaiyun.com/2017/01/ec51ac0a432ab1801a7a968f65e11654.png)
**注意：**下文我基本就用截图来解释代码了，因为我发现比起粘贴代码，这样更清晰（有颜色对比）。
**2、源码文件简单介绍**
STL标准规定：STL的allocator定义于文件中，主要包含了一些头文件，我们主要说的是两个：
![](http://jbcdn2.b0.upaiyun.com/2017/01/3bc865125ad9b531cf2f3b4cfcd47468.png)
**<stl_alloc.h>负责内存空间的配置与释放；<stl_construct.h>负责对象内容的构造与析构**
**3、构造和析构工具：construct()和destroy()**
先来说一下简单的文件。这部分也不涉及什么思想，只是有一个版本的destroy()应该认真看看。
**（1）**构造工具：construct()
construct()只有一个版本：
![](http://jbcdn2.b0.upaiyun.com/2017/01/129228efe6d0e6a2ba028e063890e877.png)
这里使用了placement new表达式（定位new 表达式），它的作用是p指向的内存类型为T1，用value值初始化这块内存。
**（2）**析构工具
destroy()倒是有几个版本：
**第一个版本：**
![](http://jbcdn2.b0.upaiyun.com/2017/01/9bd2d6e36a7ce094e231f0d91c88de0c.png)
这种显示调用析构函数的做法，你也应该要熟悉。
**第二个版本：**
![](http://jbcdn2.b0.upaiyun.com/2017/01/746884401e4aa8ddc5a6afd3dbf180cf.png)
第二个版本可有点说法。**调用层次是这样的：destroy-> __destroy-> __destroy_aux，__destroy_aux最终调用第一个版本的destroy****。**这个版本的destroy接受一对迭代器作为参数，析构迭代器所指向的范围内元素。
讲解这个流程前，先简单说一下trivial_destructor。
如果用户不定义析构函数，而是用编译器合成的，则说明析构函数基本没有什么用（但默认会被调用），称之为trivial destructor。
**那么，如果一对迭代器所指向的元素都是trivial destructor的，就没必要浪费时间对每个对象依次执行它的析构函数了，依靠编译器的行为就好了。这样在效率上是一种提升。这是STL allocator优化的一个点。**
首先利用value_type()取得迭代器指向对象的型别，再利用__type_traits判断对象的析构函数是否为trivial_destructor。如果是__true_type就什么都不做，否则循环调用第一版本的destroy()。
**第三个版本：**
![](http://jbcdn2.b0.upaiyun.com/2017/01/2c88262d3fb3f0d2a14d78ebd3e3bb3e.png)
这是针对迭代器为char*和wchar_t*的特化版本，看到它们的函数体为空，你应该猜到了，无须执行析构操作。
**4、内存的申请与销毁，std::alloc**
内存的申请和销毁由<stl_alloc.h>负责。SGI关于这一点的设计哲学是：
**（1）向system heap要求空间。**
**（2）考虑多线程状态。**
**（3）考虑内存不足时的应变策略。**
**（4）考虑过多“小型区块”可能造成的内存碎片问题。**
其实我最主要想说的是（3）（4）的设计策略，尤其是内存池的思路。
**std::alloc的整体设计思想为：**
SGI设计了双层级配置器，第一级配置器直接使用malloc和free；第二级配置器视情况不同采用不同策略：当配置区块超过128bytes时，视为“足够大”，调用第一级配置器处理；当配置区块小于128bytes时，视为“过小”，为降低额外负担，采用memory pool（内存池）处理方式，不再借助于第一级配置器。
**一、第一级内存配置器解析**
第一级配置器主要函数有：allocate分配内存、deallocate释放内存、reallocate重新分配内存等。
**（1）allocate**
![](http://jbcdn2.b0.upaiyun.com/2017/01/98f565527ca7c3758c65567ade519095.png)
直接调用C函数malloc，如果内存无法满足需求，就调用oom_malloc函数。
![](http://jbcdn2.b0.upaiyun.com/2017/01/ae7a47ca344d0968aa018639fd750c15.png)
原来，这是自己实现的handler函数啊，为什么自己实现呢？因为它使用的并不是operator new配置的内存，所以无法使用C++new-handler机制。
关于这个机制，实际上能有不少东西可说呢，如果你不熟悉它的用途或自己实现的方法，我建议你看看《Effective C++》，或者看看我对《Effective C++》做的笔记。我这里主要不是想分析语法方面的东西。
**（2）deallocate**
**![](http://jbcdn2.b0.upaiyun.com/2017/01/a2ddf016ff917678c51038e46a4b4f1d.png)**
代码放上去就应该明白了。
**（3）reallocate**
**![](http://jbcdn2.b0.upaiyun.com/2017/01/c1f6d304a9ebc86712c521f9f9ec85b7.png)**
这里依然是调用C的realloc函数，如果调用失败，就调用oom_realloc函数。
![](http://jbcdn2.b0.upaiyun.com/2017/01/ab27e91eaef345aeee8c91f7e60aa03d.png)
可以看出oom_realloc也是个handler函数。
基本上第一级内存配置器就解释清楚了。这里再提一点：**SGI以malloc而非operator new来配置内存一方面是历史原因，另一方面C++并未提供realloc函数。**这样造成了SGI不能直接使用C++的set_new_handler()，只能自己仿真一个。如何仿真set_new_handler，是有特定模式的。
**二、第二级内存配置器解析**
第二级内存配置器增加了一些机制，避免**太多小额区块造成的内存碎片。小额区块带来的不仅是内存碎片，配置时的额外负担也是个大问题**。额外负担永远无法避免，毕竟系统要靠这多出来的空间来管理内存，但区块越小，额外负担所占的比例越大，自然越浪费。
**第二级内存配置器的整体思想是：**
**（1）**如果申请的区块超过128bytes，就交给第一级内存配置器处理。
**（2）**如果申请的区块小于等于128bytes，用内存池管理。
具体为：第二级内存配置器会将任何小额区块的内存需求上调至8的倍数，并维护16个free-lists，各自管理大小为8、16、24、32、40、48、56、64、72、80、88、96、104、112、120、128字节的小额区块。
free-lists中节点结构如下：（我已经将这个union注释）
![](http://jbcdn2.b0.upaiyun.com/2017/01/c26b53f1d876fc311f84bcaecfde1b4e.png)
**注意：**union的这种用法，也被称为”柔性数组“成员。本质上，与小端对齐这种存储方式有关，这是一种技巧。
**（1）allocate**
第二级内存配置器__default_alloc_template的内存分配接口是allocate函数。
![](http://jbcdn2.b0.upaiyun.com/2017/01/9b321147c0258a2ae2693a3ca3ca8890.png)
关键部分我已经用红框注释过了。FREELIST_INDEX(n)函数根据n的值返回16个free-list中合适的那个list的下标。
![](http://jbcdn2.b0.upaiyun.com/2017/01/39ea8a84fb132fc7d0ff138135aa40db.png)
再看看ROUND_UP(n)，这个函数我认为写的挺巧妙的，将bytes值调整至8字节的倍数。
![](http://jbcdn2.b0.upaiyun.com/2017/01/d92d2e8471b3d854b75ba4087812351c.png)
理解这个函数你可以先举几个bytes值，看看返回值是什么，自然就理解了。refill()函数很有用处，我放在下面再来介绍。
**（2）deallocate()**
![](http://jbcdn2.b0.upaiyun.com/2017/01/288de600db2cbd05a642bd88122baa9e.png)
首先判断区块大小，大于128字节就调用第一级配置器，否则就根据需要回收的字节大小，判断出应该把它回归到哪个free list，然后由这个free list回收。
**（3）refill()**
这个函数挺重要的，所以要单独拿出来介绍。当free list中没有可用的区块时，就调用这个函数，为该free list重新填充一部分空间。新的空间取自内存池（由chunk_alloc完成）。缺省取得20个新区块，如果内存池空间不足，获得的新区块数目可能小于20。
![](http://jbcdn2.b0.upaiyun.com/2017/01/7ae5cd5afba661e7d7da95ed1f2a1659.png)
图中两个红框是值得注意的两个点。一旦从内存池获得内存区块后，拿出一个给调用者，另外的还要找到合适的free list”穿“起来。
**（4）内存池函数chunk_alloc()**
内存池一直用来供给free list。下面要将这个函数分开截图说明了。
![](http://jbcdn2.b0.upaiyun.com/2017/01/9719dfe6def381e146400a8d74187c5a.png)
**第一个分支：**看看内存池内的容量够不够，够的话直接拿走就好。
![](http://jbcdn2.b0.upaiyun.com/2017/01/d238382bbba0c452ae063754b3283c20.png)
**第二个分支：**内存池不够20个块的容量，但是大于等于1个块的长度，就把剩下的都给出去了。此时，**内存池是空的**。
![](http://jbcdn2.b0.upaiyun.com/2017/01/037e4f9ccdb8ae9f6d9608952b91d521.png)
**第三个分支：**如果内存池连1个对应的块都不能提供了，比如需要32字节，但只有8字节了，这时候最好的做法是把这8个字节链接到相应的free list利用上。**不出意料，此时内存池也是空的。**
**![](http://jbcdn2.b0.upaiyun.com/2017/01/32ed1642268e7a290088c714d62866b5.png)**
**第四个分支：**内存池空空如也，所以内存池求助于运行时堆，堆也没有那么多空间了，于是就检查这16个free list中有哪些块没用过呢，把这些补充到内存池。
![](http://jbcdn2.b0.upaiyun.com/2017/01/5ef3be689590573286d51d9675f64c16.png)
**第五个分支：**没错，heap也无能为力了，内存池干脆直接调用第一级配置器，因为第一级配置器有new-handler机制，或许有机会释放其他内存拿来此处调用呢。如果可以，就成功否则抛出bad-alloc异常。
**小结：**
如果别人问我STL内存配置的思想。我可能会这样说：C++STL是两级配置内存的，具体来说：第一级负责管理大块内存，要保证有类似new-handler的机制；第二级负责管理小块内存，为了更好的管理内存碎片，建立16个链表，每个链表“穿”着一块一块固定大小的内存，这16个链表（0至15）分别“穿”的内存是8、16、24…128倍数关系。需要内存时，从“合适”的链表取走，如果“合适”的链表内存不够用了，从内存池里拿，如果内存池不够用了，从运行时heap里拿，如果heap也溢出了，就交给第一级配置器，因为它有new-handler机制。
所以，**堆上的东西用完了赶紧换回来，别让内存池着急**。
