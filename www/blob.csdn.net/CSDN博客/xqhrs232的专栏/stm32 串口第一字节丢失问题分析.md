# stm32 串口第一字节丢失问题分析 - xqhrs232的专栏 - CSDN博客
2013年04月28日 11:19:12[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1146
原文地址::[http://blog.21ic.com/user1/4939/archives/2012/90426.html](http://blog.21ic.com/user1/4939/archives/2012/90426.html)
相关网帖
1、**stm32串口发送数据，丢失字节问题分析**** ----[http://www.amobbs.com/forum.php?mod=viewthread&tid=5537246&reltid=5557456&pre_thread_id=0&pre_pos=1&ext=](http://www.amobbs.com/forum.php?mod=viewthread&tid=5537246&reltid=5557456&pre_thread_id=0&pre_pos=1&ext=)**
2、**STM32 串口问题 第一个字节发送不出去----[http://www.amobbs.com/thread-5557456-1-1.html](http://www.amobbs.com/thread-5557456-1-1.html)**
STM32 串口 发送 必须 先检测 状态，否则 第一个 字节 无法 发出，发送完毕，必须检测发送状态是否完成，否则，发送不成功，
使用stm32f10x调试串口通讯时，发现一个出错的现象，硬件复位重启之后，发送测试数据0x01 0x02 0x03 0x04..接收端收到的数据为：0x02 0x03 0x04，第一个数据丢失。换成发送别的数值的数据，如0x06 0x0ff，则接收到0x0ff，0x06丢失。错误依旧。 
    故障排除过程： 
    1、刚开始怀疑是接收端的错误，我是使用电脑串口，运行串口辅助调试工具接收，换成其他软件后，发现故障依旧，而且电脑软件一直是开启状态，不像和电脑软件有关。 
    2、使用单步调试，单步运行各个发送指令，都正常。能收到0x01 0x02 0x03 0x04的数据。间接的排除了不是电脑软件的问题，而是其他的错误。 
    3、单步调试运行虽然正常了，但连续运行时，错误依旧。现在有点摸不到头绪了，单步运行正常，看起来编程没有出错，那故障在哪里呢？测试程序如下 
   USART_SendData(USART2, 0x01);                                 //A 
   while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);   //B 
   USART_SendData(USART2, 0x02);                                 //C 
   while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET); 
   USART_SendData(USART2, 0x03); 
   while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET); 
   USART_SendData(USART2, 0x04); 
   while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET); 
    4、猜测，也许是因为某个特殊原因，使第二个数据覆盖了首个数据，使得首个数据丢失。假设：在执行B指令时，USART的 TC 状态位==SET，那么就会紧接着执行C指令，也就有可能发生数据的覆盖。于是，在A指令前，加入如下指令： 
   USART_ClearFlag(USART2,USART_FLAG_TC); 
    5、加入上一条指令后，运行，错误消失了。说明上一个假设，应该是成立的。 
    6、查阅stm32f10x参考手册，找到这样一句话： 
TC：发送完成 
当包含有数据的一帧发送完成后，由硬件将该位置位。如果USART_CR1中的TCIE为1，则产生中断。由软件序列清除该位(先读USART_SR，然后写入USART_DR)。TC位也可以通过写入0来清除，只有在多缓存通讯中才推荐这种清除程序。 
0：发送还未完成； 
1：发送完成。 
    7、注意到这一句：由软件序列清除该位(先读USART_SR，然后写入USART_DR)。 也就是说，要先read USART_SR,然后write USART_DR，才能完成TC状态位的清除。而硬件复位后，串口发送的首个数据之前没有read SR的操作，是直接write DR，也就是说，TC没有被清除掉。
 说明第4步的猜测是对的。 
    8、那么，应该把指令A前面加的USART_ClearFlag(USART2,USART_FLAG_TC); 改为USART_GetFlagStatus(USART2, USART_FLAG_TC);，应该也能消除错误。测试后证实，确实如此，在发送首个数据之前，先读取一下USART_SR，那么就不会出现首个数据丢失的情况了。 
    9、总结：硬件复位后，串口发送首个数据之前，先读取一下USART_SR，则能够保证首个数据发送时，不出现覆盖的情况。当然，也有别的方法，比如先清除TC状态位，或是，在write USART_DR之后，加入一个小延时，让数据发送完毕，应该也能间接排除这个错误。 

//======================================================================
备注：：
1》[STM32串口中断这样的问题是怎么回事呢？](http://www.amobbs.com/thread-5428187-1-1.html)----[http://www.amobbs.com/thread-5428187-1-1.html](http://www.amobbs.com/thread-5428187-1-1.html)
2》ARM串口通讯丢包问题，急！！！----[http://bbs.eeworld.com.cn/archiver/tid-127904.html](http://bbs.eeworld.com.cn/archiver/tid-127904.html)
