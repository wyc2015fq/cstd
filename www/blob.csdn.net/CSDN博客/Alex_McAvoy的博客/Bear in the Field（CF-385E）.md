# Bear in the Field（CF-385E） - Alex_McAvoy的博客 - CSDN博客





2019年03月30日 14:34:38[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：42
个人分类：[CodeForces																[矩阵——矩阵快速幂](https://blog.csdn.net/u011815404/article/category/8800018)](https://blog.csdn.net/u011815404/article/category/7923180)








> 
# Problem Description

Our bear's forest has a checkered field. The checkered field is an n × n table, the rows are numbered from 1 to n from top to bottom, the columns are numbered from 1 to n from left to right. Let's denote a cell of the field on the intersection of row x and column y by record (x, y). Each cell of the field contains growing raspberry, at that, the cell (x, y) of the field contains x + y raspberry bushes.

The bear came out to walk across the field. At the beginning of the walk his speed is (dx, dy). Then the bear spends exactly t seconds on the field. Each second the following takes place:

Let's suppose that at the current moment the bear is in cell (x, y).

First the bear eats the raspberry from all the bushes he has in the current cell. After the bear eats the raspberry from k bushes, he increases each component of his speed by k. In other words, if before eating the k bushes of raspberry his speed was (dx, dy), then after eating the berry his speed equals (dx + k, dy + k).

Let's denote the current speed of the bear (dx, dy) (it was increased after the previous step). Then the bear moves from cell (x, y) to cell (((x + dx - 1) mod n) + 1, ((y + dy - 1) mod n) + 1).

Then one additional raspberry bush grows in each cell of the field.

You task is to predict the bear's actions. Find the cell he ends up in if he starts from cell (sx, sy). Assume that each bush has infinitely much raspberry and the bear will never eat all of it.

# **Input**

The first line of the input contains six space-separated integers: n, sx, sy, dx, dy, t (1 ≤ n ≤ 109; 1 ≤ sx, sy ≤ n; - 100 ≤ dx, dy ≤ 100; 0 ≤ t ≤ 1018).

# Output

Print two integers — the coordinates of the cell the bear will end up in after t seconds.

# Example

**Input**

5 1 2 0 1 2

**output**

3 1

**Input**

1 1 1 -1 -1 2

**Input**

1 1


题意：给出 n、x、y、dx、dy、t 代表有一个 n*n 的图，每个点上行有 x+y 个草莓，现在从 (sx,sy) 出发，速度为 (dx,dy)，其中对于每个点，每过一秒就会多 1 个草莓，速度增加当前位置上的草莓数，问 t 秒后的位置

思路：

根据题意，有：![\left\{\begin{matrix}dx(t)=dx(t-1)+sx(t-1)+sy(t-1)+t-1 \\ dy(t)= dy(t-1)+sx(t-1)+sy(t-1)+t-1 \end{matrix}\right.](https://private.codecogs.com/gif.latex?%5Cleft%5C%7B%5Cbegin%7Bmatrix%7Ddx%28t%29%3Ddx%28t-1%29&plus;sx%28t-1%29&plus;sy%28t-1%29&plus;t-1%20%5C%5C%20dy%28t%29%3D%20dy%28t-1%29&plus;sx%28t-1%29&plus;sy%28t-1%29&plus;t-1%20%5Cend%7Bmatrix%7D%5Cright.)

其中，![\left\{\begin{matrix}sx(t)=sx(t-1)+dx(t) \\ sy(t)=sy(t-1)+dy(t) \end{matrix}\right.](https://private.codecogs.com/gif.latex?%5Cleft%5C%7B%5Cbegin%7Bmatrix%7Dsx%28t%29%3Dsx%28t-1%29&plus;dx%28t%29%20%5C%5C%20sy%28t%29%3Dsy%28t-1%29&plus;dy%28t%29%20%5Cend%7Bmatrix%7D%5Cright.)

对于时间 t，则有：![t=t-1+1](https://private.codecogs.com/gif.latex?t%3Dt-1&plus;1)

由于存在取模运算，%n 时只会产生 0~n-1 的数，因此将位置坐标都向上、向左平移一下，这样坐标范围变成 0~n-1，但这样会导致速度增加量减少了 2，因此需要加上 2

于是，有：![\left\{\begin{matrix}dx(t)= dx(t-1)+sx(t-1)+sy(t-1)+t-1+2 \\ dy(t)= dy(t-1)+sx(t-1)+sy(t-1)+t-1+2 \end{matrix}\right.](https://private.codecogs.com/gif.latex?%5Cleft%5C%7B%5Cbegin%7Bmatrix%7Ddx%28t%29%3D%20dx%28t-1%29&plus;sx%28t-1%29&plus;sy%28t-1%29&plus;t-1&plus;2%20%5C%5C%20dy%28t%29%3D%20dy%28t-1%29&plus;sx%28t-1%29&plus;sy%28t-1%29&plus;t-1&plus;2%20%5Cend%7Bmatrix%7D%5Cright.)

代入 ![\left\{\begin{matrix}sx(t)=sx(t-1)+dx(t) \\ sy(t)=sy(t-1)+dy(t) \end{matrix}\right.](https://private.codecogs.com/gif.latex?%5Cleft%5C%7B%5Cbegin%7Bmatrix%7Dsx%28t%29%3Dsx%28t-1%29&plus;dx%28t%29%20%5C%5C%20sy%28t%29%3Dsy%28t-1%29&plus;dy%28t%29%20%5Cend%7Bmatrix%7D%5Cright.)

有：![\left\{\begin{matrix}sx(t)=2*sx(t-1)+dx(t-1)+sy(t-1)+t-1+2 \\sy(t)=2*sy(t-1)+dy(t-1)+sx(t-1)+t-1+2\end{matrix}\right.](https://private.codecogs.com/gif.latex?%5Cleft%5C%7B%5Cbegin%7Bmatrix%7Dsx%28t%29%3D2*sx%28t-1%29&plus;dx%28t-1%29&plus;sy%28t-1%29&plus;t-1&plus;2%20%5C%5Csy%28t%29%3D2*sy%28t-1%29&plus;dy%28t-1%29&plus;sx%28t-1%29&plus;t-1&plus;2%5Cend%7Bmatrix%7D%5Cright.)

于是，可以构造矩阵：

![\begin{bmatrix}sx(t) \\ sy(t) \\ dx(t) \\ dy(t) \\ t \\ 1 \end{bmatrix} =\begin{bmatrix} 2&1 &1 & 0 &1 &2 \\ 1& 2 & 1&0 &1 & 2\\ 1& 1 & 1& 0& 1& 2\\ 1& 1 & 0& 1& 1 & 2\\ 0& 0 & 0& 0& 1& 1\\ 0& 0 & 0& 0& 0 &1 \end{bmatrix}\begin{bmatrix} sx(t-1)\\ sy(t-1) \\ dx(t-1) \\ dy(t-1) \\ t-1 \\ 1 \end{bmatrix}](https://private.codecogs.com/gif.latex?%5Cbegin%7Bbmatrix%7Dsx%28t%29%20%5C%5C%20sy%28t%29%20%5C%5C%20dx%28t%29%20%5C%5C%20dy%28t%29%20%5C%5C%20t%20%5C%5C%201%20%5Cend%7Bbmatrix%7D%20%3D%5Cbegin%7Bbmatrix%7D%202%261%20%261%20%26%200%20%261%20%262%20%5C%5C%201%26%202%20%26%201%260%20%261%20%26%202%5C%5C%201%26%201%20%26%201%26%200%26%201%26%202%5C%5C%201%26%201%20%26%200%26%201%26%201%20%26%202%5C%5C%200%26%200%20%26%200%26%200%26%201%26%201%5C%5C%200%26%200%20%26%200%26%200%26%200%20%261%20%5Cend%7Bbmatrix%7D%5Cbegin%7Bbmatrix%7D%20sx%28t-1%29%5C%5C%20sy%28t-1%29%20%5C%5C%20dx%28t-1%29%20%5C%5C%20dy%28t-1%29%20%5C%5C%20t-1%20%5C%5C%201%20%5Cend%7Bbmatrix%7D)

化简得：

![\begin{bmatrix}sx(t) \\ sy(t) \\ dx(t) \\ dy(t) \\ t \\ 1 \end{bmatrix} =\begin{bmatrix} 2&1 &1 & 0 &1 &2 \\ 1& 2 & 1&0 &1 & 2\\ 1& 1 & 1& 0& 1& 2\\ 1& 1 & 0& 1& 1 & 2\\ 0& 0 & 0& 0& 1& 1\\ 0& 0 & 0& 0& 0 &1 \end{bmatrix}^t\begin{bmatrix} sx(0)\\ sy(0) \\ dx(0) \\ dy(0) \\ 0 \\ 1 \end{bmatrix}=\begin{bmatrix} 2&1 &1 & 0 &1 &2 \\ 1& 2 & 1&0 &1 & 2\\ 1& 1 & 1& 0& 1& 2\\ 1& 1 & 0& 1& 1 & 2\\ 0& 0 & 0& 0& 1& 1\\ 0& 0 & 0& 0& 0 &1 \end{bmatrix}^t\begin{bmatrix} sx\\ sy \\ dx \\ dy \\ 0 \\ 1 \end{bmatrix}](https://private.codecogs.com/gif.latex?%5Cbegin%7Bbmatrix%7Dsx%28t%29%20%5C%5C%20sy%28t%29%20%5C%5C%20dx%28t%29%20%5C%5C%20dy%28t%29%20%5C%5C%20t%20%5C%5C%201%20%5Cend%7Bbmatrix%7D%20%3D%5Cbegin%7Bbmatrix%7D%202%261%20%261%20%26%200%20%261%20%262%20%5C%5C%201%26%202%20%26%201%260%20%261%20%26%202%5C%5C%201%26%201%20%26%201%26%200%26%201%26%202%5C%5C%201%26%201%20%26%200%26%201%26%201%20%26%202%5C%5C%200%26%200%20%26%200%26%200%26%201%26%201%5C%5C%200%26%200%20%26%200%26%200%26%200%20%261%20%5Cend%7Bbmatrix%7D%5Et%5Cbegin%7Bbmatrix%7D%20sx%280%29%5C%5C%20sy%280%29%20%5C%5C%20dx%280%29%20%5C%5C%20dy%280%29%20%5C%5C%200%20%5C%5C%201%20%5Cend%7Bbmatrix%7D%3D%5Cbegin%7Bbmatrix%7D%202%261%20%261%20%26%200%20%261%20%262%20%5C%5C%201%26%202%20%26%201%260%20%261%20%26%202%5C%5C%201%26%201%20%26%201%26%200%26%201%26%202%5C%5C%201%26%201%20%26%200%26%201%26%201%20%26%202%5C%5C%200%26%200%20%26%200%26%200%26%201%26%201%5C%5C%200%26%200%20%26%200%26%200%26%200%20%261%20%5Cend%7Bbmatrix%7D%5Et%5Cbegin%7Bbmatrix%7D%20sx%5C%5C%20sy%20%5C%5C%20dx%20%5C%5C%20dy%20%5C%5C%200%20%5C%5C%201%20%5Cend%7Bbmatrix%7D)

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
const int MOD=1000000007;
const int N=10+5;
//const int dx[]= {-1,1,0,0};
//const int dy[]= {0,0,-1,1};
using namespace std;
struct Matrix{
    LL s[N][N];
};
Matrix e;//单位矩阵E
Matrix x;//构造矩阵
LL mod;
Matrix mul(Matrix A,Matrix B,LL n){//矩阵乘法，n代表A、B两个矩阵是n阶方阵
    Matrix temp;//临时矩阵，存放A*B结果
    for(int i=1;i<=n;i++)
        for(int j=1;j<=n;j++)
            temp.s[i][j]=0;

    for(int i=1;i<=n;i++)
        for(int j=1;j<=n;j++)
            for(int k=1;k<=n;k++)
                temp.s[i][j]=((temp.s[i][j]+A.s[i][k]*B.s[k][j])%mod+mod)%mod;
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
LL n,sx,sy,dx,dy,t;
LL y[N];
void init(){
    for(int i=1;i<=6;i++)//主对角线为1
        e.s[i][i]=1;

    x.s[1][1]=2;x.s[1][2]=1;x.s[1][3]=1;x.s[1][4]=0;x.s[1][5]=1;x.s[1][6]=2;
    x.s[2][1]=1;x.s[2][2]=2;x.s[2][3]=0;x.s[2][4]=1;x.s[2][5]=1;x.s[2][6]=2;
    x.s[3][1]=1;x.s[3][2]=1;x.s[3][3]=1;x.s[3][4]=0;x.s[3][5]=1;x.s[3][6]=2;
    x.s[4][1]=1;x.s[4][2]=1;x.s[4][3]=0;x.s[4][4]=1;x.s[4][5]=1;x.s[4][6]=2;
    x.s[5][1]=0;x.s[5][2]=0;x.s[5][3]=0;x.s[5][4]=0;x.s[5][5]=1;x.s[5][6]=1;
    x.s[6][1]=0;x.s[6][2]=0;x.s[6][3]=0;x.s[6][4]=0;x.s[6][5]=0;x.s[6][6]=1;


    y[1]=sx-1;y[2]=sy-1;y[3]=dx;y[4]=dy;y[5]=0;y[6]=1;
}
int main(){

    while(scanf("%lld%lld%lld%lld%lld%lld",&n,&sx,&sy,&dx,&dy,&t)!=EOF){
        if(t==0)
            printf("%lld %lld",sx,sy);
        else{
            mod=n;
            init();

            Matrix res=quickPower(x,t,6);


            LL ex=0,ey=0;
            for(int i=1;i<=6;i++)
                ex=((ex+res.s[1][i]*y[i])%mod+mod)%mod;
            for(int i=1;i<=6;i++)
                ey=((ey+res.s[2][i]*y[i])%mod+mod)%mod;

            printf("%lld %lld\n",ex+1,ey+1);
        }
    }
    return 0;
}
```






