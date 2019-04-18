# 【C语言】指针深入理解-指针与数组的关系 - csdn_baotai的博客 - CSDN博客

2018年05月20日 14:25:35[csdn-[baotai]](https://me.csdn.net/csdn_baotai)阅读数：31
个人分类：[C语言](https://blog.csdn.net/csdn_baotai/article/category/7659809)



问题描述：指针与数组相关运算

                 深入理解size与strlen的用法

  结合以下例子，对于指针与数组运算进行详细解释

```cpp
#include<stdio.h>
#include<windows.h>
int main()
{
	//数组
	int a[] = { 1, 2, 3, 4 };
	printf("%d\n", sizeof(a));//16		a是数组名，取的是啊数组a的大小
	printf("%d\n", sizeof(a + 0));//4		a+0是数组首元素的地址
	printf("%d\n", sizeof(*a));//4		指向数组的指针，所有指针的大小为4
	printf("%d\n", sizeof(a + 1));//4		数组第二个元素的地址
	printf("%d\n", sizeof(a[1]));//4		数组的第一个元素
	printf("%d\n", sizeof(&a));//4		整个数组的地址
	printf("%d\n", sizeof(&a + 1));//4		下一个数组的地址
	printf("%d\n", sizeof(&a[0]));//4		首元素的地址，等同于a + 0
	printf("%d\n", sizeof(&a[0] + 1));//4		数组第二个元素的地址等同于a+1
	
	//字符数组
	char arr[] = { 'a', 'b', 'c', 'd', 'e', 'f' };
	printf("%d\n", sizeof(arr));//6		a是数组名，取数组arr的大小
	printf("%d\n", sizeof(arr + 0));//4		首元素的地址
	printf("%d\n", sizeof(*arr));//1		首元素
	printf("%d\n", sizeof(arr[1]));//1			首元素
	printf("%d\n", sizeof(&arr));//4		整个数组的地址
	printf("%d\n", sizeof(&arr + 1));//4		下一个数组的地址
	printf("%d\n", sizeof(&arr[0] + 1));//4		第二个元素的地址
	//strlen的参数必须为char*，否则类型不匹配
	printf("%d\n", strlen(arr));//19   arr代表的是首元素的地址，从首元素一直往后找，一直找到\0位置  答案随机器不同不唯一
	printf("%d\n", strlen(arr + 0));//19   原因同上
	printf("%d\n", strlen(*arr));//   类型不匹配
	printf("%d\n", strlen(arr[1]));//类型不匹配
	printf("%d\n", strlen(&arr));//19  &arr代表的是整个数组的地址，数值同数组首元素的地址
	printf("%d\n", strlen(&arr + 1));//13		下一数组的地址
	printf("%d\n", strlen(&arr[0] + 1));//18		数组第二个元素的地值
	
	//字符串
	char *p = "abcdef";
	printf("%d\n", sizeof(p)); //4		p是指针
	printf("%d\n", sizeof(p + 1));//4		p+1任然是指针，指向第二个字符
	printf("%d\n", sizeof(*p));//1		第一个字符
	printf("%d\n", sizeof(p[0]));//1		第一个字符
	printf("%d\n", sizeof(&p));//4指针的地址
	printf("%d\n", sizeof(&p + 1));//4		指针的地址+1
	printf("%d\n", sizeof(&p[0] + 1));//4第二的字符的地址
	printf("%d\n", strlen(p));//6		p指向字符串
	printf("%d\n", strlen(p + 1));//5		p指向第二个字符
	printf("%d\n", strlen(*p));//类型不匹配
	printf("%d\n", strlen(p[0]));//类型不匹配
	printf("%d\n", strlen(&p));//15		p指向字符串，&p为指针p的地址，直到找到\0为止
	printf("%d\n", strlen(&p + 1));//11		&p+1是p的下一个地址，+1的意思是加上+号之前的类型大小，直到找到\0为止，较前一个少4
	printf("%d\n", strlen(&p[0] + 1));//&p[0]是p[0]的地址，&p[0]+1是p[2]的地址
	
	//⼆维数组
	int a2[3][4] = { 0 };
	printf("%d\n", sizeof(a2));//48		数组a2的大小
	printf("%d\n", sizeof(a2[0][0]));//4		a2[0][0]的大小
	printf("%d\n", sizeof(a2[0]));//16		数组a2首元素（数组）的名称(因为定义为二维数组)
	printf("%d\n", sizeof(a2[0] + 1));//4		数组指针
	printf("%d\n", sizeof(a2 + 1));//4		数组指针
	printf("%d\n", sizeof(&a2[0] + 1));//4			a[0][1]的地址
	printf("%d\n", sizeof(*a2));//16		*a2=a[0]是数组指针
	printf("%d\n", sizeof(a2[3]));//16		数组指针，*虽然数组越界,但是有访问权限
    system("pause");
	return 0;
}

#include<windows.h>
int main()
{
	//数组
	int a[] = { 1, 2, 3, 4 };
	printf("%d\n", sizeof(a));//16		a是数组名，取的是啊数组a的大小
	printf("%d\n", sizeof(a + 0));//4		a+0是数组首元素的地址
	printf("%d\n", sizeof(*a));//4		指向数组的指针，所有指针的大小为4
	printf("%d\n", sizeof(a + 1));//4		数组第二个元素的地址
	printf("%d\n", sizeof(a[1]));//4		数组的第一个元素
	printf("%d\n", sizeof(&a));//4		整个数组的地址
	printf("%d\n", sizeof(&a + 1));//4		下一个数组的地址
	printf("%d\n", sizeof(&a[0]));//4		首元素的地址，等同于a + 0
	printf("%d\n", sizeof(&a[0] + 1));//4		数组第二个元素的地址等同于a+1
	
	//字符数组
	char arr[] = { 'a', 'b', 'c', 'd', 'e', 'f' };
	printf("%d\n", sizeof(arr));//6		a是数组名，取数组arr的大小
	printf("%d\n", sizeof(arr + 0));//4		首元素的地址
	printf("%d\n", sizeof(*arr));//1		首元素
	printf("%d\n", sizeof(arr[1]));//1			首元素
	printf("%d\n", sizeof(&arr));//4		整个数组的地址
	printf("%d\n", sizeof(&arr + 1));//4		下一个数组的地址
	printf("%d\n", sizeof(&arr[0] + 1));//4		第二个元素的地址
	//strlen的参数必须为char*，否则类型不匹配
	printf("%d\n", strlen(arr));//19   arr代表的是首元素的地址，从首元素一直往后找，一直找到\0位置  答案随机器不同不唯一
	printf("%d\n", strlen(arr + 0));//19   原因同上
	printf("%d\n", strlen(*arr));//   类型不匹配
	printf("%d\n", strlen(arr[1]));//类型不匹配
	printf("%d\n", strlen(&arr));//19  &arr代表的是整个数组的地址，数值同数组首元素的地址
	printf("%d\n", strlen(&arr + 1));//13		下一数组的地址
	printf("%d\n", strlen(&arr[0] + 1));//18		数组第二个元素的地值
	
	//字符串
	char *p = "abcdef";
	printf("%d\n", sizeof(p)); //4		p是指针
	printf("%d\n", sizeof(p + 1));//4		p+1任然是指针，指向第二个字符
	printf("%d\n", sizeof(*p));//1		第一个字符
	printf("%d\n", sizeof(p[0]));//1		第一个字符
	printf("%d\n", sizeof(&p));//4指针的地址
	printf("%d\n", sizeof(&p + 1));//4		指针的地址+1
	printf("%d\n", sizeof(&p[0] + 1));//4第二的字符的地址
	printf("%d\n", strlen(p));//6		p指向字符串
	printf("%d\n", strlen(p + 1));//5		p指向第二个字符
	printf("%d\n", strlen(*p));//类型不匹配
	printf("%d\n", strlen(p[0]));//类型不匹配
	printf("%d\n", strlen(&p));//15		p指向字符串，&p为指针p的地址，直到找到\0为止
	printf("%d\n", strlen(&p + 1));//11		&p+1是p的下一个地址，+1的意思是加上+号之前的类型大小，直到找到\0为止，较前一个少4
	printf("%d\n", strlen(&p[0] + 1));//&p[0]是p[0]的地址，&p[0]+1是p[2]的地址
	
	//⼆维数组
	int a2[3][4] = { 0 };
	printf("%d\n", sizeof(a2));//48		数组a2的大小
	printf("%d\n", sizeof(a2[0][0]));//4		a2[0][0]的大小
	printf("%d\n", sizeof(a2[0]));//16		数组a2首元素（数组）的名称(因为定义为二维数组)
	printf("%d\n", sizeof(a2[0] + 1));//4		数组指针
	printf("%d\n", sizeof(a2 + 1));//4		数组指针
	printf("%d\n", sizeof(&a2[0] + 1));//4			a[0][1]的地址
	printf("%d\n", sizeof(*a2));//16		*a2=a[0]是数组指针
	printf("%d\n", sizeof(a2[3]));//16		数组指针，*虽然数组越界,但是有访问权限
        system("pause");
	return 0;
}
```

