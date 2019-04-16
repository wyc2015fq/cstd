# HDU5780 gcd 欧拉函数 - 紫芝的博客 - CSDN博客





2018年09月22日 19:31:18[紫芝](https://me.csdn.net/qq_40507857)阅读数：65








# gcd

****Time Limit: 2000/1000 MS (Java/Others)    Memory Limit: 131072/131072 K (Java/Others)Total Submission(s): 732    Accepted Submission(s): 254****

**Problem Description**

Little White learned the greatest common divisor, so she plan to solve a problem: given x, n，

query ∑gcd(![x^a−1](https://private.codecogs.com/gif.latex?x%5Ea%25u22121)-1,![x^b](https://private.codecogs.com/gif.latex?x%5Eb)−1) (1≤a,b≤n)

**Input**

The first line of input is an integer T ( 1≤T≤300)

For each test case ，the single line contains two integers x and n ( 1≤x,n≤1000000)

**Output**

For each testcase, output a line, the answer mod 1000000007

**Sample Input**


```
5 

3 1 

4 2 

8 7 

10 5 

10 8
```

**Sample Output**


```
2 

24 

2398375 

111465 

111134466
```

**Source**

[BestCoder Round #85](http://acm.hdu.edu.cn/search.php?field=problem&key=BestCoder+Round+%2385&source=1&searchmode=source)

题意：

a的取值范围是[1,n],b的取值范围是[1,n]，求  ![gcd((x^a)-1,(x^b)-1)](https://private.codecogs.com/gif.latex?gcd%28%28x%5Ea%29-1%2C%28x%5Eb%29-1%29)  之和

分析：

首先，![gcd(x^a-1,x^b-1)=x^{gcd(a,b)}-1](https://private.codecogs.com/gif.latex?gcd%28x%5Ea-1%2Cx%5Eb-1%29%3Dx%5E%7Bgcd%28a%2Cb%29%7D-1)  ,问题转化为求1到n，最大公约数为d的个数 * (x^d)-1的和

ans=∑s[d]∗(x^​d​​−1)，记s[d]=最大公约数为d的个数，也就是1到n/d以内，互质的对数，显然是欧拉函数的前缀和

s[d]=2*(phi[1]+phi[2]+...+phi[n/d])-1

注意到：d不同，但是n/d一样，也就是s[d]可能有多个相同，比如 10/6 10/7 10/8 10/9 10/10，所以求s[d]相同的项，我们可以用等比公式求和(快速幂+逆元 ),直接进行除法会发生精度误差，所以考虑求逆元    ![sum=\frac{a1*(1-q^{n})}{1-q}](https://private.codecogs.com/gif.latex?sum%3D%5Cfrac%7Ba1*%281-q%5E%7Bn%7D%29%7D%7B1-q%7D)

所以我们只要找到每一段s[d]就可以 即 r=n/(n/d)，r为最后一个相同s[d]的下标,d为第一个s[d]相同的下标

![sum=x^{d1}+x^{d2}+x^{d3}+x^{d4}+x^{d5}+x^{d6}..........+x^{r}](https://private.codecogs.com/gif.latex?sum%3Dx%5E%7Bd1%7D&plus;x%5E%7Bd2%7D&plus;x%5E%7Bd3%7D&plus;x%5E%7Bd4%7D&plus;x%5E%7Bd5%7D&plus;x%5E%7Bd6%7D..........&plus;x%5E%7Br%7D)

![sum=\frac{x^{d}*(1-x^{n})}{1-x}](https://private.codecogs.com/gif.latex?sum%3D%5Cfrac%7Bx%5E%7Bd%7D*%281-x%5E%7Bn%7D%29%7D%7B1-x%7D)

由于第一个下标为d,最后一个下标为r,n=(r-d+1)

![sum=\frac{x^{d}*(x^{r-d+1}-1)}{x-1}](https://private.codecogs.com/gif.latex?sum%3D%5Cfrac%7Bx%5E%7Bd%7D*%28x%5E%7Br-d&plus;1%7D-1%29%7D%7Bx-1%7D)

```cpp
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int maxn=1e6+7;
#define mod 1000000007
ll phi[maxn];
void get_phi()
{
    for(int i=1;i<maxn;i++)
        phi[i]=i;
    for(int i=2;i<maxn;i++)
        if(phi[i]==i)
        for(int j=i;j<maxn;j+=i)
        phi[j]=phi[j]/i*(i-1);

    for(int i=3;i<maxn;i++)
        phi[i]=(phi[i]+phi[i-1])%mod;

    for(int i=2;i<maxn;i++)
        phi[i]=(2*phi[i]+1)%mod;
}
ll quickPow(ll a,ll b)
{
    ll res=1;
    while(b){
        if(b&1) res=(res*a)%mod;
        a=a*a%mod;
        b>>=1;
    }
    return res;
}
int main()
{
    get_phi();
    int T;
    scanf("%d",&T);
    while(T--){
        ll x,n;
        scanf("%lld%lld",&x,&n);
        if(x==1)
        {
            printf("0\n");
            continue;
        }
        ll ans=0;
        ll inv=quickPow(x-1,mod-2);//费马小定理求逆元
        for(int i=1;i<=n;i=n/(n/i)+1)
        {
            ll r=n/(n/i);//闭区间终点，i为闭区间起点
            ll part=((quickPow(x,i)*(quickPow(x,r-i+1)-1)%mod)*inv%mod-(r-i+1)+mod)%mod;//等比数列求和
            ans=(ans+(phi[n/i]*part)%mod+mod)%mod;
        }
        printf("%lld\n",(ans+mod)%mod);
    }
    return 0;
}
```





