# C++指针学习 - writeeee的专栏 - CSDN博客
2015年07月10日 08:46:49[writeeee](https://me.csdn.net/writeeee)阅读数：334
个人分类：[c++](https://blog.csdn.net/writeeee/article/category/5634855)
**指针的声明有两种风格**
int*  x,y;//  这里要尽量写成int* x； int* y; 起强调作用
int  *x,y;//声明了一个指针和一个int变量
**指针是有类型限值的**
不能用int型指针去接收double型指针变量
**Void指针**
void 指针可以指向任意类型的地址
void 指针的操作很受限值（指针比较，给函数传递参数，复制给void类型的指针），不能用此类型的指针去操作对象。
不能用 *p ,不能用p++（,因为不知道存储类型，也就不知道++对应的步长是多少）
**关于指针作为形参的问题**
```cpp
#include <STRING.H>
#include <STDIO.H>
void reset(int *p)
{
	*p=0;
	p=0;
}
int main()
{
	int ival(1024);
	printf("%d\r\n",ival);
	int *x=&ival;
	int *p=&ival;
	
	printf("%d\r\n",*p);
	reset(p);
	printf("%d\r\n",*p);
	printf("%d\r\n",*x);
	return 0;
}
```
两点新需要注意
1.指向的地址内存放的值发生了变化
2.指向的地址没有发生变化。（以前没有关注过这一条）
