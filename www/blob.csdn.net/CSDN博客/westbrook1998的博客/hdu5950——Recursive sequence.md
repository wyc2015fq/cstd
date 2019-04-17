# hdu5950——Recursive sequence - westbrook1998的博客 - CSDN博客





2018年09月01日 22:24:40[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：17标签：[矩阵快速幂](https://so.csdn.net/so/search/s.do?q=矩阵快速幂&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
Farmer John likes to play mathematics games with his N cows. Recently, they are attracted by recursive sequences. In each turn, the cows would stand in a line, while John writes two positive numbers a and b on a blackboard. And then, the cows would say their identity number one by one. The first cow says the first number a and the second says the second number b. After that, the i-th cow says the sum of twice the (i-2)-th number, the (i-1)-th number, and i4. Now, you need to write a program to calculate the number of the N-th cow in order to check if John’s cows can make it right.  

  Input 

  The first line of input contains an integer t, the number of test cases. t test cases follow.  

  Each case contains only one line with three numbers N, a and b where N,a,b < 231 as described above.  

  Output 

  For each test case, output the number of the N-th cow. This number might be very large, so you need to output it modulo 2147493647. 

  Sample Input 

  2 

  3 1 2 

  4 1 10 

  Sample Output 

  85 

  369  

  Hint 

  In the first case, the third number is 85 = 2*1十2十3^4. 

   In the second case, the third number is 93 = 2*1十1*10十3^4 and the fourth number is 369 = 2 * 10 十 93 十 4^4.
学习用矩阵快速幂做这种递推问题 构造矩阵是很重要的 还是要多练

> 
给出递推公式$f_n=f_{n-1}+2f_{n-2}+n^4 \quad f_1=a,f_2=b$

要构造一个矩阵A使得 


$> \left[> \begin{matrix}> f_{n} &f_{n-1} & n^4 & n^3 & n^2 & n & 1  > \end{matrix}> \right]> => \left[> \begin{matrix}> f_{n-1} &f_{n-2} & (n-1)^4 & (n-1)^3 & (n-1)^2 & (n-1) & 1  > \end{matrix}> \right]> \times > A> $

  根据矩阵乘法可以得到 


$> A=> \left[> \begin{matrix}> 1 & 1 & 0 & 0 & 0 & 0 & 0 \\> 2 & 0 & 0 & 0 & 0 & 0 & 0 \\> 1 & 0 & 1 & 0 & 0 & 0 & 0 \\> 4 & 0 & 4 & 1 & 0 & 0 & 0 \\> 6 & 0 & 6 & 3 & 1 & 0 & 0 \\> 4 & 0 & 4 & 3 & 2 & 1 & 0 \\> 1 & 0 & 1 & 1 & 1 & 1 & 1 \\> \end{matrix}> \right]> $
代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
typedef long long ll;
using namespace std;
const int N=7;
const ll MOD=2147493647;
struct Mat{
    ll m[N][N];
};
Mat operator * (Mat a,Mat b){
    Mat t;
    memset(t.m,0,sizeof(t.m));
    for(int k=0;k<N;k++){
        for(int i=0;i<N;i++){
            if(a.m[i][k]==0){
                continue;
            }
            for(int j=0;j<N;j++){
                if(b.m[k][j]==0){
                    continue;
                }
                t.m[i][j]=(t.m[i][j]+(a.m[i][k]%MOD)*(b.m[k][j]%MOD))%MOD;
            }
        }
    }
    return t;
}
Mat operator ^ (Mat a,int n){
    Mat t;
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            t.m[i][j]=i==j?1:0;
        }
    }
    while(n){
        if(n%2){
            t=t*a;
        }
        n/=2;
        a=a*a;
    }
    return t;
}
int main(void){
    int t;
    scanf("%d",&t);
    ll n,a,b;
    Mat k;
    memset(k.m,0,sizeof(k.m));
    k.m[0][0]=k.m[0][1]=k.m[2][0]=k.m[2][2]=k.m[3][3]=k.m[4][4]=k.m[5][5]=k.m[6][0]=k.m[6][2]=k.m[6][3]=k.m[6][4]=k.m[6][5]=k.m[6][6]=1;
    k.m[1][0]=k.m[5][4]=2;
    k.m[4][3]=k.m[5][3]=3;
    k.m[3][0]=k.m[3][2]=k.m[5][0]=k.m[5][2]=4;
    k.m[4][0]=k.m[4][2]=6;
    Mat g;
    while(t--){
        scanf("%lld%lld%lld",&n,&a,&b);
        if(n==1){
            printf("%lld\n",a%MOD);
        }
        else if(n==2){
            printf("%lld\n",b%MOD);
        }
        else{
            memset(g.m,0,sizeof(g));
            g.m[0][0]=b;
            g.m[0][1]=a;
            g.m[0][2]=16;
            g.m[0][3]=8;
            g.m[0][4]=4;
            g.m[0][5]=2;
            g.m[0][6]=1;
            printf("%lld\n",(g*(k^(n-2))).m[0][0]%MOD);
        }
    }
    return 0;
}
```







