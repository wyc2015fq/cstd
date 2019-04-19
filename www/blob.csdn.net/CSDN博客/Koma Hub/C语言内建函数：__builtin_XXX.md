# C语言内建函数：__builtin_XXX - Koma Hub - CSDN博客
2019年03月04日 22:13:21[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：34

首先内建函数有的属于预编译，有的属于运行过程的函数，这一点我现在还不是很清楚，待以后进一步详细讨论。
然后我们给出一个长代码，里面包含了常用的C语言内建函数，参考文章见文末。
总览：
- __builtin_ffs(s)；
- __builtin_popcount(s)；
- __builtin_ctz(s)；
- __builtin_clz(s)；
- __builtin_parity(i)；
- __builtin_return_address(s)；
- __builtin_bswap16(i)；
- __builtin_bswap32(i)；
- __builtin_prefetch(const void *addr, ...)；
- __builtin_constant_p(exp);
- __builtin_types_compatible_p(t1, t2);
- __builtin_expect(exp, c);
```cpp
#include <stdio.h>
#include <stdarg.h>
/**
 *	返回x中最后一个为1的位是从后向前的第几位
 *	000101000 -> 4
 *	8 -> 4
 */
#define ffs(s) __builtin_ffs(s)
/**
 *	x中1的个数
 *
 */
#define popcount(s) __builtin_popcount(s)
/**
 *	x末尾0的个数。x=0时结果未定义。
 *	x前导0的个数。x=0时结果未定义。
 */
#define ctz(s) __builtin_ctz(s)
#define clz(s) __builtin_clz(s)
/**
 *	x中1的奇偶性。
 *	奇:1, 偶:0
 */
#define parity(i) __builtin_parity(i)
/**
 *	当前函数的第n级调用者的地址，用的最多的就是__builtin_return_address(0)，
 *	即获得当前函数的调用者的地址。
 *	注意，该函数实现是体系结构相关的，有些体系结构只实现了n=0的返回结果。
 */
#define return_addr(s) __builtin_return_address(s)
void test1_return_address(){printf("%s: %p\n", __func__, return_addr(1));}
void test2_return_address(){test1_return_address(); printf("%s: %p\n", __func__, return_addr(0));}
void test3_return_address(){test2_return_address(); printf("%s: %p\n", __func__, return_addr(0));}
/**
 *	按字节翻转x，返回翻转后的结果。
 *	uint16_t __builtin_bswap16 (uint16_t x)
 *	uint32_t __builtin_bswap32 (uint32_t x)
 */
#define swap16(i) __builtin_bswap16(i)
#define swap32(i) __builtin_bswap32(i)
/**
 *	它通过对数据手工预取的方法，在使用地址addr的值之前就将其放到cache中，
 *	减少了读取延迟，从而提高了性能，但该函数也需要 CPU 的支持。
 *	该函数可接受三个参数，
 *	第一个参数addr是要预取的数据的地址，
 *	第二个参数可设置为0或1（1表示我对地址addr要进行写操作，0表示要进行读操作），
 *	第三个参数可取0-3（0表示不用关心时间局部性，取完addr的值之后便不用留在cache中，
 *	而1、2、3表示时间局部性逐渐增强）。
 *	__builtin_prefetch (const void *addr, ...)
 */
#define prefetch(...) __builtin_prefetch(__VA_ARGS__)
/**
 *	判断exp是否在编译时就可以确定其为常量，
 *	如果exp为常量，该函数返回1，否则返回0。
 *	如果exp为常量，可以在代码中做一些优化来减少处理exp的复杂度
 */
#define constant_p(exp) __builtin_constant_p(exp)
#define CONST_P	123
/**
 *	判断type1和type2是否是相同的数据类型，相同返回1，否则返回0。
 *	该函数不区分const/volatile这样的修饰符，即int和const int被认为是相同的类型
 */
#define types_cmp_p(t1, t2) __builtin_types_compatible_p(t1, t2)
#if 0
#define foo(x) \
typeof(x) tmp = (x);\
if(__builtin_types_compatible_p(typeof(x), int))\
//do something...\
else \
//do something...\
tmp;
#endif //0
/**
 *	用来引导gcc进行条件分支预测。在一条指令执行时，由于流水线的作用，
 *	CPU可以完成下一条指令的取指，这样可以提高CPU的利用率。
 *	在执行一条条件分支指令时，CPU也会预取下一条执行，但是如果条件分支跳转到了其他指令，
 *	那CPU预取的下一条指令就没用了，这样就降低了流水线的效率。
 *	内核中的likely()和unlikely()就是通过__builtin_expect来实现的。
 *	__builtin_expect (long exp, long c)函数可以优化程序编译后的指令序列，
 *	使指令尽可能的顺序执行，从而提高CPU预取指令的正确率。该函数的第二个参数c可取0和1，
 */
#define except(exp, c) __builtin_expect(exp, c)
#define likely(x) __builtin_expect(!!x, 1)
#define unlikely(x) __builtin_expect(!!x, 0)
int main()
{
	int i = 0x03f8;
	printf("%d, %d, %d, %d\n", ffs(i), popcount(i), ctz(i), clz(i));
	printf("%d\n", parity(i));
	printf("main: %p\n", main);
	test3_return_address();
	printf("%x, %x\n", i, swap32(i));
	prefetch(&i, 0, 2);
	printf("%p\n", &i);
	printf("constant: %d\n", constant_p(CONST_P));
	printf("constant: %d\n", constant_p(i));
	int j;
	printf("type cmp: %d\n", types_cmp_p(typeof(j), typeof(i)));	
	if(likely(i))
		printf("likely:%ld, %d\n", likely(i), i);
	return 0;
}
/**
 https://blog.csdn.net/jasonchen_gbd/article/details/44948523
 */
```
> 
作者：落尘纷扰 
来源：CSDN 
原文：https://blog.csdn.net/jasonchen_gbd/article/details/44948523 
版权声明：本文为博主原创文章，转载请附上博文链接！
