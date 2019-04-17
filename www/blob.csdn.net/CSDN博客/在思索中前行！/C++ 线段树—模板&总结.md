# C++ 线段树—模板&总结 - 在思索中前行！ - CSDN博客





2015年04月29日 23:15:18[_Tham](https://me.csdn.net/txl16211)阅读数：4789







在信息学竞赛中，经常遇到这样一类问题：这类问题通常可以建模成数轴上的问题或是数列的问题，具体的操作一般是每次对数轴上的一个区间或是数列中的连续若干个数进行一种相同的处理。常规的做法一般依托于线性表这种数据结构，导致了处理只能针对各个元素逐个进行，因此算法的效率较低。
线段树是一种能够有效处理区间操作的高级数据结构，利用这种数据结构，我们能够设计出针对上述问题更加高效的算法。

线段树的题目通常比较明显，一般一个很明显的特征是m次对某一区间长度的查询。或者是修改。所以我们通常需要的只是将线段树的模型稍加修改，进而套入题目中即可。

模板：

对单个点的修改，和对一段区间的查询：



```cpp
//线段树单个点修改&区间查询
#include<iostream>
#include<iomanip>
#include<cstring>
#include<climits>
#include<cmath>
#include<cstdio>
#include<cstdlib>
#include<queue>
#include<vector>
#include<map>
#include<algorithm>
#include<string>
#include<memory>
using namespace std;

const int e=100006;
struct qq
{
    int maxx;
}tree[4*e];//线段树要开4倍的点的个数
int n,t,a,b;

void updata(int l,int r,int root)
{
    if(r<a || l>a)    return;
    if(r==l)
    {
        tree[root].maxx=b;
        return;
    }

    int mid=(l+r)/2;
    updata(l,mid,root*2);
    updata(mid+1,r,root*2+1);
    tree[root].maxx=max(tree[root*2].maxx,tree[root*2+1].maxx);
}


int search(int l,int r,int root)
{
    if(l>b || r<a)    return(-999999999);

    if(l>=a && r<=b)    return(tree[root].maxx);

    int mid=(l+r)/2;
    return(max( search(l,mid,root*2), search(mid+1,r,root*2+1)));
}

int main()
{
    memset(tree,0,sizeof(tree));
    cin>>n;

    for(int i=0;i<n;i++)
    {
        scanf("%d%d%d",&t,&a,&b);

        if(t==1)    updata(1,n,1);//将点a的值改为b
        if(t==2)    cout<< search(1,n,1) << endl;//查找区间a(含)到b(含)的最大值;
    }

    return 0;
}
```



对一段区间的修改和查询：




```cpp
//线段树 区间 修改&查询
#include<iostream>
#include<iomanip>
#include<cstring>
#include<climits>
#include<cmath>
#include<cstdio>
#include<cstdlib>
#include<queue>
#include<vector>
#include<map>
#include<algorithm>
#include<string>
#include<memory>
using namespace std;

const int e=100006;
struct qq
{
    int maxx,delta;
}tree[4*e];//线段树要开4倍的点的个数
int n,t,a,b;

void updata(int l,int r,int root)//更新数据
{
    if(b<l || r<a)    return;

    if(l>=a && r<=b)//这句是核心，若当前区间包含于修改区间，就不往下传,(未传到叶子节点)；
    {
        tree[root].maxx++;
        tree[root].delta++;
        return;
    }

    int mid=(l+r)/2,delta=tree[root].delta;
    tree[root*2].maxx+=delta;
	tree[root*2].delta+=delta;
    tree[root*2+1].maxx+=delta;
	tree[root*2+1].delta+=delta;
    tree[root].delta=0;//这句很关键，根的偏移量传递到子树后清零
    updata(l,mid,root*2);
    updata(mid+1,r,root*2+1);
    tree[root].maxx=max( tree[root*2].maxx, tree[root*2+1].maxx);
    return;
}

int search(int l,int r,int root)
{
    if(l>b || r<a)    return(-99999999);

    if(l>=a && r<=b)    return(tree[root].maxx);

    int mid=(l+r)/2,delta=tree[root].delta;
    tree[root*2].maxx+=delta;
	tree[root*2].delta+=delta;
    tree[root*2+1].maxx+=delta;
	tree[root*2+1].delta+=delta;
    tree[root].delta=0;      //这句很关键，根的偏移量传递到子树后清零
    return(max( search(l,mid,root*2), search(mid+1,r,root*2+1)));
}

int main()
{
    memset(tree,0,sizeof(tree));
    cin>>n;

    for(int i=0;i<n;i++)
    {
        scanf("%d%d%d",&t,&a,&b);

        if(t==1)    updata(1,n,1);
        if(t==2)    cout<< search(1,n,1) << endl;
    }

    return 0;
}
```
以下是做线段树题目时的易错点：

1、开线段树的结构体时一定要开到4倍的点的大小；

2、写区间求和时的题目时search（）函数的返回值在不再区间里时返回0，而区间求最小值时返999999999，最大值时返回-999999999；（int时）一定要是九个9，不然有些极限数据会卡范围；

3、写修改单个点的值和一个区间的值的函数最好分开，一个是节省时间，还有可以防止代码混乱而出错；

4、Search（）&updata（）函数中判断区间范围的if语句中l，r和查找的区间a，b的关系容易出错；

5、注意根节点和子节点的关系，特别是+1的问题，归结为一句话就是：若加都加，若不加都不加即：mid=(l+r)；左子树：l~mid（不加一），根为root*2；右子树：r~mid+1（加一了），根为root*2+1（也加一）；
6、权值是在边上还是点上，这两种关系代码判断上有不同；

7、结构体赋初值的时候也要注意，根据所求的是最大还是最小还是和来判断；

8、线段树的左端点在数组中的下标一定要是1，而不是0。






