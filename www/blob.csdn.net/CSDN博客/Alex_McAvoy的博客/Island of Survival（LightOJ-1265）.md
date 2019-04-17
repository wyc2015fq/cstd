# Island of Survival（LightOJ-1265） - Alex_McAvoy的博客 - CSDN博客





2019年04月03日 11:27:36[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：50








> 
# Problem Description

You are in a reality show, and the show is way too real that they threw into an island. Only two kinds of animals are in the island, the tigers and the deer. Though unfortunate but the truth is that, each day exactly two animals meet each other. So, the outcomes are one of the following

a)      If you and a tiger meet, the tiger will surely kill you.

b)      If a tiger and a deer meet, the tiger will eat the deer.

c)      If two deer meet, nothing happens.

d)      If you meet a deer, you may or may not kill the deer (depends on you).

e)      If two tigers meet, they will fight each other till death. So, both will be killed.

If in some day you are sure that you will not be killed, you leave the island immediately and thus win the reality show. And you can assume that two animals in each day are chosen uniformly at random from the set of living creatures in the island (including you).

Now you want to find the expected probability of you winning the game. Since in outcome (d), you can make your own decision, you want to maximize the probability.

# **Input**

Input starts with an integer T (≤ 200), denoting the number of test cases.

Each case starts with a line containing two integers t (0 ≤ t ≤ 1000) and d (0 ≤ d ≤ 1000) where t denotes the number of tigers and d denotes the number of deer.

# Output

For each case, print the case number and the expected probability. Errors less than 10-6 will be ignored.

# Sample Input

**40 01 72 00 10**

# Sample Output

**Case 1:  1Case 2:  0Case 3:  0.3333333333Case 4:  1**


题意：

一个人被扔到一个岛上，且岛上有 t 只老虎和 d 只鹿两种动物，每天只有两种动物会相遇：
- 人与老虎相遇：人死
- 鹿与老虎相遇：鹿死
- 鹿与鹿相遇：都不死亡
- 老虎与老虎相遇：都会死亡
- 人与鹿相遇：鹿可以被杀死也可以不被杀死

求存活的最大概率

思路：

由于鹿要么被老虎吃掉，要么被人杀死，要么什么都不发生，因此人的生死只与老虎有关，故只需分析老虎的数量：
- 老虎是 0 只时：人存活率为 1
- 老虎是奇数时：由于两只老虎两两相遇死亡，直到剩下一头老虎，一定会杀死人，人存活率为 0
- 老虎是偶数时：由于两只老虎两两相遇死亡，因此人的存活率就是老虎两两相遇的概率

考虑老虎是偶数的情况：

每天从人与两只老虎中选取出两只老虎的概率为：![\frac{C_t^2}{C_{t+1}^2}=\frac{t*(t-1)}{2}*\frac{2}{t*(t+1)}=\frac{t-1}{t+1}](https://private.codecogs.com/gif.latex?%5Cfrac%7BC_t%5E2%7D%7BC_%7Bt&plus;1%7D%5E2%7D%3D%5Cfrac%7Bt*%28t-1%29%7D%7B2%7D*%5Cfrac%7B2%7D%7Bt*%28t&plus;1%29%7D%3D%5Cfrac%7Bt-1%7D%7Bt&plus;1%7D)

考虑每次选出两只老虎后都会死亡，那么 t 每天都会减 2，于是最终存活概率即每天选中两只老虎的概率的乘积



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
        int n,m;
        scanf("%d%d",&n,&m);

        if(n==0)
            printf("Case %d: %d\n",Case++,1);
        else if(n%2)
            printf("Case %d: %d\n",Case++,0);
        else{
            double res=1;
            while(n){
                res*=1.0*(n-1)/(n+1);
                n-=2;
            }
            printf("Case %d: %.10lf\n",Case++,res);
        }
    }
    return 0;
}
```





