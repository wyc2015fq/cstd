# hdu 1535 Invitation Cards（逆向构图+Dijkstra优先队列优化） - HJ - CSDN博客
2017年10月09日 21:13:22[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：214
[http://acm.hdu.edu.cn/showproblem.php?pid=1535](http://acm.hdu.edu.cn/showproblem.php?pid=1535)
题目大意：求从1出发到任何一个点的最短路+任何一个点到起点1的最短路
解题思路：正向构图求一遍最短路，然后把有向图的指向改变，逆向构图再求一遍最短路，最后把两次累加和就行了。
考虑到点的数据量比较大，Dijkstra需要用到优先队列优化。
首先回忆Dijkstra普通做法是：
```
清除所有点的标号
设d[0]=0,其他d[i]=INF
循环n次{
    在所有未标号结点中，选出d值最小的结点x
    给结点x标记
    对于从x出发的所有边（x,y），更新d[y]=min{d[y],d[x]+w(x,y)}
}
```
Dijkstra主要优化的是数值的插入（更新）和取出最小值两个操作，因此考虑到用堆。
Dijkstra的优先队列优化思想：在STL中用priority_queue< node > Q代替堆，把每个顶点当前的最短距离用堆来维护，在更新最短距离时，把对应的元素往根的方向移动以满足堆得性质。而每次从堆中取出的最小值就是下一次要用的顶点，这样堆中的元素共有O(V)个，更新和取出的操作有O(E)次，因此整个算法的复杂度是O(ElogV)
具体代码实现如下：
```cpp
#include<iostream>
#include<cstring>
#include<queue>
#define INF 0x3f3f3f3f
using namespace std;
const int maxn=1000005;
struct node{
    int v,len;
    bool operator < (const node&a)const{
        return len>a.len;
    }
}; 
int n,m,dis[maxn],vis[maxn];
vector<node>e[maxn];
vector<node>e1[maxn]; 
void Dijkstra(vector<node> G[])       //Dijkstra优先队列优化 
{
    memset(vis,0,sizeof(vis));
    for(int i=0;i<=n;i++)
      dis[i]=INF;
    dis[1]=0;
    priority_queue<node>Q;
    Q.push(node{1,0});
    while(!Q.empty())
    {
        node now=Q.top();
        Q.pop();
        int v=now.v;
        if(vis[v])
          continue;
        vis[v]=1;
        for(int i=0;i<G[v].size();i++)
        {
            int u=G[v][i].v,len=G[v][i].len;
            if(!vis[u]&&dis[u]>dis[v]+len)
            {
                dis[u]=dis[v]+len;
                Q.push(node{u,dis[u]});
            }
        }
    }
}
int main()
{
    int t,a,b,c;
    cin>>t;
    while(t--)
    {
        cin>>n>>m;
        for(int i=0;i<=n;i++)
        {
            e[i].clear();
            e1[i].clear();
        }
        while(m--)
        {
            cin>>a>>b>>c;
            e[a].push_back(node{b,c});   //正向图 
            e1[b].push_back(node{a,c});    //反向建边 
        }
        int ans=0;
        Dijkstra(e);
        for(int i=1;i<=n;i++)
          ans+=dis[i];
        Dijkstra(e1);
        for(int i=1;i<=n;i++)
          ans+=dis[i];
        cout<<ans<<endl;
    }
    return 0;
}
```
