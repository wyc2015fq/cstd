# POJ 3264 Balanced Lineup（ST表） - HJ - CSDN博客
2018年08月16日 15:17:32[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：51
个人分类：[====ACM====																[--------数据结构--------																[RMQ+LCA](https://blog.csdn.net/feizaoSYUACM/article/category/7611705)](https://blog.csdn.net/feizaoSYUACM/article/category/6130307)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
[http://poj.org/problem?id=3264](http://poj.org/problem?id=3264)
RMQ问题，求每次询问的区间[L,R]中的最大值与最小值的差
可以用ST表解决；
```cpp
#include<iostream>
#include<cstdio>
using namespace std;
const int N=5e5+10;
int log2[N];
int n,m,x,y,a[N],f[N][20],g[N][20];
void Init_RMQ()
{
    log2[1]=0;
    for(int i=2;i<N;i++)
      log2[i]=log2[i-1]+!(i&(i-1));
    for(int i=1;i<=n;i++)
      f[i][0]=g[i][0]=a[i];
    for(int j=1;(1<<j)<=n;j++)
      for(int i=1;i+(1<<j)-1<=n;i++)
      {
          f[i][j]=max(f[i][j-1],f[i+(1<<j-1)][j-1]);
          g[i][j]=min(g[i][j-1],g[i+(1<<j-1)][j-1]);
      }
}
int getmax(int l,int r)
{
    int k=log2[r-l+1];
    return max(f[l][k],f[r-(1<<k)+1][k]);
} 
int getmin(int l,int r)
{
    int k=log2[r-l+1];
    return min(g[l][k],g[r-(1<<k)+1][k]);
}
int main()
{
    while(~scanf("%d%d",&n,&m))
    {
        for(int i=1;i<=n;i++)
          scanf("%d",&a[i]);
        Init_RMQ();
        for(int i=1;i<=m;i++)
        {
            scanf("%d%d",&x,&y);
            printf("%d\n",getmax(x,y)-getmin(x,y)); 
        }
    }
    return 0;
}
```
