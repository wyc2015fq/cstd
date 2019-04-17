# HDU 6447YJJ's Salesman CCPC预选赛 - 安得广厦千万间的博客 - CSDN博客





2018年08月26日 11:45:13[独-](https://me.csdn.net/qq_41713256)阅读数：55








链接：[http://hdu.hustoj.com/showproblem.php?pid=6447](http://hdu.hustoj.com/showproblem.php?pid=6447)

题意：假如一个人站在（x，y）上，那么他可以往（x+1，y），（x，y+1），（x+1，y+1）三个方向走，起初在（0,0）上，地图上有n个物品，若要得到（x，y）的价值，就必须从（x-1，y-1）走来。求最大价值。

首先每个点的范围很大，需要离散化。对于（x，y）这个点的价值，在他左上方的位置走来就可以得到。于是，我们先对每个点先以x升序，再以y升序。dp【i】表示到这个点并拿这个点的最大价值，那么毫无疑问就是这个点的价值加上他的坐上方的最大价值，这一点可以有树状数组或者线段树来优化。



```
#include <bits/stdc++.h>
using namespace std;
typedef long long LL;
const int inf=999999999;
const int maxn = 1e5+5;
const LL  mod = 1e9+7;
struct node{
  int x,y,v;
  bool friend operator<(node a,node b)
  {
      if( a.x==b.x ) return a.y<b.y;
      return a.x<b.x;
  }
}a[maxn];
int y[maxn],c[maxn],dp[maxn];
void add(int x,int n,int v)
{
    for(;x<=n;x+=x&(-x))
        c[x]=max( c[x], v );
}
int get(int x)
{
    int res=0;
    for(;x;x-=x&(-x))
        res= max( res, c[x] );
    return res;
}
int main()
{
    int T; scanf( "%d", &T );
    while( T-- )
    {
        int n; scanf( "%d", &n );
        for(int i=1;i<=n;i++)
        {
            scanf( "%d%d%d", &a[i].x, &a[i].y, &a[i].v );
            y[i]=a[i].y;
        }
        sort(y+1,y+n+1);
        for(int i=1;i<=n;i++)
            a[i].y=lower_bound(y+1,y+1+n,a[i].y)-y;
        sort(a+1,a+1+n);
        memset( dp, 0, sizeof(dp) );
        memset( c, 0, sizeof(c) );
        dp[1]=a[1].v;
        int ans=dp[1];
        int now=1;
        for(int i=2;i<=n;i++)
        {
            while( a[i].x!=a[now].x ) // 我们要得到a[i]这个点的坐上的最大值，要这样更新，
            {
                add(a[now].y,n,dp[now]); // 因为我们对x升序了，那么之前与x不等的一定在他的上方。
                now++;
            }
            dp[i]=get(a[i].y-1)+a[i].v;  // 得到a[i].y左边的最大值
            ans=max( ans,dp[i] );
        }
        printf( "%d\n", ans );
    }
	return 0;
}
```





