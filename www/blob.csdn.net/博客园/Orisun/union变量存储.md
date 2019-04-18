# union变量存储 - Orisun - 博客园







# [union变量存储](https://www.cnblogs.com/zhangchaoyang/articles/2713658.html)





### 字节序

小端字节序（Little Edian）指低字节数据存放在内存低地址处，高字节数据存放在内存高地址处。比如整形1在内存中的存放情况是：

![](https://pic002.cnblogs.com/images/2012/103496/2012100711195053.png)

而换成大端字节序（Big Edian）则是：

![](https://pic002.cnblogs.com/images/2012/103496/2012100711221254.png)

基于Intel X86的体系结构的PC机是小端字节序，我们可以写一个程序来验证。

#include<stdio.h>

main(){
	int num=1;
	int c=num>>24;
	printf("%d\n",c);
	c=num & 0xff;
	printf("%d\n",c);
}


输出0和1。

我们知道union中只存储一个成员变量，union的长度是其size最大的成员变量的长度。

#include<stdio.h>

union {
	int i;
	char x[2];
}a;

main(){
	a.x[0]=10;
	a.x[1]=1;
	printf("%d\n",a.i);
}


a是全局变量，它被初始化为0。a占4个字节的内存空间，并且a.x和a.i共用此空间。联合体a中数据存储情况是：

![](https://pic002.cnblogs.com/images/2012/103496/2012100711304246.png)

所以a.i=0x010A，即266。












