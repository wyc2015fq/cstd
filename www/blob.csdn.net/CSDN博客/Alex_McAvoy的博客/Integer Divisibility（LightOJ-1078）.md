# Integer Divisibility（LightOJ-1078） - Alex_McAvoy的博客 - CSDN博客





2019年02月13日 23:23:43[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：38
个人分类：[LightOJ																[数论——快速幂与同余式定理](https://blog.csdn.net/u011815404/article/category/8796909)](https://blog.csdn.net/u011815404/article/category/8037630)








> 
# Problem Description

If an integer is not divisible by 2 or 5, some multiple of that number in decimal notation is a sequence of only a digit. Now you are given the number and the only allowable digit, you should report the number of digits of such multiple.

For example you have to find a multiple of 3 which contains only 1's. Then the result is 3 because is 111 (3-digit) divisible by 3. Similarly if you are finding some multiple of 7 which contains only 3's then, the result is 6, because 333333 is divisible by 7.

# Input

Input starts with an integer T (≤ 300), denoting the number of test cases.

Each case will contain two integers n (0 < n ≤ 106 and n will not be divisible by 2 or 5) and the allowable digit (1 ≤ digit ≤ 9).

# Output

For each case, print the case number and the number of digits of such multiple. If several solutions are there; report the minimum one.

# Examples

**Input**

3

3 1

7 3

9901 1

**Output**

Case 1: 3

Case 2: 6

Case 3: 12


题意：t 组数据，每组给出两个数 n 和 m，让 m 变为 mm、mmm...，问当 m 变为几位数时恰好能整除 n

思路：同余定理

实际是考察同余定理，对于两个数 A、B，有：
- (A + B) mod M = ( A mod M + B mod M ) mod M
- (A * B) mod M = ((A mod M) *( B mod M)) mod M  

因此，令 A = m，每次让 A*10  后再加 m 然后 mod n，直到能整除为止

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
        LL n,m;
        scanf("%lld%lld",&n,&m);

        int temp=m;
        int res=1;
        while(temp%n){
            temp=(temp*10+m)%n;
            res++;
        }

        printf("Case %d: %lld\n",Case++,res);
    }
    return 0;
}
```






