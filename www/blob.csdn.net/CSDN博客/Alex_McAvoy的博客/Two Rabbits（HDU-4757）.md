# Two Rabbits（HDU-4757） - Alex_McAvoy的博客 - CSDN博客





2018年08月17日 13:46:10[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：60
个人分类：[HDU																[动态规划——区间 DP](https://blog.csdn.net/u011815404/article/category/7813197)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

Long long ago, there lived two rabbits Tom and Jerry in the forest. On a sunny afternoon, they planned to play a game with some stones. There were n stones on the ground and they were arranged as a clockwise ring. That is to say, the first stone was adjacent to the second stone and the n-th stone, and the second stone is adjacent to the first stone and the third stone, and so on. The weight of the i-th stone is ai.

The rabbits jumped from one stone to another. Tom always jumped clockwise, and Jerry always jumped anticlockwise.

At the beginning, the rabbits both choose a stone and stand on it. Then at each turn, Tom should choose a stone which have not been stepped by itself and then jumped to it, and Jerry should do the same thing as Tom, but the jumping direction is anti-clockwise.

For some unknown reason, at any time , the weight of the two stones on which the two rabbits stood should be equal. Besides, any rabbit couldn't jump over a stone which have been stepped by itself. In other words, if the Tom had stood on the second stone, it cannot jump from the first stone to the third stone or from the n-the stone to the 4-th stone.

Please note that during the whole process, it was OK for the two rabbits to stand on a same stone at the same time. 

Now they want to find out the maximum turns they can play if they follow the optimal strategy.

# **Input**

The input contains at most 20 test cases.

For each test cases, the first line contains a integer n denoting the number of stones.

The next line contains n integers separated by space, and the i-th integer ai denotes the weight of the i-th stone.(1 <= n <= 1000, 1 <= ai <= 1000)

The input ends with n = 0.

# Output

For each test case, print a integer denoting the maximum turns.

# Sample Input

**1141 1 2 162 1 1 2 1 30**

# Sample Output

**145**


题意：n 个石头围成一圈，编号从 1 到 n，第 i 块石头重量是 ai，两只兔子从一块石头跳到另一块，一只顺时针跳一只逆时针跳，开始时，一只兔子可以选择一块石头，站在上面，每一轮，选择跳到一块他自己没有跳上去过的石头。在每一个时间，两只兔子所站的石头的重量必须相同，而且跳的时候，不能越过已经跳过的石头，两只兔子可以同时站在同一块石头上。求最多可以进行多少轮游戏。

思路：

由于两只兔子同一时刻所站的重量要求相同，一只顺时针跳一只逆时针跳，且同一时刻站在同一块石头上，那么可将石头重量看做不同的数字，那么问题就转化为求最长非连续的回文串

设 dp[i][j] 表示在 i 到 j 之间的回文序列的最大长度，那么，状态转移方程为 dp(i,j)=max(dp(i,j)，dp(i+1,j)，dp(i,j-1)）

如果 a[i]=a[j]，那么 dp(i,j)=max(dp(i,j)，dp(i+1,j-1)+2）

最后答案即为两个区间的回文长度和

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
const int N=1000+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;
int a[N];
int dp[N][N];
int main(){
    int n;
    while(scanf("%d",&n)!=EOF&&n){
        memset(dp,0,sizeof(dp));

        for(int i=1;i<=n;i++){
            scanf("%d",&a[i]);
            dp[i][i]++;//单个字符一定是一个回文子串
        }

        for(int len=2;len<=n;len++){//枚举长度
            for(int i=1;i+len-1<=n;i++){//枚举起点
                int j=i+len-1;//终点
                dp[i][j]=max(dp[i][j],max(dp[i+1][j],dp[i][j-1]));
                if(a[i]==a[j])
                    dp[i][j]=max(dp[i][j],dp[i+1][j-1]+2);
            }
        }

        int res=-INF;
        for(int i=1;i<=n;i++)
            res=max(res,dp[1][i]+dp[i+1][n]);

        printf("%d\n",res);
    }
    return 0;
}
```






