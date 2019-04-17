# hdu6025——Coprime Sequence - westbrook1998的博客 - CSDN博客





2018年09月01日 22:35:12[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：18标签：[前缀和](https://so.csdn.net/so/search/s.do?q=前缀和&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
Do you know what is called 
```
Coprime Sequence''? That is a sequence consists of n positive integers, and the GCD (Greatest Common Divisor) of them is equal to 1.
```
Coprime Sequence” is easy to find because of its restriction. But we can try to maximize the GCD of these integers by removing exactly one integer. Now given a sequence, please maximize the GCD of its elements. 

  Input 

  The first line of the input contains an integer T(1≤T≤10), denoting the number of test cases.  

  In each test case, there is an integer n(3≤n≤100000) in the first line, denoting the number of integers in the sequence.  

  Then the following line consists of n integers a1,a2,…,an(1≤ai≤109), denoting the elements in the sequence. 

  Output 

  For each test case, print a single line containing a single integer, denoting the maximum GCD. 

  Sample Input 

  3 

  3 

  1 1 1 

  5 

  2 2 2 3 2 

  4 

  1 2 4 8 

  Sample Output 

  1 

  2 

  2
利用那种前缀和 后缀和那种思想预处理 然后枚举一下删去的数

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
const int N=100050;
int a[N];
int pre[N];
int suf[N];
int gcd(int a,int b){
    return b==0 ? a : gcd(b,a%b);
}
int main(void){
    int t;
    int n;
    scanf("%d",&t);
    while(t--){
        memset(a,0,sizeof(a));
        memset(pre,0,sizeof(pre));
        memset(suf,0,sizeof(suf));
        scanf("%d",&n);
        for(int i=0;i<n;i++){
            scanf("%d",&a[i]);
        }
        //预处理前缀和后缀gcd
        pre[0]=a[0];
        for(int i=1;i<n;i++){
            pre[i]=gcd(pre[i-1],a[i]);
        }
        suf[n-1]=a[n-1];
        for(int i=n-2;i>=0;i--){
            suf[i]=gcd(suf[i+1],a[i]);
        }
        //枚举删除的数字
        int ans=1;
        for(int i=0;i<n;i++){
            ans=max(ans,gcd(pre[i-1],suf[i+1]));
        }
        printf("%d\n",ans);
    }
    return 0;
}
```





