# 求强连通分量模板（tarjan算法） - xcw0754 - 博客园
# [求强连通分量模板（tarjan算法）](https://www.cnblogs.com/xcw0754/p/4626611.html)
 关于如何求强连通分量的知识请戳 https://www.byvoid.com/blog/scc-tarjan/
```
1 void DFS(int x)
 2 {
 3     dfn[x]=lowlink[x]=++dfn_clock;
 4     stac.push_back(x);
 5     for(int i=0; i<g[x].size(); i++)    //与x相连的个点
 6     {
 7         int t=g[x][i];
 8         if(!dfn[x]) //未访问过
 9         {
10             DFS(t);
11             lowlink[x]=min(lowlink[x],lowlink[t]);
12         }
13         else if(!sccno[t])  //点t已经访问过，但还不属于任何scc
14             lowlink[x]=min(lowlink[x],dfn[t]);
15     }
16     if(lowlink[x]==dfn[x])  //下面的点最多只能连到自己
17     {
18         scc_cnt++;
19         while(true)
20         {
21             int r=stac.top();
22             stac.pop();
23             sccno[r]=scc_cnt;
24             if(x==r)    break;  //回到自己
25         }
26     }
27 }
28 
29 void find_scc(int n)
30 {
31     dfn_clock = scc_cnt = 0;    //计数器，强连通分量的个数
32     memset(sccno,0,sizeof(sccno));
33     memset(dfn,0,sizeof(dfn));
34     memset(lowlink,0,sizeof(lowlink));
35 
36     for(int i=1; i<=n; i++)
37     {
38         if(!dfn[i]) //多个连通图时这样做
39             DFS(i);
40     }
41 }
模板代码（带注释）
```

