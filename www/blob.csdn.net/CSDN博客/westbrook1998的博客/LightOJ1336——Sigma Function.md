# LightOJ1336——Sigma Function - westbrook1998的博客 - CSDN博客





2018年08月20日 16:17:07[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：36标签：[数论																[唯一分解定理](https://so.csdn.net/so/search/s.do?q=唯一分解定理&t=blog)](https://so.csdn.net/so/search/s.do?q=数论&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
Sigma function is an interesting function in Number Theory. It is denoted by the Greek letter Sigma (σ). This function actually denotes the sum of all divisors of a number. For example σ(24) = 1+2+3+4+6+8+12+24=60. Sigma of small numbers is easy to find but for large numbers it is very difficult to find in a straight forward way. But mathematicians have discovered a formula to find sigma. If the prime power decomposition of an integer is  

  Then we can write,  

  For some n the value of σ(n) is odd and for others it is even. Given a value n, you will have to find how many integers from 1 to n have even value of σ.  

  Input  

  Input starts with an integer T (≤ 100), denoting the number of test cases.  

  Each case starts with a line containing an integer n (1 ≤ n ≤ 1012).  

  Output  

  For each case, print the case number and the result.  

  Sample Input  

  4  

  3  

  10  

  100  

  1000  

  Sample Output  

  Case 1: 1  

  Case 2: 5  

  Case 3: 83  

  Case 4: 947
真的有点强这道题….  

求数1-n的因数和是偶数的有多少个，完全蒙逼，题解看了好半天才看懂  

1. 首先我们先求出和是奇数的个数，然后用n减去  

2. 因数和的式子是f(x)= （1+p1+p1^2+p1^3+…+p1^a1）*(1+p2+p2^2+…+p2^a2)*…*(1+pn+pn^2+…+pn^an); 当有一个因子p=2时，无论系数多少，(1+2^1+2^2+…+2^a1) 都是奇数  

3. 然后如果p不是2，那么这个对应的系数必须是一个偶数，也就是后面pn+pn^2+…+pn^an 要是偶数项，这样加起来才是奇数  

4. 所以满足这两个条件，就之后平方数和两倍的平方数了，为什么呢，因为比如这个数n没有2这个因子，那么就只有3啊5啊这些，而且系数都是偶数，比如2，所以这个数就是3^2*5^2，这不就是一个平方数吗，而如果这个数还有2这个因子，无论2有多少次，都可以转为平方数或者平方数乘2  

5. 所以，找出1-n中满足条件的数的个数即可
代码：

```cpp
#include <cstdio>
#include <algorithm>
using namespace std;
typedef long long ll;
int main(void){
    int t;
    ll n;
    scanf("%d",&t);
    int c=1;
    while(t--){
        ll ans=0;
        scanf("%lld",&n);
        //先求出因数和为奇数的个数
        for(ll i=1;i*i<=n;i++){
            ans++;
            if(2*i*i<=n){
                ans++;
            }
        }
        printf("Case %d: %lld\n",c++,n-ans);
    }
    return 0;
}
```







