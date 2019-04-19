# hdu 3359 Kind of a Blur（高斯消元） - HJ - CSDN博客
2017年10月19日 21:49:06[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：244
XOR is a kind of bit operator, we define that as follow: for two binary base number A and B, let C=A XOR B, then for each bit of C, we can get its value by check the digit of corresponding position in A and B. And for each digit, 1 XOR 1 = 0, 1 XOR 0 = 1, 0 XOR 1 = 1, 0 XOR 0 = 0. And we simply write this operator as ^, like 3 ^ 1 = 2,4 ^ 3 = 7. XOR is an amazing operator and this is a question about XOR. We can choose several numbers and do XOR operatorion to them one by one, then we get another number. For example, if we choose 2,3 and 4, we can get 2^3^4=5. Now, you are given N numbers, and you can choose some of them(even a single number) to do XOR on them, and you can get many different numbers. Now I want you tell me which number is the K-th smallest number among them. 
Input 
    First line of the input is a single integer T(T<=30), indicates there are T test cases.
Output 
    For each test case,first output Case #C: in a single line,C means the number of the test case which is from 1 to T. Then for each query, you should output a single line contains the Ki-th smallest number in them, if there are less than Ki different numbers, output -1.
Sample Input
```
2
2
1 2
4
1 2 3 4
3
1 2 3
5
1 2 3 4 5
```
Sample Output
```
Case #1:
1
2
3
-1
Case #2:
0
1
2
3
-1
```
Hint
If you choose a single number, the result you get is the number you choose.
浮点数高斯消元模板题。
题目大意： 
给一出个M*N的矩阵，每个点有个浮点数，求它的原矩阵，原矩阵对应点的值为曼哈顿距离<=d的点之和的平均值。
```cpp
#include<iostream> 
#include<cstring>
#include<cstdio>
#include<cmath>
#define INF 0x3f3f3f3f
using namespace std;
const int maxn=105;
double A[maxn][maxn],S[maxn][maxn];
void Gauss(int n)
{
    for(int i=0;i<n;i++)
    {
        int r=i;
        for(int j=i+1;j<n;j++)    //每次从[i,n)行中找到第i列中元素绝对值最大的行 
          if(fabs(A[j][i])>fabs(A[r][i])) //找到最大的行是为了在消元的时候把浮点数误差降到最小 
            r=j;
        if(r!=i)        //如果这个r行不是i行 就将r行和i行进行交换 
          for(int j=0;j<=n;j++)
            swap(A[r][j],A[i][j]); 
        //线性代数中的初等行变换 : 将第j行中的所有的元素 - 第i行的所有的元素 * a[j][col]/a[i][col] 
        for(int k=i+1;k<n;k++)
        {
            double f=A[k][i]/A[i][i];
            for(int j=i;j<=n;j++)
              A[k][j]-=f*A[i][j];
        }
    }
    for(int i=n-1;i>=0;i--)    //求出n个线性方程组的解 
    {
        for(int j=i+1;j<n;j++)
          A[i][n]-=A[j][n]*A[i][j];      
        A[i][n]/=A[i][i];       
    }
}
int main()
{
    int n,m,d,first=true;
    while(~scanf("%d%d%d",&n,&m,&d)&&m)
    {
        memset(A,0,sizeof(A));
        for(int i=0;i<m;i++)
          for(int j=0;j<n;j++)
            scanf("%lf",&S[i][j]);
        for(int i=0;i<m;i++)
          for(int j=0;j<n;j++)
          {
              int cnt=0;
              for(int dx=-d;dx<=d;dx++)
                for(int dy=-d;dy<=d;dy++)
                {
                    int x=i+dx;
                    int y=j+dy;
                    if(x>=0&&x<m&&y>=0&&y<n&&abs(x-i)+abs(y-j)<=d)
                    {
                        cnt++;
                        A[i*n+j][x*n+y]=1;
                    }
                }
              A[i*n+j][m*n]=cnt*S[i][j];
          }
        Gauss(m*n);
        if(first)
          first=false;
        else
          printf("\n");
        for(int i=0;i<m;i++)
        {
            for(int j=0;j<n;j++)
              printf("%8.2lf",A[i*n+j][m*n]);
            printf("\n");
        }
    }
    return 0;
}
```
