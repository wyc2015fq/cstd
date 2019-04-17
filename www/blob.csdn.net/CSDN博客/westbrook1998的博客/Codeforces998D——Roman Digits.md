# Codeforces998D——Roman Digits - westbrook1998的博客 - CSDN博客





2018年08月28日 19:13:46[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：45标签：[暴力打表																[找规律](https://so.csdn.net/so/search/s.do?q=找规律&t=blog)](https://so.csdn.net/so/search/s.do?q=暴力打表&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
Let’s introduce a number system which is based on a roman digits. There are digits I, V, X, L which correspond to the numbers 1, 5, 10 and 50 respectively. The use of other roman digits is not allowed. 

  Numbers in this system are written as a sequence of one or more digits. We define the value of the sequence simply as the sum of digits in it. 

  For example, the number XXXV evaluates to 35 and the number IXI — to 12. 

  Pay attention to the difference to the traditional roman system — in our system any sequence of digits is valid, moreover the order of digits doesn’t matter, for example IX means 11, not 9. 

  One can notice that this system is ambiguous, and some numbers can be written in many different ways. Your goal is to determine how many distinct integers can be represented by exactly n roman digits I, V, X, L. 

  Input 

  The only line of the input file contains a single integer n (1≤n≤109) — the number of roman digits to use. 

  Output 

  Output a single integer — the number of distinct integers which can be represented using n roman digits exactly. 

  Examples 

  Input 

  1 

  Output 

  4 

  Input 

  2 

  Output 

  10 

  Input 

  10 

  Output 

  244 

  Note 

  In the first sample there are exactly 4 integers which can be represented — I, V, X and L. 

  In the second sample it is possible to represent integers 2 (II), 6 (VI), 10 (VV), 11 (XI), 15 (XV), 20 (XX), 51 (IL), 55 (VL), 60 (XL) and 100 (LL).
这题暴力打表  然后做差看一下… 

过了 

代码：
```
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <set>
using namespace std;
// int a[4]={1,5,10,50};
// int da[55];
// set<int> te;
int main(void){
    //打表
    // for(int o=1;o<=50;o++){
    //     for(int i=0;i<=o;i++){
    //         for(int j=0;j<=o;j++){
    //             for(int k=0;k<=o;k++){
    //                 for(int l=0;l<=o;l++){
    //                     // if(o==1){
    //                     //     printf("%d\n",i+j+k+l);
    //                     // }
    //                     if(i+j+k+l==o){
    //                         // if(o==1){
    //                         //     printf("%d\n",a[0]*i+a[1]*j+a[2]*k+a[3]*l);
    //                         // }
    //                         te.insert(a[0]*i+a[1]*j+a[2]*k+a[3]*l);
    //                     }
    //                 }
    //             }
    //         }
    //     }
    //     da[o]=int(te.size());
    //     te.clear();
    // }
    // for(int i=1;i<=50;i++){
    //     printf("%d ",da[i]);
    // }
    // printf("\n");
    // for(int i=1;i<=50;i++){
    //     printf("%d ",da[i]-da[i-1]);
    // }
    // printf("\n");
    long long n;
    long long a[12]={0,4 ,10, 20, 35, 56, 83, 116, 155, 198, 244, 292};
    scanf("%lld",&n);
    if(n<=11){
        printf("%lld\n",a[n]);
    }
    else{
        printf("%lld\n",(n-11)*49+292);
    }
    return 0;
}
```







