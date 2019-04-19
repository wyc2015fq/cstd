# WINCE下修改config.bib文件中的FSRAMPERCENT设置值改变内存百分比 - xqhrs232的专栏 - CSDN博客
2010年01月22日 11:03:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2896

CSDN上的相关贴子
[http://topic.csdn.net/u/20090801/00/4f7f18fa-9327-41ad-9690-69c7eedb6951.html?seed=319379432&r=62915971#r_62915971](http://topic.csdn.net/u/20090801/00/4f7f18fa-9327-41ad-9690-69c7eedb6951.html?seed=319379432&r=62915971#r_62915971)
1。config.bib文件中的FSRAMPERCENT说明
FSRAMPERCENT设置属于CONFIG字段!!!
FSRAMPERCENT： 
格式：FSRAMPERCENT = number 
　　默认值为0x80808080。指定为文件系统分配的内存的百分比。number分为四个字节，由十六进制表示。 
byte0的值（单位为4KB）表示在第一个2MB中，其中每1MB包含的4KB的整数倍。 
byte1的值（单位为4KB）表示在第二个2MB中，其中每1MB包含的4KB的整数倍。 
byte2的值（单位为4KB）表示在第三个2MB中，其中每1MB包含的4KB的整数倍。 
byte3的值（单位为4KB）表示在剩下的内存中，每1MB包含的4KB的整数倍。 
　　计算一下默认值0x80808080表示的百分比：0x80*4K/1M = 0.5，因为每个字节都等于0.5，所以整个占用的百分比是50%。
2。config.bib文件中的FSRAMPERCENT修改实验数据
FSRAMPERCENT设置的值为存储内存占系统整个内存的比例，系统整个内存=存储内存+程序内存
在WINCE系统的控制面板的系统的内存选项有对应的FSRAMPERCENT设置的一个进度条反应出来
;;;2010--1--20  XQH  
;;;prog ram is %50--------程序内存占整个内存的比例
   ;;;FSRAMPERCENT=0x80808080   
;;;prog ram is 87.5%--------程序内存占整个内存的比例
   ;;;FSRAMPERCENT=0x20202020 
;;;prog ram is 50%--------程序内存占整个内存的比例------这种设置觉得没什么变化很奇怪！！！----字节顺序是反序的（0X10为第1个字节byte0）
   ;;;FSRAMPERCENT=0x80808010 
;;;prog ram is 88%--------程序内存占整个内存的比例
    FSRAMPERCENT=0x10808080 
;;;prog ram is 94%--------程序内存占整个内存的比例
   FSRAMPERCENT=0x10101010 
3。MS帮助文件里面的介绍
 //觉得不能这样简单去计算，应该跟系统的整个内存大小是有关系的！！！--------简单的做个除法是不准确的！！！
The following code example shows how to use FSRAMPERCENT to allocate available RAM to the file system.
FSRAMPERCENT=0x10203040
In this example, 64 4KB chunks are allocated to byte 0. 48 4KB chunks are allocated to byte 1,
                 32 4KB chunks are allocated to byte 2, and 16 4KB chunks are allocated to byte 3.
The total percentage of memory reserved for the file system would be ((64 + 48 + 32 + 16) * 4KB) / 4MB, or 15.6%.
