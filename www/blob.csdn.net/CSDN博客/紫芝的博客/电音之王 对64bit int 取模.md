# 电音之王 对64bit int 取模 - 紫芝的博客 - CSDN博客





2018年10月11日 21:53:51[紫芝](https://me.csdn.net/qq_40507857)阅读数：1733
个人分类：[解题模板](https://blog.csdn.net/qq_40507857/article/category/7565904)

所属专栏：[牛客刷题机](https://blog.csdn.net/column/details/29630.html)









链接：[https://www.nowcoder.com/acm/contest/205/B](https://www.nowcoder.com/acm/contest/205/B)

来源：牛客网

电音之王

时间限制：C/C++ 2秒，其他语言4秒

空间限制：C/C++ 262144K，其他语言524288K

64bit IO Format: %lld

## 题目描述

终于活成了自己讨厌的样子。

听说多听电音能加快程序运行的速度。

定义一个数列，告诉你a0,a1,m0,m1,c，定义an=m0an-1+m1an-2+c对所有n≥ 2。

求![](https://uploadfiles.nowcoder.com/files/20181004/305537_1538638356659_equation?tex=%5Cleft(%20%5Cprod_%7Bi%3D0%7D%5Ek%20a_i%20%5Cright)%20%5Cbmod%20M)

## 输入描述:

第一行一个整数T(1≤ T≤ 1000)，表示数据组数。
每组数据一行7个整数a0,a1,m0,m1,c,M,k，保证1≤ M≤ 1018,0≤ a0,a1,m0,m1,c< M, 2≤ k≤ 106，保证M为奇数。
保证。
## 输出描述:

对于每组数据，输出一行表示答案。


示例1

## 输入

复制

1
1 1 1 1 0 1000000007 10
## 输出

复制

904493530
```cpp
#include <stdio.h>
#include<vector>
#include<utility>
#include <assert.h>
using namespace std;
#define rep(i,a,n) for (int i=a;i<n;i++)
#define per(i,a,n) for (int i=n-1;i>=a;i--)
#define pb push_back
#define mp make_pair
#define all(x) (x).begin(),(x).end()
#define fi first
#define se second
#define SZ(x) ((int)(x).size())
typedef vector<int> VI;
typedef long long ll;
typedef pair<int,int> PII;
const ll mod=1000000007;
 
ll gcd(ll a,ll b) { return b?gcd(b,a%b):a;}
// head
  
typedef unsigned long long u64;
typedef __int128_t i128;
typedef __uint128_t u128;
int _,k;
u64 A0,A1,M0,M1,C,M;
  
struct Mod64 {
    Mod64():n_(0) {}
    Mod64(u64 n):n_(init(n)) {}
    static u64 init(u64 w) { return reduce(u128(w) * r2); }
    static void set_mod(u64 m) {
        mod=m; assert(mod&1);
        inv=m; rep(i,0,5) inv*=2-inv*m;
        r2=-u128(m)%m;
    }
    static u64 reduce(u128 x) {
        u64 y=u64(x>>64)-u64((u128(u64(x)*inv)*mod)>>64);
        return ll(y)<0?y+mod:y;
    }
    Mod64& operator += (Mod64 rhs) { n_+=rhs.n_-mod; if (ll(n_)<0) n_+=mod; return *this; }
    Mod64 operator + (Mod64 rhs) const { return Mod64(*this)+=rhs; }
    Mod64& operator -= (Mod64 rhs) { n_-=rhs.n_; if (ll(n_)<0) n_+=mod; return *this; }
    Mod64 operator - (Mod64 rhs) const { return Mod64(*this)-=rhs; }
    Mod64& operator *= (Mod64 rhs) { n_=reduce(u128(n_)*rhs.n_); return *this; }
    Mod64 operator * (Mod64 rhs) const { return Mod64(*this)*=rhs; }
    u64 get() const { return reduce(n_); }
    static u64 mod,inv,r2;
    u64 n_;
};
u64 Mod64::mod,Mod64::inv,Mod64::r2;
  
int main() {
    for (scanf("%d",&_);_;_--) {
        scanf("%llu%llu%llu%llu%llu%llu%d",&A0,&A1,&M0,&M1,&C,&M,&k);
        Mod64::set_mod(M);
        Mod64 a0(A0),a1(A1),m0(M0),m1(M1),c(C),ans(1),a2(0);
        for (int i=0;i<=k;i++) {
            ans=ans*a0;
            a2=m0*a1+m1*a0+c;
            a0=a1; a1=a2;
        }
        printf("%llu\n",ans.get());
    }
}
```





