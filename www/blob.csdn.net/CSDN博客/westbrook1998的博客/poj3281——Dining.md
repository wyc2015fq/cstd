# poj3281——Dining - westbrook1998的博客 - CSDN博客





2018年08月29日 20:18:28[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：33








> 
Cows are such finicky eaters. Each cow has a preference for certain foods and drinks, and she will consume no others. 

  Farmer John has cooked fabulous meals for his cows, but he forgot to check his menu against their preferences. Although he might not be able to stuff everybody, he wants to give a complete meal of both food and drink to as many cows as possible. 

  Farmer John has cooked F (1 ≤ F ≤ 100) types of foods and prepared D (1 ≤ D ≤ 100) types of drinks. Each of his N (1 ≤ N ≤ 100) cows has decided whether she is willing to eat a particular food or drink a particular drink. Farmer John must assign a food type and a drink type to each cow to maximize the number of cows who get both. 

  Each dish or drink can only be consumed by one cow (i.e., once food type 2 is assigned to a cow, no other cow can be assigned food type 2). 

  Iput 

  Line 1: Three space-separated integers: N, F, and D  

  Lines 2.. N+1: Each line i starts with a two integers Fi and Di, the number of dishes that cow i likes and the number of drinks that cow i likes. The next Fi integers denote the dishes that cow i will eat, and the Di integers following that denote the drinks that cow i will drink. 

  Output 

  Line 1: A single integer that is the maximum number of cows that can be fed both food and drink that conform to their wishes 

  Sample Input 

  4 3 3 

  2 2 1 2 3 1 

  2 2 2 3 1 2 

  2 2 1 3 1 2 

  2 1 1 3 3 

  Sample Output 

  3 

  Hint 

  One way to satisfy three cows is:  

  Cow 1: no meal  

  Cow 2: Food #2, Drink #2  

  Cow 3: Food #1, Drink #1  

  Cow 4: Food #3, Drink #3  

  The pigeon-hole principle tells us we can do no better since there are only three kinds of food or drink. Other test data sets are more challenging, of course.
今天学网络流  屁都不会 一题写了大半天 不对是 抄了大半天 

n头牛 f个食物 d个饮料 每头牛都有喜欢的食物和饮料类型 

要求能满足多少牛的需求 比较经典的题目要用到拆点 再建图 

源点连接所有食物 食物连对应的牛 牛连牛 牛再连对应的饮料 饮料连汇点 容量都是1 

再跑最大流  

拆点是为了保证不会出现所有食物饮料一头牛都喜欢，都流到一头牛了 拆点之后牛和牛之间只能流1
代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <vector>
#include <queue>
#include <cstring>
using namespace std;
const int M=1005;
const int INF=0x3f3f3f3f;
struct Edge{
    //边的起点，终点，容量，流量
    int from,to,cap,flow;
    Edge(int u,int v,int c,int f):from(u),to(v),cap(c),flow(f){}
};
int n,m;
//保存边 编号从0开始搜索
vector<Edge> edges;
//G[i]保存以i为起点的边的编号，包括反向边
vector<int> G[M];
//记录源点到当前点还有多少流量可用
int a[M];
//记录到达该点的前一条边的编号
int p[M];
void init(){
    for(int i=0;i<M;i++){
        G[i].clear();
    }
    edges.clear();
}
void addEdge(int from,int to,int cap){
    edges.push_back(Edge(from,to,cap,0));
    //加入反向边（没有容量）
    edges.push_back(Edge(to,from,0,0));
    //获取加入前面两条边后的边数（编号）
    m=edges.size();
    //m-2就是先加入的边，即正向边编号，m-1即为反向边编号
    //正向边和反向边编号刚好是^1的关系
    G[from].push_back(m-2);
    G[to].push_back(m-1);
}
int maxFlow(int s,int t){
    int flow=0;
    //bfs不断寻找增广路
    while(true){
        memset(a,0,sizeof(a));
        queue<int> q;
        q.push(s);
        //源点初始化
        a[s]=INF;
        while(!q.empty()){
            int x=q.front();
            q.pop();
            //遍历x出发的所有边，包括反向边
            for(int i=0;i<G[x].size();i++){
                Edge &e=edges[G[x][i]];
                //...    边的容量大于边的当前流量
                if(!a[e.to] && e.cap > e.flow){
                    //记录前驱
                    p[e.to]=G[x][i];
                    //到该店的可用流量等于到上一点的可用流量和这一条边容量流量差的最小值
                    a[e.to]=min(a[x],e.cap-e.flow);
                    //继续搜索
                    q.push(e.to);
                }
            }
            //找到增广路，即到汇点有可用流量
            if(a[t]){
                break;
            }
        }
        //找不到增广路，当前即为最大流
        if(!a[t]){
            break;
        }
        //每一轮bfs结束后，将路径上所有正向边的流量加上delta，反向边的流量减去delta
        for(int u=t;u!=s;u=edges[p[u]].from){
            edges[p[u]].flow+=a[t];
            edges[p[u]^1].flow-=a[t];
        }
        //这里a[t]可能有正负，只要非零都能走到这里
        flow+=a[t];
    }
    return flow;
}
int main(void){
    //freopen("data.txt","r",stdin);
    init();
    int N,F,D;
    scanf("%d%d%d",&N,&F,&D);
    n=N*2+F+D+2;
    for(int i=2;i<=F+1;i++){
        addEdge(1,i,1);
    }
    for(int i=F+2*N+2;i<n;i++){
        addEdge(i,n,1);
    }
    int k1,k2,t;
    for(int i=1;i<=N;i++){
        scanf("%d%d",&k1,&k2);
        while(k1--){
            scanf("%d",&t);
            addEdge(t+1,1+F+i,1);
        }
        while(k2--){
            scanf("%d",&t);
            addEdge(1+F+N+i,1+F+2*N+t,1);
        }
        addEdge(1+F+i,1+F+N+i,1);
    }
    int ans=maxFlow(1,n);
    printf("%d\n",ans);
    return 0;
}
```






