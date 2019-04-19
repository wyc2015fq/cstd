# hdu 2767 Proving Equivalences（强连通分量+缩点） - HJ - CSDN博客
2017年10月19日 21:36:28[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：165
Problem Description 
Consider the following exercise, found in a generic linear algebra textbook.
Let A be an n × n matrix. Prove that the following statements are equivalent:
- A is invertible.
- Ax = b has exactly one solution for every n × 1 matrix b.
- Ax = b is consistent for every n × 1 matrix b.
- Ax = 0 has only the trivial solution x = 0.
The typical way to solve such an exercise is to show a series of implications. For instance, one can proceed by showing that (a) implies (b), that (b) implies (c), that (c) implies (d), and finally that (d) implies (a). These four implications show that the four statements are equivalent.
Another way would be to show that (a) is equivalent to (b) (by proving that (a) implies (b) and that (b) implies (a)), that (b) is equivalent to (c), and that (c) is equivalent to (d). However, this way requires proving six implications, which is clearly a lot more work than just proving four implications!
I have been given some similar tasks, and have already started proving some implications. Now I wonder, how many more implications do I have to prove? Can you help me determine this?
Input 
On the first line one positive number: the number of testcases, at most 100. After that per testcase:
- One line containing two integers n (1 ≤ n ≤ 20000) and m (0 ≤ m ≤ 50000): the number of statements and the number of implications that have already been proved.
- m lines with two integers s1 and s2 (1 ≤ s1, s2 ≤ n and s1 ≠ s2) each, indicating that it has been proved that statement s1 implies statement s2.
Output 
Per testcase:
- One line with the minimum number of additional implications that need to be proved in order to prove that all statements are equivalent.
Sample Input
2 
4 0 
3 2 
1 2 
1 3
Sample Output
4 
2
这是白书上的第一个例题！
题目大意： 
给出n个结点m条边的有向图，要求加尽量少的边，使原图强连通。
解题思路： 
首先找出强连通分量，然后把每个强连通分量缩成一个点，得到一个DAG。接下来，设有n个结点（n个强连通分量）的入度为0，b个节点的出度为0 ，答案就是max{a,b}。
```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<vector>
#include<stack>
#define INF 0x3f3f3f3f
using namespace std;
const int N=20005;
stack<int> q;
vector<int> g[N];
int t,n,m,scc_cnt,dfs_clock,dfn[N],low[N],sccno[N];
int in[N],out[N];
void Init()
{
    memset(dfn,0,sizeof(dfn));
    memset(low,0,sizeof(low));
    memset(sccno,0,sizeof(sccno));  
    scc_cnt=dfs_clock=0;
    for(int i=1;i<=n;i++)
      g[i].clear();
    while(!q.empty())
      q.pop();
}
void tarjan(int u)
{
    dfn[u]=low[u]=++dfs_clock;
    q.push(u);
    for(int i=0;i<g[u].size();i++)
    {
        int v=g[u][i];
        if(!dfn[v])
        {
            tarjan(v);
            low[u]=min(low[u],low[v]);
        }
        else if(!sccno[v])         //反向边 
          low[u]=min(low[u],dfn[v]);
    }
    if(low[u]==dfn[u])
    {
        scc_cnt++;
        while(!q.empty()) 
        {
            int x=q.top();
            q.pop();
            sccno[x]=scc_cnt;   //把在同一个强连通分量的点的sccno值全部赋为scc_cnt 
            if(x==u)
              break;
        }
    }
}
int main()
{
    scanf("%d",&t);
    while(t--)
    {
        int u,v,a=0,b=0;
        scanf("%d%d",&n,&m);        
        Init();
        while(m--)
        {
            scanf("%d%d",&u,&v);
            g[u].push_back(v);
        }   
        for(int i=1;i<=n;i++)
          if(!dfn[i])
            tarjan(i);
        //缩点 
        for(int i=1;i<=scc_cnt;i++)
          in[i]=out[i]=0;
        for(int u=1;u<=n;u++)        
          for(int i=0;i<g[u].size();i++)
          {
              int v=g[u][i];   //  u->v 
              if(sccno[u]!=sccno[v])    //如果u与v不在同一个强连通分量 
                in[sccno[v]]=out[sccno[u]]=1;      //那么v的入度和u的出度都不为0 
          }
        for(int i=1;i<=scc_cnt;i++)
        {
            if(!in[i])
              a++;
            if(!out[i])
              b++;
        }
        if(scc_cnt==1)
          printf("0\n");
        else
          printf("%d\n",max(a,b));
    }
    return 0;
}
```
