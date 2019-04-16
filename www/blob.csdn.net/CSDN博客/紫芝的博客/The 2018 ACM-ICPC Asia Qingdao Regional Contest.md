# The 2018 ACM-ICPC Asia Qingdao Regional Contest - 紫芝的博客 - CSDN博客





2018年11月06日 22:51:10[紫芝](https://me.csdn.net/qq_40507857)阅读数：363








## [C - Flippy Sequence](https://cn.vjudge.net/problem/ZOJ-4060)

## [ZOJ - 4060](https://cn.vjudge.net/problem/2009622/origin)

题意：

给出A和B两个01串，通过两次反转使得A串变成B串，问有多少种方法（这个反转指的是连续的一段区间）

分类讨论：
- 两个串完全一样，第一次可以选择区间【L，R】反转，第二次选择区间【L，R】反转；这样的区间：长度为N的区间有1个，长度为N-1的区间有2个，长度为N-3的区间有3个。。。。长度为3的区间有N-3个，长度为2的区间有N-2个，长度为1的区间有N个；显然，构成首项为1，公差为1的等差数列，求和为 ![\frac{N(N+1)}{2}](https://private.codecogs.com/gif.latex?%5Cfrac%7BN%28N&plus;1%29%7D%7B2%7D)
- 两个串有一个区间不一样（区间内每一位都不同），设为【L，R】，①我们可以在区间内任选一点pos，反转【L，pos】和【pos+1，R】，共有R-L种；②可以在区间【1，L-1】内任选一点pos，反转【pos，R】和【pos，L-1】，共有L-1种；可以在区间【R+1，N】内任选一点pos，反转【L，pos】和【R+1，pos】，共有N-R种。所以，这种情况一共有L-1+R-L+N-R种
- 两个区间不一样：设为【L1，R1】和【L2，R2】，我们可以反转【L1，R1】和【L2，R2】，【L2，R2】和【L1，R1】，【L1，R2】和【R1，L2】，【R1，L2】和【L1，R2】，【L1，L2】和【R1，R2】，【R1，R2】和【L1，L2】，共6种
- 三个及三个以上，有0种

![](https://img-blog.csdnimg.cn/20181106225925436.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3,size_16,color_FFFFFF,t_70)

```cpp
#include <cstdio>
#include <cstring>
#include <algorithm>
#include<iostream>
using namespace std;
typedef long long LL;
const int maxn=1e6+10;
char s[maxn],t[maxn];
int main() {
    int T;
    scanf("%d",&T);
    int ca=1;
    while((T--)!=0) {
       LL n;
       scanf("%lld",&n);
        scanf("%s%s",s+1,t+1);
        int ans=0;//不同的区间数
        bool flag=false;
        for(int i=1;i<=n;i++){
            int x=s[i]-'0';
            int y=t[i]-'0';
            if(x!=y&&!flag){
                flag=true;
                ans++;
                continue;
            }
            if(x==y)
            flag=false;
        }
        //printf("ans=%d\n",ans);

        if(ans==0){
            printf("%lld\n",n*(n+1)/2);
            continue;
        }

        if(ans==1){
            int l=0,r=0;
            bool first=true;
            for(int i=1;i<=n;i++){
                if(s[i]!=t[i]){
                    if(first)
                        first=false,l=i;
                    else
                        r=i;
                }
            }
            //printf("l=%d r=%d\n",l,r);
            int left=l-1,right=n-r,mid=r-l;
            printf("%d\n",2*(left+right+mid));
            continue;
        }

        if(ans==2){
            printf("6\n");
            continue;
        }

        if(ans>=3){
            printf("0\n");
            continue;
        }
    }
    return 0;
}
```

## [J - Books](https://cn.vjudge.net/problem/ZOJ-4067)

## [ZOJ - 4067](https://cn.vjudge.net/problem/2009553/origin)

题意：

从N本书里买M本，买书的规则如下：

    从左到右扫描N本书，如果当前钱包里的钱大于等于书的价格，就一定要买下来（必须买）

问最多能带多少钱，必须是非负整数；如果没有方案，就输出impossible；如果可以带无限多就输出Reachman。

贪心：
- 先数一下有多少个0，即不花钱就可以得到的书，如果cnt>M,输出impossible
- 如果N等于M，输出Reachman；
- 在N-cnt本书里买M-cnt本书，对于剩下的书求最小值MIN

```cpp
#include <cstdio>
#include <cstring>
#include <algorithm>
#include<iostream>
using namespace std;
typedef long long LL;
const int maxn=1e5+10;
int a[maxn];
int main() {
    int T;
    scanf("%d",&T);
    int ca=1;
    while((T--)!=0) {
       int n,m;
       scanf("%d%d",&n,&m);

       int zero=0;
       int N=1;//非0个数
       for(int i=1;i<=n;i++){
        int x;
        scanf("%d",&x);
       if(x==0) zero++;
        else    a[N++]=x;
       }
       if(n==m){
        printf("Richman\n");
        continue;
       }
       if(zero>m){
        printf("Impossible\n");
        continue;
       }
       m-=zero;
       LL sum=0;
        for(int i=1;i<=m;i++)
            sum+=a[i];
        int MIN=1e9;
        for(int i=m+1;i<N;i++)
            MIN=min(MIN,a[i]);
            sum=sum+MIN-1;
        printf("%lld\n",sum);
    }
    return 0;
}
```

## [M - Function and Function](https://cn.vjudge.net/problem/ZOJ-4070)

## [ZOJ - 4070](https://cn.vjudge.net/problem/2011014/origin)

日常签到

```cpp
#include <cstdio>
#include <cstring>
#include <algorithm>
#include<iostream>
using namespace std;
typedef long long LL;
int f[]= {1,0,0,0,1,0,1,0,2,1};
LL F(LL x) {
    LL ans=0;
    while(x>0) {
        ans+=f[(int) (x%10)];
        x/=10;
    }
    return ans;
}
int main() {
    int T;
    scanf("%d",&T);
    int ca=1;
    while((T--)!=0) {
       LL x,k;
       scanf("%lld%lld",&x,&k);
        while(x>=2&&k>0) {
            x=F(x);
            k--;
        }
        if(k%2==1)	x^=1;
        printf("%lld\n",x);
    }
    return 0;
}
```





