# CPU的大端模式(big endian)和小端(little endian)模式 - 江南烟雨 - CSDN博客
2012年02月26日 14:05:23[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：5797
作为一个计算机相关专业的人，我们应该在计算机组成中都学习过什么叫Little endian 和Big endian。Little endian 和Big endian 是CPU 存放数据的两种不同顺序。对于整型、长整型等数据类型，Big endian 认为第一个字节是最高位字节（按照从低地址到高地址的顺序存放数据的高位字节到低位字节）；而Little
 endian 则相反，它认为第一个字节是最低位字节（按照从低地址到高地址的顺序存放数据的低位字节到高位字节）。
例如，假设从内存地址0x0000 开始有以下数据：
0x12 0x34 0xab 0xcd
如 果我们去读取一个地址为0x0000 的四个字节变量，若字节序为big-endian，则读出结果为0x1234abcd；若字节序位little-endian，则读出结果为 0xcdab3412。如果我们将0x1234abcd 写入到以0x0000 开始的内存中，则Little endian 和Big endian 模式的存放结果如下：
地址               0x0000 0x0001 0x0002 0x0003
big-endian         0x12   0x34   0xab   0xcd
little-endian      0xcd   0xab 0x34   0x12
一般来说，x86 系列CPU 都是little-endian 的字节序，PowerPC 通常是Big endian，还有的CPU 能通过跳线来设置CPU 工作于Little endian 还是Big endian 模式。
显然，解答这个问题的方法只能是将一个字节（CHAR/BYTE 类型）的数据和一个整型数据存放于同样的内存
开始地址，通过读取整型数据，分析CHAR/BYTE 数据在整型数据的高位还是低位来判断CPU 工作于Little
endian 还是Big endian 模式。得出如下的答案：
```cpp
typedef unsigned char BYTE;
int main(int argc, char* argv[])
{
	unsigned int num,*p;
	p = #
		num = 0;
	*(BYTE *)p = 0xff;//指针类型强制转换
	if(num == 0xff)
	{
		printf("The endian of cpu is little\n");
	}
	else //num == 0xff000000
	{
		printf("The endian of cpu is big\n");
	}
	return 0;
}
```
除了上述方法(通过指针类型强制转换并对整型数据首字节赋值，判断该赋值赋给了高位还是低位)外，还有没
有更好的办法呢？我们知道，union 的成员本身就被存放在相同的内存空间（共享内存，正是union 发挥作用、做贡献的去处），因此，我们可以将一个CHAR/BYTE 数据和一个整型数据同时作为一个union 的成员，得出
如下答案：
```cpp
int checkCPU()
{
	{
		union w
		{
			int a;
			char b;
		} c;
		c.a = 1;
		return (c.b == 1);
	}
}
```
实现同样的功能，我们来看看Linux 操作系统中相关的源代码是怎么做的：
```cpp
static union { char c[4]; unsigned long mylong; } endian_test = {{ 'l', '?', '?', 'b' } };
#define ENDIANNESS ((char)endian_test.mylong)
```
Linux 的内核作者们仅仅用一个union 变量和一个简单的宏定义就实现了一大段代码同样的功能！由以上一段代码我们可以深刻领会到Linux 源代码的精妙之处！
(如果ENDIANNESS=’l’表示系统为little endian,为’b’表示big endian )。
