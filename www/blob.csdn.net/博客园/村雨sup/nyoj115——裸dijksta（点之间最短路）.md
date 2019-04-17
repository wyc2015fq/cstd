# nyoj115——裸dijksta（点之间最短路） - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)






## 城市平乱

时间限制：1000 ms  |  内存限制：65535 KB

难度：4



- 描述

南将军统领着N个部队，这N个部队分别驻扎在N个不同的城市。

他在用这N个部队维护着M个城市的治安，这M个城市分别编号从1到M。

现在，小工军师告诉南将军，第K号城市发生了暴乱，南将军从各个部队都派遣了一个分队沿最近路去往暴乱城市平乱。

现在已知在任意两个城市之间的路行军所需的时间，你作为南将军麾下最厉害的程序员，请你编写一个程序来告诉南将军第一个分队到达叛乱城市所需的时间。

![](http://acm.nyist.edu.cn/JudgeOnline/admin/kind/attached/20101122174626_27048.png)

注意，两个城市之间可能不只一条路。


- 输入
第一行输入一个整数T，表示测试数据的组数。(T<20)
每组测试数据的第一行是四个整数N,M,P,Q(1<=N<=100,N<=M<=1000,M-1<=P<=100000)其中N表示部队数，M表示城市数，P表示城市之间的路的条数,Q表示发生暴乱的城市编号。
随后的一行是N个整数，表示部队所在城市的编号。
再之后的P行，每行有三个正整数，a,b,t(1<=a,b<=M,1<=t<=100)，表示a,b之间的路如果行军需要用时为t

数据保证暴乱的城市是可达的。- 输出
对于每组测试数据，输出第一支部队到达叛乱城市时的时间。每组输出占一行- 样例输入
1
3 8 9 8
1 2 3
1 2 1
2 3 2
1 4 2
2 5 3
3 6 2
4 7 1
5 7 3
5 8 2
6 8 2 - 样例输出
4
```
#include <bits/stdc++.h>
using namespace std;
typedef long long  ll;
const int INF = 0x3f3f3f3f;
const int maxn = 1005;
int V,n,p,q;
int vis[maxn];
int mp[maxn][maxn];
int dis[maxn];
int a[maxn];  //队伍编号

void dig(int x)
{
    vis[x] = 1;
    for(int i=1;i <= V;i++){
        dis[i] = mp[x][i];
    }
    while(1){                 //故意写的像最小生成树，便于记忆
        int minn = INF;
        int v = -1;
        for(int i=1;i <= V;i++){   //找到dis中最短路     
            if(vis[i] == 0&&dis[i] < minn){
                minn = dis[i];
                v = i;
            }
        }
        if(v == -1) break;
        vis[v] = 1;
        for(int i=1;i <= V;i++){   //更新dis值
            if(mp[v][i] < INF)
                dis[i] = min(dis[i],mp[v][i]+dis[v]);
        }
    }
}





int main()
{
    int t;
    scanf("%d",&t);
    while(t--){

        scanf("%d%d%d%d",&n,&V,&p,&q);
        for(int i=0;i < n;i++){
            scanf("%d",&a[i]);
        }
        memset(vis,0, sizeof(vis));
        for(int i=0;i <= V;i++){     //二维数组不能用memset
            for(int j=0;j <= V;j++) {
                mp[i][j] = INF;
                mp[i][i] = 0;
            }
        }
        for(int i=1;i <= p;i++){
            int a,b,c;
            scanf("%d%d%d",&a,&b,&c);
            if(mp[a][b] > c)     //两点之间可能有多条路径
                mp[a][b] = mp[b][a] = c;
        }
        dig(q);
        int mii = INF;
        for(int i=0;i < n;i++){
            mii = min(mii,dis[a[i]]);
        }
        printf("%d\n",mii);
    }

    return 0;
}
```

——











