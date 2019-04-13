
# C++函数重载 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年10月29日 23:24:09[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：892标签：[c++																](https://so.csdn.net/so/search/s.do?q=c++&t=blog)个人分类：[C、C++开发																](https://blog.csdn.net/heyongluoyao8/article/category/908444)



出现在相同作用域的二个函数，如果具有相同的名字而形参表不同，则称为重载函数。
如果二个函数声明的返回类型表完全匹配，则将第二个函数声明视为第一个的重复声明。如果二个函数的形参表完全相同，但返回类型不同，则第二个声明是错误的。
仅当形参是引用或者指针时，形参是否为const才有影响。
如：
int fn(int);与 int fn(const int); 他们不是重载，而是重复声明
int fn(int &); inf fn(const int&);他们是重载
也不能基于指针本身是否为const来重载。
int fn(int *conts);与int fn(int *); 他们不是重载，而是重复声明
重载与作用域：
如果局部地声明一个函数，则该函数将屏蔽而不是重载在外层作用域中声明的同名函数。由此可得，每一个版本的重载函数都应该在同一个作用域中声明

