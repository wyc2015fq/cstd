# hdu4704——Sum - westbrook1998的博客 - CSDN博客





2018年08月28日 22:27:07[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：23标签：[数论																[费马小定理](https://so.csdn.net/so/search/s.do?q=费马小定理&t=blog)](https://so.csdn.net/so/search/s.do?q=数论&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
![这里写图片描述](https://odzkskevi.qnssl.com/2fb92eb08283067dc96500ff89a997ed?v=1535458066)


emmmmm狗屁费马小定理 

我们首先定义$A_n=S_1+S_2+...+S_n$ ，所以$A_{n+1}=S_1+S_2+...+S_n+S_{n+1}$ ，而$S_{n+1}$ 表示n+1个数组成n的情况数，我们考虑第一个数为$1,2,...n$ ，比如第一个数是1的情况，$S_{n+1}$ 就转化为$S_n$ ，同理，最后$S_{n+1}=S_1+S_2+...+S_n$ ，代入前式得到，$A_{n+1}=2A_n$ ，因此所求的即是$2^n$ % $m$

$2^n\%m=(2^{n\%(m-1)}\times 2^{\frac{n}{m-1}(m-1)})\%m=(2^{n\%(m-1)})\%m\times ((2^{\frac{n}{m-1}})^{m-1})\%m=2^{n\%(m-1)})\%m$

所以，最后就是将$2^n \% m$ 转化为$2^{n\%(m-1)} \%m$  ，肯定快了

代码：

```cpp
#include <cstdio>
#include <algorithm>
using namespace std;
const int N=100050;
const int MOD=1e9+7;
char s[N];
long long qPow(long long a,long long k){
    long long sum=1;
    while(k){
        if(k%2){
            sum=(sum*a)%MOD;
        }
        a=a*a%MOD;
        k>>=1;
    }
    return sum;
}
int main(void){
    while(~scanf("%s",s)){
        long long sum=0;
        int i=0;
        while(s[i]){
            sum=(sum*10+s[i]-'0')%(MOD-1);
            i++;
        }
        long long n=sum-1;
        printf("%lld\n",qPow(2,n));
    }
    return 0;
}
```




