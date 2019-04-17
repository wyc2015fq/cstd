# Tourism Planning（HDU-4096） - Alex_McAvoy的博客 - CSDN博客





2018年10月02日 18:56:01[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：42








# Problem Description

Several friends are planning to take tourism during the next holiday. They have selected some places to visit. They have decided which place to start their tourism and in which order to visit these places. However, anyone can leave

 halfway during the tourism and will never back to the tourism again if he or she is not interested in the following places. And anyone can choose not to attend the tourism if he or she is not interested in any of the places.

Each place they visited will cost every person certain amount of money. And each person has a positive value for each place, representing his or her interest in this place. To make things more complicated, if two friends visited a place together, they will

 get a non negative bonus because they enjoyed each other’s companion. If more than two friends visited a place together, the total bonus will be the sum of each pair of friends’ bonuses.

Your task is to decide which people should take the tourism and when each of them should leave so that the sum of the interest plus the sum of the bonuses minus the total costs is the largest. If you can’t find a plan that have a result larger than 0, just

 tell them to STAY HOME.

# **Input**

There are several cases. Each case starts with a line containing two numbers N and M ( 1<=N<=10, 1<=M<=10). N is the number of friends and M is the number of places. The next line will contain M integers Pi (1<=i<=M) , 1<=Pi<=1000,

 representing how much it costs for one person to visit the ith place. Then N line follows, and each line contains M integers Vij (1<=i<=N, 1<=j<=M), 1<=Vij<=1000, representing how much the ith person is interested in the jth place. Then N line follows, and

 each line contains N integers Bij (1<=i<=N, 1<=j<=N), 0<=Bij<=1000, Bij=0 if i=j, Bij=Bji.

A case starting with 0 0 indicates the end of input and you needn’t give an output.

# Output

For each case, if you can arrange a plan lead to a positive result, output the result in one line, otherwise, output STAY HOME in one line.

# Sample Input

**2 1101550 55 03 230 5024 4840 7035 200 4 14 0 51 5 02 2100 10050 5050 500 2020 00 0**

# Sample Output

**541STAY HOME**

————————————————————————————————————————————————————

题意：

给出两个数字分别代表有 n 个人，m 个城市，然后给出 m 个数字代表每个人参观城市的花费，再给出 n 行 m 列表示每个人参观每个城市的满意度，最后给出 n 行 n 列表示每参观一个城市相互之间影响的额外满意度

现可安排 n 个人的任意多个依次参观这 m 个城市，中途可以让一个人退出，求最大的参观满意度

满意度=每个人参观城市的满意度和+相互之间影响的额外满意度和-参观花费和

思路：状压DP

由于数据 n、m 较小，因此可以考虑使用状压DP的方法，用2进制来表示有哪些人去参观，用sum来表示参观的人，用sta来表示sum的子状态来表示中途离开后剩下的人，用k记录当前访问的城市，用dp[sum][k]来表示当前状态下的最大值，即当有sum个人访问到k个城市的时候，中途退了一些人，转移到了sta，k+1城市的状态，用value[sum][k]来表示sum个人在k城市获得的总值

有：dp[sum][k]=max(dp[sta][k+1],dp[sum][k])+value[sum][k];

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
#define N 11
#define MOD 10007
#define E 1e-6
#define LL long long
using namespace std;
int n,m;
int cost[N],satisfy[N][N],extra[N][N];
int dp[1<<N][N],value[1<<N][N];
int vis[1<<N][N];
int check(int sum,int k){
    if(k>=m)
        return 0;
    if(vis[sum][k])
        return dp[sum][k];

    int res=0;
    for(int i=sum;i!=0;i=(i-1)&sum){//枚举所有子状态
        int temp=check(i,k+1)+value[sum][k];//向下递归
        if(temp>res)//记录最大值
            res=temp;
    }
    vis[sum][k]=1;
    dp[sum][k]=res;
    return dp[sum][k];
}
int main()
{
	while(scanf("%d%d",&n,&m)!=EOF&&(n+m))
    {
        /*数据输入及处理*/
        memset(vis,0,sizeof(vis));
        for(int i=0;i<m;i++)//参观城市的花费
            scanf("%d",&cost[i]);
        for(int i=0;i<n;i++)//每个人参观的满意度
            for(int j=0;j<m;j++)
                scanf("%d",&satisfy[i][j]);
        for(int i=0;i<n;i++)//城市间相互影响的额外满意度
            for(int j=0;j<n;j++)
                scanf("%d",&extra[i][j]);

        for(int i=0;i<m;i++){   //枚举城市
            for(int sum=0;sum<(1<<n);sum++){    //枚举人去或不去的所有状态
                value[sum][i]=0;//在第i个城市获得的总值
                for(int p1=0;p1<n;p1++){
                    if(!((1<<p1)&sum))//如果第j个城市没有去,继续向下枚举城市
                        continue;
                    for(int p2=0;p2<p1;p2++)//枚举从第一个到第j个城市
                        if((1<<p2)&sum)//如果去了第k个
                            value[sum][i]+=extra[p1][p2];//计算额外满意度
                    value[sum][i]+=satisfy[p1][i]-cost[i];//计算sum个人在第i个城市的总额度
                }
            }
        }


        /*问题解决*/
        int res=0;
        for(int i=0;i<(1<<n);i++)
            if(check(i,0)>res)
                res=check(i,0);

        if(!res)
            printf("STAY HOME\n");
        else
            printf("%d\n",res);

    }
	return 0;
}
```





