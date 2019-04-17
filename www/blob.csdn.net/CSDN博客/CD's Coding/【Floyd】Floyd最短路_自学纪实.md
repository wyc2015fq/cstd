# 【Floyd】Floyd最短路_自学纪实 - CD's Coding - CSDN博客





2014年06月16日 11:54:34[糖果天王](https://me.csdn.net/okcd00)阅读数：527标签：[codeforces																[动态规划																[Floyd](https://so.csdn.net/so/search/s.do?q=Floyd&t=blog)
个人分类：[算法记忆](https://blog.csdn.net/okcd00/article/category/1650753)





最近在复习数据结构备考，然后发现Floyd这个东西，想到常常听人说这个算法，一直没把他跟那个解梦的分开……

今天来好好看看的好——









## 先问问度娘：

### 思路:

通过一个图的权值矩阵求出它的每两点间的最短路径矩阵。

从图的带权邻接矩阵A=[a(i,j)] n×n开始，递归地进行n次更新，即由矩阵D(0)=A，按一个公式，构造出矩阵D(1)；又用同样地公式由D(1)构造出D(2)；……；最后又用同样的公式由D(n-1)构造出矩阵D(n)。矩阵D(n)的i行j列元素便是i号顶点到j号顶点的最短路径长度，称D(n)为图的距离矩阵，同时还可引入一个后继节点矩阵path来记录两点间的最短路径。

采用的是(松弛技术)，对在i和j之间的所有其他点进行一次松弛。所以时间复杂度为O(n^3);

其状态转移方程如下： map[i,j]:=min{map[i,k]+map[k,j],map[i,j]}

map[i,j]表示i到j的最短距离，K是穷举i,j的断点，map[n,n]初值应该为0，或者按照题目意思来做。

当然，如果这条路没有通的话，还必须特殊处理，比如没有map[i,k]这条路

### 算法流程：

1，从任意一条单边路径开始。所有两点之间的距离是边的权，如果两点之间没有边相连，则权为无穷大。

2，对于每一对顶点 u 和 v，看看是否存在一个顶点 w 使得从 u 到 w 再到 v 比已知的路径更短。如果是更新它。

把图用邻接矩阵G表示出来，如果从Vi到Vj有路可达，则G[i,j]=d，d表示该路的长度；否则G[i,j]=无穷大。定义一个矩阵D用来记录所插入点的信息，D[i,j]表示从Vi到Vj需要经过的点，初始化D[i,j]=j。把各个顶点插入图中，比较插点后的距离与原来的距离，G[i,j] = min( G[i,j], G[i,k]+G[k,j] )，如果G[i,j]的值变小，则D[i,j]=k。在G中包含有两点之间最短道路的信息，而在D中则包含了最短通路径的信息。

比如，要寻找从V5到V1的路径。根据D，假如D(5,1)=3则说明从V5到V1经过V3，路径为{V5,V3,V1}，如果D(5,3)=3，说明V5与V3直接相连，如果D(3,1)=1，说明V3与V1直接相连。

### 算法描述:

a)　初始化：D[u,v]=A[u,v]

b)　For k:=1 to n

For i:=1 to n

For j:=1 to n

If D[i,j]>D[i,k]+D[k,j] Then

D[I,j]:=D[I,k]+D[k,j];

c)　算法结束：D即为所有点对的最短路径矩阵





## Wiki娘：

Floyd-Warshall算法的原理是[动态规划](http://zh.wikipedia.org/wiki/%E5%8A%A8%E6%80%81%E8%A7%84%E5%88%92)。




## Code：

大概有那么一点点理解了……

扔一道解题报告备看：

[CF #182 Div.2 Problem D](http://codeforces.com/contest/302/problem/D)










Yaroslav is playing a game called "Time". The game has a timer showing the lifespan he's got left. As soon as the timer shows 0, Yaroslav's character dies and the game ends. Also, the game has *n* clock
 stations, station number *i* is at point (*x**i*, *y**i*) of
 the plane. As the player visits station number *i*, he increases the current time on his timer by *a**i*.
 The stations are for one-time use only, so if the player visits some station another time, the time on his timer won't grow.


A player spends *d*·*dist* time units to move between stations, where *dist* is
 the distance the player has covered and *d* is some constant. The distance between stations *i* and *j* is
 determined as |*x**i* - *x**j*| + |*y**i* - *y**j*|.


Initially, the player is at station number 1, and the player has strictly more than zero and strictly less than one units of time. At station number 1 one
 unit of money can increase the time on the timer by one time unit (you can buy only integer number of time units).


Now Yaroslav is wondering, how much money he needs to get to station *n*. Help Yaroslav. Consider the time to buy and to increase the timer value negligibly
 small.




Input


The first line contains integers *n* and *d*(3 ≤ *n* ≤ 100, 103 ≤ *d* ≤ 105) —
 the number of stations and the constant from the statement.


The second line contains *n* - 2 integers: *a*2, *a*3, ..., *a**n* - 1(1 ≤ *a**i* ≤ 103).
 The next *n* lines contain the coordinates of the stations. The *i*-th
 of them contains two integers *x**i*, *y**i*(-100 ≤ *x**i*, *y**i* ≤ 100).


It is guaranteed that no two stations are located at the same point.




Output


In a single line print an integer — the answer to the problem.




Sample test(s)




input
3 1000
1000
0 0
0 1
0 3




output
2000




input
3 1000
1000
1 0
1 1
1 2




output
1000











From:[magicnumber](http://my.csdn.net/magicnumber) @CSDN






万恶的图论题。


主要问题是在考虑每经过一个点需要增加一个权值，但是可以注意数据范围，权值a_i<=d，所以a_i<=d*dist。


A->B的权值为在A点得到的权值减去d*dist(A,B)，根据上面讨论的结果，这个权值一定大于等于0，因此，这个图不存在负权值。


所以直接Floyd解决。



```cpp
#include <stdio.h>
#include <string.h>
#include <algorithm>
using namespace std;

int a[105];
int x[105];
int y[105];
int map[105][105];

int Dist(int p,int q)
{
    return abs(x[p]-x[q])+abs(y[p]-y[q]);
}

int main()
{
    int i,j,n,d,k;
    scanf("%d%d",&n,&d);
    for (i=1;i<n-1;i++)
    {
        scanf("%d",&a[i]);
    }
    a[0]=a[n-1]=0;
    for (i=0;i<n;i++)
    {
        scanf("%d%d",&x[i],&y[i]);
    }
    for (i=0;i<n;i++)
    {
        for (j=0;j<n;j++)
        {
            if (i==j) map[i][j]=0;
            else map[i][j]=Dist(i,j)*d-a[j];
         //  printf("%d ",map[i][j]);
        }
       // printf("\n");
    }
    for (k=0;k<n;k++)
    {
        for (i=0;i<n;i++)
        {
            for (j=0;j<n;j++)
            {
                map[i][j]=min(map[i][j],map[i][k]+map[k][j]);
            }
        }
    }
    printf("%d",map[n-1][0]);
    return 0;
}
```](https://so.csdn.net/so/search/s.do?q=动态规划&t=blog)](https://so.csdn.net/so/search/s.do?q=codeforces&t=blog)




