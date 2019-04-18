# Big&amp;Little Endian　高&amp;低字节序 - ljx0305的专栏 - CSDN博客
2008年09月09日 14:01:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：1248标签：[solaris																[sun																[通讯																[编程																[网络																[x86](https://so.csdn.net/so/search/s.do?q=x86&t=blog)](https://so.csdn.net/so/search/s.do?q=网络&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=通讯&t=blog)](https://so.csdn.net/so/search/s.do?q=sun&t=blog)](https://so.csdn.net/so/search/s.do?q=solaris&t=blog)
个人分类：[C++																[C](https://blog.csdn.net/ljx0305/article/category/394717)](https://blog.csdn.net/ljx0305/article/category/380566)
  由于CPU的两大对立阵营的对抗导致了所谓的字节顺序问题。PowerPC(Moto&IBM) VS X86 = Big Vs. Little,两种不同处理多字节数据的方式在多机通讯和网络编程时会给我们带来意外的“惊喜”。开了书和网上的帖子，其实挺简单，一言以蔽之就是 Big-endian最先读写最高的字节，Little-endian反其道而行，低字节优先。 如下例示： 
**Big Endian **   低地址                                            高地址 
   -----------------------------------------> 
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
   |     12     |      34    |     56      |     78    | 
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
**Little Endian **   低地址                                            高地址 
   -----------------------------------------> 
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
   |     78     |      56    |     34      |     12    | 
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
举个例子，从内存地址0x0000开始有以下数据 
0x0000     0x12 
0x0001     0x34 
0x0002     0xab 
0x0003     0xcd 
如果我们去读取一个地址为0x0000的四个字节变量，若字节序为big-endian，则读出 
结果为0x1234abcd；若字节序位little-endian，则读出结果为0xcdab3412. 
如果我们将0x1234abcd写入到以0x0000开始的内存中，则结果为 
                big-endian     little-endian 
0x0000     0x12              0xcd
0x0001     0x23![](http://imgcache.qq.com/ac/b.gif)              0xab
0x0002     0xab               0x34
0x0003     0xcd               0x12
intel是高字节序
solaris(Sun系统)是高字节序
高字节序即高位 (所谓高位指的是2(n-1） ~2(n-8)的值) 存低地址         
低字节序即低位 (所谓低位指的是 27~20的值) 存低地址
例
#i nclude <stdio.h>
union
{
int i;
char x[2];
}a;
void main()
{
a.x[0] = 10;
a.x[1] = 1;
printf("%d",a.i);
}
答案：266 (低位低地址，高位高地址，内存占用情况是Ox010A）
