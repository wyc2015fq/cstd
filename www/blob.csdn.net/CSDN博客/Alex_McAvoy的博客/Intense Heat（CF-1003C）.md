# Intense Heat（CF-1003C） - Alex_McAvoy的博客 - CSDN博客





2018年08月05日 20:11:44[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：63
个人分类：[CodeForces																[搜索——暴力搜索及打表](https://blog.csdn.net/u011815404/article/category/7925875)](https://blog.csdn.net/u011815404/article/category/7923180)








> 
# Problem Description

The heat during the last few days has been really intense. Scientists from all over the Berland study how the temperatures and weather change, and they claim that this summer is abnormally hot. But any scientific claim sounds a lot more reasonable if there are some numbers involved, so they have decided to actually calculate some value which would represent how high the temperatures are.

Mathematicians of Berland State University came up with a special heat intensity value. This value is calculated as follows:

Suppose we want to analyze the segment of nn consecutive days. We have measured the temperatures during these nn days; the temperature during ii-th day equals aiai.

We denote the average temperature of a segment of some consecutive days as the arithmetic mean of the temperature measures during this segment of days. So, if we want to analyze the average temperature from day xx to day yy, we calculate it as ∑i=xyaiy−x+1 (note that division is performed without any rounding). The heat intensity value is the maximum of average temperatures over all segments of not less than kk consecutive days. For example, if analyzing the measures [3,4,1,2] and k=3, we are interested in segments [3,4,1], [4,1,2] and [3,4,1,2] (we want to find the maximum value of average temperature over these segments).

You have been hired by Berland State University to write a program that would compute the heat intensity value of a given period of days. Are you up to this task?

# Input

The first line contains two integers nn and kk (1≤k≤n≤5000) — the number of days in the given period, and the minimum number of days in a segment we consider when calculating heat intensity value, respectively.

The second line contains nn integers a1, a2, ..., an (1≤ai≤5000) — the temperature measures during given n days.

# Output

Print one real number — the heat intensity value, i. e., the maximum of average temperatures over all segments of not less than kk consecutive days.

Your answer will be considered correct if the following condition holds: |res−res0|<10−6, where resres is your answer, and res0 is the answer given by the jury's solution.

# Examples

**Input**

4 3

3 4 1 2

**Output**

2.666666666666667


题意：给出连续 n 的温度为 ai，用 x，y 表示任意从 x 天开始，到 y 天结束，k 表示是最短的连续天数，定义平均热度：y-x+1>=k，求最大的的平均热度

思路：使用前缀和，处理出前 i 天的温度和，即求出某个区间的和，再暴力枚举区间

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
#define N 10000005
#define MOD 123
#define E 1e-6
using namespace std;
double a[N],sum[N];
int main()
{
    int n,k;
    cin>>n>>k;

    sum[0]=0;
    double maxx=-INF;
    for(int i=1;i<=n;i++)
    {
        cin>>a[i];
        sum[i]=sum[i-1]+a[i];//记录前i天的温度和
        if(i>=k)
        {
            maxx=max(maxx,sum[i]/(i*1.0));
            for(int j=1;i-j>=k;j++)//枚举区间
                maxx=max(maxx,(sum[i]-sum[j])/((i-j)*1.0));
        }
    }
    printf("%.15f\n",maxx);//注意输出保留的小数位数
    return 0;
}
```






