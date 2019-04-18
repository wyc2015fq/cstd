# 【C++ STL学习之七】STL算法之find和find_if - 江南烟雨 - CSDN博客
2012年04月23日 20:49:52[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：21385
**一、find()算法**
```cpp
template<class InputIterator, class T>
  InputIterator find ( InputIterator first, InputIterator last, const T& value )
  {
    for ( ;first!=last; first++) if ( *first==value ) break;
    return first;
  }
```
返回区间[first,end)中第一个值等于value的元素的位置。
如果没有找到匹配元素，则返回end。
复杂度：线性复杂度。最多比较次数是：元素的总个数。
程序实例：
下面的程序在int类型的vector中搜寻元素5和12，如果搜索到，就返回其位置if欧泽输出提示信息。
main.cpp（头文件algostuff.h和上一篇博客中的相同）：
```cpp
#include "algostuff.h"
using namespace std;
int main()
{
	vector<int> intVec;
	INSERT_ELEMENTS(intVec,1,9);
	vector<int>::iterator pos;
	pos = find(intVec.begin(),intVec.end(),5);
	if(pos != intVec.end())
		cout << "The value 5 exists,and its position is " <<
		distance(intVec.begin(),pos) + 1 << endl;
	else
		cout << "The value 4 not found!" << endl;
	pos = find(intVec.begin(),intVec.end(),12);
	if(pos != intVec.end())
		cout << "The value 12 exists,and its position is " <<
		distance(intVec.begin(),pos) + 1 << endl;
	else
		cout << "The value 12 not found!" << endl;
}
```
运行结果（头文件algostuff.h和上一篇博客中的相同）：
![](https://img-my.csdn.net/uploads/201204/23/1335185138_1981.jpg)
**二、find_if()算法**
```cpp
template<class InputIterator, class Predicate>
  InputIterator find_if ( InputIterator first, InputIterator last, Predicate pred )
  {
    for ( ; first!=last ; first++ ) if ( pred(*first) ) break;
    return first;
  }
```
它在区间[first,end)中搜寻使一元判断式pred为true的第一个元素。
如果没找到，返回end。
程序实例：
下面程序找出第一个能够被3整除的元素，如果找到返回其位置。
main.cpp：
```cpp
#include "algostuff.h"
using namespace std;
int main()
{
	vector<int> intVec;
	INSERT_ELEMENTS(intVec,1,9);
	vector<int>::iterator pos;
	pos = find_if(intVec.begin(),intVec.end(),
		not1(bind2nd(modulus<int>(),3)));
	if(pos != intVec.end())
		cout << "The value divided by 3 exists,and the first value's position is " <<
		distance(intVec.begin(),pos) + 1 << endl;
	else
		cout << "The value divided by 3 not found!" << endl;
}
```
运行结果：
![](https://img-my.csdn.net/uploads/201204/23/1335185584_6613.jpg)
