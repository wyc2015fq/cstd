# Boost库之bind - 深之JohnChen的专栏 - CSDN博客

2017年05月10日 14:56:32[byxdaz](https://me.csdn.net/byxdaz)阅读数：446


      boost::bind是标准库函数std::bind1st和std::bind2nd的一种泛化形式。其可以支持函数对象、函数、函数指针、成员函数指针，并且绑定任意参数到某个指定值上或者将输入参数传入任意位置。

一般来说boost::bind有两种方式的调用，一种是对自由方法，也取非类方法，一种是对类方法。

对自由方法来说，直接boost::bind(函数名,参数1，参数2，...)

对类方法来说，直接boost::bind(&类名::方法名，类实例指针，参数1，参数2）

这里需要额外注意的问题是，函数对象也可以像自由方法一样被bind，而boost::function也是一种函数对象。

向原始函数 fun绑定一部分参数，我们需要使用_1,
 _2这样的参数来占位.这里的
 _1代表该新函数对象被调用时.实参表的第1个参数，这里的
 _2 代表该新函数对象被调用时.实参表的第2个参数。

举个例子

void test(int a, int b, int c)

boost::bind(test, 1, _1, _2)得到一个函数对象b，当我们调用b(3,4)时，相当于调用test(1,3,4)

boost::bind(test, _2, 3, _1)得到一个函数对象b，当我们调用b(3,4)时，相当于调用test(4,3,3)

```cpp
#include "stdafx.h"
#include <iostream>
#include <boost/function.hpp>
#include<boost/bind.hpp>
using namespace std;

//print
int sum(int a,int b,int c) 
{ 
	int nSum = a + b + c;
	cout << nSum <<endl; 
	return a; 
} 

class Demo{
public:
	Demo(){};
	int sum(int a,int b,int c) 
	{ 
		int nSum = a + b + c;
		cout << nSum <<endl; 
		return a; 
	} 
};

typedef boost::function<int(int a,int b,int c)> FuncPrint;

int main(int argc, char* argv[]) 
{ 
	//外部函数
	boost::bind<int>(sum,_1,_2,_3)(1,2,3);
	boost::bind(sum,_1,_2,_3)(1,2,3);
	boost::bind(sum,_1,_2,100)(1,2);
	FuncPrint f = boost::bind(sum,_1,_2,_3);
	f(1,1,1);

	//类成员函数
	Demo x;
	FuncPrint f2 = bind(&Demo::sum,&x,_1,_2,_3);
	f2(10,20,30);
	//使用ref库包装了对象的引用
	boost::bind<int>(boost::ref(f2),_1,_2,_3)(10,20,30);

	return 0; 
}
```


注意事项：

1、boost::bind最多支持9个参数，如果是类成员函数，就支持8个参数。

2、函数与调用实例的函数参数必须一致。

3、bind一个重载函数会导致错误，因为无法确定到底bind重载函数。

4、bind(f,
 ...)形式和bind<R>(f,...)形式的不当用法.

bind(f, a1,a2, ..., aN)会对f自动进行类型识别，f必须是一个函数或者成员函数指针。当f是函数对象时，大多数编译器将不能工作。

bind<R>(f,a1, a2, ..., aN)支持任意类型的函数对象。虽然在有些编译器上，传入函数或者成员函数指针也能工作，但是不推荐这么做。

5、使用ref库包装了对象的引用，可以让bind存储对象引用的拷贝，从而降低了拷贝的代价。ref库可以包装对象的引用，在传递参数时消除对象拷贝的代价，或者将不可拷贝的对象变为可以拷贝；ref库提供了两个便捷的工厂函数ref()和cref()(const
 ref()),可以通过参数类型很容易推导出构造reference_wrapper对象。一般情况下，泛型算法中的函数对象，传值语义是可行的，但是也有很多特殊情况，作为参数的函数对象拷贝代价过高（具有复杂的内部状态），或者不希望拷贝对象（内部状态不应该被改变），甚至拷贝是不可行的（noncopyable，单件）。

6、调用约定

根据调用约定的不同，不同的平台可能支持几种类型的（成员）函数。例如：

Windows API函数和COM接口成员函数使用__stdcall;

Borland VCL使用__fastcall;

Mac toolbox函数使用pascal。

与__stdcall函数一起使用bind时，在包含<boost/bind.hpp>之前#define the macro BOOST_BIND_ENABLE_STDCALL;

与__stdcall成员函数一起使用bind时，在包含<boost/bind.hpp>之前#define the macro BOOST_MEM_FN_ENABLE_STDCALL;

与__fastcall函数一起使用bind时，在包含<boost/bind.hpp>之前#define the macro BOOST_BIND_ENABLE_ FASTCALL;

与__fastcall成员函数一起使用bind时，在包含<boost/bind.hpp>之前#define the macro BOOST_MEM_FN_ENABLE_ FASTCALL;

与pascal函数一起使用bind时，在包含<boost/bind.hpp>之前#define the macro BOOST_BIND_ENABLE_ PASCAL;

与__cdecl成员函数一起使用bind时，在包含<boost/bind.hpp>之前#define the macro BOOST_MEM_FN_ENABLE_CDECL;

一个比较好的建议是：如果需要使用bind，要么提前在工程选项中定义这些宏，要么通过命令行选项-D定义，要么直接在使用bind的.cc文件中定义。否则如果包含bind.hpp的文件中，发生了在定义这些宏之前including
 bind.hpp，那么可能导致难以发现的错误。


