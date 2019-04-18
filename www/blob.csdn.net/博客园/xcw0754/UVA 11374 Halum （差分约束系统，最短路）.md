# UVA 11374 Halum （差分约束系统，最短路） - xcw0754 - 博客园
# [UVA 11374 Halum （差分约束系统，最短路）](https://www.cnblogs.com/xcw0754/p/4693415.html)
题意：给定一个带权有向图，每次你可以选择一个结点v 和整数d ，把所有以v为终点的边权值减少d，把所有以v为起点的边权值增加d，最后要让所有的边权值为正，且尽量大。若无解，输出结果。若可无限大，输出结果。否则，输出最小边权的最大值。
思路：差分约束系统用最短路来解。列式子后建图，新图的边就是原图的边，权值也不变。有3种情况，要分开判断。
　　（1）若连最小的权值1都达不到，肯定无解。
　　（2）若可以超过所给边的最大权值，那么最小权值肯定可以继续增大。
　　（3）接下来用二分猜答案，答案范围在[1,big]。只要无负环，就是可取的。
　　注意：很容易超时，优化一下spfa吧。
700ms+
```
#include <bits/stdc++.h>
#define INF 0x7f7f7f7f
#define pii pair<int,int>
#define LL unsigned long long
using namespace std;
const int N=550;
struct node
{
    int from,to,cost;
    node(){};
    node(int from,int to,LL cost):from(from),to(to),cost(cost){};
}edge[4000];
vector<int > vect[N];
int edge_cnt;
void add_node(int from,int to,int cost)
{
    edge[edge_cnt]=node(from,to,cost);
    vect[from].push_back(edge_cnt++);
}
int inq[N], cost[N], cnt[N];
bool spfa(int s, int n, int d)
{
    memset(cost, 0, sizeof(cost));
    memset(inq, 0, sizeof(inq));
    memset(cnt, 0, sizeof(cnt));
    deque<int> que(1,s);
    inq[s]=1;
    while(!que.empty())
    {
        int x=que.front();que.pop_front();
        inq[x]=0;
        for(int i=0; i<vect[x].size(); i++)
        {
            node e=edge[vect[x][i]];
            if(cost[e.to]>cost[x]+e.cost-d )
            {
                cost[e.to]=cost[x]+e.cost-d;
                if(!inq[e.to])
                {
                    inq[e.to]=1;
                    if(++cnt[e.to]>n)   return false;
                    //优化很有用
                    if(!que.empty()&&cost[e.to]<cost[que.front()])    que.push_front(e.to);
                    else    que.push_back(e.to);
                }
            }
        }
    }
    return true;
}
int cal(int s,int n, int L, int R)
{
    while(L<R)
    {
        LL mid=L+(R-L+1)/2;
        if( spfa(s, n, mid)  )    L=mid;    //不产生环，则满足要求
        else    R=mid-1;        //大过头了也可能产生环的
    }
    return L;
}
int main()
{
    freopen("input.txt", "r", stdin);
    int n, m, a, b, c;
    while(~scanf("%d %d",&n,&m))
    {
        edge_cnt=0;
        memset(edge,0,sizeof(edge));
        for(int i=0; i<=n; i++) vect[i].clear();
        int big=0;
        for(int i=0; i<m; i++)
        {
            scanf("%d %d %d", &a, &b, &c);
            add_node(a, b, c);
            big=max(big, c);
        }
        for(int i=1; i<=n; i++) add_node(0, i, 0);  //加源点
        if( spfa(0, n, big+1) )   //如果连最大权都能超过，肯定可以inf。因为若有环，一般都是有权加就有权减。
        {
            puts("Infinite");
            continue;
        }
        if( !spfa(0, n, 1) )    //没有大于0的边
        {
            puts("No Solution");
            continue;
        }
        printf("%d\n",  cal(0, n, 1, big) );
    }
    return 0;
}
AC代码
```

