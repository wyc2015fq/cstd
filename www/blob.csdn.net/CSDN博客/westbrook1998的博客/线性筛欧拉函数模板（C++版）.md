# 线性筛欧拉函数模板（C++版） - westbrook1998的博客 - CSDN博客





2018年11月14日 23:41:03[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：44








```
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1e7+50;
int p[N],phi[N];
bool check[N];
//同时筛出素数和欧拉函数
void init(){
    int t;
    check[1]=true;
    phi[1]=1;
    for(int i=2;i<=N;i++){
        if(!check[i]){
            p[++p[0]]=i;
            phi[i]=i-1;
        }
        for(int j=1;j<=p[0];j++){
            t=i*p[j];
            if(t>N){
                break;
            } 
            check[t]=true;
            //t拥有多个相同质因子(p[j]至少就2次)
            if(i%p[j]==0){
                //i是p[j]的倍数，那t和i的质因子相同，由欧拉函数计算式可得两者只差一个系数
                phi[t]=phi[i]*p[j];
            }else{
                //欧拉函数是积性函数
                phi[t]=phi[i]*(p[j]-1);
            }
        }
    }
}
int main(void){
    init();
    int t,n;
    scanf("%d",&t);
    while(t--){
        scanf("%d",&n);
        printf("%d\n",phi[n]);
    }
    return 0;
}
```



