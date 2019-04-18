# 排序-翁恺《C语言程序进阶》 - 心纯净，行致远 - CSDN博客





2018年06月21日 11:18:05[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：135









**题目内容：**

程序读入一个正整数n（0<n<=100000），然后读入n个整数，均为32位下的整数。输出对这个整数排序后的结果，每个整数后面有一个空格。




**输入格式:**


一个表示个数的正整数n，和n个整数，以空格间隔。




**输出格式：**


排序后的n的整数，每个整数后面有一个空格。




**输入样例：**


6 2 23 54 12 6 8




**输出样例：**


2 6 8 12 23 54


时间限制：500ms内存限制：32000kb

```cpp
#include <stdio.h> 
#include <stdlib.h>
main()
{
	int i;
	scanf("%d",&i);
	int *array;    //一维动态数组申请
	array=(int*)malloc(sizeof(int)*i);
	int j,k,temp;
	for(j=0;j<i;j++)
	{
	 scanf("%d",&array[j]);
	}
	for(j=0;j<i;j++)    //冒泡排序
	{
		for(k=j+1;k<i;k++)
		{
			if(array[j]>array[k])
			{
				temp=array[k];
				array[k]=array[j];
				array[j]=temp;
					
			}
		}
	}
	for(j=0;j<i;j++)
	{
	 printf("%d ",array[j]);
	}
    free(array);
return 0;
 }
```




