# Arbiter（HDU-3118） - Alex_McAvoy的博客 - CSDN博客





2019年03月13日 20:45:55[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：99
个人分类：[HDU																[图论——二分图](https://blog.csdn.net/u011815404/article/category/8391132)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

Arbiter is a kind of starship in the StarCraft science-fiction series. The Arbiter-class starship is a Protoss warship specializing in providing psychic support. Arbiters were crewed exclusively by Judicators; unlike other warships that were manned predominantly by Templar. The Judicator used the Arbiter as a base to provide support using space-time manipulation. 

Arbiters could weaken space-time, tearing rifts in the fabric of space-time, creating a vortex linking another location to the Arbiter’s location. This could be used to move personnel over long distances between stars. 

In the meantime of widely used Arbiter to transfer, KMXS, the captain of one Arbiter, was warning that some person had got a serious mental disorder after the trip on his Arbiter. By using mice as model animals, he found the sake, it’s because of chirality! 

Every person has chirality, either left-handed or right-handed. Actually all the persons must live with the food which has the same chirality. When one person took Arbiter from one star to another one, his chirality will be changed (from left-handed to right-handed or from right-handed to left-handed). If a person took a long trip and finally got back to his own star, however, his chirality might be changed to the opposite state other than his original, which would cause fatal mental disorder, or even death. 

KMXS has the channels map among the starts and he need to prohibit minimum number of channels from traveling so that wherever a person starts his traveling from when he gets his original star he’ll be safe. KMXS turns to your help. 

# **Input**

The first line of input consists of an integer T, indicating the number of test cases. 

The first line of each case consists of two integers N and M, indicating the number of stars and the number of channels. Each of the next M lines indicates one channel (u, v) which means there is a bidirectional channel between star u and star v (u is not equal to v). 

# Output

Output one integer on a single line for each case, indicating the minimum number of channels KMXS must prohibit to avoid mental disorder. 

Constraints 

0 < T <= 10 

0 <= N <= 15 0 <= M <= 300 

0 <= u, v < N and there may be more than one channel between two stars. 

# Sample Input

**13 30 11 22 0**

# Sample Output

**1**


题意：t 组数据，每组给出 n 个点以及 m 条无向边，已知每次经过一条边，手性会发生改变，即从左撇子变为右撇子或从右撇子变成左撇子，现在某个人从自己的点出发，要求他无论如何走，当他回到自己的点时，手性不发生改变，问最少删除多少条边

思路：

以题目输入为例，假设初始手性是左，并从 0 号点出发，经过 0-1 手性变为右，经过 1-2 手性又变回左，经过 2-0 手性再次变为右，与开始的手性相反，不合题意。可以在给出的三条边中删掉任意一条边，这样从 0 号点出发，无论如何走，最终回到 0 号点一定与开始的手性相同。

通过题意与输入样例可以发现，题目其实就是要求从任何一个点出发，都不能经过奇数次返回初始点，也即要求图中不能有奇数环。

可根据二分图的充要条件（图中至少有两个点，且所有的回路长度均是偶数）来进行删边，即对于二分图的两部分，同一部分是没有边的，因此只要将同一部分的边删掉，直到构成二分图为止。

简单来说，对于 n 个点，其初始手性均有两种可能，那么就共有 2^n 种情况，对于每种情况，去枚举 n 个点与其对应的点，判断两点是否在同一部分，若在同一部分，则累加边数（要删掉的），最后在这 2^n 种情况中选取最小的一个即可

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
#define E 1e-9
#define INF 0x3f3f3f3f
#define LL long long
const int MOD=10007;
const int N=2000+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;

int n,m;
int G[N][N];
int del(int k){
    int num=0;//删边个数
    for(int i=0;i<n;i++){//对于左点集的n个点
        for(int j=i+1;j<n;j++){//对应右点集的点
            if( (k&(1<<i))==0 && (k&(1<<j))==0 )//同属一个集合
                num+=G[i][j];//要删去这个集合中所有相连的边
            else if( (k&(1<<i))!=0 && (k&(1<<j))!=0 )//同属另一个集合
                num+=G[i][j];//要删去这个集合中所有相连的边
        }
    }
    return num;
}
int main(){
    int t;
    scanf("%d",&t);
    while(t--){
        memset(G,0,sizeof(G));
        scanf("%d%d",&n,&m);

        if(n==0||m==0){
            printf("0\n");
            continue;
        }

        for(int i=1;i<=m;i++){//m条无向边
            int x,y;
            scanf("%d%d",&x,&y);
            G[x][y]++;
            G[y][x]++;
        }

        int res=INF;
        for(int i=1;i<(1<<n);i++)//对于n个点，每个点的手性或左或右，共有2^n种状态
            res=min(res,del(i));//选取2^n种状态里删边个数最少的

        printf("%d\n",res);
    }
    return 0;
}
```






