# C语言调用函数设计 - xiahouzuoxin - CSDN博客





2012年11月07日 14:34:46[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：1204








在程序设计过程中，我们常常有这样的困惑，如何设计函数的数据输入和输出。

一个函数从内存的角度来看，可将参数分为输入缓冲、输出缓冲和返回STATE。

说白了，一个函数的功能就是对输入缓冲区的值执行某种操作，将执行的结果放入输出缓冲区中。




![](https://img-my.csdn.net/uploads/201211/07/1352270280_5709.jpg)















如上图所示，设输入缓冲区为*in_buf，长度为in_len，输出缓冲区为*out_buf，长度为out_len。返回类型为ERROR。则按如下方式设计函数的好处

ERROR fun(uint8 *in_buf, uint16 in_len, uint8 *out_buf,uint16 out_len)

（1）return ERROR的好处：利于将函数封装成API，当外人调用时，可以通过返回信息判断何处出了问题，这也是一种调试的手段。暑假实习中一位工程师明确要求要返回Error代码。

（2）传递缓冲区长度好处：在函数中检查函数长度，以避免输出缓冲不足的内存ERROR，增强程序的健壮性。



