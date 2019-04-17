# C++基类和派生类的构造析构顺序 - 长歌行 - CSDN博客





2013年01月09日 09:23:19[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：1710








构造的时候先调用基类的构造函数，再调用派生类的构造函数

析构的时候先调用派生类的析构函数，再调用基类的析构函数




看一个例子：



```cpp
#include "stdafx.h"
 class Base
 {
 public:
 	Base()
 	{
 		printf("Base construct!\n");
 	}
 	virtual ~Base()
 	{
 		printf("Base release!\n");
 	}
 };
 
 class Sub : public Base
 {
 public:
 	Sub()
 	{
 		printf("Sub construct!\n");
 	}
 	~Sub()
 	{
 		printf("Sub release!\n");
 	}
 };
 
 class Factory
 {
 public:
 	Base *CreateSub()
 	{
 		return new Sub();
 	}
 };
 
 
 int _tmain(int argc, _TCHAR* argv[])
 {
 	Factory fy;
 	Base *pBase = fy.CreateSub();
 	delete pBase;
 	pBase = NULL;
 	return 0;
 }
```


输出结果：

Base construct!

 Sub construct!

 Sub release!

 Base release!

 请按任意键继续. . .





注意，本例中如果基类析构函数不加virutal，本例中的情况就不会调用派生类的析构函数。

除非把pBase再转为Sub*类型。






