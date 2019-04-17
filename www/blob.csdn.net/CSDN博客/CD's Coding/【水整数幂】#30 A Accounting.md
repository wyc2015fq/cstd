# 【水整数幂】#30 A. Accounting - CD's Coding - CSDN博客





2014年07月06日 17:07:43[糖果天王](https://me.csdn.net/okcd00)阅读数：797
所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)


















A long time ago in some far country lived king Copa. After the recent king's reform, he got so large powers that started to keep the books by himself.


The total income *A* of his kingdom during 0-th year is known,
 as well as the total income *B* during *n*-th year (these
 numbers can be negative — it means that there was a loss in the correspondent year).


King wants to show financial stability. To do this, he needs to find common coefficient *X* — the coefficient of income growth during one year. This coefficient
 should satisfy the equation:
*A*·*X**n* = *B*.

Surely, the king is not going to do this job by himself, and demands you to find such number *X*.


It is necessary to point out that the fractional numbers are not used in kingdom's economy. That's why all input numbers as well as coefficient *X* must be
 integers. The number *X* may be zero or negative.




Input


The input contains three integers *A*, *B*, *n* (|*A*|, |*B*| ≤ 1000, 1 ≤ *n* ≤ 10).




Output


Output the required integer coefficient *X*, or «No solution», if such a coefficient does not exist or it is fractional. If there are several possible solutions,
 output any of them.




Sample test(s)




input
2 18 2




output
3



input
-1 8 3




output
-2



input
0 0 10




output
5



input
1 16 5




output
No solution









因为数值范围太小甚至都觉得用快速幂是不是大材小用了……



注意幂运算的速度，以及正负符号的判断，特判成立时记得return免得多重输出，基本上算法难度没有，作为一道骗WA的水题还是不错的= =。



```cpp
#include <cmath>   
#include <vector>  
#include <cctype>  
#include <cstdio>  
#include <string>  
#include <cstdlib>  
#include <cstring>  
#include <iostream>  
#include <algorithm>  
using namespace std;

int Pow(int a,int k){  
	int sum=1;  
    while(k){  
        if(k&1)sum=sum*a;  
        a=a*a;  
        k>>=1;  //k=k>>1;
    }  
    return sum;  
}  

int main()
{
	int a,b,n;
	scanf("%d%d%d",&a,&b,&n);
	if(a==0&&b!=0)
	{
		puts("No solution");
		return 0;
	}
	else if(b==0)
	{
		printf(a==0?"1":"0");
		return 0;
	}
	else 
	{
		int Mi=b/a;
		if(abs(Mi)==1)
		{
			if(Mi==1)printf("1");
			else printf(n%2==0?"No solution":"-1");
			return 0;
		}
		else for(int i=1;Pow(i,n)<=abs(Mi);i++)
		{
			if(Pow(i,n)==Mi)
			{
				printf("%d",i);
				return 0;
			}
			else if(Pow(0-i,n)==Mi)
			{
				printf("-%d",i);
				return 0;
			}
		}
		printf("No solution");
	}
	return 0;
}
```










