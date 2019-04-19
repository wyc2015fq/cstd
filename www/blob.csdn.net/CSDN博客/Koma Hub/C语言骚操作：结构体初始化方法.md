# C语言骚操作：结构体初始化方法 - Koma Hub - CSDN博客
2018年09月01日 14:47:44[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：249
首先定义俩结构体：
```cpp
typedef struct 
{
	int a0;
	int b0;
}S0;
typedef struct 
{
	int a1;
	int b1;
	S0 s0;
}S1;
```
首先想到的初始化方法是：
```cpp
S0 s;
s.a0 = 1;
s.b0 = 2;
S1 ss;
ss.a1 = 3;
ss.b1 = 4;
ss.s0 = s;
```
或者是：
```cpp
S0 s = {1, 2};
S1 ss= {3, 4, s};
```
但是很多人不知道其实还可以这样初始化（用大括号括起来的病省略掉结构体名字的初始化方法，同样适用于结构体数组的初始化）：
```cpp
S0 s0 = {
	.a0 = 1,
	.b0 = 2,
};
S1 s1[10] = {
	[0 ... 9] = {
		.a1 = 3,
		.b1 = 4,
		.s0 = s0,
	}
};
```
这种结构体数组初始化不要太方便的说。直接用这个点省略繁琐的结构体名，更有数组直接使用省略号初始化。
整体的代码：
```cpp
#include <stdio.h>
#include <stdlib.h>
typedef struct 
{
	int a0;
	int b0;
}S0;
typedef struct 
{
	int a1;
	int b1;
	S0 s0;
}S1;
int main(int argc, char **argv)
{
	
	S0 s0 = {
		.a0 = 1,
		.b0 = 2,
	};
	
	S1 s1[10] = {
		[0 ... 9] = {
			.a1 = 3,
			.b1 = 4,
			.s0 = s0,
		}
	};
	
	printf("%d\n", s1[2].s0.a0);
	
	return 0;
}
```
