# hdu 2289  Cup（二分） - HJ - CSDN博客
2016年08月07日 20:13:58[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：673
Problem Description 
The WHU ACM Team has a big cup, with which every member drinks water. Now, we know the volume of the water in the cup, can you tell us it height? 
The radius of the cup’s top and bottom circle is known, the cup’s height is also known.
Input 
The input consists of several test cases. The first line of input contains an integer T, indicating the num of test cases. 
Each test case is on a single line, and it consists of four floating point numbers: r, R, H, V, representing the bottom radius, the top radius, the height and the volume of the hot water.
Technical Specification
- T ≤ 20.
- 1 ≤ r, R, H ≤ 100; 0 ≤ V ≤ 1000,000,000.
- r ≤ R.
- r, R, H, V are separated by ONE whitespace.
- There is NO empty line between two neighboring cases.
Output 
For each test case, output the height of hot water on a single line. Please round it to six fractional digits.
Sample Input 
1 
100 100 100 3141562
Sample Output 
99.999024
因为考虑到精度的问题，公式解决不了就应该马上想到用二分来解。
解析：二分高，但是需要注意的是，在更改底面圆半径大小的时候需要用相似三角形而不能单纯地二分，因为会有很大的误差。，详解看下面代码：
```cpp
#include<iostream>
#include<cstdio>
#include<cmath>
using namespace std;
#define PI acos(-1.0)                   //有的时候要赋值3,1415926,这个需要看数据而定
double cctt(double rr,double r,double h)
{
    return PI*h*(rr*rr+r*r+rr*r)/3.0;         //圆台体积公式
} 
int main()
{
    int t;
    cin>>t;
    while(t--)
    {
        double R,r,H,v,v2,rr;
        cin>>r>>R>>H>>v2;
        double s=0,l=100,m;
        while(l-s>1e-9)               //精度可以任意开，一般只要大于七位就可以了
        {
            m=(s+l)/2.0;
            double rr=m*(R-r)/H+r;            //用相似三角形求出二分出来的半径
            if(cctt(rr,r,m)>v2)
              l=m;
            else
              s=m;
        }
        printf("%.6lf\n",(s+l)/2.0);
    }
    return 0;
}
```
