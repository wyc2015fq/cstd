# Brackets Sequence（POJ-1141） - Alex_McAvoy的博客 - CSDN博客





2018年08月18日 09:44:39[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：54








# Problem Description

Let us define a regular brackets sequence in the following way: 

1. Empty sequence is a regular sequence. 

2. If S is a regular sequence, then (S) and [S] are both regular sequences. 

3. If A and B are regular sequences, then AB is a regular sequence. 

For example, all of the following sequences of characters are regular brackets sequences: 

(), [], (()), ([]), ()[], ()[()] 

And all of the following character sequences are not: 

(, [, ), )(, ([)], ([(] 

Some sequence of characters '(', ')', '[', and ']' is given. You are to find the shortest possible regular brackets sequence, that contains the given character sequence as a subsequence. Here, a string a1 a2 ... an is called a subsequence of the string b1 b2 ... bm, if there exist such indices 1 = i1 < i2 < ... < in = m, that aj = bij for all 1 = j = n.

# **Input**

The input file contains at most 100 brackets (characters '(', ')', '[' and ']') that are situated on a single line without any other characters among them.

# Output

Write to the output file a single line that contains some regular brackets sequence that has the minimal possible length and contains the given sequence as a subsequence.

# Sample Input

**([(]**

# Sample Output

**()[()]**

————————————————————————————————————————————————————

题意：给出一串括号，要求输出添加括号最少，并使所有括号均有匹配的串

思路：用 dp[i][j] 表示从第 i 个括号到第 j 个括号添加括号的个数，用 pos[i][j] 表示 i、j 中哪个位置分开使得两部分匹配，首先判断括号是否匹配，如果匹配，则 dp[i][j]=dp[i+1][j-1]，然后判断 dp[i][j]=min(dp[i][k]+dp[k+1][j]) ，最后递归输出即可

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
#define N 1001
#define MOD 10007
#define E 1e-6
#define LL long long
using namespace std;
char str[N];
int pos[N][N];
int dp[N][N];
bool judge(int i,int j)
{
    if(str[i]=='('&&str[j]==')')
        return true;
    if(str[i]=='['&&str[j]==']')
        return true;
    return false;
}
void print(int i,int j)
{
    if(i>j)
        return;
    if(i==j)
    {
        if(str[i]=='('||str[i]==')')
            printf("()");
        else
            printf("[]");
    }
    else if(pos[i][j]==-1)
    {
        printf("%c",str[i]);
        print(i+1,j-1);
        printf("%c",str[j]);
    }
    else
    {
        print(i,pos[i][j]);
        print(pos[i][j]+1,j);
    }
}
int main()
{
    while(gets(str))
    {
        int n=strlen(str);
        if(n==0)
        {
            printf("\n");
            continue;
        }

        memset(dp,0,sizeof(dp));
        for(int i=0;i<n;i++)
            dp[i][i]=1;

        for(int len=1;len<n;len++)
        {
            for(int i=0;i<n-len;i++)
            {
                int j=len+i;
                dp[i][j]=INF;

                if(judge(i,j))
                {
                    if(dp[i][j]>dp[i+1][j-1])
                        dp[i][j]=dp[i+1][j-1];
                    pos[i][j]=-1;
                }

                for(int k=i;k<j;k++)
                {
                    int temp=dp[i][k]+dp[k+1][j];
                    if(dp[i][j]>temp)
                    {
                        dp[i][j]=temp;
                        pos[i][j]=k;
                    }
                }
            }
        }
        print(0,n-1);
        printf("\n");
    }

    return 0;
}
```





