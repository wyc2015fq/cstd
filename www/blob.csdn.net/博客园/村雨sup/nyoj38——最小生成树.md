# nyoj38——最小生成树 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)






## 布线问题

时间限制：1000 ms  |  内存限制：65535 KB

难度：4



- 描述
南阳理工学院要进行用电线路改造，现在校长要求设计师设计出一种布线方式，该布线方式需要满足以下条件：
1、把所有的楼都供上电。
2、所用电线花费最少

- 输入
第一行是一个整数n表示有n组测试数据。(n<5)
每组测试数据的第一行是两个整数v,e.
v表示学校里楼的总个数(v<=500)
随后的e行里，每行有三个整数a,b,c表示a与b之间如果建铺设线路花费为c(c<=100)。（哪两栋楼间如果没有指明花费，则表示这两栋楼直接连通需要费用太大或者不可能连通）
随后的1行里，有v个整数,其中第i个数表示从第i号楼接线到外界供电设施所需要的费用。( 0<e<v*(v-1)/2 )
（楼的编号从1开始），由于安全问题，只能选择一个楼连接到外界供电设备。
数据保证至少存在一种方案满足要求。- 输出
每组测试数据输出一个正整数,表示铺设满足校长要求的线路的最小花费。- 样例输入
1
4 6
1 2 10
2 3 10
3 1 10
1 4 1
2 4 1
3 4 1
1 3 5 6- 样例输出
4
```
#include <bits/stdc++.h>
using namespace std;
typedef long long  ll;
const int INF = 0x3f3f3f3f;
const int maxn = 505;

int cost[maxn][maxn];
int mincost[maxn];
bool used[maxn];
int V;  //V顶点数

int prim()
{
    int res = 0;
    for(int i=1;i <= V;i++){
        used[i] = false;
    }
    mincost[1] = 0;
    while(1){
        int v = -1;
        for(int u = 1;u <= V;u++){
            if(!used[u]&&(v == -1||mincost[u] < mincost[v]))
                v = u;
        }
        if(v == -1) break;
        used[v] = true;
        res += mincost[v];
        for(int u=1;u <= V;u++){
            mincost[u] = min(mincost[u],cost[u][v]);
        }
    }
    return res;
}


int main()
{
    int t;
    scanf("%d",&t);
    while(t--){
        memset(cost,INF, sizeof(cost));
        memset(mincost,INF, sizeof(mincost));
        int n;
        scanf("%d%d",&V,&n);
        for(int i=1;i <= n;i++){
            int a,b,c;
            scanf("%d%d%d",&a,&b,&c);
            cost[a][b] = c;
            cost[b][a] = c;
        }
        int ans = prim();
        int minnum = INF;
        for(int i=1;i <= V;i++){
            int x;
            scanf("%d",&x);
            minnum = min(minnum,x);
        }
        printf("%d\n",ans+minnum);
    }
    return 0;
}
```

复习了一遍最小生成树的写法。

这里考了一个最小生成树的概念就是，无论你从哪点出发，最小生成树的值是一定的。











