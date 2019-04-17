# Positive Negative Sign（LightOJ-1294） - Alex_McAvoy的博客 - CSDN博客





2019年02月13日 23:26:47[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：27
个人分类：[LightOJ																[数学——其他](https://blog.csdn.net/u011815404/article/category/8279050)](https://blog.csdn.net/u011815404/article/category/8037630)








> 
# Problem Description

Given two integers: n and m and n is divisible by 2m, you have to write down the first n natural numbers in the following form. At first take first m integers and make their sign negative, then take next m integers and make their sign positive, the next m integers should have negative signs and continue this procedure until all the n integers have been assigned a sign. For example, let n be 12 and m be 3. Then we have

-1 -2 -3 +4 +5 +6 -7 -8 -9 +10 +11 +12

If n = 4 and m = 1, then we have

-1 +2 -3 +4

Now your task is to find the summation of the numbers considering their signs.

# Input

Input starts with an integer T (≤ 10000), denoting the number of test cases.

Each case starts with a line containing two integers: n and m (2 ≤ n ≤ 109, 1 ≤ m). And you can assume that n is divisible by 2*m.

# Output

For each case, print the case number and the summation.

# Examples

**Input**

2

12 3

4 1

**Output**

Case 1: 18

Case 2: 2


题意：t 组数据，每组给出 n、m 两个值，代表给定数字 1-n，就是给定1-n 的数字，数字正负以 - + - + - + 的形式交替，每 m 个为一组，求这 n 个数字的和

思路：找规律

观察这 n 个数，发现无论 m 是多大，每 m 个数与上 m 个数的和都为 m*m，因此直接求这 n 个数中有几组 m 即可

由于数据范围，注意使用 long long

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
int row[N],col[N];
int main(){
    int t;
    scanf("%d",&t);

    int Case=1;
    while(t--){
        LL n,m;
        scanf("%lld%lld",&n,&m);
        LL num=n/m;//m组数
        LL res=(num/2)*m*m;//n个数字之和

        printf("Case %d: %lld\n",Case++,res);
    }
    return 0;
}
```




