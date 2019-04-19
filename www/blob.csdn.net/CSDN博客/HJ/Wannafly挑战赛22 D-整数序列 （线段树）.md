# Wannafly挑战赛22 D-整数序列 （线段树） - HJ - CSDN博客
2018年08月19日 10:28:16[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：140
个人分类：[====ACM====																[--------数据结构--------																[线段树																[---------OJ---------																[牛客网](https://blog.csdn.net/feizaoSYUACM/article/category/7556010)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308065)](https://blog.csdn.net/feizaoSYUACM/article/category/6130307)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
[https://www.nowcoder.com/acm/contest/160/D](https://www.nowcoder.com/acm/contest/160/D)
做法：用线段树维护，对于树上的每个叶子结点，维护一个复数结构体 cos(a[i] + sin(a[i])*i ,每次增加一个v，则相当于乘以一个复数cos(v) + sin(v)*i ， 其它的求和即常规操作即可。
```cpp
#include<iostream>
#include<cstdio>
#include<cmath>
using namespace std;
typedef long long ll;
const int maxn=2e5+10;
#define lson l,mid
#define rson mid+1,r
struct Complex{     
    double sin,cos;   //定义复数结构体，cos(x)+sin(x)*i 其中i^i=-1 
};
int a[maxn];
Complex add[maxn<<2],sum[maxn<<2];
ll ans[maxn<<2];   //ans为求和数组 即非叶子结点维护的一个值 
Complex operator + (Complex a,Complex b)  //定义复数相加 
{
    return Complex({a.sin+b.sin,a.cos+b.cos});
}
void Multi(Complex &rt,Complex p)  //复数相乘 
{
    rt=Complex({rt.sin*p.cos+rt.cos*p.sin,rt.cos*p.cos-rt.sin*p.sin});
}
void build(int l,int r,int rt)
{
    add[rt].cos=1;
    if(l==r)
    {
        sum[rt]=Complex({sin(a[l]),cos(a[l])});
        return ;
    }
    int mid=(l+r)>>1;
    build(lson,rt<<1);
    build(rson,rt<<1|1);
    sum[rt]=sum[rt<<1]+sum[rt<<1|1]; 
}
void update(int l,int r,int x,int y,int val,int c)
{
    if(x<=l&&r<=y)
    {
        Complex p=Complex({sin(val),cos(val)});
        ans[c]+=val;
        Multi(add[c],p),Multi(sum[c],p); 
        return ;
    }
    int mid=(l+r)>>1;
    if(x<=mid) update(lson,x,y,val,c<<1);
    if(y>mid) update(rson,x,y,val,c<<1|1);
    sum[c]=sum[c<<1]+sum[c<<1|1];
    Multi(sum[c],add[c]);
}
Complex query(int l,int r,int x,int y,int c)
{
    if(x<=l&&r<=y) return sum[c];
    int mid=(l+r)>>1;
    Complex ans=Complex({0,0});
    if(x<=mid) ans=query(lson,x,y,c<<1);
    if(y>mid) ans=ans+query(rson,x,y,c<<1|1);
    Multi(ans,add[c]);
    return ans;
}
int main()
{
    int n,q,op,x,y,c;
    while(cin>>n)
    {
        for(int i=1;i<=n;i++)
          cin>>a[i];
        build(1,n,1);
        cin>>q;
        while(q--)
        {
            cin>>op>>x>>y;
            if(op==1)
              cin>>c,update(1,n,x,y,c,1);
            else
              printf("%.1lf\n",query(1,n,x,y,1).sin); 
        }
    }
    return 0;
}
```
