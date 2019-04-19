# Floating-Point Unit Registers. - Koma Hub - CSDN博客
2019年03月18日 21:55:02[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：12

![](https://img-blog.csdnimg.cn/20190318215326540.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
> 
When discussing processor evolution, references to the data bus size are significant because they affect system performance. When a processor uses an 8-bit data bus to transfer a 32-bit integer to memory, for example, the integer must be broken into four parts, and four separate data transfer operations are required to complete the operation. Given that there is a significant delay (called latency) involved in each data transfer operation, an 8-bit data bus transfers data at onefourth the speed of a 32-bit data bus.
在讨论处理器演进时，对数据总线大小的引用非常重要，因为它们会影响系统性能。例如，当处理器使用8位数据总线将32位整数传输到内存时，整数必须分成四部分，并且需要四个单独的数据传输操作来完成该操作。考虑到每个数据传输操作都有一个很大的延迟（称为延迟），8位数据总线以32位数据总线速度的四分之一传输数据。
