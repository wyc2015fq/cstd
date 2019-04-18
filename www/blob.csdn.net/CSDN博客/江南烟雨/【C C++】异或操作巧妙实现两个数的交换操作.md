# 【C/C++】异或操作巧妙实现两个数的交换操作 - 江南烟雨 - CSDN博客
2012年06月08日 19:48:50[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：3411
今天在看OpenGL加载TGA格式图像用作纹理的代码时，看到关于RGB(A)顺序转换的一行代码时，捉一开始感到很困惑，后来想了想，就是实现交换操作。
原始代码：
```cpp
texture->imageData[cswap] ^= texture->imageData[cswap+2] ^=
texture->imageData[cswap] ^= texture->imageData[cswap+2];
```
写了一段代码测试了一下：
```cpp
#include <iostream>
using namespace std;
int main()
{
	int a = 1;
	int b = 2;
	a ^= b ^= a ^= b;
	cout << "a = " <<a << endl;
	cout << "b = " <<b << endl;
}
```
运行结果：
![](https://img-my.csdn.net/uploads/201206/08/1339155844_3035.jpg)
^ 在 C 里面是按位异或操作符，相同的话异或的结果就是 0，不同的话就是 1。
其实用笔推一下就知道了。
首先，运算顺序是从右往左的。
假设a、b原始值记为a0,b0.
最右边的^=运算之后：
b不变。b = b0.
a = a0 ^ b0;
倒数第二个^=运算之后：
a不变。a = a0 ^ b0;
b = b0 ^ a = b0 ^ (a0 ^ b0) = b0 ^ (b0 ^ a0) = a0;
最前面的^=运算之后：
b不变。b= a0;
a = a ^ b = (a0 ^ b0) ^ a0 = b0.
这样就实现了a和b两个数的交换操作。
说实话，第一次见到这样的语句时，确实很费解。诚然，这样的操作相比声明一个临时变量，节省空间。但是感觉代码的可读性并不是很好。
我看到原始代码这条语句上还有一个注释，说是XX优化的。
但是我写了一个程序测试了一下，异或操作方法好像还要慢一些。
```cpp
#include <iostream>
#include <time.h>
#include <windows.h>
using namespace std;
int main()
{
	int a = 1;
	int b = 2;
	
	int temp = 0;
	DWORD time1 = GetTickCount();
	cout << "time1 = " << time1 << endl;
	for (int i = 0;i < 100000000;++i)
	{
		//a ^= b ^= a ^= b;
		temp = a;
		a = b;
		b = temp;
	}
	DWORD time2 = GetTickCount();
	cout << "time2 = " << time2 << endl;
	cout << time2 - time1 << endl;
}
```
普通方法：
![](https://img-my.csdn.net/uploads/201206/08/1339157089_4540.jpg)
异或方法：
![](https://img-my.csdn.net/uploads/201206/08/1339157103_7243.jpg)
个人感觉还是用普通的方法好点。路过的给点建议哈~~
