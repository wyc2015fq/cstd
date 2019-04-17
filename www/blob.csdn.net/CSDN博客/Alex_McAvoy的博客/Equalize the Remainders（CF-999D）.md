# Equalize the Remainders（CF-999D） - Alex_McAvoy的博客 - CSDN博客





2018年08月07日 19:42:36[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：80
个人分类：[CodeForces																[STL 的使用——容器与迭代器](https://blog.csdn.net/u011815404/article/category/8793969)](https://blog.csdn.net/u011815404/article/category/7923180)








> 
# Problem Description

You are given an array consisting of n integers a1,a2,…,an, and a positive integer m. It is guaranteed that m is a divisor of n.

In a single move, you can choose any position i between 1 and nn and increase aiai by 1.

Let's calculate crcr (0≤r≤m−1) — the number of elements having remainder rr when divided by m. In other words, for each remainder, let's find the number of corresponding elements in aa with that remainder.

Your task is to change the array in such a way that c0=c1=⋯=cm−1=n/m.

Find the minimum number of moves to satisfy the above requirement.

# Input

The first line of input contains two integers n and m (1≤n≤2⋅10^5,1≤m≤n). It is guaranteed that mm is a divisor of n.

The second line of input contains n integers a1,a2,…,an (0≤ai≤10^9), the elements of the array.

# Output

In the first line, print a single integer — the minimum number of moves required to satisfy the following condition: for each remainder from 0 to m−1, the number of elements of the array having this remainder equals n/m.

In the second line, print any array satisfying the condition and can be obtained from the given array with the minimum number of moves. The values of the elements of the resulting array must not exceed 10^18.

# Examples

**Input**

6 3

3 2 0 6 10 12

**Output**

3

3 2 0 7 10 14 

**Input**

4 2

0 1 2 3

**Output**

0

0 1 2 3 


题意：给出 n 个数字，对 m 取余有 m 种情况，使得每种情况的个数都为 n/m 个，求最少需要操作多少次？操作：把某个数字+1。输出最少操作次数，和操作后的序列（可以输出任意一种）。

思路：对于余数 x 来说，若余数为 x 的数不够 n/m 个，那么就让他不变，若达到 n/m 个，就让他变为距离他最近的没凑够的余数，可用一数组 C 来维护余数为 i 的数的个数，定义一个 set<int> 用于维护没凑够的余数。因此可以对每一个数字求他的余数 d，然后找到 set 中第一个大于等于 d 的数字 x，然后 C[x]++，等到 C[x]=n/m 就将 x 从 set 中删除。

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
using namespace std;
long long n,m;
int a[N],c[N];
int ans;
set<int> s;
int main()
{
    cin>>n>>m;

    for(int i=0;i<m;i++)
        s.insert(i);

    for(int i=0;i<n;i++)
    {
        cin>>a[i];
        int d=a[i]%m;
        int x;

        if(d>*s.rbegin())//找到d能加到的最近的余数x
            x=*s.begin();//d比最大的还大
        else
            x=*s.lower_bound(d);

        if(++c[x]==n/m)//余数为x的数凑够了
            s.erase(x);

        ans+=(x-d+m)%m;
        a[i]+=(x-d+m)%m;
    }

    cout<<ans<<endl;
    for(int i=0;i<n;i++)
        cout<<a[i]<<" ";
    cout<<endl;

    return 0;
}
```






