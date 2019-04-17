# Two-gram（CF-977B） - Alex_McAvoy的博客 - CSDN博客





2018年08月09日 17:19:53[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：77
个人分类：[基础算法——排序																[CodeForces](https://blog.csdn.net/u011815404/article/category/7923180)](https://blog.csdn.net/u011815404/article/category/7439201)








# Problem Description

Two-gram is an ordered pair (i.e. string of length two) of capital Latin letters. For example, "AZ", "AA", "ZA" — three distinct two-grams.

You are given a string s consisting of n capital Latin letters. Your task is to find any two-gram contained in the given string as a substring (i.e. two consecutive characters of the string) maximal number of times. For example, for string s = "BBAABBBA" the answer is two-gram "BB", which contained in ss three times. In other words, find any most frequent two-gram.

Note that occurrences of the two-gram can overlap with each other.

# Input

The first line of the input contains integer number nn (2≤n≤100) — the length of string s. The second line of the input contains the string s consisting of nn capital Latin letters.

# Output

Print the only line containing exactly two capital Latin letters — any two-gram contained in the given string s as a substring (i.e. two consecutive characters of the string) maximal number of times.

# Examples

**Input**

7

ABACABA

**Output**

AB

**Input**

5

ZZZAA

**Output**

ZZ

————————————————————————————————————————————

题意： 给出一长度为 n 的大写字母序列，每相邻两个字母视为一个单元，输出重复最多的单元

思路：一开始把问题想复杂了，出去抽烟的时候看到厕所的水桶，突然想到，不就是个二维桶排嘛

建一个 26*26 的桶，将相邻的两个字符存进去统计个数，最后跑一遍，输出最大的即可。

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
#define MOD 1e9+7
#define E 1e-6
typedef long long ll;
using namespace std;
int bucket[27][27];
string str;
int main()
{
    int n;
    cin>>n;
    cin>>str;

    for(int i=0;i<n-1;i++)
        bucket[str[i]-'A'][str[i+1]-'A']++;

    int maxx=0;
    int x,y;
    for(int i=0;i<26;i++)
    {
        for(int j=0;j<26;j++)
        {
            if(bucket[i][j]>maxx)
            {
                x=i;
                y=j;
                maxx=bucket[i][j];
            }
        }
    }

    cout<<char(x+'A')<<char(y+'A')<<endl;

    return 0;
}
```






