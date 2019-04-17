# The Balance（POJ-2142） - Alex_McAvoy的博客 - CSDN博客





2018年08月10日 15:48:52[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：54
个人分类：[POJ																[数论——方程](https://blog.csdn.net/u011815404/article/category/8796916)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

Ms. Iyo Kiffa-Australis has a balance and only two kinds of weights to measure a dose of medicine. For example, to measure 200mg of aspirin using 300mg weights and 700mg weights, she can put one 700mg weight on the side of the medicine and three 300mg weights on the opposite side (Figure 1). Although she could put four 300mg weights on the medicine side and two 700mg weights on the other (Figure 2), she would not choose this solution because it is less convenient to use more weights. 

You are asked to help her by calculating how many weights are required. 

![](https://img-blog.csdn.net/20180810152317199?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# **Input**

The input is a sequence of datasets. A dataset is a line containing three positive integers a, b, and d separated by a space. The following relations hold: a != b, a <= 10000, b <= 10000, and d <= 50000. You may assume that it is possible to measure d mg using a combination of a mg and b mg weights. In other words, you need not consider "no solution" cases. 

The end of the input is indicated by a line containing three zeros separated by a space. It is not a dataset.

# Output

The output should be composed of lines, each corresponding to an input dataset (a, b, d). An output line should contain two nonnegative integers x and y separated by a space. They should satisfy the following three conditions. 

You can measure dmg using x many amg weights and y many bmg weights. 

The total number of weights (x + y) is the smallest among those pairs of nonnegative integers satisfying the previous condition. 

The total mass of weights (ax + by) is the smallest among those pairs of nonnegative integers satisfying the previous two conditions.

No extra characters (e.g. extra spaces) should appear in the output.

# Sample Input

**700 300 200500 200 300500 200 500275 110 330275 110 385648 375 40023 1 100000 0 0**

# Sample Output

**1 31 11 00 31 149 743333 1**


题意：给出质量为 m、n 的砝码，要称重量为 k 的东西且要求两种砝码数量越小越好，如果有相同的解输出 x+y 小的那组

思路：容易列出方程 mx+ny=k，用扩展欧几里得求出 x、y 后分成两组，根据 x 推 y 与根据 y 推 x ，最后取 x+y 小的那组即可

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<string>
#include<cstdlib>
#include<queue>
#include<set>
#include<map>
#include<stack>
#include<ctime>
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 1000005
#define MOD 1e9+7
#define E 1e-6
#define LL long long
using namespace std;
LL Extended_GCD(LL a,LL b,LL &x,LL &y)
{
    if(b==0)
    {
        x=1;
        y=0;
        return a;
    }
    LL gcd=Extended_GCD(b,a%b,x,y);
    LL temp=x;
    x=y;
    y=temp-a/b*y;
    return gcd;
}
int main()
{
    LL m,n,k;
    while(scanf("%lld%lld%lld",&m,&n,&k)!=EOF&&(m+n+k))
    {
        LL x,y;
        LL gcd=Extended_GCD(m,n,x,y);

        /*求x推y*/
        LL x1,y1;
        x1=x*k/gcd;
        x1=(x1%(n/gcd)+(n/gcd))%(n/gcd);
        y1=(k/gcd-(m/gcd)*x1)/(n/gcd);
		if(y1<0)
            y1=-y1;
            
        /*求y推x*/
		LL x2,y2;
		y2=y*k/gcd;
		y2=(y2%(m/gcd)+(m/gcd))%(m/gcd);
		x2=(k/gcd-(n/gcd)*y2)/(m/gcd);
		if(x2<0)
            x2=-x2;

        /*比较输出x+y最小的一组*/
        if(x1+y1<x2+y2)
            printf("%lld %lld\n",x1,y1);
        else
            printf("%lld %lld\n",x2,y2);

    }
    return 0;
}
```






