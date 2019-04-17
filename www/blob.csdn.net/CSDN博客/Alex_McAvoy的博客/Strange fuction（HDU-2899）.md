# Strange fuction（HDU-2899） - Alex_McAvoy的博客 - CSDN博客





2018年03月13日 17:47:10[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：92
个人分类：[HDU																[查找——三分查找](https://blog.csdn.net/u011815404/article/category/8116901)](https://blog.csdn.net/u011815404/article/category/7923257)








# Problem Description

    Now, here is a fuction:F(x) = 6 * x^7+8*x^6+7*x^3+5*x^2-y*x (0 <= x <=100)

    Can you find the minimum value when x is between 0 and 100.

# Input

    The first line of the input contains an integer T(1<=T<=100) which means the number of test cases. Then T lines follow, each line has only one real numbers Y.(0 < Y <1e10)

# Output

    Just the minimum value (accurate up to 4 decimal places),when x is between 0 and 100.

# Sample Input

**    2**

**    100**

**    200**

# Sample Output

**    -74.4291**

**    -178.8534**

# Source Program

```cpp
#include<cstdio>//追求效率，使用scanf、printf输入输出  
#include<cmath>  
#define E 1e-10//涉及浮点数的精度判断，自定义一个精度进行判断  
int main()  
{  
    int t;
    double y;  
    double min,max,mid,mid_mid;
	double mid_res,mid_mid_res;
  
    scanf("%d",&t);//输入测试数据个数t  
    while(t--)
    {  
        scanf("%lf",&y);//输入每组数据
		
		/*三分查找*/
        min=0;max=100;//设置当前查找区间上、下界的初值  
        while(max-min>E)  
        {
            mid=(max+min)/2;//设置中值1
			mid_mid=(mid+max)/2;//设置中值2
			mid_res=6*pow(mid,7)+8*pow(mid,6)+7*pow(mid,3)+5*pow(mid,2)-y*mid;//计算x=mid时的函数值大小
			mid_mid_res=6*pow(mid_mid,7)+8*pow(mid_mid,6)+7*pow(mid_mid,3)+5*pow(mid_mid,2)-y*mid_mid;//计算x=mid_mid时的函数值大小
			if(mid_res<mid_mid_res)//比较x=mid与x=mid_mid时的函数值大小，不断更新区间上、下界，确定最优解
				max=mid_mid;
			else
				min=mid;
        }
        printf("%.4lf\n",mid_res);
    }  
    return 0;
}
```






