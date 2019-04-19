# hdu  2224 The shortest path（双调欧几里得旅行商问题） - HJ - CSDN博客
2017年10月11日 19:18:23[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：221
[http://acm.split.hdu.edu.cn/showproblem.php?pid=2224](http://acm.split.hdu.edu.cn/showproblem.php?pid=2224)
旅行商问题描述：给出平面上的n个点，确定一条连接各点的最短闭合旅程。
双调旅行商问题：给出平面上的n个点，从 1 号点出发走到最右端的点 N ，再返回 1 ，要求中途不能重复走某个点而且必须把所有点全走一遍，求最小的总路径。
旅行商问题求解 [http://blog.csdn.net/xiajun07061225/article/details/8092247](http://blog.csdn.net/xiajun07061225/article/details/8092247) （一篇很好的博客）
```cpp
#include<iostream>
#include<algorithm>
#include<cstdio>
#include<cmath>
#define INF 0x3f3f3f3f
using namespace std;
const int maxn=205;
struct node{
    int x,y;
}s[maxn];
double d[maxn][maxn];
int cmp(node a,node b)
{
    return a.x<b.x;
}
double dist(node p,node q)
{
    return sqrt((p.x-q.x)*(p.x-q.x)+(p.y-q.y)*(p.y-q.y)); 
}
int main()
{
    int n;
    while(cin>>n)
    {
        for(int i=1;i<=n;i++)
          cin>>s[i].x>>s[i].y;
        sort(s,s+n,cmp);        //将各点按照x坐标从小到大排列 
        d[1][2]=dist(s[1],s[2]);      //d[i][j]代表起始点到i的距离 + 起始点到j的距离 中间没有交叉点 且没有遗漏点 
        for(int j=3;j<=n;j++)
        {
            for(int i=1;i<=j-1;i++)
            {
                if(i<j-1)          // 当i<j-1的时候  dp[i][j]是从dp[i][j-1]传递过去的 
                  d[i][j]=d[i][j-1]+dist(s[j-1],s[j]);
                else              // 当 i==j-1的时候  dp[i][j]是由dp[i][k]+dis[k][j]得到的 
                {
                    d[i][j]=INF;
                    for(int k=1;k<j-1;k++)
                      d[i][j]=min(d[i][j],d[k][i]+dist(s[k],s[j]));
                }
            }
        }
        d[n][n]=d[n-1][n]+dist(s[n-1],s[n]);
        printf("%.2lf\n",d[n][n]);
    }
    return 0;
}
```
