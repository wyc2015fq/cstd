# hdu 1317 XYZZY（floyd+SPFA） - HJ - CSDN博客
2017年10月10日 15:23:46[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：125
个人分类：[====ACM====																[---------OJ---------																[HDU																[----------图论---------																[最短路](https://blog.csdn.net/feizaoSYUACM/article/category/6307997)](https://blog.csdn.net/feizaoSYUACM/article/category/6308029)](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
[http://acm.split.hdu.edu.cn/showproblem.php?pid=1317](http://acm.split.hdu.edu.cn/showproblem.php?pid=1317)
题意：有n个房间(n<=100)，每个房间有一个点权（第1号房间和第n号房间权值均为0），到达该房间时会自动获得该点权（可能为负权）。给出一些无向边。有一个人，初始有能量值100，初始位置是第1号房间，要走到第n号房间，且路途中不得使身上能量值小于或等于0。能到达第n个房间就算赢，问能否赢。
思路：floyd+SPFA
floyd 用于判断是否能从u到达v 即求出g[][]数值 ，其中松弛条件应该写成
```
g[i][j]=g[i][j]||(g[i][k]&&g[k][j]);
```
SPFA用于松弛求出d[]数组 ，其中判断条件应该有三个
```
if(map[u][i]&&d[u]+e[i]>d[i]&&d[u]+e[i]>0)
```
保证u->i连通，更新值更大，并且途中每一次的松弛必须保证值>0
具体代码如下：
```cpp
#include<iostream>
#include<cstring> 
#include<queue>
#define INF -0x3f3f3f3f
using namespace std;
const int maxn=105;
bool g[maxn][maxn],map[maxn][maxn];
int n,m,v,num[maxn],d[maxn],e[maxn];
void Init()
{ 
    memset(g,false,sizeof(g));          //g[u][v] 判断 u->v 的连通性 
    memset(map,false,sizeof(map));           //保存原图 用来SPFA中的松弛条件 
    fill(d,d+maxn,INF);            //从源s点到当前点u的路径上的点权之和 
    memset(num,0,sizeof(num));       
}
void floyd()           //floyd 用于判断是否能从u到达v 即求出g[][]数值  
{
    for(int k=0;k<n;k++)
      for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
          g[i][j]=g[i][j]||(g[i][k]&&g[k][j]);
} 
int SPFA(int s)           // SPFA用于松弛求出d[]数组 
{
    queue<int>q;
    d[s]=100;
    q.push(s);
    while(!q.empty())
    {
        int u=q.front();
        q.pop();
        num[u]++;
        if(num[u]>=n)           //如果一个点的进队次数大于等于n说明它在环中 并且能判断出此环为正环 
          return g[u][n-1];          //因此只要判断能不能从该点到达终点即可 
        for(int i=0;i<n;i++)
        {
            if(map[u][i]&&d[u]+e[i]>d[i]&&d[u]+e[i]>0)    //注意途中都不能出现<=0 因此松弛优化要满足优化值即 d[u]+e[i]>0 
            {
                d[i]=d[u]+e[i];
                q.push(i);
            }
        }
    }
    return d[n-1]>0;       //最后判断从源点s到达第n个房间时最大可以获得多少能量值 这个值是不是大于0 
}
int main()
{
    while(cin>>n&&n!=-1)
    {
        Init();
        for(int i=0;i<n;i++)
        {
            cin>>e[i]>>m;
            for(int j=0;j<m;j++)
            {
                cin>>v;        //注意题目数据是从1~n 需要减一  
                v--;
                map[i][v]=g[i][v]=true;
            }
        }
        floyd();
        if(!g[0][n-1])
          cout<<"hopeless"<<endl;
        else
        {
            if(SPFA(0))
              cout<<"winnable"<<endl;
            else
              cout<<"hopeless"<<endl;
        }
    }
    return 0;
}
```
