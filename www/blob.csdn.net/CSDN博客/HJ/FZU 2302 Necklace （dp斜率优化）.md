# FZU 2302 Necklace （dp斜率优化） - HJ - CSDN博客
2018年08月08日 10:41:09[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：59
[http://acm.fzu.edu.cn/problem.php?pid=2302](http://acm.fzu.edu.cn/problem.php?pid=2302)
题目大意：
给出一个n元环，要求任意剪断k次，求剩下的k段中各段和的平方的累加起来的和最小为多少？
解题思路：
裸的一个dp斜率优化题目，我们可以设dp[i][j]表示前 i 个数剪断 j 次的最小值，那么状态转移方程就为
```
dp[i] = min{  dp[j] + (sum[i] - sum[j]) ^ 2 };
```
然后因为是一个环，初始位置可以有n种，那么就开两倍数组，外加一重循环遍历即可。
```
#include<iostream>
#include<cstdio>
using namespace std;
const int maxn=505;
int a[maxn],sum[maxn];
int dp[maxn/2][maxn/2];
int q[maxn];
int n,m,head,tail;
bool better(int j,int k,int i,int r)
{
    int x=(dp[j][r]+sum[j]*sum[j])-(dp[k][r]+sum[k]*sum[k]);
    int y=2*sum[i]*(sum[j]-sum[k]);
    return x<y;
}
bool small(int a,int b,int c,int r)
{
    int x=((dp[b][r]+sum[b]*sum[b])-(dp[a][r]+sum[a]*sum[a]))*(sum[c]-sum[b]);
    int y=((dp[c][r]+sum[c]*sum[c])-(dp[b][r]+sum[b]*sum[b]))*(sum[b]-sum[a]);
    return x>=y;
}
int getdp(int i,int j,int r)
{
    return dp[j][r]+(sum[i]-sum[j])*(sum[i]-sum[j]);
}
int main()
{
    int t;
    scanf("%d",&t);
    while(t--)
    {
        scanf("%d%d",&n,&m);
        sum[0]=0;
        for(int i=1;i<=n;i++)
        {
            scanf("%d",&a[i]);
            a[n+i]=a[i];
        }
        int ans=0x3f3f3f3f;
        for(int i=1;i<=n;i++)
        {
            for(int j=1;j<=n;j++)
              sum[j]=sum[j-1]+a[j];
            for(int j=1;j<=n;j++)
              dp[j][1]=sum[j]*sum[j];
            for(int j=2;j<=m;j++)
            {
                head=tail=0;
                q[tail++]=j-1;
                for(int k=j;k<=n;k++)
                {
                    while(head+1<tail&&better(q[head+1],q[head],k,j-1))
                      head++;
                    dp[k][j]=getdp(k,q[head],j-1);
                    while(head+1<tail&&small(q[tail-2],q[tail-1],k,j-1))
                      tail--;
                    q[tail++]=k;
                }
            }
            ans=min(ans,dp[n][m]);
            for(int j=1;j<=2*n;j++)
              a[j]=a[j+1];
        }
        printf("%d\n",ans);
    }
    return 0;
}
```
