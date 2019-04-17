# Almost Arithmetic Progression（CF-978D） - Alex_McAvoy的博客 - CSDN博客





2018年08月10日 16:47:32[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：79








# Problem Description

Polycarp likes arithmetic progressions. A sequence [a1,a2,…,an] is called an arithmetic progression if for each i (1≤i<n) the value ai+1−ai is the same. For example, the sequences [42], [5,5,5], [2,11,20,29] and [3,2,1,0] are arithmetic progressions, but [1,0,1], [1,3,9] and [2,3,1] are not.

It follows from the definition that any sequence of length one or two is an arithmetic progression.

Polycarp found some sequence of positive integers [b1,b2,…,bn]. He agrees to change each element by at most one. In the other words, for each element there are exactly three options: an element can be decreased by 1, an element can be increased by 1, an element can be left unchanged.

Determine a minimum possible number of elements in bb which can be changed (by exactly one), so that the sequence b becomes an arithmetic progression, or report that it is impossible.

It is possible that the resulting sequence contains element equals 0.

# Input

The first line contains a single integer n (1≤n≤100000) — the number of elements in b.

The second line contains a sequence b1,b2,…,bn (1≤bi≤10^9).

# Output

If it is impossible to make an arithmetic progression with described operations, print -1. In the other case, print non-negative integer — the minimum number of elements to change to make the given sequence becomes an arithmetic progression. The only allowed operation is to add/to subtract one from an element (can't use operation twice to the same position).

# Examples

**Input**

4

24 21 14 10

**Output**

3

**Input**

2

500 500

**Output**

0

**Input**

3

14 5 1

**Output**

-1

**Input**

5

1 3 6 9 12

**Output**

1

# Note

In the first example Polycarp should increase the first number on 1, decrease the second number on 1, increase the third number on 1, and the fourth number should left unchanged. So, after Polycarp changed three elements by one, his sequence became equals to [25,20,15,10], which is an arithmetic progression.

In the second example Polycarp should not change anything, because his sequence is an arithmetic progression.

In the third example it is impossible to make an arithmetic progression.

In the fourth example Polycarp should change only the first element, he should decrease it on one. After that his sequence will looks like [0,3,6,9,12], which is an arithmetic progression

————————————————————————————————————————————

题意： 给出长为 n 的数列，可对每一个数进行 +1，-1，不变的操作，求最少要几次操作能使这个数列变成等差数列，如果不能变为等差数列，则输出 -1

思路：由于只要确定了前两个数的差，就可以确定后面所有的数了，所以，直接枚举前两个数的差，然后检查后面的数是否符合要求，然后暴力枚举即可

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
#define LL long long
using namespace std;
int a[N],b[N];
int main()
{
    int n;
    cin>>n;
    for(int i=1;i<=n;i++)
        cin>>a[i];

    int minn=INF;
    for(int i=-1;i<=1;i++)//共3*3=9种情况
    {
        for(int j=-1;j<=1;j++)
        {
            bool flag=true;
            int ans=abs(i)+abs(j);

            b[1]=a[1]+i;
            b[2]=a[2]+j;

            int d=b[2]-b[1];
            for(int k=3;k<=n;k++)
            {
                if( abs( (b[k-1]+d)-a[k] )<=1 )
                {
                    b[k]=b[k-1]+d;
                    ans+=abs(b[k]-a[k]);
                }
                else
                {
                    flag=false;
                    break;
                }
            }

            if(flag)
                minn=min(minn,ans);
        }
    }

    if(minn!=INF)
        cout<<minn<<endl;
    else
        cout<<-1<<endl;

    return 0;
}
```





