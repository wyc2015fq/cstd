# Find a multiple（POJ-2356） - Alex_McAvoy的博客 - CSDN博客





2018年08月13日 06:38:36[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：672
个人分类：[POJ																[—————组合数学—————](https://blog.csdn.net/u011815404/article/category/7900800)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

The input contains N natural (i.e. positive integer) numbers ( N <= 10000 ). Each of that numbers is not greater than 15000. This numbers are not necessarily different (so it may happen that two or more of them will be equal). Your task is to choose a few of given numbers ( 1 <= few <= N ) so that the sum of chosen numbers is multiple for N (i.e. N * k = (sum of chosen numbers) for some natural number k).

# **Input**

The first line of the input contains the single number N. Each of next N lines contains one number from the given set.

# Output

In case your program decides that the target set of numbers can not be found it should print to the output the single number 0. Otherwise it should print the number of the chosen numbers in the first line followed by the chosen numbers themselves (on a separate line each) in arbitrary order. 

If there are more than one set of numbers with required properties you should print to the output only one (preferably your favorite) of them.

# Sample Input

**512341**

# Sample Output

**223**


————————————————————————————————————————————————————

题意：给出一个含有 n 个数字的序列，要找一个连续的子序列，使他们的和一定是 n 的倍数

思路：抽屉原理经典应用，模板：[点击这里](https://blog.csdn.net/u011815404/article/details/81357109)

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
LL a[N],drawer[N];
LL sum[N];
int main()
{
    LL n;
    while(scanf("%lld",&n)!=EOF&&n)
    {
        memset(drawer,-1,sizeof(drawer));
		drawer[0]=0;
		sum[0]=0;

		for(LL i=1;i<=n;i++)
        {
			scanf("%lld",&a[i]);
			sum[i]=sum[i-1]+a[i];
		}

		for(LL i=1;i<=n;i++)
        {
			if(drawer[sum[i]%n]!=-1)
			{
			    printf("%lld\n",i-drawer[sum[i]%n]);
				for(LL j=drawer[sum[i]%n]+1;j<=i;j++)
				    printf("%lld\n",a[j]);
                break;
			}
			else
                drawer[sum[i]%n]=i;
        }
    }
    return 0;
}
```






