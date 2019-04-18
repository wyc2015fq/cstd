# 【C++基础学习】C++中union结构 - 江南烟雨 - CSDN博客
2012年02月26日 13:54:03[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：13947
C++ union结构式一种特殊的类。它能够包含访问权限、成员变量、成员函数（可以包含构造函数和析构函数）。它不能包含虚函数和静态数据变量。它也不能被用作其他类的基类，它本身也不能有从某个基类派生而来。Union中得默认访问权限是public。
在C/C++程序的编写中，当多个基本数据类型或复合数据结构要占用同一片内存时，我们要使用联合体；当多种类型，多个对象，多个事物只取其一时（我们姑且通俗地称其为“n 选1”），我们也可以使用联合体来发挥其长处。
在某一时刻，一个union中只能有一个值是有效的。
 union的内存分配
先看一段代码：
```cpp
#include <iostream>
using namespace std;
union Test{
	struct{
		int x;
		int y;
		int z;
	}s;
		int k;
}myUnion;
int main()
{
	myUnion.s.x = 4;
	myUnion.s.y = 5;
	myUnion.s.z = 6;
	myUnion.k = 0;
	cout<< myUnion.s.x <<endl;
	cout<< myUnion.s.y <<endl;
	cout<< myUnion.s.z <<endl;
	cout<< myUnion.k <<endl;
}
```
运行=结果：
![](http://hi.csdn.net/attachment/201202/26/0_1330234211bww8.gif)
union类型是共享内存的，以size最大的结构作为自己的大小。每个数据成员在内存中得其实地址是相同的。这样的话，myun这个结构就包含u这个结构体，而大小也等于u这个结构体的大小，在内存中的排列为声明的顺序x,y,z从低到高，然后赋值的时候，在内存中，就是x的位置放置4，y的位置放置5，z的位置放置6，现在对k赋值，对k的赋值因为是union，要共享内存，所以从union的首地址开始放置，首地址开始的位置其实是x的位置，这样原来内存中x的位置就被k所赋的值代替了，就变为0了，这个时候要进行打印，就直接看内存里就行了，x的位置也就是k的位置是0，而y，z的位置的值没有改变。
union的一个用法就是可以用来测试CPU是大端模式还是小端模式：
```cpp
#include <iostream>
using namespace std;
void checkCPU()
{
	union MyUnion{
		int a;
		char c;
	}test;
	test.a = 1;
	if (test.c == 1)
		cout << "little endian" <<endl;
	else cout << "big endian" <<endl;
}
int main()
{
	checkCPU();
	return 0;
}
```
因为是共享内存的，所以通过设置char型的值是1，判断int型成员的值是否是1即可判断CPU的模式了。
![](http://hi.csdn.net/attachment/201202/26/0_1330237106JVCC.gif)
union和struct的区别
在struct类型中，所有数据成员是存储在相邻的内存中的。一个struct对象的大小事其所有数据成员的大小总和。
它有如下特性：
能够用构造函数来初始化变量。
能够有析构函数。
构造函数不能为虚函数。
成员变量默认是public权限。
struct不同成员的赋值是不会相互影响的。这点和union不同。
