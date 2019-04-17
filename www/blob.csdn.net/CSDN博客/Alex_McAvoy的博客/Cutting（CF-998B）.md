# Cutting（CF-998B） - Alex_McAvoy的博客 - CSDN博客





2018年08月26日 16:23:18[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：60








# Problem Description

There are a lot of things which could be cut — trees, paper, "the rope". In this problem you are going to cut a sequence of integers.

There is a sequence of integers, which contains the equal number of even and odd numbers. Given a limited budget, you need to make maximum possible number of cuts such that each resulting segment will have the same number of odd and even integers.

Cuts separate a sequence to continuous (contiguous) segments. You may think about each cut as a break between two adjacent elements in a sequence. So after cutting each element belongs to exactly one segment. Say, [4,1,2,3,4,5,4,4,5,5] → two cuts → [4,1|2,3,4,5|4,4,5,5]. On each segment the number of even elements should be equal to the number of odd elements.

The cost of the cut between x and y numbers is |x−y| bitcoins. Find the maximum possible number of cuts that can be made while spending no more than B bitcoins.

# Input

First line of the input contains an integer n (2≤n≤100) and an integer B (1≤B≤100) — the number of elements in the sequence and the number of bitcoins you have.

Second line contains n integers: a1, a2, ..., an (1≤ai≤100) — elements of the sequence, which contains the equal number of even and odd numbers

# Output

Print the maximum possible number of cuts which can be made while spending no more than B bitcoins.

# Examples

**Input**

6 4

1 2 5 10 15 20

**Output**

1

**Input**

4 10

1 3 2 4

**Output**

0

**Input**

6 100

1 2 3 4 5 6

**Output**

2

————————————————————————————————————————————

题意：给出 n 个数字，保证奇数个数和偶数个数相等，可以把该序列分割成连续的片段，使得每一段的奇数和偶数个数相等，假设在 x 和 y 元素中间分割，那么花费 |x-y| 块钱，现有k块钱，求最多可以分割的次数

思路：从开头统计奇数和偶数的个数，如果某个位置奇数和偶数个数相等就把当前这个花费放入数组，然后贪心的选择最小的即可

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
int a[105];
int b[105],c[105],x[105];
int main()
{
   int n,m;
   cin>>n>>m;
   for(int i=1;i<=n;i++)
   {
       scanf("%d",&x[i]);

       if(x[i]%2==0)
           a[i]=a[i-1]+1;
       else
           a[i]=a[i-1];
       
       if(x[i]%2!=0)
           b[i]=b[i-1]+1;
       else
           b[i]=b[i-1];
   }

   int k=0;
   for(int i=1;i<=n;i++)
   {
       if(a[i]==b[i]&&i!=n)
        {
            k++;
            c[k]=fabs(x[i+1]-x[i]);
        }

   }
  
  sort(c+1,c+k+1);
  
  int ans=0;
  for(int i=1;i<=k;i++)
  {
      if(m-c[i]<0) 
          break;
      m-=c[i];
      ans++;
  }
  cout<<ans<<endl;
  return 0;
}
```





