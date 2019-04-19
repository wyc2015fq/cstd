# Linux 内核数据结构：位图（Bitmap） - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [乔永琪](http://www.jobbole.com/members/qiaofeng) 翻译，[唐尤华](http://www.jobbole.com/members/tangyouhua) 校稿。未经许可，禁止转载！
英文出处：[0xAX](https://github.com/0xAX/linux-insides/blob/master/DataStructures/bitmap.md)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
**本系列：**
- 《[Linux 内核数据结构：Radix 树](http://blog.jobbole.com/87675/)》
- 《[Linux 内核数据结构：双向链表](http://blog.jobbole.com/87687/)》
## 位图和位运算
除了各种链式和树形数据结构，Linux内核还提供了位图接口。位图在Linux内核中大量使用。下面的源代码文件包含这些结构的通用接口：
- [lib/bitmap.c](https://github.com/torvalds/linux/blob/master/lib/bitmap.c)
- [include/linux/bitmap.h](https://github.com/torvalds/linux/blob/master/include/linux/bitmap.h)
除了这两个文件，还有一个特定的架构头文件，对特定架构的位运算进行优化。对于[x86_64](https://en.wikipedia.org/wiki/X86-64)架构，使用下面头文件：
- [arch/x86/include/asm/bitops.h](https://github.com/torvalds/linux/blob/master/arch/x86/include/asm/bitops.h)
正如我前面提到的，位图在Linux内核中大量使用。比如，位图可以用来存储系统在线/离线处理器，来支持[CPU热插拔](https://www.kernel.org/doc/Documentation/cpu-hotplug.txt)；再比如，位图在Linux内核等初始化过程中存储已分配的[中断请求](https://en.wikipedia.org/wiki/Interrupt_request_%28PC_architecture%29)。
因此，本文重点分析位图在Linux内核中的具体实现。
# 位图声明
位图接口使用前，应当知晓Linux内核是如何声明位图的。一种简单的位图声明方式，即unsigned long数组。比如：

C
```
unsigned long my_bitmap[8]
```
第二种方式，采用DECLARE_BITMAP宏，此宏位于头文件[include/linux/types.h](https://github.com/torvalds/linux/blob/master/include/linux/types.h)中：

C
```
#define DECLARE_BITMAP(name,bits) 
    unsigned long name[BITS_TO_LONGS(bits)]
```
DECLARE_BITMAP宏有两个参数：
- name – 位图名字；
- bits – 位图中比特总数目
并且扩展元素大小为BITS_TO_LONGS(bits)、类型unsigned long的数组，而BITS_TO_LONGS宏将位转换为long类型，或者说计算出bits中包含多少byte元素：

C
```
#define BITS_PER_BYTE           8
#define DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))
#define BITS_TO_LONGS(nr)       DIV_ROUND_UP(nr, BITS_PER_BYTE * sizeof(long))
```
例如：DECLARE_BITMAP(my_bitmap, 64)结果为：

C
```
>>> (((64) + (64) - 1) / (64))
1
```
和：

C
```
unsigned long my_bitmap[1];
```
位图声明后，我们就可以使用它了。
# 特定架构的位运算
我们已经查看了操作位图[接口](https://en.wikipedia.org/wiki/Application_programming_interface)的两个源码文件和一个头文件。位图最重要最广泛的应用接口是特定架构，它位于头文件[arch/x86/include/asm/bitops.h](https://github.com/torvalds/linux/blob/master/arch/x86/include/asm/bitops.h)中
首先，我们来看两个重要的函数：
- `set_bit`;
- `clear_bit`.
我认为没有必要介绍这些函数是做什么的，通过函数名就可以知晓。我们来看函数的实现。进入头文件[arch/x86/include/asm/bitops.h](http://arch/x86/include/asm/bitops.h)，你会注意到每个函数分两种类型：[原子类型](https://en.wikipedia.org/wiki/Linearizability)和非原子类型。在深入这些函数实现前，我们需要先了解一些原子性运算。
一言以蔽之，原子性操作保障，位于同一数据上的两个甚至多个运算，不能并发执行。x86架构提供一组原子性指令，如指令[xchg](http://x86.renejeschke.de/html/file_module_x86_id_328.html)、指令[cmpxchg](http://x86.renejeschke.de/html/file_module_x86_id_41.html)。除了原子性指令，一些非原子性指令可借助指令[lock](http://x86.renejeschke.de/html/file_module_x86_id_159.html)进行原子性运算。目前我们了解这些原子性运算就足够了，接下来可以开始考虑set_bit和clear_bit函数。
先从非原子性类型的函数开始，非原子性set_bit和clear_bit函数名始于双下划线。正如你所了解的，所有的函数定义在头文件arch/x86/include/asm/bitops.h中，第一个函数__set_bit：

C
```
static inline void __set_bit(long nr, volatile unsigned long *addr)
{
    asm volatile("bts %1,%0" : ADDR : "Ir" (nr) : "memory");
}
```
它拥有两个参数：
- nr –  位图中比特数目
- addr –  位图中某个比特需要设值的地址
注意参数addr定义为volatile，告诉编译器此值或许被某个地址改变。而__set_bit容易实现。正如你所见，恰好它包含一行[内联汇编](https://en.wikipedia.org/wiki/Inline_assembler)代码。本例中，使用指令[bts](http://x86.renejeschke.de/html/file_module_x86_id_25.html)选择位图中的某个比特值作为首个操作数，将已选择比特值存入寄存器[CF](https://en.wikipedia.org/wiki/FLAGS_register)标签中，并设置此比特。
此处可以看到nr的用法，那addr呢？或许你已猜到其中的奥秘就在ADDR中。而ADDR是定义在头文件中的宏，扩展字符串，在该地址前面加入+m约束：

C
```
#define ADDR                BITOP_ADDR(addr)
#define BITOP_ADDR(x) "+m" (*(volatile long *) (x))
```
除了+m，我们可以看到__set_bit函数中其它约束。让我们查看这些约束，试着理解其中的含义：
- `+m` – 表示内存操作数，+表示此操作数为输入和输出操作数；
- I – 表示整数常数；
- r -表示寄存器操作数
除了这些约束，还看到关键字memory，它会告知编译器此代码会更改内存中的值。接下来，我们来看同样功能，原子类型函数。它看起来要比非原子类型函数复杂得多：

C
```
static __always_inline void
set_bit(long nr, volatile unsigned long *addr)
{
    if (IS_IMMEDIATE(nr)) {
        asm volatile(LOCK_PREFIX "orb %1,%0"
            : CONST_MASK_ADDR(nr, addr)
            : "iq" ((u8)CONST_MASK(nr))
            : "memory");
    } else {
        asm volatile(LOCK_PREFIX "bts %1,%0"
            : BITOP_ADDR(addr) : "Ir" (nr) : "memory");
    }
}
```
注意它与函数__set_bit含有相同的参数列表，不同的是函数被标记有属性__always_inline。__always_inline是定义在[include/linux/compiler-gcc.h](https://github.com/torvalds/linux/blob/master/include/linux/compiler-gcc.h)中的宏，只是扩展了always_inline属性：

C
```
#define __always_inline inline __attribute__((always_inline))
```
这意味着函数会被内联以减少Linux内核镜像的大小。接着，我们试着去理解函数set_bit实现。函数set_bit伊始，便对比特数目进行检查。IS_IMMEDIATE是定义在相同[头文件](https://github.com/torvalds/linux/blob/master/arch/x86/include/asm/bitops.h)中的宏，用于扩展内置函数[gcc](https://en.wikipedia.org/wiki/GNU_Compiler_Collection)：

C
```
#define IS_IMMEDIATE(nr)        (__builtin_constant_p(nr))
```
内置函数__builtin_constant_p返回1的条件是此参数在编译期为常数；否则返回0。无需使用指令bts设置比特值，因为编译期比特数目为一常量。仅对已知字节地址进行按位或运算，并对比特数目bits进行掩码，使其高位为1，其它为0. 而比特数目在编译期若非常量，函数__set_bit中运算亦相同。宏CONST_MASK_ADDR：

C
```
#define CONST_MASK_ADDR(nr, addr)   BITOP_ADDR((void *)(addr) + ((nr)>>3))
```
采用偏移量扩展某个地址为包含已知比特的字节。比如地址0x1000，以及比特数目0x9。0x9等于一个字节，加一个比特，地址为addr+1：

C
```
>>> hex(0x1000 + (0x9 >> 3))
'0x1001'
```
宏CONST_MASK表示看做字节的某已知比特数目，高位为1，其它比特为0：

C
```
#define CONST_MASK(nr)          (1 << ((nr) & 7))
```


C
```
>>> bin(1 << (0x9 & 7))
'0b10
```
最后，我们使用按位或运算。假设address为0x4097，需要设置ox9比特：

C
```
>>> bin(0x4097)
'0b100000010010111'
>>> bin((0x4097 >> 0x9) | (1 << (0x9 & 7)))
'0b100010'
```
第九个比特将被设置
注意所有的操作均标记有LOCK_PREFIX，即扩展为指令[lock](http://x86.renejeschke.de/html/file_module_x86_id_159.html)，确保运算以原子方式执行。
如我们所知，除了set_bit和__set_bit运算，Linux内核还提供了两个逆向函数以原子或非原子方式清理比特，clear_bit和__clear_bit。这个两个函数均定义在相同的[头文件](https://github.com/torvalds/linux/blob/master/arch/x86/include/asm/bitops.h)中，并拥有相同的参数列表。当然不仅是参数相似，函数本身和set_bit以及 __set_bit都很相似。我们先来看非原子性函数__clear_bit

C
```
static inline void __clear_bit(long nr, volatile unsigned long *addr)
{
    asm volatile("btr %1,%0" : ADDR : "Ir" (nr));
}
```
正如我们所看到的，它们拥有相同参数列表，以及相似的内联汇编函数块。不同的是__clear_bit采用指令[btr](http://x86.renejeschke.de/html/file_module_x86_id_24.html)代替指令bts。从函数名我们可以看出，函数用来清除某个地址的某个比特值。指令btr与指令bts类似，选择某个比特值作为首个操作数，将其值存入寄存器CF标签中，并清除位图中的这个比特值，且将位图作为指令的第二个操作数。
__clear_bit的原子类型为clear_bit:

C
```
static __always_inline void
clear_bit(long nr, volatile unsigned long *addr)
{
    if (IS_IMMEDIATE(nr)) {
        asm volatile(LOCK_PREFIX "andb %1,%0"
            : CONST_MASK_ADDR(nr, addr)
            : "iq" ((u8)~CONST_MASK(nr)));
    } else {
        asm volatile(LOCK_PREFIX "btr %1,%0"
            : BITOP_ADDR(addr)
            : "Ir" (nr));
    }
}
```
正如我们所看到的，它和set_bit相似，仅有两处不同。第一个不同，使用指令btr进行比特清理，而set_bit使用指令bts比特存储。第二个不同，使用消除掩码以及指令and清理某个byte中的bit值，而set_bit使用指令or。
到目前为止，我们可以给任何位图设值、清除或位掩码运算。
位图最常用的运算为Linux内核中位图的设值以及比特值的清除。除了这些运算外，为位图添加额外的运算也是有必要的。Linux内核中，另一个广泛的运算是判定位图是否已设置比特值。可借助test_bit宏进行判定，此宏定义在头文件[arch/x86/include/asm/bitops.h](https://github.com/torvalds/linux/blob/master/arch/x86/include/asm/bitops.h)中，并依据比特数目，选择调用constant_test_bit 或 variable_test_bit：

C
```
#define test_bit(nr, addr)          
    (__builtin_constant_p((nr))                 
     ? constant_test_bit((nr), (addr))          
     : variable_test_bit((nr), (addr)))
```
若nr在编译期为常数，调用test_bit中函数constant_test_bit，否则调用函数variable_test_bit。我们来看这些函数实现，先从函数variable_test_bit开始：

C
```
static inline int variable_test_bit(long nr, volatile const unsigned long *addr)
{
    int oldbit;
    asm volatile("bt %2,%1nt"
             "sbb %0,%0"
             : "=r" (oldbit)
             : "m" (*(unsigned long *)addr), "Ir" (nr));
    return oldbit;
}
```
函数variable_test_bit拥有set_bit等函数相似的参数列表。同样，我们看到内联汇编代码，执行指令[bt](http://x86.renejeschke.de/html/file_module_x86_id_22.html)、[sbb](http://x86.renejeschke.de/html/file_module_x86_id_286.html)。指令bt或bit test，从位图中选择某个比特值作为首个操作数，而位图作为第二个操作数，并将选定的比特值存入寄存器[CF](https://en.wikipedia.org/wiki/FLAGS_register)标签中。而指令sbb则会将首个操作数从第二个操作数中移除，并移除CF标签值。将位图某个比特值写入CF标签寄存器，执行指令sbb，计算CF为00000000 ，最后将结果写入oldbit。
函数constant_test_bit与set_bit相似：

C
```
static __always_inline int constant_test_bit(long nr, const volatile unsigned long *addr)
{
    return ((1UL << (nr & (BITS_PER_LONG-1))) &
        (addr[nr >> _BITOPS_LONG_SHIFT])) != 0;
}
```
它能够产生一个字节，其高位时1，其它比特为0，对这个包含比特数目的字节做按位与运算。
接下来比较广泛的位图运算是，位图中的比特值的改变运算。为此，Linux内核提供两个帮助函数：
- `__change_bit`;
- `change_bit`.
或许你已能猜到，与set_bit和 __set_bit相似，存在两个类型，原子类型和非原子类型。我们先来看函数__change_bit的实现：

C
```
static inline void __change_bit(long nr, volatile unsigned long *addr)
{
    asm volatile("btc %1,%0" : ADDR : "Ir" (nr));
}
```
很容易，难道不是吗？__change_bit与__set_bit拥有相似的实现，不同的是，前者采用的指令[btc](http://x86.renejeschke.de/html/file_module_x86_id_23.html)而非bts。指令选择位图中的某个比特值，然后将此值放入CF中，然后使用补位运算改变其值。若比特值为1则改变后的值为0，反之亦然：

C
```
>>> int(not 1)
0
>>> int(not 0)
1
```
函数__change_bit的原子版本为函数change_bit：

C
```
static inline void change_bit(long nr, volatile unsigned long *addr)
{
    if (IS_IMMEDIATE(nr)) {
        asm volatile(LOCK_PREFIX "xorb %1,%0"
            : CONST_MASK_ADDR(nr, addr)
            : "iq" ((u8)CONST_MASK(nr)));
    } else {
        asm volatile(LOCK_PREFIX "btc %1,%0"
            : BITOP_ADDR(addr)
            : "Ir" (nr));
    }
}
```
与函数set_bit相似，但有两处不同。第一处不同的是xor运算而非or；第二处不同的是btc而非bts。
至此，我们了解了最重要的位图架构相关运算，接下来我们来查看通用位图接口。
# 通用比特运算
除了来自头文件[arch/x86/include/asm/bitops.h](https://github.com/torvalds/linux/blob/master/arch/x86/include/asm/bitops.h)的特定架构接口，Linux内核还提供了位图的通用接口。从前文就已了解，头文件[include/linux/bitmap.h](https://github.com/torvalds/linux/blob/master/include/linux/bitmap.h)，以及* l[ib/bitmap.c](https://github.com/torvalds/linux/blob/master/lib/bitmap.c)源码文件。不过在查看源码文件之前，我们先来看头文件[include/linux/bitops.h，](https://github.com/torvalds/linux/blob/master/include/linux/bitops.h)它提供了一组有用的宏。我们来看其中的一些：
先看下面四个宏：
- `for_each_set_bit`
- `for_each_set_bit_from`
- `for_each_clear_bit`
- `for_each_clear_bit_from`
这些宏提供了位图迭代器，首个宏迭代集合set，第二个宏也是，不过从集合指定的比特处开始。后面两个宏也是如此，不同的是迭代清空的比特。我们先来看宏for_each_set_bit的实现：

C
```
#define for_each_set_bit(bit, addr, size) 
    for ((bit) = find_first_bit((addr), (size));        
         (bit) < (size);                    
         (bit) = find_next_bit((addr), (size), (bit) + 1))
```
正如大家所看到的，此宏拥有三个参数，以及循环从set集合第一个比特开始，到最后一个比特结束，迭代比特数目小于最后一个size，循环最后返回函数find_first_bit。
除了这四个宏，[arch/x86/include/asm/bitops.h](http://arch/x86/include/asm/bitops.h)还提供了64位或32位等值的迭代。
同样，[头文件](https://github.com/torvalds/linux/blob/master/include/linux/bitmap.h)也提供了位图的其它接口。比如下面的这两个函数：
- `bitmap_zero`;
- `bitmap_fill`.
清除位图，并为其填值1 。我们来看函数bitmap_zero实现：

C
```
static inline void bitmap_zero(unsigned long *dst, unsigned int nbits)
{
    if (small_const_nbits(nbits))
        *dst = 0UL;
    else {
        unsigned int len = BITS_TO_LONGS(nbits) * sizeof(unsigned long);
        memset(dst, 0, len);
    }
}
```
同样，先检查nbits，函数small_const_nbits定义在相同[头文件](https://github.com/torvalds/linux/blob/master/include/linux/bitmap.h)中的宏，具体如下：

C
```
#define small_const_nbits(nbits) 
    (__builtin_constant_p(nbits) && (nbits) <= BITS_PER_LONG)
```
正如大家所见，检查nbits在编译期是否为一常量，nbits值是否超过BITS_PER_LONG或64 。倘若bits的数目没有超出long类型的总量，将其设置为0 。否则，需计算多少个long类型值填入位图中，当然我们借助[memset](http://man7.org/linux/man-pages/man3/memset.3.html)填入。
函数bitmap_fill的实现与bitmap_zero相似，不同的是位图的填值为0xff或0b11111111：

C
```
static inline void bitmap_fill(unsigned long *dst, unsigned int nbits)
{
    unsigned int nlongs = BITS_TO_LONGS(nbits);
    if (!small_const_nbits(nbits)) {
        unsigned int len = (nlongs - 1) * sizeof(unsigned long);
        memset(dst, 0xff,  len);
    }
    dst[nlongs - 1] = BITMAP_LAST_WORD_MASK(nbits);
}
```
除了函数bitmap_fill和bitmap_zero，头文件[include/linux/bitmap.h](https://github.com/torvalds/linux/blob/master/include/linux/bitmap.h)还提供了函数bitmap_copy，它与bitmap_zero相似，不一样的是使用[memcpy](http://man7.org/linux/man-pages/man3/memcpy.3.html)而非[memset](http://man7.org/linux/man-pages/man3/memset.3.html)。与此同时，也提供了诸如bitmap_and、bitmap_or, bitamp_xor等函数进行按位运算。考虑到这些函数实现容易理解，在此我们就不做说明；对这些函数感兴趣的读者朋友们，请打开头文件[include/linux/bitmap.h](https://github.com/torvalds/linux/blob/master/include/linux/bitmap.h)进行研究。
就写到这里。
# 链接
- [bitmap](https://en.wikipedia.org/wiki/Bit_array)
- [linked data structures](https://en.wikipedia.org/wiki/Linked_data_structure)
- [tree data structures](https://en.wikipedia.org/wiki/Tree_%28data_structure%29)
- [hot-plug](https://www.kernel.org/doc/Documentation/cpu-hotplug.txt)
- [cpumasks](https://0xax.gitbooks.io/linux-insides/content/Concepts/cpumask.html)
- [IRQs](https://en.wikipedia.org/wiki/Interrupt_request_%28PC_architecture%29)
- [API](https://en.wikipedia.org/wiki/Application_programming_interface)
- [atomic operations](https://en.wikipedia.org/wiki/Linearizability)
- [xchg instruction](http://x86.renejeschke.de/html/file_module_x86_id_328.html)
- [cmpxchg instruction](http://x86.renejeschke.de/html/file_module_x86_id_41.html)
- [lock instruction](http://x86.renejeschke.de/html/file_module_x86_id_159.html)
- [bts instruction](http://x86.renejeschke.de/html/file_module_x86_id_25.html)
- [btr instruction](http://x86.renejeschke.de/html/file_module_x86_id_24.html)
- [bt instruction](http://x86.renejeschke.de/html/file_module_x86_id_22.html)
- [sbb instruction](http://x86.renejeschke.de/html/file_module_x86_id_286.html)
- [btc instruction](http://x86.renejeschke.de/html/file_module_x86_id_23.html)
- [man memcpy](http://man7.org/linux/man-pages/man3/memcpy.3.html)
- [man memset](http://man7.org/linux/man-pages/man3/memset.3.html)
- [CF](https://en.wikipedia.org/wiki/FLAGS_register)
- [inline assembler](https://en.wikipedia.org/wiki/Inline_assembler)
- [gcc](https://en.wikipedia.org/wiki/GNU_Compiler_Collection)
> 
**打赏支持我翻译更多好文章，谢谢！**
[打赏译者](#rewardbox)
#### 打赏支持我翻译更多好文章，谢谢！
任选一种支付方式
![](http://jbcdn2.b0.upaiyun.com/2016/08/0e7f3ed833bce2ad9a7586304992d287.png)![](http://jbcdn2.b0.upaiyun.com/2016/08/a64465daeaf26033e2afcd40659beab2.jpg)
