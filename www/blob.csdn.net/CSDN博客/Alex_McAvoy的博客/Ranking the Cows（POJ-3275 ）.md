# Ranking the Cows（POJ-3275 ） - Alex_McAvoy的博客 - CSDN博客





2018年06月23日 22:17:41[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：69
个人分类：[POJ																[图论——Floyd](https://blog.csdn.net/u011815404/article/category/8312424)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

Each of Farmer John's N cows (1 ≤ N ≤ 1,000) produces milk at a different positive rate, and FJ would like to order his cows according to these rates from the fastest milk producer to the slowest.

FJ has already compared the milk output rate for M (1 ≤ M ≤ 10,000) pairs of cows. He wants to make a list of C additional pairs of cows such that, if he now compares those C pairs, he will definitely be able to deduce the correct ordering of all N cows. Please help him determine the minimum value of C for which such a list is possible.

# **Input**

Line 1: Two space-separated integers: N and M 

Lines 2..M+1: Two space-separated integers, respectively: X and Y. Both X and Y are in the range 1...N and describe a comparison where cow X was ranked higher than cow Y.

# Output

Line 1: A single integer that is the minimum value of C.


# Sample Input

**5 5**

**2 11 52 31 43 4**

# Sample Output

**3**


题意：已知有n头牛，给出m对产量关系 x>y，求至少还要多少对产量关系才能给出所有牛的产量关系。

思路：n 头牛一共有 n*(n-1)/2 对关系，现在给出 m 对关系，假设根据这 m 对关系无法确定的关系有 sum 对，则题目也就是求 (sum-n)/2

枚举中间节点 K 之后就开始枚举起点 x 和终点 y，若 x 与 k，或 y 与 k 之间不联通，则无法确立关系，因此从起点 x 只检查能通向 k 的节点，从终点 y 只检查 k 能通向的节点，使用 Floyd 算法进行传递闭包运算，最后根据无法确定关系的牛的对数输出结果即可。

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
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 1001
#define MOD 123
#define E 1e-6
using namespace std;
int f[N][N];
int main ()
{
    int n,m;
    while(scanf("%d%d",&n,&m)!=EOF)
    {
        for(int i=1;i<=m;i++)
        {
            int x,y;
            scanf("%d%d",&x,&y);
            f[x][y]=1;
        }

        for(int k=1;k<=n;k++)
            for(int i=1;i<=n;i++)
                for(int j=1;j<=n;j++)
                    if(f[i][k]&f[k][j])
                        f[i][j]=1;
        
        for(int i=1;i<=n;i++)//一旦具自反性，输出-1
            if(f[i][i])
            {
                printf("-1");
                return 0;
            }

        int sum=0;
        for(int i=1;i<=n;i++)
            for(int j=1;j<=n;j++)
                if(!f[i][j]&&!f[j][i])//一旦存在未知
                    sum++;
        printf("%d",(sum-n)/2);
    }
    return 0;
}
```






