# 【C++ STL学习之六】STL算法之for_each - 江南烟雨 - CSDN博客
2012年04月23日 20:25:47[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：2868标签：[each																[算法																[c++																[function																[class																[iterator](https://so.csdn.net/so/search/s.do?q=iterator&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)](https://so.csdn.net/so/search/s.do?q=算法&t=blog)](https://so.csdn.net/so/search/s.do?q=each&t=blog)
个人分类：[C/C++](https://blog.csdn.net/xiajun07061225/article/category/723127)
for_each()函数将调用者提供的操作施加于每一个元素身上。它既可以是非变动性算法，也可以说是变动性算法。
```cpp
template <class InputIterator, class Function>
   Function for_each (InputIterator first, InputIterator last, Function f);
```
将函数f施加于区间[first,last)的每一个元素身上。其实现：
```cpp
template<class InputIterator, class Function>
  Function for_each(InputIterator first, InputIterator last, Function f)
  {
    for ( ; first!=last; ++first ) f(*first);
    return f;
  }
```
它返回f已在算法内部变动过的一个副本。
f可以是普通函数，也可是仿函数。它的任何返回值都将被忽略。
程序实例：
下面的例子实现了两个功能：
一是使用普通函数print()打印所以元素；而是使用自定义的仿函数来改变每个元素：将每个元素乘以3.
main.cpp:
```cpp
#include "algostuff.h"
using namespace std;
void print(int elem)
{
	cout << elem << " ";
}
//define a functor
//multiply every element with the value initialized
template <class T>
class MultiplyValue{
private:
	T value;
public:
	MultiplyValue(const T val):value(val){}
	//the function call
	void operator()(T &elem)
	{
		elem *= value;
	}
};
int main()
{
	vector<int> intVec;
	INSERT_ELEMENTS(intVec,1,9);
	cout << "elements : " << endl;
	for_each(intVec.begin(),intVec.end(),print);
	cout << endl << endl;
	for_each(intVec.begin(),intVec.end(),MultiplyValue<int>(3));
	PRINT_ELEMNTS(intVec,"after multiply : ");
	cout << endl;
}
```
algostuff.h：
```cpp
#ifndef ALGOSTUFF_H
#define ALGOSTUFF_H
#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <set>
#include <map>
#include <string>
#include <algorithm>
#include <functional>
#include <numeric>
//print all the elements
template <class T>
inline void PRINT_ELEMNTS(const T &col,const char *optcstr = " ")
{
	typename T::const_iterator pos;
	cout << optcstr;
	for(pos = col.begin();pos != col.end();++pos)
		cout << *pos << " ";
	cout << endl;
}
//insert values from first to last into the collection
template <class T>
inline void INSERT_ELEMENTS(T &col,int first,int last)
{
	for(int i = first;i <= last;++i)
		col.insert(col.end(),i);
}
#endif
```
运行结果：
![](https://img-my.csdn.net/uploads/201204/23/1335184125_4844.jpg)
