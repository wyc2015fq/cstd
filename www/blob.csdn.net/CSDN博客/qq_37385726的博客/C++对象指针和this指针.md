# C++对象指针和this指针~ - qq_37385726的博客 - CSDN博客





2017年04月29日 10:40:38[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：125








**①对象指针**

是与对象类型相同的，该对象在栈区分配的内存空间。它的值就是该对象的起始地址。可以使用 指针->去访问该对象的数据成员和成员函数。







```cpp
#ifndef _WOSHILEIA_H_
#define _WOSHILEIA_H
#include
using namespace std;
class CPoint
{
public:
	CPoint(){};
	~CPoint(){};
	void print(void);
	int x;
	int y;
};
#endif
```



```cpp
#include"woshileia.h"
void CPoint::print(void)
{
	cout << "(" << x << "," << y << ")" << endl;
}
```



```cpp
#include"woshileia.h"
#include
int main(void)
{
	CPoint *p = new CPoint[3];	//为含3个对象的CPoint类型的对象数组在栈区分配空间（即p的存储空间），指向CPoint类型的对象数组
	p->print();//指针使用->去访问数据成员或成员函数
    (p+1)->print();//（p+i）使用指针的方式逐个访问对象数组的对象
	(p+2)->print();
	CPoint point[3];//声明了一个对象数组
	point[0].print();//用数组的方式去逐个访问对象
	point[1].print();
	point[2].print();
	delete []p;//释放为对象数组分配的指针
	system("pause");
	return 0;
}
```







**②this指针**
先从一个问题来看：当不同的对象调用同一个成员函数的时候，系统是如何判断所调用的数据成员是属于哪一个对象的呢？

——每一个类的非静态成员函数中都存在一个隐式定义的this指针（自动生成，自动隐藏），用来指向调用该成员函数的对象的地址。

必须显式定义this指针的两种情况：

1.当非静态的成员函数的返回值为调用对象本身的时候  return (*this);

2.当数据成员的名字与参数的名字相同的时候，为了增加程序的可读性应该 this->n=n;




```cpp
#ifndef _WOSHILEIA_H_
#define _WOSHILEIA_H
#include
using namespace std;
class CPoint
{
public:
	CPoint(){};
	CPoint(int x,int y);
	~CPoint(){};
	CPoint SetCPoint(CPoint a, CPoint b);
	int GetX(void);
	int GetY(void);
	void print(void);
private:
	int x;
	int y;
};
#endif
```



```cpp
#include"woshileia.h"
CPoint::CPoint(int x, int y)
{
	this->x = x;
	this->y = y;//这个时候也必须显式定义this指针，因为也符合我们说的必须显式定义的那两种情况之一（参数与数据成员同名）
}
CPoint CPoint::SetCPoint(CPoint a, CPoint b)
{
	this->x = a.x + b.x;//这个就是将this指针的隐式表达方式显化了
	this->y = a.y + b.y;//在对象调用非静态的成员函数的时候使用this指针（对象的地址）去指向该对象的数据成员，告诉系统这个数据成员到底是属于哪个对象的
						//this指针系统会隐式定义，但是这种情况必须显式定义对象指针因为正好符合我们说的那两种情况的其中一种(返回值为类对象本身)
	return (*this);//这里不能使用构造函数，因为构造函数没有返回值
	               //返回值是类，通过这个返回值.成员函数，可以去调用这个对象的成员函数
}
int CPoint::GetX(void)
{
	return (this->x);//将隐式定义显化了，但是其实没必要，不是我们说的必须显化的两种情况
}
int CPoint::GetY(void)
{
	return y;
}
```



```cpp
#include"woshileia.h"
#include
int main(void)
{
	CPoint p1(2, 3);
	CPoint p2(4, 5);
	CPoint p3;
	cout << "x="<
```




这里有一个地方值得注意，this指针是只存在于非静态的成员函数中的，用于对象调用成员函数，成员函数访问数据成员时为了告诉系统是哪个对象在访问它的数据成员的时候才起作用，所以对象直接去访问数据成员跟this指针一点关系都没有，直接加“.”访问符就好了；










