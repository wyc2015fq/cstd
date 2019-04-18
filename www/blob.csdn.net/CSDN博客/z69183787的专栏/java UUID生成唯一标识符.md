# java UUID生成唯一标识符 - z69183787的专栏 - CSDN博客
2014年04月08日 12:34:14[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3155
java.util.UUID.randomUUID().toString();
目前只知道可以生成一个数字和字符混合的字符串，害得我只能将原来的switch全都改成if-else。
GUID是一个128位长的数字，一般用16进制表示。算法的核心思想是结合机器的网卡、当地时间、一个随即数来生成GUID。从理论上讲，如果一台机器每秒产生10000000个GUID，则可以保证（概率意义上）3240年不重复。
UUID是1.5中新增的一个类，在java.util下，用它可以产生一个号称全球唯一的ID。
