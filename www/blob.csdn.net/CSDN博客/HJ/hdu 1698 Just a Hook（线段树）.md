# hdu  1698  Just a Hook（线段树） - HJ - CSDN博客
2017年01月26日 22:46:18[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：203
个人分类：[====ACM====																[---------OJ---------																[HDU																[--------数据结构--------																[线段树](https://blog.csdn.net/feizaoSYUACM/article/category/6308065)](https://blog.csdn.net/feizaoSYUACM/article/category/6130307)](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Problem Description 
In the game of DotA, Pudge’s meat hook is actually the most horrible thing for most of the heroes. The hook is made up of several consecutive metallic sticks which are of the same length.
![这里写图片描述](http://acm.hdu.edu.cn/data/images/C116-1010-1.JPG)
Now Pudge wants to do some operations on the hook.
Let us number the consecutive metallic sticks of the hook from 1 to N. For each operation, Pudge can change the consecutive metallic sticks, numbered from X to Y, into cupreous sticks, silver sticks or golden sticks. 
The total value of the hook is calculated as the sum of values of N metallic sticks. More precisely, the value for each kind of stick is calculated as follows:
For each cupreous stick, the value is 1. 
For each silver stick, the value is 2. 
For each golden stick, the value is 3.
Pudge wants to know the total value of the hook after performing the operations. 
You may consider the original hook is made up of cupreous sticks.
Input 
The input consists of several test cases. The first line of the input is the number of the cases. There are no more than 10 cases. 
For each case, the first line contains an integer N, 1<=N<=100,000, which is the number of the sticks of Pudge’s meat hook and the second line contains an integer Q, 0<=Q<=100,000, which is the number of the operations. 
Next Q lines, each line contains three integers X, Y, 1<=X<=Y<=N, Z, 1<=Z<=3, which defines an operation: change the sticks numbered from X to Y into the metal kind Z, where Z=1 represents the cupreous kind, Z=2 represents the silver kind and Z=3 represents the golden kind.
Output 
For each case, print a number in a line representing the total value of the hook after the operations. Use the format in the example.
Sample Input
1 
10 
2 
1 5 2 
5 9 3
Sample Output
Case 1: The total value of the hook is 24.
Source 
2008 “Sunline Cup” National Invitational Contest 
题目大意： 
一段线段由n条小线段组成，每次操作把一个区间的小线段变成金银铜之一（金的价值为3，银为2，铜为1），最初可当做全为铜；最后求这条线段的总价值。
区间更新： 是指更新某个区间内的叶子节点的值，因为涉及到的叶子节点不止一个，而叶子节点会影响其相应的非叶父节点，那么回溯需要更新的非叶子节点也会有很多，如果一次性更新完，操作的时间复杂度肯定不是O(lgn)，例如当我们要更新区间[0,3]内的叶子节点时，需要更新出了叶子节点3,9外的所有其他节点。为此引入了线段树中的延迟标记概念，这也是线段树的精华所在。
延迟标记：每个节点新增加一个标记，记录这个节点是否进行了某种修改(这种修改操作会影响其子节点)，对于任意区间的修改，我们先按照区间查询的方式将其划分成线段树中的节点，然后修改这些节点的信息，并给这些节点标记上代表这种修改操作的标记。在修改和查询的时候，如果我们到了一个节点p，并且决定考虑其子节点，那么我们就要看节点p是否被标记，如果有，就要按照标记修改其子节点的信息，并且给子节点都标上相同的标记，同时消掉节点p的标记。
注意：不要更新到叶子节点。（否则会超）
```cpp
#include<iostream>
#include<cstdio>
using namespace std;                 //区间更新，无需更新到叶子节点，防止超时 
#define max 100000
struct tree{
    int l,r,val;
}s[max*4];
void build(int left,int right,int root)
{
    s[root].l=left;
    s[root].r=right;
    s[root].val=1;
    if(left==right)
      return ;
    int mid=(left+right)/2;
    build(left,mid,root*2);
    build(mid+1,right,root*2+1);
} 
void insert(int left,int right,int root,int val)           
{
    if(s[root].val==val)                  //相同则不用修改了   
      return ;  
    if(s[root].l==left&&s[root].r==right)          //找到了区间，直接更新 
    {
        s[root].val=val;
        return ;
    }
    if(s[root].val!=-1)                  //如果该区间只有一种颜色            
    { 
        s[root*2].val=s[root*2+1].val=s[root].val;         //由于后面必定对子树操作，所以更新子树的值等于父亲的值 
        s[root].val=-1;                  //由于该区域颜色与修改不同，而不是给定区域，所以该区域必定为杂色 
    }
    //父区间为杂色时对所有子节点进行操作
    int mid=(s[root].l+s[root].r)/2;
    if(right<=mid)
      insert(left,right,root*2,val);
    else if(left>mid)
      insert(left,right,root*2+1,val);
    else
    {
        insert(left,mid,root*2,val);
        insert(mid+1,right,root*2+1,val);
    }   
}
int find(int root)                                    //区间求和 
{
    if(s[root].val!=-1)                              //纯色直接算这个区间 
      return (s[root].r-s[root].l+1)*s[root].val;
    else                                             //否则从左右子树去找 
      return find(root*2)+find(root*2+1);
}
int main()
{
    int t,n,m,a,b,c,tt=0;
    scanf("%d",&t);
    while(t--)
    {
        int ans=0;
        scanf("%d%d",&n,&m);
        build(1,n,1);
        for(int i=0;i<m;i++)
        {
            scanf("%d%d%d",&a,&b,&c);
            insert(a,b,1,c);
        }
        printf("Case %d: The total value of the hook is %d.\n",++tt,find(1));
    }
    return 0;
}
```
