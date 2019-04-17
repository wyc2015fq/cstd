# hdu4109——Instrction Arrangement - westbrook1998的博客 - CSDN博客





2018年09月01日 22:31:01[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：42








> 
Ali has taken the Computer Organization and Architecture course this term. He learned that there may be dependence between instructions, like WAR (write after read), WAW, RAW.  

  If the distance between two instructions is less than the Safe Distance, it will result in hazard, which may cause wrong result. So we need to design special circuit to eliminate hazard. However the most simple way to solve this problem is to add bubbles (useless operation), which means wasting time to ensure that the distance between two instructions is not smaller than the Safe Distance.  

  The definition of the distance between two instructions is the difference between their beginning times.  

  Now we have many instructions, and we know the dependent relations and Safe Distances between instructions. We also have a very strong CPU with infinite number of cores, so you can run as many instructions as you want simultaneity, and the CPU is so fast that it just cost 1ns to finish any instruction. 

  Your job is to rearrange the instructions so that the CPU can finish all the instructions using minimum time. 

  Input 

  The input consists several testcases.  

  The first line has two integers N, M (N <= 1000, M <= 10000), means that there are N instructions and M dependent relations.  

  The following M lines, each contains three integers X, Y , Z, means the Safe Distance between X and Y is Z, and Y should run after X. The instructions are numbered from 0 to N - 1.  

  Output 

  Print one integer, the minimum time the CPU needs to run.  

  Sample Input 

  5 2 

  1 2 1 

  3 4 1 

  Sample Output 

  2 

  Hint 

  In the 1st ns, instruction 0, 1 and 3 are executed; 

  In the 2nd ns, instruction 2 and 4 are executed. 

  So the answer should be 2.
差分约束系统的问题 其实就是几个点 然后给出其中某些点两个点之间距离要大于某个值 

这种大于等于不等式就转化为求图的最长路问题 

然后要自己加入一个起点和终点  权值设为1（设为0后面无法更新 松弛那里出问题
代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <vector>
#include <cstring>
#include <queue>
using namespace std;
const int N=1005;
const int INF=0x3f3f3f3f;
int n;
int m;
int dis[N];
int cnt[N];
bool vis[N];
struct Edge{
    int v;
    int c;
    Edge(int _v,int _c):v(_v),c(_c){}
};
vector<Edge> e[N];
bool spfa(int s){
    memset(vis,false,sizeof(vis));
    memset(cnt,0,sizeof(cnt));
    for(int i=0;i<n+2;i++){
        dis[i]=0;
    }
    //dis[s]=INF;
    vis[s]=true;
    queue<int> q;
    q.push(s);
    cnt[s]=1;
    while(!q.empty()){
        int u=q.front();
        q.pop();
        vis[u]=false;
        for(int i=0;i<e[u].size();i++){
            int v=e[u][i].v;
            int w=e[u][i].c;
            //printf("%d %d %d\n",u,v,w);
            if(dis[v]<dis[u]+w){
                dis[v]=dis[u]+w;
                if(!vis[v]){
                    vis[v]=true;
                    q.push(v);
                    if(++cnt[v]>n){
                        return false;
                    }
                }
            }
        }
    }
    return true;
}
int main(void){
    //freopen("data.txt","r",stdin);
    while(~scanf("%d%d",&n,&m)){
        for(int i=0;i<n+2;i++){
            e[i].clear();
        }
        int u,v,c;
        int s=n;
        int t=n+1;
        while(m--){
            scanf("%d%d%d",&u,&v,&c);
            e[u].push_back(Edge(v,c));
        }
        for(int i=0;i<n;i++){
            e[s].push_back(Edge(i,1));
        }
        for(int i=0;i<n;i++){
            e[i].push_back(Edge(t,1));
        }
        spfa(s);
        printf("%d\n",dis[t]-1);
    }
    return 0;
}
```






