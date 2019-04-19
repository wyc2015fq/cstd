# Linux 下IO端口编程访问 - xqhrs232的专栏 - CSDN博客
2016年04月28日 22:13:13[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：479
原文地址::[http://blog.csdn.net/liyuanbhu/article/details/35991407](http://blog.csdn.net/liyuanbhu/article/details/35991407)
相关文章
1、[Linux下的IO端口和IO内存](http://blog.csdn.net/acs713/article/details/7911025)----[http://blog.csdn.net/acs713/article/details/7911025](http://blog.csdn.net/acs713/article/details/7911025)
2、IO操作函数----[http://biancheng.dnbcw.info/linux/318118.html](http://biancheng.dnbcw.info/linux/318118.html)
以前写的一篇笔记，偶尔翻出来了，放在这里做个纪念
# Linux 下IO端口编程访问
这里记录的方法是在用户态访问IO端口，不涉及驱动程序的编写。
首先要包含头文件 /usr/include/asm/io.h
## ioperm()
在 unistd.h 中声明。用来打开对IO端口的访问权限，要求程序执行时必须有root 权限。只可以打开0x00到0x3ff 这一地址段的IO端口。
 ioperm( from, num, turn_on)
比如：
**[python]**[view
 plain](http://blog.csdn.net/liyuanbhu/article/details/35991407#)[copy](http://blog.csdn.net/liyuanbhu/article/details/35991407#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/411466/fork)
- #include<asm/io.h>
- #include<unistd.h>
- ioperm( 0x300, 5, 1);  
表示从0x300 地址起的连续5个IO地址的访问权限被打开。
##  iopl()
如果需要访问比0x3ff更高的地址段，需要用iopl() 函数。
iopl(3) 表示打开对全部IO端口的访问权限。
## IO 读写操作
**[cpp]**[view
 plain](http://blog.csdn.net/liyuanbhu/article/details/35991407#)[copy](http://blog.csdn.net/liyuanbhu/article/details/35991407#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/411466/fork)
- inb(port);  
- inw(port);  
- outb(value, port);  
- outw(value, port);  
对IO端口的读写大约用时1us。需要注意的是outb 函数与 turbo C 中的 outportb 函数的参数的顺序是反的，不要搞错。
**[cpp]**[view
 plain](http://blog.csdn.net/liyuanbhu/article/details/35991407#)[copy](http://blog.csdn.net/liyuanbhu/article/details/35991407#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/411466/fork)
- inb_p(port);  
- inw_p(port);  
- outb_p(value, port);  
- outw_p(value, port);  
这四个函数和上面四个函数的功能基本类似，但是在访问完IO端口后会增加一个1us左右的延时。如果定义了REALLY_SLOW_IO这个宏，则延时约4us。
**[cpp]**[view
 plain](http://blog.csdn.net/liyuanbhu/article/details/35991407#)[copy](http://blog.csdn.net/liyuanbhu/article/details/35991407#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/411466/fork)
- #define REALLY_SLOW_IO 1
- #include<asm/io.h>
- inb_p(port);  
## /dev/port
另外一种访问IO端口的方法是通过对/dev/port 设备文件来访问。
这种方法不需要有root 权限，只有有对/dev/port的访问权限就行。
访问起来就像普通的文件访问一样， 先 lseek(), 然后read()、 write() 函数。
但是不能使用 select() 函数和 poll() 函数，因为IO端口不支持这些功能。
## 开关 中断
iopl(3) 打开访问权限后可以通过插入汇编语句的方式开关中断。
**[cpp]**[view
 plain](http://blog.csdn.net/liyuanbhu/article/details/35991407#)[copy](http://blog.csdn.net/liyuanbhu/article/details/35991407#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/411466/fork)
- asm(“cli”); //关中断
- asm(“sti”); //开中断
