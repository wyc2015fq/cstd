# hdu2588——GCD - westbrook1998的博客 - CSDN博客





2018年06月16日 20:26:20[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：43标签：[数论																[gcd																[欧拉函数](https://so.csdn.net/so/search/s.do?q=欧拉函数&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)





> 
The greatest common divisor GCD(a,b) of two positive integers a and b,sometimes written (a,b),is the largest divisor common to a and b,For example,(1,2)=1,(12,18)=6.  

  (a,b) can be easily found by the Euclidean algorithm. Now Carp is considering a little more difficult problem:  

  Given integers N and M, how many integer X satisfies 1<=X<=N and (X,N)>=M. 

  Input 

  The first line of input is an integer T(T<=100) representing the number of test cases. The following T lines each contains two numbers N and M (2<=N<=1000000000, 1<=M<=N), representing a test case. 

  Output 

  For each test case,output the answer on a single line. 

  Sample Input 

  3 

  1 1 

  10 2 

  10000 72 

  Sample Output 

  1 

  6 

  260
这题顺便学了个欧拉函数 f(n)表示1到n中与n互质的数的个数 

这题要求的是有多少个x在[1,n]中，满足gcd(x,n)>=m 
`假设d=gcd(x,n) 那么gcd(x/d,n/d)==1`，这个结论很重要 

而这里1到n/d(x/d肯定在里面)有多少个与n/d互质(gcd==1)的数其实就是欧拉函数f(n/d) 

而前面提到d是gcd(x,n)，也就是我们可以来枚举n的因数d，然后把每一个f(n/d)加起来 

这里犯了两个错误，刚开始想把欧拉函数先打表存起来结果数组太大 hdu编译不了，第二个错误就是不应该从1到n枚举 而只需要枚举到sqrt(n) 因为枚举了d，即n/d也一定是n的因数，所以这样才不会超时
代码：

```
#include <cstdio>
#include <algorithm>
using namespace std;
int Euler(int n){
    int res=n;
    int a=n;
    for(int i=2;i*i<=a;i++){
        if(a%i==0){
            res=res/i*(i-1);
            while(a%i==0){
                a/=i;
            }
        }
    }
    if(a>1){
        res=res/a*(a-1);
    }
    return res;
}
int main(void){
    int n;
    scanf("%d",&n);
    int N,M;
    for(int i=0;i<n;i++){
        scanf("%d%d",&N,&M);
        long long res=0;
        for(int j=1;j*j<=N;j++){
            if(N%j==0){
                if(j*j==N){
                    if(j>=M){
                        //printf("%d\n",j);
                        res+=Euler(N/j);
                    }
                }
                else{
                    if(j>=M){
                        //printf("%d\n",j);
                        res+=Euler(N/j);
                    }
                    if(N/j>=M){
                        //printf("%d\n",j);
                        res+=Euler(j);
                    }
                }
            }
        }
        printf("%lld\n",res);

    }
    return 0;
}
```](https://so.csdn.net/so/search/s.do?q=gcd&t=blog)](https://so.csdn.net/so/search/s.do?q=数论&t=blog)




