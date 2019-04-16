# ProjectEuler500 【组合数学】【数论】 - 紫芝的博客 - CSDN博客





2018年11月18日 18:06:56[紫芝](https://me.csdn.net/qq_40507857)阅读数：1118
个人分类：[组合数学																[数论](https://blog.csdn.net/qq_40507857/article/category/7647322)](https://blog.csdn.net/qq_40507857/article/category/7984865)








## [Problem 500!!!](https://projecteuler.net/problem=500)

The number of divisors of 120 is 16.

In fact 120 is the smallest number having 16 divisors.

Find the smallest number with 2500500 divisors.

Give your answer modulo 500500507.

前500500个质数是第一个假的解，然后维护一个"乱七八糟的数据结构"里面存着第一个质数的平方，第二个质数的平方，...，然后每次把这个数据结构里最小的那个和当前解最大的单幂次质数做个比较，如果那个元素更小就替换，然后在数据结构里推进刚刚用来替换的那个形如p^k的数的平方......大概就这么个思路吧...

```cpp
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll mod = 500500507;
const ll mi = 500500+1;
const ll maxn = 2e7;//从2开始，到第500500个素数是7e6左右
ll a[4] = {2, 4, 8, 16};
ll prime[mi+100];
ll check[maxn], tot;
void init(){
    memset(check, 0, sizeof(check));
    prime[tot++] = 1;
    for (ll i = 2; i < maxn; ++i){
        if(!check[i]){
            prime[tot++] = i;
        }
        if(tot == mi){
            break;
        }
        for (ll j = 1; j < tot; ++j){
            if (i * prime[j] > maxn){
                break;
            }
            check[i*prime[j]] = 1;
            if (i % prime[j] == 0){
                break;
            }
        }
    }
}
//快速幂，返回a^b
ll Pow(ll a,ll b)
{
    ll ret=1;
    while(b){
        if(b&1) ret=(ret*a)%mod;
        a=(a*a)%mod;
        b>>=1;
    }
    return ret;
}

int main(){
    init();
    priority_queue<ll>que;
    que.push(1);
    ll Size = 1;
    ll Top = 4;
    for(ll i = 1; i < mi; i++){
        if(Size >= mi){
            if(prime[i] >= que.top()){
                break;
            }
            else{
                que.pop();
                que.push(prime[i]);
                for(ll j = 0; j < Top; j++){
                    if(Pow(prime[i], a[j]) >= prime[tot-1]){
                        Top = j;
                        break;
                    }
                    if(Pow(prime[i], a[j]) >= que.top()){
                        Top = j;
                        break;
                    }
                    else{
                        que.pop();
                        que.push(Pow(prime[i], a[j]));
                    }
                }
            }
        }
        else{
            que.push(prime[i]);
            Size++;
            for(ll j = 0; j < Top; j++){
                if(Pow(prime[i], a[j]) >= prime[tot-1]){
                    Top = j;
                    break;
                }
                if(Size >= mi){
                    if(Pow(prime[i], a[j]) >= que.top()){
                        Top = j;
                        break;
                    }
                    else{
                        que.pop();
                        que.push(Pow(prime[i], a[j]));
                    }
                }
                else{
                    if(Pow(prime[i], a[j]) >= prime[tot-1]){
                        Top = j;
                        break;
                    }
                    que.push(Pow(prime[i], a[j]));
                    Size++;
                }
            }
        }
    }
    ll ans = 1;
    while(!que.empty()){
        ans *= que.top();
        que.pop();
        ans%=mod;
    }
    cout<<ans<<endl;
    return 0;
}
```

```cpp
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll mod = 500500507;
const ll N = 500500;
const ll L =N*16;
ll np[L+1];

struct xpowy{
    ll x,y;
    bool operator <(xpowy &b){
        return log(x)*(y+1)>log(b.x)*(b.y+1);
    }
};
//快速幂，返回a^b
ll Pow(ll a,ll b)
{
    ll ret=1;
    while(b){
        if(b&1) ret=(ret*a)%mod;
        a=(a*a)%mod;
        b>>=1;
    }
    return ret;
}

int main(){
    vector<xpowy> v;
    for(ll n=2;n<=L;n++)
    if(!np[n]){
        for(ll x=n*n;x<=L;x+=n)
            np[x]=1;
        v.push_back({n,0});
    }
    make_heap(v.begin(),v.end());
    for(int i=0;i<N;i++){
        v[0].y=v[0].y*2+1;
        pop_heap(v.begin(),v.end());
        push_heap(v.begin(),v.end());
    }
    ll s=1;
    for(int i=0;i<v.size();i++)
        s=s*Pow(v[i].x,v[i].y)%mod;
    cout<<s;
}
```






