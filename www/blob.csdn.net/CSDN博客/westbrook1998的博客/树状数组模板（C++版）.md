# 树状数组模板（C++版） - westbrook1998的博客 - CSDN博客





2018年11月13日 20:54:45[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：27








n是数组长度，a数组是原数列，c数组是辅助数组（树状数组，在第一种模板中表示前缀和，第二种模板中表示区间最大值），也就是树状数组的节点

### 单点更新和区间求和

```
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1e5+50;
ll a[N],c[N],n,x;
int lowbit(int x){
    return x&-x;
}
void update(int i,ll d){
    while(i<=n){
        c[i]+=d;
        i+=lowbit(i);
    }
}
ll getSum(int i){
    ll ans=0;
    while(i>0){
        ans+=c[i];
        i-=lowbit(i);
    }
    return ans;
}
int main(void){
    return 0;
}
```

### 单点更新和区间最值

```
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1e5+50;
ll a[N],c[N],n,x;
int lowbit(int x){
    return x&-x;
}
//这里要先更新a数组，若是有插入操作，也要更新n
void update(int x){
    while(x<=n){
    	//c[x]记录max(x...x-lowbit(x)+1)
        c[x]=a[x];
        int lx=lowbit(x);
        //每次加1复杂度太高，因为x只能从x-2^k转移而来(lowbit的性质)
        for(int i=1;i<lx;i<<=1){
            c[x]=max(c[x],c[x-i]);
        }
        x+=lowbit(x);
    }
}
ll getMax(int l,int r){
    ll ans=0;
    while(l<=r){
        ans=max(ans,a[r]);
        for(r--;r-lowbit(r)>=l;r-=lowbit(r)){
            ans=max(ans,c[r]);
        }
    }
    return ans;
}
int main(void){
    return 0;
}
```



