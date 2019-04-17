# Clever Y（POJ-3243） - Alex_McAvoy的博客 - CSDN博客





2019年03月21日 15:19:15[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：54








> 
# Problem Description

Little Y finds there is a very interesting formula in mathematics:

X^Y mod Z = K

Given X, Y, Z, we all know how to figure out K fast. However, given X, Z, K, could you figure out Y fast?

# Input

Input data consists of no more than 20 test cases. For each test case, there would be only one line containing 3 integers X, Z, K (0 ≤ X, Z, K ≤ 109). 

Input file ends with 3 zeros separated by spaces. 

# Output

For each test case output one line. Write "No Solution" (without quotes) if you cannot find a feasible Y (0 ≤ Y < Z). Otherwise output the minimum Y you find.

# Sample Input

**5 58 332 4 30 0 0**

# Sample Output

**9No Solution**


题意：对于高次同余方程 ![A^x\equiv B(mod\:C)](https://private.codecogs.com/gif.latex?A%5Ex%5Cequiv%20B%28mod%5C%3AC%29)，依次给出 A、C、B，且 C 不一定是质数，求 x 的最小值

思路：扩展 BSGS 模版题

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
    while(scanf("%d%d%d",&A,&C,&B)!=EOF&&(A+B+C)){
        int res=extendBSGS(A,B,C);
        if(res==-1)
            printf("No Solution\n");
        else
            printf("%d\n",res);
    }
    return 0;
}
```





