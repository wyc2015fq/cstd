# Codeforces903D——Almost Difference - westbrook1998的博客 - CSDN博客





2018年11月06日 22:19:44[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：16标签：[前缀和																[思维](https://so.csdn.net/so/search/s.do?q=思维&t=blog)](https://so.csdn.net/so/search/s.do?q=前缀和&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








题意很简单，给一个函数，如果x y差的绝对值小于等于1，函数值为0，否则函数值为y-x

然后给一个数列，求$\sum f(a_i,a_j)$ 其中$i&lt;j$

首先暴力是不可能暴力的，n那么大，数列考虑一下前缀和，比如现在如果我们不考虑函数值为0的情况，也就是要求$\sum a_j-a_i$，那就简单了，记录前缀和，枚举每个数，然后累加$a_i*(i-1)-pre[i-1]$，而这题还有0的情况，那我们只要记录j前面有几个数的情况会满足$f(i,j)==0$，怎么记录这个呢，用一个map边计算边扫一遍来记录每个数出现的次数即可，然后对于a[i]来说，a[i]+1和a[i]-1的函数值就是0，也就是刚才上面那个多/少算了1，看这个map对应这个数在前面出现了多少次即可

然后有一个坑点就是会爆long long，要用long double
代码：

```
#include <bits/stdc++.h>
using namespace std;
const int N=200050;
typedef long long ll;
typedef long double ld;
int n;
ll a[N],sum[N];
map<int,int> cnt;
int main(void){
    scanf("%d",&n);
    for(int i=1;i<=n;i++){
        scanf("%lld",&a[i]);
        sum[i]=sum[i-1]+a[i];
    }
    ld ans=0;
    for(int i=1;i<=n;i++){
        //printf("%d\n",i);
        ans+=(ld)a[i]*(i-1)-sum[i-1];
        //printf("** %lld\n",a[i]*(i-1)-sum[i-1]);
        ans-=cnt[a[i]-1];
        ans+=cnt[a[i]+1];
        cnt[a[i]]++;
        //printf("--%lld--\n",ans);
    }
    printf("%.0Lf\n",ans);
    return 0;
}
```





