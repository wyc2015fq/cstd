# 线性筛模板（C++版） - westbrook1998的博客 - CSDN博客





2018年11月16日 18:37:26[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：64








分别是线性筛素数，欧拉函数，莫比乌斯函数，约数个数，约数和，其中有些都是可以一起筛的

这里给出分别的模板

### 1. 素数

最基本的，后面基本都要用到素数的筛法

```
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

### 2. 欧拉函数

```
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
```

### 3. 莫比乌斯函数

```
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
```

### 4. 约数个数

```
const int N=1e7+50;
//num记录最小质因子指数
int d[N],num[N],p[N];
bool check[N];
void init(){
    int t;
    d[1]=1;
    check[1]=true;
    for(int i=2;i<=N;i++){
        if(!check[i]){
            d[i]=2;
            num[i]=1;
            p[++p[0]]=i;
        }
        for(int j=1;j<=p[0];j++){
            t=i*p[j];
            if(t>N){
                break;
            }  
            check[t]=true;
            //t比i多了一个最小质因子次数
            if(i%p[j]==0){
                num[t]=num[i]+1;
                d[t]=d[i]/(num[i]+1)*(num[i]+2);
            }else{
                //t比i多了一个不同的质因子，次数为1，根据公式得出两倍关系
                num[t]=1;
                d[t]=d[i]*2;
            }
        }
    }
}
```

### 5. 约数和

$σ(x)=(1+p_1^1+p_1^2+…+p_1^{a1})(1+p_2^1+p_2^2+…+p_2^{a2})…(1+p^1+p_n^2+…+p_n^{an})$

```
const int N=1e7+50;
//prod记录一个以1为首项，i的最小质因子为公比的等比数列和
int prod[N],sumd[N],p[N];
bool check[N];
void init(){
    int t;
    prod[1]=sumd[1]=1;
    check[1]=true;
    for(int i=2;i<=N;i++){
    	//素数因子只有1和本身
        if(!check[i]){
            prod[i]=sumd[i]=i+1;
            p[++p[0]]=i;
        }
        for(int j=1;j<=p[0];j++){
            t=i*p[j];
            if(t>N){
                break;
            }
            check[t]=true;
            if(i%p[j]==0){
            	//t中p[j]的次数比i中的多一次，更新prod
                prod[t]=prod[i]*p[j]+1;
                //类似约数个数的一个更新
                sumd[t]=sumd[i]/prod[i]*prod[t];
            }else{
            	//p[j]为t的最小质因子，由公式可得
                prod[t]=p[j]+1;
                sumd[t]=sumd[i]*(p[j]+1);
            }
        }
    }
}
```



