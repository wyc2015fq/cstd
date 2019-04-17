# Remove Duplicates（CF-978A） - Alex_McAvoy的博客 - CSDN博客





2018年08月09日 17:23:21[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：49
个人分类：[CodeForces																[搜索——暴力搜索及打表](https://blog.csdn.net/u011815404/article/category/7925875)](https://blog.csdn.net/u011815404/article/category/7923180)








# Problem Description

Petya has an array aa consisting of n integers. He wants to remove duplicate (equal) elements.

Petya wants to leave only the rightmost entry (occurrence) for each element of the array. The relative order of the remaining unique elements should not be changed.

# Input

The first line contains a single integer n (1≤n≤50) — the number of elements in Petya's array.

The following line contains a sequence a1,a2,…,an (1≤ai≤1000) — the Petya's array.

# Output

In the first line print integer x — the number of elements which will be left in Petya's array after he removed the duplicates.

In the second line print x integers separated with a space — Petya's array after he removed the duplicates. For each unique element only the rightmost entry should be left.

# Examples

**Input**

6

1 5 5 1 6 1

**Output**

3

5 6 1 

**Input**

5

2 4 2 4 4

**Output**

2

2 4 

**Input**

5

6 6 6 6 6

**Output**

1

6 

————————————————————————————————————————————

题意： 给出 n 个数，输出不同的数的个数，并将每个不同的数按最后一次出现的顺序输出

思路：倒序枚举，如果出现一次，就保存在数组中，最后再输出即可

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
int a[N],b[N];
int vis[N];
int main()
{
    int n;
    cin>>n;
    for(int i=1;i<=n;i++)
        cin>>a[i];

    int cnt=0;
    for(int i=n;i>0;i--)
    {
        if(!vis[a[i]])
        {
            vis[a[i]]=1;
            b[++cnt]=a[i];
        }
    }

    cout<<cnt<<endl;
    for(int i=cnt;i>0;i--)
        cout<<b[i]<<" ";
    cout<<endl;

    return 0;
}
```






