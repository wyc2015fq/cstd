# Traveling（HDU-3011） - Alex_McAvoy的博客 - CSDN博客





2018年08月22日 14:09:18[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：74








# Problem Description

After coding so many days,Mr Acmer wants to have a good rest.So travelling is the best choice!He has decided to visit n cities(he insists on seeing all the cities!And he does not mind which city being his start station because superman can bring him to any city at first but only once.), and of course there are m roads here,following a fee as usual.But Mr Acmer gets bored so easily that he doesn't want to visit a city more than twice!And he is so mean that he wants to minimize the total fee!He is lazy you see.So he turns to you for help.

# **Input**

There are several test cases,the first line is two intergers n(1<=n<=10) and m,which means he needs to visit n cities and there are m roads he can choose,then m lines follow,each line will include three intergers a,b and c(1<=a,b<=n),means there is a road between a and b and the cost is of course c.Input to the End Of File.

# Output

Output the minimum fee that he should pay,or -1 if he can't find such a route.

# Sample Input

**2 11 2 1003 21 2 402 3 503 31 2 31 3 42 3 10**

# Sample Output

**100907 **

————————————————————————————————————————————————————

题意：10 个点的 TSP 问题，要求每个点最多走两次，求最短距离

思路：用三进制的状态压缩来解决，代表走过这个点的次数，以 dp[state][i] 表示到达 i 点状态为 state 的最短距离

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
#define N 1000001
#define MOD 1e9+7
#define E 1e-6
#define LL long long
using namespace std;
int dig[3<<10+1][11];
int dp[3<<10+1][11];
int edge[11][11];
int tri[12]={0,1,3,9,27,81,243,729,2187,6561,19683,59049};
int main()
{
    for(int i=0;i<(3<<10)+1;i++)//预处理每个状态的第k位
    {
        int temp=i;
        for(int j=1;j<=10;j++)
        {
            dig[i][j]=temp%3;
            temp/=3;
            if(temp==0)
                break;
        }
    }

    int n,m;
    while(scanf("%d%d",&n,&m)!=EOF)
    {
        memset(dp,INF,sizeof(dp));
        memset(edge,INF,sizeof(edge));

        while(m--)
        {
            int x,y,w;
            cin>>x>>y>>w;
            if(w<edge[x][y])
                edge[x][y]=edge[y][x]=w;
        }

        for(int i=1;i<=n;i++)
            dp[tri[i]][i]=0;

        int minn=INF;
        for(int s=0;s<tri[n+1];s++)//枚举所有状态
        {
            int visit= 1;
            for(int i=1;i<=n;i++)//枚举所有点
            {
                if(dig[s][i]==0)
                    visit=0;
                if(dp[s][i]==INF)
                    continue;

                for(int j=1;j<=n;j++)
                {
                    if(i==j)
                        continue;
                    if(edge[i][j]==INF||dig[s][j]>=2)
                        continue;
                    int new_state=s+tri[j];
                    dp[new_state][j] =min(dp[new_state][j],dp[s][i]+edge[i][j]);
                }
            }

            if(visit)
                for(int j=1;j<=n;j++)
                    minn=min(minn,dp[s][j]);
        }

        if(minn==INF)
            cout<<-1<<endl;
        else
            cout<<minn<<endl;
    }

    return 0;
}
```





