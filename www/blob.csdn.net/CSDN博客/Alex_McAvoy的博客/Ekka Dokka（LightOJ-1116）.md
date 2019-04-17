# Ekka Dokka（LightOJ-1116） - Alex_McAvoy的博客 - CSDN博客





2019年02月13日 23:29:27[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：47








> 
# Problem Description

Ekka and his friend Dokka decided to buy a cake. They both love cakes and that's why they want to share the cake after buying it. As the name suggested that Ekka is very fond of odd numbers and Dokka is very fond of even numbers, they want to divide the cake such that Ekka gets a share of N square centimeters and Dokka gets a share of M square centimeters where N is odd and M is even. Both N and M are positive integers.

They want to divide the cake such that N * M = W, where W is the dashing factor set by them. Now you know their dashing factor, you have to find whether they can buy the desired cake or not.

# Input

Input starts with an integer T (≤ 10000), denoting the number of test cases.

Each case contains an integer W (2 ≤ W < 263). And W will not be a power of 2.

# Output

For each case, print the case number first. After that print "Impossible" if they can't buy their desired cake. If they can buy such a cake, you have to print N and M. If there are multiple solutions, then print the result where M is as small as possible.

# Examples

**Input**

3

10

5

12

**Output**

Case 1: 5 2

Case 2: Impossible

Case 3: 3 4


题意：t 组数据，每组给出一个数 w，现已知 n*m=w，其中 m 是偶数，求出 n、m 的值，要求 m 的值尽可能的小，若求不出则输出 Impossible

思路：

当 w 是偶数时，一定有解，因此可对 m 从 2 开始进行枚举，直到找到 n 为奇数为止

当 w 是奇数时，由于一个偶数乘以一个奇数一定是一个偶数，因此无解

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
#include<deque>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-6
#define INF 0x3f3f3f3f
#define N 100001
#define LL long long
const int MOD=1e9+7;
const int dx[]={-1,1,0,0};
const int dy[]={0,0,-1,1};
using namespace std;

int main(){
    int t;
    scanf("%d",&t);
    int Case=1;
    while(t--){
        LL w;
        scanf("%lld",&w);

        if(w%2)
            printf("Case %d: Impossible\n",Case++);
        else{
            LL n=w,m=2;
            while(n%2==0){
                n/=2;
                if(n%2)
                    break;
                m*=2;
            }
            printf("Case %d: %lld %lld\n",Case++,n,m);
        }
    }
    return 0;
}
```





