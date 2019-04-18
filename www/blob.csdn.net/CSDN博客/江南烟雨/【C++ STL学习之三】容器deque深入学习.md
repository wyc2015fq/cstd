# 【C++ STL学习之三】容器deque深入学习 - 江南烟雨 - CSDN博客
2012年04月10日 08:55:17[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：21289
C++ STL容器deque和vector很类似，也是采用动态数组来管理元素。
使用deque之前需包含头文件：
*#include <deque>*
它是定义在命名空间std内的一个class template：
*template<class _Ty,class _Ax = allocator<_Ty> >class deque;*
第一个template参数用来表示元素型别，第二个可有可无，指定内存模型。一般使用默认的内存模型。
与vector不同的是deque的动态数组首尾都开放，因此能够在首尾进行快速地插入和删除操作。
**deque的逻辑结构：**
![](https://img-my.csdn.net/uploads/201204/09/1333979310_4087.jpg)
**deque的内部结构**
deque是一种优化了的对序列两端元素进行添加和删除操作的基本序列容器。通常由一些独立的区块组成，第一区块朝某方向扩展，最后一个区块朝另一方向扩展。它允许较为快速地随机访问但它不像vector一样把所有对象保存在一个连续的内存块，而是多个连续的内存块。并且在一个映射结构中保存对这些块以及顺序的跟踪。
其内部结构如下图所示：
![](https://img-my.csdn.net/uploads/201204/09/1333979333_4240.jpg)
**deque的特点：**
1、支持随机访问，即支持[]以及at()，但是性能没有vector好。
2、可以在内部进行插入和删除操作，但性能不及list。
**deque和vector的不同之处：**
1、两端都能够快速插入和删除元素。vector只能在尾端进行。
2、deque的元素存取和迭代器操作会稍微慢一些。因为deque的内部结构会多一个间接过程。
3、迭代器是特殊的智能指针，而不是一般指针。它需要在不同的区块之间跳转。
4、deque可以包含更多的元素，其max_size可能更大。因为不止使用一块内存。
5、不支持对容量和内存分配时机的控制。
注意：在除了首尾两端的其他地方插入和删除元素，都将会导致指向deque元素的任何pointers、references、iterators失效。不过，deque的内存重分配优于vector。因为其内部结构显示不需要复制所有元素。
6、deque的内存区块不再被使用时，会被释放。deque的内存大小是可缩减的。不过，是不是这么做以及怎么做由实作版本定义。
**deque和vector相似的特性：**
1、在中间部分插入和删除元素相对较慢，因为所有元素都要被移动。
2、迭代器属于随即存取迭代器。
**最好采用deque的情形：**
1、需要在两端插入和删除元素。
2、无需引用容器内的元素。
3、要求容器释放不再使用的元素。
deque的操作函数
**构造函数和析构函数：**
![](https://img-my.csdn.net/uploads/201204/10/1334017438_1503.jpg)
**非变动性操作：**
![](https://img-my.csdn.net/uploads/201204/10/1334017462_5465.jpg)
**变动性操作：**
![](https://img-my.csdn.net/uploads/201204/10/1334017532_3707.jpg)
![](https://img-my.csdn.net/uploads/201204/10/1334017498_3740.jpg)
deque的各项操作只有一下两点和vector不同：
deque不提供容量操作：capacity()和reverse()。
deque直接提供函数完成首尾元素的插入和删除。
其他均与vector相同。
注意：
1、除了at()函数，其他成员函数都不会检查索引或迭代器是否有效。
2、元素的插入和删除可能会导致内存重新分配。所以任何插入或删除操作都会使所有指向deque元素的pointers、reference、iterators失效。唯一例外的是在首尾插入元素之后，pointers和reference可能仍然有效。
程序示例：
```cpp
#include <iostream>
#include <deque>
#include <string>
#include <algorithm>
using namespace std;
int main()
{
	deque<string> strDeq;
	strDeq.assign(4,string("CHINA"));
	strDeq.push_back("first_string");
	strDeq.push_front("last_string");
	copy(strDeq.begin(),strDeq.end(),
		ostream_iterator<string>(cout," "));
	cout << endl;
	cout << endl;
	for(int i = 0;i < strDeq.size();++i)
		cout << "strDeq[" << i << "] : " << strDeq[i] << endl;
	cout << endl;
	
	for(int i = 0;i < strDeq.size();++i)
		cout << "strDeq.at(" << i << ") : " << strDeq.at(i) << endl;
	cout << endl;
	strDeq.pop_front();
	strDeq.pop_back();
	copy(strDeq.begin(),strDeq.end(),
		ostream_iterator<string>(cout," "));
	cout << endl;
	cout << endl;
	for(int i = 1;i < strDeq.size();++i)
		strDeq[i] = "pre" + strDeq[i];
	copy(strDeq.begin(),strDeq.end(),
		ostream_iterator<string>(cout," "));
	cout << endl;
	cout << endl;
	strDeq.resize(4,"resized string");
	copy(strDeq.begin(),strDeq.end(),
		ostream_iterator<string>(cout," "));
	cout << endl;
	cout << endl;
}
```
运行结果：
![](https://img-my.csdn.net/uploads/201204/10/1334019425_6482.jpg)
