# And（CF-1013B） - Alex_McAvoy的博客 - CSDN博客





2018年08月15日 18:57:32[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：64
个人分类：[基础算法——模拟																[CodeForces](https://blog.csdn.net/u011815404/article/category/7923180)](https://blog.csdn.net/u011815404/article/category/7812141)








# Problem Description

There is an array with n elements a1, a2, ..., an and the number x.

In one operation you can select some i (1 ≤ i ≤ n) and replace element ai with ai & x, where & denotes the bitwise and operation.

You want the array to have at least two equal elements after applying some operations (possibly, none). In other words, there should be at least two distinct indices i ≠ j such that ai = aj. Determine whether it is possible to achieve and, if possible, the minimal number of operations to apply.

# Input

The first line contains integers n and x (2 ≤ n ≤ 100 000, 1 ≤ x ≤ 100 000), number of elements in the array and the number to and with.

The second line contains n integers ai (1 ≤ ai ≤ 100 000), the elements of the array.

# Output

Print a single integer denoting the minimal number of operations to do, or -1, if it is impossible.

# Examples

**Input**

4 3

1 2 3 7

**Output**

1

**Input**

2 228

1 1

**Output**

0

**Input**

3 7

1 2 3

**Output**

-1

————————————————————————————————————————————

题意：给出两个数 n 与 x，以及长度为 n 的一个数组 a[n]，现在问数组中是否有相同的元素，如果有输出0，如果没有则进行 a[i] & x 操作，然后与原数组相比，如果有相同的元素，就输出1，如果还是没有，就看是否存在一个数操作完后与这个数相等，如果存在就输出2，否则，输出-1 

思路：

读题读的螺旋升天。。。看着挺难其实就是一个模拟水题。。。

先比较原数组，看是否有相同的元素，如果有直接输出0

再进行 a[i] & x 操作，将操作后的数全部记录下来，与原来的数组进行比较，看是否有相同的，如果有，就输出1

如果还是没有，就用这个数操作完之后的数在新的数组中寻找相同的数，如果能找到，就输出2

如果以上三步操作后均没有，就输出-1

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
#define N 200001
#define MOD 1e9+7
#define E 1e-6
#define LL long long
using namespace std;
int a[N];
int b[N];
int main()
{
    int n,x;
    cin>>n>>x;

    for(int i=1;i<=n;i++)
        cin>>a[i];

    sort(a+1,a+n+1);
    for(int i=1;i<=n-1;i++)//原数组寻找相同元素
    {
        if(a[i]==a[i+1])
        {
            cout<<0<<endl;
            return 0;
        }
    }

    for(int i=1;i<=n;i++)//存储新数组
        b[i]=a[i]&x;

    for(int i=1;i<=n;i++)//在新数组中寻找与原数组相同的元素
    {
        for(int j=1;j<=n;j++)
        {
            if(b[i]==a[j]&&i!=j)
            {
                cout<<1<<endl;
                return 0;
            }
        }
    }


    sort(b+1,b+n+1);
    for(int i=1;i<=n-1;i++)//在新数组寻找相同元素
    {
        if(b[i]==b[i+1])
        {
            cout<<2<<endl;
            return 0;
        }
    }

    cout<<-1<<endl;

    return 0;
}
```






