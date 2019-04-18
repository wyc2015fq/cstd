# 【C语言】杨辉三角问题 - csdn_baotai的博客 - CSDN博客

2018年05月18日 17:51:18[csdn-[baotai]](https://me.csdn.net/csdn_baotai)阅读数：191
个人分类：[C语言](https://blog.csdn.net/csdn_baotai/article/category/7659809)



# 【C语言】杨辉三角问题

## 问题描述：
- 
#### 每个数等于它上方两数之和。每行数字左右对称，由1开始逐渐变大。第n行的数字有n项。第n行数字和为2n-1。第n行的m个数可表示为 *C(n-1，m-1)*，即为从n-1个不同元素中取m-1个元素的组合数。第n行的第m个数和第n-m+1个数相等 ，为[组合数](https://baike.baidu.com/item/%E7%BB%84%E5%90%88%E6%95%B0)性质之一。每个数字等于上一行的左右两个数字之和。

![](https://img-blog.csdn.net/20180518174444133)


```cpp
3.打印杨辉三角
#include<stdio.h>
#include<windows.h>

int main()
{
	int a[10][10] = {0};//对于定义的数组一定要先初始化
	int i;
	int j=0;
	int k = 0;
	for (i = 0; i < 10; i++)//按照杨辉三角的排列规律进行赋值
	{
		a[i][j] = 1;
		a[i][i] = 1;
	}
	for (i = 2; i < 10; i++)//前两行都是1
	{
		for (j = 1; j < 10; j++)
		{
			a[i][j] = a[i - 1][j - 1] + a[i - 1][j];
		}
	}
	
	for (i = 0; i < 10; i++)//打印杨辉三角
	{
		for (k = 0; k<10-i; k++)//打印空格，使之美观
		{
			printf("  ");
		}
		for (j = 0; j <=i; j++)//打印数据
		{
			printf("%4d", a[i][j]);
		}
		printf("\n");
	}
	system("pause");
	return 0;
}
```

运行结果：

![](https://img-blog.csdn.net/20180518174902679?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NzZG5fYmFvdGFp/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


