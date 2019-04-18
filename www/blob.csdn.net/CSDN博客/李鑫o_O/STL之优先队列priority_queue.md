# STL之优先队列priority_queue - 李鑫o_O - CSDN博客

2016年05月03日 10:37:06[hustlx](https://me.csdn.net/HUSTLX)阅读数：1079
个人分类：[c/c++](https://blog.csdn.net/HUSTLX/article/category/6122695)



STL之优先队列

原本以为priority_queue很简单，才知道原来懂的只是最简单的形式。

头文件：

#include<queue> 
#include <functional>

```cpp
auto comp = [](pair<int, int>&a, pair<int, int>&b) {
        return a.first + a.second > b.first + b.second;
    };
    priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(comp)> temp(comp);
```

```cpp
struct comp {
        bool operator()(const tweet& a, const tweet& b) {
            return a.time_ < b.time_;
        }
    };
    priority_queue<tweet, vector<tweet>, comp> res;
```

优先队列，也就是原来我们学过的堆，按照自己定义的优先级出队时。默认情况下底层是以Vector实现的heap。

既然是队列，也就只有入队、出队、判空、大小的操作，并不具备查找功能。

函数列表：
empty() 如果优先队列为空，则返回真

pop() 删除第一个元素

push() 加入一个元素

size() 返回优先队列中拥有的元素的个数

top() 返回优先队列中有最高优先级的元素

用途就不用多说了吧，例如Huffman编码、分支限界、A*启发式都需要用到优先队列存放信息。

来看最常用的几个功能，了解一下其中的知识点：

一：最基本的功能
|#include<iostream>#include<queue>using namespace std;int main(){priority_queue<int> Q;Q.push(2);Q.push(5);Q.push(3);while(!Q.empty()){cout<<Q.top()<<endl;Q.pop();}system("pause");return 0;}|优先队列最基本的功能就是出队时不是按照先进先出的规则，而是按照队列中优先级顺序出队。知识点：1、一般存放实型类型，可比较大小2、默认情况下底层以Vector实现3、默认情况下是大顶堆，也就是大者优先级高，后面可以自定义优先级比较规则|

二：次基本的功能
|#include<iostream>#include<queue>using namespace std;int main(){int a[5]={3,4,5,2,1};priority_queue<int> Q(a,a+5);while(!Q.empty()){cout<<Q.top()<<endl;Q.pop();}system("pause");return 0;}|可以将一个存放实型类型的数据结构转化为优先队列，这里跟优先队列的构造函数相关。上面那个默认构造一个空的优先队列，什么都使用默认的。而这里使用的是Priority_queue(InputIteratorfirst,InputIterator last)我理解的就是给出了一个容器的开口和结尾，然后把这个容器内容拷贝到底层实现(默认vector)中去构造出优先队列。这里也使用了一个默认的比较函数，也是默认大顶堆|

三应该掌握的功能:

|#include<iostream>#include<queue>using namespace std;typedef pair<long,int> Node;priority_queue< Node,vector< Node >,greater< Node > > Q;|这个里面定义了一个制定存放元素(Node),底层实现以vector实现（第二个参数）,优先级为小顶堆(第三个参数)。前两个参数没什么说的，很好理解，其中第三个参数，默认有三写法：小顶堆：greater<TYPE>大顶堆：less<TYPE>如果想自定义优先级而TYPE不是基本类型，而是复杂类型，例如结构体、类对象，则必须重载其中的operator()，见下面的例子。|

例子：

#include<iostream>

#include<queue>

using namespace std;

//模拟存放节点信息

typedef struct

{

int a;

int b;

}Node;

//自定义优先级类型

struct cmp

{

bool
 operator()(const Node &t1,const Node &t2)

{

return
 t1.b<t2.b;//相当于less,大顶堆

}

};

int main()

{

//初始化

int
 n;

cin>>n;

Node
 *arr=new Node[n];

for(int
 i=0;i<n;i++)

{

cin>>arr[i].a>>arr[i].b;

}

//定义优先队列，自定义优先级,跟Qsort里面自定义相似

priority_queue<Node,vector<Node>,cmp>
 Q(arr,arr+n);

while(!Q.empty())

{

Node
 n=Q.top();

cout<<n.a<<"
 "<<n.b<<endl;

Q.pop();

}

system("pause");

return
 0;

}

一个例题就是ugly number

-------------------------------------------

Ugly numbers are numbers whose only prime factors are 2, 3 or 5. The sequence

1, 2, 3, 4, 5, 6, 8, 9, 10, 12, 15, ...

shows the first 11 ugly numbers. By convention, 1 is included.

Write a program to find and print the 1500'th ugly number.

转帖一个人写的使用优先队列解决的代码
#include <iostream> #include <queue>  using namespace std;  typedef pair<unsigned long, int> node_type;  main() {  unsigned long result[1500]; priority_queue< node_type, vector<node_type>, greater<node_type> > Q; Q.push( make_pair(1, 2) );  for (int i=0; i<1500; i++)  { node_type node = Q.top(); Q.pop(); switch(node.second) {  case 2: Q.push( make_pair(node.first*2, 2) ); case 3: Q.push( make_pair(node.first*3, 3) );  case 5: Q.push( make_pair(node.first*5, 5) ); }  result[i] = node.first;  } int n; cin >> n;  while (n>0)  {  cout << result[n-1] << endl; cin >> n; }  return 1; }
```cpp
vector<int> topKFrequent(vector<int>& nums, int k) {
        unordered_map<int, int> counts;
        priority_queue<int, vector<int>, greater<int>> max_k;
        for(auto i : nums) ++counts[i];
        for(auto & i : counts) {
            max_k.push(i.second);
            // Size of the min heap is maintained at equal to or below k
            while(max_k.size() > k) max_k.pop();
        }
        vector<int> res;
        for(auto & i : counts) {
            if(i.second >= max_k.top()) res.push_back(i.first);
        }
        return res;
    }
```

