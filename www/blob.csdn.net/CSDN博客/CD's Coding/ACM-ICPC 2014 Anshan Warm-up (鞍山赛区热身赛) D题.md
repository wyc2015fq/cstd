# ACM-ICPC 2014 Anshan Warm-up (鞍山赛区热身赛) D题 - CD's Coding - CSDN博客





2014年10月18日 19:23:36[糖果天王](https://me.csdn.net/okcd00)阅读数：944












|43|53|重庆大学 Screen/42|1|31|1/-|-/-|-/-|1/31|1/2|
|----|----|----|----|----|----|----|----|----|----|

白气球一枚入手，吾辈是第30个写出White的~ 在队友们读完赛题之前1A掉了D题拿到White

 不过好可惜……我们未能攻破第二题的装甲……

D虽然全场到场的基本没有做不出来的，不过想了想还是拿来说一说吧，毕竟能水一篇博文嘛~

题意有点简单就不多说啦……

好吧你们都猜到我还是会说的……

![](https://img-blog.csdn.net/20141018191824915?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


【吐槽】看呐看呐！！！！Asia拼写错了有木有！写成Aisa了呢~~~^_^

咳咳……就是说给你一个n，就是两个数的和啦，问分成两个数，他们的最小公倍数最大是多少，输出那个最大的最小公倍数。

那么我们都知道(x+i)(x-i)=x*x-i*i

所以越靠近越好咯~

·奇数的话不用说，分成一奇数和一偶数，肯定互质不解释~

·偶数咋办呢，咱可千万别除以二然后一个加一一个减一哦，有可能是俩偶数啊！

···那么除以二之后如果是偶数，那就一个加一一个减一，肯定互质（啥，又问为什么……那个辗转相除知道不？大的减去小的等于2，小的是奇数，互质）

···如果是奇数，那么……好吧我们继续加二减二……加三减三……直到互质为止……

哎你等会别去敲代码啊！ 不是互质了就行了的，这期间如果有谁的最小公倍数比你大咋办呢！

弄个maxv存着？哎对~~~^_^

那么代码就是这样：



```cpp
#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>

using namespace std;
typedef long long ll;

ll gcd(ll a,ll b)   //a>=b
{
    if(a%b==0 || b==1) return b;
    ll x=a-(a/b)*b,y=b;
    if(x<y)
    {
        ll tmp=x;
        x=y;y=tmp;
    }
    return gcd(x,y);

}

ll lcm(ll a,ll b)
{
    ll x=a,y=b;
    return x/gcd(x,y)*y;
}

bool judge(ll n,ll x)
{
    if(gcd(n,x)==1)return true;
    else return false;
}

int main()
{
    ll T;
    cin>>T;
    while(T--)
    {
        ll n,maxv=0;   cin>>n;
        if(n==2) cout<<1<<endl;
        else if(n%2) cout<<(n/2)*(n/2+1)<<endl;
        else if(n%4==0) cout<<(n/2+1)*(n/2-1)<<endl;
        else for(ll i=n/2;i>=1;)
        {
            if(judge(n-i,i))
            {
                cout<<max(i*(n-i),maxv)<<endl;
                break;
            }
            else
            {
                ll vnow=lcm(n-i,i);
                if(maxv< vnow) maxv=vnow;
            }
            i--;
        }
    }
    return 0;
}
```





