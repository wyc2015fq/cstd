# Boost库之function - 深之JohnChen的专栏 - CSDN博客

2017年05月10日 14:53:32[byxdaz](https://me.csdn.net/byxdaz)阅读数：463


      Boost.Function库包含了一个类族的函数对象的包装。它的概念很像广义上的回调函数。其有着和函数指针相同的特性但是又包含了一个调用的接口。一个函数指针能够在能以地方被调用或者作为一个回调函数。boost.function能够代替函数指针并提供更大的灵活性。

Boost.Function其语法如下：boost::function<float(intx,
 int y)>f

如果Function未赋予一个函数或者不合法的函数而被调用，则会抛出一个boost::bad_function_call异常。

```cpp
#include "stdafx.h"
#include <iostream>
#include <boost/function.hpp>
#include<boost/bind.hpp>
using namespace std;

//print
int print(int a) 
{ 
	cout << a <<endl; 
	return a; 
} 

class X{
public:
	X(){};
	int print(int a) 
	{ 
		cout << a <<endl; 
		return a; 
	} 
};

typedef boost::function<int(int a)> FuncPrint;

int main(int argc, char* argv[]) 
{ 
	//外部函数
	boost::function<int(int)>f = print;
	f(5);

	//类成员函数，通过bind绑定
	X x;
	FuncPrint f2 = bind(&X::print,&x,_1);
	f2(10);
	//类成员函数，通过bind绑定，直接调用函数
	bind(&X::print,&x,_1)(15);

	//函数赋值不合法时，抛出一个boost::bad_function_call异常
	try
	{
		boost::function<void(char *)> f3;
		f3("");
	}
	catch (boost::bad_function_call &ex)
	{
		cout<<ex.what()<<endl;
	}

	return 0; 
}
```



