# 深入理解C C++异常处理机制

2017-03-25 10:06 阅读 4,520 次 [评论 1 条](https://www.1024do.com/?p=2455#comments)

## 

程序在运行过程中有些错误是可以预料但不可避免的，这时就要力争做到允许用户排除错误，继续运行程序，或者至少给出适当的提示信息、[C](https://www.1024do.com/?tag=c%e8%af%ad%e8%a8%80)/[C++](https://www.1024do.com/?tag=c%e9%9d%a2%e5%90%91%e5%af%b9%e8%b1%a1%e5%bc%80%e5%8f%91)都有各自的异常处理机制。无疑异常处理成为了所有预料的运行错误进行处理的一套实现机制。

## C异常处理机制

异常处理的核心思想是：把功能模块代码与系统中可能出现错误的处理代码分离开来，以此来达到使我们的代码组织起来更美观、逻辑上更清晰，并且同时从根本上来提高我们软件系统长时间稳定运行的可靠性。无论哪种编程语言，都会有属于自己的异常处理机制，当然我们的C语言也不例外。

①终止程序（比如除数为0），这样会导致编译器强制终止我们的程序。

## ![img](https://www.1024do.com/wp-content/uploads/2017/03/57.jpg)

![img](https://www.1024do.com/wp-content/uploads/2017/03/95@4_F9LDKB868GENQB.png)

②返回一个错误的值，附加错误码（比如GetLastError），它显示的是最近一次的程序错误，返回的是系统为我们获取的错误码。

```cpp
int FindFile()
{
	FILE *fp;
	fp = fopen("1.txt", "a+");
	if (fp == NULL)
		cout << GetLastError() << endl;
	return errno;
}
```

③返回一个合法值，让程序处于某种非法的状态，比如大家耳听能熟的atoi。不知道大家是否依稀记得那个坑了很多程序员的atoi呢？

```cpp
int my_atoi(const char *str)
{
	int flag = 1;
	long long ret = 0;
	assert(str);
	while (*str == ' ') //遇到空格就跳过
	{
		str++;
	}
	if (*str == '\0')   //字符串已经遍历完，直接返回0
		return 0;
	if (*str == '+')		//判断正数
		str++;
	else if (*str == '-') //判断负数
	{
		flag = -1;		 //为负数时，先记录状态
		str++;
	}
	while (*str >= '0' && *str <= '9')	  //将字符数字转换成整型
	{
		ret = ret * 10 + flag*(*str - '0');
		if (ret >= INT_MAX || ret <= INT_MIN)  //如果存在溢出，跳出循环
			break;
		str++;
	}
	//if (*str == '\0')
		//state = NORMAL;
	return (int)ret;
}
int main()
{
	char arr[30] = { 0 };
	int ret = 0;
	scanf("%s", arr);
	ret = my_atoi(arr);
	//if (state == UNNORMAL)
		//printf("异常返回\n");
	printf("%d\n", ret);
	system("pause");
	return 0;
}
```

![img](https://www.1024do.com/wp-content/uploads/2017/03/atoi1.png)

很容易发现，两种输入是同一个返回值，这明显违背了atoi函数所要表达的功能，程序通过一个合法的0值，却让程序处于了某种不安全的状态。因此附加一个状态位来区分是很重要的，对于atoi不解的可以看一下我之前的博客<https://www.1024do.com/?p=1521>。

④调用一个预先准备好在出现"错误"的情况下用的函数，比如回调函数。顺便为大家科普一下回调函数：回调函数就是一个通过函数指针调用的函数。如果你把函数的指针（地址）作为参数传递给另一个函数，当这个指针被用来调用其所指向的函数时，我们就说这是回调函数。回调函数不是由该函数的实现方直接调用，而是在特定的事件或条件发生时由另外的一方调用的，用于对该事件或条件进行响应。

⑤简单粗暴的方法：abort()或者exit。1）exit会做一些释放工作：释放所有的静态的全局的对象，缓存，关掉所有的I/O通道，然后终止程序。直接使用就可以强制使程序退出。2）abort：立刻terminate程序，没有任何清理工作。注意：如果是用c++的话，exit调用的时候，对象不会被正确析构的，所以在exit前一定要释放应该释放的资源。

⑥基于goto语句的异常处理，通过goto的都知道，是一把十足的双刃剑，但是goto还有一个重要的作用：处理异常，当程序出现不当操作时，可以直接跳转到指定的模块。也就是说它可以将错误处理模块的代码与其他代码分离开来。但是goto所带来的弊端也是不容忽视的，具体有下面几点：1）goto语句破坏了程序的结构化设计，使代码难以测试，缺乏易读性 2）只能在同一作用域跳转，限制较大 3） 如果在C++中使用goto，就会给面向对象造成极大破坏，影响程序的效率。

⑦setjmp和longjmp的组合。setjmp() 为跳转返回保存现场并为异常提供处理程序，longjmp() 则进行跳转（抛出异常），setjmp() 与 longjmp() 可以在函数间进行跳转（类似于一个跨作用域的goto）。下面看一个程序：

```cpp
#include <iostream>
#include <setjmp.h>
using namespace std;
jmp_buf mark;
void FunTest1()
{
	longjmp(mark, 1);
}
void FunTest2()
{
	longjmp(mark, 2);
}
void FunTest3()
{
	longjmp(mark, 3);
}
int main()
{
	int iState = setjmp(mark);
	if (0 == iState)
	{
		FunTest1();
		FunTest2();
		FunTest3();
	}
	else
	{
		switch (iState)
		{
		case 1:
			cout << "FunTest1() error" << endl;
			break;
		case 2:
			cout << "FunTest2() error" << endl;
			break;
		case 3:
			cout << "FunTest3() error" << endl;
			break;
		}
	}
	cout << "Main end" << endl;
	system("pause");
	return 0;
}
```

![img](https://www.1024do.com/wp-content/uploads/2017/03/setjmp.png)

[注意]：①setjmp必须先调用，在异常位置通过longjmp以恢复先前保存的程序执行点，否则将导致不可预测的结果，甚至程序奔溃。②在调用setjmp之前的函数返回之前调用longjmp，否则结果不可预料。

[setjmp()与longjmp()存在的缺陷]： ① 函数的使用者必须非常靠近函数调用的地方编写错误处理代码，无疑使代码变的臃肿笨拙。② setjmp()和longjmp()并不能够很好的支持C++面向对象的语义。

## C++异常机制

C++在处理异常时，如果在执行一个函数时出现异常，可以不再本函数中立即处理，而是发出一个信息，传给它的上一级解决，若上一级不能处理，就再传给上一级，若到了最高层还未处理，运行系统一般会自动调用系统函数terminate（），由它调用abort（）终止程序。

## 异常的抛出(throw)与捕获(catch)

1）C++处理异常的机制是由检查、抛出和捕获3部分组成，分别有3种语句来完成，分别是try（检查）、throw（抛出）和catch（捕获）。比如下面这个程序：

```cpp
void FunTest()
{
	int *p = (int*)malloc(0x7fffffff);
	try//检查异常
	{
		if (NULL == p)
		{
			throw 1;
		}
	}
	catch (int err)//捕获异常
	{
		cout << "Error(int)::FunTest()" << endl;
	}
}
```

![img](https://www.1024do.com/wp-content/uploads/2017/03/ph1.png)

查看错误码可以看出，显示存储空间不足，因为32位计算机仅仅有4g内存空间，出错也是意料之中。在上例中我们在try的块中进行错误检查，你可以把你认为有问题的语句全部放入try中检查，catch语句是一个捕获异常的语句，那么catch捕获的依据是什么呢？看下面这个程序：

```cpp
void FunTest()
{
	int *p = (int*)malloc(0x7fffffff);
	try//检查异常
	{
		if (NULL == p)
		{
			throw 1;
		}
	}
	catch (char err)//捕获异常
	{
		cout << "Error(int)::FunTest()" << endl;
	}
}
```

沿用上例的程序，只需要将catch中的类型换成char。那么现在程序会如何运行呢？是否还会一如既往？

![img](https://www.1024do.com/wp-content/uploads/2017/03/ph2.png)

看样子catch捕获异常并不是平白无故的，万物皆因果，为了以防万一，我们catch上两种类型。

```cpp
void FunTest()
{
	int *p = (int*)malloc(0x7fffffff);
	try//检查异常
	{
		if (NULL == p)
		{
			throw 1;
		}
	}
	catch (char err)//捕获char异常
	{
		cout << "Error(char)::FunTest()" << endl;
	}
	catch (int err)//捕获int异常
	{
		cout << "Error(int)::FunTest()" << endl;
	}
}

```

![img](https://www.1024do.com/wp-content/uploads/2017/03/ph3.png)

你会发现，当我们抛出int型异常时，catch捕获的是与之对应的int型异常,即使char放在int之前。

[总结]：异常是通过抛出对象引发的，该对象的类型决定了应该激活哪个处理代码，这时编译器自动执行的。

2）如果同时抛出多个异常，那么编译器会如何捕获呢？请看下面这个程序：

```cpp
void FunTest1()
{
	int err = 1;
	throw err;
}
void FunTest2()
{
	char a = 'b';
	throw a;
}
void FunTest()
{	
	FunTest1();
	FunTest2();
}
int main()
{
	try
	{
		FunTest();
	}
	catch (const int err)
	{
		cout << "int:FunTest1()" << endl;
	}
	catch (const char err)
	{
		cout << "char:FunTest2()" << endl;
	}
	system("pause");
	return 0;
}

```

![img](https://www.1024do.com/wp-content/uploads/2017/03/yh4.png)

[总结]：当抛出多个异常且catch捕获中有多个对应类型匹配时，被选中的处理代码会调用链中离抛出异常位置最近的那一个异常。

3）异常抛出之后，是否会释放内存空间，能否避免内存泄漏，下面这个程序可以较好的说明：

```
void FunTest()
{
	try
	{
		char a = 'b';
		cout << &a << endl;
		throw a;
	}
	catch (char a)
	{
		cout << &a << endl;
	}
}
```

![img](https://www.1024do.com/wp-content/uploads/2017/03/yh5.png)

[总结]：抛出异常后会释放局部存储对象，所以被抛出的对象也就归还给系统了，throw表达式会初始化一个抛出特殊的异常对象副本，即匿名对象，异常对象由编译器管理，异常对象在传给对象的catch处理之后撤销。

当然，当有多个异常同时出现时，就需要用到catch（...），它可以捕获多种类型的异常对象，

```
catch(int err)
	{
		;
	}
	catch(double& err)
	{
		cout<<err<<endl;
	}
	catch(...)
	{
		cout<<"未知异常"<<endl;
	}
```



## 栈展开

抛出异常的时候，将暂停当前函数的执行，开始查找对应的匹配catch子句。 首先检查throw本身是否在try块内部，如果是再查找匹配的catch语句。 如果有匹配的，则处理。没有则退出当前函数栈，继续在调用函数的栈中进行查找。 不断重复上述过程。若到达main函数的栈，依旧没有匹配的，则终止程序。

那么下面就用一个简单的调用程序看一下抛出的异常是如何在调用中捕获的：

```cpp
#include <iostream>
using namespace std;
void FunTest1()
{
	try
	{
		char err = 'a';
		throw err;
	}
	catch (int err)
	{
		cout << "FunTest1():int(err)" << endl;
	}
}
void FunTest()
{
	try
	{
		FunTest1();
	}
	catch (int err)
	{
		cout << "FunTest():int(err)" << endl;
	}
}
int main()
{
	try
	{
		FunTest();
	}
	catch (char err)
	{
		cout << "main():char(err)" << endl;
	}
	system("pause");
	return 0;
}
```

![img](https://www.1024do.com/wp-content/uploads/2017/03/zzkn.png)

## 异常捕获的匹配规则

在查找匹配的catch期间，找到的是第一个可以处理该异常的catch。异常的类型与catch说明符的类型必须完全匹配，只有以下几种情况是例外的。
① 允许从非const对象到const的转换。
② 允许从派生类类型到基类类型的转换。
③ 将数组转换为指向数组类型的指针，将函数转换为指向函数类型的指针

```cpp
#include <iostream>
using namespace std;
void FunTest1()
{
	int err = 1;
	throw err;
}
class B
{};
class C :public B
{};
 
void FunTest2()
{
	throw C();
}
 
void FunTest3()
{
	int array[10];
	throw array;
}
 
void FunTest5()
{
	cout << "FunTest5()" << endl;
}
 
void FunTest4()
{
	throw FunTest5;
}
 
int main()
{
	try
	{
		FunTest4();
		FunTest3();
		FunTest2();
		FunTest1();
	}
	catch (const int& err)
	{
		cout << err << endl;
	}
	catch (B& err)
	{
		cout << "B()" << endl;
	}
	catch (int* err)
	{
		cout << "int*" << endl;
	}
	catch (void(*p)())
	{
		p();
	}
	system("pause");
	return 0;
}
```

![img](https://www.1024do.com/wp-content/uploads/2017/03/lxzhc.png)

上面就是4种特殊类型的转换，在记住catch匹配规则的同时，也应该对特殊的转换有所了解。

## 异常的重新抛出

有可能单个的catch不能完全处理一个异常，在进行一些校正处理以后，希望再交给更外层的调用链函数来处理，catch则可以通过重新抛出将异常传递给更上层的函数进行处理。

被重新抛出的异常是原来的异常对象，不是catch形参。该异常类型取决于异常对象的动态类型，而不是catch形参的静态类型。比如来自基类类型形参catch的重新抛出，可能实际抛出的是一个派生类对象。只有当异常说明符是引用时，在catch中对形参的改变，才会传播到重新抛出的异常对象中。

```cpp
#include <iostream>
using namespace std;
void HerFun(int x)
{
	if (x > 10)
		throw 1;
	if (x < 0)
		throw -1;
}
void MyFun(int x)
{
	try
	{
		HerFun(x);
	}
	catch (int x)
	{
		switch (x)
		{
		case 1:
			throw "x > 10";
			break;
		case -1:
			throw "x < 0";
			break;
		}
	}
}
int main()
{
	try
	{
		MyFun(20);
	}
	catch(const char* s)
	{
		cout << "error:" << s << endl;
	}
 
	system("pause");
	return 0;
}
```

![img](https://www.1024do.com/wp-content/uploads/2017/03/cxpcyc.png)

## 异常规范

在函数声明之后，列出该函数可能抛出异常类型，并保证该函数不会抛出其他类型的异常。
① 成员函数在类内声明和类外定义两处必须有相同的异常规范。但是在我们的VS2017下真的是这样吗？大家不妨看看这段代码：

```cpp
#include <iostream>
using namespace std;
class A
{
public:
	void FunTest()throw(int);
};
void A::FunTest()
{
	throw '1';
}
int main()
{
	system("pause");
	return 0;
}

```

![img](https://www.1024do.com/wp-content/uploads/2017/03/lnlw.png)

既然vs下违背了异常规范，那么g++编译器释放还会一蹴而就，我们在linux下跑一遍：

![img](https://www.1024do.com/wp-content/uploads/2017/03/lnlwl.png)

我们发现，在g++编译环境下，是遵循异常规范的，在VS下是违背异常规范的。
② 函数抛出一个没有被列在它异常规范中的异常时(且函数中抛出异常没有在函数内部进行处理)，系统调用C++标准库中定义的函数unexpected( )。
③ 如果异常规范为throw(),则表示不抛出任何异常，该函数不用放在try块中。
④派生类的虚函数的异常规范必须与基类虚函数的异常规范一样或更严格(是基类虚函数的异常的子集)。因为派生类的虚函数被指向基类类型的指针调用时，保证不会违背基类成员函数的异常规范。

```cpp
#include <iostream>
using namespace std;
class A
{
public:
	virtual void FunTest()throw(int);
};
 
class B :public A
{
public:
	virtual void FunTest()throw(int, double)
	{
		A::FunTest();
		throw '1';
	}
};
int main()
{
	return 0;
}
```

用虚函数来构成重写，这样的目的在于派生类与基类的异常规范一致，派生类虚函数是基类虚函数的一个子集。

## 异常之构造函数与析构函数

① 构造函数完成对象的构造和初始化，需要保证不要在构造函数中抛出异常，否则可能导致对象不完整或没有完全初始化。

```cpp
#include <iostream>  
using namespace std;
class A 
{
public:
	A(const int& a) 
		:_a(a)
	{
		throw exception("A:test");  
		cout << "A() " << endl;
	};
	~A()
	{
		cout << "~A() " << endl;
	};
private:
	int _a;
};
 
int main()
{
	try
	{
		A x(10);
	}
	catch (exception c)
	{
		cout << c.what() << endl;
	};
	system("pause");
	return 0;
}
```

![img](https://www.1024do.com/wp-content/uploads/2017/03/gzyc.png)

由运行结果可以看出，析构函数并没有执行，而且构造函数构造的对象也没有输出。原因就在于：在构造函数处抛出的异常，异常点之后的语句都不再执行。

但是，在知乎上看到陈硕的评论，也是很有道理：

![img](https://www.1024do.com/wp-content/uploads/2017/09/%E6%9E%84%E9%80%A0.png)

② 析构函数主要完成资源的清理，需要保证不要在析构函数内抛出异常，否则可能导致资源泄漏 （内存泄漏、句柄未关闭等）。

当你的析构函数不得已抛出异常时，就必须要把可能出现的异常全部捕获并且完全封装在析构函数内部，抛出函数也就意味着内存泄漏，程序奔溃的严重问题。

```
~A()
{
	try 
	{
       do_something();
 
	}
	catch(...)
	{  
		捕获所有可能出现的异常
	}
}
```

当然，这样的方法只是一时的解决了问题，但是如果我们的程序要抛出异常也顺利运行时，就必须用到智能指针了，后面我也会写一篇关于智能指针的博客。




  