# Roman Digits（CF-998D） - Alex_McAvoy的博客 - CSDN博客





2018年08月26日 16:35:40[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：79








# Problem Description

Let's introduce a number system which is based on a roman digits. There are digits I, V, X, L which correspond to the numbers 1, 5, 10 and 50 respectively. The use of other roman digits is not allowed.

Numbers in this system are written as a sequence of one or more digits. We define the value of the sequence simply as the sum of digits in it.

For example, the number XXXV evaluates to 35 and the number IXI — to 12.

Pay attention to the difference to the traditional roman system — in our system any sequence of digits is valid, moreover the order of digits doesn't matter, for example IX means 11, not 9.

One can notice that this system is ambiguous, and some numbers can be written in many different ways. Your goal is to determine how many distinct integers can be represented by exactly nn roman digits I, V, X, L.

# Input

The only line of the input file contains a single integer n (1≤n≤109) — the number of roman digits to use.

# Output

Output a single integer — the number of distinct integers which can be represented using n roman digits exactly.

# Examples

**Input**

1

**Output**

4

**Input**

2

**Output**

10

**Input**

10

**Output**

244

————————————————————————————————————————————

题意：有 1,5,10,50 四种数字，求用 n 个数字，能构成多少个不同的数字

思路：暴力打表找规律

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
#define N 1000001
#define MOD 1e9+7
#define E 1e-6
#define LL long long
using namespace std;
LL a[N]={0,4,10,20,35,56,83,116,155,198,244,292};
int main()
{
    LL n;
    cin>>n;

    if(n<=11)
        cout<<a[n];
    else
        cout<<a[11]+(n-11)*49;

    return 0;
}
```





