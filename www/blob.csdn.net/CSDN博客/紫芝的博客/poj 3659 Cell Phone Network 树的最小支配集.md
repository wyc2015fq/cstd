# poj 3659 Cell Phone Network 树的最小支配集 - 紫芝的博客 - CSDN博客





2018年07月29日 11:26:01[紫芝](https://me.csdn.net/qq_40507857)阅读数：31
个人分类：[动态规划																[贪心](https://blog.csdn.net/qq_40507857/article/category/7568514)](https://blog.csdn.net/qq_40507857/article/category/7565832)








# Cell Phone Network
|**Time Limit:** 1000MS||**Memory Limit:** 65536K|
|----|----|----|
|**Total Submissions:** 7207||**Accepted:** 2573|

Description

Farmer John has decided to give each of his cows a cell phone in hopes to encourage their social interaction. This, however, requires him to set up cell phone towers on his *N* (1 ≤ *N* ≤ 10,000) pastures (conveniently numbered 1..*N*) so they can all communicate.

Exactly *N*-1 pairs of pastures are adjacent, and for any two pastures *A* and *B* (1 ≤ *A* ≤ *N*; 1 ≤ *B* ≤ *N*; *A* ≠ *B*) there is a sequence of adjacent pastures such that *A *is the first pasture in the sequence and *B* is the last. Farmer John can only place cell phone towers in the pastures, and each tower has enough range to provide service to the pasture it is on and all pastures adjacent to the pasture with the cell tower.

Help him determine the minimum number of towers he must install to provide cell phone service to each pasture.

Input

* Line 1: A single integer: *N*

* Lines 2..*N*: Each line specifies a pair of adjacent pastures with two space-separated integers: *A* and *B*

Output

* Line 1: A single integer indicating the minimum number of towers to install

Sample Input

```
5
1 3
5 2
4 3
3 5
```

Sample Output

```
2
```

Source

[USACO 2008 January Gold](http://poj.org/searchproblem?field=source&key=USACO+2008+January+Gold)

### 题意：

John想让他的所有牛用上手机以便相互交流，他需要建立几座信号塔在N块草地中。已知与信号塔相邻的草地能收到信号。

给你N-1个草地(A，B)的相邻关系，问：最少需要建多少个信号塔能实现所有草地都有信号

### 分析：

解法1

**第一步：确定状态**

①dp[i][0]：选点i，并且以点i为根的子树都被覆盖了。

②dp[i][1]：不选点i，i被其儿子覆盖

③dp[i][2]：不选点i，i没有被子节点覆盖（被其父亲覆盖）

**第二步：确定状态转移方程**

dp[i][0]=1+Σmin(dp[u][0],dp[u][1],dp[u][2])（u是i的儿子）

dp[i][2]=Σ(dp[u][1])

对于dp[i][1]的讨论稍微复杂一点——他的所有儿子里面必须有一个取dp[u][1]

那么：if(i没有子节点)dp[i][1]=INF

else dp[i][1]=Σmin(dp[u][0],dp[u][1])+inc

其中对于inc有：

if(上面式子中的Σmin(dp[u][0],dp[u][1])中包含某个dp[u][0])inc=0;

else inc=min(dp[u][0]-dp[u][1])。

代码：

待写

贪心做法

```cpp
#include<cstdio>
#include<iostream>
#include<vector>
#include<cstring>
using namespace std;
const int maxn = 1e4+5;
vector<int> G[maxn];
int ans;
bool vis[maxn];//标记节点是否访问过
bool ok[maxn];
int n;
void dfs(int x,int father){
  bool flag=0;
  vis[x]=1;
  for(int i=0;i<G[x].size();i++)
  {
      int son=G[x][i];
      if(!vis[son]){
        dfs(son,x);
        if(ok[son]) flag=1;//这个点的孩子被覆盖
      }
  }
//如果它的孩子没有被覆盖，他和他的父亲都没有被覆盖，就覆盖他的父亲，答案+1。
  if(father==0){
    if(!flag&&!ok[x]){
        ans++;ok[x]=1;
    }
  }
  else if(!ok[father]&&!ok[x]&&!flag)
  {
        ok[father]=1;ans++;
  }
}
void init()
{
    memset(vis,0,sizeof(vis));
    memset(ok,0,sizeof(ok));
     for(int i = 0;i <= n; i++)
      G[i].clear();
}
int main()
{
  while(scanf("%d",&n) != EOF){
    for(int i = 0;i < n - 1; i++){
        int a,b;
      scanf("%d %d",&a,&b);
      G[a].push_back(b);
      G[b].push_back(a);
    }
    ans=0;
    dfs(1,0);//从任意一点开始搜索
    printf("%d\n",ans);
  }
  return 0;
}
```






