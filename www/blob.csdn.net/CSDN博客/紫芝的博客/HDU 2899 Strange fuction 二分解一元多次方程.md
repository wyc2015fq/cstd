# HDU 2899 Strange fuction 二分解一元多次方程 - 紫芝的博客 - CSDN博客





2018年07月23日 09:01:15[紫芝](https://me.csdn.net/qq_40507857)阅读数：33








# Strange fuction

****Time Limit: 2000/1000 MS (Java/Others)    Memory Limit: 32768/32768 K (Java/Others)Total Submission(s): 9479    Accepted Submission(s): 6447****

**Problem Description**

Now, here is a fuction:

  F(x) = 6 * x^7+8*x^6+7*x^3+5*x^2-y*x (0 <= x <=100)

Can you find the minimum value when x is between 0 and 100.

**Input**

The first line of the input contains an integer T(1<=T<=100) which means the number of test cases. Then T lines follow, each line has only one real numbers Y.(0 < Y <1e10)

**Output**

Just the minimum value (accurate up to 4 decimal places),when x is between 0 and 100.

**Sample Input**

2 

100 

200

**Sample Output**

-74.4291 

-178.8534

**Author**

Redow

**Recommend**

lcy   |   We have carefully selected several similar problems for you:  [2199](http://acm.hdu.edu.cn/showproblem.php?pid=2199)[2289](http://acm.hdu.edu.cn/showproblem.php?pid=2289)[2298](http://acm.hdu.edu.cn/showproblem.php?pid=2298)[2141](http://acm.hdu.edu.cn/showproblem.php?pid=2141)[3400](http://acm.hdu.edu.cn/showproblem.php?pid=3400)

### 求函数的最小值F(x) = 6 * x^7+8*x^6+7*x^3+5*x^2-y*x (0 <= x <=100)当导数为0时就是函数的极值点他的导数42X^6+48X^5+21X^2+10X-Y，很明显只有一个转折点所以在这里取得最小值，实际上就是求他的导数等于0时候的值。这一题用三分法也是可以的。用导数更直接。

```cpp
#include<bits/stdc++.h>
using namespace std;
#define eps 1e-8
int y;
double f(double x)
{
    return 6*pow(x,7)+8*pow(x,6)+7*pow(x,3)+5*pow(x,2)-y*x;
}
double ff(double x)
{
    return 42*pow(x,6)+48*pow(x,5)+21*pow(x,2)+10*x-y;
}
int main()
{
    int T;
    scanf("%d",&T);
    while(T--){
        scanf("%d",&y);
        double low=0,high=100,mid;
        while(high-low>eps){
            mid=(low+high)/2;
            if(ff(mid)<eps) low=mid;
            else high=mid;
        }
        printf("%.4lf\n",f(mid));
    }
    return 0;
}
```





