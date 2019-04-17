# Can you solve this equation?（HDU-2199） - Alex_McAvoy的博客 - CSDN博客





2018年03月12日 21:41:34[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：120
个人分类：[HDU																[查找——二分查找](https://blog.csdn.net/u011815404/article/category/8116900)](https://blog.csdn.net/u011815404/article/category/7923257)








# Problem Description

    Now,given the equation 8*x^4 + 7*x^3 + 2*x^2 + 3*x + 6 == Y,can you find its solution between 0 and 100;

    Now please try your lucky.

# Input

   The first line of the input contains an integer T(1<=T<=100) which means the number of test cases. Then T lines follow, each line has a real number Y (fabs(Y) <= 1e10);

# Output

    For each test case, you should just output one real number(accurate up to 4 decimal places),which is the solution of the equation,or “No solution!”,if there is no solution for the equation between 0 and 100.

# Sample Input

**2**

**100**

**-4**

# Sample Output

**1.6152**

**No solution!**

# Source Program

```cpp
#include<cstdio>//追求效率，使用scanf、printf输入输出
#include<cmath>
#define E 1e-10//涉及浮点数的精度判断，自定义一个精度进行判断
int main()
{
	int t;
	int max_res=8*100*100*100*100+7*100*100*100+2*100*100+3*100+6;//函数式的最大值
	double y;
	double min,max,x;

	scanf("%d",&t);//输入测试数据个数t
	while(t--)
	{
		scanf("%lf",&y);//输入每组数据

		if(y<6||y>max_res)//当y<6或y大于max_res时可知，无解
			printf("No solution!\n");
		else//二分查找
		{
			min=0;max=100;//设置当前查找区间上、下界的初值
			while(max-min>E)
			{
				x=(max+min)/2;//设置中值
				if(8*x*x*x*x+7*x*x*x+2*x*x+3*x+6>y)	max=x;
				else	min=x;
			}
			printf("%.4lf\n",x);
		}
	}
	return 0;
}
```






