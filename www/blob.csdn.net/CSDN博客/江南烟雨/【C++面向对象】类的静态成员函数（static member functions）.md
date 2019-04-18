# 【C++面向对象】类的静态成员函数（static member functions） - 江南烟雨 - CSDN博客
2013年04月10日 15:13:03[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：2787
## 一、静态成员函数的引入
在引入静态成员函数之前，C++语言要求所有的成员函数都必须经由该类的对象来调用。而实际上，只有当成员函数中有存取非静态数据成员时才需要类对象。类对象提供this指针给这种函数使用。这个this指针把“在成员函数中存取的非静态数据成员”绑定于“对象内对应的成员”身上。如果没有任何一个成员被直接存取，事实上就不需要this指针，因此也就不需要通过一个类对象来调用一个成员函数。不过C++语言到现在为止还不能识别这种情况。
这样在存取静态数据成员时产生了一些不规则性。如果把静态数据成员声明为nonpublic，就必须同成员函数来存取它。虽然可以不靠类对象来存取static member，但是存取函数必须绑定于类对象上。
看下面代码：
```cpp
#include <iostream>
using namespace std;
class Test{
public:
	Test():val(10){};
	static int object_count_s();
	int object_count();
private:
	int val;
	static int sta_val;
};
int Test::sta_val = 20;
int Test::object_count()
{
	//val = 10;
	cout << "nonstatic function is revoked!" << endl;
	cout << "sta_val = " << sta_val << endl;
	return 1;
}
Test getObj()
{
	Test TestObj;
	cout << "function getObj is revoked" << endl;
	return TestObj;
}
int main()
{
	((Test*)0)->object_count();
	return 0;
}
```
运行结果：
![](https://img-my.csdn.net/uploads/201304/11/1365646263_9684.jpg)
代码中函数object_count只引用了类的静态成员，本来可以不需要与类对象进行绑定。但是由于引入静态成员函数之前C++语言要求所有的成员函数必须经由类对象来调用，因此将0强制转化为一个class指针提供this指针实体。
但是某些时候我们需要独立于类对象之外的存取操作。程序上的解决方法是，将0强制转化为一个class指针，因而提供一个this指针。
语言上的解决方法是：引入静态成员函数。
## 二、静态成员函数的特性
静态成员函数的主要特性是没有this指针。它因此有以下特性：
（1）不能够直接存取类中的nonstatic members。
（2）不能被声明为const、volatile或virtual。
（3）不需要经过类对象才被调用。
“member selection”语法的使用时一种符号上的遍历，它会被转化为一个直接调用操作：
if(Test::object_count_s()>1)...
如果类对象是由某个表达式获得的，表达式仍然会被计算：
如下测试代码：
```cpp
#include <iostream>
using namespace std;
class Test{
public:
	Test():val(10){};
	static int object_count_s();
	int object_count();
private:
	int val;
	static int sta_val;
};
int Test::sta_val = 20;
inline int Test::object_count_s()
{
	cout << "static function is revoked!" << endl;
	return 1;
}
int Test::object_count()
{
	return 1;
}
Test getObj()
{
	Test TestObj;
	cout << "function getObj is revoked" << endl;
	return TestObj;
}
int main()
{
	cout << typeid(&Test::object_count).name() << endl;
	cout << typeid(&Test::object_count_s).name() << endl;
	((Test*)0)->object_count_s();
	getObj().object_count_s();
	return 0;
}
```
运行结果：
![](https://img-my.csdn.net/uploads/201304/10/1365578108_3217.jpg)
可以发现表达式仍然被计算了。
如果去一个静态成员函数的地址，获得的将是其在内存中的位置。而且由于其没有this指针，其地址类型不是指向类成员函数的指针，而是一个非成员函数指针。上述代码给出了测试结果。
