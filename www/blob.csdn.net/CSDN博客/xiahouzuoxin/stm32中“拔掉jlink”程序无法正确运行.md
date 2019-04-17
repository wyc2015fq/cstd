# stm32中“拔掉jlink”程序无法正确运行 - xiahouzuoxin - CSDN博客





2013年08月16日 21:35:15[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：6248








Q：在调试stm32程序时，从电脑USB口拔掉jlink后开发板上程序为什么无法正确运行？断点复位后也不行。

A：首先，要拔了jlink与开发板的连接（而不仅仅是其与PC的连接），因为jlink本省内部也有电路，对开发板上的程序运行有所影响；

其次，检查stm32的boot[1:0]的配置，将BOOT0配置为0才能从复位后Flash启动。BOOT的具体配置如下：

BOOT1       BOOT0           启动模式                               说明                        

            X                 0                     Flash                  复位后选择主闪存启动

            0                 1                    系统存储器         复位后选择系统存储器启动

            1                 1                    内置SRAM          复位后从内置SRAM区启动



