
# C两个小例子（数组与指针） - 嵌入式Linux - CSDN博客

2014年03月11日 11:46:24[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：879个人分类：[C																](https://blog.csdn.net/weiqifa0/article/category/1389385)




### 大端模式
所谓的大端模式，是指数据的高位，保存在内存的低地址中，而数据的低位，保存在内存的高地址中，这样的存储模式有点儿类似于把数据当作字符串顺序处理：地址由小向大增加，而数据从高位往低位放；
例子：
0000430: e684 6c4e 0100 1800 53ef 0100 0100 0000
0000440: b484 6c4e 004e ed00 0000 0000 0100 0000
在大端模式下，前16位应该这样读: e684
记忆方法: 地址的增长顺序与值的增长顺序相同
### 小端模式
所谓的小端模式，是指数据的高位保存在内存的高[地址](http://baike.baidu.com/view/494802.htm)中，而数 据的低位保存在内存的低地址中，这种存储模式将地址的高低和[数据](http://baike.baidu.com/view/38752.htm)位权有效地结合起来，高地址部分权值高，低地址部分权值低，和我们的逻辑方法一致。
例子：
0000430: e684 6c4e 0100 1800 53ef 0100 0100 0000
0000440: b484 6c4e 004e ed00 0000 0000 0100 0000
在小端模式下，前16位应该这样读: 84e6
记忆方法: 地址的增长顺序与值的增长顺序相反
### 大小端模式
为什么会有大小端模式之分呢？这是因为在[计算机系统](http://baike.baidu.com/view/1130583.htm)中，我们是以字节为单位的，每个地址单元都对应着一个字节，一个字节为 8bit。但是在C语言中除了8bit的char之外，还有16bit的short型，32bit的long型（要看具体的[编译器](http://baike.baidu.com/view/487018.htm)），另外，对于位数大于
 8位的处理器，例如16位或者32位的处理器，由于寄存器宽度大于一个字节，那么必然存在着一个如何将多个字节安排的问题。因此就导致了大端存储模式和小端存储模式。例如一个16bit的short型x，在内存中的地址为0x0010，x的值为0x1122，那么0x11为高字节，0x22为低字节。对于 大端模式，就将0x11放在低地址中，即0x0010中，0x22放在高地址中，即0x0011中。小端模式，刚好相反。我们常用的X86结构是小端模式，而KEIL C51则为大端模式。很多的ARM，DSP都为小端模式。有些ARM处理器还可以由硬件来选择是大端模式还是小端模式。

下面两个例子，只有一个是可以正确输出的。如下：
例一：

```python
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(void)
{	
	char a[20];
	int i;
	int *p=a;
	for(i=0;i<20;i++)
	{
		a[i]=i;
		printf("a[%d]=%d\n",i,a[i]);
	}
	//p++;
	printf("%d\n",*p);
	printf("&P=0x%X\n",p);
	printf("a=0x%X &a=0x%X\n",a,&a);
	printf("&a[0]=0x%X\n",&a[0]);
}
```
重点讲一下第一个例子：
首先要知道存储的大小端问题：
**大端模式，是指数据的高位，保存在内存的低地址中，而数据的低位，保存在内存的高地址中**
**小端模式，是指数据的高位保存在内存的高****[地址](http://baike.baidu.com/view/494802.htm)****中，而数
 据的低位保存在内存的低地址中**

```python
int main(void)
{
	//大端模式，是指数据的高位，保存在内存的低地址中，而数据的低位，保存在内存的高地址中
	//小端模式，是指数据的高位保存在内存的高地址中，而数 据的低位保存在内存的低地址中
	char x1,x0;
	int x=0x1122; 
	x0=((char*)&x)[0]; //低地址单元 如果等于0X22就说明是小端模式 如果等于0X11就说明是大端模式  0x11是高位数据 0x22是低位数据
	x1=((char*)&x)[1]; //高地址单元 
	printf("x0=%x x1=%x\n",x0,x1);
	if(0x22==x0)
	{
		printf("计算机是小端模式\n");
	}
	else
	{
		printf("计算机是大端模式\n");	
	}
}
```
最后得出是小端模式，也就 是数据的**低****位数据保存在****低****地址中 （一般情况，就我以前的编程经验，小端模式的计算机比较多）**
好了，现在知道是小端模式了，那么。再看回上面的代码。
a数组从0~19 依次是a[0]~a[19]的值是0~19.
那么，我p的地址和a的地址是一样的，p指向a数组的首元素。而且p是指向int类型，a是char类型。那么*p应该是从a[0]到a[3]4个元素。p会取得这四个元素的值。
又因为是小端模式（高位数据在高地址，低位数据在低地址，原来3是在高位数据，肯定回到高位地址，也就是0X30201000）。这四个字节取出来的值应该是：00000011 00000010 00000001 00000000 也就是 3210 转成int就是：**50462976**
![](https://img-blog.csdn.net/20140312091924343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpcWlmYTA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

好了。从上面的例子来看，我很快得出一个想法，看下面的代码

```python
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{	
	char a[20];
	char b='0';
	int i;
	int *p;
	p=a;
	for(i=0;i<20;i++)
	{
		a[i]=i;
		printf("a[%d]=%d\n",i,a[i]);
	}
	//p++;
	printf("%X\n",*p);
	printf("&P=0x%X\n",p);
	printf("b=%x b=%d\n",b,b);
	printf("a=0x%X &a=0x%X\n",a,&a);
	printf("&a[0]=0x%X\n",&a[0]);
}
```
![](https://img-blog.csdn.net/20140312092626343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpcWlmYTA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
例二：

```python
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(void)
{	
	char a[20];
	int i;
	char *p=a;
	for(i=0;i<20;i++)
	{
		a[i]=i;
		printf("a[%d]=%d\n",i,a[i]);
	}
	//p++;
	printf("%d\n",*p);
	printf("&P=0x%X\n",p);
	printf("a=0x%X &a=0x%X\n",a,&a);
	printf("&a[0]=0x%X\n",&a[0]);
}
```



