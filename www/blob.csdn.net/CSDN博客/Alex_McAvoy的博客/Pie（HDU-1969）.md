# Pie（HDU-1969） - Alex_McAvoy的博客 - CSDN博客





2018年03月13日 22:10:20[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：98
个人分类：[HDU																[查找——二分查找](https://blog.csdn.net/u011815404/article/category/8116900)](https://blog.csdn.net/u011815404/article/category/7923257)








# Problem Description

    My birthday is coming up and traditionally I'm serving pie. Not just one pie, no, I have a number N of them, of various tastes and of various sizes. F of my friends are coming to my party and each of them gets a piece of pie. This should be one piece of one pie, not several small pieces since that looks messy. This piece can be one whole pie though.

    My friends are very annoying and if one of them gets a bigger piece than the others, they start complaining. Therefore all of them should get equally sized (but not necessarily equally shaped) pieces, even if this leads to some pie getting spoiled (which is better than spoiling the party). Of course, I want a piece of pie for myself too, and that piece should also be of the same size. 

    What is the largest possible piece size all of us can get? All the pies are cylindrical in shape and they all have the same height 1, but the radii of the pies can be different.

# Input

    One line with a positive integer: the number of test cases. Then for each test case:

    ---One line with two integers N and F with 1 <= N, F <= 10 000: the number of pies and the number of friends.

    ---One line with N integers ri with 1 <= ri <= 10 000: the radii of the pies.

# Output

    For each test case, output one line with the largest possible volume V such that me and my friends can all get a pie piece of size V. The answer should be given as a floating point number with an absolute error of at most 10^(-3).

# Sample Input

**    3**

**    3 3    4 3 3    1 2****4**

**    5**

**    10 5    1 4 2 3 4 5 6 5 4 2**

# Sample Output

**    25.1327**

**    3.1416    50.2655**

————————————————————————————————————————————

思路：

    m+1个人分n个蛋糕，每人馅饼必须是整块的，不能拼接，求最大分到的面积。

    由于不能拼接，所以在0与最大的面积间二分，同时记录所有馅饼的总面积，划分面积不能超过总面积。

# Source Program

```cpp
#include<cstdio>
#include<cmath>
double a[10001];
#define PI acos(-1.0);//为追求高精度，使用反余弦函数acos，因为cosπ=-1，故：acos(-1)=π
int main()
{  
    int t;
    int n,f;
    int i;
    double max_pie=0,sum=0;
  	double left,right,mid;
    int num;
    
    scanf("%d",&t);//输入测试数据个数t  
    while(t--)
    {
        scanf("%d%d",&n,&f);//输入馅饼数n与朋友数f
        f=f+1;//分馅饼的人数=朋友数f+“我”
		for(i=1;i<=n;i++)
        {
            scanf("%lf",&a[i]);//输入每个馅饼半径
            a[i]=a[i]*a[i]*PI;//计算每个馅饼的面积
            if(max_pie<a[i])//寻找面积最大的馅饼
				max_pie=a[i];
        }
        
         /*二分查找*/
        left=0;//设置查找区间上界初值
        right=max_pie;//设置查找区间下界初值
		while(right-left>1e-6)  
        {
            mid=(right+left)/2;//设置中值
            num=0;//设置参与分割人数初值
            for(i=1;i<=n;i++)//记录按mid分割每个馅饼时的人数
                num+=int(a[i]/mid);
			if(num<f)//参与分割人数小于总人数
				right=mid;//更改右边界
			else//否则
				left=mid;//更改左边界
        }
        printf("%.4lf\n",left);
    }  
    return 0;
}
```






