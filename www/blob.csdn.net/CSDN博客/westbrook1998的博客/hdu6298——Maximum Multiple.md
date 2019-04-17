# hdu6298——Maximum Multiple - westbrook1998的博客 - CSDN博客





2018年07月30日 11:37:42[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：44标签：[打表找规律](https://so.csdn.net/so/search/s.do?q=打表找规律&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
Given an integer n, Chiaki would like to find three positive integers x, y and z such that: n=x+y+z, x∣n, y∣n, z∣n and xyz is maximum.  

  Input 

  There are multiple test cases. The first line of input contains an integer T (1≤T≤106), indicating the number of test cases. For each test case:  

  The first line contains an integer n (1≤n≤106).  

  Output 

  For each test case, output an integer denoting the maximum xyz. If there no such integers, output −1 instead.  

  Sample Input 

  3 

  1 

  2 

  3 

  Sample Output 

  -1 

  -1 

  1
题解的证明看不懂 

打表找了一下规律 

1 -1 

2 -1 

3 1=1*1*1 

4 2=1*1*2 

5 -1 

6 8=2*2*2 

7 -1 

8 16=2*2*4
所以就分为三种情况 %3==0 %4==0 还是else 

代码：

```cpp
#include <cstdio>
#include <algorithm>
using namespace std;
int main(void){
    int t;
    scanf("%d",&t);
    long long n;
    while(t--){
        scanf("%lld",&n);
        if(n%3==0){
            printf("%lld\n",(n/3)*(n/3)*(n/3));
        }
        else if(n%4==0){
            printf("%lld\n",(n/4)*(n/4)*(n/2));
        }
        else{
            printf("-1\n");
        }
    }
    return 0;
}
```






