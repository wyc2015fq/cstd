# Linux Kernel 代码艺术——编译时断言 - 文章 - 伯乐在线
原文出处： [hazir](http://www.cnblogs.com/hazir/p/static_assert_macro.html)
本系列文章主要写我在阅读Linux内核过程中，关注的比较难以理解但又设计巧妙的代码片段（不关注OS的各个模块的设计思想，此部分我准备写在“深入理解Linux Kernel” 系列文章中），一来通过内核代码复习一下C语言及汇编语言的语法，二来学习内核开发大牛们书写代码的风格及思路。
在内核文件 include/linux/bug.h中，有下面两行的宏定义:


```
/* Force a compilation error if condition is true, but also produce a
result (of value 0 and type size_t), so the expression can be used
e.g. in a structure initializer (or where-ever else comma expressions
aren't permitted). */
#define BUILD_BUG_ON_ZERO(e) (sizeof(struct { int:-!!(e); }))
#define BUILD_BUG_ON_NULL(e) ((void *)sizeof(struct { int:-!!(e); }))
```
以第一个分析，它表示的是：检查表达式e是否为0，为0编译通过且返回0；如果不为0，则编译不通过。
可能从这个宏的名字上看可能容易理解错，或者改为“BUILD_BUG_OR_ZERO”更好，关于这个的讨论有人也提交这个patch（[http://lkml.indiana.edu/hypermail/linux/kernel/0703.1/1546.html](http://lkml.indiana.edu/hypermail/linux/kernel/0703.1/1546.html)），但未能被社区接受。我们且不管这个宏定义名字怎样，来逐步分析一下这个宏是如何来实现的：
sizeof(struct { int : –!!(e); } ))
> 
1. (e): 表达式e的声明
2. !!(e): 对e的结果进行两次求非。即如果e开始是0的话，结果就是0；如果e不为0，则结果为1。
3. –!!(e): 再乘以-1。如果第2步结果为0，则仍为0；否则结果为-1。
4. struct { int : –!!(0); }  –>  struct { int : 0; } : 如果e的结果为0，则我们声明一个结构体拥有一个int型的数据域，并且规定它所占的位的个数为0。这没有任何问题，我们认为一切正常。
5. struct { int : –!!(1); }  –>  struct { int : –1; } : 如果e的结果非0，结构体的int型数据域的位域将变为一个负数，将位域声明为负数这是一个语法的错误。
6. 现在要么结果为声明了一个位域为0的结构体，要么出现位域为负数编译出错；如果能正确编译，然后我们对该结构体进行sizeof操作，得到一个类型为size_t的结果，值为0。
再总结一下，BUILD_BUG_ON_ZERO(e) 表示的就是若表达式e结果为0，则编译通过，该宏的值也为0；若表达式e的结果不为0，则编译不通过。
这会让人联想到C语言中 assert 宏的用法：
void assert(int expression);
如果参数expression 计算的结果为0，它先向stderr打印一条出错信息，然后通过调用 abort 来终止程序运行；否则断言成立，继续执行。
我们讨论的宏与assert本质区别在于，我们的宏在编译时进行测试，而assert宏是在运行时测试。我们希望能尽早地捕获到我们编译时的错误，而不是推迟到运行时。我管这种宏用法叫做“编译时断言”，assert为“运行时断言”。
理解了上面之后，再来看看第二个BUILD_BUG_ON_NULL(e)宏，与第一个类似，用来在编译时断言e是否为NULL，若是这个宏返回(void *)0 (即NULL，与第一个宏的区别)；不为NULL时编译出错。
除了上面的两个编译时断言之外，include/linux/bug.h文件中还有另几个大家可以思考表示何意，如：

```
#define BUILD_BUG_ON(condition) ((void)sizeof(char[1 - 2*!!(condition)]))   
#define BUILD_BUG_ON_NOT_POWER_OF_2(n)             
    BUILD_BUG_ON((n) == 0 || (((n) & ((n) - 1)) != 0))
```
含义可以参考文件中宏定义的注释说明。
参考资料：
[http://blog.csdn.net/jiyucn/article/details/862085](http://blog.csdn.net/jiyucn/article/details/862085)   C语言中关于结构体位域的详细说明
[http://blog.csdn.net/jiyucn/article/details/862062](http://blog.csdn.net/jiyucn/article/details/862062#plain)   C语言中sizeof相关问题
[http://www.cplusplus.com/reference/cassert/assert/](http://www.cplusplus.com/reference/cassert/assert/)   assert用法说明
[http://stackoverflow.com/questions/9229601/what-is-in-c-code](http://stackoverflow.com/questions/9229601/what-is-in-c-code)   问题及解答均来源于Stackoverflow
