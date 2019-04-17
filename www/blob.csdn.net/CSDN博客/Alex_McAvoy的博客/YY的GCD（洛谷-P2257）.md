# YY的GCD（洛谷-P2257） - Alex_McAvoy的博客 - CSDN博客





2018年08月05日 16:15:00[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：97








> 
## 题目描述

神犇YY虐完数论后给傻×kAc出了一题

给定N, M,求1<=x<=N, 1<=y<=M且gcd(x, y)为质数的(x, y)有多少对

kAc这种傻×必然不会了，于是向你来请教……

多组输入

## 输入输出格式

**输入格式：**

第一行一个整数T 表述数据组数

接下来T行，每行两个正整数，表示N, M

**输出格式：**

T行，每行一个整数表示第i组数据的结果

## 输入输出样例

**输入样例#1：**

**210 10100 100**

**输出样例#1：**

**302791**


思路：

题目要求的实际上就是 ![\sum^n_{i=1}\sum^m_{j=1}[GCD(x,y)=prime]](https://private.codecogs.com/gif.latex?%5Csum%5En_%7Bi%3D1%7D%5Csum%5Em_%7Bj%3D1%7D%5BGCD%28x%2Cy%29%3Dprime%5D) 的值

设 f(n) 为满足 ![GCD(x,y)=d](https://private.codecogs.com/gif.latex?GCD%28x%2Cy%29%3Dd) 的个数

设 F(n) 为满足 ![GCD(x,y)=d](https://private.codecogs.com/gif.latex?GCD%28x%2Cy%29%3Dd) 和 d 的倍数的个数

显然：![F(x)=(n/x)*(m/x)](https://private.codecogs.com/gif.latex?F%28x%29%3D%28n/x%29*%28m/x%29)

反演后得到：![f(x)=\sum_{x|d}\mu (d/x)F(d)=\sum_{x|d}\mu (d/x)((n/d)*(m/d))](https://private.codecogs.com/gif.latex?f%28x%29%3D%5Csum_%7Bx%7Cd%7D%5Cmu%20%28d/x%29F%28d%29%3D%5Csum_%7Bx%7Cd%7D%5Cmu%20%28d/x%29%28%28n/d%29*%28m/d%29%29)

由于题目要求 ![GCD(x,y)=prime](https://private.codecogs.com/gif.latex?GCD%28x%2Cy%29%3Dprime)，那么，枚举每一个质数 p，得到：

![ans=\sum_p^{min(n,m)}[\sum_d^{min(n,m)}\mu (d) ((n/pd)*(m/pd))]](https://private.codecogs.com/gif.latex?ans%3D%5Csum_p%5E%7Bmin%28n%2Cm%29%7D%5B%5Csum_d%5E%7Bmin%28n%2Cm%29%7D%5Cmu%20%28d%29%20%28%28n/pd%29*%28m/pd%29%29%5D)

由于会超时，继续进行优化

设 T=pd，那么![ans=\sum_{i=1}^{min(n,m)}(n/T)*(m/T)(\sum_{p|T}\mu (T/p))](https://private.codecogs.com/gif.latex?ans%3D%5Csum_%7Bi%3D1%7D%5E%7Bmin%28n%2Cm%29%7D%28n/T%29*%28m/T%29%28%5Csum_%7Bp%7CT%7D%5Cmu%20%28T/p%29%29)

因此，可以先预处理所有的 T 对应的 ![\sum_{p|T}\mu (T/p)](https://private.codecogs.com/gif.latex?%5Csum_%7Bp%7CT%7D%5Cmu%20%28T/p%29) 的值

设 ![g(x)=\sum_{p|x}\mu (x/p)](https://private.codecogs.com/gif.latex?g%28x%29%3D%5Csum_%7Bp%7Cx%7D%5Cmu%20%28x/p%29)，sum(x) 是 g(x) 的前缀和。

则：若 k 整除 x，那么 ![\mu(kx)=0,g(kx)=\mu(x)](https://private.codecogs.com/gif.latex?%5Cmu%28kx%29%3D0%2Cg%28kx%29%3D%5Cmu%28x%29)；若 k 不整除 x，那么 ![\mu(kx)=-\mu(x),g(kx)=\mu(x)-g(x)](https://private.codecogs.com/gif.latex?%5Cmu%28kx%29%3D-%5Cmu%28x%29%2Cg%28kx%29%3D%5Cmu%28x%29-g%28x%29)

## 源代码

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<string>
#include<cstdlib>
#include<queue>
#include<set>
#include<map>
#include<stack>
#include<ctime>
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 10000005
#define MOD 123
#define E 1e-6
using namespace std;

long long mu[N],prime[N];
long long sum[N],g[N];
bool vis[N];
long long cnt;
void Get_mu(int n)
{
    mu[1]=1;
    cnt=0;
    memset(vis,0,sizeof(vis));

    for(int i=2;i<=n;i++)
    {
        if(!vis[i])
        {
            mu[i]=-1;
            prime[++cnt]=i;
        }
        for(int j=1;j<=cnt&&prime[j]*i<=n;j++)
        {
            vis[i*prime[j]]=1;
            if(i%prime[j])
                mu[prime[j]*i]=-mu[i];
            else
               break;
        }
    }

    for(int j=1;j<=cnt;j++)
        for(int i=1;i*prime[j]<=n;i++)
            g[i*prime[j]]+=mu[i];

    sum[0]=0;
    for(int i=1;i<=n;i++)
        sum[i]=sum[i-1]+g[i];
}

int main()
{

    int t;
    scanf("%d",&t);

    Get_mu(10000000);
    while(t--)
    {
        int n,m;
        scanf("%d%d",&n,&m);
        if(n>m)
            swap(n,m);

        long long ans=0;
        for(int i=1,last;i<=n;i=last+1)
        {
            last=min(n/(n/i),m/(m/i));
            ans+=(long long)((n/i)*(m/i)*(sum[last]-sum[i-1]));
        }
        printf("%lld\n",ans);
    }

    return 0;
}
```





