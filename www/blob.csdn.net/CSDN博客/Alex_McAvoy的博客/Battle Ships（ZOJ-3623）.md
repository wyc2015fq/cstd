# Battle Ships（ZOJ-3623） - Alex_McAvoy的博客 - CSDN博客





2019年03月16日 15:33:29[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：65








> 
# Problem Description

Battle Ships is a new game which is similar to Star Craft. In this game, the enemy builds a defense tower, which has L longevity. The player has a military factory, which can produce N kinds of battle ships. The factory takes ti seconds to produce the i-th battle ship and this battle ship can make the tower loss li longevity every second when it has been produced. If the longevity of the tower lower than or equal to 0, the player wins. Notice that at each time, the factory can choose only one kind of battle ships to produce or do nothing. And producing more than one battle ships of the same kind is acceptable.

Your job is to find out the minimum time the player should spend to win the game.

# **Input**

There are multiple test cases. 

The first line of each case contains two integers N(1 ≤ N ≤ 30) and L(1 ≤ L ≤ 330), N is the number of the kinds of Battle Ships, L is the longevity of the Defense Tower. Then the following N lines, each line contains two integers t i(1 ≤ t i ≤ 20) and li(1 ≤ li ≤ 330) indicating the produce time and the lethality of the i-th kind Battle Ships.

# Output

Output one line for each test case. An integer indicating the minimum time the player should spend to win the game.

# Sample Input

**1 11 12 101 12 53 1001 103 2010 100**

# Sample Output

**245**


题意：每组数据给出一个防御塔的血量 l，然后给出 n 种战舰的生产时间与每秒对防御塔造成的伤害，当防御塔的血量小于等于 0 时，游戏获胜，问最短的游戏获胜时间是多少

思路：

将防御塔血量 l 视为背包容量，n 种战舰可以无限生产，将战舰的生产时间视为价值，造成的伤害视为重量，那么问题就转为了完全背包问题

设 dp[i] 为在时间 i 内造成的伤害，那么则有 dp[i]=max(dp[i],dp[i-time[j]]+(i-time[j])*value[j])，其中 dp[i-time[j]]+(i-time[j])*value[j] 表示为在时间 i 前增加 time[j] 的时间来制造第 j 种船，然后在接下来的 i-time[j] 的时间内每秒造成 value[j] 的伤害

最后只要枚举 i，找出第一个大于等于 l 的值即可

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
int dp[N];
int times[N];
int value[N];
int main(){
    int n,l;
    while(scanf("%d%d",&n,&l)!=EOF&&(n+l)){
        memset(dp,0,sizeof(dp));

        for(int i=1;i<=n;i++)
            scanf("%d%d",×[i],&value[i]);

        for(int j=1;j<=n;j++){//枚举战舰种类
            for(int i=times[j];i<=times[j]+l;i++){//枚举时间
                dp[i]=max(dp[i],dp[i-times[j]]+(i-times[j])*value[j]);
            }
        }

        int minn=INF;
        for(int i=0;i<=1000;i++){//枚举时间
            if(dp[i]>=l){
                minn=i;
                break;
            }
        }
        printf("%d\n",minn);

    }
    return 0;
}
```





