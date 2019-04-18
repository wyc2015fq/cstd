# 【STL学习】优先级队列Priority Queue详解与C++编程实现 - 江南烟雨 - CSDN博客
2013年01月30日 16:43:11[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：16032
## 优先级队列Priority Queue介绍
优先级队列是一个拥有权值观念的queue。它允许在底端添加元素、在顶端去除元素、删除元素。
优先级队列内部的元素并不是按照添加的顺序排列，而是自动依照元素的权值排列。权值最高者排在最前面。
缺省情况下，优先级队列利用一个大顶堆完成。关于堆可以参考：[STL堆详解与编程实现](http://blog.csdn.net/xiajun07061225/article/details/8553808)
优先级队列以底部容器完成其所有工作，具有这种“修改某物接口，形成另一种风貌”这种性质者，成为配接器（adapter）。在STL中优先级队列不被归类为容器，而被归类为容器配接器（container
 adapter）。
## SGI STL中优先级队列定义
定义完整代码：
```cpp
template <class T, class Sequence = vector<T>, 
          class Compare = less<typename Sequence::value_type> > 
class priority_queue { 
public: 
  typedef typename  Sequence::value_type  value_type; 
  typedef typename  Sequence::size_type  size_type; 
  typedef typename  Sequence::reference  reference; 
  typedef typename  Sequence::const_referenceconst_reference; 
protected: 
Sequence c;  //底层容器 
    Compare comp;//元素大小比较标准 
public: 
priority_queue() : c() {} 
  explicit  priority_queue(const Compare& x) :  c(), comp(x) {} 
 
//以下用到的 make_heap(), push_heap(), pop_heap()都是泛型算法 
//注意，任一个建构式都立刻于底层容器内产生一个 implicit representation heap。 
  template  <class  InputIterator> 
priority_queue(InputIterator first, InputIterator last, const Compare& x) 
    : c(first, last), comp(x) {make_heap(c.begin(), c.end(), comp); } 
  template  <class  InputIterator> 
priority_queue(InputIterator first, InputIterator last) 
    : c(first,  last)  { make_heap(c.begin(), c.end(), comp); } 
 
  bool empty() const { return c.empty(); } 
  size_typesize() const { return c.size(); } 
  const_referencetop() const { return c.front(); } 
  void push(const value_type& x) { 
    __STL_TRY { 
// push_heap是泛型算法，先利用底层容器的 push_back() 将新元素 
      // 推入᳿端，再重排 heap。见 C++ Primer p.1195。 
c.push_back(x); 
push_heap(c.begin(), c.end(), comp);// push_heap是泛型算法 
    } 
    __STL_UNWIND(c.clear()); 
  } 
  void pop() { 
    __STL_TRY { 
// pop_heap 是泛型算法，从 heap 内取出一个元素。它并不是真正将元素 
      // 弹出，而是重排 heap，然后再以底层容器的 pop_back() 取得被弹出 
      // 的元素。见 C++ Primer p.1195。 
pop_heap(c.begin(), c.end(), comp); 
c.pop_back(); 
    } 
    __STL_UNWIND(c.clear()); 
  } 
};
```
## 优先级队列编程实现（C Plus Plus）
在这里自己用C++编程实现了简易版的优先级队列。其中用到了前一篇博客里面的堆heap.h：
```cpp
//STL堆算法实现（大顶堆）
//包含容器vector的头文件：Heap用vector来存储元素
#include <vector>
#include <iostream>
#include <functional>
#define MAX_VALUE 999999 //某个很大的值，存放在vector的第一个位置（最大堆）
const int StartIndex = 1;//容器中堆元素起始索引
using namespace std;
//堆类定义
//默认比较规则less
template <class ElemType,class Compare = less<ElemType> >
class MyHeap{
private:
	vector<ElemType> heapDataVec;//存放元素的容器
	int numCounts;//堆中元素个数
	Compare comp;//比较规则
public:
	MyHeap();
	vector<ElemType>& getVec();
	void initHeap(ElemType *data,const int n);//初始化操作
	void printfHeap();//输出堆元素
	void makeHeap();//建堆
	void sortHeap();//堆排序算法
	void pushHeap(ElemType elem);//向堆中插入元素
	void popHeap();//从堆中取出堆顶的元素
	void adjustHeap(int childTree,ElemType adjustValue);//调整子树
	void percolateUp(int holeIndex,ElemType adjustValue);//上溯操作
	void setNumCounts(int val);//设置当前所要构建的堆中元素个数
};
template <class ElemType,class Compare>
MyHeap<ElemType,Compare>::MyHeap()
:numCounts(0)
{
	heapDataVec.push_back(MAX_VALUE);
}
template <class ElemType,class Compare>
vector<ElemType>& MyHeap<ElemType,Compare>::getVec()
{
	return heapDataVec;
}
template <class ElemType,class Compare>
void MyHeap<ElemType,Compare>::initHeap(ElemType *data,const int n)
{
	//拷贝元素数据到vector中
	for (int i = 0;i < n;++i)
	{
		heapDataVec.push_back(*(data + i));
		++numCounts;
	}
}
template <class ElemType,class Compare>
void MyHeap<ElemType,Compare>::printfHeap()
{
	cout << "Heap : ";
	for (int i = 1;i <= numCounts;++i)
	{
		cout << heapDataVec[i] << " ";
	}
	cout << endl;
}
template <class ElemType,class Compare>
void MyHeap<ElemType,Compare>::makeHeap()
{
	//建堆的过程就是一个不断调整堆的过程，循环调用函数adjustHeap依次调整子树
	if (numCounts < 2)
		return;
	//第一个需要调整的子树的根节点多音
	int parent = numCounts / 2;
	while(1)
	{
		adjustHeap(parent,heapDataVec[parent]);
		if (StartIndex == parent)//到达根节点
			return;
		--parent;
	}
}
template <class ElemType,class Compare>
void MyHeap<ElemType,Compare>::sortHeap()
{
	//堆排序思路
	//每执行一次popHeap操作，堆顶的元素被放置在尾端，然后针对前面的一次再执行popHeap操作
	//依次下去，最后即得到排序结果
	while(numCounts > 0)
		popHeap();
}
template <class ElemType,class Compare>
void MyHeap<ElemType,Compare>::pushHeap(ElemType elem)
{
	//将新元素添加到vector中
	heapDataVec.push_back(elem);
	++numCounts;
	//执行一次上溯操作，调整堆，以使其满足最大堆的性质
	percolateUp(numCounts,heapDataVec[numCounts]);
}
template <class ElemType,class Compare>
void MyHeap<ElemType,Compare>::popHeap()
{
	//将堆顶的元素放在容器的最尾部，然后将尾部的原元素作为调整值，重新生成堆
	ElemType adjustValue = heapDataVec[numCounts];
	//堆顶元素为容器的首元素
	heapDataVec[numCounts] = heapDataVec[StartIndex];
	//堆中元素数目减一
	--numCounts;
	adjustHeap(StartIndex,adjustValue);
}
//调整以childTree为根的子树为堆
template <class ElemType,class Compare>
void MyHeap<ElemType,Compare>::adjustHeap(int childTree,ElemType adjustValue)
{
	//洞节点索引
	int holeIndex = childTree;
	int secondChid = 2 * holeIndex + 1;//洞节点的右子节点（注意：起始索引从1开始）
	while(secondChid <= numCounts)
	{
		if (comp(heapDataVec[secondChid],heapDataVec[secondChid - 1]))
		{
			--secondChid;//表示两个子节点中值较大的那个
		}
		//上溯
		heapDataVec[holeIndex] = heapDataVec[secondChid];//令较大值为洞值
		holeIndex = secondChid;//洞节点索引下移
		secondChid = 2 * secondChid + 1;//重新计算洞节点右子节点
	}
	//如果洞节点只有左子节点
	if (secondChid == numCounts + 1)
	{
		//令左子节点值为洞值
		heapDataVec[holeIndex] = heapDataVec[secondChid - 1];
		holeIndex = secondChid - 1;
	}
	//将调整值赋予洞节点
	heapDataVec[holeIndex] = adjustValue;
	//此时可能尚未满足堆的特性，需要再执行一次上溯操作
	percolateUp(holeIndex,adjustValue);
}
//上溯操作
template <class ElemType,class Compare>
void MyHeap<ElemType,Compare>::percolateUp(int holeIndex,ElemType adjustValue)
{
	//将新节点与其父节点进行比较，如果键值比其父节点大，就父子交换位置。
	//如此，知道不需要对换或直到根节点为止
	int parentIndex = holeIndex / 2;
	while(holeIndex > StartIndex && comp(heapDataVec[parentIndex],adjustValue))
	{
		heapDataVec[holeIndex] = heapDataVec[parentIndex];
		holeIndex = parentIndex;
		parentIndex /= 2;
	}
	heapDataVec[holeIndex] = adjustValue;//将新值放置在正确的位置
}
template <class ElemType,class Compare>
void MyHeap<ElemType,Compare>::setNumCounts(int val)
{
	numCounts = val;
}
```
PriorityQueue.h：
```cpp
#include "Heap.h"
//优先级队列类定义
//默认：值最小的权值最大
template <class ElemType,class Compare = less<ElemType> >
class MyPriorityQueue{
private:
	MyHeap<ElemType,Compare> heap;//底层用堆实现
public:
	//构造函数
	MyPriorityQueue(ElemType *data,int n);
	//判断优先级队列是否为空
	int empty(){return heap.getVec().size() - 1;}
	//返回优先级队列大小
	long size(){return heap.getVec().size() - 1;}//注意底层容器第一个元素是无效元素
	//取得优先级队列头元素
	ElemType top(){return heap.getVec()[StartIndex];}
	//添加元素
	void push(const ElemType &val);
	//弹出队首元素
	void pop();
	MyHeap<ElemType,Compare>& getHeap(){return heap;};
};
template <class ElemType,class Compare>
MyPriorityQueue<ElemType,Compare>::MyPriorityQueue(ElemType *data, int n)
{
	heap.initHeap(data,n);
	heap.makeHeap();
	heap.sortHeap();
}
template <class ElemType,class Compare>
void MyPriorityQueue<ElemType,Compare>::push(const ElemType &val)
{
	heap.setNumCounts(heap.getVec().size() - 1);//排除容器首部的哨兵元素
	heap.makeHeap();
	heap.pushHeap(val);
	heap.sortHeap();
}
template <class ElemType,class Compare>
void MyPriorityQueue<ElemType,Compare>::pop()
{
	heap.getVec().erase(heap.getVec().begin() + 1);//删除队列首部的元素
	heap.setNumCounts(heap.getVec().size() - 1);//排除容器首部的哨兵元素
	heap.makeHeap();
	heap.sortHeap();
}
```
PriorityQueueTest.cpp：
```cpp
#include "PriorityQueue.h"
#include <iostream>
#include <string>
using namespace std;
int main()
{
	const int n = 9;
	int data[n] = {0,1,2,3,4,8,9,3,5};
	MyPriorityQueue<int> *priorityObj1 = new MyPriorityQueue<int>(data,n);
	cout << "Current Heap: " << endl;
	for (int i = 1;i <= priorityObj1->size();++i)
	{
		cout << priorityObj1->getHeap().getVec()[i] << " ";
	}
	cout << endl;
	cout << "Size = " << priorityObj1->size() << endl;
	cout << "Top element = " << priorityObj1->top() << endl;
	priorityObj1->pop();
	cout << "After pop one element:" << endl;
	cout << "Size = " << priorityObj1->size() << endl;
	cout << "Top element = " << priorityObj1->top() << endl;
	cout << "Current Heap: " << endl;
	for (int i = 1;i <= priorityObj1->size();++i)
	{
		cout << priorityObj1->getHeap().getVec()[i] << " ";
	}
	cout << endl;
	priorityObj1->pop();
	cout << "After pop one element:" << endl;
	cout << "Size = " << priorityObj1->size() << endl;
	cout << "Top element = " << priorityObj1->top() << endl;
	cout << "Current Heap: " << endl;
	for (int i = 1;i <= priorityObj1->size();++i)
	{
		cout << priorityObj1->getHeap().getVec()[i] << " ";
	}
	cout << endl;
	priorityObj1->push(7);
	cout << "After push one element 7:" << endl;
	cout << "Size = " << priorityObj1->size() << endl;
	cout << "Top element = " << priorityObj1->top() << endl;
	cout << "Current Heap: " << endl;
	for (int i = 1;i <= priorityObj1->size();++i)
	{
		cout << priorityObj1->getHeap().getVec()[i] << " ";
	}
	cout << endl;
	delete priorityObj1;
}
```
运行结果（Win7 + VS2008）：
![](https://img-my.csdn.net/uploads/201301/31/1359619123_1214.jpg)
