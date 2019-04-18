# C语言编程练习题库 - 心纯净，行致远 - CSDN博客





2018年04月07日 15:08:18[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：4014








# 1. 题目：写一个程序，判断两个浮点数是否足够精确。

```
/*  File name:ApproximatelyEqual
	Function: ensure the accurcy of two numbers.|x-y|/min(|x|,|y|)<e 
	Time: 2018.04.07
	edited by QJX	
*/

#include <stdio.h>
#include <math.h>
double Minfabsnum(double x,double y);	//Calculate the min of fabs(x),fabs(y)
void GiveInstruction(void);	//Give instruction and reference to users
#define e 0.0001	//The accurcy 

double main()
{
	 double x,y,c;
	 GiveInstruction();
	 printf("Please input 2 numers:");
	 scanf("%lf,%lf",&x,&y);
	 c=Minfabsnum(x,y);
	 if(fabs(x-y)/c<e)
	 	printf("Two numers satisfy the accurcy!");
	else 
		printf("Two numers not satisfy the accurcy!");
} 

double Minfabsnum(double x,double y)	//Calculate the min of fabs(x),fabs(y)
{
	double a,b;
	a=fabs(x);
	b=fabs(y);
	if(a<=b)
	return a;
	else
	return b;
}

void GiveInstruction(void)	//Give instruction and reference to users
{
	printf("This program can ensure the accurcy of two double numbers!\n");
	printf("And the accurcy is 0.0001\n\n");
}
```

![](https://img-blog.csdn.net/20180407161813731)


# 2. 题目：打印杨辉三角前八行

```cpp
/*  File name:Pascal triangle or YangHui triangle
	Founction: display the 8 raws of YangHui triangle
	Time:2018.04.07
	edited by qjx
*/

#include <stdio.h>
#define N 8
int Factorial(int n);
int Combinations(int n,int k);
void GiveInstruction(void);

main()
{
	int raw,i,j,k;
	GiveInstruction();
	printf("Please input the raws of YangHui triangle:");
	scanf("%d",&raw);
	for(i=1;i<=raw;i++)
	{
		for(j=0;j<raw-i;j++)	//print the spacebar
		{
			printf(" ");
		}
		for(k=0;k<=i-1;k++)	//print the YangHui triangle
		{
			printf("%4d ",Combinations(i-1,k));	
		//	printf(" ");
		}
		printf("\n");
	}
}

int Factorial(int n)   //calculate n！
{
	int i,product=1;
	for(i=1;i<=n;i++)
	{
		product*=i;
	}
	return product; 
}

int Combinations(int n,int k)    //calculate C(n,k)=n!/(k!*(n-k)!)
{
	int a,b,c;
	a=Factorial(n);
	b=Factorial(k);
	c=Factorial(n-k);
	return a/(b*c);
}

void GiveInstruction(void)
{
	printf("This program can display the YangHui triangle!\n");
	printf("For example:\n");
	printf("  1  \n");
	printf(" 1 1 \n");
	printf("1 2 1\n");
}
```

![](https://img-blog.csdn.net/2018040716205169)

当用更大的数据去测试时，发现最多只能到13，到需要输出14行杨辉三角时就会出现错误。


![](https://img-blog.csdn.net/20180407162235129)

本来以为是超出了int型的范围，但是手动计算之后并没有超出啊，而且如果超出的话，在计算12！时，就已经超出范围了。不解！

解决：还是int范围的问题

```cpp
#include <stdio.h> 
#include <limits.h>
#define N 13
int Factorial(int n);

main()
{
	int a;
	a=Factorial(N);
	printf("%d",a);
	printf("The value of INT_MAX is %i\n", INT_MAX);	
	printf("The value of INT_MIN is %i\n", INT_MIN);	
	printf("An int takes %d bytes\n", sizeof(int));

}

int Factorial(int n)   //calculate n！  
{  
    int i,product=1;  
    for(i=1;i<=n;i++)  
    {  
        product*=i;  
    }  
    return product;   
}
```

采用如上代码，改变N的值，可以清楚地看到溢出的时刻。


|N|N!|
|----|----|
|10|3628800|
|12|479001600|
|13|1932053504|
|int|-2147483647~2147483647|







