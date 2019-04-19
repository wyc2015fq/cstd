# C++归纳法解决动物繁殖问题算法 - fanyun的博客 - CSDN博客
2018年11月28日 21:27:34[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：259
1.有一对兔子，从出生后第3个月起每个月都生一对兔子，小兔子长到第三个月,后每个月又生一对兔子，假如兔子都不死，问每个月的兔子总数为多少？
2.实现原理
      先推算前几步，然后自己分析一个规律，再验证这个规律，其实就是归纳法，分析完之后，发现兔子的继承关系和最右侧的兔子总数，符合斐波那契数列规律。
3.代码实现如下
```cpp
#include "stdafx.h"
#include<stdio.h>
int main()
{
	int a = 1;
	int i;
	int b = 1;
	int c = 0;
	for (i = 3; i < 12 + 1; i++)
	{
		c = b;
		b = a + b;
		a = c;
		printf("第%d个月,兔子为:%d\n", i, b);
	}
	return 0;
}
```
4.代码输出
3![](https://img-blog.csdnimg.cn/20181127171919393.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zhbnl1bl8wMQ==,size_16,color_FFFFFF,t_70)
