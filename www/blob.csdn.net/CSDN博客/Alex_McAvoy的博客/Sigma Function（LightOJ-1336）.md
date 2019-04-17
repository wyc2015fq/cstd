# Sigma Function（LightOJ-1336） - Alex_McAvoy的博客 - CSDN博客





2019年03月19日 17:39:02[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：24
个人分类：[LightOJ																[数论——唯一分解定理](https://blog.csdn.net/u011815404/article/category/8796922)](https://blog.csdn.net/u011815404/article/category/8037630)








> 
# Problem Description

Sigma function is an interesting function in Number Theory. It is denoted by the Greek letter Sigma (σ). This function actually denotes the sum of all divisors of a number. For example σ(24) = 1+2+3+4+6+8+12+24=60. Sigma of small numbers is easy to find but for large numbers it is very difficult to find in a straight forward way. But mathematicians have discovered a formula to find sigma. If the prime power decomposition of an integer is

![](https://img-blog.csdnimg.cn/20190319161138376.jpg)

Then we can write,

![](https://img-blog.csdnimg.cn/20190319161146560.jpg)

For some n the value of σ(n) is odd and for others it is even. Given a value n, you will have to find how many integers from 1 to n have even value of σ.

# Input

Input starts with an integer T (≤ 100), denoting the number of test cases.

Each case starts with a line containing an integer n (1 ≤ n ≤ 1012).

# Output

For each case, print the case number and the result.

# **Sample Input**

**43101001000**

# Sample Output

**Case 1: 1Case 2: 5Case 3: 83Case 4: 947**


题意：t 组数据，每组给出 1 个数 n，求 1~n 中，所有数的约数和是偶数的数的个数

思路：

通过给的 ![](https://img-blog.csdnimg.cn/20190319161146560.jpg)

可以发现，式子中的每一项都是一个等比数列 ![p_i^0+p_i^1+p_i^2+...+p_i^{e_i}](https://private.codecogs.com/gif.latex?p_i%5E0&plus;p_i%5E1&plus;p_i%5E2&plus;...&plus;p_i%5E%7Be_i%7D) 的和

那么，![\sigma (n)=(p_1^0+p_1^1+p_1^2+...+p_1^{e_1})*(p_2^0+p_2^1+p_2^2+...+p_2^{e_2})*...](https://private.codecogs.com/gif.latex?%5Csigma%20%28n%29%3D%28p_1%5E0&plus;p_1%5E1&plus;p_1%5E2&plus;...&plus;p_1%5E%7Be_1%7D%29*%28p_2%5E0&plus;p_2%5E1&plus;p_2%5E2&plus;...&plus;p_2%5E%7Be_2%7D%29*...)![*(p_k^0+p_k^1+p_k^2+...+p_k^{e_k})](https://private.codecogs.com/gif.latex?*%28p_k%5E0&plus;p_k%5E1&plus;p_k%5E2&plus;...&plus;p_k%5E%7Be_k%7D%29)

因此，对于式子 ![\sigma (n)=(p_1^0+p_1^1+p_1^2+...+p_1^{e_1})*(p_2^0+p_2^1+p_2^2+...+p_2^{e_2})*...](https://private.codecogs.com/gif.latex?%5Csigma%20%28n%29%3D%28p_1%5E0&plus;p_1%5E1&plus;p_1%5E2&plus;...&plus;p_1%5E%7Be_1%7D%29*%28p_2%5E0&plus;p_2%5E1&plus;p_2%5E2&plus;...&plus;p_2%5E%7Be_2%7D%29*...)![*(p_k^0+p_k^1+p_k^2+...+p_k^{e_k})](https://private.codecogs.com/gif.latex?*%28p_k%5E0&plus;p_k%5E1&plus;p_k%5E2&plus;...&plus;p_k%5E%7Be_k%7D%29)，只要其中的一项为偶数，那么 ![\sigma (n)](https://private.codecogs.com/gif.latex?%5Csigma%20%28n%29) 就一定为偶数，故而只要找到**一项为偶数**就可以

观察式子可以发现：当 ![p_i](https://private.codecogs.com/gif.latex?p_i) 为偶数时，那么 ![p_i^x(x>0)](https://private.codecogs.com/gif.latex?p_i%5Ex%28x%3E0%29) 一定为偶数，而 ![p_i^0=1](https://private.codecogs.com/gif.latex?p_i%5E0%3D1)，那么由于 1+偶+偶+...+偶=奇，因此 ![p_i^0+p_i^1+p_i^2+...+p_i^{e_i}](https://private.codecogs.com/gif.latex?p_i%5E0&plus;p_i%5E1&plus;p_i%5E2&plus;...&plus;p_i%5E%7Be_i%7D) 一定是一个奇数，那么，只有**当 ![p_i](https://private.codecogs.com/gif.latex?p_i) 为奇数**时，**![p_i^0+p_i^1+p_i^2+...+p_i^{e_i}](https://private.codecogs.com/gif.latex?p_i%5E0&plus;p_i%5E1&plus;p_i%5E2&plus;...&plus;p_i%5E%7Be_i%7D)****才可能为偶数**

再看 ![p_i^{e_i}(x>0)](https://private.codecogs.com/gif.latex?p_i%5E%7Be_i%7D%28x%3E0%29)，![p_i^0=1](https://private.codecogs.com/gif.latex?p_i%5E0%3D1)：
- 当 ![e_i](https://private.codecogs.com/gif.latex?e_i) 为偶数时，此时有偶数个奇数项，则：1+奇+奇+...+奇=奇，此时 ![p_i^0+p_i^1+p_i^2+...+p_i^{e_i}](https://private.codecogs.com/gif.latex?p_i%5E0&plus;p_i%5E1&plus;p_i%5E2&plus;...&plus;p_i%5E%7Be_i%7D) 仍为一个奇数
- **当 ![e_i](https://private.codecogs.com/gif.latex?e_i) 为奇数时**，此时有奇数个奇数项，则：1+奇+奇+...+奇=偶，此时 ![p_i^0+p_i^1+p_i^2+...+p_i^{e_i}](https://private.codecogs.com/gif.latex?p_i%5E0&plus;p_i%5E1&plus;p_i%5E2&plus;...&plus;p_i%5E%7Be_i%7D)**才是一个偶数**

因此，对 n 做素因子分解，只要存在一个 ![p_i,e_i](https://private.codecogs.com/gif.latex?p_i%2Ce_i)**均为奇数**，那么 ![\sigma (n)](https://private.codecogs.com/gif.latex?%5Csigma%20%28n%29) 就一定为偶数

然而这样做会 TLE，因此还要进一步的推导：

我们可以考虑使用 总数-不满足的数=满足的数 来求解，也即求出所有 ![p_i,e_i](https://private.codecogs.com/gif.latex?p_i%2Ce_i) 不均为奇数的数

当素因子分解后，有四种形式：奇^奇、偶^偶、奇^偶、偶^奇，根据前面的分析，只有当 ![p_i,e_i](https://private.codecogs.com/gif.latex?p_i%2Ce_i) 均为奇数时，![\sigma (n)](https://private.codecogs.com/gif.latex?%5Csigma%20%28n%29) 才为偶数

那么，如果当素因子分解后，全是 **偶^偶、奇^偶、偶^奇** 的形式，因子和就一定为奇数

由于 2 是唯一的偶素数，我们将这个特殊的数拿出来分开考虑，根据唯一分解定理 ![](https://img-blog.csdnimg.cn/20190319161138376.jpg)

那么：
- **当 ![e_i](https://private.codecogs.com/gif.latex?e_i) 为偶数时，有 偶^偶、奇^偶 两种情况**，其能拆成某个数的平方，即：偶^偶=( 偶^(偶/2) )^2、奇^偶=( 奇^(偶/2) )^2，那么 偶^奇、奇^偶 则两种情况就被** n 以内的平方数** 给包含了，即有 **![\sqrt n](https://private.codecogs.com/gif.latex?%5Csqrt%20n)**种情况
- **当 ![e_i](https://private.codecogs.com/gif.latex?e_i) 为奇数时，有 偶^奇 一种情况**，由于其一定能拆成 2*x^2 的形式，那么 偶^奇 的情况就被 **n 以内的 2*平方数 **给包含了，即有 **![\sqrt {\frac{n}2}](https://private.codecogs.com/gif.latex?%5Csqrt%20%7B%5Cfrac%7Bn%7D2%7D)**种情况

故最后结果为：**![n-\sqrt n-\sqrt {\frac{n}2}](https://private.codecogs.com/gif.latex?n-%5Csqrt%20n-%5Csqrt%20%7B%5Cfrac%7Bn%7D2%7D)**

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
        LL temp1=(LL)sqrt(n);
        LL temp2=(LL)sqrt(n/2);
        printf("Case %d: %lld\n",Case++,n-temp1-temp2);
    }
    return 0;
}
```




