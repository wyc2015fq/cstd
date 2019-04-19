# __builtin_expect提高运行效率 - Koma Hub - CSDN博客
2019年01月29日 21:36:55[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：32
个人分类：[Glibc																[C/C++](https://blog.csdn.net/Rong_Toa/article/category/7156199)](https://blog.csdn.net/Rong_Toa/article/category/8605571)
这个指令是gcc引入的，作用是"允许程序员将最有可能执行的分支告诉编译器"。这个指令的写法为：__builtin_expect(EXP, N)。意思是：EXP==N的概率很大。
给出glibc-2.28中
cdefs.h    misc\sys    18008    2018/8/1    
```cpp
#if __GNUC__ >= 3
# define __glibc_unlikely(cond)	__builtin_expect ((cond), 0)
# define __glibc_likely(cond)	__builtin_expect ((cond), 1)
#else
# define __glibc_unlikely(cond)	(cond)
# define __glibc_likely(cond)	(cond)
#endif
```
一个简单的例子：e_fmod.c    sysdeps\ieee754\dbl-64    4023    2018/8/1    141
```cpp
/* determine iy = ilogb(y) */
  if (__glibc_unlikely (hy < 0x00100000))                  /* subnormal y */
    {
      if (hy == 0)
	{
	  for (iy = -1043, i = ly; i > 0; i <<= 1)
	    iy -= 1;
	}
      else
	{
	  for (iy = -1022, i = (hy << 11); i > 0; i <<= 1)
	    iy -= 1;
	}
    }
  else
    iy = (hy >> 20) - 1023;
  /* set up {hx,lx}, {hy,ly} and align y to x */
  if (__glibc_likely (ix >= -1022))
    hx = 0x00100000 | (0x000fffff & hx);
  else                  /* subnormal x, shift x to normal */
    {
      n = -1022 - ix;
      if (n <= 31)
	{
	  hx = (hx << n) | (lx >> (32 - n));
	  lx <<= n;
	}
      else
	{
	  hx = lx << (n - 32);
	  lx = 0;
	}
    }
```
相关文章：
[http://blog.man7.org/2012/10/how-much-do-builtinexpect-likely-and.html](http://blog.man7.org/2012/10/how-much-do-builtinexpect-likely-and.html)
[https://blog.csdn.net/shuimuniao/article/details/8017971](https://blog.csdn.net/shuimuniao/article/details/8017971)
