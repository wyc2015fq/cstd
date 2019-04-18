# 【C++基础学习】关于C++静态成员函数和变量 - 江南烟雨 - CSDN博客
2011年10月10日 19:09:36[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：3811
代码一：
```cpp
class Point{
public:
	void output()
	{
	}
	static void init()
	{
	}
};
int main()
{
	Point pt;
	pt.init();
	pt.output();
}
```
代码二：
```cpp
class Point{
public:
	void output()
	{
	}
	static void init()
	{
	}
};
int main()
{
	Point::init();
	Point::output();//报错
}
```
两个代码编译运行，代码一成功，代码二报错。错误信息：
test1.cpp(16) : errorC2352: “Point::output”: 非静态成员函数的非法调用。
错误信息提示：非法调用非静态成员函数Point::output。但为什么init函数的调用没有错呢？该函数是一个静态函数。静态成员函数和静态成员变量属于类本身，在类加载的时候，即为他们分配了空间，所以可以通过类名::函数名或类名:变量名来访问。而非静态成员函数和非静态成员变量属于对象的方法和数据，也就是应该首先产生类的对象，然后通过类的对象去引用。
代码三：
```cpp
#include <iostream>
using namespace std;
class Point{
public:
	void output()
	{
	}
	static void init()
	{
		x = 0;
		y = 0;
	}
private:
	int x,y;
};
int main()
{
	Point::init();
	//Point::output();//报错
}
```
运行报错：
1>e:\学习\程序练习\test1\test1\test1.cpp(11): error C2597: 对非静态成员“Point::x”的非法引用
1>e:\学习\程序练习\test1\test1\test1.cpp(12): error C2597: 对非静态成员“Point::y”的非法引用
在静态成员函数中非法引用Point对象的x和y数据成员。因为init是静态函数，不属于某个具体的对象，也就是说在还没有产生Point类的任一个具体的对象时，该函数就已经存在于程序的代码区了。但是，Point类的数据成员x和y还没有分配内存空间，这样，在init函数中对他们进行赋值操作会失败。也就是说，在静态成员函数中不能调用非静态成员，包括非静态成员函数和非静态成员变量。静态成员函数只能访问静态成员变量。
如果把上述代码中对x、y的赋值操作放在非静态函数中，就会成功。
但是，在非静态成员函数中可以调用静态成员函数。
我们可以在上述代码中，将Point类的数据成员x和y的定义之前加上static限定符，使他们成为静态成员变量。
staticintx,y;
编译运行，程序报错。错误信息：
1>test1.obj: error LNK2001: 无法解析的外部符号"private:static int Point::y" (?y@Point@@0HA)
1>test1.obj: error LNK2001: 无法解析的外部符号"private:static int Point::x" (?x@Point@@0HA)
这是链接时发生的错误。出错的原因是对于静态成员变量，必须对他们进行初始化，并且应在类的定义之外进行此操作。我们在Point类的定义之外，加上如下两条初始化语句：
```cpp
intPoint::x = 0;
intPoint::y = 0;
```
则运行成功。
