# 最短路模板（SPFA） - HJ - CSDN博客
2018年05月08日 21:01:04[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：114
个人分类：[====ACM====																[ACM模板](https://blog.csdn.net/feizaoSYUACM/article/category/6364160)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
```cpp
void SPFA(int s)
{
    queue<int> q;
    memset(vis,0,sizeof(vis));
    d[s]=0;
    vis[s]=1;
    q.push(s);
    while(!q.empty())
    {
        int u=q.front();
        q.pop();
        vis[u]=0;
        for(int i=0;i<vec[u].size();i++)
        {
            if(d[vec[u][i]]>d[u]+cost[u])
            {
                d[vec[u][i]]=d[u]+cost[u];
                if(!vis[vec[u][i]])
                {
                    vis[vec[u][i]]=1;
                    q.push(vec[u][i]);
                }
            }
        } 
    } 
}
```
