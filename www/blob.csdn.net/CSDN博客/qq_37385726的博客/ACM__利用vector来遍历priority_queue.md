# ACM__利用vector来遍历priority_queue - qq_37385726的博客 - CSDN博客





2018年05月23日 08:30:40[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：244








![](https://img-blog.csdn.net/20180523082517867?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**·对于上述的rank的条件部分可以使用规定优先队列的优先级来指定**

```cpp
bool operator < (const Node& a) const
	{
		if(l>a.l)
		{
			return 1;
		}
		else if(l<a.l)
		{
			return 0;
		}
		else 
		{
			if(t>a.t)
			{
				return 1;
			}
			else if(t<a.t) 
			{
				return 0;
			}
			else
			{
			 	if(w>a.w)
			 	{
			 		return 1;
				 }
				 else if(w<a.w)
				 {
				 	return 0;
				 }
				 else
				 {
				 	if(c>a.c)
				 	{
				 		return 1;
					 }
					 else if(c<a.c)
					 {
					 	return 0;
					 }
					 else
					 {
					 	if(f>a.f)
					 	{
					 		return 1;
						}
						else if(f<a.f)
						{
							return 0;
						}
						else
						{
							return 0;
						}
					 }
				 }
			 }
		 } 
	}
```

**·其中又需要访问rank的元素，使用vector来实现**

```cpp
vector<Node> vtor;
		for(int k=0;k<n;k++)
		{
			vtor.push_back(Node(pq.top().u,pq.top().v,pq.top().l,pq.top().t,pq.top().w,pq.top().c,pq.top().f));
			pq.pop();
		 }
```

**·总实现代码如下**

```cpp
#include<iostream>
using namespace std;
#include<queue>
#include<vector>

struct Node
{
	long long u; //端点
	long long v;//端点
	long long l;//长度
	long long t;//时间
	long long w;//修路费用
	long long c;//车流最大容量
	long long f;//当前车流量
	
	Node(long long uu=0,long long vv=0,long long ll=0,long long tt=0,long long ww=0,long long cc=0,long long ff=0)
	{
		u=uu;
		v=vv;
		l=ll;
		t=tt;
		w=ww;
		c=cc;
		f=ff;
	} 
	
	bool operator < (const Node& a) const
	{
		if(l>a.l)
		{
			return 1;
		}
		else if(l<a.l)
		{
			return 0;
		}
		else 
		{
			if(t>a.t)
			{
				return 1;
			}
			else if(t<a.t) 
			{
				return 0;
			}
			else
			{
			 	if(w>a.w)
			 	{
			 		return 1;
				 }
				 else if(w<a.w)
				 {
				 	return 0;
				 }
				 else
				 {
				 	if(c>a.c)
				 	{
				 		return 1;
					 }
					 else if(c<a.c)
					 {
					 	return 0;
					 }
					 else
					 {
					 	if(f>a.f)
					 	{
					 		return 1;
						}
						else if(f<a.f)
						{
							return 0;
						}
						else
						{
							return 0;
						}
					 }
				 }
			 }
		 } 
	}
};



int main(void)
{
	int n;
	int m;
	while(~scanf("%d",&n))
	{
		long long uu,vv,ll,tt,ww,cc,ff;
		priority_queue<Node> pq;
		priority_queue<Node> temp;
		for(int i=0;i<n;i++)
		{
			scanf("%lld %lld %lld %lld %lld %lld %lld",&uu,&vv,&ll,&tt,&ww,&cc,&ff);
			pq.push(Node(uu,vv,ll,tt,ww,cc,ff));
		}
		vector<Node> vtor;
		for(int k=0;k<n;k++)
		{
			vtor.push_back(Node(pq.top().u,pq.top().v,pq.top().l,pq.top().t,pq.top().w,pq.top().c,pq.top().f));
			pq.pop();
		 } 
		scanf("%d",&m);
		int rank;
		for(int i=0;i<m;i++)
		{
			scanf("%d",&rank);
			printf("%lld %lld %lld %lld %lld %lld %lld\n",vtor[rank-1].u,vtor[rank-1].v,vtor[rank-1].l,vtor[rank-1].t,vtor[rank-1].w,vtor[rank-1].c,vtor[rank-1].f);
		}
	}
 }
```




####转#####



Vector是同一种类型的对象的集合，每个对象都有一个对应的整数索引值。Vector的数据安排及操作方式与array非常相似，唯一的差别在于array是静态空间，一旦配置了就不能改变；vector是动态空间，随着元素的加入，它的内部机制会自行扩充空间以容纳新元素。 
使用vector之前，必须包含相应的头文件和命名空间：

```cpp
#include <vector>
using std::vector
```
- 1
- 2

vector不是一种数据类型，而只是一个类模版，可以用来定义多种数据类型。在定义了vector对象之后，vector类型的每一种都指定了其保存元素的类型。例如：使用的vector和vector都是数据类型。

## 1、vector对象的定义及初始化

```cpp
vector<type> vec1;  // v1保存类型为type的对象，默认构造函数，v1为空
vector<type> vec2(size);  // v2含有值初始化的元素的size个副本
vector<type> vec3(size,value);  // v3包含值为value的size个元素
vector<type> vec4(myvector);  // v4是myvector的一个副本
vector<type> vec5(first,last);  // 用迭代器first和last之间的元素创建v5
```
- 1
- 2
- 3
- 4
- 5

## 2、vector与Array的转换

vector赋值不可像数组一样使用花括号显示初始化，但可借用数组来初始化。

```cpp
int array[ ]={1, 2, 3, 4, 5, 6, 7};
vector<int> vec(array, array+7);  // 相当于使用begin end 此时vec中的元素也是1234567
```
- 1
- 2

## 3、vector对象的基本操作

```php
v.empty()：如果v为空，则返回true，否则返回false。
v.size()：返回v中元素的个数。
v.push_back(t)：在v的末尾增加一个值为t的元素。
v[n]：返回v中位置为n的元素。
v1=v2：把v1的元素替换成v2中元素的副本。
v1==v2：如果v1与v2相等，则返回true。
!=, <, <=, >, >=：保持这些操作符惯有的含义。
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7

Vector的下标操作符接受一个值，并返回vector中的该对应位置的元素。Vector元素的位置从0开始，使用vector::size_type作为下标的类型。下标操作不能添加元素，只能获取已存在的元素，想在vector中插入元素，使用push_back()函数。也就是说，必须是已存在的元素才能用下标操作符进行索引，通过下标操作符进行赋值时，不会添加任何元素。下标操作符[ ]仅能提取确实已存在的元素。

## 4、容器的容器与2维数组

因为容器受容器元素类型的约束，所以可定义元素是容器类型的容器。例如，可以定义 vector 类型的容器 lines，其元素为 string 类型的 vector 对象：

```cpp
vector< vector<string> > lines;  
// 这里需要特别说明的是C++11中已经不在使用两个空格隔开相邻的>符号
```
- 1
- 2

声明容器的容器，必须用空格隔开两个相邻的 > 符号，以示这是两个分开的符号，否则，系统会认为 >> 是单个符号，为右移操作符，并导致编译时错误。

```cpp
vector<vector<int> > mat(3, vector<int>(4));
```
- 1

mat含有3个元素，每个元素是含有4个int值的向量，mat.size()是3 ，即3行。这个用法即相当于一个2维数组。

## 5、vector元素的常用操作

这里我们以`vector<int> c;`为例进行说明。

### 1、容器的大小操作
- c.max_size()：返回向量类型的最大容量（2^30-1=0x3FFFFFFF）
- c.capacity()：返回向量当前开辟的空间大小（<= max_size，与向量的动态内存分配策略相关）。
- c.size()：返回向量中现有元素的个数（<=capacity）。
- c.resize(n)：调整向量的长度使其能容纳n个元素。
- c.resize(n,x)：把向量的大小改为n，所有新元素的初值赋为x。
- c.empty()：如果向量为空，返回真。

### 2、元素的赋值操作
- c.assign(first,last)：将迭代器first,last所指定范围内的元素复制到c 中。
- c.assign(num,val)：用val的num份副本重新设置c。

### 3、元素的访问操作
- c.at(n)：等价于下标运算符[]，返回向量中位置n的元素，因其有越界检查，故比[ ]索引访问安全。
- c.front()：返回向量中第一个元素的引用。
- c.back()：返回向量中最后一个元素的引用。
- c.begin()：返回向量中第一个元素的迭代器。
- c.end()：返回向量中最后一个元素的下一个位置的迭代器，仅作结束游标，不可解引用。
- c.rbegin()：返回一个反向迭代器，该迭代器指向容器的最后一个元素。
- c.rend()：返回一个反向迭代器，该迭代器指向容器第一个元素前面的位置。

### 4、元素的删除操作
- c.pop_back()：删除向量最后一个元素。
- c.clear()：删除向量中所有元素。
- c.erase(iter)：删除迭代器iter所指向的元素，返回一个迭代器指向被删除元素后面的元素。
- c.erase(start, end)：删除迭代器start、end所指定范围内的元素，返回一个迭代器指向被删除元素段后面的元素。

### 5、元素的插入操作
- c.push_back(x)：把x插入到向量的尾部。
- c.insert(iter, x)：在迭代器iter指向的元素之前插入值为x的新元素，返回指向新插入元素的迭代器。
- c.insert(iter,n,x)：在迭代器iter指向的元素之前插入n个值为x的新元素，返回void。
- c.insert(iter,start,end)：把迭代器start和end所指定的范围内的所有元素插入到迭代器iter所指向的元素之前，返回void。

### 6、元素的交换操作
- c.reverse()：反转元素顺序。
- c.swap(c2)：交换2个向量的内容，c和c2 的类型必须相同。



