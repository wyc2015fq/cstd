# Toxophily（HDU-2298） - Alex_McAvoy的博客 - CSDN博客





2018年03月13日 23:41:41[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：101
个人分类：[HDU																[查找——三分查找](https://blog.csdn.net/u011815404/article/category/8116901)](https://blog.csdn.net/u011815404/article/category/7923257)








# Problem Description

    The recreation center of WHU ACM Team has indoor billiards, Ping Pang, chess and bridge, toxophily, deluxe ballrooms KTV rooms, fishing, climbing, and so on.

    We all like toxophily.

    Bob is hooked on toxophily recently. Assume that Bob is at point (0,0) and he wants to shoot the fruits on a nearby tree. He can adjust the angle to fix the trajectory. Unfortunately, he always fails at that. Can you help him?

    Now given the object's coordinates, please calculate the angle between the arrow and x-axis at Bob's point. Assume that g=9.8N/m. 

# Input

    The input consists of several test cases. The first line of input consists of an integer T, indicating the number of test cases. Each test case is on a separated line, and it consists three floating point numbers: x, y, v. x and y indicate the coordinate of the fruit. v is the arrow's exit speed.

    Technical Specification

    1. T ≤ 100.

    2. 0 ≤ x, y, v ≤ 10000. 

# Output

    For each test case, output the smallest answer rounded to six fractional digits on a separated line.

    Output "-1", if there's no possible answer.

    Please use radian as unit. 

# Sample Input

**    3**

**    0.222018 23.901887 121.909183    39.096669 110.210922 20.270030    138.355025 2028.716904 25.079551**

# Sample Output

**    1.561582**

**    -1    -1**

————————————————————————————————————————————

思路：

由物理公式分析可知：

                        Vx=v*cos(angle)

                        Vy=v*sin(angle)

                        t=x/Vx=x/( v*cos(angle) )

                        y=-(1/2)*g*t*t+Vy*t=-(1/2)*g*t*t+v*sin(angle)*t

根据以上四个公式，对角度0~90°进行三分，寻找箭所能达到的高度最大值。

若最大值小于题目所给高度，则无法达到；否则，对角度0~三分结果进行二分，查找能够到达的最小角度。

# Source Program

```cpp
#include<cstdio>
#include<cmath>
#define PI acos(-1.0)
double x,y,v;
inline double calculate(double angle)//已知角度计算所能达到高度的函数
{
    double time;
    time=x/( v*cos(angle) );
    return -0.5*9.8*time*time+v*sin(angle)*time;
}
int main()
{
    int t;
    double left,right,mid,midmid;
    
    scanf("%d",&t);//输入测试数据的个数t
    while(t--)
    {
        scanf("%lf%lf%lf",&x,&y,&v);//输入果实横坐标x，纵坐标y以及箭离弦时的速度
        /*三分查找寻找箭达到果实高度y时的最大角度*/
        left=0;right=PI/2;//设置上、下界初值，在0~90°寻找
        while(right-left>1e-10)
        {
            mid=(right+left)/2;//计算中值
            midmid=(mid+right)/2;//计算中值
            if(calculate(mid)<calculate(midmid))
                left=mid;
            else
                right=midmid;
        }
        
        if(calculate(mid)<y)//当所能达到的最大高度无法到达所给高度时，说明箭无法射中果实
            printf("-1\n");//输出-1
        else//否则
        {
            /*二分查找满足题设的最小角度*/
            left=0;right=mid;//设置上、下界初值，在0~mid寻找
            while(right-left>1e-10)
            {
                mid=(left+right)/2;//设置中值
                if(calculate(mid)<y)
                    left=mid;
                else
                    right=mid;
            }
            printf("%.6lf\n",left);
        }
    }
    return 0;
}
```










