# 【C++ STL学习之四】容器list深入学习 - 江南烟雨 - CSDN博客
2012年04月13日 20:46:53[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：4074标签：[list																[c++																[算法																[pointers																[merge																[vector](https://so.csdn.net/so/search/s.do?q=vector&t=blog)](https://so.csdn.net/so/search/s.do?q=merge&t=blog)](https://so.csdn.net/so/search/s.do?q=pointers&t=blog)](https://so.csdn.net/so/search/s.do?q=算法&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)](https://so.csdn.net/so/search/s.do?q=list&t=blog)
个人分类：[C/C++](https://blog.csdn.net/xiajun07061225/article/category/723127)
**一、list基础**
List使用一个双向链表来管理元素。图一显示了list的结构。
![](https://img-my.csdn.net/uploads/201204/13/1334318414_7212.jpg)
图一 list的结构
任何型别只要具备赋值和可拷贝两种性质，就可以作为list的元素。
**二、list的功能**
list的内部结构和vector和deque截然不同，在很多方面都表现不同：
1、list不支持随机存取。所及既不提供下表操作符，也不提供at()函数。
2、任何位置上插入元素删除元素都很快。
3、插入和删除操作不会造成指向其他元素的pointers、references、iterators失效。
4、list对于异常的处理方式是：要么成功，要么什么都不发生。
5、不提供容量、空间重新分配等操作函数。
6、提供了一些函数专门用于移动元素。
**三、list的操作函数**
生成、赋值和销毁与序列式容器相同。
**构造函数和析构函数：**
![](https://img-my.csdn.net/uploads/201204/13/1334318872_3818.jpg)
**非变动性操作**
![](https://img-my.csdn.net/uploads/201204/13/1334318950_1589.jpg)
**赋值**
![](https://img-my.csdn.net/uploads/201204/13/1334318975_6397.jpg)
**元素存取**
![](https://img-my.csdn.net/uploads/201204/13/1334318998_8453.jpg)
一般来说，这些操作不会检查元素是否为空，所以程序员必须保证容器不为空。
**迭代器相关函数**
存取元素只能用迭代器。然而由于list不能随机存取，这些迭代器只是双向而非随机迭代器。所以凡是用到随机存取迭代器的算法（所有用来操作元素顺序的算法-特别是排序算法都归于此类）都不能调用。不过可以用list的成员函数sort()完成此功能。
![](https://img-my.csdn.net/uploads/201204/13/1334319172_1460.jpg)
**元素的插入和删除**
![](https://img-my.csdn.net/uploads/201204/13/1334319233_1663.jpg)
![](https://img-my.csdn.net/uploads/201204/13/1334319248_8916.jpg)
list提供deque的多余功能，还增加了remove()和remove_if()算法应用于list‘身上的特殊版本。这些成员函数比STL的remove算法更快，因为它们只进行内部指目标工作，无需估计元素。
要将所有与某值相等的元素删除，可以用如下语句（注意：确实是将这些元素删除了，而不是像vector那样只是某种移动而已）
```cpp
list<int> list1;
list1.remove(val);
```
list还提供了其他一些成员函数，用来改变元素和区间的次序，或是重新串链。我们可以用他们来移动单一list里的元素，也可以移动两个list之间的元素（前提是list的型别一致）。
![](https://img-my.csdn.net/uploads/201204/13/1334319718_6094.jpg)
**四、list的异常处理**
在所有的STL标准容器中，list提供了最佳支持。几乎所有的操作都是要么正确，要么什么都不发生。只有少数几个操作没有这样的保证：赋值和sort()。不过他们也不会泄露资源，也不会与容器的恒常特性发生冲突。merge(),remove(),remove_if(),unique()提供的保证是有前提的L就是元素间的比较动作不会抛出异常。
**五、list示例程序**
```cpp
#include <iostream>
#include <list>
#include <algorithm>
using namespace std;
void outputLists(const list<int> &l1,const list<int> &l2)
{
	cout << "list1:";
	copy(l1.begin(),l1.end(),ostream_iterator<int>(cout," "));
	cout << endl << "list2:";
	copy(l2.begin(),l2.end(),ostream_iterator<int>(cout," "));
	cout << endl << endl;
}
int main()
{
	list<int> iList1,iList2,iList3;
	for (int i = 1;i < 9;++i)
	{
		iList1.push_back(i);
		iList2.push_front(i);
		iList3.push_front(i);
	}
	outputLists(iList1,iList2);
	iList2.splice(iList2.end(),iList2,iList2.begin());
	outputLists(iList1,iList2);
	iList2.sort();
	outputLists(iList1,iList2);
	iList2.unique();
	outputLists(iList1,iList2);
	iList1.merge(iList2);
	outputLists(iList1,iList2);
	/*outputLists(iList3,iList1);
	iList1.merge(iList3);
	outputLists(iList3,iList1);*/
}
```
运行结果：
![](https://img-my.csdn.net/uploads/201204/13/1334321154_1001.jpg)
注意：merge只使用与合并两个元素已序（而且都是升序）排列的的容器。如上面的程序，最后merge ilist1和ilist3程序就会报错。
