# Codeforces1064B——Equations of Mathematical Magic - westbrook1998的博客 - CSDN博客





2018年10月14日 20:29:53[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：45








> 
给定a，求方程$a-(a 异或 x)-x==0$的非负数解的个数，方程其实就是$a-x==a异或x$


找规律，发现都是2的幂，队友直接看出是a中1的个数是n，则2^n

（不会证明，不过可以看出按位减法和异或的关系，0-0 和0异或0是不同的，其他都是相同的，所以找出a中1的位数有n个，所以x就由这n个1中选出几个来与a中的1对应异或或者相减即可，那么组合就有2^n中情况

代码：

```
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <cmath>
using namespace std;
int main(void){
    // for(int a=1;a<=100;a++){
    //     int sum=0;
    //     for(int x=0;x<=a;x++){
    //         if(a-(a^x)-x==0){
    //             sum++;
    //         }
    //     }
    //     printf("%d %d\n",a,sum);
    // }
    int t;
    scanf("%d",&t);
    long long a;
    while(t--){
        scanf("%I64d",&a);
        int s=0;
        while(a){
            if(a&1){
                s++;
            }
            a/=2;
        }
        //printf("%d\n",s);
        printf("%.0lf\n",pow(2,s));
    }
    return 0;
}
```



