
# C-C++函数指针与指针函数 - 嵌入式Linux - CSDN博客

2018年12月20日 15:39:02[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：57



## 前面说的话
面试的时候，经常有面试官问这个问题，在Linux内核里面也是经常被使用的，在看很多大神的代码里面，我们也经常遇到函数指针与指针函数，一样，如果你自己没问题了，就不用往下看了。

## 定义
我们看个代码
`int *func(int a,int b)`我们之前说过运算符的优先级，*****的优先级低于**（**
由于“*”的优先级低于“()”的优先级，因而func首先和后面的“()”结合，也就意味着，func是一个函数。即：
`int *(func)(int a,int b)`然后这个函数的返回值的类型是**int * 即：**指向int类型的指针
然后我们这样修改上面的代码
`int (*func)(int a,int b)`**（*func）**说明func是一个指针，然后后面跟着**（）**说明这个指针指向一个函数，即函数指针。
所以
**函数指针：**首先是一个指针，这个指针指向一个函数
**指针函数：**首先是一个函数，这个函数的返回值一个指针

## 用typedef声明一个函数指针
我们声明一个函数指针，正常方法是
`int (*pfunc)(int a,int b)`当我们命名很多个函数指针的时候，用上面的方法显得非常不方便,所以我们可以这样做
`typedef int (*PF) (int a,intb)
PF pfunc;`
例程：
`#include "stdio.h"
typedef int(*PF)(int, int);
int add(int a, int b)
{
	return a + b;
}
int reduce(int a, int b)
{
	return a - b;
}
int main()
{
	PF pfunc = NULL;
	pfunc = add;
	printf("add:%d\n",pfunc(3, 4));
	pfunc = reduce;
	printf("reduce:%d\n", pfunc(3, 4));
	/*getchar是用VS编写方便查看输出*/
	getchar();
	return 0;
}`![](https://img-blog.csdnimg.cn/20181220153902315)
![](https://img-blog.csdnimg.cn/20181220153902334)
欢迎关注微信公众号-**嵌入式Linux**
觉得不错，请帮忙**转发，点赞**，您的每一次支持，我都将铭记于心

