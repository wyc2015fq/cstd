# 用Eclipse编写并烧写Arduino程序中出现的错误 - V__KING__的专栏 - CSDN博客





2013年11月14日 10:30:27[v__king__](https://me.csdn.net/V__KING__)阅读数：2296








                
问题1：可能会出现这样的两种错误(弹出对话框提示)：

1）No Programmer has been set for the activebuild configuration.  

 Pleaseselect a Programmer in the project properties(properties->avrdude->Programer)    

2)   No Programmerselected

Checkthe avrdude properties for the project

Reson:

原因：

你没有选中你的程序配置Program configration

解决：

鼠标点在你的工程（Blink）上->右键->properties
->AVR->AVRDude，仔细看有没有Program configration，然后点击你的配置。

问题2：你编译的时候没有问题，下载upload的时候也不出现错误。但是有以下这几种提示：

1）avrdude: stk500_getsync(): not in sync: resp=0xe0

2)avrdude:
 stk500_getsync(): not in sync: resp=0x00

3)avrdude:
 stk500_getsync(): not in sync: resp=0x45

4)等等类似其他的提示

原因：

1）Program configration 中的Program hardware没有设置好

2）波特率不匹配，Program configration 中的波特率没有设置好

3）波特率不同，提示中的avrdude: stk500_getsync(): not in sync: resp=“十六进制数”，十六进制也是不一样的

解决：

没有什么好的方法，我是一个一个试的，因为按照其他网站上的的有些行不通，本人用的Uno的板子，其他的板子也可以一个一个试

在Program hardware中选择Arduino（官网说选择Atmel
 STK500 Version 1.x Firmware，其他有些国外网站也是如此）

波特率选择115200





