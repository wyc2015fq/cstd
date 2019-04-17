# Error Curves（HDU-3714） - Alex_McAvoy的博客 - CSDN博客





2019年02月15日 19:32:40[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：129








> 
# Problem Description

Josephina is a clever girl and addicted to Machine Learning recently. She 

pays much attention to a method called Linear Discriminant Analysis, which 

has many interesting properties. 

In order to test the algorithm's efficiency, she collects many datasets. 

What's more, each data is divided into two parts: training data and test 

data. She gets the parameters of the model on training data and test the 

model on test data. To her surprise, she finds each dataset's test error curve is just a parabolic curve. A parabolic curve corresponds to a quadratic function. In mathematics, a quadratic function is a polynomial function of the form f(x) = ax2 + bx + c. The quadratic will degrade to linear function if a = 0. 

![](https://img-blog.csdnimg.cn/20190215190502458.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

It's very easy to calculate the minimal error if there is only one test error curve. However, there are several datasets, which means Josephina will obtain many parabolic curves. Josephina wants to get the tuned parameters that make the best performance on all datasets. So she should take all error curves into account, i.e., she has to deal with many quadric functions and make a new error definition to represent the total error. Now, she focuses on the following new function's minimum which related to multiple quadric functions. The new function F(x) is defined as follows: F(x) = max(Si(x)), i = 1...n. The domain of x is [0, 1000]. Si(x) is a quadric function. Josephina wonders the minimum of F(x). Unfortunately, it's too hard for her to solve this problem. As a super programmer, can you help her?

# Input

The input contains multiple test cases. The first line is the number of cases T (T < 100). Each case begins with a number n (n ≤ 10000). Following n lines, each line contains three integers a (0 ≤ a ≤ 100), b (|b| ≤ 5000), c (|c| ≤ 5000), which mean the corresponding coefficients of a quadratic function.

# Output

For each test case, output the answer in a line. Round to 4 digits after the decimal point.

# **Sample Input**

**212 0 022 0 02 -4 2**

# Sample Output

**0.00000.5000**


题意：t 组数据，每组给出 n 个数 a、b、c，代表第 i 个抛物线 fi 的系数 ai、bi、ci，现定义 F(i) = max(f(i))，求 F(x) 在区间 [0,1000] 上的最小值

思路：

对于第 i 个抛物线 f(i)，由于 a>=0，当 a=0 时，曲线为直线，当 a>0 时，曲线为开口向上的抛物线，为下凸函数

因此函数 F(x) 亦为下凸函数，因而可算出 F(x) 的具体值，然后用三分法求极小值

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<cstring>
#include<cmath>
#include<ctime>
#include<algorithm>
#include<utility>
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-9
#define INF 0x3f3f3f3f
#define N 100001
#define LL long long
const int MOD=20091226;
const int dx[]={-1,1,0,0};
const int dy[]={0,0,-1,1};
using namespace std;
double a[N],b[N],c[N];
int n;
double calculate(double x){
    double maxx=a[1]*x*x+b[1]*x+c[1];
    for(int i=2;i<=n;i++){
        double y=a[i]*x*x+b[i]*x+c[i];
        maxx=max(maxx,y);
    }
    return maxx;
}
int main(){
    int t;
    scanf("%d",&t);
    while(t--){
        scanf("%d",&n);
        for(int i=1;i<=n;i++)
            scanf("%lf%lf%lf",&a[i],&b[i],&c[i]);

        double left=0,right=1000;
        while(right-left>E){
            double mid=(right+left)/2;
            double midd=(mid+right)/2;
            if(calculate(mid)<calculate(midd))
                right=midd;
            else
                left=mid;
        }
        printf("%.4lf\n",calculate(left));
    }
    return 0;
}
```





