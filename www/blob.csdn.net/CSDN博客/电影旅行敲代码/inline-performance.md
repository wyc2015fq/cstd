# inline - performance - 电影旅行敲代码 - CSDN博客
2016年09月01日 21:56:35[电影旅行敲代码](https://me.csdn.net/dashuniuniu)阅读数：527标签：[C++](https://so.csdn.net/so/search/s.do?q=C++&t=blog)
个人分类：[c++基础](https://blog.csdn.net/dashuniuniu/article/category/5682427)
# 引言
函数内联（inline）算是C/C++很重要的语言特性，对于函数体比较小的函数来说 inline 可以说是一种比较重要的优化了。inline 在函数调用处（Call Site）使用函数体进行替换， inline 和 macro 很相似，但是又有一些不同，inline expansion 是在编译期间进行的，而 macro expansion 是在预处理阶段完成的。在编译期间可以获得类型等信息可以做一些编译期的语义检查，所以 inline 相比于 macro 来说很安全。
下面我们会针对 inline 来了解一下 inline 对程序性能真正的影响。
## inline 之于性能
由于省掉了函数调用的开销，例如函数压栈以及初始化栈（例如sub esp, length）的开销基本就省去了，内联后的函数运行速度通常是比不选择内联要快的，但是凡事总有两面性，函数内联会增加对内存的占用。函数内联通常用于执行次数很多，函数体又很小的函数，这样对内存的增加不会太多，同时又能够大大提高程序执行的效率。
inline 绝不仅仅只是省去函数调用的开销，更重要的是 inline 后，可以更方便的应用优化 pass。
> 
However, the primary benefit of inline expansion is to allow further optimizations and improved scheduling, **due to increasing the size of the functino body, as better optimization is possible on larger functions.**
即使是这样，刻画 inline 对性能的影响还是很复杂，因为 code size 还会影响 cache 表现。
> 
Depending on the specific program and cache, **inlining particular functions can increase or decrease performance.**
### function call的开销
由于内联会删掉function call，所以function call的开销需要我们搞清楚。函数调用分为以下几种：
- Direct，这是最普通的函数调用方式，函数地址在编译期就可以确定
- Indirect，这种方式通常都是通过函数指针的调用，函数地址在运行时才可获知
- Virtual，这种方式通常是面向对象编程中的多态，这也是 indirect 调用的一种
对于 Direct 调用来说，开销仅仅限于一条call指令，由于call 指令也要进行指令的跳转，涉及到指令跳转就要考虑 branch prediction ，但是由于 direct 指令的目标函数地址是固定的，只要该函数的调用足够频繁，调用时间足够局部，多次分支预测成功就会抵消第一次函数调用对pipeline的影响。
***To Do: 这里不够严谨，没有列举具体的体系结构***
对于 indirect call来说，开销除了上面 direct call的开销之外，还有获取目标函数地址的代价。例如：
```
function_ptr(arg1, arg2);
------------ Instructions below-------------
push arg2
push arg1
// loads the target function address from memory location
call function_ptr
```
对于没有 branch prediction 的 CPU 来说，indirect call基本上会花费 [the length of the pipeline]周期的开销。处理器必须等到分支地址计算完成才能获取下一条指令。但是如果indirect call每次调用的函数地址都相同，那么 branch predictor 就可以起到效果，现代 CPU 都有一种 BTB （branch target buffer）来记录跳转的历史记录，可以直接从中获取到正确的目标地址而不用等到整条流水线计算完成。
如果 CPU 支持乱序（out-of-order）执行， branch delay 也可以抵消这些开销。
### inline 对于其他优化技术的影响
就像我们前面提到的，inline 最基本的好处就是可以应用更长远的优化。Inline 后不用IPO（interprocedural optimization）就可以应用到其他优化技术，例如常量传播可以更方便的跨越函数调用，寄存器分配也可以在跨越“函数调用”进行分配。另外 function call 也会占用一些寄存器进行参数和返回值的传递，inline 也会减少register spill。
示例如下：
```
// Before inlining
int pred(int x)
{
    if (x == 0)
        return 0;
    else
        return x - 1;
}
int f(int y)
{
    return pred(y) + pred(0) + pred(y + 1);
}
```
在函数直接 inline 之后的代码如下：
```
int f(int y)
{
    int temp;
    if (y == 0) temp = 0; else tmp = y - 1; /*(1)*/
    if (0 == 0) temp += 0； else temp += 0 - 1; /*(2)*/
    if (y+1 == 0) temp +=0; else temp += (y + 1) - 1; /*(3)*/
}
```
如上所示在 inline 展开后，会揭示出更多的优化机会，例如第二条 if 语句就可以直接删除。最终 f() 函数可以优化成下面的情况。
```
int f(int y)
{
    if (y == 0)
        return 0;
    if (y == -1)
        return -2;
    return 2 * y - 1;   
}
```
***注：具体例子可以参看[wiki: Inline expansion](https://en.wikipedia.org/wiki/Inline_expansion#CITEREFChenChangConteHwu1993)***
### inline 对 cache 的影响
inline另一方面会对内存模型也有很大的影响，
- 删除分支并将code放置在一起可以提升 instruction 局部性
可是在某些情况下，inlining也会对 cache 也有不利的影响。如果当前某一层cache，在函数 inline 之前，可以满足程序的[working set](https://en.wikipedia.org/wiki/Working_set)，但是在 inline 之后由于代码大小的增长，装不下程序的working set，极有可能出现cache thrashing。
Inline 对 cache 性能的影响还是比较复杂的，如果 cache size 较小（远远小于程序的working set），inline 带来的顺序性（由于删除了跳转）起比较大的作用，此时 inline 可以提高 cache 性能。如果 cache size 接近于程序的working set，但是 inline 以后会大于程序的 working set，那么此时 inline 会降低 cache 的性能。
总之，inline 对于性能的影响比较复杂，针对不同的情况会有不同的表现。isocpp上也有对inline的讨论[Do inline functions improve performance?](https://isocpp.org/wiki/faq/inline-functions)，答案中也提到了没有一个确定的答案。
***注：原谅我恬不知耻的抄袭，下面的描述摘自isocpp***
> 
**Inline functions might make the code faster, they might make it slower.**
**They might cause thrashing, they might prevent thrashing.**
**And they might be, and often, totally irrelevant to speed.**
**inline function might make it *faster***: Procedural integration might remove a bunch of unnecessary instructions, which might make things run faster. 
**inline function might make it *slower*:** Too much inling might cause code bloat, which might cause “thrashing” on demand-paged virtual-memory systems. In other words, if the executable size is too big, the system might spend most of its time going out to disk to fetch the next chunk of code.
**inline functions might make it *larger*:** This is the notion of code bloat, as described above. For example, if a system has 100 inline functions each of which expands to 100 bytes of executable code and is called in 100 places, that’s an increase of 1MB. Is that 1MB going to cause problems? Who knows, but it is possible that last 1MB could cause the system to “thrash”, and that could slow thinds down.
**inline functions might make it *smaller*: The compiler often gernerate more code to push/pop registers/parameters that it would by inline-expanding the function’s body.** This happens with very small functions, and it also happens with large functions when the optimizer is able to remove a lot of redundant code through procedural integration - that is, when the optimizer is able to make the large function small.
**注：上面粗体部分有些疑问，inline不是在compile-time 生成中间代码之前就完成了吗，为什么会涉及到寄存器相关问题？**
**inline functions might cause *thrashing* :** Inlining might increase the size of the binary executable, and that might cause thrashing.
**inline functions might *prevent thrashing*:** The working set size(number of pages that need to be in memory at once) might go down even if the executable size goes up. When f() calls g(), the code if often on two distinct pages; when the compiler procedurally integrates the code of g() into f(), the code is often on the same page.
**inline function might *increase the number of cache misses*:** Inlining might cause an inner loop to span across multiple lines of the memory cache, and that might cause thrashing of the memory cache.
**inline functions might *decrease the number of cache misses*:** Inlining usually improves locality of references within the binary code, which might decrease the number of cache lines needed to store the code of an inner loop. This ultimately could cause a CPU-bound application to run faster.
**inline functions might be *irrelevant to speed*:** Most systems are not CPU-bound. Most systems are I/O-bound, database-bound or network-bound, meaning the bottleneck in the system’s overall performance is the file system, the database or the network. Unless your “CPU meter” is pegged at 100%, inline functions probably won’t make your system faster. (Even in CPU-bound systems, inline will help only when used within the bottleneck itself, and the bottleneck is typically in only a small percentage of the code.)
**There are no simple answers:** You have to play with it to see what is best. Do *not* settle for simplistic answers like, “Never use inline functions” or “Always use inline functions” or “Use inline functions if and only if the function is less than N lines of code.” **These one-size-fits-all rules may be easy to write down, but they will produce sub-optimal results.**
就像上面的描述所说，从不要急切下一个定论，没有适应所有场景的规则。同样inline也不适用于所有场景。
