# POJ1284 Primitive Roots 数论 求原根的个数 - 紫芝的博客 - CSDN博客





2018年09月17日 21:09:12[紫芝](https://me.csdn.net/qq_40507857)阅读数：25








## Primitive Roots
|**Time Limit:** 1000MS||**Memory Limit:** 10000K|
|----|----|----|
|**Total Submissions:** 5642||**Accepted:** 3219|

Description

We say that integer x, 0 < x < p, is a primitive root modulo odd prime p if and only if the set { (x^i mod p) | 1 <= i <= p-1 } is equal to { 1, ..., p-1 }. For example, the consecutive powers of 3 modulo 7 are 3, 2, 6, 4, 5, 1, and thus 3 is a primitive root modulo 7. 

Write a program which given any odd prime 3 <= p < 65536 outputs the number of primitive roots modulo p. 

Input

Each line of the input contains an odd prime numbers p. Input is terminated by the end-of-file seperator.

Output

For each p, print a single number that gives the number of primitive roots in a single line.

Sample Input

```
23
31
79
```

Sample Output

```
10
8
24
```

题意：

求一个数p的原根的个数，设m是正整数，a是整数，若a模m的阶等于φ(m)，则称a为模m的一个原根。φ(m)表示m的欧拉函数。

**定义：**设![](https://img-blog.csdn.net/20140212121248625)，![](https://img-blog.csdn.net/20140212121354515)，使得![](https://img-blog.csdn.net/20140212121541671)成立的最小的![](https://img-blog.csdn.net/20140212121719812)，称为![](https://img-blog.csdn.net/20140212121834687)对模![](https://img-blog.csdn.net/20140212121927734)的阶，记为![](https://img-blog.csdn.net/20140212122148671)。

**定理：**如果模![](https://img-blog.csdn.net/20140212121927734)有原根，那么它一共有![](https://img-blog.csdn.net/20140212122615406)个原根。

**定理：**如果模![](https://img-blog.csdn.net/20140212121927734)有原根且m为质数，那么它一共有**φ(m-1)**个原根。

**定理：**如果![](https://img-blog.csdn.net/20140210202159109)为素数，那么素数![](https://img-blog.csdn.net/20140210202159109)一定存在原根，并且模![](https://img-blog.csdn.net/20140210202159109)的原根的个数为![](https://img-blog.csdn.net/20140210202317031)。

**定理：**设![](https://img-blog.csdn.net/20140210202516843)是正整数，![](https://img-blog.csdn.net/20140210202528578)是整数，若![](https://img-blog.csdn.net/20140210202528578)模![](https://img-blog.csdn.net/20140210202516843)的阶等于![](https://img-blog.csdn.net/20140210202728046)，则称![](https://img-blog.csdn.net/20140210202528578)为模![](https://img-blog.csdn.net/20140210202516843)的一个原根。

   假设一个数![](https://img-blog.csdn.net/20140210203021625)对于模![](https://img-blog.csdn.net/20140210202159109)来说是原根，那么![](https://img-blog.csdn.net/20140210203155421)的结果两两不同,且有![](https://img-blog.csdn.net/20140210203348031)，那么![](https://img-blog.csdn.net/20140210203021625)可以称为是模![](https://img-blog.csdn.net/20140210202159109)的一个原根，归根到底就是![](https://img-blog.csdn.net/20140210203628421)当且仅当指数为![](https://img-blog.csdn.net/20140210203757296)的时候成立。（这里![](https://img-blog.csdn.net/20140210202159109)是素数）

**模![](https://img-blog.csdn.net/20140212121927734)有原根的充要条件：**![](https://img-blog.csdn.net/20140212123623281)，其中![](https://img-blog.csdn.net/20140212123646562)是奇素数。

**求模素数![](https://img-blog.csdn.net/20140210202159109)原根的方法：**对![](https://img-blog.csdn.net/20140210203757296)素因子分解，即![](https://img-blog.csdn.net/20140210205528765)是![](https://img-blog.csdn.net/20140210203757296)的标准分解式，若恒有

![](https://img-blog.csdn.net/20140210205811375)

成立，则![](https://img-blog.csdn.net/20140210203021625)就是![](https://img-blog.csdn.net/20140210202159109)的原根。（对于合数求原根，只需把![](https://img-blog.csdn.net/20140212124321781)换成![](https://img-blog.csdn.net/20140212124434953)即可）

**定理：**如果模![](https://img-blog.csdn.net/20140212121927734)有原根，那么它一共有![](https://img-blog.csdn.net/20140212122615406)个原根。

```cpp
#include<map>
#include<stack>
#include<queue>
#include<vector>
#include<math.h>
#include<stdio.h>
#include<iostream>
#include<string.h>
#include<stdlib.h>
#include<algorithm>
using namespace std;
typedef long long  ll;
#define inf 1000000000
#define mod 100000000
#define  lowbit(x) (x&-x)
#define  eps 1e-10
ll phi(ll x)
{
    ll res=x;
    for(ll i=2;i*i<=x;i++)
    {
        if(x%i==0)
        {
            res-=res/i;
            while(x%i==0)   x/=i;
        }
    }
    if(x>1)
        res-=res/x;
    return res;
}
int main(void)
{
	ll n;
	while(scanf("%lld",&n)!=EOF&&n){
        //n--;
        printf("%lld\n",phi(phi(n)));
	}
	return 0;
}
```

**定理：**如果模![](https://img-blog.csdn.net/20140212121927734)有原根且m为质数，那么它一共有**φ(m-1)**个原根。

```cpp
#include<map>
#include<stack>
#include<queue>
#include<vector>
#include<math.h>
#include<stdio.h>
#include<iostream>
#include<string.h>
#include<stdlib.h>
#include<algorithm>
using namespace std;
typedef long long  ll;
#define inf 1000000000
#define mod 100000000
#define  lowbit(x) (x&-x)
#define  eps 1e-10
ll phi(ll x)
{
    ll res=x;
    for(ll i=2;i*i<=x;i++)
    {
        if(x%i==0)
        {
            res-=res/i;
            while(x%i==0)   x/=i;
        }
    }
    if(x>1)
        res-=res/x;
    return res;
}
int main(void)
{
	ll n;
	while(scanf("%lld",&n)!=EOF&&n){
        //n--;
        printf("%lld\n",phi(n-1));
	}
	return 0;
}
```





