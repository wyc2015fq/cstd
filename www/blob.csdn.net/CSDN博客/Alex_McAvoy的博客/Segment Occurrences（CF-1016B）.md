# Segment Occurrences（CF-1016B） - Alex_McAvoy的博客 - CSDN博客





2018年08月13日 16:50:16[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：41
个人分类：[CodeForces																[字符串处理——模拟与暴力](https://blog.csdn.net/u011815404/article/category/8650811)](https://blog.csdn.net/u011815404/article/category/7923180)








> 
# Problem Description

You are given two strings ss and tt, both consisting only of lowercase Latin letters.

The substring s[l..r] is the string which is obtained by taking characters sl,sl+1,…,sr without changing the order.

Each of the occurrences of string aa in a string bb is a position i (1≤i≤|b|−|a|+1) such that b[i..i+|a|−1]=a (|a| is the length of string a).

You are asked qq queries: for the i-th query you are required to calculate the number of occurrences of string t in a substring s[li..ri].

# Input

The first line contains three integer numbers n, m and q (1≤n,m≤10^3, 1≤q≤10^5) — the length of string s, the length of string tt and the number of queries, respectively.

The second line is a string s (|s|=n), consisting only of lowercase Latin letters.

The third line is a string t (|t|=m), consisting only of lowercase Latin letters.

Each of the next q lines contains two integer numbers li and ri (1≤li≤ri≤n) — the arguments for the i-th query.

# Output

Print q lines — the i-th line should contain the answer to the i-th query, that is the number of occurrences of string t in a substring s[li..ri].

# Examples

**Input**

10 3 4

codeforces

for

1 3

3 10

5 6

5 7

**Output**

0

1

0

1

**Input**

15 2 3

abacabadabacaba

ba

1 15

3 4

2 14

**Output**

4

0

3

**Input**

3 5 2

aaa

baaab

1 3

1 1

**Output**

0

0


题意：给出长度为 n 的字符串1与长度为 m 的字符串2，再给出 q 组 l，r，求在串1区间 [ l , r ] 中有多少个串2

思路：一开始想用 find() 写，写了半天没写出来，干脆用前缀和来写，先求串1中第 i 个有多少个串2，然后求前缀和，最后区间相减即可

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
#define N 100001
#define MOD 1e9+7
#define E 1e-6
#define LL long long
using namespace std;
int pos[N];
int sum[N];
int main()
{
    int s,t,q;
    string str1,str2;
    cin>>s>>t>>q;
    cin>>str1>>str2;

    for(int i=0;i<s;i++)
    {
        for(int j=0;j<t;j++)
        {
            if(str1[i+j]!=str2[j])//如果不是子串则退出
                break;
            else if(j==t-1)//记录串二首字母在串一中的位置
                pos[i]=1;
        }
    }

    sum[0]=pos[0];
    for(int i=1;i<s;i++)//串1前i个字符中串2出现的次数
        sum[i]=sum[i-1]+pos[i];

    while(q--)
    {
        int l,r;
        cin>>l>>r;
        l--;
        r--;

        if(t>s)
            cout<<0<<endl;
        else if(r-l+1<t)
            cout<<0<<endl;
        else
            cout<<sum[r-t+1]-sum[l-1]<<endl;
    }
    return 0;
}
```






