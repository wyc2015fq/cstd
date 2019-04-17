# poj3070——Fibonacci - westbrook1998的博客 - CSDN博客





2018年06月13日 19:01:38[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：29标签：[矩阵快速幂](https://so.csdn.net/so/search/s.do?q=矩阵快速幂&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
In the Fibonacci integer sequence, F0 = 0, F1 = 1, and Fn = Fn − 1 + Fn − 2 for n ≥ 2. For example, the first ten terms of the Fibonacci sequence are: 

  0, 1, 1, 2, 3, 5, 8, 13, 21, 34, … 

  An alternative formula for the Fibonacci sequence is 
![这里写图片描述](https://odzkskevi.qnssl.com/23da167d8116d97497f11a1e8ecd74fa?v=1528886385)

  Given an integer n, your goal is to compute the last 4 digits of Fn. 

  Input 

  The input test file will contain multiple test cases. Each test case consists of a single line containing n (where 0 ≤ n ≤ 1,000,000,000). The end-of-file is denoted by a single line containing the number −1. 

  Output 

  For each test case, print the last four digits of Fn. If the last four digits of Fn are all zeros, print ‘0’; otherwise, omit any leading zeros (i.e., print Fn mod 10000). 

  Sample Input 

  0 

  9 

  999999999 

  1000000000 

  -1 

  Sample Output 

  0 

  34 

  626 

  6875
学习矩阵快速幂的一道例题

代码：

```cpp
#include <cstdio>
#include <algorithm>
using namespace std;
const int MAXN=2;
const int MOD=10000;
struct Mat{
    int m[MAXN][MAXN];
}mat;
Mat mul(Mat a,Mat b){
    Mat t;
    for(int i=0;i<MAXN;i++){
        for(int j=0;j<MAXN;j++){
            t.m[i][j]=0;
            for(int k=0;k<MAXN;k++){
                t.m[i][j]=(t.m[i][j]+(a.m[i][k]*b.m[k][j]))%MOD;
            }
        }
    }
    return t;
}
Mat quick_pow(Mat a,int n){
    Mat t;
    for(int i=0;i<MAXN;i++){
        for(int j=0;j<MAXN;j++){
            t.m[i][j]= i==j ? 1 : 0;
        }
    }
    while(n>0){
        if(n%2==1){
            t=mul(t,a);
        }
        n=n/2;
        a=mul(a,a);
    }
    return t;
}
int main(void){
    int n;
    while(~scanf("%d",&n)){
        if(n==-1){
            break;
        }
        mat.m[0][0]=1;
        mat.m[0][1]=1;
        mat.m[1][0]=1;
        mat.m[1][1]=0;
        printf("%d\n",quick_pow(mat,n).m[0][1]);
    }
    return 0;
}
```





