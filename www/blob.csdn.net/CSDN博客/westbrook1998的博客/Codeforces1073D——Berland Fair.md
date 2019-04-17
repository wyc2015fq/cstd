# Codeforces1073D——Berland Fair - westbrook1998的博客 - CSDN博客





2018年10月27日 15:33:41[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：109








第一圈读取所有数字的时候就可以去除那些加上sum大于T的了

然后记录去除后第一圈的和sum，直接取模

然后接下来也是重复同样的操作，当循环一次之后没有任何一个可以买的就退出while
```
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
int n;
ll T;
const int N=2*1e5+50;
int a[N];
bool vis[N];
int x;
int main(void){
    //freopen("data.txt","r",stdin);
    scanf("%d%lld",&n,&T);
    ll sum=0;
    int cnt=0;
    for(int i=0;i<n;i++){
        scanf("%d",&x);
        if(sum+x<=T){
            a[cnt++]=x;
            sum+=x;
        }
    }
    ll ans=0;
    if(sum>0){
        ans=T/sum*cnt;
        T%=sum;
    }
    while(T){
        bool flag=false;
        sum=0;
        int tmp=0;
        for(int i=0;i<cnt;i++){
            if(sum+a[i]<=T){
                sum+=a[i];
                tmp++;
                flag=true;
            }
        }
        if(!flag){
            break;
        }
        ans+=(T/sum*tmp);
        T%=sum;
    }
    printf("%lld\n",ans);
    return 0;
}
```




