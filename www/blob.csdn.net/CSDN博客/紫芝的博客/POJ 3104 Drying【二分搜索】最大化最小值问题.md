# POJ 3104 Drying【二分搜索】最大化最小值问题 - 紫芝的博客 - CSDN博客





2018年11月15日 17:11:44[紫芝](https://me.csdn.net/qq_40507857)阅读数：454








### 题意

有N件衣服，每件衣服的函数量为ai，每分钟可以自然脱水1单位；有一个脱水机，每次只能用于一件衣服，每分钟脱水K单位，脱水时，不自然风干，求所有衣服全部脱水的最短时间

### 分析

二分查找最短时间，下界为1，上界为N件衣服中最大含水量，全部脱水的最小时间mid
- 对于每一件衣服，若ai小于等于mid，则让其自然风干
- 若ai大于mid，则采用脱水机[X1]分钟+自然风干[X2]分钟的方法，那么mid=X1+X2，ai<=x1*k+x2，即x1>=(ai-mid)/(k-1)
- (ai-mid)/(k-1)向上取整就是这件衣服脱水的最短时间

```cpp
#include<stdio.h>
#include<iostream>
#include<string.h>
#include<queue>
#include<cstdio>
#include<string>
#include<math.h>
#include<algorithm>
#include<map>
#include<set>
#include<stack>
#define mod 998244353
#define INF 0x3f3f3f3f
#define eps 1e-6
using namespace std;
typedef long long ll;
using namespace std;
const int maxn=1e5+10;
ll a[maxn];
ll k;
int n;
bool check(ll mid){
    ll sum=0;
    for(int i=1;i<=n;i++){
        if(a[i]>mid)
        sum+=(ll)ceil(1.0*(a[i]-mid)/(k-1));
    }
    return sum>mid;
}
int main(){
    while(scanf("%d",&n)!=EOF){
           ll MAX=-1;
        for(int i=1;i<=n;i++){
            scanf("%lld",&a[i]);
            MAX=max(MAX,a[i]);
        }
        scanf("%lld",&k);
        if(k==1){
            printf("%lld\n",MAX);
            continue;
        }
        ll right=MAX,left=1;
        while(left<=right){
            ll mid=(left+right)>>1;
            if(check(mid))
                left=mid+1;
            else
                right=mid-1;
        }
        printf("%lld\n",left);
    }
    return 0;
}
/*
3
2 3 9
5
3
2 3 6
5
*/
```





