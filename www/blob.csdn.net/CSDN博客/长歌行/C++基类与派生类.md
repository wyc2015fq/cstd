# C++基类与派生类 - 长歌行 - CSDN博客





2013年01月05日 13:32:54[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：799








一、构造

构造函数不能为虚函数

派生类要先调用基类的构造函数，而不能直接覆盖基类的构造函数。

所以在派生类不能再成员初始化列表中初始化基类的成员




见如下例子：



```cpp
class Base
 {
 public:
 	Base(double dNum)
 		: nBase(1)
 		, dBase(dNum)
 	{
 	
 	}
 	int ShowNum(){return nBase;}
 protected:
 	int		nBase;
 	double	dBase;
 private:
 };
 
 
 class Derived : public Base
 {
 
 public:
 	Derived(double dNum)
 		: Base(dNum)
 		, nBase(3)
 	{
 		nBase = 2;
 	}
 protected:
 private:
 };
```


会在,nBase(3)处报错 error C2614: “Derived”: 非法的成员初始化:“nBase”不是基或成员







二、待续......



