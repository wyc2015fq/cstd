# Balanced Number（HDU-3709） - Alex_McAvoy的博客 - CSDN博客





2018年09月05日 19:25:55[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：80








# Problem Description

A balanced number is a non-negative integer that can be balanced if a pivot is placed at some digit. More specifically, imagine each digit as a box with weight indicated by the digit. When a pivot is placed at some digit of the number, the distance from a digit to the pivot is the offset between it and the pivot. Then the torques of left part and right part can be calculated. It is balanced if they are the same. A balanced number must be balanced with the pivot at some of its digits. For example, 4139 is a balanced number with pivot fixed at 3. The torqueses are 4*2 + 1*1 = 9 and 9*1 = 9, for left part and right part, respectively. It's your job

to calculate the number of balanced numbers in a given range [x, y].

# **Input**

The input contains multiple test cases. The first line is the total number of cases T (0 < T ≤ 30). For each case, there are two integers separated by a space in a line, x and y. (0 ≤ x ≤ y ≤ 1018).

# Output

For each case, print the number of balanced numbers in the range [x, y] in a line.

# Sample Input

**20 937604 24324**

# Sample Output

**10897**

————————————————————————————————————————————————————

题意：给出区间 [l,r] 内平衡数的个数，平衡数是指，以某位作为支点，此位左边的 数字*距离 的和与右边相等 

思路：以 dp[pos][x][sta] 代表第pos位的数支点是x，目前力矩为sta的数的个数，对于每一位，以此位为支点计算力矩，然后判断这个数是否为平衡数

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
#define N 21
#define MOD 10007
#define E 1e-6
typedef long long LL;
using namespace std;
int bit[N];
LL dp[N][N][N*100];
LL dfs(int pos,int x,int sta,bool limit)
{
    if(pos<=0)
        return sta==0;
    if(sta<0)
        return 0;
    if(!limit&&dp[pos][x][sta]!=-1)
        return dp[pos][x][sta];

    int res=0;
    int up=limit?bit[pos]:9;
    for(int i=0;i<=up;i++)
    {
        int next=sta+i*(pos-x);
        res+=dfs(pos-1,x,next,limit&&(i==up));
    }
    if(!limit)
        dp[pos][x][sta]=res;

    return res;
}
LL solve(LL x)
{
    int len=0;
    while(x)
    {
        bit[++len]=x%10;
        x/=10;
    }

    LL sum=0;
    for(int i=1;i<=len;i++){
        sum+=dfs(len,i,0,true);
    }
    return sum-(len-1);//减去 00、000 、0000...的情况
}
int main()
{
    int t;
    scanf("%d",&t);
    while(t--)
    {
        LL left,right;
        scanf("%lld%lld",&left,&right);

        memset(dp,-1,sizeof(dp));
        printf("%lld\n",solve(right)-solve(left-1));
    }
    return 0;
}
```





