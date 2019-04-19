# LLVM官方文档翻译---- LLVM原子指令与并发指引 - SHINING的博客 - CSDN博客
2017年03月21日 11:34:18[snsn1984](https://me.csdn.net/snsn1984)阅读数：1655
英文原文地址：[http://llvm.org/docs/Atomics.html](http://llvm.org/docs/Atomics.html)
译文原文地址：[http://blog.csdn.net/wuhui_gdnt/article/details/52485591](http://blog.csdn.net/wuhui_gdnt/article/details/52485591)
注：该文章转载已经得到译者授权。
---------------------------------------------------------------------------------------------------------------------------------------------
## 介绍
LLVM支持在线程与异步信号面前定义良好的指令。
原子指令专门为以下定义来提供可读的IR以及优化的代码生成：
·        [C++](http://lib.csdn.net/base/cplusplus)11 <atomic>头文件。（[C++11草案在此](http://www.open-std.org/jtc1/sc22/wg21/)）。（[C11草案在此](http://www.open-std.org/jtc1/sc22/wg14/)）。
·        [Java](http://lib.csdn.net/base/javase)风格内存适用的语义，用于volatile以及规范的共享变量（[Java规范](http://docs.oracle.com/javase/specs/jls/se8/html/jls-17.html)）。
·        Gcc兼容的__sync__*固有函数（[描述](https://gcc.gnu.org/onlinedocs/gcc/_005f_005fsync-Builtins.html)）。
·        其他使用原子语义的场景，包括C++中带有非平凡构造函数的static变量。
在IR中原子化与volatile是正交的：volatile是C/C++的volatile，它确保每个volatile读、写会发生，且以声明的次序执行。几个例子：如果一个顺序一致（SequentiallyConsistent）写后紧跟着对同一个地址的另一个顺序一致写，第一个写可以被去除。对一对volatile写，这个转换是不允许的。另一方面，一个非volatile、非原子写可以自由地跨过一个volatile写，但不能跨过一个Acquire写。
本文意在对为LLVM写一个前端或者工作在LLVM优化遍的人提供，在并发条件下，如何处理具有特殊语义指令的一个指引。这不准备成为这些语义一个精确的指引；细节可以极端复杂且难以阅读，这通常不是必要的。
## 原子化以外的优化
基本的load与store允许各种优化，但在并发环境下会导致未定义的结果；参考NotAtomic一节。本节特别地进入应用在并发环境中的一个优化器限制，我们对描述稍作展开，因为任何处理store的优化都需要了解它。
从优化器看来，规则是：如果没有涉及任何带有原子化次序的指令，并发不是问题，除了一个例外：如果一个变量对另一个线程或信号处理句柄可见，一个写不能插入到它原本不会执行的路径上。考虑以下例子：
*/*C code, for readability; run through clang -O2 -S -emit-llvm to get*
*    equivalent IR */*
int x;
voidf(int* a) {
**for** (int i =0; i <100; i++) {
**if** (a[i])
       x +=1;
   }
 }
下面是非并发情形下等效的代码：
int x;
voidf(int* a) {
int xtemp = x;
**for** (int i =0; i <100; i++) {
**if** (a[i])
      xtemp +=1;
  }
  x = xtemp;
}
不过，LLVM不允许从前者到后者的转换：它会间接地引入未定义行为，如果另一个线程可以同时访问x。（这个例子特别令人感兴趣，是因为在实现并发模型之前，LLVM会执行这个转换）。
注意投机性的写（speculative load）是允许的；作为一个竞争的一部分的写返回undef，但不是未定义的行为。
## 原子指令
对于简单读写不足够的情形，LLVM提供了各种原子指令。提供的具体保证依赖于表序（ordering）；参见“原子化表序”一节
原子读写提供了与非原子读写相同的基本语义，但在涉及线程与信号的环境里，提供了额外的保证。
cmpchg与atomicrmw本质上就像一个原子读，后跟一个原子写（对cmpxchg写是有条件的），但在读写之间，在任何线程上都不能发生其他内存操作。
Fence提供了不少另一个操作部分的Acquire以及/或者Release表序；它通常与单调的（Monotonic）内存操作一起使用。一个单调读后跟一个Acquirefence大致上等同于一个Acquire读，一个单调写后跟一个Releasefence大致上等同于一个Release写。顺序一致fence同时具有一个Acquirefence以及一个Releasefence的行为，并提供了额外的复杂的保证，细节参考C++11标准。
产生原子指令的前端通常需要一定程度地了解目标机器；原子指令是保证无锁的，因此比目标机器天然支持的宽度要宽的指令是不可能产生的。
## 原子化表序
为了实现性能与必要保证间的平衡，有6级原子性。它们按排序强度列出；每级包括前面级别的所有保证，除了Acquire/Relase。（还可参考[LangRef Ordering](http://llvm.org/docs/LangRef.html#ordering)一节）
### [NotAtomic](http://llvm.org/docs/Atomics.html#id8)
NotAtomic是显而易见的，一个非原子的读或写。（这不是真正的一级原子性，但列在这里用于比较）。这本质上是一个普通的读或写。如果在一个给定内存位置有竞争，从该位置读将返回undef。
相关标准
这目的在于匹配C/C++中的共享变量，用在任何其他内存访问是必须的，竞争是不可能的上下文中。（精确的定义在[LangRef Memory Model](http://llvm.org/docs/LangRef.html#memmodel)一节）
对前端的注解
这个规则本质上是多线程所有以基本读写进行的内存访问都应该由一把锁或其他同步机制保护；否则，你很可能碰到未定义的行为。如果你的前端是用于一个像Java的“安全”语言，对任何共享变量的读写使用Unordered。注意NotAtomicvolatile读写不是正确原子化的；不要尝试使用它们作为替代。（按照C/C++标准，volatile确实围绕异步信息提供了一些有限的保证，但原子化通常是一个更好的方案）。
对优化器的注解
在原来不存在读的代码路径上对共享变量引入读是允许的；对共享变量引入写则不允许。参考“原子化以外的优化”一节。
对代码生成的注解
这里一个有趣的限制是，不允许写一个写操作相关字节之外的字节。这与非对齐写最为相关：通常不允许将一个非对齐写转换为两个与之具有相同宽度的对齐写。后端也被预期将一个i8写生成为一个i8写，而不是一条写入周围字节的指令。（如果正在为一个不能满足这些限制同时关注并发性的[架构](http://lib.csdn.net/base/architecture)编写后端，请向llvm-dev发邮件）。
### [Unordered](http://llvm.org/docs/Atomics.html#id9)
Unordered是最低级别的原子性。它本质上保证竞争产生稍微正常的结果，而不是具有未定义的行为。它还保证操作是无锁的，因此它不依赖数据成为一个特殊原子化结构体的部分，或者依赖一个独立的每进程的全局锁。注意对不支持的原子操作，代码生成将失败；如果你需要这样一个操作，使用显式锁定。
相关标准
这目的在于匹配Java内存模型的共享变量。
对前端的注解
这不能用于同步，但对于Java及其他需要保证生成代码永远不会出现未定义行为的“安全”语言是有用的。注意对在通常平台上一个自然宽度的读，这个保证是廉价的，但对更宽的读，像ARM上的一个64位读（译注，原文为store，但按上下文，应为load），是昂贵或不可用的。（Java或其他“安全”语言的前端通常在ARM上将一个64位读分解为两个32位ordered读）。
对优化器的注解
就优化器而言，这禁止了任何将单个读转换为多个读、将单个写转换为多个写、缩窄一个写、或写入一个不能写入的值的转换。不安全优化的一些例子有：将一个赋值缩窄为一个比特位，重新物化（rematerializing）一个读，以及将读写转换为一个memcpy调用。重排unordered操作是安全的，优化器应该利用之，因为unordered的操作在需要它们的语言中是常见的。
对代码生成的注解
这些操作要求是原子性的，在这个意义上，如果你使用unordered读与unordered写，读不能看到从未写过的值。一个普通的读或写指令通常是足够的，但注意一个unordered读或写不能分解为多条指令（或者一条执行多个内存操作的指令，像没有LPAE的ARM上的LDRD，或者LPAEARM上非自然对齐的LDRD）。
### [Monotonic](http://llvm.org/docs/Atomics.html#id10)
Monotonic是可以用在同步原语中的最弱一级的原子性，尽管它不提供任何一般性的同步。它本质上保证，如果你采取所有影响一个特定地址的操作，存在一个一致的次序。
相关标准
这对应于C++11/C11memory_order_relaxed；确切定义参考这些标准。
对前端的注解
如果你正在编写直接使用这的一个前端，小心使用。就同步而言保证非常弱，因此确保这些仅用在你知道是正确的一个模式（pattern）。通常，这将要么用于不保护其他内存的原子操作（像一个原子计数器），或者连同一个fence一起。
对优化器的注解
就优化器而言，这可以被处理作在相关内存位置上的一个读+写（别名分析将利用之）。另外，在Monotonic读周围重排非原子读与Unordered读是合法的。CSE/DSE以及其他几个优化是允许的，但Monotonic操作不太可能以使得这些优化有用的方式来使用。
对代码生成的注解
代码生成本质上与Unordered读写相同。不要求fence。Cmpxchg与atomicrmw要求呈现为单个操作。
### [Acquire](http://llvm.org/docs/Atomics.html#id11)
Acquire提供了获取一把锁以普通读写访问其他内存所需的屏障（barrier）。
相关标准
这对应于C++11/C11的memory_order_acquire。它也用于C++11/C11的memory_order_consume。
对前端的注解
如果你正在编写直接使用这的一个前端，小心使用。Acquire仅在与一个Release操作结对时才提供一个语义保证。
对优化器的注解
不清楚原子性的优化器可以将这处理为一个非异常抛出调用。将一个Acquire读或一个读-修改-写操作前的写移到它之后，以及将一个Acquire操作前的非Acquire读移到它之后是可能的。
对代码生成的注解
带有弱内存序的架构（基本上除了x86与SPARC，现在都是这样的架构）要求某种fence来维护Acquire语义。所要求的正确fence根据架构变化很大，不过对于一个简单的实现，大多数架构提供了一个足够强大的屏障（ARM的dmb，PowerPC的sync等）。将这样一个fence放在等效的Monotonic操作后足以对一个内存操作维护Acquire语义。
### [Release](http://llvm.org/docs/Atomics.html#id12)
Release类似于Acquire，但带有一个释放一把锁所需的屏障。
相关标准
这对应于C++11/C11的memory_order_release。
对前端的注解
如果你正在编写直接使用这的一个前端，小心使用。Release仅在与一个Acquire操作结对时才提供一个语义保证。
对优化器的注解
不清楚原子性的优化器可以将这处理为一个非异常抛出调用。将一个Release写或一个读-修改-写操作后的读移到它之前，以及将一个Release操作后的非Release写移到它之前是可能的。
对代码生成的注解
参考关于Acquire的章节；在相关操作前的一个fence对Release通常足够。注意一个写-写fence不足以实现Release语义；写-写fence通常不暴露给IR，因为它们极难正确使用。
### [AcquireRelease](http://llvm.org/docs/Atomics.html#id13)
AcquireRelease（IR中的acq_rel）同时提供了Acquire与Release屏障（对于同时读写内存的fence与操作）。
相关标准
这对应于C++11/C11的memory_order_acq_rel。
对前端的注解
如果你正在编写直接使用这的一个前端，小心使用。Acquire仅在与一个Release操作结对时才提供一个语义保证，反之亦然。
对优化器的注解
通常，优化器应该将这处理为一个非异常抛出调用；可能的优化通常是不令人感兴趣的。
对代码生成的注解
这个操作具有Acquire与Release的语义；参考关于Acquire与Release的章节。
### [SequentiallyConsistent](http://llvm.org/docs/Atomics.html#id14)
SequentiallyConsistent（IR中的seq_cst）对读提供了Acquire语义，对写提供了Release语义。另外，它保证在所有SequentiallyConsistent操作间存在一个全续（atotal ordering）。
相关标准
这对应于C++11/C11的memory_order_seq_cst，Javavolatile，以及gcc兼容的__sync_*固有函数。
对前端的注解
如果前端正在导出原子操作，对于程序员，相比其他类型的操作，这些容易推理得多，使用它们通常是一个可行的性能权衡。
对优化器的注解
不清楚原子性的优化器可以将这处理为一个非异常抛出调用。对于SequentiallyConsistent的读与写，允许对Acquire读与Release写进行相同的重排，但不能重排SequentiallyConsistent的操作。
对代码生成的注解
SequentiallyConsistent的读最低限度要求与Acquire操作相同的屏障，而SequentiallyConsistent的写要求Release屏障。另外，代码生成器必须严格执行SequentiallyConsistent的写与后跟的SequentiallyConsistent的读之间的次序。这通常通过在读之前或在写之后发布一个完整的fence来完成；不同的架构有自己的偏好。
## 原子化与IR优化
优化器作者用于查询的谓词（Predicate）：
·        isSimple()：一个不是volatile或原子化的读或写。这是，比如memcpyopt，为它可能转换的操作所做的检查。
·        isUnordered()：一个不是volatile并且最多是Unordered的读或写。这是，比如LICM，在提升一个操作前所应该检查的。
·        mayReadFromMemory()/mayWriteToMemory()：现存的谓词，不过注意它们对任何是volatile或至少是Monotonic的操作返回true。
·        isStrongerThan / isAtLeastOrStrongerThan：这些是在定序（ordering）上的谓词。对于知道原子操作的遍它们是有用的，例如跨过单个原子访问，但不跨过一个release-acquire对的DSE执行（这样的一个例子，参考MemoryDependencyAnalysis）。
·        别名分析：注意AA将对Acquire或Release的任何东西，以及由任何Monotonic操作访问的地址，返回ModRef。
为了支持围绕原子操作的优化，确保你使用正确的谓词；如果做到了，应该就一切正常。如果你的遍应该优化某些原子操作（特别是Unordered操作），确保它不会以一个非原子操作替换一个原子读或写。
优化如何与各种原子操作交互的一些例子：
·        Memcpyopt：一个原子操作不能被优化为一个memcpy/memset的部分，包括unordered的读写。它可以将操作拖过某些原子操作。
·        LICM：Unordered读写可以移出一个循环。它仅是将Monotonic操作处理作对一个内存位置的读+写，任何比这严格的处理作一个非异常抛出调用。
·        DSE（死写消除，dead store elimination）：Unordered写可以像普通写那样处理。在某些情形下，Monotonic写也可以DSE掉，但推理是微妙的，且不是特别重要。在某些情形下，DSE跨过一个更强的原子操作工作是可能的，但它相当棘手。DSE将这个推理委托给MemoryDependencyAnalysis（这被其他遍像GVN使用）。
·        折叠一个读：任何来自一个全局常量的原子读可以被常量折叠，因为它不能被观察到。类似的推理允许对原子读写进行sroa（ScalarReplacement Of Aggregates，聚集类型的标量替换）。
## 原子化与Codegen
在SelectionDAG中原子操作以ATOMIC_* 操作码来表示。在对所有原子定序（ordering）使用屏障指令的架构上（像ARM），如果使用了setInsertFencesForAtomic()，合适的fence可以由Codegen遍AtomicExpand发布。
所有原子操作的MachineMemOperand目前被标记为volatile；就IR的volatile观念，这是不正确的，但CodeGen保守地处理任何标记为volatile的东西。在将来某个时候，这会得到修正。
原子操作的一个非常重要的属性是，如果你的后端支持任何一个给定尺寸的内联无锁原子操作，你应该以一个无锁的方式支持该尺寸的**所有**操作。
当目标机器实现原子化cmpxchg或LL/SC指令（像大多数那样）时，这是无足轻重的：所有其他操作可以在这些原语之上实现。不过，在许多旧的CPU上（比如ARMv5，SparcV8，Intel80386），有原子化的读写指令，但没有cmpxchg或LL/SC。尽管使用原生指令实现原子化读是无效的，但使用一个使用mutex的函数的库调用的cmpxchg是有效的，原子读也必须在这样的架构上扩展为一个库调用，因此通过使用同一个mutex，就一个同步的cmpxchg而言，它可以保持原子性。
AtomicExpandPass可以对此有帮助：它将所有超过setMaxAtomicSizeInBitsSupported设置最大值（缺省为0）的任何尺寸的原子操作展开为合适的__atomic_*libcall。
在x86上，所有原子读产生一个MOV。SequentiallyConsistent写产生一个XCHG，其他写产生一个MOV。SequentiallyConsistentfence产生一个MFENCE，其他fence不会产生任何代码。Cmpxchg使用LOCKCMPXCHG指令。atomicrmw xchg使用XCHG，atomicrmw add与atomicrmw sub 使用XADD，所有其他atomicrmw操作产生一个带有LOCKCMPXCHG的循环。依赖于结果的使用者，某些atomicrmw操作可以被转换为像LOCKAND的操作，但这通常不能奏效。
在ARM（v8以前），MIPS以及许多其他RISC架构上，Acquire，Release与SequentiallyConsistent语义对每个这样的操作要求屏障指令。读与写产生普通的指令。cmpxchg与atomicrmw可以使用一个带有在缓存线上获取某种互斥锁的LL/SC形式指令（ARM上的LDREX与STRE等）的循环来代表。
后端使用AtomicExpandPass来降级某些原子构造通常是最简单的。以下是一些它可以做的降级：
·        Cmpxchg：通过重载的shouldExpandAtomicCmpXchgInIR()，emitLoadLinked()，emitStoreConditional()，降级到带有load-linked（ll）/ store-conditional（sc）的循环。
·        大的 loads/stores ：通过重载overriding shouldExpandAtomicStoreInIR()，shouldExpandAtomicLoadInIR()，降级到ll-sc/cmpxchg。
·        强原子访问：通过重载shouldInsertFencesForAtomic()，emitLeadingFence()，emitTrailingFence()，降级为monotonic访问+fences
·        原子rmw：通过重载overriding expandAtomicRMWInIR()，降级到带有cmpxchg或load-linked（ll）/store-conditional（SC）的循环
·        对于不支持的尺寸扩展为__atomic_* libcalls。
所有这些的例子，查阅ARM后端。
## [Libcalls：__atomic_*](http://llvm.org/docs/Atomics.html#id17)
有两种原子库调用由LLVM产生。请注意这两组库函数有点混淆地与clang定义的固有函数共享名字。尽管这样，这些库函数与这些固有函数不是直接关联的：__atomic_*固有函数不是降级到__atomic_*库调用，__sync_*固有函数也不是降级到__sync_*库调用。
第一组库函数被命名为__atomic_*。这组已经被GCC“标准化”，在下面描述。（也可参考[GCC的文档](https://gcc.gnu.org/wiki/Atomic/GCCMM/LIbrary)）。
LLVM的AtomicExpandPass将数据大小超过MaxAtomicSizeInBitsSupported的原子操作转换为对这些函数的调用。
有4个通用函数，可以任意尺寸或对齐的数据调用它们：
void__atomic_load(size_t size, void *ptr, void *ret, int ordering)
void__atomic_store(size_t size, void *ptr, void *val, int ordering)
void__atomic_exchange(size_t size, void *ptr, void *val, void *ret, int ordering)
bool __atomic_compare_exchange(size_t size, void *ptr, void *expected, void *desired, int success_order, int failure_order)
上面函数也有特定的版本，它们仅能与合适尺寸的自然对齐指针一起使用。在下面的署名里，N是1，2，4，8之一，iN是该尺寸的合适的整数类型；如果不存在这样的整数类型，不能使用这个专用函数：
iN__atomic_load_N(iN *ptr, iN val, int ordering)
void__atomic_store_N(iN *ptr, iN val, int ordering)
iN__atomic_exchange_N(iN *ptr, iN val, int ordering)
bool __atomic_compare_exchange_N(iN *ptr, iN *expected, iN desired, int success_order, int failure_order)
最后，存在一些读-修改-写函数，它们仅有指定尺寸的版本可用（其他的尺寸使用一个_atomic_compare_exchange循环）：
iN__atomic_fetch_add_N(iN *ptr, iN val, int ordering)
iN__atomic_fetch_sub_N(iN *ptr, iN val, int ordering)
iN__atomic_fetch_and_N(iN *ptr, iN val, int ordering)
iN__atomic_fetch_or_N(iN *ptr, iN val, int ordering)
iN__atomic_fetch_xor_N(iN *ptr, iN val, int ordering)
iN__atomic_fetch_nand_N(iN *ptr, iN val, int ordering)
这组库函数有一些需要注意的，有趣的实现要求：
·        它们支持所有的尺寸与对齐——包括在任何现存硬件上不能自然实现的那些。因此，它们将肯定对某些尺寸、对齐使用互斥量。
·        结果，它们不能在一个静态链接的编译器支持库中发布，因为它们具有必须在程序载入的所有DSO中共享的状态。它们必须提供为一个所有对象使用的共享库。
·        支持无锁的原子化尺寸的集合必须是任何编译器能发布的大小集合的一个超集。即：如果一个新编译器引入对尺寸N的内联无锁原子操作的支持，__atomic_*函数也必须对尺寸N有一个无锁实现。
·        这个要求使得由旧编译器产生的代码（将调用__atomic_*函数）与新编译器生成的代码（将使用原生原子操作指令）可以交互。
注意，通过使用编译器原子操作内置函数，实现在所支持大小，自然对齐的指针上的操作，否则通过一个通用的互斥量的实现，来编写这些库函数的一个完全目标机器无关的实现是可能的。
## [Libcalls：__sync_*](http://llvm.org/docs/Atomics.html#id18)
某些目标机器或OS/目标机器组合可以支持无锁原子操作，但出于各种原因，发布内联指令是不现实的。
这有两个典型的例子。
有些CPU支持可以在函数调用边界来回切换的多个指令集。例如，MIPS支持MPIS16ISA，它具有比MIPS32ISA更小的指令编码。类似的，ARM具有ThumbISA。在MPIS16以及Thumb的更早版本，原子操作指令是不可编码的。不过，这些指令通过对一个带有更长编码的函数的调用可用。
另外，少数OS/目标机器对提供内核支持的无锁原子操作。ARM/[Linux](http://lib.csdn.net/base/linux)是一个例子：内核提供了一个函数，在更老的CPU上包含一个“魔术般可重启的” 原子操作序列（只要只有一个CPU，它看起来是原子的），在更新的多核模型上包含真实的原子指令。这种功能通常在任何架构上都会提供，如果所有缺少原子化比较-交换支持的CPU是单处理器（非SMP）。现实几乎总是这样的。没有这个属性的仅有的通用架构是SPARC——SPARC8SMP系统是常见的，但它不支持任何种类比较-交换操作。
在这些情形里，LLVM中的Target可以宣称支持一个合适大小的原子操作，然后通过对__sync__*函数的libcall实现这些操作的某些子集。这样的函数在它们的实现里不能使用锁，因为不像由AtomicExpandPass使用的__atomic__*例程，通过目标机器降级，它们可能与原生指令混合-及-匹配（mixed-and-matched）。
另外，这些例程不需要共享，因为它们是无状态的。因此，在一个库里包含多个拷贝没有问题。因此，这些例程通常由静态链接的编译器运行时支持库实现。
LLVM将发布对一个合适__sync_*例程的调用，如果目标机器IselLowering代码将对应的ATOMIC_CMPXCHG，ATOMIC_SWAP，或ATOMIC_LOAD_*操作设置为“展开”，而且如果它已经通过对initSyncLibcalls的一个调用选择这些库函数。
可以由LLVM调用的完整的函数集是（N是1，2，4，8或16）：
iN__sync_val_compare_and_swap_N(iN *ptr, iN expected, iN desired)
iN__sync_lock_test_and_set_N(iN *ptr, iN val)
iN__sync_fetch_and_add_N(iN *ptr, iN val)
iN__sync_fetch_and_sub_N(iN *ptr, iN val)
iN__sync_fetch_and_and_N(iN *ptr, iN val)
iN__sync_fetch_and_or_N(iN *ptr, iN val)
iN__sync_fetch_and_xor_N(iN *ptr, iN val)
iN__sync_fetch_and_nand_N(iN *ptr, iN val)
iN__sync_fetch_and_max_N(iN *ptr, iN val)
iN__sync_fetch_and_umax_N(iN *ptr, iN val)
iN__sync_fetch_and_min_N(iN *ptr, iN val)
iN__sync_fetch_and_umin_N(iN *ptr, iN val)
这个列表不包括任何原子读写函数；所有已知的架构直接支持原子读写（可能通过在一个普通读写的一边发布一个栅栏）。
某种程度上还有独立地降级ATOMIC_FENCE为 __sync_synchronize()的可能性。这发生与否与上面无关，完全由setOperationAction(ISD::ATOMIC_FENCE, ...)控制。
