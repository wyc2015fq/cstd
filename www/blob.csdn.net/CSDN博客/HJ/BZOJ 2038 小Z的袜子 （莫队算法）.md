# BZOJ 2038 小Z的袜子 （莫队算法） - HJ - CSDN博客
2018年08月09日 14:36:04[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：26
个人分类：[====ACM====																[莫队																[--------数据结构--------																[---------OJ---------																[杂题](https://blog.csdn.net/feizaoSYUACM/article/category/6684121)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6130307)](https://blog.csdn.net/feizaoSYUACM/article/category/7921726)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
[https://www.lydsy.com/JudgeOnline/problem.php?id=2038](https://www.lydsy.com/JudgeOnline/problem.php?id=2038)
莫队板子题，测一下板子。
简单莫队介绍：
```
1、技巧： 离线处理查询，让查询“有序”；
2、预处理：将序列分块，然后根据块将查询安排进去；
3、O(n)遍历：将所有查询利用一条总线串起来，每次相邻两个查询所用时间复杂度为 |L - L'| + |R - R'|
```
```
#include<iostream> 
#include<algorithm>
#include<cstdio>
#include<cmath>
using namespace std;
typedef long long ll;
const int maxn=1e5+10;
struct node{
    int l,r,id;
    ll a,b;
}q[maxn];
int n,m,pos[maxn],a[maxn];
ll s[maxn],ans;
bool cmp1(node a,node b)    //分块排序 
{
    return pos[a.l]==pos[b.l]?a.r<b.r:pos[a.l]<pos[b.l];
}
bool cmp2(node a,node b)   //将序号还原 
{
    return a.id<b.id;
}
void update(int i,int val)   //每次查询的更新操作 
{
    ans-=s[a[i]]*s[a[i]];
    s[a[i]]+=val;
    ans+=s[a[i]]*s[a[i]];
}
ll gcd(ll x,ll y)
{
    return x%y?gcd(y,x%y):y;
}
int main()
{
    scanf("%d%d",&n,&m);
    for(int i=1;i<=n;i++)
      scanf("%d",&a[i]);
    int block=int(sqrt(n+0.5));
    for(int i=1;i<=n;i++)   //将序列分块 
      pos[i]=(i-1)/block+1;
    for(int i=1;i<=m;i++)
    {
        scanf("%d%d",&q[i].l,&q[i].r);
        q[i].id=i;
    }
    sort(q+1,q+m+1,cmp1);   //将所有查询分块排序 
    for(int i=1,l=1,r=0;i<=m;i++)
    {
        while(r<q[i].r)
          update(r+1,1),r++;
        while(r>q[i].r)
          update(r,-1),r--;
        while(l<q[i].l)
          update(l,-1),l++;
        while(l>q[i].l)
          update(l-1,1),l--;
        if(q[i].l==q[i].r)
        {
            q[i].a=0,q[i].b=1;
            continue;
        }
        q[i].a=ans-(q[i].r-q[i].l+1);  //每次查询更新分子即可 
        q[i].b=(ll)(q[i].r-q[i].l+1)*(q[i].r-q[i].l);
        ll k=gcd(q[i].a,q[i].b);
        q[i].a/=k,q[i].b/=k;
    }
    sort(q+1,q+m+1,cmp2);   //将查询的次序还原并输出 
    for(int i=1;i<=m;i++)
      printf("%lld/%lld\n",q[i].a,q[i].b);
    return 0;
}
```
