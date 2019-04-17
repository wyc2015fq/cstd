# Magic Numbers（CF-628D） - Alex_McAvoy的博客 - CSDN博客





2019年01月17日 10:40:53[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：41
个人分类：[CodeForces																[动态规划——数位 DP](https://blog.csdn.net/u011815404/article/category/8008418)](https://blog.csdn.net/u011815404/article/category/7923180)








# Problem Description

Consider the decimal presentation of an integer. Let's call a number d-magic if digit d appears in decimal presentation of the number on even positions and nowhere else.

For example, the numbers 1727374, 17, 1 are 7-magic but 77, 7, 123, 34, 71 are not 7-magic. On the other hand the number 7 is 0-magic, 123 is 2-magic, 34 is 4-magic and 71 is 1-magic.

Find the number of d-magic numbers in the segment [a, b] that are multiple of m. Because the answer can be very huge you should only find its value modulo 109 + 7 (so you should find the remainder after dividing by 109 + 7).

# Input

The first line contains two integers m, d (1 ≤ m ≤ 2000, 0 ≤ d ≤ 9) — the parameters from the problem statement.

The second line contains positive integer a in decimal presentation (without leading zeroes).

The third line contains positive integer b in decimal presentation (without leading zeroes).

It is guaranteed that a ≤ b, the number of digits in a and b are the same and don't exceed 2000.

# Output

Print the only integer a — the remainder after dividing by 109 + 7 of the number of d-magic numbers in segment [a, b] that are multiple of m.

# Examples

**Input**

2 6

10

99

**Output**

8

**Input**

2 0

1

9

**Output**

4

**Input**

19 7

1000

9999

**Output**

6

————————————————————————————————————————————

题意：从左向右，偶数位置为 n，奇数位置不为 n 的一类数称为 n-magic，先给出 m、d 两个数，再给出边界 [a,b]，要求在边界范围内找出所有可以被 m 整除的 d-magic 的个数

思路：数位dp，在 [a,b] 范围内求有一个满足条件的数，即：数是 m 的倍数，从左向右的偶数位一定是 k，奇数位一定不是 k

由于取模运算是可加的，因此高位的余数*10+低位再取模不影响结果，所以数位dp递归到最后一位就可得到这个数是不是 m 的倍数

由于数位dp一般是求 (a,b] 范围内，而此题是要求 [a,b] 范围内，因此要额外判断 a 是否满足条件

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
#define E 1e-6
#define MOD 1000000007
#define INF 0x3f3f3f3f
#define N 2001
#define LL long long
using namespace std;
int m,d;
char str1[N],str2[N];
LL bit[N];
LL dp[N][N];
LL dfs(int pos,LL sum,bool limit,int len){
    if(pos==-1)
        return sum==0;

    if(!limit&&dp[pos][sum]!=-1)
        return dp[pos][sum];

    LL res=0;
    int up=limit?bit[pos]:9;
    for(int i=0;i<=up;i++){
        if( (len-pos)%2==0 && i!=d )//从左向右的偶数位i不等于d
            continue;
        if( (len-pos)%2==1 && i==d )//从左向右的奇数位i等于d
            continue;

        res=(res%MOD+dfs(pos-1,(sum*10+i)%m,limit&&(i==bit[pos]),len)%MOD)%MOD;
    }
    return limit?res:dp[pos][sum]=res;
}
LL solve(char *str){//分解数位
    int len=strlen(str);
    for(int i=0;i<len;i++)
        bit[i]=str[len-i-1]-'0';
    return dfs(len-1,0,true,len);
}
bool check(){
    int len=strlen(str1);
    LL sum=0;
    for(int i=0;i<len;i++){
        sum=(sum*10+str1[i]-'0')%m;

        if( (i+1)%2==0 && str1[i]-'0'!=d )
            return false;
        if( (i+1)%2==1  && str1[i]-'0'==d )
            return false;
    }

    if(sum==0)
        return true;
    else
        return false;
}
int main()
{
    while(scanf("%d%d",&m,&d)!=EOF&&(m+d)){
        scanf("%s%s",str1,str2);

        memset(dp,-1,sizeof(dp));
        LL res=solve(str2)-solve(str1);

        if(check())//判断a是否合法
            res++;

        res=(res+MOD)%MOD;
        printf("%lld\n",res);
    }
    return 0;
}
```






