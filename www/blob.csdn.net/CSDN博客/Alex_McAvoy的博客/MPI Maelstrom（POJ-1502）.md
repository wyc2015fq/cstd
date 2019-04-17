# MPI Maelstrom（POJ-1502） - Alex_McAvoy的博客 - CSDN博客





2018年11月07日 22:38:08[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：31
个人分类：[POJ																[图论——Dijkstra](https://blog.csdn.net/u011815404/article/category/8312429)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

BIT has recently taken delivery of their new supercomputer, a 32 processor Apollo Odyssey distributed shared memory machine with a hierarchical communication subsystem. Valentine McKee's research advisor, Jack Swigert, has asked her to benchmark the new system. 

``Since the Apollo is a distributed shared memory machine, memory access and communication times are not uniform,'' Valentine told Swigert. ``Communication is fast between processors that share the same memory subsystem, but it is slower between processors that are not on the same subsystem. Communication between the Apollo and machines in our lab is slower yet.'' 

``How is Apollo's port of the Message Passing Interface (MPI) working out?'' Swigert asked. 

``Not so well,'' Valentine replied. ``To do a broadcast of a message from one processor to all the other n-1 processors, they just do a sequence of n-1 sends. That really serializes things and kills the performance.'' 

``Is there anything you can do to fix that?'' 

``Yes,'' smiled Valentine. ``There is. Once the first processor has sent the message to another, those two can then send messages to two other hosts at the same time. Then there will be four hosts that can send, and so on.'' 

``Ah, so you can do the broadcast as a binary tree!'' 

``Not really a binary tree -- there are some particular features of our network that we should exploit. The interface cards we have allow each processor to simultaneously send messages to any number of the other processors connected to it. However, the messages don't necessarily arrive at the destinations at the same time -- there is a communication cost involved. In general, we need to take into account the communication costs for each link in our network topologies and plan accordingly to minimize the total time required to do a broadcast.''

# **Input**

The input will describe the topology of a network connecting n processors. The first line of the input will be n, the number of processors, such that 1 <= n <= 100. 

The rest of the input defines an adjacency matrix, A. The adjacency matrix is square and of size n x n. Each of its entries will be either an integer or the character x. The value of A(i,j) indicates the expense of sending a message directly from node i to node j. A value of x for A(i,j) indicates that a message cannot be sent directly from node i to node j. 

Note that for a node to send a message to itself does not require network communication, so A(i,i) = 0 for 1 <= i <= n. Also, you may assume that the network is undirected (messages can go in either direction with equal overhead), so that A(i,j) = A(j,i). Thus only the entries on the (strictly) lower triangular portion of A will be supplied. 

The input to your program will be the lower triangular section of A. That is, the second line of input will contain one entry, A(2,1). The next line will contain two entries, A(3,1) and A(3,2), and so on.

# Output

Your program should output the minimum communication time required to broadcast a message from the first processor to all the other processors.

# Sample Input

**55030 5100 20 5010 x x 10**

# Sample Output

**35**


题意：给出一 n 个点的无向图的邻接矩阵的下三角部分，输出从第 0 个点到其他所有点的距离的最大值

思路：补全邻接矩阵，Dijkstra 模版直接处理即可

# **Source Program**

```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<cstring>
#include<cmath>
#include<ctime>
#include<algorithm>
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-6
#define MOD 16007
#define INF 0x3f3f3f3f
#define N 1001
#define LL long long
using namespace std;
int n,m;
struct Edge{//边
    int from;//下一条边的编号
    int to;//边到达的点
    int dis;//边的长度
    Edge(int f,int t,int d){//构造函数
        from=f;
        to=t;
        dis=d;
    }
};

struct HeapNode{//Dijkstra用到的优先队列的结点
    int dis;//点到起点距离
    int u;//点的序号
    HeapNode(int a,int b){
        dis=a;
        u=b;
    }
    bool operator < (const HeapNode &rhs) const  {
        return dis > rhs.dis;
    }
};

struct Dijkstra{
    int n,m;//边数、点数，均从0开始
    vector<Edge> edges;//边列表
    vector<int> G[N];//每个结点出发的边的编号
    bool vis[N];//是否走过
    int dis[N];//起点s到各点的距离
    int p[N];//从起点s到i的最短路中的最后一条边的编号

    void init(int n) {//初始化
        this->n = n;
        for(int i=0;i<n;i++) //清空邻接表
            G[i].clear();
        edges.clear();//清空边列表
    }

    void AddEdge(int from,int to,int diss) {//添加边，若为无向图，调用两次
        edges.push_back(Edge(from,to,diss));
        m=edges.size();//边的个数
        G[from].push_back(m-1);//添加至边列表
    }

    void dijkstra(int s) {//求s到所有点的距离

        memset(dis,INF,sizeof(dis));
        memset(vis,false,sizeof(vis));
        dis[0]=0;

        priority_queue<HeapNode> Q;//优先队列
        Q.push(HeapNode(0,s));
        while(!Q.empty()) {
            HeapNode x=Q.top();
            Q.pop();

            int u=x.u;
            if(vis[u])//若已被访问
                continue;

            vis[u]=true;//标记为已访问
            for(int i=0;i<G[u].size();i++) {//枚举所有与当前点相连的边
                Edge &e=edges[G[u][i]];
                if(dis[e.to] > dis[u]+e.dis) {//更新距离
                    dis[e.to] = dis[u]+e.dis;
                    Q.push(HeapNode(dis[e.to],e.to));
                }
            }
        }
    }
}DJ;

int main()
{
    while(scanf("%d",&n)!=EOF&&n)
    {
        DJ.init(n);//初始化
        for(int i=1;i<n;i++){
            for(int j=0;j<i;j++){
                char str[10];
                scanf("%s",str);
                if(str[0]!='x'){
                    int dis;
                    sscanf(str,"%d",&dis);
                    DJ.AddEdge(i,j,dis);
                    DJ.AddEdge(j,i,dis);
                }
            }
        }

        DJ.dijkstra(0);
        int maxx=-INF;
        for(int i=0;i<n;i++)
            maxx=max(maxx,DJ.dis[i]);
        printf("%d\n",maxx);

    }
    return 0;
}
```






