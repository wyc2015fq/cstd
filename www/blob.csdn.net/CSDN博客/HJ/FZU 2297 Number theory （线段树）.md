# FZU 2297 Number theory （线段树） - HJ - CSDN博客
2018年08月08日 10:25:10[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：164
[http://acm.fzu.edu.cn/problem.php?pid=2297](http://acm.fzu.edu.cn/problem.php?pid=2297)
题目大意：
有一个默认初始值为1的x，给出一系列数据应用，每次输入一个大写字母’M’或者’N’以及一个整数。当大写字母为’M’时，表示这个x乘以后面的整数；当大写字母为’N’时，表示x整除后面整数所指的查询序号所输入的整数，问每次输入，输出的x的值为多少？答案对1e9+7取膜。
解题思路：
常规的乘除明显是不行的，因为数据结果很大，需要对1e9+7取膜，整除势必会丢失结果的精确性。但是每次又需要在最多O(log)的时间内求出值，怎么办呢？
打完比赛才知道可以用线段树，每个父节点维护左右两颗子树的乘积，如果需要整除的话，那么就查询到那个数，把值改为1，回溯的时候向上更新即可。
```
#include<iostream>
#include<cstdio>
using namespace std;
typedef long long ll;
const int maxn=1e5;
ll tree[maxn*4];
ll value,mod;
void update(int l,int r,int root,int k)
{
    if(l==r)
    {
        tree[root]=value;
        return ;
    }
    int m=(l+r)/2;
    if(k<=m)
      update(l,m,root*2,k);
    else
      update(m+1,r,root*2+1,k);
    tree[root]=tree[root*2]*tree[root*2+1]%mod;
}
int main()
{
    int t,y,x,q;
    scanf("%d",&t);
    while(t--)
    {
        scanf("%d%lld",&q,&mod);
        for(int i=1;i<=4*maxn;i++)
          tree[i]=1;
        for(int i=1;i<=q;i++)
        {
            char ch[10];
            scanf("%s%d",ch,&x);
            if(ch[0]=='M')
            {
                value=x;
                update(1,maxn,1,i);
                printf("%lld\n",tree[1]);
            }
            else
            {
                value=1;
                update(1,maxn,1,x);
                printf("%lld\n",tree[1]);
            }
        }
    }
    return 0;
}
```
