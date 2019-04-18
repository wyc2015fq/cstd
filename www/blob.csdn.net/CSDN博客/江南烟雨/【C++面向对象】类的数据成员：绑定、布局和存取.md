# 【C++面向对象】类的数据成员：绑定、布局和存取 - 江南烟雨 - CSDN博客
2012年05月19日 21:34:08[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：2585
**一、数据成员的绑定（The binding of a data member）**
先看一段代码：
```cpp
//某个头文件，从某处含入
extern float x;
class Point3d
{
public:
	Point3d(float,float,float);
	float X()
	{
		return x;
	}
	void X(float new_x)
	{
		x = new_x;
	}
private:
	float x,y,z;
};
```
现在的问题是：在类的两个成员函数X中，被返回和设定的x是哪一个呢？是extern声明的x还是类的成员变量x呢？
**在现在的编译器中，一般都指的是类中的成员变量x**。但是在C++最早的编译器中则不是这样做的。因此导致了两种程序设计风格：
（1）把所有的数据成员放在类的声明的起始处。
（2）所有的内联函数都放在类声明之外。
虽然他们的必要性已经自C++2.0之后消失了，但是这种程序设计风格在今天依然存在。这个古来的语言规则被称为“member rewriting rule”，意思是“一个内联函数实体，在整个类声明未被完全看见之前，是不会被评估求值的”。C++标准中的效果是：一个内联函数在类声明之后立即被定义的话，依然对他评估求值，但是是在整个类声明完成之后才开始。
**但是对于成员函数的参数列表则不是这样的。参数列表中的参数还是i会在它们第一次遇到时被决议**。例如：
```cpp
typedef int length;
class Point3d
{
public:
	void mumble(length val)
	{
		_val = val;
	}
	length mumble()
	{
		return _val;
	}
private:
	typedef float length;
	length _val;
};
```
两个函数umber中的length会被决议为int，但是后续出现的length声明则会使先前的操作不合法。
因此，我们最好把嵌套类型声明放在类的起始处。
**二、数据成员的布局（Data member layout）**
先看下面一个类的定义：
```cpp
class Point3d
{
public:
	//...
private:
	float x;
	static List<Point3d*> *freeList;
	float y;
	static const int chunksize = 250;
	float z;
};
```
数据成员的布局情况是：
（1）非静态成员在类对象中的排列顺序和声明顺序一致，任何在其中间声明的静态成员都不会被放进对象布局中。
（2）静态数据成员存放在程序的data segment中，和个别类对象无关。
注：**data segement**（来自维基百科）：
是程序的虚地址空间的一部分，包含全局变量和静态变量，其大小由程序在运行之前程序员所放置的变量决定。
**Program memory**包括三部分：
（1）Data Segment(Data + BSS + Head)l
（2）Stack
（3）Code Segment
**C++标准规定，在同一个访问块即private、public、protected等区段中，成员的排列只需符合较晚出现的成员在类对象中有较高的地址即可。**也就是说，**并不一定要连续排列**。
什么东西可能介于被声明的成员之间的？由于对齐可能需要填补一些字节。
**编译器还会合成一些内部使用的数据成员，以支持整个对象模型。**比如指向虚函数表的指针vptr。vptr的放置位置由编译器决定。传统是放在类的所声明的成员的最后。
**C++标准允许编译器把多个访问块指针的数据成员自由排列，而不必在乎在类声明中的次序。**
**当前的众多编译器都是把一个以上的访问块连锁在一起，依照声明次序形成一个连续区块。而且访问块的多少不会带来额外负担。**
例如在一个访问块中声明4个成员和在4个访问块中分别声明一个成员得到的对象大小相同。
判断一个类中两个成员谁先谁后可用如下代码实现：
```cpp
template<class class_type,
			class data_type1,
			class data_type2>
				char* acess_order(data_type1 class_type::*mem1,
											data_type1 class_type::*mem2)
			{
				assert(mem1 != mem2);
				return mem1 < mem2
					? "mem1 first"
					: "mem2 first";
			}
```
**三、数据成员的存取**
**静态数据成员**
**每一个 静态数据成员只有一个实体，被各个类对象所共享。它被存放在程序的data segment中，并被视为一个全局变量，值在类声明范围之内。**
**注意：每一静态成员的存取许可（private、protected和public）以及与类的关联并不会导致任何空间或执行时间上的额外负担。**
**每次程序对静态成员的调用，都会被内部转化为该唯一的实体的直接操作。所以通过指针或对象来调用静态成员，效果完全相同。**比如
```cpp
Point3d origin,*pt = &origin;
origin.x = 0.0;
pt ->x = 0;
```
**这也是C++中通过指针和通过对象存取数据成员结果完全相同的唯一情况。**
这是因为，静态数据成员并不在类对象之中，存取静态成员不需要通过类对象。通过操作符“.”进行存取只是语法上的一种便宜行事而已。
如果静态数据成员是从一个复杂关系中继承而来的，那么程序中也依然只有一个实体，存取依然是直接的。
如果取一个静态数据成员的地址，会得到一个指向其数据类型的指针，而不是指向其类成员的指针，因为静态成员并不在类对象之中。
比如：
```cpp
#include <iostream>
#include <typeinfo>
using namespace std;
class Point3d
{
public:
	float x;
	static const int chunksize = 250;
	float z;
};
int main()
{
	cout << typeid(&Point3d::chunksize).name() << endl;
	cout << typeid(&Point3d::x).name() << endl;
}
```
程序输出：
![](https://img-my.csdn.net/uploads/201205/20/1337499177_6415.jpg)
如果有两个类，都声明了同一个静态成员比如sta_mem，当它们都被放在程序的data segment时，会导致名称冲突，编译器会进行解决：暗中对每一个静态数据成员编码以获得唯一的程序识别代码。
```cpp
class base1
{
public:
	base1(int x=8):a(x){};//const成员在构造函数初始化
	const int a;
	static int b;
	const static int c=9;//const static成员在类内初始化
};
class base2
{
public:
	base2(int x=8):a(x){};//const成员在构造函数初始化
	const int a;
	static int b;
	const static int c=9;//const static成员在类内初始化
};
int base1::b=9;//static成员在类外初始化，可以修改
int base2::b=10;//static成员在类外初始化，可以修改
int main()
{
	base1 obj1;
	base2 obj2;
	cout<<base1::b<<endl;
	cout<<base2::b<<endl;
}
```
程序会输出9和10.
**非静态数据成员**
存放在每一个类对象中，只有经由显式的（explicit）或者隐式的（implicit）类对象才能存取。
程序员在成员函数中处理非静态数据成员，隐式的类对象就会发生。比如：
```cpp
void Point3d::translate(const Point3d &pt)
{
	x += pt.x;
	y += pt.y;
	z += pt.z;
}
```
事实上x、y、z的存取是经由this指针完成的。其函数参数应该是：
```cpp
void Point3d::translate(Point3d * const this,const Point3d &pt)
{
	this ->x += pt.x;
	this ->y += pt.y;
	this ->z += pt.z;
}
```
每一个非静态数据成员的偏移量在编译时期即可获得，即便这个成员属于一个基类（派生自单一或者多重继承串链）子对象也是一样的。
因此，存取一个非静态数据成员的效率和存取一个C struct成员或者非继承类的成员是相同的。
下面看看虚拟继承。虚拟继承将为经由基类子对象存取类成员导入一层新的间接性。比如
```cpp
Point3d *pt1;
Point3d pt2;
pt1 ->_x = 0;
pt2._x = 0.1;
```
从类指针存取和从类对象存取有什么重大差异？
当类Point3d是一个继承类，在其继承结构中有一个虚拟基类，并且被存取的成员是从该基类继承未来的时候，就会有大的差异。
因为在这个时候我们不知道pt1到底指向那种类类型，也就不知道这个成员的真正偏移量。所以存取操作要被延迟至执行期，经由一个额外的导引，才能解决。
但是用类对象来存取则不会导致这样的问题。
