# Fast Matrix Calculation（HDU-4965） - Alex_McAvoy的博客 - CSDN博客





2019年03月28日 19:43:55[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：31
个人分类：[HDU																[矩阵——矩阵快速幂](https://blog.csdn.net/u011815404/article/category/8800018)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

One day, Alice and Bob felt bored again, Bob knows Alice is a girl who loves math and is just learning something about matrix, so he decided to make a crazy problem for her. 

Bob has a six-faced dice which has numbers 0, 1, 2, 3, 4 and 5 on each face. At first, he will choose a number N (4 <= N <= 1000), and for N times, he keeps throwing his dice for K times (2 <=K <= 6) and writes down its number on the top face to make an N*K matrix A, in which each element is not less than 0 and not greater than 5. Then he does similar thing again with a bit difference: he keeps throwing his dice for N times and each time repeat it for K times to write down a K*N matrix B, in which each element is not less than 0 and not greater than 5. With the two matrix A and B formed, Alice’s task is to perform the following 4-step calculation. 

Step 1: Calculate a new N*N matrix C = A*B. 

Step 2: Calculate M = C^(N*N). 

Step 3: For each element x in M, calculate x % 6. All the remainders form a new matrix M’. 

Step 4: Calculate the sum of all the elements in M’. 

Bob just made this problem for kidding but he sees Alice taking it serious, so he also wonders what the answer is. And then Bob turn to you for help because he is not good at math.

# **Input**

The input contains several test cases. Each test case starts with two integer N and K, indicating the numbers N and K described above. Then N lines follow, and each line has K integers between 0 and 5, representing matrix A. Then K lines follow, and each line has N integers between 0 and 5, representing matrix B. 

The end of input is indicated by N = K = 0.

# Output

For each case, output the sum of all the elements in M’ in a line.

# Sample Input

**4 25 54 45 40 04 2 5 51 3 1 56 31 2 30 3 02 3 44 3 22 5 50 5 03 4 5 1 1 05 3 2 3 3 23 1 5 4 5 20 0**

# Sample Output

**1456**


题意：给出 n*k 的两个矩阵，令矩阵相乘后求 n*n 次幂，最后求所有位置上的元素和

思路：

按照题目所述进行矩阵快速幂计算即可，但由于 n 最大到 1000，k 最大到 6，两个 n*k 的矩阵相乘后得到的矩阵大小为 n*n，再求 n*n 次幂，无疑会超时，因此需要对矩阵进行简化操作

根据 ![(A*B)^{n*n}=A*(B*A)^{n*n-1}*B](https://private.codecogs.com/gif.latex?%28A*B%29%5E%7Bn*n%7D%3DA*%28B*A%29%5E%7Bn*n-1%7D*B)，那么可知 ![(B*A)^{n*n-1}](https://private.codecogs.com/gif.latex?%28B*A%29%5E%7Bn*n-1%7D) 中 B*A 的大小为 k*k 最大为 6，极大的简化了运算

因此，可以先计算 ![(B*A)^{n*n-1}](https://private.codecogs.com/gif.latex?%28B*A%29%5E%7Bn*n-1%7D)，然后再对矩阵进行乘法，分别乘上 A、B 两个矩阵

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
#define E 1e-9
#define PI acos(-1.0)
#define INF 0x3f3f3f3f
#define LL long long
const int MOD=6;
const int N=1000+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;
struct Matrix{
    LL s[10][10];
};
Matrix C;//B*A
Matrix e;//单位矩阵E
int n,k;
void init(){
    for(int i=1;i<10;i++)//主对角线为1
        e.s[i][i]=1;
}
Matrix mul(Matrix A,Matrix B){//矩阵乘法，n代表A、B两个矩阵是n阶方阵
    Matrix temp;//临时矩阵，存放A*B结果

    for(int i=1;i<=k;i++)
        for(int j=1;j<=k;j++)
            temp.s[i][j]=0;

    for(int i=1;i<=k;i++)
        for(int j=1;j<=k;j++)
            for(int t=1;t<=k;t++)
                temp.s[i][j]=(temp.s[i][j]+A.s[i][t]*B.s[t][j])%MOD;
    return temp;
}
Matrix quickPower(Matrix a,int b){//矩阵快速幂，求矩阵n阶矩阵的b次幂
    Matrix ans=e;
    while(b){
        if(b&1)
            ans=mul(ans,a);//ans=e*a
        a=mul(a,a);//a=a*a
        b>>=1;
    }
    return ans;
}
int A[N][10],B[10][N];
int a[N][N],b[N][N];
int main(){

    init();
    while(scanf("%d%d",&n,&k)!=EOF&&(n+k)){

        for(int i=1;i<=n;i++)
            for(int j=1;j<=k;j++)
                scanf("%d",&A[i][j]);
        for(int i=1;i<=k;i++)
            for(int j=1;j<=n;j++)
                scanf("%d",&B[i][j]);



        //计算B*A
        for(int i=1;i<=k;i++){
            for(int j=1;j<=k;j++){
                C.s[i][j]=0;
                for(int t=1;t<=n;t++){
                    C.s[i][j]=(C.s[i][j]+B[i][t]*A[t][j])%MOD;
                }
            }
        }


        Matrix res=quickPower(C,n*n-1);

        //计算A(B*A)
        for(int i=1;i<=n;i++){
            for(int j=1;j<=k;j++){
                a[i][j]=0;
                for(int t=1;t<=k;t++){
                    a[i][j]=(a[i][j]+A[i][t]*res.s[t][j])%MOD;
                }
            }
        }

        //计算A(B*A)B
        for(int i=1;i<=n;i++){
            for(int j=1;j<=n;j++){
                b[i][j]=0;
                for(int t=1;t<=k;t++){
                    b[i][j]=(b[i][j]+a[i][t]*B[t][j])%MOD;
                }
            }
        }

        int sum=0;
        for(int i=1;i<=n;i++)
            for(int j=1;j<=n;j++)
                sum+=b[i][j];

        printf("%d\n",sum);
    }
    return 0;
}
```






