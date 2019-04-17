# Margarite and the best present（CF-1080B） - Alex_McAvoy的博客 - CSDN博客





2018年12月04日 17:40:57[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：35
个人分类：[CodeForces																[数学——其他](https://blog.csdn.net/u011815404/article/category/8279050)](https://blog.csdn.net/u011815404/article/category/7923180)








> 
# Problem Description

Little girl Margarita is a big fan of competitive programming. She especially loves problems about arrays and queries on them.

Recently, she was presented with an array aa of the size of 109 elements that is filled as follows:

a1=−1

a2=2

a3=−3

a4=4

a5=−5

And so on ...

That is, the value of the i-th element of the array aa is calculated using the formula ai=i⋅(−1).

She immediately came up with qq queries on this array. Each query is described with two numbers: ll and rr. The answer to a query is the sum of all the elements of the array at positions from ll to rr inclusive.

Margarita really wants to know the answer to each of the requests. She doesn't want to count all this manually, but unfortunately, she couldn't write the program that solves the problem either. She has turned to you — the best programmer.

Help her find the answers!

# Input

The first line contains a single integer q (1≤q≤103) — the number of the queries.

Each of the next q lines contains two integers l and r (1≤l≤r≤109) — the descriptions of the queries.

# Output

Print q lines, each containing one number — the answer to the query.

# Examples

**Input**

5

1 3

2 5

5 5

4 4

2 3

**Output**

-2

-2

-5

4

-1


题意：给出一个数列，第 i 项是 ai=i⋅(−1)，给出 q 组询问，每组给出 l、r 两个数，求数列 (l,r) 区间内的和

思路：将 l、r 分成奇奇、偶偶、奇偶、偶奇 4 种情况分别讨论找规律即可

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
#define N 10001
#define LL long long
using namespace std;
int main(){
    int q;
    scanf("%d",&q);
    while(q--){
        int l,r;
        scanf("%d%d",&l,&r);

        if(l%2==1&&r%2==1){
            printf("%d\n",-1*((l-r)/2+r));
        }
        if(l%2==1&&r%2==0){
            printf("%d\n",(r-l+1)/2);
        }
        if(l%2==0&&r%2==0){
            printf("%d\n",((l-r)/2+r));
        }
        if(l%2==0&&r%2==1){
            printf("%d\n",-1*(r-l+1)/2);
        }
    }
    return 0;
}
```






