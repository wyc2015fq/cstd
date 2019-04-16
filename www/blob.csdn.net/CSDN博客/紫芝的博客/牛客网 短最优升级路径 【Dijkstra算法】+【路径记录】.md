# 牛客网  短最优升级路径 【Dijkstra算法】+【路径记录】 - 紫芝的博客 - CSDN博客





2018年11月13日 15:18:11[紫芝](https://me.csdn.net/qq_40507857)阅读数：321








链接：[https://www.nowcoder.com/questionTerminal/a7052c5bd8634edb9ccee711a5c1ea54](https://www.nowcoder.com/questionTerminal/a7052c5bd8634edb9ccee711a5c1ea54)

来源：牛客网

短最优升级路径

题目描述：游戏网站提供若干升级补丁，每个补丁大小不一，玩家要升级到最新版，如何选择下载哪些补丁下载量最小。

输入：

第一行输入                

第一个数为用户版本 第二个数为最新版本，空格分开

接着输入N行补丁数据       

第一个数补丁开始版本 第二个数为补丁结束版本 第三个数为补丁大小，空格分开

**输出：**

对于每个测试实例，输出一个升级路径以及最后实际升级的大小

**样例输入：**

1000 1050

1000 1020 50

1000 1030 70

1020 1030 15

1020 1040 30

1030 1050 40

1040 1050 20

**样例输出：**

1000->1020->1040->1050(100)

迪杰斯特拉最短路径算法 建立邻接矩阵，标识版本间的来去路线，求最初版本到最末版本之间的最短路径。

在dijkstra算法模板的基础上加上一个pre数组，用于记录该节点的上一个节点，即该点是经过哪一点才到达该点的。pre数组具体在边松弛的过程中进行重新赋值，松弛成功就将pre值记录k点，及该点是由起点经过k点后所得到的。最后把pre数组中的值递归输出一遍即可。

```cpp
#include <cstdio>
#include <cstring>
#include <queue>
#include <algorithm>
#include<bits/stdc++.h>
using namespace std;
typedef long long LL;
const int maxn=10000;
#define INF 0x3f3f3f3f
#define inf 0x3f3f3f3f
map<int,int> mp,rmp;
int road[maxn][maxn];
int dis[maxn];
bool vis[maxn];
int pre[maxn];
int n;

void dijkstra(int s,int e)
{//s为起点，e为终点
    memset(vis, false, sizeof(vis));//标记是否求出最短路径
    vis[s] = true;//标记起点到这一点的最小距离已经求出
    for(int i = 1; i < n; i++){
        dis[i] = road[s][i];//初始化起点到每一个点的距离
        pre[i]=s;//初始化路径，每个点的上一个点为起点
    }
    for(int u = 1; u < n-1; u++)
    {
        int minD =inf ,k = -1;
        for(int i = 1; i< n; i++)
        {   //寻找没有访问过的最短路
            if(!vis[i]&&dis[i]<minD)
            {
                k = i;//记录下标
                minD = dis[i];//记录最小值
            }
        }
        if(k==e)    break;
        vis[k] = true;//标记已经访问过
        //松弛操作
        for(int i = 1; i< n; i++)
        {
            if(!vis[i]&&dis[k]+road[k][i]<dis[i])
            {
                dis[i]=dis[k]+road[k][i];
                pre[i]=k;
            }//if
        }//for
    }
}

void print(int cur){
    if(cur==1){
        printf("%d",rmp[cur]);
        return ;
    }
    print(pre[cur]);
    printf("->%d",rmp[cur]);
}

int main(){
    int start,end;
    n=1;
    scanf("%d%d",&start,&end);
    rmp.clear();
    mp.clear();

    mp[start]=n;
    rmp[n]=start;
    n++;

    mp[end]=n;
    rmp[n]=end;
    n++;

    int N=(end-start)/10;
    memset(road,INF,sizeof road);

    for(int i=0;i<=N;i++){
        int u,v,w;
        scanf("%d%d%d",&u,&v,&w);
        if(!mp[u]) {
            mp[u]=n;
            rmp[n]=u;
            n++;
        }
        if(!mp[v]) {
            mp[v]=n;
            rmp[n]=v;
            n++;
        }
        road[mp[u]][mp[v]]=road[mp[v]][mp[u]]=min(road[mp[v]][mp[u]],w);
    }
    for(int i=1;i<n;i++){
        for(int j=1;j<n;j++)
        printf("%d  ",road[i][j]==INF?-1:road[i][j]);
        printf("\n");
    }

    dijkstra(mp[start],mp[end]);
    if(dis[mp[end]]==INF)
        printf("-1");
    else
    {
        print(mp[end]);
        printf("(%d)\n",dis[mp[end]]);
    }
    return 0;
}
```





