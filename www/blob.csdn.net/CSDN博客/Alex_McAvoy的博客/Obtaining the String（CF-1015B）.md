# Obtaining the String（CF-1015B） - Alex_McAvoy的博客 - CSDN博客





2018年08月06日 10:49:03[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：101
个人分类：[CodeForces																[基础算法——排序](https://blog.csdn.net/u011815404/article/category/7439201)](https://blog.csdn.net/u011815404/article/category/7923180)








# Problem Description

You are given two strings s and t. Both strings have length nn and consist of lowercase Latin letters. The characters in the strings are numbered from 1 to n.

You can successively perform the following move any number of times (possibly, zero):

swap any two adjacent (neighboring) characters of ss (i.e. for any i={1,2,…,n−1} you can swap sisi and si+1).

You can't apply a move to the string t. The moves are applied to the string ss one after another.

Your task is to obtain the string t from the string s. Find any way to do it with at most 104 such moves.

You do not have to minimize the number of moves, just find any sequence of moves of length 10^4 or less to transform s into t.

# Input

The first line of the input contains one integer nn (1≤n≤50) — the length of strings s and t.

The second line of the input contains the string s consisting of n lowercase Latin letters.

The third line of the input contains the string t consisting of n lowercase Latin letters.

# Output

If it is impossible to obtain the string tt using moves, print "-1".

Otherwise in the first line print one integer k — the number of moves to transform s to t. Note that kk must be an integer number between 0 and 10^4 inclusive.

In the second line print kk integers cj (1≤cj<n), where cj means that on the jj-th move you swap characters scj and scj+1.

If you do not need to apply any moves, print a single integer 0 in the first line and either leave the second line empty or do not print it at all

# Examples

**Input**

6

abcdef

abdfec

**Output**

4

3 5 4 5

**Input**

4

abcd

accd

**Output**

-1

# Note

In the first example the string ss changes as follows: "abcdef" → "abdcef" → "abdcfe" → "abdfce" → "abdfec".

In the second example there is no way to transform the string ss into the string tt through any allowed moves.

————————————————————————————————————————————

题意：给出两个字符串，要求让串1变成串2，规则为只能交换串 1 的 ai 与 ai+1，要求输出交换次数以及交换的位置

思路：冒泡排序的思路，串1的每一个字母有一个对应位置，如果串2的字母与串1的相同，那么串1相当于串2的一种排列，即每一个字母的位置发生了改变，因此对数字的位置进行排序即可，而交换规则正好符合冒泡排序。

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
#define N 1000005
#define MOD 123
#define E 1e-6
using namespace std;
char a[N],b[N];
int num_a[27],num_b[27];
int sum[N];
int main()
{
    int n;
    while(scanf("%d",&n)!=EOF)
    {
        scanf("%s",b);
        scanf("%s",a);

        memset(sum,0,sizeof(sum));
        memset(num_a,0,sizeof(num_a));
        memset(num_b,0,sizeof(num_b));

        for(int i=0;i<n;i++)//统计字符串字母个数
        {
            num_a[a[i]-'a']++;
            num_b[b[i]-'a']++;
        }

        bool flag=false;
        for(int i=0;i<26;i++)//判断两字符串字母是否相同
        {
            if(num_a[i]!=num_b[i])
            {
                flag=true;
                cout<<-1<<endl;
                break;
            }
        }

        int cnt=0;
        if(flag)
            continue;
        else
        {
            for(int i=0;i<n;i++)
            {
                if(a[i]==b[i])
                    continue;
                else
                {
                    for(int j=i;j<n;j++)
                    {
                        if(a[i]==b[j])
                        {
                            int left=i,right=j;
                            while(left<right)
                            {
                                sum[++cnt]=right;
                                right--;
                                swap(b[right],b[right+1]);
                            }
                            break;
                        }
                    }
                }
            }
        }

        cout<<cnt<<endl;
        for(int i=1;i<=cnt;i++)
            cout<<sum[i]<<" ";
        cout<<endl;
    }

    return 0;
}
```






