# How many integers can you find（HDU-1796） - Alex_McAvoy的博客 - CSDN博客





2018年08月12日 21:22:05[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：56
个人分类：[HDU																[—————组合数学—————](https://blog.csdn.net/u011815404/article/category/7900800)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

Now you get a number N, and a M-integers set, you should find out how many integers which are small than N, that they can divided exactly by any integers in the set. For example, N=12, and M-integer set is {2,3}, so there is another set {2,3,4,6,8,9,10}, all the integers of the set can be divided exactly by 2 or 3. As a result, you just output the number 7.

# **Input**

There are a lot of cases. For each case, the first line contains two integers N and M. The follow line contains the M integers, and all of them are different from each other. 0<N<2^31,0<M<=10, and the M integer are non-negative and won’t exceed 20.

# Output

For each case, output the number.

# Sample Input

**12 22 **

# Sample Output

**7**


题意：给出一个 m 的集合，求 1到 n 中，能被集合里任一元素整除的个数

思路：求与一个数整除的数的个数时，一定会有重复的数字，因此要用容斥原理减去重复的数字，枚举所有元素，计算最小公倍数，元素个数为奇时加，为偶时减

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
#define MOD 1e9+7
#define E 1e-6
#define LL long long
using namespace std;
LL a[N];
LL GCD(LL a,LL b)
{
    return b==0?a:GCD(b,a%b);
}
int main()
{
    LL n,m;
    while(scanf("%lld%lld",&n,&m)!=EOF&&(n+m))
    {
        for(LL i=0;i<m;i++)
            scanf("%lld",&a[i]);

        LL sum=0;
        for(LL i=1;i<(1<<m);i++)
        {
            LL lcm=1;
            LL num=0;
            for(LL j=0;j<m;j++)
            {
                if(i&(1<<j))
                {
                    num++;
                    lcm=lcm*a[j]/GCD(lcm,a[j]);
                }
            }

            LL temp=0;
            if(lcm!=0)
                temp=(n-1)/lcm;
            if(num&1)
                sum+=temp;
            else
                sum-=temp;
        }

        if(sum<0)
            sum=0;
        printf("%lld\n",sum);
    }
    return 0;
}
```






