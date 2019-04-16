# Wannafly summer camp - 紫芝的博客 - CSDN博客





2018年09月28日 12:38:50[紫芝](https://me.csdn.net/qq_40507857)阅读数：201








## [2053: Birthday](http://newoj.acmclub.cn/problems/2053)

![](https://img-blog.csdn.net/20180827201811526?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

样例输入

```
3 3
1 2
1 2
1 2
```

样例输出

```
5
```

### 分析：

考虑费用流时把每个part拆成n个点，选择第i个点的代表为放置i块蛋糕和(i - 1)块蛋糕的时间差，这个时间差是递增的，因此在费用流的过程中必定会从小到大选择

具体建图：左边n个点代表n个蛋糕，右边m * n个点代表m个part,每个part拆成n个点。源点向每个左边的点连一条流量1费用0的边，每个右边的点向汇点连一条流量1费用0的编。每个蛋糕向可以放的两个part的所有点连边，连向第i个点的费用为i^2 - (i - 1)^2，流量为1。这样求最小费用流既为答案。

```cpp
#include <cstdio>
#include <cstring>
#include <vector>
#include <queue>
 
using namespace std;
 
const int INF = 0x3f3f3f3f;
const int MAXN = 5005; 
 
struct Edge{
	int value,flow,to,rev;
	Edge(){}
	Edge(int a,int b,int c,int d):to(a),value(b),flow(c),rev(d){}
};
 
vector<Edge> E[MAXN];
 
inline void Add(int from,int to,int flow,int value){
	E[from].push_back(Edge(to,value,flow,E[to].size()));
	E[to].push_back(Edge(from,-value,0,E[from].size()-1));
}
 
bool book[MAXN];//用于SPFA中标记是否在queue中 
int cost[MAXN];//存费用的最短路径 
int pre[MAXN];//存前节点 
int pree[MAXN];//存在前节点的vector中的下标 
 
bool Spfa(int from,int to){
	memset(book,false,sizeof book);
	memset(cost,INF,sizeof cost);
	book[from] = true;
	cost[from] = 0;
	queue<int> Q;
	Q.push(from);
	while(!Q.empty()){
		int t = Q.front();
		book[t] = false;
		Q.pop();
		for(int i=0 ; i<E[t].size() ; ++i){
			Edge& e = E[t][i];
			if(e.flow > 0 && cost[e.to] > cost[t] + e.value){
				cost[e.to] = cost[t] + e.value;
				pre[e.to] = t;
				pree[e.to] = i;
				if(book[e.to] == false){
					Q.push(e.to);
					book[e.to] = true;
				}
			}
		}
	}
	return cost[to] != INF;
}
 
int Work(int from,int to){
	int sum = 0;
	while(Spfa(from,to)){
		int mflow = INF;//SPFA找到的最短路径的最小容量 
		int flag = to;
		while(flag != from){
			mflow = min(mflow,E[pre[flag]][pree[flag]].flow);
			flag = pre[flag];
		}
		flag = to;
		while(flag != from){
			sum += E[pre[flag]][pree[flag]].value * mflow;
			E[pre[flag]][pree[flag]].flow -= mflow;
			E[flag][E[pre[flag]][pree[flag]].rev].flow += mflow;
			flag = pre[flag];
		}
	}
	return sum;
}
 
int main(){
	
	int N,M;
	while(scanf("%d %d",&N,&M) == 2){
		int a,b;
		for(int i=1 ; i<=N ; ++i){
			scanf("%d %d",&a,&b);
			Add(i,a+N,1,0);
			Add(i,b+N,1,0);
			Add(0,i,1,0); 
		}
		for(int i=1 ; i<=M ; ++i){
			for(int j=1 ; j<=99 ; j+=2){//99 = 2*50-1;
				Add(i+N,M+N+1,1,j);
			}
		} 
		printf("%d\n",Work(0,M+N+1));
		for(int i=0 ; i<=M+N+1 ; ++i)E[i].clear(); 
	}
	
	return 0;
}
```

## [2088: 电音之王](http://newoj.acmclub.cn/problems/2088)

![](https://img-blog.csdn.net/20180827171051892?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

样例输入

```
1
1 1 1 1 0 1000000007 10
```

样例输出
`904493530`
### 分析：

![S=(\prod f[i]) mod M (0\leq i\leq n)](https://private.codecogs.com/gif.latex?S%3D%28%5Cprod%20f%5Bi%5D%29%20mod%20M%20%280%5Cleq%20i%5Cleq%20n%29)

由于取模的常数为64bit整数，所以考虑使用**[O(1)快速乘](https://blog.csdn.net/qq_40507857/article/details/82110505)**，然后暴力求解即可

```cpp
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
ll ksc(ll x,ll y,ll mod)
{
    return (x*y-(ll)((long double)x/mod*y)*mod+mod)%mod;
}
ll m0,m1,c,M,k;
ll a0,a1;
int main()
{
    int T;
    scanf("%d",&T);
    while(T--){
       scanf("%lld%lld%lld%lld%lld%lld%lld",&a0,&a1,&m0,&m1,&c,&M,&k);
    ll ans=ksc(a0,a1,M);
    ll a2;
    for(int i=2;i<=k;i++)
    {
        a2=((ksc(m0,a1,M)+ksc(m1,a0,M))%M+c)%M;
        ans=ksc(ans,a2,M);
        a0=a1;
        a1=a2;
    }
    printf("%lld\n",ans);
    }
    return 0;
}
```

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
```





