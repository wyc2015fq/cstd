# Codeforces1003C——Intense Heat - westbrook1998的博客 - CSDN博客





2018年08月09日 18:58:29[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：41








> 
The heat during the last few days has been really intense. Scientists from all over the Berland study how the temperatures and weather change, and they claim that this summer is abnormally hot. But any scientific claim sounds a lot more reasonable if there are some numbers involved, so they have decided to actually calculate some value which would represent how high the temperatures are. 

  Mathematicians of Berland State University came up with a special heat intensity value. This value is calculated as follows: 

  Suppose we want to analyze the segment of n consecutive days. We have measured the temperatures during these n days; the temperature during i-th day equals ai. 

  We denote the average temperature of a segment of some consecutive days as the arithmetic mean of the temperature measures during this segment of days. So, if we want to analyze the average temperature from day x to day y, we calculate it as ∑i=xyaiy−x+1 (note that division is performed without any rounding). The heat intensity value is the maximum of average temperatures over all segments of not less than k consecutive days. For example, if analyzing the measures [3,4,1,2] and k=3, we are interested in segments [3,4,1], [4,1,2] and [3,4,1,2] (we want to find the maximum value of average temperature over these segments). 

  You have been hired by Berland State University to write a program that would compute the heat intensity value of a given period of days. Are you up to this task? 

  Input 

  The first line contains two integers n and k (1≤k≤n≤5000) — the number of days in the given period, and the minimum number of days in a segment we consider when calculating heat intensity value, respectively. 

  The second line contains n integers a1, a2, …, an (1≤ai≤5000) — the temperature measures during given n days. 

  Output 

  Print one real number — the heat intensity value, i. e., the maximum of average temperatures over all segments of not less than k consecutive days. 

  Your answer will be considered correct if the following condition holds: |res−res0|<10−6, where res is your answer, and res0 is the answer given by the jury’s solution. 

  Example 

  Input 

  4 3 

  3 4 1 2 

  Output 

  2.666666666666667
这题就是用到前缀和 读取的时候就先求前缀和，然后后面就枚举区间了 

这里注意区间长度是不小于k 一开始题没看仔细 

代码：
```cpp
#include <cstdio>
#include <algorithm>
using namespace std;
const int MAXN=5050;
int a[MAXN];
int p[MAXN];
int n,k;
int t;
int main(void){
    scanf("%d%d",&n,&k);
    for(int i=1;i<=n;i++){
        scanf("%d",&t);
        p[i]=p[i-1]+t;
    }
    double res=p[n]*1.0/n;
    for(int i=1;i+k-1<=n;i++){
        for(int j=i+k-1;j<=n;j++){
            //printf("%d %d %d\n",i,j,(p[j]-p[i-1])*1.0/j-i+1);
            double tmp=(p[j]-p[i-1])*1.0/(j-i+1);
            res=max(res,tmp);
        }
    }
    printf("%.15lf\n",res);
    return 0;
}
```






