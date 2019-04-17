# Pieces（HDU-4628） - Alex_McAvoy的博客 - CSDN博客





2018年10月04日 12:02:46[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：60








# Problem Description

You heart broke into pieces.My string broke into pieces.But you will recover one day,and my string will never go back. 

Given a string s.We can erase a subsequence of it if this subsequence is palindrome in one step. We should take as few steps as possible to erase the whole sequence.How many steps do we need? 

For example, we can erase abcba from axbyczbea and get xyze in one step.

# **Input**

The first line contains integer T,denote the number of the test cases. Then T lines follows,each line contains the string s (1<= length of s <= 16).

T<=10.

# Output

For each test cases,print the answer in a line.

# Sample Input

**2aaabb**

# Sample Output

**12**

————————————————————————————————————————————————————

题意：给一个字符串，每次可以在字符串中选取任意个能组成回文串的字符（可为不连续的）去掉，单个字符亦视为回文，

思路：状压DP

字符串长度<=16，可以考虑枚举所有状态，将每个串压缩成一个状态，用0、1分别表示字符串某位置的字符去掉、保留，比较状态和子状态与相应的状态，进行状态转移取小值即可

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
#define N 20
#define MOD 10007
#define E 1e-6
#define LL long long
using namespace std;
char str[N];
int sta[N];
int dp[1<<N];
int len;
int init(int x){
    int head=0;
    memset(sta,0,sizeof(sta));

    for(int i=0;i<=len;i++)
        if(x&(1<<i))
            sta[head++]=i;
    head--;
    for(int i=0;i<head/2;i++)//从中点向两端看，判断是否是回文
        if(str[sta[i]]!=str[sta[head-i]])//若不是回文，则返回是回文的状态x的长度
            return head;
    return 1;
}
int main()
{
    int n;
    scanf("%d",&n);
    while(n--){
        memset(dp,0,sizeof(dp));
        scanf("%s",str);

        len=strlen(str);
        int allState=1<<len;
        for(int i=1;i<allState;i++){//枚举所有状态
            dp[i]=init(i);//对每一状态初始化
            for(int j=i;j>0;j=(j-1)&i)
                dp[i]=min(dp[i],dp[j]+dp[j^i]);
        }

        printf("%d\n",dp[allState-1]);
    }
	return 0;
}
```





