# C进阶指南（3）：显式内联、矢量扩展、C的逸闻轶事 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [醉酒屠夫](http://www.jobbole.com/members/topoisonooo) 翻译。未经许可，禁止转载！
英文出处：[Peter Fačka](http://pfacka.binaryparadise.com/articles/guide-to-advanced-programming-in-C.html)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
前两篇：
- 《[C进阶指南（1）：整型溢出和类型提升、内存申请和管理](http://blog.jobbole.com/72830/)》
- 《[C进阶指南（2）：数组和指针、打桩](http://blog.jobbole.com/73094/)》
## 五、显式内联
（想让）函数代码被直接集成到调用函数中，而非产生独立的函数目标和单个调用，可显式地使用 inline 限定符来指示编译器这么做。根据 [section 6.7.4 of C standard](http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf)**inline **限定符仅建议编译器使得”调用要尽可能快”，并且“此建议是否有效由具体实现定义”
要用内联函数优点的最简单方法是把函数定义为 **static** ，然后将定义放入头文件。

C
```
/* middle.h */
static inline int middle(int a, int b){
    return (b-a)/2;
}
```
独立的函数对象仍然可能被导出，但在翻译单元的外部它是不可见的。这种头文件被包含在多个翻译单元中，编译器可能为每个单元发射函数的多份拷贝。因此，有可能两个变量指向相同的函数名，指针的值可能不相等。
另一种方法是，既提供外部可连接的版本，也提供内联版本，两个版本功能相同，让编译器决定使用哪个。这实际上是内嵌限定符的定义：
> 
*If all of the file scope declarations for a function in a translation unit include the inline function specifier without extern, then the definition in that translation unit is an inline definition. An inline definition does not provide an external definition for the function, and does not forbid an external definition in another translation unit. An inline definition provides an alternative to an external definition, which a translator may use to implement any call to the function in the same translation unit. It is unspecified whether a call to the function uses the inline definition or the external definition.*
在一个翻译单元中，若某个函数在所有的文件范围内都包含不带extern的内联函数限定符，则此翻译单元中此函数定义是内联定义。内联定义不为函数提供外部的定义，也不禁止其他翻译单元的外部定义。内联定义为外部定义提供一个可选项，在同一翻译单元内翻译器可用它实现对函数的任意调用。调用函数时，使用内联定义或外联定义是不确定的。
（译者注：即gcc中的 extern inline，优先使用内联版本，允许外部版本的存在）
对于函数的两个版本，我们可以把下面的定义放在头文件中：

C
```
/* middle.h */
inline int middle(int a, int b){
    return (b-a)/2;
}
```
然后在具体的源文件中，用extern限定符发射翻译单元中外部可链接的版本：

C
```
#include "middle.h"
extern int middle(int a, int b);
```
GCC编译器的实现不同于上述译码方式。若函数由 **inline** 声明，GCC总是发射外部可链接的目标代码，并且程序中只存在一个这样的定义。若函数被声明为export inline的，GCC将永不为此函数发射外部可链接的目标代码。自GCC 4.3版本起，可使用-STD= c99的选项使能为内联定义使能C99规则。若C99的规则被启用，则定义GNUC_STDC_INLINE。之前描述的 **static** 使用方法不受GCC对内联函数解释的影响。如果你需要同时使用内联和外部可链接功能的函数，可考虑以下解决方案：

C
```
/* global.h */
#ifndef INLINE
# if __GNUC__ && !__GNUC_STDC_INLINE__
#  define INLINE extern inline
# else
#  define INLINE inline
# endif
#endif
```
头文件中有函数定义：

C
```
/* middle.h  */
#include "global.h"
INLINE int middle(int a, int b) {
  return (b-a)/2;
}
```
在某个具体实现的源文件中：

C
```
#define INLINE
#include "middle.h
```
若要对函数强制执行内联，GCC和Clang编译器都可用 **always_inline** 属性达成此目的。下面的例子中，独立的函数对象从未被发射。

C
```
/* cdefs.h */
# define __always_inline   inline __attribute__((always_inline))
/* middle.h */
#include <cdefs.h>
static __always_inline int middle(int a, int b) {
  return (b-a)/2;
}
```
一旦编译器内联失败，编译将因错误而终止。例如  [Linux kernel](https://www.kernel.org/doc/local/inline.html) 就使用这种方法。可在 cdefs.h 中上述代码中使用的 **__always_inline** 。
## 六、矢量扩展
许多微处理器（特别是x86架构的）提供单指令多数据（SIMD）指令集来使能矢量操作。例如下面的代码：

C
```
#include <stdint.h>
#include <string.h>
#define SIZE 8
int16_t a[SIZE], b[SIZE];
void addtwo(){
    int16_t i = 0;
    while (i < SIZE) {
        a[i] = b[i] + 2;
        i++;
    }
}
int main(){
    addtwo();
    return a[0];
}
```
**addtwo** 中的循环迭代 8 次，每次往数组 b 上加 2，数组 b 每个元素是 16 位的有符号整型。函数 **addtwo** 将被编译成下面的汇编代码：

Shell
```
$ gcc -O2 auto.c -S -o auto_no.asm
```


C
```
addtwo:
.LFB22:
        .cfi_startproc
        movl    $0, %eax
.L2:
        movzwl  b(%rax), %edx
        addl    $2, %edx
        movw    %dx, a(%rax)
        addq    $2, %rax
        cmpq    $16, %rax
        jne     .L2
        rep
        ret
        .cfi_endproc
```
起初，0 写入到 **eax** 寄存器。标签 **L2** 标着循环的开始。b 的首个元素由 **movzwl** 指令被装入的32位寄存器 **edx** 前16位。 edx寄存器的其余部分填 0。然后 **addl** 指令往 **edx** 寄存器中 **a** 的第一个元素的值加 2 并将结果存在 **dx** 寄存器中。累加结果从 dx（edx 寄存器的低16位）复制到** a** 的第一个元素。最后，显然存放了步长为 2 （占2个字节 – 16位）的数组的 **rax** 寄存器与数组的总大小（以字节为单位）进行比较。如果 **rax** 不等于16，执行跳到 **L2** ，否则会继续执行，函数返回。
SSE2 指令集提供了能够一次性给 8 个 16 位整型做加法的指令 **paddw**。实际上，最现代化的编译器都能够自动使用如 **paddw** 之类的矢量指令优化代码。Clang 默认启用自动向量化。 GCC的编译器中可用 *-ftree-vectorize* 或 -O3 开关启用它。这样一来，向量指令优化后的 **addtwo** 函数汇编代码将会大有不同：

Shell
```
$ gcc -O2 -msse -msse2 -ftree-vectorize -ftree-vectorizer-verbose=5 auto.c -S -o auto.asm
```


C
```
addtwo:
.LFB22:
        .cfi_startproc
        movdqa  .LC0(%rip), %xmm0
        paddw   b(%rip), %xmm0
        movdqa  %xmm0, a(%rip)
        ret
        .cfi_endproc
;...
.LC0:
        .value  2
        .value  2
        .value  2
        .value  2
        .value  2
        .value  2
        .value  2
        .value  2
```
最显着的区别在于循环处理消失了。首先，8 个 16 位值为 2 整数被标记为 LC0，由 **movdqa** 加载到 **xmm0** 寄存器。然后paddw 把 b 的每个 16 位的元素分别加到 xmm0 中的多个数值 2上。结果写回到 a，函数可以返回。指令 **movqda** 只能用在由16个字节对齐的内存对象上。这表明编译器能够对齐两个数组的内存地址以提高效率。
数组的大小不必一定只是 8 个元素，但它必须以 16 字节对齐（需要的话，填充），因此也可以用 128 位向量。用内联函数也可能是一个好主意，特别是当数组作为参数传递的时候。因为数组被转换为指针，指针地址需要16字节对齐。如果函数是内联的，编译器也许能减少额外的对齐开销。

C
```
#include <stdint.h>
void __always_inline addtwo(int16_t* a, int16_t *b, int16_t size){
    int16_t i;
    for (i = 0; i < size; i++) {
        a[i] = b[i] + 2;
    }
}
int main(){
    const int16_t size = 1024;
    int16_t a[size], b[size];
    addtwo(a, b, size);
    return a[0];
}
```
循环迭代 1024 次，每次把两个长度为 16 比特的有符号整型相加。使用矢量操作的话，上例中的循环总数可减少到 128。但这也可能自动完成，在GCC环境中，可用 **vector_size** 定义矢量数据类型，用这些数据和属性显式指导编译器使用矢量扩展操作。此处列举出 emmintrin.h 定义的采用 SSE 指令集的多种矢量数据类型。

C
```
/* SSE2 */
typedef double __v2df __attribute__ ((__vector_size__ (16)));
typedef long long __v2di __attribute__ ((__vector_size__ (16)));
typedef int __v4si __attribute__ ((__vector_size__ (16)));
typedef short __v8hi __attribute__ ((__vector_size__ (16)));
typedef char __v16qi __attribute__ ((__vector_size__ (16)));
```
这是用 __v8hi 类型优化之前的示例代码后的样子：

C
```
#include <stdint.h>
#include <string.h>
#include <emmintrin.h>
static void __always_inline _addtwo(__v8hi *a, __v8hi *b, const int16_t sz){
    __v8hi c = {2,2,2,2,2,2,2,2};
    int16_t i;
    for (i = 0; i < sz; i++) {
        a[i] = b[i] + c;
    }
}
static void __always_inline addtwo(int16_t *a, int16_t *b, const int16_t sz){
    _addtwo((__v8hi *) a, (__v8hi *) b, sz/8);
}
int main(){
    const int16_t size = 1024;
    int16_t a[size], b[size];
    /* ... */
    addtwo(a, b, size);
    return a[0];
}
```
关键是把数据转到合适的类型（此例中为 __v8hi），然后由此调整其他的代码。优化的效果主要看操作类型和处理数据量的大小，可能不同情况的结果差异很大。下表是上例中 addtwo 函数被循环调用 1 亿次的执行时间：
|Compiler|Time|
|----|----|
|gcc 4.5.4 O2|1m 5.3s|
|gcc 4.5.4 O2 auto vectorized|12.7s|
|gcc 4.5.4 O2 manual|8.9s|
|gcc 4.7.3 O2 auto vectorized|25.s|
|gcc 4.7.3 O2 manual|8.9s|
|clang 3.3 O3 auto vectorized|8.1s|
|clang 3.3 O3 manual|9.5s|
Clang 编译器自动矢量化得更快，可能是因为用以测试的外部循环被优化的更好。慢一点的 GCC 4.7.3在内存对齐（见下文）方面效率稍低。

C
```
int32_t i;
for(i=0; i < 100000000; i++){
    addtwo(a, b, size);
}
```
### 6.1 使用内建函数（ Intrinsic Function）
GCC 和 Clang 编译器也提供了内建函数，用来显式地调用汇编指令。
确切的内建函数跟编译器联系很大。x86 平台下，GCC 和 Clang 编译器都提供了带有定义的头文件，通过 x86intrin.h 匹配 Intel 编译器的内建函数（即 GCC 和 Clang 用 Intel 提供的头文件，调用 Intel 的内建函数。译者注）。下表是含特殊指令集的头文件：
- MMX: *mmintrin.h*
- SSE: *xmmintrin.h*
- SSE2: *emmintrin.h*
- SSE3: *mm3dnow.h*
- 3dnow: *tmmintrin.h*
- AVX: *immintrin.h*
使用内建函数后，前面的例子可以改为：

C
```
#include <stdint.h>
#include <string.h>
#include <emmintrin.h>
static void __always_inline addtwo(int16_t *a, int16_t *b, int16_t size){
    int16_t i;
    __m128i c = _mm_set1_epi16(2);
    for (i = 0; i < size; i+=8) {
        __m128i bb = _mm_loadu_si128(b+i);  // movqdu b+i -> xmm0
        __m128i r = _mm_add_epi16(bb, c);   // paddw c + xmm0 -> xmm0
        _mm_storeu_si128(a+i, r);           // movqdu xmm0 -> a+i
    }
}
int main(){
    const int16_t size = 1024;
    int16_t a[size], b[size];
    /* ... */
    addtwo(a, b, size);
    return a[0];
}
```
当编译器产生次优的代码，或因代码中的 if 条件矢量类型不可能表达需要的操作时时，可能需要这种编写代码的方法。
### 6.2 内存对齐
注意到上个例子用了与 **movqdu** 而非 **movqda **（上面的例子里仅用 SIMD 产生的汇编指令使用的是 movqda。译者注）同义的 **_mm_loadu_si128**。这因为不确定 **a** 或 **b** 是否已按 16 字节对齐。使用的指令是期望内存对象对齐的，但使用的内存对象是未对齐的，这样肯定会导致运行错误或数据毁坏。为了让内存对象对齐，可在定义时用 **aligned** 属性指导编译器对齐内存对象。某些情况下，可考虑把关键数据按 64 字节对齐，因为 x86 L1 缓存也是这个大小，这样能提高缓存使用率。

C
```
#include <stdint.h>
#include <string.h>
#include <emmintrin.h>
static void __always_inline addtwo(int16_t *a, int16_t *b, int16_t size){
    int16_t i;
    __m128i c = _mm_set1_epi16(2) __attribute__((aligned(16)));
    for (i = 0; i < size; i+=8) {
        __m128i bb = _mm_load_si128(b+i);  // movqda b+i -> xmm0
        __m128i r = _mm_add_epi16(bb, c);   // paddw c + xmm0 -> xmm0
        _mm_store_si128(a+i, r);           // movqda xmm0 -> a+i
    }
}
int main(){
    const int16_t size = 1024;
    int16_t a[size], b[size] __attribute__((aligned(16)));
    /* ... */
    addtwo(a, b, size);
    return a[0];
}
```
考虑到程序运行速度，使用自动变量好过静态或全局变量，情况允许的话还应避免动态内存分配。当动态内存分配无法避免时，Posix 标准 和 Windows 分别提供了 **posix_memalign** 和 **_aligned_malloc** 函数返回对齐的内存。
高效使用矢量扩展喊代码优化需要深入理解目标架构工作原理和能加速代码运行的汇编指令。这两个主题相关的信息源有  [Agner`s CPU blog](http://www.agner.org/optimize/blog/) 和它的装订版 [Optimization manuals](http://www.agner.org/optimize/)。
## 七、逸闻轶事
本文最后一节讨论 C 编程语言里一些有趣的地方：

C
```
array[i] == i[array];
```
因为下标操作符等价于*(array + i)，因此 array 和 i 是可交换的，二者等价。

C
```
$ gcc -dM -E - < /dev/null | grep -e linux -e unix
#define unix 1
#define linux 1
```
默认情况下，GCC 把 **linux** 和 **unix** 都定义为 1，所以一旦把其中一个用作函数名，代码就会编不过。

C
```
int x = 'FOO!';
short y = 'BO';
```
没错，字符表达式可扩展到任意整型大小。

C
```
x = i+++k;
x = i++ +k;
```
后缀自增符在加号之前被词法分析扫描到。
（即示例中两句等价，不同于 x = i +  (++k) 。译者注）

C
```
x = i+++++k; //error
x = i++ ++ +k; //error
y = i++ + ++k; //ok
```
词法分析查找可被处理的最长的非空格字符序列（C标准6.4节）。第一行将被解析成第二行的样子，它们俩都会产生关于缺少左值的错误，缺失的左值本应该被第二个自增符处理。
## 致谢
若有需要增改之处，欢迎留言到 [原文链接](http://pfacka.binaryparadise.com/articles/guide-to-advanced-programming-in-C.html) 。
## 参考文献
- [Expert C Programming, Book by Peter van Linden](http://www.amazon.com/Expert-Programming-Peter-van-Linden/dp/0131774298)
- [C99 Standard](http://www.open-std.org/jtc1/sc22/WG14/www/docs/n1256.pdf)
- [C11 Standard](http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf)
- [POSIX.1-2008 Standard](http://pubs.opengroup.org/onlinepubs/9699919799/)
- [Inside memory management](http://www.ibm.com/developerworks/linux/library/l-memory/)
- [C Reference Counting and You](http://stoneship.org/essays/c-reference-counting-and-you/)
- [Stack Overflow Problems](http://www.cs.nyu.edu/exact/core/doc/stackOverflow.txt)
- [GNU Obstack](http://gcc.gnu.org/onlinedocs/libiberty/Obstacks.html)
- [Talloc Documantation](http://talloc.samba.org/talloc/doc/html/index.html)
- [Ravenbrook Memory Pool System](http://www.ravenbrook.com/project/mps/)
- [libbsd](http://libbsd.freedesktop.org/wiki/)
- [Boehm-Demers-Weiser conservative garbage collector](http://www.hpl.hp.com/personal/Hans_Boehm/gc/)
- [How To Write Shared Libraries by Ulrich Drepper](http://people.redhat.com/drepper/dsohowto.pdf)
- [Inline Functions In C](http://www.greenend.org.uk/rjk/tech/inline.html)
- [Blog of Agner Fog](http://www.agner.org/optimize/blog/)
- [Optimization Manuals by Agner Fog](http://www.agner.org/optimize/)
- [Auto-vectorization in GCC](http://gcc.gnu.org/projects/tree-ssa/vectorization.html)
- [Auto-Vectorization in LLVM](http://llvm.org/docs/Vectorizers.html)
