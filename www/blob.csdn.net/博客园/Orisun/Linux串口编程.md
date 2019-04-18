# Linux串口编程 - Orisun - 博客园







# [Linux串口编程](https://www.cnblogs.com/zhangchaoyang/articles/1942361.html)






`![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)代码`
#include<termios.h>

struct
termio

{
	unsigned
short c_iflag;		//输入模式

	unsigned
short c_oflag;	//输出模式

	unsigned
short c_cflag;		//控制模式

	unsigned
short c_lflag;		//本地模式

	unsigned
char c_line;		//line discipline

	unsigned
char c_cc[NCC];	//control characters

}














