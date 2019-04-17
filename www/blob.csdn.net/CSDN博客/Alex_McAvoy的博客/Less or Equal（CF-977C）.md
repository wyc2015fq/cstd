# Less or Equal（CF-977C） - Alex_McAvoy的博客 - CSDN博客





2018年08月09日 17:15:33[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：89
个人分类：[基础算法——模拟																[CodeForces](https://blog.csdn.net/u011815404/article/category/7923180)](https://blog.csdn.net/u011815404/article/category/7812141)








# Problem Description

You are given a sequence of integers of length n and integer number k. You should print any integer number x in the range of [1;109] (i.e. 1≤x≤10^9) such that exactly k elements of given sequence are less than or equal to x.

Note that the sequence can contain equal elements.

If there is no such x, print "-1" (without quotes).

# Input

The first line of the input contains integer numbers nn and kk (1≤n≤2⋅10^5, 0≤k≤n). The second line of the input contains n integer numbers a1,a2,…,an (1≤ai≤10^9) — the sequence itself.

# Output

Print any integer number x from range [1;109] such that exactly k elements of given sequence is less or equal to x.

If there is no such x, print "-1" (without quotes).

# Examples

**Input**

7 4

3 7 5 1 10 3 20

**Output**

6

**Input**

7 2

3 7 5 1 10 3 20

**Output**

-1

————————————————————————————————————————————

题意： 给出 n 个数，求是否存在一个整数使得这个 n 个数中的 k 个数小于或等于这个整数，如果有这个整数，输出这个整数，如果没有，输出 -1

思路：一开始以为用 sort 排序后直接输出第 k 个即可，后来发现没这么简单，当 k 大于 1 或小于 n 时是可以的，但当 k 等于 1 或等于 n 时，情况就不同了，因此在 n 个数两端，根据要求的[1,1e9]，加上一个最大值与最小值，再 sort 排序即可。

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
int a[N];
int main()
{
    int n,k;
    cin>>n>>k;
    for(int i=1;i<=n;i++)
        cin>>a[i];

    a[0]=1;
    sort(a,a+n+1);

    a[n+2]=INF;
    if(a[k]==a[k+1])
        cout<<-1<<endl;
    else
        cout<<a[k]<<endl;

    return 0;
}
```






