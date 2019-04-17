# SDRAM的逻辑Bank与芯片容量表示方法 - xiahouzuoxin - CSDN博客





2013年12月06日 22:26:49[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：6036








## 1、逻辑Bank与芯片位宽

      SDRAM的内部是一个存储阵列。因为如果是管道式存储（就如排队买票），就很难做到随机访问了。
      阵列就如同表格一样，将数据“填”进去，你可以把它想象成一张表格。和表格的检索原理一样，先指定一个行（Row），再指定一个列（Column），我们就可以准确地找到所需要的单元格，这就是内存芯片寻址的基本原理。对于内存，这个单元格可称为存储单元,那么这个表格（存储阵列）叫什么呢？它就是逻辑Bank（Logical Bank，下文简称L-Bank）。

![](https://img-blog.csdn.net/20131206222808359?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhaG91enVveGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



L-Bank存储阵列示意图

      由于技术、成本等原因，不可能只做一个全容量的L-Bank，而且最重要的是，由于SDRAM的工作原理限制，单一的L-Bank将会造成非常严重的寻址冲突，大幅降低内存效率。所以人们在SDRAM内部分割成多个L-Bank，较早以前是两个，目前基本都是4个，这也是SDRAM规范中的最高L-Bank数量。到了RDRAM则最多达到了32个，在最新DDR-Ⅱ的标准中，L-Bank的数量也提高到了8个。
      这样，在进行寻址时就要先确定是哪个L-Bank，然后再在这个选定的L-Bank中选择相应的行与列进行寻址。可见对内存的访问，一次只能是一个L-Bank工作，而每次与北桥交换的数据就是L-Bank存储阵列中一个“存储单元”的容量。在某些厂商的表述中，将L-Bank中的存储单元称为Word（此处代表位的集合而不是字节的集合）。
      SDRAM的一个存储单元并不是存储一个bit，一般是8bit的整数倍（8bits,16bits,32bits），这个存储单元的容量就是芯片的位宽（也是L-Bank的位宽），SDRAM内存芯片一次传输率的数据量就是芯片位宽。




![](https://img-blog.csdn.net/20131206222826437?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhaG91enVveGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)






## 2、内存芯片的容量

  现在我们应该清楚内存芯片的基本组织结构了。那么内存的容量怎么计算呢？显然，内存芯片的容量就是所有L-Bank中的存储单元的容量总合。计算有多少个存储单元和计算表格中的单元数量的方法一样：




**存储单元数量=2^行数 × 2^列数（得到一个L-Bank的存储单元数量）× L-Bank的数量   “单位：bits”**






在很多内存产品介绍文档中，都会用M×W的方式来表示芯片的容量（或者说是芯片的规格/组织结构）。M是该芯片中存储单元的总数，单位是兆（英文简写M，精确值是1048576，而不是1000000），W代表每个存储单元的容量，也就是SDRAM芯片的位宽（Width），单位是bit。计算出来的芯片容量也是以bit为单位，但用户可以采用除以8的方法换算为字节（Byte）。比如8M×8，这是一个8bit位宽芯片，有8M个存储单元，总容量是64Mbit（8MB）。




以MT48LC16M16A2为例，该芯片结构为4 x 16M x 16bits，表示4个bank，4个bank共16M个单元（注意不是每个bank有16M单元），位宽16bits，大小计算

Capacity = 16M * 16bits = 256 Mbits = 32 MBytes





参考文章地址：[http://www.cnblogs.com/yuphone/archive/2010/05/09/1730889.html](http://www.cnblogs.com/yuphone/archive/2010/05/09/1730889.html)



