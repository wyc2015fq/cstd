# Birthday Paradox（LightOJ-1104） - Alex_McAvoy的博客 - CSDN博客





2019年04月02日 15:01:23[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：18
个人分类：[LightOJ																[概率与期望——概率](https://blog.csdn.net/u011815404/article/category/8820558)](https://blog.csdn.net/u011815404/article/category/8037630)








> 
# Problem Description

Sometimes some mathematical results are hard to believe. One of the common problems is the birthday paradox. Suppose you are in a party where there are 23 people including you. What is the probability that at least two people in the party have same birthday? Surprisingly the result is more than 0.5. Now here you have to do the opposite. You have given the number of days in a year. Remember that you can be in a different planet, for example, in Mars, a year is 669 days long. You have to find the minimum number of people you have to invite in a party such that the probability of at least two people in the party have same birthday is at least 0.5.

# **Input**

Input starts with an integer T (≤ 20000), denoting the number of test cases.

Each case contains an integer n (1 ≤ n ≤ 105) in a single line, denoting the number of days in a year in the planet.

# Output

For each case, print the case number and the desired result.

# Sample Input

**2365669**

# Sample Output

**Case 1: 22Case 2: 30**


题意：在某个星球上一年有 n 天，现在要邀请人参加聚会，现在使得聚会上至少两个人同一天生日的概率最小为 0.5，问最少有多少人

思路：

对于 n 天来说，每个人生日的概率为 ![\frac{1}{n}](https://private.codecogs.com/gif.latex?%5Cfrac%7B1%7D%7Bn%7D)

假设邀请了 x 个人，那么当加上自己时，x+1 个人生日都不同的概率为：![p=1*(1-\frac{1}{n})*(1-\frac{2}{n})*...*(1-\frac{x-1}{n})](https://private.codecogs.com/gif.latex?p%3D1*%281-%5Cfrac%7B1%7D%7Bn%7D%29*%281-%5Cfrac%7B2%7D%7Bn%7D%29*...*%281-%5Cfrac%7Bx-1%7D%7Bn%7D%29)

那么，至少两个人生日同一天的概率为：![q=1-p](https://private.codecogs.com/gif.latex?q%3D1-p)

现在要使两个人同一天生日相同的概率最小为 0.5，即 q>=0.5，因此可以枚举邀请人数 x，计算 p，一旦当 1-p>=0.5 时，此时的 x 就是最小人数

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
int main(){
    int t;
    scanf("%d",&t);

    int Case=1;
    while(t--){
        int n;
        scanf("%d",&n);

        int x=0;
        double p=1.0,q=0.0;
        while(q<0.5){
            x++;
            p=p*(1-1.0*(x-1)/n);
            q=1-p;
            //x++;
        }
        
        x--;//减去自己
        printf("Case %d: %d\n",Case++,x);
    }

    return 0;
}
```






