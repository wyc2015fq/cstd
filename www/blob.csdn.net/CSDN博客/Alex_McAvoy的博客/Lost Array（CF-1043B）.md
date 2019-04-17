# Lost Array（CF-1043B） - Alex_McAvoy的博客 - CSDN博客





2018年11月07日 23:11:57[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：96








# Problem Description

Bajtek, known for his unusual gifts, recently got an integer array x0,x1,…,xk−1.

Unfortunately, after a huge array-party with his extraordinary friends, he realized that he'd lost it. After hours spent on searching for a new toy, Bajtek found on the arrays producer's website another array aa of length n+1. As a formal description of a says, a0=0 and for all other i (1≤i≤n) ai=x(i−1)mod k+ai−1, where pmodqpmodq denotes the remainder of division p by q.

For example, if the x=[1,2,3] and n=5, then:

a0=0,

a1=x0mod3+a0=x0+0=1,

a2=x1mod3+a1=x1+1=3,

a3=x2mod3+a2=x2+3=6,

a4=x3mod3+a3=x0+6=7,

a5=x4mod3+a4=x1+7=9.

So, if the x=[1,2,3] and n=5, then a=[0,1,3,6,7,9].

Now the boy hopes that he will be able to restore x from a! Knowing that 1≤k≤n, help him and find all possible values of k — possible lengths of the lost array.

# Input

The first line contains exactly one integer n (1≤n≤1000) — the length of the array a, excluding the element a0.

The second line contains nn integers a1,a2,…,an (1≤ai≤106).

Note that a0 is always 0 and is not given in the input.

# Output

The first line of the output should contain one integer ll denoting the number of correct lengths of the lost array.

The second line of the output should contain ll integers — possible lengths of the lost array in increasing order.

# Examples

**Input**

5

1 2 3 4 5

**Output**

5

1 2 3 4 5

**Input**

5

1 3 5 6 8

**Output**

2

3 5 

**Input**

3

1 5 3

**Output**

1

2

————————————————————————————————————————————

题意：给出 n 个数以及数组 a，要求出 x 数组的长度，并输出 x 数组

思路：先根据 a 数组将所有 x 数组求出来，然后枚举长度，依次判断是否合法即可

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<cstring>
#include<cmath>
#include<ctime>
#include<algorithm>
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-6
#define MOD 16007
#define INF 0x3f3f3f3f
#define N 1001
#define LL long long
using namespace std;
int a[N];
int sub[N];
int x[N];
int main()
{
    int n;
    cin>>n;
    for(int i=1;i<=n;i++){
        cin>>a[i];
        sub[i]=a[i]-a[i-1];
    }

    int k=0;
    for(int i=1;i<=n;i++){
        bool flag=true;
        for(int j=i+1;j<=n;j++){
            if(sub[j]!=sub[j-i]){
                flag=false;
                break;
            }
        }
        if(flag){
            k++;
            x[k]=i;
        }
    }

    cout<<k<<endl;
    for(int i=1;i<=k;i++)
        cout<<x[i]<<" ";
    cout<<endl;

    return 0;
}
```





