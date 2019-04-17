# The Fewest Coins（POJ-3260） - Alex_McAvoy的博客 - CSDN博客





2018年07月24日 23:27:34[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：95








# Problem Description

Farmer John has gone to town to buy some farm supplies. Being a very efficient man, he always pays for his goods in such a way that the smallest number of coins changes hands, i.e., the number of coins he uses to pay plus the number of coins he receives in change is minimized. Help him to determine what this minimum number is.

FJ wants to buy T (1 ≤ T ≤ 10,000) cents of supplies. The currency system has N (1 ≤ N ≤ 100) different coins, with values V1, V2, ..., VN (1 ≤ Vi ≤ 120). Farmer John is carrying C1 coins of value V1, C2 coins of value V2, ...., and CN coins of value VN (0 ≤ Ci ≤ 10,000). The shopkeeper has an unlimited supply of all the coins, and always makes change in the most efficient manner (although Farmer John must be sure to pay in a way that makes it possible to make the correct change).

# **Input**

Line 1: Two space-separated integers: N and T. 

Line 2: N space-separated integers, respectively V1, V2, ..., VN coins (V1, ...VN) 

Line 3: N space-separated integers, respectively C1, C2, ..., CN

# Output

Line 1: A line containing a single integer, the minimum number of coins involved in a payment and change-making. If it is impossible for Farmer John to pay and receive exact change, output -1.

# Sample Input

**3 705 25 505 2 1**

# Sample Output

**3**

————————————————————————————————————————————————————

题意：购买 T 元的东西，店主有 N 的不同的硬币，值为 V1，V2，....，Vn，可以无限供应，顾客有 C1 个值为 V1 的硬币，C2 个值为 V2 的硬币，......，Cn 个值为 Vn 的硬币，要求用来支付的硬币数量加上在找零中收到的硬币数量最小。

思路：

一个混合背包问题，店主硬币无穷多，属于完全背包，顾客的硬币有限，属于多重背包。

完全背包的容量为店主硬币最大币值的平方，多重背包的容量为店主硬币最大币值的平方加 T，用 f_multiple[N] 来表示支付 i 元所需最小硬币数，用 f_complete[N] 来表示找零 i 元所需最小硬币数，则买 T 元东西所需的最小硬币数为  f_multiple[T+i] + f_complete[i]

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
#define N 100001
#define MOD 123
#define E 1e-6
using namespace std;
int n,t;
int v[N],c[N];
int max_value;
int f_multiple[N];//支付i元所需最小硬币数
int f_complete[N];//找零i元所需最小硬币数
void CompletePack()
{
    int V=max_value*max_value;//背包最大容量
    for(int i=0;i<n;i++)
        for(int j=v[i];j<V;j++)
            f_complete[j]=min(f_complete[j],f_complete[j-v[i]]+1);
}
void MultiplePack()
{
    for(int i=0;i<n;i++)
    {
        int k=1;
        int sum=0;
        int V=max_value*max_value+t;//背包最大容量
        while(sum<c[i])
        {
            for(int j=V;j>=v[i]*k;j--)
                f_multiple[j]=min(f_multiple[j],f_multiple[j-v[i]*k]+k);
            sum+=k;
            if(sum+k*2>c[i])
                k=c[i]-sum;
            else
                k*=2;
        }
    }
}

int main()
{
    scanf("%d%d",&n,&t);
    for(int i=0;i<n;i++)
    {
        scanf("%d",&v[i]);//店主的币值
        max_value=max(max_value,v[i]);//寻找最大币值
    }
    for(int i=0;i<n;i++)
        scanf("%d",&c[i]);//顾客的币值

    memset(f_multiple,INF,sizeof(f_multiple));
    memset(f_complete,INF,sizeof(f_complete));
    f_multiple[0]=0;
    f_complete[0]=0;

    CompletePack();//完全背包求解
    MultiplePack();//多重背包求解

    int minn=INF;
    int V=max_value*max_value+t;
    for(int i=t;i<=V;i++)
        minn=min(minn,f_multiple[i]+f_complete[i-t]);

    if(minn==INF)
        printf("-1\n");
    else
        printf("%d\n",minn);

    return 0;
}
```





