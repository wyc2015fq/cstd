# likely() 和 unlikely()内建宏定义函数 - Koma Hub - CSDN博客
2019年03月04日 21:21:41[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：25

这给出列内建函数可能发生和不可能发生的编译选项，这是一个预处理，在代码被编译成二进制后，程序在运行时候由于if/else if/else或者其他类型的“地址转换”过程，通过使用likely可以告诉编译器，这个判断发生的概率较小，编译器会将其编译到“更远”（地址跳转幅度更大的地方），这样，减小了代码跳转的幅度，减小了运行时间。
给出代码定义
```cpp
#include <stdio.h>
# define likely(x) __builtin_expect(!!(x), 1)
# define unlikely(x) __builtin_expect(!!(x), 0)
int main()
{
	int i = 0;
	//is unlikely happen, but is still possible happen
	if(unlikely(i+1))
		printf("i+1 = %d\n", i+1);
	//so does likely()
	return 0;
}
```
