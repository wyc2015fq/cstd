# Relatives（POJ-2407） - Alex_McAvoy的博客 - CSDN博客





2018年08月10日 10:19:46[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：44
个人分类：[POJ																[数论——欧拉函数](https://blog.csdn.net/u011815404/article/category/8796918)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

Given n, a positive integer, how many positive integers less than n are relatively prime to n? Two integers a and b are relatively prime if there are no integers x > 1, y > 0, z > 0 such that a = xy and b = xz.

# Input

There are several test cases. For each test case, standard input contains a line with n <= 1,000,000,000. A line containing 0 follows the last case.

# Output

For each test case there should be single line of output answering the question posed above.

# Sample Input

**7120**

# Sample Output

**64**


题意：给一个数 n，求这个数小于等于 n 的数中与 n 互质的数的个数。

思路：欧拉函数模板题

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
#define N 1005
#define MOD 1e9+7
#define E 1e-6
#define LL long long
using namespace std;
int Euler(int x)
{
    int res=x;
    for(int i=2;i<(int)sqrt(x*1.0)+1;i++)
    {
        if(x%i==0)
        {
            res=res/i*(i-1);
            while(x%i==0)
                x/=i;
        }
    }
    if(x>1)
        res=res/x*(x-1);
    return res;
}
int main()
{
    int n;
    while(scanf("%d",&n)!=EOF&&n)
    {
        int res=Euler(n);
		printf("%d\n",res);

    }

    return 0;
}
```






