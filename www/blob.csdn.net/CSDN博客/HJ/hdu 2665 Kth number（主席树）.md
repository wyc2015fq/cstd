# hdu 2665 Kth number（主席树） - HJ - CSDN博客
2018年08月16日 15:11:16[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：65
[http://acm.hdu.edu.cn/showproblem.php?pid=2665](http://acm.hdu.edu.cn/showproblem.php?pid=2665)
入门题的主席树
代码风格一：
```cpp
#include<iostream>
#include<algorithm>
#include<cstring>
#include<cstdio>
using namespace std;
#define lson l,m
#define rson m+1,r
const int maxn=1e5+10;
int L[maxn<<5],R[maxn<<5],sum[maxn<<5];
int tot,a[maxn],s[maxn],h[maxn];
int build(int l,int r)
{
    int rt=++tot;
    sum[rt]=0;
    if(l<r)
    {
        int m=(l+r)>>1;
        L[rt]=build(lson),R[rt]=build(rson); 
    }
    return rt;
}
int update(int pre,int l,int r,int x)
{
    int rt=++tot;
    L[rt]=L[pre],R[rt]=R[pre],sum[rt]=sum[pre]+1;
    if(l<r)
    {
        int m=(l+r)>>1;
        if(x<=m) L[rt]=update(L[pre],lson,x);
        else R[rt]=update(R[pre],rson,x);
    }
    return rt;
}
int query(int u,int v,int l,int r,int k)
{
    if(l>=r) return l;
    int m=(l+r)>>1,num=sum[L[v]]-sum[L[u]];
    if(num>=k) return query(L[u],L[v],lson,k);
    else return query(R[u],R[v],rson,k-num);
}
int main()
{
    int t,n,m,l,r,k;
    scanf("%d",&t);
    while(t--)
    {
        tot=0;
        scanf("%d%d",&n,&m);
        for(int i=1;i<=n;i++)
          scanf("%d",&a[i]),h[i]=a[i];
        sort(h+1,h+n+1);
        int d=unique(h+1,h+n+1)-h-1;  //离散化处理 找到值的范围，即[1,d]; 
        s[0]=build(1,d);   //根据值进行建树
        for(int i=1;i<=n;i++)
        {
            int x=lower_bound(h+1,h+d+1,a[i])-h;
            s[i]=update(s[i-1],1,d,x);  
        }
        while(m--)
        {
            scanf("%d%d%d",&l,&r,&k);
            printf("%d\n",h[query(s[l-1],s[r],1,d,k)]);
        }
    }
    return 0;
}
```
