# 233 Matrix（HDU-5051） - Alex_McAvoy的博客 - CSDN博客





2019年03月27日 13:51:16[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：26
个人分类：[HDU																[矩阵——矩阵快速幂](https://blog.csdn.net/u011815404/article/category/8800018)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

In our daily life we often use 233 to express our feelings. Actually, we may say 2333, 23333, or 233333 ... in the same meaning. And here is the question: Suppose we have a matrix called 233 matrix. In the first line, it would be 233, 2333, 23333... (it means a 0,1 = 233,a 0,2 = 2333,a 0,3 = 23333...) Besides, in 233 matrix, we got a i,j = a i-1,j +a i,j-1( i,j ≠ 0). Now you have known a 1,0,a 2,0,...,a n,0, could you tell me a n,m in the 233 matrix?

# **Input**

There are multiple test cases. Please process till EOF. 

For each case, the first line contains two postive integers n,m(n ≤ 10,m ≤ 10 9). The second line contains n integers, a 1,0,a 2,0,...,a n,0(0 ≤ a i,0 < 2 31).

# Output

For each case, output a n,m mod 10000007.

# Sample Input

**1 112 20 03 723 47 16**

# Sample Output

**234279972937**

**Hint**

**![](https://img-blog.csdnimg.cn/20190327131044934.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)**


题意：已知一个矩阵第 0 行是 0 233 2333 23333 ...，然后给出这个矩阵的递推公式：a[i][j]=a[i-1][j]+a[i][j-1]

现在给出多组数据，每组给出一个 n 和 m 代表矩阵大小，然后给出第 0 列的 n 个值，要求计算 a[n][m]

思路：

由于已经知道第一列的值，那么根据第一行的规律：0 233 2333 23333...

可知：a[0][1]=233，a[0][2]=2333，a[0][3]=23333，...，a[0][i]=10*a[0][i-1]+3，由于矩阵的递推公式不受 a[0][0]=0 影响，那么根据第一行的规律，可以设 a[0][0]=23，于是可得到第 0 行的递推公式：**a[0][i]=10*a[0][i-1]+3，其中 i>0，a[0][0]=23**

那么，由于已经给出第 0 列的值 a[1]~a[n]，那么可以构造第 0 列的矩阵 A：

![\begin{bmatrix}A[0] [0] \\ A[1][0] \\ A[2][0] \\ ... \\ A[n] [0] \end{bmatrix}= \begin{bmatrix}23 \\ a[1] \\ a[2] \\ ... \\ a[n] \end{bmatrix}](https://private.codecogs.com/gif.latex?%5Cbegin%7Bbmatrix%7DA%5B0%5D%20%5B0%5D%20%5C%5C%20A%5B1%5D%5B0%5D%20%5C%5C%20A%5B2%5D%5B0%5D%20%5C%5C%20...%20%5C%5C%20A%5Bn%5D%20%5B0%5D%20%5Cend%7Bbmatrix%7D%3D%20%5Cbegin%7Bbmatrix%7D23%20%5C%5C%20a%5B1%5D%20%5C%5C%20a%5B2%5D%20%5C%5C%20...%20%5C%5C%20a%5Bn%5D%20%5Cend%7Bbmatrix%7D)

根据题目给出的递推公式：**a[i][j]=a[i-1][j]+a[i][j-1]**，可以根据第 0 列与第 0 行构造第 1 列的矩阵：

![\begin{bmatrix}A[0][1] \\ A[1][1] \\ A[2][1] \\ ... \\ A[n][1] \end{bmatrix} =\begin{bmatrix}233 \\ a[1]+233 \\ a[2]+(a[1]+233) \\ ... \\ a[n]+(a[1]+a[2]+...a[n-1]+233) \end{bmatrix}](https://private.codecogs.com/gif.latex?%5Cbegin%7Bbmatrix%7DA%5B0%5D%5B1%5D%20%5C%5C%20A%5B1%5D%5B1%5D%20%5C%5C%20A%5B2%5D%5B1%5D%20%5C%5C%20...%20%5C%5C%20A%5Bn%5D%5B1%5D%20%5Cend%7Bbmatrix%7D%20%3D%5Cbegin%7Bbmatrix%7D233%20%5C%5C%20a%5B1%5D&plus;233%20%5C%5C%20a%5B2%5D&plus;%28a%5B1%5D&plus;233%29%20%5C%5C%20...%20%5C%5C%20a%5Bn%5D&plus;%28a%5B1%5D&plus;a%5B2%5D&plus;...a%5Bn-1%5D&plus;233%29%20%5Cend%7Bbmatrix%7D)

继续构造可得第 2 列矩阵：

![\begin{bmatrix}A[0][2] \\ A[1][2] \\ A[2][2] \\ ... \\ A[n-2][2]\\ A[n-1][2] \\ A[n][2] \end{bmatrix} =\begin{bmatrix}2333 \\ (a[1]+233)+2333 \\ (a[1]+a[2]+233)+(a[1]+233)+2333 \\ ... \\ a[n-2]+A[n-3][2] \\ a[n-1]+(a[n-2]+A[n-3][2]) \\ a[n]+[a[n-1]+(a[n-2]+A[n-3][2])]\end{bmatrix}](https://private.codecogs.com/gif.latex?%5Cbegin%7Bbmatrix%7DA%5B0%5D%5B2%5D%20%5C%5C%20A%5B1%5D%5B2%5D%20%5C%5C%20A%5B2%5D%5B2%5D%20%5C%5C%20...%20%5C%5C%20A%5Bn-2%5D%5B2%5D%5C%5C%20A%5Bn-1%5D%5B2%5D%20%5C%5C%20A%5Bn%5D%5B2%5D%20%5Cend%7Bbmatrix%7D%20%3D%5Cbegin%7Bbmatrix%7D2333%20%5C%5C%20%28a%5B1%5D&plus;233%29&plus;2333%20%5C%5C%20%28a%5B1%5D&plus;a%5B2%5D&plus;233%29&plus;%28a%5B1%5D&plus;233%29&plus;2333%20%5C%5C%20...%20%5C%5C%20a%5Bn-2%5D&plus;A%5Bn-3%5D%5B2%5D%20%5C%5C%20a%5Bn-1%5D&plus;%28a%5Bn-2%5D&plus;A%5Bn-3%5D%5B2%5D%29%20%5C%5C%20a%5Bn%5D&plus;%5Ba%5Bn-1%5D&plus;%28a%5Bn-2%5D&plus;A%5Bn-3%5D%5B2%5D%29%5D%5Cend%7Bbmatrix%7D)

以此类推，再加上第 0 行的递推公式：**a[0][i]=10*a[0][i-1]+3，其中 i>0，a[0][0]=23****，**即可构造出第 m 列的矩阵：

![\begin{bmatrix}A[0][m] \\ A[1][m] \\ A[2][m] \\ ... \\A[n-2][m] \\A[n-1][m] \\ A[n][m] \\ 3 \end{bmatrix}= \begin{bmatrix}10&0&0&0&...&0&0&0&1 \\ 10 & 1&0&0&...&0&0&0&1 \\ 10 & 1&1&0&...&0&0&0&1 \\ ...&...&...&...&...&...&...&...&... \\10 & 1&1&1&...&1&0&0&1 \\10 & 1&1&1&...&1&1&0&1 \\ 10 & 1&1&1&...&1&1&1&1 \\ 0 & 0&0&0&...&0&0&0&1 \end{bmatrix} \begin{bmatrix}A[0][m-1] \\ A[1][m-1] \\ A[2][m-1] \\ ... \\A[n-2][m-1] \\A[n-1][m-1] \\ A[n][m-1] \\ 3 \end{bmatrix}](https://private.codecogs.com/gif.latex?%5Cbegin%7Bbmatrix%7DA%5B0%5D%5Bm%5D%20%5C%5C%20A%5B1%5D%5Bm%5D%20%5C%5C%20A%5B2%5D%5Bm%5D%20%5C%5C%20...%20%5C%5CA%5Bn-2%5D%5Bm%5D%20%5C%5CA%5Bn-1%5D%5Bm%5D%20%5C%5C%20A%5Bn%5D%5Bm%5D%20%5C%5C%203%20%5Cend%7Bbmatrix%7D%3D%20%5Cbegin%7Bbmatrix%7D10%260%260%260%26...%260%260%260%261%20%5C%5C%2010%20%26%201%260%260%26...%260%260%260%261%20%5C%5C%2010%20%26%201%261%260%26...%260%260%260%261%20%5C%5C%20...%26...%26...%26...%26...%26...%26...%26...%26...%20%5C%5C10%20%26%201%261%261%26...%261%260%260%261%20%5C%5C10%20%26%201%261%261%26...%261%261%260%261%20%5C%5C%2010%20%26%201%261%261%26...%261%261%261%261%20%5C%5C%200%20%26%200%260%260%26...%260%260%260%261%20%5Cend%7Bbmatrix%7D%20%5Cbegin%7Bbmatrix%7DA%5B0%5D%5Bm-1%5D%20%5C%5C%20A%5B1%5D%5Bm-1%5D%20%5C%5C%20A%5B2%5D%5Bm-1%5D%20%5C%5C%20...%20%5C%5CA%5Bn-2%5D%5Bm-1%5D%20%5C%5CA%5Bn-1%5D%5Bm-1%5D%20%5C%5C%20A%5Bn%5D%5Bm-1%5D%20%5C%5C%203%20%5Cend%7Bbmatrix%7D)

那么，就有：

![\begin{bmatrix}A[0][m] \\ A[1][m] \\ A[2][m] \\ ... \\A[n-2][m] \\A[n-1][m] \\ A[n][m] \\ 3 \end{bmatrix}= \begin{bmatrix}10&0&0&0&...&0&0&0&1 \\ 10 & 1&0&0&...&0&0&0&1 \\ 10 & 1&1&0&...&0&0&0&1 \\ ...&...&...&...&...&...&...&...&... \\10 & 1&1&1&...&1&0&0&1 \\10 & 1&1&1&...&1&1&0&1 \\ 10 & 1&1&1&...&1&1&1&1 \\ 0 & 0&0&0&...&0&0&0&1 \end{bmatrix} ^m \begin{bmatrix}23 \\ a[1] \\ a[2] \\ ... \\a[n-2] \\a[n-1] \\ a[n] \\ 3 \end{bmatrix}](https://private.codecogs.com/gif.latex?%5Cbegin%7Bbmatrix%7DA%5B0%5D%5Bm%5D%20%5C%5C%20A%5B1%5D%5Bm%5D%20%5C%5C%20A%5B2%5D%5Bm%5D%20%5C%5C%20...%20%5C%5CA%5Bn-2%5D%5Bm%5D%20%5C%5CA%5Bn-1%5D%5Bm%5D%20%5C%5C%20A%5Bn%5D%5Bm%5D%20%5C%5C%203%20%5Cend%7Bbmatrix%7D%3D%20%5Cbegin%7Bbmatrix%7D10%260%260%260%26...%260%260%260%261%20%5C%5C%2010%20%26%201%260%260%26...%260%260%260%261%20%5C%5C%2010%20%26%201%261%260%26...%260%260%260%261%20%5C%5C%20...%26...%26...%26...%26...%26...%26...%26...%26...%20%5C%5C10%20%26%201%261%261%26...%261%260%260%261%20%5C%5C10%20%26%201%261%261%26...%261%261%260%261%20%5C%5C%2010%20%26%201%261%261%26...%261%261%261%261%20%5C%5C%200%20%26%200%260%260%26...%260%260%260%261%20%5Cend%7Bbmatrix%7D%20%5Em%20%5Cbegin%7Bbmatrix%7D23%20%5C%5C%20a%5B1%5D%20%5C%5C%20a%5B2%5D%20%5C%5C%20...%20%5C%5Ca%5Bn-2%5D%20%5C%5Ca%5Bn-1%5D%20%5C%5C%20a%5Bn%5D%20%5C%5C%203%20%5Cend%7Bbmatrix%7D)

于是，根据构造矩阵，利用矩阵快速幂即可得到 A[n][m] 的答案

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
const int N=100+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;
struct Matrix{
    LL s[N][N];
}x;
Matrix e;//单位矩阵E
LL n,m;
void init(){
    for(int i=1;i<=N;i++)//主对角线为1
        e.s[i][i]=1;

    //构造矩阵
    for(int i=1;i<=15;i++){
        for(int j=1;j<=i;j++){
            x.s[i][j]=1;
            if(j==2)
                x.s[i][j]=10;
        }
    }
}
Matrix mul(Matrix A,Matrix B){//矩阵乘法，n代表A、B两个矩阵是n阶方阵
    Matrix temp;//临时矩阵，存放A*B结果

    for(int i=1;i<=n;i++)
        for(int j=1;j<=n;j++)
            temp.s[i][j]=0;

    for(int i=1;i<=n;i++)
        for(int j=1;j<=n;j++)
            for(int k=1;k<=n;k++)
                temp.s[i][j]=(temp.s[i][j]+(A.s[i][k]*B.s[k][j])%MOD)%MOD;
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
LL a[N];
int main(){
    init();
    while(scanf("%lld%lld",&n,&m)!=EOF){
        a[1]=3;
        a[2]=23;
        n+=2;
        for(int i=3;i<=n;i++)
            scanf("%lld",&a[i]);

        Matrix res=quickPower(x,m);

        LL ans=0;
        for(int i=1;i<=n;i++)
            ans=(ans+a[i]*res.s[n][i])%MOD;
        printf("%lld\n",ans);
    }
    return 0;
}
```






