# Halloween Costumes（LightOJ-1422） - Alex_McAvoy的博客 - CSDN博客





2018年08月28日 19:19:49[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：62








> 
# Problem Description

Gappu has a very busy weekend ahead of him. Because, next weekend is Halloween, and he is planning to attend as many parties as he can. Since it's Halloween, these parties are all costume parties, Gappu always selects his costumes in such a way that it blends with his friends, that is, when he is attending the party, arranged by his comic-book-fan friends, he will go with the costume of Superman, but when the party is arranged contest-buddies, he would go with the costume of 'Chinese Postman'.

Since he is going to attend a number of parties on the Halloween night, and wear costumes accordingly, he will be changing his costumes a number of times. So, to make things a little easier, he may put on costumes one over another (that is he may wear the uniform for the postman, over the superman costume). Before each party he can take off some of the costumes, or wear a new one. That is, if he is wearing the Postman uniform over the Superman costume, and wants to go to a party in Superman costume, he can take off the Postman uniform, or he can wear a new Superman uniform. But, keep in mind that, Gappu doesn't like to wear dresses without cleaning them first, so, after taking off the Postman uniform, he cannot use that again in the Halloween night, if he needs the Postman costume again, he will have to use a new one. He can take off any number of costumes, and if he takes off k of the costumes, that will be the last k ones (e.g. if he wears costume A before costume B, to take off A, first he has to remove B).

Given the parties and the costumes, find the minimum number of costumes Gappu will need in the Halloween night.

# **Input**

Input starts with an integer T (≤ 200), denoting the number of test cases.

Each case starts with a line containing an integer N (1 ≤ N ≤ 100) denoting the number of parties. Next line contains N integers, where the ith integer ci (1 ≤ ci ≤ 100) denotes the costume he will be wearing in party i. He will attend party 1 first, then party 2, and so on.

# Output

For each case, print the case number and the minimum number of required costumes.

# Sample Input

**2**

**4**

**1 2 1 2**

**7**

**1 2 1 1 3 2 1**

# Sample Output

**Case 1: 3Case 2: 4**


题意：给出 n 天分别要穿的衣服的编号，可以套着穿，但一旦脱下衣服来就不能再穿了，求这 n 天最少准备的衣服件数

思路：

用 dp[i][j] 来表示第 i 天到第 j 天要穿的最少衣服，考虑第 j 天穿不穿，若穿的话则有 dp[i][j]=dp[i][j-1]+1，若不穿的话则去枚举区间 [i,j]，其中有一天 k ，使得第 j 天和第 k 天穿的衣服相同，那么将 k+1 到 j-1 的衣服套着穿后全部脱掉，故有：dp[i][j]=dp[i][k]+dp[k+1][j-1]；

最后再取所有情况最小值即可

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
const int MOD=10007;
const int N=1000+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;
int a[N];
int dp[N][N];
int main(){
    int t;
    scanf("%d",&t);

    int Case=1;
    while(t--){
        memset(dp,0,sizeof(dp));

        int n;
        scanf("%d",&n);
        for(int i=1;i<=n;i++){
            scanf("%d",&a[i]);
            dp[i][i]=1;//第一天一定穿一件
        }

        for(int j=2;j<=n;j++){//结束天数
            for(int i=1;i<j;i++){//开始天数
                dp[i][j]=dp[i][j-1]+1;//第j天穿
                for(int k=i;k<j;k++){//第j天不穿
                    if(a[k]==a[j])
                        dp[i][j]=min(dp[i][j],dp[i][k]+dp[k+1][j-1]);
                }
            }
        }
        printf("Case %d: %d\n",Case++,dp[1][n]);
    }
    return 0;
}
```





