# Line Belt（HDU-3400） - Alex_McAvoy的博客 - CSDN博客





2018年03月14日 18:58:07[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：73
个人分类：[HDU																[查找——三分查找](https://blog.csdn.net/u011815404/article/category/8116901)](https://blog.csdn.net/u011815404/article/category/7923257)








# Problem Description

    In a two-dimensional plane there are two line belts, there are two segments AB and CD, lxhgww's speed on AB is P and on CD is Q, he can move with the speed R on other area on the plane.

    How long must he take to travel from A to D?

# Input

    The first line is the case number T.

    For each case, there are three lines.

    The first line, four integers, the coordinates of A and B: Ax Ay Bx By.

    The second line , four integers, the coordinates of C and D:Cx Cy Dx Dy.

    The third line, three integers, P Q R.

    Technical Specification：

        1) 0<= Ax，Ay，Bx，By，Cx，Cy，Dx，Dy<=1000

        2) 1<=P，Q，R<=10

# Output

     The minimum time to travel from A to D, round to two decimals.

# Sample Input

**    1**

**    0 0 0 100    100 0 100 100    2 2 1**

# Sample Output

**    136.60**

————————————————————————————————————————————

思路：

设AB所在直线为x，CD所在直线为y，E、F分别是AB、CD上两动点，EF所在直线为z

则：

    人在线段AB上花的时间为：time1=AE/p

        人走完Z和Y所花的时间为：time2=EF/r+FD/q

        总时间：time=time1+time2

        因此，先三分AB上的点，找到E，再三分CD上的点找到F，利用上述公式计算出最短时间即可

# Source Program

```cpp
#include<cstdio>  
#include<cmath>
double Ax,Ay,Bx,By,Cx,Cy,Dx,Dy;
double Ex,Ey,Fx,Fy;
double p,q,r;
inline double d(double a,double b,double c,double d);//计算两点距离的函数
double calculate_E(double k);//求time的函数
double calculate_F(double k);//求time2的函数

int main()
{
    int t;
    double left,right,mid,midmid;
    
    scanf("%d",&t);//输入测试数据个数
    while(t--)
    {  
        scanf("%lf %lf %lf %lf",&Ax,&Ay,&Bx,&By);//输入A、B的横纵坐标
        scanf("%lf %lf %lf %lf",&Cx,&Cy,&Dx,&Dy);//输入C、D的横纵坐标
        scanf("%lf %lf %lf",&p,&q,&r);//输入在AB上的速度p，在CD上的速度q，在平面其它地方的速度r
        
        /*三分找点E*/
        left=0,right=1;//设置初值
        while(right-left>1e-10)
        {
            mid=(right+left)/2;//设置中值
            midmid=(mid+right)/2;//设置中值
            if(calculate_E(mid)<calculate_E(midmid))
                right=midmid;
            else
                left=mid;
        }  
        printf("%.2f\n",calculate_E(mid));//输出最短时间
    }
    return 0;  
}
double d(double a,double b,double c,double d)//计算两点距离的内联函数
{
    return sqrt( (c-a)*(c-a) + (d-b)*(d-b) );
}
double calculate_E(double k)//求time的函数
{
    double left,right,mid,midmid;
    double time1,time2,time;
    
    Ex=Ax+(Bx-Ax)*k;//求E的横坐标
    Ey=Ay+(By-Ay)*k;//求E的纵坐标
    
    /*三分找点F*/
    left=0;right=1;//设置初值
    while(right-left>1e-10)
    {
        mid=(right+left)/2;//设置中值
        midmid=(mid+right)/2;//设置中值
        if(calculate_F(mid)<calculate_F(midmid))
                right=midmid;
        else
                left=mid;
    }
    time1=d(Ax,Ay,Ex,Ey)/p;
    time2=calculate_F(mid);
    time=time1+time2;
    return time;
}  

double calculate_F(double k)//求time2的函数
{
    double time2;
    
    Fx=Cx+(Dx-Cx)*k;//求F的横坐标
    Fy=Cy+(Dy-Cy)*k;//求F的纵坐标
    
    time2=d(Dx,Dy,Fx,Fy)/q+d(Ex,Ey,Fx,Fy)/r;
    return time2;
}
```




