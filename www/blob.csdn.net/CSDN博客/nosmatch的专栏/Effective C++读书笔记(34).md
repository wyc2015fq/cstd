# Effective C++读书笔记(34) - nosmatch的专栏 - CSDN博客
2013年07月21日 16:41:08[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：730
条款34：区分接口继承和实现继承
> 
在对Base class 进行public继承的时候，根据Base class中成员函数的类型我们能够得到不同的继承方式：如下：
```cpp
class Shape{
public:
	virtual void draw() const = 0;
	virtual void error(const std::string& msg);
	int objectId() const;
};
class Rectangle:public Shap{...};
```
在上面的类Shap中，成员函数draw是pure的，因此class 是抽象基类，也就不能有Shap类型的对象，此时draw提供的只
是一个函数的接口，在Rectangle类中必须要对这个接口进行实现，这个是接口的继承，当然在Shap基类中，pure virtual函数也
可以有默认的实现方式，这样我们在其子类Rectangle中就可以使用默认的基类纯虚函数，此时我们必须要显示的进行调用。
> 
对于error的函数，是普通的virtual函数，他对其子类提供的是接口和默认实现的继承，此时如果子类中没有显示的定义一
个则将会使用父类中申明的成员函数。
> 
在objectId成员函数中，我们定义的是普通的成员函数，对于public继承的体系来说我们不应该在子类中对该成员函数进
行重定义，因为这会覆盖掉父类函数的定义！
请记住：
- 接口继承和实现继承不同，在public继承下，derived classes总是继承base class的接口
- pure virtual函数只是具体的指定接口继承
- 普通的virutal 函数具体制定接口继承以及缺省的实现继承
- non-virtual函数具体制定接口继承以及强制性实现继承

