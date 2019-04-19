# Go语言内幕（6）：启动和内存分配初始化 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [yhx](http://www.jobbole.com/members/scbzyhx) 翻译，[黄利民](http://www.jobbole.com/members/huanglimin) 校稿。未经许可，禁止转载！
英文出处：[Siarhei Matsiukevich](http://blog.altoros.com/golang-internals-part-6-bootstrapping-and-memory-allocator-initialization.html)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
- 《[Go语言内幕（1）：主要概念与项目结构](http://blog.jobbole.com/90574/)》
- 《[Go语言内幕（2）：深入 Go 编译器](http://blog.jobbole.com/90875/)》
- 《[Go语言内幕（3）：链接器、链接器、重定位](http://blog.jobbole.com/92371/)》
- 《[Go语言内幕（4）：目标文件和函数元数据](http://blog.jobbole.com/92368/)》
- 《[Go语言内幕（5）：运行时启动过程](http://blog.jobbole.com/93100/)》
本文是 Golang 内部机制探索系列博客的后续。这个系列博客的目的是探索 Go 启动过程，这个过程也是理解 Go 运行时（runtime）的关键之处。本文中我们将一起去看看启动过程的第二个部分，分析参数是怎么被初始化的及其中有哪些函数调用等等。
### 启动顺序
我们从上次结束的地方继续。在 runtime.r0_to 函数中，我们还有一部分没有分析：

Assembly (x86)
```
CLD                         // convention is D is always left cleared
        CALL    runtime·check(SB)
        MOVL    16(SP), AX          // copy argc
        MOVL    AX, 0(SP)
        MOVQ    24(SP), AX          // copy argv
        MOVQ    AX, 8(SP) 
        CALL    runtime·args(SB)
        CALL    runtime·osinit(SB)
        CALL    runtime·schedinit(SB)
```
第一条指令（CLD）清除 FLAGS 寄存器方向标志。该标志会影响到 string 处理时的方向。
接下来调用 runtime.check 函数，这个函数对我们分析运行时并没什么太大的帮助。在该函数中，运行时创建所有内置类型的实例，检查他们的大小及其它参数等。如果其中出了什么错，就会产生 panic 错误。请读者自行阅读这个函数的代码。
### 参数分析
runtime.check 函数后调用 runtime.Args 函数，这个函数更有意思一些。除了将参数（argc 和 argv ）存储到静态变量中之外，在 Linux 系统上时它还会分析 处理 ELF 辅助向量以及初始化系统系统调用的地址。
这里需要解释一下。操作系统将程序加载到内存中时，它会用一些预定义格式的数据初始化程序的初始栈。在栈顶就存储着这些参数–指向环境变量的指针。在栈底，我们可以看到 “ELF 辅助向量”。事实上，这个辅助向量是一个记录数组，这些记录存储着另外一些有用的信息，比如程序头的数量和大小等。更多关于 ELF 辅助向量的内容请参考这篇[文章](http://articles.manugarg.com/aboutelfauxiliaryvectors)。
runtime.Args 函数负责处理这个向量。在辅助向量存储的所有信息中，运行时只关心 startupRandomData，它主要用来初始化哈希函数以及指向系统调用位置的指针。在这里初始化了以下这些变量：


```
__vdso_time_sym 
__vdso_gettimeofday_sym 
__vdso_clock_gettime_sym
```
它们用于在不同的函数中获取当前时间。所有这些变量都有其默认值。这允许 Golang 使用 [vsyscall](http://www.ukuug.org/events/linux2001/papers/html/AArcangeli-vsyscalls.html)机制调用相应的函数。
### runtime.osinit 函数
在启动过程中接下来调用的是 runtime.osinit 函数。在 Linux 系统上，这个函数唯 一做的事就是初始化 ncpu 变量，这个变量存储了当前系统的 CPU 的数量。这是通过一个系统调用来实现的。
### runtime.schedinit 函数
接下便调用了 runtime.schedinit 函数，这个函数比较有意思。首先，它获得当前 goroutine 的指针，该指针指向一个 g 结构体。在讨论 TLS 实现的时候，我们就已经讨论过这个指针是如何存储的。接下来，它会调用 runtime.raceinit。这里我们不会讨论 runtime.raceinit 函数，因为正常情况下竞争条件（race condition）被禁止时，这个函数是不会被调用的。随后，runtime.schedinit 函数中还会调用另外一些初始化函数。
让我们依次来看一下。
### 初始化 traceback
runtime.tracebackinit 负责初始化 traceback。traceback 是一个函数栈。这些函数会在我们到达当前执行点之前被调用。举个例子，每次产生一个 panic 时我们都可以看到它们。 Traceback 是通过调用 runtime.gentraceback 函数产生的。要让这个函数工作， 我们需要知道一些内置函数的地址（例如，因为我们不希望它们被包含到 traceback 中）。runtime.traceback 就负责初始化这些地址。
### 验证链接器符号
链接器符号是由链接器产生输出到可执行目标文件中的数据。其中大部分数据已经在《[Go语言内幕（3）：链接器、链接器、重定位](http://blog.jobbole.com/92371/)》中讨论过了。在运行时包中，链接器符号被映射到 moduledata 结构体。 runtime.moduledataverify 函数负责检查这些数据，以确保所有结构体的正确性。
### 初始化栈池
要想搞明白接下来这个步骤，你需要了解一点 Go 中栈增长的实现方法。当一个新的 goroutine 被生成时，系统会为其分配一个较小的固定大小的栈。当栈达到某个阈值时，栈的大小会增大一倍并将原来栈中的数据全部拷贝到新的栈中。
还有许多细节，比如如何判断是否达到阈值，Go 如何调整栈中的指针等。在前面的博客中介绍 stackguard0 与函数元数据时，我已经介绍了部分相关的内容。更多的内容，你可以参考这篇[文档](https://docs.google.com/document/d/1wAaf1rYoM4S4gtnPh0zOlGzWtrZFQ5suE8qr2sD8uWQ/pub)。
Go 用栈池来缓存暂时不用的栈。这个栈池实际上就是一个由 runtime.stackinit 函数初始化的数组。这个数组中的每一项是一个包含相同大小栈的链表。
这一步还初始化了另外一个变量 runtime.stackFreeQueue。这个变量也存储了一个栈的链表，但是这些栈都是在垃圾回收时加入的，并且回收结束时会被清空。注意，只有大小为 2 KB，4 KB，8 KB，以及 16 KB 的栈才能会被缓存。更大的栈则会直接分配。
初始化内存分配器
内存分配的过程在这篇[源代码注解](https://github.com/golang/go/blob/go1.5.1/src/runtime/malloc.go#L5)有详细的介绍。如果你想搞明白 Go 内存分配是如何工作的话，我强烈建议你去阅读该文档。关于内存分配的内容，我会在后面的博客中详细分析。内存分配器的初始化在 runtime.mallocinit 函数中完成的，所以让我们仔细看一下这个函数。
### 初始化大小类
我们可以看到 runtime.mallocinit 函数做的第一件事就是调用另外一个函数– initSizes。这个函数用于计算大小类。但是，每一个类应该多大呢？分配小对象（小于 32 KB）时，Go 运行时先将大小调整为运行时既定义的类的大小。因此分配的内存块的大小只可能是既定义的几个大小之一。通常情况下，分配的内存会比请求的内存大小更大。这会导致小部分内存的浪费，但是这可以让我们更好地复用这些内存块。
initSizes 函数负责计算这些类的大小。在这个函数开始处，我们可以以看到如下的代码：

Go
```
align := 8
	for size := align; size <= _MaxSmallSize; size += align {
		if size&(size-1) == 0 { 
			if size >= 2048 {
				align = 256
			} else if size >= 128 {
				align = size / 8
			} else if size >= 16 {
				align = 16 
…
			}
		}
```
我们可以看到最小的两个类的大小分别是 8 字节与 16 字节。随后每递增 16 字节为一个新的类一直到 128 字节。从 128 字节到 2048 字节，类的大小每次增加 size/8 字节。2048 字节后，每递增 256 字节为一个新类。
initSize 方法会初始化 class_to_size 数组，该数组用于将类（这里指其在全局类列表中的索引值）映射为其所占内存空间的大小。initSize 方法还会初始化 class_to_allocnpages。这个数组存储对于指定类的对象需要多大的存储空间。除此之外，size_to_class8 与 size_to_class128 两个数组也是在这个方法中初始化的。这两个数组用于根据对象的大小得出相应的类的索引。前者用于大小小于 1 KB 的对象，后者用于 1 – 32 KB 大小的对象。
### 虚拟内存的预约
下面，我们会一起看看虚拟内存预约函数 mallocinit，此函数会提前从操作系统分配一部分内存用于未来的内存分配。让我们看一下它在 x64 架构下是如何工作的。首先，我们需要初始化下面的变量：

Go
```
pSize = bitmapSize + spansSize + arenaSize + _PageSize  
p = uintptr(sysReserve(unsafe.Pointer(p), pSize, &reserved))
```
- bitmapSize 对应于垃圾收集器位图所需的内存的大小。垃圾收集器的位图是一块特殊的内存，该内存标明了内存中哪些位置是指针哪些位置是对象，以方便垃圾收集器释放。这块空间由垃圾收集器管理。对于每个分配的字节，我们需要两个比特存储信息，这也就是为什么位图所需内存大小的计算式为：*arenaSize / (ptrSize * 8 / 4)*
- spanSize 表示存储指向 memory span 的指针数组所需内存空间大小。所谓 memory span 是指一种将内存块封装以便分配给对象的数组结构。
上述所有变量计算出来后，就可以完成真正的资源预留的工作了：

Go
```
pSize = bitmapSize + spansSize + arenaSize + _PageSize  
p = uintptr(sysReserve(unsafe.Pointer(p), pSize, &reserved))
```
最后，我们初始化全局变量 mheap。这个变量用于集中存储内存相关的对象。

Go
```
p1 := round(p, _PageSize)
mheap_.spans = (**mspan)(unsafe.Pointer(p1))
mheap_.bitmap = p1 + spansSize
mheap_.arena_start = p1 + (spansSize + bitmapSize)
mheap_.arena_used = mheap_.arena_start
mheap_.arena_end = p + pSize
mheap_.arena_reserved = reserved
```
注意，初始始 mheap_.arena_used 的值与 mheap_.arena_start 相等，这是因为还没有为任何对象分配空间。
### 初始化堆
接下来，调用 mHeap_Init 函数来初始化堆。该函数所做的第一件事就是初始化分配器。

Go
```
fixAlloc_Init(&h.spanalloc, unsafe.Sizeof(mspan{}), recordspan, unsafe.Pointer(h), &memstats.mspan_sys)
fixAlloc_Init(&h.cachealloc, unsafe.Sizeof(mcache{}), nil, nil, &memstats.mcache_sys)
fixAlloc_Init(&h.specialfinalizeralloc, unsafe.Sizeof(specialfinalizer{}), nil, nil, &memstats.other_sys)
fixAlloc_Init(&h.specialprofilealloc, unsafe.Sizeof(specialprofile{}), nil, nil, &memstats.other_sys)
```
为了更好的理解分配器，让我们先看一看是如何使用它的。每当我们希望分配新的 mspan、mcache、specialfinalizer 或者 specialprofile 结构体时，都可以通过 fixAlloc_Alloc 函数来调用分配器。 此函数的主要部分如下：

Go
```
if uintptr(f.nchunk) < f.size {
	f.chunk = (*uint8)(persistentalloc(_FixAllocChunk, 0, f.stat))
	f.nchunk = _FixAllocChunk
}
```
它会分配一块内存，但是它并不是按结构体的实际大小（f.size）进行分配，而是直接留出 _FixAllocChunk （目前是 16 KB）大小的空间。多余的存储空间存储在分配器中。当下一次再为相同的结构体分配空间时，就勿需再调用耗时的 persistentcalloc 操作。
persistentalloc 函数用于分配不会被垃圾回收的内存空间。它的工作流程如下所示：
- 如果分配的块大于 64 KB， 则它直接从 OS 内存中分配。
- 否则，找到一个永久分配器（persistent allocator）。
- 每个永久分配器与一个进程对应。其主要是为了在永久分配器中使用锁。因此，我们使用永久分配器时都是使用的当前进程的永久分配器。
- 如果不能获得当前进程的信息，则使用全局的分配器。
- 如果分配器已经没有足够多的空闲内存，则从 OS 申请更多的内存。
- 从分配器的缓存中返回所请求大小的内存。
persistentalloc 与 fixAlloc_Alloc 函数的工作机制是非常相似的。可以说，这些函数实现了一个两级的缓存机制。你应该可以意识到 persitentalloc 函数不仅仅只在 fixAlloc_Alloc 函数中使用，在其它很多使用永久内存的地方都会用到它。
让我们再回到 mHeap_Init 函数中。一个亟需回答的问题是在函数开始时初始化的四个结构体到底有什么用：
- mspan 只是那些应该被垃圾回收的内存块的一个包装。在前面讨论内存大小分类时，我们已讨论过它了。当创建一个特定大小类别的对象时就会创建一个 mspan。
- mcache 是每个进程相关的结构体。它负责缓存扩展。每外进程拥有独立的 mcache 主要是为了避免使用锁。
- specialfinalizeralloc 是在 runtime.SetFinalizer 函数调用时分配的结构体，而这个函数是在我们希望系统在对象结束时执行某些清理代码的时候调用的。例如，os.NewFile 函数就会为每个新文件关联一个 finalizer。而这个 finalizer 负责关闭系统的文件描述符。
- specialprofilealloc 是在内存分析器中使用的一个结构体。
初始化内存分配器后，mHeap_Initfunction 会调用 mSpanList_Init 函数初始化链表。这个过程非常的简单，它所做的所有初始化工作仅仅是初始化链表的入口结点。mheap 结构体包含多个这样的链表。
- mheap.free 与 mheap.busy 数组用于存储大对象的空闲链表（大对象指大于 32 KB 而小于 1 MB 的对象）。每个可能的大小都在数组中都有一个对应的项。在这里，大小是用页来衡量的，每个页的大小为 32 KB。也就是说，数组中的第一项链表管理大小为 32 KB 的内存块，第二个项的管理 64 KB 的内存块，依次类推。
-  mheap.freelarge 与 mheap.busylarge 是大小于 1 MB 对象空间的空闲与忙链表。
接下来就是初始化 mheap.central，该变量管理所有存储小对象（小于 32 KB）的内存块。mheap.central 中，链表根据其管理内存块的大小进行分组。初始化过程与前面看到的非常类似，初始化过程中只是将所有空闲链表进行初始化。
### 初始化缓存
现在，我们几乎已完成了所有内存分配器的初始化。mallocinit 函数中剩下的最后一件事就是 mcache 的初始化了：

Go
```
_g_ := getg()
_g_.m.mcache = allocmcache()
```
首先获得当前的协程。每个 goroutine 都包含一个指向 m 结构体的指针。该结构体对操作系统线程进行了包装。在这个结构体的 mcache 域就是在这几行代码中初始化的。 allomcache 函数调用 fixAlloc_Alloc 初始化新的 mcache 结构体。我们已经讨论过了该结构体的分配以及其含义了。
细心的读者可能注意到我前面说每个 mcache 与一个进程关联，但是我们现在又说它与 m 结构体关联，而 m 结构体是与 OS 进程相关联，而非一个处理器。这并不是一个错误，mcache 只有在进程正在执行时才会初始化，而每当进程切换后它也重新切换为另外一个线程 m 结构体。
### 更多关于 Go 启动过程
再接下来的博客中，我们会继续讨论启动过程中的垃圾收集器的初始化过程以及主 goroutine 是如何启动的。同时，欢迎大家积极在博客中评论。
