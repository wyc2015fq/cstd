# nyoj20——有向无环图深搜模板 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)






## 吝啬的国度

时间限制：1000 ms  |  内存限制：65535 KB

难度：3



- 描述
在一个吝啬的国度里有N个城市，这N个城市间只有N-1条路把这个N个城市连接起来。现在，Tom在第S号城市，他有张该国地图，他想知道如果自己要去参观第T号城市，必须经过的前一个城市是几号城市（假设你不走重复的路）。

- 输入
第一行输入一个整数M表示测试数据共有M(1<=M<=5)组
每组测试数据的第一行输入一个正整数N(1<=N<=100000)和一个正整数S(1<=S<=100000)，N表示城市的总个数，S表示参观者所在城市的编号
随后的N-1行，每行有两个正整数a,b(1<=a,b<=N)，表示第a号城市和第b号城市之间有一条路连通。- 输出
每组测试数据输N个正整数，其中，第i个数表示从S走到i号城市，必须要经过的上一个城市的编号。（其中i=S时，请输出-1）- 样例输入
1
10 1
1 9
1 8
8 10
10 3
8 6
1 2
10 4
9 5
3 7
- 样例输出
-1 1 10 10 9 8 3 1 1 8



```
#include <bits/stdc++.h>
using namespace std;
typedef long long  ll;
const int INF = 0x3f3f3f3f;
const int maxn = 1e5+6;

int pre[maxn];
vector<int>v[maxn];

void dfs(int x)
{
    for(int i=0;i < v[x].size();i++){
        if(pre[v[x][i]] == 0){  //表示与x相连的节点未被遍历过
            pre[v[x][i]] = x;
            dfs(v[x][i]);
        }
    }
}


int main()
{
    int t;
    scanf("%d",&t);
    while(t--){
        memset(pre,0, sizeof(pre));
        memset(v,0, sizeof(v));
        int n,s;
        scanf("%d%d",&n,&s);
        pre[s] = -1;
        for(int i=0;i < n-1;i++){
            int a,b;
            scanf("%d%d",&a,&b);
            v[a].push_back(b);
            v[b].push_back(a);
        }
        dfs(s);
        for(int i=1;i <= n;i++){
            printf("%d ",pre[i]);
        }
    }
    return 0;
}
```

一开始我不是很理解题意，什么叫“必须要经过的上一个城市的编号”，看了别人的代码更加迷惑。

后来看到是输入 n-1 组关系数据，瞬间明白了，是有向无环图，所以说不会出现有多条路径到达同一点的情况。











