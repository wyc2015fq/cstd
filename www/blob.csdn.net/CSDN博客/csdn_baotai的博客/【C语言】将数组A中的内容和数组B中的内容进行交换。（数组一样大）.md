# 【C语言】将数组A中的内容和数组B中的内容进行交换。（数组一样大） - csdn_baotai的博客 - CSDN博客

2018年04月21日 15:51:09[csdn-[baotai]](https://me.csdn.net/csdn_baotai)阅读数：24
个人分类：[C语言](https://blog.csdn.net/csdn_baotai/article/category/7659809)



问题描述：将数组A中的内容和数组B中的内容进行交换。（数组一样大）

```cpp
#include<stdio.h>
#include<windows.h>
int main()
{ 
int arr1[] = { 1, 3, 5, 7, 9 };
int arr2[] = { 3, 4, 5, 6, 7 };
int tmp;
int i = 0;
for (i = 0; i < sizeof(arr1) / sizeof(arr1[0]); i++)//数组长度
{
		tmp = arr1[i];
		arr1[i] = arr2[i];
		arr2[i] = tmp;
}
printf("array1:");
//打印arr[1]
for (i = 0; i < sizeof(arr1) / sizeof(arr1[0]); i++)
{
	printf("%d ", arr1[i]);
}
printf("\n");
printf("array2:");
//打印arr[2]	
for (i = 0; i < sizeof(arr1) / sizeof(arr1[0]); i++)
{
	printf("%d ", arr2[i]);
}
printf("\n");

system("pause");
return 0;
}
```

