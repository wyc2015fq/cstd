# Codeforces992C——Nastya and a Wardrobe - westbrook1998的博客 - CSDN博客





2018年08月30日 19:20:48[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：40








> 
Nastya received a gift on New Year — a magic wardrobe. It is magic because in the end of each month the number of dresses in it doubles (i.e. the number of dresses becomes twice as large as it is in the beginning of the month). 

  Unfortunately, right after the doubling the wardrobe eats one of the dresses (if any) with the 50% probability. It happens every month except the last one in the year. 

  Nastya owns x dresses now, so she became interested in the expected number of dresses she will have in one year. Nastya lives in Byteland, so the year lasts for k + 1 months. 

  Nastya is really busy, so she wants you to solve this problem. You are the programmer, after all. Also, you should find the answer modulo 109 + 7, because it is easy to see that it is always integer. 

  Input 

  The only line contains two integers x and k (0 ≤ x, k ≤ 1018), where x is the initial number of dresses and k + 1 is the number of months in a year in Byteland. 

  Output 

  In the only line print a single integer — the expected number of dresses Nastya will own one year later modulo 109 + 7. 

  Examples 

  Input 

  2 0 

  Output 

  4 

  Input 

  2 1 

  Output 

  7 

  Input 

  3 2 

  Output 

  21 

  Note 

  In the first example a year consists on only one month, so the wardrobe does not eat dresses at all. 

  In the second example after the first month there are 3 dresses with 50% probability and 4 dresses with 50% probability. Thus, in the end of the year there are 6 dresses with 50% probability and 8 dresses with 50% probability. This way the answer for this test is (6 + 8) / 2 = 7.
这题反而简单 很明显的规律 推个公式然后快速幂取模

取模wa了好几次…

代码：

```cpp
#include <cstdio>
#include <algorithm>
using namespace std;
long long x,k;
const int MOD=1e9+7;
long long pow_mod(long long a,long long n){
    long long ans=1;
    while(n){
        if(n%2){
            ans=(ans*a)%MOD;
        }
        n/=2;
        a=(a*a)%MOD;
    }
    return ans%MOD;
}
int main(void){
    scanf("%lld%lld",&x,&k);
    //0的特判
    if(x==0){
        printf("0\n");
    }
    else{
        //这里得到的结果都是取模之后的  所以2^(k+1) 有可能小于2^k
        //这里x也要先模一下
        long long ans1=(pow_mod(2,k+1)*(x%MOD))%MOD;
        long long ans2=pow_mod(2,k);
        printf("%lld\n",((ans1-ans2+MOD)%MOD+1)%MOD);
    }
    return 0;
}
```





