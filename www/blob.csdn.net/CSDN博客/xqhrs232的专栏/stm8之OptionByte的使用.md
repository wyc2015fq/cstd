# stm8之OptionByte的使用 - xqhrs232的专栏 - CSDN博客
2017年06月29日 10:32:55[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：717
原文地址::[http://blog.csdn.net/q759040024/article/details/23174567](http://blog.csdn.net/q759040024/article/details/23174567)
相关文章
1、STM8S的配置字节OptionByte----[https://wenku.baidu.com/view/205181b2d0d233d4b14e69fa.html](https://wenku.baidu.com/view/205181b2d0d233d4b14e69fa.html)
2、**STM8S option byte操作方式----[http://www.openedv.com/thread-35618-1-1.html](http://www.openedv.com/thread-35618-1-1.html)**
1.选项字节的概念（参考stm8s中文数据手册）：
选项字节包括芯片硬件特性的配置和存储器的保护信息，这些字节保存在存储器中一个专用的
块内。除了ROP(读出保护)字节，每个选项字节必须被保存两次，一个是通常的格式(OPTx)和
一个用来备份的互补格式(NOPTx)。可以在ICP模式(通过SWIM)下访问下表中EEPROM的地址来修改选项字节。
选项字节也可以通过应用程序在IAP模式下修改，但是ROP选项只能在ICP模式(通过SWIM)下被
修改。
2.ISP IAP ICP的概念（有待学习）：
  ISP：In System Programing，      在系统编程   程序完全下载
  IAP：In applicating Programing，在应用编程   我的理解是用应用程序去修改芯片里边固定的一些数据（与应用编程无关的数据)
  ICP：In Circuit Programing，        在电路编程  
3.stm8s芯片的备用功能重映射：
       stm8s芯片上带有很多备用的功能，这些功能必须使用重映射功能才能实现，
       重映射其实就是配置某个flash地址的某一位，使某个引脚具备备用功能或者默认功能的其中一个
       而且一旦功能改，想恢复原来的功能必须重新写入。
       比如stm8s的PD4端口具有TIM2_CH1 功能和BEEP功能，默认的情况下是TIM2_CH1功能，
       当我们要用到BEEP功能时必须写选项字，将这个引脚重映射为BEEP功能。
寄存器：
![](https://img-blog.csdn.net/20140408121013906?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcTc1OTA0MDAyNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
stm8s的多个备选功能：
![](https://img-blog.csdn.net/20140408121001828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcTc1OTA0MDAyNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
4.库函数实现重映射
//对于stm8s的选项字地址为0x4803
#define Beep_OptionAdd  0x4803
   void Set_Beep_OptionByte(void)
   {
    uint16_t Beep_Option_status;/*记录激活备选功能Beep/TIM2_CH1的状态*/
    Beep_Option_status=FLASH_ReadOptionByte(Beep_OptionAdd);
    /*Beep_Option_status的最高位为1激活了Beep,否则不激活,为TIM2_CH1*/
     if((Beep_Option_status&0x8000))
     {
      FLASH_ProgramOptionByte(Beep_OptionAdd, (uint8_t)(Beep_Option_status&0x7fff));
    /*向Beep_OptionAdd  0x4803 置0，不激活激活了Beep，恢复TIM2_CH1通道*/
     }
  }

