# Best Cow Line（POJ-3617） - Alex_McAvoy的博客 - CSDN博客





2018年06月09日 22:37:15[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：62
个人分类：[POJ																[基础算法——贪心](https://blog.csdn.net/u011815404/article/category/7628972)](https://blog.csdn.net/u011815404/article/category/7504500)








# Problem Description

FJ is about to take his N (1 ≤ N ≤ 2,000) cows to the annual"Farmer of the Year" competition. In this contest every farmer arranges his cows in a line and herds them past the judges.

The contest organizers adopted a new registration scheme this year: simply register the initial letter of every cow in the order they will appear (i.e., If FJ takes Bessie, Sylvia, and Dora in that order he just registers BSD). After the registration phase ends, every group is judged in increasing lexicographic order according to the string of the initials of the cows' names.

FJ is very busy this year and has to hurry back to his farm, so he wants to be judged as early as possible. He decides to rearrange his cows, who have already lined up, before registering them.

FJ marks a location for a new line of the competing cows. He then proceeds to marshal the cows from the old line to the new one by repeatedly sending either the first or last cow in the (remainder of the) original line to the end of the new line. When he's finished, FJ takes his cows for registration in this new order.

Given the initial order of his cows, determine the least lexicographic string of initials he can make this way.


# **Input**

Line 1: A single integer: N

Lines 2..N+1: Line i+1 contains a single initial ('A'..'Z') of the cow in the ith position in the original line

# Output

The least lexicographic string he can make. Every line (except perhaps the last one) contains the initials of 80 cows ('A'..'Z') in the new line.

# Sample Input

**6**

**ACDBCB**

# Sample Output

**ABCBCD**

———————————————————————————————————————————————————————

题意：给出长度为n的字符串，要构建一个新串，有两种操作，要么将原来第一个字符加到新字符串尾部，要么将原来最后一个字符加到新串尾部，要求最后得到的字符串的字典序最小。注意字符串满80个字母后视为新串。

思路

一开始没读懂题，不知道什么是字典序，后来查了一下才知道字典序是指从前到后比较两个字符串大小的方法，即：首先比较第一个字符大小，如果不同则第一个字符小的字符串更小，如果相同则比较第二个……以此类推。

明白后发现原来是水题一枚，贪心思想反复比较前后字符串大小输出即可。

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
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 2001
#define MOD 123
#define E 1e-6
using namespace std;
char str[N];
int main()
{
    int n;
    while(scanf("%d",&n)!=EOF)
    {
        for(int i=0;i<n;i++)
            cin>>str[i];

        int left=0,right=n-1;
        int cnt=0;
        while(left<=right)
        {
            bool flag=false;
            for(int i=0;i+left<=right;i++)
            {
                if(str[i+left]<str[right-i])
                {
                    flag=true;
                    break;
                }
                else if(str[i+left]>str[right-i])
                {
                    flag=false;
                    break;
                }
            }
            if(flag)
                cout<<str[left++];
            else
                cout<<str[right--];
                cnt++;
            if(cnt==80)
            {
                cout<<endl;
                cnt=0;
            }
        }
    }
    return 0;
}
```






