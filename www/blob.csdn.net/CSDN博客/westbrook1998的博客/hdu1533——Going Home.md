# hdu1533——Going Home - westbrook1998的博客 - CSDN博客





2018年08月29日 20:20:43[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：41标签：[最小费用最大流																[网络流](https://so.csdn.net/so/search/s.do?q=网络流&t=blog)](https://so.csdn.net/so/search/s.do?q=最小费用最大流&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
On a grid map there are n little men and n houses. In each unit time, every little man can move one unit step, either horizontally, or vertically, to an adjacent point. For each little man, you need to pay a $1 travel fee for every step he moves, until he enters a house. The task is complicated with the restriction that each house can accommodate only one little man.  

  Your task is to compute the minimum amount of money you need to pay in order to send these n little men into those n different houses. The input is a map of the scenario, a ‘.’ means an empty space, an ‘H’ represents a house on that point, and am ‘m’ indicates there is a little man on that point.  
![这里写图片描述](https://odzkskevi.qnssl.com/0d77b0d8574d5186f189f53f1ecd55a5?v=1535543488)

  You can think of each point on the grid map as a quite large square, so it can hold n little men at the same time; also, it is okay if a little man steps on a grid with a house without entering that house. 

  Input 

  There are one or more test cases in the input. Each case starts with a line giving two integers N and M, where N is the number of rows of the map, and M is the number of columns. The rest of the input will be N lines describing the map. You may assume both N and M are between 2 and 100, inclusive. There will be the same number of ‘H’s and ‘m’s on the map; and there will be at most 100 houses. Input will terminate with 0 0 for N and M.  

  Output 

  For each test case, output one line with the single integer, which is the minimum amount, in dollars, you need to pay.  

  Sample Input 

  2 2 

  .m 

  H. 

  5 5 

  HH..m 

  ….. 

  ….. 

  ….. 

  mm..H 

  7 8 

  …H…. 

  …H…. 

  …H…. 

  mmmHmmmm 

  …H…. 

  …H…. 

  …H…. 

  0 0 

  Sample Output 

  2 

  10 

  28
这是最小费用最大流的问题 也是要建图 把人和房子的位置都看成点 然后加一个源点和汇点 然后容量都是1 费用是距离 求最小费用最大流

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <vector>
#include <queue>
#include <cstring>
#include <cmath>
using namespace std;
const int N=10005;
const int INF=0x3f3f3f3f;
char mp[105][105];
int n,m;
int idx;
struct Edge{
    int from,to,cap,flow,cost;
    Edge(int u,int v,int c,int f,int co):from(u),to(v),cap(c),flow(f),cost(co){}
};
vector<Edge> edges;
vector<int> G[N];
int a[N];
int p[N];
bool vis[N];
int d[N];
int cost;
int flow;
void init(){
    for(int i=0;i<N;i++){
        G[i].clear();
    }
    edges.clear();
}
void addEdge(int from,int to,int cap,int cost){
    edges.push_back(Edge(from,to,cap,0,cost));
    edges.push_back(Edge(to,from,0,0,-cost));
    idx=edges.size();
    G[from].push_back(idx-2);
    G[to].push_back(idx-1);
}
void spfa(int s,int t){
    queue<int> q;
    memset(vis,false,sizeof(vis));
    for(int i=0;i<N;i++){
        d[i]=INF;
    }
    d[s]=0;
    q.push(s);
    vis[s]=true;
    p[s]=0;
    a[s]=INF;
    while(!q.empty()){
        int v=q.front();
        vis[v]=false;
        q.pop();
        for(int i=0;i<G[v].size();i++){
            Edge &e=edges[G[v][i]];
            if(e.cap>e.flow && d[e.to]>d[v]+e.cost){
                d[e.to]=d[v]+e.cost;
                p[e.to]=G[v][i];
                a[e.to]=min(a[v],e.cap-e.flow);
                if(!vis[e.to]){
                    q.push(e.to);
                    vis[e.to]=true;
                }
            }
        }
    }
}
void mcmf(int s,int t){
    flow=0;
    cost=0;
    while(true){
        spfa(s,t);
        if(d[t]==INF){
            break;
        }
        flow+=a[t];
        cost+=d[t]*a[t];
        for(int u=t;u!=s;u=edges[p[u]].from){
            edges[p[u]].flow+=a[t];
            edges[p[u]^1].flow-=a[t];
        }
    }
}
int main(void){
    //freopen("data.txt","r",stdin);
    while(~scanf("%d%d",&n,&m)){
        if(n==0 && m==0){
            break;
        }
        init();
        for(int i=1;i<=n;i++){
            scanf("%s",mp[i]+1);
        }
        //建立超级源点和超级汇点
        int from=n*m+1;
        int to=from+1;
        //记录人和房子的个数和位置
        int mx[N],my[N],mcnt=0;
        int hx[N],hy[N],hcnt=0;
        //源点与人的边
        //房子与汇点的边
        for(int i=1;i<=n;i++){
            for(int j=1;j<=m;j++){
                if(mp[i][j]=='m'){
                    addEdge(from,(i-1)*m+j,1,0);
                    mx[mcnt]=i;
                    my[mcnt]=j;
                    mcnt++;
                }
                else if(mp[i][j]=='H'){
                    addEdge((i-1)*m+j,to,1,0);
                    hx[hcnt]=i;
                    hy[hcnt]=j;
                    hcnt++;
                }
            }
        }
        //人和房子之间的边
        for(int i=0;i<mcnt;i++){
            for(int j=0;j<hcnt;j++){
                addEdge((mx[i]-1)*m+my[i],(hx[j]-1)*m+hy[j],1,abs(mx[i]-hx[j])+abs(my[i]-hy[j]));
            }
        }
        // for(int i=1;i<=n*m+2;i++){
        //     if(G[i].size()!=0){
        //         printf("%d:\n",i);
        //     }
        //     for(int j=0;j<G[i].size();j++){
        //         printf("%d ",G[i][j]);
        //     }
        //     if(G[i].size()!=0){
        //         printf("\n");
        //     }
        // }
        mcmf(from,to);
        printf("%d\n",cost);
    }
    return 0;
}
```






