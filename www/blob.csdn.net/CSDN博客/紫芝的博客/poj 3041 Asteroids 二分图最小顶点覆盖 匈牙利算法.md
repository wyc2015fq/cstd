# poj 3041 Asteroids 二分图最小顶点覆盖 匈牙利算法 - 紫芝的博客 - CSDN博客





2018年08月28日 11:30:40[紫芝](https://me.csdn.net/qq_40507857)阅读数：31








# [Asteroids](http://poj.org/problem?id=3041)
|**Time Limit:** 1000MS||**Memory Limit:** 65536K|
|----|----|----|
|**Total Submissions:** 26508||**Accepted:** 14321|

Description

Bessie wants to navigate her spaceship through a dangerous asteroid field in the shape of an N x N grid (1 <= N <= 500). The grid contains K asteroids (1 <= K <= 10,000), which are conveniently located at the lattice points of the grid. 


Fortunately, Bessie has a powerful weapon that can vaporize all the asteroids in any given row or column of the grid with a single shot.This weapon is quite expensive, so she wishes to use it sparingly.Given the location of all the asteroids in the field, find the minimum number of shots Bessie needs to fire to eliminate all of the asteroids.

Input

* Line 1: Two integers N and K, separated by a single space. 

* Lines 2..K+1: Each line contains two space-separated integers R and C (1 <= R, C <= N) denoting the row and column coordinates of an asteroid, respectively.

Output

* Line 1: The integer representing the minimum number of times Bessie must shoot.

Sample Input

3 4
1 1
1 3
2 2
3 2

Sample Output

2

Hint

INPUT DETAILS: 

The following diagram represents the data, where "X" is an asteroid and "." is empty space: 

X.X 

.X. 

.X. 


OUTPUT DETAILS: 

Bessie may fire across row 1 to destroy the asteroids at (1,1) and (1,3), and then she may fire down column 2 to destroy the asteroids at (2,2) and (3,2).

Source

[USACO 2005 November Gold](http://poj.org/searchproblem?field=source&key=USACO+2005+November+Gold)

### 题意：

在n*n的平面内，分布着一些小行星，打一炮之后，一行或一列的行星就消失了，打完所有的行星需要几次？

### 分析：

以行号x作为n1点集，列号y作为n2点集，若位置（x，y）处有行星，则x->y有边，建立二分图。 

则问题可转化为，求最少的点覆盖所有的边。

根据公式1： 二分图最小顶点覆盖 = 二分图最大匹配

```cpp
#include<cstdio>
#include<cstring>
using namespace std;
const int N=1001;
int n1,n2,k;
//n1,n2为二分图的顶点集，其中x∈n1,y∈n2
int map[N][N],vis[N],link[N];
//link记录n2中的点y在n1中所匹配的x点的编号
int find(int x)
{
    int i;
    for(i=1;i<=n2;i++)
    {
        //x->i有边，且节点i未被搜索
        if(map[x][i]&&!vis[i])
        {
            vis[i]=1;//标记节点已被搜索

            //如果i不属于前一个匹配M或者i匹配到的节点可以寻找到增广路
            if(link[i]==0||find(link[i])){
                link[i]=x;//更新
                return 1;//匹配成功
            }
        }
    }
    return 0;
}
int main()
{
    scanf("%d%d",&n1,&k);
    n2=n1;
    for(int i=0;i<k;i++)
    {
        int x,y;
        scanf("%d%d",&x,&y);
        map[x][y]=1;
    }
    int s=0;
    for(int i=1;i<=n1;i++)
    {
        memset(vis,0,sizeof(vis));
        if(find(i)) s++;
    }
    printf("%d\n",s);
    return 0;
}
```





