# Hibernate 配置文件precision与scale的说法 - z69183787的专栏 - CSDN博客
2014年02月19日 16:03:35[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1915
Oracle使用标准、可变长度的内部格式来存储数字。这个内部格式精度可以高达38位。 
    NUMBER数据类型可以有两个限定符，如： 
    column NUMBER （ precision， scale） 
    precision表示数字中的有效位。如果没有指定precision的话，Oracle将使用38作为精度。 
    scale表示数字小数点右边的位数，scale默认设置为0.　　如果把scale设成负数，Oracle将把该数字取舍到小数点左边的指定位数。
