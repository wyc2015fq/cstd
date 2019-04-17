# 牛可乐发红包脱单ACM赛C——区区区间间间 - westbrook1998的博客 - CSDN博客





2018年11月02日 22:09:09[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：49标签：[单调栈](https://so.csdn.net/so/search/s.do?q=单调栈&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









求出每个子区间的最大值减最小值的和

可以化成求出每个子区间的最大值的和减去每个子区间最小值的和

又可以看成求每个数作为最大值能左右延伸多远，就直接用这个数乘以这个区间能组成多少个子区间（分区间是否跨过这个数来计数）

然后这个问题就可以用单调栈来解决了
```
#include <bits/stdc++.h>
using namespace std;
const int N=1e5+50;
typedef long long ll;
ll le[N],ri[N];
ll a[N];
int t,n;
ll solve(){
    memset(le,0,sizeof(le));
    memset(ri,0,sizeof(ri));
    stack<int> s,t;
    for(int i=1;i<=n;i++){
        while(s.size()>0 && a[i]>=a[s.top()]){
            s.pop();
        }
        if(s.size()==0){
            le[i]=1;
        }
        else{
            le[i]=s.top()+1;
        }
        s.push(i);
    }
    for(int i=n;i>=1;i--){
        while(t.size()>0 && a[i]>a[t.top()]){
            t.pop();
        }
        if(t.size()==0){
            ri[i]=n;
        }
        else{
            ri[i]=t.top()-1;
        }
        t.push(i);
    }
    ll ans=0;
    for(int i=1;i<=n;i++){
        //a[i]直接乘以以a[i]作为最大值的区间个数，分跨过a[i]和没跨过a[i]两种
        ans+=a[i]*(ri[i]-le[i]+(ri[i]-i)*(i-le[i]));
    }
    for(int i=1;i<=n;i++){
        printf("%lld ",le[i]);
    }
    printf("\n");
    return ans;
}
int main(void){
    scanf("%d",&t);
    while(t--){
        scanf("%d",&n);
        for(int i=1;i<=n;i++){
            scanf("%lld",&a[i]);
        }
        ll ans=solve();
        for(int i=1;i<=n;i++){
            a[i]=-a[i];
        }
        ans+=solve();
        printf("%lld\n",ans);
    }
    return 0;
}
```




