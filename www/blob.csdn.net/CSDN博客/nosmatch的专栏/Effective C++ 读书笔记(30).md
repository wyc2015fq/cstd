# Effective C++ 读书笔记(30) - nosmatch的专栏 - CSDN博客
2013年07月20日 14:53:29[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：677
个人分类：[读书笔记](https://blog.csdn.net/HDUTigerkin/article/category/1432055)
条款30：透彻了解inline函数的里里外外
> 
inline函数是直接将函数调用用函数的本体来替换，这样就免去了函数调用时候的消耗，但是由于每次对于函数的调用都
会用函数的本体来替换，因此过度的应用inline函数会对程序的代码造成膨胀，会造成指令缓冲的命中率，因此我们在应用inline
函数的时候也一定要把握好度！
> 
看下面的例子：
```cpp
class Base{
public:
	Base(){...}
	~Base(){...}
	...
private:
	string strp;
	string strq;
};
class Derive:public Base{
public:
	Derive():Base(){...}
	~Derive(){...}
	string getp(){
		return drivep;
	}
	...
public:
	string drivep
	string driveq;
};
```
在例子中我们对于getp成员函数在类定义的时候定义的，此时函数就是隐式的inline函数，该函数比较短小，适合作为
inline函数，同样的对于Drive类的构造函数中，虽然我们具体的实现代码比较小，但是对于类的构造函数和析构函数，编译器都
替我们做了很多的事情，在这里我们能够看到在Drived的构造函数中会包含Base的构造函数，还有其成员变量的构造函数，包括父
类的，在这里就是四个string的构造函数，此外在构造函数中，还包含一些异常判断的代码，这样其实看起来基本没有代码的构造函
数其实编译器给实现了很多内部的操作，这样构造函数其实就不适合作为inline函数了，对于析构函数也是一样的，因此是否将函数
设置为inline函数需要慎重的考虑的！此外由于inline函数是直接对程序对函数的调用进行替换的，因此不适合程序的调试，不过编译
器可以对inline后的代码进行代码的优化！
请记住：
- 请保证inline函数只用在代码比较少，调用频繁的函数上，这样就能保证程序的优化，还能不至于造成调试的困难，也不会造成代码的膨胀。
- 在template模板中，模板的定义不是inline函数哦
