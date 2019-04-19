# System calls - Koma Hub - CSDN博客
2019年02月28日 21:54:42[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：14
个人分类：[Linux kernel](https://blog.csdn.net/Rong_Toa/article/category/7348890)
> 
Kernel programming is vital for as long as new hardware is being designed and produced or old-obsolete hardware is maintained.
● A syscall causes a programmed exception (trap) on the CPU
○ syscall(number, arguments)
● Within the kernel you cannot access user space buffers
![](https://img-blog.csdnimg.cn/2019022821542046.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
