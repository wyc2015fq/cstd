# linux内核：__user,__kernel,__safe,__force,__iomem - Koma Hub - CSDN博客
2019年01月21日 21:59:25[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：220

首先看一下linux内核4.20.1源码：
## linux/linux/compile_types.h
```cpp
# define __user		__attribute__((noderef, address_space(1)))
# define __kernel	__attribute__((address_space(0)))
# define __safe		__attribute__((safe))
# define __force	__attribute__((force))
# define __nocast	__attribute__((nocast))
# define __iomem	__attribute__((noderef, address_space(2)))
# define __must_hold(x)	__attribute__((context(x,1,1)))
# define __acquires(x)	__attribute__((context(x,0,1)))
# define __releases(x)	__attribute__((context(x,1,0)))
# define __acquire(x)	__context__(x,1)
# define __release(x)	__context__(x,-1)
# define __cond_lock(x,c)	((c) ? ({ __acquire(x); 1; }) : 0)
# define __percpu	__attribute__((noderef, address_space(3)))
# define __rcu		__attribute__((noderef, address_space(4)))
# define __private	__attribute__((noderef))
```
## 引出Sparse
Sparse 诞生于 2004 年, 是由linux之父开发的, 目的就是提供一个静态检查代码的工具, 从而减少linux内核的隐患. 其实在Sparse之前, 已经有了一个不错的代码静态检查工具("SWAT"), 只不过这个工具不是免费软件, 使用上有一些限制.所以 linus 还是自己开发了一个静态检查工具.（[参考](https://blog.csdn.net/Rong_Toa/article/details/86584999)，[原文](https://lwn.net/Articles/87538/)）
Sparse通过 gcc 的扩展属性 __attribute__ 以及自己定义的 __context__ 来对代码进行静态检查.
|**宏名称**|**宏定义**|**检查点**|
|----|----|----|
|__bitwise|__attribute__((bitwise))|确保变量是相同的位方式(比如 bit-endian, little-endiandeng)|
|__user|__attribute__((noderef, address_space(1)))|指针地址必须在用户地址空间|
|__kernel|__attribute__((noderef, address_space(0)))|指针地址必须在内核地址空间|
|__iomem|__attribute__((noderef, address_space(2)))|指针地址必须在设备地址空间|
|__safe|__attribute__((safe))|变量可以为空|
|__force|__attribute__((force))|变量可以进行强制转换|
|__nocast|__attribute__((nocast))|参数类型与实际参数类型必须一致|
|__acquires(x)|__attribute__((context(x, 0, 1)))|参数x 在执行前引用计数必须是0,执行后,引用计数必须为1|
|__releases(x)|__attribute__((context(x, 1, 0)))|与 __acquires(x) 相反|
|__acquire(x)|__context__(x, 1)|参数x 的引用计数 + 1|
|__release(x)|__context__(x, -1)|与 __acquire(x) 相反|
|__cond_lock(x,c)|((c) ? ({ __acquire(x); 1; }) : 0)|参数c 不为0时,引用计数 + 1, 并返回1|
其中 __acquires(x) 和 __releases(x), __acquire(x) 和 __release(x) 必须配对使用, 否则 Sparse 会给出警告。以上内容来自：[https://www.cnblogs.com/wang_yb/p/3575039.html](https://www.cnblogs.com/wang_yb/p/3575039.html)
## **Sparse 使用方法**
### __bitwise 的使用
主要作用就是确保内核使用的整数是在同样的位方式下.
在内核代码根目录下 grep -r '__bitwise', 会发现内核代码中很多地方都使用了这个宏.
对于使用了这个宏的变量, Sparse 会检查这个变量是否一直在同一种位方式(big-endian, little-endian或其他)下被使用,
如果此变量在多个位方式下被使用了, Sparse 会给出警告.
内核代码中的例子:
```cpp
/* 内核版本:v2.6.32.61  file:include/sound/core.h 51行 */
typedef int __bitwise snd_device_type_t;
```
### __user 的使用
如果使用了 __user 宏的指针不在用户地址空间初始化, 或者指向内核地址空间, 设备地址空间等等, Sparse会给出警告.
内核代码中的例子:
```cpp
/* 内核版本:v2.6.32.61  file:arch/score/kernel/signal.c 45行 */
static int setup_sigcontext(struct pt_regs *regs, struct sigcontext __user *sc)
```
### __kernel 的使用
如果使用了 __kernel 宏的指针不在内核地址空间初始化, 或者指向用户地址空间, 设备地址空间等等, Sparse会给出警告.
内核代码中的例子:
```cpp
/* 内核版本:v2.6.32.61  file:arch/s390/lib/uaccess_pt.c 180行 */
memcpy(to, (void __kernel __force *) from, n);
```
### __iomem 的使用
如果使用了 __iomem 宏的指针不在设备地址空间初始化, 或者指向用户地址空间, 内核地址空间等等, Sparse会给出警告.
内核代码中的例子:
```cpp
/* 内核版本:v2.6.32.61  file:arch/microblaze/include/asm/io.h 22行 */
static inline unsigned char __raw_readb(const volatile void __iomem *addr)
```
### __safe 的使用
使用了 __safe修饰的变量在使用前没有判断它是否为空(null), Sparse会给出警告.
我参考的内核版本(v2.6.32.61) 中的所有内核代码都没有使用 __safe, 估计可能是由于随着gcc版本的更新,
gcc已经会对这种情况给出警告, 所以没有必要用Sparse去检查了.
### __force 的使用
使用了__force修饰的变量可以进行强制类型转换, 没有使用 __force修饰的变量进行强制类型转换时, Sparse会给出警告.
内核代码中的例子:
```cpp
/* 内核版本:v2.6.32.61  file:arch/s390/lib/uaccess_pt.c 180行 */
memcpy(to, (void __kernel __force *) from, n);
```
### __nocast 的使用
使用了__nocast修饰的参数的类型必须和实际传入的参数类型一致才行，否则Sparse会给出警告.
内核代码中的例子:
```cpp
/* 内核版本:v2.6.32.61  file:fs/xfs/support/ktrace.c 55行 */
ktrace_alloc(int nentries, unsigned int __nocast sleep)
```
### __acquires __releases __acquire __release的使用
这4个宏都是和锁有关的, __acquires 和 __releases 必须成对使用, __acquire 和 __release 必须成对使用, 否则Sparse会给出警告.
## **Sparse 在编译内核中的使用**
用 Sparse 对内核进行静态分析非常简单.
```bash
# 检查所有内核代码
make C=1 检查所有重新编译的代码
make C=2 检查所有代码, 不管是不是被重新编译
```
## **补充**
Sparse除了能够用在内核代码的静态分析上, 其实也可以用在一般的C语言程序中.
比如下面的小例子:
```cpp
/******************************************************************************
 * @file    : sparse_test.c
 * @author  : wangyubin
 * @date    : Fri Feb 28 16:33:34 2014
 * 
 * @brief   : 测试 sparse 的各个检查点
 * history  : init
 ******************************************************************************/
#include <stdio.h>
#define __acquire(x) __context__(x,1)
#define __release(x) __context__(x,-1)
int main(int argc, char *argv[])
{
    int lock = 1;
    __acquire(lock);
    /* TODO something */
    __release(lock);            /* 注释掉这一句 sparse 就会报错 */
    return 0;
}
```
如果安装了 Sparse, 执行静态检查的命令如下:
```
$ sparse -a sparse_test.c 
sparse_test.c:15:5: warning: context imbalance in 'main' - wrong count at exit
```
以上内容来自：[https://www.cnblogs.com/wang_yb/p/3575039.html](https://www.cnblogs.com/wang_yb/p/3575039.html)，在此鸣谢。
