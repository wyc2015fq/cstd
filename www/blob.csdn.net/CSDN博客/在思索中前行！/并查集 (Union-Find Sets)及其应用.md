# 并查集 (Union-Find Sets)及其应用 - 在思索中前行！ - CSDN博客





2015年04月14日 21:30:53[_Tham](https://me.csdn.net/txl16211)阅读数：1445








## 定义

## 并查集是一种树型的[数据结构](http://baike.baidu.com/view/9900.htm)，用于处理一些不相交[集合](http://baike.baidu.com/view/15216.htm)（Disjoint Sets）的合并及查询问题。常常在使用中以[森林](http://baike.baidu.com/view/14402.htm)来表示。

集就是让每个元素构成一个单元素的[集合](http://baike.baidu.com/view/15216.htm)，也就是按一定顺序将属于同一组的[元素](http://baike.baidu.com/view/19993.htm)所在的集合合并。

## 主要操作


初始化

把每个点所在[集合](http://baike.baidu.com/view/15216.htm)初始化为其自身。

通常来说，这个[步骤](http://baike.baidu.com/view/1121831.htm)在每次使用该[数据结构](http://baike.baidu.com/view/9900.htm)时只需要[执行](http://baike.baidu.com/subview/278995/5096407.htm)一次，无论何种实现方式，[时间复杂度](http://baike.baidu.com/view/104946.htm)均为O(N)。

查找

查找元素所在的[集合](http://baike.baidu.com/view/15216.htm)，即根节点。

合并

将两个元素所在的[集合](http://baike.baidu.com/view/15216.htm)合并为一个集合。

通常来说，合并之前，应先判断两个元素是否属于同一[集合](http://baike.baidu.com/view/15216.htm)，这可用上面的“查找”操作实现。

## 例题描述


### 描述


若某个[家族](http://baike.baidu.com/view/103848.htm)人员过于庞大，要判断两个是否是亲戚，确实还很不容易，给出某个亲戚关系图，求任意给出的两个人是否具有亲戚关系。 规定：x和y是亲戚，y和z是亲戚，那么x和z也是亲戚。如果x,y是亲戚，那么x的亲戚都是y的亲戚，y的亲戚也都是x的[亲戚](http://baike.baidu.com/subview/188616/5128048.htm)。


第一行：三个整数n,m,p，（n< =5000,m< =5000,p< =5000），分别表示有n个人，m个亲戚关系，询问p对亲戚关系。 以下m行：每行两个数Mi，Mj，1< =Mi，Mj< =N，表示Mi和Mj具有亲戚关系。 接下来p行：每行两个数Pi，Pj，询问Pi和Pj是否具有亲戚关系。


P行，每行一个’Yes’或’No’。表示第i个询问的答案为“具有”或“不具有”亲戚关系。

###   分析——问题实质


初步分析觉得本题是一个[图论](http://baike.baidu.com/view/79350.htm)中判断两个点是否在同一个连通子图中的问题。对于题目中的样例，以人为点，关系为边，建立[无向图](http://baike.baidu.com/view/93110.htm)如下：


图0-0-1 {请补充图解}


比如判断3和4是否为亲戚时，我们检查3和4是否在同一个连通子图中，结果是在，于是他们是亲戚。又如7和10不在同一个连通子图中，所以他们不是亲戚。

用图的[数据结构](http://baike.baidu.com/view/9900.htm)的最大问题是，我们无法存下多至(M=)2 000 000条边的图，后面关于算法时效等诸多问题就免谈了。


用图表示关系过于“[奢侈](http://baike.baidu.com/subview/9396/5368068.htm)”了。其实本题只是一个对分离[集合](http://baike.baidu.com/view/15216.htm)（并查集）操作的问题。


我们可以给每个人建立一个[集合](http://baike.baidu.com/view/15216.htm)，集合的元素值有他自己，表示最开始时他不知道任何人是它的亲戚。以后每次给出一个亲戚关系a, b，则a和他的亲戚与b和他的亲戚就互为亲戚了，将a所在[集合](http://baike.baidu.com/view/15216.htm)与b所在集合合并。对于样例数据的操作全过程如下：


输入关系 分离[集合](http://baike.baidu.com/view/15216.htm)


初始状态


(2,4) {2,4}


(5,7) {2,4} {5,7}


(1,3) {1,3} {2,4} {5,7}


(8,9) {1,3} {2,4} {5,7} {8,9}


(1,2) {1,2,3,4} {5,7} {8,9}


(5,6) {1,2,3,4} {5,6,7} {8,9}


(2,3) {1,2,3,4} {5,6,7} {8,9}


最后我们得到3个[集合](http://baike.baidu.com/view/15216.htm){1,2,3,4}, {5,6,7}, {8,9}，于是判断两个人是否亲戚的问题就变成判断两个数是否在同一个集合中的问题。如此一来，需要的[数据结构](http://baike.baidu.com/view/9900.htm)就没有图结构那样庞大了。


算法需要以下几个子过程：


(1) 开始时，为每个人建立一个[集合](http://baike.baidu.com/view/15216.htm)SUB-Make-Set(x)；


(2) 得到一个关系后a,b，合并相应[集合](http://baike.baidu.com/view/15216.htm)SUB-Union(a,b)；


(3) 此外我们还需要判断两个人是否在同一个[集合](http://baike.baidu.com/view/15216.htm)中，这就涉及到如何标识集合的问题。我们可以在每个[集合](http://baike.baidu.com/view/15216.htm)中选一个代表标识集合，因此我们需要一个子过程给出每个集合的代表元SUB-Find-Set(a)。于是判断两个人是否在同一个[集合](http://baike.baidu.com/view/15216.htm)中，即两个人是否为亲戚，等价于判断SUB-Find-Set(a)=SUB-Find-Set(b)。


有了以上子过程的支持，我们就有如下算法。


PROBLEM-Relations(N, M, a1,…,aM, b1,…,bM, Q, c1,…,cQ, d1,…,dQ)


1 for i←1 to N


2 do SUB-Make-Set(i)


3 for i←1 to M


4 do if SUB-Find-Set(ai) != SUB-Find-Set(bi)


5 then SUB-Union(ai, bi)


6 for i←1 to Q


7 do if SUB-Find-Set(ci)=SUB-Find-Set(di)


8 then output “Yes?”


9 else output “No?”

解决问题的关键便为选择合适的[数据结构](http://baike.baidu.com/view/9900.htm)实现并查集的操作，使算法的实现效率最高。

### 注意事项

### 本题的输入数据量很大，这使得我们的程序会在输入中花去不少时间。如果你用Pascal写程序，可以用库函数SetTextBuf为输入文件设置[缓冲区](http://baike.baidu.com/view/266782.htm)，这可以使输入过程加快不少。如果你是用C语言的话，就不必为此操心了，系统会自动分配[缓冲区](http://baike.baidu.com/view/266782.htm)。

### 单链表实现



一个节点对应一个人，在同一个[集合](http://baike.baidu.com/view/15216.htm)中的节点串成一条[链表](http://baike.baidu.com/view/549479.htm)就得到了[单链表](http://baike.baidu.com/view/178637.htm)的实现。在[集合](http://baike.baidu.com/view/15216.htm)中我们以[单链表](http://baike.baidu.com/view/178637.htm)的第一个节点作为集合的代表元。于是每个节点x（x也是人的编号）应包含这些信息：指向代表元即表首的指针head[x]，指向表尾的指针tail[x]，下一个节点的指针next[x]。


SUB-Make-Set(x)过程设计如下：


SUB-Make-Set(x)


10 head[x]←x


11 tail[x]←x


12 next[x]←NIL


求代表元的SUB-Find-Set(x)过程设计如下：


SUB-Find-Set(x)


13 return head[x]

前两个过程比较简单，SUB-Union(a,b)稍微复杂一点。我们要做的是将b所在[链表](http://baike.baidu.com/view/549479.htm)加到a所在链表尾，然后b所在链表中的所有节点的代表元[指针](http://baike.baidu.com/view/159417.htm)改指a所在链表的表首节点，如图所示。


图0-0-2


过程的伪代码如下：


SUB-Union(a,b)


14 next[tail[head[a]]]←head


15 tail[head[a]]←tail[head]


16 p←head


17 while p != NIL


18 do head[p]←head[a]


19 p←next[p]


我们来分析一下算法的时间效率。SUB-Make-Set(x)和SUB-Find-Set(x)都只需要O(1)的时间，而SUB-Union(a,b)的时间效率与b所在[链表](http://baike.baidu.com/view/549479.htm)的长度成线性关系。最坏情况下，即有操作序列SUB-Union(N-1,N),
 SUB-Union(N-2,N-1), …, SUB-Union(1,2)时，整个算法PROBLEM-Relations的[时间复杂度](http://baike.baidu.com/view/104946.htm)为O(N+M+N^2+Q)=O(N^2+M+Q)。


由于算法的[时间复杂度](http://baike.baidu.com/view/104946.htm)中O(M+Q)是必需的，因此我们要让算法更快，就要考虑如何使减小O(N^2)。


我们想到合并[链表](http://baike.baidu.com/view/549479.htm)时，我们可以用一种启发式的方法：将较短的表合并到较长表上。为此每个节点中还需包含表的长度的信息。这比较容易实现，我们就不写出[伪代码](http://baike.baidu.com/view/585970.htm)了。

[时间复杂度](http://baike.baidu.com/view/104946.htm)。


首先我们给出一个固定对象x的代表元指针head[x]被更新次数的[上界](http://baike.baidu.com/view/45901.htm)。由于每次x的代表元指针被更新时，x必然在较小的[集合](http://baike.baidu.com/view/15216.htm)中，因此x的代表元指针被更新一次后，集合至少含2个元素。类似地，下一次更新后，[集合](http://baike.baidu.com/view/15216.htm)至少含4个元素，继续下去，当x的代表元指针被更新
 log k 次后，集合至少含k个元素，而集合最多含n个元素，所以x的代表元指针至多被更新 log n 次。所以M次SUB-Union(a,b)操作的[时间复杂度](http://baike.baidu.com/view/104946.htm)为O(NlogN+M)。算法总的[时间复杂度](http://baike.baidu.com/view/104946.htm)为O(NlogN+M+Q)。

## 并查集森林




并查集的另一种更快的实现是用有根树来表示[集合](http://baike.baidu.com/view/15216.htm)：每棵树表示一个[集合](http://baike.baidu.com/view/15216.htm)，树中的节点对应一个人。图示出了一个并查集[森林](http://baike.baidu.com/view/14402.htm)。

图0-0-3

每个节点x包含这些信息：父节点指针p[x]，树的深度rank[x]。其中rank[x]将用于启发式合并过程。

于是建立集合过程的[时间复杂度](http://baike.baidu.com/view/104946.htm)依然为O(1)。

SUB-Make-Set(x)

20 p[x]←x

21 rank[x]←0

用[森林](http://baike.baidu.com/view/14402.htm)的[数据结构](http://baike.baidu.com/view/9900.htm)来实现的最大好处就是降低SUB-Union(a,b)过程的[时间复杂度](http://baike.baidu.com/view/104946.htm)。

SUB-Union(a,b)

22 SUB-Link(SUB-Find-Set(a),SUB-Find-Set(b))

SUB-Link(a,b)

23 p[a]←b

合并[集合](http://baike.baidu.com/view/15216.htm)的工作只是将a所在树的根节点的父节点改为b所在树的根节点。这个操作只需O(1)的时间。而SUB-Union(a,b)的时间效率决定于SUB-Find-Set(x)的快慢。

SUB-Find-Set(x)

24 if x=p[x]

25 then return x

26 else return SUB-Find-Set(p[x])

这个过程的时效与树的深度成线性关系，因此其平均[时间复杂度](http://baike.baidu.com/view/104946.htm)为O(logN)，但在最坏情况下（树退化成[链表](http://baike.baidu.com/view/549479.htm)），时间复杂度为O(N)。于是PROBLEM-Relations最坏情况的[时间复杂度](http://baike.baidu.com/view/104946.htm)为O(N(M+Q))。有必要对算法进行优化。

第一个优化是启发式合并。在优化[单链表](http://baike.baidu.com/view/178637.htm)时，我们将较短的表链到较长的表尾，在这里我们可以用同样的方法，将深度较小的树指到深度较大的树的根上。这样可以防止树的退化，最坏情况不会出现。SUB-Find-Set(x)的[时间复杂度](http://baike.baidu.com/view/104946.htm)为O(log
 N)，PROBLEM-Relations时间复杂度为O(N + logN (M+Q))。SUB-Link(a,b)作相应改动。

SUB-Link(a,b)

27 if rank[a]>rank

28 then p←a

29 else p[a]←b

30 if rank[a]=rank

31 then rank←rank+1

然而算法的耗时主要还是花在SUB-Find-Set(x)上。

第二个优化是路径压缩。它非常简单而有效。如图所示，在SUB-Find-Set(1)时，我们“顺便”将节点1, 2, 3的父节点全改为节点4，以后再调用SUB-Find-Set(1)时就只需O(1)的时间。

图0-0-4

于是SUB-Find-Set(x)的代码改为：

SUB-Find-Set(x)

32 if x≠p[x]

33 then p[x]←SUB-Find-Set(p[x])

34 return p[x]

该过程首先找到树的根，然后将路径上的所有节点的父节点改为这个根。实现时，递归的程序有许多栈的操作，改成非递归会更快些。

SUB-Find-Set(x)

35 r←x

36 while r≠p[r]

37 do r←p[r]

38 while x?r

39 do q←p[x]

40 p[x]←r

41 x←q

42 return r

改进后的算法[时间复杂度](http://baike.baidu.com/view/104946.htm)的分析十分复杂，如果完整的写出来足可写一节，这里我们只给出结论：改进后的PROBLEM-Relations其时间复杂度为O(N+(M+Q)*A(M+Q,N))，其中A(M+Q,N)为Ackerman函数的增长极为缓慢的逆函数。你不必了解与Ackerman函数相关的内容，只需知道在任何可想象得到的并查集[数据结构](http://baike.baidu.com/view/9900.htm)的应用中，A(M+Q,N)≤4，因此PROBLEM-Relations的[时间复杂度](http://baike.baidu.com/view/104946.htm)可认为是线性的O(N+M+Q)。

## 并查集优化——路径压缩



### 思想


每次查找的时候，如果路径较长，则修改信息，以便下次查找的时候速度更快。

### 实现


第一步，找到根结点。


第二步，修改查找路径上的所有节点，将它们都指向根结点。

### 代码

```cpp
find3(x)
{
	r = x;
	while (set[r] <> r) //循环结束，则找到根节点
		r = set[r];
	i = x;
	while (i <> r) //本循环修改查找路径中所有节点
	{
		j = set[i];
		set[i] = r;
		i = j;
	}	 
}
```

### 图示


![](http://d.hiphotos.baidu.com/baike/s%3D220/sign=28e5a21a0a23dd542573a06ae108b3df/d788d43f8794a4c29ee839550df41bd5ac6e3981.jpg)

![](http://d.hiphotos.baidu.com/baike/s%3D220/sign=71bd4a50abd3fd1f3209a538004e25ce/aa18972bd40735fa15fb97c79d510fb30f240841.jpg)





### C++实现亲戚代码

```cpp
#include<iostream>
#include<cstring>
#include<cstdio>
#include<cstdlib>
using namespace std;
 
int father[50002],a,b,m,n,p;
int find(int x){
    if(father[x]!=x)
       father[x]=find(father[x]);
/*
x代表例题中的人，father[x]中所存的数代表这一集合中所有人都与一个人有亲戚关系
相当于例题中第一个集合所有的元素都与第一个元素有亲戚关系
搜索时只要找元素所指向的father[x]=x的元素(即父元素)
然后比较两个元素的父元素是否相同就可以判断其关系
*/
    return father[x];
}
int main()
{
  int i;
  scanf("%d%d%d",&n,&m,&p);
  for(i=1;i<=n;i++)
    father[i]=i;
    for(i=1;i<=m;i++)
    {
      scanf("%d%d",&a,&b);
      a=find(a);
      b=find(b);
      father[a]=b;
    }
    for(i=1;i<=p;i++)
    {
      scanf("%d%d",&a,&b);
      a=find(a);
      b=find(b);
      if(a==b)
        printf("Yes");
      else
        printf("No");
    }
  return 0;
}
```

## 并查集的操作演示

并查集支持以下三中种操作:
　　－Union (Root1, Root2) //并操作；把子集合Root2并入集合Root1中.要求：Root1和 Root2互不相交,否则不执行操作.
　　－Find (x) //搜索操作；搜索单元素x所在的集合,并返回该集合的名字.
　　－UFSets (s) //构造函数。将并查集中s个元素初始化为s个只有一个单元素的子集合.
　　－对于并查集来说，每个集合用一棵树表示。
　　－集合中每个元素的元素名分别存放在树的结点中，此外，树的每一个结点还有一个指向其双亲结点的指针。
　　－设 S1= {0, 6, 7, 8 }，S2= { 1, 4, 9 }，S3= { 2, 3, 5 }

![](https://p-blog.csdn.net/images/p_blog_csdn_net/fisher_jiang/t7.jpg)![](https://p-blog.csdn.net/images/p_blog_csdn_net/fisher_jiang/t8.jpg)![](https://p-blog.csdn.net/images/p_blog_csdn_net/fisher_jiang/t9.jpg)



－为简化讨论，忽略实际的集合名，仅用表示集合的树的根来标识集合。

－为此，采用树的双亲表示作为集合存储表示。集合元素的编号从0到 n-1。其中 n 是最大元素个数。在双亲表示中，第 i 个数组元素代表包含集合元素 i 的树结点。根结点的双亲为-1，表示集合中的元素个数。为了区别双亲指针信息( ≥ 0 )，集合元素个数信息用负数表示。　　　
|下标 parent![](https://p-blog.csdn.net/images/p_blog_csdn_net/fisher_jiang/250.bmp)||
|----|----|

**集合S1, S2和S3的双亲表示:**



![](https://p-blog.csdn.net/images/p_blog_csdn_net/fisher_jiang/t4.jpg)

**S1 ∪ S2的可能的表示方法**

```cpp
find3(x)
{
	r = x;
	while (set[r] <> r) //循环结束，则找到根节点
		r = set[r];
	i = x;
	while (i <> r) //本循环修改查找路径中所有节点
	{
		j = set[i];
		set[i] = r;
		i = j;
	}	 
}



const int DefaultSize = 10;
class UFSets { //并查集的类定义
　　	private:
　　　			int *parent;
　　　			int size;
　　	public:
　　　			UFSets ( int s = DefaultSize ); 
　　　			~UFSets ( ) { delete [ ] parent; } 
　　　			UFSets & operator = ( UFSets const & Value );//集合赋值
　　　			void Union ( int Root1, int Root2 ); 
　　　			int Find ( int x );
　　　			void UnionByHeight ( int Root1, int Root2 ); 
};

UFSets::UFSets ( int s ) 
{ //构造函数
　　　		size = s; 
　　　		parent = new int [size+1];
　　　		for ( int i = 0; i <= size; i++ ) 
					parent[i] = -1; 
}	

unsigned int UFSets::Find ( int x ) 
{ //搜索操作
　　　if ( parent[x] <= 0 ) return x; 
　　　else return Find ( parent[x] );
}

void UFSets::Union ( int Root1, int Root2 ) 
{ //并操作 
　　　parent[Root2] = Root1; //Root2指向Root1
}
```


  Find和Union操作性能不好。假设最初 n 个元素构成 n 棵树组成的森林，parent[i]
 = -1。做处理Union(0, 1), Union(1, 2), …, Union(n-2, n-1)后，将产生如图所示的退化的树。

![](https://p-blog.csdn.net/images/p_blog_csdn_net/fisher_jiang/t5.jpg)



**        执行一次Union操作所需时间是O(1)，n-1次Union操作所需时间是O(n)。若再执行Find(0), Find(1), …, Find(n-1), 若被        搜索的元素为i，完成Find(i)操作需要时间为O(i)，完成 n 次搜索需要的总时间将达到![](https://p-blog.csdn.net/images/p_blog_csdn_net/fisher_jiang/253.bmp)**

Union操作的加权规则

为避免产生退化的树，改进方法是先判断两集合中元素的个数，如果以 i 为根的树中的结点个数少于以 j 为根的树中的结点个数，即parent[i]
 > parent[j]，则让 j 成为 i 的双亲，否则，让i成为j的双亲。此即Union的加权规则。**![](https://p-blog.csdn.net/images/p_blog_csdn_net/fisher_jiang/t10.jpg)**

**parent[0](== -4) < parent[4] (== -3)**




```cpp
void UFSets::WeightedUnion(int Root1, int Root2)
{
　　　//按Union的加权规则改进的算法
　　　int temp = parent[Root1] + parent[Root2];
　　　if ( parent[Root2] < parent[Root1] ) 
	  {
　　　　	parent[Root1] = Root2; //Root2中结点数多
　　　　	parent[Root2] = temp; 　//Root1指向Root2
　　　}
　　　else 
   	  { 
　　　　	parent[Root2] = Root1; //Root1中结点数多
　　　　	parent[Root1] = temp; 　//Root2指向Root1
　　　}
}
```

![](https://p-blog.csdn.net/images/p_blog_csdn_net/fisher_jiang/t11.jpg)



## 关于并查集的典型习题


**题目： 亲戚(Relations)**

      或许你并不知道，你的某个朋友是你的亲戚。他可能是你的曾祖父的外公的女婿的外甥的表姐的孙子。如果能得到完整的家谱，判断两个人是否亲戚应该是可行的，但如果两个人的最近公共祖先与他们相隔好几代，使得家谱十分庞大，那么检验亲戚关系实非人力所能及.在这种情况下，最好的帮手就是计算机。

为了将问题简化，你将得到一些亲戚关系的信息，如同Marry和Tom是亲戚，Tom和B en是亲戚，等等。从这些信息中，你可以推出Marry和Ben是亲戚。请写一个程序，对于我们的关心的亲戚关系的提问，以最快的速度给出答案。

       参考输入输出格式 输入由两部分组成。

       第一部分以N，M开始。N为问题涉及的人的个数(1 ≤ N ≤ 20000)。这些人的编号为1,2,3,…,N。下面有M行(1 ≤ M ≤ 1000000)，每行有两个数ai, bi，表示已知ai和bi是亲戚.

       第二部分以Q开始。以下Q行有Q个询问(1 ≤ Q ≤ 1 000 000)，每行为ci, di，表示询问ci和di是否为亲戚。

       对于每个询问ci, di，若ci和di为亲戚，则输出Yes，否则输出No。

**样例输入与输出**

输入relation.in

**10 7**

**2 4**

**5 7**

**1 3**

**8 9**

**1 2**

**5 6**

**2 3**

**3**

**3 4**

**7 10**

**8 9**

输出relation.out

**Yes**

**No**

**Yes**

**如果这道题目不用并查集，而只用链表或数组来存储集合，那么效率很低，肯定超时。**

**例程:**

```cpp
#include <iostream>
#include <cstdio>
 
using namespace std;
 
int father[20010]; //father[i]表示i的父亲
 
int Find(int a) //查找其父亲并压缩路径
{
    if(father[a] != a)
        father[a] = Find(father[a]);
    return father[a];
}
 
int main()
{
    int N,M,Q;
    int a,b,x,y;
    scanf("%d%d",&N,&M);
 
    //给每个元素建立一个集合
    for(int i = 1 ; i <= N ; ++i)
        father[i] = i;
 
    //合并
    for(int i = 0 ; i < M ; ++i)
    {
        scanf("%d%d",&a,&b);
        a = Find(a);
        b = Find(b);
        father[a] = b;
    }
 
    //查询
    scanf("%d",&Q);
    while(Q--)
    {
        scanf("%d%d",&x,&y);
        x = Find(x);
        y = Find(y);
        if(x == y)
            printf("Yes\n");
        else
            printf("No\n");
    }
 
    return 0;
}
```






The Suspects

|**Time Limit:** 1000MS||**Memory Limit:** 20000K|
|----|----|----|
|**Total Submissions:** 13311||**Accepted:** 6297|





Description


Severe acute respiratory syndrome (SARS), an atypical pneumonia of unknown aetiology, was recognized as a global threat in mid-March 2003. To minimize transmission to others, the best strategy is to separate the suspects from others. 

In the Not-Spreading-Your-Sickness University (NSYSU), there are many student groups. Students in the same group intercommunicate with each other frequently, and a student may join several groups. To prevent the possible transmissions of SARS, the NSYSU collects
 the member lists of all student groups, and makes the following rule in their standard operation procedure (SOP). 

Once a member in a group is a suspect, all members in the group are suspects. 

However, they find that it is not easy to identify all the suspects when a student is recognized as a suspect. Your job is to write a program which finds all the suspects.


Input


The input file contains several cases. Each test case begins with two integers n and m in a line, where n is the number of students, and m is the number of groups. You may assume that 0 < n <= 30000 and 0 <= m <= 500. Every student is numbered by a unique integer
 between 0 and n−1, and initially student 0 is recognized as a suspect in all the cases. This line is followed by m member lists of the groups, one line per group. Each line begins with an integer k by itself representing the number of members in the group.
 Following the number of members, there are k integers representing the students in this group. All the integers in a line are separated by at least one space. 

A case with n = 0 and m = 0 indicates the end of the input, and need not be processed.


Output


For each case, output the number of suspects in one line.


Sample Input
100 4
2 1 2
5 10 13 11 12 14
2 0 1
2 99 2
200 2
1 5
5 1 2 3 4 5
1 0
0 0

Sample Output
4
1
1
题目大意。一共有n个学生(编号0 至 n-1)，m个组，一个学生可以同时加入不同的组。现在有一种传染病，如果一个学生被感染，那么和他同组的学生都会被感染。现在已知0号学生被感染，问一共有多少个人被感染。

恩，就是基本的并查集操作。首先将每个学生都初始化为一个集合，然后将同组的学生合并，设置一个数组num[]来记录每个集合中元素的个数，最后只要输出0号学生所在集合中元素的个数即可。

第一次是16MS AC的，后来看到说可以通过压缩路径来优化，就是在查找到祖先节点之后将路径上所有它的子孙节点都连接到它上面，这样树的高度就大大降低了...使用之后果然，0MS AC...

### **CSDN天堂人间并查集学习笔记：**

l并查集：(union-find
 sets)

一种简单的用途广泛的集合. 并查集是若干个不相交集合，能够实现较快的合并和判断元素所在集合的操作，应用很多，如其求无向图的连通分量个数等。最完美的应用当属：实现Kruskar算法求最小生成树。

l并查集的精髓（即它的三种操作，结合实现代码模板进行理解）：

1、Make_Set(x) 把每一个元素初始化为一个集合

初始化后每一个元素的父亲节点是它本身，每一个元素的祖先节点也是它本身（也可以根据情况而变）。

2、Find_Set(x) 查找一个元素所在的集合

查找一个元素所在的集合，其精髓是找到这个元素所在集合的祖先！这个才是并查集判断和合并的最终依据。
判断两个元素是否属于同一集合，只要看他们所在集合的祖先是否相同即可。
合并两个集合，也是使一个集合的祖先成为另一个集合的祖先，具体见示意图

3、Union(x,y) 合并x,y所在的两个集合

合并两个不相交集合操作很简单：
利用Find_Set找到其中两个集合的祖先，将一个集合的祖先指向另一个集合的祖先。如图

![](http://images.cnblogs.com/cnblogs_com/cherish_yimi/%E9%9B%86%E5%90%88%E5%90%88%E5%B9%B6.jpg)





l并查集的优化

1、Find_Set(x)时 路径压缩
寻找祖先时我们一般采用递归查找，但是当元素很多亦或是整棵树变为一条链时，每次Find_Set(x)都是O(n)的复杂度，有没有办法减小这个复杂度呢？
答案是肯定的，这就是路径压缩，即当我们经过"递推"找到祖先节点后，"回溯"的时候顺便将它的子孙节点都直接指向祖先，这样以后再次Find_Set(x)时复杂度就变成O(1)了，如下图所示；可见，路径压缩方便了以后的查找。



2、Union(x,y)时 按秩合并
即合并的时候将元素少的集合合并到元素多的集合中，这样合并之后树的高度会相对较小。

![](http://images.cnblogs.com/cnblogs_com/cherish_yimi/%E8%B7%AF%E5%BE%84%E5%8E%8B%E7%BC%A9.jpg)

[天堂人间解题报告：](http://blog.csdn.net/tiantangrenjian/article/details/7084609)


```cpp
#include <iostream>  
using namespace std;  
  
#define MAXN 30000  
int father[MAXN],rank[MAXN];  
  
void Init(int n)  
{  
    int i;  
    for(i=0;i<n;i++)  
    {  
        father[i]=i;  
        rank[i]=1;  
    }  
}  
  
int Find_Set(int x)  
{  
    if(x!=father[x])  
    {  
        father[x]=Find_Set(father[x]);  
    }  
    return father[x];  
}  
  
void Union(int x,int y)  
{  
    x=Find_Set(x);  
    y=Find_Set(y);  
    if(x==y)return;  
    if(rank[x]>=rank[y])  
    {  
        father[y]=x;  
        rank[x]=rank[x]+rank[y];  
    }else  
    {  
        father[x]=y;  
        rank[y]=rank[y]+rank[x];  
    }  
}  
  
int main()  
{  
    freopen("in.txt","r",stdin);  
    int n,m,k,first,next;  
    int i,j;  
    while(1)  
    {  
        cin>>n>>m;  
        if(n==0 && m==0)break;  
        Init(n);  
      
        for(i=0;i<m;i++)  
        {  
            cin>>k>>first;  
            for(j=1;j<k;j++)  
            {  
                cin>>next;  
                Union(first,next);  
            }  
        }  
        cout<<rank[father[0]]<<endl;  
    }  
    return 0;  
}
```
[Slyar解题报告](http://www.slyar.com/blog/poj-1611-c.html)

```cpp
#include <stdio.h>
#include <stdlib.h>
 
/*
num[]存储节点所在集合元素的个数
father[]存储节点的父节点
*/
int num[30001], father[30001];
 
/* 初始化集合 */
void Make_Set(int x)
{
	father[x] = -1;
	num[x] = 1;
}
 
/* 查找x元素所在的集合，返回根节点 */
int Find_Set(int x)
{
	/* 保存待查找值x */
	int r = x, tmp; 
	/* 找到根节点r */
	while (father[r] != -1)
	{
		r = father[r];
	}
	/* 压缩路径，将路径上所有r的子孙都连接到r上 */
	while (x != r)
	{
		tmp = father[x];
		father[x] = r;
		x = tmp;
	}
	return x;
}
 
/* 合并a,b所在的集合 */
void Union_Set(int a, int b)
{
	a = Find_Set(a);
	b = Find_Set(b);
	/* 如果两个元素在同一个集合则不需要合并 */
	if (a == b) return;
	/* 将小集合合并到大集合中，更新集合个数 */
	if (num[a] <= num[b])
	{
		father[a] = b;
		num[b] += num[a];
	}
	else
	{
		father[b] = a;
		num[a] += num[b];
	}
}
 
int main()
{
	int a, b, n, m, t;
	int i, j;
	while (1)
	{
		scanf("%d%d", &n, &m);
		if (n + m == 0) break;
		for (i = 0; i < n; i++)
		{
			Make_Set(i);	
		}
		for (i = 0; i < m; i++)
		{
			scanf("%d", &t);
			scanf("%d", &a);
			for (j = 1; j < t; j++)
			{
				scanf("%d", &b);
				Union_Set(a, b);
			}
		}
		printf("%d\n", num[Find_Set(0)]);
	}
	return 0;
}
```
Slyar解法2
后来发现对于根节点来说，father[]本身就可以记录节点数目，只不过值是负的。所以又写了一个没有sun[]的版本。不过貌似负数运算比较慢，交了3次都是16MS...



```cpp
#include <stdio.h>
#include <stdlib.h>
 
/* father[]存储节点的父节点，对于根节点x，father[x]为子孙数 */
int father[30001];
 
/* 初始化集合 */
void Make_Set(int x)
{
	father[x] = -1;
}
 
/* 查找x元素所在的集合，返回根节点 */
int Find_Set(int x)
{
	/* 保存待查找值x */
	int r = x, tmp; 
	/* 找到根节点r */
	while (father[r] > 0)
	{
		r = father[r];
	}
	/* 压缩路径，将路径上所有r的子孙都连接到r上 */
	while (x != r)
	{
		tmp = father[x];
		father[x] = r;
		x = tmp;
	}
	return x;
}
 
/* 合并a,b所在的集合 */
void Union_Set(int a, int b)
{
	a = Find_Set(a);
	b = Find_Set(b);
	/* 如果两个元素在同一个集合则不需要合并 */
	if (a == b) return;
	/* 将小集合合并到大集合中(这里是负数比较)，更新集合个数*/
	if (father[a] < father[b])
	{
		father[a] += father[b];
		father[b] = a;
	}
	else
	{
		father[b] += father[a];
		father[a] = b;
	}
}
 
int main()
{
	int i, j;
	int a, b, n, m, t;
	while (1)
	{
		scanf("%d%d", &n, &m);
		if (n + m == 0) break;
		for (i = 0; i < n; i++)
		{
			Make_Set(i);	
		}
		for (i = 0; i < m; i++)
		{
			scanf("%d", &t);
			scanf("%d", &a);
			for (j = 1; j < t; j++)
			{
				scanf("%d", &b);
				Union_Set(a, b);
			}
		}
		/* 节点数目为负值 */
		printf("%d\n", -father[Find_Set(0)]);
	}
	return 0;
}
```
我的对此题的看法

【算法分析】


这道题的意思很简单,n个人编号,从0到n-1,这n个人分成m个集合(1个人可以参加不同的集合),求的就是最后所有和0号有关系的集合的人数.

如果这道题目不用并查集，而只用链表或数组来存储集合，那么效率很低，肯定超时.我们在题目给出的每个集合的人员编号时,进行并查操作,不过在进行合并操作时,合并的是两个集合的元素个数.最后0号元素所在的集合数目就是所求.
解题思路---->显然并查集。并查集的详细解释在可以点击 [并查集（不相交集合）](http://www.cnblogs.com/ktyanny/archive/2009/12/09/1620043.html)进行学习。采用num[]存储该集合中元素个数，并在集合合并时更新num[]即可。然后找出0所在的集合的根节点x，因此，num[x]就是answer了。



```cpp
#include <stdio.h>//by ktyanny
#include <iostream>
using namespace std;

const int MAXN = 30001; /*结点数目上线*/
int pa[MAXN];    /*p[x]表示x的父节点*/
int rank[MAXN];    /*rank[x]是x的高度的一个上界*/
int num[MAXN];/*num[]存储该集合中元素个数，并在集合合并时更新num[]即可*/

void make_set(int x)
{/*创建一个单元集*/
    pa[x] = x;
    rank[x] = 0;
    num[x] = 1;
}

int find_set(int x)
{/*带路径压缩的查找*/
    /*保存待查找的数*/
    int r = x, temp;
    /*找到根节点*/
    while(pa[r] != r) r = pa[r];
    while(x != r)
    {
        temp = pa[x];
        pa[x] = r;
        x = temp;
    }
    return x;
    //if(x != pa[x]) //注释掉的其实也是可以的，不过不想用递归来做啦
    //    pa[x] = find_set(pa[x]);
    //return pa[x];
}

/*按秩合并x，y所在的集合*/
void union_set(int x, int y)
{
    x = find_set(x);
    y = find_set(y);
    if(x == y)return ;
    if(rank[x] > rank[y])/*让rank比较高的作为父结点*/
    {
        pa[y] = x;
        num[x] += num[y];
    }
    else 
    {
        pa[x] = y;
        if(rank[x] == rank[y])
            rank[y]++;
        num[y] += num[x];
    }
}

//answer to 1611 
int main()
{
    int n, m, x, y, i, t, j;
    while(scanf("%d%d", &n, &m))
    {
        if(m==n && n == 0) break;
        if(m == 0)
        {
            cout << "1\n"; continue;
        }
        for(i = 0; i < n; i++)
            make_set(i);
        for(i = 0; i < m; i++)
        {
            scanf("%d", &t);
            scanf("%d", &x);
            for(j = 1; j < t; j++){
                scanf("%d", &y);
                union_set(x, y);
                x = y;
            }
        }
        x = find_set(0);/*找到0所在的树的树根*/
        //int ans = 0;
        //for(i = 0; i < n; i++)
        //    if(pa[i] == x)
        //        ans++;
        //cout << ans << endl;
        cout << num[x] << endl;
    }
    return 0;
}
```

**银河英雄传说**

**【问题描述】**

        公元五八○一年，地球居民迁移至金牛座α第二行星，在那里发表银河联邦创立宣言，同年改元为宇宙历元年，并开始向银河系深处拓展。

        宇宙历七九九年，银河系的两大军事集团在*巴米利恩星域*爆发战争。泰山压顶集团派宇宙舰队司令*莱因哈特*率领十万余艘战舰出征，气吞山河集团点名将*杨威利*组织麾下三万艘战舰迎敌。

        杨威利擅长排兵布阵，巧妙运用各种战术屡次以少胜多，难免恣生骄气。在这次决战中，他将巴米利恩星域战场划分成30000列，每列依次编号为1, 2, …, 30000。之后，他把自己的战舰也依次编号为1, 2, …, 30000，让第i号战舰处于第i列(i = 1, 2, …, 30000)，形成“一字长蛇阵”，诱敌深入。这是初始阵形。当进犯之敌到达时，杨威利会多次发布合并指令，将大部分战舰集中在某几列上，实施密集攻击。合并指令为M
 i j，含义为让第i号战舰所在的整个战舰队列，作为一个整体（头在前尾在后）接至第j号战舰所在的战舰队列的尾部。显然战舰队列是由处于同一列的一个或多个战舰组成的。合并指令的执行结果会使队列增大。

        然而，老谋深算的莱因哈特早已在战略上取得了主动。在交战中，他可以通过庞大的情报网络随时监听杨威利的舰队调动指令。

        在杨威利发布指令调动舰队的同时，莱因哈特为了及时了解当前杨威利的战舰分布情况，也会发出一些询问指令：C i j。该指令意思是，*询问电脑，杨威利的第i号战舰与第j号战舰当前是否在同一列中，如果在同一列中，那么它们之间布置有多少战舰。*

        作为一个资深的高级程序设计员，你被要求编写程序分析杨威利的指令，以及回答莱因哈特的询问。

        最终的决战已经展开，银河的历史又翻过了一页……

**【输入文件】**

输入文件galaxy.in的第一行有一个整数T（1<=T<=500,000），表示总共有T条指令。

以下有T行，每行有一条指令。指令有两种格式：

1.        M  i  j  ：i和j是两个整数（1<=i , j<=30000），表示指令涉及的战舰编号。该指令是莱因哈特窃听到的杨威利发布的舰队调动指令，并且保证第i号战舰与第j号战舰不在同一列。

2.        C  i  j  ：i和j是两个整数（1<=i , j<=30000），表示指令涉及的战舰编号。该指令是莱因哈特发布的询问指令。

**【输出文件】**

输出文件为galaxy.out。你的程序应当依次对输入的每一条指令进行分析和处理：

如果是杨威利发布的舰队调动指令，则表示舰队排列发生了变化，你的程序要注意到这一点，但是不要输出任何信息；

如果是莱因哈特发布的询问指令，你的程序要输出一行，仅包含一个整数，表示在同一列上，第i号战舰与第j号战舰之间布置的战舰数目。如果第i号战舰与第j号战舰当前不在同一列上，则输出-1。

**【样例输入】**

4

M 2 3

C 1 2

M 2 4

C 4 2

**【样例输出】**

-1

1

**【样例说明】**

战舰位置图：表格中阿拉伯数字表示战舰编号
||第一列|第二列|第三列|第四列|……|
|----|----|----|----|----|----|
|初始时|1|2|3|4|……|
|M 2 3|1||32|4|……|
|C 1 2|1号战舰与2号战舰不在同一列，因此输出-1| | | | |
|M 2 4|1|||432|……|
|C 4 2|4号战舰与2号战舰之间仅布置了一艘战舰，编号为3，输出1| | | | |

**【算法分析】**

同一列的战舰组成一个并查集,在集合中,我们以当前列的第一艘战舰作为集合的代表元.并查集的数据类型采用树型,树的根结点即为集合的代表元.为了查询的效率达到最优,我们进行了路径压缩的优化:首先找到树根,然后将路径上所有结点的父结点改为根,使得树的深度为1.

        问题是,题目不仅要求判别两个结点是否在同一个集合(即两艘战舰是否在同一列),而且还要求计算结点在有序集合的位置(即每一艘战舰相隔列的第一艘战舰几个位置),**我们增加了一个数组behind[x],记录战舰x在例中的相对位置.**

查找一个元素x所在集合的代表元时，先从x沿着父亲节点找到这个集合的代表元root，然后再从x开始一次到root的遍历，累计其间经过的每一个子结点的behind值,其和即为behind[i].,如下图所示:

![](https://p-blog.csdn.net/images/p_blog_csdn_net/fisher_jiang/251.bmp)



       按照题意,合并指令Mxy,含义是让战舰x 所在的整个战舰队列,作为一个整体(头在前,尾在后)接至战舰y所在的战舰队列的尾部,显然两个队列合并成同一列后,其集合代表元为结点y所在的树的根结点fy,x所在的树的根结点fx,合并后,fx的相对位置为合并前y所在集合的结点数, behind[fx]=num[fy],新集合的结点数为原来两个集合结点数的和 num[fy]+=num[fx].
 则如果战舰x和战舰y在同一列,则他们相隔

|behind[x]-behind[y]|-1艘战舰.如下图:



![](https://p-blog.csdn.net/images/p_blog_csdn_net/fisher_jiang/252.bmp)
代码：

```cpp
#include<iostream>
#include<cstdio>
#include<cmath>
#include<cstring>
using namespace std;
const int size=30001;
int pre[size],num[size],behind[size];//behind[x]战舰x在列中的相对位置
int n,m,k;
void makeset(int x)
 {
     pre[x]=-1;
     num[x]=1;
 }
int find(int x)//查找结点x所在树的根结点,并对该树进行路径压缩
 {
     int r=x;
     int j;
     while(pre[r]!=-1)//找出结点x所在树的根结点r
      r=pre[r];
     while(x!=r)
      {
          int q=pre[x];//路径压缩
          pre[x]=r;
          j=q;
          do{//迭代求出路径上每一个子结点相对于r的相对位置
              behind[x]+=behind[j];
              j=pre[j];
            }while (j!=-1);   
          x=q;
      }
    return r;      
 }    
void MoveShip(int a,int b)
{
    int t1,t2;
    t1=find(a);//计算a所在树的根结点t1
    t2=find(b);//计算b所在树的根结点t2
    pre[t1]=t2;//将t1的父结点设为t2
    behind[t1]=num[t2];//计算t1的相对位置为num[t2]
    num[t2]+=num[t1]; //计算新集合的结点数
 
}
 
void CheckShip(int x,int y)
 {
     int f1,f2;
     f1=find(x);
     f2=find(y);
     if(f1!=f2)
      cout<<-1<<endl;
     else
      cout<<abs(behind[x]-behind[y])-1<<endl;
 }
int main()
 {
     int n,x,y;
     char ch;
     while(cin>>n)
      {
          for(int i=1;i<size;i++)
           {
               makeset(i);
           }
          memset(behind,0,sizeof(behind));   
          while(n--)
           {
               cin>>ch>>x>>y;
               if(ch=='M')
                 MoveShip(x,y); //处理合并指令
               else if(ch=='C')
                 CheckShip(x,y);  //处理询问指令
           }   
      } 
   return 0;    
 }
```







