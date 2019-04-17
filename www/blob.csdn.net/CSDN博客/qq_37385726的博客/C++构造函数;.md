# C++构造函数; - qq_37385726的博客 - CSDN博客





2017年04月28日 23:06:51[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：163








**C++中构造函数的分类:**

（===构造函数是用来初始化数据成员的，当然不包括静态数据成员）

（===无返回值）

（是当对象在创建的时候系统会自动调用构造函数去初始化对象，但是如果构造函数有参数就需要在定义的时候加一个参数表）

1.默认构造函数：类名（）；//即使自己不定义默认构造函数，系统也会自动定义一个默认构造函数，只不过此时对于数据成员的初始化是随机分配的数

2.含参构造函数：类名（参数表）；

3.拷贝构造函数：类名（const 类名 &obj）；  //const可以省略

4.重载构造函数：就是在一个类里面有多个方法名都是类名，参数表不同的函数（参数表是可以不用参数名的，所以也就是说参数名不同不等于参数表不同）




以下文均以Date类为例

//定义的类的声明头文件




```cpp
#ifndef _WOSHILEIA_H_
#define _WOSHILEIA_H
#include
using namespace std;
class Date
{
public:
	Date();
	Date(int y, int m, int d) :year(y), mon(m), day(d){};
//	Date(const Date &obj);  避免造成歧义
	Date(Date &obj);
	~Date();
	void Getdate(void);
private:
	int year;
	int mon;
	int day;
};
#endif
```

//定义的类的定义（实现）的cpp文件





```cpp
#include"woshileia.h"
Date::Date()
{
	year = 2000;
	mon = 1;
	day = 1;
}
/*
Date::Date(const Date &obj)
{
	year = obj.year;
	mon = obj.mon;
	day = obj.day;
}
*/
Date::Date(Date &obj)
{
	year = obj.year;
	mon = obj.mon;
	day = obj.day;
	obj.year = 1998;
}
Date::~Date()
{
	cout << "正在析构" << year << "年" << mon << "月" << day << "日" << "对象"<
```

//定义的主函数的cpp文件





```cpp
#include"woshileia.h"
#include
int main(void)
{
	
	Date date1;//调用无参构造函数或者叫做默认构造函数
	Date date2(1999, 1, 23);//调用含三参构造函数
	Date date3(date2);//调用拷贝构造函数----因为不管定义的拷贝构造函数有没有const,对象在调用构造函数的时候是没有考虑的，所以会有歧义产生
	Date date4=date1;//调用拷贝构造函数---故把那个有const的代码给注释掉了，仅展现没有const会产生什么样的结果，进而说明为什么拷贝构造函数最好加const
	date1.Getdate();//输出结果为1998年1月1日
	date2.Getdate();//输出结果为1998年1月23日
	date3.Getdate();//输出结果为1999年1月23日
	date4.Getdate();//输出结果为2000年1月1日
	system("pause");
	return 0;//析构函数在执行return的时候自动调用，当然这在VS上面的观看效果是一瞬间的事情
			 //在同一个作用域下，先构建的变量后析构
}
```








**C++中的析构函数**

形式：~类名（）；

//无返回值，无参数；

//即使自己不定义析构函数，系统也会自动定义一个空的析构函数；

//是当对象准备消亡的时候（delete的时候或者在return的时候或者在system("exit"的时候，反正就是在对象要消亡的时候），系统会自动调用该对象的析构函数，而且调用顺序是：在相同作用域内，先创建的对象后析构，后创建的对象先析构。

---------------------------------------------------------------------------------------------------------------------









