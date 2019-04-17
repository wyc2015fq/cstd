# Sonya and Hotels（CF-1004A） - Alex_McAvoy的博客 - CSDN博客





2018年08月26日 16:06:14[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：46
个人分类：[CodeForces																[基础算法——贪心](https://blog.csdn.net/u011815404/article/category/7628972)](https://blog.csdn.net/u011815404/article/category/7923180)








# Problem Description

Sonya decided that having her own hotel business is the best way of earning money because she can profit and rest wherever she wants.

The country where Sonya lives is an endless line. There is a city in each integer coordinate on this line. She has n hotels, where the i-th hotel is located in the city with coordinate xi. Sonya is a smart girl, so she does not open two or more hotels in the same city.

Sonya understands that her business needs to be expanded by opening new hotels, so she decides to build one more. She wants to make the minimum distance from this hotel to all others to be equal to dd. The girl understands that there are many possible locations to construct such a hotel. Thus she wants to know the number of possible coordinates of the cities where she can build a new hotel.

Because Sonya is lounging in a jacuzzi in one of her hotels, she is asking you to find the number of cities where she can build a new hotel so that the minimum distance from the original nn hotels to the new one is equal to d.

# Input

The first line contains two integers n and d (1≤n≤100, 1≤d≤109) — the number of Sonya's hotels and the needed minimum distance from a new hotel to all others.

The second line contains n different integers in strictly increasing order x1,x2,…,xn (−109≤xi≤109) — coordinates of Sonya's hotels.

# Output

Print the number of cities where Sonya can build a new hotel so that the minimum distance from this hotel to all others is equal to d.

# Examples

**Input**

4 3

-3 2 9 16

**Output**

6

**Input**

5 2

4 8 11 18 19

**Output**

5

————————————————————————————————————————————

题意：给出 n 个数轴上的点，现在想加入一些点，使得这些点与跟他距离最近的点为 d，求能加多少个不同的点

思路：每个位置的数字都可以向左或向右添加一个距离为 d 的点，向左边添加的话那么它距离当前点的距离一定要大于等于 d，向右同理，此时会可能把某个点重复计算，因此要把这种情况归到某一边，即左边为 >= 时右边为 >，或者左边为 > 时右边为 >=

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
int a[N];
int main()
{
    int n,d;
    cin>>n>>d;
    for(int i=1;i<=n;i++)
        cin>>a[i];

    int sum=2;
    for(int i=1;i<=n;i++)
    {
        if(a[i]-d>a[i-1]+d&&i>=2)
            sum++;
        if(a[i]+d<=a[i+1]-d&&i<=n-1)
            sum++;
    }
    cout<<sum<<endl;
    return 0;
}
```






