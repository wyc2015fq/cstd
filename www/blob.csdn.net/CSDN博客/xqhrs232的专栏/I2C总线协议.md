# I2C总线协议 - xqhrs232的专栏 - CSDN博客
2016年07月08日 23:13:09[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：990
原文地址::[http://blog.csdn.net/subkiller/article/details/6854910](http://blog.csdn.net/subkiller/article/details/6854910)
相关文章
1、
## 嵌入式I2C总线协议
 ----[http://blog.sina.com.cn/s/blog_8795b0970101j9rg.html](http://blog.sina.com.cn/s/blog_8795b0970101j9rg.html)
2、i2c总线协议中文版----[http://download.csdn.net/download/waiwn/2184650](http://download.csdn.net/download/waiwn/2184650)
3、i2c总线协议(中文版pdf)----[http://wenku.baidu.com/link?url=qj6AswESx7wUOgUF_u6EX3OOKaY0LFBHMeFwSkyKGbiwu8AUlHosiQwd8HOmsOZAjv0QnO6xrJp3FGivwwN5qLSMl-bCEm74fYr6GBHPxGC](http://wenku.baidu.com/link?url=qj6AswESx7wUOgUF_u6EX3OOKaY0LFBHMeFwSkyKGbiwu8AUlHosiQwd8HOmsOZAjv0QnO6xrJp3FGivwwN5qLSMl-bCEm74fYr6GBHPxGC)
4、I2C总线协议中文版PDF----[http://www.docin.com/p-56479990.html](http://www.docin.com/p-56479990.html)
5、I2C总线协议及工作原理----[http://wenku.baidu.com/link?url=4rZQIfDw8Qr3DwWAmyM9wJzINpLHG2P79RgdR5r-sZqBxo4HPaPley-ZNEA2Xi9xX1f63n_3yvVRIb-vRfT22sVb8nTlZjFxAKy2idVTSea](http://wenku.baidu.com/link?url=4rZQIfDw8Qr3DwWAmyM9wJzINpLHG2P79RgdR5r-sZqBxo4HPaPley-ZNEA2Xi9xX1f63n_3yvVRIb-vRfT22sVb8nTlZjFxAKy2idVTSea)
1.I2C协议
 2条双向串行线，一条数据线SDA，一条时钟线SCL。
   SDA传输数据是大端传输，每次传输8bit，即一字节。
   支持多主控(multimastering)，任何时间点只能有一个主控。
   总线上每个设备都有自己的一个addr，共7个bit，广播地址全0.
   系统中可能有多个同种芯片，为此addr分为固定部分和可编程部份，细节视芯片而定，看datasheet。
1.1 I2C位传输
   数据传输：SCL为高电平时，SDA线若保持稳定，那么SDA上是在传输数据bit；
   若SDA发生跳变，则用来表示一个会话的开始或结束（后面讲）
   数据改变：SCL为低电平时，SDA线才能改变传输的bit
![I2C学习笔记 - dp - dp: 生活的脚步，进步的点滴...](http://img229.ph.126.net/80fkQVkO3xf4s6JvsC06bw==/2144557847560009779.jpg)
1.2
 I2C开始和结束信号
开始信号：SCL为高电平时，SDA由高电平向低电平跳变，开始传送数据。
   结束信号：SCL为高电平时，SDA由低电平向高电平跳变，结束传送数据。
![I2C学习笔记 - dp - dp: 生活的脚步，进步的点滴...](http://img763.ph.126.net/quJFSxY28dC9sDoleMTrFA==/1997909384693792049.jpg)
**1.3 I2C应答信号**
   Master每发送完8bit数据后等待Slave的ACK。
   即在第9个clock，若从IC发ACK，SDA会被拉低。
   若没有ACK，SDA会被置高，这会引起Master发生RESTART或STOP流程，如下所示：
![I2C学习笔记 - dp - dp: 生活的脚步，进步的点滴...](http://img775.ph.126.net/eIz4ez2q8SUeXKiSapsGXA==/4843058449283985368.jpg)
1.4
 I2C写流程
写寄存器的标准流程为：
1.    Master发起START
2.    Master发送I2C addr（7bit）和w操作0（1bit），等待ACK
3.    Slave发送ACK
4.    Master发送reg addr（8bit），等待ACK
5.    Slave发送ACK
6.    Master发送data（8bit），即要写入寄存器中的数据，等待ACK
7.    Slave发送ACK
8.    第6步和第7步可以重复多次，即顺序写多个寄存器
9.    Master发起STOP
***写一个寄存器***
![I2C学习笔记 - dp - dp: 生活的脚步，进步的点滴...](http://img609.ph.126.net/j7OZijBSkop1sPycnNnBfQ==/1892919218380911741.jpg)
**写多个寄存器**
![I2C学习笔记 - dp - dp: 生活的脚步，进步的点滴...](http://img535.ph.126.net/j7w9Dcj4sYiPaZA5jV1Emg==/1297318167660661668.jpg)
1.5
 I2C读流程
读寄存器的标准流程为：
1.    Master发送I2C addr（7bit）和w操作1（1bit），等待ACK
2.    Slave发送ACK
3.    Master发送reg addr（8bit），等待ACK
4.    Slave发送ACK
5.    Master发起START
6.    Master发送I2C addr（7bit）和r操作1（1bit），等待ACK
7.    Slave发送ACK
8.    Slave发送data（8bit），即寄存器里的值
9.    Master发送ACK
10.    第8步和第9步可以重复多次，即顺序读多个寄存器
**读一个寄存器**
![I2C学习笔记 - dp - dp: 生活的脚步，进步的点滴...](http://img837.ph.126.net/0_dFcVHoWn1lQOLdn6-w7Q==/770678486235691412.jpg)
*读多个寄存器*
![I2C学习笔记 - dp - dp: 生活的脚步，进步的点滴...](http://img611.ph.126.net/L9QOsROhgTgCezJCVcQvRQ==/1669991036826428611.jpg)
2. PowerPC的I2C实现
Mpc8560的CCSR中控制I2C的寄存器共有6个。
![I2C学习笔记 - dp - dp: 生活的脚步，进步的点滴...](http://img858.ph.126.net/-WRIut5YNuDD7q9-dvyXtA==/3097350643725093417.jpg)
**2.1 I2CADR 地址寄存器**
CPU也可以是I2C的Slave，CPU的I2C地址有 I2CADR指定
![I2C学习笔记 - dp - dp: 生活的脚步，进步的点滴...](http://img170.ph.126.net/JHTq5lL0BIevBmQ_9Hm54g==/1602999992368960276.jpg)
2.2
 I2CFDR 频率设置寄存器
![I2C学习笔记 - dp - dp: 生活的脚步，进步的点滴...](http://img66.ph.126.net/FD_KXFeijfsyEPfIkNuKpQ==/838795430599547114.jpg)
The serial bit clock frequency of SCL
 is equal to the CCB clock divided by the divider.
用来设置I2C总线频率
2.3 I2CCR 控制寄存器
![I2C学习笔记 - dp - dp: 生活的脚步，进步的点滴...](http://img124.ph.126.net/9RU1Ui0hBfLd1C0v8kFNnQ==/3050062847638670264.jpg)
MEN： Module Enable.    置1时，I2C模块使能
MIEN：Module Interrupt Enable. 置1时，I2C中断使能。
MSTA：Master/slave mode. 1 Master mode，0 Slave mode.
        当1->0时，CPU发起STOP信号
        当0->1时，CPU发起START信号
MTX：Transmit/receive mode select.0 Receive mode，1 Transmit mode
TXAK：Transfer acknowledge. 置1时，CPU在9th clock发送ACK拉低SDA
RSTA：Repeat START. 置1时，CPU发送REPEAT START
BCST：置1，CPU接收广播信息（信息的slave addr为7个0）
2.4 I2CSR 状态寄存器
![I2C学习笔记 - dp - dp: 生活的脚步，进步的点滴...](http://img242.ph.126.net/VhJ0qM9c8zXz0WiW70JmzA==/1432426156482553175.jpg)
MCF：0  Byte transfer is in process
     1  Byte transfer is completed
MAAS：当CPU作为Slave时，若I2CDR与会话中Slaveaddr匹配，此bit被置1
MBB：0 I2C bus idle  
     1 I2C bus busy
MAL：若置1，表示仲裁失败
BCSTM：若置1，表示接收到广播信息
SRW：When MAAS is set, SRW indicates the value of the R/W command bit of the calling address, which is sent from the master.
   0 Slave receive, master writing to slave
   1 Slave transmit, master reading from slave
MIF：Module interrupt. The MIF bit is set when an interrupt is pending, causing a processor interrupt request(provided
 I2CCR[MIEN] is set)
RXAK：若置1，表示收到了ACK
2.5 I2CDR 数据寄存器
![I2C学习笔记 - dp - dp: 生活的脚步，进步的点滴...](http://img389.ph.126.net/2foV_BQLlY33hIeBB0bySA==/2409144325667732566.jpg)
这个寄存器储存CPU将要传输的数据。
3. PPC-Linux中I2C的实现
  内核代码中，通过I2C总线存取寄存器的函数都在文件drivers/i2c/busses/i2c-mpc.c中
  最重要的函数是mpc_xfer.
- staticint mpc_xfer(struct i2c_adapter *adap,struct i2c_msg *msgs,int num)
- {
- struct i2c_msg *pmsg;
- int i;
- int ret = 0;
- unsignedlong orig_jiffies = jiffies;
- struct mpc_i2c *i2c = i2c_get_adapdata(adap);
- 
-     mpc_i2c_start(i2c);//
 设置I2CCR[MEN], 使能I2C module 
- 
- /* Allow bus up to 1s to become not busy */
- //一直读I2CSR[MBB]，等待I2C总线空闲下来
- while(readb(i2c->base + MPC_I2C_SR)& CSR_MBB){
- if(signal_pending(current)){
-             pr_debug("I2C:
 Interrupted\n");
-             writeccr(i2c, 0);
- return-EINTR;
- }
- if(time_after(jiffies, orig_jiffies + HZ)){
-             pr_debug("I2C:
 timeout\n");
- if(readb(i2c->base + MPC_I2C_SR)==
- (CSR_MCF | CSR_MBB | CSR_RXAK))
-                 mpc_i2c_fixup(i2c);
- return-EIO;
- }
-         schedule();
- }
- 
- for(i = 0; ret >= 0 && i < num; i++){
-         pmsg =&msgs[i];
-         pr_debug("Doing
 %s %d bytes to 0x%02x - %d of %d messages\n",
-              pmsg->flags & I2C_M_RD ?"read":"write",
-              pmsg->len, pmsg->addr, i + 1, num);
- //根据消息里的flag进行读操作或写操作
- if(pmsg->flags & I2C_M_RD)
-             ret = mpc_read(i2c, pmsg->addr, pmsg->buf, pmsg->len, i);
- else
-             ret = mpc_write(i2c, pmsg->addr, pmsg->buf, pmsg->len, i);
- }
-     mpc_i2c_stop(i2c);//保证为I2CCSR[MSTA]为0，保证能触发STOP
- return(ret < 0)? ret : num;
- }
- staticint mpc_write(struct mpc_i2c *i2c,int target,
- const u8 * data,int length,int restart)
- {
- int i;
- unsigned timeout = i2c->adap.timeout;
-     u32 flags = restart ? CCR_RSTA : 0;
- 
- /* Start with MEN *///以防万一，保证I2C模块使能起来
- if(!restart)
-         writeccr(i2c, CCR_MEN);
- /* Start as master *///写了I2CCR[CCR_MSTA]，触发CPU发起START信号
-     writeccr(i2c, CCR_MIEN | CCR_MEN | CCR_MSTA | CCR_MTX | flags);
- /* Write target byte *///CPU发送一个字节，slave
 I2C addr和0 (写操作bit) 
-     writeb((target << 1), i2c->base + MPC_I2C_DR);
- 
- if(i2c_wait(i2c, timeout, 1)< 0)//等待slave
 发ACK
- return-1;
- 
- for(i = 0; i < length; i++){
- /* Write data byte */
-         writeb(data[i], i2c->base + MPC_I2C_DR);//CPU接着发数据，包括reg
 addr和data
- 
- if(i2c_wait(i2c, timeout, 1)< 0)//等待slave
 发ACK
- return-1;
- }
- 
- return 0;
- }
- staticint i2c_wait(struct mpc_i2c *i2c,unsigned timeout,int writing)
- {
- unsignedlong orig_jiffies = jiffies;
-     u32 x;
- int result = 0;
- 
- if(i2c->irq == 0)
- {//循环读I2CSR，直到I2CSR[MIF]置1
- while(!(readb(i2c->base + MPC_I2C_SR)& CSR_MIF)){
-             schedule();
- if(time_after(jiffies, orig_jiffies + timeout)){
-                 pr_debug("I2C:
 timeout\n");
-                 writeccr(i2c, 0);
-                 result =-EIO;
- break;
- }
- }
-         x = readb(i2c->base + MPC_I2C_SR);
-         writeb(0, i2c->base + MPC_I2C_SR);
- }else{
- /* Interrupt mode */
-         result = wait_event_interruptible_timeout(i2c->queue,
- (i2c->interrupt & CSR_MIF), timeout * HZ);
- 
- if(unlikely(result < 0)){
-             pr_debug("I2C:
 wait interrupted\n");
-             writeccr(i2c, 0);
- }elseif(unlikely(!(i2c->interrupt & CSR_MIF))){
-             pr_debug("I2C:
 wait timeout\n");
-             writeccr(i2c, 0);
-             result =-ETIMEDOUT;
- }
- 
-         x = i2c->interrupt;
-         i2c->interrupt = 0;
- }
- 
- if(result < 0)
- return result;
- 
- if(!(x & CSR_MCF)){
-         pr_debug("I2C:
 unfinished\n");
- return-EIO;
- }
- 
- if(x & CSR_MAL){//仲裁失败
-         pr_debug("I2C:
 MAL\n");
- return-EIO;
- }
- 
- if(writing &&(x & CSR_RXAK)){//写后没收到ACK
-         pr_debug("I2C:
 No RXAK\n");
- /* generate stop */
-         writeccr(i2c, CCR_MEN);
- return-EIO;
- }
- return 0;
- }
- staticint mpc_read(struct mpc_i2c *i2c,int target,
-             u8 * data,int length,int restart)
- {
- unsigned timeout = i2c->adap.timeout;
- int i;
-     u32 flags = restart ? CCR_RSTA : 0;
- 
- /* Start with MEN *///以防万一，保证I2C模块使能
- if(!restart)
-         writeccr(i2c, CCR_MEN);
- /* Switch to read - restart */
- //注意这里，再次把CCR_MSTA置1，再触发 START 
-     writeccr(i2c, CCR_MIEN | CCR_MEN | CCR_MSTA | CCR_MTX | flags);
- 
- 
- /* Write target address byte - this time with the read flag set */
- //CPU发送slave I2C addr和读操作1
-     writeb((target << 1)| 1, i2c->base + MPC_I2C_DR);
     //等待Slave发ACK- if(i2c_wait(i2c, timeout, 1)< 0)
- return-1;
- 
- if(length){
- if(length == 1)
-             writeccr(i2c, CCR_MIEN | CCR_MEN | CCR_MSTA | CCR_TXAK);
- else//为什么不置
 TXAK
-             writeccr(i2c, CCR_MIEN | CCR_MEN | CCR_MSTA);
- /* Dummy read */
-         readb(i2c->base + MPC_I2C_DR);
- }
- 
- for(i = 0; i < length; i++){
- if(i2c_wait(i2c, timeout, 0)< 0)
- return-1;
- 
- /* Generate txack on next to last byte */
- //注意这里TXAK置1，表示CPU每收到1byte数据后，会发送ACK
- if(i == length - 2)
-             writeccr(i2c, CCR_MIEN | CCR_MEN | CCR_MSTA | CCR_TXAK);
- 
- /* Generate stop on last byte */
- //注意这里CCR_MSTA [1->0] CPU会触发STOP
- if(i == length - 1)
-             writeccr(i2c, CCR_MIEN | CCR_MEN | CCR_TXAK);
- 
-         data[i]= readb(i2c->base + MPC_I2C_DR);
- }
- 
- return length;
- }
文章来源：[http://dpinglee.blog.163.com/blog/static/14409775320112239374615/](http://dpinglee.blog.163.com/blog/static/14409775320112239374615/)
