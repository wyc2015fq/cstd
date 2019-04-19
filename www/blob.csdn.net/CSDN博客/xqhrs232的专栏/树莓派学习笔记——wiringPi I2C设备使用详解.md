# 树莓派学习笔记——wiringPi I2C设备使用详解 - xqhrs232的专栏 - CSDN博客
2017年03月03日 12:00:47[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1042
原文地址::[http://blog.csdn.net/xukai871105/article/details/18033945](http://blog.csdn.net/xukai871105/article/details/18033945)
相关文章
1、[树莓派学习笔记——I2C PCF8574 BCM2835 Library](http://blog.csdn.net/xukai871105/article/details/14109211)----[http://blog.csdn.net/xukai871105/article/details/14109211](http://blog.csdn.net/xukai871105/article/details/14109211)
2、[树莓派学习笔记——I2C Tools 学习笔记](http://blog.csdn.net/xukai871105/article/details/15029843)----[http://blog.csdn.net/xukai871105/article/details/15029843](http://blog.csdn.net/xukai871105/article/details/15029843)
# **1.前言**
    最近认真学习了树莓派，从浅到深认真分析了wiringPi实现代码，借助树莓派学习linux收获颇丰。深入学习linux一段时间后发现它非常有魅力，一个简单的IO口输出操作尽有那么多的“玩法”。wiringPi是一个简单易用的函数库，通过wiringPi可以扩展SPI和I2C等芯片，关于wiringPi的介绍和安装请参考我的另一篇【[博文](http://blog.csdn.net/xukai871105/article/details/17737005)】。
    本篇博文将通过一个简单的例子呈现wiringPi的使用，虽然例子简单但会深入分析wiringPi内部实现代码。
# 2.若干简述
## 2.1 MCP23107简述
**a) 硬件基本信息**
    MCP23017的采用7位I2C地址，7位I2C地址中的低3位从高到低分别为A2 A1和A0，该3位为地址选择位，若地址选择全部接GND，那么MCP23017的I2C从机地址为**0x20**。下图为MCP23017的硬件基本信息，左侧为MCP23017引脚图，右侧为I2C从机地址组成示意图，**从机地址的高4位为0100，高4位数字固定不变**。
![](https://img-blog.csdn.net/20140109094145765?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHVrYWk4NzExMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](file:///C:/Documents%20and%20Settings/xukai/Local%20Settings/Application%20Data/YNote/Data/xukai19871105@126.com/a5e9f79781004d35ad6893c52715bdad/clipboard.png)
图1 MCP23017硬件基本信息
    MCP23017具有GPA和GPB共16个扩展端口，另外INTA和INTB为IO中断管脚，可反映GPA或GPB的电平变化情况。请注意，RESET为复位管脚低电平有效，**可直接上拉到VDD或接一个1K到10K的电阻上拉至VDD。**（经过两天的调试，我发现MCP23017运行几秒便不能运行，后来发现RESET管脚悬空，之后RESET和VDD相连问题解决。）
**b) 字节模式**
    MCP23X17 系列能够工作于**字节模式或顺序模式** （**IOCON.SEQOP**）。 
**字节模式禁止地址指针自动递增**。工作于字节模式时，MCP23X17系列在数据传送期间，传送每个字节后不会递增其内部地址计数器。**顺序模式可使地址指针自动递增**。工作于顺序模式时，MCP23X17系列在数据传送期间，传送每个字节后会递增其地址计数器。访问最后一个寄存器后，地址指针自动返回到地址00h。（SEQOP位默认值为0）
**c) 寄存器映射关系**
**BANK位用于更改寄存器的映射方式**
    如果BANK = 1，与每个端口关联的寄存器将相互独立。与PORTA关联的寄存器从地址00h - 0Ah 映射，与PORTB关联的寄存器从10h - 1Ah映射；如果BANK = 0，A/B寄存器将配对组合。例如，IODIRA将映射到地址00h，而IODIRB将映射到 下一地址 (地址01h)。所有寄存器的映射是从00H到15H。（BANK默认值为0）
![](https://img-blog.csdn.net/20140109094150531?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHVrYWk4NzExMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](file:///C:/Documents%20and%20Settings/xukai/Local%20Settings/Application%20Data/YNote/Data/xukai19871105@126.com/e06bfd16125f483499b051de26914b36/clipboard.png)
图2 寄存器地址映射关系
【其他重要寄存器】
【IODIR I/O方向寄存器。
    写1为输入状态，写0为输出状态。请注意在大多数MCU中，写1为输出状态，而MCP23017这样类似的设置完全是microchip的特色。
【GPPU】GPPU上拉电阻寄存器
     写1为使能上拉，写入0为禁止上拉。使能芯片内部的上拉电阻往往有很有用。        
【GPIO】通用I/O端口寄存器
     写1可以使相应的端口输出高电平，写0可以使响应的端口输出低电平。读取该寄存器可以获得相应端口的输入值。
## **2.2 用户空间开发I2C设备**
    通常情况下I2C设备由内核控制。但是在用户空间也可以通过 /dev设备接口访问I2C设备。每一个在内核注册的I2C设备都具有一个编号，该编号从0开始计数，可以查看 /sys/class/i2c-dev/查看具体编号，也可以通过i2ctool工具中的i2cdetect查看。【[i2cdetect工具的安装和使用](http://blog.csdn.net/xukai871105/article/details/15029843)】【[I2C操作时序介绍](http://blog.csdn.net/xukai871105/article/details/15028117)】
![](file:///C:/Documents%20and%20Settings/xukai/Local%20Settings/Application%20Data/YNote/Data/xukai19871105@126.com/9693d688430c4c93b958459c9a722f59/clipboard.png)
![](https://img-blog.csdn.net/20140109094156468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHVrYWk4NzExMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
图3 查看 sys/class/i2c-dev
    在linux系统中所有的设备都可以采用文件的形式访问，树莓派的i2c设备位于/dev/i2c-x(对应树莓派版本2为/dev/i2c-1)，可通过open，close，wirte，read和ioclt控制i2c设备。例如open为打开I2C设备，close为关闭I2C设备，wirte为向I2C设备写入字节内容，read为从I2C设备获得字节内容，ioctl可控制I2C设备的运行参数，例如从机地址或I2C速度等参数。更多内容请参考【[linux内核
 I2C驱动说明](https://www.kernel.org/doc/Documentation/i2c/dev-interface)】。
    默认情况下，I2C和SPI设备被拉入了“黑名单”，换句话说在树莓派初始化时SPI和I2C设备并没有载入内核。在使用I2C设备之前需要载入SPI和I2C设备，具体参开请参考博文【树莓派I2C和SPI设备配置】
# 3.简单测试代码
**[cpp]**[view
 plain](http://blog.csdn.net/xukai871105/article/details/18033945#)[copy](http://blog.csdn.net/xukai871105/article/details/18033945#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/148758/fork)
- #include <stdio.h>
- #include <wiringPi.h>
- #include <mcp23017.h>
- 
- // 扩展MCP23017 起始PIN地址
- // MCP23017占用 100-115 
- #define EXTEND_BASE 100
- 
- int main (void)  
- {  
- // wiringPi初始化
-     wiringPiSetup( );  
- 
- // mcp23017初始化，此时MCP23017的I2C地址为0x20
-     mcp23017Setup( EXTEND_BAS E, 0x20 );  
- 
- int i;  
- for ( i = 0 ; i < 16 ; i++ )  
-     {  
-         pinMode( EXTEND_BASE + i, OUTPUT );  
-     }  
- 
- for (;;)  
-     {  
- for( i = 0 ; i < 16; i++)  
-         {  
-             digitalWrite ( EXTEND_BASE + i, HIGH);  
-             delay (500);  
-             digitalWrite ( EXTEND_BASE + i, LOW);  
-             delay (500);  
-         }  
-     }  
- return 0 ;  
- }  
# 4.代码详解
## 4.1 wiringPiSetup
    【[见另一篇博文](http://blog.csdn.net/xukai871105/article/details/17881433)】
## 4.2 mcp23017Setup
**[cpp]**[view
 plain](http://blog.csdn.net/xukai871105/article/details/18033945#)[copy](http://blog.csdn.net/xukai871105/article/details/18033945#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/148758/fork)
- int mcp23017Setup (constint pinBase, constint i2cAddress)  
- {  
- int fd ;  
- struct wiringPiNodeStruct *node ;  
- // 初始化I2C设备
- if ((fd = wiringPiI2CSetup (i2cAddress)) < 0)  
- return fd ;  
- 
- // 附加操作，设置MCP23017设备 I2C操作地址不自动递增
-     wiringPiI2CWriteReg8 (fd, MCP23x17_IOCON, IOCON_INIT) ;  
- 
- // MCP23017加入链表
-     node = wiringPiNewNode (pinBase, 16) ;  
- 
- // 赋值相应的操作函数 my开头的函数均位于mcp23017.c文件中
-     node->fd = fd ;  
-     node->pinMode = myPinMode ;  
-     node->pullUpDnControl = myPullUpDnControl ;  
-     node->digitalRead = myDigitalRead ;  
-     node->digitalWrite = myDigitalWrite ;  
-     node->data2 = wiringPiI2CReadReg8 (fd, MCP23x17_OLATA) ;  
-     node->data3 = wiringPiI2CReadReg8 (fd, MCP23x17_OLATB) ;  
- return 0 ;  
- }  
    mcp23017Setup是一个较为复杂的函数，主要实现三部分功能——**第一，初始化树莓派I2C设备**，通过打开**/dev/i2c-1**实现（相对于树莓派2版本），通过open可返回一个**文件fd**（可理解为设备的标记或编号，就是一个数字，好比你的身份证号）；**第二，加入新节点**，wiringPi通过一个链表维护所有的pin，在树莓派中pin是一个比较有趣的概念，例如一个MCP23017具有16个输入或输出PIN，这使得wiringPi增加额外的16个PIN，该16个PIN起始地址可以设定，但必须大于64，16个PIN的地址连续；**第三，增加控制函数**，例如digitalRead，digitalWrite和pullUpDnControl等。
### **4.2.1 I2C初始化 wiringPiI2CSetup**
初始化的最终目标为 ** fd = open (device, O_RDWR) **和** ioctl (fd, I2C_SLAVE, devId) **
【调用关系】 【wiringPiI2CSetup】 --> 【wiringPiI2CSetupInterface】
                                                          【wiringPiI2CSetupInterface】 --> 【 fd = open (device, O_RDWR) 】
                                                                                                      --> 【 ioctl (fd, I2C_SLAVE, devId)   】 
### **4.2.2 加入新节点 wiringPiNewNode**
wiringPi的外部设备有一个链表构成，相应的数字量读写和模拟量读写均根据该链表完成。该链表是wiringPi的"核心"，链表结构体见以下代码。在该**结构体中记录了节点编号**（设备的起始编号 pinBase），以及各种操作函数【操作函数的说明请看4.2.3】。
**[cpp]**[view
 plain](http://blog.csdn.net/xukai871105/article/details/18033945#)[copy](http://blog.csdn.net/xukai871105/article/details/18033945#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/148758/fork)
- struct wiringPiNodeStruct  
- {  
- int pinBase ;  
- int pinMax ;  
- int fd ; // Node specific
-     unsigned int data0 ; // ditto
-     unsigned int data1 ; // ditto
-     unsigned int data2 ; // ditto
-     unsigned int data3 ; // ditto
- void (*pinMode) (struct wiringPiNodeStruct *node, int pin, int mode) ;  
- void (*pullUpDnControl) (struct wiringPiNodeStruct *node, int pin, int mode) ;  
- int (*digitalRead) (struct wiringPiNodeStruct *node, int pin) ;  
- void (*digitalWrite) (struct wiringPiNodeStruct *node, int pin, int value) ;  
- void (*pwmWrite) (struct wiringPiNodeStruct *node, int pin, int value) ;  
- int (*analogRead) (struct wiringPiNodeStruct *node, int pin) ;  
- void (*analogWrite) (struct wiringPiNodeStruct *node, int pin, int value) ;  
- struct wiringPiNodeStruct *next ;  
- } ;  
wiringPiNewNode相关操作可分为3步
【1】查找MCP23017的pin是否被占用。
【2】在内存中开辟一个wiringPiNewNode。
【3】给该结构体变量设置默认值，并添加到控制链表中。
### **4.2.3 编写控制函数 **
【pinMode】IO口功能设置函数，mode参数——**INPUT为输入状态**，**OUTPUT为输出状态**
【pullUpDnControl】IO口输入功能设置，mode参数
【digitalRead】读取IO口管脚电平状态
【digitalWrite】设置IO口电平状态
【analogRead】若该IO为模拟量输入端口，读取该端口的模拟输入
【analogWrite】若该IO为模拟量输出端口，设置该端口的模拟输出
## 4.3 pinMode
**MCP23017的扩展pin必须大于64**，在MCP23017Setup函数中注册了相应的pin，此处通过wiringPiFindNode在链表中查找MCP23017的node信息，**node信息包括pin编号、设置IO方向、读取和设置IO管脚等。**请注意
  node->pinMode (node, pin, mode) 指向 MCP23017中的myPinMode函数，该注册工作在mcp23017Setup发生。
**[cpp]**[view
 plain](http://blog.csdn.net/xukai871105/article/details/18033945#)[copy](http://blog.csdn.net/xukai871105/article/details/18033945#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/148758/fork)
- // 位于wiringPi.c
- void pinMode (int pin, int mode)  
- {  
- int fSel, shift, alt ;  
- struct wiringPiNodeStruct *node = wiringPiNodes ;  
- 
- // PIN编号小于64，内部设备
- if ((pin & PI_GPIO_MASK) == 0)  
-     {  
- // 代码省略
-     }  
- else
-     {  
- // 查找是否存在该设备
- if ((node = wiringPiFindNode (pin)) != NULL)  
-         node->pinMode (node, pin, mode) ;  
- return ;  
-     }  
- }  
- // 位于mcp23017.c   
- staticvoid myPinMode (struct wiringPiNodeStruct *node, int pin, int mode)  
- {  
- int mask, old, reg ;  
-     pin -= node->pinBase ;  
- // 操作GPA 或 GPB
- if (pin < 8)  
-     reg = MCP23x17_IODIRA ;  
- else
-     {  
-         reg = MCP23x17_IODIRB ;  
-         pin &= 0x07 ;  
-     }  
- 
- // 获得IODIRx原始值
-     mask = 1 << pin ;  
-     old = wiringPiI2CReadReg8 (node->fd, reg) ;  
- // 设置新值
- if (mode == OUTPUT)  
-     old &= (~mask) ;  
- else
-     old |= mask ;  
-     wiringPiI2CWriteReg8 (node->fd, reg, old) ;  
- }  
## 4.4 digitalWrite
    digitalWrite函数和pinMode函数操作思路相同，先检测该pin位于板载区域还是扩展区域，如果是扩展区域该pin是否有效，如果有效则调用digitalRead函数。
**[cpp]**[view
 plain](http://blog.csdn.net/xukai871105/article/details/18033945#)[copy](http://blog.csdn.net/xukai871105/article/details/18033945#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/148758/fork)
- // 位于wiringPi.c
- int digitalWrite (int pin)      
- {  
- char c ;  
- struct wiringPiNodeStruct *node = wiringPiNodes ;  
- // PIN编号小于64，内部设备
- if ((pin & PI_GPIO_MASK) == 0)  
-     {  
- // 代码省略
-     }  
- else
-     {  
- // 查找是否存在该设备
- if ((node = wiringPiFindNode (pin)) == NULL)  
- return LOW ;  
- return node->digitalWrite (node, pin) ;  
-     }  
- }  
- // 位于mcp23017.c
- staticvoid myDigitalWrite (struct wiringPiNodeStruct *node, int pin, int value)  
- {  
- int bit, old ;  
- 
-     pin -= node->pinBase ;   
-     bit = 1 << (pin & 7) ;  
- 
- // 操作GPA
- if (pin < 8)  
-     {  
-         old = node->data2 ;  
- 
- if (value == LOW)  
-         old &= (~bit) ;  
- else
-         old |= bit ;  
- 
-         wiringPiI2CWriteReg8 (node->fd, MCP23x17_GPIOA, old) ;  
-         node->data2 = old ;  
-     }  
- // 操作GPB
- else
-     {  
-         old = node->data3 ;  
- 
- if (value == LOW)  
-         old &= (~bit) ;  
- else
-         old |= bit ;  
- 
-         wiringPiI2CWriteReg8 (node->fd, MCP23x17_GPIOB, old) ;  
-         node->data3 = old ;  
-     }  
- }  
## **4.6 wiringPiI2CReadReg8 和 wiringPiI2CWriteReg8**
    这两个函数涉及到linux驱动方面的内容，均通过ioctl (fd, I2C_SMBUS, &args)实现，具体请看以下代码，更多底层的内容请参考【[linux内核
 I2C驱动说明](https://www.kernel.org/doc/Documentation/i2c/dev-interface)】
**[cpp]**[view
 plain](http://blog.csdn.net/xukai871105/article/details/18033945#)[copy](http://blog.csdn.net/xukai871105/article/details/18033945#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/148758/fork)
- struct i2c_smbus_ioctl_data  
- {  
- char read_write ;  
-     uint8_t command ;  
- int size ;  
- union i2c_smbus_data *data ;  
- } ;  
- staticinlineint i2c_smbus_access (int fd, char rw, uint8_t command, int size, union i2c_smbus_data *data)  
- {  
- struct i2c_smbus_ioctl_data args ;  
-     args.read_write = rw ;  
-     args.command = command ;  
-     args.size = size ;  
-     args.data = data ;  
- return ioctl (fd, I2C_SMBUS, &args) ;  
- }  
- int wiringPiI2CReadReg8 (int fd, int reg)  
- {  
- union i2c_smbus_data data;  
- if (i2c_smbus_access (fd, I2C_SMBUS_READ, reg, I2C_SMBUS_BYTE_DATA, &data))  
- return -1 ;  
- else
- return data.byte & 0xFF ;  
- }  
- int wiringPiI2CWriteReg8 (int fd, int reg, int value)  
- {  
- union i2c_smbus_data data ;  
-     data.byte = value ;  
- return i2c_smbus_access (fd, I2C_SMBUS_WRITE, reg, I2C_SMBUS_BYTE_DATA, &data) ;  
- }  
# 5.总结与提高
    wiringPi提供了一个很好的封装，但是也是一把双刃剑。对于初学而言可以很好的利用树莓派实现应用，绕开linux驱动方面的知识，但是慢慢熟悉之后还是要补充linux驱动方面的内容个，使用open、wirte和ioctl实现I2C设备的控制。
    国外有很多博文指出，BCM2835的I2C存在问题，该问题发生在I2C Stop时序保持的时间太短导致I2C从设备无法正常体质。参考资料中的第1项包含对I2C硬件问题的讨论和解决方法，请各位参考。
# **6.参考资料和精彩博文**
1.【[修改I2C设备默认速度](http://www.raspberrypi.org/phpBB3/viewtopic.php?f=44&t=34734)】【[修改I2C设备默认速度](http://raspberrypi.znix.com/hipidocs/topic_i2cdev.htm)】
2.【[MCP23017
 C++ Example 国外](http://hertaville.com/2013/04/01/interfacing-an-i2c-gpio-expander-mcp23017-to-the-raspberry-pi-using-c/)】

