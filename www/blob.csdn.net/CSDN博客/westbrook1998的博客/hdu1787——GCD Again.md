# hdu1787——GCD Again - westbrook1998的博客 - CSDN博客





2018年08月17日 21:23:02[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：33标签：[欧拉函数																[简单数论](https://so.csdn.net/so/search/s.do?q=简单数论&t=blog)](https://so.csdn.net/so/search/s.do?q=欧拉函数&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
Do you have spent some time to think and try to solve those unsolved problem after one ACM contest?  

  No? Oh, you must do this when you want to become a “Big Cattle”.  

  Now you will find that this problem is so familiar:  

  The greatest common divisor GCD (a, b) of two positive integers a and b, sometimes written (a, b), is the largest divisor common to a and b. For example, (1, 2) =1, (12, 18) =6. (a, b) can be easily found by the Euclidean algorithm. Now I am considering a little more difficult problem:  

  Given an integer N, please count the number of the integers M`(0<M<N)`which satisfies `(N,M)`>1.  

  This is a simple version of problem “GCD” which you have done in a contest recently,so I name this problem “GCD Again”.If you cannot solve it still,please take a good think about your method of study.  

  Good Luck!  

  Input 

  Input contains multiple test cases. Each test case contains an integers N`(1<N<100000000)`. A test case containing 0 terminates the input and this test case is not to be processed.  

  Output 

  For each integers N you should output the number of integers M in one line, and with one line of output for each line in input.  

  Sample Input 

  2 

  4 

  0 

  Sample Output 

  0 

  1
这题以为是素数，结果数组没办法开那么打，原来是欧拉函数

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
const int N=100000020;
int Euler(int n){
    int res=n;
    int a=n;
    for(int i=2;i*i<=a;i++){
        if(a%i==0){
            //每个因子计算一次
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
    while(~scanf("%d",&n) && n){
        printf("%d\n",n-1-Euler(n));
    }
    return 0;
}
```






