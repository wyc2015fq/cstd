# C++ 对数组进行排序并返还对应索引号 - sinat_31425585的博客 - CSDN博客
2017年07月06日 21:41:51[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：3563
刚写代码，需要对一个数组进行排序，并返还对应索引号，参考了[integrity0123](http://blog.csdn.net/integrity0123/article/details/6760931)：[点击打开链接](http://blog.csdn.net/integrity0123/article/details/6760931)和[陈鹏万里](http://blog.csdn.net/qq245671051/article/details/50756770)：[点击打开链接](http://blog.csdn.net/qq245671051/article/details/50756770)的代码，主要思想是另外生成一个与需要进行排序数组相同大小的保存索引号数组，排序的过程中，对索引号数组进行相同的操作。现在做一下笔记mark一下，具体demo如下：
```cpp
#include <vector>
#include <iostream>
using namespace std;
void BubbleSort(float  *p, int length, int * ind_diff)
{
	for (int m = 0; m < length; m++)
	{
		ind_diff[m] = m;
	}
	for (int i = 0; i < length; i++)
	{
		for (int j = 0; j < length- i - 1; j++)
		{
			if (p[j] > p[j + 1])
			{
				float temp = p[j];
				p[j] = p[j + 1];
				p[j + 1] = temp;
				int ind_temp = ind_diff[j];
				ind_diff[j] = ind_diff[j + 1];
				ind_diff[j + 1] = ind_temp;
			}
		}
	}
}
void main()
{
	float a[8] = { 1.0, 1.8, 1.5, 1.1, 4.3, 5.5, 2.2, 8.8 };
	int ind[8] = { 0 };
	BubbleSort(a, 8, ind);
	for (int i = 0; i < 8; i++)
	{
		cout << "value: " << a[i] << "Index: " << ind[i] << endl;
	}
}
```
打完收工！
