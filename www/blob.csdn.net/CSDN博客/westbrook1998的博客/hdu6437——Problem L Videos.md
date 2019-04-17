# hdu6437——Problem L.Videos - westbrook1998的博客 - CSDN博客





2018年09月23日 22:57:46[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：39








> 
C-bacteria takes charge of two kinds of videos: ’The Collection of Silly Games’ and ’The Collection of Horrible Games’.

For simplicity’s sake, they will be called as videoA and videoB.

There are some people who want to watch videos during today, and they will be happy after watching videos of C-bacteria.

There are n hours a day, m videos are going to be show, and the number of people is K.

Every video has a type(videoA or videoB), a running time, and the degree of happi- ness after someone watching whole of it.

People can watch videos continuous(If one video is running on 2pm to 3pm and another is 3pm to 5pm, people can watch both of them).

But each video only allows one person for watching.

For a single person, it’s better to watch two kinds to videos alternately, or he will lose W happiness.

For example, if the order of video is ’videoA, videoB, videoA, videoB, …’ or ’B, A, B, A, B, …’, he won’t lose happiness; But if the order of video is ’A, B, B, B, A, B, A, A’, he will lose 3W happiness.

Now you have to help people to maximization the sum of the degree of happiness.

Input

Multiple query.

On the first line, there is a positive integer T, which describe the number of data. Next there are T groups of data.

for each group, the first line have four positive integers n, m, K, W : n hours a day, m videos, K people, lose W happiness when watching same videos).

and then, the next m line will describe m videos, four positive integers each line S, T, w, op : video is the begin at S and end at T, the happiness that people can get is w, and op describe it’s tpye(op=0 for videoA and op=1 for videoB).

There is a blank line before each groups of data.

T<=20, n<=200, m<=200, K<=200, W<=20, 1<=S<T<=n, W<=w<=1000,

op=0 or op=1

Output

Your output should include T lines, for each line, output the maximum happiness for the corresponding datum.

Sample Input

2

10 3 1 10

1 5 1000 0

5 10 1000 1

3 9 10 0

10 3 1 10

1 5 1000 0

5 10 1000 0

3 9 10 0

Sample Output

2000

1990
看完题觉得就是网络流…然后就查题解了，果然是，emmm建图真神奇

然后就用了自己收藏的模板再加上题解的加边…然后居然能过神奇

这个建图有点复杂，首先要加一个源点0和一个汇点2*m+2和一个次源点2*m+1，源点到次源点容量为k，保证只能有k个人看

然后中间拆点就是1-2*m

然后中间的处理就是两个电影判断时间区间，如果可以连续看就加边，然后花费按照是否同一类型，如果是则花费为W（加边是加正的W，最后最小花费再取反，因为这里是要求的最大的happy值），然后就次源点与中间，中间与汇点依次加边，跑最小费用最大流，最后将最小费用取反，就是最大的happy值（这个其实还是想不太通）

因为最小费用最大流模板是每条边需要的费用，这里如果能走过中间的一条边是获得happy值，所以要取反，也就是加边时是-w[i]，不然就是优先考虑happy值小的
代码：

```
#include <cstdio>
#include <algorithm>
#include <vector>
#include <queue>
#include <cstring>
using namespace std;
const int N=1005;
const int INF=0x3f3f3f3f;
struct Edge{
    int from,to,cap,flow,cost;
    Edge(int u,int v,int c,int f,int co):from(u),to(v),cap(c),flow(f),cost(co){}
};
int f;
vector<Edge> edges;
vector<int> G[N];
int a[N];
int p[N];
int l[N];
int r[N];
int w[N];
int tp[N];
int n,m,K,W;
void init(){
    for(int i=0;i<N;i++){
        G[i].clear();
    }
    memset(l,0,sizeof(l));
    memset(r,0,sizeof(r));
    memset(w,0,sizeof(w));
    memset(tp,0,sizeof(tp));
    edges.clear();
}
void addEdge(int from,int to,int cap,int cost){
    edges.push_back(Edge(from,to,cap,0,cost));
    edges.push_back(Edge(to,from,0,0,-cost));
    f=edges.size();
    G[from].push_back(f-2);
    G[to].push_back(f-1);
}
int d[N];
bool vis[N];
int cost;
int flow;
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
    int t;
    scanf("%d",&t);
    while(t--){
        init();
        scanf("%d%d%d%d",&n,&m,&K,&W);
        for(int i=1;i<=m;i++){
            scanf("%d%d%d%d",&l[i],&r[i],&w[i],&tp[i]);
        }
        addEdge(0,2*m+1,K,0);
        for(int i=1;i<=m;i++){
            for(int j=1;j<=m;j++){
                if(i==j){
                    continue;
                }
                //两个视频可以连看
                if(r[i]<=l[j]){
                    addEdge(i+m,j,1,(tp[i]==tp[j]?W:0));
                }
            }
        }
        for(int i=1;i<=m;i++){
            addEdge(i,i+m,1,-w[i]);
        }
        for(int i=1;i<=m;i++){
            addEdge(2*m+1,i,1,0);
        }
        for(int i=1;i<=m;i++){
            addEdge(m+i,2*m+2,1,0);
        }
        mcmf(0,2*m+2);
        printf("%d\n",-cost);
    }
    return 0;
}
```






