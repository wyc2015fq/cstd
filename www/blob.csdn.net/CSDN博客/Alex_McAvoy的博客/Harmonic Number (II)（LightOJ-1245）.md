# Harmonic Number (II)（LightOJ-1245） - Alex_McAvoy的博客 - CSDN博客





2019年03月20日 11:21:58[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：45
个人分类：[LightOJ																[数学——其他](https://blog.csdn.net/u011815404/article/category/8279050)](https://blog.csdn.net/u011815404/article/category/8037630)








> 
# Problem Description

I was trying to solve problem '1234 - Harmonic Number', I wrote the following code

```cpp
long long H( int n ) {
    long long res = 0;
    for( int i = 1; i <= n; i++ )
        res = res + n / i;
    return res;
}
```

Yes, my error was that I was using the integer divisions only. However, you are given n, you have to find H(n) as in my code.

# Input

Input starts with an integer T (≤ 1000), denoting the number of test cases.

Each case starts with a line containing an integer n (1 ≤ n < 2^31).

# Output

For each case, print the case number and H(n) calculated by the code.

# **Sample Input**

**11123456789102147483647**

# Sample Output

**Case 1: 1Case 2: 3Case 3: 5Case 4: 8Case 5: 10Case 6: 14Case 7: 16Case 8: 20Case 9: 23Case 10: 27Case 11: 46475828386**


题意：给出 t 组数，每组输入一个数 n，要求根据代码求 H(n)

思路：

首先，暴力肯定会超时，那么就需要进行推导了

根据代码可知，问题实质是要求：![H(n)=[\frac{n}{1}]+[\frac{n}{2}]+...+[\frac{n}{n}]](https://private.codecogs.com/gif.latex?H%28n%29%3D%5B%5Cfrac%7Bn%7D%7B1%7D%5D&plus;%5B%5Cfrac%7Bn%7D%7B2%7D%5D&plus;...&plus;%5B%5Cfrac%7Bn%7D%7Bn%7D%5D)

对于式子中的每一项，都是一个反比例函数：![f(n)=\frac{n}{x}](https://private.codecogs.com/gif.latex?f%28n%29%3D%5Cfrac%7Bn%7D%7Bx%7D)，其与 ![y=x](https://private.codecogs.com/gif.latex?y%3Dx) 的对称点刚好为 ![\sqrt n](https://private.codecogs.com/gif.latex?%5Csqrt%20n)

![](https://img-blog.csdnimg.cn/20190320111148215.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

于是，对于每一项 ![x=i](https://private.codecogs.com/gif.latex?x%3Di)，可先求出红色的部分 ![[\frac{n}{i}]](https://private.codecogs.com/gif.latex?%5B%5Cfrac%7Bn%7D%7Bi%7D%5D)，然后乘以 2，由于 ![\sqrt n](https://private.codecogs.com/gif.latex?%5Csqrt%20n) 部分存在重叠，因此最后还要再减去重叠部分 ![\sqrt n *\sqrt n](https://private.codecogs.com/gif.latex?%5Csqrt%20n%20*%5Csqrt%20n)

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
#define E 1e-9
#define INF 0x3f3f3f3f
#define LL long long
const int MOD=10007;
const int N=1000000+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;
int main(){
    int t;
    scanf("%d",&t);

    int Case=1;
    while(t--){
        LL n;
        scanf("%lld",&n);

        int m=sqrt(n);//关于√n对称
        LL res=0;
        for(int i=1;i<=m;i++)//累加每一项
            res+=n/i;

        res*=2;//总数乘以2
        res-=m*m;//减去重叠部分

        printf("Case %d: %lld\n",Case++,res);
    }
    return 0;
}
```






