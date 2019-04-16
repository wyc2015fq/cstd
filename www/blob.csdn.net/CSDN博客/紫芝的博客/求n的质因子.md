# 求n的质因子 - 紫芝的博客 - CSDN博客





2018年09月26日 20:50:32[紫芝](https://me.csdn.net/qq_40507857)阅读数：415








**质因子**（或**质因数**）在[数论](https://baike.baidu.com/item/%E6%95%B0%E8%AE%BA/3700)里是指能整除给定正[整数](https://baike.baidu.com/item/%E6%95%B4%E6%95%B0/1293937)的[质数](https://baike.baidu.com/item/%E8%B4%A8%E6%95%B0/263515)。根据[算术基本定理](https://baike.baidu.com/item/%E7%AE%97%E6%9C%AF%E5%9F%BA%E6%9C%AC%E5%AE%9A%E7%90%86/10920095)，不考虑排列顺序的情况下，每个正整数都能够以唯一的方式表示成它的质因数的乘积。两个没有共同质因子的正整数称为[互质](https://baike.baidu.com/item/%E4%BA%92%E8%B4%A8/577412)。因为1没有质因子，[1](https://baike.baidu.com/item/1)与任何正整数（包括1本身）都是互质。只有一个质因子的正整数为质数，质数的质因子就是它本身。

将一个正整数表示成质因数乘积的过程和得到的表示结果叫做[**质因数分解**](https://baike.baidu.com/item/%E8%B4%A8%E5%9B%A0%E6%95%B0%E5%88%86%E8%A7%A3)。显示质因数分解结果时，如果其中某个质因数出现了不止一次，可以用幂次的形式表示。例如360的质因数分解是：

![](https://gss1.bdstatic.com/-vo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D272/sign=e365781495510fb37c197090eb32c893/2f738bd4b31c8701a423844b2c7f9e2f0608ff81.jpg)

其中的质因数2、3、5在360的质因数分解中的幂次分别是3，2，1。

数论中的不少[函数](https://baike.baidu.com/item/%E5%87%BD%E6%95%B0/301912)与正整数的质因子有关，比如取值为*n*的质因数个数的函数和取值为*n*的质因数之和的函数。它们都是加性函数，但并非完全加性函数。



## 例子
- 
1没有质因子。

- 
5只有1个质因子，5本身。（5是质数。）

- 
6的质因子是2和3。(6 = 2×3)

- 
2、4、8、16等只有1个质因子：2（2是质数，4 = 22，8 = 23，如此类推。）

- 
100有2个质因子：2和5。(100 = 2^2×5^2)

## 性质

- 
两个没有共同质因子的[正整数](https://baike.baidu.com/item/%E6%AD%A3%E6%95%B4%E6%95%B0)称为[互质](https://baike.baidu.com/item/%E4%BA%92%E8%B4%A8)。

- 
数字1与任何正整数（包括1 本身）都是互质。

- 


- 
正整数的[因数分解](https://baike.baidu.com/item/%E5%9B%A0%E6%95%B0%E5%88%86%E8%A7%A3)给出一连串的质因子；所有质因子相乘后。质因子如重复会以指数表示。

- 
根据Fundamental theorem of arithmetic（算术基本定理），任正整数有独一无二的质因子[分解式](https://baike.baidu.com/item/%E5%88%86%E8%A7%A3%E5%BC%8F)。

- 
设任正整数n，其质因子数目及其质因子的和是n的算术函数(arithmetic function)。

- 
例子 6的质因子是3和2。(6 = 3 × 2)

- 
5只有1个质因子，5本身。（5是[质数](https://baike.baidu.com/item/%E8%B4%A8%E6%95%B0)。）

- 
10有2个质因子：2和5。(10 = 2 x 5, 且10=5 x 2，只有2和5是质数)

- 
2、4、8、16等只有1个质因子：2（2是质数，4 =**2**x 2，8 =**2**x 4，如此类推。偶数(6除外)的因子中，只有2是质数。）


1没有质因子。（1是empty product）

```cpp
#include<iostream>
#include<algorithm>
#include<cstring>
#include<cmath>
#include<cstdio>
#include<vector>
using namespace std;
const int mod=1e9+7;
typedef long long ll;
ll fac[1010];
void findFactor(ll x)//求x的质因子
{
    ll s=x;
    fac[0]=0;//表示质因子的个数
    for(ll i=2;i*i<=s;i++)
    {
        if(x%i==0)
        {
            fac[++fac[0]]=i;
            while(x%i==0)
                x/=i;
        }
    }
     if(x>1)    fac[++fac[0]]=x;
}
ll phi(ll x)//求x的欧拉函数值
{
    ll res=x;
    for(ll i=2;i*i<=x;i++)
    {
        if(x%i==0){
            res=res/i*(i-1);
            while(x%i==0)   x/=i;
        }
    }
    if(x>1) res=res/x*(x-1);
    return res;
}
int main()
{
    ll a,b;
    findFactor(150);
    for(int i=1;i<=fac[0];i++)
        printf("%lld\n",fac[i]);
    printf("%lld\n",phi(150));
}
```



