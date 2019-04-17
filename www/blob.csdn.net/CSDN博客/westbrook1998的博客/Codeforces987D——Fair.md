# Codeforces987D——Fair - westbrook1998的博客 - CSDN博客





2018年11月01日 13:24:19[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：39标签：[bfs](https://so.csdn.net/so/search/s.do?q=bfs&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
Some company is going to hold a fair in Byteland. There are n towns in Byteland and m two-way roads between towns. Of course, you can reach any town from any other town using roads.

There are k types of goods produced in Byteland and every town produces only one type. To hold a fair you have to bring at least s different types of goods. It costs d(u,v) coins to bring goods from town u to town v where d(u,v) is the length of the shortest path from u to v. Length of a path is the number of roads in this path.

The organizers will cover all travel expenses but they can choose the towns to bring goods from. Now they want to calculate minimum expenses to hold a fair in each of n towns.

Input

There are 4 integers n, m, k, s in the first line of input (1≤n≤105, 0≤m≤105, 1≤s≤k≤min(n,100)) — the number of towns, the number of roads, the number of different types of goods, the number of different types of goods necessary to hold a fair.

In the next line there are n integers a1,a2,…,an (1≤ai≤k), where ai is the type of goods produced in the i-th town. It is guaranteed that all integers between 1 and k occur at least once among integers ai.

In the next m lines roads are described. Each road is described by two integers u v (1≤u,v≤n, u≠v) — the towns connected by this road. It is guaranteed that there is no more than one road between every two towns. It is guaranteed that you can go from any town to any other town via roads.

Output

Print n numbers, the i-th of them is the minimum number of coins you need to spend on travel expenses to hold a fair in town i. Separate numbers with spaces.

Examples

Input

5 5 4 3

1 2 4 3 2

1 2

2 3

3 4

4 1

4 5

Output

2 2 2 2 3

Input

7 6 3 2

1 2 3 3 2 2 1

1 2

2 3

3 4

2 5

5 6

6 7

Output

1 1 1 2 2 1 1

Note

Let’s look at the first sample.

To hold a fair in town 1 you can bring goods from towns 1 (0 coins), 2 (1 coin) and 4 (1 coin). Total numbers of coins is 2.

Town 2: Goods from towns 2 (0), 1 (1), 3 (1). Sum equals 2.

Town 3: Goods from towns 3 (0), 2 (1), 4 (1). Sum equals 2.

Town 4: Goods from towns 4 (0), 1 (1), 5 (1). Sum equals 2.

Town 5: Goods from towns 5 (0), 4 (1), 3 (2). Sum equals 3.
n个点 m条边 每个点有一种类型的货物，一共有k种货物，每个点要得到一共s种货物，也就是需要s-1种货物从其他地方运过来，需要花费的钱数就是距离，边权为1

如果是对每个点都bfs一遍，最坑的情况可能是要遍历所有节点才找到一个节点的满足的货物，1e5*1e5

所以改成对k种货物进行bfs，然后用dis[i][j]记录i点到j货物的最短距离，再bfs过程中不断更新你，最后排序取前s种即可
代码：

```
#include <bits/stdc++.h>
using namespace std;
const int N=1e5+50;
const int INF=0x3f3f3f3f;
int n,m,s,k;
int t[N],dis[N][105];
int u,v;
int cnt;
int head[N];
struct Edge{
    int v,next;
}edge[2*N];
void init(){
    cnt=0;
    memset(head,-1,sizeof(head));
    memset(dis,INF,sizeof(dis));
    for(int i=1;i<=n;i++){
        dis[i][t[i]]=0;
    }
}
void addEdge(int u,int v){
    edge[cnt]=Edge{v,head[u]};
    head[u]=cnt++;
    edge[cnt]=Edge{u,head[v]};
    head[v]=cnt++;
}
vector<int> tt[105];
struct node{
    int i,dis;
};
bool vis[N];
void bfs(){
    queue<node> q;
    for(int i=1;i<=k;i++){
        while(!q.empty()){
            q.pop();
        }
        memset(vis,false,sizeof(vis));
        for(int j=0;j<tt[i].size();j++){
            q.push(node{tt[i][j],0});
            vis[tt[i][j]]=true;
        }
        while(!q.empty()){
            node tmp=q.front();
            //printf("%d\n",tmp.i);
            q.pop();
            int u=tmp.i;
            for(int j=head[u];j!=-1;j=edge[j].next){
                int v=edge[j].v;
                if(vis[v]){
                    continue;
                }
                vis[v]=true;
                dis[v][i]=min(dis[v][i],tmp.dis+1);
                q.push(node{v,tmp.dis+1});
            }
        }
        //printf("------------\n");
        // for(int j=1;j<=n;j++){
        //     printf("%d ",dis[j][i]);
        // }
        // printf("\n");
    }
    // for(int i=1;i<=n;i++){
    //     for(int j=1;j<=k;j++){
    //         printf("%d ",dis[i][j]);
    //     }
    //     printf("\n");
    // }
}
vector<int> res;
int main(void){
    scanf("%d%d%d%d",&n,&m,&k,&s);
    for(int i=1;i<=n;i++){
        scanf("%d",&t[i]);
        tt[t[i]].push_back(i);
    }
    init();
    for(int i=0;i<m;i++){
        scanf("%d%d",&u,&v);
        addEdge(u,v);
    }
    bfs();
    for(int i=1;i<=n;i++){
        sort(dis[i]+1,dis[i]+k+1);
        int ans=0;
        for(int j=1;j<=s;j++){
            ans+=dis[i][j];
        }
        res.push_back(ans);
    }
    printf("%d",res[0]);
    for(int i=1;i<n;i++){
        printf(" %d",res[i]);
    }
    printf("\n");
    return 0;
}
```






