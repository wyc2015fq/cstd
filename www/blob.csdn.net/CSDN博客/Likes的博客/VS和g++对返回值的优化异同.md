# VS和g++对返回值的优化异同 - Likes的博客 - CSDN博客
2019年01月25日 16:24:26[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：20标签：[C++																[编译器优化																[g++																[VS](https://so.csdn.net/so/search/s.do?q=VS&t=blog)](https://so.csdn.net/so/search/s.do?q=g++&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器优化&t=blog)](https://so.csdn.net/so/search/s.do?q=C++&t=blog)
个人分类：[C++](https://blog.csdn.net/songchuwang1868/article/category/7898933)
下面是测试代码：
```cpp
#include <iostream>
using namespace std;
class A
{
public:
	A()
	{
		cout << "A()" << endl;
	}
	A(const A& a)
	{
		cout << "A(const A& a)" << endl;
	}
	A& operator=(const A& a)
	{
		cout << "A& operator=(const A& a)" << endl;
		return *this;
	}
};
A fun()
{
	A a_;
	return a_;
}
int main()
{
	A a=fun();
	cout << "##########" << endl;
	A&& b = fun();
	cout << "##########" << endl;
	A c;
	c = fun();
	return 0;
}
```
在VS中的输出：
![](https://img-blog.csdnimg.cn/20190125151435501.png)
分析：
1、对于A a=fun();
在没有优化的情况下：
- main函数在调用fun前先为临时对象开辟一块内存空间M。
- 之后在fun中调用构造函数构造对象a_
- 调用拷贝构造函数在空间M中构造临时对象
- 调用拷贝构造函数使用临时对象拷贝构造对象a
- 析构临时对象
综上一共调用拷贝构造和构造函数共三次
编译器优化后：（RVO）
- main函数在调用fun前为对象a在栈上分配一块内存M，但并没有构造
- 之后在fun中调用构造函数构造对象a_
- 调用拷贝构造函数在空间M中构造临时对象
综上一共调用拷贝构造和构造函数共两次。就是截图所示的情况。
优化结果类似于如下示例代码：
```cpp
void f(m)
{
    A a_;//A()
    在内存m上直接用a_拷贝构造生成m;//A(A& a)
    return;
}
int main()
{
    为对象a分配一块内存m；
    f(m);//把地址传给f函数
    return 0;
}
```
这其实就是大名鼎鼎的RVO（Return Value Optimization）了，推荐看一哈《深度探索C++对象模型》
优化方式主要就是不使用临时对象，直接为对象a分配空间并构造。
2、对于A&& b = fun();
在没有优化和进行优化的情况一样：
- main函数在调用fun前先为临时对象开辟一块内存空间M。
- 之后在fun中调用构造函数构造对象a_
- 调用拷贝构造函数在空间M中构造临时对象
- 使用右值引用接住临时变量。右值引用成功延长了临时变量的生命周期
综上一共调用拷贝构造和构造函数共两次。就是截图所示的情况。
3、对于A c;c = fun();
在没有优化和进行优化的情况一样：
- 调用构造函数构造c对象
- main函数在调用fun前先为临时对象开辟一块内存空间M。
- 之后在fun中调用构造函数构造对象a_
- 调用拷贝构造函数在空间M中构造临时对象
- 调用拷贝赋值函数将临时变量赋值给对象c
- 析构临时对象
综上一共调用拷贝构造、构造函数、拷贝赋值共4次。
接下来做一点改进，加上右值的情况：
```cpp
#include <iostream>
using namespace std;
class A
{
public:
	A()
	{
		cout << "A()" << endl;
	}
	A(const A& a)
	{
		cout << "A(const A& a)" << endl;
	}
	A& operator=(const A& a)
	{
		cout << "A& operator=(const A& a)" << endl;
		return *this;
	}
	A(const A&& a)
	{
		cout << "A(const A&& a)" << endl;
	}
	A& operator=(const A&& a)
	{
		cout << "A& operator=(const A&& a)" << endl;
		return *this;
	}
};
A fun()
{
	A a_;
	return a_;
}
int main()
{
	A a=fun();
	cout << "##########" << endl;
	A&& b = fun();
	cout << "##########" << endl;
	A c;
	c = fun();
	return 0;
}
```
![](https://img-blog.csdnimg.cn/20190125154257571.png)
所有的拷贝构造和赋值有调用的右值函数。
原因在于VS编译器在做优化时会将所有的返回值改成return move(result);的形式，就是会先适配右值函数，若右值函数没有才会匹配其他函数，右值引用本身是左值，可以顺利传给参数要求是左值的函数。这也是第一次试验没有右值函数时，全部通过左值函数完成任务的原因。
VS编译器为什么要这么做呢？因为我们一般实现的右值函数的效率会更高（不然也不用费时费力地去多实现一个函数，复用左值的函数即可）
总体给人感觉VS的优化已经不错了，但g++的优化更为出色，同样的代码在g++中跑，结果如下：
![](https://img-blog.csdnimg.cn/20190125155527712.png)
每种情况都少了一次拷贝构造！？
因为编译器发现可以改成这样子：
```cpp
A fun()
{
    return A();
}
```
直接构造临时变量，而不是先构造局部变量，再用局部变量拷贝构造临时变量。

