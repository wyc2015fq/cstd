# Necklace（HDU-3091） - Alex_McAvoy的博客 - CSDN博客





2018年10月04日 01:03:08[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：84








# Problem Description

One day , Partychen gets several beads , he wants to make these beads a necklace . But not every beads can link to each other, every bead should link to some particular bead(s). Now , Partychen wants to know how many kinds of necklace he can make.

# **Input**

It consists of multi-case . 

Every case start with two integers N,M ( 1<=N<=18,M<=N*N ) 

The followed M lines contains two integers a,b ( 1<=a,b<=N ) which means the ath bead and the bth bead are able to be linked. 

# Output

An integer , which means the number of kinds that the necklace could be.

# Sample Input

**3 31 21 32 3**

# Sample Output

**2**

————————————————————————————————————————————————————

题意：给出贝壳个数 n 以及 m 组贝壳能连接的关系，求最多可组成的项链数

思路：状压DP

n<=18，可以枚举所有情况，因此可将所有珠子压缩成一个二进制数，使用状压DP来做，用 dp[i][j] 表示到达状态 i 第 j 个珠子时的方案数

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
#define N 20
#define MOD 10007
#define E 1e-6
#define LL long long
using namespace std;
int g[N][N];
LL dp[1<<N][N];
int main()
{
    int n,m;
    while(scanf("%d%d",&n,&m)!=EOF){
        memset(g,0,sizeof(g));
        memset(dp,0,sizeof(dp));

        while(m--){
            int x,y;
            scanf("%d%d",&x,&y);
            //存储连接关系
            g[x-1][y-1]=1;
            g[y-1][x-1]=1;
        }

        int allState=1<<n;
        dp[1][0]=1;
        for(int i=1;i<allState;i++)//枚举状态
            for(int j=0;j<n;j++)//枚举珠子个数
                if(dp[i][j]&&(i&(1<<j)))//判断条件
                    for(int k=1;k<n;k++)//枚举在当前珠子后面的珠子
                        if(!(i&(1<<k))&&g[j][k])
                            dp[i|(1<<k)][k]+=dp[i][j];


        LL res=0;
        for(int i=0;i<n;i++)
            if(g[0][i])
                res+=dp[allState-1][i];
        printf("%lld\n",res);
    }
	return 0;
}
```





