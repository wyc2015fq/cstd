# C++ 类中调用基类带参数构造函数以及常量如何初始化 - 文章 - 伯乐在线
原文出处： [ruanjwei2011，2011-05-24](http://blog.chinaunix.net/uid-25422700-id-335255.html)
有时候，基类构造函数是带有参数，而子类构造函数是不带参数的，如下：


```
class Animal
{
public:
	Animal(int width, int height) { this->width = width; this->height = height; }
private:
	int width, height;
};
class Cat:public Animal
{
public:
	Cat() { cout << "init cat" << endl; }
};
```
在建立Cat类对象时，编译出错：
C:\Documents and Settings\Administrator\桌面\abc\Text1.cpp(104) : error C2512: ‘Animal’ : no appropriate default constructor available
解决这个问题应该在Cat的构造函数中显式调用基类的带参构造函数。因为在基类中定义了带参构造函数，编译器不会提供默认构造函数。
(或者可以在基类中增加一个不带参数的构造函数)这个问题将解决。
下面采用的是调用基类带参构造函数的方式：


```
class Cat:public Animal
{
public:
	Cat():Animal(100,200) { cout << "init cat" << endl; }
};
```
即在构造函数的后面增加一个冒号，后面是基类的构造函数。
这种方式同样可以用来初始化类中的常量。
由于在类定义中不允许对成员进行初始化，常量也不允许。
如下所示：


```
class Cat:public Animal
{
public:
	Cat():Animal(100,200),Age(2),IsLikeFish(true) { cout << "init cat" << endl; }
private:
	const int Age;
	const bool IsLikeFish;
};
```
