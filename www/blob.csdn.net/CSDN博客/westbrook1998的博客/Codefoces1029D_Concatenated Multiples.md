# Codefoces1029D_Concatenated Multiples - westbrook1998的博客 - CSDN博客





2018年12月16日 21:57:12[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：44








给n个数和一个k

求多少种组合方式使得两个数相连之后%k为0

也就是$(a[i]+10^{len(a[j])}+a[j])\%k==0$可以化为$(a[i]+10^{len(a[j])}\%k+a[j]\%k)==k || ==0$

直接看代码,先预处理出$10^{len(a[j])}\%k$和$a[j]\%k$,然后枚举a[i]和len(a[j])也就能算出$a[i]+10^{len(a[j])}\%k$,然后二分查找$k-a[i]+10^{len(a[j])}\%k$,得到的这个区间就是答案,加上即可,判断一下是否是自身连自身,如果是就再减1

代码:
```
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=2e5+50;
const int LOGN=11;
int n,k;
int a[N],len[N];
vector<int> rems[LOGN];
int pw[LOGN];
int main(void){
    scanf("%d%d",&n,&k);
    for(int i=0;i<n;i++){
        scanf("%d",&a[i]);
    }
    //预处理10^len(a[j])%k
    pw[0]=1;
    for(int i=1;i<LOGN;i++){
        pw[i]=pw[i-1]*10%k;
    }
    //预处理对应长度的a[j]%k
    for(int i=0;i<n;i++){
        int x=a[i];
        while(x){
            len[i]++;
            x/=10;
        }
        rems[len[i]].push_back(a[i]%k);
    }
    for(int i=0;i<LOGN;i++){
        sort(rems[i].begin(),rems[i].end());
    }
    ll ans=0;
    //枚举a[i]
    for(int i=0;i<n;i++){
        for(int j=1;j<=LOGN;j++){
            //枚举a[i]*10^len(a[j])%k
            int rem=(a[i]*ll(pw[j]))%k;
            //二分查找剩下的部分 a[j]%k
            int xrem=(k-rem)%k;
            auto l=lower_bound(rems[j].begin(),rems[j].end(),xrem);
            auto r=upper_bound(rems[j].begin(),rems[j].end(),xrem);
            //这区间内的a[j]%k都满足条件
            ans+=(r-l);
            //判断是否自己连了自己
            if(len[i]==j && (rem+a[i]%k)%k==0){
                ans--;
            }
        }
    }
    printf("%lld\n",ans);
    return 0;
}
```




