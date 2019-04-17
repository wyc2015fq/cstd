# 生成树的计数 Matrix-Tree（矩阵树）定理 - 在思索中前行！ - CSDN博客





2017年03月14日 16:41:22[_Tham](https://me.csdn.net/txl16211)阅读数：1357








信息学竞赛中，有关生成树的最优化问题如最小生成树等是我们经常遇到的，而对生成树的计数及其相关问题则少有涉及。事实上，生成树的计数是十分有意义的，在许多方面都有着广泛的应用。本文从一道信息学竞赛中出现的例题谈起，首先介绍了一种指数级的动态规划[算法](http://lib.csdn.net/base/datastructure)，然后介绍了行列式的基本概念、性质，并在此基础上引入Matrix-Tree定理，同时通过与一道数学问题的对比，揭示了该定理所包含的数学思想。最后通过几道例题介绍了生成树的计数在信息学竞赛中的应用，并进行总结。




生成树的计数 Matrix-Tree定理




问题的提出



[例一]高速公路（SPOJ 104 Highways）



一个有n座城市的组成国家，城市1至n编号，其中一些城市之间可以修建高速公路。现在，需要有选择的修建一些高速公路，从而组成一个交通网络。你的任务是计算有多少种方案，使得任意两座城市之间恰好只有一条路径？

数据规模：1≤n≤12。

[分析]



我们可以将问题转化到成图论模型。因为任意两点之间恰好只有一条路径，所以我们知道最后得到的是原图的一颗生成树。因此，我们的问题就变成了，给定一个无向图G，求它生成树的个数t(G)。这应该怎么做呢？

经过分析，我们可以得到一个时间复杂度为O(3n*n2)的动态规划算法，因为原题的规模较小，可以满足要求。但是，当n再大一些就不行了，有没有更优秀的算法呢？答案是肯定的。在介绍算法之前，首先让我们来学习一些基本的预备知识。




新的方法介绍

       下面我们介绍一种新的方法——Matrix-Tree定理(Kirchhoff矩阵-树定理)。Matrix-Tree定理是解决生成树计数问题最有力的武器之一。它首先于1847年被Kirchhoff证明。在介绍定理之前，我们首先明确几个概念：

1、G的度数矩阵D[G]是一个n*n的矩阵，并且满足：当i≠j时,dij=0；当i=j时，dij等于vi的度数。

2、G的邻接矩阵A[G]也是一个n*n的矩阵， 并且满足：如果vi、vj之间有边直接相连，则aij=1，否则为0。

我们定义G的Kirchhoff矩阵(也称为拉普拉斯算子)C[G]为C[G]=D[G]-A[G]，则Matrix-Tree定理可以描述为：G的所有不同的生成树的个数等于其Kirchhoff矩阵C[G]任何一个n-1阶主子式的行列式的绝对值。所谓n-1阶主子式，就是对于r(1≤r≤n)，将C[G]的第r行、第r列同时去掉后得到的新矩阵，用Cr[G]表示。



生成树计数

[算法](http://lib.csdn.net/base/datastructure)步骤：

1、 构建拉普拉斯矩阵

     Matrix[i][j] =

degree(i) , i==j

-1，i-j有边

 0，其他情况

2、 去掉第r行，第r列（r任意）

3、 计算矩阵的行列式

论文 周冬 《生成树计数应用》



```cpp
#include <map>  
#include <cstdio>  
#include <cstring>  
#include <iostream>  
#include <algorithm>  
using namespace std;  
typedef long long ll;  
const int maxn = 105;  
const int maxm = 100005;  
const int INF = 1e9;  
int degree[maxn];  
ll g[maxn][maxn];  
int n, m;  
  
ll det(ll a[][maxn], int n)  
{  
    ll ret = 1;  
    for(int i=1; i<n; ++i){  
        for(int j=i+1; j<n; ++j){  
            while(a[j][i]){  
                ll t = a[i][i]/a[j][i];  
                for(int k=i; k<n; ++k){  
                    a[i][k] = (a[i][k]-a[j][k]*t);  
                }  
                for(int k=i; k<n; ++k){  
                    swap(a[i][k], a[j][k]);  
                }  
                ret = -ret;  
            }  
        }  
        if(a[i][i]==0){  
            return 0;  
        }  
        ret = ret*a[i][i];  
    }  
    if(ret<0){  
        ret = -ret;  
    }  
    return ret;  
}  
  
void solve()  
{  
    int u, v;  
    memset(degree, 0, sizeof degree );  
    memset(g, 0, sizeof g );  
    scanf("%d%d", &n, &m);  
    while(m--){  
        scanf("%d%d", &u, &v);  
        u--,v--;  
        g[u][v] = g[v][u] = -1;  
        degree[u]++;  
        degree[v]++;  
    }  
    for(int i=0; i<n; ++i){  
        g[i][i] = degree[i];  
    }  
    printf("%lld\n", det(g, n));  
}  
  
int main()  
{  
    int t;  
    scanf("%d", &t);  
    while(t--){  
        solve();  
    }  
    return 0;  
}
```








