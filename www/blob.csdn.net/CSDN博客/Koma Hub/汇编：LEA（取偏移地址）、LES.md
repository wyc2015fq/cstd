# 汇编：LEA（取偏移地址）、LES - Koma Hub - CSDN博客
2019年03月21日 22:28:17[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：25
个人分类：[Assembly																[汇编](https://blog.csdn.net/Rong_Toa/article/category/8750815)](https://blog.csdn.net/Rong_Toa/article/category/8750816)
**Table of Contents**
[LEA](#LEA)
[LES](#LES)
# LEA
LEA是微机8086/8088系列的一条指令，取自英语**Load effect address——取有效地址**，也就是取偏移地址。在微机8086/8088中有20位物理地址，由16位段基址向左偏移4位再与偏移地址之和得到。　　　
取偏移地址指令　　
指令格式如下：　　
```
LEA reg16, mem　　
lea reg16, memory
```
LEA指令将存储器操作数mem的4位16进制偏移地址送到指定的寄存器。这里，源操作数必须是存储器操作数，目标操作数必须是16位通用寄存器。因该寄存器常用来作为地址指针，故在此最好选用四个间址寄存器BX,BP,SI,DI之一。　　LEA 取有效地址指令 （Load Effective Address )　　
**指令格式：LEA 目的，源　　**
**指令功能**：取源操作数地址的偏移量，并把它传送到目的操作数所在的单元。　　
LEA 指令要求原操作数必须是[存储单元](http://baike.baidu.com/view/1223079.htm)，而且目的操作数必须是一个除段寄存器之外的16位或32位寄存器。当目的操作数是16位通用寄存器时，那么只装入有效地址的低16位。使用时要注意它与MOV指令的区别，MOV指令传送的一般是源操作数中的内容而不是地址。　　
**例1 **
假设：SI=1000H , DS=5000H, (51000H)=1234H　　
执行指令 LEA BX , [SI]后，BX=1000H　　
执行指令 MOV BX , [SI]后，BX=1234H　　
有时，LEA指令也可用取偏移地址的MOV指令替代。　　
**例2 **
下面两条指令就是等价的，他们都取TABLE的偏移地址，然后送到BX中，
即　　
LEA BX,TABLE　　
MOV BX,OFFSET TABLE　　
但有些时候，必须使用LEA指令来完成某些功能，不能用MOV指令来实现，必须使用下面指令：　　
LEA BX, 6[DI]　　
解释：某[数组](http://baike.baidu.com/view/209670.htm)含20个元素，每个元素占一个字节，序号为0~19。设DI指向数组开头处，如果把序号为6的元素的偏移地址送到BX中。
# LES
[LES](https://www.baidu.com/s?wd=LES&tn=SE_PcZhidaonwhc_ngpagmjz&rsv_dl=gh_pc_zhidao)指令的功能是：把内存中指定位置的双字操作数的低位字装入指令中指定的寄存器、高位字装入ES寄存器。
`les reg16, memory32`
因为ds:[BX]中的低位字的内容是：0520H、高位字的内容是：4800H，
所以，指令
`LES DI,[BX] `
执行后，([DI](https://www.baidu.com/s?wd=DI&tn=SE_PcZhidaonwhc_ngpagmjz&rsv_dl=gh_pc_zhidao))=0520H，(ES)=4800H
这两个数不是算出来的，而是取出来的。
