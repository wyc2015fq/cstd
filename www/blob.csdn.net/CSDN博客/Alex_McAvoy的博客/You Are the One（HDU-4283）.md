# You Are the One（HDU-4283） - Alex_McAvoy的博客 - CSDN博客





2018年08月17日 16:06:41[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：79








# Problem Description

The TV shows such as You Are the One has been very popular. In order to meet the need of boys who are still single, TJUT hold the show itself. The show is hold in the Small hall, so it attract a lot of boys and girls. Now there are n boys enrolling in. At the beginning, the n boys stand in a row and go to the stage one by one. However, the director suddenly knows that very boy has a value of diaosi D, if the boy is k-th one go to the stage, the unhappiness of him will be (k-1)*D, because he has to wait for (k-1) people. Luckily, there is a dark room in the Small hall, so the director can put the boy into the dark room temporarily and let the boys behind his go to stage before him. For the dark room is very narrow, the boy who first get into dark room has to leave last. The director wants to change the order of boys by the dark room, so the summary of unhappiness will be least. Can you help him?

# **Input**

The first line contains a single integer T, the number of test cases.  For each case, the first line is n (0 < n <= 100)

The next n line are n integer D1-Dn means the value of diaosi of boys (0 <= Di <= 100)

# Output

For each test case, output the least summary of unhappiness.

# Sample Input

**2512345**

**554322**

# Sample Output

**Case #1: 20Case #2: 24**

————————————————————————————————————————————————————

题意：n 个人排队，每个人有一个值 pi，第 i 个上台的人的不满意度为 (i-1)*pi，现在可以利用一个栈来调整顺序，第 i 个人进栈可以让第 i+1 个人先入场，对于栈里的元素必须是后进先出，求如何设置一个合理的调度顺序使得不满意度最小

思路：想了半天觉得是贪心也可以做，写出来了但没过，看了题解才明白是如何转成区间 DP 做的，还是水平不够

用 dp[i][j] 表示 i~j 中的元素可得到的最小的值，对于 i~j 中的元素 i 让他第 k 个入场，那么其后面的 k-1 个元素就要先入场

这时就有了：dp[i+1][i+k-1] 和 dp[i+k][j]，对于第 i 个元素的愤怒值为 ：(k-1)*a[i]，第 i+k~j 的值则要加上 k*(sum[j]-sum[i+1-1])

最后的 dp[1][n] 即为答案

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
#define MOD 10007
#define E 1e-6
#define LL long long
using namespace std;
int a[N];
int sum[N];
int dp[N][N];
int main()
{
    int t;
    scanf("%d",&t);

    int Case=1;
    while(t--)
    {
        int n;
        scanf("%d",&n);

        memset(dp,0,sizeof(dp));
        memset(sum,0,sizeof(sum));

        for(int i=1;i<=n;i++)
        {
            scanf("%d",&a[i]);
            sum[i]=sum[i-1]+a[i];
        }

        for(int len=1;len<n;len++)
        {
            for(int i=1;i<=n;i++)
            {
                int j=i+len;
                dp[i][j]=INF;
                for(int k=1;k<=j-i+1;k++)
                    dp[i][j]=min(dp[i][j],dp[i+1][i+k-1]+dp[i+k][j]+(k-1)*a[i]+k*(sum[j]-sum[i+k-1]));
            }
        }

        printf("Case #%d: %d\n",Case++,dp[1][n]);
    }

    return 0;
}
```





