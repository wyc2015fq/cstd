# UVA 1660 Cable TV Network 电视网络(无向图，点连通度，最大流) - xcw0754 - 博客园
# [UVA 1660 Cable TV Network 电视网络(无向图，点连通度，最大流)](https://www.cnblogs.com/xcw0754/p/4662429.html)
题意：给一个无向图，求其点连通度？（注意输入问题）
思路：
　　如果只有1个点，那么输出“1”；
　　如果有0条边，那么输出“0”；
　　其他情况：用最大流解决。下面讲如何建图：
 　　图的连通度问题是指：在图中删去部分元素（点或边），使得图中指定的两个点s和t不连通（即不存在从s到t的路径），求至少要删去几个元素。
　　图的连通度分为点连通度和边连通度：
　　　　（1）点连通度：只许删点，求至少要删掉几个点（当然，s和t不能删去，这里保证原图中至少有三个点）；
　　　　（2）边连通度：只许删边，求至少要删掉几条边。
　　并且，有向图和无向图的点连通度求法不同，因此还要分开考虑。说明：最大流对应的是最小割。
　　【有向图】：这个其实就是最小割问题。以s为源点，t为汇点建立网络，原图中的每条边在网络中仍存在，容量为1。
　　【无向图】：需要拆点，每个点都拆成两个点v和v'，并连1条有向边v->v'，容量为1。点v承接原图中所有入边，点v'承接原图中所有出边。那么对于原图每条有向边就得建两条边了，容量无穷，且涉4个新顶点。
　　【混合图】无向边按无向图处理，有向边按有向图处理。
　　最后，源点S到汇点，跑一次最大流，就得到了答案。如果无指定ST，那么应该顶下1个S，再穷举其他点作为T。这个S应该如何定就不知道了，时间充裕就穷举S和T，不充裕就找个度最少的为S。
　　针对此题，随意定个0号点为源点也能过，当然穷举两点也能过。
```
1 #include <bits/stdc++.h>
  2 #define LL long long
  3 #define pii pair<int,int>
  4 #define INF 0x7f7f7f7f
  5 using namespace std;
  6 const int N=110;
  7 int n, m, edge_cnt;
  8 vector<int> vect[N*2];
  9 int path[N], flow[N];
 10 
 11 struct node
 12 {
 13     int from, to, cap, flow;
 14     node(){};
 15     node(int f,int t,int c,int fl):from(f),to(t),cap(c),flow(fl){};
 16 }edge[50000], cpy[50000];
 17 
 18 void add_node(int from, int to, int cap, int flow)
 19 {
 20     edge[edge_cnt]=node(from,to,cap,flow);
 21     vect[from].push_back(edge_cnt++);
 22 }
 23 
 24 int BFS(int s,int e)
 25 {
 26     deque<int> que(1,s);
 27     flow[s]=INF;
 28 
 29     while(!que.empty())
 30     {
 31         int x=que.front();
 32         que.pop_front();
 33         for(int i=0; i<vect[x].size(); i++)
 34         {
 35             node e=cpy[vect[x][i]];
 36             if(!flow[e.to] && e.cap>e.flow )
 37             {
 38                 flow[e.to]=min(flow[e.from],e.cap-e.flow);
 39                 path[e.to]=vect[x][i];
 40                 que.push_back(e.to);
 41             }
 42         }
 43         if(flow[e]) break;
 44     }
 45     return flow[e];
 46 }
 47 
 48 int max_flow(int s,int e)
 49 {
 50     int ans_flow=0;
 51     while(true)
 52     {
 53         memset(path,0,sizeof(path));
 54         memset(flow,0,sizeof(flow));
 55 
 56         int tmp=BFS(s,e);
 57         if(!tmp)    return ans_flow;
 58         ans_flow+=tmp;
 59 
 60         int ed=e;
 61         while(ed!=s)
 62         {
 63             int t=path[ed];
 64             cpy[t].flow+=tmp;
 65             cpy[t^1].flow-=tmp;
 66             ed=cpy[t].from;
 67         }
 68     }
 69 }
 70 
 71 int cal()
 72 {
 73     int ans=INF;
 74 
 75     for(int i=0; i<edge_cnt; i++)
 76     {
 77         if( edge[i].from==0 && edge[i].to==1 && edge[i].cap>0 )
 78         {
 79             edge[i].cap=INF;
 80             break;
 81         }
 82     }
 83     for(int k=1; k<n; k++)  //枚举汇点
 84     {
 85         memcpy(cpy, edge, sizeof(edge));
 86         for(int i=0; i<edge_cnt; i++)   //所有边
 87         {
 88             if( cpy[i].from==k*2 && cpy[i].to==k*2+1 &&cpy[i].cap>0 )
 89             {
 90                 cpy[i].cap=INF;
 91                 break;
 92             }
 93         }
 94         ans=min(ans, max_flow(0,k*2+1));
 95     }
 96     return ans==INF?n:ans;
 97 }
 98 
 99 
100 int main()
101 {
102     freopen("input.txt", "r", stdin);
103     int a, b;
104     char c;
105     while(cin>>n>>m)
106     {
107         edge_cnt=0;
108         memset(edge,0,sizeof(edge));
109         for(int i=n*2; i>=0; i--) vect[i].clear();
110 
111         for(int i=0; i<n; i++)  //拆点
112         {
113             add_node(i*2, i*2+1, 1, 0);
114             add_node(i*2+1, i*2, 0, 0);
115         }
116 
117         for(int i=0; i<m; i++)
118         {
119             while(c=getchar(), c!='(' );
120             scanf("%d%c%d%c",&a,&c,&b,&c);
121 
122             add_node(a*2+1, b*2, INF, 0 );  //每条无向边拆2条有向
123             add_node(b*2, a*2+1, 0, 0 );
124 
125             add_node(b*2+1, a*2, INF, 0);
126             add_node(a*2, b*2+1, 0, 0);
127         }
128         printf("%d\n",cal());
129     }
130     return 0;
131 }
AC代码(任意源)
```
```
#include <bits/stdc++.h>
#define LL long long
#define pii pair<int,int>
#define INF 0x7f7f7f7f
using namespace std;
const int N=110;
int n, m, edge_cnt;
vector<int> vect[N*2];
int path[N], flow[N];
struct node
{
    int from, to, cap, flow;
    node(){};
    node(int f,int t,int c,int fl):from(f),to(t),cap(c),flow(fl){};
}edge[50000], cpy[50000], old[50000];
void add_node(int from, int to, int cap, int flow)
{
    edge[edge_cnt]=node(from,to,cap,flow);
    vect[from].push_back(edge_cnt++);
}
int BFS(int s,int e)
{
    deque<int> que(1,s);
    flow[s]=INF;
    while(!que.empty())
    {
        int x=que.front();
        que.pop_front();
        for(int i=0; i<vect[x].size(); i++)
        {
            node e=cpy[vect[x][i]];
            if(!flow[e.to] && e.cap>e.flow )
            {
                flow[e.to]=min(flow[e.from],e.cap-e.flow);
                path[e.to]=vect[x][i];
                que.push_back(e.to);
            }
        }
        if(flow[e]) break;
    }
    return flow[e];
}
int max_flow(int s,int e)
{
    int ans_flow=0;
    while(true)
    {
        memset(path,0,sizeof(path));
        memset(flow,0,sizeof(flow));
        int tmp=BFS(s,e);
        if(!tmp)    return ans_flow;
        ans_flow+=tmp;
        int ed=e;
        while(ed!=s)
        {
            int t=path[ed];
            cpy[t].flow+=tmp;
            cpy[t^1].flow-=tmp;
            ed=cpy[t].from;
        }
    }
}
int cal()
{
    int ans=INF;
    for(int s=0; s<n; s++)  //枚举源点
    {
        memcpy(old, edge, sizeof(edge));
        for(int i=0; i<edge_cnt; i++)
        {
            if( old[i].from==s*2 && old[i].to==s*2+1 && old[i].cap>0 )
            {
                old[i].cap=INF;
                break;
            }
        }
        for(int k=s+1; k<n; k++)  //枚举汇点
        {
            memcpy(cpy, old, sizeof(old));
            for(int i=0; i<edge_cnt; i++)   //所有边
            {
                if( cpy[i].from==k*2 && cpy[i].to==k*2+1 &&cpy[i].cap>0 )
                {
                    cpy[i].cap=INF;
                    break;
                }
            }
            ans=min(ans, max_flow(s*2,k*2+1));
        }
    }
    return ans==INF?n:ans;
}
int main()
{
    //freopen("input.txt", "r", stdin);
    int a, b;
    char c;
    while(cin>>n>>m)
    {
        edge_cnt=0;
        memset(edge,0,sizeof(edge));
        for(int i=n*2; i>=0; i--) vect[i].clear();
        for(int i=0; i<n; i++)  //拆点
        {
            add_node(i*2, i*2+1, 1, 0);
            add_node(i*2+1, i*2, 0, 0);
        }
        for(int i=0; i<m; i++)
        {
            while(c=getchar(), c!='(' );
            scanf("%d%c%d%c",&a,&c,&b,&c);
            add_node(a*2+1, b*2, INF, 0 );  //每条无向边拆2条有向
            add_node(b*2, a*2+1, 0, 0 );
            add_node(b*2+1, a*2, INF, 0);
            add_node(a*2, b*2+1, 0, 0);
        }
        printf("%d\n",cal());
    }
    return 0;
}
AC代码（穷举S和T）
```

