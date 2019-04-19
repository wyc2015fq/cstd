# __attribute__((format(printf,m,n))) - Koma Hub - CSDN博客
2019年02月25日 22:21:05[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：89
可以使编译器检查函数声明和函数实际调用参数之间的格式化字符串是否匹配。
This **__attribute__** allows assigning **printf**-like or **scanf**-like characteristics to the declared function, and this enables the compiler to check the format string against the parameters provided throughout the code. This is *exceptionally* helpful in tracking down hard-to-find bugs.
There are two flavors:
- **__attribute__((format(printf,*m*,*n*)))**
- **__attribute__((format(scanf,*m*,*n*)))**
but in practice we use the first one much more often.
The (*m*) is the number of the "format string" parameter, and (*n*) is the number of the first variadic parameter. To see some examples:
```cpp
/* like printf() but to standard error only */
extern void eprintf(const char *format, ...)
	__attribute__((format(printf, 1, 2)));  /* 1=format 2=params */
/* printf only if debugging is at the desired level */
extern void dprintf(int dlevel, const char *format, ...)
	__attribute__((format(printf, 2, 3)));  /* 2=format 3=params */
```
With the functions so declared, the compiler will examine the argument lists
```
$ cat test.c
1  extern void eprintf(const char *format, ...)
2               __attribute__((format(printf, 1, 2)));
3
4  void foo()
5  {
6      eprintf("s=%s\n", 5);             /* error on this line */
7
8      eprintf("n=%d,%d,%d\n", 1, 2);    /* error on this line */
9  }
```
```
$ cc -Wall -c test.c
test.c: In function `foo':
test.c:6: warning: format argument is not a pointer (arg 2)
test.c:8: warning: too few arguments for format
```
Note that the "standard" library functions - printf and the like - are already understood by the compiler by default.
**format的语法格式为：**
format (archetype, string-index, first-to-check)
　　其中，“archetype”指定是哪种风格；“string-index”指定传入函数的第几个参数是格式化字符串；“first-to-check”指定从函数的第几个参数开始按上述规则进行检查。
具体的使用如下所示：
__attribute__((format(printf, a, b)))
__attribute__((format(scanf, a, b)))
--------------------- 
作者：huangjh2018 
来源：CSDN 
原文：https://blog.csdn.net/huangjh2017/article/details/76944564 
版权声明：本文为博主原创文章，转载请附上博文链接！
