# 【C++ STL学习之二】容器vector大总结 - 江南烟雨 - CSDN博客
2012年04月09日 08:17:26[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：5699
**一、容器vector**
使用vector必须包含头文件<vector>:
#include<vector>
型别vector是一个定义于namespace std内的template：
```cpp
template<class _Ty,
	class _Ax = allocator<_Ty> >
```
第二个参数定义内存模型。我们一般采用默认的内存模型。
**二、vector的功能**
vector模塑出一个动态数组。vector将其元复制到内部的动态数组中。元素之间总是存在某种顺序，它是一种有序群集。支持随即存取。它的迭代器是随机存取迭代器，所以对任何一个STL算法都奏效。
向vector添加一个元素或者删除其中的一个元素，其后的所有元素都要移动位置，每一次移动都要调用赋值操作符。所以，在末端添加或删除元素，性能很好。但是在前段或者中部的话，性能较差。
vector优异性能的秘诀之一是它配置比其所容纳的元素所需更多的内存。我们需要了解**大小**和**容量**的关系。
函数size()可以返回vector的大小，即vector中实际元素的个数。
而capacity()返回容量，是当前的vector所实际能够容纳的元素的数量。它应该总是大于或者等于vector的大小。如果需要向vector中放置比capacity更多的元素，则需要重新配置内部存储器。vector的容量也会随之增长。看下面的示例代码：
```cpp
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;
int main()
{
	vector<string> sentence(1);
	cout << "max_size():" << sentence.max_size() << endl;
	cout << "size():" << sentence.size() << endl;
	cout << "capacity():" << sentence.capacity() << endl;
	sentence.reserve(5);
	sentence.push_back("Hello,");
	sentence.push_back("how ");
	sentence.push_back("are ");
	sentence.push_back("you ");
	sentence.push_back("?");
	copy(sentence.begin(),sentence.end(),
		ostream_iterator<string>(cout," "));
	cout << endl;
	cout << "max_size():" << sentence.max_size() << endl;
	cout << "size():" << sentence.size() << endl;
	cout << "capacity():" << sentence.capacity() << endl;
	swap(sentence[1],sentence[3]);
	sentence.insert(find(sentence.begin(),sentence.end(),"?"),
		"always");
	sentence.back() = "!";
	copy(sentence.begin(),sentence.end(),
		ostream_iterator<string>(cout," "));
	cout << endl;
	cout << "max_size():" << sentence.max_size() << endl;
	cout << "size():" << sentence.size() << endl;
	cout << "capacity():" << sentence.capacity() << endl;
}
```
运行结果：
![](https://img-my.csdn.net/uploads/201204/08/1333892711_6350.jpg)
在程序中，当再次向vector插入元素时，由于vector的容量不够，所以引起了内存的重新分配。但是capacity()的结果与实作版本有关，max_size也是。
vector的容量十分重要，是因为：
1、一旦内存重新配置，与之相关的所有的reference、pointers、iterators都会失效。
2、内存配置很费时。
解决这个问题的方法有：
1、可以使用reserve()保留适当容量，减少重新配置内存的次数。示例代码：
```cpp
vector<string> sentence(1);
sentence.reserve(50);
```
2、在初始化期间向构造函数传递附加参数，构造出足够的空间。
```cpp
vector<T> v(5);
```
当然，这种元素的型别必须提供默认构造函数。但是如果元素的型别比较复杂，初始化操作也很耗时。如果只是为了保留足够的内存，使用方法1较好。
注意：reserve不能缩减vector的容量。由此，我们可以知道，即使删除元素，其reference、pointers、iterators也会继续有效，指向动作发生前的位置。
但是插入操作可能使reference、pointers、iterators失效（因为可能会导致重新配置空间）。
使用swap函数可以缩减vector容量。因为两个vector交换内容后，他们的容量也会互换。
1、
```cpp
template<class T>
void shrinkCapacity(vector<T> &v)
{
	vector<T> tmp(v);
	v.swap(tmp);
}
```
2、
```cpp
vector<T>(v).swap(v);
```
上面两种方法等价。
都是先构造出一个临时vector对象，以v的元素进行初始化，再与v进行交换。需要注意的是：临时对象一般都是精确分配实际所需的内存。所以能够起到减小vector容量的效果。
**三、vector的操作函数**
所有的**构造函数和析构函数**如下：
![](https://img-my.csdn.net/uploads/201204/08/1333893807_3520.jpg)
**非变动性操作：**
![](https://img-my.csdn.net/uploads/201204/08/1333893843_6592.jpg)
**赋值操作：**
![](https://img-my.csdn.net/uploads/201204/08/1333893867_9004.jpg)
上述操作进行的是将新元素赋值给vector,并**将旧元素全部移除**！示例代码：
```cpp
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;
int main()
{
	vector<string> sentence(1);
	cout << "max_size():" << sentence.max_size() << endl;
	cout << "size():" << sentence.size() << endl;
	cout << "capacity():" << sentence.capacity() << endl;
	sentence.reserve(5);
	sentence.push_back("Hello,");
	sentence.push_back("how ");
	sentence.push_back("are ");
	sentence.push_back("you ");
	sentence.push_back("?");
	copy(sentence.begin(),sentence.end(),
		ostream_iterator<string>(cout," "));
	cout << endl;
	sentence.assign(3,"new");
	copy(sentence.begin(),sentence.end(),
		ostream_iterator<string>(cout," "));
	cout << endl;
}
```
运行结果：
![](https://img-my.csdn.net/uploads/201204/08/1333894161_7146.jpg)
可以看出原来的元素全部被删除了。
**元素存取**
![](https://img-my.csdn.net/uploads/201204/08/1333894210_1522.jpg)
在这几个函数中，唯一进行下标检查的是at函数。
因此，在调用operator[]的时候，必须心理清楚索引是否是有效的。
**迭代器相关函数**
![](https://img-my.csdn.net/uploads/201204/08/1333894306_2503.jpg)
迭代器失效的两种情况是：
1、在一个较小的位置上删除或者是移动元素。
2、由于容量的变换引起内存重新分配。
**插入和移除元素**
![](https://img-my.csdn.net/uploads/201204/08/1333894399_3341.jpg)
插入和移除元素，都会使“作用点”之后的各元素的reference、pointers、iterators失效。插入操作还可能引发内存重新分配，那么该容器上的所有的reference、pointers、iterators都会失效。
**四、把vector当做**一般数组使用
现在的C++标准保证vector的元素必须分布于连续空间中。对于vector中的一个合法索引，满足下列表达式：
&v[i] = &v[0] + i;
我们必须保证vector能够容纳所有数据。如果使用的是C-String，记住最后有个'\0'。
只要我们需要一个元素型别为T的数组，就可以采用vector<T>，然后传递第一个元素的地址给它。
注意：千万不要把迭代器当做第一元素的地址来传递。因为vector迭代器是由实作版本定义的，不一定是一个一般指针。
```cpp
printf("%s",v.begin());//ERROR(might work,but not portable)
printf("%s",&v[0]);//OK
```
