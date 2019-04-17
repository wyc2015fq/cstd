# 图论 —— Floyd 算法 - Alex_McAvoy的博客 - CSDN博客





2019年03月18日 19:47:46[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：75
个人分类：[图论——Floyd																[——————图论——————](https://blog.csdn.net/u011815404/article/category/7664328)](https://blog.csdn.net/u011815404/article/category/8312424)








# 【概述】 

Floyd 算法又称为插点法，是一种用于寻找给定的加权图中多源点之间最短路径的算法。

其最大特点是可以计算出现负边权时的最短路，实际应用中，很多题目不是问如何用 Floyd 求最短路，而且用 Floyd 的动态规划思想来解决类似 Floyd 的问题。

其时间复杂度是 O(N*N*N)，N是顶点数。

# 【极大值的选择】

设置无穷大时，0x7fffffff 是 32-bit int 的最大值，如果这个无穷大只用于一般的比较，那么 0x7fffffff 是一个完美的选择，但在更多情况下，其并不是一个好的选择。

在最短路的松弛操作时，如果 u、v 间无边，那么 w[u][v]=INF，此时若 INF 取 0x7fffffff，那么 dis[u]+w[u][v] 会溢出而变成负数，此时松弛操作便会出错，准确来说，0x7fffffff 不能满足无穷大加一个有穷的数依然是无穷大，而是变成了一个很小的负数。

```cpp
if(dis[u]+w[u][v]<dis[v]) 
    dis[v]=dis[u]+w[u][v];
```

由于要找一个能够满足无穷大加无穷大依然是无穷大的数，因此，可以选用 0x3f3f3f3f

0x3f3f3f3f 的十进制是 1061109567，是 10^9 级别的，与 0x7fffffff 一个数量级，而一般场合下的数据都是小于10^9的，所以它可以作为无穷大使用而不致出现数据大于无穷大的情形。 

另一方面，由于一般的数据都不会大于 10^9，所以当我们把无穷大加上一个数据时，它并不会溢出，事实上 0x3f3f3f3f + 0x3f3f3f3f = 2122219134，这个数虽然非常大但却没有超过 32-bit int 的表示范围，因此 0x3f3f3f3f 还满足了无穷大加无穷大还是无穷大的需求。

此外，当想将某个数组清零或全部赋值为 -1，通常会使用 memset() 函数，但是当想将某个数组全部赋值为无穷大时，就不能使用memset 函数而是写循环了，因为 memset 是按字节操作的，它能够对数组清零是因为 0 的每个字节都是 0。但如果将无穷大设为 0x3f3f3f3f，由于其每个字节都是 0x3f，因此可以直接使用 memset() 函数来操作。

# 【求最短路】

1.初始化：

设 dis[i][j] 为 i、j 两点的距离，w[i][j] 为 i、j 两点的权值。

若点 u、v 有边连接，则：dis[u][v]=w[u][v]，即：初始化两点最短距离为两点权值。

若点 u、v 无边连接，则：dis[u][v]=0x3f3f3f3f，即：初始化为一极大值。

2.算法主体

```cpp
for(int k=1;k<=n;k++)//第一重循环为i→j的中间点k
    for(int i=1;i<=n;i++)//第二重循环为起点i
        for(int j=1;j<=n;j++)//第三重循环为终点j
            if(dis[i][j]>dis[i][k]+dis[k][j])//如果i→k的距离加上k→j的距离小于i→j的距离
                dis[i][j]=dis[i][k]+dis[k][j];//更新最短路径
```

3.算法结束：dis[i][j] 即为 i→j 的最短路径。

# 【判断图的连通性】

Floyd 可以用来判断图中两点是否连通。

对于一个没有边权的图，可将相邻两点距离设为 dis[i][j]=true，不相邻的两点距离设为 dis[i][j]=false，而后进行 Floyd 算法即可。 

此外，可以在求连通性的同时，进行传递闭包计算：对于一个节点 i，如果 j 能到 i，i 能到 k，那么 j 就能到 k，求传递闭包，就是把图中所有满足这样传递性的节点计算出来，计算完成后，就知道任意两个节点之间是否相连。 

```cpp
for(int k=1;k<=n;k++)//第一重循环为i→j的中间点k
    for(int i=1;i<=n;i++)//第二重循环为起点i
        for(int j=1;j<=n;j++)//第三重循环为终点j
            if(dis[i][j]>dis[i][k]+dis[k][j])//如果i→k的距离加上k→j的距离小于i→j的距离
                if(dis[i][k]&&dis[k][j])//更新最短路径
                    dis[i][j]=true;
```

# 【求最小环】

Floyd 算法还可以用来解决最小环问题，所谓最小环问题，最小环就是指在一张图中找出一个环，使得这个环上的各条边的权值之和最小。

记两点间的最短路为 dis[i][j]，w[i][j] 为边 < i,j > 的权值，res 为图的最小环

一个环中最大的节点为 k，与它相连的节点为 i、j，这个环的最短长度为 w[i][k]+w[k][j]+(i 到 j 的路径中所有节点编号都小于 k 的最短路径长度)

根据 Floyed 原理，在最外层进行 k-1 次循环之后 dis[i][j] 代表了 i 到 j 的路径中，所有结点编号都小于 k 的最短路径，因此该算法一定能找到图中的最小环。

```cpp
int res=INF;
for(int k=1;k<=n;k++){//第一重循环为i→j的中间点k

    for(int i=1;i<=n;i++)//第二重循环为起点i
        for(int j=1;j<=n;j++)//第三重循环为终点j
            res=min(res,dis[i][j]+w[j][k]+w[k][i]);//环的最短长度
    
    for(int i=1;i<=n;i++)//第二重循环为起点i
        for(int j=1;j<=n;j++)//第三重循环为终点j
            dis[i][j]=min(dis[i][j],w[i][k]+w[k][j]);//最短路径
}
```

# 【例题】
- Bus System（HDU-1690）**(Floyd)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80399978)
- 最短路（HDU-2544）**(Floyd)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80397057)
- Checking an Alibi（POJ-2394）**(Floyd)**：[点击这里](https://blog.csdn.net/u011815404/article/details/81254268)
- 六度分离（HDU-1869）**(Floyd)**：[点击这里](https://blog.csdn.net/u011815404/article/details/84073787)
- 信使（信息学奥赛一本通-T1376）**(Floyd)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80752631)
- 最短路径（信息学奥赛一本通-T1378）**(Floyd)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80796182)
- 最短路径问题（信息学奥赛一本通-T1342）**(边的权值预处理+Floyd)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80752636)
- 牛的旅行（信息学奥赛一本通-T1343）**(边的权值预处理+Floyd)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80752635)
- 最优乘车（信息学奥赛一本通-T1377）**(输出流的应用+Floyd)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80796185)
- 刻录光盘（信息学奥赛一本通-T1383）**(Floyd 判断连通性)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80722226)
- Find the safest road（HDU-1596）**(Floyd 变形求最长路)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80404682)
- Shortest Path（HDU-5636）**(有技巧的 Floyd)**：[点击这里](https://blog.csdn.net/u011815404/article/details/86552967)
- Cow Contest（POJ-3660 ）**(传递闭包)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80501334)
- Ranking the Cows（POJ-3275 ）**(传递闭包)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80787203)
- 珍珠（信息学奥赛一本通-T1384）**(传递闭包)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80722225)
- Dima and Bacteria（CF-400D）**(传递闭包+并查集)**：[点击这里](https://blog.csdn.net/u011815404/article/details/86519792)
- Cow Hurdles（POJ-3615 ）**(最大值中的最小值)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80597985)




