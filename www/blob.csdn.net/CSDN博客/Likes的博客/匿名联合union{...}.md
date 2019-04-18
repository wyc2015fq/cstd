# 匿名联合union{...} - Likes的博客 - CSDN博客
2018年08月02日 19:13:56[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：268
*匿名联合体*是不同时定义任何变量（包含联合体类型的对象、引用或指向联合体的指针）的无名联合体定义。
匿名联合体有更多限制：它们不能有成员函数，不能有静态数据成员，且所有数据成员必须为公开。允许的声明仅有非静态数据成员和[static_assert](https://zh.cppreference.com/w/cpp/language/static_assert) 声明 (C++14 起)。
匿名联合体的成员被注入外围作用域（而且必须不与声明于其处的其他名称冲突）。
示例代码：
```cpp
#include <iostream>
using namespace std;
int main()
{
	union
	{
		int a;
		const char* p;
	};
	a = 1;
	p = "Jennifer";
	cout << &a << endl << &p << endl;
	return 0;
}
```
实验结果：
![](https://img-blog.csdn.net/20180802191140319?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NvbmdjaHV3YW5nMTg2OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
说明a，p使用的事同一个实例
