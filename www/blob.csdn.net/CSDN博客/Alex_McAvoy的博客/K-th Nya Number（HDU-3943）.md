# K-th Nya Number（HDU-3943） - Alex_McAvoy的博客 - CSDN博客





2018年09月12日 21:05:39[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：80
个人分类：[HDU																[动态规划——数位 DP](https://blog.csdn.net/u011815404/article/category/8008418)](https://blog.csdn.net/u011815404/article/category/7923257)








# Problem Description

Arcueid likes nya number very much. A nya number is the number which has exactly X fours and Y sevens(If X=2 and Y=3 , 172441277 and 47770142 are nya numbers.But 14777 is not a nya number ,because it has only 1 four). Now, Arcueid wants to know the K-th nya number which is greater than P and not greater than Q.

# **Input**

The first line contains a positive integer T (T<=100), indicates there are T test cases. 

The second line contains 4 non-negative integers: P,Q,X and Y separated by spaces. 

( 0<=X+Y<=20 , 0< P<=Q <2^63) 

The third line contains an integer N(1<=N<=100). 

Then here comes N queries. 

Each of them contains an integer K_i (0<K_i <2^63).

# Output

For each test case, display its case number and then print N lines. 

For each query, output a line contains an integer number, representing the K_i-th nya number in (P,Q]. 

If there is no such number,please output "Nya!"(without the quotes). 

# Sample Input

**138 400 1 11012345678910**

# Sample Output

**Case #1:4774147174247274347374Nya!Nya!**

————————————————————————————————————————————————————

题意：给定 x 和 y 的值，对于数位 4 出现 x 次，7 出现 y 次的数字称为 nya 数，给定区间 [p,q]，查询在区间上找到的第 k 个nya 数，如果不存在则输出 Nya！

思路：数位DP+二分求值

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
#include<ctime>
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 31
#define MOD 10007
#define E 1e-6
typedef long long LL;
using namespace std;
LL p,q,x,y;
int bit[N];
LL dp[N][N][N];
LL dfs(int pos,int sta1,int sta2,bool limit)
{
    if(pos<=0)
        return sta1==x&&sta2==y;
    if(sta1>x||sta2>y)
        return 0;


    if(!limit&&dp[pos][sta1][sta2]!=-1)
        return dp[pos][sta1][sta2];

    LL res=0;
    int up=limit?bit[pos]:9;
    for(int i=0;i<=up;i++)
    {
        if(i==4)
            res+=dfs(pos-1,sta1+1,sta2,limit&&i==up);
        else if(i==7)
            res+=dfs(pos-1,sta1,sta2+1,limit&&i==up);
        else
            res+=dfs(pos-1,sta1,sta2,limit&&i==up);

    }

    if(!limit)
        dp[pos][sta1][sta2]=res;

    return res;
}
LL solve(LL x)
{
    int pos=0;
    while(x)
    {
        bit[++pos]=x%10;
        x/=10;
    }

    return dfs(pos,0,0,true);
}
LL Find(LL left,LL right,LL m)
{
    LL l=p+1,r=q;
    while(l<r)
    {
        LL mid=(l+r)/2;
        if(solve(mid)>=left+m)
            r=mid;
        else
            l=mid+1;
    }
    return l;
}
int main()
{
    int t;
    scanf("%d",&t);

    int Case=1;
    while(t--)
    {
        printf("Case #%d:\n",Case++);
        scanf("%lld%lld%lld%lld",&p,&q,&x,&y);
        memset(dp,-1,sizeof(dp));

        LL left=solve(p);
        LL right=solve(q);

        int n;
        scanf("%d",&n);
        while(n--)
        {
            LL m;
            scanf("%lld",&m);

            LL res;
            if(right-left<m)
                printf("Nya!\n");
            else
            {
                res=Find(left,right,m);
                printf("%lld\n",res);
            }
        }
    }
    return 0;
}
```






