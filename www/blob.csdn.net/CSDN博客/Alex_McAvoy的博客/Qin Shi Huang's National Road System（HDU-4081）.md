# Qin Shi Huang's National Road System（HDU-4081） - Alex_McAvoy的博客 - CSDN博客





2019年01月18日 17:07:17[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：101








> 
# Problem Description

During the Warring States Period of ancient China(476 BC to 221 BC), there were seven kingdoms in China ---- they were Qi, Chu, Yan, Han, Zhao, Wei and Qin. Ying Zheng was the king of the kingdom Qin. Through 9 years of wars, he finally conquered all six other kingdoms and became the first emperor of a unified China in 221 BC. That was Qin dynasty ---- the first imperial dynasty of China(not to be confused with the Qing Dynasty, the last dynasty of China). So Ying Zheng named himself "Qin Shi Huang" because "Shi Huang" means "the first emperor" in Chinese.

Qin Shi Huang undertook gigantic projects, including the first version of the Great Wall of China, the now famous city-sized mausoleum guarded by a life-sized Terracotta Army, and a massive national road system. There is a story about the road system: 

There were n cities in China and Qin Shi Huang wanted them all be connected by n-1 roads, in order that he could go to every city from the capital city Xianyang. 

Although Qin Shi Huang was a tyrant, he wanted the total length of all roads to be minimum,so that the road system may not cost too many people's life. A daoshi (some kind of monk) named Xu Fu told Qin Shi Huang that he could build a road by magic and that magic road would cost no money and no labor. But Xu Fu could only build ONE magic road for Qin Shi Huang. So Qin Shi Huang had to decide where to build the magic road. Qin Shi Huang wanted the total length of all none magic roads to be as small as possible, but Xu Fu wanted the magic road to benefit as many people as possible ---- So Qin Shi Huang decided that the value of A/B (the ratio of A to B) must be the maximum, which A is the total population of the two cites connected by the magic road, and B is the total length of none magic roads. 

Would you help Qin Shi Huang? 

A city can be considered as a point, and a road can be considered as a line segment connecting two points.

# Input

The first line contains an integer t meaning that there are t test cases(t <= 10). 

For each test case: 

The first line is an integer n meaning that there are n cities(2 < n <= 1000). 

Then n lines follow. Each line contains three integers X, Y and P ( 0 <= X, Y <= 1000, 0 < P < 100000). (X, Y) is the coordinate of a city and P is the population of that city. 

It is guaranteed that each city has a distinct location.

# Output

For each test case, print a line indicating the above mentioned maximum ratio A/B. The result should be rounded to 2 digits after decimal point.

# Sample Input

**241 1 201 2 30200 2 80200 1 10031 1 201 2 302 2 40**

# **Sample Output**

**65.0070.00**


题意：给出 n 个点的城市坐标与城市的人口，要求将所有城市连通起来，其中有一条道路是魔法道路没有边权，要求这条道路连接的两城市的人口数和为 A，其余道路权值和为 B，求 A/B 的最大值

思路：先根据城市坐标求出任两个城市相连道路的边权，然后根据边权求最小生成树，由于连接两个人口数最大的城市的道路没有边权，因此删掉所枚举的边后剩余的边权和是固定的，即 B 是固定的，因而可以枚举所有边，确立边两端的点的人口数之和，寻找最大的人口数之和即 A，除枚举的边之外的边权和即 B

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<cstring>
#include<cmath>
#include<ctime>
#include<algorithm>
#include<utility>
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-6
#define MOD 1000003
#define INF 0x3f3f3f3f
#define N 1001
#define LL long long
using namespace std;
struct Node{//点
    int x,y;
    double p;
}node[N];

struct Edge{//边
    int x,y;
    double dis;
    Edge(){}
    Edge(int x,int y,double dis):x(x),y(y),dis(dis){}
    bool operator < (const Edge &rhs)const{
        return dis>rhs.dis;
    }
}tree[N];//存储最小生成树
int n;
vector<int> G[N];
bool vis[N];
int father[N];
priority_queue<Edge> edge;//存储边
double calculate(Node a,Node b){//计算两点距离
    return sqrt((double)(a.x-b.x)*(a.x-b.x)+(double)(a.y-b.y)*(a.y-b.y));
}
int Find(int x){
    if(father[x]==x)
        return x;
    return Find(father[x]);
}
double Kruskal(){
    for(int i=0;i<=n;i++){
        father[i]=i;
        G[i].clear();
    }

    int cnt=0;
    double MST=0;//最小生成树权值
    while(!edge.empty()){
        Edge x=edge.top();
        edge.pop();

        int u=Find(x.x);
        int v=Find(x.y);
        if(u!=v){
            father[u]=v;
            MST+=x.dis;

            G[x.x].push_back(x.y);
            G[x.y].push_back(x.x);
            tree[cnt++]=x;
        }

        if(cnt==n-1)
            break;
    }
    return MST;
}
double dfs(int x,double sum){//寻找人口数量
    vis[x]=true;

    sum=max(sum,node[x].p);
    for(int y=0;y<G[x].size();y++)
        if(!vis[G[x][y]])
            sum=dfs(G[x][y],sum);

    return sum;
}
int main(){
    int t;
    scanf("%d",&t);
    while(t--){
        memset(node,0,sizeof(node));
        memset(tree,0,sizeof(tree));


        scanf("%d",&n);
        for(int i=1;i<=n;i++)
            scanf("%d%d%lf",&node[i].x,&node[i].y,&node[i].p);

        while(!edge.empty())
            edge.pop();

        for(int i=1;i<=n;i++)
            for(int j=1;j<i;j++)
                edge.push(Edge(j,i,calculate(node[i],node[j])));

        double dis=Kruskal();
        double maxx=-INF;
        for(int i=0;i<n-1;i++){//枚举所有边
            memset(vis,false,sizeof(vis));
            double people=0;
            int x=tree[i].x;
            int y=tree[i].y;

            vis[y]=true;//将x-y两个城市分隔

            people+=dfs(x,0);//计算x一侧人口最大的城市
            people+=dfs(y,0);//计算y一侧人口最大的城市

            maxx=max(maxx,people/(dis-tree[i].dis));

        }
        printf("%0.2lf\n",maxx);
    }
    return 0;
}
```





