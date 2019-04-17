# nyoj42——连通图加欧拉(连通图板子）dfs - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





## 一笔画问题

时间限制：3000 ms  |  内存限制：65535 KB

难度：4


- 描述

zyc从小就比较喜欢玩一些小游戏，其中就包括画一笔画，他想请你帮他写一个程序，判断一个图是否能够用一笔画下来。

规定，所有的边都只能画一次，不能重复画。




- 输入
第一行只有一个正整数N(N<=10)表示测试数据的组数。
每组测试数据的第一行有两个正整数P,Q(P<=1000,Q<=2000)，分别表示这个画中有多少个顶点和多少条连线。（点的编号从1到P）
随后的Q行，每行有两个正整数A,B(0<A,B<P)，表示编号为A和B的两点之间有连线。- 输出
如果存在符合条件的连线，则输出"Yes",
如果不存在符合条件的连线，输出"No"。- 样例输入
2
4 3
1 2
1 3
1 4
4 5
1 2
2 3
1 3
1 4
3 4- 样例输出
No
Yes
本题来自：[http://acm.nyist.net/JudgeOnline/problem.php?pid=42](http://acm.nyist.net/JudgeOnline/problem.php?pid=42)

问题分析：

欧拉定理   如果一个网络是连通的并且奇顶点的个数等于0或2，那么它可以一笔画出；否则它不可以一笔画出。

判断一笔画的方法：

　　①是连通的。一个图，如果图上任意二点总有线段连接着，就称为连通的。不是连通的就不能一笔画出。

　　②奇点个数是0或者是2。图上线段的端点可以分成二类，奇点和偶点。一个点，以它为端点的线段数是奇数就称为奇点，线段数是偶数就称为偶点。

　　一个图是否是一笔画就看奇点的个数，奇点个数是 0 或者 2，就是一笔画，否则就不是一笔画。

所以这个问题完全可以转化策略为：

           第一步： 首先我们不管它三七二十几，先进行连通性的判断。

           第二步：

                      （1）如果是连通的，我们来判断此图的度的奇点的个数是0或者是2 ，如果是，则说明这个是欧拉图，即可以一笔画出，反之则不能一笔画出

                      （2）如果是非连通的，这说明这个图肯定不能一笔画出。

```
#include <bits/stdc++.h>
using namespace std;
typedef long long  ll;
const int INF = 0x3f3f3f3f;
const int maxn = 1005;

int V;
bool mp[maxn][maxn];
int vis[maxn];
int bian[maxn];

void dfs(int x)
{
    vis[x] = 1;
    for(int i=1;i <= V;i++){
        if(mp[i][x]&&!vis[i])
            dfs(i);
    }
}


int main()
{
    int t;
    scanf("%d",&t);
    while(t--){
        memset(vis,0, sizeof(vis));
        memset(mp,0,sizeof(mp));
        memset(bian,0,sizeof(bian));
        int n;
        scanf("%d%d",&V,&n);
        for(int i=1;i <= n;i++){
            int a,b;
            scanf("%d%d",&a,&b);
            mp[a][b] = 1;
            mp[b][a] = 1;
        }
        dfs(1);
        int flag = 0;
        for(int i=1;i <= V;i++){
            if(vis[i] == 0){
                flag = 1;
                break;
            }
        }
        if(flag == 1) printf("No\n");
        else{
            int num = 0;
            for(int i=1;i <= V;i++){
                int cnt = 0;
                for(int j=1;j <= V;j++){
                    if(mp[i][j])cnt++;
                }
                if(cnt%2 == 1) num++;
            }
            if(num == 0||num == 2) printf("Yes\n");
            else printf("No\n");
        }
    }
    return 0;
}
```

——dfs图板子











