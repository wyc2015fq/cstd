# Three Parts of the Array（CF-1006C） - Alex_McAvoy的博客 - CSDN博客





2018年08月06日 09:07:09[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：45
个人分类：[CodeForces																[查找——顺序查找](https://blog.csdn.net/u011815404/article/category/8116903)](https://blog.csdn.net/u011815404/article/category/7923180)








# Problem Description

You are given an array d1,d2,…,dn consisting of nn integer numbers.

Your task is to split this array into three parts (some of which may be empty) in such a way that each element of the array belongs to exactly one of the three parts, and each of the parts forms a consecutive contiguous subsegment (possibly, empty) of the original array.

Let the sum of elements of the first part be sum1, the sum of elements of the second part be sum2 and the sum of elements of the third part be sum3. Among all possible ways to split the array you have to choose a way such that sum1=sum3 and sum1 is maximum possible.

More formally, if the first part of the array contains a elements, the second part of the array contains b elements and the third part contains c elements, then:

sum1=∑(1≤i≤a)di,

sum2=∑(a+1≤i≤a+b)di,

sum3=∑(a+b+1≤i≤a+b+c)di.

The sum of an empty array is 0.

Your task is to find a way to split the array such that sum1=sum3 and sum1 is maximum possible.

# Input

The first line of the input contains one integer nn (1≤n≤2⋅10^5) — the number of elements in the array d.

The second line of the input contains nn integers d1,d2,…,dn (1≤di≤10^9) — the elements of the array d.

# Output

Print a single integer — the maximum possible value of sum1, considering that the condition sum1=sum3 must be met.

Obviously, at least one valid way to split the array exists (use a=c=0 and b=n).

# Examples

**Input**

5

1 3 1 1 4

**Output**

5

**Input**

5

1 3 2 1 4

**Output**

4

**Input**

3

4 1 2

**Output**

0

# Note

In the first example there is only one possible splitting which maximizes sum1: [1,3,1],[ ],[1,4].

In the second example the only way to have sum1=4 is: [1,3],[2,1],[4].

In the third example there is only one way to split the array: [ ],[4,1,2],[ ].

————————————————————————————————————————————

题意：给出 n 个值，将这个数组分成三部分，sum1，sum2，sum3，可以为空，要求 sum1 = sum3 且 sum1 最大，求 sum1 的值

思路：

一开始用两个数组存从前往后与从后往前的和，然后双重循环暴力解决，结果超时了。。。

然后用了双重指针，分别指向第一个和最后一个，再分别记录从左到右与从右到左的和，然后通过移动指针来不断比较两个和，如果相等就把这个值记录下来，不断更新，直到左指针大于等于右指针

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
#define MOD 123
#define E 1e-6
using namespace std;
long long a[N];
int main()
{
    int n;
    while(scanf("%d",&n)!=EOF)
    {
        for(int i=1;i<=n;i++)
            scanf("%d",&a[i]);

        long long head=1,tail=n;
        long long sum1=0,sum2=0;
        long long maxx=0;
        while(head<=tail)
        {
            if(sum1>sum2)
            {
                sum2+=a[tail];
                tail--;
            }
            else if(sum2>sum1)
            {
                sum1+=a[head];
                head++;
            }

            if(sum1==sum2)
            {
                maxx=max(maxx,sum1);
                sum1+=a[head];
                head++;
            }
        }
        printf("%lld\n",maxx);
    }

    return 0;
}
```






