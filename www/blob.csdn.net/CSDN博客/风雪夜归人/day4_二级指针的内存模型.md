# day4_二级指针的内存模型 - 风雪夜归人 - CSDN博客
2017年09月05日 22:34:07[cdjccio](https://me.csdn.net/qq_34624951)阅读数：137
1、二级指针的内存模型
```cpp
#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
void main2()
{
	int i = 0;
	//指针数组
	char *   p1[] = {"123", "456", "789"};
	//二维数组 数据直接存放在堆区
	char p2[3][4]  = {"123", "456", "789"};
	//手工二维内存
	char **p3 = (char **)malloc(3 * sizeof(char *)); //int array[3];
	for (i=0; i<3; i++)
	{
		p3[i] = (char *)malloc(10*sizeof(char)); //char buf[10]
		sprintf(p3[i], "%d%d%d", i, i, i);
	}
}
```
2、如图所示
![](https://img-blog.csdn.net/20170905223513197?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzQ2MjQ5NTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
