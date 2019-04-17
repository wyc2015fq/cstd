# Mishka and Contest（CF-999A） - Alex_McAvoy的博客 - CSDN博客





2018年08月07日 18:46:31[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：57
个人分类：[CodeForces																[搜索——暴力搜索及打表](https://blog.csdn.net/u011815404/article/category/7925875)](https://blog.csdn.net/u011815404/article/category/7923180)








# Problem Description

Mishka started participating in a programming contest. There are nn problems in the contest. Mishka's problem-solving skill is equal to kk.

Mishka arranges all problems from the contest into a list. Because of his weird principles, Mishka only solves problems from one of the ends of the list. Every time, he chooses which end (left or right) he will solve the next problem from. Thus, each problem Mishka solves is either the leftmost or the rightmost problem in the list.

Mishka cannot solve a problem with difficulty greater than kk. When Mishka solves the problem, it disappears from the list, so the length of the list decreases by 1. Mishka stops when he is unable to solve any problem from any end of the list.

How many problems can Mishka solve?

# Input

The first line of input contains two integers nn and kk (1≤n,k≤100) — the number of problems in the contest and Mishka's problem-solving skill.

The second line of input contains nn integers a1,a2,…,an (1≤ai≤100), where ai is the difficulty of the i-th problem. The problems are given in order from the leftmost to the rightmost in the list.

# Output

Print one integer — the maximum number of problems Mishka can solve.

# Examples

**Input**

8 4

4 2 3 1 5 1 6 4

**Output**

5

**Input**

5 2

3 1 2 1 3

**Output**

0

**Input**

5 100

12 34 55 43 21

**Output**

5

————————————————————————————————————————————

题意：给出 n 个问题的难度以及可以解决问题的能力 k，每次只能从问题列表的左边或右边解决问题，如果问题难度高于 k，其后或其前的问题就无法解决，求最多可以解决多少问题。

思路：分别从右向左、从左向右枚举，注意如果 k 大于所有问题的难度，即解决的问题数为2倍的给出问题数，直接输出问题数即可。

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
#define N 10005
#define MOD 123
#define E 1e-6
using namespace std;
int a[N];
int main()
{
    int n,k;
    cin>>n>>k;
    for(int i=1;i<=n;i++)
        cin>>a[i];

    int cnt=0;
    for(int i=1;i<=n;i++)//从左向右
    {
        if(a[i]<=k)
            cnt++;
        else
            break;
    }

    for(int i=n;i>=1;i--)//从右向左
    {
        if(a[i]<=k)
            cnt++;
        else
            break;
    }

    if(cnt==2*n)//如果解决问题数为问题总数两倍，说明所有问题难度都小于k
        cout<<n<<endl;
    else
        cout<<cnt<<endl;

    return 0;
}
```






