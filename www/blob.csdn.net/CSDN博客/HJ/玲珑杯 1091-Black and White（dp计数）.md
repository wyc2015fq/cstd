# 玲珑杯 1091 - Black and White（dp计数） - HJ - CSDN博客
2017年07月12日 14:09:57[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：245
DESCRIPTION
Constroy likes the game called Reversi. He has a long paper tape with n 
grids, where each grid should fill by one black chess or one white chess exactly. Constroy dislikes the situation with a consecutive black chesses or b
consecutive white chesses, so he intends to know how many situaions satisfy his preference.
The answer may be so large, but you only need to give the answer modulo (109+7)
.
INPUT 
The first line contains a positive integer T 
, which represents there are T test cases. The following is test cases. For each test case: The only one line contains three integers a,b and n. It is guaranteed that no more than 50 test cases satisfy n≥104. 1≤T≤103,1≤a,b,n≤106 
OUTPUT 
For each test case, output in one line, contains one integer, which represents the number of situations satisfy his preference modulo (109+7) 
. 
SAMPLE INPUT 
10 
1 1 2 
2 3 3 
4 6 5 
5 6 4 
4 5 6 
8 1 9 
9 1 8 
9 9 10 
16 16 16 
1000000 1000000 1000000
SAMPLE OUTPUT 
0 
4 
29 
16 
53 
0 
1 
1018 
65534 
235042057
题目大意：
一共有N个格子，对于这N个格子来讲，要么涂成颜色a，要么涂成颜色b，要求不能有连续的a个颜色a出现，也不能有连续的b个颜色b出现。
问有多少种分配方式。
解题思路：
统计计数问题，考虑dp，设定d[i][2]，其中：
①d[i][0]表示长度为i的格子，以a颜色结尾的情况数。
②d[i][1]表示长度为i的格子，以b颜色结尾的情况数。
则可以得到其状态转移方程： 
d[i][0]=Σd[i-j][1]（0 < j < a）
d[i][1]=Σd[i-j][0] (0 < j < b)
还需要注意的是，如果每次循环累加和的话会超时，需要维护一个d[i][0]和d[i][1]的前缀和。
```
#include<cstdio>
#include<cstring>
using namespace std;
#define maxn 1000005
#define mod 1000000007
int d[maxn][2],sum[maxn][2];
int main()
{
    int t;
    scanf("%d",&t);
    while(t--)
    {
        int a,b,n;
        scanf("%d%d%d",&a,&b,&n);
        memset(d,0,sizeof(d));
        memset(sum,0,sizeof(sum));
        d[0][0]=d[0][1]=1;
        sum[0][0]=sum[0][1]=1;
        for(int i=1;i<=n;i++)
        {
            if(i<a)
              d[i][0]=(d[i][0]+sum[i-1][1])%mod;
            else
              d[i][0]=(sum[i-1][1]-sum[i-a][1]+mod)%mod;
            if(i<b)
              d[i][1]=(d[i][1]+sum[i-1][0])%mod;
            else
              d[i][1]=(sum[i-1][0]-sum[i-b][0]+mod)%mod;
            sum[i][0]=(sum[i-1][0]+d[i][0])%mod;
            sum[i][1]=(sum[i-1][1]+d[i][1])%mod;
        }
        int ans=(d[n][0]+d[n][1])%mod;
        printf("%d\n",(ans+mod)%mod);
    }
    return 0;
}
```
