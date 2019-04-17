# Problem Solving（POJ-3265） - Alex_McAvoy的博客 - CSDN博客





2018年06月24日 13:58:14[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：67
个人分类：[POJ																[动态规划——线性 DP](https://blog.csdn.net/u011815404/article/category/7813170)](https://blog.csdn.net/u011815404/article/category/7504500)








# Problem Description

In easier times, Farmer John's cows had no problems. These days, though, they have problems, lots of problems; they have P (1 ≤ P ≤ 300) problems, to be exact. They have quit providing milk and have taken regular jobs like all other good citizens. In fact, on a normal month they make M (1 ≤ M ≤ 1000) money.

Their problems, however, are so complex they must hire consultants to solve them. Consultants are not free, but they are competent: consultants can solve any problem in a single month. Each consultant demands two payments: one in advance (1 ≤ payment ≤ M) to be paid at the start of the month problem-solving is commenced and one more payment at the start of the month after the problem is solved (1 ≤ payment ≤ M). Thus, each month the cows can spend the money earned during the previous month to pay for consultants. Cows are spendthrifts: they can never save any money from month-to-month; money not used is wasted on cow candy.

Since the problems to be solved depend on each other, they must be solved mostly in order. For example, problem 3 must be solved before problem 4 or during the same month as problem 4.

Determine the number of months it takes to solve all of the cows' problems and pay for the solutions.



# **Input**

Line 1: Two space-separated integers: M and P. 

Lines 2..P+1: Line i+1 describes problem i with two space-separated integers: Bi and Ai. Bi is the payment to the consult BEFORE the problem is solved; Ai is the payment to the consult AFTER the problem is solved. 

# Output

Line 1: The number of months it takes to solve and pay for all the cows' problems.


# Sample Input

**100 5**

**40 2060 2030 5030 5040 40**

# Sample Output

**6**

——————————————————————————————————————————————————————————————————————————————————————————

题意：有p个问题，每人每月解决一个问题，解决问题有两个代价，当月给a[i]，第二月给b[i]，然后每个月钱有m，求最快多久能解决所有问题。


思路：

假设j之前为上个月及之前完成的，j到i为在本月完成的，那么：

    当 before[j-1]+a[j...i]<=m 成立时，最小值为 f[j-1]+1

    当 before[j-1]+a[j...i]>m 成立时，最小值为 f[j-1]+2

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
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 1001
#define MOD 123
#define E 1e-6
using namespace std;
int a[N],b[N];
int before[N],f[N];
int main()
{
    int m,p;
    scanf("%d%d",&m,&p);
    for(int i=1;i<=p;i++)
        scanf("%d%d",&a[i],&b[i]);

    before[0]=INF;
    f[0]=1;
    for(int i=1;i<=p;i++)
    {
        int cost_a=0,cost_b=0;
        f[i]=f[i-1]+2;
        before[i]=b[i];
        for (int j=i;j>=1;j--)//j之前为上个月及之前完成的，j到i为在本月完成的
        {
            cost_a+=a[j];
            cost_b+=b[j];

            if (cost_a > m||cost_b>m)//超出当月钱数，退出
                break;

            if (cost_a+before[j-1]<=m&&f[j-1]+1<=f[i])
            {
                if (f[j-1]+1==f[i])
                    before[i]=min(before[i],cost_b);
                else
                    before[i]=cost_b;
                f[i]=f[j-1]+1;
            }

            if (cost_a+before[j-1]>m&&f[j-1]+2<=f[i])
            {
                if(f[j-1]+2==f[i])
                    before[i]=min(before[i],cost_b);
                else
                    before[i]=cost_b;
                f[i]=f[j-1]+2;
            }
        }
    }
    printf("%d\n",f[p]);
    return 0;
}
```






