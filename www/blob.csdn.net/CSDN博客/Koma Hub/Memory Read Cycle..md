# Memory Read Cycle. - Koma Hub - CSDN博客
2019年03月18日 21:41:25[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：28
个人分类：[Assembly																[体系结构																[汇编](https://blog.csdn.net/Rong_Toa/article/category/8750815)](https://blog.csdn.net/Rong_Toa/article/category/7496482)](https://blog.csdn.net/Rong_Toa/article/category/8750816)
![](https://img-blog.csdnimg.cn/20190318213711577.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
The following is a simplified description of what happens during each clock cycle during a memory read:
> - Cycle 1: The address bits of the memory operand are placed on the address bus (ADDR). The address lines in the diagram cross, showing that some bits equal 1 and others equal 0.
- Cycle 2: The read line (RD) is set low (0) to notify memory that a value is to be read.
- Cycle 3: The CPU waits one cycle to give memory time to respond. During this cycle, the memory controller places the operand on the data bus (DATA).
- Cycle 4: The read line goes to 1, signaling the CPU to read the data on the data bus.
- 周期1：内存操作数的地址位放在地址总线（addr）上。图中的地址行交叉，表示一些位等于1，另一些位等于0。
- 周期2：读取行（rd）设置为低（0），以通知内存要读取值。
- 周期3：CPU等待一个周期以给内存时间来响应。在这个循环中，内存控制器将操作数放在数据总线（数据）上。
- 周期4：读取线转到1，向CPU发送信号，以读取数据总线上的数据。
### Cache Memory
> 
Because conventional memory is so much slower than the CPU, computers use high-speed cache memory to hold the most recently used instructions and data. The first time a program reads a block of data, it leaves a copy in the cache. If the program needs to read the same data a second time, it looks for the data in cache. A cache hit indicates the data is in cache; a cache  miss indicates the data is not in cache and must be read from conventional memory. In general, cache memory has a noticeable effect on improving access to data, particularly when the cache is large.
由于传统的内存比CPU慢得多，计算机使用高速缓存来保存最近使用的指令和数据。程序第一次读取数据块时，会在缓存中留下一个副本。如果程序需要再次读取相同的数据，它会在缓存中查找数据。缓存命中表示数据在缓存中；缓存未命中表示数据不在缓存中，必须从常规内存中读取。通常，缓存内存对改善数据访问有显著的影响，特别是当缓存很大时。
