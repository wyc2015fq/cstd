# Dollar Dayz（POJ-3181） - Alex_McAvoy的博客 - CSDN博客





2018年07月19日 22:20:57[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：46








# Problem Description

Farmer John goes to Dollar Days at The Cow Store and discovers an unlimited number of tools on sale. During his first visit, the tools are selling variously for $1, $2, and $3. Farmer John has exactly $5 to spend. He can buy 5 tools at $1 each or 1 tool at $3 and an additional 1 tool at $2. Of course, there are other combinations for a total of 5 different ways FJ can spend all his money on tools. Here they are: 

        1 @ US$3 + 1 @ US$2

        1 @ US$3 + 2 @ US$1

        1 @ US$2 + 3 @ US$1

        2 @ US$2 + 1 @ US$1

        5 @ US$1

Write a program than will compute the number of ways FJ can spend N dollars (1 <= N <= 1000) at The Cow Store for tools on sale with a cost of $1..$K (1 <= K <= 100).

# **Input**

A single line with two space-separated integers: N and K.

# Output

A single line with a single integer that is the number of unique ways FJ can spend his money.

# Sample Input

**5 3**

# Sample Output

**5**

————————————————————————————————————————————————————

题意：用 n 美元买工具，工具价格从 1 到 k 元，求有多少种买法

思路：完全背包问题，WA 后死活找不到错误所在，看了题解，才明白忽略了数据范围，当 n 为 1000，k=100 时，结果是 32 位数，超过了 long long，需要用高精度算法模拟加法进行优化。  

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
#define N 10001
#define MOD 123
#define E 1e-6
using namespace std;
int f[N][100];
int add(int a[],int b[])//高精度加法
{
    for(int i=0;i<100;i++)
    {
        b[i]+=a[i];
        if(b[i]>9)
        {
            b[i+1]++;
            b[i]-=10;
        }
    }
    return *b;
}
int main()
{
    int n,k;

    scanf("%d%d",&n,&k);

    /*动规方程：f[j]=f[j]+f[j-i]*/
    f[0][0]=1;
    for(int i=1;i<=k;i++)
        for(int j=i;j<=n;j++)
            *f[j]=add(f[j-i],f[j]);

    int i=100;
    while(i)//寻找最高位
    {
        if(f[n][i])
            break;
        i--;
    }

    while(i>=0)//倒序输出
    {
        printf("%d",f[n][i]);
        i--;
    }

    printf("\n");

	return 0;
}
```





