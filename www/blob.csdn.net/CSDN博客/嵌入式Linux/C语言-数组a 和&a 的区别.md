
# C语言-数组a 和&a 的区别 - 嵌入式Linux - CSDN博客

2018年12月20日 15:35:25[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：76



## 面试经典题目
`#include "stdio.h"
int main()
{
	int a[5] = { 1,2,3,4,5 };
	int *ptr = (int *)(&a + 1);
	printf("%d,%d", *(a + 1), *(ptr - 1));
	/*getchar是用VS编写方便查看输出*/
	getchar();
	return 0;
}`
请思考一下上面的输出结果，如果你非常自信了，可以不用往下看

## 题目剖析
这个题目主要考察**&a**和**a**
a 在这里代表是的数组首元素的地址即 a[0]的首地址，其值为 0x005efda0。
&a 代表的是数组的首地址，其值为 0x005efda0。
a+1 的值是 0x005efda0+1*sizeof（int），等于 0x005efda4。
&a+1 的值是（0x005efda0 +（sizeof（a）= 5*sizeof(int)）= 0x005efdb4 {0xcccccccc, 0xcb626a90, 0x005efdd0, 0x0029200e, 0x00000001}
![](https://img-blog.csdnimg.cn/20181220153525238)

欢迎关注微信公众号-**嵌入式Linux**
觉得不错，请帮忙**转发，点赞**，您的每一次支持，我都将铭记于心

