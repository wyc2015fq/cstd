# POJ 2104 K-th Number（主席树） - HJ - CSDN博客
2018年08月16日 15:13:05[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：67
[http://poj.org/problem?id=2104](http://poj.org/problem?id=2104)
入门题的主席树
代码风格二：
```cpp
#include<iostream>
#include<algorithm>
#include<vector>
#include<cstdio>
#include<cstring>
using namespace std;
typedef long long ll;
const int maxn=1e5+10;
int n,m,cnt,root[maxn],a[maxn],x,y,k;
struct node{
    int l,r,sum;
}T[maxn*40];
vector<int> v;
void update(int l,int r,int &x,int y,int pos)
{
    T[++cnt]=T[y],T[cnt].sum++,x=cnt;
    if(l==r) return ;
    int mid=(l+r)/2;
    if(mid>=pos) update(l,mid,T[x].l,T[y].l,pos);
    else update(mid+1,r,T[x].r,T[y].r,pos);
}
int query(int l,int r,int x,int y,int k)
{
    if(l==r) return l;
    int mid=(l+r)/2,sum=T[T[y].l].sum-T[T[x].l].sum;
    if(sum>=k) return query(l,mid,T[x].l,T[y].l,k);
    else return query(mid+1,r,T[x].r,T[y].r,k-sum);
}
int main()
{
    while(~scanf("%d%d",&n,&m))
    { 
        v.clear();
        for(int i=1;i<=n;i++)
          scanf("%d",&a[i]),v.push_back(a[i]);
        sort(v.begin(),v.end());
        v.erase(unique(v.begin(),v.end()),v.end());
        for(int i=1;i<=n;i++)
        {
            int pos=lower_bound(v.begin(),v.end(),a[i])-v.begin()+1;
            update(1,n,root[i],root[i-1],pos);
        }
        while(m--)
        {
            scanf("%d%d%d",&x,&y,&k);
            printf("%d\n",v[query(1,n,root[x-1],root[y],k)-1]);
        } 
    }
    return 0;   
}
```
