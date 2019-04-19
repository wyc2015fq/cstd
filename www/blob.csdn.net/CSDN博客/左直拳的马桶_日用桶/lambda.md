# lambda - 左直拳的马桶_日用桶 - CSDN博客
2010年10月25日 17:48:00[左直拳](https://me.csdn.net/leftfist)阅读数：1015标签：[lambda](https://so.csdn.net/so/search/s.do?q=lambda&t=blog)
个人分类：[.NET](https://blog.csdn.net/leftfist/article/category/94497)
lambda是函数。是匿名函数。
最简单的lambda表达式，如：x => x * x，其实最完整的写法为：
(int x) => x * x
所以，左边的是函数参数，右边的函数体。只不过只有一个参数的话，括号（）就不用写而已；并且系统会猜测参数的类型，所以一般参数类型也不用写。
delegate int D(int x);
……
D d;
d = (int x) => x * x;
如果没有参数，要写成 () => x * x;
