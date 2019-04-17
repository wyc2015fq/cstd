# Jzzhu and Sequences（CF-450B） - Alex_McAvoy的博客 - CSDN博客





2019年03月28日 13:42:04[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：32
个人分类：[CodeForces																[矩阵——矩阵快速幂](https://blog.csdn.net/u011815404/article/category/8800018)](https://blog.csdn.net/u011815404/article/category/7923180)








> 
# Problem Description

Jzzhu has invented a kind of sequences, they meet the following property:

![](https://img-blog.csdnimg.cn/20190328124516826.jpg)

You are given *x* and *y*, please calculate *f**n* modulo 1000000007 (109 + 7).

# Input

The first line contains two integers x and y (|x|, |y| ≤ 109). The second line contains a single integer n (1 ≤ n ≤ 2·109).

# Output

Output a single integer representing fn modulo 1000000007 (109 + 7).

# Examples

**Input**

2 3

3

**Output**

1

**Input**

0 -1

2

**Output**

1000000006


题意：输入 x，y，n，其中 f(1)=x，f(2)=y，f(i)=f(i-1)+f(i+1)，求 f(n)

思路： n 很大，直接递推的话一定会 TLE

可以考虑构造满足递推式的矩阵用矩阵快速幂来求

已知：![f(i)=f(i-1)+f(i+1)](https://private.codecogs.com/gif.latex?f%28i%29%3Df%28i-1%29&plus;f%28i&plus;1%29)，那么有：![f(i+1)=f(i)-f(i-1)](https://private.codecogs.com/gif.latex?f%28i&plus;1%29%3Df%28i%29-f%28i-1%29)

即：![f(n)=f(n-1)-f(n-2)](https://private.codecogs.com/gif.latex?f%28n%29%3Df%28n-1%29-f%28n-2%29)，则：![f(n+1)=f(n)-f(n-1)](https://private.codecogs.com/gif.latex?f%28n&plus;1%29%3Df%28n%29-f%28n-1%29)

构造系数矩阵，有：![\begin{vmatrix}f(n) \\ f(n-1) \end{vmatrix} =\begin{vmatrix} 1&-1\\ 1 &0 \end{vmatrix}\begin{vmatrix}f(n-1) \\ f(n-2) \end{vmatrix}](https://private.codecogs.com/gif.latex?%5Cbegin%7Bvmatrix%7Df%28n%29%20%5C%5C%20f%28n-1%29%20%5Cend%7Bvmatrix%7D%20%3D%5Cbegin%7Bvmatrix%7D%201%26-1%5C%5C%201%20%260%20%5Cend%7Bvmatrix%7D%5Cbegin%7Bvmatrix%7Df%28n-1%29%20%5C%5C%20f%28n-2%29%20%5Cend%7Bvmatrix%7D)

化简得：![\begin{vmatrix}f(n) \\ f(n-1) \end{vmatrix} =\begin{vmatrix} 1&-1\\ 1 &0 \end{vmatrix}^{n-2}\begin{vmatrix}f(2) \\ f(1) \end{vmatrix}=\begin{vmatrix} 1&-1\\ 1 &0 \end{vmatrix}^{n-2}\begin{vmatrix}y \\ x \end{vmatrix}](https://private.codecogs.com/gif.latex?%5Cbegin%7Bvmatrix%7Df%28n%29%20%5C%5C%20f%28n-1%29%20%5Cend%7Bvmatrix%7D%20%3D%5Cbegin%7Bvmatrix%7D%201%26-1%5C%5C%201%20%260%20%5Cend%7Bvmatrix%7D%5E%7Bn-2%7D%5Cbegin%7Bvmatrix%7Df%282%29%20%5C%5C%20f%281%29%20%5Cend%7Bvmatrix%7D%3D%5Cbegin%7Bvmatrix%7D%201%26-1%5C%5C%201%20%260%20%5Cend%7Bvmatrix%7D%5E%7Bn-2%7D%5Cbegin%7Bvmatrix%7Dy%20%5C%5C%20x%20%5Cend%7Bvmatrix%7D)

所以答案即为系数矩阵的 n-2 次幂的值 A[1][1]*y+A[1][2]*x

此外要注意特判

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
void init(){
    for(int i=1;i<=2;i++)//主对角线为1
        e.s[i][i]=1;

    //构造矩阵
    x.s[1][1]=1;x.s[1][2]=-1;
    x.s[2][1]=1;x.s[2][2]=0;
}
Matrix mul(Matrix A,Matrix B,LL n){//矩阵乘法，n代表A、B两个矩阵是n阶方阵
    Matrix temp;//临时矩阵，存放A*B结果

    for(int i=1;i<=n;i++)
        for(int j=1;j<=n;j++)
            temp.s[i][j]=0;

    for(int i=1;i<=n;i++)
        for(int j=1;j<=n;j++)
            for(int k=1;k<=n;k++)
                temp.s[i][j]=(temp.s[i][j]+A.s[i][k]*B.s[k][j])%MOD;
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
    LL x0,y0,n;
    while(scanf("%lld%lld%lld",&x0,&y0,&n)!=EOF){
        if(x0==0&&y0==0)
            printf("0\n");
        else if(n==1)
            printf("%lld\n",(x0%MOD+MOD)%MOD);
        else if(n==2)
            printf("%lld\n",(y0%MOD+MOD)%MOD);
        else{
            Matrix res=quickPower(x,n-2,2);
            LL temp=(res.s[1][1]*y0+res.s[1][2]*x0)%MOD;
            if(temp<0)
                temp=(temp%MOD+MOD)%MOD;
            printf("%lld\n",temp);
        }
    }
    return 0;
}
```






