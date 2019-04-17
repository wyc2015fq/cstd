# Making the Grade（POJ-3666） - Alex_McAvoy的博客 - CSDN博客





2019年03月14日 15:21:26[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：30
个人分类：[数据结构——离散化																[POJ](https://blog.csdn.net/u011815404/article/category/7504500)](https://blog.csdn.net/u011815404/article/category/8726557)








> 
# Problem Description

A straight dirt road connects two fields on FJ's farm, but it changes elevation more than FJ would like. His cows do not mind climbing up or down a single slope, but they are not fond of an alternating succession of hills and valleys. FJ would like to add and remove dirt from the road so that it becomes one monotonic slope (either sloping up or down).

You are given N integers A1, ... , AN (1 ≤ N ≤ 2,000) describing the elevation (0 ≤ Ai ≤ 1,000,000,000) at each of N equally-spaced positions along the road, starting at the first field and ending at the other. FJ would like to adjust these elevations to a new sequence B1, . ... , BN that is either nonincreasing or nondecreasing. Since it costs the same amount of money to add or remove dirt at any position along the road, the total cost of modifying the road is

| A 1 - B 1| + | A 2 - B 2| + ... + | AN - BN |

Please compute the minimum cost of grading his road so it becomes a continuous slope. FJ happily informs you that signed 32-bit integers can certainly be used to compute the answer.

# **Input**

* Line 1: A single integer: N

* Lines 2..N+1: Line i+1 contains a single integer elevation: Ai

# Output

* Line 1: A single integer that is the minimum cost for FJ to grade his dirt road so it becomes nonincreasing or nondecreasing in elevation.

# Sample Input

**71324539**

# Sample Output

**3**


题意：给出一个长度为 n 的序列，要求使序列变为单调上升或单调不减序列，问花费的最少代价

思路：

题目显然是 LIS 问题的变种，线性 DP 无疑

对于长度为 i 的序列，其最优解只与两个值有关，一个是这个序列处理的代价，另一个则是这个序列处理后的尾部的值。那么显然，我们希望序列处理的代价越小越好，而序列处理后的尾部的值越小越好，因为当序列处理完毕后，尾部的值是这个序列的最大值，其值越小，第 i+1 个数不花任何代价直接接在其后面的可能性就更大。

设 dp[i][j] 为长度为前 i 个数构成的序列，且处理完最大值为 j 所花费的相应代价，那么可以得出状态转移方程：

**dp[i][j]=abs(j-a[i])+min(dp[i-1][k]),k<=j**，其中 abs(j-a[i]) 代表处理的代价，min(dp[i-1][k]) 代表前 i-1 个数构成的序列最大值为 k 时所花费的最小代价

注意到 j 最大可达到 1,000,000,000，那么显然枚举的话一定会 TLE，而 n 的大小最大只有 2000，那么使用**离散化的思想**，先对序列 a[i] 进行处理，即：

对于长度为 n 的序列，可以发现序列中的某个数 a[i]，无论怎么变化，最小代价一定是序列中的某个数与当前这个数 a[i] 的差，因此我们可以建立一个备份数组 b[i]，将原来的 j-a[i] 转变为 b[j]-a[i]，也即 abs(b[j]-a[i])，然后再进行动态规划

最后，再从 dp[n][1~n] 中寻找最小值即可

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

int a[N];
int dp[N][N];
int b[N];//备份数组
int main(){
    int n;
    cin>>n;
    for(int i=1;i<=n;i++){
        cin>>a[i];
        b[i]=a[i];
    }
    sort(b+1,b+1+n);

    for(int i=1;i<=n;i++){
        int minn=INF;
        for(int j=1;j<=n;j++){
            int cost=abs(a[i]-b[j]);//处理的代价
            minn=min(minn,dp[i-1][j]);//前i-1个数最大值为j时的最小代价
            dp[i][j]=cost+minn;
        }
    }

    int res=INF;
    for(int i=1;i<=n;i++)
        res=min(res,dp[n][i]);
    cout<<res<<endl;

    return 0;
}
```






