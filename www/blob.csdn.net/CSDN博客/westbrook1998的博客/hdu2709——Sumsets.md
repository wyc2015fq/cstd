# hdu2709——Sumsets - westbrook1998的博客 - CSDN博客





2018年05月10日 23:36:24[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：42








> 
Farmer John commanded his cows to search for different sets of numbers that sum to a given number. The cows use only numbers that are an integer power of 2. Here are the possible sets of numbers that sum to 7:  

  1) 1+1+1+1+1+1+1  

  2) 1+1+1+1+1+2  

  3) 1+1+1+2+2  

  4) 1+1+1+4  

  5) 1+2+2+2  

  6) 1+2+4  

  Help FJ count all possible representations for a given integer N (1 <= N <= 1,000,000). 

  Input 

  A single line with a single integer, N. 

  Output 

  The number of ways to represent N as the indicated sum. Due to the potential huge size of this number, print only last 9 digits (in base 10 representation). 

  Sample Input 

  7
数学题…用到递归，题目就是说将一个整数分解为`2的幂`相加（如`4=1+1+2`）共有多少种方法 

思路是这样的，首先`a[0]`和`a[1]`都是`1`，从`2`开始，如果`i`是奇数，即`i=1+(i-1)`那`a[i]`肯定等于`a[i-1]+1`，而如果i是偶数，那么两种情况，如果分解的式子中存在`1`，那一定存在两个`1`，所以`i=1+1+(i-2)`，即`a[i]=a[i-2]+2`，而如果不存在`1`，那么说明分解的式子中全都是偶数，那么也就是整个式子可以除以`2`，即`a[i]=a[i/2]`

代码：

```cpp
#include <cstdio>
#include <algorithm>
using namespace std;
long long arr[1000010];
int main(void){
    int n;
    arr[0]=1;
    arr[1]=1;
    for(int i=2;i<=1000000;i++){
        if(i%2==0){
            arr[i]=(arr[i-2]+arr[i/2])%1000000000;
        }
        else{
            arr[i]=arr[i-1]%1000000000;
        }
    }
    while(~scanf("%d",&n)){
        printf("%lld\n",arr[n]%1000000000);
    }
    return 0;
}
```

注意看清楚题意然后取模





