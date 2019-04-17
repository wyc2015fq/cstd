# String painter（HDU-2476） - Alex_McAvoy的博客 - CSDN博客





2018年09月01日 11:27:35[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：47








# Problem Description

There are two strings A and B with equal length. Both strings are made up of lower case letters. Now you have a powerful string painter. With the help of the painter, you can change a segment of characters of a string to any other character you want. That is, after using the painter, the segment is made up of only one kind of character. Now your task is to change A to B using string painter. What’s the minimum number of operations?

# **Input**

Input contains multiple cases. Each case consists of two lines:

The first line contains string A.

The second line contains string B.

The length of both strings will not be greater than 100.

# Output

A single line contains one integer representing the answer.

# Sample Input

**zzzzzfzzzzzabcdefedcbaababababababcdcdcdcdcdcd**

# Sample Output

**67**

————————————————————————————————————————————————————

题意：给出两长度相同的字符串，每次操作可以把第一个字符串的一段区间内的所有字母都换成同一个字母，现要将第一个字符串转换成第二个，求最少执行多少次操作

思路：用 dp[i][j] 来表示在区间 i~j 内操作的最少次数，先对字符串2进行处理，假设第 i 个字符需要操作一次，则有 dp[i][j]=dp[i+1][j]+1，在区间 i~j 内，若第 k 个字符与第 i 个相同，则可以将区间 i~j 分成两部分 dp[i][j]=min(dp[i+1][k]+dp[k+1][j]);

处理完第2个字符串后，用 sum[i] 记录字符串的区间 0~i 的喷刷次数，如果字符串1的 i 位置与字符串2的 i 位置相同，那么这个位置就不需要进行处理，即 sum[i]=sum[i-1]，如果不同，就在新区间内分割开求最小值

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
int sum[N];
int dp[N][N];
int main()
{
    string str1,str2;
    while(cin>>str1>>str2)
    {
        memset(dp,0,sizeof(dp));

        int len=str1.length();
        for(int j=0;j<len;j++)
        {
            for(int i=j;i>=0;i--)
            {
                dp[i][j]=dp[i+1][j]+1;
                for(int k=i+1;k<=j;k++)
                {
                    if(str2[i]==str2[k])
                        dp[i][j]=min(dp[i][j],dp[i+1][k]+dp[k+1][j]);
                }
            }
        }

        for(int i=0;i<len;i++)
            sum[i]=dp[0][i];

        for(int i=0;i<len;i++)
        {
            if(str1[i]==str2[i])
                sum[i]=sum[i-1];
            else
                for(int j=0;j<i;j++)
                    sum[i]=min(sum[i],sum[j]+dp[j+1][i]);
        }

        printf("%d\n",sum[len-1]);
    }

    return 0;
}
```





