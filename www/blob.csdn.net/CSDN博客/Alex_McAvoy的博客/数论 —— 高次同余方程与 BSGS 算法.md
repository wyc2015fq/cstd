# 数论 —— 高次同余方程与 BSGS 算法 - Alex_McAvoy的博客 - CSDN博客





2019年03月20日 20:39:07[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：32








# 【概述】

BSGS（Baby Step Giant Step）算法，又称大小步算法，其主要用于解形如 ![A^x\equiv B(mod\:\:C)](https://private.codecogs.com/gif.latex?A%5Ex%5Cequiv%20B%28mod%5C%3A%5C%3AC%29) 的高次同余方程中的 x，其核心思想是**分块**。

当 A 与 C 互质时，通过费马小定理：![A^{C-1} \equiv 1(mod\:\:C)](https://private.codecogs.com/gif.latex?A%5E%7BC-1%7D%20%5Cequiv%201%28mod%5C%3A%5C%3AC%29)

可知，当 ![x\geqslant C-1](https://private.codecogs.com/gif.latex?x%5Cgeqslant%20C-1) 时，会出现一个循环节，于是就能保证答案 x 若存在，必然有 ![x < C-1](https://private.codecogs.com/gif.latex?x%20%3C%20C-1)

因此，当 C 比较小时，可使用暴力，直接令从 0 枚举到 C-1，检验其是否为方程的解，而当 C 比较大时，使用暴力会 TLE，此时可以采用 BSGS 算法来求解 x，其时间复杂度是 ![\sqrt n](https://private.codecogs.com/gif.latex?%5Csqrt%20n) 级别的

朴素的 BSGS 算法只能处理 C 是质数的情况，扩展的 BSGS 通过同余性质消因子来解决 C 不是质数的情况。

# 【BSGS 算法】

## 1.算法思想

根据分块思想，设置一个变量 ![size=ceil(\sqrt {C})](https://private.codecogs.com/gif.latex?size%3Dceil%28%5Csqrt%20%7BC%7D%29)，此时使用 ceil() 函数向上取整，可以保证 ![size*size\geqslant C](https://private.codecogs.com/gif.latex?size*size%5Cgeqslant%20C)，从而避免遗漏答案。

不难发现，此时 x 可表示为：![x=i*size+j,(i,j\in N,j<size)](https://private.codecogs.com/gif.latex?x%3Di*size&plus;j%2C%28i%2Cj%5Cin%20N%2Cj%3Csize%29)

那么 ![A^x\equiv B(mod\:\:C)](https://private.codecogs.com/gif.latex?A%5Ex%5Cequiv%20B%28mod%5C%3A%5C%3AC%29) 就转为 ![A^{(i*size+j)}\equiv B(mod\:\:C)](https://private.codecogs.com/gif.latex?A%5E%7B%28i*size&plus;j%29%7D%5Cequiv%20B%28mod%5C%3A%5C%3AC%29)

移项，得：![A^j\equiv A^{-(i*size)}*B(mod\:\:C)](https://private.codecogs.com/gif.latex?A%5Ej%5Cequiv%20A%5E%7B-%28i*size%29%7D*B%28mod%5C%3A%5C%3AC%29)

此时对左边的 j 从 0 枚举到 size-1，将  ![(A^j \:mod\: C,j)](https://private.codecogs.com/gif.latex?%28A%5Ej%20%5C%3Amod%5C%3A%20C%2Cj%29) 加入 Hash 表中（这一步是 Baby Step）

再对右边进行枚举 ![A^{-(i*size)}*B(mod\:\:C)](https://private.codecogs.com/gif.latex?A%5E%7B-%28i*size%29%7D*B%28mod%5C%3A%5C%3AC%29)（这一步是 Giant Step），然后再从 Hash 表中查找是否有这个值，若有的话，则得到一组 (i,j)，那么可得到正确解：![x=i*size+j](https://private.codecogs.com/gif.latex?x%3Di*size&plus;j)

## 2.实现

```cpp
struct HashMap{//哈希表
    static const int Hash=999917,maxn=46340;
    int num,link[Hash],son[maxn+5],next[maxn+5],w[maxn+5];
    int top,Stack[maxn+5];
    void clear(){//清空表
        num=0;
        while(top)
            link[Stack[top--]]=0;
    }
    void add(int x,int y){//添加键值元素
        son[++num]=y;
        next[num]=link[x];
        w[num]=INF;
        link[x]=num;
    }
    bool count(int y){//判断表中是否有对应值
        int x=y%Hash;
        for(int j=link[x];j;j=next[j])
            if(y==son[j])
                return true;
        return false;
    }
    int &operator [](int y){//获取键的对应值
        int x=y%Hash;
        for(int j=link[x];j;j=next[j])
            if(y==son[j])
                return w[j];
        add(x,y);
        Stack[++top]=x;
        return w[num];
    }
}hashMap;

int extendedGCD(int x,int y,int &a,int &b){
    if(y==0){
        a=1;
        b=0;
        return x;
    }

    int temp;
    int gcd=extendedGCD(y,x%y,a,b);
    temp=a;
    a=b;
    b=temp-x/y*b;
    return gcd;
}
int BSGS(int A,int B,int C){
    //三种特判
    if(C==1){
        if(!B)
            return A!=1;
        return -1;
    }
    if(B==1){
        if(A)
            return 0;
        return -1;
    }
    if(A%C==0){
        if(!B)
            return 1;
        return -1;
    }

    hashMap.clear();
    int Size=ceil(sqrt(C)),D=1,Base=1;
    for(int i=0;i<=Size-1;i++){//将A^j存进哈希表
        hashMap[Base]=min(hashMap[Base],i);//存储最小的
        Base=((LL)Base*A)%C;
    }
    for(int i=0;i<=Size-1;i++){//扩展欧几里得求A^j
        int x,y;
        int gcd=extendedGCD(D,C,x,y);//求出x、y
        x=((LL)x*B%C+C)%C;
        if(hashMap.count(x))//找到答案
            return i*Size+hashMap[x];
        D=((LL)D*Base)%C;
    }
    return -1;//无解
}

int main(){
    int A,B,C;
    while(scanf("%d%d%d",&A,&B,&C)!=EOF&&(A+B+C)){
        int res=BSGS(A,B,C);
        if(res==-1)
            printf("No Solution\n");
        else
            printf("%d\n",res);
    }
    return 0;
}
```

# 【扩展 BSGS 算法】

## 1.算法思想

对于 C 不是质数的情况，![A^x\equiv B(mod\:\:C)](https://private.codecogs.com/gif.latex?A%5Ex%5Cequiv%20B%28mod%5C%3A%5C%3AC%29) 等价于 ![A^x+y*C=B](https://private.codecogs.com/gif.latex?A%5Ex&plus;y*C%3DB)

对其进行消因子处理，使得 ![A^x+y*C=B](https://private.codecogs.com/gif.latex?A%5Ex&plus;y*C%3DB) 化为 ![aA^{x'}+y*C'=B'](https://private.codecogs.com/gif.latex?aA%5E%7Bx%27%7D&plus;y*C%27%3DB%27) 使得 C' 与 A 不再有可以约的因子：cnt=x-x'，每次消因子过程中，方程右边同时消除一样的因子

将 ![A^{x'}](https://private.codecogs.com/gif.latex?A%5E%7Bx%27%7D) 作为一个整体，利用扩展欧几里得得到其解系，假设得到的一组原始特解为 ![x_0](https://private.codecogs.com/gif.latex?x_0)，则：![A^{x'}=x_0B'+KC'(k\in Z)](https://private.codecogs.com/gif.latex?A%5E%7Bx%27%7D%3Dx_0B%27&plus;KC%27%28k%5Cin%20Z%29)

化为高次同余方程：![A^{x'}\equiv x_0B'(mod\:C')](https://private.codecogs.com/gif.latex?A%5E%7Bx%27%7D%5Cequiv%20x_0B%27%28mod%5C%3AC%27%29)，此时可利用 BSGS 求解得到 ![x_0'](https://private.codecogs.com/gif.latex?x_0%27)，加回 cnt 即可得到原方程的解

需要注意的是，这样得到的只有不大于 cnt 的解，可能会漏掉小于 cnt 的解，因此一般先从 1~log(50) 查找一遍

## 2.实现

```cpp
struct HashMap{//哈希表
    static const int Hash=999917,maxn=46340;
    int num,link[Hash],son[maxn+5],next[maxn+5],w[maxn+5];
    int top,Stack[maxn+5];
    void clear(){//清空表
        num=0;
        while(top)
            link[Stack[top--]]=0;
    }
    void add(int x,int y){//添加键值元素
        son[++num]=y;
        next[num]=link[x];
        w[num]=INF;
        link[x]=num;
    }
    bool count(int y){//判断表中是否有对应值
        int x=y%Hash;
        for(int j=link[x];j;j=next[j])
            if(y==son[j])
                return true;
        return false;
    }
    int &operator [](int y){//获取键的对应值
        int x=y%Hash;
        for(int j=link[x];j;j=next[j])
            if(y==son[j])
                return w[j];
        add(x,y);
        Stack[++top]=x;
        return w[num];
    }
}hashMap;
int GCD(int a,int b){
    if(b==0)
        return a;
    return GCD(b,a%b);
}
int extendedGCD(int x,int y,int &a,int &b){
    if(y==0){
        a=1;
        b=0;
        return x;
    }

    int temp;
    int gcd=extendedGCD(y,x%y,a,b);
    temp=a;
    a=b;
    b=temp-x/y*b;
    return gcd;
}

int extendBSGS(int A,int B,int C){
    //三种特判
    if(C==1){
        if(!B)
            return A!=1;
        return -1;
    }
    if(B==1){
        if(A)
            return 0;
        return -1;
    }
    if(A%C==0){
        if(!B)
            return 1;
        return -1;
    }

    int gcd=GCD(A,C);
    int D=1,num=0;
    while(gcd!=1){//把A,C变成(A,C)=1为止
        if(B%gcd)
            return -1;

        B/=gcd;//从B中约去因子
        C/=gcd;//约C中约去因子
        D=((LL)D*A/gcd)%C;//将多出的乘给D
        num++;//统计约去次数
        gcd=GCD(A,C);
    }

    int now=1;
    for(int i=0;i<=num-1;i++){//枚举0~num-1
        if(now==B)
            return i;
        now=((LL)now*A)%C;
    }

    hashMap.clear();
    int Size=ceil(sqrt(C)),Base=1;
    for(int i=0;i<=Size-1;i++){//将A^j存进哈希表
        hashMap[Base]=min(hashMap[Base],i);//存储答案最小的
        Base=((LL)Base*A)%C;
    }
    for(int i=0;i<=Size-1;i++){//扩展欧几里得求A^j
        int x,y;
        int gcd=extendedGCD(D,C,x,y);//求出x、y
        x=((LL)x*B%C+C)%C;
        if(hashMap.count(x))
            return i*Size+hashMap[x]+num;//加回num
        D=((LL)D*Base)%C;
    }
    return -1;//无解
}

int main(){
    int A,B,C;
    while(scanf("%d%d%d",&A,&B,&C)!=EOF&&(A+B+C)){
        int res=extendBSGS(A,B,C);
        if(res==-1)
            printf("No Solution\n");
        else
            printf("%d\n",res);
    }
    return 0;
}
```





