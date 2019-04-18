# Effective C++读书笔记(33) - nosmatch的专栏 - CSDN博客
2013年07月21日 15:11:28[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：603
条款33：避免遮掩继承而来的名称
> 
对于继承的父类中的函数，如果在子类中有同名的函数，则父类中的同名函数都将被隐藏，例如：
```cpp
class Base{
private:
	int x;
public:
	virtual void mf1() = 0;
	virtual void mf1(int x);
	virtual void mf2();
	void mf3();
	void mf3(doubel);
};
class Derived : public Base{
public:
	virtual void mf1();
	void mf3();
	void mf4();
};
Derived d;
int x;
d.mf1();
d.mf1(x) //错误，父类中的mf1同名的函数被子类中的函数名隐藏
d.mf2(); //调用父类的mf2函数
d.mf3()
d.mf3(x)//错误， 父类中的mf3同名的函数被子类中的函数名隐藏
d.mf4();
```
在上面的例子中，base类中的mf3、mf4和mf1中的所有同名字的函数都被子类中的函数给隐藏了，即使是参数类型不同
的或者virtual和no-virtual函数都是有同样的效果的！为了能够防止父类中的同名函数被隐藏我们可以在子类中用using来引入父类
的成员函数如：
> 
```cpp
class Derived : public Base{
public:
	using Base::mf1;
	using Base::mf3;
	virtual void mf1();
	void mf3();
	void mf4();
};
```
在public继承的体系中，子类不应该要隐藏父类的成员函数，因为这样就与public继承的原理要背驰，public继承就是保证
父类的所有函数在子类中都是相同的！在子类中主动的调用父类的成员函数我们可以通过Base::mf1来实现！
请记住：
- Drived classes内的名称会遮盖带Base class内的名称，在public继承体系中这样做事不合适的！
- 为了让被遮掩的名称那个再见天日，可以使用using声明或者转交函数(即在子类函数中主动调用父类的函数通过Base::mf1)
