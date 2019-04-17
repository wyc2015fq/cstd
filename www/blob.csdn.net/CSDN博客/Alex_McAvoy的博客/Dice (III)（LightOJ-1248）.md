# Dice (III)（LightOJ-1248） - Alex_McAvoy的博客 - CSDN博客





2019年04月03日 09:14:16[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：19
个人分类：[LightOJ																[动态规划——概率 DP 与期望 DP](https://blog.csdn.net/u011815404/article/category/8820787)](https://blog.csdn.net/u011815404/article/category/8037630)








> 
# Problem Description

Given a dice with n sides, you have to find the expected number of times you have to throw that dice to see all its faces at least once. Assume that the dice is fair, that means when you throw the dice, the probability of occurring any face is equal.

For example, for a fair two sided coin, the result is 3. Because when you first throw the coin, you will definitely see a new face. If you throw the coin again, the chance of getting the opposite side is 0.5, and the chance of getting the same side is 0.5. So, the result is

1 + (1 + 0.5 * (1 + 0.5 * ...))

= 2 + 0.5 + 0.52 + 0.53 + ...

= 2 + 1 = 3

# **Input**

Input starts with an integer T (≤ 100), denoting the number of test cases.

Each case starts with a line containing an integer n (1 ≤ n ≤ 105).

# Output

For each case, print the case number and the expected number of times you have to throw the dice to see all its faces at least once. Errors less than 10-6 will be ignored.

# Sample Input

**551236100**

# Sample Output

**Case 1: 1Case 2: 3Case 3: 5.5Case 4: 14.7Case 5: 518.7377517640**


题意：t 组数据，每组给一个数 n 代表一个有 n 面的骰子，骰子的值从 1~n，现在扔骰子，直到所有的面都出现一次，问掷骰子次数的期望值

思路：

设 f[i] 为扔出 i 面次数的期望值，那么对于 n 面的骰子，每掷一次只有两种情况：
- 出现已出现的面：其概率为 i/n
- 出现未出现的面：其概率为 (n-i)/n

那么扔出 i 面次数的期望值为：扔出已出现的面的概率*扔出 i 面次数的期望+扔出未出现的面的概率*扔出 i+1 面次数的期望+1

则有状态转移方程：f[i]=f[i]*i/n+f[i+1]*(n-i)/n+1

化简后有：f[i]=f[i+1]+n/(n-i)

由于期望 DP 的状态去逆推，扔出 0 面次数的期望值为 0，即 f[n]=0，那么答案即为 f[0]

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
#define E 1e-9
#define PI acos(-1.0)
#define INF 0x3f3f3f3f
#define LL long long
const int MOD=7;
const int N=100000+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;
double f[N];//f[i]为扔出i面次数的期望值
int main(){
    int t;
    scanf("%d",&t);

    int Case=1;
    while(t--){
        int n;
        scanf("%d",&n);

        memset(f,0,sizeof(f));
        f[n]=0;
        for(int i=n-1;i>=0;i--)
            f[i]=f[i+1]+1.0*n/(n-i);

        printf("Case %d: %.10lf\n",Case++,f[0]);
    }
    return 0;
}
```






