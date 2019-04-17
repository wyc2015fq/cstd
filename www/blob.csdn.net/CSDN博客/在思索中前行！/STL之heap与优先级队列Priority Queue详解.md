# STL之heap与优先级队列Priority Queue详解 - 在思索中前行！ - CSDN博客





2015年03月24日 10:56:49[_Tham](https://me.csdn.net/txl16211)阅读数：2139








## **一、heap**

heap并不属于STL容器组件，它分为 max heap 和min heap，在缺省情况下，max-heap是优先队列（priority queue）的底层实现机制。而这个实现机制中的max-heap实际上是以一个vector表现的完全二叉树（complete binary tree）。STL在<algorithm.h>中实现了对 存储在vector/deque 中的元素进行堆操作的函数，包括make_heap, pop_heap, push_heap, sort_heap，对不愿自己写数据结构堆的C++选手来说，这几个算法函数很有用，详细解释可以参见： [http://www.cplusplus.com/reference/algorithm/make_heap/](http://www.cplusplus.com/reference/algorithm/push_heap/)

下面的_First与_Last为可以随机访问的迭代器（指针），_Comp为比较函数（仿函数），其规则——如果函数的第一个参数小于第二个参数应返回true，否则返回false。

1.make_heap()：

make_heap(_First, _Last)

make_heap(_First, _Last, _Comp)

默认是建立最大堆的。对int类型，可以在第三个参数传入greater<int>()得到最小堆。

2.push_heap(_First, _Last)：

新添加一个元素在末尾，然后重新调整堆序。也就是把元素添加在底层vector的end()处。

该算法必须是在一个已经满足堆序的条件下，添加元素。该函数接受两个随机迭代器，分别表示first,end,区间范围。

关键是我们执行一个siftup()函数，上溯函数来重新调整堆序。具体的函数机理很简单，可以参考我的编程珠玑里面堆的实现的文章。

3.pop_heap(_First, _Last)：

这个算法跟push_heap类似，参数一样。不同的是我们把堆顶元素取出来，放到了数组或者是vector的末尾，用原来末尾元素去替代，然后end迭代器减1，执行siftdown()下溯函数来重新调整堆序。

注意算法执行完毕后，最大的元素并没有被取走，而是放于底层容器的末尾。如果要取走，则可以使用底部容器（vector）提供的pop_back()函数。

4.sort_heap(_First, _Last)：

既然每次pop_heap可以获得堆中最大的元素，那么我们持续对整个heap做pop_heap操作，每次将操作的范围向前缩减一个元素。当整个程序执行完毕后，我们得到一个非降的序列。注意这个排序执行的前提是，在一个堆上执行。

下面是这几个函数操作vector中元素的例子。

```cpp
#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

int main()
{
  int a[] = {15, 1, 12, 30, 20};
  vector<int> ivec(a, a+5);
  for(vector<int>::iterator iter=ivec.begin();iter!=ivec.end();++iter)
    cout<<*iter<<" ";
  cout<<endl;

  make_heap(ivec.begin(), ivec.end());//建堆
  for(vector<int>::iterator iter=ivec.begin();iter!=ivec.end();++iter)
    cout<<*iter<<" ";
  cout<<endl;

  pop_heap(ivec.begin(), ivec.end());//先pop,然后在容器中删除
  ivec.pop_back();
  for(vector<int>::iterator iter=ivec.begin();iter!=ivec.end();++iter)
    cout<<*iter<<" ";
  cout<<endl;

  ivec.push_back(99);//先在容器中加入，再push
  push_heap(ivec.begin(), ivec.end());
  for(vector<int>::iterator iter=ivec.begin();iter!=ivec.end();++iter)
    cout<<*iter<<" ";
  cout<<endl;

  sort_heap(ivec.begin(), ivec.end());
  for(vector<int>::iterator iter=ivec.begin();iter!=ivec.end();++iter)
    cout<<*iter<<" ";
  cout<<endl;

  return 0;
}
```


二、priority queue



优先队列(priority_queue)首先是一个queue，那就是必须在末端推入，必须在顶端取出元素。除此之外别无其他存取元素的途径。内部元素按优先级高低排序，优先级高的在前。缺省情况下，priority_heap利用一个max-heap完成，后者是一个以vector表现的完全二叉树。我们说优先队列不是一个STL容器，它以底部容器而实现，修改了接口，形成另一种性质，这样的东西称之为适配器（adapter）。

详情参见：[http://www.cplusplus.com/reference/stl/priority_queue/](http://www.cplusplus.com/reference/stl/priority_queue/)



优先级队列是一个拥有权值观念的queue。它允许在底端添加元素、在顶端去除元素、删除元素。

优先级队列内部的元素并不是按照添加的顺序排列，而是自动依照元素的权值排列。权值最高者排在最前面。

缺省情况下，优先级队列利用一个大顶堆完成。关于堆可以参考：[STL堆详解与编程实现](http://blog.csdn.net/xiajun07061225/article/details/8553808)
优先级队列以底部容器完成其所有工作，具有这种“修改某物接口，形成另一种风貌”这种性质者，成为配接器（adapter）。在STL中优先级队列不被归类为容器，而被归类为容器配接器（container
 adapter）
priority_queue 对于基本类型的使用方法相对简单。

他的模板声明带有三个参数，priority_queue<Type, Container, Functional>

Type 为数据类型， Container 为保存数据的容器，Functional 为元素比较方式。

Container 必须是用数组实现的容器，比如 vector, deque 但不能用 list.

STL里面容器默认用的是 vector. 比较方式默认用 operator< , 所以如果你把后面俩个参数 缺省的话，优先队列就是大顶堆，队头元素最大。

看例子



```cpp
#include <iostream>
#include <queue>
using namespace std;
int main(){
    priority_queue<int,vector<int>,less<int> >q;//使用priority_queue<int> q1;一样
    for(int i=0;i<10;i++) 
		q1.push(i);
    while(!q1.empty()){
        cout<<q1.top()<< endl;
        q1.pop();
    }
    return 0;
}
```
如果要用到小顶堆，则一般要把模板的三个参数都带进去。

STL里面定义了一个仿函数 greater<>，对于基本类型可以用这个仿函数声明小顶堆

例子:





```cpp
#include <iostream>
#include <queue>
using namespace std;
int main(){
    priority_queue<int,vector<int>,greater<int> >q;
    for(int i=0;i<10;i++) 
		q.push(i);
    while(!q.empty()){
        cout<<q.top()<< endl;
        q.pop();
    }
    return 0;
}
```
对于自定义类型，则必须自己重载 operator< 或者自己写仿函数先看看例子:

```cpp
#include <iostream>
#include <queue>
using namespace std;
struct Node{
	int x, y;
}node;
 bool operator<( Node a, Node b){
    if(a.x==b.x) return a.y>b.y;
    return a.x>b.x;
}
 int main(){
    priority_queue<Node>q;
    for(int i=0;i<10;i++){
    	node.x=i;
    	node.y=10-i/2;
    	q.push(node);
    }	
    while(!q.empty()){
        cout<<q.top().x <<' '<<q.top().y<<endl;
        q.pop();
    }
    return 0;
}
```
自定义类型重载 operator< 后，声明对象时就可以只带一个模板参数。

此时不能像基本类型这样声明priority_queue<Node, vector<Node>, greater<Node> >;

原因是 greater<Node> 没有定义，如果想用这种方法定义

则可以按如下方式例子:（个人喜欢这种方法，因为set的自定义比较函数也可以写成这种形式）

```cpp
#include <iostream>
#include <queue>
using namespace std;
struct Node{
	int x, y;
}node;
struct cmp{
    bool operator()(Node a,Node b){
        if(a.x==b.x) return a.y>b.y;
        return a.x>b.x;}
};

 int main(){
    priority_queue<Node,vector<Node>,cmp>q;
    for(int i=0;i<10;i++){
    	node.x=i;
    	node.y=10-i/2;
		q.push(node);	
    }	
    while(!q.empty()){
        cout<<q.top().x<<' '<<q.top().y<<endl;
        q.pop();
    }
    return 0;
}
```



## SGI STL中优先级队列定义

## 定义完整代码：

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



在这里自己用C++编程实现了简易版的优先级队列。

其中用到了前一篇博客里面的堆heap.h：



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
运算结果：
![](https://img-my.csdn.net/uploads/201301/31/1359619123_1214.jpg)





