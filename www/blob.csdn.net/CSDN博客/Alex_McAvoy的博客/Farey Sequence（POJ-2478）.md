# Farey Sequence（POJ-2478） - Alex_McAvoy的博客 - CSDN博客





2018年08月10日 10:43:38[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：62
个人分类：[POJ																[数论——欧拉函数](https://blog.csdn.net/u011815404/article/category/8796918)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

The Farey Sequence Fn for any integer n with n >= 2 is the set of irreducible rational numbers a/b with 0 < a < b <= n and gcd(a,b) = 1 arranged in increasing order.

The first few are 

F2 = {1/2} 

F3 = {1/3, 1/2, 2/3} 

F4 = {1/4, 1/3, 1/2, 2/3, 3/4} 

F5 = {1/5, 1/4, 1/3, 2/5, 1/2, 3/5, 2/3, 3/4, 4/5} 

You task is to calculate the number of terms in the Farey sequence Fn.

# Input

There are several test cases. Each test case has only one line, which contains a positive integer n (2 <= n <= 106). There are no blank lines between cases. A line with a single 0 terminates the input.

# Output

For each test case, you should output one line, which contains N(n) ---- the number of terms in the Farey sequence Fn.

# Sample Input

**23450**

# Sample Output

**1359**


题意：给一个数 n，对于序列 Fn，其是不可约分的有理数 a/b 的集合，其中 0 <a <b <= n 且 GCD(a,b)=1

思路：

观察序列，每次 n 增大 1，序列添加的个数就是分子与分母互质的个数，问题即为求小于 n 的与 n 互质的数的个数的和，即求欧拉函数从 2 到 n 的和。

先打表求欧拉函数，再用一数组存储所有欧拉函数的和，最后输出即可。

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
#define N 1000005
#define MOD 1e9+7
#define E 1e-6
#define LL long long
using namespace std;
int phi[N];
LL sum[N];
void Euler()
{
    for(int i=1;i<=N;i++)
        phi[i]=i;

    for(int i=2;i<=N;i+= 2)
        phi[i]/=2;

    for(int i=3;i<=N;i+= 2)
    {
        if(phi[i]==i)
        {
            for(int j=i;j<=N;j+=i)
                phi[j]=phi[j]/i*(i-1);
        }
    }
}
int main()
{
    Euler();
    for(int i=2;i<=N;i++)
        sum[i]=sum[i-1]+phi[i];

    int n;
    while(scanf("%d",&n)!=EOF&&n)
        printf("%lld\n",sum[n]);

    return 0;

}
```






