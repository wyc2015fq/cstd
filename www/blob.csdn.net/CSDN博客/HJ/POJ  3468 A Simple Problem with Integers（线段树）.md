# POJ   3468  A Simple Problem with Integers（线段树） - HJ - CSDN博客
2016年08月18日 16:26:21[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：229
个人分类：[====ACM====																[---------OJ---------																[POJ																[--------数据结构--------																[线段树](https://blog.csdn.net/feizaoSYUACM/article/category/6308065)](https://blog.csdn.net/feizaoSYUACM/article/category/6130307)](https://blog.csdn.net/feizaoSYUACM/article/category/6308043)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Description
You have N integers, A1, A2, … , AN. You need to deal with two kinds of operations. One type of operation is to add some given number to each number in a given interval. The other is to ask for the sum of numbers in a given interval.
Input
The first line contains two numbers N and Q. 1 ≤ N,Q ≤ 100000. 
The second line contains N numbers, the initial values of A1, A2, … , AN. -1000000000 ≤ Ai ≤ 1000000000. 
Each of the next Q lines represents an operation. 
“C a b c” means adding c to each of Aa, Aa+1, … , Ab. -10000 ≤ c ≤ 10000. 
“Q a b” means querying the sum of Aa, Aa+1, … , Ab.
Output
You need to answer all Q commands in order. One answer in a line.
Sample Input
10 5 
1 2 3 4 5 6 7 8 9 10 
Q 4 4 
Q 1 10 
Q 2 4 
C 3 6 3 
Q 2 4
Sample Output
4 
55 
9 
15
Hint 
The sums may exceed the range of 32-bit integers
线段树的区间更新很技巧，需要用到lazy操作，首先记录当前需要更新的区间范围，到下次查询的时候，路过这个数据的时候顺带更新，这样就不用每次区间更新都去开一个循环加大复杂度了，代码如下：
```cpp
#include<iostream>
using namespace std;
#define imax 100005
int n,m,num[imax];
struct node{
    int l,r;
    __int64 add,sum;    //Sum为子树所有值的和，add为懒惰标记变量 
}f[imax*3];
void build(int t,int l,int r)
{
    f[t].l=l;
    f[t].r=r;
    f[t].add=0;      //lazy标记消除 
    if(l==r)
    { 
        f[t].sum=num[l];    //叶子节点的sum即为num[]值 
        return ;
    }
    int mid=(l+r)/2;
    build(t*2,l,mid);
    build(t*2+1,mid+1,r);
    f[t].sum=f[t*2].sum+f[t*2+1].sum;   //更新非叶子节点的sum值 
}
void Add(int t,int l,int r,int c)
{
    if(f[t].l==l&&f[t].r==r)
    {
        f[t].add+=c;      //lazy标记，因为有可能一个节点有多次标记，所以是累加而不是赋值 
        return ;
    }
    f[t].sum+=c*(r-l+1);    //lazy标记下的总和add乘以子树个数r-l+1，更新到当前节点的sum中 
    int mid=(f[t].l+f[t].r)/2;
    if(r<=mid)
      Add(t*2,l,r,c);
    else
    {
        if(l>mid)
          Add(t*2+1,l,r,c);
        else
        {
            Add(t*2,l,mid,c);
            Add(t*2+1,mid+1,r,c);
        }
    }
}
__int64 query(int t,int l,int r)
{
    if(f[t].l==l&&f[t].r==r)
      return f[t].sum+f[t].add*(f[t].r-f[t].l+1);   //返回当前节点的sum值与还未更新的add值之和 
    int mid=(f[t].l+f[t].r)/2;
    f[2*t].add+=f[t].add;     
    f[2*t+1].add+=f[t].add;                   //往两边子树更新add值
    f[t].sum+=f[t].add*(f[t].r-f[t].l+1);    //更新当前节点的sum值 
    f[t].add=0;       //更新完之后，当前节点的lazy标记消除 然后继续往下左右子树更新 
    if(r<=mid)
      return query(t*2,l,r);
    else
    {
        if(l>mid)
          return query(t*2+1,l,r);
        else
          return query(t*2,l,mid)+query(t*2+1,mid+1,r);
    }
}
int main()
{
    while(cin>>n>>m)
    {
        for(int i=1;i<=n;i++)
          cin>>num[i];
        build(1,1,n);
        int x,y,c;
        char ch[10];
        for(int i=0;i<m;i++)
        {
            cin>>ch;
            if(ch[0]=='Q')
            {
                cin>>x>>y;
                cout<<query(1,x,y)<<endl;
            }
            else
            {
                cin>>x>>y>>c;
                Add(1,x,y,c);
            }
        }
    }
    return 0;
}
```
