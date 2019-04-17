# Dragon Balls（HDU-3635） - Alex_McAvoy的博客 - CSDN博客





2018年05月23日 18:56:13[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：55
个人分类：[HDU																[数据结构——并查集](https://blog.csdn.net/u011815404/article/category/8376617)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

Five hundred years later, the number of dragon balls will increase unexpectedly, so it's too difficult for Monkey King(WuKong) to gather all of the dragon balls together.

His country has N cities and there are exactly N dragon balls in the world. At first, for the ith dragon ball, the sacred dragon will puts it in the ith city. Through long years, some cities' dragon ball(s) would be transported to other cities. To save physical strength WuKong plans to take Flying Nimbus Cloud, a magical flying cloud to gather dragon balls.

Every time WuKong will collect the information of one dragon ball, he will ask you the information of that ball. You must tell him which city the ball is located and how many dragon balls are there in that city, you also need to tell him how many times the ball has been transported so far.

# **Input**

The first line of the input is a single positive integer T(0 < T <= 100).

For each case, the first line contains two integers: N and Q (2 < N <= 10000 , 2 < Q <= 10000).

Each of the following Q lines contains either a fact or a question as the follow format:

T A B : All the dragon balls which are in the same city with A have been transported to the city the Bth ball in. You can assume that the two cities are different.

Q A : WuKong want to know X (the id of the city Ath ball is in), Y (the count of balls in Xth city) and Z (the tranporting times of the Ath ball). (1 <= A, B <= N)

# Output

For each test case, output the test case number formated as sample output. Then for each query, output a line with three integers X Y Z saparated by a blank space.

# Sample Input

**2**

**3 3T 1 2T 3 2Q 23 4T 1 2Q 1T 1 3Q 1**

# Sample Output

**Case 1:2 3 0Case 2:2 2 13 3 2**


———————————————————————————————————————————————————————

题意：n个龙珠，编号为1...n，分别在编号1....n的城市中。有两种操作，T a b，把 a 所在的城市的龙珠全部放到 b 所在的城市里；Q a，查询龙珠a所在的城市，以及城市现在有几个球，以及a被转移了几次。

思路：统计类型的带权并查集，注意输出格式即可

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
#define INF 0x3f3f3f3f
#define N 1000001
#define MOD 2520
#define E 1e-12
using namespace std;
int father[N];
int value[N];
int main()
{
    int t;
    int i=1;
    scanf("%d",&t);
    while (t--)
    {
        int n,q;
        scanf("%d%d",&n,&q);

        for(int i=0;i<=n;i++)
        {
            father[i]=i;
            value[i]=1;
        }

        printf("Case %d:\n",i++);

        while(q--)
        {
            char ch[2];
            int a,b;

            scanf("%s",ch);
            if (ch[0]=='T')//转移操作
             {
                scanf("%d%d",&a,&b);
                while(a!=father[a])
                    a=father[a];
                while(b!=father[b])
                    b=father[b];
                value[b]+=value[a];//记录转移数
                value[a]=0;
                father[a]=b;
            }
            else//查找操作
            {
                scanf("%d",&a);
                b=0;
                while(a!=father[a])
                {
                    a=father[a];
                    b++;
                }
                printf("%d %d %d\n",a,value[a],b);
            }
        }
    }
    return 0;
}
```






