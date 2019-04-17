# Close Encounter（POJ-3039） - Alex_McAvoy的博客 - CSDN博客





2018年07月25日 21:03:14[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：60








# Problem Description

Lacking even a fifth grade education, the cows are having trouble with a fraction problem from their textbook. Please help them. The problem is simple: Given a properly reduced fraction (i.e., the greatest

common divisor of the numerator and denominator is 1, so the fraction cannot be further reduced) find the smallest properly reduced fraction with numerator and denominator in the range 1..32,767 that is closest (but not equal) to the given fraction. 

# **Input**

* Line 1: Two positive space-separated integers N and D (1 <= N < D <= 32,767), respectively the numerator and denominator of the given fraction.

# Output

* Line 1: Two space-separated integers, respectively the numerator and denominator of the smallest, closest fraction different from the input fraction.

# Sample Input

**2 3**

# Sample Output

**21845 32767**

————————————————————————————————————————————————————

题意：给出一个分数的分子 m 与分母 n，求与这个分数 m/n 最接近但不相等的分数 x/y ，数据限制在 1<=x,y<= 32767 之间

思路：枚举分母，从 1 枚举到 32767，与 m/n 最接近的分数一定是 x/y 或 x+1/y，剩下的注意精度即可，可以用 m*y==n*x 来判断精度。

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
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 1001
#define MOD 123
#define E 1e-6
using namespace std;
int main()
{
    int m,n;
    int x,y;
    scanf("%d%d",&m,&n);

    double ans=INF;
    for(int i=2;i<=32767;i++)
    {
        int temp1=i*m/n;//分子
        //double temp2=fabs((double)temp1/(double)i-(double)m/(double)n);
        if( fabs((double)temp1/(double)i-(double)m/(double)n)<ans && temp1*n!=i*m )//计算x/y
        {
            x=temp1;
            y=i;
            ans=fabs((double)temp1/(double)i-(double)m/(double)n);
        }
        temp1++;
        if( fabs((double)temp1/(double)i-(double)m/(double)n)<ans && temp1*n!=i*m )//计算x+1/y
        {
            x=temp1;
            y=i;
            ans=fabs((double)temp1/(double)i-(double)m/(double)n);
        }

    }

    printf("%d %d\n",x,y);

    return 0;
}
```





