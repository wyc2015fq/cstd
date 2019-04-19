# hdu 4709  Herding（叉乘法求面积） - HJ - CSDN博客
2016年08月19日 17:46:54[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：393
Problem Description 
Little John is herding his father’s cattles. As a lazy boy, he cannot tolerate chasing the cattles all the time to avoid unnecessary omission. Luckily, he notice that there were N trees in the meadow numbered from 1 to N, and calculated their cartesian coordinates (Xi, Yi). To herding his cattles safely, the easiest way is to connect some of the trees (with different numbers, of course) with fences, and the close region they formed would be herding area. Little John wants the area of this region to be as small as possible, and it could not be zero, of course.
Input 
The first line contains the number of test cases T( T<=25 ). Following lines are the scenarios of each test case. 
The first line of each test case contains one integer N( 1<=N<=100 ). The following N lines describe the coordinates of the trees. Each of these lines will contain two float numbers Xi and Yi( -1000<=Xi, Yi<=1000 ) representing the coordinates of the corresponding tree. The coordinates of the trees will not coincide with each other.
Output 
For each test case, please output one number rounded to 2 digits after the decimal point representing the area of the smallest region. Or output “Impossible”(without quotations), if it do not exists such a region.
Sample Input
1 
4 
-1.00 0.00 
0.00 -3.00 
2.00 0.00 
2.00 2.00
Sample Output
2.00
解析：给出n个点，要你求出在n个点中任选三个点围成的最小面积是多少。
需要考虑两点：
（1）点的个数少于三个
（2）存在三点或者更多的点共线的情况
解法推荐用叉乘，求出所有不同三角形的面积，遍历一遍求出最小值。
叉乘法：假设选取的三个点A（a[0]，b[0]）、B（a[1]，b[1]）、C（a[2]，b[2]）
则三角形面积公式为：
```
fabs((a[1]-a[0])*(b[1]-b[0])-(a[2]-a[0])*(b[2]-b[0]))/2.0;
```
因为是三角形三个点，因此循环开三层，代码如下：
```cpp
#include<iostream>
#include<cstdio>
#include<algorithm>
#include<cstring>
#include<string>
#include<cmath>
using namespace std;
double a[105],b[105];
int main()
{
    int t;
    cin>>t;
    while(t--)
    {
        int n;
        cin>>n;
        for(int i=1;i<=n;i++)
          cin>>a[i]>>b[i];
        if(n<3)       //少于三条边，无法构成至少一个三角形
          printf("Impossible\n");
        else
        {
            int blag=0;
            double min=1000000.0;
            for(int i=1;i<=n;i++)
              for(int j=i+1;j<=n;j++)
                for(int k=j+1;k<=n;k++)
                {
                    double tt=0.0,s=0.0;
                    tt=((a[j]-a[i])*(b[k]-b[i]))-((b[j]-b[i])*(a[k]-a[i]));
                    s=fabs(tt)/2.0;
                    if(min>s&&s!=0)      //要考虑面积为0，也就是三点共线的特殊情况
                    {
                        min=s;
                        blag=1;          //一旦有面积大于零的三角形，便可以确定不输出无解
                    }
                }
            if(blag)
                printf("%.2lf\n",min);
            else                    //所有点都在一条直线上，不存在三角形
              printf("Impossible\n");
        }
    }
    return 0;
}
```
