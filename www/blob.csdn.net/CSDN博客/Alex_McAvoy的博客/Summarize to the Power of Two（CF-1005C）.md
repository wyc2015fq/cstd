# Summarize to the Power of Two（CF-1005C） - Alex_McAvoy的博客 - CSDN博客





2018年08月06日 07:54:26[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：59
个人分类：[CodeForces																[STL 的使用——容器与迭代器](https://blog.csdn.net/u011815404/article/category/8793969)](https://blog.csdn.net/u011815404/article/category/7923180)








> 
# Problem Description

A sequence a1,a2,…,an is called good if, for each element aiai, there exists an element ajaj (i≠j) such that ai+aj is a power of two (that is, 2d for some non-negative integer dd).

For example, the following sequences are good:

[5,3,11] (for example, for a1=5 we can choose a2=3. Note that their sum is a power of two. Similarly, such an element can be found for a2 and a3),

[1,1,1,1023]

[7,39,89,25,89],

[][].

Note that, by definition, an empty sequence (with a length of 0) is good.

For example, the following sequences are not good:

[16] (for a1=16, it is impossible to find another element aj such that their sum is a power of two),

[4,16] (for a1=4, it is impossible to find another element aj such that their sum is a power of two),

[1,3,2,8,8,8] (for a3=2, it is impossible to find another element aj such that their sum is a power of two).

You are given a sequence a1,a2,…,an. What is the minimum number of elements you need to remove to make it good? You can delete an arbitrary set of elements.

# Input

The first line contains the integer nn (1≤n≤120000) — the length of the given sequence.

The second line contains the sequence of integers a1,a2,…,an(1≤ai≤10^9).

# Output

Print the minimum number of elements needed to be removed from the given sequence in order to make it good. It is possible that you need to delete all nn elements, make it empty, and thus get a good sequence.

# Examples

**Input**

6

4 7 1 5 4 9

**Output**

1

**Input**

5

1 2 3 4 5

**Output**

2

**Input**

1

16

**Output**

1

**Input**

4

1 1 1 1023

**Output**

0


题意：给出 n 个数，对于每个数 ai，要求 ai+aj 都为2的幂，求不符合条件的个数

思路：

要判断是否和为 2 的幂，因此可以贪心的从 2^30 向下进行枚举，如果数组中的每一个数 ai，在数组中存在 j-ai 的话，则证明符合条件，反之则不符合。

因为题目的数据范围较大，因此用 map 存储。

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
map<long long,long long> mp;
map<long long,bool>vis;
long long a[N];
int main()
{
    int n;
    cin>>n;

    for(int i=0;i<n;i++)
    {
        cin>>a[i];
        mp[a[i]]++;
    }

    sort(a,a+n);

    int cnt=0;
    for(int i=0;i<n;i++)
    {
        if(vis[a[i]])
            continue;

        bool flag=true;
        for(long long j=1<<30;j>=1;j>>=1)
        {
            if(j<a[i])
                break;
            if(mp.count(j-a[i]))
            {
                if(j-a[i]==a[i]&&mp[j-a[i]]==1)
                    continue;

                vis[a[i]]=1;
                vis[j-a[i]]=1;
                flag=false;
                break;
            }
        }
        if(flag)
            cnt++;
    }
    cout<<cnt<<endl;
    return 0;
}
```






