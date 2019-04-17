# 【Sprintf】llx&I64x _sprintf字符串读写的妙用 - CD's Coding - CSDN博客





2015年04月22日 15:59:09[糖果天王](https://me.csdn.net/okcd00)阅读数：1806








今天在网上GET了一个相当棒的存在，"llx"和"I64x"，

是一种可以将一个long long长度的十进制整数直接转化为十六进制存于buffer中的方式，

曾经有了解过sscanf和sprintf，但这次这个方式的发现着实非常令人开心，回来再学一次，作为一种技巧好好收着~

P.S. 通过更改x的大小写可以决定十六进制的大写/小写表示哦~




Code：



```cpp
#include<cmath>
#include<cstdio>
#include<iostream>
#include<algorithm>
using namespace std;

int main()
{
	char buffer[1024];
	unsigned long long ull=2222222222222222222;
	sprintf (buffer, "%llx", ull);
	//sprintf (buffer, "%I64x", ull);
	//sprintf (buffer, "%llX", ull);
	//sprintf (buffer, "%I64X", ull);
	cout<<buffer<<endl;
	return 0;
}
```


前两种输出为：1ed6eb565788e38e

后两种输出为：1ED6EB565788E38E



