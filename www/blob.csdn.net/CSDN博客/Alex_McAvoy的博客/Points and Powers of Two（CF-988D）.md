# Points and Powers of Two（CF-988D） - Alex_McAvoy的博客 - CSDN博客





2018年08月07日 19:36:47[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：69
个人分类：[CodeForces																[STL 的使用——容器与迭代器](https://blog.csdn.net/u011815404/article/category/8793969)](https://blog.csdn.net/u011815404/article/category/7923180)








> 
# Problem Description

There are n distinct points on a coordinate line, the coordinate of i-th point equals to xi. Choose a subset of the given set of points such that the distance between each pair of points in a subset is an integral power of two. It is necessary to consider each pair of points, not only adjacent. Note that any subset containing one element satisfies the condition above. Among all these subsets, choose a subset with maximum possible size.

In other words, you have to choose the maximum possible number of points xi1,xi2,…,xim such that for each pair xij, xik it is true that |xij−xik|=2d where d is some non-negative integer number (not necessarily the same for each pair of points).

# Input

The first line contains one integer nn (1≤n≤2⋅10^5) — the number of points.

The second line contains n pairwise distinct integers x1,x2,…,xn (−10^9≤xi≤10^9) — the coordinates of points.

# Output

In the first line print mm — the maximum possible number of points in a subset that satisfies the conditions described above.

In the second line print m integers — the coordinates of points in the subset you have chosen.

If there are multiple answers, print any of them.

# Examples

**Input**

6

3 5 4 7 10 12

**Output**

3

7 3 5

**Input**

5

-1 2 5 8 11

**Output**

1

8


题意： 给出 n 个数，问最多能在这 n 个数中选多少个，使得由他们组成的集合中，两两之间的差的绝对值为2的幂。

思路：通过反证法可以证得集合元素最多为 3 个，使用 map，根据集合中的元素个数分别为 3、2、1 进行模拟即可。

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
int a[N];
map<int,int> mp;
int main()
{
    int n;
    cin>>n;

    for(int i=0;i<n;i++)
    {
        cin>>a[i];
        mp[a[i]]=1;
    }

    sort(a,a+n);
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<32;j++)
        {
            int x=1<<j;
            if(mp[a[i]+x]&&mp[a[i]+2*x])
            {
                cout<<3<<endl;
                cout<<a[i]<<" "<<a[i]+x<<" "<<a[i]+x*2<<endl;
                return 0;
            }
            if(a[i]+x>a[n-1] || a[i]+2*x>a[n-1])
                break;
        }
    }

    for(int i=0;i<n;i++)
    {
        for(int j=0;j<32;j++)
        {
            int x=1<<j;
            if(mp[a[i]+x])
            {
                cout<<2<<endl;
                cout<<a[i]<<" "<<a[i]+x<<endl;
                return 0;
            }
            if(mp[a[i]+x*2])
            {
                cout<<2<<endl;
                cout<<a[i]<<" "<<a[i]+2*x<<endl;
                return 0;
            }
            if(a[i]+x>a[n-1] || a[i]+2*x>a[n-1])
                break;
        }
    }

    cout<<1<<endl;
    cout<<a[0]<<endl;

    return 0;

}
```






