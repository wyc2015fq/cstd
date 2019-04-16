# HDU5521 Meeting 最短路dijkstra算法（关键在于建图） - 紫芝的博客 - CSDN博客





2018年09月07日 13:02:02[紫芝](https://me.csdn.net/qq_40507857)阅读数：35








# Meeting

****Time Limit: 12000/6000 MS (Java/Others)    Memory Limit: 262144/262144 K (Java/Others)Total Submission(s): 5085    Accepted Submission(s): 1600****

Problem Description

Bessie and her friend Elsie decide to have a meeting. However, after Farmer John decorated his

fences they were separated into different blocks. John's farm are divided into *n* blocks labelled from 1 to *n*.

Bessie lives in the first block while Elsie lives in the *n*-th one. They have a map of the farm

which shows that it takes they *t**i* minutes to travel from a block in *E**i* to another block

in *E**i* where *E**i* (1≤*i*≤*m*) is a set of blocks. They want to know how soon they can meet each other

and which block should be chosen to have the meeting.

Input

The first line contains an integer *T* (1≤*T*≤6), the number of test cases. Then *T* test cases follow.


The first line of input contains *n* and *m*. 2≤*n*≤105. The following *m* lines describe the sets *E**i* (1≤*i*≤*m*). Each line will contain two integers *t**i*(1≤*t**i*≤109) and *S**i* (*S**i*>0) firstly. Then *S**i* integer follows which are the labels of blocks in *E**i*. It is guaranteed that ∑*m**i*=1*S**i*≤106.

Output

For each test case, if they cannot have the meeting, then output "Evil John" (without quotes) in one line.


Otherwise, output two lines. The first line contains an integer, the time it takes for they to meet.

The second line contains the numbers of blocks where they meet. If there are multiple

optional blocks, output all of them in ascending order.

Sample Input

```cpp
2
5 4
1 3 1 2 3
2 2 3 4
10 2 1 5
3 3 3 4 5
3 1
1 2 1 2
```

Sample Output

```cpp
Case #1: 3 
3 4 
Case #2: Evil John
```

***Hint***

In the first case, it will take Bessie 1 minute travelling to the 3rd block, and it will take Elsie 3 minutes travelling to the 3rd block. It will take Bessie 3 minutes travelling to the 4th block, and it will take Elsie 3 minutes travelling to the 4th block. In the second case, it is impossible for them to meet.

Source

[2015ACM/ICPC亚洲区沈阳站-重现赛（感谢东北大学）](http://acm.hdu.edu.cn/search.php?field=problem&key=2015ACM%2FICPC%D1%C7%D6%DE%C7%F8%C9%F2%D1%F4%D5%BE-%D6%D8%CF%D6%C8%FC%A3%A8%B8%D0%D0%BB%B6%AB%B1%B1%B4%F3%D1%A7%A3%A9&source=1&searchmode=source)

Recommend

wange2014   |   We have carefully selected several similar problems for you:  [6447](http://acm.hdu.edu.cn/showproblem.php?pid=6447)[6446](http://acm.hdu.edu.cn/showproblem.php?pid=6446)[6445](http://acm.hdu.edu.cn/showproblem.php?pid=6445)[6444](http://acm.hdu.edu.cn/showproblem.php?pid=6444)[6443](http://acm.hdu.edu.cn/showproblem.php?pid=6443)

题意：

给n个点，m个集合，每个集合有s个点，同一个集合中两点之间的时间为t，求起点（1点）到终点（n点）的最短时间，以及满足最短时间的点，若无法到达，则输出"Evil John"。.

分析：

由于s <= 10^6,如果集合内的点两两建边，进行最短路必定超时。

![](https://img-blog.csdn.net/20180907212108612?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

1.考虑给每个集合建立一个虚拟节点，虚拟节点到集合内的点的时间为t/2，可能会产生浮点数误差，所以建边的时候，虚拟节点到集合内每个节点的时间假设为t，最后的最短时间除以2即可。

![](https://img-blog.csdn.net/20180907213029863?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

2.以1点为起点，求出起点到n+m点（n个实际节点，m个虚拟节点）的最短时间dis[0][]；

以n点为起点，求出起点到n+m点（n个实际节点，m个虚拟节点）的最短时间dis[1][]；

3.然后遍历1到n个点，dis[0][i]为起点1到i点的最短距离，dis[1][I]为终点n到i点的最短时间，取二者的最大值即为i点的见面时间，再取1-n中的最小值即为最短距离，最后判断1-n中有多少个点的见面时间为最短时间，输出即可

```cpp
//hdu5521
#include<bits/stdc++.h>
using namespace std;
typedef long long LL;
int n,m;
vector <int> ans;
const LL INF = 0x3f3f3f3f3f;
const int MAXN = 3200010;
struct qnode
{
    int v;
    LL c;
    qnode (int _v = 0, LL _c = 0) : v(_v), c(_c) {}
    bool operator < (const qnode &r) const { return c > r.c; }
};
struct Edge
{
    int v;
    LL cost;
    Edge (int _v = 0,LL _cost = 0) : v(_v), cost (_cost){}
};
vector <Edge> E[MAXN];
bool vis[MAXN];
LL dist[2][MAXN];
void dijkstra(int n,int start,int poi) //点从编号1开始
{
    memset(vis,0,sizeof(vis));
    for (int i = 1; i <= MAXN; i++) dist[poi][i] = INF;
    priority_queue <qnode> que;
    while(!que.empty()) que.pop();
    dist[poi][start] = 0;
    que.push(qnode(start,0));
    qnode tmp;
    while(!que.empty())
    {
        tmp = que.top();
        que.pop();
        int u = tmp.v;
        if (vis[u]) continue;
        vis[u] = true;
        for (int i = 0; i < E[u].size(); i++)
        {
            int v = E[tmp.v][i].v;
            LL cost = E[u][i].cost;
            if (!vis[v] && dist[poi][v] > dist[poi][u] + cost)
            {
                dist[poi][v] = dist[poi][u] + cost;
                que.push(qnode(v,dist[poi][v]));
            }
        }
    }
}
void addedge(int u,int v,LL w)
{
    E[u].push_back(Edge(v,w));
}

int main(){
    int T;
    cin >> T;
    int cas = 1;
    while(T--)
    {
        for (int i = 0; i < MAXN; i++)
        {
            E[i].clear();
        }
        scanf("%d%d",&n,&m);
        int pos = n;    //用于记录新创建的点
        while(m--)
        {
            LL t;
            int s;
            scanf("%lld%d",&t,&s);
            pos++;
            while(s--)
            {
                int u;
                scanf("%d",&u);
                addedge(u,pos,t);
                addedge(pos,u,t);
            }
        }
        dijkstra(pos,1,0);  //从1出发，结果保存在dist[0][]中
        dijkstra(pos,n,1); //从n出发，结果保存在dist[1][]中
        LL mindis = INF;
        ans.clear();
        for (int i = 1; i <= n; i++)
        {
            mindis = min(mindis,max(dist[0][i],dist[1][i]));
        }
        if (mindis == INF) printf("Case #%d: Evil John\n",cas++);
        else
        {
            printf("Case #%d: %lld\n",cas++,mindis / 2);
            for (int i = 1; i <= n; i++)
            {
                if (mindis == max(dist[0][i],dist[1][i]))
                {
                    ans.push_back(i);
                }
            }
            printf("%d",ans[0]);
            for (int i = 1; i < ans.size(); i++)
            {
                printf(" %d",ans[i]);
            }
            printf("\n");
        }
    }
    return 0;
}
```





