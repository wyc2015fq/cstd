# C++ 快速随机数生成器 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [LynnShaw](http://www.jobbole.com/members/shaoai37) 翻译，[wrm](http://www.jobbole.com/members/wrm205) 校稿。未经许可，禁止转载！
英文出处：[stackoverflow](http://stackoverflow.com/questions/1640258/need-a-fast-random-generator-for-c)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
**问题：**
> 
在计算旅行商问题中的欧几里得距离时，我试图使用 [3-opt 算法](https://en.wikipedia.org/wiki/3-opt)来交换某些节点；因为大多情况下我有大约超过500个节点，我需要随机地选择至少1/3的节点进行交换。
所以我需要一个**快速**产生随机数的函数（标准函数rand()太慢了）。不需要多么巧妙，差不多就行了。
**注**：忘了说，我无法使用除了标准库（比如 STL，iostream 等）以外的库，所以 boost 库也不行。
### AndyV 的最佳答案：
其他人的答案提到了 Marsaglia 的 Xorshift 算法（[https://en.wikipedia.org/wiki/Xorshift](https://en.wikipedia.org/wiki/Xorshift)），但是没人贴代码。

C++
```
static unsigned long x=123456789, y=362436069, z=521288629;
unsigned long xorshf96(void) {          //period 2^96-1
unsigned long t;
    x ^= x << 16;
    x ^= x >> 5;
    x ^= x << 1;
   t = x;
   x = y;
   y = z;
   z = t ^ x ^ y;
  return z;
}
```
这段代码几乎在任何地方都适用；然而当我试图产生随机的二进制矩阵时，它却失败了。大概是 95×95 的矩阵，它生成了太多或太少的奇异矩阵（我忘了具体是多还是少了）。这个随机数生成器相当于一个[线性反馈](https://zh.wikipedia.org/wiki/%E7%BA%BF%E6%80%A7%E5%8F%8D%E9%A6%88%E7%A7%BB%E4%BD%8D%E5%AF%84%E5%AD%98%E5%99%A8)[移位](https://zh.wikipedia.org/wiki/%E7%BA%BF%E6%80%A7%E5%8F%8D%E9%A6%88%E7%A7%BB%E4%BD%8D%E5%AF%84%E5%AD%98%E5%99%A8)[寄存器](https://zh.wikipedia.org/wiki/%E7%BA%BF%E6%80%A7%E5%8F%8D%E9%A6%88%E7%A7%BB%E4%BD%8D%E5%AF%84%E5%AD%98%E5%99%A8)。不过如果你要做加密或者蒙特卡洛的相关工作，这个生成器就不能用了。
### 补充：Xorshift 算法介绍
**Xorshift** 随机数生成器是 [George Marsaglia](https://en.wikipedia.org/wiki/George_Marsaglia) 发明的一类[伪随机数生成器](https://en.wikipedia.org/wiki/Pseudorandom_number_generator)。它们通过和自己[逻辑移位](https://en.wikipedia.org/wiki/Logical_shift)后的数进行[异或操作](https://en.wikipedia.org/wiki/Bitwise_operation#XOR)来生成序列中的下一个数。这在现代计算机体系结构非常快。它们是[线性反馈](https://zh.wikipedia.org/wiki/%E7%BA%BF%E6%80%A7%E5%8F%8D%E9%A6%88%E7%A7%BB%E4%BD%8D%E5%AF%84%E5%AD%98%E5%99%A8)[移位](https://zh.wikipedia.org/wiki/%E7%BA%BF%E6%80%A7%E5%8F%8D%E9%A6%88%E7%A7%BB%E4%BD%8D%E5%AF%84%E5%AD%98%E5%99%A8)[寄存器](https://zh.wikipedia.org/wiki/%E7%BA%BF%E6%80%A7%E5%8F%8D%E9%A6%88%E7%A7%BB%E4%BD%8D%E5%AF%84%E5%AD%98%E5%99%A8)的一个子类，其简单的实现使它们速度更快且使用更少的空间。然而，必须仔细选择合适参数以达到长周期。
Xorshift生成器是非[密码安全的随机数生成器](https://en.wikipedia.org/wiki/Cryptographically_secure_pseudorandom_number_generator)中最快的一种，只需要非常短的代码和状态。虽然它们没有进一步改进以通过统计检验，这个缺点非常著名且容易修改（Marsaglia 在原来的论文中指出），用复合一个非线性函数的方式，可以得到比如像 xorshift+ 或 xorshift* 生成器。一个简单的C语言实现的 xorshift+ 生成器通过了所有的 BigCrush 的测试（比 [Mersenne Twister 算法](https://en.wikipedia.org/wiki/Mersenne_Twister)和 [WELL 算法](https://en.wikipedia.org/wiki/Well_equidistributed_long-period_linear)的失败次数减少了一个数量级），而且在 x86 上产生一个随机数通常只需要不到十个时钟周期，多亏了指令流水线。
因为一般的 xorshift 生成器（没有非线性这一步）在一些统计测试中失败了，所以经常被指责是不可靠的。
**示例实现**
一个 xorshift 生成器的 C/C++ 版本实现：

C
```
#include <stdint.h>
/* These state variables must be initialized so that they are not all zero. */
uint32_t x, y, z, w;
uint32_t xorshift128(void) {
    uint32_t t = x ^ (x << 11);
    x = y; y = z; z = w;
    return w = w ^ (w >> 19) ^ t ^ (t >> 8);
}
```
这个算法有一个最大的周期2128 − 1 ，且通过了[diehard 测试](https://en.wikipedia.org/wiki/Diehard_tests)。不过，它在 [TestU01](https://en.wikipedia.org/wiki/TestU01) 框架的 BigCrush 测试套件中的 MatrixRank 和 LinearComp 测试中失败了。
