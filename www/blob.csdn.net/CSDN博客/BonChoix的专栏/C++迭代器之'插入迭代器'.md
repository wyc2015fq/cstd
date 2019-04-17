# C++迭代器之'插入迭代器' - BonChoix的专栏 - CSDN博客





2012年10月11日 23:00:10[Brother灬Nam](https://me.csdn.net/BonChoix)阅读数：6724








       插入迭代器(Insert Iterator)，又叫插入器(Inserter)，是继上次的反向迭代器之后C++中的又一个迭代器适配器。插入迭代器的主要功能为把一个赋值操作转换为把相应的值插入容器的操作。插入迭代器对标准算法库而言尤其重要。算法库对所有在容器上的操作有个承诺：决不修改容器的大小（不插入、不删除）。有了插入迭代器，既使得算法库可以通过迭代器对容器插入新的元素，又不违反这一承诺，即保持了设计上的一致性。

       插入迭代器提供了以下几种操作：*itr，itr++，++itr，itr = value。但实际上，前三种操作为“空操作”(no-op)，仅仅返回itr。第四种操作itr = value才是插入迭代器的核心，这个操作通过调用容器的成员函数（push_back()，push_front()，insert()，取决于插入器类型）把value插入到插入器对应容器的相应的位置上。

       插入迭代器分为三种类型：尾部插入器(back_insert_iterator)，首部插入器(front_insert_iterator)和普通插入器(insert_iterator)。第一种通过调用容器的push_back成员函数来插入元素，因此这种插入器只对vector,list,deque和string有效。 第二种通过调用容器的push_front成员函数来插入元素，因此它只对list和deque有效。第三种通过调用insert成员函数来插入元素，并由用户指定插入位置，它对所有标准的容器类型都有效，因为所有容器都定义了insert成员函数。对于普通的插入器，itr
 = value 等效于如下操作：

```cpp
pos = container.insert(pos,value);
++pos;
```

即在指定位置插入元素，同时更新下次插入位置。

       对应于这三种插入迭代器，标准库分别提供了三个包装函数（模板），分别为back_inserter(), front_inserter()和inserter()，返回类型为相应的插入迭代器。下面通过一个简单的例子来说明插入迭代器的基本使用：

```cpp
#include <iostream>
#include <vector>
#include <list>
#include <iterator>

using namespace std;

template<typename T>
void PrintElements(T c)
{
	T::const_iterator itr = c.begin();
	while(itr != c.end())
	{
		cout<<*itr++<<" ";
	}
}

int main()
{
	vector<int> vecSrc;
	list<int> vecDest;

	for(vector<int>::size_type i=0; i<3; ++i)
	{
		vecSrc.push_back(i);
	}

	copy(vecSrc.begin(),vecSrc.end(),back_insert_iterator<list<int> >(vecDest));
	PrintElements(vecDest);
	cout<<endl;

	copy(vecSrc.begin(),vecSrc.end(),front_insert_iterator<list<int> >(vecDest));
	PrintElements(vecDest);
	cout<<endl;

	copy(vecSrc.begin(),vecSrc.end(),insert_iterator<list<int> >(vecDest,++vecDest.begin()));
	PrintElements(vecDest);

	return 0;
}
```

这个例子中，vecSrc一开始内容为[0, 1, 2]，然后通过copy函数复制到vecDest容器中。copy函数通过前两个迭代器来指定源数据的范围，通过第三个迭代器来指定目的地的起始处。调用copy函数一定要保证目的容器从指定位置开始后面有足够的空间来存放源容器中指定范围内的元素，否则将会发生不可预知的后果。这里vecDest初始为空，因此绝不能直接用vecDest.end()作为第三个参数，而插入迭代器则帮我们解决了这个问题。如上所示，第一次copy，每次从vecDest尾部插入元素，打印结果为[0
 1 2]，第二次接着从首部插入，打印结果为[2 1 0 0 1 2]，第三个copy用到普通迭代器，从第二个元素位置处开始插入，即第一个和第二个元素之间(2和1)插入，因此最后结果为[2 0 1 2 1 0 0 1 2]。

当然，back_insert_iterator<list<int> >(vecDest)书写比较麻烦，可以直接用包装函数back_inserter(vecDest)来代替，其他一样。

       站在copy函数的角度来看，它完全不关心传进来的是普通迭代器还是插入迭代器，它要做的仅仅是利用普通迭代器的操作，遍历源数据，并从指定目的地起始处开始一个个赋值，即：

```cpp
*destItr++ = *srcItr;
```

对于插入迭代器来说，*itr和itr++都是空操作(只返回itr)，因此左边等效于itr，因itr = *srcItr则是把源数据插入到目的容器中，这正是插入迭代器的“奇特功能”。



