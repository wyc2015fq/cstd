# hdu  2199   Can you solve this equation?（二分） - HJ - CSDN博客
2016年08月08日 22:28:57[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：283
Can you solve this equation?
Problem Description 
Now,given the equation 8*x^4 + 7*x^3 + 2*x^2 + 3*x + 6 == Y,can you find its solution between 0 and 100; 
Now please try your lucky.
Input 
The first line of the input contains an integer T(1<=T<=100) which means the number of test cases. Then T lines follow, each line has a real number Y (fabs(Y) <= 1e10);
Output 
For each test case, you should just output one real number(accurate up to 4 decimal places),which is the solution of the equation,or “No solution!”,if there is no solution for the equation between 0 and 100.
Sample Input 
2 
100 
-4
Sample Output 
1.6152 
No solution!
解析：二分入门题，可以作为模板。
代码如下：
```cpp
#include<iostream>
#include<cstdio>
#include<cmath>
using namespace std;
double f(double a)
{
    return 8*pow(a,4.0)+7*pow(a,3.0)+2*pow(a,2.0)+3*a+6;   //计算公式
}
int main()
{
    double b;
    int t;
    cin>>t;
    while(t--)
    {
        cin>>b;
        double i,j,m,n;
        if(f(0)<=b&&f(100)>=b)     //二分条件
        {
            m=0;
            n=100;
            while(n-m>1e-6)          //精度取值
            {
                i=(m+n)/2;            //二分取段
                double ans=f(i);     //段位取值
                if(ans>b) 
                  n=i-1e-7;     //如果大于，则把左边的赋值二分位
                else
                  m=i+1e-7;    //否则把右边的赋值二分位
            }
            printf("%.4lf\n",(m+n)/2);
        }
        else
          cout<<"No solution!"<<endl;    //否则输出无解
    }
    return 0;
}
```
