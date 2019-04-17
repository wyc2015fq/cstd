# Stages（CF-1011A） - Alex_McAvoy的博客 - CSDN博客





2018年08月15日 14:54:52[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：47
个人分类：[CodeForces																[基础算法——贪心](https://blog.csdn.net/u011815404/article/category/7628972)](https://blog.csdn.net/u011815404/article/category/7923180)








# Problem Description

Natasha is going to fly to Mars. She needs to build a rocket, which consists of several stages in some order. Each of the stages is defined by a lowercase Latin letter. This way, the rocket can be described by the string — concatenation of letters, which correspond to the stages.

There are n stages available. The rocket must contain exactly k of them. Stages in the rocket should be ordered by their weight. So, after the stage with some letter can go only stage with a letter, which is at least two positions after in the alphabet (skipping one letter in between, or even more). For example, after letter 'c' can't go letters 'a', 'b', 'c' and 'd', but can go letters 'e', 'f', ..., 'z'.

For the rocket to fly as far as possible, its weight should be minimal. The weight of the rocket is equal to the sum of the weights of its stages. The weight of the stage is the number of its letter in the alphabet. For example, the stage 'a 'weighs one ton,' b 'weighs two tons, and' z' — 26 tons.

Build the rocket with the minimal weight or determine, that it is impossible to build a rocket at all. Each stage can be used at most once.

# Input

The first line of input contains two integers — n and k (1≤k≤n≤50) – the number of available stages and the number of stages to use in the rocket.

The second line contains string ss, which consists of exactly nn lowercase Latin letters. Each letter defines a new stage, which can be used to build the rocket. Each stage can be used at most once.

# Output

Print a single integer — the minimal total weight of the rocket or -1, if it is impossible to build the rocket at all.

# Examples

**Input**

5 3

xyabd

**Output**

29

**Input**

7 4

problem

**Output**

34

**Input**

2 2

ab

**Output**

-1

**Input**

12 1

abaabbaaabbb

**Output**

1

————————————————————————————————————————————

题意：长度为 n 的字符串，从中选 k 个，选出来的按照字典序排序且相邻字符不能在26个字母中挨着，要求不能有重复，求选出来的 k 个字符的最小值

思路：先对字符串排序，然后按要求从头到尾枚举，如果相邻元素差大于1则记录，否则跳过，当元素够 k 个时输出，如果不够则输出 -1

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
#define N 10001
#define MOD 1e9+7
#define E 1e-6
#define LL long long
using namespace std;
char str[N];
int main()
{
    int n,k;
    cin>>n>>k;
    cin>>str;

    sort(str,str+n);

    int sum=str[0]-'a'+1;
    char temp=str[0];
    int num=1;
    for(int i=1;i<n;i++)
    {
        if(num==k)
            break;
        if(str[i]-temp>1)
        {
            num++;
            temp=str[i];
            sum+=temp-'a'+1;
        }
    }

    if(num==k)
        cout<<sum<<endl;
    else
        cout<<-1<<endl;

    return 0;
}
```






