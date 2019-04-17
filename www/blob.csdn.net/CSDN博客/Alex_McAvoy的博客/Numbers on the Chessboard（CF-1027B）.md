# Numbers on the Chessboard（CF-1027B） - Alex_McAvoy的博客 - CSDN博客





2018年08月24日 11:05:55[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：64
个人分类：[CodeForces																[基础算法——递推](https://blog.csdn.net/u011815404/article/category/7615003)](https://blog.csdn.net/u011815404/article/category/7923180)








# Problem Description

You are given a chessboard of size n×n. It is filled with numbers from 1 to n^2 in the following way: the first ⌈n^2/2⌉ numbers from 1 to ⌈n^2/2⌉ are written in the cells with even sum of coordinates from left to right from top to bottom. The rest n^2−⌈n^2/2⌉ numbers from ⌈n^2/2⌉+1 to n^2 are written in the cells with odd sum of coordinates from left to right from top to bottom. The operation ⌈x/y⌉ means division x by y rounded up.

For example, the left board on the following picture is the chessboard which is given for n=4 and the right board is the chessboard which is given for n=5n=5.

![](https://img-blog.csdn.net/20180824104537998?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

You are given q queries. The i-th query is described as a pair xi,yixi,yi. The answer to the i-th query is the number written in the cellxi,yi (xi is the row, yi is the column). Rows and columns are numbered from 1 to n.

# Input

The first line contains two integers n and q (1≤n≤109, 1≤q≤105) — the size of the board and the number of queries.

The next q lines contain two integers each. The i-th line contains two integers xi,yi (1≤xi,yi≤n) — description of the i-th query.

# Output

For each query from 1 to q print the answer to this query. The answer to the i-th query is the number written in the cell xi,yi (xixi is the row, yi is the column). Rows and columns are numbered from 1 to n. Queries are numbered from 1 to q in order of the input.

# Examples

**Input**

4 5

1 1

4 4

4 3

3 2

2 4

**Output**

1

8

16

13

4

**Input**

5 4

2 1

4 2

3 3

3 4

**Output**

16

9

7

20

————————————————————————————————————————————

题意：以上图的方式填充一个 n*n 大小的格子，查找询问位置点的数字大小

思路：通过存数过程可以知道，第一轮放的横纵坐标和为偶数，第二轮放的坐标和为奇数，根据规律可以知道第一轮放的数满足(x-1)*n+y+1 ，则第二轮存放的数量需要加上 n*n，要注意的是，由于是以一、二轮讨论存放情况的，因此每一轮的每一行只有 n/2 个数，所以最后结果需要除以2

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<string>
#include<cstdlib>
#include<queue>
#include<set>
#include<map>
#include<stack>
#include<ctime>
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 1001
#define MOD 1e9+7
#define E 1e-6
#define LL long long
using namespace std;
int main()
{
    LL n,t;
    cin>>n>>t;
    while(t--)
    {
        LL x,y;
        cin>>x>>y;

        LL ans=(x-1)*n+y+1;
        if((x+y)%2)
            ans+=n*n;
        cout<<ans/2<<endl;
    }

    return 0;
}
```






