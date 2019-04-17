# hdu1019——Least Common Multiple - westbrook1998的博客 - CSDN博客





2018年07月14日 08:26:10[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：37标签：[水题](https://so.csdn.net/so/search/s.do?q=水题&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
The least common multiple (LCM) of a set of positive integers is the smallest positive integer which is divisible by all the numbers in the set. For example, the LCM of 5, 7 and 15 is 105.  

  Input 

  Input will consist of multiple problem instances. The first line of the input will contain a single integer indicating the number of problem instances. Each instance will consist of a single line of the form m n1 n2 n3 … nm where m is the number of integers in the set and n1 … nm are the integers. All integers will be positive and lie within the range of a 32-bit integer.  

  Output 

  For each problem instance, output a single line containing the corresponding LCM. All results will lie in the range of a 32-bit integer.  

  Sample Input 

  2 

  3 5 7 15 

  6 4 10296 936 1287 792 1 

  Sample Output 

  105 

  10296
水题飘过

代码：

```cpp
#include <cstdio>
#include <algorithm>
using namespace std;
long long gcd(long long a,long long b){
    return b==0 ? a : gcd(b,a%b);
}
long long LCM(long long a,long long b){
    return a/gcd(a,b)*b;
}
int main(void){
    int t;
    scanf("%d",&t);
    while(t--){
        int n;
        scanf("%d",&n);
        long long ans,t;
        scanf("%lld",&t);
        ans=t;
        for(int i=1;i<n;i++){
            scanf("%lld",&t);
            ans=LCM(ans,t);
        }
        printf("%lld\n",ans);
    }
    return 0;
}
```





