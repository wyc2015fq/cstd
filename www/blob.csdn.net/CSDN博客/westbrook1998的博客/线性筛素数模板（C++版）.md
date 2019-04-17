# 线性筛素数模板（C++版） - westbrook1998的博客 - CSDN博客





2018年11月12日 15:36:58[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：38








```
#include <bits/stdc++.h>
using namespace std;
const int N=1e7+50;
int p[N];
//true表示被筛，即不是素数
bool check[N];
int n,m,a;
void init(){
    int t;
    check[1]=true;
    for(int i=2;i<=n;i++){
        //素数，直接记录
        if(!check[i]){
            p[++p[0]]=i;
        }
        //p[0]记录的是素数个数
        //枚举现在已知的所有素数
        for(int j=1;j<=p[0];j++){
            //筛掉p[j]的倍数
            t=i*p[j];
            if(t>n){
                break;
            }
            check[t]=true;
            //保证i*p[j]这个数只被访问一次
            //要不是为i的倍数时访问，要不是为p[j]的倍数时访问
            if(i%p[j]==0){
                break;
            }
        }
    }
}
```



