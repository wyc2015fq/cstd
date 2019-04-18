# 图解Go内存分配器 - weixin_33985507的博客 - CSDN博客
2019年03月07日 08:00:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
内存分配器一直是性能优化的重头戏，其结构复杂、内容抽象，涉及的数据结构繁多，相信很多人都曾被它搞疯了。本文将从内存的基本知识入手，到一般的内存分配器，进而延伸到Go内存分配器，对其进行全方位深层次的讲解，希望能让你对进程内存管理有一个全新的认识。
## 物理内存 VS 虚拟内存
在研究内存分配器之前，让我们先看一下物理内存和虚拟内存的背景知识。剧透一下，内存分配器实际上操作的不是物理内存而是虚拟内存。
![\"\"](https://static001.geekbang.org/resource/image/4a/7c/4aaf9e89c9e732add6fd79771f93f57c.png)
物理内存细胞结构简化图
内存细胞作为物理内存结构的最小单元，工作原理如下：
- 地址线（三相晶体管）其实是连接数据线与数据电容的三相开关。
- 当地址线负载时（红线），数据线开始向电容中写数据，电容处于充电状态，逻辑值变为1
- 当地址线空载时（绿线），数据线不能向电容中写数据，电容处于未充电状态，逻辑值为0
- 当CPU从RAM中读值时，它首先会给地址线发送一个电流信号从而合上开关，连通数据电路。这时如果电容处于高电位，则电容中的电流会流向数据线，CPU读数为1；否则，数据线中没有电流负载，CPU读数为0。
![\"\"](https://static001.infoq.cn/resource/image/f2/ad/f2e67599cf7ab78e21ce88119b6d64ad.png)
CPU和内存的交互
CPU实际上通过地址总线、数据总线和控制总线实现对内存的访问。
- 数据总线：在CPU和内存之间传递数据的通道；
- 控制总线：在CPU和内存之间传递各种控制/状态信号的通道；
- 地址总线： 传送地址信号，以确定所要访问的内存地址。
让我们进一步分析一下**地址线**和**按字节寻址：**
![\"\"](https://static001.infoq.cn/resource/image/0e/57/0ead99df83d86ae49d9a2b4495193557.png)
- 
在DRAM中，每一个字节都有一个唯一的地址。“可寻址字节不一定等于地址线的数量”，
例如16位的Intel 8088、PAE（物理地址扩展）等，其物理字节大于地址线数量。
- 
每一条地址线可以传送1-bit的数值，可表示寻址字节中的一位。
- 
图中有32位地址线，所以可认为可寻址字节是32位的。
[ 00000000000000000000000000000000 ] —低位内存地址。
[ 11111111111111111111111111111111 ] — 高位内存地址。
4. 因为上图物理字节有32条地址线，所以其寻址空间大小为2的32次方，也就是4GB
可寻址字节的大小其实取决于地址线的数量，例如具有64个地址线的CPU（x86–64 处理器）可以寻址2的64次方，但是目前大多数64位的CPU其实只使用了其中的48位（AMD）或者42位（Intel）。尽管理论上可访问2的64次方（256TB）大小的地址空间，但是通常操作系统并没有完全支持它们（Linux的[四层页表结构](%5C)允许处理器访问128TB大小的地址空间，Windows支持192TB）。
由于实际物理内存的大小是有限制的，所以每个进程都运行在各自的沙盒中，也就是所谓的“虚拟地址空间”，简称虚拟内存。
**虚拟内存中的字节地址其实并不是实际的物理地址**。操作系统需要记录所有虚拟地址到物理地址的映射转换，也就是我们熟知的页表。
进程中的虚拟地址如下图所示：
![\"\"](https://static001.infoq.cn/resource/image/db/42/dbb664bdf78211d8e2af8770f1cb9542.jpeg)
虚拟地址空间示意图
所以当CPU执行内存中一条指令的时候，它首先需要把VMA（虚拟内存区域）中的逻辑地址转换为线性地址，转化过程通过MMU（内存管理单元）实现。
![\"\"](https://static001.infoq.cn/resource/image/30/7d/304c1eaed32a3ffda2e61a7ee93cb17d.png)
虚拟地址与实际物理地址的映射
由于逻辑地址太大很难被有效地管理，于是引入了页（page）的概念。所有的虚拟内存空间被分成很多相对较小的区域（通常为4KB），也就是我们所称的页。页是虚拟内存管理中最小的单位，虚拟内存通常不存储任何内容，只是简单的将程序地址空间映射到底层的物理地址。
用户进程只能使用虚拟内存地址。让我们来看一下程序如何申请堆内存空间：
![\"\"](https://static001.infoq.cn/resource/image/61/06/61e9ebee604cab77a29cdded0f460406.png)
（堆内存申请的汇编实现）
![\"\"](https://static001.infoq.cn/resource/image/92/42/921d0f89e058a7fc15c1966c4a798b42.png)
堆内存增长
程序通常使用系统调用[brk](%5C)(sbrk/mmap)来获取更多的内存，内核仅更新堆的VMA，并没有进行进行实际的申请操作。
> 
系统在内存分配的时候，其实并没有申请相应的物理页帧，只有在真正赋值的时候才会申请物理页帧。这也是VSZ（进程虚拟内存大小）和RSS（常驻物理内存大小）的最大区别。
# 内存分配器
相信通过前面对“虚拟地址空间”以及堆内存申请的学习，相信我们对内存分配器说也就不难理解了。
> 
如果堆中有足够多的内存空间，那么分配器就可以独立完成内存的申请而不需要访问内核。否则，系统将会通过系统调用函数brk来扩展堆，通常是增加变量MMAP_THRESHOLD 的默认值(128KB)。
当然内存分配器的职责不仅仅是更新brk地址，更多的还是用于减少碎片以及快速分配内存块。让我们来看一个实例：假设我们的程序通过函数malloc来申请一块连续内存块，使用函数free来释放申请的内存块，步骤p1到p4的整个操作顺序如下：
![\"\"](https://static001.infoq.cn/resource/image/17/52/17bfccf29e31ae9b0bd51d2d5f363a52.png)
内存碎片演示
到步骤p4的时候，尽管剩余的内存块数量大于需要申请的数量，但是因为碎片的关系，我们已经不能获得6个连续的内存块了。我们该如何减少内存碎片呢？答案要取决于具体使用的分配算法。
由于Go内存分配器同TCMalloc分配器非常相似，我们先看一下相对简单的TCMalloc。
## TCMalloc
[TCMalloc](%5C)（Thread Cache Malloc）的核心思想是将内存分解为多层，从而减小内存锁的粒度。TC-Malloc内存管理分为**线程内存**以及**页堆**两部分：
# 线程内存
为减少内存碎片，每个内存页都被分成了多个固定类大小的空闲列表。这样每一个线程都都有一个不带锁的小对象缓存，从而可以高效的为并行程序分配小对象（\u0026lt;=32KB）。
![\"\"](https://static001.infoq.cn/resource/image/f5/bf/f560cae3d8fb8c7f897b409247b96dbf.png)
线程缓存 (每个线程都有一个本地线程缓存)
## 页堆
TCMalloc管理的堆其实由一组页构成，而这样一组连续的页又被称为页堆（span）。当我们申请大于32K的对象时，TCMalloc将使用页堆进行分配。
![\"\"](https://static001.infoq.cn/resource/image/aa/59/aaa8fad1a0b965b57566fef511277e59.png)
页堆 （span）管理
当没有足够的内存来分配小对象时，将使用页堆内存；而如果页堆内存也不不能满足时，将会向操作系统申请更多的内存。这种基于用户空间内存池的管理模式极大地提高了内存分配和释放的效率。
> 
注: 早期的go内存分配器是基于TCMalloc开发的，但时至今日，两者已经大不相同了。
## Go内存分配器
Go运行时调度器其实把Goroutines (G)绑定到逻辑处理器（P）上执行。同TCMalloc一样，Go内存分配器将内存页分成了67个不同类大小的块。
> 
如果你不熟悉Go调度器的话，建议先阅读一下文章（[Go scheduler: Ms, Ps \u0026amp; Gs](%5C)）。
![\"\"](https://static001.infoq.cn/resource/image/c4/45/c463ee466da2771d8f35e54d32648845.png)
Go中的内存页大小列表
Go中内存的最小粒度为8KB，如果页被分成大小为1KB的块，那么将会有如下8个块。
![\"\"](https://static001.infoq.cn/resource/image/7f/6b/7f403c65ead51e35715f6af8923fa46b.png)
8 KB的页被成了8个大小为1KB的块
Go通过数据结构mspan来管理这些页。
## mspan
简单来讲，mspan是一个双端链表，包含了页起始地址，span类以及这个类中页的数量。
![\"\"](https://static001.infoq.cn/resource/image/cd/c9/cd0a263ea692c52a8c709dbe3032f2c9.png)
mspan示意图
# mcache
同TCMalloc一样，Go内存分配器为每一个逻辑处理器§提供了一个本地线程缓存，也就是mcache。如果Goroutine需要内存，可以直接从mcache中获取，由于只有一个Goroutine运行在逻辑处理器（P）上，所以中间不需要使用任何锁。
mcache包含了所有类大小的mspan。
![\"\"](https://static001.infoq.cn/resource/image/78/2a/7867c2cb84b642d9d61ec1b6e6fc7b2a.png)
Go中P、mcache以及mspan的关系示意图
> 
*由于mcache是基于CPU存在的，从mcache获取内存时没有必要使用锁机制。*
每一种类大小的mspan都有两种类型：
- scan — 含有指针的对象。
- noscan — 没有指针的对象。
这样分类的好处是在垃圾回收的时候，不需要遍历noscan对象（noscan中根本就没有指针）。
**那什么情况下内存分配器会从mcache中申请内存呢?**
> - \u0026lt;=32K字节的对象将直接从mchae中相应大小的mspan申请。*
**如果mcache没有可用空间的时候会怎么样?**
将会从mcentra中相应大小的mspanl列表中分配一个新的mspan。
# mcentral
mcentral对象收集了所有给定类大小的span，每一个mcentral都包含了两个mspan列表：
- empty mspanList — 没有空闲对象或者已经被mcache缓存的mspans列表。
- noempty mspanList — 所有空闲对象的span列表。
![\"\"](https://static001.infoq.cn/resource/image/1a/7c/1a99c2cdc5c6a79522ba598dabf4a87c.png)
mcentral结构示意图
每一个mcentral结构体都由mheap结构体维护。
# mheap
> 
mheap是一个全局变量，管理着Go中所有的虚拟地址空间。
![\"\"](https://static001.infoq.cn/resource/image/ff/67/ffcb59dd8f3929f98b623ea109719667.png)
mheap示意图
如上图所示：mheap保存了一个mcentral的数组，而mcentral又保存了所有span。
`central [numSpanClasses]struct {  \tmcentral mcentral    \tpad      [sys.CacheLineSize unsafe.Sizeof(mcentral{})%sys.CacheLineSize]byte}`
> 
因为我们用mcentral保存了所有span，当mchache向mcentral申请一个mspan的时候，我们需要锁住mcentral层，但我们还是可以同时请求其他大小的mspan。
Padding（对齐填充）确保了mcentrals按照CacheLine的大小对齐，所以每一个MCentral.lock 都可以获得自己的的cache line，避免了伪共享问题。
如果mcentral为空会发生什么呢？mcentral会从mheap中申请一些页来创建不同大小的span。
- free[_MaxMHeapList]mSpanList: 一个 spanList数组。每一个spanList中的mspan都包含了1 ~ 127个页 (_MaxMHeapList - 1) 。例如，free[3]是一个包含了3个页的mspan链表，Free表示列表为空，未分配，对应busy list。
- freelarge mSpanList: 一个mspan列表。每一个元素的页数大于127。通过数据结构mtreap来管理，对应busylarge。
> 
大于32K的对象被称为大对象，直接从mheap中申请。每次申请大对象都需要事先调用一个全局锁，因此每次只能处理一个P申请。
- 小于16B，使用mcache的Tiny分配。
- 大小介于16B和32k，计算sizeClass的大小，然后在mcache中申请相应大小的内存块。
- 大于32k的大对象, 直接从mheap中分配。
- 如果mcache中找不到相应大小的内存块，则转向mcentral申请。
- 如果mcentral中也没有相应大小的内存块，则转向mheap申请，使用BestFit策略找寻最合适的mspan；如果申请到的mspan太大，则根据用户的需求进行切分，剩余的页构成一个新的mspan，并放回到mheap的空闲列表。
- 如果mheap中没有可用的span，将会直接向操作系统申请新的内存页（至少1M）
> 
如果要申请更大的内存块（arena），将会转向操作系统申请。一次申请大批量的内存页会减少访问操作系统的次数。
所有在在堆上申请的内存都来自arena，让我们接下来看一看arean：
# Go虚拟内存：Arena
让我们通过一个简单的Go程序来看一下内存使用情况：
`func main() {    for {}}`
![\"\"](https://static001.infoq.cn/resource/image/00/a7/003011f23d806b42d752277c7de440a7.png)
程序进程信息统计
即便是只有三行的小程序也使用了大约100MB的虚拟内存，但RSS（实际物理内存占用大小）
仅为696KB。让我们先看一下两者的区别：
![\"\"](https://static001.infoq.cn/resource/image/81/c9/816bcda7e1fb1b3793e5298d6d7c3bc9.png)
map和smap统计
这里有一些大小为2MB、32MB和64MB的内存区域，这些区域其实就是arena内存块。
Go的虚拟内存其实由一系列的arena构成，初始堆映射也是一个arena，如go 1.11.5
采用了64MB的arena内存块。
![\"\"](https://static001.infoq.cn/resource/image/0d/54/0de966eecab63a036fed6e2d08e2ed54.png)
不同系统中arena大小
当前Go内存分配器是按照程序需要逐步增加内存映射的，初始只预留留了一个arena的大小（约64MB）。而早期的Go内存分配器会先保留一大段虚拟内存，在64位系统上为512GB（发散问题：如果申请的内存太大，以至于被mmap拒绝了怎么办？）
这些arena就是我们所说的堆。Go中每一个arena都按照8KB的粒度进行管理。
![\"\"](https://static001.infoq.cn/resource/image/b2/7b/b219076f40e890028a1518dfa0ed5c7b.png)
单个arena ( 64 MB )
Go同时还有两个其它块：span和bitmap。两者都独立于堆内存空间之外，并且保存了所有arena的元数据。他们主要在垃圾回收的时候使用，我们暂且不在这里讨论。
# 结语
我们刚刚讨论的内存分配策略只是众多内存分配器的冰山一角。但其管理核心本质上是一致的：针对不同大小的对象，在不同的cache层中，使用不同的内存结构；将从系统中获得的一块连续内存分割成多层次的cache，以减少锁的使用以提高内存分配效率；申请不同类大小的内存块来减少内存碎片，同时加速内存释放后的垃圾回收。
最后让我们用GO内存分配器的结构示意图作为结束：
![\"\"](https://static001.infoq.cn/resource/image/c0/1a/c05ea151ac6e0fb22c250df77c56551a.png)
内存分配器示意图
**英文原文地址**：[https://blog.learngoprogramming.com/a-visual-guide-to-golang-memory-allocator-from-ground-up-e132258453ed](%5C)
