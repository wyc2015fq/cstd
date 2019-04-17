# So Easy!（HDU - 4565） - Alex_McAvoy的博客 - CSDN博客





2019年03月28日 21:16:04[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：27
个人分类：[HDU																[矩阵——矩阵快速幂](https://blog.csdn.net/u011815404/article/category/8800018)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

A sequence S n is defined as: 

![](https://img-blog.csdnimg.cn/20190328202359920.jpg)

Where a, b, n, m are positive integers.┌x┐is the ceil of x. For example, ┌3.14┐=4. You are to calculate S n. 

You, a top coder, say: So easy! 

# **Input**

There are several test cases, each test case in one line contains four positive integers: a, b, n, m. Where 0< a, m < 2 15, (a-1) 2< b < a 2, 0 < b, n < 2 31.The input will finish with the end of file.

# Output

For each the case, output an integer S n.

# Sample Input

**2 3 1 20132 3 2 20132 2 1 2013**

# Sample Output

**4144**


题意：给出 a、b、n、m 按照上图的公式，求 Sn

思路：共轭矩阵的构造，具体思路：[点击这里](https://blog.csdn.net/u011815404/article/details/88841494)

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
const int MOD=1e9+7;
const int N=10+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;
struct Matrix{
    LL s[N][N];
};
Matrix e;//单位矩阵E
Matrix x;//构造矩阵

LL mod;

void init(){
    for(int i=1;i<=2;i++)//主对角线为1
        e.s[i][i]=1;
}
Matrix mul(Matrix A,Matrix B,LL n){//矩阵乘法，n代表A、B两个矩阵是n阶方阵
    Matrix temp;//临时矩阵，存放A*B结果

    for(int i=1;i<=n;i++)
        for(int j=1;j<=n;j++)
            temp.s[i][j]=0;

    for(int i=1;i<=n;i++)
        for(int j=1;j<=n;j++)
            for(int k=1;k<=n;k++)
                temp.s[i][j]=(temp.s[i][j]+A.s[i][k]*B.s[k][j])%mod;
    return temp;
}
Matrix quickPower(Matrix a,LL b,LL n){//矩阵快速幂，求矩阵n阶矩阵的b次幂
    Matrix ans=e;
    while(b){
        if(b&1)
            ans=mul(ans,a,n);//ans=e*a
        a=mul(a,a,n);//a=a*a
        b>>=1;
    }
    return ans;
}
int main(){
    init();
    LL a,b,n;
    while(scanf("%lld%lld%lld%lld",&a,&b,&n,&mod)!=EOF){
        if(n<=1)
            printf("%lld\n",((2*a)%mod+mod)%mod);
        else{
            x.s[1][1]=2*a;x.s[1][2]=-(a*a-b);
            x.s[2][1]=1;x.s[2][2]=0;

            Matrix res=quickPower(x,n-1,2);
            printf("%lld\n",((res.s[1][1]*2*a+res.s[1][2]*2)%mod+mod)%mod);
        }
    }
    return 0;
}
```






