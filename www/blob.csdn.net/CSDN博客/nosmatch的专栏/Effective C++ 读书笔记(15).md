# Effective C++ 读书笔记(15) - nosmatch的专栏 - CSDN博客
2013年07月12日 08:06:50[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：409
个人分类：[读书笔记](https://blog.csdn.net/HDUTigerkin/article/category/1432055)
条款15：在资源管理类中提供对原始资源的访问
> 
前面几个条款主要的将了资源管理类对资源的管理，但是如果需要资源管理类的原始资源的时候该怎么做呢？例如：
```cpp
class Investment{
public:
    bool isTexFree() const;
    ...
};
```
```cpp
std::tr1::shared_ptr<Investment> pinv = createInvestment();
```
此时有个函数调用：
```cpp
void dayHeld(Investment* ph);
```
在shared_ptr和auto_ptr等系统的智能指针资源管理类中，存在一个get()的函数，可以获得对应的原始资源例如：
```cpp
void dayHeld(pinv.get());
```
此时如果直接用pinv是错误的！
此外在几乎所用的智能指针中几乎都对*和->操作符做了重载，例如：
```cpp
bool isTrue = pinv->isTextFree();
bool isFalse = (*pinv).isTextFree();
```
为了兼容性，我们一般在自己的资源管理类中也会定义get()函数来获得对原始资源的调用，例如：
```cpp
class Font{
public:
	explicit Font(FontHandle fn):f(fn){}
	....
	FontHandle get() const{
		return f;
	}
	...
	~Font(){realseFont(f);}
private:
	FontHandle f;
};
```
上面定义的Font类可以看做是FontHandle类的资源管理类，对于需要FontHandle类型的函数调用，我们可以通过Font类的get()
函数获得，与智能指针的用法几乎相同。此外还可以用隐式类型转化来替换get()函数调用，这种用法容易出现问题，建议不要使用
请记住：
- APIs往往需要取得RAII的原始资源，因此对于RAII都要提供一种对原始资源的访问，就像get()函数
- 对原始资源的访问有隐式和显式两种，我们这里只介绍了显式类型转化，对于隐式类型转化应用比较少

> 


