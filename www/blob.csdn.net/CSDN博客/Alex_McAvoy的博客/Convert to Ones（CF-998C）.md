# Convert to Ones（CF-998C） - Alex_McAvoy的博客 - CSDN博客





2018年08月26日 16:28:20[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：60
个人分类：[CodeForces																[基础算法——贪心](https://blog.csdn.net/u011815404/article/category/7628972)](https://blog.csdn.net/u011815404/article/category/7923180)








# Problem Description

You've got a string a1,a2,…,an, consisting of zeros and ones.

Let's call a sequence of consecutive elements ai,ai + 1,…, aj (1≤ i≤ j≤ n) a substring of string a.

You can apply the following operations any number of times:

Choose some substring of string aa (for example, you can choose entire string) and reverse it, paying x coins for it (for example, «0101101» → «0111001»);

Choose some substring of string aa (for example, you can choose entire string or just one symbol) and replace each symbol to the opposite one (zeros are replaced by ones, and ones — by zeros), paying y coins for it (for example, «0101101» → «0110001»).

You can apply these operations in any order. It is allowed to apply the operations multiple times to the same substring.

What is the minimum number of coins you need to spend to get a string consisting only of ones?

# Input

The first line of input contains integers n, x and y (1 ≤ n ≤ 300000,0≤x,y≤109) — length of the string, cost of the first operation (substring reverse) and cost of the second operation (inverting all elements of substring).

The second line contains the string a of length n, consisting of zeros and ones.

# Output

Print a single integer — the minimum total cost of operations you need to spend to get a string consisting only of ones. Print 0, if you do not need to perform any operations.

# Examples

**Input**

5 1 10

01000

**Output**

11

**Input**

5 10 1

01000

**Output**

2

**Input**

7 2 3

1111111

**Output**

0

————————————————————————————————————————————

题意：给出一个长度为 n 的二进制串，有两种操作：将某一段反序，花费为 x；将某一段取反，花费为 y，求将这个串全都变为1的最小花费

思路：假设 0 的段数为cnt，则：如果 x<=y，那么就进行 cnt-1 次操作 1 和一次操作 2 ；如果 x>y，那么就进行 cnt 次操作 2 

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
int main()
{
    int n,x,y;
    string str;
    cin>>n>>x>>y;
    cin>>str;

    LL cnt=0;
    for(LL i=0;i<n;i++)
        if(str[i]=='0'&&str[i+1]=='1')
            cnt++;
    if(str[n-1]=='0')
        cnt++;

    LL minn;

    if(x<y)
        minn=x*(cnt-1)+y;
    else
        minn=y*cnt;

    if(cnt==0)
        minn=0;

    cout<<minn<<endl;

    return 0;
}
```






