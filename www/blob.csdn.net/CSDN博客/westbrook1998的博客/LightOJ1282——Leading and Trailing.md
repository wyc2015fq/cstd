# LightOJ1282——Leading and Trailing - westbrook1998的博客 - CSDN博客





2018年08月20日 23:12:20[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：26标签：[基础数论](https://so.csdn.net/so/search/s.do?q=基础数论&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
You are given two integers: n and k, your task is to find the most significant three digits, and least significant three digits of nk. 

  Input 

  Input starts with an integer T (≤ 1000), denoting the number of test cases. 

  Each case starts with a line containing two integers: n (2 ≤ n < 231) and k (1 ≤ k ≤ 107). 

  Output 

  For each case, print the case number and the three leading digits (most significant) and three trailing digits (least significant). You can assume that the input is given such that nk contains at least six digits. 

  Sample Input 

  5 

  123456 1 

  123456 2 

  2 31 

  2 32 

  29 8751919 

  Sample Output 

  Case 1: 123 456 

  Case 2: 152 936 

  Case 3: 214 648 

  Case 4: 429 296 

  Case 5: 665 669
求n^k的前3位和后3位，后三位好解决，快速幂取模，前三位就又是骚操作了 

因为n^k==10^ak 然后ak就是一个小数x+y，整数部分x其实 10^x就是表示这个乘积的位数 小数部分y 10^y才是乘以这个位数才是乘积 比如10^x是10000 10^y是1.2345  乘积是12345 

那么我们只要求出y就能求出10^y 从而得到前三位
代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <cmath>
using namespace std;
typedef long long ll;
const int MOD=1000;
ll quick_pow(ll n,ll k){
    ll ans=1;
    while(k){
        if(k%2){
            ans=(ans*n)%MOD;
        }
        k/=2;
        n=(n*n)%MOD;
    }
    return ans;
}
int main(void){
    int t;
    int c=1;
    long long n,k;
    scanf("%d",&t);
    while(t--){
        scanf("%lld%lld",&n,&k);
        long long a=pow(10.0,2.0+fmod((double)k*log10((double)n),1));
        long long b=quick_pow(n,k);
        printf("Case %d: %0*lld %0*lld\n",c++,3,a,3,b);
    }
    return 0;
}
```






