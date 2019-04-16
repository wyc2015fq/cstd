# hdu 3037 Saving Beans（lucas定理模板） - 紫芝的博客 - CSDN博客





2018年09月13日 11:10:32[紫芝](https://me.csdn.net/qq_40507857)阅读数：33








# Saving Beans

****Time Limit: 6000/3000 MS (Java/Others)    Memory Limit: 32768/32768 K (Java/Others)Total Submission(s): 7631    Accepted Submission(s): 3076****

Problem Description

Although winter is far away, squirrels have to work day and night to save beans. They need plenty of food to get through those long cold days. After some time the squirrel family thinks that they have to solve a problem. They suppose that they will save beans in n different trees. However, since the food is not sufficient nowadays, they will get no more than m beans. They want to know that how many ways there are to save no more than m beans (they are the same) in n trees.


Now they turn to you for help, you should give them the answer. The result may be extremely huge; you should output the result modulo p, because squirrels can’t recognize large numbers.

Input

The first line contains one integer T, means the number of cases.


Then followed T lines, each line contains three integers n, m, p, means that squirrels will save no more than m same beans in n different trees, 1 <= n, m <= 1000000000, 1 < p < 100000 and p is guaranteed to be a prime.

Output

You should output the answer modulo p.

Sample Input

2

1 2 5

2 1 5

Sample Output

3

3

***Hint***

Hint For sample 1, squirrels will put no more than 2 beans in one tree. Since trees are different, we can label them as 1, 2 … and so on. The 3 ways are: put no beans, put 1 bean in tree 1 and put 2 beans in tree 1. For sample 2, the 3 ways are: put no beans, put 1 bean in tree 1 and put 1 bean in tree 2.

Source

[2009 Multi-University Training Contest 13 - Host by HIT](http://acm.hdu.edu.cn/search.php?field=problem&key=2009+Multi-University+Training+Contest+13+-+Host+by+HIT&source=1&searchmode=source)

## 题意：

求在n棵树上摘不超过m颗豆子的方案，结果对p取模。

## 解析：

![](https://img-blog.csdn.net/20180913110606851?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 关于Lucas定理

Lucas(n,m,p)=C(n%p,m%p,p)∗Lucas(n/p,m/p,p)

//这里可以采用对n分段递归求解

Lucas(x,0,p)=1

将n，m分解变小之后问题又转换成了求(a/b)%p。



(a/b)%p可以转换成a∗Inv(b，p)

Inv(b,p)为b对p的逆元。


```cpp
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int maxn=1e5+7;
ll fact[maxn];
ll modpow(ll a,ll b,ll mod)
{
    ll ret=1;
    while(b){
        if(b&1) ret=(ret*a)%mod;
        a=(a*a)%mod;
        b>>=1;
    }
    return ret;
}
ll getfact(ll p)
{
    fact[0]=1;
    for(int i=1;i<=p;i++)
        fact[i]=(fact[i-1]*i)%p;
}
ll lucas(ll n,ll m,ll p)
{
    ll ret=1;
    while(n&&m){
        ll a=n%p,b=m%p;
        if(a<b) return 0;
        ret=(ret*fact[a]*modpow(fact[b]*fact[a-b]%p,p-2,p))%p;
        n/=p;
        m/=p;
    }
    return ret;
}
int main()
{
    int T;
    ll n,m,p;
    scanf("%d",&T);
    while(T--){
        scanf("%lld%lld%lld",&n,&m,&p);
        getfact(p);
        printf("%lld\n",lucas(n+m,m,p));
    }
}
```





