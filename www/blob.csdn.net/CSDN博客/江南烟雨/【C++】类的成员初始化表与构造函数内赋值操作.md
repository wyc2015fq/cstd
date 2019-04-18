# 【C++】类的成员初始化表与构造函数内赋值操作 - 江南烟雨 - CSDN博客
2012年08月17日 20:16:56[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：3833
我们常常在初始化类数据成员的时候，对使用初始化表和在构造函数中十分困惑，这二者有什么区别呢？我们应该如何选择呢？
先举个简单的例子，看下面的两段代码：
代码A：
```cpp
inline Account::Account(const char* name,doubl opening_bal)
						::_name(name),
						_balance(opening_bal)
{
	_acc_nmbr = get_unique_acct_nmbr();
}
```
代码B：
```cpp
inline Account::Account(const char* name,doubl opening_bal)
{
	_name = name;
	_acc_nmbr = get_unique_acct_nmbr();
	_balance = opening_bal;
}
```
上述两段代码的区别是：
代码A把对数据成员_name和_balance的初始化放在初始化表中，而代码B是在构造函数中进行赋值操作。
上述两段代码的结果是相同的。在构造函数的结束处，三个成员都含有相同的值，区别是成员初始化表只提供该类数据成员的初始化。
尤其是要注意的是：构造函数中的对成员进行设置值是赋值操作，不是初始化操作，其初始化操作已经在初始化表中进行。
我们可以认为构造函数的执行过程分为两个阶段：隐式或显式初始化阶段，以及一般的计算阶段。计算阶段由构造函数体内的所以语句构成。
初始化阶段可以是显式的或是隐式的，取决于是否存在成员初始化表。隐式初始化阶段按照声明的顺序依次调用所以基类的缺省构造函数，然后是所有成员类对象的缺省构造函数。
例如如下代码：
```cpp
class Account{
public:
private:
	unsigned int _acct_nmbr;
	double _balance;
	string _name;
};
```
```cpp
inline Account::Account()
{
	_name = "";
	_balance = 0.0;
	_acc_nmbr = 0;
}
```
则初始化阶段是隐式的。在构造函数体执行之前，先调用与_name相关联的缺省string构造函数，这意味着把空串赋值给_name是不必要的。
对于类对象，在初始化和赋值直接的区别是巨大的。成员类对象应该总是在成员初始化表中被初始化，而不是在构造函数体内被赋值。缺省Account构造函数更正确的实现如下：
```cpp
inline Account::Account()
:_name(string())
{
	_balance = 0.0;
	_acc_nmbr = 0;
}
```
去掉了不必要的构造函数体里面的赋值操作。
对于缺省构造函数的显式调用也是不必要的，可以如下实现：
```cpp
inline Account::Account()
:_name()
{
	_balance = 0.0;
	_acc_nmbr = 0;
}
```
对于内置类型的数据成员，用成员初始化表和在构造函数体内初始化是相同的吗？不。
除了两个例外，两个在结果和性能上都是等价的。更受欢迎的是用成员初始化表。
两个例外是任何类型的const和引用数据成员。因为他们必须在成员初始化表中被初始化，否则会产生编译时刻错误。
每个成员只能在成员初始化表中出现一次，初始化顺序是由成员在类中被声明的顺序决定的，而不是由名字在初始化表中的顺序决定的。但是在初始化表中出现的成员总是在构造函数体内成员的赋值之前被初始化。
