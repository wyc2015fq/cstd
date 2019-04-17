# Race（LightOJ-1326） - Alex_McAvoy的博客 - CSDN博客





2019年03月26日 13:08:13[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：23
个人分类：[LightOJ																[动态规划——线性 DP](https://blog.csdn.net/u011815404/article/category/7813170)](https://blog.csdn.net/u011815404/article/category/8037630)








> 
# Problem Description

Disky and Sooma, two of the biggest mega minds of Bangladesh went to a far country. They ate, coded and wandered around, even in their holidays. They passed several months in this way. But everything has an end. A holy person, Munsiji came into their life. Munsiji took them to derby (horse racing). Munsiji enjoyed the race, but as usual Disky and Sooma did their as usual task instead of passing some romantic moments. They were thinking- in how many ways a race can finish! Who knows, maybe this is their romance!

In a race there are n horses. You have to output the number of ways the race can finish. Note that, more than one horse may get the same position. For example, 2 horses can finish in 3 ways.

1.      Both first

2.      horse1 first and horse2 second

3.      horse2 first and horse1 second

# **Input**

Input starts with an integer T (≤ 1000), denoting the number of test cases.

Each case starts with a line containing an integer n (1 ≤ n ≤ 1000).

# Output

For each case, print the case number and the number of ways the race can finish. The result can be very large, print the result modulo 10056.

# Sample Input

**3123**

# Sample Output

**Case 1: 1Case 2: 3Case 3: 13**


题意：t 组数据，每组给出一个数 n 代表 n 匹马，两匹马之间进行赛跑有三种结局：同时到达、第一个先到第二个后到、第一个后到第二个先到，现在这 n 匹马赛跑，问有多少种情况

思路：

由于每匹马都是不同的，而且有可能名词并列，所以 i 匹马可能有 j 个排名，其中 j<=i，用 dp[i][j] 来表示有 i 匹马 j 个排名

当只有一匹马时，仅有一种可能，即：dp[1][1]=1

如果加入一匹新马的话，那么可能有两种情况：
- 新马可能在 j 个排名中，那么有：dp[i+1][j]=dp[i][j]*j
- 新马可能产生一个新的排名，那么有：dp[i+1][j+1]=dp[i][j]

那么，当有 i 匹马 j 个排名时，有：dp[i][j]=dp[i-1][j]*j+dp[i-1][j-1]*j

因此当有 i 匹马时，统计 i 匹马 j 个排名的情况即可，记得需要求模

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
#define PI acos(-1.0)
#define E 1e-9
#define INF 0x3f3f3f3f
#define LL long long
const int MOD=10056;
const int N=1000+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;
int dp[N][N];
int res[N];
void init(){//提前打表
    res[1]=dp[1][1]=1;//仅有一匹马时
    for(int i=2;i<=1000;i++){//马最多为1000匹
        for(int j=1;j<=i;j++){//马的排名
            dp[i][j]=(dp[i-1][j]*j+dp[i-1][j-1]*j)%MOD;
            res[i]=(res[i]+dp[i][j])%MOD;
        }
    }
}
int main(){
    init();

    int t;
    scanf("%d",&t);

    int Case=1;
    while(t--){
        int n;
        scanf("%d",&n);
        printf("Case %d: %d\n",Case++,res[n]);
    }
    return 0;
}
```






