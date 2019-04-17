# Running（POJ-3661） - Alex_McAvoy的博客 - CSDN博客





2018年05月29日 20:12:09[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：70








# Problem Description

The cows are trying to become better athletes, so Bessie is running on a track for exactly N (1 ≤ N ≤ 10,000) minutes. During each minute, she can choose to either run or rest for the whole minute.

The ultimate distance Bessie runs, though, depends on her 'exhaustion factor', which starts at 0. When she chooses to run in minute i, she will run exactly a distance of Di (1 ≤ Di ≤ 1,000) and her exhaustion factor will increase by 1 -- but must never be allowed to exceed M (1 ≤ M ≤ 500). If she chooses to rest, her exhaustion factor will decrease by 1 for each minute she rests. She cannot commence running again until her exhaustion factor reaches 0. At that point, she can choose to run or rest.

At the end of the N minute workout, Bessie's exaustion factor must be exactly 0, or she will not have enough energy left for the rest of the day.

Find the maximal distance Bessie can run.

# **Input**

Line 1: Two space-separated integers: N and M

Lines 2..N+1: Line i+1 contains the single integer: Di

# Output

Line 1: A single integer representing the largest distance Bessie can run while satisfying the conditions.

# Sample Input

**5 2**

**534210**

# Sample Output

**9**

———————————————————————————————————————————————————————

题意：在赛道跑n分钟，每分钟可以选择跑或者休息，以分钟i跑时会跑完Di的距离但疲劳因子会+1，如果休息，每分钟疲劳因子会-1，但要减到0才能再次跑。疲劳因子不会超过m，在n分钟结束后，疲劳因子必须为0，求最大距离。



思路

是一个区间dp题，用f[i][j]表示第i分钟疲劳值为j时的最大值，由于每分钟有休息或者跑两种状态。

当选择跑时，当前分钟的最大值由上一分钟的最大值决定，故有：f[i][j]=f[i-1][j-1]+D[i]

当选择休息时有：当前状态的最大值由前面决定开始休息的时间点i-j决定，故有：f[i][0]=max{ d[i-j][j] }，j<=m

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
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 10001
#define MOD 2520
#define E 1e-12
using namespace std;
int f[N][501],d[N];
int main()
{
    int n,m;

    while(scanf("%d%d",&n,&m)!=EOF)
    {
        memset(f,0,sizeof(f));
        for(int i=1;i<=n;i++)
            scanf("%d",&d[i]);

        for(int i=1;i<=n;i++)
        {
            for(int j=1;j<=m;j++)//跑
                f[i][j]=f[i-1][j-1]+d[i];

            f[i][0]=f[i-1][0];
            for(int j=1;j<=i&&j<=m;j++)//休息
                f[i][0]=max(f[i][0],f[i-j][j]);
        }
        printf("%d\n",f[n][0]);
    }
    return 0;
}
```









