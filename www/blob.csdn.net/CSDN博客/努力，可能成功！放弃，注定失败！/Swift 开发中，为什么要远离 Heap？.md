# Swift 开发中，为什么要远离 Heap？ - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年05月31日 14:17:39[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：604标签：[ios开发																[iOS学习资料																[iOS 资源大全																[Swift库](https://so.csdn.net/so/search/s.do?q=Swift库&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS 资源大全&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS学习资料&t=blog)](https://so.csdn.net/so/search/s.do?q=ios开发&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
![](http://upload-images.jianshu.io/upload_images/1721232-5bc098f26c149ab5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
本文来自简书，原文地址:[http://www.jianshu.com/p/aca50c5a9d64](http://www.jianshu.com/p/aca50c5a9d64)
WWDC的视频 — Understanding Swift Performance 中，苹果上来就说，Heap 的操作复杂度要远远超越 Stack。所以大家在选择数据结构时，要尽量选择诸如结构体这种存储在 Stack 上的值数据类型，而不要选择像类这种存储在 Heap上的数据类型。问题是，相比于 Stack，Heap 操作复杂体现在什么地方？
要回答这个问题，我们就必须了解 Swift 中，Heap 是用来做什么的。同时，在 Heap 上发生了哪些操作，才导致其在性能上被诟病？
# 什么是 Heap
一般提到 Heap，可能指两种东西，一种是数据结构中的 Heap，另一种是内存中的 Heap。本文要谈的是内存中的 Heap。对于数据结构的 Heap，就是一种特殊的二叉树。它满足以下条件：
- 堆的最小或最大值在根节点。其所有子节点都小于或大于其父节点。
- 堆是完全的二叉树。除最底层所有节点都被填满。最底层节点填充从左到右。
具体的细节这里不作展开，网上相关的文章一大把，大家可以自行查阅。
言归正传。本文中，堆是可以被用来动态分配空间的内存块。这个定义中有一个关键词 —— 动态分配。
所谓动态分配，就是对于数据、变量，系统并不预先分配一定的空间，而是根据程序的运行和需求进行即时分配，它发生在程序调入和运行（run time）的时候。而静态分配，是在编译时就已经知道数据需要的空间，所以在程序编译和连接（compile time）时，系统就给相应数据分配了空间。举个例子：
```
// assume defaultCellHeight is a static global constant
static let defaultCellHeight = 44.0  
// assume bar is a var used in a specific data structure
var bar: MyClass?
```
其中`defaultCellHeight`一看就是个浮点变量，所需的内存大小确定，所以它的分配是静态分配。而`bar`是个类，编译时不知道它有多少个字节、需要多少空间，故而只有当程序运行后，内存才可以确定这些细节，故而它是动态分配。
而内存中，负责动态分配内存的数据区有两个，一个是栈（Stack），另一个是堆（Heap）。
![](http://upload-images.jianshu.io/upload_images/1721232-890afb5e7f4ec0d3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
# Heap 和 Stack 在内存管理上的比较
||Heap|Stack|
|----|----|----|
|结构|基于链表、数组、树|栈|
|特点|手动分配大小，随时释放空间，数据进出无序|自动分配大小，自动释放内存，数据先进后出|
|操作|查询之后分配/释放，之后再做整合，复杂度高|依靠栈底指针移动来分配/释放，复杂度低|
|对象|引用类型如 class。引用计数，变量类型等信息|值类型如 struct, enum, Int。函数返回值，局部变量|
|场景|C 中的 malloc 和 free 操作，java 中的garbage collection，iOS 中的 MRC、ARC|适用于撤销、保存操作|
|线程|共享，多线程不安全|独享，多线程安全|
# Swift 中 Heap 的设计
Swift 中 Heap 是由双向链表实现的，其操作也是调用了 C++ 的 malloc 和 dealloc 方法。那么为什么是双向链表？我们来仔细分析一下。
刚才已经说明，Stack 的操作只是指针移动，故而复杂度低，为常数。而 Heap 的操作却十分复杂，那么具体是怎样的？我们不妨来看两个底层函数：retain 和 release。
首先明确一下需求，retain 即分配空间，比如 `[myString retain]`，就是给一个字符串分配一定字节的内存。而
 release 即释放之前的空间，比如`[myString release]`，就是释放这个字符串分配的内存。
### 最直观的设计：数组
最简单粗暴的设计 Heap 的方法如下：将其设计成数组，其中所占的内存切分成 n 等分，每一等分代表一个字节。从左往右顺序分配空间，同样顺序释放空间，这样所有的操作都是线性。然而想象很美好，现实却很残酷。假如 Heap 一共有10个字节，我们有以下4个字符串：
```
let string1 = "abcd" // 假设4个字节
let string2 = "a"    // 假设1个字节
let string3 = "abc"  // 假设3个字节
let string4 = "abcde"  // 假设5个字节
```
然后我们做下面几个操作：
![](http://upload-images.jianshu.io/upload_images/1721232-992b1fd6fafe5ad0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
每一步的 heap 数组长这样，注意数字代表是内存大小，`'`代表空闲：
```
[heap init] -> [10‘]
[string1 retain] -> [4, 6']
[string2 retain] -> [4, 1, 5']
[string3 retain] -> [4, 1, 3, 2']
[string3 release] -> [4, 1, 3', 2']
[string4 retain] -> ?
```
这时候我们发现，Heap 中虽然有5个字节的空余空间，却无法分配给 string4，因为这5个字节的空余空间不连续。系统只认为有一个3字节的空余空间和一个2字节的空余空间。于是我们发现数组的想法过于天真，没有处理 release 之后整合空余空间的问题。
### 链表设计
于是我们想想有什么办法解决这个问题。假如我们利用链表，将所有的内存块连起来，并且在 release 时通过调整链表指针来整合空间，这样就能解决我们刚才的问题。顺着这个思路，我们实现了下面这种 Heap 结构：
![](http://upload-images.jianshu.io/upload_images/1721232-e5e37a821f907919.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
- 内存块用链表进行连接
- 每个内存块的头结点表明了内存块的大小，以及该内存块是否已被 retain（0表示空余，1表示已被占用）
- 最开头和最末尾的节点表明已经到了 Heap 的头和尾
Retain 操作这个时候有下面三种设计方式可控选择：
- 从头遍历链表。找出第一个能分配足够空间的空闲内存块。这样的操作的复杂度是线性的。
- 从之前搜索过的位置起搜索链表的空余内存块，并找到合适的那块。这样可以跳过之前搜索过的、肯定不符合的内存块，一般情况下会稍微快点。
- 从头遍历链表。找到最适合（大小最接近）的空闲内存块，这样空间利用率会很高，可惜时间复杂度上相比于之前两种更高。
Release 的操作除了分配内存空间，还要注意整合内存块。我们刚才那个例子，当 `[string3 release]`之后，它分配出来的内存块会和空余的整合在一起。这个过程如下所示：
![](http://upload-images.jianshu.io/upload_images/1721232-051ef2099c66fa30.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
这个设计已经及格了。但是它有一个很严重的问题：性能。因为一般而言，Heap 比较大，每次遍历去找空余空间比较耗时；其二，每次 release 之后都必须判断当前内存块的前一块和后一块是否为空闲，如果是则要分别整合。这又牵涉到遍历链表查询的问题，当然解决办法也比较简单，用双向链表。
### 优化：双向链表
双向链表的引入主要是引入 release 之后的内存块整合问题，这样可以快速查询前后内存块是否为空。同时为了解决之前设计每次遍历极度耗时的性能问题，我们这样设计，我们只把空闲内存块用指针连起来形成链表。于是 Heap 的数据结构变成了下面这样：
![](http://upload-images.jianshu.io/upload_images/1721232-24b8cde04a3fbdd8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
这样每次 retain 操作，我们可以少遍历一半的内存（已经分配的），效率理论上来讲提高一倍。而 release 操作，我们可以采用 LIFO 机制，将多出来的空余空间插入到 Heap 头处，并与原来的第一个空余空间整合。这样的做法复杂度是常数，非常高效。最后，我们再也不用花多余的精力去用 1 和 0 来表示当前内存块是否为空。
![](http://upload-images.jianshu.io/upload_images/1721232-a911bf08a0398eb2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
release 操作之后整合空闲内存块的一种情况
为了提高空间利用率，我们还可以引入数组。数组中的每一个元素都是一个双向链表。每一个双向链表连接了 Heap 中的大小相近的空余空间。这样我们在 retain 的时候，我们先根据大小快速定位到比较合适的空余空间所在链表，再做遍历。如此一来，空间利用率得到提高，遍历元素数量减少，Heap 效率更高。
![](http://upload-images.jianshu.io/upload_images/1721232-1b4f9ef9c901c3f1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
# 总结
虽然现在苹果已经用 ARC 帮我们自动处理内存分配和释放的问题了。相比于 MRC 时代手动的 retain 和 release 操作，我们无需过度担忧内存调度。然而，在优化方面，苹果推荐 Stack 和值类型，是因为 Stack 的性能很高，复杂度几乎为常数。虽然 Heap 在动态内存分配中似乎更自由、更灵活，但相对而言，其性能很低，复杂度较高。所以，Swift 的值类型才如此受欢迎啊。
