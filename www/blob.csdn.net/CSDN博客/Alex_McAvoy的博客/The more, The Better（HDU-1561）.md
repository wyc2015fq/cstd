# The more, The Better（HDU-1561） - Alex_McAvoy的博客 - CSDN博客





2019年01月23日 17:49:22[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：36
个人分类：[HDU																[动态规划——树形 DP](https://blog.csdn.net/u011815404/article/category/8104361)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

ACboy很喜欢玩一种战略游戏，在一个地图上，有N座城堡，每座城堡都有一定的宝物，在每次游戏中ACboy允许攻克M个城堡并获得里面的宝物。但由于地理位置原因，有些城堡不能直接攻克，要攻克这些城堡必须先攻克其他某一个特定的城堡。你能帮ACboy算出要获得尽量多的宝物应该攻克哪M个城堡吗？ 

# Input

每个测试实例首先包括2个整数，N,M.(1 <= M <= N <= 200);在接下来的N行里，每行包括2个整数，a,b. 在第 i 行，a 代表要攻克第 i 个城堡必须先攻克第 a 个城堡，如果 a = 0 则代表可以直接攻克第 i 个城堡。b 代表第 i 个城堡的宝物数量, b >= 0。当N = 0, M = 0输入结束。

# Output

对于每个测试实例，输出一个整数，代表ACboy攻克M个城堡所获得的最多宝物的数量。

# Sample Input

**3 20 10 20 37 42 20 10 42 17 17 62 20 0**

# Sample Output

**513**


————————————————————————————————————————————

思路：树形 DP

设 dp[i][j] 为以第 i 号为根取 j 个的最大值，显然，当以 i 号结点为根取 1 个时，其值为第 i 号城堡本省的价值，即 dp[i][1]=w[i]

以样例 2 为例，可以建得如下的一个图

![](https://img-blog.csdnimg.cn/20190123174333705.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

选择一棵树，从根节点向子节点直接进行动态规划，由于对任意结点 i 有 dp[i][1]=w[i]，那么如果要求 dp[i][j] 的话，就要对其子树达到最优后再解决当前的问题，即有：dp[i][j]=max{ dp[i][j],dp[i][k]+dp[子节点][i-k] }，其中 k 代表取k个点

得到状态转移方程后，可以发现，图是一个森林，这个时候，就需要加一个虚拟的根节点，将其权值设为 0，从其开始进行状态转移，这样一来，就要求包括其自身在内的 m+1 个点的最优值

![](https://img-blog.csdnimg.cn/20190123174858408.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

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
#define MOD 1000000007
#define INF 0x3f3f3f3f
#define N 1001
#define LL long long
using namespace std;
struct Edge{
    int next;
    int to;
}edge[N];
int head[N],cnt;
int n,m;
int dp[N][N];//以i为根攻破j个城堡的最大值
bool vis[N];
void addEdge(int next,int to){
    edge[cnt].to=to;
    edge[cnt].next=head[next];
    head[next]=cnt++;
}
int dfs(int x){//以x为根结点向下搜索
    vis[x]=true;
    for(int i=head[x];i!=-1;i=edge[i].next){
        int y=edge[i].to;//与x邻接的点
        if(!vis[y])
            dfs(y);

        for(int j=m+1;j>=2;j--)//除虚拟的根结点外，枚举要攻破的城堡个数
            for(int k=1;k<j;k++)//取k个点
                if(dp[y][j-k]!=-1&&dp[x][k]!=-1)
                    dp[x][j]=max(dp[x][j],dp[x][k]+dp[y][j-k]);

    }
}
int main(){
    while(scanf("%d%d",&n,&m)!=EOF&&(n+m)){
        memset(dp,-1,sizeof(dp));
        memset(head,-1,sizeof((head)));
        memset(vis,false,sizeof(vis));
        cnt=0;

        //由于图可能为一森林，因此加一虚拟根节点，使其值为0
        for(int i=0;i<=n;i++)
            dp[i][0]=0;
        dp[0][1]=0;

        for(int i=1;i<=n;i++){
            int x,y;
            scanf("%d%d",&x,&y);
            addEdge(x,i);
            dp[i][1]=y;//以第i个点为根攻破1的城堡的值一定是这个城堡自身的值
        }
        dfs(0);//以虚拟结点为根节点向下搜索
        printf("%d\n",dp[0][m+1]);
    }

	return 0;
}
```






