# Discrete Logging（POJ-2417） - Alex_McAvoy的博客 - CSDN博客





2019年03月21日 14:18:52[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：44








> 
# Problem Description

Given a prime P, 2 <= P < 231, an integer B, 2 <= B < P, and an integer N, 1 <= N < P, compute the discrete logarithm of N, base B, modulo P. That is, find an integer L such that 

    BL == N (mod P)

# Input

Read several lines of input, each containing P,B,N separated by a space.

# Output

For each line print the logarithm on a separate line. If there are several, print the smallest; if there is none, print "no solution".

# **Sample Input**

**5 2 15 2 25 2 35 2 45 3 15 3 25 3 35 3 45 4 15 4 25 4 35 4 412345701 2 11111111111111121 65537 1111111111**

# Sample Output

**013203120no solutionno solution19584351462803587**


题意：对于高次同余方程 ![A^x\equiv B(mod\:C)](https://private.codecogs.com/gif.latex?A%5Ex%5Cequiv%20B%28mod%5C%3AC%29)，依次给出 C、A、B，且 C 是质数，求最小的 x

思路：朴素的 BSGS 算法模版题，注意先输入的是 C

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<cstring>
#include<cmath>
#include<ctime>
#include<algorithm>
#include<utility>
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-9
#define INF 0x3f3f3f3f
#define LL long long
const int MOD=10007;
const int N=100000+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;

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
    while(scanf("%d%d%d",&C,&A,&B)!=EOF){
        int res=BSGS(A,B,C);
        if(res==-1)
            printf("no solution\n");
        else
            printf("%d\n",res);
    }
    return 0;
}
```





