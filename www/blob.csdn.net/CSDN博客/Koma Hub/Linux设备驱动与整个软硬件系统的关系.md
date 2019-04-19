# Linux设备驱动与整个软硬件系统的关系 - Koma Hub - CSDN博客
2019年02月27日 22:14:43[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：24
**Table of Contents**
[编写Linux设备驱动的技术基础](#%E7%BC%96%E5%86%99Linux%E8%AE%BE%E5%A4%87%E9%A9%B1%E5%8A%A8%E7%9A%84%E6%8A%80%E6%9C%AF%E5%9F%BA%E7%A1%80)
![Linux设备驱动与整个软硬件系统的关系](https://img-blog.csdnimg.cn/201902272212359.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# 编写Linux设备驱动的技术基础
- 编写Linux 设备驱动要求工程师具有良好的硬件基础，懂得SRAM、Flash、SDRAM、磁盘的读写方式，UART、I2C、USB等设备的接口，轮询、中断、DMA的原理，PCI总线的工作方式以及CPU的内存管理单元（MMU）等。
- 编写Linux 设备驱动要求工程师具有良好的C语言基础，能灵活地运用C语言的结构体、指针、函数指针及内存动态申请和释放等。
- 编写Linux 设备驱动要求工程师具有一定的Linux 内核基础，虽然并不要求工程师对内核各个部分有深入的研究，但至少要了解设备驱动与内核的接口，尤其是对于块设备、网络设备、Flash设备、串口设备等复杂设备。
- 编写Linux 设备驱动要求工程师具有良好的多任务并发控制和同步的基础，因为在设备驱动中会大量使用自旋锁、互斥、信号量、等待队列等并发与同步机制。
