# Riddle（2018 CCPC (秦皇岛站) I 题） - Alex_McAvoy的博客 - CSDN博客





2019年01月02日 13:15:41[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：111








# Problem Description

![](https://img-blog.csdnimg.cn/2019010213112867.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

# **Input**

![](https://img-blog.csdnimg.cn/2019010213122179.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

# Output

![](https://img-blog.csdnimg.cn/20190102131250318.png)

# Sample Input

**331 1 151 1 2 2 3101 2 3 4 5 6 7 8 9 10**

# Sample Output

**715127**

# **Sample Explanation**

**![](https://img-blog.csdnimg.cn/20190102131511853.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)**

————————————————————————————————————————————————————

题意：有 t 组数据，对于每组数据，给出 n 个数字，最多有 n 个玩具，其中每个数字有两种含义，要么是一个玩具的重量，要么是一个袋的重量，袋子的重量表示袋子中的玩具的重量，其重量为任意个玩具的重量之和，问最多有多少种方案

思路：

对于不超过 15 个数字的 n 个数字，每个数字有两种状态，即要么是表单个玩具重量，要么是表袋内部分玩具的总重，根据前面数字所代表的状态很容易判断后面数字代表的状态，满足无后效性原则，因此很容易想到可以用状压 DP 来解决。

但袋子中玩具的个数无法确定，因此无法用简单的 1、0 来表示是玩具还是袋子，但由于袋子的重量是由袋子中玩具的重量相加得来，因此可用 1 来表示考虑当前数字，0 表示不考虑当前数字，通过对数字的考虑来进行组合，从而判断袋子的重量是否合理，例如：101 表示考虑第 1、3 数字时的合法方案数，即第 1、3 个数字可以组成一个袋子，用 f[101] 即可表示组成袋子的方案数，再用 dp[i] 来表示合法方案数。

这样一来，就有 1<<n  种状态，考虑袋子 i 去枚举状态 j ，由于袋子 i 中的物品必定属于袋子，因此不能与状态 j 有冲突，故有 i&j=0，以此来进行状态转移，故有：dp[i|j]+=dp[j]*f[i]

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
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-6
#define MOD 16007
#define INF 0x3f3f3f3f
#define N 16
#define LL long long
using namespace std;
int a[N];
int f[1<<N];//组成袋子的合法方案数
int dp[1<<N];//合法方案数
int weight[1<<N];//第i种状态的重量
int main()
{
    int t;
    scanf("%d",&t);
    while(t--){
        int n;
        scanf("%d",&n);
        for(int i=1;i<=n;i++)
            scanf("%d",&a[i]);

        for(int i=0;i<(1<<n);i++){
            weight[i]=0;
            f[i]=0;
            dp[i]=0;
        }

        for(int i=1;i<=n;i++)//n位数字
            for(int j=1;j<(1<<n);j++)//2^n种状态
                if( 1<<(i-1) & j )//若第i位是1
                    weight[j]+=a[i];//记录第j个状态的重量

        for(int i=1;i<=n;i++)//n位数字
            for(int j=1;j<(1<<n);j++)//2^n种状态
                if( 1<<(i-1) & j )//若第i位是1
        if(weight[j]-a[i]==a[i])//如果第j个状态的重量减去第i个物品的重量等于第i个物品的重量说明选择第j个状态是一个合法的袋子
                f[j]++;

        for(int i=1;i<(1<<n);i++){//包裹2^n种状态
            int k=(1<<n)-1-i;//与i相斥的状态
            for(int j=k;;j=(j-1)&k){//选物品的状态且其不能选为包裹
                dp[i|j]+=dp[j]*f[i];
                if(j==0)
                    break;
            }
        }
        printf("%d\n",dp[(1<<n)-1]);
    }
    return 0;
}
```





