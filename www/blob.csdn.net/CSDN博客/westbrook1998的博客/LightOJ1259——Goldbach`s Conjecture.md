# LightOJ1259——Goldbach`s Conjecture - westbrook1998的博客 - CSDN博客





2018年08月20日 23:14:37[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：26标签：[素数																[基础数论](https://so.csdn.net/so/search/s.do?q=基础数论&t=blog)](https://so.csdn.net/so/search/s.do?q=素数&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
Goldbach’s conjecture is one of the oldest unsolved problems in number theory and in all of mathematics. It states: 

  Every even integer, greater than 2, can be expressed as the sum of two primes [1]. 

  Now your task is to check whether this conjecture holds for integers up to 107. 

  Input 

  Input starts with an integer T (≤ 300), denoting the number of test cases. 

  Each case starts with a line containing an integer n (4 ≤ n ≤ 107, n is even). 

  Output 

  For each case, print the case number and the number of ways you can express n as sum of two primes. To be more specific, we want to find the number of (a, b) where 

  1)      Both a and b are prime 

  2)      a + b = n 

  3)      a ≤ b 

  Sample Input 

  2 

  6 

  4 

  Sample Output 

  Case 1: 1 

  Case 2: 1 

  Note 

  1.      An integer is said to be prime, if it is divisible by exactly two different integers. First few primes are 2, 3, 5, 7, 11, 13, …
要求出n分解成两个素数的方法数 

先素数打表 然后暴力 这里素数打表是打所有素数 而不是判断每一个数是否是素数

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <cmath>
const int N=1e7+10;
bool prime[N];
int p[700000];
int k;
void init(){
    memset(prime,true,sizeof(prime));
    prime[0]=prime[1]=false;
    for(int i=2;i<=N;i++){
        if(prime[i]){
            p[k++]=i;
        }
        for(int j=2*i;j<=N;j+=i){
            prime[j]=false;
        }
    }
}
using namespace std;
int main(void){
    init();
    int t,n;
    int c=1;
    scanf("%d",&t);
    while(t--){
        scanf("%d",&n);
        int ans=0;
        for(int i=0;i<k && p[i]<=n/2;i++){
            if(prime[n-p[i]]){
                //printf("%d %d\n",p[i],n-p[i]);
                ans++;
            }
        }
        printf("Case %d: %d\n",c++,ans);
    }
    return 0;
}
```






