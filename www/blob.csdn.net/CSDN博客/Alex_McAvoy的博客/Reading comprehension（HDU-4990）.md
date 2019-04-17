# Reading comprehension（HDU-4990） - Alex_McAvoy的博客 - CSDN博客





2019年03月27日 18:24:33[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：170
个人分类：[HDU																[矩阵——矩阵快速幂](https://blog.csdn.net/u011815404/article/category/8800018)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

Read the program below carefully then answer the question. 

```cpp
#pragma comment(linker, "/STACK:1024000000,1024000000") 
#include <cstdio> 
#include<iostream> 
#include <cstring> 
#include <cmath> 
#include <algorithm> 
#include<vector> 
const int MAX=100000*2; 
const int INF=1e9; 
int main() 
{ 
  int n,m,ans,i; 
  while(scanf("%d%d",&n,&m)!=EOF) 
  { 
    ans=0; 
    for(i=1;i<=n;i++) 
    { 
      if(i&1)ans=(ans*2+1)%m; 
      else ans=ans*2%m; 
    } 
    printf("%d\n",ans); 
  } 
  return 0; 
}
```

# **Input**

Multi test cases，each line will contain two integers n and m. Process to end of file. 

[Technical Specification] 

1<=n, m <= 1000000000

# Output

For each case，output an integer，represents the output of above program.

# Sample Input

**1 103 100**

# Sample Output

**15**


题意：根据，输入 n、m 给出结果

思路：

n、m 数据范围能到1E9，直接暴力一定会 TLE

分析一下代码中的递推公式：
- n 为奇数时：![f[n] = f[n-1] * 2 + 1](https://private.codecogs.com/gif.latex?f%5Bn%5D%20%3D%20f%5Bn-1%5D%20*%202%20&plus;%201)
- n 为偶数时：![f[n] = f[n-1] * 2](https://private.codecogs.com/gif.latex?f%5Bn%5D%20%3D%20f%5Bn-1%5D%20*%202)

假设 n 为奇数，那么 ![f[n] = f[n-1] * 2 + 1](https://private.codecogs.com/gif.latex?f%5Bn%5D%20%3D%20f%5Bn-1%5D%20*%202%20&plus;%201)，得到的 n-1 必定是偶数

于是有：![f[n] = f[n-1] + f[n-2] * 2 + 1](https://private.codecogs.com/gif.latex?f%5Bn%5D%20%3D%20f%5Bn-1%5D%20&plus;%20f%5Bn-2%5D%20*%202%20&plus;%201)

同理，假设 n 为偶数，那么 ![f[n] = f[n-1] * 2](https://private.codecogs.com/gif.latex?f%5Bn%5D%20%3D%20f%5Bn-1%5D%20*%202)，得到的 n-1 必定是奇数

于是有：![f[n] = f[n-1]+ f[n-2] * 2+1](https://private.codecogs.com/gif.latex?f%5Bn%5D%20%3D%20f%5Bn-1%5D&plus;%20f%5Bn-2%5D%20*%202&plus;1)

那么，无论 n 为奇还是为偶，![f[n] = f[n-1]+ f[n-2] * 2+1](https://private.codecogs.com/gif.latex?f%5Bn%5D%20%3D%20f%5Bn-1%5D&plus;%20f%5Bn-2%5D%20*%202&plus;1) 一定成立

根据公式构造矩阵：![\begin{bmatrix}f(n) \\ f(n-1) \\ 1 \end{bmatrix}= \begin{bmatrix} 1 &2 &1 \\ 1&0 &0 \\ 0 &0 &1 \end{bmatrix} \begin{bmatrix}f(n-1) \\ f(n-2) \\ 1 \end{bmatrix}](https://private.codecogs.com/gif.latex?%5Cbegin%7Bbmatrix%7Df%28n%29%20%5C%5C%20f%28n-1%29%20%5C%5C%201%20%5Cend%7Bbmatrix%7D%3D%20%5Cbegin%7Bbmatrix%7D%201%20%262%20%261%20%5C%5C%201%260%20%260%20%5C%5C%200%20%260%20%261%20%5Cend%7Bbmatrix%7D%20%5Cbegin%7Bbmatrix%7Df%28n-1%29%20%5C%5C%20f%28n-2%29%20%5C%5C%201%20%5Cend%7Bbmatrix%7D)

化简后：![\begin{bmatrix}f(n) \\ f(n-1) \\ 1 \end{bmatrix}= \begin{bmatrix} 1 &2 &1 \\ 1&0 &0 \\ 0 &0 &1 \end{bmatrix}^{n-1} \begin{bmatrix}f(1) \\ f(0) \\ 1 \end{bmatrix}](https://private.codecogs.com/gif.latex?%5Cbegin%7Bbmatrix%7Df%28n%29%20%5C%5C%20f%28n-1%29%20%5C%5C%201%20%5Cend%7Bbmatrix%7D%3D%20%5Cbegin%7Bbmatrix%7D%201%20%262%20%261%20%5C%5C%201%260%20%260%20%5C%5C%200%20%260%20%261%20%5Cend%7Bbmatrix%7D%5E%7Bn-1%7D%20%5Cbegin%7Bbmatrix%7Df%281%29%20%5C%5C%20f%280%29%20%5C%5C%201%20%5Cend%7Bbmatrix%7D)

由于 f(0)=0，f(1)=1，那么有：![\begin{bmatrix}f(n) \\ f(n-1) \\ 1 \end{bmatrix}= \begin{bmatrix} 1 &2 &1 \\ 1&0 &0 \\ 0 &0 &1 \end{bmatrix}^{n-1} \begin{bmatrix}1 \\ 0 \\ 1 \end{bmatrix}](https://private.codecogs.com/gif.latex?%5Cbegin%7Bbmatrix%7Df%28n%29%20%5C%5C%20f%28n-1%29%20%5C%5C%201%20%5Cend%7Bbmatrix%7D%3D%20%5Cbegin%7Bbmatrix%7D%201%20%262%20%261%20%5C%5C%201%260%20%260%20%5C%5C%200%20%260%20%261%20%5Cend%7Bbmatrix%7D%5E%7Bn-1%7D%20%5Cbegin%7Bbmatrix%7D1%20%5C%5C%200%20%5C%5C%201%20%5Cend%7Bbmatrix%7D)

故答案为：![f(n)=A[1][1]*1+A[1][2]*0+A[1][3]*1=A[1][1]+A[1][3]](https://private.codecogs.com/gif.latex?f%28n%29%3DA%5B1%5D%5B1%5D*1&plus;A%5B1%5D%5B2%5D*0&plus;A%5B1%5D%5B3%5D*1%3DA%5B1%5D%5B1%5D&plus;A%5B1%5D%5B3%5D)

因此矩阵快速幂求出 A 后即得答案

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
const int MOD=10000007;
const int N=10+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;
struct Matrix{
    LL s[N][N];
};
Matrix e;//单位矩阵E
Matrix x;//构造矩阵
LL n,m;
void init(){
    for(int i=1;i<=3;i++)//主对角线为1
        e.s[i][i]=1;

    //构造矩阵
    x.s[1][1]=1L;x.s[1][2]=2L;x.s[1][3]=1L;
    x.s[2][1]=1L;x.s[2][2]=0L;x.s[2][3]=0L;
    x.s[3][1]=0L;x.s[3][2]=0L;x.s[3][3]=1L;
}
Matrix mul(Matrix A,Matrix B){//矩阵乘法，n代表A、B两个矩阵是n阶方阵
    Matrix temp;//临时矩阵，存放A*B结果

    for(int i=1;i<=3;i++)
        for(int j=1;j<=3;j++)
            temp.s[i][j]=0;

    for(int i=1;i<=3;i++)
        for(int j=1;j<=3;j++)
            for(int k=1;k<=3;k++)
                temp.s[i][j]=(temp.s[i][j]+(A.s[i][k]*B.s[k][j])%m)%m;
    return temp;
}
Matrix quickPower(Matrix a,LL b){//矩阵快速幂，求矩阵n阶矩阵的b次幂
    Matrix ans=e;
    while(b){
        if(b&1)
            ans=mul(ans,a);//ans=e*a
        a=mul(a,a);//a=a*a
        b>>=1;
    }
    return ans;
}
int main(){
    init();
    while(scanf("%lld%lld",&n,&m)!=EOF&&(n+m)){
        Matrix res=quickPower(x,(LL)(n-1));
        printf("%lld\n",(res.s[1][1]+res.s[1][3])%m);
    }
    return 0;
}
```






