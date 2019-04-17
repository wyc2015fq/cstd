# Wannafly挑战赛27——A.灰魔法师 - westbrook1998的博客 - CSDN博客





2018年10月27日 09:03:04[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：41








坑爹的签到题，差点做不出来

给一个数组，求其中两个数相加为完全平方数的对数

n是1e5

暴力会超时的，所以先预处理出2*1e5内的完全平方数，然后每输入一个数，就标记下它需要和哪个数相加才能凑成完全平方数，然后后面的每读入一个除了要记录，还要看这个数是否前面已经被记录过，就是加上它就能凑成完全平方数，直接加上num[a[i]]，然后还要开long long
```
#include <bits/stdc++.h>
using namespace std;
int n;
const int N=1e6+50;
int a[N];
int Pow[N];
int num[N];
int main(void){
    int k=0;
    for(int i=2;i*i<N;i++){
        Pow[k++]=i*i;
        //printf("%d %d\n",i,Pow[k-1]);
    }
    long long cnt=0;
    scanf("%d",&n);
    for(int i=0;i<n;i++){
        scanf("%d",&a[i]);
        if(num[a[i]]){
            cnt+=num[a[i]];
        }
        for(int j=0;j<k;j++){
            num[Pow[j]-a[i]]++;
        }
    }
    printf("%lld\n",cnt);
    return 0;
}
```




