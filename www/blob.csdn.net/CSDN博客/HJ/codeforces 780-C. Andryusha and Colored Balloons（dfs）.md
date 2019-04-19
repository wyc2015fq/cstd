# codeforces 780-C. Andryusha and Colored Balloons（dfs） - HJ - CSDN博客
2017年07月16日 18:34:04[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：252
个人分类：[====ACM====																[-----------搜索-----------																[DFS																[---------OJ---------																[codeforce](https://blog.csdn.net/feizaoSYUACM/article/category/6353637)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6317663)](https://blog.csdn.net/feizaoSYUACM/article/category/6307975)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
Andryusha goes through a park each day. The squares and paths between them look boring to Andryusha, so he decided to decorate them.
The park consists of n squares connected with (n - 1) bidirectional paths in such a way that any square is reachable from any other using these paths. Andryusha decided to hang a colored balloon at each of the squares. The baloons’ colors are described by positive integers, starting from 1. In order to make the park varicolored, Andryusha wants to choose the colors in a special way. More precisely, he wants to use such colors that if a, b and c are distinct squares that a and b have a direct path between them, and b and c have a direct path between them, then balloon colors on these three squares are distinct.
Andryusha wants to use as little different colors as possible. Help him to choose the colors! 
Input
The first line contains single integer n (3 ≤ n ≤ 2·105) — the number of squares in the park.
Each of the next (n - 1) lines contains two integers x and y (1 ≤ x, y ≤ n) — the indices of two squares directly connected by a path.
It is guaranteed that any square is reachable from any other using the paths. 
Output
In the first line print single integer k — the minimum number of colors Andryusha has to use.
In the second line print n integers, the i-th of them should be equal to the balloon color on the i-th square. Each of these numbers should be within range from 1 to k. 
Examples 
Input
3 
2 3 
1 3
Output
3 
1 3 2 
Input
5 
2 3 
5 3 
4 3 
1 3
Output
5 
1 3 2 5 4 
Input
5 
2 1 
3 2 
4 3 
5 4
Output
3 
1 2 3 1 2 
题目大意 
给你一棵树，让你用最少的颜色给这棵树染色（需要保证相邻节点的颜色不同）。
解题思路： 
用邻接表存起来每两个相连的节点，然后从第一个节点开始，dfs染色，每次比较每个节点所需要的最少染色种数。
```cpp
#include<iostream>
#include<cstring>
#include<vector>
using namespace std;
#define maxn 200005
vector<int> G[maxn];
int vis[maxn];
void dfs(int now,int last)
{
    int cnt=1;
    for(int i=0;i<G[now].size();i++)
    {
        if(G[now][i]!=last)
        {
            while(cnt==vis[now]||cnt==vis[last])
              cnt++;
            vis[G[now][i]]=cnt++;
        }
    }
    for(int i=0;i<G[now].size();i++)
      if(G[now][i]!=last)
        dfs(G[now][i],now);
}
int main()
{
    int n;
    while(cin>>n)
    {
        int ans=0,x,y;      
        for(int i=0;i<=n;i++)
          G[i].clear();
        for(int i=0;i<n-1;i++)
        {
            cin>>x>>y;
            G[x].push_back(y);
            G[y].push_back(x);
        }
        memset(vis,0,sizeof(vis));
        vis[1]=1;
        dfs(1,0);
        for(int i=1;i<=n;i++)
          ans=max(ans,vis[i]);
        cout<<ans<<endl;
        for(int i=1;i<=n;i++)
          cout<<vis[i]<<" ";
        cout<<endl;
    }
    return 0;
}
```
