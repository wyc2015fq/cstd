# Not Equal on a Segment（CF-622C） - Alex_McAvoy的博客 - CSDN博客





2019年01月23日 21:27:20[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：42








> 
# Problem Description

You are given array a with n integers and m queries. The i-th query is given with three integers li, ri, xi.

For the i-th query find any position pi (li ≤ pi ≤ ri) so that api ≠ xi.

# Input

The first line contains two integers n, m (1 ≤ n, m ≤ 2·105) — the number of elements in a and the number of queries.

The second line contains n integers ai (1 ≤ ai ≤ 106) — the elements of the array a.

Each of the next m lines contains three integers li, ri, xi (1 ≤ li ≤ ri ≤ n, 1 ≤ xi ≤ 106) — the parameters of the i-th query.

# Output

Print m lines. On the i-th line print integer pi — the position of any number not equal to xi in segment [li, ri] or the value - 1 if there is no such number.

# Sample Input

**6 41 2 1 1 3 51 4 12 6 23 4 13 4 2**

# Sample Output

**26-14**


————————————————————————————————————————————

题意：给出 n 个数 m 组询问，每组询问 l、r、x，任意输出区间 [l,r] 上不等于 x 的一个位置，若在区间内全等于 x，则输出 -1 

思路：有些类似并查集路径压缩的思想，开一个数组 res[]，令 res[i] 记录第 i  个数前面第一个和他不等的数的下标就行了，如果相等就令 res[i]=res[i-1]，然后每次对 [l,r] 查询时，从最右端开始与 x 对比即可

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
#include<utility>
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-6
#define MOD 1000000007
#define INF 0x3f3f3f3f
#define N 1001
#define LL long long
using namespace std;
int a[N];
int res[N];
int main()
{
    int n,m;
    scanf("%d%d",&n,&m);
    for(int i=1;i<=n;++i)
        scanf("%d",&a[i]);

    res[n]=n;
    for(int i=n-1;i>=1;i--){
        if(a[i]==a[i+1])
            res[i]=res[i+1];
        else
            res[i]=i;
    }

    while(m--){
        int l,r,x;
        scanf("%d%d%d",&l,&r,&x);
        if(a[l]!=x)
            printf("%d\n",l);
        else if(res[l]+1<=r)
            printf("%d\n",res[l]+1);
        else
            printf("-1\n");
    }
    return 0;
}
```





