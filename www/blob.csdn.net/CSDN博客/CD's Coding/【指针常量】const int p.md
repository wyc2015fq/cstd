# 【指针常量】const int* p - CD's Coding - CSDN博客





2014年10月27日 11:52:18[糖果天王](https://me.csdn.net/okcd00)阅读数：596








最近看到群里讨论起了JAVA里的final可否修改值的问题，然后引申到了C++的const……

觉得还是写一个关于const int* 的东西好了……




翻了下以前给@Zoecur 看的 const 的帖子，也在这里贴一下好了：

http://triumple163.blog.163.com/blog/static/1279782662009102594241969/





用两个简单的程序给你们看看：



```cpp
#include<iostream>
using namespace std;

int main()
{
	int a[10];
	const int *p= a;
//	const int *a=new int[10];
	a[0] = 1;
	return 0;
}
```



编译器: TDM-GCC 4.8.1 64-bit Release

执行 g++.exe...

g++.exe "D:\Temp\未命名1.cpp" -o "D:\Temp\未命名1.exe" -g3 -I"C:\Program Files (x86)\Dev-Cpp\MinGW64\include" -I"C:\Program Files (x86)\Dev-Cpp\MinGW64\x86_64-w64-mingw32\include" -I"C:\Program Files (x86)\Dev-Cpp\MinGW64\lib\gcc\x86_64-w64-mingw32\4.8.1\include"
 -I"C:\Program Files (x86)\Dev-Cpp\MinGW64\lib\gcc\x86_64-w64-mingw32\4.8.1\include\c++" -L"C:\Program Files (x86)\Dev-Cpp\MinGW64\lib" -L"C:\Program Files (x86)\Dev-Cpp\MinGW64\x86_64-w64-mingw32\lib" -static-libgcc -g3

编译 0.81 秒后成功







```cpp
#include<iostream>
using namespace std;

int main()
{
//	int a[10];
//	const int *p= a;
	const int *a=new int[10];
	a[0] = 1;
	return 0;
}
```
97D:\Temp\未命名1.cpp[Error] assignment of read-only location '* a'






所以呢，我们可以const int* a来指向一个已声明的数组地址，这样的话我们无法修改*a 但是却可以修改 a[n]

然而使用new的话我们可以新建一个容量为10的数组，但是无法更改其中的值。

大概就说到这就差不多了吧~ 主要是说一下指针常量而已



啊刚刚老师来提醒我说还可以把const放后面就可以用new了~



```cpp
#include<iostream>
using namespace std;

int main()
{
//	int a[10];
//	const int *p= a;
	int* const a=new int[10];
	a[0] = 1;
	return 0;
}
```
编译器: TDM-GCC 4.8.1 64-bit Release

执行 g++.exe...

g++.exe "D:\Temp\未命名1.cpp" -o "D:\Temp\未命名1.exe" -g3 -I"C:\Program Files (x86)\Dev-Cpp\MinGW64\include" -I"C:\Program Files (x86)\Dev-Cpp\MinGW64\x86_64-w64-mingw32\include" -I"C:\Program Files (x86)\Dev-Cpp\MinGW64\lib\gcc\x86_64-w64-mingw32\4.8.1\include"
 -I"C:\Program Files (x86)\Dev-Cpp\MinGW64\lib\gcc\x86_64-w64-mingw32\4.8.1\include\c++" -L"C:\Program Files (x86)\Dev-Cpp\MinGW64\lib" -L"C:\Program Files (x86)\Dev-Cpp\MinGW64\x86_64-w64-mingw32\lib" -static-libgcc -g3

编译 0.75 秒后成功













