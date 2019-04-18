# newcoder错题集 - YuDale的博客 - CSDN博客
2017年03月09日 21:34:24[YuDale](https://me.csdn.net/YuDale)阅读数：194
1.运算（93&-8）的结果为（）；
负数需要用补码运算
01011101
&10001000
  01011000
2.假设在一个 32 位 little endian 的机器上运行下面的程序，结果是多少？
![](https://img-blog.csdn.net/20170309214035729?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvWXVEYWxl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
大端模式：数据的高字节保存在内存的低地址中，而数据的低字节保存的内存的高地址中。
小端模式：数据的高字节保存在内存的低地址中，而数据的低字节保存在内存的高地址中。
long long 占8字节
int 占4字节
![](https://img-blog.csdn.net/20170309213951172?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvWXVEYWxl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
b++：b=b;b=b+1;
++b: b=b+1;b=b;
