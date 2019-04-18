# 单行dp复习hdu1087 - weixin_33985507的博客 - CSDN博客
2017年04月11日 19:28:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3

我写的想法是每个dp【i】都是前dp【i】的最大值
dp【i】就等于前全部dp【0。。。i-1】的最大值加上dp【i】
最大值是一个中间变量
最大值得选取条件就是序列的值大小都是递增的，也就是a[i]>a[前面的]
```cpp
#include<cstdio>
#include<algorithm>
#include<cstring>
using namespace std;
typedef long long LL;
LL a[1000+100];
LL dp[1100];
int main()
{
    int n;
    while(scanf("%d",&n),n!=0){
        LL maxx=-4294967296;
       for(int i=0;i<n;i++) scanf("%lld",&a[i]);
       for(int i=0;i<n;i++){
        LL x=-4294967296;
        for(int j=0;j<i;j++)
            if(a[i]>a[j]&&dp[j]>x) x=max(dp[j],x);
        if(x == -4294967296) x=0;
        dp[i]=x+a[i];
        if(dp[i] > maxx) maxx=dp[i];
       }
    printf("%lld\n",maxx);
    }
    return 0;
}
```
看了别人的代码，也就是把我写的x中间变量变成了b[i]
求dp【i】的时候b【i】是无用的。能够用来存中间值
就省了点代码，原理是一样的
```cpp
#include<cstdio>
#include<algorithm>
#include<cstring>
using namespace std;
int a[1100],b[1100];
int main()
{
    int n;
    while(scanf("%d",&n)&&n!=0){
        memset(b,0,sizeof(b));
        int maxx=-4294967296;
        for(int i=0;i<n;i++) scanf("%d",&a[i]);
        for(int i=0;i<n;i++){
            b[i]=a[i];
                for(int j=0;j<i;j++)
            {
                if(a[i]>a[j]&&b[i]<a[i]+b[j]) b[i]=b[j]+a[i];
                if(b[i]>maxx) maxx=b[i];
            }
        }
        printf("%d\n",maxx);
    }
    return 0;
}
```
