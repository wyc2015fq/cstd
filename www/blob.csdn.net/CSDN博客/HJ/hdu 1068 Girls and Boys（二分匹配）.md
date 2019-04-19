# hdu 1068 Girls and Boys（二分匹配） - HJ - CSDN博客
2017年10月14日 13:05:28[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：145
个人分类：[====ACM====																[---------OJ---------																[HDU																[----------图论---------																[二分图最大匹配](https://blog.csdn.net/feizaoSYUACM/article/category/6308031)](https://blog.csdn.net/feizaoSYUACM/article/category/6308029)](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
[http://acm.split.hdu.edu.cn/showproblem.php?pid=1068](http://acm.split.hdu.edu.cn/showproblem.php?pid=1068)
题意： 
    有n个人，一些人认识另外一些人，选取一个集合，使得集合里的每个人都互相不认识，求该集合中人的最大个数。
思路： 
    这题就是求最大独立集，但是这并不是两个集合，而是一个集合，所以求出最大匹配后需要/2，然后代公式：最大独立集=N-最大匹配。
用匈牙利算法求最大匹配。
```cpp
#include<iostream>
#include<cstring>
#include<cstdio>
using namespace std;
const int maxn=1005;
int n,link[maxn],map[maxn][maxn];
bool vis[maxn];
bool dfs(int x)
{
    for(int i=0;i<n;i++)
    {
        if(!vis[i]&&map[x][i])
        {
            vis[i]=true;
            if(link[i]==-1||dfs(link[i]))
            {
                link[i]=x;
                return true;
            }
        }
    }
    return false;
}
int hungary()
{
    int ans=0;
    memset(link,-1,sizeof(link));
    for(int i=0;i<n;i++)
    {
        memset(vis,false,sizeof(vis));
        if(dfs(i))
          ans++;
    }
    return ans;
}
int main()
{
    while(scanf("%d",&n)!=EOF)
    {
        int a,b,num;
        memset(map,0,sizeof(map));
        for(int i=1;i<=n;i++)
        {
            scanf("%d: (%d)",&a,&num);
            while(num--)
            {
                scanf("%d",&b);
                map[a][b]=1;
            }
        }
        int ans=hungary();
        printf("%d\n",n-ans/2);
    }
    return 0;
}
```
