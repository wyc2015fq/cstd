# Throwing Balls into the Baskets（LightOJ-1317） - Alex_McAvoy的博客 - CSDN博客





2019年04月03日 10:50:01[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：189








> 
# Problem Description

You probably have played the game "Throwing Balls into the Basket". It is a simple game. You have to throw a ball into a basket from a certain distance. One day we (the AIUB ACMMER) were playing the game. But it was slightly different from the main game. In our game we were N people trying to throw balls into M identical Baskets. At each turn we all were selecting a basket and trying to throw a ball into it. After the game we saw exactly S balls were successful. Now you will be given the value of N and M. For each player probability of throwing a ball into any basket successfully is P. Assume that there are infinitely many balls and the probability of choosing a basket by any player is 1/M. If multiple people choose a common basket and throw their ball, you can assume that their balls will not conflict, and the probability remains same for getting inside a basket. You have to find the expected number of balls entered into the baskets after K turns.

# **Input**

Input starts with an integer T (≤ 100), denoting the number of test cases.

Each case starts with a line containing three integers N (1 ≤ N ≤ 16), M (1 ≤ M ≤ 100) and K (0 ≤ K ≤ 100) and a real number P (0 ≤ P ≤ 1). P contains at most three places after the decimal point.

# Output

For each case, print the case number and the expected number of balls. Errors less than 10-6 will be ignored.

# Sample Input

**21 1 1 0.51 1 2 0.5**

# Sample Output

**Case 1:  0.5Case 2:  1.000000**


题意：有无限个球，n 个人投 m 个篮筐，选择每个篮筐的概率是 1/m，每次投进的概率都是 p，问进行 k 轮后的期望是多少

思路：

只有一轮时，期望是 n*p

当有 k 轮时，期望是 n*p*k

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
        int n,m,k;
        double p;
        scanf("%d%d%d%lf",&n,&m,&k,&p);

        printf("Case %d: %.6lf\n",Case++,n*p*k);
    }
    return 0;
}
```





