# nyoj  20 吝啬的国度（dfs） - HJ - CSDN博客
2017年06月29日 18:25:27[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：186
个人分类：[====ACM====																[-----------搜索-----------																[DFS																[---------OJ---------																[杂题](https://blog.csdn.net/feizaoSYUACM/article/category/6684121)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6317663)](https://blog.csdn.net/feizaoSYUACM/article/category/6307975)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
描述 
    在一个吝啬的国度里有N个城市，这N个城市间只有N-1条路把这个N个城市连接起来。现在，Tom在第S号城市，他有张该国地图，他想知道如果自己要去参观第T号城市，必须经过的前一个城市是几号城市（假设你不走重复的路）。
输入 
    第一行输入一个整数M表示测试数据共有M(1<=M<=5)组 
    每组测试数据的第一行输入一个正整数N(1<=N<=100000)和一个正整数S(1<=S<=100000)，N表示城市的总个数，S表示参观者所在城市的编号 
    随后的N-1行，每行有两个正整数a,b(1<=a,b<=N)，表示第a号城市和第b号城市之间有一条路连通。 
输出 
    每组测试数据输N个正整数，其中，第i个数表示从S走到i号城市，必须要经过的上一个城市的编号。（其中i=S时，请输出-1）
样例输入 
    1 
    10 1 
    1 9 
    1 8 
    8 10 
    10 3 
    8 6 
    1 2 
    10 4 
    9 5 
    3 7
样例输出 
    -1 1 10 10 9 8 3 1 1 8
解题方法： 
简单dfs！
```cpp
#include<iostream>
#include<vector>
#include<cstring>
using namespace std;
#define maxn 1000000+5
vector<int> G[maxn];
int p[maxn];
void dfs(int x)
{
    for(int i=0;i<G[x].size();i++)
    {
        if(p[G[x][i]]==0)      //如果没有被遍历 
        {
            p[G[x][i]]=x;       //赋值 
            dfs(G[x][i]);           //深搜到叶子节点 
        }
    }
}
int main()
{
    int t,n,s,x,y;
    cin>>t;
    while(t--)
    {
        memset(G,0,sizeof(G));
        memset(p,0,sizeof(p));      
        cin>>n>>s;  
        p[s]=-1; 
        for(int i=0;i<n-1;i++)
        {
            cin>>x>>y;
            G[x].push_back(y);
            G[y].push_back(x); 
        }
        dfs(s);
        for(int i=1;i<=n;i++)
          cout<<p[i]<<" ";
        cout<<endl;    
    }
    return 0;
}
```
