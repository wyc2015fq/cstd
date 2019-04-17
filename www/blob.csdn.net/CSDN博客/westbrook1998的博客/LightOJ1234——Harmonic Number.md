# LightOJ1234——Harmonic Number - westbrook1998的博客 - CSDN博客





2018年08月21日 08:15:56[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：31标签：[调和级数																[基础数论](https://so.csdn.net/so/search/s.do?q=基础数论&t=blog)](https://so.csdn.net/so/search/s.do?q=调和级数&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
In mathematics, the nth harmonic number is the sum of the reciprocals of the first n natural numbers: 
![这里写图片描述](https://odzkskevi.qnssl.com/0cb534bd5b204f332e61e09afc075aa3?v=1534735514)

  In this problem, you are given n, you have to find Hn. 

  Input 

  Input starts with an integer T (≤ 10000), denoting the number of test cases. 

  Each case starts with a line containing an integer n (1 ≤ n ≤ 108). 

  Output 

  For each case, print the case number and the nth harmonic number. Errors less than 10-8 will be ignored. 

  Sample Input 

  12 

  1 

  2 

  3 

  4 

  5 

  6 

  7 

  8 

  9 

  90000000 

  99999999 

  100000000 

  Sample Output 

  Case 1: 1 

  Case 2: 1.5 

  Case 3: 1.8333333333 

  Case 4: 2.0833333333 

  Case 5: 2.2833333333 

  Case 6: 2.450 

  Case 7: 2.5928571429 

  Case 8: 2.7178571429 

  Case 9: 2.8289682540 

  Case 10: 18.8925358988 

  Case 11: 18.9978964039 

  Case 12: 18.9978964139
差不多就是求调和级数的前n项和 

n比较小用暴力求 n比较大用近似值公式 有可能超时或精度wa 调整一下这个边界

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <cmath>
using namespace std;
const double C=0.5772156649015328606;
int main(void){
    int t;
    int n;
    scanf("%d",&t);
    int c=1;
    while(t--){
        scanf("%d",&n);
        double ans=0;
        if(n<1000000){
            for(int i=1;i<=n;i++){
                ans+=1.0/i;
            }
        }
        else{
            ans=log((double)n)+C+1.0/(2*n);
        }
        printf("Case %d: %.10lf\n",c++,ans);
    }
    return 0;
}
```






