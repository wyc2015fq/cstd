# Introduction to the Linux Kernel - Koma Hub - CSDN博客
2019年02月28日 21:42:52[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：26
个人分类：[Linux driver																[Linux kernel](https://blog.csdn.net/Rong_Toa/article/category/7348890)](https://blog.csdn.net/Rong_Toa/article/category/8645170)

![](https://img-blog.csdnimg.cn/20190228213723162.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190228213747582.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190228213817545.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# ● System call interface (SCI)
- ○ A thin layer that provides a method to interact from user space to kernel space
# ● Process Management (PM)
- ○ Create, destroy processes
- ○ Communication between different processes (kernel threads)
- ○ CPU scheduling
# ● Memory Management (MM)
- ○ Physical to virtual memory management
- ○ Memory allocation
- ○ Swapping, from memory to hard disk
# **● Virtual File System (VFS)**
- **○ Eports the common file interface**
- **○ Abstract file system functionality from implementation**
![](https://img-blog.csdnimg.cn/20190228214136165.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# **● File Systems**
- **○ Implementation of FS functionality**
# **● Buffer Cache**
- **○ A set of functions to manipulate main memory designed for FS**
# **● Device Driver**
# **● Physical Device**
- **○ Where data live**
# ● Network Stack
- ○ Implement the network protocols
- ○ Deliver packets across programs and network interfaces
# ● Device Drivers (DD)
- ○ Interact with the hardware
- ○ Extract an abstraction of the device functionalities
# ● Arch
- ○ Architecture dependent code
> 
Praktikum Kernel Programming
University of Hamburg
Scientific Computing
Winter semester 2014/2015
