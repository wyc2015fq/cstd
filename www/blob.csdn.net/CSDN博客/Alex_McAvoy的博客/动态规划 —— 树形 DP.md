# 动态规划 ——  树形 DP - Alex_McAvoy的博客 - CSDN博客





2018年10月06日 14:43:25[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：149








# 【概述】

树形动态规划是在树的数据结构上的动态规划，在各个阶段呈现树状关系的时候可以采用树形 DP，其基本思想是由子节点的信息推出父节点的信息。

树形 DP 中，是通过以下 4 点树的特点来进行建图的
-  n 个点，n-1 条边的无向图，任意两顶点间可达
- 无向图中任意两个点间有且只有一条路
- 一个点至多有一个前趋，但可以有多个后继
- 无向图中没有环

# 【计算顺序】

计算顺序与线性动态规划的顺推、逆推相似，同样有两个方向： 
- 叶 => 根：在回溯时，从叶节点向上转移状态
- 根 => 叶：在叶向根 DFS 一遍后（预处理），再重新向下获取答案

# 【表示方法】

树形 DP 还涉及到建图的问题，如果题目能很清晰的输入一个树，一般使用 vector<int> s[N]，用 s[i] 来保存节点 i 的所有儿子。

当树是一般树且涉及到权值时，一般采用链式存储法，即用结构体数组 edge 存边，edge[i] 表示第 i 条边，head[i] 存以 i 为起点的第一条边（在edge中的下标）

```cpp
struct Node{
    int next; //下一条边的存储下标
    int to; //这条边的终点
    int w; //权值
}edge[N*2]; //由于是无向图，因此要开2倍
```

常用的建图方法如下

```cpp
for(int i=1;i<=n;i++){
    int x,y;
    scanf("%d%d",&x,&y);
    addEdge(x,i);
    w[i]=y;
}
```

若以点 i 为起点的边新增了一条，则在 edge 中的下标为 j，那么 edge[j].next=head[i]，然后head[i]=j，即每次新加的边作为第一条边，最后倒序遍历即可。

```cpp
int cnt;//边的计数
void Add(int x,int y,int w) {    //起点x, 终点y, 权值w
    edge[cnt].x=x;
    edge[cnt].w=w;
    edge[cnt].next=head[x];
    head[y]=cnt++;
}
```

遍历以 x 为起点的边时，以 i 开始为第一条边，每次指向下一条(以-1为结束标志)

```cpp
memset(head,-1,sizeof(head));
for(int i=head[st]; i!=-1; i=edge[i].next){
    ...
}
```

# 【计算方法】

树形 DP 的计算方法则与线性 DP 不同，线性 DP 一般采用传统迭代，而树是通过递归定义的，因此树形 DP 要递归来求解。

一般而言，树形 DP 常与背包问题结合起来，常用 dp[node] 表示的是以 node 为根的子树能得到的最优解，dp[node] 需要从 node 的子结点进行状态转移，由于 dp[node] 的状态是由它的儿子转移而来，因此可以 node 的 n 个儿子看做 n 个物品，对这 n 个物品抉择得到最优的 dp[node] 就用到背包的思想。

当然，树形 DP 不止 node 这一维，第二维一般都是以题目给出的限制需要来，然后根据题意保存好状态，写出状态转移方程，递归的求解 DP[root]

# 【基本步骤】

1.若问题是一棵隐性树（不以树为直接背景，但各个阶段呈树状关系），则需要将问题转化为一棵显性树，并存储各阶段的树状联系。

2.根据题目要求与数据量，选择合适的树的存储方式。

若节点数小于5000，一般选用邻接矩阵存储；若节点数大于5000，一般选用邻接表来存储（边要开到2*N，因为是无向图）；若是二叉树或需要多叉转二叉，则可以用两个一维数组 brother[N]、child[N] 来存储。

3.写出动归方程

通过孩子和父亲之间的关系建立方程，根据要求选用 根 => 叶 或 叶 => 根 的计算方式。

# 【题目类型】

## 1.常规问题
- 没有上司的舞会（洛谷-P1352）**(经典题，入门必做)**：[点击这里](https://blog.csdn.net/u011815404/article/details/82952012)

	英文版：Anniversary party（HDU-1520）：[点击这里](https://blog.csdn.net/u011815404/article/details/82952830)
- Computer（HDU-2196）**(经典题，入门必做)**：[点击这里](https://blog.csdn.net/u011815404/article/details/82960087)

## 2.树形背包问题

求在树上选一些点满足价值最大的问题，一般可设 dp[i][j] 表示 i 这颗子树选 j 个点的最优解。
- 有线电视网（洛谷-P1273）**(分组背包)**：[点击这里](https://blog.csdn.net/u011815404/article/details/82953081)

	英文版：TELE（POJ-1115）：[点击这里](https://blog.csdn.net/u011815404/article/details/82964205)
- Starship Troopers（HDU-1011）**(分组背包，注意特判m=0的情况)**：[点击这里](https://blog.csdn.net/u011815404/article/details/82987797)
- Rebuilding Roads（POJ-1947）**(分组背包)**：[点击这里](https://blog.csdn.net/u011815404/article/details/82988294)
- The more, The Better（HDU-1561）**(01背包+虚拟根节点)**：[点击这里](https://blog.csdn.net/u011815404/article/details/86614735)

## **3.删点或删边问题**

统计子树和，通过加减一些子树满足题目中要求的某些性质。
- Garland（CF-767C）**(点权固定，注意特判)**：[点击这里](https://blog.csdn.net/u011815404/article/details/82952158)
- 最大子树和（洛谷-P1122）**(根据具体权值大小来判断)**：[点击这里](https://blog.csdn.net/u011815404/article/details/82953145)
- 重建道路（洛谷-P1272）**(删边+01背包)**：[点击这里](https://blog.csdn.net/u011815404/article/details/82952738)



