# 「EA 如何做内存和 C++ 调试的」实录 - 文章 - 伯乐在线
原文出处： [顾露（@顾露-Gu_Lu）](http://gulu-dev.com/post/2015-10-11-memory-debugging)
Scott Wardle 在 CppCon 2015 上的分享题为《Memory and C++ debugging at EA》，是关于内存和调试方面的一些心得。这里是[视频链接](https://www.youtube.com/watch?v=8KIvWJUYbDA)(需梯子)，和[演讲稿链接](https://github.com/CppCon/CppCon2015/tree/master/Presentations/Memory%20and%20C%2B%2B%20debugging%20at%20EA)。
![](http://ww2.sinaimg.cn/mw690/5e4d414cgw1exuftzsclxj20zi0i2gnt.jpg)
Scott 同学有 20+ 年游戏开发的经验，这个分享包括了在不同时期 (2000 年的 PS2 时期，2005 年左右的 XBox360/PS3 时期，当前的 PS4/XBox One 时期) 的技术演进情况，在此分享中，我收获颇多，这里简单记录一下。
以 [GL_Note] 开头的是我夹带的私货，见谅。
### **2000 年左右时的一些原始工具和策略**
2000 年左右时，不少程序员都是从 C 转过来没多久。那时的常见做法是像下面这样重载 new：
![](http://ww2.sinaimg.cn/mw690/5e4d414cgw1exufu04dt3j20c4063wf3.jpg)
和这样：
![](http://ww4.sinaimg.cn/mw690/5e4d414cgw1exufu0v8mij20g006wgn9.jpg)
debug_name 用于标示用途，flag 标示分配方向等选项。相信大家都这么干过吧。
内存中的布局是这样的：
![](http://ww2.sinaimg.cn/mw690/5e4d414cgw1exufu0wbi9j20g003ygm1.jpg)
除了一块专用的“小块内存分配器”外，其他一整块地址空间，从两端开始往中间用。
对待内存碎片化的处理主要是按照生命期，把临时的短暂的内存放前面，较长的放后面。
![](http://ww1.sinaimg.cn/mw690/5e4d414cgw1exufu1sumnj20g304bt9j.jpg)
上图中的典型例子就是在 Low 这边把贴图读入内存，再解压缩到 High 那头，保证短期的和长期的互不干扰，就不易形成空洞。
### **2005 年左右的进展情况**
到了 2005 年也就是 360/PS3 的年代，开始支持多分配器：
![](http://ww1.sinaimg.cn/mw690/5e4d414cgw1exufu1tr2tj20eb07iab1.jpg)
C++ 里面重载 delete 是不能有参数的，所以析构时还要手动传分配器，比较痛苦。
[GL_Note] 这个问题实践中可通过直接在分配出的 block header 里存放 allocator 的指针来解决。但这样会为每块内存浪费 4 个字节，当然通常 allocator 数量很少，一个 byte 也许就够了。
[GL_Note] 还有一个见过的做法是，规定凡是自定义的 allocator 都划分自己专属的一段内存，拿任意一个指针/地址过来，通过对某些区间位做一下位运算，就能判定是哪个定制的 allocator，如果都不是的话，就是默认的 allocator。这种做法用地址空间上的限制消除了额外记录的需求。
更好的利用地址空间的策略 (以时间，尺寸和不同的团队边界等作为标准)：
![](http://ww4.sinaimg.cn/mw690/5e4d414cgw1exufu2ahiuj20cj03smxv.jpg)
[GL_Note] 简单解释一下，
- 第一行时间因素：时间条上从左向右的每一项的生命期都显著不同，把它们彼此标记和隔离，有助于从根本上避免产生碎片。
- 第二行尺寸因素：按照尺寸尽量把不同量级的内存分开，可以让新的内存请求更有效地 fit 进已有的空洞，从而提高利用率，降低极限情况下的最大尺寸开销。
- 第三行团队因素：按照团队切分，能有效地快速定位问题到不同的组 (就能快速找到负责的人) 这里的 SBA 是 Small Block Allocator。 这三种特征通常需要综合起来考虑。
不管以何种方式分块，块与块的边界处的 corruption 都是比较难以处理的，如下图：
![](http://ww2.sinaimg.cn/mw690/5e4d414cgw1exufu2idrjj20ci02kgm3.jpg)
他们意识到，如果像之前那样把一些调试信息放在新分配的内存的尾部，当发生 corruption 时十有八九就会被写坏，妨碍查错。于是就单独开了个调试堆，把地址尺寸分类标示等调试信息 hash 后存在这里。
![](http://ww4.sinaimg.cn/mw690/5e4d414cgw1exufu31hksj20cr05n75r.jpg)
这样当 corruption 发生时，可以精确地找到当时的时间和空间的上下文，看看发生了什么。
所有这些信息同时被记录在硬盘上，如下图。
![](http://ww3.sinaimg.cn/mw690/5e4d414cgw1exufu3gwa2j20cl05k0tx.jpg)
可以选择和查看任意一个时间点上的分配情况，也可以选择一段时间区间，查看在那一段时间里变化的部分。每一个分配都可以查看对应的堆栈信息。
这是 BlockView，可以从空间上直观地看到不同类型内存的分配情况，以及空间上不同区域的利用率和碎片化的信息。
![](http://ww3.sinaimg.cn/mw690/5e4d414cgw1exufu3t03rj20by06bdgy.jpg)
当选中一个 block 时，可以看到那个 block 相关的详细信息 (左下角)
另一个强力工具是 Stomp Allocator：
![](http://ww1.sinaimg.cn/mw690/5e4d414cgw1exufu4fwdej20b605tgm8.jpg)
这个专门用来查 corruption 的。当内存请求发生时，它利用虚拟内存分配 4k 的可读写内存并返回尾部的可用空间，并在后面追加一个 4k 的只读内存，这样一旦发生越界写立刻就会 crash。这个工具因为内存开销大，所以总是在**已经定位到较小范围内的怀疑对象**时使用。
关于智能指针的循环依赖问题，
![](http://ww4.sinaimg.cn/mw690/5e4d414cgw1exufu4jqoaj20cf03n3z5.jpg)
Scott 说如果加上循环依赖的检测就开始变得像垃圾回收了。所以明确使用规则，避免滥用即可。
[GL_Note] 简单解释一下，
> 
智能指针的使用规则很简单，一句话就可以概括：当生命期明确的时候，使用 unique_ptr；只有当需要共享对象/数据的所有权导致生命期不确定的时候，才使用 shared_ptr。
这条规则隐含着一个认识：在绝大多数情况下，**相互依赖的双方，必有一方生命期是相对确定的**，否则常常说明有隐含的设计问题。
接着 Scott 说到了 EASTL，
![](http://ww3.sinaimg.cn/mw690/5e4d414cgw1exufu54rvzj20aj05ljrz.jpg)
在 188 个单独的测试中，大部分比最新的 VS2015 自带的快，debug 版更是快上两个数量级。
![](http://ww2.sinaimg.cn/mw690/5e4d414cgw1exufu593baj20ah05ywev.jpg)
[GL_Note] 除了运行速度，一直以来我惊讶的是 EASTL 的良好的可读性，不得不说这是诸多 STL 版本里，最接近写给人看的版本。试举一例，摘自[这里](https://github.com/paulhodge/EASTL/blob/community/include/EASTL/heap.h)
![](http://ww3.sinaimg.cn/mw690/5e4d414cgw1exugd3cpbhj20kk0as421.jpg)
接下来 Scott 讲了一些 EASTLICA (EASTL ICoreAllocator) 的实现细节和一些传参和 type erasure 的问题处理。这些问题都属于 stl 定制 allocator 相关的问题，在网络上讨论也很普遍，实际上因为 EASTL 是一个专属版本，在这个专属环境下问题更容易协调和解决，这里就不多说了，感兴趣的可以直接看视频。
### **目前 (2015) 的系统**
他们对逐渐开发出来的各种调试工具进行了强力的整合，下面逐一介绍。
**内存调试工具改进**
首先是内存分配的接口逐渐不再使用一个单一的 debug_name (因为这种单个的字符串标签提供的信息量太小了)，而是使用了 scope 这个上下文相关的概念，来把更多的信息关联到这次内存分配，比如跟对应的资源名及子系统名挂钩。
其次，现在任意一个 allocator 都可以方便地找到自己所在的上一级内存区域 (parent arena)，可以根据这个调整自己的行为。
比如下面这个类 (其中的 eastl 使用了上面提到的 EASTLICA)
![](http://ww4.sinaimg.cn/mw690/5e4d414cgw1exufu5whf5j209s04wmx9.jpg)
由于可以利用这些额外的信息来定制分配策略，逻辑上相关联的对象在物理上也会分配在一起，最终在内存中的布局可能是下面这样：
![](http://ww3.sinaimg.cn/mw690/5e4d414cgw1exufu66fyij20h009jjsm.jpg)
**调试工具 DeltaViewer**
DeltaViewer 会记录游戏运行从头到尾的整个 session (one run of the game)，上传到一个 http server，并存在数据库里。
**日志 (Trace Log)**
首先是日志 (Trace Log) 的记录和查看：
![](http://ww4.sinaimg.cn/mw690/5e4d414cgw1exufu6m5znj20ej071zl5.jpg)
**IO 负载剖析器 (Turbo Tuner)**
IO 负载剖析器 (Turbo Tuner) 是一个查看任意时刻 IO 负载的工具，用这个可以很直观地看出系统性能受到 IO 影响的情况。
![](http://ww1.sinaimg.cn/mw690/5e4d414cgw1exufu6wymwj20gf06z3zn.jpg)
注意这里的 Bundles 是需要同步加载的完整资源，Chunks 是可异步加载的碎片资源。
仔细地看可以看到，上面第一行的 http log 可以看出任意时刻的 Log 量的大小和频繁程度；bundle states / chunk states 这两栏可以看到 IO 在不同状态间切换的时间点。
**关联使用**
Trace Log 跟 Turbo Tuner 这两个是关联的 (实际上后续介绍的这些工具相互之间都是相关联的)，也就是说对于一些关键的时间点，如果在日志中选择了对应的一条记录，可以精确地看到那个时间点上发生了什么，如下图：
![](http://ww1.sinaimg.cn/mw690/5e4d414cgw1exufu7aq0bj20dp07u0ty.jpg)
可以看到不同的游戏阶段，以及系统资源随时间流逝的变化情况，从而得到宏观的运行状况。
![](http://ww1.sinaimg.cn/mw690/5e4d414cgw1exufu7qrhtj20df07cabn.jpg)
当鼠标悬停在任意一次 bundle request 上时，可以得到那一次请求的所有相关的细节，如下图：
![](http://ww1.sinaimg.cn/mw690/5e4d414cgw1exufu83kw5j20d106ggn6.jpg)
可以看到有请求 ID (Sequence Number) / 序列 ID (可用来查前后时序相关的问题)，StartTime/EndTime/Duration (起始，终止和持续时间)，Priority (优先级)，Size / Patch Size 尺寸相关信息，所在的资源包名 (bundle name)，等等。
**Performance Timer**
接下来是性能剖析器 Frame rate and Job thread profiler (Performance Timer)
![](http://ww1.sinaimg.cn/mw690/5e4d414cgw1exufu8ul9fj20m80bsad8.jpg)
最上面一栏是帧率，每个蓝色条纹就是一帧。用鼠标选中就可以高亮那一帧及相邻的几帧。下面则依次是几个 CPU 上的负载情况，可以看到栈调用的层次关系和时间开销，很像 Telemetry 这一类工具，就不多说了。
这个工具跟前面的工具结合起来使用，看起来是下面这样子的：
![](http://ww1.sinaimg.cn/mw690/5e4d414cgw1exufu9a3kzj20h409mdhx.jpg)
**Memory Investigator**
接下来是使用 Memory Investigator 查找内存泄漏。
![](http://ww1.sinaimg.cn/mw690/5e4d414cgw1exufu9c0s5j20m80ag3zz.jpg)
传统意义上的内存泄漏是一个宽泛的概念，new 了之后只要最终 delete 了就不算内存泄漏。而在游戏里这个概念要严格得多，在关卡与关卡之间严格来讲不允许有累积的未释放内存，当第二关的加载结束时，理论上第一关范围内分配的内存都应已被释放。
![](http://ww2.sinaimg.cn/mw690/5e4d414cgw1exufu9teeij20h309l0ug.jpg)
用这个工具可以选择一个时间段 (A-B) 和一个时间点 C，然后列出在 (A-B) 这段时间内所有到了点 C 仍未被释放的内存分配，并查看它们的各种相关信息。
也可以查看不同的时间点上，内存的分类对比情况
![](http://ww1.sinaimg.cn/mw690/5e4d414cgw1exufua57pqj20h2096wfz.jpg)
可以看到不同尺寸 (512B/64K/2M/Large) 的内存被分类统计，其中一百多次大分配占据了 1.7G 左右，而两百多万的小分配占据了 100M 左右，这有助于我们更细致地了解内存的使用状况。
这是按照资源模块分类的情况
![](http://ww2.sinaimg.cn/mw690/5e4d414cgw1exufuaktvgj20h108omys.jpg)
### **小结和问答**
![](http://ww2.sinaimg.cn/mw690/5e4d414cgw1exufuar930j20d204rdg1.jpg)
![](http://ww4.sinaimg.cn/mw690/5e4d414cgw1exufubbo4vj20g5086abc.jpg)
在后面的问答中，有人问这个工具会不会开源，Scott 说目前不会，但 EASTLICA 可能会随着 EASTL 一起开源，所以日后也不排除这个可能性。关于 EASTL，有同学问性能提升主要来自哪里，Scott 回答说主要是 1) 用指针做 iterator 和 2) 不依赖 inline 把很深的嵌套调用拍扁。有人问获取这么多数据会影响游戏的运行性能吗，Scott 说他一直都很惊讶于这个工具的运行性能，游戏实时运行没有问题，基本上只会损失 10%-20% (3-4ms)。
![](http://ww2.sinaimg.cn/mw690/5e4d414cgw1exufubt6hdj20jy0bgwf1.jpg)
这个分享的信息量挺大，很多思路都非常有价值。受益匪浅，简单记录，以备日后参考。最后再提一下，如果我的细节描述不够，请移步前往视频以获得完整的内容。
