# 线性筛莫比乌斯函数（C++版） - westbrook1998的博客 - CSDN博客





2018年11月14日 23:42:11[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：71








```
#include <bits/stdc++.h>
using namespace std;
const int N=1e7+50;
//同时筛出素数和莫比乌斯函数
int p[N],miu[N];
bool check[N];
int pre[N];
void init(){
    int t;
    miu[1]=1;
    check[1]=true;
    for(int i=2;i<=N;i++){
        if(!check[i]){
            p[++p[0]]=i;
            miu[i]=-1;
        }
        for(int j=1;j<=p[0];j++){
            t=i*p[j];
            if(t>N){
                break;
            }  
            check[t]=true;
            //有平方因子，函数值为0
            if(i%p[j]==0){
                miu[t]=0;
            }else{
                //质因子数量改变，符号改变
                miu[t]=-miu[i];
            }
        }
    }
}
int main(void){
    init();
    return 0;
}
```



