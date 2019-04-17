# Turn the corner（HDU-2438） - Alex_McAvoy的博客 - CSDN博客





2018年03月14日 15:16:32[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：99
个人分类：[HDU																[查找——三分查找](https://blog.csdn.net/u011815404/article/category/8116901)](https://blog.csdn.net/u011815404/article/category/7923257)








# Problem Description

    Mr. West bought a new car! So he is travelling around the city.

    One day he comes to a vertical corner. The street he is currently in has a width x, the street he wants to turn to has a width y. The car has a length l and a width d.

    Can Mr. West go across the corner?

![](https://img-blog.csdn.net/20180314153410360)



# Input

    Every line has four real numbers, x, y, l and w.

    Proceed to the end of file.

# Output

     If he can go across the corner, print "yes". Print "no" otherwise.

# Sample Input

**    10 6 13.5 4**

**    10 6 14.5 4**

# Sample Output

**    yes    no**

————————————————————————————————————————————

思路：

汽车已转弯，设水平方向为x轴，垂直方向为y轴。

则汽车的内边界的直线方程式f(x)：y=x*tan(θ)+l*sin(θ)+d/cos(θ).其中θ是汽车与x轴的夹角。

尽可能让车贴着外面的墙璧转弯，也就是图中的x轴和y轴，此时红线的方程就是图中的方程，此时p点的位置就是让y=X时解得的x值，要保证p点在Y内，也就是-x<y,假若在转弯的所有角度中都满足这个条件，那么就能转弯，分析得，-x先增大后减小，所以用三分求最大-x值。

图示： 

![](https://img-blog.csdn.net/20180314153815119)

（图源网络 侵权致歉）



# Source Program

```cpp
#include<cstdio>
#include<cmath>
#define PI acos(-1.0)
double x,y,l,w;

inline double calculate(double angle)//计算当前角度下允许通过的长度
{
	return -x*tan(angle)+l*sin(angle)+w/cos(angle);
}
int main()
{
    double right,left,mid,midmid;
   
    while( scanf("%lf%lf%lf%lf",&x,&y,&l,&w)!=EOF )//输入街道长、宽与汽车长、宽
    {
         /*三分查找最大角度*/
        left=0;right=PI/2;//设置上、下界初值，在0~90°寻找
        while(right-left>1e-10)
        {
            mid=(left+right)/2;//计算中值
            midmid=(mid+right)/2;//计算中值
            if(calculate(mid)<calculate(midmid))
            	left=mid;
            else
                right=midmid;
        }  
        if(calculate(mid)<=y)
			printf("yes\n");
        else
			printf("no\n");
    }
    return 0;
}
```




