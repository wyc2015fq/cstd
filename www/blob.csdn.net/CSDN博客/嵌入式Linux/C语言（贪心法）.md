
# C语言（贪心法） - 嵌入式Linux - CSDN博客

2014年04月30日 10:02:27[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：2347


C语言有这样一个规则，每一个符号应该包含尽可能多的字符。也就是说，编译器将程序分解成符号的方法是，从左到右一个一个字符地读入，如果字条可能组成一个符号，那么再读入下一个字符，判断已经读入的两个字符组成的字符串是否可能是一个符号的组成部分，如果可能，继续读入下一个字条，重复上述判断，直到读入的字符组成的字符串已经不再可能组成一个有意义的符号。这个处理的策略被称为“贪心法”。需要注意的是，除了字符串与字符常量，符号的中间不能嵌有空白（空格、制表符、换行符等）。

看一下下面的代码：想一下输出会是什么？

```python
#include "stdio.h"
int main(void)
{
	int a=2;
	int b=3;
	int i=1;
	printf("%d\n",a+++b);
	printf("%d,%d\n",a,b);
	a=2;b=3;
	printf("%d\n",a+++(++b));
	printf("%d,%d\n",a,b);
	a=2;b=3;
	printf("%d\n",i);
	printf("%d\n",++i+(++i)+(++i));
	printf("%d\n",i);
	i=1;
	printf("%d\n",++i+(++i)+(++i)+(++i));
	printf("%d\n",i);
	i=1;
	printf("%d\n",++i+(++i)+((++i)+(++i)));
	printf("%d\n",i);

	return 0;
}
```
![](https://img-blog.csdn.net/20140430100118796?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpcWlmYTA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
在计算机里面始终只有一个i.++i+(++i) 计算机会先进行两个++i操作，然后才会进行+操作。所以会有以上的结果。



