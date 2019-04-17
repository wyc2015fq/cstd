# gym101061A——Cards - westbrook1998的博客 - CSDN博客





2018年10月24日 22:12:55[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：27








奇数肯定在第一个里，而偶数就不一定了，要判断这个偶数要除以多少次2才能变成奇数，如果次数是奇数次，比如1次，那肯定是第二个里了，因为第二个放的就是第一个的两倍嘛

```
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
int n;
ll x;
int main(void){
    scanf("%d",&n);
    while(n--){
        scanf("%lld",&x);
        if(x%2){
            printf("First Box\n");
        }
        else{
            int cnt=0;
            while(x%2==0){
                cnt++;
                x/=2;
            }
            if(cnt%2){
                printf("Second Box\n");
            }
            else{
                printf("First Box\n");
            }
        }
    }
    return 0;
}
```



